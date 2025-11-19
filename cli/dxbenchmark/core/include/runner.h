#pragma once
#include <string>
#include <utility> 
#include <map>

#include "dxrt/dxrt_api.h"

using std::string;

struct Data
{
    double sd;
    double mean;
    double cv;
};

struct Result
{
    std::pair<string, string> modelName; // (Model Name, Full Path)
    float fps;
    Data infTime;
    Data latency;
};

class Runner
{
    public:
        Runner(string modelName, dxrt::InferenceOption op);
        ~Runner();
        void Run(int time, int loops, int warmup);
        const Result GetResult() const;

    private:
        dxrt::InferenceEngine _ie;
        Result result;
        std::mutex _mutex;
        std::condition_variable _cv;
        std::atomic<int> _doneCount{0};
        int _runCount = 0;
        bool _completed = false;
};