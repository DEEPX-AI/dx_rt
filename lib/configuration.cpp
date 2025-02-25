
#include "dxrt/configuration.h"
#include "dxrt/exception/exception.h"
#include <memory>

#ifdef USE_SERVICE
#define SERVICE_DEFAULT_VALUE true
#else
#define SERVICE_DEFAULT_VALUE false
#endif


namespace dxrt {

    std::shared_ptr<Configuration> Configuration::GetInstance()
    {
        static std::shared_ptr<Configuration> _instance(new Configuration());
        return _instance;
    }

    Configuration::Configuration()
    {
        // default configuration
        _enableSettings[ITEM::DEBUG] = false;
        _enableSettings[ITEM::PROFILER] = false;
        _enableSettings[ITEM::SERVICE] = SERVICE_DEFAULT_VALUE;

        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SHOW_DATA] = "OFF";
        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SHOW_TASK_FLOW] = "OFF";
        _attributes[ITEM::PROFILER][ATTRIBUTE::PROFILER_SAVE_DATA] = "OFF";

#ifndef USE_SERVICE
        _isReadonly[ITEM::SERVICE].first = true;
#endif
    }

    void Configuration::SetEnable(const ITEM item, bool enabled)
    {
        if (_isReadonly[item].first == true)
        {
            throw dxrt::InvalidOperationException("change configuration not allowed");
        }
        _enableSettings[item] = enabled;
    }

    void Configuration::SetAttribute(const ITEM item, const ATTRIBUTE attrib, const std::string& value)
    {
        if (_isReadonly[item].second[attrib] == true)
        {
            throw dxrt::InvalidOperationException("change configuration not allowed");
        }
        _attributes[item][attrib] = value;
    }

    bool Configuration::GetEnable(const ITEM item) const
    {
        auto it = _enableSettings.find(item);
        if (it == _enableSettings.end())
        {
            return false;
        }
        return it->second;
    }

    std::string Configuration::GetAttribute(const ITEM item, const ATTRIBUTE attrib) const
    {
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

}

