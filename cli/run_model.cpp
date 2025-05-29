// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 

#include "dxrt/dxrt_api.h"
#include "cxxopts.hpp"
#include "dxrt/filesys_support.h"
#include "dxrt/profiler.h"

using namespace std;

#define APP_NAME "DXRT " DXRT_VERSION " run_model"
// #define TARGET_FPS_DEBUG

enum RunModelMode {
    BENCHMARK_MODE  = 0,
    SINGLE_MODE     = 1,
    TARGET_FPS_MODE = 2,
};
static RunModelMode mode;
static int bounding = 0;

std::ostream& operator<<(std::ostream& os, RunModelMode mode) {
    switch (mode) {
        case BENCHMARK_MODE:  os << "Benchmark Mode"; break;
        case SINGLE_MODE:     os << "Single Mode"; break;
        case TARGET_FPS_MODE: os << "Target FPS Mode"; break;
        default:              os << "Unknown Mode"; break;
    }
    return os;
}

std::string float_to_string_fixed(float value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

void PrintInfResult(const std::string& inputFile, const std::string& outputFile, const std::string& modelFile,
                    float latencyMs, float infTimeMs, float fps_val, int loops, RunModelMode current_mode) {
    std::vector<std::string> lines;
    (void)modelFile; 

    const std::string desc_npu_time = "Actual NPU core computation time for a single request";
    const std::string desc_latency = "End-to-end time measured individually for each specific request within the engine, including data transfer and system overheads";
    const std::string desc_fps = "Overall user-observed inference throughput (inputs/second), reflecting perceived speed";

    const int description_parenthesis_start_column = 45;

    auto build_formatted_line =
        [&](const std::string& label,        // e.g., "  - NPU Processing Time  : "
            const std::string& value_str,    // e.g., "2.819"
            const std::string& unit_str,     // e.g., "ms" or ""
            const std::string& description) -> std::string {
        std::string value_with_unit = value_str + (unit_str.empty() ? "" : " " + unit_str);
        std::string core_content = label + value_with_unit;

        std::string line = core_content;
        int current_length = core_content.length();
        int spaces_to_add = description_parenthesis_start_column - current_length;

        if (spaces_to_add <= 0) {
            spaces_to_add = 1; // Ensure at least one space if content is too long or exact
        }
        
        line += std::string(spaces_to_add, ' '); // Add calculated padding
        line += "(" + description + ")";
        return line;
    };

    std::string infTimeStr = float_to_string_fixed(infTimeMs, 3); // 3 decimal places for ms
    std::string latencyStr = float_to_string_fixed(latencyMs, 3); // 3 decimal places for ms
    std::string fpsStr     = float_to_string_fixed(fps_val, 2);   // 2 decimal places for FPS

    if (!inputFile.empty()) {
        lines.push_back("* Processing File : " + inputFile);
        lines.push_back("* Output Saved As : " + outputFile);
    }

    if (current_mode == SINGLE_MODE) { // Use the passed 'current_mode'
        lines.push_back("* Benchmark Result (single input)");
        lines.push_back(build_formatted_line("  - NPU Processing Time  : ", infTimeStr, "ms", desc_npu_time));
        lines.push_back(build_formatted_line("  - Latency              : ", latencyStr, "ms", desc_latency));
        lines.push_back(build_formatted_line("  - FPS                  : ", fpsStr,     "",   desc_fps));
    } else { // BENCHMARK_MODE or TARGET_FPS_MODE
        lines.push_back("* Benchmark Result (" + std::to_string(loops) + " inputs)");
        lines.push_back(build_formatted_line("  - NPU Processing Time Average : ", infTimeStr, "ms", desc_npu_time));
        lines.push_back(build_formatted_line("  - Latency Average             : ", latencyStr, "ms", desc_latency));
        lines.push_back(build_formatted_line("  - Total FPS                   : ", fpsStr,     "",   desc_fps));
    }

    size_t maxLength = 0;
    for (const auto& line : lines) {
        maxLength = std::max(maxLength, line.length());
    }

    std::cout << std::string(maxLength, '=') << std::endl;
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
    std::cout << std::string(maxLength, '=') << std::endl;
}


void SetRunModelMode(bool single, int targetFps)
{
    if (single) {
        mode = SINGLE_MODE;
    } else if (targetFps) {
        mode = TARGET_FPS_MODE;
    } else {
        mode = BENCHMARK_MODE;
    }
    cout << "Run model target mode : " << mode << endl;
}

int main(int argc, char *argv[])
{
    string modelFile, inputFile, outputFile;
    bool benchmark = false;
    bool single = false;
    int loops = 1;
    string devices_spec;
    int targetFps = 0; 
    bool skip_inference_io = false;
    bool use_ort = false;
    cxxopts::Options options("run_model", APP_NAME);
    options.add_options()
        ("m, model", "Model file (.dxnn)" , cxxopts::value<string>(modelFile))
        ("i, input", "Input data file", cxxopts::value<string>(inputFile))
        ("o, output", "Output data file", cxxopts::value<string>(outputFile)->default_value("output.bin"))
        ("b, benchmark", "Perform a benchmark test (Maximum throughput, This is the default mode if --single or --fps > 0 are not specified)", cxxopts::value<bool>(benchmark)->default_value("false"))
        ("s, single", "Perform a single run test (Sequential single-input inference on a single-core)", cxxopts::value<bool>(single)->default_value("false"))
        ("n, npu", 
            "NPU bounding (default:0)\n"
            "  0: NPU_ALL\n  1: NPU_0\n  2: NPU_1\n  3: NPU_2\n"
            "  4: NPU_0/1\n  5: NPU_1/2\n  6: NPU_0/2", cxxopts::value<int>(bounding) ) 
        ("l, loops", "Number of inference loops to perform (default: 30)", cxxopts::value<int>(loops)->default_value("30") )
        ("d, devices",
            "Specify target NPU devices. Examples:\n"
            "  'all' (default): Use all available/bound NPUs.\n"
            "  '0': Use NPU0 only.\n"
            "  '0,1,2': Use NPU0, NPU1, and NPU2.\n"
            "  'count:N': Use the first N NPUs (e.g., 'count:2' for NPU0, NPU1).",
            cxxopts::value<std::string>(devices_spec)->default_value("all"))
        ("f, fps", "arget FPS for TARGET_FPS_MODE (enables this mode if > 0 and --single is not set, default: 0)", cxxopts::value<int>(targetFps) )
        ("skip-io", "Attempt to skip Inference I/O (Benchmark mode only)", cxxopts::value<bool>(skip_inference_io)->default_value("false"))
#ifdef USE_ORT
        ("use-ort", "Enable ONNX Runtime for CPU tasks in the model graph. If disabled, only NPU tasks operate", cxxopts::value<bool>(use_ort)->default_value("false"))
#endif
        ("h, help", "Print usage" )
    ;

    try
    {
        auto cmd = options.parse(argc, argv);
        if (cmd.count("help"))
        {
            cout << options.help() << endl;
            exit(0);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        cout << options.help() << endl;
        exit(0);
    }

    if ( modelFile.length() == 0)
    {
        cout << options.help() << endl;
        exit(0);
    }

    LOG_VALUE(modelFile);
    LOG_VALUE(inputFile);
    LOG_VALUE(outputFile);
    LOG_VALUE(benchmark);
    LOG_VALUE(loops);
    dxrt::InferenceOption op;

    if (devices_spec.empty() || devices_spec == "all") 
    {
        cout << "Device specification: 'all' (default)" << endl;
    } 
    else if (devices_spec.rfind("count:", 0) == 0) 
    { 
        try 
        {
            string count_str = devices_spec.substr(6);
            int count = std::stoi(count_str);
            if (count > 0) 
            {
                for (int i = 0; i < count; ++i) 
                {
                    op.devices.push_back(i);
                }
                cout << "Device specification: First " << count << " NPU(s) {";
                for(size_t i = 0; i < op.devices.size(); ++i) 
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
            std::cerr << "[ERR] Invalid number in '" << devices_spec << "' for 'count:N' format." << std::endl;
            return -1;
        } 
        catch (const std::out_of_range& oor) 
        {
            std::cerr << "[ERR] Number out of range in '" << devices_spec << "' for 'count:N' format." << std::endl;
            return -1;
        }
    }
    else 
    {
        std::stringstream ss(devices_spec);
        std::string segment;
        bool first_device = true;
        cout << "Device specification: Specific NPU(s) {";
        while(std::getline(ss, segment, ',')) 
        {
            try 
            {
                segment.erase(std::remove_if(segment.begin(), segment.end(), ::isspace), segment.end());
                if (segment.empty()) continue;
                int device_id = std::stoi(segment);
                op.devices.push_back(device_id);
                if (!first_device) cout << ", ";
                cout << device_id;
                first_device = false;
            } 
            catch (const std::invalid_argument& ia) 
            {
                std::cerr << "[ERR] Invalid device ID '" << segment << "' in --devices list." << std::endl;
                return -1;
            } 
            catch (const std::out_of_range& oor) 
            {
                std::cerr << "[ERR] Device ID '" << segment << "' out of range in --devices list." << std::endl;
                return -1;
            }
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
        dxrt::InferenceEngine ie(modelFile, op);
        vector<uint8_t> inputBuf(ie.GetInputSize(), 0);
        if (!inputFile.empty())
        {
            DXRT_ASSERT(ie.GetInputSize() == static_cast<uint64_t>(dxrt::getFileSize(inputFile)), "input file size mismatch");
            dxrt::DataFromFile(inputFile, inputBuf.data());
        }

        SetRunModelMode(single, targetFps);

        if (skip_inference_io)
        {
            if (benchmark == false)
            {
                cout << "[ERR] Skip-inference option only for benchmark mode" << endl;
                return -1;
            }
            if (mode != BENCHMARK_MODE)
            {
                cout << "[ERR] Skip-inference option only for benchmark mode" << endl;
                return -1;
            }
            dxrt::SKIP_INFERENCE_IO = 1;
        }

        switch (mode)
        {
            case SINGLE_MODE: {
                uint64_t infTime = 0;
                float fps = 0.0;
                for (int i = 0; i < loops; i++)
                {
                    auto start_clock = std::chrono::steady_clock::now();
                    auto outputs = ie.Run(inputBuf.data());
                    auto end_clock = std::chrono::steady_clock::now();
                    infTime = std::chrono::duration_cast<chrono::microseconds>(end_clock - start_clock).count();
                    fps = 1000000.0 * loops/infTime;
                    if (!inputFile.empty())
                        dxrt::DataDumpBin(outputFile, outputs.front()->data(), ie.GetOutputSize());
                    PrintInfResult(inputFile, outputFile, modelFile, ie.GetLatency()/1000., ie.GetNpuInferenceTime()/1000., fps, 1, mode);
                }
                break;
            }
            case TARGET_FPS_MODE: {
#ifdef TARGET_FPS_DEBUG
                vector<string> results;  // Company and time storage
#endif
                atomic<int> callback_cnt;
                static auto startTime = chrono::high_resolution_clock::now();  // Start time
                auto& profiler = dxrt::Profiler::GetInstance();
                uint64_t infTime = 0;
                float fps = 0.0;

                function<int(vector<shared_ptr<dxrt::Tensor>>, void*)> postProcCallBack = \
                    [&](vector<shared_ptr<dxrt::Tensor>> outputs, void *arg)
                    {
                        (void)arg;
                        ignore = outputs;
                        callback_cnt++;
                        return 0;
                    };
                callback_cnt = 0;
                ie.RegisterCallback(postProcCallBack);

                auto start_clock = std::chrono::steady_clock::now();
                profiler.Start("TargetFps");
                for (int i = 0; i < loops; i++)
                {
#ifdef TARGET_FPS_DEBUG
                    auto loopStartTime = chrono::high_resolution_clock::now();  // Start time for this loop
#endif
                    (void)ie.RunAsync(inputBuf.data(), 0);
#ifdef TARGET_FPS_DEBUG
                    auto elapsed = chrono::high_resolution_clock::now() - loopStartTime;
                    auto elapsedMs = chrono::duration_cast<chrono::milliseconds>(elapsed).count();
                    results.push_back("Iteration " + to_string(i + 1) + ": " + inputFile + " -> " + outputFile +
                                    ", Time: " + to_string(elapsedMs) + " ms");
#endif
                    // Calculate the expected time per frame
                    if (targetFps > 0)
                    {
                        auto targetTime = 1000 / targetFps;  // in milliseconds
                        auto totalElapsed = chrono::high_resolution_clock::now() - startTime;
                        auto totalElapsedMs = chrono::duration_cast<chrono::milliseconds>(totalElapsed).count();

                        if (totalElapsedMs < (i + 1) * targetTime)
                        {
                            auto sleepDuration = (i + 1) * targetTime - totalElapsedMs;
                            this_thread::sleep_for(chrono::milliseconds(sleepDuration));
                        }
                    }
                }
                profiler.End("TargetFps");
                auto end_clock = std::chrono::steady_clock::now();
                ie.Wait(loops);
                infTime = std::chrono::duration_cast<chrono::microseconds>(end_clock - start_clock).count();
                fps = 1000000.0 * loops/infTime;
                PrintInfResult(inputFile, outputFile, modelFile, ie.GetLatencyMean()/1000., ie.GetNpuInferenceTimeMean()/1000., fps, loops, mode);

#ifdef TARGET_FPS_DEBUG
                for (const auto& result : results) {
                    cout << result << endl;
                }
#endif
                break;
            }
            case BENCHMARK_MODE: {
                // PrintInfResult(inputFile, outputFile, modelFile, 0.0, 0.0, fps);
                float fps = ie.RunBenchmark(loops, inputBuf.data());
                if (!inputFile.empty())
                {
                    auto outputs = ie.Run(inputBuf.data());
                    dxrt::DataDumpBin(outputFile, outputs.front()->data(), ie.GetOutputSize());  /* TODO: sparse tensor */
                }
                PrintInfResult(inputFile, outputFile, modelFile, ie.GetLatencyMean()/1000., ie.GetNpuInferenceTimeMean()/1000., fps, loops, mode);

                break;
            }
            default:
                cout << "Unknown run model mode:" << mode << endl;
                return -1;
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
    return 0;
}
