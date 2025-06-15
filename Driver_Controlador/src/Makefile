# Nombre del objeto del módulo
obj-m += led_driver.o

# Variables para cross-compilación (modifícalas si usas headers personalizados)
KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
