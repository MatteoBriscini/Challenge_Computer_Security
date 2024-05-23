# Mission 1 ~ cross site scripting (XSS)
## Goal and instructions
People are worried AI will take over humankind. Personally, I think that's all bu****it. Can't even get an image of a bowl of rice without chopsticks...

To validate your solution, please submit an URL belonging to the domain web2.chall.necst.it, which, when opened in a browser:
retrieves the value of cookie aNrBwXC, which is is already present in the cookie jar of the browser that will visit your URL (the developers were sloppy, and this cookie is not HTTPOnly). Pay attention to the fact that there are multiple cookies in the cookie jar: you need to submit only the value of aNrBwXC!
submits it through a GET request to https://chall.necst.it/challenges/web2/verify/[PERSONA_CODE]/[cookie], where [cookie] is the value of the cookie aNrBwXC
For example, if document.cookie is aNrBwXC=th1s1ssupersecret, your payload must perform a GET request to: https://chall.necst.it/challenges/web2/verify/[PERSONA_CODE]/th1s1ssupersecret.

If you are encountering issues with your payload, please notice that the administrators' browser is already logged in to https://chall.necst.it/, and that it actually needs to be logged in to submit a valid solution. Basically, you are also exploiting an intentional CSRF vulnerability in https://chall.necst.it/
## Web app structure
## Code overview
## Solution