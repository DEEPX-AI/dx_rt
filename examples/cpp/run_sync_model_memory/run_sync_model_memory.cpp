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
#include <fstream>
#include <vector>


int main(int argc, char* argv[])
{
    std::string model_path;
    int loop_count;
    bool verbose;
    
    auto &log = dxrt::Logger::GetInstance();

    cxxopts::Options options("run_sync_model_memory", "Run synchronous model memory inference");
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

    log.Info("Start run_sync_model_memory test for model: " + model_path);

    try
    {
        // Load model file into memory first
        std::ifstream model_file(model_path, std::ios::binary | std::ios::ate);
        if (!model_file.is_open()) {
            log.Error("Failed to open model file: " + model_path);
            return -1;
        }

        std::streamsize model_size = model_file.tellg();
        model_file.seekg(0, std::ios::beg);

        std::vector<uint8_t> model_buffer(model_size);
        if (!model_file.read(reinterpret_cast<char*>(model_buffer.data()), model_size)) {
            log.Error("Failed to read model file: " + model_path);
            return -1;
        }
        model_file.close();

        log.Info("Model loaded into memory, size: " + std::to_string(model_size) + " bytes");
        
        // create inference engine instance with model from memory
        dxrt::InferenceEngine ie(model_buffer.data(), model_buffer.size());

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr(ie.GetInputSize(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        // inference loop
        for(int i = 0; i < loop_count; ++i)
        {
            
            // inference synchronously, use one npu core
            auto outputs = ie.Run(inputPtr.data());

            log.Debug("Inference outputs (" + std::to_string(i) + ")");
            // poset processing
            // postProcessing(outputs);
            (void)outputs;

        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        log.Info("-----------------------------------");
        log.Info("Total Time: " + std::to_string(total_time) + " ms");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms");
        log.Info("FPS: " + std::to_string(fps) + " frame/sec");
        log.Info("Success");
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
    catch(...)
    {
        log.Error("Exception");
        return -1;
    }
    
    return 0;
}