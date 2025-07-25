// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/cli.h"

#include <string>

#include "dxrt/device.h"
#include "dxrt/fw.h"
#include "dxrt/util.h"
#include "dxrt/device_info_status.h"
#include "dxrt/filesys_support.h"
#include "dxrt/driver.h"
#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/istreamwrapper.h"
#include "dxrt/objects_pool.h"
#include "../lib/resource/log_messages.h"
#include "dxrt/device_version.h"
#include "dxrt/device_struct.h"
#include "dxrt/device_struct_operators.h"

using std::string;
using std::vector;
using std::shared_ptr;


namespace dxrt {

static string ParseFwUpdateSubCmd(string cmd, uint32_t* subCmd)
{
    string path = getPath(cmd);
    if ( !fileExists(path) ) {
        path = "";
        if (cmd == "unreset") {
            *subCmd |= FWUPDATE_DEV_UNRESET;
        } else if (cmd == "force") {
            *subCmd |= FWUPDATE_FORCE;
        } else {
            std::cout << "[ERR] Unknown sub-command or not found file path: " << cmd << std::endl;
            exit(-1);
        }
    }
    return path;
}

static void HelpJsonConfig(void)
{
    const char* helpMessage = R"(
{
    "throttling_table": [
      { "mhz": 1000, "temper": 65 },
      { "mhz": 800,  "temper": 70 },
      { "mhz": 700,  "temper": 75 },
      { "mhz": 600,  "temper": 80 },
      { "mhz": 500,  "temper": 85 },
      { "mhz": 400,  "temper": 90 },
      { "mhz": 300,  "temper": 93 },
      { "mhz": 200,  "temper": 95 }
    ],
    "throttling_cfg" : {
        "emergency" : 100,
        "enable" : 1
    }
}
)";
    std::cout << "[Json format example]";
    std::cout << helpMessage;
}

CLICommand::CLICommand(cxxopts::ParseResult &cmd)
: _cmd(cmd)
{
    if (_cmd.count("device"))
    {
        _deviceId = _cmd["device"].as<int>();
    }
}

CLICommand::~CLICommand(void)
{
}

void CLICommand::Run(void)
{
    vector<shared_ptr<Device> > devices;

    if (_withDevice)
    {
        auto& devicesAll = CheckDevices(_checkDeviceSkip, _subCmd);
        if (_deviceId == -1)
        {
            devices = devicesAll;
        }
        else
        {
            devices = {devicesAll[_deviceId]};
        }
        for (auto &device : devices)
        {
            doCommand(device);
        }
    }
    else
    {
        doCommand(nullptr);
    }
}

DeviceStatusCLICommand::DeviceStatusCLICommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DeviceStatusCLICommand::doCommand(DevicePtr devicePtr)
{
    std::cout << DeviceStatus::GetCurrentStatus(devicePtr);
}

DeviceStatusMonitor::DeviceStatusMonitor(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DeviceStatusMonitor::doCommand(DevicePtr devicePtr)
{
    uint32_t delay = _cmd["monitor"].as<uint32_t>();
    if ( delay < 1 ) delay = 1;

    while (true) {
        DeviceStatus::GetCurrentStatus(devicePtr).StatusToStream(std::cout);
        std::this_thread::sleep_for(std::chrono::seconds(delay));
    }
}


DeviceInfoCLICommand::DeviceInfoCLICommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DeviceInfoCLICommand::doCommand(DevicePtr devicePtr)
{
    DeviceStatus::GetCurrentStatus(devicePtr).InfoToStream(std::cout);
}

FWVersionCommand::FWVersionCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = false;
}
void FWVersionCommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    std::ignore = devicePtr;
    string fwFile = _cmd["fwversion"].as<string>();
    cout << "fwFile:" << fwFile << endl;
    Fw fw(fwFile);
    fw.GetFwBinVersion();
}
DeviceResetCommand::DeviceResetCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
    _checkDeviceSkip = SkipMode::IDENTIFY_SKIP;
}
void DeviceResetCommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    int resetOpt = _cmd["reset"].as<int>();
    cout << "    Device " << devicePtr->id() << " reset by option " << resetOpt << endl;
    devicePtr->Reset(resetOpt);
}

FWUpdateCommand::FWUpdateCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd), _fwUpdateSubCmd(0)
{
    _withDevice = true;
    string path;
    for (const auto& cmd : _cmd["fwupdate"].as<vector<string>>())
    {
        if ((path = ParseFwUpdateSubCmd(cmd, &_fwUpdateSubCmd)) != "")
            _fwUpdateFile = path;
    }
    _checkDeviceSkip = SkipMode::VERSION_CHECK;
}

void FWUpdateCommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    Fw fw = Fw(_fwUpdateFile);
    cout << "    Device " << devicePtr->id() << " update firmware[" << fw.GetFwBinVersion() <<
        "] by " << _fwUpdateFile << ", SubCmd:" << _fwUpdateSubCmd;
    int ret = devicePtr->UpdateFw(_fwUpdateFile, _fwUpdateSubCmd);
    if (ret == 0) {
        cout << " : SUCCESS" << endl;
    } else {
        cout << " : FAIL (" << ret << ")" << endl;
        cout << " === firmware update fail reason === " << endl;
        cout << fw.GetFwUpdateResult(ret) << endl;
    }
}

