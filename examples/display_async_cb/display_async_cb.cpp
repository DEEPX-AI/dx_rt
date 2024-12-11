// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/dxrt_api.h"
#include "concurrent_queue.h"
#include "simple_circular_data_pool.h"

#include <string>
#include <iostream>

// input processing main thread
// with 2 InferenceEngine (asynchronous)
// display 1 threads


class DataBuffer
{
    uint8_t* _buffer = nullptr;
    size_t _size = 0;
    int _type;
    int _numElements;

public:
    DataBuffer(size_t size) 
    {
        _buffer = new uint8_t[size];
        _size = size;
        //std::cout << "InputData size=" << size << std::endl;
    }

    ~DataBuffer() {
        if ( _buffer != nullptr ) delete[] _buffer;
    }

    uint8_t* getPointer() {
        return _buffer;
    }

    size_t getSize() const {
        return _size;
    }

    void setType(int type) {
        _type = type;
    }

    int getType() const {
        return _type;
    }

    void setNumElements(int num) {
        _numElements = num;
    }

    int getNumElements() const {
        return _numElements;
    }

    void copy(const uint8_t* data, size_t size) 
    {
        if ( size <= _size ) 
        {
            std::memcpy(_buffer, data, size);
        }
    }

    void reset() 
    {
        // clear buffer
        memset( _buffer, 0, _size);
    }

};


class UserData 
{
    int _loopIndex = -1;
    DataBuffer* _inputBuffer = nullptr;
public:

    void setLoopIndex(int index)
    {
        _loopIndex = index;
    }

    int getLoopIndex()
    {
        return _loopIndex;
    }

    void setInputBuffer(DataBuffer* data)
    {
        _inputBuffer = data;
    }

    DataBuffer* getInputBuffer()
    {
        return _inputBuffer;
    }

    void reset()
    {
        _loopIndex = -1;
        _inputBuffer = nullptr;
    }
};


static ConcurrentQueue<DataBuffer*> gOutputQueueA(1);
static ConcurrentQueue<DataBuffer*> gOutputQueueB(1);

// input buffer
static std::shared_ptr<SimpleCircularDataPool<DataBuffer>> gInputDataPoolA;
static std::shared_ptr<SimpleCircularDataPool<DataBuffer>> gInputDataPoolB;

// output buffer
static std::shared_ptr<SimpleCircularDataPool<DataBuffer>> gOutputDataPoolA;
static std::shared_ptr<SimpleCircularDataPool<DataBuffer>> gOutputDataPoolB;

// user data buffer
static std::shared_ptr<SimpleCircularDataPool<UserData>> gUserDataPool;

// data pool size
static const int DATA_POOL_SIZE = 100;

// total display count
static int gTotalDisplayCount = 0;


static void postProcessing(const DataBuffer* outputA, const DataBuffer* outputB)
{
    // something to do
    std::cout << "postProcessing output a=" << outputA->getType() << " b=" << outputB->getType() << std::endl;
    
}

static int displayThreadFunc(int loopCount)
{

    while(gTotalDisplayCount < loopCount)
    {
        auto outputA = gOutputQueueA.pop();
        auto outputB = gOutputQueueB.pop();

        // post-processing w/ outputA & outputB
        postProcessing(outputA, outputB);


        std::cout << "displayThreadFunc input-count=" << gTotalDisplayCount << std::endl;

        gTotalDisplayCount++;
    }

    return 0;
}

// invoke this function asynchronously after the inference is completed
static int onInferenceCallbackFunc_A(dxrt::TensorPtrs &outputs, void *userArg)
{
    // the outputs are guaranteed to be valid only within this callback function
    // processing this callback functions as quickly as possible is beneficial 
    // for improving inference performance

    // user data type casting
    UserData *user_data = reinterpret_cast<UserData*>(userArg);

    // loop index
    int loop_index = user_data->getLoopIndex();

    // Copy the output data to be handled by the displayThread
    auto outputDataA = gOutputDataPoolA->pointer();
    outputDataA->copy(reinterpret_cast<uint8_t*>(outputs.front()->data()), outputDataA->getSize());
    outputDataA->setType(outputs.front()->type());
    outputDataA->setNumElements(outputs.front()->shape().front());
    gOutputQueueA.push(outputDataA);
   

    std::cout << "onInferenceCallbackFuncA loop-index=" << loop_index << " output-size=" << outputDataA->getSize() << std::endl;
    
    return 0;
}

