obj-m+= ioctl.o
#obj-m is to build as module
#obj-y is to build as statistically

KDIR=/lib/modules/5.11.0-27-generic/build

all:
	make -C $(KDIR)  M=$(shell pwd) modules
clean:
	make -C $(KDIR)  M=$(shell pwd) clean
							

