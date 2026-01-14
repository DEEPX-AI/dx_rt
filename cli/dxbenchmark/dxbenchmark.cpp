// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.
#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <memory>

#include "dxrt/dxrt_api.h"
#include "dxrt/extern/cxxopts.hpp"
#include "dxrt/filesys_support.h"
#include "dxrt/device_info_status.h"
#include "dxrt/profiler.h"

#include "core/include/utils.h"
#include "core/include/render.h"
#include "core/include/runner.h"


#define APP_NAME "DXRT " DXRT_VERSION " dxbenchmark"
// #define TARGET_FPS_DEBUG

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::string;

static int bounding = 0;

int main(int argc, char *argv[])
{
    // always showing the model information
    dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::SHOW_MODEL_INFO, true);

    string modelDir;
    int loops;
    int time;
    int warmup;
    string devices_spec;
    bool use_ort = false;
    bool verbose = false;
    string order;
    string criteria = "name";
    string result_path;
    bool only_data;
    bool recursive;

    cxxopts::Options options("dxbenchmark", APP_NAME);
    options.add_options()
        ("dir", "Model directory" , cxxopts::value<string>(modelDir))
        ("result-path", "Destination of result file" , cxxopts::value<string>(result_path)->default_value("."))
        ("sort",
            "Sorting criteria\n"
            "  name: Model Name\n  fps: FPS\n  time: NPU Inference Time\n  latency: Latency" , cxxopts::value<string>(criteria)->default_value("name"))
        ("order", "Sorting order\n" "  asc: Ascending order\n  desc: Descending order", cxxopts::value<string>(order)->default_value("asc"))
        ("l, loops", "Number of inference loops to perform", cxxopts::value<int>(loops)->default_value("0") )
        ("t, time", "Time duration to perform", cxxopts::value<int>(time)->default_value("0") )
        ("warmup", "Warmup time", cxxopts::value<int>(warmup)->default_value("10") )
        ("n, npu",
            "NPU bounding (default:0)\n"
            "  0: NPU_ALL\n  1: NPU_0\n  2: NPU_1\n  3: NPU_2\n"
            "  4: NPU_0/1\n  5: NPU_1/2\n  6: NPU_0/2", cxxopts::value<int>(bounding) )
        ("d, devices",
            "Specify target NPU devices.\nExamples:\n"
            "  'all' (default): Use all available/bound NPUs\n"
            "  '0': Use NPU0 only\n"
            "  '0,1,2': Use NPU0, NPU1, and NPU2\n"
            "  'count:N': Use the first N NPUs\n  (e.g., 'count:2' for NPU0, NPU1)",
            cxxopts::value<std::string>(devices_spec)->default_value("all"))
        ("only-data", "Make only data result (csv, json)", cxxopts::value<bool>(only_data)->default_value("false"))
        ("recursive", "Search models recursively in subdirectories (default: base directory only)", cxxopts::value<bool>(recursive)->default_value("false"))
#ifdef USE_ORT
        ("use-ort", "Enable ONNX Runtime for CPU tasks in the model graph\nIf disabled, only NPU tasks operate", cxxopts::value<bool>(use_ort)->default_value("false"))
