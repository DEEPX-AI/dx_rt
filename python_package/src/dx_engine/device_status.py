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
        # self._instance: C.DeviceStatus = C.DeviceStatus.get_current_status(deviceId)
        pass

    @classmethod
    def get_current_status(cls, deviceId: int) -> object:  # NOSONAR
        devStatus = DeviceStatus()  # NOSONAR
        devStatus._instance = C.DeviceStatus.get_current_status(deviceId)
        return devStatus

    @classmethod
    def get_device_count(cls) -> int:
        return C.DeviceStatus.get_device_count()

    def get_temperature(self, ch: int) -> int:
        '''Get the temperature of the device for a specific channel.

        Returns INT16_MIN(0xFFFF8000, -32768) if the channel is invalid.
        '''
        return C.device_status_get_temperature(self._instance, ch)

    def get_id(self) -> int:
        return C.device_status_get_id(self._instance)

    def get_npu_voltage(self, ch: int) -> int:
        return C.device_status_get_npu_voltage(self._instance, ch)

    def get_npu_clock(self, ch: int) -> int:
        return C.device_status_get_npu_clock(self._instance, ch)

    def get_core_utilization(self, core_id: int) -> float:
        '''Get the utilization of a specific NPU core (0.0~100.0%).

        Returns -1.0 if core_id is out of range.
        '''
        return C.device_status_get_core_utilization(self._instance, core_id)

    def get_memory_used(self) -> int:
        '''Get the amount of NPU DRAM currently in use (bytes).'''
        return C.device_status_get_memory_used(self._instance)

    def get_memory_free(self) -> int:
        '''Get the amount of free NPU DRAM (bytes).'''
        return C.device_status_get_memory_free(self._instance)

    def is_valid(self) -> bool:
        '''Returns True if the device status data is valid (up-to-date).

        False means the monitoring service was not running and data may be stale.
        '''
        return C.device_status_is_valid(self._instance)

    def get_driver_version(self) -> str:
        '''Get the runtime driver version string (e.g., "1.2.3").'''
        return C.device_status_get_driver_version(self._instance)
