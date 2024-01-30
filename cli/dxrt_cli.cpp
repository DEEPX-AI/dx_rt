#include <getopt.h>
#include <vector>
#include <sstream>
#include "dxrt/dxrt_api.h"
#include "cxxopts.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cxxopts::Options options("dxrt-cli", "DXRT "DXRT_VERSION" CLI");
    options.add_options()
        (  "s, status", "Get device status" )
        (  "r, reset", "Reset device(0: reset only NPU, 1: reset entire device)", cxxopts::value<int>()->default_value("0") )
        (  "d, device", "Device ID (if not specified, CLI commands will be sent to all devices.)", cxxopts::value<int>()->default_value("-1") )
        (  "p, dump", "Dump device internals to a file", cxxopts::value<string>() )
        (  "c, fwconfig", "Update firmware settings from list of parameters", cxxopts::value<vector<uint32_t>>() )
        (  "l, fwlog", "Extract firmware logs to a file", cxxopts::value<string>() )
        (  "u, fwupdate", "Update firmware", cxxopts::value<string>() )
        (  "h, help", "Print usage" )
    ;

    auto cmd = options.parse(argc, argv);
    if(cmd.count("help"))
    {
        cout << "DXRT "DXRT_VERSION << endl;
        cout << options.help() << endl;
        exit(0);
    }
    auto cli = dxrt::CLI(cmd);
    cli.Run();

    return 0;
}