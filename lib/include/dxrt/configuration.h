/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <memory>

#include "dxrt/common.h"

namespace dxrt {

    /**
    * @brief A singleton class for managing application configurations.
    *
    * The `Configuration` class manages various settings and their attributes
    * that are used throughout the application.
    * Settings can be enabled/disabled, and specific attributes are set with string values.
    * It uses a mutex to ensure thread safety when accessing configuration values,
    * preventing concurrency issues.
    *
    * This class follows the singleton design pattern, meaning you should access
    * its unique instance via the `GetInstance()` method.
    *
    * @note The constructor, destructor, copy constructor, and assignment operator
    * of this class are private and deleted to enforce the singleton pattern.
    */
    class DXRT_API Configuration
    {
    public:
        // destructor
        ~Configuration();

    private:

        // constructor
        Configuration();

        

        // Delete copy constructor and assignment operator
        Configuration(const Configuration&) = delete;
        Configuration& operator=(const Configuration&) = delete;

        friend class ObjectsPool;
        static Configuration* _staticInstance;
        static void deleteInstance();

    public:
        /**
        * @brief Returns the unique instance of the `Configuration` class.
        *
        * This method must be used to access the `Configuration` singleton.
        * If the instance does not exist, this method will create it.
        *
        * @return A reference to the `Configuration` instance. Always returns a valid instance.
        */
        static Configuration& GetInstance();

        /**
        * @brief Loads configurations from the specified file.
        *
        * This method reads configuration values from the given file
        * and initializes the internal setting maps of the class.
        * The specific file format depends on the implementation.
        *
        * @param fileName The path and name of the configuration file to load.
        */
        void LoadConfigFile(const std::string& fileName);

        /**
        * @brief Enumeration defining types of configuration items.
        */
        enum class ITEM {
            DEBUG = 1,                      ///< Configuration related to debug mode.
            PROFILER,                       ///< Configuration related to profiler functionality.
            SERVICE,                        ///< Configuration related to service operation.
            DYNAMIC_CPU_THREAD,             ///< Configuration related to dynamic CPU thread management.
            TASK_FLOW,                      ///< Configuration related to task flow management.
            SHOW_THROTTLING,                ///< Whether to display throttling information.
            SHOW_PROFILE,                   ///< Whether to display profile information.
            SHOW_MODEL_INFO,                ///< Whether to display model information.
            CUSTOM_INTRA_OP_THREADS = 9,    ///< Use custom ONNX Runtime intra-op thread count.
            CUSTOM_INTER_OP_THREADS = 10    ///< Use custom ONNX Runtime inter-op thread count.
        };

        /**
        * @brief Enumeration defining attributes for configuration items.
        */
        enum class ATTRIBUTE {
            PROFILER_SHOW_DATA = 1001,          ///< Attribute for showing profiler data.
            PROFILER_SAVE_DATA = 1002,          ///< Attribute for saving profiler data.
            CUSTOM_INTRA_OP_THREADS_NUM = 1003, ///< Attribute for custom ONNX Runtime intra-op thread count.
            CUSTOM_INTER_OP_THREADS_NUM = 1004  ///< Attribute for custom ONNX Runtime inter-op thread count.
        };
 
        /**
        * @brief Sets the enabled status for a specific configuration item.
        *
        * @param item The configuration item whose enabled status will be changed.
        * @param enabled `true` to enable, `false` to disable.
        * @note Uses an internal mutex to ensure thread safety.
        */
        void SetEnable(const ITEM item, bool enabled);

        /**
        * @brief Sets a specific attribute value for a given configuration item.
        *
        * @param item The configuration item for which to set the attribute.
        * @param attrib The type of attribute to set.
        * @param value The attribute value (as a string).
        * @note Uses an internal mutex to ensure thread safety.
        */
        void SetAttribute(const ITEM item, const ATTRIBUTE attrib, const std::string& value);

        /**
        * @brief Retrieves the enabled status of a specific configuration item.
        *
        * @param item The configuration item whose enabled status to retrieve.
        * @return `true` if the item is enabled, `false` otherwise.
        * @note Uses an internal mutex to ensure thread safety.
        */
        bool GetEnable(const ITEM item);

        /**
        * @brief Retrieves the value of a specific attribute for a given configuration item.
        *
        * @param item The configuration item for which to retrieve the attribute value.
        * @param attrib The type of attribute to retrieve.
        * @return The value of the attribute as a string. May return an empty string if the attribute does not exist.
        * @note Uses an internal mutex to ensure thread safety.
        */
        std::string GetAttribute(const ITEM item, const ATTRIBUTE attrib);

        /**
         * @brief Retrieves an integer attribute value for a given configuration item and attribute.
         * 
         * @param item The configuration item.
         * @param attrib The attribute of the configuration item.
         * @return The integer value of the attribute.
         *         Returns 0 if not found or cannot be parsed to integer.
         *         Note: For attributes like thread counts, 0 indicates "not set/invalid" and should trigger a safe default.
         * @note Uses an internal mutex to ensure thread safety.
         */
        int GetIntAttribute(const ITEM item, const ATTRIBUTE attrib);

        /**
        * @brief Locks a specific configuration item, making it read-only.
        *
        * After this function is called, the `_enableSettings` and associated
        * `_attributes` for this `ITEM` cannot be modified further.
        *
        * @param item The configuration item to set as read-only.
        * @note Uses an internal mutex to ensure thread safety.
        */
        void LockEnable(const ITEM item);

        /**
        * @brief Retrieves the version of this DXRT library.
        * @return The library version as a string (e.g., "1.2.3").
        */
        std::string GetVersion() const;

        /**
        * @brief Retrieves the version of the associated device driver.
        * @return The driver version as a string.
        * @throws InvalidOperationException If the driver's version is below the minimum requirement.
        */
        std::string GetDriverVersion() const;

        /**
        * @brief Retrieves the version of the PCIe driver.
        * @throws InvalidOperationException If the PCIe driver's version is below the minimum requirement.
        */
        std::string GetPCIeDriverVersion() const;

        /**
        * @brief Retrieves the firmware versions of all detected devices.
        * @return A vector of pairs, where each pair contains a device ID and its firmware version string.
        * @throws InvalidOperationException If the firmware's version is below the minimum requirement.
        */
        std::vector<std::pair<int, std::string>> GetFirmwareVersions() const;

        /**
        * @brief Retrieves the version of the ONNX Runtime library being used.
        * @return The ONNX Runtime version as a string.
        * @throws InvalidOperationException If the ONNX Runtime library's version does not meet the minimum requirement.
        * @note This version corresponds to the library that this software is linked against.
        */
        std::string GetONNXRuntimeVersion() const;

    private:
        std::unordered_map<ITEM, bool> _enableSettings;
        std::unordered_map<ITEM, std::unordered_map<ATTRIBUTE, std::string> > _attributes;
        std::unordered_map<ITEM, std::pair<bool, std::unordered_map<ATTRIBUTE, bool> > > _isReadonly;
        std::mutex _mutex;
        
        // Implementation methods without mutex locking (for internal use)
        void setEnableWithoutLock(const ITEM item, bool enabled);
        void setAttributeWithoutLock(const ITEM item, const ATTRIBUTE attribute, const std::string& value);
        
        // Utility method for parsing and clamping thread count values
        int parseClampThreadCount(const std::string& value);
    };

}  // namespace dxrt
