# Mission 0 ~ SQL injection
## Goal and instructions
[We have implemented a new version of our web app. It is badly designed, and implemented even worse, but you can go on and test it! P.S. One of our not-so-experienced developers has wiped the users table and we have lost all data, but we should have a copy somewhere in the database :). Can you retrieve the password of user `Jacquelyn`?](https://web1.chall.necst.it/)
## Web app structure
The website is structured with a simple login page and a second page, after login, where the login username is shown.
## Code overview
We will focus on the vulnerable part of the source code, the login function performs an SQL query to validate username and password and return the first 2 results. <br>
the query is prepared through string concatenation, without filtering the user input, and so is vulnerable to SQL injection.
```
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
```
## Solution
As described we can see the query result, therefore we want to modify the query, through SQL injection, in a way to put desired values in the first 2 results of the query, to be able to read them. <br>
First, we want to discover the table where the user is saved, we insert the following payload in the password field in order to substitute the username with a table name.
> **note:** We can *"navigate"* by modifying the offset value through different tables.
```
prova2' UNION SELECT 1, TABLE_NAME, 'tmp' FROM INFORMATION_SCHEMA.TABLEs where TABLE_SCHEMA='db'
LIMIT 1 OFFSET 1; --
```
We found that the username is saved in the *old_users* table.
Now we want to retrieve the user password, writing the following payload in the password field to substitute the username with it.
```
prova2' UNION SELECT 0, password, 'tmp' FROM old_users WHERE username = 'Jacquelyn' LIMIT 1 OFFSET 1; --
```
> **note:** in order to set properly the UNION operator the original query and the payload have to match in the structure.
