#include "gtest/gtest.h"
using namespace std;

TEST(gtest, basic)
{
    cout << "hello, googletest" << endl;
    EXPECT_GT(3, 0);
    EXPECT_EQ(2, 2);
}