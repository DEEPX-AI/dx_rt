#include "gtest/gtest.h"
#include "dxrt/buffer.h"
#include "dxrt_test.h"

using namespace std;

TEST(buffer, basic)
{
    uint32_t totalSize = 128*1024*1024;
    uint32_t requestSize = 5*1024*1024;
    int numReq = 2*totalSize/requestSize;
    void *buf, *prevBuf;
    dxrt::Buffer buffer(totalSize);
    LOG_VALUE_HEX(buffer.Get());
    for(int i=0;i<numReq;i++)
    {        
        buf = buffer.Get(requestSize);
        if(i==0)
            EXPECT_EQ(buf, buffer.Get());
        else
            EXPECT_TRUE(buf == static_cast<void*>(static_cast<uint8_t*>(prevBuf) + requestSize) || buf == buffer.Get());
        prevBuf = buf;
        // cout << i << ": " << buf << endl;
    }
}