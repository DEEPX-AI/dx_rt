/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

/**
 * @file run_async_model_profiler.cpp
 * @brief Async inference with per-job profiling using the Wait pattern.
 *
 * Follows the run_async_model_wait flow:
 *   Main thread  : RunAsync() × N  →  push jobId to queue
 *   Worker thread: pop jobId → ie.Wait(jobId) → profiler.GetJobMetrics(jobId) → print
 *
 * This gives accurate per-job H2D / NPU / D2H / Task timings for each job.
 *
 * Usage:
 *   run_async_model_profiler -m <model.dxnn> [-l <loops>] [--use-ort]
 */

#include "dxrt/dxrt_cxx_api.h"
#include "dxrt/extern/cxxopts.hpp"
#include "../include/logger.h"
#include "../include/concurrent_queue.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

static ConcurrentQueue<int> gJobIdQueue(32);

static void printJobMetrics(dxrt::Logger& log, int jobIdx, int jobId,
                            const dxrt::JobMetrics& jm)
{
    if (!jm.valid)
    {
        log.Info("[Job #" + std::to_string(jobIdx) + " id=" + std::to_string(jobId) +
                 "] No profiler data");
        return;
    }

    for (const auto& task : jm.tasks)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3);
        oss << "[Job #" << jobIdx << " id=" << jobId << "] task=" << task.task_name;

        for (const auto& devPair : task.devices)
        {
            const int devId  = devPair.first;
            const auto& d    = devPair.second;
            oss << "  |  Dev" << devId
                << ": InputNFH=" << d.input_format_us << "us"
                << " H2D=" << d.h2d_us  << "us"
                << " Inference=" << d.inference_core_all_us << "us"
                << " (Core0=" << d.inference_core_0_us
                << " Core1=" << d.inference_core_1_us
                << " Core2=" << d.inference_core_2_us << ")"
                << " D2H=" << d.d2h_us  << "us"
                << " OutputNFH=" << d.output_format_us << "us"
                << " NPU Task=" << d.total_us << "us";
        }

        if (task.cpu_task_us > 0.0)
        {
            oss << "  |  CPU Task=" << task.cpu_task_us << "us";
        }

        log.Info(oss.str());
    }
}

// Worker thread: Wait for each job and print profiling metrics
static int waitThreadFunc(const dxrt::InferenceEngine& ie,
                          dxrt::Profiler& profiler,
                          int loopCount)
{
    static const auto& log = dxrt::Logger::GetInstance();

    for (int idx = 0; idx < loopCount; ++idx)
    {
        int jobId = gJobIdQueue.pop();

        try
        {
            auto outputs = ie.Wait(jobId);
            (void)outputs;
        }
        catch (const dxrt::Exception& e)
        {
            log.Error(std::string(e.what()) +
                      " error-code=" + std::to_string(static_cast<int>(e.code())));
            return -1;
        }

        auto metrics = profiler.GetJobMetrics(jobId);
        printJobMetrics(const_cast<dxrt::Logger&>(log), idx, jobId, metrics);
    }

    return 0;
}

int main(int argc, char* argv[])
{
    std::string model_path;
    int         loop_count;
    bool        use_ort;
    bool        verbose;

    auto& log = dxrt::Logger::GetInstance();

    cxxopts::Options options("run_async_model_profiler",
                             "Async inference with per-job profiling (Wait + GetJobMetrics)");
    options.add_options()
        ("m,model",   "Path to model file (.dxnn)",
         cxxopts::value<std::string>(model_path))
        ("l,loops",   "Number of inference loops",
         cxxopts::value<int>(loop_count)->default_value("10"))
        ("use-ort",   "Enable ORT (CPU post-processing) for the model",
         cxxopts::value<bool>(use_ort)->default_value("false"))
        ("v,verbose", "Enable verbose/debug logging",
         cxxopts::value<bool>(verbose)->default_value("false"))
        ("h,help",    "Print usage");

    try
    {
        auto result = options.parse(argc, argv);
        if (result.count("help") || !result.count("model"))
        {
            std::cout << options.help() << std::endl;
            return result.count("help") ? 0 : -1;
        }
        if (verbose)
        {
            log.SetLevel(dxrt::Logger::Level::LOGLEVEL_DEBUG);
        }
    }
    catch (const std::exception& e)
    {
        log.Error(std::string("Error parsing arguments: ") + e.what());
        std::cout << options.help() << std::endl;
        return -1;
    }

    log.Info("Model  : " + model_path);
    log.Info("Loops  : " + std::to_string(loop_count));
    log.Info("UseORT : " + std::string(use_ort ? "true" : "false"));

    try
    {
        // ── Enable profiler ───────────────────────────────────────────────
        auto& config = dxrt::Configuration::GetInstance();
        config.SetEnable(dxrt::Configuration::ITEM::PROFILER, true);
        config.SetAttribute(dxrt::Configuration::ITEM::PROFILER,
                            dxrt::Configuration::ATTRIBUTE::PROFILER_SHOW_DATA, "ON");
        config.SetAttribute(dxrt::Configuration::ITEM::PROFILER,
                            dxrt::Configuration::ATTRIBUTE::PROFILER_SAVE_DATA, "OFF");

        auto& profiler = dxrt::Profiler::GetInstance();

        // ── Create InferenceEngine ────────────────────────────────────────
        dxrt::InferenceOption option;
        option.useORT = use_ort;
        dxrt::InferenceEngine ie(model_path, option);

        // ── Start worker thread ───────────────────────────────────────────
        auto worker = std::thread(waitThreadFunc, std::ref(ie), std::ref(profiler), loop_count);

        // ── Submit all jobs ───────────────────────────────────────────────
        std::vector<uint8_t> input(ie.GetInputSize(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < loop_count; ++i)
        {
            int jobId = ie.RunAsync(input.data());
            gJobIdQueue.push(jobId);
            log.Debug("Submitted jobId=" + std::to_string(jobId));
        }

        worker.join();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        double total_ms = elapsed.count();
        double avg_ms   = total_ms / static_cast<double>(loop_count);
        double fps      = 1000.0 / avg_ms;

        log.Info("-----------------------------------");
        log.Info("Total   : " + std::to_string(total_ms) + " ms");
        log.Info("Average : " + std::to_string(avg_ms)   + " ms/job");
        log.Info("FPS     : " + std::to_string(fps));
        log.Info("-----------------------------------");


    }
    catch (const dxrt::Exception& e)
    {
        log.Error(std::string("dxrt::Exception: ") + e.what());
        return -1;
    }
    catch (const std::exception& e)
    {
        log.Error(std::string("std::exception: ") + e.what());
        return -1;
    }
    catch (...)
    {
        log.Error("Unknown exception");
        return -1;
    }

    return 0;
}
