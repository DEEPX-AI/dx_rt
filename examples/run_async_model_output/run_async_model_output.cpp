// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "concurrent_queue.h"
#include "simple_circular_buffer_pool.h"

#include <string>
#include <iostream>


// concurrent queue is a thread-safe queue data structure 
// designed to be used in a multi-threaded environment
static ConcurrentQueue<std::pair<int, uint8_t*>> gJobIdQueue(32);

static const int BUFFER_POOL_SIZE = 10;
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gOutputBufferPool;
static std::atomic<int> gOutputCount = {0};

// user thread to wait for the completion of inference 
static int inferenceThreadFunc(dxrt::InferenceEngine& ie, int loopCount)
{
    int count = 0;

    while(true)
    {
        // pop item from queue 
        //int jobId = gJobIdQueue.pop();
        auto jobInfo = gJobIdQueue.pop();

        try
        {
            // waiting for the inference to complete by jobId
            // ownership of the outputs is transferred to the user 
            auto outputs = ie.Wait(jobInfo.first);

            // post processing
            // postProcessing(outputs);
            (void)outputs;
            if ( outputs.front()->data() != reinterpret_cast<void*>(jobInfo.second) ) 
            {
                std::cerr << "Output buffer pointer != user output pointer" << std::endl;
            }
            else 
            {
                gOutputCount++;
            }

            // something to do
        }
        catch (const dxrt::Exception& e)
        {
            std::cerr << e.what() << " error-code=" << e.code() << std::endl;
            return -1;
        }

        std::cout << "Inference outputs corresponding to jobId(" << jobInfo.first << ")" << std::endl;

        count++;
        if ( count >= loopCount ) break;

    } // while

    return 0;
}

int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    
    std::string modelPath;
    int loop_count = DEFAULT_LOOP_COUNT;
    if ( argc > 1 )
    {
        modelPath = argv[1];

        if ( argc > 2 ) 
        {
            loop_count = std::stoi(argv[2]);
        }

    }
    else
    {
        std::cout << "[Usage] run_async_model_output [dxnn-file-path] [loop-count]" << std::endl;
        return -1;
    }


    try
    {
        // create inference engine instance with model
        dxrt::InferenceEngine ie(modelPath);

        // create output buffer pool
        gOutputBufferPool = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, ie.output_size());

        // create thread
        auto t1 = std::thread(inferenceThreadFunc, std::ref(ie), loop_count);

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr(ie.input_size(), 0);

        auto start = std::chrono::high_resolution_clock::now();

        // inference loop
        gOutputCount = 0;
        for(int i = 0; i < loop_count; ++i)
        {
            // no need user argument
            // UserData *userData = getUserDataInstanceFromDataPool();

            // inference asynchronously, use all npu cores
            // if device-load >= max-load-value, this function will block
            // provide the output buffer pointer so the user can manage the output directly
            auto pointer = gOutputBufferPool->pointer();
            auto jobId = ie.RunAsync(inputPtr.data(), nullptr, pointer);

            // push jobId in global queue variable
            gJobIdQueue.push(std::pair<int, uint8_t*>(jobId, pointer));

            std::cout << "Inference request submitted with jobId(" << jobId << ")" << std::endl;

        } // for i

        t1.join();
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Total Time: " << total_time << " ms" << std::endl;
        std::cout << "Average Latency: " << avg_latency << " ms" << std::endl;
        std::cout << "FPS: " << fps << " frames/sec" << std::endl;
        std::cout << "loop-count=" << loop_count << " output-count=" << gOutputCount << std::endl;
        if ( gOutputCount == loop_count ) std::cout << "Success" << std::endl;
        else std::cout << "Failure" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
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

