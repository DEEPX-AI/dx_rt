/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/dxrt_api.h"
#include "dxrt/exception/exception.h"
#include "dxrt/device_info_status.h"
#include "dxrt/device_util.h"
#include <iostream>
#include <string>


#ifdef __linux__
#include <getopt.h>

static struct option const opts[] = {
    { "model", required_argument, 0, 'm' },
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 }
};
#endif

using std::cout;
using std::endl;
using std::string;

const char* usage = "parse model\n"
                    "  -m, --model     model path\n"
                    "  -h, --help      show help\n";

void help()
{
    cout << usage << endl;
}

int main(int argc, char *argv[])
{
    int ret;
    string modelPath = "";
    if (argc ==1)
    {
        cout << "Error: no arguments." << endl;
        help();
        return -1;
    }

#ifdef __linux__
    int optCmd;
    while ((optCmd = getopt_long(argc, argv, "m:h", opts,
        NULL)) != -1) {
        switch (optCmd) {
            case '0':
                break;
            case 'm':
                modelPath = strdup(optarg);
                break;
            case 'h':
            default:
                help();
                exit(0);
                break;
        }
    }
#elif _WIN32
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-m" || arg == "--model") {
            if (i + 1 < argc) {
                modelPath = argv[++i];
            }
            else
            {
                std::cerr << "Error: -m option requires an argument." << endl;
                return -1;
            }
        }
        else if (arg == "-h" || arg == "--help")
        {
            help();
            return 0;
        }
    }
#endif

    LOG_VALUE(modelPath);

    try {
        /*auto& devices = dxrt::CheckDevices();
        if (!devices.empty()) {
            auto& device = devices[0];
            auto devStatus = dxrt::DeviceStatus::GetCurrentStatus(device);
            const auto& devInfo = devStatus.Info();
            const auto& devDrvInfo = device->devInfo();

            cout << "=======================================================" << endl;
            cout << " * Device 0             : " << devStatus.DeviceTypeStr() << endl;
            cout << "====================  Version  ========================" << endl;
            cout << " * DXRT version         : " << DXRT_VERSION << endl;
            cout << "-------------------------------------------------------" << endl;
            cout << " * RT Driver version    : v" << dxrt::GetDrvVersionWithDot(devDrvInfo.rt_drv_ver) << endl;
            cout << " * PCIe Driver version  : v" << dxrt::GetDrvVersionWithDot(devDrvInfo.pcie.driver_version) << endl;
            cout << "-------------------------------------------------------" << endl;
            cout << " * FW version           : v" << dxrt::GetFwVersionWithDot(devInfo.fw_ver) << endl;
            cout << "=======================================================" << endl;
        }*/

        ret = dxrt::ParseModel(modelPath);
    }
    catch (const dxrt::Exception& e)
    {
        std::cerr << e.what() << " error-code=" << e.code() << std::endl;
        return -1;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cerr << "Exception" << std::endl;
        return -1;
    }
    
    return ret;
}
