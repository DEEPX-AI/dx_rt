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
    // Use std::vector for automatic memory management (RAII).
    std::vector<std::vector<T> > _pool;
    size_t _next_index = 0;
    const size_t _size_per_buffer;
    mutable std::mutex _mutex;  // `mutex` is mutable to allow locking in const member functions.

 public:
    /**
     * @brief Constructor for CircularBufferPool.
     * @param count The number of buffers to create in the pool.
     * @param buffer_size The number of elements in each buffer.
     */
    SimpleCircularBufferPool(size_t count, size_t buffer_size)
        : _pool(count, std::vector<T>(buffer_size)), _size_per_buffer(buffer_size)
    {
        if (count == 0 || buffer_size == 0) {
            throw std::invalid_argument("Buffer count and size must be greater than zero."
             "count: " + std::to_string(count) + ", size: " + std::to_string(buffer_size));
        }
    }

    // Destructor: No code needed as vector handles memory deallocation automatically.
    ~SimpleCircularBufferPool() = default;

    // Delete copy constructor and copy assignment operator
    SimpleCircularBufferPool(const SimpleCircularBufferPool&) = delete;
    SimpleCircularBufferPool& operator=(const SimpleCircularBufferPool&) = delete;

    // Define move constructor and move assignment operator
    SimpleCircularBufferPool(SimpleCircularBufferPool&&) noexcept = default;
    SimpleCircularBufferPool& operator=(SimpleCircularBufferPool&&) noexcept = default;



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
        auto buffer = _pool[_next_index].data();

        // Move to the next index (circulates using the modulo operator).
        _next_index = (_next_index + 1) % _pool.size();

        return buffer;
    }
};
