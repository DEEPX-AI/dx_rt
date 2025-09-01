#include <vector>
#include <string>
#include <iostream> 
#include <numeric>  
#include <stdexcept>

#include "dxrt/dxrt_api.h" 

namespace dxrt
{

    void pyConfiguration_SetEnable(Configuration &conf, int item, bool enabled)
    {
        conf.SetEnable(static_cast<Configuration::ITEM>(item), enabled);
    }

    int pyConfiguration_GetEnable(Configuration &conf, int item)
    {
        return conf.GetEnable(static_cast<Configuration::ITEM>(item));
    }

    void pyConfiguration_SetAttribute(Configuration &conf, int item, int attrib, std::string value)
    {
        conf.SetAttribute(static_cast<Configuration::ITEM>(item), 
            static_cast<Configuration::ATTRIBUTE>(attrib), value);
    }

    std::string pyConfiguration_GetAttribute(Configuration &conf, int item, int attrib)
    {
        return conf.GetAttribute(static_cast<Configuration::ITEM>(item), 
            static_cast<Configuration::ATTRIBUTE>(attrib));
    }

    std::string pyConfiguration_GetVersion(Configuration &conf)
    {
        return conf.GetVersion();
    }

    std::string pyConfiguration_GetDriverVersion(Configuration &conf)
    {
        return conf.GetDriverVersion();
    }

    std::string pyConfiguration_GetPCIeDriverVersion(Configuration &conf)
    {
        return conf.GetPCIeDriverVersion();
    }

    void pyConfiguration_LoadConfigFile(Configuration &conf, const std::string &fileName)
    {
        conf.LoadConfigFile(fileName);
    }

} // namespace dxrt