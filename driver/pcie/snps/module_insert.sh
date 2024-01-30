#!/bin/bash
module="dx_dma.ko"
module_n=$(echo $module | cut  -d '.' -f1)

if [[ "$(id -u)" -ne "0" ]]; then
	echo "[ERR] Module insertion script should run as root"
	exit 1
fi

echo "** Start loading the DeepX PCIe driver [$module_n] **"
## Check dependancy module
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

chk_module=$(lsmod | grep -c ${module_n})
if [[ "$chk_module" -ge "1" ]]; then
    echo "PCIe driver is already loaded and remove it"
    sudo rmmod ${module_n}
    if [ $? -eq 0 ]; then
        echo "rmmod success(${module_n})"
    else
        echo "[ERR] rmmod failed"
        exit
    fi
fi
## Module load
sudo insmod $module

chk_module=$(lsmod | grep -c ${module_n})
if [[ "$chk_module" -ge "1" ]]; then
    echo "PCIe driver loaded successfully!!"
else
    echo "[ERR] Please check compile!!"
fi
