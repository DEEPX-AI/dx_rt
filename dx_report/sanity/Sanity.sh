#!/bin/bash
# Sanity Check script to verify deepx sdk environments
LOG_DIR="result"
mkdir -p "$LOG_DIR"
LOG_FILE="$LOG_DIR/sanity_check_result_$(date +'%Y%m%d_%H%M%S').log"
DMESG_FILE="$LOG_DIR/dmesg_$(date +'%Y%m%d_%H%M%S').log"
PCIE_INFO_FILE="$LOG_DIR/pcie_$(date +'%Y%m%d_%H%M%S').log"

function SC_DevFile() {
    echo "==== Device File Check ====" | tee -a "$LOG_FILE"
    local DEVICE_PATTERN="/dev/dxrt*"
    local FOUND_DEVICES=false
    local ERROR_FOUND=0

    for DEV in $DEVICE_PATTERN; do
        [[ -e "$DEV" ]] || continue

        FOUND_DEVICES=true
        echo "[OK] $DEV exists." | tee -a "$LOG_FILE"

        if [[ -c "$DEV" ]]; then
            echo "[OK] $DEV is a character device." | tee -a "$LOG_FILE"
        else
            echo "[ERROR] $DEV is not a character device." | tee -a "$LOG_FILE"
            ERROR_FOUND=1
            continue
        fi

        PERMS=$(stat -c "%a" "$DEV")
        if [[ "$PERMS" == "666" ]]; then
            echo "[OK] $DEV has correct permissions (0666)." | tee -a "$LOG_FILE"
        else
            echo "[ERROR] $DEV has incorrect permissions: $PERMS (expected: 0666)" | tee -a "$LOG_FILE"
            ERROR_FOUND=1
        fi
    done

    if [[ "$FOUND_DEVICES" == false ]]; then
        echo "[ERROR] No devices found matching pattern: $DEVICE_PATTERN" | tee -a "$LOG_FILE"
        ERROR_FOUND=1
    fi

    return $ERROR_FOUND
}

pci_id=""
function ExtractString() {
    local input_string="$1"
    local temp

    if [[ $input_string == *:* && $input_string != *:*:* ]]; then
        temp="$input_string"
    elif [[ $input_string == *:*:* ]]; then
        temp=$(echo "$input_string" | awk -F':' '{print $2":"$3}')
    fi
    echo "$temp"
}
function GetPCIeId(){
    local temp=`eval lspci -n | grep "$1" | tr ' ' '\n' | grep "[0-9]:*\."`
    local ext_temp=''
    if [ "$temp" != "" ]; then
        for id in ${temp}
        do
            ext_temp+=$(ExtractString "${id}")
            ext_temp+=" "
        done
        pci_id+=$(echo $ext_temp)
    fi
    # echo Detected PCI Device ID : ${pci_id}
}

RT_DRV_KO="dxrt_driver"
PCIE_DRV_KO="dx_dma"
function SC_DriverCheck() {
    echo "==== Kernel Module Check ====" | tee -a "$LOG_FILE"
    local ERROR_FOUND=0

    if lsmod | grep -q $RT_DRV_KO; then
        echo "[OK] $RT_DRV_KO module is loaded." | tee -a "$LOG_FILE"
    else
        echo "[ERROR] $RT_DRV_KO module is NOT loaded." | tee -a "$LOG_FILE"
        ERROR_FOUND=1
    fi

    if lsmod | grep -q $PCIE_DRV_KO; then
        echo "[OK] $PCIE_DRV_KO module is loaded." | tee -a "$LOG_FILE"
    else
        echo "[ERROR] $PCIE_DRV_KO module is NOT loaded." | tee -a "$LOG_FILE"
        ERROR_FOUND=1
    fi

    GetPCIeId 1ff4
    for id in ${pci_id}
    do
        if lspci -vvk -s ${id}| grep -q "Kernel driver in use: dx_dma_pcie"; then
            echo "[OK] PCIe ${id} driver probe is success." | tee -a "$LOG_FILE"
        else
            echo "[ERROR] PCIe ${id} driver probe is fail." | tee -a "$LOG_FILE"
            ERROR_FOUND=1
        fi
    done

    return $ERROR_FOUND
}

DX_VENDOR_ID="1ff4"
function SC_PCIeLinkUp() {
    echo "==== PCI Link-up Check ====" | tee -a "$LOG_FILE"
    local DEV_NUM=$(lspci -n | grep -c "$DX_VENDOR_ID")
    if [ "$DEV_NUM" -gt 0 ]; then
        echo "[OK] Vendor ID $DX_VENDOR_ID is present in the PCI devices.(num=$DEV_NUM)" | tee -a "$LOG_FILE"
    else
        echo "[ERROR] Vendor ID $DX_VENDOR_ID is NOT found in the PCI devices." | tee -a "$LOG_FILE"
        return 1
    fi
    return 0
}

function CaptureDmesg() {
    sudo dmesg > "$DMESG_FILE"
    echo "dmesg logs saved to: $DMESG_FILE" | tee -a "$LOG_FILE"
}

function CapturePCIeInfo() {
    touch $PCIE_INFO_FILE
    for id in ${pci_id}
    do
        sudo lspci -vvv -s ${id} >> "$PCIE_INFO_FILE"
    done
    echo "pcie infomation saved to: $PCIE_INFO_FILE" | tee -a "$LOG_FILE"
}

# main body of this script
if [[ $(id -u) -ne 0 ]]; then
    echo "Error: Please run this script as root (use 'sudo')."
    exit 2
fi

echo "============================================================================"
echo "==== Sanity Check Date : $(date) ====" | tee "$LOG_FILE"
echo "Log file location : $(pwd)/$LOG_FILE" | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"

SC_PCIeLinkUp
VENDOR_STATUS=$?

SC_DevFile
DEV_STATUS=$?

SC_DriverCheck
DRIVER_STATUS=$?

echo
echo "============================================================================"
if [[ $DEV_STATUS -ne 0 || $DRIVER_STATUS -ne 0 || $VENDOR_STATUS -ne 0 ]]; then
    echo "** Sanity check FAILED! Check logs at: $LOG_FILE" | tee -a "$LOG_FILE"
    echo "** Please report this result to DEEPX with logs"
    CaptureDmesg
    CapturePCIeInfo
    echo "============================================================================"
    exit 1
else
    echo "** Sanity check PASSED!" | tee -a "$LOG_FILE"
    echo "============================================================================"
    exit 0
fi