FWUploadCommand::FWUploadCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
    _subCmd = dxrt::dxrt_ident_sub_cmd_t::DX_IDENTIFY_FWUPLOAD;
}

void FWUploadCommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    vector<string> fwUploadFiles = _cmd["fwupload"].as<vector<string>>();
    if (fwUploadFiles.size() != 2) {
        cout << "Please check firmware file" << endl;
        for (auto f : fwUploadFiles)
            cout << "file :" << f << endl;
    } else {
        for (auto f : fwUploadFiles) {
            cout << "    Device " << devicePtr->id() << " upload firmware by " << f << endl;
            devicePtr->UploadFw(f);
        }
    }
}


// function for cmds
vector<uint32_t> Dump(DevicePtr devicePtr)
{
    vector<uint32_t> dump(1000, 0);
    devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
    return dump;
}
void UpdateFwConfig(DevicePtr devicePtr, vector<uint32_t> cfg)
{
    int size = sizeof(uint32_t)*cfg.size();
    devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG, cfg.data(), size);
}
shared_ptr<FwLog> GetFwLog(DevicePtr devicePtr)
{
    vector<dxrt_device_log_t> logBuf(static_cast<int>(16 * 1024 / sizeof(dxrt_device_log_t)), {0, 0, {0, }});
    devicePtr ->Process(dxrt::dxrt_cmd_t::DXRT_CMD_GET_LOG, logBuf.data());
    auto fwlog = std::make_shared<FwLog>(logBuf);
    return fwlog;
}


DeviceDumpCommand::DeviceDumpCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DeviceDumpCommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;
    using std::hex;

    string dumpFileName = _cmd["dump"].as<string>();
    cout << "    Device " << devicePtr->id() << " dump to file " << dumpFileName << endl;
    auto dump = Dump(devicePtr);
    for (size_t i = 0; i < dump.size(); i+=2)
    {
        if (dump[i] == 0xFFFFFFFF) break;
        cout << hex << dump[i] << " : " << dump[i+1] << endl;
    }
    dxrt::DataDumpBin(dumpFileName, dump.data(), dump.size());
    dxrt::DataDumpTxt(dumpFileName+".txt", static_cast<uint32_t*>(dump.data()), 1, dump.size()/2, 2, true);
}

FWConfigCommand::FWConfigCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void FWConfigCommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    auto fwConfig = _cmd["fwconfig"].as<vector<uint32_t>>();
    cout << "    Device " << devicePtr->id() << " update firmware config by " << fwConfig.size() << endl;
    UpdateFwConfig(devicePtr, fwConfig);
}

FWConfigCommandJson::FWConfigCommandJson(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void FWConfigCommandJson::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    string fwConfigJson = _cmd["fwconfig_json"].as<string>();
    cout << "    Device " << devicePtr->id() << " update firmware config by " << fwConfigJson;
    int ret = devicePtr->UpdateFwConfig(fwConfigJson);

    if (ret == 0) {
        cout << " : SUCCESS" << endl;
    } else {
        cout << " : FAIL (" << ret << ")" << endl;
        HelpJsonConfig();
    }
}


FWLogCommand::FWLogCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;

    string logFileName = _cmd["fwlog"].as<string>();

    // create the file
    std::ofstream outputFile(logFileName);
    if (outputFile.is_open())
    {
        outputFile.close();
    }
}
void FWLogCommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    string logFileName = _cmd["fwlog"].as<string>();
    cout << "    Device " << devicePtr->id() << " get log to file " << logFileName << endl;
    auto fwLog = GetFwLog(devicePtr);

    // append log (device id + log)
    fwLog->SetDeviceInfoString("Device: " + std::to_string(devicePtr->id()));
    fwLog->ToFileAppend(logFileName);  // append
    cout << fwLog->str() << endl;
}

ShowVersionCommand::ShowVersionCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = false;
}
void ShowVersionCommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    std::ignore = devicePtr;
    cout << "DXRT " DXRT_VERSION << endl;
    cout << "Minimum Driver Versions" << endl;
    cout << "  Device Driver: v" << dxrt::LogMessages::ConvertIntToVersion(RT_DRV_VERSION_CHECK) << endl;
    cout << "  PCIe Driver: v" << dxrt::LogMessages::ConvertIntToVersion(PCIE_VERSION_CHECK) << endl;
    cout << "  Firmware: v" << dxrt::LogMessages::ConvertIntToVersion(FW_VERSION_CHECK) << endl;

    cout << "Minimum Compiler Versions" << endl;
    cout << "  Compiler: v" << MIN_COMPILER_VERSION << endl;
    cout << "  .dxnn File Format: v" << MIN_SINGLEFILE_VERSION << endl;
}


PcieStatusCLICommand::PcieStatusCLICommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void PcieStatusCLICommand::doCommand(DevicePtr devicePtr)
{
    std::cout << std::endl;
    devicePtr->ShowPCIEDetails();
}
DDRErrorCLICommand::DDRErrorCLICommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DDRErrorCLICommand::doCommand(DevicePtr devicePtr)
{
    using std::cout;
    using std::endl;

    cout << "Device " << devicePtr->id() << ": " << DeviceStatus::GetCurrentStatus(devicePtr).DdrBitErrStr() << endl;
}
}  // namespace dxrt

