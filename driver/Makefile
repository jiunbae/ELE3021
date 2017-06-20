# kbuild part of makefile
obj-m  := device_driver_test.o

KERNEL_DIR := /lib/modules/`uname -r`/build
PWD := `pwd`

default:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) clean
