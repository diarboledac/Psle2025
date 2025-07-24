from flask import Flask, render_template
from .scanner import scan_network, scan_ports

app = Flask(__name__)

@app.route('/')
def index():
    devices = scan_network()
    for device in devices:
        ports = scan_ports(device['ip'])
        device['ports'] = ports
    return render_template('index.html', devices=devices)
