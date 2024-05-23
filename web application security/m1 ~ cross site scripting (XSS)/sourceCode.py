from flask import Flask, render_template, request, redirect
import os

# Create the Flask app
app = Flask(__name__, template_folder=os.path.dirname(__file__))

def sanitize(string):
    blacklist = ['script', 'img', 'sgv']

    # Sanitize the name and comment
    # Now they won't be able to break my beautiful website
    while any('<' + word in string for word in blacklist):
        for word in blacklist:
            string = string.replace(f'<{word}', f'&lt;{word}')

    return string

# Define a route for the index page
@app.route('/', methods=['GET'])
def index():
    return render_template('app.html')

@app.route('/chat', methods=['GET'])
def chat():
    query = request.args.get('query')

    if not query:
        # Redirect to the index page
        return redirect('/')

    # Sanitize the query
    query = sanitize(query)

    return render_template('response.html', query=query)

# Allow access to favicon.ico
@app.route('/favicon.ico')
def favicon():
    return app.send_static_file('favicon.ico')

# Run the Flask app
if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
