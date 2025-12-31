#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import enum
from typing import Callable, Optional
import dx_engine.capi._pydxrt as C
from enum import IntEnum

class RuntimeEventDispatcher:
    """
    Singleton class for dispatching and handling runtime events from the DX-RT system.
    
    This class provides a centralized event dispatching mechanism for runtime events
    such as device errors, warnings, and notifications. It supports custom event handlers
    and automatic logging of events with different severity levels.
    """

    # Class variable to store the singleton instance
    _instance = None
    

    # Event severity levels for categorizing runtime events.
    class LEVEL(IntEnum):
        INFO = 1        # Informational messages for normal operation events
        WARNING = 2     # Warning messages for potential issues that don't stop execution
        ERROR = 3       # Error messages for recoverable failures
        CRITICAL = 4    # Critical errors that may cause system instability
    

    # Event type categories for classifying the source of events.
    class TYPE(IntEnum):
        DEVICE_CORE = 1000    # Events related to NPU core operations
        DEVICE_STATUS = 1001         # Device status change events
        DEVICE_IO = 1002             # Input/Output operation events
        DEVICE_MEMORY = 1003         # Memory management events
        UNKNOWN = 1004              # Unknown or unclassified event types
    
    # Specific event codes for identifying the exact nature of events.
    class CODE(IntEnum):
        WRITE_INPUT = 2000     # Input data write operation event
        READ_OUTPUT = 2001            # Output data read operation event
        MEMORY_OVERFLOW = 2002        # Memory overflow or capacity exceeded
        MEMORY_ALLOCATION = 2003      # Memory allocation failure or issue
        DEVICE_EVENT = 2004           # General device event notification
        RECOVERY_OCCURRED = 2005      # Device recovery action taken
        TIMEOUT_OCCURRED = 2006       # Operation timeout event
        THROTTLING_NOTICE = 2007      # Device throttling notification
        THROTTLING_EMERGENCY = 2008   # Device throttling emergency notification
        UNKNOWN = 2009                 # Unknown or unclassified event code
   

    def __init__(self):
        """
        Private constructor to enforce singleton pattern.
        Gets the singleton instance from C++ RuntimeEventDispatcher.
        """
        self._instance: C.RuntimeEventDispatcher = C.RuntimeEventDispatcher.get_instance()

    def dispatch_event(self, level: LEVEL, type: TYPE, code: CODE, event_message: str) -> None:
        """
        Dispatches a runtime event with specified parameters.
        
        Args:
            level: Severity level of the event (INFO, WARNING, ERROR, CRITICAL)
            type: Category of the event (DEVICE_CORE, DEVICE_IO, etc.)
            code: Specific event code identifying the exact event
            event_message: Descriptive message providing event details
        
        This method logs the event and invokes any registered custom event handler.
        Events are filtered based on the current level threshold set via set_current_level.
        """
        if not isinstance(event_message, str):
            raise ValueError("event_message must be a string")
        
        C.runtime_event_dispatcher_dispatch_event(
            self._instance, 
            int(level), 
            int(type), 
            int(code), 
            event_message
        )
    
    def register_event_handler(self, handler: Callable[[int, int, int, str, str], None]) -> None:
        """
        Registers a custom event handler callback function.
        
        Args:
            handler: Callback function that will be invoked for each dispatched event.
                     The handler signature should be:
                     handler(level: int, type: int, code: int, message: str, timestamp: str) -> None
        
        Note: Only one handler can be registered at a time; subsequent calls will replace 
              the previous handler. The handler is invoked synchronously but with minimal 
              lock holding time to avoid blocking.
        
        Example:
            def my_handler(level, type, code, message, timestamp):
                print(f"[{timestamp}] Level {level}: {message}")
            
            dispatcher = RuntimeEventDispatcher()
            dispatcher.register_event_handler(my_handler)
        """
        if not callable(handler):
            raise ValueError("handler must be a callable function")
        
        C.runtime_event_dispatcher_register_event_handler(self._instance, handler)
    
    def set_current_level(self, level: LEVEL) -> None:
        """
        Sets the minimum event level threshold.
        
        Args:
            level: Minimum severity level for events to be processed
        
        Events below this level may be filtered out by custom handlers.
        """
        C.runtime_event_dispatcher_set_current_level(self._instance, int(level))
    
    def get_current_level(self) -> LEVEL:
        """
        Gets the current minimum event level threshold.
        
        Returns:
            Current minimum event severity level
        """
        level_int = C.runtime_event_dispatcher_get_current_level(self._instance)
        return self.LEVEL(level_int)

