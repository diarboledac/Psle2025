# web/app.py
from flask import Flask

app = Flask(__name__)

@app.route('/')
def index():
    return 'Servidor Flask funcionando correctamente desde la Lychee.'
