/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

/*
 * Device Status Example
 *
 * Demonstrates how to use the DeviceStatus API to retrieve
 * all device information (spec, status, utilization, memory).
 * Optionally runs inference while monitoring the device in a background thread.
 *
 * Usage:
 *   device_monitoring -m <model_path> [-l loops] [-p poll_ms] [--no-inference]
 */

#include "dxrt/dxrt_cxx_api.h"
#include "dxrt/extern/cxxopts.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

static std::atomic<bool> g_running{true};

void printDeviceInfo(const dxrt::DeviceStatus &ds)
{
    std::cout << "=== Device Info (id=" << ds.GetId() << ") ===" << std::endl;
    std::cout << ds.GetInfoString() << std::endl;
}

void printDeviceDynamicStatus(const dxrt::DeviceStatus &ds)
{
    std::cout << "[Device " << ds.GetId() << "]"
              << " temp=["
              << ds.GetTemperature(0) << ", "
              << ds.GetTemperature(1) << ", "
              << ds.GetTemperature(2) << "]C"
              << ", voltage=["
              << ds.GetNpuVoltage(0) << ", "
              << ds.GetNpuVoltage(1) << ", "
              << ds.GetNpuVoltage(2) << "]mV"
              << ", clock=["
              << ds.GetNpuClock(0) << ", "
              << ds.GetNpuClock(1) << ", "
              << ds.GetNpuClock(2) << "]MHz"
              << " | util=["
              << ds.GetCoreUtilization(0) << "%, "
              << ds.GetCoreUtilization(1) << "%, "
              << ds.GetCoreUtilization(2) << "%]"
              << ", mem_used=" << ds.GetMemoryUsed()
              << ", mem_free=" << ds.GetMemoryFree()
              << std::endl;
}

void monitorLoop(int deviceCount, int pollMs)
{
    // Print static device info once
    for (int id = 0; id < deviceCount; id++)
    {
        auto ds = dxrt::DeviceStatus::GetCurrentStatus(id);
        if (ds.IsValid())
        {
            printDeviceInfo(ds);
        }
    }
    std::cout << std::endl;

    // Poll dynamic status
    while (g_running.load())
    {
        for (int id = 0; id < deviceCount; id++)
        {
            auto ds = dxrt::DeviceStatus::GetCurrentStatus(id);
            if (ds.IsValid())
            {
                printDeviceDynamicStatus(ds);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(pollMs));
    }
    std::cout << "[Monitor] Stopped." << std::endl;
}

int main(int argc, char *argv[])
{
    std::string model_path;
    int loop_count;
    int poll_ms;
    bool no_inference;

    cxxopts::Options options("device_monitoring", "Device Status API example");
    options.add_options()
        ("m,model", "Path to model file (.dxnn)", cxxopts::value<std::string>(model_path))
        ("l,loops", "Number of inference loops", cxxopts::value<int>(loop_count)->default_value("5"))
        ("p,poll", "Status polling interval (ms)", cxxopts::value<int>(poll_ms)->default_value("200"))
        ("no-inference", "Only monitor without running inference", cxxopts::value<bool>(no_inference)->default_value("false"))
        ("h,help", "Print usage");

    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (!no_inference && !result.count("model"))
        {
            std::cerr << "Error: --model is required unless --no-inference is set." << std::endl;
            std::cout << options.help() << std::endl;
            return -1;
        }
    }
    catch (const cxxopts::exceptions::exception &e)
    {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return -1;
    }

    int device_count = dxrt::DeviceStatus::GetDeviceCount();

    // --- No-inference mode: just query status and exit ---
    if (no_inference)
    {
        std::cout << "Querying device status (no inference)..." << std::endl;
        for (int id = 0; id < device_count; id++)
        {
            auto ds = dxrt::DeviceStatus::GetCurrentStatus(id);
            if (!ds.IsValid())
            {
                std::cerr << "Warning: Device " << id << " data is not available." << std::endl;
                continue;
            }
            printDeviceInfo(ds);
            printDeviceDynamicStatus(ds);
            std::cout << std::endl;
        }
        return 0;
    }

    // --- Inference + monitoring mode ---
    std::cout << "Starting inference with device monitoring" << std::endl;
    std::cout << "  Model: " << model_path << std::endl;
    std::cout << "  Loops: " << loop_count << std::endl;
    std::cout << "  Poll interval: " << poll_ms << " ms" << std::endl;
    std::cout << std::endl;

    // Start monitor thread
    std::thread monitor(monitorLoop, device_count, poll_ms);

    // Run benchmark
    dxrt::InferenceEngine engine(model_path);
    std::vector<uint8_t> input_buf(engine.GetInputSize(), 0);
    float fps = engine.RunBenchmark(loop_count, input_buf.data());

    // Stop monitor
    g_running.store(false);
    monitor.join();

    std::cout << std::endl;
    std::cout << "=== Done: RunBenchmark " << loop_count
              << " loops, average " << fps << " FPS ===" << std::endl;
    return 0;
}
