#!/bin/bash
dxrt_ver=`cat release.ver`

SCRIPT_DIR=$(realpath "$(dirname "$0")")

# color env settings
source "${SCRIPT_DIR}/scripts/color_env.sh"
source "${SCRIPT_DIR}/scripts/common_util.sh"

help() {
    echo -e "Usage: ${COLOR_CYAN}$0 [OPTIONS]${COLOR_RESET}"
    echo -e "Build the dx-rt project with various options."
    echo -e ""
    echo -e "${COLOR_BOLD}Options:${COLOR_RESET}"
    echo -e "  ${COLOR_GREEN}--help${COLOR_RESET}            Display this help message and exit."
    echo -e "  ${COLOR_GREEN}--clean${COLOR_RESET}           Perform a clean build, removing previous build artifacts."
    echo -e "  ${COLOR_GREEN}--verbose${COLOR_RESET}         Show detailed build commands during the process."
    echo -e "  ${COLOR_GREEN}--type <TYPE>${COLOR_RESET}     Specify the CMake build type. Valid options: [Release, Debug, RelWithDebInfo]."
    echo -e "  ${COLOR_GREEN}--arch <ARCH>${COLOR_RESET}     Specify the target CPU architecture. Valid options: [x86_64, aarch64]."
    echo -e "  ${COLOR_GREEN}--install <PATH>${COLOR_RESET}  Specify the installation path for dx-rt files."
    echo -e "  ${COLOR_GREEN}--uninstall${COLOR_RESET}       Remove previously installed dx-rt files."
    echo -e "  ${COLOR_GREEN}--docker${COLOR_RESET}          Build the project within a Docker environment."
    echo -e "  ${COLOR_GREEN}--clang${COLOR_RESET}           Use Clang as the compiler for the build."
    echo -e ""
    echo -e "  ${COLOR_GREEN}--python_exec <PATH>${COLOR_RESET} Specify the Python executable to use for the build."
    echo -e "                            If omitted, the default system 'python3' will be used."
    echo -e "  ${COLOR_GREEN}--venv_path <PATH>${COLOR_RESET}  Specify the path to a virtual environment to activate for the build."
    echo -e "                            If omitted, no virtual environment will be activated."
    echo -e "  ${COLOR_GREEN}--use_service_on${COLOR_RESET}  Enable the use of the service in the build."
    echo -e "  ${COLOR_GREEN}--use_service_off${COLOR_RESET} Disable the use of the service in the build."
    echo -e "  ${COLOR_GREEN}--use_ort_on${COLOR_RESET}      Enable the use of the ORT component in the build."
    echo -e "  ${COLOR_GREEN}--use_ort_off${COLOR_RESET}     Disable the use of the ORT component in the build."
    echo -e ""
    echo -e "${COLOR_BOLD}Examples:${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --type Release --arch x86_64${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --clean --install /opt/dx-rt${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --docker --verbose${COLOR_RESET}"
    echo -e ""
    echo -e "  ${COLOR_YELLOW}$0 --python_exec /usr/local/bin/python3.8${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --venv_path ./venv-dxnn${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --use_service_off --use_ort_off${COLOR_RESET}"

    if [ "$1" == "error" ] && [[ ! -n "$2" ]]; then
        echo -e "${TAG_ERROR} Invalid or missing arguments."
        exit 1
    elif [ "$1" == "error" ] && [[ -n "$2" ]]; then
        echo -e "${TAG_ERROR} $2"
        exit 1
    elif [[ "$1" == "warn" ]] && [[ -n "$2" ]]; then
        echo -e "${TAG_WARN} $2"
        return 0
    fi
    exit 0
}

check_cross_compile() {
    if [ $target_arch == "arm64" ]; then
        target_arch=aarch64
    fi

    if [ $target_arch == $host_arch ]; then
        if [ -z $install ]; then
            install=/usr/local
        fi
        CROSS_COMPILE="native"
    else
        CROSS_COMPILE=$target_arch
    fi

    build_dir=build_"$target_arch"
    out_dir=bin
}

