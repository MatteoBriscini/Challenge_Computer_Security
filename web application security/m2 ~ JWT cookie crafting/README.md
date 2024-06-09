# Mission 2 ~ JWT cookie crafting


## Goal and instructions

[The director of the Astral Express invites you to clain your daily rewards](https://web3.chall.necst.it/). Are you able to retrieve the rewards of user `3095f2ec-0f9f-408e-8c1d-00eb6fcb120f`.


## Web app structure

The web application features an initial page that allows users to log in or sign up. It also includes a page devoted to exercises, through which users can earn points by completing them.


## Code overview

Analizzando il codice sorgente, si può notare che, una volta completate tutte le attività, gli utenti possono ottenere una ricompensa che è il nostro obiettivo.
```php
if secret:
    message = f"Congratz! Here is your secret! {secret.flag}"
else:
    message = "Congratz! You have completed all the tasks! But there is no reward for you! :("
```
Continuing the analysis and looking at the login function, we see that if the user has logged in correctly a [JWT](https://jwt.io/) access token is created with his username.
```php
if user and bcrypt.checkpw(password.encode('utf-8'), user.password.encode('utf-8')):
    access_token = create_access_token(identity=username)
    response = make_response(redirect('/tasks'))
    set_access_cookies(response, access_token)
    return response
else:
    flash('Invalid credentials', category='error')
    return render_template('login.html')
```
Finally, looking at the beginning of the code, it can be seen that the configuration keys for the [JWT](https://jwt.io/) access token are present.
```php
jwt = [JWT](https://jwt.io/)Manager(app)

app.config['[JWT](https://jwt.io/)_SECRET_KEY'] = “sup3r-s3cr3t-c0d3!”
app.secret_key = “sup3r-s3cret-k3y!”

app.config['MAX_CONTENT_LENGTH'] = 10 * 1024 * 1024
app.config['[JWT](https://jwt.io/)_TOKEN_LOCATION'] = ['cookies']
app.config['[JWT](https://jwt.io/)_COOKIE_CSRF_PROTECT'] = False
app.config['[JWT](https://jwt.io/)_ACCESS_TOKEN_EXPIRES'] = False
```


## Solution

To complete the exploit, you need to follow the following steps:
1. Log into the application with a username and password
2. Open the Inspect Browser Element and go to the Storage tab, there you can see the JWT access token
3. Decode the access token using a [JWT decoder](https://jwt.io/).
    - Copy the value of the access token present in the web application
    - Paste the value into the decoder
    - Overwrite the *sub* field with the required user `3095f2ec-0f9f-408e-8c1d-00eb6fcb120f`
    - Add the *signature key* `sup3r-s3cr3t-c0d3!`
    - Copy the resulting access toker and paste it into the value of the previously found access token.

