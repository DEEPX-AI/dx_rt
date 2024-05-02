#!/bin/bash
# dependencies install script in host
pushd .

cmd=()
DX_SRC=$PWD
echo "DX_SRC the default one $DX_SRC"
target_arch=$(uname -p)  
install_opencv=false  
install_onnx=false  
install_dep=false   

function help()
{
    echo "./install.sh [ options ]"
    echo "    --help            show this help"
    echo "    --arch            target CPU architecture : [ x86_64, arm64, riscv64 ]"
    echo "    --dep             install dependencies : cmake, gcc, ninja, etc.."
    echo "    --opencv          (optional) install opencv pkg "
    echo "    --onnxruntime     (optional) show build commands"
}

function install_dep()
{
    cmake_version=3.14
    if [ "$install_dep" == true ]; then
        echo " Install dependence package tools "
        sudo apt-get -y install build-essential make zlib1g-dev libcurl4-openssl-dev wget tar zip
        if [ "$install_onnx" == true ]; then
            echo " Install CMake version 3.18.0, to build onnxruntime "
            cmake_version=3.18
        fi
        if ! test -e $DX_SRC/util; then 
            mkdir $DX_SRC/util
        fi
        cd $DX_SRC/util
        if ! test -e $DX_SRC/util/cmake-$cmake_version.0; then
            echo " Install CMake v$$cmake_version.0 "
            wget https://cmake.org/files/v$cmake_version/cmake-$cmake_version.0.tar.gz --no-check-certificate    
            tar xvf cmake-$cmake_version.0.tar.gz
        else
            echo " Already Exist CMake "
        fi
        cd cmake-$cmake_version.0
        ./bootstrap --system-curl
        make -j8
        sudo make install 
        sudo apt install ninja-build
        sudo apt-get -y install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
        sudo apt-get -y install gcc-riscv64-linux-gnu g++-riscv64-linux-gnu
    fi
}

function install_opencv()
{
    if [ "$install_opencv" == true ]; then
        echo " Install opencv dependent library "
        sudo apt install libjpeg-dev libtiff5-dev libpng-dev libavcodec-dev \
             libavformat-dev libswscale-dev libdc1394-22-dev libxvidcore-dev \
             libx264-dev libxine2-dev libv4l-dev v4l-utils libgstreamer1.0-dev \
             libgstreamer-plugins-base1.0-dev libgtk-3-dev
        
        if ! test -e $DX_SRC/util; then 
            mkdir $DX_SRC/util
        fi
        cd $DX_SRC/util
        if ! test -e $DX_SRC/util/opencv.4.5.5.zip; then
            wget -O opencv.4.5.5.zip https://github.com/opencv/opencv/archive/4.5.5.zip 
        fi
        if ! test -e $DX_SRC/util/opencv_contrib.4.5.5.zip; then
            wget -O opencv_contrib.4.5.5.zip https://github.com/opencv/opencv_contrib/archive/4.5.5.zip
        fi
        echo " unzip opencv & opencv contrib "
        if test -e $DX_SRC/util/opencv-4.5.5/; then
            sudo rm -rf $DX_SRC/util/opencv-4.5.5
        fi
        if test -e $DX_SRC/util/opencv_contrib-4.5.5/; then
            sudo rm -rf $DX_SRC/util/opencv_contrib-4.5.5
        fi
        unzip opencv.4.5.5.zip
        unzip opencv_contrib.4.5.5.zip
        cd opencv-4.5.5
        mkdir build_$target_arch
        cd build_$target_arch
        cmake \
        -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.5.5/modules \
        -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=$DX_SRC/extern/$target_arch \
        -D CMAKE_TOOLCHAIN_FILE=$DX_SRC/cmake/toolchain.$target_arch.cmake \
        -D BUILD_LIST=core,imgproc,imgcodecs,calib3d,features2d,flann,highgui,videoio \
        -D WITH_TBB=ON -D WITH_IPP=OFF -D WITH_1394=OFF \
        -D BUILD_WITH_DEBUG_INFO=OFF -D BUILD_DOCS=OFF \
        -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON \
        -D BUILD_EXAMPLES=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF \
        -D WITH_QT=OFF -D WITH_GTK=ON -D WITH_OPENGL=ON \
        -D WITH_V4L=ON -D WITH_FFMPEG=ON -D WITH_XINE=ON -D BUILD_NEW_PYTHON_SUPPORT=ON \
        -D OPENCV_GENERATE_PKGCONFIG=ON -D WITH_CUDA=OFF ../
        make -j8
        sudo make install
    fi
}

function install_onnx()
{
    if [ "$install_onnx" == true ]; then
        echo " Install ONNX-Runtime API " 
        if ! test -e $DX_SRC/util; then 
            mkdir $DX_SRC/util
        fi
        cd $DX_SRC/util
        if ! test -e $DX_SRC/util/onnxruntime-linux-$target_arch-1.13.1.tgz; then
        # get onnxruntime source code release version 1.13.1
            wget -O onnxruntime-linux-$target_arch-1.13.1.tgz \
            https://github.com/microsoft/onnxruntime/releases/download/v1.13.1/onnxruntime-linux-x64-1.13.1.tgz
        fi
        mkdir -p onnxruntime_$target_arch
        tar -xvzf onnxruntime-linux-$target_arch-1.13.1.tgz -C onnxruntime_$target_arch --strip-components 1
        sudo cp -r onnxruntime_$target_arch /usr/local/
        echo /usr/local/onnxruntime_$target_arch/lib/ > onnxruntime_$target_arch.conf
        sudo cp onnxruntime_$target_arch.conf /etc/ld.so.conf.d/
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/onnxruntime_$target_arch/lib/
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
        --opencv) install_opencv=true; shift;;     
        --onnxruntime) install_onnx=true; shift;;
        *)       echo "Invalid argument : " $1 ; help; exit 1;;
    esac
done

install_dep
install_opencv
install_onnx

cd $DX_SRC

popd
