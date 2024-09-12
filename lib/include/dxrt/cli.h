// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include "dxrt/common.h"
#include "dxrt/device.h"
#include "cxxopts.hpp"


namespace dxrt {
class CLICommand
{
 public:
    DXRT_API explicit CLICommand(cxxopts::ParseResult &);
    virtual DXRT_API ~CLICommand();
    DXRT_API void Run();
 protected:
    cxxopts::ParseResult _cmd;
    int _deviceId = -1;
    bool _withDevice = true;
    int _subCommand = 0;
    dxrt::SkipMode _checkDeviceSkip = dxrt::SkipMode::COMMON_SKIP;
    virtual void doCommand(DevicePtr devicePtr) = 0;
};

class DeviceStatusCLICommand : public CLICommand
{
 public:
    explicit DeviceStatusCLICommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};
class DeviceInfoCLICommand : public CLICommand
{
 public:
    explicit DeviceInfoCLICommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};
class FWVersionCommand : public CLICommand
{
 public:
    explicit FWVersionCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};
class DeviceResetCommand : public CLICommand
{
 public:
    explicit DeviceResetCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};
class FWUpdateCommand : public CLICommand
{
 public:
    explicit FWUpdateCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
    uint32_t _fwUpdateSubCmd;
    std::string _fwUpdateFile;
};

class FWUploadCommand : public CLICommand
{
 public:
    explicit FWUploadCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
    uint32_t _fwUpdateSubCmd;
    std::string _fwUpdateFile;
};



class DeviceDumpCommand : public CLICommand
{
 public:
    explicit DeviceDumpCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};

class FWConfigCommand : public CLICommand
{
 public:
    explicit FWConfigCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};

class FWLogCommand : public CLICommand
{
 public:
    explicit FWLogCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};




}  // namespace dxrt
