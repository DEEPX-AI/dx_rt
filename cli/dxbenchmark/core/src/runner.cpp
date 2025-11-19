#include <string>
#include <vector>
#include <iostream>

#include "../include/runner.h"
#include "../include/utils.h"

#include "dxrt/dxrt_api.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

Runner::Runner(string name, dxrt::InferenceOption op)
    : _ie(name, op)
{

}

Runner::~Runner()
{
    // Destructor body (if needed)
}

void Runner::Run(int time, int loops, int warmup)
{
    vector<uint8_t> inputBuf(_ie.GetInputSize(), 0);
    double elapsed_time;
    _runCount = 0;

    for (int i  = 0; i < warmup; ++i)
    {
        _ie.Run(inputBuf.data());
    }

    auto call_back = [this](dxrt::TensorPtrs &outputs, void *userArg) -> int{
        std::ignore = outputs;
        std::ignore = userArg;

        int current_count = _doneCount.fetch_add(1) + 1;

        if (current_count == _runCount && _completed) {
            std::lock_guard<std::mutex> lock(_mutex);
            _cv.notify_one();
        }
        return 0;
    };  // callback used to count inference

    _ie.RegisterCallback(call_back);

    auto start = std::chrono::high_resolution_clock::now();
    if (loops > 0)
    {
        int inference_count = std::max(1, loops);

        for (int i=0 ; i < inference_count ; i++)
        {
            _ie.RunAsync(inputBuf.data());
            _runCount++;
        }
    }
    else if (time > 0)
    {
        const int TIME_CHECK_INTERVAL = 100; 

        for(int iter = 0; ; iter++)
        {
            _ie.RunAsync(inputBuf.data());
            _runCount++;
            if (iter % TIME_CHECK_INTERVAL == 0)
            {
                auto current = std::chrono::high_resolution_clock::now();
                int duration_sec = std::chrono::duration_cast<std::chrono::seconds>(current - start).count();
                if (duration_sec >= time)
                {
                    break;
                }
            }
        }
    }
    else
    {
        throw std::invalid_argument("Either time or loops must be greater than zero.");
    }

    _completed = true;

    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]{
        return _doneCount.load() == _runCount;
    });

    auto end = std::chrono::high_resolution_clock::now();


    _ie.RegisterCallback(nullptr);

    elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    result.fps = (elapsed_time > 0) ? (_runCount * 1000.0 / elapsed_time) : 0.0;

    result.latency.mean = _ie.GetLatencyMean()/1000.;
    result.latency.sd = _ie.GetLatencyStdDev()/1000.;
    result.latency.cv = (result.latency.mean != 0) ? result.latency.sd/result.latency.mean : -1;

    result.infTime.mean = _ie.GetNpuInferenceTimeMean()/1000.;
    result.infTime.sd = _ie.GetNpuInferenceTimeStdDev()/1000.;
    result.infTime.cv = (result.infTime.mean != 0) ? result.infTime.sd/result.infTime.mean : -1;
}

const Result Runner::GetResult() const
{
    return result;
}