// invoke this function asynchronously after the inference is completed
static int onInferenceCallbackFunc_B(dxrt::TensorPtrs &outputs, void *userArg)
{
    // the outputs are guaranteed to be valid only within this callback function
    // processing this callback functions as quickly as possible is beneficial 
    // for improving inference performance

    // user data type casting
    UserData *user_data = reinterpret_cast<UserData*>(userArg);

    // loop index
    int loop_index = user_data->getLoopIndex();

    // Copy the output data to be handled by the displayThread
    auto outputDataB = gOutputDataPoolB->pointer();
    outputDataB->copy(reinterpret_cast<uint8_t*>(outputs.front()->data()), outputDataB->getSize());
    outputDataB->setType(outputs.front()->type());
    outputDataB->setNumElements(outputs.front()->shape().front());
    gOutputQueueB.push(outputDataB);
   
    std::cout << "onInferenceCallbackFuncB loop-index=" << loop_index << " output-size=" << outputDataB->getSize() << std::endl;
 
    return 0;
}

void preProcessing(DataBuffer* inputBuffer)
{
    uint8_t data[inputBuffer->getSize()] = {0, };
    inputBuffer->copy(data, inputBuffer->getSize());
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
        // user data pool
        gUserDataPool = std::make_shared<SimpleCircularDataPool<UserData>>(DATA_POOL_SIZE);

        // create inference engine instance with model
        dxrt::InferenceEngine ieA(modelPath);

        // register call back function
        ieA.RegisterCallBack(onInferenceCallbackFunc_A);

        std::cout << "Model-A path=" << modelPath << std::endl;
        std::cout << "        input-size=" << ieA.input_size() << " output-size=" << ieA.output_size() << std::endl;
        
        gInputDataPoolA = std::make_shared<SimpleCircularDataPool<DataBuffer>>(DATA_POOL_SIZE, ieA.input_size());
        gOutputDataPoolA = std::make_shared<SimpleCircularDataPool<DataBuffer>>(DATA_POOL_SIZE, ieA.output_size());

        // create inference engine instance with model
        dxrt::InferenceEngine ieB(modelPath);

        // register call back function
        ieB.RegisterCallBack(onInferenceCallbackFunc_B);

        gInputDataPoolB = std::make_shared<SimpleCircularDataPool<DataBuffer>>(DATA_POOL_SIZE, ieB.input_size());
        gOutputDataPoolB = std::make_shared<SimpleCircularDataPool<DataBuffer>>(DATA_POOL_SIZE, ieB.output_size());

        std::cout << "Model-B path=" << modelPath << std::endl;
        std::cout << "        input-size=" << ieB.input_size() << " output-size=" << ieB.output_size() << std::endl;
    
        auto start = std::chrono::high_resolution_clock::now();

        // create thread
        //std::thread inferenceThread(inferenceThreadFunc, std::ref(ieA), std::ref(ieB), loop_count);
        std::thread displayThread(displayThreadFunc, loop_count);


        // input processing
        for(int i = 0; i < loop_count; ++i)
        {

            auto inputBufferA = gInputDataPoolA->pointer();
            preProcessing(inputBufferA);

            auto inputBufferB = gInputDataPoolB->pointer();
            preProcessing(inputBufferB);

            // user data
            UserData* userDataA = gUserDataPool->pointer();
            userDataA->reset();
            userDataA->setInputBuffer(inputBufferA);

            UserData* userDataB = gUserDataPool->pointer();
            userDataB->reset();
            userDataB->setInputBuffer(inputBufferB);

            userDataA->setLoopIndex(i);
            userDataB->setLoopIndex(i);

            // copy data
            //std::cout << "Copy Data ptr=" << inputDataPtrA.get() << std::endl;
            ieA.RunAsync(inputBufferA->getPointer(), userDataA);
            ieB.RunAsync(inputBufferB->getPointer(), userDataB);

            //usleep(10000);

        }

        //inferenceThread.join();
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