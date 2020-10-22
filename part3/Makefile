obj-m := elevator.o

PWD := `pwd`
KERNELDIR := /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

insert:
	insmod elevator.ko
	dmesg

clean:
	rm -f *.ko *.o Module* *mod*

remove:
	rmmod elevator.ko
	dmesg
