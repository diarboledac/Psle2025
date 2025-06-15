// led_driver.c - Módulo base para driver de LED en Lychee RV Dock

#include <linux/module.h>   // Necesario para todos los módulos
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // Macros __init y __exit

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Módulo base del kernel para control de LED vía GPIO");
MODULE_VERSION("0.1");

// Función que se ejecuta al cargar el módulo
static int __init led_driver_init(void) {
    printk(KERN_INFO "[led_driver] Módulo cargado correctamente.\n");
    return 0;
}

// Función que se ejecuta al remover el módulo
static void __exit led_driver_exit(void) {
    printk(KERN_INFO "[led_driver] Módulo descargado.\n");
}

module_init(led_driver_init);
module_exit(led_driver_exit);