setup_env() {

    # cmake command
    cmd=()
    clean_build=false
    install_ort=false
    verbose=false
    build_type=release
    host_arch=$(uname -m)
    target_arch=$(uname -m)
    build_mode="Release Build"
    install=""
    uninstall=false
    clang=false
    build_in_docker=false
    python_exec="python3"
    venv_path=""


    # parsing dxrt.cfg.cmake
    CMAKE_USE_ORT=false
    CMAKE_USE_SERVICE=true
    CMAKE_USE_PYTHON=true

    parse_cmake_options() {
        local file="$1"
        while IFS= read -r line || [ -n "$line" ]; do
            parsed=$(echo "$line" | sed -E 's/option\(([^)]+)\)/\1/' )
            IFS= read -r -a parts <<< "$(echo "$parsed" | sed -E 's/[[:space:]]+/ /g')"
            var_name=$(echo "$parsed" | awk -F'"' '{print $1}' | xargs | cut -d' ' -f1)
            default_val=$(echo "$parsed" | awk -F'"' '{print $2; print $3}' | tail -n1 | xargs)
            if [ "$var_name" == 'USE_ORT' ]; then
                if [ "$default_val" == 'ON' ]; then
                    CMAKE_USE_ORT=true
                else
                    CMAKE_USE_ORT=false
                fi
            elif [ "$var_name" == 'USE_SERVICE' ]; then
                if [ "$default_val" == 'ON' ]; then
                    CMAKE_USE_SERVICE=true
                else
                    CMAKE_USE_SERVICE=false
                fi
            elif [ "$var_name" == 'USE_PYTHON' ]; then
                if [ "$default_val" == 'ON' ]; then
                    CMAKE_USE_PYTHON=true
                else
                    CMAKE_USE_PYTHON=false
                fi
            fi
        done < "$file"
    }

    # read cmake/dxrt.cfg.cmake file
    CMAKE_FILE="cmake/dxrt.cfg.cmake"
    if [ -f "$CMAKE_FILE" ]; then
        parse_cmake_options "$CMAKE_FILE"
    else
        print_colored_v2 "WARNING" "File '$CMAKE_FILE' not found."
    fi

    # display cmake options
    print_colored_v2 "INFO" "========== CMAKE OPTIONS ======================================"
    print_colored_v2 "INFO" "CMAKE_USE_ORT=$CMAKE_USE_ORT"
    print_colored_v2 "INFO" "CMAKE_USE_SERVICE=$CMAKE_USE_SERVICE"
    print_colored_v2 "INFO" "CMAKE_USE_PYTHON=$CMAKE_USE_PYTHON"
    print_colored_v2 "INFO" "==============================================================="
}

