// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "concurrent_queue.h"

#include <string>
#include <iostream>


static std::atomic<int> gCallbackCnt{0};
static ConcurrentQueue<int> gResultQueue(1);
static int gGlobalIndex{0};
static std::mutex gCBMutex;
static std::mutex gInferenceMutex;

// invoke this function asynchronously after the inference is completed
static int onInferenceCallbackFunc(dxrt::TensorPtrs &outputs, void *userArg)
{
   
    // user data type casting
    std::pair<int, int>* user_data = reinterpret_cast<std::pair<int, int>*>(userArg);

    // post processing with outputs
    // ...
    (void)outputs;


    std::cout << "Callback triggered for inference with user_arg(" 
        << user_data->first + 1 << ", " << user_data->second << ")" << std::endl;
  

    {
        // Mutex locks should be properly adjusted 
        // to ensure that callback functions are thread-safe.
        std::lock_guard<std::mutex> lock(gCBMutex);

        gCallbackCnt ++;

        // end of the loop
        int cbCount = gCallbackCnt.load();
        if ( user_data->second == cbCount ) // check loop count
        {
            gResultQueue.push(cbCount);
        }
    }

    // delete argument object
    delete user_data;


    return 0;
}

// submits inference jobs using shared InferenceEngine
void RunInferenceThread(dxrt::InferenceEngine& ie, std::vector<uint8_t>& inputBuffer, int loopCount)
{
    while(true)
    {
        int inferenceIndex;
        {
            std::lock_guard<std::mutex> lock(gInferenceMutex);

            if (gGlobalIndex == loopCount ) break;
            inferenceIndex = gGlobalIndex++;
        }
        // user argument
        auto* userData = new std::pair<int,int>(inferenceIndex, loopCount);

        // inference asynchronously, use all npu cores
        ie.RunAsync(inputBuffer.data(), userData);

        std::cout << "Inference request submitted with user_arg(" << inferenceIndex << ")" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    const int THREAD_COUNT = 3;
    
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
        std::cout << "[Usage] display_async_thread [dxnn-file-path] [loop-count]" << std::endl;
        return -1;
    }

    int total_callback_count = 0;

    try 
    {
    
        // create inference engine instance with model
        dxrt::InferenceEngine ie(modelPath);

        // register call back function
        ie.RegisterCallback(onInferenceCallbackFunc);

        // create input buffers for each thread (same size as model input)
        std::vector<std::vector<uint8_t>> inputBuffers(THREAD_COUNT, std::vector<uint8_t>(ie.GetInputSize(), 0));

        auto start = std::chrono::high_resolution_clock::now();

        // launch THREAD_COUNT threads to submit async inference jobs
        std::vector<std::thread> threads;
        for(int t = 0; t < THREAD_COUNT; ++t) {
            threads.emplace_back(RunInferenceThread, std::ref(ie), std::ref(inputBuffers[t]) , loop_count);
        }

        // wait for all threads to complete
        for (auto& th: threads) {
            th.join();
        }

        // wait until all callbacks have been processed
        total_callback_count = gResultQueue.pop();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Total Time: " << total_time << " ms" << std::endl;
        std::cout << "Average Latency: " << avg_latency << " ms" << std::endl;
        std::cout << "FPS: " << fps << " frames/sec" << std::endl;
        std::cout << "Total callback-count / loop-count: " << total_callback_count << " / " << loop_count 
            << (total_callback_count == loop_count ? " (Success)" : " (Failure)") << std::endl;
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
    
    return (total_callback_count == loop_count ? 0 : -1);
}