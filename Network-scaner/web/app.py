from flask import Flask, render_template, request
from .scanner import scan_network

app = Flask(__name__)

@app.route("/", methods=["GET", "POST"])
def scan():
    devices = []
    if request.method == "POST":
        devices = scan_network()
    return render_template("index.html", devices=devices)
