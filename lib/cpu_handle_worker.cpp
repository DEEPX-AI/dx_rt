// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/util.h"
#include "dxrt/request.h"
#include "dxrt/task.h"

namespace dxrt {


CpuHandleWorker::CpuHandleWorker(string name_, int numThreads, CpuHandle *cpuHandle_)
: Worker(name_, Type::CPU_HANDLE, numThreads, nullptr, cpuHandle_)
{
    InitializeThread();
}
CpuHandleWorker::~CpuHandleWorker()
{
}
shared_ptr<CpuHandleWorker> CpuHandleWorker::Create(string name_, int numThreads, CpuHandle *cpuHandle_)
{
    shared_ptr<CpuHandleWorker> ret = make_shared<CpuHandleWorker>(name_, numThreads, cpuHandle_);
    return ret;
}

void CpuHandleWorker::ThreadWork(int id)
{
    std::ignore = id;
    string threadName = getName();
    int loopCnt = 0;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    while (true)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        unique_lock<mutex> lk(_lock);
        _cv.wait(
            lk, [this] {
                return _queue.size() || _stop;
            }
        );
        LOG_DXRT_DBG << threadName << " : wake up. (" << _queue.size() << ") " << endl;
        if (_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        auto req = _queue.front();
        _queue.pop();
        lk.unlock();
        if (_debugData > 0)
        {
            DataDumpBin(req->task()->name() + "_input.bin", req->inputs());
        }
        _cpuHandle->Run(req);
        if (_debugData > 0)
        {
            DataDumpBin(req->task()->name() + "_output.bin", req->outputs());
            DataDumpBin(req->task()->name() + "_output_done.bin", &loopCnt, 1);
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}

int CpuHandleWorker::request(std::shared_ptr<Request> req)
{
    unique_lock<mutex> lk(_lock);
    _queue.push(req);
    _cv.notify_all();
    return 0;
}

}  // namespace dxrt
