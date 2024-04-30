// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "cxxopts.hpp"
#include "dxrt/filesys_support.h"

using namespace std;

#define APP_NAME "DXRT " DXRT_VERSION " run_model"

int main(int argc, char *argv[])
{
    string modelFile, inputFile, outputFile;
    bool benchmark = false;
    int loops = 1;
    cxxopts::Options options("run_model", APP_NAME);
    options.add_options()
        (  "m, model", "Model file (.dxnn)" , cxxopts::value<string>(modelFile) )
        (  "i, input", "Input data file", cxxopts::value<string>(inputFile) )
        (  "o, output", "Output data file", cxxopts::value<string>(outputFile)->default_value("output.bin") )
        (  "b, benchmark", "Perform benchmark test", cxxopts::value<bool>(benchmark)->default_value("false") )
        (  "l, loops", "Loops to test", cxxopts::value<int>(loops)->default_value("1") )
        (  "h, help", "Print usage" )
    ;    

    auto cmd = options.parse(argc, argv);
    if(cmd.count("help"))
    {        
        cout << options.help() << endl;
        exit(0);
    }
    LOG_VALUE(modelFile);
    LOG_VALUE(inputFile);
    LOG_VALUE(outputFile);
    LOG_VALUE(benchmark);
    LOG_VALUE(loops);

    dxrt::InferenceEngine ie(modelFile);
    if(benchmark)
    {
        auto fps = ie.RunBenchMark(loops);
        cout << "-----------------------------------" << dec << endl;
        cout << modelFile << endl;
        cout << "  Inference time : " << 1000./fps << "ms" << endl;
        cout << "  FPS : " << fps << endl;
        cout << "-----------------------------------" << dec << endl;
    }
    else
    {
        vector<uint8_t> inputBuf(ie.input_size(), 0);
        if(!inputFile.empty())
        {
            DXRT_ASSERT(ie.input_size()==static_cast<uint64_t>(dxrt::getFileSize(inputFile)), "input file size mismatch");
            dxrt::DataFromFile(inputFile, inputBuf.data());
        }
        for(int i=0; i<loops; i++)
        {
            auto outputs = ie.Run(inputBuf.data());
            dxrt::DataDumpBin(outputFile, outputs.front()->data(), ie.output_size()); /* TODO: sparse tensor */
            cout << "-----------------------------------" << dec << endl;
            cout << inputFile << " -> " << outputFile << endl;
            cout << "  Latency : " << ie.latency() << "us" << endl;
            cout << "  Inference time : " << ie.inference_time() << "us" << endl; /* TODO */
            cout << "-----------------------------------" << dec << endl;
        }
    }
    
    return 0;
}