#include "gtest/gtest.h"
#include "dxrt/tensor.h"
#include "dxrt_test.h"

using namespace std;
using namespace dxrt;

TEST(tensor, basic)
{
    dxrt::Tensor tensor("test", {1,2,3,4}, dxrt::DataType::UINT16, reinterpret_cast<void*>(0xffaabb));
    cout << tensor << endl;
}

TEST(tensor, copy)
{
    dxrt::Tensor tensor("test", {1,2,3,4}, dxrt::DataType::UINT16, reinterpret_cast<void*>(0xffaabb));
    auto newTensor = Tensor(tensor, reinterpret_cast<void*>(0xcafe));
    newTensor.name() = "test-new";
    cout << tensor << endl;
    cout << newTensor << endl;
    EXPECT_EQ(newTensor.data(), reinterpret_cast<void*>(0xcafe));
    EXPECT_EQ(newTensor.name(), "test-new");
    EXPECT_EQ(tensor.name(), "test");
}