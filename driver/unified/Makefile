TARGET_MODULE := dxrt_driver

# DEVICE_TYPE: 0: ACC, 1: STD
# DEVICE_VARIANT: 100: L1, 101: L2, 102: L3, 103: L4, 200: M1, 201: M1A 
ifeq ($(device),)
device = m1
endif
ifeq ($(pcie),)
pcie = snps
endif
ifeq ($(kdir),)
kdir = /lib/modules/$(shell uname -r)/build
endif

$(TARGET_MODULE)-objs += dxrt_drv.o
$(TARGET_MODULE)-objs += dxrt_drv_cdev.o
$(TARGET_MODULE)-objs += dxrt_drv_message.o
$(TARGET_MODULE)-objs += dxrt_drv_thread.o
$(TARGET_MODULE)-objs += dxrt_drv_queue.o
$(TARGET_MODULE)-objs += dxrt_drv_npu.o

# ccflags-y += -fno-stack-protector

ifeq ($(device),m1)
ccflags-y += -I$(PWD)/../pcie/$(pcie)/include
EXTRA_CFLAGS += -DIS_ACCELERATOR=1
EXTRA_CFLAGS += -DIS_STANDALONE=0
EXTRA_CFLAGS += -DDEVICE_TYPE=0
EXTRA_CFLAGS += -DDEVICE_VARIANT=DX_M1
EXTRA_CFLAGS += -DNUM_DEVICES=0
else ifeq ($(device),l1)
$(TARGET_MODULE)-objs += dxrt_drv_npu_l1.o
EXTRA_CFLAGS += -DIS_ACCELERATOR=0
EXTRA_CFLAGS += -DIS_STANDALONE=1
EXTRA_CFLAGS += -DDEVICE_TYPE=1
EXTRA_CFLAGS += -DDEVICE_VARIANT=DX_L1
EXTRA_CFLAGS += -DNUM_DEVICES=1
else ifeq ($(device),l3)
$(TARGET_MODULE)-objs += dxrt_drv_npu_l3.o
EXTRA_CFLAGS += -DIS_ACCELERATOR=0
EXTRA_CFLAGS += -DIS_STANDALONE=1
EXTRA_CFLAGS += -DDEVICE_TYPE=1
EXTRA_CFLAGS += -DDEVICE_VARIANT=DX_L3
EXTRA_CFLAGS += -DNUM_DEVICES=1
else
$(error Error: Invalid device: $(device))
endif

ifeq ($(debug),1)
EXTRA_CFLAGS += -DDEBUG
endif

obj-m += $(TARGET_MODULE).o

PWD:=$(shell pwd)

ifeq ($(arch),)
all:
	@echo "arch: $(arch)"
	@echo "device: $(device)"
	$(MAKE) -C $(kdir) M=$(PWD) modules KBUILD_EXTRA_SYMBOLS=`pwd`/../pcie/$(pcie)/Module.symvers
clean:
	rm -f *.ko *.o *.mod* modules.order Module.symvers .tmp_versions .*cmd
else ifeq ($(arch),riscv64)
all:
	@echo "arch: $(arch)"
	@echo "device: $(device)"
	@echo "kdir: $(kdir)"
	@echo "cross_compile: $(cross_compile)"
	$(MAKE) -C $(kdir) ARCH=riscv CROSS_COMPILE=$(cross_compile) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod* modules.order Module.symvers .tmp_versions .*cmd
else
all:
	@echo "Error: $(arch) is unsupported architecture."
clean:
	rm -f *.ko *.o *.mod* modules.order Module.symvers .tmp_versions .*cmd
endif