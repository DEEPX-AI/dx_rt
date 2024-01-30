#!/bin/bash
kernel_dir=/home/yr/eyenix/en677_linux_sdk/bsp_release/en677_linux_bsp/work/linux
cross_compile=riscv64-enx122-linux-gnu-

#################################################################################

source common.sh

rt=./unified

build_kernel_module $rt device=l1 arch=riscv64 kdir=$kernel_dir cross_compile=$cross_compile $1
