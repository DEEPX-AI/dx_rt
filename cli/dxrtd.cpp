// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.


#include "dxrt/common.h"
#include "dxrt/service_util.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
int dxrt_service_main(int argc, char* argv[]);


int main(int argc, char* argv[])
{
    std::ignore = argc;
    std::ignore = argv;
    if (dxrt::isDxrtServiceRunning())
    {
        std::cout << "Other instance of dxrtd is running" << std::endl;
        return -1;
    }
#ifdef USE_SERVICE
    return dxrt_service_main(argc, argv);
#else
    std::cout << "USE_SERVICE is not set, so dxrt_service will not work" << std::endl;
    return -1;
#endif
}