import string
import os
from functools import cache

import psycopg2
from flask import Flask, request, render_template, send_from_directory

app = Flask(__name__)

whitelist_name = set(string.ascii_letters)
whitelist_gnomes = set(string.ascii_letters + string.digits + "_" + "' ")
blacklist = [
    "like",
    "--", ";", "/*", "*/", "@@", "@", "%",
    "char", "nchar", "varchar", "nvarchar",
    "alter", "begin", "cast", "create", "cursor", "declare", "delete",
    "drop", "end", "exec", "execute", "fetch", "insert", "kill",
    "select", "sys", "sysobjects", "syscolumns", 'from', 'where',
    "table", "update", "union", "join",
    "=", "<", ">", "<=", ">=", "<>",
    "and", "not",
    "+", "-", "*", "/", "||",
    "all", "any", "some",
    "concat", "substring", "length", "ascii", "char_length", "replace", "coalesce" "sleep",
    "int", "float", "date", "bool",
    "case", "iif",
    "\\n", "\\r", "\\t"
]

@app.route('/images/<filename>')
def serve_static(filename):
    return send_from_directory(app.root_path + '/static/', filename)

@cache
def get_database_connection():
    # Get database credentials from environment variables
    db_user = os.environ.get("POSTGRES_USER")
    db_password = os.environ.get("POSTGRES_PASSWORD")
    db_host = "db"

    # Establish a connection to the PostgreSQL database
    connection = psycopg2.connect(user=db_user, password=db_password, host=db_host)

    return connection

@app.post("/submit")
def submit_form():
    try:
        assigned_name = request.form["assigned_name"]
        gnome_name = request.form["gnome_name"]
        conn = get_database_connection()

        assert all(c in whitelist_name for c in assigned_name), "Assigned name: Invalid character detected!"
        assert all(c in whitelist_gnomes for c in gnome_name), "Gnome name: Invalid character detected!"
        assert all(forbidden not in gnome_name.lower() for forbidden in blacklist), "Gnome name: Forbidden pattern detected!"

        with conn.cursor() as curr:
            curr.execute("SELECT * FROM gnomes WHERE assigned_name = '%s' AND (gnome_name = '%s')" % (assigned_name, gnome_name))
            result = curr.fetchall()

        if len(result):
            return render_template('success.html')
        return render_template('failure.html')

    except Exception as e:
        app.logger.error(f"Error handling request: {str(e)}")
        return str(e), 400

    # Ensure to commit to keep the connection in good state
    finally:
        conn.commit()

@app.get("/")
def index():
    return """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>The Gnomes' secret - Revenge</title>
    <style>
        body {
            background-image: url('/images/axarmedgnome.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            background-color: black; /* Fallback color */
            height: 100vh; /* Full viewport height */
            margin: 0;
            display: flex;
            align-items: flex-start; /* Center content vertically */
            padding-top: 50px;
            justify-content: center; /* Center content horizontally */
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }
        form {
            width: 500px; /* Fixed width */
            background-color: rgba(255, 255, 255, 0.9); /* Semi-transparent white */
            padding: 30px;
            border-radius: 8px; /* Rounded corners */
            box-shadow: 0 6px 10px rgba(0, 0, 0, 0.2); /* Soft shadow */
            text-align: center;
        }
        input[type="text"], input[type="submit"] {
            width: 100%; /* Full width */
            padding: 16px; /* Increased padding */
            margin-top: 8px; /* Margin-top for spacing */
            border: 1px solid #ddd; /* Light grey border */
            border-radius: 4px; /* Slightly rounded corners for input */
            box-sizing: border-box; /* Include padding and border in the width */
            font-size: 18px; /* Increased font size */
        }
        input[type="submit"] {
            border: none;
            background-color: #4CAF50; /* Green background */
            color: white;
            font-weight: bold;
            cursor: pointer;
        }
        input[type="submit"]:hover {
            background-color: #45a049; /* Darker green on hover */
        }
    </style>
</head>
<body>
    <form action="/submit" method="POST">
        <input type="text" name="assigned_name" placeholder="Assigned name" required>
        <input type="text" name="gnome_name" placeholder="Look for a gnome, find the flag" required>
        <input type="submit" value="Log In">
    </form>
</body>
</html>
""", 200

if __name__ == "__main__":
    app.run(debug=True)
