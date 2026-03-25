/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/testdata.h"
#include "dxrt/util.h"
#include "dxrt/filesys_support.h"

#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::cout;
using std::endl;
using std::vector;

namespace dxrt {

TestData::TestData(int id_, const string& inputFile_, const vector<string>& refOutputFile_, const string& outputFile_,
    const string& modelPath_, uint32_t inputSize, uint32_t outputSize)
:id(id_), input(std::vector<uint8_t>(inputSize, 0)), inputFile(inputFile_), refOutputFile(refOutputFile_),
    outputFile(outputFile_), modelPath(modelPath_)
{
    DataFromFile(inputFile, input.data());
    for (const auto &rf : refOutputFile)
    {
        size = static_cast<int>(getFileSize(rf));
        refOutput.emplace_back(size, 0);
        if (static_cast<uint32_t>(size) > outputSize)
            type = 0;
        else
            type = 1;
        DataFromFile(rf, refOutput.back().data());
    }
    if (outputFile.empty())
    {
        outputFile = inputFile + ".failoutputdata";
    }
}
TestData::TestData() = default;
TestData::~TestData() = default;
void TestData::Show() const
{
    cout << "  [" << id << "] " << type << ", "
        << inputFile << "(" << input.size() << " bytes) ->";
    for (const auto &rf : refOutputFile)
    {
        cout << rf << "(" << size << " bytes) ";
    }
    cout << endl;
}

}  // namespace dxrt
