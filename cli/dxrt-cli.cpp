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

#ifdef __linux__
#include <getopt.h>
#endif
#include <iostream>
#include <vector>
#include "dxrt/dxrt_api.h"
#include "dxrt/cli.h"
#include "dxrt/extern/cxxopts.hpp"
#include "dxrt/exception/exception.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
    std::cout << "DXRT v" << dxrt::Configuration::GetInstance().GetVersion() << std::endl;

    cxxopts::Options options("dxrt-cli", "DXRT v" + dxrt::Configuration::GetInstance().GetVersion() + " CLI");
    options.add_options()
        ("s, status", "Get device status")
        ("i, info", "Get device info")
        ("m, monitor", "Monitoring device status every [arg] seconds (arg > 0)",cxxopts::value<uint32_t>() )
        //("r, reset", "Reset device(0: reset only NPU, 1: reset entire device)", cxxopts::value<int>()->default_value("0"))
        ("r, reset", "Reset device(0: reset only NPU)", cxxopts::value<int>()->default_value("0")->implicit_value("0"))
        ("d, device", "Device ID (if not specified, CLI commands will be sent to all devices.)", cxxopts::value<int>()->default_value("-1"))
        ("u, fwupdate", "Update firmware with deepx firmware file.\nsub-option : [force:force update, unreset:device unreset(default:reset)]", cxxopts::value<std::vector<std::string>>())

        ("g, fwversion", "Get firmware version with deepx firmware file", cxxopts::value<string>())
        ("C, fwconfig_json", "Update firmware settings from [JSON]", cxxopts::value<string>())
        ("v, version", "Print minimum versions")


        ("h, help", "Print usage");

    options.add_options("internal") //
        ("l, fwlog", "Extract firmware logs to a file", cxxopts::value<std::string>())
        ("p, dump", "Dump device internals to a file", cxxopts::value<string>() )
        ("w, fwupload", "Upload firmware with deepx firmware file.[2nd_boot/rtos]", cxxopts::value<std::vector<std::string>>() )
        ("errorstat", "show internal error status")
        ("ddrerror", "show ddr error count")
        ("check-h1", "check h1 status")
        ("check-m1", "check m1 status")
        ("check-m1m", "check m1m status");

    try
    {
        auto cmd = options.parse(argc, argv);
        if (cmd.count("help"))
        {
            cout << options.help({""}) << endl;

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
            if (cmd["reset"].as<int>() == 1)
            {
                cout << "Option reset 1 refers to resetting the entire device" << endl;
                cout << "It may cause undesired behavior and currently disabled." << endl;
                cout << "Would you like to continue? (yes/no): ";

                string user_input;
                std::getline(std::cin, user_input);

                // Convert to lowercase for case-insensitive comparison
                for(auto& c : user_input) c = std::tolower(c);

                if (user_input != "yes" && user_input != "y")
                {
                    cout << "Reset operation cancelled." << endl;
                    return 0;
                }
            }
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
        else if (cmd.count("errorstat"))
        {
            dxrt::PcieStatusCLICommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("ddrerror"))
        {
            dxrt::DDRErrorCLICommand cli(cmd);
            cli.Run();
        }
        else if (cmd.count("check-h1"))
        {
            if ( dxrt::CheckH1Devices() ) {
                cout << "H1 devices are properly recognized." << endl;
            } else {
                cout << "H1 devices are NOT properly recognized." << endl;
                return 1;
            }   
        }
        else if (cmd.count("check-m1"))
        {
            if ( dxrt::CheckM1Devices(dxrt::CHECK_M1_DEVICE) ) {
                cout << "M1 devices are properly recognized." << endl;
            } else {
                cout << "M1 devices are NOT properly recognized." << endl;
                return 1;
            }   
        }
        else if (cmd.count("check-m1m"))
        {
            if ( dxrt::CheckM1Devices(dxrt::CHECK_M1M_DEVICE) ) {
                cout << "M1M devices are properly recognized." << endl;
            } else {
                cout << "M1M devices are NOT properly recognized." << endl;
                return 1;
            }   
        }
        else
        {
            cout << options.help({""}) << endl;
        }

        return 0;
    }
    catch(cxxopts::exceptions::exception& e)
    {
        cout << e.what() << endl;
        cout << options.help({""}) << endl;
    }
    catch(const dxrt::Exception& e)
    {
        cout << e.what() << endl;
    }
    catch(const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}
