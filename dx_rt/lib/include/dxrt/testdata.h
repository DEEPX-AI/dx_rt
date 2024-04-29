// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"

/** \brief DXRT C++ APIs are provided in this namespace
 * 
*/
namespace dxrt {

/** \brief model test data information
 * \headerfile "dxrt/dxrt_api.h"
*/
struct TestData
{
    TestData(int id_, std::string inputFile_, std::vector<std::string> refOutputFile_, std::string outputFile_, 
        std::string modelPath_, uint32_t inputSize, uint32_t outputSize);
    TestData();
    ~TestData();
    int id;
    std::vector<uint8_t> input;
    std::vector<std::vector<uint8_t>> refOutput;
    std::string inputFile="";
    std::vector<std::string> refOutputFile;
    std::string outputFile="";
    std::string modelPath="";
    int type; /* Bit match test type, 0: check output all, 1: check last output */
    int size;
    void Show();
};

} // namespace dxrt