# 🔌 Linux GPIO LED Driver - Proyecto Lychee RV Dock

Este proyecto implementa un **driver de caracteres para el kernel de Linux** que permite controlar un LED físico conectado a un pin GPIO desde espacio de usuario.

---

## 🧠 Objetivos

- Aprender a crear un dispositivo de caracteres en `/dev`.
- Controlar hardware (LED) desde el kernel.
- Conectar espacio de usuario ↔️ kernel mediante comandos simples.
- Implementar y compilar módulos para arquitectura RISC-V.

---

## 🛠️ Componentes del proyecto

| Archivo            | Descripción                                       |
|--------------------|---------------------------------------------------|
| `src/led_driver.c` | Módulo del kernel para controlar el LED           |
| `src/user_app.c`   | Aplicación en C que interactúa con el driver      |
| `install.sh`       | Script para cargar el módulo y ejecutar la app    |
| `README.md`        | Documentación del proyecto                        |
| `Makefile`         | Compilación del módulo y app                      |

---

## ⚙️ Compilación

### 🔧 En PC (Cross-compilación)

1. Instalar toolchain:
   ```bash
   sudo apt install gcc-riscv64-linux-gnu
