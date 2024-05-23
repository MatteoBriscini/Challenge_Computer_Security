# Mission 5 ~ cross site scripting
## Goal and instructions
Game of Apulia is the latest strategic, medieval game set in Puglia, Italy. Developed by Tarantella Tech, it pushes the boundaries of what it means to play videogames in 2024. Coming soon on PC, PS4, PS5, Xbox Series S and X, Nintendo Switch.

To validate your solution, please submit an URL belonging to the domain web6.chall.necst.it, which, when opened in a browser:
retrieves the value of cookie JxNHT, which is is already present in the cookie jar of the browser that will visit your URL (the developers were sloppy, and this cookie is not HTTPOnly). Pay attention to the fact that there are multiple cookies in the cookie jar: you need to submit only the value of JxNHT!
submits it through a GET request to https://chall.necst.it/challenges/web6/verify/[PERSONA_CODE]/[cookie], where [cookie] is the value of the cookie JxNHT
For example, if document.cookie is JxNHT=th1s1ssupersecret, your payload must perform a GET request to: https://chall.necst.it/challenges/web6/verify/[PERSONA_CODE]/th1s1ssupersecret.

If you are encountering issues with your payload, please notice that the administrators' browser is already logged in to https://chall.necst.it/, and that it actually needs to be logged in to submit a valid solution. Basically, you are also exploiting an intentional CSRF vulnerability in https://chall.necst.it/!
## Web app structure
## Code overview
## Solution