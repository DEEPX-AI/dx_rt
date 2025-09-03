/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/dxrt_api.h"
#include "concurrent_queue.h"
#include "simple_circular_buffer_pool.h"
#include "../include/logger.h"

#include <string>
#include <iostream>

// input processing main thread
// with 2 InferenceEngine (asynchronous)
// display thread

struct FrameJobId {
    int jobId_A = -1;
    int jobId_B = -1;
    uint8_t* inputBufferA;
    uint8_t* inputBufferB;
    void* frameBuffer = nullptr;

    int loopIndex;
};

static const int BUFFER_POOL_SIZE = 10;
static const int QUEUE_SIZE = 10;

static ConcurrentQueue<FrameJobId> gCPUOPQueue(QUEUE_SIZE);
static ConcurrentQueue<FrameJobId> gDisplayQueue(QUEUE_SIZE);
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gInputBufferPool_A;
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gInputBufferPool_B;
static std::shared_ptr<SimpleCircularBufferPool<uint8_t>> gFrameBufferPool;

// total display count
static std::atomic<int> gTotalDisplayCount{0};


static void postProcessingA(uint8_t* buffer, dxrt::TensorPtrs& outputA)
{
    // something to do
    //std::cout << "postProcessing A output a=" << outputA.front()->type() << std::endl;
    std::ignore = buffer;
    std::ignore = outputA;
   
}

static void postProcessingB(dxrt::TensorPtrs& outputB)
{
    // something to do
    //std::cout << "postProcessing B output a=" << outputB.front()->type() << std::endl;
    std::ignore = outputB;
   
}

static int displayThreadFunc(int loopCount, dxrt::InferenceEngine& ieB)
{
    static auto& log = dxrt::Logger::GetInstance();
    while(gTotalDisplayCount.load() < loopCount)
    {
        // consumer framebuffer & jobIds
        auto frameJobId = gDisplayQueue.pop();

        // output data of ieB
        auto outputB = ieB.Wait(frameJobId.jobId_B);

        // post-processing w/ outputA & outputB
        postProcessingB(outputB);

        log.Debug("displayThreadFunc loop-index=" + std::to_string(frameJobId.loopIndex));

        gTotalDisplayCount++;

        // display (update framebuffer)
        if ( frameJobId.loopIndex == (loopCount - 1)) break;
    }

    return 0;
}

static int cpuOperationThreadFunc(int loopCount, dxrt::InferenceEngine& ieA, dxrt::InferenceEngine& ieB)
{
    static auto& log = dxrt::Logger::GetInstance();
    while(gTotalDisplayCount.load() < loopCount)
    {
        // consumer framebuffer & jobIds
        auto frameJobIdA = gCPUOPQueue.pop();

        // output data of ieA
        auto outputA = ieA.Wait(frameJobIdA.jobId_A);

        // post-processing w/ outputA
        postProcessingA(frameJobIdA.inputBufferB, outputA);

        log.Debug("cpuOperationThreadFunc loop-index=" + std::to_string(frameJobIdA.loopIndex));

        FrameJobId frameJobIdB;
        frameJobIdB.loopIndex = frameJobIdA.loopIndex;
        frameJobIdB.jobId_B = ieB.RunAsync(frameJobIdA.inputBufferB);

        gDisplayQueue.push(frameJobIdB);

        // display (update framebuffer)
        if ( frameJobIdA.loopIndex == (loopCount - 1)) break;
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
        log.Info("[Usage] display_async_pipe [dxnn-file-path] [loop-count] [--verbose|-v]");
        return -1;
    }

    if (logging) {
        log.SetLevel(dxrt::Logger::Level::DEBUG);
    }

    log.Info("Start display_async_pipe for model: " + model_path);

    bool result = false;

    try
    {

        // create inference engine instance with model
        dxrt::InferenceEngine ieA(model_path);

        log.Debug("Model-A path=" + model_path);
        log.Debug("        input-size=" + std::to_string(ieA.GetInputSize()) + " output-size=" + std::to_string(ieA.GetOutputSize()));


        gInputBufferPool_A = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, ieA.GetInputSize());
      
        // create inference engine instance with model
        dxrt::InferenceEngine ieB(model_path);

        log.Debug("Model-B path=" + model_path);
        log.Debug("        input-size=" + std::to_string(ieB.GetInputSize()) + " output-size=" + std::to_string(ieB.GetOutputSize()));

        gInputBufferPool_B = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, ieB.GetInputSize());

        const int W = 512, H = 512, CH = 3;
        gFrameBufferPool = std::make_shared<SimpleCircularBufferPool<uint8_t>>(BUFFER_POOL_SIZE, W*H*CH);
    
        auto start = std::chrono::high_resolution_clock::now();

        // create thread
        std::thread cpuOperationThread(cpuOperationThreadFunc, loop_count, std::ref(ieA), std::ref(ieB));
        std::thread displayThread(displayThreadFunc, loop_count, std::ref(ieB));
        
        
        // input processing
        for(int i = 0; i < loop_count; ++i)
        {
            uint8_t* frameBuffer = gFrameBufferPool->pointer(); 
            readFrameBuffer(frameBuffer, W, H, CH);

            uint8_t* inputA = gInputBufferPool_A->pointer();
            preProcessing(inputA, frameBuffer);

            // struct to pass to a thread 
            FrameJobId frameJobId;

            frameJobId.inputBufferA = inputA;
            frameJobId.inputBufferB = gInputBufferPool_B->pointer();

            // start inference of A model
            frameJobId.jobId_A = ieA.RunAsync(inputA);

            // framebuffer used for input data
            frameJobId.frameBuffer = frameBuffer;
            frameJobId.loopIndex = i;

            // producer frame & jobId
            gCPUOPQueue.push(frameJobId);

        }

        cpuOperationThread.join();
        displayThread.join();
  
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        double total_time = duration.count();
        double avg_latency = total_time / static_cast<double>(loop_count);
        double fps = 1000.0 / avg_latency;

        log.Info("-----------------------------------");
        log.Info("Total Time: " + std::to_string(total_time) + " ms");
        log.Info("Average Latency: " + std::to_string(avg_latency) + " ms");
        log.Info("FPS: " + std::to_string(fps) + " frames/sec");

        result = gTotalDisplayCount.load() == loop_count;
        log.Info("Total count=(" + std::to_string(gTotalDisplayCount.load()) + "/" + std::to_string(loop_count) + ") " + 
                (result ? "Success" : "Failure"));
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
