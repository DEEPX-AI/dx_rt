/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once


#include <vector>
#include <memory> // for std::unique_ptr and std::make_unique
#include <mutex>
#include <stdexcept>

template <typename T>
class SimpleCircularBufferPool
{
private:
    // Use std::unique_ptr for automatic memory management (RAII).
    std::vector<std::unique_ptr<T[]>> _pool;
    size_t _next_index;
    const size_t _size_per_buffer;
    mutable std::mutex _mutex; // `mutex` is mutable to allow locking in const member functions.

public:
    /**
     * @brief Constructor for CircularBufferPool.
     * @param count The number of buffers to create in the pool.
     * @param buffer_size The number of elements in each buffer.
     */
    SimpleCircularBufferPool(size_t count, size_t buffer_size)
        : _next_index(0), _size_per_buffer(buffer_size)
    {
        if (count == 0) {
            // Handle the case of a zero-sized pool.
            return;
        }
        _pool.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            // Use std::make_unique (introduced in C++14) for exception-safe memory allocation.
            _pool.emplace_back(std::make_unique<T[]>(buffer_size));
        }
    }

    // Destructor: No code needed as unique_ptr handles memory deallocation automatically.
    ~SimpleCircularBufferPool() = default;


    /**
     * @brief Returns the size (number of elements) of each individual buffer in the pool.
     * @return size_t The size of a single buffer.
     */
    size_t size_per_buffer() const {
        return _size_per_buffer;
    }

    /**
     * @brief Returns the total number of buffers in the pool.
     * @return size_t The count of buffers.
     */
    size_t pool_count() const {
        // Lock to prevent other threads from modifying the pool.
        std::lock_guard<std::mutex> lock(_mutex);
        return _pool.size();
    }

    /**
     * @brief Acquires a pointer to the next reusable buffer.
     * @return T* A pointer to the buffer. Returns nullptr if the pool is empty.
     */
    T* acquire_buffer() {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_pool.empty()) {
            return nullptr;
        }

        // Get the buffer pointer at the current index.
        T* buffer = _pool[_next_index].get();
        
        // Move to the next index (circulates using the modulo operator).
        _next_index = (_next_index + 1) % _pool.size();

        return buffer;
    }
};
