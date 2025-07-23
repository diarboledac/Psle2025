from flask import Flask, render_template_string
from .scanner import scan_network

app = Flask(__name__)

@app.route('/')
def index():
    return 'Servidor Flask funcionando correctamente desde la Lychee.'

@app.route('/scan')
def scan():
    devices = scan_network()
    html = """
    <h2>Dispositivos conectados a la red</h2>
    <table border="1">
        <tr><th>IP</th><th>MAC</th></tr>
        {% for d in devices %}
        <tr><td>{{ d.ip }}</td><td>{{ d.mac }}</td></tr>
        {% endfor %}
    </table>
    """
    return render_template_string(html, devices=devices)
