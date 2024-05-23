from flask import Flask, render_template, redirect, request
import uuid
import os
import re

# Create the Flask app
app = Flask(__name__, template_folder=os.path.dirname(__file__))

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

# Define a route for the index page
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/index.html')
def index2():
    return render_template('index.html')

@app.route('/contact.html')
def contact():
    return render_template('contact.html')

@app.route('/blog.html')
def blog():
    return render_template('blog.html')

@app.route('/services.html')
def services():
    return render_template('services.html')

@app.route('/comments.html')
def comments():
    return render_template('comments.html')

@app.route('/comment', methods=['POST'])
def leave_a_comment():
    # Get the comment from the request
    username = request.form['username']
    comment = request.form['comment']
    
    username = sanitize(username)
    comment = sanitize(comment)

    # Generate a UUID for the comment
    comment_id = str(uuid.uuid4())

    comment_path = f'comments/{comment_id}.txt'

    # Save the comment to a file
    with open(comment_path, 'w') as f:
        f.write(f'{username}\n')
        f.write(f'{comment}\n')

    # Redirect the user back to the comments page
    return redirect(f'/comment/{comment_id}')

@app.route('/comment/<comment_id>', methods=['GET'])
def get_comment(comment_id):
    # No path traversal allowed
    if '/' in comment_id:
        return 'Invalid comment ID', 400

    comment_path = f'comments/{comment_id}.txt'

    # Read the comment from the file
    with open(comment_path, 'r') as f:
        username = f.readline().strip()
        comment = f.read()

    return render_template('your_comment.html', username=username, comment=comment)

# Run the Flask app
if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
