#!/bin/bash

source common.sh

if [[ "$(id -u)" -ne "0" ]]; then
	echo "Error: Module insertion script should run as root"
	exit 1
fi

module_path=$1
is_pcie_module=${2:-false}
module_path=$(readlink -f $module_path)
echo "** Start loading module from [$module_path] **"
module=$(check_module_file $module_path)
if [ "$module" = "" ]; then    
    echo "Error: No module in $module_path."
    exit 1
fi
module_name=$(echo $(basename $module) | cut  -d '.' -f1)

# echo "Module : $module"
# echo "Module Name: $module_name"

chk_module=$(lsmod | grep -c ${module_name})
if [[ "$chk_module" -ge "1" ]]; then
    echo "$module_name is already loaded and remove it"
    sudo rmmod $module_name
fi

## Check dependancy for pcie module
if [ "$is_pcie_module" = true ]; then
    dependancy_f=/boot/config-$(uname -r)
    if [[ -f "$dependancy_f" ]]; then
        dependancy=$(cat ${dependancy_f} | grep CONFIG_DMA_VIRTUAL_CHANNELS | cut -d '=' -f2)
        if [[ "$dependancy" == "y" ]]; then
            echo "Dependancy check pass!!"
        else
            echo "Check Pre-build module for virt_dma"
            virt_dma=virt_dma
            virt_dma_m=/lib/modules/$(uname -r)/kernel/drivers/dma/virt-dma.ko
            if [[ -f "$virt_dma_m" ]]; then
                if [[ "$(lsmod | grep -c ${virt_dma})" -eq "0" ]]; then
                    echo "module insertion(${virt_dma})"
                    sudo insmod $virt_dma_m
                fi
            else
                echo "Please contact to pcie engineer"
                exit 1
            fi
        fi
    else
        echo "Dependancy check skip!!"
    fi
    sudo ./pcie_rescan.sh
fi
## Module load
sudo insmod $module
check_module_install $module_name