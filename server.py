from flask import Flask, request, jsonify, send_file
import subprocess
import json
import os

app = Flask(__name__)

# Path to the C++ executable
EXECUTABLE = 'shameer.exe'

def run_cpp_command(args):
    """Runs the C++ executable with the given arguments."""
    try:
        # Check if executable exists
        if not os.path.exists(EXECUTABLE):
             return None, "Executable not found. Please compile shameer.cpp to shameer.exe"

        result = subprocess.run(
            [EXECUTABLE] + args, 
            capture_output=True, 
            text=True,
            check=False # Don't raise exception on error code, handle it
        )
        return result.stdout, result.stderr
    except Exception as e:
        return None, str(e)

@app.route('/')
def index():
    return send_file('library.html')

@app.route('/api/books', methods=['GET'])
def get_books():
    stdout, stderr = run_cpp_command(['get_books'])
    
    if not stdout:
        return jsonify({'error': stderr or 'Failed to execute C++ backend'}), 500
    
    try:
        # The C++ program might output initialization logs before the JSON
        # valid JSON array starts with [ and ends with ]
        start_idx = stdout.find('[')
        end_idx = stdout.rfind(']')
        
        if start_idx == -1 or end_idx == -1:
             return jsonify({'error': 'No JSON found in C++ output', 'raw_output': stdout}), 500

        json_str = stdout[start_idx : end_idx + 1]
        books = json.loads(json_str)
        return jsonify(books)
    except json.JSONDecodeError:
        return jsonify({'error': 'Invalid JSON from backend', 'raw_output': stdout}), 500

@app.route('/api/books', methods=['POST'])
def add_book():
    data = request.json
    # args: add_book <title> <author> <isbn> <year>
    # Note: Wrap in quotes to safely pass to shell if needed, but subprocess list handles args safely
    args = [
        'add_book',
        data['title'],
        data['author'],
        data['isbn'],
        str(data['year'])
    ]
    
    stdout, stderr = run_cpp_command(args)
    
    if stderr and "Error" in stderr: # Customize based on your C++ error output
         return jsonify({'success': False, 'message': stderr})
         
    return jsonify({'success': True, 'output': stdout})

@app.route('/api/borrow', methods=['POST'])
def borrow_book():
    data = request.json
    # args: borrow_book <user_name> <book_id>
    args = [
        'borrow_book',
        data.get('student_name', 'Guest'),
        str(data['book_id'])
    ]
    
    stdout, stderr = run_cpp_command(args)
    
    if "Error" in stdout or (stderr and "Error" in stderr):
         return jsonify({'success': False, 'message': stdout + (stderr or "")})
         
    return jsonify({'success': True, 'output': stdout})

@app.route('/api/return', methods=['POST'])
def return_book():
    data = request.json
    # args: return_book <transaction_id>
    args = [
        'return_book',
        str(data['transaction_id'])
    ]
    
    stdout, stderr = run_cpp_command(args)
    
    if "Error" in stdout or (stderr and "Error" in stderr):
         return jsonify({'success': False, 'message': stdout + (stderr or "")})
         
    return jsonify({'success': True, 'output': stdout})

@app.route('/api/transactions', methods=['GET'])
def get_transactions():
    stdout, stderr = run_cpp_command(['get_transactions'])
    
    if not stdout:
        return jsonify({'error': stderr or 'Failed to execute C++ backend'}), 500
    
    try:
        start_idx = stdout.find('[')
        end_idx = stdout.rfind(']')
        
        if start_idx == -1 or end_idx == -1:
             return jsonify({'error': 'No JSON found in C++ output', 'raw_output': stdout}), 500

        json_str = stdout[start_idx : end_idx + 1]
        transactions = json.loads(json_str)
        return jsonify(transactions)
    except json.JSONDecodeError:
        return jsonify({'error': 'Invalid JSON from backend', 'raw_output': stdout}), 500

if __name__ == '__main__':
    app.run(debug=True, port=5000)
