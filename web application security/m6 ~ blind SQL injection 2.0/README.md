# Mission 0 ~ SQL injection
## Goal and instructions
Now that you have found the secret gnome, another one has arrived, and things are more complicated this time. A GNOMO ARMATO DI ASCIA is trying to protect the secret gnome; pay attention. This time, your name is Kelsey.
## Web app structure
The website is structured with a simple login page. Similarly to the 4th challenge, the login page will redirect to 2 different pages in case of success or not, but those pages are static and we cannot print the results of any query.
## Code overview
We will focus on the vulnerable part of the source code: the submit_form function performs an SQL query to validate username and password and redirects the user to the correct page.
The query is prepared through string concatenation, the user input is filtered with a white and blacklist, this time the blacklist is a lot more complete, however, it's still vulnerable to SQL injection.
The blacklist is reported below:
```
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
    "\\n", "\\r", "\\t"a
]
```
## Solution
To exploit the vulnerability we have to perform a blind SQL injection (to find the secret gnome name), we can't use the special characters *'%'* as in challenge 4 (it is banned in the blacklist), also the SIMILAR statement is forbidden.
The only available solution is to use SIMILAR TO combined with the special characters *'_'*, we have to proceed in 2 steps:
1. Estimate the length of the secret word, adding one *'_'* at once, until the website returns the success page.
2. Replace iteratively one *'_'* in order to estimate the the secret word

We have provided the payload query below but the whole Python code is provided [here](https://github.com/MatteoBriscini/Challenge_Computer_Security/blob/master/web%20application%20security/m6%20~%20blind%20SQL%20injection%202.0/exploit.py)
```
query1 = "prova' OR gnome_name SIMILAR TO '"
```
