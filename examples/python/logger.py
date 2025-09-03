#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import sys
from enum import IntEnum

class LogLevel(IntEnum):
    NONE = 0
    ERROR = 1
    INFO = 2
    DEBUG = 3
    
class Logger:
    _instance = None
    _level = LogLevel.INFO
    
    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(Logger, cls).__new__(cls)
        return cls._instance
    
    def set_level(self, level: LogLevel):
        self._level = level
    
    def get_level(self) -> LogLevel:
        return self._level
    
    def error(self, msg: str):
        if self._level >= LogLevel.ERROR:
            print(f"[ERROR] {msg}", file=sys.stderr)

    def info(self, msg: str):
        if self._level >= LogLevel.INFO:
            print(f"[INFO] {msg}")

    def debug(self, msg: str):
        if self._level >= LogLevel.DEBUG:
            print(f"[DEBUG] {msg}")