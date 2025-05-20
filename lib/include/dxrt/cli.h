// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include "dxrt/common.h"
#include "dxrt/device.h"
#include "cxxopts.hpp"


namespace dxrt {
class DXRT_API CLICommand
{
 public:
    explicit CLICommand(cxxopts::ParseResult &);
    virtual ~CLICommand();
    void Run();
 protected:
    cxxopts::ParseResult _cmd;
    int _deviceId = -1;
    bool _withDevice = true;
    dxrt::dxrt_ident_sub_cmd_t _subCmd = dxrt::dxrt_ident_sub_cmd_t::DX_IDENTIFY_NONE;
    dxrt::SkipMode _checkDeviceSkip = dxrt::SkipMode::COMMON_SKIP;
    virtual void doCommand(DevicePtr devicePtr) = 0;
};

class DXRT_API DeviceStatusCLICommand : public CLICommand
{
 public:
    explicit DeviceStatusCLICommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};
class DXRT_API DeviceInfoCLICommand : public CLICommand
{
 public:
    explicit DeviceInfoCLICommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};
class DXRT_API DeviceStatusMonitor : public CLICommand
{
   public:
      explicit DeviceStatusMonitor(cxxopts::ParseResult &);
   private:
      void doCommand(DevicePtr devicePtr) override;
};
class DXRT_API DeviceMonitorDebug : public CLICommand
{
   public:
      explicit DeviceMonitorDebug(cxxopts::ParseResult &);
   private:
      void doCommand(DevicePtr devicePtr) override;
};
class DXRT_API FWVersionCommand : public CLICommand
{
 public:
    explicit FWVersionCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};
class DXRT_API DeviceResetCommand : public CLICommand
{
 public:
    explicit DeviceResetCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};
class DXRT_API FWUpdateCommand : public CLICommand
{
 public:
    explicit FWUpdateCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
    uint32_t _fwUpdateSubCmd;
    std::string _fwUpdateFile;
};

class DXRT_API FWUploadCommand : public CLICommand
{
 public:
    explicit FWUploadCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
    std::string _fwUpdateFile;
};



class DXRT_API DeviceDumpCommand : public CLICommand
{
 public:
    explicit DeviceDumpCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};

class DXRT_API FWConfigCommand : public CLICommand
{
 public:
    explicit FWConfigCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};

class DXRT_API FWConfigCommandJson : public CLICommand
{
 public:
    explicit FWConfigCommandJson(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};

class DXRT_API DDRTargetCommand : public CLICommand
{
   public:
      explicit DDRTargetCommand(cxxopts::ParseResult &);
   private:
      void doCommand(DevicePtr devicePtr) override;
};

class DXRT_API OTPCommand : public CLICommand
{
   public:
      explicit OTPCommand(cxxopts::ParseResult &);
   private:
      void doCommand(DevicePtr devicePtr) override;
};

class DXRT_API FWLogCommand : public CLICommand
{
 public:
    explicit FWLogCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};

class DXRT_API ShowVersionCommand : public CLICommand
{
 public:
    explicit ShowVersionCommand(cxxopts::ParseResult &);
 private:
    void doCommand(DevicePtr devicePtr) override;
};

}  // namespace dxrt
