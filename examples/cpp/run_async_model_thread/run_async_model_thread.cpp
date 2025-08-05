// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "../include/logger.h"
#include "concurrent_queue.h"

#include <string>
#include <iostream>

// 1 InferenceEngine and 3 threads (asynchronous)


static const int THREAD_COUNT = 3;
static std::atomic<int> gResultCount = {0};
static std::atomic<int> gTotalCount = {0};
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

static int inferenceThreadFunc(dxrt::InferenceEngine& ie, std::vector<uint8_t>& inputPtr, int threadIndex, int loopCount)
{
    static auto& log = dxrt::Logger::GetInstance();
    // inference loop
    for(int i = 0; i < loopCount; ++i) 
    {
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
            ie.RunAsync(inputPtr.data(), userData);
        }
        catch(const dxrt::Exception& e)
        {
            log.Error(std::string("DXRT Exception: ") + e.what() + " error-code=" + std::to_string(e.code()));
            std::exit(-1);
        }
        catch(const std::exception& e)
        {
            log.Error(std::string("std::exception: ") + e.what());
            std::exit(-1);
        }
        log.Debug("inferenceThreadFunc thread-index=" + std::to_string(threadIndex) +
          " loop-index=" + std::to_string(i));

    } // for i

    

    return 0;

}

// invoke this function asynchronously after the inference is completed
static int onInferenceCallbackFunc(dxrt::TensorPtrs &outputs, void *userArg)
{
    static auto& log = dxrt::Logger::GetInstance();
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


    log.Debug("onInferenceCallbackFunc thread-index=" + std::to_string(thread_index) +
          " loop-index=" + std::to_string(loop_index));

    // result count
    {
        // Mutex locks should be properly adjusted 
        // to ensure that callback functions are thread-safe.
        std::lock_guard<std::mutex> lock(gCBMutex);

        gResultCount++;
        if ( gResultCount.load() == gTotalCount.load() ) gResultQueue.push(0);
    }

    // delete argument object
    delete user_data;

    return 0;
}


int main(int argc, char* argv[])
{
    const int DEFAULT_LOOP_COUNT = 1;
    
    std::string model_path;
    int loop_count = DEFAULT_LOOP_COUNT;
    bool logging = false;

    auto &log = dxrt::Logger::GetInstance();

    if ( argc > 1 )
    {
        model_path = argv[1];

        if ( argc > 2 ) 
        {
            loop_count = std::stoi(argv[2]);

            if (argc > 3 )
            {
                std::string last_arg = argv[3];
                if (last_arg == "--verbose" || last_arg == "-v")
                {
                    logging = true;
                }
            }
        }
    }
    else
    {
        log.Info("[Usage] run_async_model_thread [dxnn-file-path] [loop-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start async_model_thread test for model: " + model_path);

    bool result = false;

    try
    {
        // create inference engine instance with model
        dxrt::InferenceEngine ie(model_path);

        // register call back function
        ie.RegisterCallback(onInferenceCallbackFunc);    
    
    
        auto start = std::chrono::high_resolution_clock::now();

        // create temporary input buffer for example
        std::vector<uint8_t> inputPtr(ie.GetInputSize(), 0);

        gTotalCount.store(loop_count * THREAD_COUNT);

        // thread vector 
        std::vector<std::thread> thread_array;

        for(int i = 0; i < THREAD_COUNT; ++i)
        {
            // create thread
            thread_array.push_back(std::thread(inferenceThreadFunc, std::ref(ie), std::ref(inputPtr), i, loop_count));
        }

        for(auto &t : thread_array)
        {
            t.join();
        } // for t


        // wait until all callbacks have been processed
        gResultQueue.pop();
        

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        result = (gResultCount.load() == (loop_count * THREAD_COUNT));

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        log.Info("-----------------------------------");
        log.Info("Total Time: " + std::to_string(total_time) + " ms");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms");
        log.Info("FPS: " + std::to_string(fps) + " frames/sec");
        log.Info("Total count=(" + std::to_string(gResultCount.load()) + "/" +
                std::to_string(loop_count * THREAD_COUNT) + ") " + (result ? "Success" : "Failure"));
        log.Info("-----------------------------------");

    }
    catch (const dxrt::Exception& e)
    {
        log.Error(std::string(e.what()) + " error-code=" + std::to_string(e.code()));
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
    
    return result ? 0 : -1;
}