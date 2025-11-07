/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#pragma once

#include <memory>
#include <string>

#include "dxrt/driver_adapter/driver_adapter.h"
#include "dxrt/common.h"

namespace dxrt {

class DriverAdapterFactory {
 public:
        static std::unique_ptr<DriverAdapter> CreateForDeviceFile(const std::string& devicePath);
        static std::unique_ptr<DriverAdapter> CreateForNetwork();
};

}  // namespace dxrt
