#!/bin/bash
source common.sh

pcie=./pcie/snps
rt=./unified

build_kernel_module $pcie $1
build_kernel_module $rt pcie=snps $1

./uninstall.sh
./module_insert_host.sh $pcie true
./module_insert_host.sh $rt