#include "gtest/gtest.h"
#include "dxrt/inference_engine.h"
#include "dxrt/util.h"
#include "dxrt_test.h"

using namespace std;

TEST(ie, basic)
{
    LOG_VALUE(testModelPath);
    dxrt::InferenceEngine ie(testModelPath);
    cout << ie << endl;
}

TEST(ie, run)
{
    LOG_VALUE(testModelPath);
    LOG_VALUE(testNum);
    dxrt::InferenceEngine ie(testModelPath);
    auto outputsOrg = ie.outputs();
    vector<uint8_t> input(ie.input_size(), 0);
    int numReq = testNum;
    atomic<int> callBackCnt;
    callBackCnt = 0;
    ie.RegisterCallBack(
        [&](dxrt::TensorPtrs &outputs, void *arg)->int {
            callBackCnt++;
            LOG_VALUE(callBackCnt);
            LOG_VALUE(reinterpret_cast<uint64_t>(arg));
            std::ignore = outputs;
            return 0;
        }
    );
    for(int i=0;i<numReq;i++)
    {
        auto outputs = ie.Run( input.data(), reinterpret_cast<void*>(i) );
        for(auto &output:outputs)
        {
            cout << *output << endl;
        }
        EXPECT_EQ(outputsOrg.size(), outputs.size());
        EXPECT_EQ(outputsOrg.front().type(), outputs.front()->type());
        EXPECT_EQ(outputsOrg.front().shape().size(), outputs.front()->shape().size());
        // auto shape = outputs.front()->shape();
        // dxrt::DataDumpTxt(
        //     "output.txt",
        //     (float*)outputs.front()->data(),
        //     shape[0],
        //     shape[1],
        //     shape[2]
        // );
    }    
    usleep(500); // wait for callback
    EXPECT_EQ(callBackCnt, numReq);
}

TEST(ie, run_async)
{
    LOG_VALUE(testModelPath);
    LOG_VALUE(testNum);
    dxrt::InferenceEngine ie(testModelPath);
    auto outputsOrg = ie.outputs();
    vector<uint8_t> input(ie.input_size(), 0);
    vector<int> requests;
    int numReq = testNum;
    atomic<int> callBackCnt;
    callBackCnt = 0;
    ie.RegisterCallBack(
        [&](dxrt::TensorPtrs &outputs, void *arg)->int {            
            std::ignore = outputs;
            std::ignore = arg;
            callBackCnt++;
            LOG_VALUE(callBackCnt);
            return 0;
        }
    );
    for(int i=0;i<numReq;i++)
    {
        // LOG_VALUE_HEX((void*)input.data());
        requests.emplace_back( ie.RunAsync( input.data(), reinterpret_cast<void*>(i) ) );
        // requests.emplace_back( ie.RunAsync(nullptr) );
    }
    // sleep(1);
    // cout << "........................" << endl;
    for(auto &request:requests)
    {
        auto outputs = ie.Wait(request);
        // for(auto &output:outputs)
        // {
        //     cout << output << endl;
        // }
        EXPECT_EQ(outputsOrg.size(), outputs.size());
        EXPECT_EQ(outputsOrg.front().type(), outputs.front()->type());
        EXPECT_EQ(outputsOrg.front().shape().size(), outputs.front()->shape().size());
    }
    usleep(10000); // wait for callback
    EXPECT_EQ(callBackCnt, numReq);
}

TEST(ie, tensor)
{
    LOG_VALUE(testModelPath);
    uint64_t inputPhyAddr = 0x90001000;
    uint64_t outputPhyAddr = 0xA0002000;
    dxrt::InferenceEngine ie(testModelPath);
    vector<uint8_t> inputBuf(ie.input_size(), 0);
    vector<uint8_t> outputBuf(ie.output_size(), 0);
    auto inputs = ie.inputs(inputBuf.data(), inputPhyAddr);
    auto outputs = ie.outputs(outputBuf.data(), outputPhyAddr);
    for(auto &tensor:inputs)
    {
        cout << tensor << endl;
    }
    for(auto &tensor:outputs)
    {
        cout << tensor << endl;
    }

}

TEST(ie, validate_device)
{
    LOG_VALUE(testModelPath);
    LOG_VALUE(testNum);
    dxrt::InferenceEngine ie(testModelPath);
    auto outputsOrg = ie.outputs();
    vector<uint8_t> input(ie.input_size(), 0);
    int numReq = testNum;
    atomic<int> callBackCnt;
    callBackCnt = 0;
    ie.RegisterCallBack(
        [&](dxrt::TensorPtrs &outputs, void *arg)->int {
            std::ignore = outputs;
            callBackCnt++;
            LOG_VALUE(callBackCnt);
            LOG_VALUE(reinterpret_cast<uint64_t>(arg));
            return 0;
        }
    );
    for(int i=0;i<numReq;i++)
    {
        auto outputs = ie.ValidateDevice( input.data(), 0 );
        EXPECT_EQ(outputs.size(), 1);
        EXPECT_EQ(outputs.front()->type(), dxrt::DataType::INT8);
        EXPECT_EQ(outputs.front()->shape().size(), 1);
    }
    usleep(500); // wait for callback
    EXPECT_EQ(callBackCnt, numReq);
}