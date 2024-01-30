// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "gtest/gtest.h"
#include "dxrt/tensor.h"
#include "dxrt/request.h"
#include "dxrt_test.h"

using namespace std;
using namespace dxrt;

TEST(request, basic)
{    
    {
        Request::Clear();
        vector<RequestPtr> requests;
        for(int i=1; i<5; i++)
        {
            vector<Tensor> inputs, outputs;
            inputs.emplace_back(
                dxrt::Tensor("input0", {1,2,3,4}, dxrt::DataType::UINT16, (void*)(0x1000*i+0xffaabb) )
            );
            outputs.emplace_back(
                dxrt::Tensor("output0", {1,5,23,100}, dxrt::DataType::UINT8, (void*)(0x1000*i+0x1) )
            );
            outputs.emplace_back(
                dxrt::Tensor("output1", {1,15,42,75}, dxrt::DataType::UINT8, (void*)(0x1000*i+0x2) )
            );
            outputs.emplace_back(
                dxrt::Tensor("output2", {3,5,2}, dxrt::DataType::UINT8, (void*)(0x1000*i+0x3) )
            );
            requests.emplace_back(
                Request::Create(nullptr, inputs, outputs)
            );            
            auto &req = requests.back();
            // cout << *req << endl;
            EXPECT_EQ(req.use_count(), 1);
            EXPECT_EQ(req->id(), i);
            EXPECT_TRUE(!req->inputs().empty());
            EXPECT_TRUE(!req->outputs().empty());
        }        
    }
    {
        Request::Clear();
        Request::Init();
        dxrt::Request::ShowAll();
    }
}

