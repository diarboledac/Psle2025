from flask import Flask, render_template
from .scanner import scan_network, scan_ports
from flask import Flask, render_template
from .scanner import scan_network, scan_ports, load_previous_devices, save_devices

app = Flask(__name__)

@app.route('/')
def index():
    previous = load_previous_devices()
    current = scan_network()

    # Marcar si es nuevo
    previous_ips = {dev['ip'] for dev in previous}
    for device in current:
        device['is_new'] = device['ip'] not in previous_ips
        device['ports'] = scan_ports(device['ip'])

    # Guardar el nuevo estado
    save_devices(current)

    return render_template('index.html', devices=current)
