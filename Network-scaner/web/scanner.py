from scapy.all import ARP, Ether, srp
from manuf import manuf
import socket

def scan_network(ip_range="192.168.1.0/24"):
    devices = []
    parser = manuf.MacParser()

    arp = ARP(pdst=ip_range)
    ether = Ether(dst="ff:ff:ff:ff:ff:ff")
    packet = ether / arp

    result = srp(packet, timeout=2, verbose=0)[0]

    for sent, received in result:
        mac = received.hwsrc
        ip = received.psrc

        # Buscar nombre de host
        try:
            hostname = socket.gethostbyaddr(ip)[0]
        except socket.herror:
            hostname = "No disponible"

        # Buscar fabricante
        vendor = parser.get_manuf(mac) or "No disponible"

        devices.append({
            "ip": ip,
            "mac": mac,
            "vendor": vendor,
            "hostname": hostname
        })
    
    print(devices)


    return devices
