#!/bin/bash
SCRIPT_DIR=$(realpath "$(dirname "$0")")

# color env settings
source "${SCRIPT_DIR}/scripts/color_env.sh"

# dependencies install script in host
pushd .
cmd=()
DX_SRC_DIR=$PWD
target_arch=$(uname -m)  
install_onnx=false  
install_dep=false  
python_version=""
venv_path=""

function help() {
    echo -e "Usage: ${COLOR_CYAN}$0 [OPTIONS]${COLOR_RESET}"
    echo -e "Install necessary components and libraries for the project."
    echo -e ""
    echo -e "${COLOR_BOLD}Options:${COLOR_RESET}"
    echo -e "  ${COLOR_GREEN}--help${COLOR_RESET}                       Display this help message and exit."
    echo -e "  ${COLOR_GREEN}--arch <ARCH>${COLOR_RESET}                Specify the target CPU architecture. Valid options: [x86_64, aarch64]."
    echo -e "  ${COLOR_GREEN}--dep${COLOR_RESET}                        Install core dependencies such as cmake, gcc, ninja, etc and python3."
    echo -e "  ${COLOR_GREEN}--onnxruntime${COLOR_RESET}                (Optional) Install the ONNX Runtime library."
    echo -e "  ${COLOR_GREEN}--all${COLOR_RESET}                        Install all dependencies and the ONNX Runtime library."
    echo -e ""
    echo -e "  ${COLOR_GREEN}--python_version <VERSION>${COLOR_RESET}   Specify the Python version to install (e.g., 3.10.4)."
    echo -e "                                 * Minimum supported version: ${MIN_PY_VERSION}."
    echo -e "                                 * If not specified:"
    echo -e "                                     - For Ubuntu 20.04+, the OS default Python 3 will be used."
    echo -e "                                     - For Ubuntu 18.04, Python ${MIN_PY_VERSION} will be source-built."
    echo -e "  ${COLOR_GREEN}--venv_path <PATH>${COLOR_RESET}          Specify the path for the virtual environment."
    echo -e "                                 * If this option is omitted, no virtual environment will be created."
    echo -e "  ${COLOR_GREEN}--help${COLOR_RESET}                      Display this help message and exit."
    echo -e ""
    echo -e "${COLOR_BOLD}Examples:${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --arch aarch64 --dep${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --all --python_version 3.10.4${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --onnxruntime --venv_path /opt/my_project_venv${COLOR_RESET}"
    echo -e ""
    echo -e "  ${COLOR_YELLOW}$0 --python_version 3.10.4 --venv_path /opt/my_venv${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --python_version 3.9.18  # Installs Python, but no venv${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --venv_path ./venv-dxnn # Installs default Python, creates venv${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 # Installs default Python, but no venv${COLOR_RESET}"
    echo -e ""

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

function compare_version() 
{
    awk -v n1="$1" -v n2="$2" 'BEGIN { if (n1 >= n2) exit 0; else exit 1; }'
}

function install_dep()
{
    cmake_version_required=3.14
    install_cmake=false
    if [ "$install_dep" == true ]; then
        echo " Install dependence package tools "
	    sudo apt-get update
        sudo apt-get -y install build-essential make zlib1g-dev libcurl4-openssl-dev wget tar zip cmake git
        
        echo ""
        echo " Install python libraries" 
        sudo apt-get -y install python3-dev python3-setuptools python3-pip python3-tk python3-lxml python3-six

        # install ncurses
        install_ncurses

        cmake_version=$(cmake --version |grep -oP "\d+\.\d+\.\d+")
        if compare_version "$cmake_version" "$cmake_version_required"; then
            install_cmake=false
        else
            install_cmake=true
        fi
        if [ "$install_cmake" == true ]; then
            if ! test -e $DX_SRC_DIR/util; then 
                mkdir $DX_SRC_DIR/util
            fi
            cd $DX_SRC_DIR/util
            if ! test -e $DX_SRC_DIR/util/cmake-$cmake_version_required.0; then
                echo " Install CMake v$cmake_version_required.0 "
                wget https://cmake.org/files/v$cmake_version_required/cmake-$cmake_version_required.0.tar.gz --no-check-certificate 
                tar xvf cmake-$cmake_version_required.0.tar.gz
            else
                echo " Already Exist CMake "
            fi
            cd cmake-$cmake_version_required.0
            ./bootstrap --system-curl
            make -j$(nproc)
            sudo make install 
        fi
        sudo apt install ninja-build
        if [ $(uname -m) == "x86_64" ]; then
            sudo apt-get -y install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
        fi
    fi

    install_python
}

function install_onnx()
{
    if [ "$target_arch" == "riscv64" ] && [ "$install_onnx" == true ]; then
        echo "The riscv64 architecture is not yet supported ONNXRUNTIME."
        install_onnx=false
    fi
    onnxruntime_arch="x64"
    if [ "$target_arch" == "x86_64" ]; then
        onnxruntime_arch="x64"
    elif [ "$target_arch" == "aarch64" ]; then
        onnxruntime_arch="aarch64"
    fi
    if [ "$install_onnx" == true ]; then
        echo " Install ONNX-Runtime API " 
        ONNX_VERSION="1.20.1"
        ONNX_ARCH_FOLDER='onnxruntime_'$onnxruntime_arch
        ONNX_PACKAGE_NAME='onnxruntime-linux-'$onnxruntime_arch'-'$ONNX_VERSION'.tgz'
        mkdir -p $DX_SRC_DIR/util
        cd $DX_SRC_DIR/util
        rm -rf $DX_SRC_DIR/util/$ONNX_ARCH_FOLDER
        
        # if onnx-package-file exists
        if [ -f "$DX_SRC_DIR/util/$ONNX_PACKAGE_NAME" ]; then
            echo "Already downloaded util/$ONNX_PACKAGE_NAME file"
        else
            wget https://github.com/microsoft/onnxruntime/releases/download/v$ONNX_VERSION/$ONNX_PACKAGE_NAME --no-check-certificate --backups=0
        fi

        mkdir -p $ONNX_ARCH_FOLDER
        tar -zxvf $ONNX_PACKAGE_NAME -C $ONNX_ARCH_FOLDER --strip-components=1     
        
        if [ $(uname -m) != "$target_arch" ]; then
            echo " onnxruntime install library for Cross Compilation (host : $(uname -m), target : $target_arch)"
        else
            echo ""
            echo " onnxruntime install library for your local system "
            sudo cp -a $ONNX_ARCH_FOLDER/* /usr/local/
            echo " copy onnxruntime libraries to your local system (/usr/local)"
            echo ""
        fi
        sudo ldconfig
    fi
}

function install_ncurses()
{
    echo ""
    echo " Install ncurses libraries"

    if [ $(uname -m) != "$target_arch" ]; then
        echo "Not supported for Cross Compilation (host : $(uname -m), target : $target_arch)"
        echo ""
    else
        sudo apt-get -y install libncurses5-dev libncursesw5-dev
    fi
}

function install_python()
{
    
    python_version_arg=""
    if [ -n "$python_version" ]; then
        python_version_arg="--python_version=$python_version"
    fi

    venv_path_arg=""
    if [ -n "$venv_path" ]; then
        venv_path_arg="--venv_path=$venv_path"
    fi

    ${SCRIPT_DIR}/scripts/install_python_and_venv.sh $python_version_arg $venv_path_arg
    if [ $? -ne 0 ]; then
        echo -e "${TAG_ERROR} Python and Virual environment setup failed. Exiting."
        exit 1
    fi
}

[ $# -gt 0 ] && \
while (( $# )); do
    case "$1" in
        --help) help; exit 0;;      
        --arch)  
            shift
            target_arch=$1
            shift;;       
        --dep) install_dep=true; shift;;        
        --onnxruntime) install_onnx=true; shift;;
        --all) install_onnx=true;install_dep=true; shift;;  
        --python_version)
            shift
            python_version=$1
            shift;;
        --venv_path)
            shift
            venv_path=$1
            shift;;
        *)
            help "error" "Invalid argument : $1"
            exit 1;;
    esac
done

if [ $target_arch == "arm64" ]; then
    target_arch=aarch64
fi

install_dep
install_onnx

popd
