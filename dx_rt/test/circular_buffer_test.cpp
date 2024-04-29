#include "gtest/gtest.h"
#include "dxrt/circular_buffer.h"
#include "dxrt_test.h"

using namespace std;

TEST(circular_buffer, basic)
{
    int bufSize = 5;
    int numReq = 8;
    dxrt::CircularBuffer<int> buffer(bufSize);
    for(int i=0;i<numReq;i++)
    {
        buffer.Push(i);
    }
    auto result = buffer.ToVector();
    for(auto &x:result)
    {
        cout << x << endl;
    }
    EXPECT_EQ(buffer.size(), bufSize);
    EXPECT_EQ(buffer.count(), buffer.size());
}