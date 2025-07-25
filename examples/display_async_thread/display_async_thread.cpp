// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "concurrent_queue.h"

#include <string>
#include <iostream>

// 1 InferenceEngine and 500 threads (asyncronous)

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

// submits inference jobs using shared InferenceEngine
static int inferenceThreadFunc(dxrt::InferenceEngine& ie, std::vector<uint8_t>& inputPtr, int threadIndex, int loopCount)
{
    //inference Loop
    for(int i = 0; i < loopCount; ++i )
    {
        //user argument
        UserData *userData = new UserData();

        //thread index
        userData->setThreadIndex(threadIndex);

        //total loop count
        userData->setLoopCount(loopCount);

        //total index
        userData->setLoopIndex(i);

        try
        {
            // inference asynchronously, use all npu cores
            // if device-load >= max-load-value, this function will block  
            ie.RunAsync(inputPtr.data(), userData);
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
    
    }// for i
    
    return 0;
}

// invoke this function asynchronously after the inference is completed
static int onInferenceCallbackFunc(dxrt::TensorPtrs &outputs, void *userArg)
{

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


    std::cout << "onInferenceCallbackFunc thread-index=" << thread_index << " loop-index=" << loop_index << std::endl;


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
    const int DEFAULT_THREAD_COUNT = 3;
    const bool ENABLE_ORT = false;
    
    std::string model_path;
    int loop_count = DEFAULT_LOOP_COUNT;
    int thread_count = DEFAULT_THREAD_COUNT;

    if ( argc > 1 )
    {
        model_path = argv[1];

        if ( argc > 2 ) 
        {
            loop_count = std::stoi(argv[2]);
        }

        if ( argc > 3)
        {
            thread_count = std::stoi(argv[3]);
        }

    }
    else
    {
        std::cout << "[Usage] display_async_thread [dxnn-file-path] [loop-count] [thread-count]" << std::endl;
        return -1;
    }

    bool result = false;
    
    try     
    {
        
        // create inference engine instance with model
        dxrt::InferenceOption io;
        io.useORT = ENABLE_ORT;

        dxrt::InferenceEngine ie(model_path, io);

        // register call back function
        ie.RegisterCallback(onInferenceCallbackFunc);

        auto start = std::chrono::high_resolution_clock::now();

        // create input buffers for each thread (same size as model input)
        std::vector<uint8_t> input_buffer(ie.GetInputSize(), 0); 

        gTotalCount.store(loop_count * thread_count);

        // launch THREAD_COUNT threads to submit async inference jobs
        std::vector<std::thread> threads;
        threads.reserve(thread_count);
        
        for(int t = 0; t < thread_count; ++t) {
            threads.emplace_back(inferenceThreadFunc, std::ref(ie), std::ref(input_buffer), t, loop_count);
        }

        // wait for all threads to complete
        for (auto& th: threads) {
            th.join();
        }

        // wait until all callbacks have been processed
        gResultQueue.pop();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        
        result = (gResultCount.load() == (loop_count * thread_count));
        
        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Total Time: " << total_time << " ms" << std::endl;
        std::cout << "Average Latency: " << avg_latency << " ms" << std::endl;
        std::cout << "FPS: " << fps << " frames/sec" << std::endl;
        std::cout << "Total count=(" << gResultCount.load() << "/" << loop_count * thread_count << ") " 
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