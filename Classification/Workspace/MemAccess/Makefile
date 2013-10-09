obj-m += MemAccess.o

HOME_DIR = /home/tomer/Android

KERNELDIR ?= $(HOME_DIR)/kernels/goldfishWeka

PWD := $(shell pwd)


CROSS_COMPILE=$(HOME_DIR)/EmulatorFramework/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	rm -R -f *.o *.d *.so *.a *~ *.ko *.mod.* *.cmd .*\.cmd modules.order Module.symvers .tmp_versions

deploy:
	$(HOME_DIR)/deploy --adb=$(HOME_DIR)/adb MemAccess.ko
	