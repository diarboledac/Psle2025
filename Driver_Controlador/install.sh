#!/bin/sh

MODULE="led_driver"
DEVICE="led_driver"
CLASS="led"

echo "[INFO] Cargando módulo del kernel..."
insmod src/$MODULE.ko

if [ $? -ne 0 ]; then
    echo "[ERROR] No se pudo cargar el módulo."
    exit 1
fi

# Obtener el número mayor desde dmesg
MAJOR=$(dmesg | grep "Major asignado" | tail -n1 | grep -o '[0-9]*$')

if [ -z "$MAJOR" ]; then
    echo "[ERROR] No se pudo obtener el número mayor desde dmesg."
    exit 1
fi

echo "[INFO] Creando /dev/$DEVICE con major=$MAJOR..."
if [ ! -e /dev/$DEVICE ]; then
    mknod /dev/$DEVICE c $MAJOR 0
    chmod 666 /dev/$DEVICE
fi

echo "[INFO] Ejecutando aplicación de usuario..."
./src/user_app

echo "[INFO] Limpiando..."
rmmod $MODULE
rm /dev/$DEVICE
