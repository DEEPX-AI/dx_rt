#!/bin/bash
set -e

TARGET_DIR=../../lib/npuif

# NPU_VARIANT=$1

# if [ $# -lt 1 ]; then
#     echo "Error."
#     echo "    ./$0 <NPU variant>"
#     echo "    Example: ./$0 G_MAC4K_2MB_AIOT_RTL"
#     exit 2
# elif [ ! -f $1.xlsx ]; then
#     echo "Error: $1.xlsx doesn't exist."
#     exit 1
# fi

# python3 npu_api_gen.py $1.xlsx && rm -rf $TARGET_DIR/$NPU_VARIANT && cp -r ./$NPU_VARIANT $TARGET_DIR/


python3 npu_api_gen.py DX_M1.xlsx && rm -rf $TARGET_DIR/DX_M1 && cp -r ./DX_M1 $TARGET_DIR/
python3 npu_api_gen.py DX_M1A.xlsx && rm -rf $TARGET_DIR/DX_M1A && cp -r ./DX_M1A $TARGET_DIR/
python3 npu_api_gen.py DX_L1.xlsx && rm -rf $TARGET_DIR/DX_L1 && cp -r ./DX_L1 $TARGET_DIR/
python3 npu_api_gen.py DX_L2.xlsx && rm -rf $TARGET_DIR/DX_L2 && cp -r ./DX_L2 $TARGET_DIR/
