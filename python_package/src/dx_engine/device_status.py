#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import threading
import os

import dx_engine.capi._pydxrt as C

class DeviceStatus:

    # Class variable to store the singleton instance
    _instance = None
    

    def __init__(self):
        #self._instance: C.DeviceStatus = C.DeviceStatus.get_current_status(deviceId)
        pass

    @classmethod
    def get_current_status(cls, deviceId : int) -> object:
        devStatus = DeviceStatus()
        devStatus._instance = C.DeviceStatus.get_current_status(deviceId)
        return devStatus

    @classmethod
    def get_device_count(cls) -> int:
        return C.DeviceStatus.get_device_count()

    def get_temperature(self, ch : int) -> int:
        return C.device_status_get_temperature(self._instance, ch)

    def get_id(self) -> int:
        return C.device_status_get_id(self._instance)
        
    def get_npu_voltage(self, ch : int) -> int:
        return C.device_status_get_npu_voltage(self._instance, ch)

    def get_npu_clock(self, ch : int) -> int:
        return C.device_status_get_npu_clock(self._instance, ch)
    
