from flask import Flask, render_template
from .scanner import scan_network, scan_ports

app = Flask(__name__)

@app.route('/')
def index():
    previous = load_previous_devices()
    current = scan_network()
    
    for device in current:
        ports = scan_ports(device['ip'])
        device['ports'] = ports

    current = compare_devices(current, previous)
    save_current_devices(current)

    return render_template('index.html', devices=current)
