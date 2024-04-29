// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/testdata.h"
#include "dxrt/util.h"
#include "dxrt/filesys_support.h"

using namespace std;

namespace dxrt {

TestData::TestData(int id_, string inputFile_, vector<string> refOutputFile_, string outputFile_, string modelPath_, uint32_t inputSize, uint32_t outputSize)
:id(id_), inputFile(inputFile_), refOutputFile(refOutputFile_), outputFile(outputFile_), modelPath(modelPath_)
{
    input = vector<uint8_t>(inputSize, 0);
    DataFromFile(inputFile, input.data());
    for(auto &rf:refOutputFile)
    {
        size = getFileSize(rf);
        refOutput.emplace_back(move(vector<uint8_t>(size, 0)));
        if(static_cast<uint32_t>(size) > outputSize)
            type = 0;
        else
            type = 1;
        DataFromFile(rf, refOutput.back().data());
    }
    if(outputFile.empty())
    {
        outputFile = inputFile + ".failoutputdata";
    }
}
TestData::TestData() {}
TestData::~TestData() {}
void TestData::Show()
{
    cout << "  [" << id << "] " << type << ", "
        << inputFile << "(" << input.size() << " bytes) ->";
    for(auto &rf:refOutputFile)
    {
        cout << rf << "(" << size << " bytes) ";
    }
    cout << endl;
}

} // namespace dxrt
