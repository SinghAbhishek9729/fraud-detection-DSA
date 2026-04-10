from flask import Flask, request, render_template
import subprocess

app = Flask(__name__)

# -------------------------------
# HOME PAGE
# -------------------------------
@app.route('/')
def home():
    return render_template('index.html')


# -------------------------------
# FRAUD CHECK ROUTE
# -------------------------------
@app.route('/check', methods=['POST'])
def check():

    try:
        # Get data from form
        user = request.form.get('user')
        amount = request.form.get('amount')
        location = request.form.get('location')

        # Basic validation
        if not user or not amount or not location:
            return render_template('index.html', result="❌ Missing Input")

        # Call C program
        process = subprocess.run(
            ['./fraud', user, amount, location],
            capture_output=True,
            text=True
        )

        # Check for errors
        if process.returncode != 0:
            output = "⚠️ Error in processing"
        else:
            output = process.stdout.strip()

    except Exception as e:
        output = f"⚠️ Server Error: {str(e)}"

    return render_template('index.html', result=output)


# -------------------------------
# RUN SERVER
# -------------------------------
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)