KBUILD_EXTRA_SYMBOLS := /home/student/Projects/linux-kernel-classes/modules/chardev/Module.symvers

obj-m += depnmodule.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
