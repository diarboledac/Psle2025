// led_driver.c - Módulo del kernel que registra un character device

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>           // Para alloc_chrdev_region, struct file_operations
#include <linux/cdev.h>         // Para struct cdev, cdev_init, etc.
#include <linux/device.h>       // Para class_create y device_create

#define DEVICE_NAME "led_driver"
#define CLASS_NAME  "led"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diego Arboleda");
MODULE_DESCRIPTION("Driver de caracter para control de LED");
MODULE_VERSION("0.2");

static int majorNumber;
static struct class*  ledClass  = NULL;
static struct device* ledDevice = NULL;
static struct cdev led_cdev;

// Funciones del device
static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "[led_driver] Dispositivo abierto.\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "[led_driver] Dispositivo cerrado.\n");
    return 0;
}

// Estructura con los callbacks del device
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
};

// Init del módulo
static int __init led_driver_init(void) {
    int ret;
    dev_t dev_no;

    printk(KERN_INFO "[led_driver] Cargando módulo...\n");

    // 1. Reservar un major y minor dinámico
    ret = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ALERT "[led_driver] Fallo al asignar major number.\n");
        return ret;
    }

    majorNumber = MAJOR(dev_no);
    printk(KERN_INFO "[led_driver] Major asignado: %d\n", majorNumber);

    // 2. Inicializar y agregar el cdev
    cdev_init(&led_cdev, &fops);
    led_cdev.owner = THIS_MODULE;

    ret = cdev_add(&led_cdev, dev_no, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev_no, 1);
        printk(KERN_ALERT "[led_driver] Fallo al agregar cdev.\n");
        return ret;
    }

    // 3. Crear clase del dispositivo
    ledClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(ledClass)) {
        cdev_del(&led_cdev);
        unregister_chrdev_region(dev_no, 1);
        printk(KERN_ALERT "[led_driver] Fallo al crear clase del dispositivo.\n");
        return PTR_ERR(ledClass);
    }

    // 4. Crear el dispositivo
    ledDevice = device_create(ledClass, NULL, dev_no, NULL, DEVICE_NAME);
    if (IS_ERR(ledDevice)) {
        class_destroy(ledClass);
        cdev_del(&led_cdev);
        unregister_chrdev_region(dev_no, 1);
        printk(KERN_ALERT "[led_driver] Fallo al crear el dispositivo.\n");
        return PTR_ERR(ledDevice);
    }

    printk(KERN_INFO "[led_driver] Dispositivo creado exitosamente en /dev/%s\n", DEVICE_NAME);
    return 0;
}

// Exit del módulo
static void __exit led_driver_exit(void) {
    device_destroy(ledClass, MKDEV(majorNumber, 0));
    class_unregister(ledClass);
    class_destroy(ledClass);
    cdev_del(&led_cdev);
    unregister_chrdev_region(MKDEV(majorNumber, 0), 1);
    printk(KERN_INFO "[led_driver] Módulo descargado y dispositivo eliminado.\n");
}

module_init(led_driver_init);
module_exit(led_driver_exit);
