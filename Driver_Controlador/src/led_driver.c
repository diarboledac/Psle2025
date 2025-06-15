/**
 * led_driver.c - Módulo de kernel para controlar un LED conectado a un pin GPIO
 * 
 * Este módulo crea un dispositivo de caracteres en /dev/led_driver.
 * Los usuarios pueden escribir "on" o "off" para controlar el estado del LED.
 * 
 * Autor: Diego Arboleda
 * Fecha: junio 2025
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/gpio.h>          // API de GPIO
#include <linux/uaccess.h>       // copy_from_user()

#define DEVICE_NAME "led_driver"
#define CLASS_NAME "led"
#define GPIO_PIN 37              // Cambiar este número segun el  GPIO que se este usando

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diego Arboleda");
MODULE_DESCRIPTION("Driver de caracter para control de LED usando GPIO");
MODULE_VERSION("0.3");

static int majorNumber;
static struct class*  ledClass  = NULL;
static struct device* ledDevice = NULL;
static struct cdev led_cdev;
#define BUF_LEN 10
static char command_buf[BUF_LEN];

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "[led_driver] Dispositivo abierto\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "[led_driver] Dispositivo cerrado\n");
    return 0;
}




/**
 * dev_write - Interpreta comandos "on"/"off" para controlar el LED
 * @filep: puntero al archivo
 * @buffer: cadena recibida desde user space
 * @len: longitud del buffer
 * @offset: desplazamiento del archivo (no usado)
 * 
 * Retorna el número de bytes escritos o un error.
 */

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    if (len > BUF_LEN - 1)
        len = BUF_LEN - 1;

    if (copy_from_user(command_buf, buffer, len) != 0) {
        printk(KERN_WARNING "[led_driver] Error al copiar datos desde espacio de usuario\n");
        return -EFAULT;
    }

    command_buf[len] = '\0';

    // Eliminar salto de línea si lo hay
    command_buf[strcspn(command_buf, "\n")] = 0;

    if (strcasecmp(command_buf, "on") == 0) {
        gpio_set_value(GPIO_PIN, 1);
        printk(KERN_INFO "[led_driver] LED ENCENDIDO\n");
    } else if (strcasecmp(command_buf, "off") == 0) {
        gpio_set_value(GPIO_PIN, 0);
        printk(KERN_INFO "[led_driver] LED APAGADO\n");
    } else {
        printk(KERN_WARNING "[led_driver] Comando inválido: %s\n", command_buf);
        return -EINVAL;
    }

    return len;
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .write = dev_write,
};

static int __init led_driver_init(void) {
    int ret;
    dev_t dev_no;

    printk(KERN_INFO "[led_driver] Inicializando módulo...\n");

    // 1. Registrar major/minor
    ret = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);
    if (ret < 0) return ret;

    majorNumber = MAJOR(dev_no);

    // 2. Inicializar cdev
    cdev_init(&led_cdev, &fops);
    led_cdev.owner = THIS_MODULE;
    if (cdev_add(&led_cdev, dev_no, 1) < 0) goto fail_region;

    // 3. Crear clase y dispositivo
    ledClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(ledClass)) goto fail_cdev;
    ledDevice = device_create(ledClass, NULL, dev_no, NULL, DEVICE_NAME);
    if (IS_ERR(ledDevice)) goto fail_class;

    // 4. Solicitar GPIO
    if (!gpio_is_valid(GPIO_PIN)) {
        printk(KERN_ALERT "[led_driver] GPIO no válido\n");
        goto fail_device;
    }

    ret = gpio_request(GPIO_PIN, "LED_GPIO");
    if (ret) {
        printk(KERN_ALERT "[led_driver] No se pudo solicitar GPIO\n");
        goto fail_device;
    }

    gpio_direction_output(GPIO_PIN, 0);  // LED apagado al inicio
    printk(KERN_INFO "[led_driver] LED listo en GPIO %d\n", GPIO_PIN);

    return 0;

fail_device:
    device_destroy(ledClass, dev_no);
fail_class:
    class_destroy(ledClass);
fail_cdev:
    cdev_del(&led_cdev);
fail_region:
    unregister_chrdev_region(dev_no, 1);
    return -1;
}

static void __exit led_driver_exit(void) {
    gpio_set_value(GPIO_PIN, 0);  // Apagar LED al salir
    gpio_free(GPIO_PIN);
    device_destroy(ledClass, MKDEV(majorNumber, 0));
    class_unregister(ledClass);
    class_destroy(ledClass);
    cdev_del(&led_cdev);
    unregister_chrdev_region(MKDEV(majorNumber, 0), 1);
    printk(KERN_INFO "[led_driver] Módulo descargado y GPIO liberado\n");
}

module_init(led_driver_init);
module_exit(led_driver_exit);
