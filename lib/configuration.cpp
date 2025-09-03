
/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 * 
 * This file uses ONNX Runtime (MIT License) - Copyright (c) Microsoft Corporation.
 */

#include "dxrt/common.h"
#include "dxrt/configuration.h"
#include "dxrt/exception/exception.h"
#include "dxrt/profiler.h"
#include "dxrt/device_info_status.h"
#include "dxrt/device.h"
#include "dxrt/device_version.h"
#include "./resource/log_messages.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <atomic>
#include <thread>


#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif // USE_ORT



#ifdef USE_SERVICE
#define USE_SERVICE_DEFAULT_VALUE true
#else
#define USE_SERVICE_DEFAULT_VALUE false
#endif

#if DEBUG_DXRT
#define DEBUG_DXRT_DEFAULT_VALUE true
#else
#define DEBUG_DXRT_DEFAULT_VALUE false
#endif

#if USE_PROFILER
#define USE_PROFILER_DEFAULT_VALUE true
#else
#define USE_PROFILER_DEFAULT_VALUE false
#endif

#if DXRT_DYNAMIC_CPU_THREAD
#define DXRT_DYNAMIC_CPU_THREAD_DEFAULT_VALUE true
#else
#define DXRT_DYNAMIC_CPU_THREAD_DEFAULT_VALUE false
#endif

#if SHOW_PROFILER_DATA
#define SHOW_PROFILER_DATA_DEFAULT_VALUE "on"
#else
#define SHOW_PROFILER_DATA_DEFAULT_VALUE "off"
#endif

#if SHOW_TASK_FLOW
#define SHOW_TASK_FLOW_DEFAULT_VALUE true
#else
#define SHOW_TASK_FLOW_DEFAULT_VALUE false
#endif

#if SAVE_PROFILER_DATA
#define SAVE_PROFILER_DATA_DEFAULT_VALUE "on"
#else
#define SAVE_PROFILER_DATA_DEFAULT_VALUE "off"
#endif

#ifndef USE_CUSTOM_INTRA_OP_THREADS
#define USE_CUSTOM_INTRA_OP_THREADS_DEFAULT_VALUE false
#else
#define USE_CUSTOM_INTRA_OP_THREADS_DEFAULT_VALUE true
#endif

#ifndef USE_CUSTOM_INTER_OP_THREADS
#define USE_CUSTOM_INTER_OP_THREADS_DEFAULT_VALUE false
#else
#define USE_CUSTOM_INTER_OP_THREADS_DEFAULT_VALUE true
#endif

// Convert macro values to strings for default attributes
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifndef CUSTOM_INTRA_OP_THREADS_COUNT
#define CUSTOM_INTRA_OP_THREADS_COUNT_DEFAULT_VALUE "1"
#else
#define CUSTOM_INTRA_OP_THREADS_COUNT_DEFAULT_VALUE TOSTRING(CUSTOM_INTRA_OP_THREADS_COUNT)
#endif

#ifndef CUSTOM_INTER_OP_THREADS_COUNT
#define CUSTOM_INTER_OP_THREADS_COUNT_DEFAULT_VALUE "1"
#else
#define CUSTOM_INTER_OP_THREADS_COUNT_DEFAULT_VALUE TOSTRING(CUSTOM_INTER_OP_THREADS_COUNT)
#endif

#ifdef SHOW_MODEL_INFO_DEFINE
    #define SHOW_MODEL_INFO_DEFAULT_VALUE true
#else
    #define SHOW_MODEL_INFO_DEFAULT_VALUE false
#endif // SHOW_MODEL_INFO


namespace dxrt {

    static bool isDebugFlag = DEBUG_DXRT_DEFAULT_VALUE;
    static bool isShowTaskFlowFlag = SHOW_TASK_FLOW_DEFAULT_VALUE;

