/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>
#include <atomic>
#include "dxrt/common.h"

#undef ERROR

namespace dxrt {

    /**
     * @class RuntimeEventDispatcher
     * @brief Singleton class for dispatching and handling runtime events from the DX-RT system.
     * 
     * This class provides a centralized event dispatching mechanism for runtime events
     * such as device errors, warnings, and notifications. It supports custom event handlers
     * and automatic logging of events with different severity levels.
     * 
     * Thread-safety: Uses separate mutexes for handler registration and logging to minimize
     * contention. Event level checking uses std::atomic for lock-free access.
     */
    class DXRT_API RuntimeEventDispatcher
    {
    public:
        /**
         * @enum LEVEL
         * @brief Event severity levels for categorizing runtime events.
         */
        enum class LEVEL
        {
            INFO = 1,      ///< Informational messages for normal operation events
            WARNING,       ///< Warning messages for potential issues that don't stop execution
            ERROR,         ///< Error messages for recoverable failures
            CRITICAL       ///< Critical errors that may cause system instability
        };

        /**
         * @enum TYPE
         * @brief Event type categories for classifying the source of events.
         */
        enum class TYPE
        {
            DEVICE_CORE = 1000,    ///< Events related to NPU core operations
            DEVICE_STATUS,         ///< Device status change events
            DEVICE_IO,             ///< Input/Output operation events
            DEVICE_MEMORY,         ///< Memory management events
            UNKNOWN                ///< Unknown or unclassified event types
        };

        /**
         * @enum CODE
         * @brief Specific event codes for identifying the exact nature of events.
         */
        enum class CODE
        {
            WRITE_INPUT = 2000,     ///< Input data write operation event
            READ_OUTPUT,            ///< Output data read operation event
            MEMORY_OVERFLOW,        ///< Memory overflow or capacity exceeded
            MEMORY_ALLOCATION,      ///< Memory allocation failure or issue
            DEVICE_EVENT,           ///< General device event notification
            RECOVERY_OCCURRED,      ///< Device recovery action taken
            TIMEOUT_OCCURRED,       ///< Operation timeout event
            THROTTLING_NOTICE,      ///< Device throttling notification
            THROTTLING_EMERGENCY,   ///< Device throttling emergency notification
            UNKNOWN                 ///< Unknown or unclassified event code
        };


    public:
        /**
         * @brief Gets the singleton instance of RuntimeEventDispatcher.
         * @return Reference to the singleton RuntimeEventDispatcher instance.
         * 
         * Creates the instance on first call and registers it for automatic cleanup at program exit.
         */
        static RuntimeEventDispatcher& GetInstance();

        /**
         * @brief Dispatches a runtime event with specified parameters.
         * @param level Severity level of the event (INFO, WARNING, ERROR, CRITICAL)
         * @param type Category of the event (DEVICE_CORE, DEVICE_IO, etc.)
         * @param code Specific event code identifying the exact event
         * @param eventMessage Descriptive message providing event details
         * 
         * This method logs the event and invokes any registered custom event handler.
         * Events are filtered based on the current level threshold set via SetCurrentLevel.
         */
        void DispatchEvent(LEVEL level, TYPE type, CODE code, const std::string& eventMessage);

        /**
         * @brief Registers a custom event handler callback function.
         * @param handler Callback function that will be invoked for each dispatched event
         * 
         * The handler function signature: void(LEVEL, TYPE, CODE, const std::string& message, const std::string& timestamp)
         * Note: Only one handler can be registered at a time; subsequent calls will replace the previous handler.
         * The handler is invoked synchronously but with minimal lock holding time to avoid blocking.
         */
        void RegisterEventHandler(std::function<void(LEVEL, TYPE, CODE, const std::string& message, const std::string& timestamp)> handler);

        /**
         * @brief Sets the minimum event level threshold.
         * @param level Minimum severity level for events to be processed
         * 
         * Events below this level may be filtered out by custom handlers.
         */
        void SetCurrentLevel(LEVEL level) { _currentLevel.store(level); }

        /**
         * @brief Gets the current minimum event level threshold.
         * @return Current minimum event severity level
         */
        LEVEL GetCurrentLevel() const { return _currentLevel.load(); }

    public:
        /**
         * @brief Destructor for RuntimeEventDispatcher.
         */
        ~RuntimeEventDispatcher() = default;
        
    private:
        /**
         * @brief Private constructor to enforce singleton pattern.
         */
        RuntimeEventDispatcher() {}

        /**
         * @brief Deleted copy constructor to prevent copying.
         */
        RuntimeEventDispatcher(const RuntimeEventDispatcher&) = delete;

        /**
         * @brief Deleted assignment operator to prevent assignment.
         */
        RuntimeEventDispatcher& operator=(const RuntimeEventDispatcher&) = delete;

        /**
         * @brief Invokes the registered custom event handler.
         * @param level Event severity level
         * @param type Event category type
         * @param code Specific event code
         * @param eventMessage Event description message
         * @param timestamp Event timestamp
         * @return true if a handler was registered and invoked, false otherwise
         * 
         * Uses copy-and-execute pattern to minimize mutex lock duration.
         * Handler is copied under lock, then executed outside the lock.
         */
        bool invokeEventHandler(LEVEL level, TYPE type, CODE code, const std::string& eventMessage, const std::string& timestamp);

        /**
         * @brief Logs the event with formatted output including level, type, and code strings.
         * @param level Event severity level
         * @param type Event category type
         * @param code Specific event code
         * @param eventMessage Event description message
         * @param timestamp Event timestamp
         * 
         * Converts enum values to human-readable strings for better log readability.
         * Output format: [Runtime Event] level=LEVEL_STRING type=TYPE_STRING code=CODE_STRING message=MSG timestamp=TIMESTAMP
         */
        void handleEventLogging(LEVEL level, TYPE type, CODE code, const std::string& eventMessage, const std::string& timestamp);

    private:
        static RuntimeEventDispatcher* _staticInstance;  ///< Singleton instance pointer
        std::function<void(LEVEL, TYPE, CODE, const std::string& message, const std::string& timestamp)> _eventHandler{nullptr};  ///< Custom event handler callback
        std::atomic<LEVEL> _currentLevel{LEVEL::WARNING};  ///< Current minimum event level threshold
        std::mutex _handlerMutex;  ///< Mutex for thread-safe event handler registration and access

    };

}  // namespace dxrt

