// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "gtest/gtest.h"
#include "dxrt/inference_engine.h"
#include "dxrt_test.h"
#include "dxrt/filesys_support.h"

using namespace std;
using namespace dxrt;

TEST(rmap_info, basic)
{
    LOG_VALUE(testModelPath);
    if(dxrt::fileExists(testModelPath))
    {        
        auto model = dxrt::LoadModelParam(testModelPath);
        // EXPECT_TRUE(!rmapInfo.version().npu().empty());
        // EXPECT_TRUE(!rmapInfo.version().rmap().empty());
        // EXPECT_TRUE(!rmapInfo.version().rmapinfo().empty());
        // cout << rmapInfo.model() << endl;
    }
}

