obj-m += led_driver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CROSS_COMPILE := riscv64-linux-gnu-
ARCH := riscv

all:
	make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