build_dxrt() {
    # install onnx runtime
    if [ $install_ort == "true" ]; then
        ./install.sh --onnxruntime --arch $target_arch || {
            return_code=$?
            print_colored_v2 "ERROR" "Failed to install ONNX Runtime."
            exit $return_code
        }
    fi

    cmd+=(-DCMAKE_TOOLCHAIN_FILE=cmake/toolchain.$target_arch.cmake)
    if [ $clang == "true" ]; then
        cmd+=(-DUSE_CLANG=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++)
    fi

    cmd+=(-DCMAKE_VERBOSE_MAKEFILE=$verbose)
    if [ $build_type == "release" ] || [ $build_type == "debug" ] || [ $build_type == "relwithdebinfo" ]; then
        cmd+=(-DCMAKE_BUILD_TYPE=$build_type);
    else
        cmd+=(-DCMAKE_BUILD_TYPE=release);
    fi

    cmd+=(-DCMAKE_GENERATOR=Ninja)

    if [ ! -z $install ]; then
        cmd+=(-DCMAKE_INSTALL_PREFIX=$install)
    fi
    
    #cmd+=(-DPython_EXECUTABLE=$(which ${python_exec}))
    # Force update Python path in CMake cache to avoid stale path issues
    # If virtual environment is active, use it; otherwise use the specified python_exec
    if [ -n "$VIRTUAL_ENV" ]; then
        PYTHON_PATH="$VIRTUAL_ENV/bin/python3"
        print_colored_v2 "INFO" "Using Python from virtual environment: $PYTHON_PATH"
    else
        PYTHON_PATH=$(which ${python_exec})
        print_colored_v2 "INFO" "Using system Python: $PYTHON_PATH"
    fi
    cmd+=(-DPython_EXECUTABLE:FILEPATH=${PYTHON_PATH})


    # set cmake options for service
    if [ $CMAKE_USE_SERVICE == "true" ]; then
        cmd+=(-DUSE_SERVICE=ON)
    else
        cmd+=(-DUSE_SERVICE=OFF)
    fi

    # set cmake options for ort
    if [ $CMAKE_USE_ORT == "true" ]; then
        cmd+=(-DUSE_ORT=ON)
    else
        cmd+=(-DUSE_ORT=OFF)
    fi

    print_colored_v2 "INFO" "========== CMAKE ARGS ========================================"
    print_colored_v2 "INFO" "cmake args : ${cmd[@]}"
    print_colored_v2 "INFO" "==============================================================="

    [ $clean_build == "true" ] && sudo rm -rf $build_dir

    # Remove stale Python extension modules when cleaning to avoid ABI/loader conflicts
    if [ $clean_build == "true" ]; then
        print_colored_v2 "INFO" "[CLEAN] Removing stale Python extension modules (_pydxrt*.so)"
        rm -f "${SCRIPT_DIR}/python_package/src/dx_engine/capi/_pydxrt"*.so || true
    fi

    mkdir -p $build_dir
    mkdir -p $out_dir
    sudo rm -rf $build_dir/bin
    
    # Proactively remove stale Python extension modules before (re)build
    print_colored_v2 "INFO" "[PREP] Removing stale Python extension modules (_pydxrt*.so)"
    rm -f "${SCRIPT_DIR}/python_package/src/dx_engine/capi/_pydxrt"*.so || true
    
    pushd $build_dir >/dev/null
    # Prefer system CMake to avoid Python-venv cmake shim issues
    if [ -x /usr/local/bin/cmake ]; then
        CMAKE_BIN=/usr/local/bin/cmake
    elif [ -x /usr/bin/cmake ]; then
        CMAKE_BIN=/usr/bin/cmake
    else
        CMAKE_BIN=$(command -v cmake)
    fi

    echo "[BUILD] Using CMake: ${CMAKE_BIN}"
    ${CMAKE_BIN} .. ${cmd[@]} || {
        CMAKE_STATUS=$?
        print_colored_v2 "ERROR" "CMake configuration failed. If a Python virtualenv is active, it may be shadowing system cmake. Try deactivating the venv or ensure system CMake is used."
        exit $CMAKE_STATUS
    }

    ninja || {
        return_code=$?
        print_colored_v2 "ERROR" "Failed to build the project."
        exit $return_code
    }

    sudo ninja install || {
        return_code=$?
        print_colored_v2 "ERROR" "Failed to install the project."
        exit $return_code
    }
    popd >/dev/null

    if [ $CROSS_COMPILE != "native" ]; then
        print_colored_v2 "WARNING" "dxtop is not intended for cross-compilation. If required, please build it natively on the target system."
    fi
}

# This function checks the system's python3 version.
# It prints a warning if the version is 3.8 or lower.
check_python_version() {
    # Check if python3 is installed and available in the system's PATH.
    if ! command -v python3 &> /dev/null; then
        print_colored_v2 "WARNING" "python3 is not installed or not in PATH."
        return 1
    fi

    # Get the python3 version string (e.g., "3.8.10").
    # This extracts the second field from an output like "Python 3.8.10".
    local PY_VERSION
    PY_VERSION=$(python3 --version 2>&1 | awk '{print $2}')

    # Split the version string into major and minor parts using the '.' delimiter.
    local MAJOR MINOR
    MAJOR=$(echo "$PY_VERSION" | cut -d. -f1)
    MINOR=$(echo "$PY_VERSION" | cut -d. -f2)

    # Check if the major version is less than 3, or if the major version is 3
    # and the minor version is 9 or less.
    if [[ "$MAJOR" -lt 3 || ( "$MAJOR" -eq 3 && "$MINOR" -le 8 ) ]]; then
        # If the condition is met, print the warning message.
        print_colored_v2 "WARNING" "The current Python version ($PY_VERSION) is 3.8 or lower. Some features may not work correctly."
        return 1
    else
        # If the condition is not met, print the confirmation message.
        print_colored_v2 "INFO" "Python version ($PY_VERSION) meets the requirements."
        return 0
    fi
}

