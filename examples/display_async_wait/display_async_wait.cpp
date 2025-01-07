// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "concurrent_queue.h"
#include "simple_circular_buffer_pool.h"

#include <string>
#include <iostream>

// input processing main thread
// with 2 InferenceEngine (asynchronous)
// display thread

struct FrameJobId {
    int jobId_A = -1;
    int jobId_B = -1;
    void* frameBuffer = nullptr;
    int loopIndex = -1;
};

static const int BUFFER_POOL_SIZE = 10;
static const int QUEUE_SIZE = 10;

static ConcurrentQueue<FrameJobId> gFrameJobIdQueue(QUEUE_SIZE);
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gInputBufferPool_A;
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gInputBufferPool_B;
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gFrameBufferPool;

// total display count
static std::atomic<int> gTotalDisplayCount = {0};


static void postProcessing(dxrt::TensorPtrs& outputA, dxrt::TensorPtrs& outputB)
{
    // something to do
    std::cout << "postProcessing output a=" << outputA.front()->type() 
        << " b=" << outputB.front()->type() << std::endl;
    
}

static int displayThreadFunc(int loopCount, dxrt::InferenceEngine& ieA, dxrt::InferenceEngine& ieB)
{

    while(gTotalDisplayCount < loopCount)
    {
        // consumer framebuffer & jobIds
        auto frameJobId = gFrameJobIdQueue.pop();

        // output data of ieA
        auto outputA = ieA.Wait(frameJobId.jobId_A);

        // output data of ieB
        auto outputB = ieB.Wait(frameJobId.jobId_B);

        // post-processing w/ outputA & outputB
        postProcessing(outputA, outputB);

        std::cout << "displayThreadFunc loop-index=" << frameJobId.loopIndex << std::endl;

        gTotalDisplayCount++;

        // display (update framebuffer)
    }

    return 0;
}


void preProcessing(void* inputPtr, void* frameBuffer)
{
    std::ignore = inputPtr;
    std::ignore = frameBuffer;
}

void readFrameBuffer(uint8_t* frameBuffer, int w, int h, int ch)
{
    std::memset(frameBuffer, 0, w * h * ch);
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
        std::cout << "[Usage] display_async_wait [dxnn-file-path] [loop-count]" << std::endl;
        return -1;
    }

    bool result = false;

    try
    {

        // create inference engine instance with model
        dxrt::InferenceEngine ieA(modelPath);

        std::cout << "Model-A path=" << modelPath << std::endl;
        std::cout << "        input-size=" << ieA.input_size() << " output-size=" << ieA.output_size() << std::endl;

        gInputBufferPool_A = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, ieA.input_size());
      
        // create inference engine instance with model
        dxrt::InferenceEngine ieB(modelPath);

        std::cout << "Model-B path=" << modelPath << std::endl;
        std::cout << "        input-size=" << ieB.input_size() << " output-size=" << ieB.output_size() << std::endl;

        gInputBufferPool_B = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, ieB.input_size());

        const int W = 512, H = 512, CH = 3;
        gFrameBufferPool = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, W*H*CH);
    
        auto start = std::chrono::high_resolution_clock::now();

        // create thread
        std::thread displayThread(displayThreadFunc, loop_count, std::ref(ieA), std::ref(ieB));
        
        // input processing
        for(int i = 0; i < loop_count; ++i)
        {
            uint8_t* frameBuffer = gFrameBufferPool->pointer(); 
            readFrameBuffer(frameBuffer, W, H, CH);

            uint8_t* inputA = gInputBufferPool_A->pointer();
            preProcessing(inputA, frameBuffer);

            uint8_t* inputB = gInputBufferPool_B->pointer();
            preProcessing(inputB, frameBuffer);

            // struct to pass to a thread 
            FrameJobId frameJobId;

            std::cout << "main loop-index=" << i << std::endl;

            // start inference of A model
            frameJobId.jobId_A = ieA.RunAsync(inputA);

            // start inference of B model
            frameJobId.jobId_B = ieB.RunAsync(inputB);

            // framebuffer used for input data
            frameJobId.frameBuffer = frameBuffer;
            frameJobId.loopIndex = i;

            // producer frame & jobId
            gFrameJobIdQueue.push(frameJobId);

        }

        displayThread.join();
  
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Total Time: " << total_time << " ms" << std::endl;
        std::cout << "Average Latency: " << avg_latency << " ms" << std::endl;
        std::cout << "FPS: " << fps << " frames/sec" << std::endl;

        result = gTotalDisplayCount == loop_count;
        std::cout << "Total count=(" << gTotalDisplayCount << "/" << loop_count << ") " 
                << ( result ? "Success" : "Failure") << std::endl;
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