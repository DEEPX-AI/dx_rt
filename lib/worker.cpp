// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"

using std::string;
using std::endl;

namespace dxrt {

Worker::Worker(string name_, Type type_, int numThreads, Device *device_, CpuHandle *cpuHandle_)
: _device(device_), _cpuHandle(cpuHandle_), _threads(numThreads), _name(name_), _type(type_)
{
    LOG_DXRT_DBG << name_ << " will be created." << endl;
}

void Worker::DoThread(int id)
{
    ThreadWork(id);
}

void Worker::InitializeThread()
{
    for (size_t i=0; i < _threads.size(); i++)
    {
        _threads[i] = std::thread(&dxrt::Worker::DoThread, this, i);
    }
    LOG_DXRT_DBG << _name << " created." << endl;
}

Worker::Worker()
{
    LOG_DXRT_DBG << endl;
}

Worker::~Worker()
{
    std::unique_lock<std::mutex> lk(_lock);
    LOG_DXRT_DBG << "Destroying " << _name << endl;
    Stop();

    for (auto &t : _threads)
    {
        LOG_DXRT_DBG << "Detach a thread, threads :" << _threads.size()<< endl;
        t.detach();
    }

    _threads.clear();
}

void Worker::Stop()
{
    LOG_DXRT_DBG << "Stop " << _name <<" threads size : "<< _threads.size()<< endl;
    if (_threads.empty())
    {
        return;
    }
    _stop = true;
    _cv.notify_all();
}

}  // namespace dxrt
