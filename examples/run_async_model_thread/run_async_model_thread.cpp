// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "concurrent_queue.h"

#include <string>
#include <iostream>

// 1 InferenceEngine and 3 threads (asynchronous)


static const int THREAD_COUNT = 3;
static std::vector<std::atomic<bool>> gResultComplete(THREAD_COUNT);
static std::vector<std::atomic<int>> gResultCount(THREAD_COUNT);
static ConcurrentQueue<int> gResultQueue(1);
static std::mutex gCBMutex;

class UserData 
{
    int _threadIndex = -1;
    int _loopIndex = -1;
    int _loopCount = -1;
public:
    void setThreadIndex(int index) {
        _threadIndex = index;
    }

    int getThreadIndex() {
        return _threadIndex;
    }

    void setLoopIndex(int index) {
        _loopIndex = index;
    }

    int getLoopIndex() {
        return _loopIndex;
    }

    void setLoopCount(int count) {
        _loopCount = count;
    }

    int getLoopCount() {
        return _loopCount;
    }
};

static int inferenceThreadFunc(dxrt::InferenceEngine& ie, int threadIndex, int loopCount)
{
    // inference loop
    for(int i = 0; i < loopCount; ++i)
    {
        // input
        uint8_t inputPtr[ie.input_size()] = {0, };

        // user argument
        UserData *userData = new UserData();

        // thread index 
        userData->setThreadIndex(threadIndex);

        // total loop count
        userData->setLoopCount(loopCount);

        // loop index
        userData->setLoopIndex(i);

        try
        {
            // inference asynchronously, use all npu cores
            // if device-load >= max-load-value, this function will block  
            ie.RunAsync(inputPtr, userData);
        }
        catch(const dxrt::Exception& e)
        {
            std::cerr << e.what() << " error-code=" << e.code() << std::endl;
            std::exit(-1);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            std::exit(-1);
        }
               

        std::cout << "inferenceThreadFunc thread-index=" << threadIndex << " loop-index=" << i << std::endl;

    } // for i

    return 0;

}

// invoke this function asynchronously after the inference is completed
static int onInferenceCallbackFunc(dxrt::TensorPtrs &outputs, void *userArg)
{
     std::lock_guard<std::mutex> guard(gCBMutex);

    // the outputs are guaranteed to be valid only within this callback function
    // processing this callback functions as quickly as possible is beneficial 
    // for improving inference performance

    // user data type casting
    UserData *user_data = reinterpret_cast<UserData*>(userArg);

    // thread index
    int thread_index = user_data->getThreadIndex();

    // loop index
    int loop_index = user_data->getLoopIndex();

    // post processing
    // transfer outputs to the target thread by thread_index
    // postProcessing(outputs, thread_index);
    (void)outputs;


    std::cout << "onInferenceCallbackFunc thread-index=" << thread_index << " loop-index" << loop_index << std::endl;

    // Result count per thread
    gResultCount[thread_index] ++;
    
    // complete of the loop
    if ( gResultCount[thread_index] == user_data->getLoopCount() )
    {
        gResultComplete[thread_index] = true;

        // check all threads for inference completion
        int complete_count = 0;
        for(int i = 0; i < THREAD_COUNT; ++i)
        {
            if ( gResultComplete[i] ) complete_count ++;
        }

        // inference is complete for all threads
        if ( complete_count == THREAD_COUNT ) gResultQueue.push(0);
    }

    // delete argument object
    delete user_data;

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
        std::cout << "[Usage] run_async_model_thread [dxnn-file-path] [loop-count]" << std::endl;
        return -1;
    }

    bool result = false;

    try
    {
        // create inference engine instance with model
        dxrt::InferenceEngine ie(modelPath);

        // register call back function
        ie.RegisterCallBack(onInferenceCallbackFunc);    
    
    
        auto start = std::chrono::high_resolution_clock::now();

        // thread vector 
        std::vector<std::thread> thread_array;
        for(int i = 0; i < THREAD_COUNT; ++i)
        {
            gResultComplete[i].store(false);
            gResultCount[i].store(0);

            // create thread
            thread_array.push_back(std::thread(inferenceThreadFunc, std::ref(ie), i, loop_count));
        }

        for(auto &t : thread_array)
        {
            t.join();
        } // for t

        // wait until all callbacks have been processed
        gResultQueue.pop();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        int total_result_count = 0;
        for(auto &c : gResultCount) total_result_count += c;

        result = (total_result_count == loop_count * THREAD_COUNT);

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Total Time: " << total_time << " ms" << std::endl;
        std::cout << "Average Latency: " << avg_latency << " ms" << std::endl;
        std::cout << "FPS: " << fps << " frames/sec" << std::endl;
        std::cout << "Total count=(" << total_result_count << "/" << loop_count * THREAD_COUNT << ") " 
                << (result ? "Success" : "Failure") << std::endl;
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
    
    return result ? 0 : -1;
}