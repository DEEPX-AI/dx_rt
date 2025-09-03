/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/exception/exception.h"
#include "dxrt/device.h"

using std::string;
using std::endl;

namespace dxrt {

Worker::Worker(string name_, Type type_, int numThreads, Device *device_, CpuHandle *cpuHandle_)
: _device(device_), _cpuHandle(cpuHandle_), _threads(numThreads), _name(name_), _type(type_)
{
    LOG_DXRT_DBG << name_ << " will be created." << endl;
}

Worker::Worker()
{
    LOG_DXRT_DBG << endl;
}

void Worker::DoThread(int id)
{
    try {
        ThreadWork(id);
    } catch (dxrt::Exception& e) {
        e.printTrace();
        LOG_DXRT << "worker error " << _name <<  endl;
    }catch (std::exception& e) {
        LOG_DXRT << e.what() << " std callback error " << _name << endl;
    } catch (...) {
        LOG_DXRT << "callback error unknown " << _name << endl;
    }
    _stopCount++;
}
void Worker::InitializeThread()
{
    for (size_t i=0; i < _threads.size(); i++)
    {
        _threads[i] = std::thread(&dxrt::Worker::DoThread, this, i);
    }
    LOG_DXRT_DBG << _name << " created." << endl;
}

float Worker::GetAverageLoad() {
    std::unique_lock<std::mutex> lk(_statsLock);
    return (_checkQueueCnt.load() > 0) ? static_cast<float>(_accumulatedQueueSize.load()) / _checkQueueCnt.load() : 0.0f;
}

Worker::~Worker()
{
    LOG_DXRT_DBG << "Destroying " << _name << endl;
    _stop.store(true);

    for (auto &t : _threads)
    {
        LOG_DXRT_DBG << "Detach a thread, threads :" << _threads.size()<< endl;
        {
            std::unique_lock<std::mutex> lk(_lock);
            _cv.notify_all();
        }
        if ((_useSystemCall) && (_type == Worker::Type::DEVICE_OUTPUT))
        {
            t.detach();
        }
        else
        {
            if (t.joinable())
            {
                t.join();
            }
            else
            {
                DXRT_ASSERT(false, "CANNOT JOIN WORKER "+ _name);
            }
        }

    }
}

void Worker::Stop()
{
    LOG_DXRT_DBG << "Stop " << _name <<" threads size : "<< _threads.size()<< endl;
    if (_threads.empty())
    {
        return;
    }
    _stop.store(true);

}

void Worker::UpdateQueueStats(int queueSize) {
    std::unique_lock<std::mutex> lk(_statsLock);
    _checkQueueCnt++;
    _accumulatedQueueSize.fetch_add(queueSize);
}

bool Worker::isStopped()
{
    return (_stopCount.load() > 0);
}

void Worker::UnHold()
{
    _hold = false;
}

}  // namespace dxrt
