#
  # Makefile for kernel test
  #
PWD         := $(shell pwd) 
KVERSION    := $(shell uname -r)
#KERNEL_DIR   = /usr/src/linux-headers-$(KVERSION)/
KERNEL_DIR  := /lib/modules/3.18.28/build
MODULE_NAME  = slave_device
obj-m       := $(MODULE_NAME).o   

all:
	cp ../ksocket/Module.symvers ./Module.symvers
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
