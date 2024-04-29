#include "gtest/gtest.h"
#include "dxrt/util.h"
#include "dxrt_test.h"

using namespace std;
using namespace dxrt;

TEST(util, select_elem)
{
    vector<uint32_t> input{1,2,3,4,5};
    vector<int> indices{0, 2, 4};
    auto output = SelectElements(input, indices);
    EXPECT_EQ(output.size(), indices.size());
    for(auto &index : indices)
    {
        EXPECT_EQ( input.at(index) - 1, index);
    }
}