install_python_package() {
    if [ $CMAKE_USE_PYTHON == "true" ]; then
        if [ $CROSS_COMPILE == "native" ]; then

            print_colored_v2 "INFO" "[Install python package ...]"
            check_python_version
            
            pushd ${SCRIPT_DIR}/python_package >/dev/null
            
            # Attempt to install the package using python3 -m pip (more reliable than pip3)
            install_result=$(python3 -m pip install . 2>&1)
            local exit_code=$?
            
            # Check for PEP 668 specific error
            if [[ "$install_result" == *"PEP 668"* ]]; then
                print_colored_v2 "WARNING" "PEP 668 externally-managed-environment error detected!"
                print_colored_v2 "WARNING" "Skipping Python package installation due to system restrictions."
                print_colored_v2 "HINT" "Please use --venv_path option or activate a virtual environment:"
                echo -e "${COLOR_BRIGHT_YELLOW_ON_BLACK}example)$ python3 -m venv ./venv-dxnn; $0 --venv_path ./venv-dxnn ${COLOR_RESET}"
                popd >/dev/null
                return 0  # Skip installation
            fi
            
            if [ $exit_code -eq 0 ]; then
                python3 -m pip list | grep dx-engine
                print_colored_v2 "INFO" "The Python package has been installed in the following Python version folder."
                echo -e "${COLOR_BRIGHT_YELLOW_ON_BLACK}  : $(python3 -m pip --version)${COLOR_RESET}"
                if [ ! -n "$VIRTUAL_ENV" ]; then
                    print_colored_v2 "HINT" "To install it in a Python virtual environment,"
                    print_colored_v2 "HINT" "you need to activate the virtual environment and then"
                    print_colored_v2 "HINT" "reinstall it by running 'python3 -m pip install .' from the python_package folder."
                fi
                print_colored_v2 "SUCCESS" "[Installed python package]"
                if [ -n "$venv_path" ]; then
                    print_colored_v2 "HINT" "To activate the virtual environment, run:"
                    echo -e "${COLOR_BRIGHT_YELLOW_ON_BLACK}  source ${venv_path}/bin/activate ${COLOR_RESET}"
                elif [ -n "$VIRTUAL_ENV" ]; then
                    print_colored_v2 "HINT" "To activate the virtual environment, run:"
                    echo -e "${COLOR_BRIGHT_YELLOW_ON_BLACK}  source ${VIRTUAL_ENV}/bin/activate ${COLOR_RESET}"
                fi
            else
                print_colored_v2 "FAIL" "Fail to install python package: $install_result"
                exit $exit_code
            fi
            popd >/dev/null
        else
            print_colored_v2 "WARNING" "Manually install the dxrt Python module on the target system."
        fi
    fi
}

