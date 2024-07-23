#!/bin/bash
# dependencies install script in host
pushd .
cmd=()
DX_SRC_DIR=$PWD
target_arch=$(uname -m)  
install_onnx=false  
install_dep=false  

function help()
{
    echo "./install.sh"
    echo "    --help            show this help"
    echo "    --arch            target CPU architecture : [ x86_64, aarch64, riscv64 ]"
    echo "    --dep             install dependencies : cmake, gcc, ninja, etc.."
    echo "    --onnxruntime     (optional) install onnxruntime library"
    echo "    --all             install dependencies & onnxruntime library"
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
        sudo apt-get -y install build-essential make zlib1g-dev libcurl4-openssl-dev wget tar zip cmake
        echo ""
        echo " Install python libraries" 
        sudo apt-get -y install python3-dev python3-distutils python3-pip python3-tk python3-lxml python3-six
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
            if ! test -e $DX_SRC_DIR/util/cmake-$cmake_version.0; then
                echo " Install CMake v$$cmake_version.0 "
                wget https://cmake.org/files/v$cmake_version/cmake-$cmake_version.0.tar.gz --no-check-certificate    
                tar xvf cmake-$cmake_version.0.tar.gz
            else
                echo " Already Exist CMake "
            fi
            cd cmake-$cmake_version.0
            ./bootstrap --system-curl
            make -j$(nproc)
            sudo make install 
        fi
        sudo apt install ninja-build
        sudo apt-get -y install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
        sudo apt-get -y install gcc-riscv64-linux-gnu g++-riscv64-linux-gnu
    fi
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
        test $(mkdir $DX_SRC_DIR/util)
        cd $DX_SRC_DIR/util
        test $(sudo rm -r $DX_SRC_DIR/util/onnxruntime_$target_arch)
        wget https://github.com/microsoft/onnxruntime/releases/download/v1.12.0/onnxruntime-linux-$onnxruntime_arch-1.12.0.tgz
        mkdir onnxruntime_$target_arch
        tar -zxvf onnxruntime-linux-$onnxruntime_arch-1.12.0.tgz -C onnxruntime_$target_arch --strip-components=1 
        if [ $(uname -m) != "$target_arch" ]; then
            if [ $(uname -m) == "aarch64" ] && [ $target_arch == "aarch64" ]; then  
                sudo cp -a onnxruntime_$target_arch/* /usr/local/
            else
                echo " onnxruntime install library for Cross Compilation (host : $(uname -m), target : $target_arch)"
            fi
        else
            echo ""
            echo " onnxruntime install library for your local system "
            sudo cp -a onnxruntime_$target_arch/* /usr/local/
            echo " copy onnxruntiem libraries to your local system (/usr/local)"
            echo ""
        fi
        sudo ldconfig
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
        *)       echo "Invalid argument : " $1 ; help; exit 1;;
    esac
done

if [ $target_arch == "arm64" ]; then
    target_arch=aarch64
fi

install_dep
install_onnx

popd
