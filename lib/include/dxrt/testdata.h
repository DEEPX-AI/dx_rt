/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"

/** \brief DXRT C++ APIs are provided in this namespace
 *
*/
namespace dxrt {

/// @cond
/** \brief model test data information
 * \headerfile "dxrt/dxrt_api.h"
*/
/// @endcond
struct DXRT_API TestData
{
    TestData(int id_, const std::string& inputFile_, const std::vector<std::string>& refOutputFile_, const std::string& outputFile_,
        const std::string& modelPath_, uint32_t inputSize, uint32_t outputSize);
    TestData();
    ~TestData();
    int id = 0;
    std::vector<uint8_t> input = {};
    std::vector<std::vector<uint8_t>> refOutput = {};
    std::string inputFile = "";
    std::vector<std::string> refOutputFile = {};
    std::string outputFile = "";
    std::string modelPath = "";
    int type = 0; /* Bit match test type, 0: check output all, 1: check last output */
    int size = 0;
    void Show() const;
};

} // namespace dxrt
