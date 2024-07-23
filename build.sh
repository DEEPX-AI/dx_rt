#!/bin/bash
dxrt_ver=`cat release.ver`

function help()
{
    echo "./build.sh [ options ]"
    echo "    --help     show this help"
    echo "    --clean    clean build"
    echo "    --verbose  show build commands"
    echo "    --type     cmake build type : [ Release, Debug, RelWithDebInfo ]"
    echo "    --arch     target CPU architecture : [ x86_64, aarch64, riscv64 ]"
    echo "    --install  install path"
    echo "    --uninstall  uninstall dx-rt files"
}

# cmake command
cmd=()
clean_build=false
verbose=false
build_type=release
host_arch=$(uname -m)
target_arch=$(uname -m)
build_mode="Release Build"
install=""
uninstall=false
[ $# -gt 0 ] && \
while (( $# )); do
    case "$1" in
        --help)  help; exit 0;;
        --clean) clean_build=true; shift;;
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
        *)       echo "Invalid argument : " $1 ; help; exit 1;;
    esac
done

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
cmd+=(-DPython_EXECUTABLE=$(which python))

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
