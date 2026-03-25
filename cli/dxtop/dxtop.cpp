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

#include "dxrt/extern/cxxopts.hpp"
#include "dxrt/exception/exception.h"

#include "core/version.h"
#include "core/npu_monitor.h"
#include "core/data_source/service_data_source.h"
#include "core/data_source/noservice_data_source.h"
#include "core/input_provider/input_provider.h"
#include "core/view/renderer.h"


#include "core/input_provider/linux_input_provider.h"
#include "core/view/linux_renderer.h"
#include "dxrt/service_util.h"


int main(int argc, char *argv[])
{
    cxxopts::Options options("dxtop", "DX-TOP " DX_TOP_VERSION);
    options.add_options()
        ("h, help", "Print usage");

    try{
        auto cmd = options.parse(argc, argv);
        if (cmd.count("help"))
        {
            std::cout << options.help() << std::endl;
            exit(0);
        }
        
#ifdef USE_SERVICE
        bool serviceRunning = dxrt::isDxrtServiceRunning();
        if (!serviceRunning)
        {
            std::cerr << "Error: dxrt.service is not running\n"
                      << "\n"
                      << "This build of dxtop requires dxrt.service.\n"
                      << "Please start the service with one of the following:\n"
                      << "  sudo systemctl start dxrt.service\n"
                      << "  sudo service dxrt start\n"
                      << "\n"
                      << "Alternatively, rebuild dxtop without USE_SERVICE for standalone mode:\n"
                      << "  cmake -DUSE_SERVICE=OFF ..\n"
                      << std::endl;
            exit(1);
        }
        
        dxrt::ServiceDataSource dataSource;
        if (!dataSource.IsAvailable())
        {
            std::cerr << "Error: dxrt.service is running but not responding properly\n"
                      << "Try restarting the service: sudo systemctl restart dxrt.service"
                      << std::endl;
            exit(1);
        }
#else
        dxrt::NoServiceDataSource dataSource;
#endif
        
        dxrt::NpuMonitor monitor(dataSource);
        dxrt::NcursesRenderer renderer;
        dxrt::NcursesInputProvider inputProvider;
        
        monitor.Initialize(renderer);
        monitor.Run(inputProvider, renderer);
    }

    catch(cxxopts::exceptions::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    catch(const dxrt::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
