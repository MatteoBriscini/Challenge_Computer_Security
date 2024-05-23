from flask import Flask, jsonify, render_template, redirect, flash, request, make_response
from flask_cors import CORS
from flask_jwt_extended import create_access_token, JWTManager, jwt_required, get_jwt_identity, set_access_cookies
import psycopg2
from psycopg2.extras import RealDictCursor
from datetime import datetime
import os
import bcrypt
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__, static_folder=os.path.join(os.getcwd(), 'static'))
CORS(app)

jwt = JWTManager(app)

app.config['JWT_SECRET_KEY'] = "sup3r-s3cr3t-c0d3!"
app.secret_key = "sup3r-s3cret-k3y!"

app.config['MAX_CONTENT_LENGTH'] = 10 * 1024 * 1024
app.config['JWT_TOKEN_LOCATION'] = ['cookies']
app.config['JWT_COOKIE_CSRF_PROTECT'] = False
app.config['JWT_ACCESS_TOKEN_EXPIRES'] = False

db_user = os.environ['POSTGRES_USER']
db_pwd = os.environ['POSTGRES_PASSWORD']

app.config['SQLALCHEMY_DATABASE_URI'] = f'postgresql://{db_user}:{db_pwd}@db/daily-training'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)

class User(db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(300), unique=True, nullable=False)
    password = db.Column(db.String(100), nullable=False)

class Task(db.Model):
    __tablename__ = 'tasks'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.Text, nullable=False)
    points = db.Column(db.Integer, nullable=False)
    description = db.Column(db.Text)

class CompletedTask(db.Model):
    __tablename__ = 'completed_tasks'
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    task_id = db.Column(db.Integer, db.ForeignKey('tasks.id'), nullable=False)
    user = db.relationship('User', backref=db.backref('completed_tasks', lazy=True))
    task = db.relationship('Task', backref=db.backref('completed_tasks', lazy=True))
    __table_args__ = (db.UniqueConstraint('user_id', 'task_id', name='_user_task_uc'),)

class Secret(db.Model):
    __tablename__ = 'secrets'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(50), db.ForeignKey('users.username'), nullable=False)
    flag = db.Column(db.String(50), nullable=False)

@jwt.unauthorized_loader
def missing_token_callback(jwt_header):
    flash("Dear trailblazer, you need to login first!", category='error')
    flash("- Pom Pom, the train conductor", category="error")
    return redirect('/login')

def hash_password(password):
    salt = bcrypt.gensalt()
    hash = bcrypt.hashpw(password.encode('utf-8'), salt)
    return hash

def get_user_by_username(username):    
    return User.query.filter_by(username=username).first()

@app.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        user = get_user_by_username(username)
        
        if user and bcrypt.checkpw(password.encode('utf-8'), user.password.encode('utf-8')):
            access_token = create_access_token(identity=username)
            response = make_response(redirect('/tasks'))
            set_access_cookies(response, access_token)
            return response
        else:
            flash('Invalid credentials', category='error')
            return render_template('login.html')
    elif request.method == 'GET':
        return render_template('login.html')

@app.route('/signup', methods=['POST', 'GET'])
def signup():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        if len(username) > 50 or len(username) < 4:
            flash('Wrong username length ( 4<= len <= 50 characters )', category='error')
            return redirect('/signup')
        
        if len(password) < 16:
            flash('Password too short', category='error')
            return redirect('/signup')
        
        hash = hash_password(password)
        new_user = User(username=username, password=hash.decode('utf-8'))
        try:
            db.session.add(new_user)
            db.session.commit()
            access_token = create_access_token(identity=username)
            response = make_response(redirect('/tasks'))
            set_access_cookies(response, access_token)
            return response
        except Exception as e:
            db.session.rollback()
            flash('Username already taken', category='error')
            return redirect('/signup')

    return render_template('signup.html')

def fetch_completed():
    username = get_jwt_identity()
    user = get_user_by_username(username)
    if user:
        return db.session.query(Task).join(CompletedTask).filter(CompletedTask.user_id == user.id).all()
    return []

def fetch_uncompleted():
    username = get_jwt_identity()
    user = get_user_by_username(username)
    if user:
        subquery = db.session.query(CompletedTask.task_id).filter(CompletedTask.user_id == user.id)
        return db.session.query(Task).filter(~Task.id.in_(subquery)).all()
    return []

def fetch_all_tasks():
    return Task.query.all()

@app.route('/tasks')
@jwt_required()
def tasks():
    uncompleted = fetch_uncompleted()
    completed = fetch_completed()

    for x in uncompleted:
        x.status = 'uncompleted'
    for x in completed:
        x.status = 'completed'
    
    total = uncompleted + completed

    tasks = fetch_all_tasks()
    total_points = sum([task.points for task in tasks])
    completed_points = sum([task.points for task in completed])

    progress = {'value': completed_points, 'valuemin': 0, 'valuemax': total_points}

    if (total_points == completed_points):
        username = get_jwt_identity()
        secret = Secret.query.filter_by(username=username).first()

        if secret:
            message = f"Congratz! Here is your secret! {secret.flag}"
        else:
            message = "Congratz! You have completed all the tasks! But there is no reward for you! :("

        return render_template('tasks.html', cards=total, progress=progress, flag=message)  
    else:
        return render_template('tasks.html', cards=total, progress=progress)  


@app.route('/')
def index():
    return render_template('index.html')

@app.route('/complete_task/<int:task_id>', methods=['POST'])
@jwt_required()
def complete_task(task_id):
    username = get_jwt_identity()
    user = get_user_by_username(username)
    if user:
        new_completed_task = CompletedTask(user_id=user.id, task_id=task_id)
        try:
            db.session.add(new_completed_task)
            db.session.commit()
        except Exception as e:
            db.session.rollback()
            return jsonify({'status': 'error', 'msg': 'Already completed'})
    
    return redirect('/tasks')

if __name__ == '__main__':
    app.run(debug=True)
