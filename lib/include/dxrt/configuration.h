// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <memory>

#include "dxrt/common.h"

namespace dxrt {

    class DXRT_API Configuration
    {
    private:
        // constructor
        Configuration();
        ~Configuration();

        // Delete copy constructor and assignment operator
        Configuration(const Configuration&) = delete;
        Configuration& operator=(const Configuration&) = delete;

        friend class ObjectsPool;
        static Configuration* _staticInstance;
        static void deleteInstance();

    public:
        // static function
        static Configuration& GetInstance();
        void LoadConfigFile(const std::string& fileName);

        // ITEM
        enum class ITEM {
            DEBUG = 1,
            PROFILER,
            SERVICE,
            DYNAMIC_CPU_THREAD,
            TASK_FLOW,
            SHOW_THROTTLING,
            SHOW_PROFILE
        };

        // ATTRIBUTE
        enum class ATTRIBUTE {
            PROFILER_SHOW_DATA = 1001,
            PROFILER_SAVE_DATA
        };

        void SetEnable(const ITEM item, bool enabled);

        void SetAttribute(const ITEM item, const ATTRIBUTE attrib, const std::string& value);

        bool GetEnable(const ITEM item);

        std::string GetAttribute(const ITEM item, const ATTRIBUTE attrib);

        void LockEnable(const ITEM item);

    private:
        std::unordered_map<ITEM, bool> _enableSettings;
        std::unordered_map<ITEM, std::unordered_map<ATTRIBUTE, std::string> > _attributes;
        std::unordered_map<ITEM, std::pair<bool, std::unordered_map<ATTRIBUTE, bool> > > _isReadonly;
        std::mutex _mutex;
    };

}  // namespace dxrt
