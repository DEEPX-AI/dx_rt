// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/cli.h"
#include "dxrt/device.h"
#include "dxrt/fw.h"
#include "dxrt/util.h"
#include "dxrt/device_info_status.h"
#include "dxrt/filesys_support.h"
#include "dxrt/driver.h"

using namespace std;

namespace dxrt {

static uint32_t fwUpdateSubCmd;
static string ParseFwUpdateSubCmd(string cmd)
{
    string path = getPath(cmd);
    if ( !fileExists(path) ) {
        path = "";
        if (cmd == "reset") {
            fwUpdateSubCmd |= FWUPDATE_DEV_RESET;
        } else if (cmd == "force") {
            fwUpdateSubCmd |= FWUPDATE_FORCE;
        } else {
            cout << "[ERR] Unknown sub-command or not found file path: " << cmd << endl;
            exit(-1);
        }
    }
    return path;
}

CLI::CLI(cxxopts::ParseResult &cmd)
: _cmd(cmd)
{
    if(_cmd.count("status"))
    {
        _printDeviceStatus = true;
    }
    if(_cmd.count("reset"))
    {
        _resetOpt = _cmd["reset"].as<int>();
    }
    if(_cmd.count("device"))
    {
        _deviceId = _cmd["device"].as<int>();
    }
    if(_cmd.count("dump"))
    {
        _dumpFile = _cmd["dump"].as<string>();
    }
    if(_cmd.count("fwconfig"))
    {
        _fwConfig = _cmd["fwconfig"].as<vector<uint32_t>>();
    }
    if(_cmd.count("fwlog"))
    {
        _fwLogFile = _cmd["fwlog"].as<string>();
    }
    if(_cmd.count("fwupdate"))
    {
        string path;
        for (const auto& cmd : _cmd["fwupdate"].as<vector<string>>())
        {
            if ((path = ParseFwUpdateSubCmd(cmd)) != "")
                _fwUpdateFile = path;
        }
    }
    if(_cmd.count("info"))
    {
        _printDeviceInfo = true;
    }
    if(_cmd.count("fwversion"))
    {
        _fwFile = _cmd["fwversion"].as<string>();
    }
}

CLI::~CLI(void)
{
}

void CLI::Run(void)
{
    vector<shared_ptr<Device>> devices;
    bool withDev = true;
    /* Option condition is without device */
    if(!_fwFile.empty())
    {
        Fw fw(_fwFile);
        fw.GetFwBinVersion();
        withDev = false;
    }
    if (withDev)
    {
        auto devicesAll = CheckDevices((fwUpdateSubCmd & FWUPDATE_FORCE) ? true : false);
        if(_deviceId==-1)
        {
            devices = devicesAll;
        }
        else
        {
            devices = {devicesAll[_deviceId]};
        }
        for(auto &device:devices)
        {
            if(_printDeviceStatus)
            {
                cout << DxrtDeviceInfoWithStatus::getStatusInfo(device);
            }
            if(!_dumpFile.empty())
            {
                cout << "    Device " << device->id() << " dump to file " << _dumpFile << endl;
                auto dump = device->Dump();
                for(size_t i=0;i<dump.size(); i+=2)
                {
                    if(dump[i]==0xFFFFFFFF) break;
                    cout << hex << dump[i] << " : " << dump[i+1] << endl;
                }
                dxrt::DataDumpBin(_dumpFile, dump.data(), dump.size());            
                dxrt::DataDumpTxt(_dumpFile+".txt", (uint32_t*)dump.data(), 1, dump.size()/2, 2, true);
            }
            if(!_fwLogFile.empty())
            {
                cout << "    Device " << device->id() << " get log to file " << _fwLogFile << endl;
                auto fwLog = device->GetFwLog();
                fwLog->toFile(_fwLogFile);
                cout << fwLog->str() << endl;
            }
            if(!_fwConfig.empty())
            {
                cout << "    Device " << device->id() << " update firmware config by " << _fwConfig.size() << endl;
                device->UpdateFwConfig(_fwConfig);
            }
            if(!_fwUpdateFile.empty())
            {
                Fw fw = Fw(_fwUpdateFile);
                cout << "    Device " << device->id() << " update firmware[" << fw.GetFwBinVersion() <<
                    "] by " << _fwUpdateFile << ", SubCmd:" << fwUpdateSubCmd;
                int ret = device->UpdateFw(_fwUpdateFile, fwUpdateSubCmd);
                if(ret==0) {
                    cout << " : SUCCESS" << endl;
                } else {
                    cout << " : FAIL (" << ret << ")" << endl;
                    cout << " === firmware update fail reason === " << endl;
                    cout << fw.GetFwUpdateResult(ret) << endl;
                }
            }
            if(_resetOpt==0 || _resetOpt==1)
            {
                cout << "    Device " << device->id() << " reset by option " << _resetOpt << endl;
                device->Reset(_resetOpt);
            }
            if(_printDeviceInfo)
            {
                cout << "Device "<< device->id()<<":";
                DxrtDeviceInfoWithStatus::getStatusInfo(device).infoToStream(cout);
                cout<<endl;
            }
        }
    }
}

} // namespace dxrt
