/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Wrapper — Configuration (prebuilt delivery).
 */

#pragma once
#include "dxrt/dxrt_c_api.h"
#include "dxrt/common.h"
#include <cstring>
#include <map>
#include <utility>

namespace dxrt {

class Configuration
{
public:
    enum class ITEM
    {
        DEBUG = 1,
        PROFILER,
        SERVICE,
        DYNAMIC_CPU_THREAD,
        TASK_FLOW,
        SHOW_THROTTLING,
        SHOW_PROFILE,
        SHOW_MODEL_INFO,
        CUSTOM_INTRA_OP_THREADS,
        CUSTOM_INTER_OP_THREADS,
        NFH_ASYNC
    };

    enum class ATTRIBUTE
    {
        PROFILER_SHOW_DATA      = 1001,
        PROFILER_SAVE_DATA      = 1002,
        CUSTOM_INTRA_OP_THREADS_NUM = 1003,
        CUSTOM_INTER_OP_THREADS_NUM = 1004
    };

    static Configuration& GetInstance()
    {
        static Configuration instance;
        return instance;
    }

    std::string GetVersion() const
    {
        char buf[128];
        if (dxrt_config_get_version(buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string GetDriverVersion() const
    {
        char buf[128];
        if (dxrt_config_get_driver_version(buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string GetPCIeDriverVersion() const
    {
        char buf[128];
        if (dxrt_config_get_pcie_driver_version(buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::string GetONNXRuntimeVersion() const
    {
        char buf[128];
        if (dxrt_config_get_ort_version(buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    std::vector<std::pair<int, std::string>> GetFirmwareVersions() const
    {
        int count = 0;
        dxrt_config_get_firmware_versions(nullptr, nullptr, 0, &count);
        std::vector<std::pair<int, std::string>> result;
        if (count > 0)
        {
            std::vector<int> ids(static_cast<size_t>(count));
            std::vector<char> storage(static_cast<size_t>(count) * 128);
            std::vector<char*> ptrs(static_cast<size_t>(count));
            for (int i = 0; i < count; ++i)
            {
                ptrs[static_cast<size_t>(i)] = &storage[static_cast<size_t>(i) * 128];
            }
            dxrt_config_get_firmware_versions(ids.data(), ptrs.data(), 128, &count);
            for (int i = 0; i < count; ++i)
            {
                result.emplace_back(ids[static_cast<size_t>(i)],
                                    std::string(ptrs[static_cast<size_t>(i)]));
            }
        }
        return result;
    }

    void SetEnable(ITEM item, bool enabled)
    {
        dxrt_config_set_enable(static_cast<dxrt_config_item_t>(item), enabled ? 1 : 0);
    }

    bool GetEnable(ITEM item)
    {
        int enabled = 0;
        dxrt_config_get_enable(static_cast<dxrt_config_item_t>(item), &enabled);
        return enabled != 0;
    }

    void SetAttribute(ITEM item, ATTRIBUTE attr, const std::string& value)
    {
        dxrt_config_set_attribute(static_cast<dxrt_config_item_t>(item),
                                  static_cast<dxrt_config_attr_t>(attr),
                                  value.c_str());
    }

    std::string GetAttribute(ITEM item, ATTRIBUTE attr)
    {
        char buf[1024];
        if (dxrt_config_get_attribute(static_cast<dxrt_config_item_t>(item),
                                      static_cast<dxrt_config_attr_t>(attr),
                                      buf, sizeof(buf)) == DXRT_OK)
        {
            return std::string(buf);
        }
        return "";
    }

    int GetIntAttribute(ITEM item, ATTRIBUTE attr)
    {
        int val = 0;
        dxrt_config_get_int_attribute(static_cast<dxrt_config_item_t>(item),
                                      static_cast<dxrt_config_attr_t>(attr),
                                      &val);
        return val;
    }

    void LockEnable(ITEM item)
    {
        dxrt_config_lock_enable(static_cast<dxrt_config_item_t>(item));
    }

    void LoadConfigFile(const std::string& fileName)
    {
        dxrt_config_load_file(fileName.c_str());
    }

    void SetFWConfigWithJson(const std::string& json_file)
    {
        dxrt_config_set_fw_config_json(json_file.c_str());
    }

private:
    Configuration() = default;
};

} // namespace dxrt
