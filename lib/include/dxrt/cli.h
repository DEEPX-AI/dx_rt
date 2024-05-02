// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "cxxopts.hpp"

namespace dxrt {
class CLI
{
public:
    CLI(cxxopts::ParseResult &);
    ~CLI();
    void Run();
private:
    cxxopts::ParseResult _cmd;
    int _resetOpt = -1;
    int _deviceId = -1;
    bool _printDeviceStatus = false;
    std::string _dumpFile = "";
    std::vector<uint32_t> _fwConfig;
    std::string _fwLogFile = "";
    std::string _fwUpdateFile = "";
    std::string _fwFile = "";
    bool _printDeviceInfo = false;
};

} // namespace dxrtdks