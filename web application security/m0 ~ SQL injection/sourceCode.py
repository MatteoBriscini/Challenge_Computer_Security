import mysql.connector
from flask import Flask, render_template, request, redirect, flash
from app import app

AUTHENTICATED = False

@app.before_request
def get_db_connection():
    global db_cur
    global db_conn

    db_conn = mysql.connector.connect(
        host="db",
        user="admin",
        password="supersafe",
        database="db"
    )
    db_cur = db_conn.cursor()

@app.route("/", methods=['GET'])
def index():
    return render_template('index.html')

@app.route("/login", methods=['POST'])
def login():
    global AUTHENTICATED

    username = request.form['id']
    pw = request.form['pw']

    try:
        query = f"SELECT * FROM users WHERE username='{username}' AND password='{pw}';"
        db_cur.execute(query)
        query_result = db_cur.fetchone()

        print(query_result)

        if query_result is not None:
            id = query_result[0]
            user = query_result[1]
        else:
            return redirect("/")
        
    except Exception as e:
        user = "DB ERROR"
        print(e)

    AUTHENTICATED = True
    return redirect(f"/personal_page?id={id}&username={user}")

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form['id']
        pw = request.form['pw']

        try:
            query = f"INSERT INTO `users` (`username`, `password`) VALUES (%s, %s);"
            db_cur.execute(query, (username, pw))
            
            print(f"Insert query result: {db_cur.lastrowid}")
            db_conn.commit()
        except Exception as e:
            print(e)
        
        return redirect("/")

@app.route("/personal_page", methods=["GET"])
def personal_page():
    if not AUTHENTICATED:
        return "Not authenticated. Try again!"
    
    else:
        username = request.args.get("username")
        return render_template("personal_page.html", username=username)
    

@app.route("/logout", methods=["GET"])
def logout():
    global AUTHENTICATED
    AUTHENTICATED = False
    redirect("/")
