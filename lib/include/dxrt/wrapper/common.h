/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Wrapper — Common types for prebuilt delivery.
 *
 * This header provides the dxrt namespace types needed by user code:
 * Exception, Tensor, TensorPtrs, InferenceOption.
 * It is a thin C++ header that does not depend on internal C++ classes.
 */

#pragma once
#include "dxrt/exception/exception.h"
#include "dxrt/dxrt_c_api.h"
#include "dxrt/datatype.h"
#include "dxrt/gen.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

/* DXRT_API: visibility/export macro.
 * SDK wrapper headers are header-only so this is typically empty,
 * but some legacy headers reference it. */
#ifndef DXRT_API
  #ifdef _WIN32
    #ifdef DXRT_EXPORTS
      #define DXRT_API __declspec(dllexport)
    #else
      #define DXRT_API __declspec(dllimport)
    #endif
  #else
    #define DXRT_API
  #endif
#endif

namespace dxrt {

class Tensor
{
public:
    const std::string& name() const { return name_; }
    DataType type() const { return static_cast<DataType>(type_); }
    const std::vector<int64_t>& shape() const { return shape_; }
    const void* data() const { return data_; }
    void* data() { return const_cast<void*>(data_); }
    void* data(int height, int width, int channel)
    {
        if (shape_.size() < 4 || !data_) return nullptr;
        if (height < 0 || height >= shape_[1] ||
            width  < 0 || width  >= shape_[2] ||
            channel < 0 || channel >= shape_[3]) return nullptr;
        auto C = static_cast<size_t>(shape_[3]);
        auto W = static_cast<size_t>(shape_[2]);
        size_t stride = C * elem_size_;
        size_t offset = static_cast<size_t>(height) * W * stride
                      + static_cast<size_t>(width) * stride
                      + static_cast<size_t>(channel) * elem_size_;
        return static_cast<void*>(static_cast<uint8_t*>(const_cast<void*>(data_)) + offset);
    }
    uint32_t& elem_size() { return elem_size_; }
    uint32_t elem_size() const { return elem_size_; }
    uint64_t& phy_addr() { return phy_addr_; }
    uint64_t phy_addr() const { return phy_addr_; }
    size_t size_in_bytes() const
    {
        return size_ ? size_ : calc_elems(shape_) * elem_size_;
    }

    Tensor(std::string n, int t, std::vector<int64_t> s, const void* d, size_t sz)
        : name_(std::move(n)), type_(t), shape_(std::move(s)), data_(d),
          elem_size_(sz && !shape_.empty()
              ? static_cast<uint32_t>(sz / calc_elems(shape_)) : 0),
          size_(sz)
    {
    }

    Tensor(std::string n, std::vector<int64_t> s, int t, void* d = nullptr, int memory_type = 1)
        : name_(std::move(n)), type_(t), shape_(std::move(s)), data_(d),
          elem_size_(0), size_(0), memory_type_(memory_type)
    {
    }

    Tensor(std::string n, int t, std::vector<int64_t> s, const void* d, size_t sz, bool own)
        : name_(std::move(n)), type_(t), shape_(std::move(s)),
          elem_size_(sz && !shape_.empty()
              ? static_cast<uint32_t>(sz / calc_elems(shape_)) : 0),
          size_(sz)
    {
        if (own && d && sz > 0) {
            owned_.resize(sz);
            std::memcpy(owned_.data(), d, sz);
            data_ = owned_.data();
        } else {
            data_ = d;
        }
    }

private:
    std::string name_;
    int type_;
    std::vector<int64_t> shape_;
    const void* data_;
    uint32_t elem_size_;
    size_t size_;
    int memory_type_ = 1;
    std::vector<uint8_t> owned_;
    uint64_t phy_addr_ = 0;

    static size_t calc_elems(const std::vector<int64_t>& s)
    {
        size_t n = 1;
        for (auto d : s) {
            if (d < 0) continue;
            n *= static_cast<size_t>(d);
        }
        return n;
    }
};

using TensorPtr = std::shared_ptr<Tensor>;
using TensorPtrs = std::vector<TensorPtr>;
using Tensors = std::vector<Tensor>;

class InferenceOption
{
public:
    enum BOUND_OPTION {
        NPU_ALL = 0,
        NPU_0,
        NPU_1,
        NPU_2,
        NPU_01,
        NPU_12,
        NPU_02
    };
    int bufferCount = 0;
    uint32_t boundOption = BOUND_OPTION::NPU_ALL;
    std::vector<int> devices;
    bool useORT = ort_available_();

private:
    static bool ort_available_()
    {
        static const bool val = []{
            char buf[64];
            return dxrt_config_get_ort_version(buf, sizeof(buf)) == DXRT_OK;
        }();
        return val;
    }
};

} // namespace dxrt
