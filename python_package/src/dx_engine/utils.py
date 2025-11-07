#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import numpy as np
import warnings
from typing import Sequence, Union
import dx_engine.capi._pydxrt as C

def parse_model(model_path, options=None) -> int:
    """
    Parse a model file and display information about it.
    
    Args:
        model_path (str): Path to the .dxnn model file
        options (dict, optional): Parsing options containing:
            - verbose (bool): Show detailed task dependencies and memory usage
            - json_extract (bool): Extract JSON binary data to files  
            - no_color (bool): Disable color output
            - output_file (str): Save output to file (without color codes)
    
    Returns:
        int: 0 if successful, -1 if failed
    """
    if options is None:
        return C.parse_model(model_path)
    else:
        return C.parse_model(model_path, options)

def ensure_contiguous(
    data: Union[np.ndarray, Sequence]
) -> Union[np.ndarray, list]:
    if isinstance(data, np.ndarray):
        if not data.flags['C_CONTIGUOUS']:
            warnings.warn(
                f"ndarray(shape={data.shape}, dtype={data.dtype}) is not contiguous; converting.",
                UserWarning
            )
            try:
                return np.ascontiguousarray(data)
            except MemoryError:
                raise MemoryError(
                    f"Unable to allocate contiguous array for shape {data.shape}"
                )
        return data

    if isinstance(data, (list, tuple)):
        converted = [ensure_contiguous(elem) for elem in data]
        return type(data)(converted)

    raise TypeError(f"Unsupported type for ensure_contiguous: {type(data)}")