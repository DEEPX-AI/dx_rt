// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#ifdef __linux__
#include <getopt.h>
#endif
#include <vector>
#include <sstream>
#include "dxrt/dxrt_api.h"
#include "dxrt/cli.h"
#include "cxxopts.hpp"
#include "dxrt/exception/exception.h"


using namespace std;

int main(int argc, char *argv[])
{
    cxxopts::Options options("dxrt-cli", "DXRT " DXRT_VERSION " CLI");
    options.add_options()
        ("s, status", "Get device status")
        ("i, info", "Get device info")
        ("m, monitor", "Monitoring device status every [arg] seconds",cxxopts::value<uint32_t>() )
        ("r, reset", "Reset device(0: reset only NPU, 1: reset entire device)", cxxopts::value<int>()->default_value("0"))
        ("d, device", "Device ID (if not specified, CLI commands will be sent to all devices.)", cxxopts::value<int>()->default_value("-1"))
        ("u, fwupdate", "Update firmware with deepx firmware file.\nsub-option : [force:force update, unreset:device unreset(default:reset)]", cxxopts::value<std::vector<std::string>>())
        ("w, fwupload", "Upload firmware with deepx firmware file.[2nd_boot/rtos]", cxxopts::value<std::vector<std::string>>() )
        ("g, fwversion", "Get firmware version with deepx firmware file", cxxopts::value<string>())
        ("p, dump", "Dump device internals to a file", cxxopts::value<string>() )
        ("l, fwlog", "Extract firmware logs to a file", cxxopts::value<string>() )
        ("C, fwconfig_json", "Update firmware settings from [JSON]", cxxopts::value<string>())
        ("v, version", "Print minimum versions")

        ("h, help", "Print usage");
    try{
        auto cmd = options.parse(argc, argv);
        if (cmd.count("help"))
        {
            cout << "DXRT " DXRT_VERSION << endl;
            cout << options.help() << endl;
            exit(0);
        }
        else if (cmd.count("status"))
        {
            dxrt::DeviceStatusCLICommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("info"))
        {
            dxrt::DeviceInfoCLICommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("monitor"))
        {
            dxrt::DeviceStatusMonitor cli(cmd);
            cli.Run();
        }
        else if (cmd.count("reset"))
        {
            dxrt::DeviceResetCommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("fwupdate"))
        {
            dxrt::FWUpdateCommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("fwupload"))
        {
            dxrt::FWUploadCommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("fwversion"))
        {
            dxrt::FWVersionCommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("dump"))
        {
            dxrt::DeviceDumpCommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("fwconfig"))
        {
            dxrt::FWConfigCommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("fwconfig_json"))
        {
            dxrt::FWConfigCommandJson cli(cmd);
            cli.Run();
        }
        else if (cmd.count("fwlog"))
        {
            dxrt::FWLogCommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("version"))
        {
            dxrt::ShowVersionCommand cli(cmd);
            cli.Run();
        }
        else 
        {
            cout << options.help() << endl;
        }

        return 0;
    }
    catch(cxxopts::exceptions::exception& e)
    {
        cout << e.what() << endl;
    }
    catch(const dxrt::Exception& e)
    {
        cout << e.what() << endl;
    }
    catch(const std::exception& e)
    {
        cout << e.what() << endl;
    }
    
    return 0;
}