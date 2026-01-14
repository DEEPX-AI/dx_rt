/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 *
 * This file uses cxxopts (MIT License) - Copyright (c) 2014 Jarryd Beck.
 */

#pragma once

#include <string>
#include "dxrt/common.h"
#include "dxrt/device_pool.h"
#include "dxrt/extern/cxxopts.hpp"


namespace dxrt {

// lpddr type (1 = lpddr4, 2= lpddr5)
const uint16_t M1_DDR_TYPE_LPDDR4 = 1;
const uint16_t M1_DDR_TYPE_LPDDR5 = 2;
const uint16_t M1_DDR_TYPE_LPDDR5X = 3;

// M1 M.2 board type (2)
// board type (1 = SOM, 2 = M.2, 3 = H1)
const uint16_t BOARD_TYPE_M_dot_2 = 2;
const uint16_t BOARD_TYPE_H1 = 3;


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
    // dxrt::SkipMode _checkDeviceSkip = dxrt::SkipMode::COMMON_SKIP;
    virtual void doCommand(std::shared_ptr<DeviceCore> devicePtr) = 0;
    virtual void finish() { }
};

class DXRT_API DeviceStatusCLICommand : public CLICommand
{
 public:
    explicit DeviceStatusCLICommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};
class DXRT_API DeviceInfoCLICommand : public CLICommand
{
 public:
    explicit DeviceInfoCLICommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};
class DXRT_API DeviceStatusMonitor : public CLICommand
{
   public:
      explicit DeviceStatusMonitor(cxxopts::ParseResult &);
   private:
      void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};
class DXRT_API FWVersionCommand : public CLICommand
{
 public:
    explicit FWVersionCommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};
class DXRT_API DeviceResetCommand : public CLICommand
{
 public:
    explicit DeviceResetCommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};
class DXRT_API FWUpdateCommand : public CLICommand
{
 public:
    explicit FWUpdateCommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
    void finish() override;

    std::string getSubCmdString();
    uint32_t _fwUpdateSubCmd;
    std::string _fwUpdateFile;
    bool _showLogOnce;
    bool _showDonotTunrOff;
    int _updateDeviceCount;
};

class DXRT_API FWUploadCommand : public CLICommand
{
 public:
    explicit FWUploadCommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
    std::string _fwUpdateFile;
};



class DXRT_API DeviceDumpCommand : public CLICommand
{
 public:
    explicit DeviceDumpCommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};

class DXRT_API FWConfigCommand : public CLICommand
{
 public:
    explicit FWConfigCommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};

class DXRT_API FWConfigCommandJson : public CLICommand
{
 public:
    explicit FWConfigCommandJson(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};

class DXRT_API FWLogCommand : public CLICommand
{
 public:
    explicit FWLogCommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};

class DXRT_API ShowVersionCommand : public CLICommand
{
 public:
    explicit ShowVersionCommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};
class DXRT_API PcieStatusCLICommand : public CLICommand
{
 public:
    explicit PcieStatusCLICommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};
class DXRT_API DDRErrorCLICommand : public CLICommand
{
 public:
    explicit DDRErrorCLICommand(cxxopts::ParseResult &);
 private:
    void doCommand(std::shared_ptr<DeviceCore> devicePtr) override;
};

const int CHECK_M1_DEVICE = 100;
const int CHECK_M1M_DEVICE = 101;
bool DXRT_API CheckH1Devices();
bool DXRT_API CheckM1Devices(int deviceType);  

}  // namespace dxrt
