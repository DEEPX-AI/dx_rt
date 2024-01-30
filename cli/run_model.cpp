#include <getopt.h>
#include <iostream>
#include <filesystem>
#include <regex>
#include "dxrt/dxrt_api.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"
#include "rapidjson/rapidjson.h"
using namespace std;
using namespace rapidjson;

/////////////////////////////////////////////////////////////////////////////////////////////////
static struct option const opts[] = {
    { "config", required_argument, 0, 'c' },
    { "model", required_argument, 0, 'm' },
    { "input", required_argument, 0, 'i' },
    { "output", required_argument, 0, 'o' },
    { "ref",  required_argument, 0, 'r' },
    { "loop", no_argument, 0, 'l' },
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 }
};
const char* usage =
"run model\n"
"  -c, --config    config json file\n"
"  -m, --model     model path\n"
"  -i, --input     input data file\n"
"  -o, --output    file to save output data\n"
"  -r, --ref       reference output data file to compare\n"
"  -l, --loop      loop test\n"
"  -h, --help      show help\n"
;
void help()
{
    cout << usage << endl;    
}

int main(int argc, char *argv[])
{
    int optCmd, loops = 1;
    atomic<int> bitMatchSequential = -1, bitMatchParallel = -1;
    string modelPath="", inputFile="", outputFile="output.bin", refOutputFile="";
    string configJsonFile="";
    string failLog;
    vector<dxrt::TestData> dataPool;
    float fps;
    if(argc==1)
    {
        cout << "Error: no arguments." << endl;
        help();
        return -1;
    }

    while ((optCmd = getopt_long(argc, argv, "c:m:i:o:r:l:h", opts,
        NULL)) != -1) {
        switch (optCmd) {
            case '0':
                break;
            case 'c':
                configJsonFile = strdup(optarg);
                break;
            case 'm':
                modelPath = strdup(optarg);
                break;
            case 'i':
                inputFile = strdup(optarg);
                break;
            case 'o':
                outputFile = strdup(optarg);
                break;
            case 'r':
                refOutputFile = strdup(optarg);
                break;
            case 'l':
                loops = stoi(optarg);
                break;
            case 'h':
            default:
                help();
                exit(0);
                break;
        }
    }
    LOG_VALUE(configJsonFile);
    LOG_VALUE(modelPath);
    LOG_VALUE(inputFile);
    LOG_VALUE(outputFile);
    LOG_VALUE(refOutputFile);
    if(!configJsonFile.empty())
    {
        // return dxrt::RunModels(configJsonFile); // TODO
        return 0;
    }
    else
    {
        DXRT_ASSERT(!modelPath.empty(), "no model path");
    }

    cout << "-----------------------------------" << dec << endl;
    cout << "loops : " << loops << endl;
    cout << "-----------------------------------" << dec << endl;
    auto ie = dxrt::InferenceEngine(modelPath);
    auto& profiler = dxrt::Profiler::GetInstance();
    cout << "Start benchmark." << endl;
    fps = ie.RunBenchMark(loops);
    cout << "Completed benchmark." << endl;
    /* Data Integrity Test - Setup */
    vector<string> refOutputFiles{refOutputFile};
    if(refOutputFile.empty()) refOutputFiles.clear();
    if(inputFile.empty() && refOutputFile.empty())
        dataPool = {};
    else
        dataPool = vector<dxrt::TestData>{dxrt::TestData(0, inputFile, refOutputFiles, outputFile, modelPath, ie.input_size(), ie.output_size())};

    /* Data Integrity Test - Sequential & Parallel */
    auto dataIntegrity = ie.RunDataIntegrityCheck(loops, dataPool);
    // auto dataIntegrity = pair<int, int>(0, 0);
    cout << "-----------------------------------" << dec << endl;
    cout << modelPath << endl;
    cout << "  Inference time : " << 1000./fps << "ms" << endl;
    cout << "  FPS : " << fps << endl;
    cout << "  Data Integrity Check : " 
        <<  (((dataIntegrity.first + dataIntegrity.second)==0)?"SKIP":(dataIntegrity.second>0?"FAIL":"OK"))
        << " (" << dataIntegrity.first << " pass, " << dataIntegrity.second << " fail )"
        << endl;
    cout << "-----------------------------------" << dec << endl;

    return 0;
}
