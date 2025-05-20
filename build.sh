#!/bin/bash
dxrt_ver=`cat release.ver`

function help()
{
    echo "./build.sh [ options ]"
    echo "    --help       show this help"
    echo "    --clean      clean build"
    echo "    --verbose    show build commands"
    echo "    --type       cmake build type : [ Release, Debug, RelWithDebInfo ]"
    echo "    --arch       target CPU architecture : [ x86_64, aarch64, riscv64 ]"
    echo "    --install    install path"
    echo "    --uninstall  uninstall dx-rt files"
    echo "    --clang      compiler using clang"
}


# parsing dxrt.cfg.cmake
CMAKE_USE_ORT=false
CMAKE_USE_SERVICE=true
CMAKE_USE_PYTHON=true
parse_cmake_options() {
  local file="$1"
  while IFS= read -r line; do
    parsed=$(echo "$line" | sed -E 's/option\(([^)]+)\)/\1/' )
    IFS= read -r -a parts <<< "$(echo "$parsed" | sed -E 's/[[:space:]]+/ /g')"
    var_name=$(echo "$parsed" | awk -F'"' '{print $1}' | xargs | cut -d' ' -f1)
    default_val=$(echo "$parsed" | awk -F'"' '{print $2; print $3}' | tail -n1 | xargs)
    if [ $var_name == 'USE_ORT' ]; then
        if [ $default_val == 'ON' ]; then
            CMAKE_USE_ORT=true
        else
            CMAKE_USE_ORT=false
        fi
        echo CMAKE_USE_ORT=$CMAKE_USE_ORT
    elif [ $var_name == 'USE_SERVICE' ]; then
        if [ $default_val == 'ON' ]; then
            CMAKE_USE_SERVICE=true
        else
            CMAKE_USE_SERVICE=false
        fi
        echo CMAKE_USE_SERVICE=$CMAKE_USE_SERVICE
    elif [ $var_name == 'USE_PYTHON' ]; then
        if [ $default_val == 'ON' ]; then
            CMAKE_USE_PYTHON=true
        else
            CMAKE_USE_PYTHON=false
        fi
        echo CMAKE_USE_PYTHON=$CMAKE_USE_PYTHON
    fi
  done < "$file"
}

# read cmake/dxrt.cfg.cmake file
CMAKE_FILE="cmake/dxrt.cfg.cmake"
if [ -f "$CMAKE_FILE" ]; then
  parse_cmake_options "$CMAKE_FILE"
else
  echo "Error: File '$CMAKE_FILE' not found."
fi

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
            shift;;
        --install)
            shift
            install=$1
            shift;;
        --uninstall) uninstall=true; shift;;
        --clang) clang=true; shift;;
        *)       echo "Invalid argument : " $1 ; help; exit 1;;
    esac
done

# install onnx runtime
if [ $install_ort == "true" ]; then
    ./install.sh --onnxruntime --arch $target_arch
fi

if [ $target_arch == "arm64" ]; then
    target_arch=aarch64
fi

if [ $target_arch == $host_arch ]; then
    if [ -z $install ]; then
        install=/usr/local
    fi
fi
build_dir=build_"$target_arch"
out_dir=bin

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
cmd+=(-DPython_EXECUTABLE=$(which python3))

echo cmake args : ${cmd[@]}
[ $clean_build == "true" ] && sudo rm -rf $build_dir

if [ $uninstall == "true" ]; then
    cd $build_dir
    sudo ninja uninstall
else
    mkdir $build_dir
    mkdir $out_dir
    sudo rm -rf $build_dir/bin
    cd $build_dir
    cmake .. ${cmd[@]}
    ninja    
    sudo ninja install
fi

return_code=$?
#echo $return_code

if [ $return_code -ne 0 ]; then
  exit $return_code
fi

# install python package
if [ $CMAKE_USE_PYTHON == "true" ]; then
    echo '[Install python package ...]'
    cd ../python_package
    pip install .
    cd ..
    echo '[Installed python package]'
fi

# start or restart dxrt system service (dxrtd)
if [ $CMAKE_USE_SERVICE == "true" ]; then
    echo '[Restarted the dxrt service (dxrtd)]'
    sudo systemctl restart dxrt.service
    if [ $? -ne 0 ]; then
        echo '[Registration of the dxrt service (dxrtd) - start]'
        sudo cp ./service/dxrt.service /etc/systemd/system/
        sudo systemctl start dxrt.service
        sudo systemctl enable dxrt.service
        echo '[Registration of the dxrt service (dxrtd) - end]'
    fi
else
    echo '[Stoped the dxrt service (dxrtd)]'
    sudo systemctl stop dxrt.service
    sudo systemctl disable dxrt.service
    if [ $? -ne 0 ]; then
        echo '[No registered the dxrt service (dxrtd)]'
    fi
fi
