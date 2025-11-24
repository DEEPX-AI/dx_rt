#!/bin/bash



rm -rf build cli bitmatch src test
cp -rv ../python_package/* .
cp  cross_files/pyproject.toml .
cp cross_files/CMakeLists.txt src/dx_engine/capi
cp cross_files/toolchain.cmake src/dx_engine/capi
pushd src/dx_engine/capi
rm -rf CMakeCache.txt CMakeFiles
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake .
make
popd

pip wheel .