#endif
        ("v, verbose", "Shows NPU Processing Time and Latency", cxxopts::value<bool>(verbose)->default_value("false"))
        ("h, help", "Print usage" );

    try
    {
        auto cmd = options.parse(argc, argv);
        if (cmd.count("help"))
        {
            cout << options.help() << endl;
            exit(0);
        }

        if (cmd.count("dir") == 0)
        {
            cout << "Model directory is required" << endl;
            cout << options.help() << endl;
            exit(1);
        }

        if (cmd.count("loops") == 0 && cmd.count("time") == 0)
        {
            cout << "Either loops or time duration must be specified." << endl;
            cout << options.help() << endl;
            exit(1);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        cout << options.help() << endl;
        exit(0);
    }

    try
    {
        std::cout << "Runtime Framework Version: v" << dxrt::Configuration::GetInstance().GetVersion() << std::endl; 
        std::cout << "Device Driver Version: v" << dxrt::Configuration::GetInstance().GetDriverVersion() << std::endl; 
        std::cout << "PCIe Driver Version: v" << dxrt::Configuration::GetInstance().GetPCIeDriverVersion() << std::endl; 
        std::cout << std::endl;
    } 
    catch (const dxrt::Exception &e)
    {
        std::cout << e.what() << std::endl;
    } 

    // print host info
    if ( verbose )
    {
        printCpuInfo();
        printArchitectureInfo();
        printMemoryInfo();
    }

    dxrt::InferenceOption op;

    if (devices_spec.empty() || devices_spec == "all")
    {
        cout << "Device specification: 'all' (default)" << endl;
    }
    else if (devices_spec.rfind("count:", 0) == 0)
    {
        try
        {
            int num_NPUs = dxrt::DeviceStatus::GetDeviceCount();
            string count_str = devices_spec.substr(6);
            int count = std::stoi(count_str);

            if (count > num_NPUs)
            {
                cout << endl;
                std::cerr << "[ERR] Device count "  << count << " is larger than  the number of available NPU(s): " << num_NPUs << std::endl;
                return -1;
            }

            if (count > 0)
            {
                for (int i = 0; i < count; ++i)
                {
                    op.devices.push_back(i);
                }
                cout << "Device specification: First " << count << " NPU(s) {";
                for (size_t i = 0; i < op.devices.size(); ++i)
                {
                    cout << op.devices[i] << (i == op.devices.size() - 1 ? "" : ", ");
                }
                cout << "}" << endl;
            }
            else
            {
                std::cerr << "[ERR] Device count in '" << devices_spec << "' must be positive." << std::endl;
                return -1;
            }
        }
        catch (const std::invalid_argument& ia)
        {
            std::cerr << "[ERR] Invalid number in '" << devices_spec << "' for 'count:N' format: " << ia.what() << std::endl;
            return -1;
        }
        catch (const std::out_of_range& oor)
        {
            std::cerr << "[ERR] Number out of range in '" << devices_spec << "' for 'count:N' format: " << oor.what() << std::endl;
            return -1;
        }
    }
    else
    {
        try
        {
            int num_NPUs = dxrt::DeviceStatus::GetDeviceCount();
            std::stringstream ss(devices_spec);
            std::string segment;
            bool first_device = true;
            std::set<int> dupID;
            cout << "Device specification: Specific NPU(s) {";
            while (std::getline(ss, segment, ','))
            {
                    segment.erase(std::remove_if(segment.begin(), segment.end(), ::isspace), segment.end());
                    if (segment.empty()) continue;
                    int device_id = std::stoi(segment);

                    if (device_id+1 > num_NPUs)
                    {
                        cout << endl;
                        std::cerr << "[ERR] Device number " << device_id  << "(which is count from 0) is larger than the number of available NPU(s): " << num_NPUs << std::endl;
                        return -1;
                    }

                    if (!dupID.count(device_id)) op.devices.push_back(device_id);
                    dupID.insert(device_id);

                    if (!first_device) cout << ", ";
                    cout << device_id;
                    first_device = false;
            }
        }
        catch (const dxrt::Exception& e)
        {
            std::cerr << e.what() << " error-code=" << e.code() << std::endl;
            return -1;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }
        catch(...)
        {
            std::cerr << "Exception" << std::endl;
            return -1;
        }
        
        cout << "}" << endl;
        if (op.devices.empty() && !devices_spec.empty() && devices_spec != "all")
        {
            std::cerr << "[WARN] No valid device IDs parsed from --devices string: '" << devices_spec << "'. Defaulting to 'all'." << std::endl;
        }
    }

    if (bounding >= 0 && bounding < dxrt::N_BOUND_INF_MAX)
    {
        op.boundOption = bounding;
    }
    else
    {
        cout << "[ERR] Please check bounding option value. Must be between 0 and " << (dxrt::N_BOUND_INF_MAX -1) << endl;
        return -1;
    }
    op.useORT = use_ort;

    try{

#ifdef __linux__
        vector<std::pair<string, string>> fileList = getModelLinux(modelDir, recursive);
        findDuplicates(fileList);
        
        if(fileList.size() == 0)
        {
            cout << "[ERR] The model directory is empty" << endl;
            return -1;
        }
        HostInform inform;
        getHostInform(inform);
#elif _WIN32
        vector<std::pair<string, string>> fileList = getModelWindows(modelDir, recursive);
        findDuplicates(fileList);
        
        if(fileList.size() == 0)
        {
            cout << "[ERR] The model directory is empty" << endl;
            return -1;
        }
        HostInform inform;
        getHostInform(inform);
#endif
        dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::PROFILER, true);
        dxrt::Configuration::GetInstance().SetAttribute(dxrt::Configuration::ITEM::PROFILER, dxrt::Configuration::ATTRIBUTE::PROFILER_SHOW_DATA, "ON");
        dxrt::Configuration::GetInstance().SetAttribute(dxrt::Configuration::ITEM::PROFILER, dxrt::Configuration::ATTRIBUTE::PROFILER_SAVE_DATA, "ON");

        auto& profiler = dxrt::Profiler::GetInstance();


        vector<Result> results;
        //{modelName:[{perfName:[timeSeries Data]}, {perfName:[timeSeries Data]} ... ]}
        std::map<string, vector<std::map<string, vector<int64_t>>>> time_series;
        results.reserve(fileList.size());

        for(unsigned long i=0; i < fileList.size(); ++i)
        {
            auto& file = fileList[i];

            profiler.Start("dxbenchmark_"+file.first);

            Runner runner(file.second, op);
            runner.Run(time, loops, warmup);
            Result result = runner.GetResult();

            result.modelName = file;
            results.push_back(result);

            profiler.End("dxbenchmark_"+file.first);

            time_series[file.first].push_back(profiler.GetPerformanceData());

            if(i != fileList.size()-1)
            {
                profiler.Flush();
            } 
        }

        sortModels(results, criteria, order);

        Reporter reporter(inform, results, time_series, result_path);

        if (!only_data)
        {
            reporter.makeReport();
        }

        reporter.makeData(dxrt::Configuration::GetInstance().GetVersion(), dxrt::Configuration::GetInstance().GetFirmwareVersions()[0].second, dxrt::Configuration::GetInstance().GetDriverVersion(), dxrt::Configuration::GetInstance().GetPCIeDriverVersion());
    }

    catch (const dxrt::Exception& e)
    {
        std::cerr << e.what() << " error-code=" << e.code() << std::endl;
        return -1;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cerr << "Exception" << std::endl;
        return -1;
    }
    return 0;
}