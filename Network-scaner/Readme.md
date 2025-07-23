text
# Network Scanner Web – Proyecto de Sistemas Linux Embebidos

## 📌 Formulación del Problema

En sistemas embebidos con acceso a red, es fundamental conocer qué dispositivos están conectados en la red local. Esto puede ser útil para tareas de administración, seguridad o simplemente monitoreo.

El problema planteado es: **¿cómo detectar y visualizar los dispositivos activos en la red desde un sistema embebido con Linux, sin depender de herramientas externas complejas?**

## 🎯 Objetivo del Proyecto

Diseñar una aplicación ligera que se ejecute en un sistema embebido Linux (Lychee RV Dock), que permita escanear la red local para detectar dispositivos conectados, mostrando su IP, dirección MAC, fabricante y nombre del host, accesible desde una interfaz web simple.

---

## 🧱 Arquitectura de la Solución

### 🔧 Hardware
- **Lychee RV Dock** (SoC RISC-V con Linux)
- Conectividad a red local (Ethernet o WiFi)

### 💻 Software
- Python 3
- Flask (servidor web)
- Scapy (escaneo ARP)
- Manuf (identificación de fabricantes por MAC)
- socket (obtención de nombre del host)

### 🔄 Diagrama de arquitectura
[ Navegador Web ]
↓
[ Flask Web Server – Lychee RV Dock ]
↓
[ Módulo Scapy – Escaneo ARP ]
↓
[ Resultados: IP, MAC, Fabricante, Hostname ]

text

---

## 🧪 Pruebas

### Pruebas Unitarias

✔ **Prueba de escaneo básico**  
Se ejecutó el escaneo directamente desde consola:
```bash
python3 -c "from web.scanner import scan_network; print(scan_network())"
✔ Resultado: Lista de dispositivos con IP y MAC.

✔ Prueba de resolución de nombre de host
Probado usando:

python
socket.gethostbyaddr("192.168.1.X")
✔ Resultado: devuelve el hostname cuando está disponible.

✔ Prueba de fabricante con manuf
Probado con MACs conocidas, devuelve el nombre del fabricante (TP-Link, Apple, etc).

🔗 Pruebas de Integración
✔ Se accedió exitosamente a la interfaz web desde otro dispositivo en la red usando la dirección IP del Lychee (ej: http://192.168.1.123:5000).

✔ Se escaneó la red y se mostraron correctamente IP, MAC, fabricante y hostname (cuando disponibles).

✔ El sistema puede funcionar de forma continua en segundo plano.

Capturas incluidas en el directorio /pruebas.

🧾 Documentación Técnica
📂 Estructura del proyecto
text
Network-Scanner/
├── web/
│   ├── app.py         # Servidor Flask
│   └── scanner.py     # Función scan_network
├── templates/
│   └── index.html     # Interfaz web
├── run.py             # Script principal
├── requirements.txt   # Dependencias
├── README.md          # Este archivo
└── pruebas/           # Evidencias de funcionamiento
📌 Instalación
bash
# Crear entorno virtual
python3 -m venv venv
source venv/bin/activate

# Instalar dependencias
pip install -r requirements.txt
🚀 Ejecución
bash
source venv/bin/activate
python3 run.py
Luego acceder desde el navegador:

text
http://<IP_DEL_DISPOSITIVO>:5000
📖 Créditos
Proyecto desarrollado por: Diego [Tu Apellido]
Asignatura: Programación de Sistemas Linux Embebidos
Institución: Universidad Nacional De Colombia
Año: 2025
