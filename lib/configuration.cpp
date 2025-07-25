
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
        _enableSettings[ITEM::DEBUG] = DXRT_DYNAMIC_CPU_THREAD_DEFAULT_VALUE;
        _enableSettings[ITEM::PROFILER] = USE_PROFILER_DEFAULT_VALUE;
        _enableSettings[ITEM::SERVICE] = USE_SERVICE_DEFAULT_VALUE;
        _enableSettings[ITEM::DYNAMIC_CPU_THREAD] = DXRT_DYNAMIC_CPU_THREAD_DEFAULT_VALUE;
        _enableSettings[ITEM::TASK_FLOW] = SHOW_TASK_FLOW_DEFAULT_VALUE;
        _enableSettings[ITEM::SHOW_THROTTLING] = false;
        _enableSettings[ITEM::SHOW_PROFILE] = false;
        _enableSettings[ITEM::SHOW_MODEL_INFO] = SHOW_MODEL_INFO_DEFAULT_VALUE;

        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SHOW_DATA] = SHOW_PROFILER_DATA_DEFAULT_VALUE;
        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SAVE_DATA] = SAVE_PROFILER_DATA_DEFAULT_VALUE;

    #ifndef USE_SERVICE
        _isReadonly[ITEM::SERVICE].first = true;
    #endif
    }

    Configuration::~Configuration()
    {
        LOG_DXRT_DBG << "configuration destructor" << std::endl;
    }

    // example config file
    // ENABLE_MULTI_PROCESS=ON
    // ENABLE_PROFILER=ON
    // PROFILER_SHOW_DATA=OFF
    // PROFILER_SAVE_DATA=OFF
    // ENABLE_DYNAMIC_CPU_THREAD=OFF

    void Configuration::LoadConfigFile(const std::string& fileName)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        ConfigParser parser(fileName);

        SetEnable(ITEM::DEBUG, parser.getBoolValue("ENABLE_DEBUG"));
        SetEnable(ITEM::PROFILER, parser.getBoolValue("ENABLE_PROFILER"));
    #ifdef USE_SERVICE
        SetEnable(ITEM::SERVICE, parser.getBoolValue("ENABLE_MULTI_PROCESS"));
    #endif
        SetEnable(ITEM::DYNAMIC_CPU_THREAD, parser.getBoolValue("ENABLE_DYNAMIC_CPU_THREAD"));
        SetEnable(ITEM::TASK_FLOW, parser.getBoolValue("ENABLE_TASK_FLOW"));

        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SHOW_DATA] = parser.getValue("PROFILER_SHOW_DATA");
        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SAVE_DATA] = parser.getValue("PROFILER_SAVE_DATA");

    }



    void Configuration::SetEnable(const ITEM item, bool enabled)
    {
        std::lock_guard<std::mutex> lock(_mutex);

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

        if (_isReadonly[item].second[attrib] == true)
        {
            throw dxrt::InvalidOperationException("change configuration not allowed");
        }
        _attributes[item][attrib] = value;
        if ((attrib == ATTRIBUTE::PROFILER_SAVE_DATA) || (attrib == ATTRIBUTE::PROFILER_SHOW_DATA))
        {
            Profiler::GetInstance().SetSettings(attrib, value == "ON");
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

