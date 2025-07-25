from scapy.all import ARP, Ether, srp
import socket
from manuf import manuf


import json
import os

DEVICES_FILE = "devices.json"

def scan_network(ip_range="192.168.1.0/24"):
    devices = []
    arp = ARP(pdst=ip_range)
    ether = Ether(dst="ff:ff:ff:ff:ff:ff")
    packet = ether / arp
    result = srp(packet, timeout=2, verbose=0)[0]

    parser = manuf.MacParser()

    for sent, received in result:
        mac = received.hwsrc
        vendor = parser.get_manuf(mac)
        devices.append({
            "ip": received.psrc,
            "mac": mac,
            "vendor": vendor or "Desconocido"
        })

    return devices

def scan_ports(ip, ports=[21, 22, 23, 25, 53, 80, 110, 139, 143, 443, 445, 8080]):
    open_ports = []
    for port in ports:
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(0.5)
            result = sock.connect_ex((ip, port))
            if result == 0:
                service = socket.getservbyport(port, 'tcp') if port < 1024 else 'Servicio común'
                open_ports.append({"port": port, "type": "TCP", "service": service})
            sock.close()
        except:
            pass
    return open_ports




def load_previous_devices():
    if os.path.exists(DEVICES_FILE):
        with open(DEVICES_FILE, "r") as f:
            return json.load(f)
    return []

def save_current_devices(devices):
    with open(DEVICES_FILE, "w") as f:
        json.dump(devices, f, indent=4)

def compare_devices(current, previous):
    previous_macs = {dev['mac'] for dev in previous}
    for device in current:
        device["is_new"] = device["mac"] not in previous_macs
    return current


