#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

from typing import List, Any, Optional
import numpy as np
from enum import Enum

import dx_engine.capi._pydxrt as C

class InferenceOption:
    """
    Configuration options for the InferenceEngine.

    This class wraps the C++ InferenceOption struct, providing a Pythonic
    interface to its members.
    """

    class BOUND_OPTION(Enum):
        """
        Defines how NPU cores are bound or utilized for inference.
        The values should correspond to the C++ enum/integer values.
        """
        NPU_ALL = 0
        NPU_0 = 1
        NPU_1 = 2
        NPU_2 = 3
        NPU_01 = 4
        NPU_12 = 5 
        NPU_02 = 6 

    def __init__(self) -> None:
        """Initializes a new InferenceOption object with default C++ values."""
        self.instance: C.InferenceOption = C.InferenceOption()

    @property
    def use_ort(self) -> bool:
        """Gets or sets whether to use ONNX Runtime for CPU tasks."""
        return self.instance.useORT

    @use_ort.setter
    def use_ort(self, value: bool) -> None:
        if not isinstance(value, bool):
            raise TypeError("use_ort must be a boolean value.")
        self.instance.useORT = value

    @property
    def bound_option(self) -> BOUND_OPTION:
        """
        Gets or sets the NPU core binding option.
        Uses the BOUND_OPTION enum for clarity.
        """
        try:
            return self.BOUND_OPTION(self.instance.boundOption)
        except ValueError:
            raise ValueError(
                f"Invalid boundOption value {self.instance.boundOption} received from C++. "
                f"Ensure it's defined in InferenceOption.BOUND_OPTION enum."
            )

    @bound_option.setter
    def bound_option(self, value: BOUND_OPTION) -> None:
        if not isinstance(value, self.BOUND_OPTION):
            raise TypeError("bound_option must be an instance of InferenceOption.BOUND_OPTION.")
        self.instance.boundOption = value.value

    @property
    def devices(self) -> List[int]:
        """
        Gets or sets the list of device IDs to be used for inference.
        The C++ `devices` member (std::vector<int>) is exposed as a Python list.
        """
        return self.instance.devices

    @devices.setter
    def devices(self, value: List[int]) -> None:
        if not isinstance(value, list) or not all(isinstance(i, int) for i in value):
            raise TypeError("devices must be a list of integers.")
        self.instance.devices = value

    def __repr__(self) -> str:
        return (f"InferenceOption(use_ort={self.use_ort}, "
                f"bound_option={self.bound_option.name if self.bound_option else 'None'}, "
                f"devices={self.devices})")
    
    def set_use_ort(self, use_ort):
        if not isinstance(use_ort, bool):
            raise TypeError("use_ort must be a boolean value.")
        self.instance.useORT = use_ort

    def get_use_ort(self):
        return self.instance.useORT
    
    def set_bound_option(self, boundOption):
        if not isinstance(boundOption, self.BOUND_OPTION):
            raise TypeError("bound_option must be an instance of InferenceOption.BOUND_OPTION.")
        self.instance.boundOption = boundOption.value

    def get_bound_option(self):
        return self.BOUND_OPTION(self.instance.boundOption)
    
    def set_devices(self, devices):
        if not isinstance(devices, list) or not all(isinstance(i, int) for i in devices):
            raise TypeError("devices must be a list of integers.")
        self.instance.devices = devices

    def get_devices(self):
        return self.instance.devices

