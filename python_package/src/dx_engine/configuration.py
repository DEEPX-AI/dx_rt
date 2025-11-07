#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import dx_engine.capi._pydxrt as C
from enum import IntEnum

class Configuration:

    # Class variable to store the singleton instance
    _instance = None

    # IntEnum class to define configuration items with explicit synchronization to C++
    class ITEM(IntEnum):
        DEBUG = 1
        PROFILER = 2
        SERVICE = 3
        DYNAMIC_CPU_THREAD = 4
        TASK_FLOW = 5
        SHOW_THROTTLING = 6
        SHOW_PROFILE = 7
        SHOW_MODEL_INFO = 8
        CUSTOM_INTRA_OP_THREADS = 9
        CUSTOM_INTER_OP_THREADS = 10   

    # IntEnum class to define attributes for configuration items with explicit synchronization to C++
    class ATTRIBUTE(IntEnum):
        PROFILER_SHOW_DATA = 1001
        PROFILER_SAVE_DATA = 1002
        CUSTOM_INTRA_OP_THREADS_NUM = 1003
        CUSTOM_INTER_OP_THREADS_NUM = 1004

    def __init__(self):
        self._instance: C.Configuration = C.Configuration.get_instance()

    def load_config_file(self, file_name: str):
        if not isinstance(file_name, str) or not file_name:
            raise ValueError("file_name must be a non-empty string")
        return C.configuration_load_config_file(self._instance, file_name)
    
    def set_enable(self, item: ITEM, enabled: bool):
        C.configuration_set_enable(self._instance, item, enabled)
        #print('set_enable')

    def set_attribute(self, item: ITEM, attrib: ATTRIBUTE, value: str):
        C.configuration_set_attribute(self._instance, item, attrib, value)

    def get_enable(self, item: ITEM) -> bool:
        return C.configuration_get_enable(self._instance, item)

    def get_attribute(self, item: ITEM, attrib: ATTRIBUTE) -> str:
        return C.configuration_get_attribute(self._instance, item, attrib)

    def get_version(self) -> str:
        return C.configuration_get_version(self._instance)

    def get_driver_version(self) -> str:
        return C.configuration_get_driver_version(self._instance)

    def get_pcie_driver_version(self) -> str:
        return C.configuration_get_pcie_driver_version(self._instance)

    def set_fw_config_with_json(self, json_file: str):
        C.configuration_set_fw_config_with_json(self._instance, json_file)
