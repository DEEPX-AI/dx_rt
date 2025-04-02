from typing import List, Any
import numpy as np
from enum import Enum

import dx_engine.capi._pydxrt as C
from dx_engine.dtype import NumpyDataTypeMapper
from dx_engine.utils import ensure_contiguous


class InferenceOption:

    class BOUND_OPTION(Enum):
        NPU_ALL = 0
        NPU_0 = 1
        NPU_1 = 2
        NPU_2 = 3

    def __init__(
        self,
    ) -> None:
        self.instance = C.InferenceOption()

    def set_use_ort(self, use_ort):
        C.inference_option_set_use_ort(self.instance, use_ort)

    def get_use_ort(self):
        return C.inference_option_get_use_ort(self.instance)
    
    def set_bound_option(self, boundOption):
        C.inference_option_set_bound_option(self.instance, boundOption.value)

    def get_bound_option(self):
        return self.BOUND_OPTION(C.inference_option_get_bound_option(self.instance))
    
    def set_devices(self, devices):
        C.inference_option_set_devices(self.instance, devices)

    def get_devices(self):
        return C.inference_option_get_devices(self.instance)

    