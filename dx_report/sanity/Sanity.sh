#!/bin/bash

# check option
OPTION=$1
if [ "$OPTION" == "" ]; then
    OPTION="all"
fi

USAGE="Usage: ./SanityCheck.sh [all(default) | dx_rt | dx_driver | help]"
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

    IGNORE=$(../../bin/dxcli -h)
    CLI_RETURN_VALUE=$?
    if [ $CLI_RETURN_VALUE -eq 0 ]; then
        RESULT+=$'\n'"dxcli ...OK"
    else
        RESULT+=$'\n'"dxcli ...ERROR"
    fi


    IGNORE=$(../../bin/dxrun -h)
    RUN_MODEL_RETURN_VALUE=$?
    if [ $RUN_MODEL_RETURN_VALUE -eq 0 ]; then
        RESULT+=$'\n'"dxrun ...OK"
    else
        RESULT+=$'\n'"dxrun ...ERROR"
    fi

    IGNORE=$(../../bin/dxparse -h)
    PARSE_MODEL_RETURN_VALUE=$?
    if [ $PARSE_MODEL_RETURN_VALUE -eq 0 ]; then
        RESULT+=$'\n'"dxparse ...OK"
    else
        RESULT+=$'\n'"dxparse ...ERROR"
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

    IGNORE=$(systemctl status dxrt.service)
    RETURN_VALUE=$?

    if [ $RETURN_VALUE -eq 0 ]; then
        echo "[OK] The dxrtd service is running correctly. This is the expected behavior for builds with USE_SERVICE=ON on cmake/dxrt.cfg.cmake" | tee -a "$LOG_FILE"
    else
        echo "[WARN] The dxrtd service is not running. Please check if the build option was set to USE_SERVICE=OFF on cmake/dxrt.cfg.cmake" | tee -a "$LOG_FILE"
    fi

    return $RETURN_VALUE
}

# main body of this script
echo "============================================================================" | tee -a "$LOG_FILE"
echo "==== Sanity Check Date : $(date) ====" | tee "$LOG_FILE"
echo "Log file location : $(pwd)/$LOG_FILE" | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"



if [[ "$OPTION" == "all" || "$OPTION" == "dx_driver" ]]; then

    SC_DevFile
    DEV_STATUS=$?
else
    DEV_STATUS=0
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
if [[ $DEV_STATUS -ne 0 || $VERSION_STATUS -ne 0 || $EXECUTABLE_FILE_STATUS -ne 0 ]]; then
    echo "** Sanity check FAILED! Check logs at: $(pwd)/$LOG_FILE" | tee -a "$LOG_FILE"
    echo "** Please report this result to DEEPX with logs"

    echo "============================================================================" | tee -a "$LOG_FILE"
    exit 1
else
    echo "** Sanity check PASSED!" | tee -a "$LOG_FILE"
    echo "============================================================================" | tee -a "$LOG_FILE"
    exit 0
fi
