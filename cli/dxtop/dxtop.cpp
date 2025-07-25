// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/extern/cxxopts.hpp"
#include "dxrt/exception/exception.h"

#include "core/version.h"
#include "core/npu_monitor.h"
#include "core/input_provider/input_provider.h"
#include "core/view/renderer.h"

#ifdef __linux__
    #include "core/input_provider/linux_input_provider.h"
    #include "core/view/linux_renderer.h"
#endif

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

        dxrt::NpuMonitor monitor;

#ifdef __linux__
        dxrt::NcursesRenderer renderer;
        dxrt::NcursesInputProvider inputProvider;
#else
        static_assert(false, "Renderer implementation is missing for this platform.");
#endif
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
