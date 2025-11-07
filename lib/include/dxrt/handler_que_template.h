/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#pragma once

#include <memory>
#include "dxrt/common.h"
#include "dxrt/device_struct.h"
#include "dxrt/driver.h"
#include "dxrt/device_core.h"
#include "dxrt/memory_interface.h"
#include "dxrt/exception/exception.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace dxrt {


template <typename T>
class HandlerQueueThread
{
public:
    HandlerQueueThread(std::string name_, size_t numThreads, std::function<int(const T&, int)> handler_)
    : _name(name_), _threads(), _handler(handler_), _numThreads(numThreads) {}
    void PushWork(const T& x);

    void Signal();

    void Start();

    void Stop() { _stop.store(true); }

    void UpdateQueueStats(int queueSize);
    float GetAverageLoad();

    ~HandlerQueueThread();
    std::string name() const { return _name; }

protected:

    std::string _name;

    std::queue<T> _que;
    std::vector<std::thread> _threads;
    std::mutex _lock;
    std::mutex _statsLock;
    std::condition_variable _cv;
    std::atomic<bool> _stop {false};
    std::atomic<int> _stopCount{0};




    std::function<int(const T&, int)> _handler;
    std::atomic<int> _checkQueueCnt{0};
    std::atomic<int> _accumulatedQueueSize{0};
    size_t _numThreads;

    void DoThread(int id);
    void ThreadWork(int id);
};

template <typename T>
void HandlerQueueThread<T>::Signal()
{
    std::unique_lock<std::mutex> lock (_lock);
    _cv.notify_all();
}
template <typename T>
void HandlerQueueThread<T>::PushWork(const T& x)
{
    std::unique_lock<std::mutex> lock (_lock);
    _que.push(x);
    _cv.notify_all();
}
template <typename T>
void HandlerQueueThread<T>::ThreadWork(int id)
{
    while (_stop.load(std::memory_order_acquire) == false)
    {
        T response;
        {
            std::unique_lock<std::mutex> lk(_lock);
            _cv.wait(
                lk, [this] {
                    return _que.size() || _stop.load(std::memory_order_acquire);
                }
            );


            // stop handler
            if (_stop.load(std::memory_order_acquire))
            {
                return;
            }

            response = _que.front();
            _que.pop();
        }
        _handler(response, id);
        if (_stop.load(std::memory_order_acquire))
        {
            return;
        }
    }
}

template <typename T>
void HandlerQueueThread<T>::DoThread(int id)
{
    try {
        ThreadWork(id);
    } catch (dxrt::Exception& e) {
        e.printTrace();
        LOG_DXRT << "worker error " << _name << "\n";
    }catch (std::exception& e) {
        LOG_DXRT << e.what() << " std callback error " << _name << "\n";
    } catch (...) {
        LOG_DXRT << "callback error unknown " << _name << "\n";
    }
    _stopCount++;
}
template <typename T>
void HandlerQueueThread<T>::Start()
{
    _threads.reserve(_numThreads);
    for (size_t i = 0; i < _numThreads; i++)
    {
        _threads.emplace_back(&HandlerQueueThread<T>::DoThread, this, i);
    }
    LOG_DXRT_DBG << _name << " created." << "\n";
}


template <typename T>
HandlerQueueThread<T>::~HandlerQueueThread()
{
LOG_DXRT_DBG << "Destroying " << _name << "\n";
    _stop.store(true);

    for (auto &t : _threads)
    {
        LOG_DXRT_DBG << "Detach a thread, threads :" << _threads.size()<< "\n";
        {
            std::unique_lock<std::mutex> lk(_lock);
            _cv.notify_all();
        }

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
template <typename T>
void HandlerQueueThread<T>::UpdateQueueStats(int queueSize) {
    std::unique_lock<std::mutex> lk(_statsLock);
    _checkQueueCnt++;
    _accumulatedQueueSize.fetch_add(queueSize);
}

template <typename T>
float HandlerQueueThread<T>::GetAverageLoad() {
    std::unique_lock<std::mutex> lk(_statsLock);
    return (_checkQueueCnt.load() > 0) ? static_cast<float>(_accumulatedQueueSize.load()) / _checkQueueCnt.load() : 0.0f;
}

}  // namespace dxrt
