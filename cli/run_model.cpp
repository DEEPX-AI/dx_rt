// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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

void PrintInfResult(const string& inputFile, const string& outputFile, const string& modelFile, float latencyMs, float infTimeMs, float fps) {
    vector<string> lines;

    (void)modelFile;

    if (!inputFile.empty()) {
        lines.push_back("* Processing File : " + inputFile);
        lines.push_back("* Output Saved As : " + outputFile);
    }
    //lines.push_back("* Model Used : " + modelFile);
    if ((bounding > 0 && bounding < 4) || (mode == SINGLE_MODE))
    {
        lines.push_back("* Benchmark Result"); //- NPU" + to_string(bounding-1));
        if (infTimeMs)
            lines.push_back("  - NPU Processing Time  : " + to_string(infTimeMs) + " ms");
        else
            lines.push_back("  - NPU Processing Time  : " + to_string(1000. / fps) + " ms");
        if (latencyMs)
            lines.push_back("  - Total Latency        : " + to_string(latencyMs) + " ms");
        
        if (fps)
            lines.push_back("  - FPS                  : " + to_string(fps));
        else
            lines.push_back("  - FPS                  : " + to_string(1000/infTimeMs));
}
    else
    {
        if (bounding == 0)
            lines.push_back("* Benchmark Result");
        else if (bounding > 3)
            lines.push_back("* Benchmark Result");
        else
            lines.push_back("* Unknown Bounding Option");
        if (fps)
            lines.push_back("  - FPS : " + to_string(fps));
        else
            lines.push_back("  - FPS : " + to_string(1000/infTimeMs));
    }

    size_t maxLength = 0;
    for (const auto& line : lines) {
        maxLength = max(maxLength, line.size());
    }

    cout << string(maxLength, '=') << endl;
    for (const auto& line : lines) {
        cout << setprecision(3) << line << endl;
    }
    cout << string(maxLength, '=') << endl;
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
    int device = -1;
    int targetFps = 0;  // Target FPS
    int core_count = 0;
    bool skip_inference_io = false;
    bool use_ort = false;
    cxxopts::Options options("run_model", APP_NAME);
    options.add_options()
        ("m, model", "Model file (.dxnn)" , cxxopts::value<string>(modelFile))
        ("i, input", "Input data file", cxxopts::value<string>(inputFile))
        ("o, output", "Output data file", cxxopts::value<string>(outputFile)->default_value("output.bin"))
        ("b, benchmark", "Perform a benchmark test (Maximum throughput)", cxxopts::value<bool>(benchmark)->default_value("false"))
        ("s, single", "Perform a single run test (Sequential single-input inference on a single-core)", cxxopts::value<bool>(single)->default_value("false"))
        ("n, npu",
            "NPU bounding (default:0)\n"
            " - Bounding value 0 : inference with all NPU\n"
            " - Bounding value 1 : inference with NPU0\n"
            " - Bounding value 2 : inference with NPU1\n"
            " - Bounding value 3 : inference with NPU2\n"
            " - Bounding value 4 : inference with NPU0/1\n"
            " - Bounding value 5 : inference with NPU1/2\n"
            " - Bounding value 6 : inference with NPU0/2", cxxopts::value<int>(bounding) )
        ("l, loops", "Loops to test", cxxopts::value<int>(loops)->default_value("1") )
        ("d, device", "device to use(blank: all)", cxxopts::value<int>(device)->default_value("-1"))
        ("f, fps", "Target frames per second", cxxopts::value<int>(targetFps) )
        ("skip-io", "Skip Inference I/O(Benchmark mode only)", cxxopts::value<bool>(skip_inference_io)->default_value("false"))
        ("c, core_count", "how many NPUs used", cxxopts::value<int>(core_count)->default_value("0"))
#ifdef USE_ORT
        ("use-ort", "use ONNX Runtime", cxxopts::value<bool>(use_ort)->default_value("false"))
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
    if (device >= 0)
    {
        op.devices = {device};
    }
    else if (core_count > 0)
    {
        for (int i = 0; i < core_count; i++)
        {
            op.devices.push_back(i);
        }
    }
    if (bounding >= 0 && bounding < dxrt::N_BOUND_INF_MAX) {
        op.boundOption = bounding;
    } else {
        cout << "[ERR] Please check bounding option" << endl;
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
                for (int i = 0; i < loops; i++)
                {
                    auto outputs = ie.Run(inputBuf.data());
                    if (!inputFile.empty())
                        dxrt::DataDumpBin(outputFile, outputs.front()->data(), ie.GetOutputSize()); /* TODO: sparse tensor */
                    PrintInfResult(inputFile, outputFile, modelFile, ie.GetLatency()/1000., ie.GetNpuInferenceTime()/1000., 0);
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
                PrintInfResult(inputFile, outputFile, modelFile, 0, 0, fps);
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
                PrintInfResult(inputFile, outputFile, modelFile, 0.0, 0.0, fps);
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