manage_dxrt_service() {
    if [ $build_in_docker == "false" ]; then
        # start or restart dxrt system service (dxrtd)
        if [ $CROSS_COMPILE == "native" ]; then
            if [ $CMAKE_USE_SERVICE == "true" ]; then
                print_colored_v2 "INFO" "Service mode: on"

                # Create the service file
                if [ ! -f "/etc/systemd/system/dxrt.service" ]; then
                    sudo cp ./service/dxrt.service /etc/systemd/system/
                    if [ $? -eq 0 ]; then
                        print_colored_v2 "SUCCESS" "[Created dxrt.service file]"
                        # Reload systemd to recognize the change
                        sudo systemctl daemon-reload
                        print_colored_v2 "SUCCESS" "[Reloaded systemd daemon]"
                    else
                        print_colored_v2 "WARNING" "[Failed to create dxrt.service file]"
                    fi
                else
                    print_colored_v2 "INFO" "[dxrt.service file already exist]"
                fi

                # check to enabled dxrt service
                if [ $(sudo systemctl is-enabled dxrt.service) == "enabled" ]; then
                    print_colored_v2 "INFO" "[dxrt service already enabled]"
                else
                    print_colored_v2 "INFO" "[dxrt service not enabled]"

                    # enable dxrt service
                    DXRTD_LOG=$(sudo systemctl enable dxrt.service)
                    if [ $? -eq 0 ]; then
                        print_colored_v2 "SUCCESS" "[Enabled the dxrt service (dxrtd)]"
                    else
                        print_colored_v2 "WARNING" "[Failed to enable the dxrt service (dxrtd)]"
                    fi
                fi

                # check to start dxrt service
                if [ $(sudo systemctl is-active dxrt.service) == "active" ]; then
                    print_colored_v2 "INFO" "[dxrt service already started]"

                    # restart dxrt service
                    DXRTD_LOG=$(sudo systemctl restart dxrt.service)
                    if [ $? -eq 0 ]; then
                        print_colored_v2 "SUCCESS" "[Restarted the dxrt service (dxrtd)]"
                    else
                        print_colored_v2 "WARNING" "[Failed to restart the dxrt service (dxrtd)]"
                    fi
                else
                    print_colored_v2 "INFO" "[dxrt service not started]"

                    # start dxrt service
                    DXRTD_LOG=$(sudo systemctl start dxrt.service)
                    if [ $? -eq 0 ]; then
                        print_colored_v2 "SUCCESS" "[Started the dxrt service (dxrtd)]"
                    else
                        print_colored_v2 "WARNING" "[Failed to start the dxrt service (dxrtd)]"
                    fi
                fi
            else
                print_colored_v2 "INFO" "Service mode: off"

                # check to active dxrt service
                if [ $(sudo systemctl is-active dxrt.service) == "active" ]; then
                    print_colored_v2 "INFO" "[dxrt service started]"

                    # stop dxrt service
                    DXRTD_LOG=$(sudo systemctl stop dxrt.service)
                    if [ $? -eq 0 ]; then
                        DXRTD_LOG=$(sudo systemctl disable dxrt.service)
                        print_colored_v2 "SUCCESS" "[Stoped the dxrt service (dxrtd)]"
                    else
                        print_colored_v2 "FAIL" "[No registered the dxrt service (dxrtd)]"
                    fi
                else
                    print_colored_v2 "INFO" "[dxrt service not started]"
                fi

                # check to enabled dxrt service
                if [ $(sudo systemctl is-enabled dxrt.service) == "enabled" ]; then
                    print_colored_v2 "INFO" "[dxrt service enabled]"

                    # disable dxrt service
                    DXRTD_LOG=$(sudo systemctl disable dxrt.service)
                    if [ $? -eq 0 ]; then
                        print_colored_v2 "SUCCESS" "[Disabled the dxrt service (dxrtd)]"
                    else
                        print_colored_v2 "WARNING" "[Failed to disable the dxrt service (dxrtd)]"
                    fi
                else
                    print_colored_v2 "INFO" "[dxrt service not enabled]"
                fi
            fi
        else
            print_colored_v2 "WARNING" "Manually start the dxrtd service on the target system."
        fi # native compile

    fi # not build_in_docker
}

uninstall_dxrt() {
    if [ -d $build_dir ]; then
        pushd $build_dir >/dev/null
        sudo ninja uninstall && {
            print_colored_v2 "SUCCESS" "Uninstalled ninja build files"
        } || {
            return_code=$?
            print_colored_v2 "FAIL" "Failed to uninstall the ninja build files."
            exit $return_code
        }
        popd >/dev/null
    else
        print_colored_v2 "INFO" "No build directory found, skip to uninstall ninja build files."
    fi
}

uninstall_python_package() {
    print_colored_v2 "INFO" "[Uninstall python package ...]"

    # Attempt to uninstall the package using python3 -m pip (more reliable than pip3)
    uninstall_result=$(python3 -m pip uninstall -y dx-engine 2>&1)
    local exit_code=$?
    
    # Check for PEP 668 specific error
    if [[ "$uninstall_result" == *"PEP 668"* ]]; then
        print_colored_v2 "WARNING" "PEP 668 externally-managed-environment error detected!"
        print_colored_v2 "WARNING" "Skipping Python package uninstallation due to system restrictions."
        print_colored_v2 "HINT" "Please use --venv_path option or activate a virtual environment:"
        return 0  # Skip uninstallation
    fi
    
    if [ $exit_code -eq 0 ]; then
        print_colored_v2 "SUCCESS" "Uninstalled python package"
    else
        print_colored_v2 "FAIL" "Fail to uninstall python package: $uninstall_result"
        exit $exit_code
    fi
}

