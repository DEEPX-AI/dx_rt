// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include <iostream>
#include "dxrt/common.h"

int dxrt_service_main();
int main(int argc, char* argv[])
{
    std::ignore = argc;
    std::ignore = argv;
#ifdef USE_SERVICE
    return dxrt_service_main();
#else
    DXRT_ASSERT(false, "USE_SERVICE is not set, so dxrt_service will not work");
#endif
}