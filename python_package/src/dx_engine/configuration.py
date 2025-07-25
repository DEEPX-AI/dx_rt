import threading
import os

import dx_engine.capi._pydxrt as C

class Configuration:

    # Class variable to store the singleton instance
    _instance = None

    # Enum-like class to define configuration items
    class ITEM:
        DEBUG = 1
        PROFILER = 2
        SERVICE = 3
        DYNAMIC_CPU_THREAD = 4
        TASK_FLOW = 5
        SHOW_THROTTLING = 6
        SHOW_PROFILE = 7
        SHOW_MODEL_INFO = 8

    # Enum-like class to define attributes for configuration items
    class ATTRIBUTE:
        PROFILER_SHOW_DATA = 1001
        PROFILER_SAVE_DATA = 1002

    def __init__(self):
        self._instance: C.Configuration = C.Configuration.get_instance()

    def load_config_file(self, file_name: str):
        print('load_config_file')
    
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
