/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/dxrt_cxx_api.h"
#include <iostream>
#include <string>


#ifdef __linux__
#include <getopt.h>

static struct option const opts[] = {
    { "model", required_argument, 0, 'm' },
    { "verbose", no_argument, 0, 'v' },
    { "output", required_argument, 0, 'o' },
    { "json", no_argument, 0, 'j' },
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 }
};
#endif

using std::cout;
using std::endl;
using std::string;

void help(const std::string& prog)
{
    cout << "parse model\n"
         << "Usage: " << prog << " [options]\n\n"
         << "Options:\n"
         << "  -m, --model FILE    model path (required)\n"
         << "  -v, --verbose       show detailed task dependencies and memory usage\n"
         << "  -o, --output FILE   save the raw console output to a file (without color codes)\n"
         << "  -j, --json          extract JSON binary data (graph_info, rmap_info) to files\n"
         << "  -h, --help          show this help message\n\n"
         << "Examples:\n"
         << "  " << prog << " -m model.dxnn\n"
         << "  " << prog << " -m model.dxnn -v\n"
         << "  " << prog << " -m model.dxnn -o output.txt\n"
         << "  " << prog << " -m model.dxnn -j    # Extracts model_graph_info.json, model_rmap_info_*.json\n"
         << endl;
}

int main(int argc, char *argv[])
{
    string prog_name(argv[0]);
    { auto s = prog_name.rfind('/');  if (s != string::npos) prog_name = prog_name.substr(s + 1); }
    { auto s = prog_name.rfind('\\'); if (s != string::npos) prog_name = prog_name.substr(s + 1); }

    int ret;
    string modelPath = "";
    bool verbose = false;
    bool json_extract = false;
    string outputFile = "";
    
    if (argc == 1)
    {
        cout << "Error: no arguments." << endl;
        help(prog_name);
        return -1;
    }

    
#ifdef __linux__
    int optCmd;
    while ((optCmd = getopt_long(argc, argv, "m:vo:jh", opts,
        NULL)) != -1) {
        switch (optCmd) {
            case '0':
                break;
            case 'm':
                modelPath = strdup(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                outputFile = strdup(optarg);
                break;
            case 'j':
                json_extract = true;
                break;
            case 'h':
            default:
                help(prog_name);
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
        else if (arg == "-v" || arg == "--verbose")
        {
            verbose = true;
        }
        else if (arg == "-o" || arg == "--output")
        {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            }
            else
            {
                std::cerr << "Error: -o option requires an argument." << endl;
                return -1;
            }
        }

        else if (arg == "-j" || arg == "--json")
        {
            json_extract = true;
        }
        else if (arg == "-h" || arg == "--help")
        {
            help(prog_name);
            return 0;
        }
    }
#endif

    if (modelPath.empty()) {
        cout << "Error: model path is required." << endl;
        help(prog_name);
        return -1;
    }

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

        // Create parse options
        dxrt::ParseOptions options;
        options.verbose = verbose;
        options.json_extract = json_extract;
        options.output_file = outputFile;
        options.no_color = !outputFile.empty(); // Disable color for file output

        ret = dxrt::ParseModel(modelPath, options);
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
