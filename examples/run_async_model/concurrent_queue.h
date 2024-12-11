// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue(size_t max_size) : _max_size(max_size) {}

    // push data (wait if the queue is full)
    void push(const T& value) {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv_push.wait(lock, [this]() { return _q.size() < _max_size; }); // Wait until there is space in the queue
        _q.push(value);
        _cv_pop.notify_one(); // Wake up the thread waiting for a pop
    }

    // pop data (wait if the queue is empty)
    T pop() {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv_pop.wait(lock, [this]() { return !_q.empty(); }); // Wait until a push occurs in the queue
        T value = _q.front();
        _q.pop();
        _cv_push.notify_one(); // Wake up the thread waiting for a push
        return value;
    }

    // check if the queue is empty
    bool empty() {
        std::lock_guard<std::mutex> lock(_mtx);
        return _q.empty();
    }

    // clear all items
    void clear() {
        std::lock_guard<std::mutex> lock(_mtx);
        while (!_q.empty()) {
            _q.pop();
        }
    }

    // current queue size
    size_t size() const {
        std::lock_guard<std::mutex> lock(_mtx);
        return _q.size();
    }

    // max queue size
    size_t maxSize() const {
        std::lock_guard<std::mutex> lock(_mtx);
        return _max_size;
    }

private:
    std::queue<T> _q; // stl queue
    std::mutex _mtx;  // mutex lock
    std::condition_variable _cv_push; // push condition variable
    std::condition_variable _cv_pop;  // pop confition variable
    size_t _max_size;  // max queue size
};