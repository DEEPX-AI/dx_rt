// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <memory>

#include "common.h"

namespace dxrt {

    class DXRT_API Configuration {

    private:

        // constructor
        Configuration();

    public:

        // static function
        static std::shared_ptr<Configuration>& GetInstance();
        void LoadConfigFile(const std::string& fileName);

        // ITEM
        enum class ITEM {
            DEBUG = 1,
            PROFILER,
            SERVICE,
            DYNAMIC_CPU_THREAD,
            TASK_FLOW
            //DEBUG
            //USE_PROFILER
            //SHOW_PROFILER_DATA
            //SHOW_TASK_FLOW
            //SAVE_PROFILER_DATA
        };

        // ATTRIBUTE
        enum class ATTRIBUTE {
            PROFILER_SHOW_DATA = 1001,
            PROFILER_SAVE_DATA
        };

        void SetEnable(const ITEM item, bool enabled);

        void SetAttribute(const ITEM item, const ATTRIBUTE attrib, const std::string& value);

        bool GetEnable(const ITEM item) const;

        std::string GetAttribute(const ITEM item, const ATTRIBUTE attrib) const;

        void LockEnable(const ITEM item);

    private:

        std::unordered_map<ITEM, bool> _enableSettings;
        std::unordered_map<ITEM, std::unordered_map<ATTRIBUTE, std::string>> _attributes;
        std::unordered_map<ITEM, std::pair<bool,std::unordered_map<ATTRIBUTE, bool> > > _isReadonly;
    };


    

}  // namespace dxrt
