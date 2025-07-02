#include "dxrt/dxrt_api.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __linux__
#include <getopt.h>

static struct option const opts[] = {
    { "model", required_argument, 0, 'm' },
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 }
};
#endif 

const char* usage = "parse model\n"
                    "  -m, --model     model path\n"
                    "  -h, --help      show help\n";

void help()
{
    cout << usage << endl;    
}

int main(int argc, char *argv[])
{
    int ret;
    string modelPath="";
    if(argc==1)
    {
        cout << "Error: no arguments." << endl;
        help();
        return -1;
    }

#ifdef __linux__
    int optCmd;
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
#elif _WIN32
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-m" || arg == "--model") {
            if (i + 1 < argc) {
                modelPath = argv[++i];
            }
            else {
                cerr << "Error: -m option requires an argument." << endl;
                return -1;
            }
        }
        else if (arg == "-h" || arg == "--help") {
            help();
            return 0;
        }
    }
#endif

    LOG_VALUE(modelPath);
    DXRT_ASSERT(!modelPath.empty(), "no model path");
    ret = dxrt::ParseModel(modelPath);
    return ret;
}