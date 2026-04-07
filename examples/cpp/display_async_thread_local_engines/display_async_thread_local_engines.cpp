/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/dxrt_api.h"
#include "dxrt/extern/cxxopts.hpp"
#include "../include/logger.h"
#include <string>
#include <iostream>
#include <algorithm>

// Each thread creates its own InferenceEngine instance independently.
// Suitable for isolating thread resources but may increase memory usage.
static double RunInferenceThread(const std::string& modelPath, int loopCount, int threadIndex, bool ort)
{
    // create inference engine instance with model
    const auto& log = dxrt::Logger::GetInstance();
    dxrt::InferenceOption io;
    io.useORT = ort;

    dxrt::InferenceEngine ie(modelPath, io);

    std::mutex cv_mutex;
    std::condition_variable cv;
    int cb_index = 0;


    // register call back function
    ie.RegisterCallback([loopCount, &cb_index, &cv_mutex, &cv, threadIndex, &log] (const dxrt::TensorPtrs &outputs, void *userArg) {
        std::ignore = outputs;
        std::ignore = userArg;
        std::unique_lock<std::mutex> lock(cv_mutex);
        cb_index ++;

        log.Debug("[Thread " + std::to_string(threadIndex) + "] callback triggered for inference with callbackIndex(" + std::to_string(cb_index) + ")");

        if ( loopCount == cb_index )
        {
            cv.notify_one();
        }

        return 0;
    });

    // create input buffer
    std::vector<uint8_t> input_buffer(ie.GetInputSize(), 0);

    // Start timing for pure inference (excluding engine creation)
    auto inference_start = std::chrono::high_resolution_clock::now();

    // Submit 'loopCount' number of asynchronous inference requests
    for(int i = 0; i < loopCount; ++i)
    {
        // inference asynchronously, use all npu cores
        ie.RunAsync(input_buffer.data());

        log.Debug("[Thread " + std::to_string(threadIndex) + "] request submitted with loopcount(" + std::to_string(i + 1) + ")");
    }

    std::unique_lock<std::mutex> lock(cv_mutex);
    cv.wait(lock, [&cb_index, loopCount]{ return cb_index == loopCount; });

    auto inference_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> inference_duration = inference_end - inference_start;

    log.Info("[Thread " + std::to_string(threadIndex) + "] finished " + std::to_string(loopCount) + " inference loops, time: " + std::to_string(inference_duration.count()) + " ms");

    return inference_duration.count();
}

int main(int argc, char* argv[])
{
    const int THREAD_COUNT = 2;
    const bool ENABLE_ORT = false;

    std::string model_path;
    int loop_count;
    bool verbose;

    auto &log = dxrt::Logger::GetInstance();

    cxxopts::Options options("display_async_thread_local_engines", "Display async inference with thread-local engine instances");
    options.add_options()
        ("m,model", "Path to model file (.dxnn)", cxxopts::value<std::string>(model_path))
        ("l,loops", "Number of inference loops", cxxopts::value<int>(loop_count)->default_value("1"))
        ("v,verbose", "Enable verbose/debug logging", cxxopts::value<bool>(verbose)->default_value("false"))
        ("h,help", "Print usage");

    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help") || !result.count("model"))
        {
            std::cout << options.help() << std::endl;
            return result.count("help") ? 0 : -1;
        }

        if (verbose) {
            log.SetLevel(dxrt::Logger::Level::LOGLEVEL_DEBUG);
        }
    }
    catch (const std::exception& e)
    {
        log.Error(std::string("Error parsing arguments: ") + e.what());
        std::cout << options.help() << std::endl;
        return -1;
    }

    log.Info("Start display_async_thread_local_engines for model: " + model_path);

    try
    {
        std::vector<std::thread> threads;
        std::vector<double> thread_times(THREAD_COUNT, 0.0);

        for (int t = 0; t < THREAD_COUNT; ++t) {
            threads.emplace_back([&thread_times, t, model_path, loop_count, ENABLE_ORT]() {
                thread_times[t] = RunInferenceThread(model_path, loop_count, t, ENABLE_ORT);
            });
        }

        // wait for all threads to complete
        for (auto& th : threads) {
            th.join();
        }

        // Find the maximum time among threads (parallel execution)
        double total_time = *std::max_element(thread_times.begin(), thread_times.end());

        int total_inferences = THREAD_COUNT * loop_count;
        double avg_latency = total_time / static_cast<double>(total_inferences);
        double throughput_fps = 1000.0 / avg_latency;

        log.Info("-----------------------------------");
        log.Info("Total Time: " + std::to_string(total_time) + " ms (max of parallel threads)");
        log.Info("Total Inferences: " + std::to_string(total_inferences) + " (" + std::to_string(THREAD_COUNT) + " threads × " + std::to_string(loop_count) + " loops)");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms/inference");
        log.Info("Throughput: " + std::to_string(throughput_fps) + " inferences/sec");
        log.Info("-----------------------------------");
    }
    catch (const dxrt::Exception& e)
    {
        log.Error(std::string(e.what()) + " error-code=" + std::to_string(static_cast<int>(e.code())));
        return -1;
    }
    catch (const std::exception& e)
    {
        log.Error(std::string("std::exception: ") + e.what());
        return -1;
    }
    catch(...)
    {
        log.Error("Exception");
        return -1;
    }

    return 0;
}