    static std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    class ConfigParser
    {
    public:
        explicit ConfigParser(const std::string& filename) {
            parseFile(filename);
        }

        std::string getValue(const std::string& key) const {
            auto it = config.find(key);
            return (it != config.end()) ? it->second : "";
        }

        int getIntValue(const std::string& key) const {
            return std::stoi(getValue(key));
        }

        bool getBoolValue(const std::string& key) const {
            std::string value = getValue(key);
            return (value == "1" || value == "true" || value == "on");
        }

        bool has(const std::string& key) const {
            return config.find(key) != config.end();
        }

    private:
        std::unordered_map<std::string, std::string> config;

        void parseFile(const std::string& filename) {
            std::ifstream file(filename);
            std::string line;
            if (!file)
            {
                throw dxrt::FileNotFoundException(filename);
            }

            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::string key, value;

                if (std::getline(iss, key, '=') && std::getline(iss, value))
                {
                    key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
                    value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                    config[key] = toLower(value);
                }
            }
        }
    };


    Configuration* Configuration::_staticInstance = nullptr;

    Configuration& Configuration::GetInstance()
    {
        if ( _staticInstance == nullptr ) _staticInstance = new Configuration();
        return *_staticInstance;
    }

    void Configuration::deleteInstance()
    {
        if ( _staticInstance != nullptr ) delete _staticInstance;
        _staticInstance = nullptr;
    }

    Configuration::Configuration()
    {
        LOG_DXRT_DBG << "configuration constructor" << std::endl;

        // default configuration
        _enableSettings[ITEM::DEBUG] = DEBUG_DXRT_DEFAULT_VALUE;
        _enableSettings[ITEM::PROFILER] = USE_PROFILER_DEFAULT_VALUE;
        _enableSettings[ITEM::SERVICE] = USE_SERVICE_DEFAULT_VALUE;
        _enableSettings[ITEM::DYNAMIC_CPU_THREAD] = DXRT_DYNAMIC_CPU_THREAD_DEFAULT_VALUE;
        _enableSettings[ITEM::TASK_FLOW] = SHOW_TASK_FLOW_DEFAULT_VALUE;
        _enableSettings[ITEM::SHOW_THROTTLING] = false;
        _enableSettings[ITEM::SHOW_PROFILE] = USE_PROFILER_DEFAULT_VALUE;
        _enableSettings[ITEM::SHOW_MODEL_INFO] = SHOW_MODEL_INFO_DEFAULT_VALUE;
        _enableSettings[ITEM::CUSTOM_INTRA_OP_THREADS] = USE_CUSTOM_INTRA_OP_THREADS_DEFAULT_VALUE;
        _enableSettings[ITEM::CUSTOM_INTER_OP_THREADS] = USE_CUSTOM_INTER_OP_THREADS_DEFAULT_VALUE;

        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SHOW_DATA] = SHOW_PROFILER_DATA_DEFAULT_VALUE;
        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SAVE_DATA] = SAVE_PROFILER_DATA_DEFAULT_VALUE;
        _attributes[ITEM::CUSTOM_INTRA_OP_THREADS][ATTRIBUTE::CUSTOM_INTRA_OP_THREADS_NUM] = CUSTOM_INTRA_OP_THREADS_COUNT_DEFAULT_VALUE;
        _attributes[ITEM::CUSTOM_INTER_OP_THREADS][ATTRIBUTE::CUSTOM_INTER_OP_THREADS_NUM] = CUSTOM_INTER_OP_THREADS_COUNT_DEFAULT_VALUE;

    #ifndef USE_SERVICE
        _isReadonly[ITEM::SERVICE].first = true;
    #endif
    }

    Configuration::~Configuration()
    {
        LOG_DXRT_DBG << "configuration destructor" << std::endl;
    }

    int Configuration::parseClampThreadCount(const std::string& value)
    {
        if (value.empty()) {
            return 1; // default
        }
        
        try {
            int count = std::stoi(value);
            // Clamp between 1 and hardware_concurrency()
            int hw = static_cast<int>(std::thread::hardware_concurrency());
            int maxThreads = std::max(1, hw);
            int clamped = std::max(1, std::min(count, maxThreads));
            
            if (clamped != count) {
                LOG_DXRT_DBG << "Thread count clamped from " << count << " to " << clamped 
                             << " (max: " << maxThreads << ")" << std::endl;
            }
            
            return clamped;
        } catch (const std::exception& e) {
            LOG_DXRT_DBG << "Invalid thread count '" << value << "', using default (1): " << e.what() << std::endl;
            return 1;
        }
    }

    void Configuration::LoadConfigFile(const std::string& fileName)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        ConfigParser parser(fileName);

        // Enable flags: only override defaults if keys exist in config
        if (parser.has("ENABLE_DEBUG")) {
            setEnableWithoutLock(ITEM::DEBUG, parser.getBoolValue("ENABLE_DEBUG"));
        }
        if (parser.has("USE_PROFILER")) {
            setEnableWithoutLock(ITEM::PROFILER, parser.getBoolValue("USE_PROFILER"));
        }
    #ifdef USE_SERVICE
        if (parser.has("ENABLE_MULTI_PROCESS")) {
            setEnableWithoutLock(ITEM::SERVICE, parser.getBoolValue("ENABLE_MULTI_PROCESS"));
        }
    #endif
        if (parser.has("DXRT_DYNAMIC_CPU_THREAD")) {
            setEnableWithoutLock(ITEM::DYNAMIC_CPU_THREAD, parser.getBoolValue("DXRT_DYNAMIC_CPU_THREAD"));
        }
        if (parser.has("SHOW_TASK_FLOW_INFO")) {
            setEnableWithoutLock(ITEM::TASK_FLOW, parser.getBoolValue("SHOW_TASK_FLOW_INFO"));
        }
        
        // Only override compile-time defaults if keys are present in config file
        if (parser.has("USE_CUSTOM_INTRA_OP_THREADS")) {
            setEnableWithoutLock(ITEM::CUSTOM_INTRA_OP_THREADS, parser.getBoolValue("USE_CUSTOM_INTRA_OP_THREADS"));
        }
        if (parser.has("USE_CUSTOM_INTER_OP_THREADS")) {
            setEnableWithoutLock(ITEM::CUSTOM_INTER_OP_THREADS, parser.getBoolValue("USE_CUSTOM_INTER_OP_THREADS"));
        }

        // Attributes: only override defaults if keys exist in config
        if (parser.has("ENABLE_SHOW_PROFILER_DATA")) {
            setAttributeWithoutLock(ITEM::PROFILER, ATTRIBUTE::PROFILER_SHOW_DATA, parser.getValue("ENABLE_SHOW_PROFILER_DATA"));
        }
        if (parser.has("ENABLE_SAVE_PROFILER_DATA")) {
            setAttributeWithoutLock(ITEM::PROFILER, ATTRIBUTE::PROFILER_SAVE_DATA, parser.getValue("ENABLE_SAVE_PROFILER_DATA"));
        }
        
        // Only override compile-time defaults if keys are present in config file
        if (parser.has("CUSTOM_INTRA_OP_THREADS_COUNT")) {
            std::string validatedValue = std::to_string(parseClampThreadCount(parser.getValue("CUSTOM_INTRA_OP_THREADS_COUNT")));
            setAttributeWithoutLock(ITEM::CUSTOM_INTRA_OP_THREADS, ATTRIBUTE::CUSTOM_INTRA_OP_THREADS_NUM, validatedValue);
        }
        if (parser.has("CUSTOM_INTER_OP_THREADS_COUNT")) {
            std::string validatedValue = std::to_string(parseClampThreadCount(parser.getValue("CUSTOM_INTER_OP_THREADS_COUNT")));
            setAttributeWithoutLock(ITEM::CUSTOM_INTER_OP_THREADS, ATTRIBUTE::CUSTOM_INTER_OP_THREADS_NUM, validatedValue);
        }

    }

    void Configuration::SetEnable(const ITEM item, bool enabled)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        setEnableWithoutLock(item, enabled);
    }

    void Configuration::setEnableWithoutLock(const ITEM item, bool enabled)
    {
        if (_isReadonly[item].first == true)
        {
            throw dxrt::InvalidOperationException("configuration change not allowed");
        }
        _enableSettings[item] = enabled;
        if (item == ITEM::DEBUG)
        {
            isDebugFlag = enabled;
        }
        if (item == ITEM::TASK_FLOW)
        {
            isShowTaskFlowFlag = enabled;
        }
        if (item == ITEM::PROFILER)
        {
            Profiler::GetInstance().SetEnabled(enabled);
        }
    }

    void Configuration::SetAttribute(const ITEM item, const ATTRIBUTE attrib, const std::string& value)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        setAttributeWithoutLock(item, attrib, value);
    }

    void Configuration::setAttributeWithoutLock(const ITEM item, const ATTRIBUTE attrib, const std::string& value)
    {
        if (_isReadonly[item].second[attrib] == true)
        {
            throw dxrt::InvalidOperationException("change configuration not allowed");
        }
        _attributes[item][attrib] = value;
        if ((attrib == ATTRIBUTE::PROFILER_SAVE_DATA) || (attrib == ATTRIBUTE::PROFILER_SHOW_DATA))
        {
            const std::string v = toLower(value);
            const bool on = (v == "1" || v == "true" || v == "on");
            Profiler::GetInstance().SetSettings(attrib, on);
        }
    }

    bool Configuration::GetEnable(const ITEM item)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto it = _enableSettings.find(item);
        if (it == _enableSettings.end())
        {
            return false;
        }
        return it->second;
    }

    std::string Configuration::GetAttribute(const ITEM item, const ATTRIBUTE attrib)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto it = _attributes.find(item);
        if (it == _attributes.end())
        {
            return "";
        }
        auto it2 = it->second.find(attrib);
        if (it2 == it->second.end())
        {
            return "";
        }
        return it2->second;
    }

    int Configuration::GetIntAttribute(const ITEM item, const ATTRIBUTE attrib)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto it = _attributes.find(item);
        if (it == _attributes.end())
        {
            return 0;
        }
        auto it2 = it->second.find(attrib);
        if (it2 == it->second.end())
        {
            return 0;
        }
        
        try {
            return std::stoi(it2->second);
        } catch (const std::exception&) {
            return 0;
        }
    }

    void Configuration::LockEnable(const ITEM item)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto it = _attributes.find(item);
        if (it == _attributes.end())
        {
            return;
        }
        _isReadonly[item].first = true;
    }

    std::string Configuration::GetVersion() const
    {
        std::string version = DXRT_VERSION;
        if ( version[0] == 'v' )
            return version.substr(1);

        return version;
    }

    std::string Configuration::GetDriverVersion() const
    {
        uint32_t rt_driver_version = 0;

        std::vector<std::shared_ptr<dxrt::Device>> devices = CheckDevices();
        if ( devices.size() > 0 )
        {
            dxrt_dev_info_t dev_info = devices[0]->devInfo();
            rt_driver_version = dev_info.rt_drv_ver;
        }

        uint32_t major = rt_driver_version / 1000;
        uint32_t minor = (rt_driver_version / 100) % 10;
        uint32_t patch = rt_driver_version % 100;

        return  std::to_string(major) + "." +
                std::to_string(minor) + "." +
                std::to_string(patch); 
    }

    std::string Configuration::GetPCIeDriverVersion() const
    {
        uint32_t pcie_driver_version = 0;

        std::vector<std::shared_ptr<dxrt::Device>> devices = CheckDevices();
        if ( devices.size() > 0 )
        {
            dxrt_dev_info_t dev_info = devices[0]->devInfo();
            pcie_driver_version = dev_info.pcie.driver_version;
        }
                

        uint32_t major = pcie_driver_version / 1000;
        uint32_t minor = (pcie_driver_version / 100) % 10;
        uint32_t patch = pcie_driver_version % 100;

        return  std::to_string(major) + "." +
                std::to_string(minor) + "." +
                std::to_string(patch);   
    }

    std::vector<std::pair<int, std::string>> Configuration::GetFirmwareVersions() const
    {
        
        std::vector<std::pair<int, std::string>> fws;

        std::vector<std::shared_ptr<dxrt::Device>> devices = CheckDevices();
        if ( devices.size() > 0 )
        {
            for(auto& dev : devices)
            {
                dxrt_device_info_t device_info = dev->info();
                //uint16_t firmware_version = 
                uint32_t major = device_info.fw_ver / 100;
                uint32_t minor = (device_info.fw_ver / 10) % 10;
                uint32_t patch = device_info.fw_ver % 10;

                std::string version = std::to_string(major) + "." +
                                std::to_string(minor) + "." +
                                std::to_string(patch);

                fws.emplace_back(std::pair<int, std::string>(dev->id(), version));
            }
        }
                

        return fws;
    }

    std::string Configuration::GetONNXRuntimeVersion() const
    {
#ifdef USE_ORT
        std::string onnx_version = std::string(OrtGetApiBase()->GetVersionString());
        return onnx_version;
#else
        return "0.0.0";
#endif // USE_ORT
    }

}  // namespace dxrt

