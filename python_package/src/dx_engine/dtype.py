#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import numpy as np

class NumpyDataTypeMapper:
    NONE_TYPE = None
    UINT8 = np.uint8
    UINT16 = np.uint16
    UINT32 = np.uint32
    UINT64 = np.uint64
    INT8 = np.int8
    INT16 = np.int16
    INT32 = np.int32
    INT64 = np.int64
    FLOAT = np.float32 

    BBOX = "BBOX"
    FACE = "FACE"
    POSE = "POSE"

    MAX_TYPE = None  

    @classmethod
    def from_string(cls, dtype_str: str):
        dtype_str = dtype_str.upper()
        if hasattr(cls, dtype_str):
            return getattr(cls, dtype_str)
        raise ValueError(f"Unknown data type string: {dtype_str}")