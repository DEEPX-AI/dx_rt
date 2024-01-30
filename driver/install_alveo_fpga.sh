#!/bin/bash
source common.sh

pcie=./pcie/xilinx
rt=./unified

build_kernel_module $pcie
build_kernel_module $rt pcie=xilinx

./uninstall.sh
./module_insert_host.sh $pcie true
./module_insert_host.sh $rt