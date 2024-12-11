// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"

using std::string;
using std::endl;

namespace dxrt {

Worker::Worker(string name_, Type type_, int numThreads, Device *device_, CpuHandle *cpuHandle_)
: _device(device_), _cpuHandle(cpuHandle_), _name(name_), _type(type_), _threads(numThreads)
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
    Stop();
    LOG_DXRT_DBG << "Destroying " << _name << endl;
    for (size_t i = 0; i < _threads.size(); i++)
        // _threads[i].detach();
        _threads[i].detach();
    LOG_DXRT_DBG << "Destroying " << _name << " done, " << _threads.size() << endl;
    _threads.clear();
}

void Worker::Stop(void)
{
    if (_threads.empty())
        return;
    _stop = true;
    _cv.notify_all();
}



}  // namespace dxrt
