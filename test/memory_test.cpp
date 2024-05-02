// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "gtest/gtest.h"
#include "dxrt/driver.h"
#include "dxrt/memory.h"
#include "dxrt/profiler.h"

using namespace std;
using namespace dxrt;

TEST(memory, basic)
{
    auto& profiler = dxrt::Profiler::GetInstance();

    dxrt_device_info_t info;
    info.mem_addr = 0x0;
    info.mem_size = 0x1000;

    Memory memory(info, nullptr);
    cout << memory << endl;
    EXPECT_NE(memory.Allocate(0x100), -1);
    EXPECT_NE(memory.Allocate(0x200), -1);
    EXPECT_NE(memory.Allocate(0x300), -1);
    EXPECT_NE(memory.Allocate(0xa00), -1);
    cout << memory << endl;
    profiler.Start("dealloc");
    memory.Deallocate(0x100);
    memory.Deallocate(0x300);
    profiler.End("dealloc");
    cout << memory << endl;
    EXPECT_NE(memory.Allocate(0x128), -1);
    cout << memory << endl;

    profiler.Show();
}

TEST(memory, get_buffer)
{
    uint32_t totalSize = 128*1024*1024;
    uint32_t requestSize = 5*1024*1024 + 1;
    int numReq = 2*totalSize/requestSize;
    auto& profiler = dxrt::Profiler::GetInstance();
    dxrt_device_info_t info;
    info.mem_addr = 0x10000000;
    info.mem_size = totalSize;
    Memory memory(info, nullptr);
    cout << memory << endl;
    uint32_t offset, prevOffset;
    for(int i=0;i<numReq;i++)
    {
        // profiler.Start("get_buffer");
        offset = memory.GetBufferAsOffset(requestSize);
        // profiler.End("get_buffer");

        if(i==0)
            EXPECT_EQ(offset, 0);
        else
            EXPECT_TRUE(offset == (prevOffset + requestSize) || offset == 0);
        prevOffset = offset;
        cout << i << ": " << offset << endl;
    }
    profiler.Show();
}

TEST(memory, inference)
{
    //auto& profiler = dxrt::Profiler::GetInstance();
    dxrt_device_info_t info;
    info.mem_addr = 0xc0000000;
    info.mem_size = 0x01000000;

    Memory memory(info, reinterpret_cast<void*>(0xd0000000));
    dxrt_request_t inference0; 
    inference0.req_id = 4;
    inference0.input.data = memory.data() + 0x1000;
    inference0.input.base = 0;
    inference0.input.offset = 0;
    inference0.input.size = 0x2000;
    inference0.output.data = memory.data() + 0x8000;
    inference0.output.base = 0;
    inference0.output.offset = 0;
    inference0.output.size = 0x3000;

    dxrt_request_t inference1; 
    inference1.req_id = 14;
    inference1.input.data = 0x1234;
    inference1.input.base = 0xe0000000;
    inference1.input.offset = 0;
    inference1.input.size = 0x2000;
    inference1.output.data = 0x5678;
    inference1.output.base = 0;
    inference1.output.offset = 0;
    inference1.output.size = 0x3000;

    dxrt_request_t inference2;
    inference2.req_id = 24;
    inference2.input.data = 0;
    inference2.input.base = info.mem_addr;
    inference2.input.offset = 0;
    inference2.input.size = 0x2000;
    inference2.output.data = 1;
    inference2.output.base = info.mem_addr;
    inference2.output.offset = 0;
    inference2.output.size = 0x3000;
    
    memory.Allocate(inference0);
    memory.Allocate(inference1);
    memory.Allocate(inference2);    
    cout << inference0 << endl;
    cout << inference1 << endl;
    cout << inference2 << endl;
    cout << memory << endl;
    EXPECT_EQ(inference0.input.base, memory.start());
    EXPECT_EQ(inference0.input.offset, inference0.input.data - memory.data());
    EXPECT_NE(inference1.input.base, memory.start());
    EXPECT_EQ(inference2.input.base, memory.start());
    EXPECT_GE(inference2.output.offset, inference2.input.size);
    memory.Deallocate(inference0);
    memory.Deallocate(inference1);
    memory.Deallocate(inference2);
    cout << memory << endl;
    // memory.Deallocate(inference2);
}