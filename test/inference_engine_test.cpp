#include "gtest/gtest.h"
#include "dxrt/inference_engine.h"
#include "dxrt_test.h"

using namespace std;

TEST(ie, basic)
{
    LOG_VALUE(testModelPath);
    auto ie = dxrt::InferenceEngine(testModelPath);
    cout << ie << endl;
}

TEST(ie, run)
{
    LOG_VALUE(testModelPath);
    LOG_VALUE(testNum);
    auto ie = dxrt::InferenceEngine(testModelPath);
    auto outputsOrg = ie.outputs();
    vector<uint8_t> input(ie.input_size(), 0);
    int numReq = testNum;
    atomic<int> callBackCnt = 0;
    ie.RegisterCallBack(
        [&](dxrt::TensorPtrs &outputs, void *arg)->int {
            callBackCnt++;
            LOG_VALUE(callBackCnt);
            LOG_VALUE((uint64_t)arg);
            return 0;
        }
    );
    for(int i=0;i<numReq;i++)
    {
        auto outputs = ie.Run( input.data(), (void*)i );
        for(auto &output:outputs)
        {
            cout << *output << endl;
        }
        EXPECT_EQ(outputsOrg.size(), outputs.size());
        EXPECT_EQ(outputsOrg.front().type(), outputs.front()->type());
        EXPECT_EQ(outputsOrg.front().shape().size(), outputs.front()->shape().size());
    }
    usleep(500); // wait for callback
    EXPECT_EQ(callBackCnt, numReq);
}

TEST(ie, run_async)
{
    LOG_VALUE(testModelPath);
    LOG_VALUE(testNum);
    auto ie = dxrt::InferenceEngine(testModelPath);
    auto outputsOrg = ie.outputs();
    vector<uint8_t> input(ie.input_size(), 0);
    vector<int> requests;
    int numReq = testNum;
    int callBackCnt = 0;
    ie.RegisterCallBack(
        [&](dxrt::TensorPtrs &outputs, void *arg)->int {            
            callBackCnt++;
            LOG_VALUE(callBackCnt);
            return 0;
        }
    );
    for(int i=0;i<numReq;i++)
    {
        // LOG_VALUE_HEX((void*)input.data());
        requests.emplace_back( ie.RunAsync( input.data(), (void*)i ) );
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
    auto ie = dxrt::InferenceEngine(testModelPath);
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
