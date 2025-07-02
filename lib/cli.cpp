// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/cli.h"
#include "dxrt/device.h"
#include "dxrt/fw.h"
#include "dxrt/util.h"
#include "dxrt/device_info_status.h"
#include "dxrt/filesys_support.h"
#include "dxrt/driver.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "dxrt/objects_pool.h"
#include "../lib/resource/log_messages.h"
#include "dxrt/device_version.h"

using namespace std;
using namespace rapidjson;

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
            cout << "[ERR] Unknown sub-command or not found file path: " << cmd << endl;
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
    cout << "[Json format example]";
    cout << helpMessage;
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
    vector<shared_ptr<Device>> devices;

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
    cout << DeviceStatus::GetCurrentStatus(devicePtr);
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
        DeviceStatus::GetCurrentStatus(devicePtr).StatusToStream(cout);
        std::this_thread::sleep_for(chrono::seconds(delay));
    }
}

DeviceMonitorDebug::DeviceMonitorDebug(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DeviceMonitorDebug::doCommand(DevicePtr devicePtr)
{
    uint32_t delay = _cmd["monitor_debug"].as<uint32_t>();
    while (true) {
        auto deviceStatus = DeviceStatus::GetCurrentStatus(devicePtr);
        deviceStatus.StatusToStream(cout);
        deviceStatus.DebugStatusToStream(cout);
        std::this_thread::sleep_for(chrono::seconds(delay));
    }
}

DeviceInfoCLICommand::DeviceInfoCLICommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DeviceInfoCLICommand::doCommand(DevicePtr devicePtr)
{
    DeviceStatus::GetCurrentStatus(devicePtr).InfoToStream(cout);
}

FWVersionCommand::FWVersionCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = false;
}
void FWVersionCommand::doCommand(DevicePtr devicePtr)
{
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
    int resetOpt = _cmd["reset"].as<int>();
    cout << "    Device " << devicePtr->id() << " reset by option " << resetOpt << endl;
    devicePtr->Reset(resetOpt);
}

FWUpdateCommand::FWUpdateCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
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




//function for cmds
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
    vector<dxrt_device_log_t> logBuf((int)16*1024/sizeof(dxrt_device_log_t), {0, 0, {0, }});
    devicePtr ->Process(dxrt::dxrt_cmd_t::DXRT_CMD_GET_LOG, logBuf.data());
    auto fwlog = make_shared<FwLog>(logBuf);
    return fwlog;
}


DeviceDumpCommand::DeviceDumpCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DeviceDumpCommand::doCommand(DevicePtr devicePtr)
{
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

DDRTargetCommand::DDRTargetCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void DDRTargetCommand::doCommand(DevicePtr devicePtr)
{
    uint32_t targetFreq = _cmd["ddrtarget"].as<uint32_t>();
    vector<uint32_t> supported_freq = {5200, 5400, 5600, 5800, 6000, 6200, 6400};

    if (find(supported_freq.begin(), supported_freq.end(), targetFreq) != supported_freq.end()) {
        cout << "   Target LPDDR Frequency: " << targetFreq << "Mhz" << endl;
        devicePtr->DoCustomCommand(&targetFreq, dxrt::DX_SET_DDR_FREQ);
    }
    else {
        cout << "ERROR: Unsupported DDR Frequency : << " << targetFreq << "Mhz" << endl;
    }

    return;
}

OTPCommand::OTPCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}

#define OTP_REGION_SIZE  (16)
#define OTP_VALUE_LENGTH (13)
void OTPCommand::doCommand(DevicePtr devicePtr)
{
    string otpValue = _cmd["otp"].as<string>();
    if (otpValue == "GET")
    {
        otp_info_t otp;
        devicePtr->DoCustomCommand(&otp, dxrt::DX_GET_OTP);
        cout << otp << endl;
    } 
    else
    {
        if (otpValue.size() != OTP_VALUE_LENGTH)
        {
            cerr << "Error: OTP value must be exactly " << OTP_VALUE_LENGTH << " characters long/short." << endl;
            return;
        }

        char otpCharValue[OTP_REGION_SIZE] = {}; // Null-terminated safety
        copy_n(otpValue.c_str(), OTP_VALUE_LENGTH, otpCharValue);

        cout << "Setting OTP data to: " << otpValue << endl;
        devicePtr->DoCustomCommand(&otpCharValue, dxrt::DX_SET_OTP, OTP_REGION_SIZE);
        switch (otpCharValue[0])
        {
            case -1:
                cerr << "ERROR: OTP region of barcode is full" << endl;
                break;
            case -2:
                cerr << "ERROR: OTP Write/Read data mismatch" << endl;
                break;
            default:
                cout << "OTP data successfully written." << endl;
                break;
        }
    }
}

FWLogCommand::FWLogCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = true;
}
void FWLogCommand::doCommand(DevicePtr devicePtr)
{
    string logFileName = _cmd["fwlog"].as<string>();
    cout << "    Device " << devicePtr->id() << " get log to file " << logFileName << endl;
    auto fwLog = GetFwLog(devicePtr);
    fwLog->toFile(logFileName);
    cout << fwLog->str() << endl;
}

ShowVersionCommand::ShowVersionCommand(cxxopts::ParseResult &cmd)
: CLICommand(cmd)
{
    _withDevice = false;
}
void ShowVersionCommand::doCommand(DevicePtr devicePtr)
{
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

}  // namespace dxrt

