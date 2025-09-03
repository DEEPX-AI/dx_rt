/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


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