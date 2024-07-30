# Mission 4 ~ blind SQL injection
## Goal and instructions
Before a secret gnome arrived, there were many gnomes in the forest. Now, all the other gnomes have disappeared, and the secret gnome is the only one left. I heard his name is random. I have assigned you the name of a researcher. The name is `Faith`. [Please help me find the secret gnome!](https://web5.chall.necst.it/)
## Web app structure
The website is structured with a simple login page. differently from the first challenge on SQL injection the login page will redirect to 2 different pages in case of success or not, but those pages are static and we cannot print the results of any query.
## Code overview
We will focus on the vulnerable part of the source code: the submit_form function performs an SQL query to validate username and password and redirects the user to the correct page.
The query is prepared through string concatenation, and the user input is filtered with a white and blacklist, in general, blacklisting isn't the correct way to protect a website from SQL injection moreover this specific blacklist is incomplete and vulnerable.
The blacklist and the vulnerable function are reported below:
```
blacklist = ["alter", "begin", "cast", "create", "cursor", "declare", "delete",
             "drop", "end", "exec", "execute", "fetch", "insert", "kill",
             "table", "update", "union", "join"
]
```

```
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
```
## Solution
To exploit the vulnerability we have to perform a blind SQL injection (to find the secret gnome name), we can use the LIKE statement combined with the special characters *"%"* to disclose one character at once.
The whole procedure proceeds as follows:
1. Start from the first position of the word, try in order all the alphabetical characters
2. If the web website returns the success page we have disclosure the first letter of the secret word
3. Try to log in with the secret word estimated so far
    * In case of failure: reiterate on the next position
    * In case of success: you have found the secret word!

We have provided the payload query below but the whole Python code is provided [here](https://github.com/MatteoBriscini/Challenge_Computer_Security/blob/master/web%20application%20security/m4%20~%20blind%20SQL%20injection/exploit.py)
```
query = "random' OR(gnome_name LIKE '" + gnome + "%' AND assigned_name LIKE 'Faith%')); --"
```
> **note:**  To avoid ban the Python cycle has a sleep function used to delay the web requests.
