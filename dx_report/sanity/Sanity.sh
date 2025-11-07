#!/bin/bash

# check option
OPTION=$1
if [ "$OPTION" == "" ]; then
    OPTION="all"
fi

USAGE="Usage: sudo ./SanityCheck.sh [all(default) | dx_rt | dx_driver | help]"
if [ "$OPTION" == "help" ]; then
    echo $USAGE
    exit 0
elif [[ "$OPTION" != "dx_rt" &&  "$OPTION" != "dx_driver" && "$OPTION" != "all" ]]; then
    echo "Unkonwn option "$OPTION
    echo $USAGE
    exit 1
fi


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

function SC_Legacy_Driver_Check() {
    echo "==== Legacy Driver Installed Check ====" | tee -a "$LOG_FILE"

    local DRIVER_FILE_PATH=/lib/modules/$(uname -r)/kernel/drivers/dxrt_driver.ko
    if [ -f $DRIVER_FILE_PATH ]; then
        echo "[INFO] "$DRIVER_FILE_PATH" ... OK" | tee -a "$LOG_FILE"
    else
        echo "[INFO] "$DRIVER_FILE_PATH" ... NONE" | tee -a "$LOG_FILE"
    fi

    local DMA_FILE_PATH=/lib/modules/$(uname -r)/kernel/drivers/dx_dma.ko
    if [ -f $DMA_FILE_PATH ]; then
        echo "[INFO] "$DMA_FILE_PATH" ... OK" | tee -a "$LOG_FILE"
    else
        echo "[INFO] "$DMA_FILE_PATH" ... NONE" | tee -a "$LOG_FILE"
    fi
}

function SC_DKMS_Check() {
    echo "==== DKMS Driver Installed Check ====" | tee -a "$LOG_FILE"

    DKMS_STATUS=$(dkms status -m dxrt-driver-dkms)
    if [[ -n "$DKMS_STATUS" ]]; then
        IFS=$'\n'
        for line in $DKMS_STATUS; do
            echo "[INFO] $line" | tee -a "$LOG_FILE"
        done
        unset IFS 
    else
        echo "[INFO] Not installed dxrt-driver-dkms"
    fi

    local DRIVER_FILE_PATH=/lib/modules/$(uname -r)/updates/dkms/dxrt_driver.ko
    if [ -f $DRIVER_FILE_PATH ]; then
        echo "[INFO] "$DRIVER_FILE_PATH" ... OK" | tee -a "$LOG_FILE"
    else
        echo "[INFO] "$DRIVER_FILE_PATH" ... NONE" | tee -a "$LOG_FILE"
    fi

    local DMA_FILE_PATH=/lib/modules/$(uname -r)/updates/dkms/dx_dma.ko
    if [ -f $DMA_FILE_PATH ]; then
        echo "[INFO] "$DMA_FILE_PATH" ... OK" | tee -a "$LOG_FILE"
    else
        echo "[INFO] "$DMA_FILE_PATH" ... NONE" | tee -a "$LOG_FILE"
    fi

}

function VersionDependencyCheck()
{
    echo "==== Runtime Version Dependency Check ====" | tee -a "$LOG_FILE"
    #local ERROR_FOUND=0

    RESULT=$(../../bin/examples/check_versions)
    RETURN_VALUE=$?

    if [ $RETURN_VALUE -eq 0 ]; then
        echo "[OK] Version Dependency Check" | tee -a "$LOG_FILE"
    else
        echo "[ERROR] Version Dependency Check" | tee -a "$LOG_FILE"
    fi
    IFS=$'\n'
    for line in $RESULT; do
        # Skip ONNX Runtime Version line if version starts with 0 (e.g., v0.0.0)
        if [[ "$line" =~ ^ONNX\ Runtime\ Version:\ v0 ]]; then
            continue
        fi
        echo "   $line" | tee -a "$LOG_FILE"
    done
    unset IFS 
    return $RETURN_VALUE
}

