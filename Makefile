obj-m += project.o
KDIR = /usr/src/linux-headers-3.11.0-12-generic/
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
in:
	insmod project.ko
out:
	rmmod project.ko
