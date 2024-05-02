#include "gtest/gtest.h"
#include "dxrt/device.h"
#include "dxrt_test.h"
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <execinfo.h>

using namespace std;
using namespace dxrt;

void FaultFunction()
{
    int *ptr = nullptr;
    *ptr = 1;
}

TEST(exception_handler, basic)
{
    // inject segmentation fault
    FaultFunction();
}
