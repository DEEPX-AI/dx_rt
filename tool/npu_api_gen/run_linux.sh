#!/bin/bash
set -e

#TARGET_DIR=../../driver/draft
TARGET_DIR=../../driver/unified

rm -rf $TARGET_DIR/npu_reg*.h

python3 npu_api_gen_linux.py DX_L1.xlsx && cp -r ./DX_L1/linux/* $TARGET_DIR/
python3 npu_api_gen_linux.py DX_L3.xlsx && cp -r ./DX_L3/linux/* $TARGET_DIR/