uninstall_dxrt_service() {
    # Disable and stop dxrt service, then remove service file
    if [ $CROSS_COMPILE == "native" ]; then
        print_colored_v2 "INFO" "Uninstalling dxrt service..."
        
        # Stop the service if it's running
        if systemctl is-active --quiet dxrt.service 2>/dev/null; then
            DXRTD_LOG=$(sudo systemctl stop dxrt.service)
            if [ $? -eq 0 ]; then
                print_colored_v2 "SUCCESS" "[Stopped dxrt service]"
            else
                print_colored_v2 "WARNING" "[Failed to stop dxrt service]"
            fi
        fi
        
        # Disable the service
        if systemctl is-enabled --quiet dxrt.service 2>/dev/null; then
            DXRTD_LOG=$(sudo systemctl disable dxrt.service)
            if [ $? -eq 0 ]; then
                print_colored_v2 "SUCCESS" "[Disabled dxrt service]"
            else
                print_colored_v2 "WARNING" "[Failed to disable dxrt service]"
            fi
        fi
        
        # Remove the service file
        if [ -f "/etc/systemd/system/dxrt.service" ]; then
            sudo rm -f /etc/systemd/system/dxrt.service
            if [ $? -eq 0 ]; then
                print_colored_v2 "SUCCESS" "[Removed dxrt.service file]"
                # Reload systemd to recognize the change
                sudo systemctl daemon-reload
                print_colored_v2 "SUCCESS" "[Reloaded systemd daemon]"
            else
                print_colored_v2 "WARNING" "[Failed to remove dxrt.service file]"
            fi
        else
            print_colored_v2 "INFO" "[dxrt.service file not found]"
        fi
        
        print_colored_v2 "SUCCESS" "[dxrt service uninstallation completed]"
    else
        print_colored_v2 "WARNING" "Manually uninstall the dxrt service on the target system."
    fi
}

activate_venv() {
    # Activate virtual environment if specified
    if [ -n "$venv_path" ]; then
        print_colored_v2 "INFO" "Activating virtual environment: ${venv_path}"
        . ${venv_path}/bin/activate || {
            return_code=$?
            print_colored_v2 "ERROR" "Failed to activate the virtual environment."
            exit $return_code
        }
    else
        print_colored_v2 "INFO" "No virtual environment specified."
    fi

    # Check if virtual environment is activated
    if [ -n "$VIRTUAL_ENV" ]; then
        print_colored_v2 "INFO" "Virtual environment detected: $VIRTUAL_ENV"
    else
        print_colored_v2 "WARNING" "No virtual environment detected"
    fi
}

build_mode() {
    print_colored_v2 "INFO" "Build mode"

    build_dxrt
    install_python_package
    manage_dxrt_service

    print_colored_v2 "SUCCESS" "Build mode completed"
}

uninstall_mode() {
    print_colored_v2 "INFO" "Uninstall mode"

    uninstall_dxrt
    uninstall_python_package
    uninstall_dxrt_service

    print_colored_v2 "SUCCESS" "Uninstall mode completed"
}

main() {
    activate_venv

    if [ "$uninstall" == "true" ]; then
        uninstall_mode
    else
        build_mode
    fi
}


setup_env

[ $# -gt 0 ] && \
while (( $# )); do
    case "$1" in
        --help)  help; exit 0;;
        --clean) clean_build=true; install_ort=$CMAKE_USE_ORT; shift;;
        --verbose) verbose=true; shift;;
        --type)
            shift
            build_type="${1,,}"
            shift;;
        --arch)
            shift
            target_arch=$1
            echo "Target architecture: $target_arch"
            shift;;
        --python_exec)
            shift
            python_exec=$1
            shift;;
        --venv_path)
            shift
            venv_path=$1
            shift;;
        --install)
            shift
            install=$1
            shift;;
        --uninstall) uninstall=true; shift;;
        --clang) clang=true; shift;;
        --docker) build_in_docker=true; shift;;
        --use_service_on) CMAKE_USE_SERVICE=true; shift;;
        --use_service_off) CMAKE_USE_SERVICE=false; shift;;
        --use_ort_on) CMAKE_USE_ORT=true; shift;;
        --use_ort_off) CMAKE_USE_ORT=false; shift;;
        *)
            help "error" "Invalid argument : $1"
            exit 1;;
    esac
done

check_cross_compile
main

exit 0