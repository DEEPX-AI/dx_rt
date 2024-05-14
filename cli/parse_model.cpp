#include <getopt.h>
#include "dxrt/dxrt_api.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////
static struct option const opts[] = {
    { "model", required_argument, 0, 'm' },
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 }
};
const char* usage =
"parse model\n"
"  -m, --model     model path\n"
"  -h, --help      show help\n"
;
void help()
{
    cout << usage << endl;    
}

int main(int argc, char *argv[])
{
    int optCmd, ret;
    string modelPath="";
    if(argc==1)
    {
        cout << "Error: no arguments." << endl;
        help();
        return -1;
    }

    while ((optCmd = getopt_long(argc, argv, "m:h", opts,
        NULL)) != -1) {
        switch (optCmd) {
            case '0':
                break;
            case 'm':
                modelPath = strdup(optarg);
                break;
            case 'h':
            default:
                help();
                exit(0);
                break;
        }
    }
    LOG_VALUE(modelPath);
    DXRT_ASSERT(!modelPath.empty(), "no model path");
    ret = dxrt::ParseModel(modelPath);
    return ret;
}