// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/cli.h"
#include "dxrt/device.h"
#include "dxrt/fwlog.h"
#include "dxrt/util.h"

using namespace std;

namespace dxrt {

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
        _fwUpdateFile = _cmd["fwupdate"].as<string>();
    }
}

CLI::~CLI(void)
{
}

void CLI::Run(void)
{
    vector<shared_ptr<Device>> devices;
    auto devicesAll = CheckDevices();
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
            cout << "    Device " << device->id() << ": " << device->status() << endl;
        }
        if(!_dumpFile.empty())
        {
            cout << "    Device " << device->id() << " dump to file " << _dumpFile << endl;
            auto dump = device->Dump();
            for(int i=0;i<dump.size(); i+=2)
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
            cout << "    Device " << device->id() << " update firmware by " << _fwUpdateFile;
            int ret = device->UpdateFw(_fwUpdateFile);
            if(ret==0) cout << " : SUCCESS" << endl;
            else cout << " : FAIL (" << ret << ")" << endl;
        }
        if(_resetOpt==0 || _resetOpt==1)
        {            
            cout << "    Device " << device->id() << " reset by option " << _resetOpt << endl;
            device->Reset(_resetOpt);
        }
    }

}

} // namespace dxrt
