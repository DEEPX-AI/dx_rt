// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"

#include <string>
#include <iostream>


int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    const int DEFAULT_BATCH_COUNT = 1;
    
    std::string modelPath;
    int loop_count = DEFAULT_LOOP_COUNT;
    int batch_count = DEFAULT_BATCH_COUNT;

    if ( argc > 2 )
    {
        modelPath = argv[1];

        if ( argc > 2 ) 
        {
            loop_count = std::stoi(argv[2]);
        }

        if ( argc > 3 ) 
        {
            batch_count = std::stoi(argv[3]);
        }

    }
    else
    {
        std::cout << "[Usage] run_batch_model [dxnn-file-path] [loop-count] [batch-count]" << std::endl;
        return -1;
    }

    
    try
    {

        // create inference engine instance with model
        dxrt::InferenceEngine ie(modelPath);

        // create temporary input buffer for example
        std::vector<uint8_t> inputBuffer(ie.GetInputSize(), 0);

        // input buffer vector
        std::vector<void*> inputBuffers;
        for(int i = 0; i < batch_count; ++i)
        {
            // assigns the same buffer pointer in this example
            inputBuffers.emplace_back(inputBuffer.data());
        }
        

        std::cout << std::endl; // blank line
        std::cout << "[output-internal] Use user's output buffers" << std::endl;
        

        // output buffer vector
        std::vector<void*> output_buffers(batch_count, 0);

        // create user output buffers
        for(auto& ptr : output_buffers)
        {
            ptr = new uint8_t[ie.GetOutputSize()];
        } // for i

        std::cout << "[output-user] Create output buffers by user" << std::endl;
        std::cout << "[output-user] These buffers should be deallocated by user" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        // batch inference loop
        for(int i = 0; i < loop_count; ++i)
        {
            // inference asynchronously, use all npu core
            auto outputPtrs = ie.Run(inputBuffers, output_buffers);

            std::cout << "[output-user] Inference outputs (" << i << ")" << std::endl;
            std::cout << "[output-user] Inference outputs size=" << outputPtrs.size() << std::endl;
            std::cout << "[output-user] Inference outputs first-tensor-name" << outputPtrs.front().front()->name() << std::endl;

            // postProcessing(outputs);
            (void)outputPtrs;
            std::cout << "[output-user] Reuse the user's output buffers" << std::endl;
        }

        auto end = std::chrono::high_resolution_clock::now();

        // Deallocated the user's output buffers
        for(auto& ptr : output_buffers)
        {
            delete[] static_cast<uint8_t*>(ptr);
        } // for i
        std::cout << "[output-user] Deallocated the user's output buffers" << std::endl;

        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count*batch_count);
        double fps = 1000.0 / avg_latency;

        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Use user's output buffers" << std::endl;
        std::cout << "Total Count: loop=" << loop_count << ", batch=" << batch_count << ", total=" << loop_count * batch_count << std::endl;
        std::cout << "Total Time: " << total_time << " ms" << std::endl;
        std::cout << "Average Latency: " << avg_latency << " ms" << std::endl;
        std::cout << "FPS: " << fps << " frames/sec" << std::endl;
        std::cout << "Success" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
         
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