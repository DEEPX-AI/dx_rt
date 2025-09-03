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

def parse_model(model_path) -> str:
    return C.parse_model(model_path)

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