function ExecutableFileCheck()
{
    echo "==== Runtime Executable File Check ====" | tee -a "$LOG_FILE"

    # service
    SERVICE_NAME="dxrtd"

    IGNORE=$(../../bin/dxrt-cli -h)
    CLI_RETURN_VALUE=$?
    if [ $CLI_RETURN_VALUE -eq 0 ]; then
        RESULT+=$'\n'"dxrt-cli ...OK"
    else
        RESULT+=$'\n'"dxrt-cli ...ERROR"
    fi
    

    IGNORE=$(../../bin/run_model -h)
    RUN_MODEL_RETURN_VALUE=$?
    if [ $RUN_MODEL_RETURN_VALUE -eq 0 ]; then
        RESULT+=$'\n'"run_model ...OK"
    else
        RESULT+=$'\n'"run_model ...ERROR"
    fi

    IGNORE=$(../../bin/parse_model -h)
    PARSE_MODEL_RETURN_VALUE=$?
    if [ $PARSE_MODEL_RETURN_VALUE -eq 0 ]; then
        RESULT+=$'\n'"parse_model ...OK"
    else
        RESULT+=$'\n'"parse_model ...ERROR"
    fi

    IGNORE=$(../../bin/dxtop -h)
    DXTOP_RETURN_VALUE=$?
    if [ $DXTOP_RETURN_VALUE -eq 0 ]; then
        RESULT+=$'\n'"dxtop ...OK"
    else
        RESULT+=$'\n'"dxtop ...ERROR"
    fi

    if [ -f "../../bin/dxrtd" ]; then
        RESULT+=$'\n'"dxrtd ...OK"
        DXRTD_RETURN_VALUE=0
    else
        RESULT+=$'\n'"dxrtd ...ERROR"
        DXRTD_RETURN_VALUE=1
    fi

    # header file
    HEADER_PATH_1="/usr/include/dxrt"
    HEADER_PATH_2="/usr/local/include/dxrt"
    HEADER_RETURN_VALUE=1

    if [ -d $HEADER_PATH_1 ]; then
        RESULT+=$'\n'"Header: "$HEADER_PATH_1" ...OK"
        HEADER_RETURN_VALUE=0
    fi
    if [ -d $HEADER_PATH_2 ]; then
        RESULT+=$'\n'"Header: "$HEADER_PATH_2" ...OK"
        HEADER_RETURN_VALUE=0
    fi

    if [ $HEADER_RETURN_VALUE -ne 0 ]; then
        RESULT+=$'\n'"Header: "$HEADER_PATH_1" ...ERROR"
        RESULT+=$'\n'"Header: "$HEADER_PATH_2" ...ERROR"
    fi
    

    if [[ $RUN_MODEL_RETURN_VALUE -eq 0 
            && $CLI_RETURN_VALUE -eq 0 && $PARSE_MODEL_RETURN_VALUE -eq 0 
            && $DXTOP_RETURN_VALUE -eq 0 && $HEADER_RETURN_VALUE -eq 0 
            && $DXRTD_RETURN_VALUE -eq 0 ]]; then

        echo "[OK] Executable File Check" | tee -a "$LOG_FILE"
        RETURN_VALUE=0
    else
        echo "[ERROR] Executable File Check" | tee -a "$LOG_FILE"
        RETURN_VALUE=1
    fi
    IFS=$'\n'
    for line in $RESULT; do
        if [[ "$line" =~ ^ONNX\ Runtime\ Version:\ v0 ]]; then
            continue
        fi
        echo "   $line" | tee -a "$LOG_FILE"
    done
    unset IFS 

    return $RETURN_VALUE

}

function ServiceCheck()
{
    echo "==== Runtime dxrtd Service Check ====" | tee -a "$LOG_FILE"

    IGNORE=$(sudo systemctl status dxrt.service)
    RETURN_VALUE=$?

    if [ $RETURN_VALUE -eq 0 ]; then
        echo "[OK] The dxrtd service is running correctly. This is the expected behavior for builds with USE_SERVICE=ON on cmake/dxrt.cfg.cmake" | tee -a "$LOG_FILE"
    else
        echo "[WARN] The dxrtd service is not running. Please check if the build option was set to USE_SERVICE=OFF on cmake/dxrt.cfg.cmake" | tee -a "$LOG_FILE"
    fi

    return $RETURN_VALUE
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

echo "============================================================================" | tee -a "$LOG_FILE"
echo "==== Sanity Check Date : $(date) ====" | tee "$LOG_FILE"
echo "Log file location : $(pwd)/$LOG_FILE" | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"



if [[ "$OPTION" == "all" || "$OPTION" == "dx_driver" ]]; then

    SC_PCIeLinkUp
    VENDOR_STATUS=$?

    SC_DevFile
    DEV_STATUS=$?

    SC_DriverCheck
    DRIVER_STATUS=$?

    SC_Legacy_Driver_Check
    
    SC_DKMS_Check
  
else
    VENDOR_STATUS=0
    DEV_STATUS=0
    DRIVER_STATUS=0
fi


if [[ "$OPTION" == "all" || "$OPTION" == "dx_rt" ]]; then
    VersionDependencyCheck
    VERSION_STATUS=$?

    ExecutableFileCheck
    EXECUTABLE_FILE_STATUS=$?

    # ok or warning
    ServiceCheck
else
    VERSION_STATUS=0
    EXECUTABLE_FILE_STATUS=0
fi

echo
echo "============================================================================" | tee -a "$LOG_FILE"
if [[ $DEV_STATUS -ne 0 || $DRIVER_STATUS -ne 0 || $VENDOR_STATUS -ne 0 || $VERSION_STATUS -ne 0 || $EXECUTABLE_FILE_STATUS -ne 0 ]]; then
    echo "** Sanity check FAILED! Check logs at: $(pwd)/$LOG_FILE" | tee -a "$LOG_FILE"
    echo "** Please report this result to DEEPX with logs"

    if [[ $DEV_STATUS -ne 0 || $DRIVER_STATUS -ne 0 || $VENDOR_STATUS -ne 0 ]]; then
        CaptureDmesg
        CapturePCIeInfo
    fi
    
    echo "============================================================================" | tee -a "$LOG_FILE"
    exit 1
else
    echo "** Sanity check PASSED!" | tee -a "$LOG_FILE"
    echo "============================================================================" | tee -a "$LOG_FILE"
    exit 0
fi
