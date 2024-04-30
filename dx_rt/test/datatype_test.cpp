#include "gtest/gtest.h"
#include "dxrt/datatype.h"
#include "dxrt_test.h"

using namespace std;
using namespace dxrt;

TEST(datatype, basic)
{
    for(int i = DataType::NONE_TYPE; i<DataType::MAX_TYPE; i++)
    {
        DataType type = static_cast<DataType>(i);
        ostringstream os;
        os << type;
        cout << os.str() << endl;
        EXPECT_EQ(DataTypeToString(type), os.str());
    }
}