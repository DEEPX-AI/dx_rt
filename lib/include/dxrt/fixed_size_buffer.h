// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include<cstdint>
#include<vector>
#include<mutex>


namespace dxrt {


class FixedSizeBuffer
{
 public:
    explicit FixedSizeBuffer(int64_t size, int buffer_count);
    void* getBuffer();
    void releaseBuffer(void* ptr);
    bool hasBuffer();
 private:
    std::vector<std::vector<uint8_t> > _data;
    std::vector<void*> _pointers;

    int _count;
    int64_t _size;
    std::mutex _lock;
};


}  // namespace dxrt