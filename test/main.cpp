#include "gtest/gtest.h"
#include "dxrt/util.h"

using namespace std;
string testModelPath;
int testNum = 1;
const char *usage =
    "DX-RT Test\n"
    "  -m    Test model path\n"
    "  -n    argument number to use in testcases\n"
    "  -h    Show help\n";
void help()
{
    cout << usage << endl;
}
int main(int argc, char *argv[])
{
    int i = 1;
    while (i < argc){
        std::string arg(argv[i++]);
        if(arg=="-m")
        {
            testModelPath = strdup(argv[i++]);
        }
        else if(arg=="-n")
        {
            testNum = stoi(argv[i++]);
        }
        else if(arg=="-h" || arg=="--help" )
        {
            help();
            exit(0);
        }
    }
    ::testing::GTEST_FLAG(output) = "xml:dxrt_test_result.xml";
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
