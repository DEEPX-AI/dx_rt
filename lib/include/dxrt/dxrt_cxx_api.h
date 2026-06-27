/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT C++ API — Single-file header-only wrapper over the C ABI.
 *
 * This header provides the complete dxrt C++ API by wrapping every
 * function in dxrt_c_api.h.  It is functionally equivalent to
 * #include "dxrt/dxrt_api.h" (the multi-file wrapper umbrella)
 * but delivered as a single self-contained header.
 *
 * Usage:
 *   #include "dxrt/dxrt_cxx_api.h"
 *
 * Requirements: C++14 or later.
 *
 * WARNING: Do NOT include both dxrt_api.h and dxrt_cxx_api.h in the
 *          same translation unit — they define the same class names
 *          and will cause ODR violations.
 */

#ifndef DXRT_CXX_API_H
#define DXRT_CXX_API_H

/* ── ODR guard ───────────────────────────────────────────────────
 * dxrt_cxx_api.h and the wrapper headers each declare their own
 * dxrt::Exception (different layouts, different accessors). Mixing
 * them in the same translation unit is silently broken — fail the
 * build loudly instead.
 */
#define DXRT_CXX_API_H_INCLUDED
#ifdef DXRT_WRAPPER_HEADERS_INCLUDED
# error "dxrt_cxx_api.h and dxrt/wrapper/*.h cannot be included in the same translation unit (two dxrt::Exception classes — ODR violation risk). Use one or the other."
#endif

#include "dxrt/dxrt_c_api.h"
#include "dxrt/gen.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <ostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace dxrt {

/* ================================================================
 *  Exception
 * ================================================================ */

class Exception : public std::runtime_error
{
public:
    explicit Exception(const char* msg, dxrt_status_t st = DXRT_ERR_INTERNAL)
        : std::runtime_error(msg), status_(st) {}
    explicit Exception(const std::string& msg, dxrt_status_t st = DXRT_ERR_INTERNAL)
        : std::runtime_error(msg), status_(st) {}
    dxrt_status_t status() const noexcept { return status_; }
    dxrt_status_t code() const noexcept { return status_; }
private:
    dxrt_status_t status_;
};

namespace detail {

inline void check(dxrt_status_t st)
{
    if (st != DXRT_OK)
    {
        const char* msg = dxrt_last_error_message();
        throw Exception(msg ? msg : "unknown dxrt error", st);
    }
}

} // namespace detail

/* ================================================================
 *  Tensor / TensorPtr / TensorPtrs
 * ================================================================ */

enum DataType
{
    NONE_TYPE = 0, FLOAT, UINT8, INT8, UINT16, INT16,
    INT32, INT64, UINT32, UINT64, BBOX, FACE, POSE, MAX_TYPE,
};

inline std::string DataTypeToString(DataType type)
{
    switch (type)
    {
    case NONE_TYPE: return "NONE";
    case FLOAT:    return "FLOAT";
    case UINT8:    return "UINT8";
    case INT8:     return "INT8";
    case UINT16:   return "UINT16";
    case INT16:    return "INT16";
    case INT32:    return "INT32";
    case INT64:    return "INT64";
    case UINT32:   return "UINT32";
    case UINT64:   return "UINT64";
    case BBOX:     return "BBOX";
    case FACE:     return "FACE";
    case POSE:     return "POSE";
    default:       return "UNKNOWN";
    }
}

inline std::string DataTypeToString(int type)
{
    return DataTypeToString(static_cast<DataType>(type));
}

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

    // Owning constructor — copies data into internal buffer
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

} // namespace dxrt

namespace dxrt {

/* ================================================================
 *  InferenceOption
 * ================================================================ */

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
        NPU_02,
    };
    int bufferCount = 0;           // 0 = library default (DXRT_TASK_MAX_LOAD_VALUE)
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

namespace detail {
inline InferenceOption& default_option()
{
    static InferenceOption instance;
    return instance;
}
} // namespace detail

/* ================================================================
 *  InferenceEngine  (dxrt_engine_*)
 * ================================================================ */

class InferenceEngine
{
public:
    /* ── Lifecycle ──────────────────────────────────────────── */

    explicit InferenceEngine(const std::string& modelPath,
                             InferenceOption& option = detail::default_option())
    {
        dxrt_options_t opts;
        dxrt_options_init(&opts);
        opts.bound_option = static_cast<int>(option.boundOption);
        opts.buffer_count = option.bufferCount;
        opts.use_ort = option.useORT ? 1 : 0;
        if (option.devices.size() > 1)
        {
            detail::check(dxrt_engine_create_with_devices(
                modelPath.c_str(), &opts,
                option.devices.data(),
                static_cast<int>(option.devices.size()),
                &h_));
        }
        else
        {
            if (!option.devices.empty())
            {
                opts.device_id = option.devices[0];
            }
            detail::check(dxrt_engine_create(modelPath.c_str(), &opts, &h_));
        }
    }

    InferenceEngine(const uint8_t* data, size_t size,
                    InferenceOption& option = detail::default_option())
    {
        dxrt_options_t opts;
        dxrt_options_init(&opts);
        opts.bound_option = static_cast<int>(option.boundOption);
        opts.buffer_count = option.bufferCount;
        opts.use_ort = option.useORT ? 1 : 0;
        if (option.devices.size() > 1)
        {
            detail::check(dxrt_engine_create_from_memory_with_devices(
                data, size, &opts,
                option.devices.data(),
                static_cast<int>(option.devices.size()),
                &h_));
        }
        else
        {
            if (!option.devices.empty())
            {
                opts.device_id = option.devices[0];
            }
            detail::check(dxrt_engine_create_from_memory(data, size, &opts, &h_));
        }
    }

    ~InferenceEngine()
    {
        if (h_) dxrt_engine_destroy(h_);
    }

    InferenceEngine(const InferenceEngine&) = delete;
    InferenceEngine& operator=(const InferenceEngine&) = delete;

    // K1: Move ctor/assign are deleted because the C-ABI callback
    // (dxrt_engine_register_callback) holds `user_data = this`.
    // Moving the wrapper would leave the engine pointing at the moved-from
    // instance, causing a use-after-free when the next callback fires.
    // Transfer ownership via std::unique_ptr<InferenceEngine> instead.
    InferenceEngine(InferenceEngine&&) = delete;
    InferenceEngine& operator=(InferenceEngine&&) = delete;

    void Dispose()
    {
        detail::check(dxrt_engine_dispose(h_));
    }

    /* ── Inference ─────────────────────────────────────────── */

    TensorPtrs Run(void* input, void* userArg = nullptr, void* output = nullptr)
    {
        (void)userArg;
        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        std::vector<uint8_t> buf;
        void* out_ptr = output;
        bool owns = false;
        if (!out_ptr)
        {
            buf.resize(static_cast<size_t>(out_sz));
            out_ptr = buf.data();
            owns = true;
        }
        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        detail::check(dxrt_engine_run_with_tensor_info(
            h_, input, out_ptr, count > 0 ? infos.data() : nullptr, &count));
        infos.resize(static_cast<size_t>(count));
        return build_output_tensors_from_infos(out_ptr, out_sz, owns, infos);
    }

    int RunAsync(void* input, void* userArg = nullptr, void* output = nullptr)
    {
        int job_id = -1;
        detail::check(dxrt_engine_run_async(h_, input, userArg, output, &job_id));
        return job_id;
    }

    TensorPtrs Wait(int jobId) const
    {
        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        std::vector<uint8_t> buf(static_cast<size_t>(out_sz));
        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        detail::check(dxrt_engine_wait_with_tensor_info(
            h_, jobId, buf.data(), count > 0 ? infos.data() : nullptr, &count));
        infos.resize(static_cast<size_t>(count));
        return build_output_tensors_from_infos(buf.data(), out_sz, true, infos);
    }

    TensorPtrs Wait(int jobId, void* output) const
    {
        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        detail::check(dxrt_engine_wait_with_tensor_info(
            h_, jobId, output, count > 0 ? infos.data() : nullptr, &count));
        infos.resize(static_cast<size_t>(count));
        return build_output_tensors_from_infos(static_cast<uint8_t*>(output), out_sz, false, infos);
    }

    /**
     * Register an async inference completion callback.
     *
     * @warning Zero-copy lifetime contract: the `TensorPtrs` passed to your
     *          callback wraps engine-owned buffers that are valid ONLY for
     *          the duration of the callback invocation. Do NOT store the
     *          TensorPtrs (or any Tensor pointer from it) for use after the
     *          callback returns — doing so leads to a dangling pointer. If
     *          you need the data later, copy it out (e.g. into your own
     *          buffer or a numpy array) before returning from the callback.
     */
    void RegisterCallback(std::function<int(TensorPtrs&, void*)> cb)
    {
        cb_ = std::move(cb);
        detail::check(dxrt_engine_register_callback(h_, &callback_trampoline, this));
    }

    float RunBenchmark(int num, void* inputPtr = nullptr)
    {
        float fps = 0.0f;
        detail::check(dxrt_engine_run_benchmark(h_, num, inputPtr, &fps));
        return fps;
    }

    /* ── Batch / Multi-Input Inference ─────────────────────── */

    std::vector<TensorPtrs> Run(
        const std::vector<void*>& inputBuffers,
        const std::vector<void*>& outputBuffers,
        const std::vector<void*>& userArgs = {})
    {
        (void)userArgs;
        int batch = static_cast<int>(inputBuffers.size());
        std::vector<const void*> in_ptrs(inputBuffers.begin(), inputBuffers.end());
        std::vector<void*> out_ptrs(outputBuffers);

        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));

        // Allocate output buffers if not provided
        std::vector<std::vector<uint8_t>> owned_bufs;
        bool owning = out_ptrs.empty() || out_ptrs[0] == nullptr;
        if (owning)
        {
            owned_bufs.resize(static_cast<size_t>(batch));
            out_ptrs.resize(static_cast<size_t>(batch));
            for (int i = 0; i < batch; ++i)
            {
                owned_bufs[static_cast<size_t>(i)].resize(static_cast<size_t>(out_sz));
                out_ptrs[static_cast<size_t>(i)] = owned_bufs[static_cast<size_t>(i)].data();
            }
        }

        int output_count = 0;
        detail::check(dxrt_engine_get_output_tensor_info(h_, nullptr, &output_count));
        int info_count = batch * output_count;
        std::vector<dxrt_tensor_info_t> infos(static_cast<size_t>(info_count));
        detail::check(dxrt_engine_run_batch_with_tensor_info(
            h_, in_ptrs.data(), out_ptrs.data(), batch,
            info_count > 0 ? infos.data() : nullptr, &info_count));
        if (info_count % batch != 0)
            throw Exception("runtime output metadata count is not divisible by batch size",
                            DXRT_ERR_INTERNAL);
        const int actual_output_count = info_count / batch;
        infos.resize(static_cast<size_t>(info_count));

        std::vector<TensorPtrs> results;
        for (int i = 0; i < batch; ++i) {
            auto first = infos.begin() + static_cast<ptrdiff_t>(i * actual_output_count);
            auto last = first + static_cast<ptrdiff_t>(actual_output_count);
            results.push_back(build_output_tensors_from_infos(
                out_ptrs[static_cast<size_t>(i)], out_sz, owning,
                std::vector<dxrt_tensor_info_t>(first, last)));
        }
        return results;
    }

    TensorPtrs RunMultiInput(const std::map<std::string, void*>& inputTensors,
                             void* userArg = nullptr, void* outputPtr = nullptr)
    {
        (void)userArg;
        int num = static_cast<int>(inputTensors.size());
        std::vector<const char*> names;
        std::vector<const void*> buffers;
        for (auto& kv : inputTensors)
        {
            names.push_back(kv.first.c_str());
            buffers.push_back(kv.second);
        }

        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        std::vector<uint8_t> buf;
        void* out_ptr = outputPtr;
        if (!out_ptr)
        {
            buf.resize(static_cast<size_t>(out_sz));
            out_ptr = buf.data();
        }

        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        detail::check(dxrt_engine_run_multi_input_with_tensor_info(
            h_, names.data(), buffers.data(), num, out_ptr,
            count > 0 ? infos.data() : nullptr, &count));
        infos.resize(static_cast<size_t>(count));
        return build_output_tensors_from_infos(out_ptr, out_sz, outputPtr == nullptr, infos);
    }

    TensorPtrs RunMultiInput(const std::vector<void*>& inputPtrs,
                             void* userArg = nullptr, void* outputPtr = nullptr)
    {
        (void)userArg;
        int num = static_cast<int>(inputPtrs.size());
        std::vector<const void*> in_ptrs(inputPtrs.begin(), inputPtrs.end());

        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        std::vector<uint8_t> buf;
        void* out_ptr = outputPtr;
        if (!out_ptr)
        {
            buf.resize(static_cast<size_t>(out_sz));
            out_ptr = buf.data();
        }

        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        detail::check(dxrt_engine_run_multi_input_vector_with_tensor_info(
            h_, in_ptrs.data(), num, out_ptr,
            count > 0 ? infos.data() : nullptr, &count));
        infos.resize(static_cast<size_t>(count));
        return build_output_tensors_from_infos(out_ptr, out_sz, outputPtr == nullptr, infos);
    }

    // ValidateDevice: returns metadata-only tensors (data() == nullptr).
    // Use Run() to get actual output data.
    TensorPtrs ValidateDevice(void* inputPtr, int deviceId = 0)
    {
        detail::check(dxrt_engine_validate_device(h_, inputPtr, deviceId));
        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        return build_output_tensors(nullptr, out_sz);
    }

    TensorPtrs ValidateDevice(const std::vector<void*>& inputPtrs, int deviceId = 0)
    {
        int num = static_cast<int>(inputPtrs.size());
        std::vector<const void*> in_ptrs(inputPtrs.begin(), inputPtrs.end());
        detail::check(dxrt_engine_validate_device_vector(h_, in_ptrs.data(), num, deviceId));
        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        return build_output_tensors(nullptr, out_sz);
    }

    TensorPtrs ValidateDeviceMultiInput(const std::map<std::string, void*>& inputTensors, int deviceId = 0)
    {
        int num = static_cast<int>(inputTensors.size());
        std::vector<const char*> names;
        std::vector<const void*> buffers;
        for (auto& kv : inputTensors)
        {
            names.push_back(kv.first.c_str());
            buffers.push_back(kv.second);
        }
        detail::check(dxrt_engine_validate_device_multi_input(h_, names.data(), buffers.data(), num, deviceId));
        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        return build_output_tensors(nullptr, out_sz);
    }

    TensorPtrs ValidateDeviceMultiInput(const std::vector<void*>& inputPtrs, int deviceId = 0)
    {
        int num = static_cast<int>(inputPtrs.size());
        std::vector<const void*> in_ptrs(inputPtrs.begin(), inputPtrs.end());
        detail::check(dxrt_engine_validate_device_multi_input_vector(h_, in_ptrs.data(), num, deviceId));
        uint64_t out_sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &out_sz));
        return build_output_tensors(nullptr, out_sz);
    }

    /* ── Async Multi-Input ─────────────────────────────────── */

    int RunAsync(const std::vector<void*>& inputPtrs, void* userArg = nullptr, void* outputPtr = nullptr)
    {
        int num = static_cast<int>(inputPtrs.size());
        std::vector<const void*> in_ptrs(inputPtrs.begin(), inputPtrs.end());
        int job_id = -1;
        detail::check(dxrt_engine_run_async_multi_input_vector(h_, in_ptrs.data(), num, userArg, outputPtr, &job_id));
        return job_id;
    }

    int RunAsyncMultiInput(const std::map<std::string, void*>& inputTensors, void* userArg = nullptr, void* outputPtr = nullptr)
    {
        int num = static_cast<int>(inputTensors.size());
        std::vector<const char*> names;
        std::vector<const void*> buffers;
        for (auto& kv : inputTensors)
        {
            names.push_back(kv.first.c_str());
            buffers.push_back(kv.second);
        }
        int job_id = -1;
        detail::check(dxrt_engine_run_async_multi_input(h_, names.data(), buffers.data(), num, userArg, outputPtr, &job_id));
        return job_id;
    }

    int RunAsyncMultiInput(const std::vector<void*>& inputPtrs, void* userArg = nullptr, void* outputPtr = nullptr)
    {
        return RunAsync(inputPtrs, userArg, outputPtr);
    }

    /* ── Release Callback ──────────────────────────────────── */

    void RegisterUserInputReleaseCallback(std::function<void(void* userArg, int jobId)> cb)
    {
        release_cb_ = std::move(cb);
        detail::check(dxrt_engine_register_release_callback(h_, &release_trampoline, this));
    }

    /* ── GetAllTaskOutputs ─────────────────────────────────── */

    std::vector<TensorPtrs> GetAllTaskOutputs()
    {
        int total = 0;
        int num_tasks = 0;
        detail::check(dxrt_engine_get_all_task_outputs(h_, nullptr, &total, nullptr, &num_tasks));
        if (total == 0) return {};

        std::vector<dxrt_tensor_info_t> infos(total);
        std::vector<int> task_counts(num_tasks);
        detail::check(dxrt_engine_get_all_task_outputs(h_, infos.data(), &total, task_counts.data(), &num_tasks));

        std::vector<TensorPtrs> result;
        int idx = 0;
        for (int t = 0; t < num_tasks; ++t)
        {
            TensorPtrs task_tensors;
            for (int i = 0; i < task_counts[t] && idx < total; ++i, ++idx)
            {
                auto& info = infos[idx];
                std::vector<int64_t> shape(info.shape, info.shape + info.ndim);
                task_tensors.push_back(std::make_shared<Tensor>(
                    info.name, info.type, shape, nullptr, info.size_in_bytes));
            }
            result.push_back(std::move(task_tensors));
        }
        return result;
    }

    /* ── Device-specific inputs ────────────────────────────── */

    std::vector<Tensors> GetInputs(int devId)
    {
        int count = 0;
        detail::check(dxrt_engine_get_device_input_tensor_info(h_, devId, nullptr, &count));
        if (count == 0) return {};

        std::vector<dxrt_tensor_info_t> infos(count);
        detail::check(dxrt_engine_get_device_input_tensor_info(h_, devId, infos.data(), &count));

        // Return as single Tensors group (flatten)
        Tensors tensors;
        for (int i = 0; i < count; ++i)
        {
            auto& info = infos[i];
            std::vector<int64_t> shape(info.shape, info.shape + info.ndim);
            tensors.emplace_back(info.name, info.type, shape, nullptr, info.size_in_bytes);
        }
        return {tensors};
    }

    /* ── I/O Metadata ──────────────────────────────────────── */

    uint64_t GetInputSize()
    {
        uint64_t sz = 0;
        detail::check(dxrt_engine_get_input_size(h_, &sz));
        return sz;
    }

    uint64_t GetOutputSize() const
    {
        uint64_t sz = 0;
        detail::check(dxrt_engine_get_output_size(h_, &sz));
        return sz;
    }

    int GetInputTensorCount() const
    {
        int n = 0;
        detail::check(dxrt_engine_get_input_count(h_, &n));
        return n;
    }

    int GetNumTailTasks() const
    {
        int n = 0;
        detail::check(dxrt_engine_get_output_count(h_, &n));
        return n;
    }

    std::vector<std::string> GetInputTensorNames() const
    {
        int count = 0;
        detail::check(dxrt_engine_get_input_tensor_names(h_, &count, nullptr, 0));
        std::vector<std::string> result(static_cast<size_t>(count));
        if (count > 0)
        {
            std::vector<char> storage(static_cast<size_t>(count) * 256);
            std::vector<char*> ptrs(static_cast<size_t>(count));
            for (int i = 0; i < count; ++i)
                ptrs[static_cast<size_t>(i)] = &storage[static_cast<size_t>(i) * 256];
            detail::check(dxrt_engine_get_input_tensor_names(h_, &count, ptrs.data(), 256));
            for (int i = 0; i < count; ++i)
                result[static_cast<size_t>(i)] = ptrs[static_cast<size_t>(i)];
        }
        return result;
    }

    std::vector<std::string> GetOutputTensorNames() const
    {
        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_names(h_, &count, nullptr, 0));
        std::vector<std::string> result(static_cast<size_t>(count));
        if (count > 0)
        {
            std::vector<char> storage(static_cast<size_t>(count) * 256);
            std::vector<char*> ptrs(static_cast<size_t>(count));
            for (int i = 0; i < count; ++i)
                ptrs[static_cast<size_t>(i)] = &storage[static_cast<size_t>(i) * 256];
            detail::check(dxrt_engine_get_output_tensor_names(h_, &count, ptrs.data(), 256));
            for (int i = 0; i < count; ++i)
                result[static_cast<size_t>(i)] = ptrs[static_cast<size_t>(i)];
        }
        return result;
    }

    std::vector<uint64_t> GetInputTensorSizes()
    {
        int count = 0;
        detail::check(dxrt_engine_get_input_tensor_sizes(h_, nullptr, &count));
        std::vector<uint64_t> sizes(static_cast<size_t>(count));
        if (count > 0)
            detail::check(dxrt_engine_get_input_tensor_sizes(h_, sizes.data(), &count));
        return sizes;
    }

    std::vector<uint64_t> GetOutputTensorSizes() const
    {
        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_sizes(h_, nullptr, &count));
        std::vector<uint64_t> sizes(static_cast<size_t>(count));
        if (count > 0)
            detail::check(dxrt_engine_get_output_tensor_sizes(h_, sizes.data(), &count));
        return sizes;
    }

    size_t GetOutputTensorOffset(const std::string& tensorName) const
    {
        size_t offset = 0;
        detail::check(dxrt_engine_get_output_tensor_offset(h_, tensorName.c_str(), &offset));
        return offset;
    }

    bool HasDynamicOutput() const
    {
        int val = 0;
        detail::check(dxrt_engine_has_dynamic_output(h_, &val));
        return val != 0;
    }

    bool IsMultiInputModel() const
    {
        int val = 0;
        detail::check(dxrt_engine_is_multi_input(h_, &val));
        return val != 0;
    }

    /* ── Model Metadata ────────────────────────────────────── */

    std::string GetModelName() const
    {
        char buf[1024];
        detail::check(dxrt_engine_get_model_name(h_, buf, sizeof(buf)));
        return std::string(buf);
    }

    std::string GetModelVersion() const
    {
        char buf[256];
        detail::check(dxrt_engine_get_model_version(h_, buf, sizeof(buf)));
        return std::string(buf);
    }

    std::string GetCompileType() const
    {
        char buf[256];
        detail::check(dxrt_engine_get_compile_type(h_, buf, sizeof(buf)));
        return std::string(buf);
    }

    bool IsOrtConfigured() const
    {
        int val = 0;
        detail::check(dxrt_engine_is_ort_configured(h_, &val));
        return val != 0;
    }

    bool IsPPU() const
    {
        int val = 0;
        detail::check(dxrt_engine_is_ppu(h_, &val));
        return val != 0;
    }

    /* ── Performance Stats ─────────────────────────────────── */

    int GetLatency() const
    {
        int us = 0;
        detail::check(dxrt_engine_get_latency(h_, &us));
        return us;
    }

    int GetLatencyCnt() const
    {
        int cnt = 0;
        detail::check(dxrt_engine_get_latency_count(h_, &cnt));
        return cnt;
    }

    double GetLatencyMean() const
    {
        double us = 0.0;
        detail::check(dxrt_engine_get_latency_mean(h_, &us));
        return us;
    }

    double GetLatencyStdDev() const
    {
        double us = 0.0;
        detail::check(dxrt_engine_get_latency_stddev(h_, &us));
        return us;
    }

    uint32_t GetNpuInferenceTime()
    {
        uint32_t us = 0;
        detail::check(dxrt_engine_get_npu_inference_time(h_, &us));
        return us;
    }

    int GetNpuInferenceTimeCnt() const
    {
        int cnt = 0;
        detail::check(dxrt_engine_get_npu_time_count(h_, &cnt));
        return cnt;
    }

    double GetNpuInferenceTimeMean() const
    {
        double us = 0.0;
        detail::check(dxrt_engine_get_npu_time_mean(h_, &us));
        return us;
    }

    double GetNpuInferenceTimeStdDev() const
    {
        double us = 0.0;
        detail::check(dxrt_engine_get_npu_time_stddev(h_, &us));
        return us;
    }

    /* ── Performance Data Vectors ──────────────────────────── */

    std::vector<int> GetLatencyVector()
    {
        size_t count = 0;
        detail::check(dxrt_engine_get_latency_list(h_, nullptr, &count));
        std::vector<int> result(count);
        if (count > 0)
            detail::check(dxrt_engine_get_latency_list(h_, result.data(), &count));
        return result;
    }

    std::vector<uint32_t> GetNpuInferenceTimeVector()
    {
        size_t count = 0;
        detail::check(dxrt_engine_get_npu_time_list(h_, nullptr, &count));
        std::vector<uint32_t> result(count);
        if (count > 0)
            detail::check(dxrt_engine_get_npu_time_list(h_, result.data(), &count));
        return result;
    }

    std::vector<uint8_t> GetBitmatchMask(int index = 0)
    {
        size_t size = 0;
        detail::check(dxrt_engine_get_bitmatch_mask(h_, index, nullptr, &size));
        std::vector<uint8_t> result(size);
        if (size > 0)
            detail::check(dxrt_engine_get_bitmatch_mask(h_, index, result.data(), &size));
        return result;
    }

    /* ── Task / Mapping Queries ──────────────────────────── */

    std::vector<std::string> GetTaskOrder()
    {
        int count = 0;
        detail::check(dxrt_engine_get_task_order(h_, &count, nullptr, 0));
        if (count == 0) return {};
        std::vector<char> buf(count * 256);
        std::vector<char*> ptrs(count);
        for (int i = 0; i < count; ++i)
            ptrs[i] = buf.data() + i * 256;
        detail::check(dxrt_engine_get_task_order(h_, &count, ptrs.data(), 256));
        std::vector<std::string> result(count);
        for (int i = 0; i < count; ++i)
            result[i] = ptrs[i];
        return result;
    }

    std::map<std::string, std::string> GetInputTensorToTaskMapping()
    {
        int count = 0;
        detail::check(dxrt_engine_get_input_tensor_to_task_mapping(h_, &count,
                      nullptr, nullptr, 0));
        if (count == 0) return {};
        std::vector<char> kbuf(count * 256), vbuf(count * 256);
        std::vector<char*> kptrs(count), vptrs(count);
        for (int i = 0; i < count; ++i) {
            kptrs[i] = kbuf.data() + i * 256;
            vptrs[i] = vbuf.data() + i * 256;
        }
        detail::check(dxrt_engine_get_input_tensor_to_task_mapping(h_, &count,
                      kptrs.data(), vptrs.data(), 256));
        std::map<std::string, std::string> result;
        for (int i = 0; i < count; ++i)
            result[kptrs[i]] = vptrs[i];
        return result;
    }

    /* ── Tensor Metadata ─────────────────────────────────── */

    Tensors GetInputs(void* ptr = nullptr, uint64_t phyAddr = 0)
    {
        int count = 0;
        detail::check(dxrt_engine_get_input_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        if (count > 0)
            detail::check(dxrt_engine_get_input_tensor_info(h_, infos.data(), &count));
        Tensors result;
        uint64_t offset = 0;
        for (auto& info : infos) {
            std::vector<int64_t> shape(info.shape, info.shape + info.ndim);
            void* data = ptr ? static_cast<void*>(static_cast<uint8_t*>(ptr) + offset) : nullptr;
            Tensor t(info.name, info.type, shape, data, info.size_in_bytes);
            if (ptr) t.phy_addr() = phyAddr + offset;
            offset += info.size_in_bytes;
            result.push_back(std::move(t));
        }
        return result;
    }

    Tensors GetOutputs(void* ptr = nullptr, uint64_t phyAddr = 0)
    {
        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        if (count > 0)
            detail::check(dxrt_engine_get_output_tensor_info(h_, infos.data(), &count));
        Tensors result;
        uint64_t offset = 0;
        for (auto& info : infos) {
            std::vector<int64_t> shape(info.shape, info.shape + info.ndim);
            void* data = ptr ? static_cast<void*>(static_cast<uint8_t*>(ptr) + offset) : nullptr;
            Tensor t(info.name, info.type, shape, data, info.size_in_bytes);
            if (ptr) t.phy_addr() = phyAddr + offset;
            offset += info.size_in_bytes;
            result.push_back(std::move(t));
        }
        return result;
    }

    dxrt_engine_t handle() const noexcept { return h_; }

private:
    dxrt_engine_t h_ = nullptr;
    std::function<int(TensorPtrs&, void*)> cb_;
    std::function<void(void*, int)> release_cb_;

    static int callback_trampoline(const dxrt_tensor_slice_t* tensors,
                                   int tensor_count, void* user_arg, void* user_data)
    {
        auto* self = static_cast<InferenceEngine*>(user_data);
        if (self->cb_)
        {
            auto result = self->build_output_tensors_from_slices(tensors, tensor_count);
            return self->cb_(result, user_arg);
        }
        return 0;
    }

    static void release_trampoline(void* user_data, void* user_arg, int job_id)
    {
        auto* self = static_cast<InferenceEngine*>(user_data);
        if (self->release_cb_)
            self->release_cb_(user_arg, job_id);
    }

    TensorPtrs build_output_tensors(const void* data, uint64_t total_sz,
                                    bool own = false) const
    {
        int count = 0;
        detail::check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        if (count > 0)
            detail::check(dxrt_engine_get_output_tensor_info(h_, infos.data(), &count));
        return build_output_tensors_from_infos(data, total_sz, own, infos);
    }

    TensorPtrs build_output_tensors_from_infos(
        const void* data, uint64_t total_sz, bool own,
        const std::vector<dxrt_tensor_info_t>& infos) const
    {
        TensorPtrs result;
        const uint8_t* ptr = static_cast<const uint8_t*>(data);
        for (size_t i = 0; i < infos.size(); ++i)
        {
            std::vector<int64_t> shape(infos[i].shape, infos[i].shape + infos[i].ndim);
            size_t sz = static_cast<size_t>(infos[i].size_in_bytes);
            assert(!ptr || (ptr + sz <= static_cast<const uint8_t*>(data) + total_sz));
            (void)total_sz; // used by assert in debug builds
            if (own)
                result.push_back(std::make_shared<Tensor>(
                    infos[i].name, infos[i].type, std::move(shape), ptr, sz, true));
            else
                result.push_back(std::make_shared<Tensor>(
                    infos[i].name, infos[i].type, std::move(shape), ptr, sz));
            if (ptr) ptr += sz;
        }
        std::ignore = total_sz; // total_sz is for validation only (can check against sum of tensor sizes if desired)
        return result;
    }

    TensorPtrs build_output_tensors_from_slices(const dxrt_tensor_slice_t* slices,
                                                int tensor_count) const
    {
        TensorPtrs result;
        result.reserve(static_cast<size_t>(tensor_count));
        for (int i = 0; i < tensor_count; ++i)
        {
            const auto& info = slices[i].info;
            std::vector<int64_t> shape(info.shape, info.shape + info.ndim);
            const auto* p = static_cast<const uint8_t*>(slices[i].data);
            size_t sz = static_cast<size_t>(slices[i].size_in_bytes);
            // 5-arg ctor is non-owning: wraps `p` without copying or freeing.
            result.push_back(std::make_shared<Tensor>(
                info.name, info.type, std::move(shape), p, sz));
        }
        return result;
    }
};

/* ================================================================
 *  Configuration  (dxrt_config_*)
 * ================================================================ */

class Configuration
{
public:
    enum class ITEM
    {
        DEBUG = 1,
        PROFILER,
        SERVICE,
        DYNAMIC_CPU_THREAD,
        TASK_FLOW,
        SHOW_THROTTLING,
        SHOW_PROFILE,
        SHOW_MODEL_INFO,
        CUSTOM_INTRA_OP_THREADS,
        CUSTOM_INTER_OP_THREADS,
        NFH_ASYNC,
#ifdef DXRT_NFH_ACCELERATION_AVAILABLE
        NFH_ACCELERATION,
#endif
#ifdef DXRT_CPU_OP_ACCELERATION_AVAILABLE
        CPU_OP_ACCELERATION,
#endif
    };

    enum class ATTRIBUTE
    {
        PROFILER_SHOW_DATA          = 1001,
        PROFILER_SAVE_DATA          = 1002,
        CUSTOM_INTRA_OP_THREADS_NUM = 1003,
        CUSTOM_INTER_OP_THREADS_NUM = 1004
    };

    static Configuration& GetInstance()
    {
        static Configuration instance;
        return instance;
    }

    std::string GetVersion() const
    {
        char buf[128];
        if (dxrt_config_get_version(buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string GetDriverVersion() const
    {
        char buf[128];
        if (dxrt_config_get_driver_version(buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string GetPCIeDriverVersion() const
    {
        char buf[128];
        if (dxrt_config_get_pcie_driver_version(buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string GetONNXRuntimeVersion() const
    {
        char buf[128];
        if (dxrt_config_get_ort_version(buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::vector<std::pair<int, std::string>> GetFirmwareVersions() const
    {
        int count = 0;
        dxrt_config_get_firmware_versions(nullptr, nullptr, 0, &count);
        std::vector<std::pair<int, std::string>> result;
        if (count > 0)
        {
            std::vector<int> ids(static_cast<size_t>(count));
            std::vector<char> storage(static_cast<size_t>(count) * 128);
            std::vector<char*> ptrs(static_cast<size_t>(count));
            for (int i = 0; i < count; ++i)
                ptrs[static_cast<size_t>(i)] = &storage[static_cast<size_t>(i) * 128];
            dxrt_config_get_firmware_versions(ids.data(), ptrs.data(), 128, &count);
            for (int i = 0; i < count; ++i)
                result.emplace_back(ids[static_cast<size_t>(i)],
                                    std::string(ptrs[static_cast<size_t>(i)]));
        }
        return result;
    }

    void SetEnable(ITEM item, bool enabled)
    {
        dxrt_config_set_enable(static_cast<dxrt_config_item_t>(item),
                               enabled ? 1 : 0);
    }

    bool GetEnable(ITEM item)
    {
        int enabled = 0;
        dxrt_config_get_enable(static_cast<dxrt_config_item_t>(item), &enabled);
        return enabled != 0;
    }

    void SetAttribute(ITEM item, ATTRIBUTE attr, const std::string& value)
    {
        dxrt_config_set_attribute(static_cast<dxrt_config_item_t>(item),
                                  static_cast<dxrt_config_attr_t>(attr),
                                  value.c_str());
    }

    std::string GetAttribute(ITEM item, ATTRIBUTE attr)
    {
        char buf[1024];
        if (dxrt_config_get_attribute(static_cast<dxrt_config_item_t>(item),
                                      static_cast<dxrt_config_attr_t>(attr),
                                      buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    int GetIntAttribute(ITEM item, ATTRIBUTE attr)
    {
        int val = 0;
        dxrt_config_get_int_attribute(static_cast<dxrt_config_item_t>(item),
                                      static_cast<dxrt_config_attr_t>(attr),
                                      &val);
        return val;
    }

    void LockEnable(ITEM item)
    {
        dxrt_config_lock_enable(static_cast<dxrt_config_item_t>(item));
    }

    void LoadConfigFile(const std::string& fileName)
    {
        dxrt_config_load_file(fileName.c_str());
    }

    void SetFWConfigWithJson(const std::string& json_file)
    {
        dxrt_config_set_fw_config_json(json_file.c_str());
    }

private:
    Configuration() = default;
};

/* ================================================================
 *  DeviceStatus  (dxrt_device_*)
 * ================================================================ */

/* ================================================================
 *  Driver-level types (OTP, LED, custom commands)
 *  Binary-compatible with internal driver.h definitions.
 * ================================================================ */

#pragma pack(push, 1)
typedef struct otp_info {
    uint8_t     JEP_ID;
    uint8_t     CONTINUATION_CODE;
    char        CHIP_NAME[2];
    char        DEVICE_REV[2];
    uint16_t    RESERVED0;
    uint32_t    ECID;
    char        FOUNDRY_FAB[4];
    char        PROCESS[4];
    char        LOT_ID[12];
    char        WAFER_ID[4];
    char        X_AXIS[4];
    char        Y_AXIS[4];
    char        TEST_PGM[4];
    char        BARCODE[16];
    uint32_t    BARCODE_IDX;
} otp_info_t;
#pragma pack(pop)
static_assert(sizeof(otp_info_t) == 68, "otp_info_t binary layout mismatch");

typedef enum {
    DX_SET_DDR_FREQ         = 1,
    DX_GET_OTP              = 2,
    DX_SET_OTP              = 3,
    DX_SET_LED              = 4,
    DX_ADD_WEIGHT_INFO      = 5,
    DX_DEL_WEIGHT_INFO      = 6,
} dxrt_custom_sub_cmt_t;

/* ================================================================
 *  DeviceStatus  (dxrt_device_get_*)
 * ================================================================ */

enum class DeviceType : uint32_t
{
    ACC_TYPE = 0,
    STD_TYPE = 1,
};

class DeviceStatus
{
public:
    static int GetDeviceCount()
    {
        int count = 0;
        dxrt_device_get_count(&count);
        return count;
    }

    static DeviceStatus GetCurrentStatus(int device_id)
    {
        DeviceStatus ds;
        ds.device_id_ = device_id;
        return ds;
    }

    std::string GetInfoString() const
    {
        char buf[4096];
        if (dxrt_device_get_status(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string GetStatusString() const
    {
        char buf[4096];
        if (dxrt_device_get_status(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string DeviceTypeWord() const
    {
        char buf[128];
        if (dxrt_device_get_type_word(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string DeviceVariantStr() const
    {
        char buf[128];
        if (dxrt_device_get_variant(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string BoardTypeStr() const
    {
        char buf[128];
        if (dxrt_device_get_board_type(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string MemoryTypeStr() const
    {
        char buf[128];
        if (dxrt_device_get_memory_type(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string MemorySizeStrBinaryPrefix() const
    {
        char buf[128];
        if (dxrt_device_get_memory_size(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string AllMemoryInfoStr() const
    {
        char buf[512];
        if (dxrt_device_get_all_memory_info(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    int Temperature(int channel) const
    {
        int temp = 0;
        dxrt_device_get_temperature(device_id_, channel, &temp);
        return temp;
    }

    uint32_t NpuClock(int channel) const
    {
        uint32_t clk = 0;
        dxrt_device_get_npu_clock(device_id_, channel, &clk);
        return clk;
    }

    uint32_t Voltage(int channel) const
    {
        uint32_t v = 0;
        dxrt_device_get_voltage(device_id_, channel, &v);
        return v;
    }

    std::string NpuStatusStr(int channel) const
    {
        char buf[128];
        if (dxrt_device_get_npu_status(device_id_, channel, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string DdrStatusStr(int channel) const
    {
        char buf[128];
        if (dxrt_device_get_ddr_status(device_id_, channel, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string DdrBitErrStr() const
    {
        char buf[256];
        if (dxrt_device_get_ddr_bit_err(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    int GetId() const { return device_id_; }

    // Convenience aliases matching legacy DeviceStatus API
    int GetTemperature(int ch) const { return Temperature(ch); }
    uint32_t GetNpuVoltage(int ch) const { return Voltage(ch); }
    uint32_t GetNpuClock(int ch) const { return NpuClock(ch); }
    std::string DriverVersionStr() const
    {
        char buf[128];
        if (dxrt_config_get_driver_version(buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    int64_t MemorySize() const
    {
        int64_t bytes = 0;
        dxrt_device_get_memory_size_bytes(device_id_, &bytes);
        return bytes;
    }

    uint64_t MemoryClock() const
    {
        uint64_t mhz = 0;
        dxrt_device_get_memory_clock(device_id_, &mhz);
        return mhz;
    }

    int MemoryFrequency() const { return static_cast<int>(MemoryClock()); }

    uint64_t DmaChannel() const
    {
        uint64_t count = 0;
        dxrt_device_get_dma_channel_count(device_id_, &count);
        return count;
    }

    DeviceType GetDeviceType() const
    {
        uint32_t t = 0;
        dxrt_device_get_type_enum(device_id_, &t);
        return static_cast<DeviceType>(t);
    }

    std::string DeviceTypeStr() const
    {
        char buf[16];
        if (dxrt_device_get_type_str(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string PcieInfoStr() const
    {
        char buf[256];
        if (dxrt_device_get_pcie_info(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    // Pre-CABI compatibility: accepts same signature but uses device-reported values.
    // In practice, callers always passed devInfo.pcie.* fields which are the same as
    // what the 0-arg version returns internally.
    std::string PcieInfoStr(int spd, int wd, int bus, int dev, int func) const
    {
        (void)spd; (void)wd; (void)bus; (void)dev; (void)func;
        return PcieInfoStr();
    }

    std::string MemorySizeStrWithComma() const
    {
        char buf[128];
        if (dxrt_device_get_memory_size_comma(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    std::string FirmwareVersionStr() const
    {
        char buf[128];
        if (dxrt_device_get_firmware_version(device_id_, buf, sizeof(buf)) == DXRT_OK)
            return std::string(buf);
        return "";
    }

    // Monitoring data (reads from dxrtd shared memory)
    double GetCoreUtilization(int coreId) const
    {
        double util = -1.0;
        dxrt_device_get_core_utilization(device_id_, coreId, &util);
        return util;
    }

    uint64_t GetMemoryUsed() const
    {
        uint64_t bytes = 0;
        dxrt_device_get_memory_used(device_id_, &bytes);
        return bytes;
    }

    uint64_t GetMemoryFree() const
    {
        uint64_t bytes = 0;
        dxrt_device_get_memory_free(device_id_, &bytes);
        return bytes;
    }

    bool IsValid() const
    {
        int active = 0;
        dxrt_device_is_monitoring_active(device_id_, &active);
        return active != 0;
    }

    std::ostream& StatusToStream(std::ostream& os) const
    {
        for (int i = 0; i < 4; i++) {
            std::string s = NpuStatusStr(i);
            if (!s.empty()) os << s << "\n";
        }
        os << "=======================================================" << "\n";
        return os;
    }

    std::ostream& DebugStatusToStream(std::ostream& os) const
    {
        for (int i = 0; i < 4; i++) {
            std::string s = DdrStatusStr(i);
            if (!s.empty()) os << s << "\n";
        }
        os << DdrBitErrStr() << "\n";
        os << "=======================================================" << "\n";
        return os;
    }

    template<typename T>
    static DeviceStatus GetCurrentStatus(const std::shared_ptr<T>& device)
    {
        return GetCurrentStatus(device->id());
    }

    friend std::ostream& operator<<(std::ostream& os, const DeviceStatus& d)
    {
        os << d.GetInfoString() << "\n";
        d.StatusToStream(os);
        return os;
    }

private:
    int device_id_ = 0;
};

/* ================================================================
 *  Device / DeviceCore / DevicePool / CheckDevices
 *  Header-only — all methods call C ABI functions.
 * ================================================================ */

class DeviceCore {
 public:
    explicit DeviceCore(int id) : id_(id) {}

    void DoCustomCommand(void* data, uint32_t subCmd, uint32_t size = 0) {
        dxrt_status_t st = dxrt_device_custom_command(id_, data, subCmd, size);
        if (st != DXRT_OK) {
            throw Exception(std::string("DoCustomCommand failed: ") +
                            dxrt_last_error_message());
        }
    }

    int id() const { return id_; }

 private:
    int id_;
};

class Device {
 public:
    explicit Device(int id) : id_(id) {}

    int id() const { return id_; }

    void DoCustomCommand(void* data, uint32_t subCmd, uint32_t size = 0) const {
        dxrt_status_t st = dxrt_device_custom_command(id_, data, subCmd, size);
        if (st != DXRT_OK) {
            throw Exception(std::string("DoCustomCommand failed: ") +
                            dxrt_last_error_message());
        }
    }

    DeviceStatus GetCurrentStatus() const {
        return DeviceStatus::GetCurrentStatus(id_);
    }

 private:
    int id_;
};

class DevicePool {
 public:
    static DevicePool& GetInstance() {
        static DevicePool instance;
        return instance;
    }

    void InitCores() {
        std::call_once(initFlag_, [this]() {
            dxrt_status_t st = dxrt_device_pool_init();
            if (st != DXRT_OK) {
                throw Exception(std::string("DevicePool init failed: ") +
                                dxrt_last_error_message());
            }
            int count = 0;
            dxrt_device_get_count(&count);
            cores_.clear();
            cores_.reserve(count);
            for (int i = 0; i < count; ++i) {
                cores_.push_back(std::make_shared<DeviceCore>(i));
            }
        });
    }

    size_t GetDeviceCount() {
        InitCores();
        return cores_.size();
    }

    std::shared_ptr<DeviceCore> GetDeviceCores(int deviceId) {
        InitCores();
        return cores_.at(deviceId);
    }

 private:
    DevicePool() = default;
    ~DevicePool() = default;
    DevicePool(const DevicePool&) = delete;
    DevicePool& operator=(const DevicePool&) = delete;

    std::vector<std::shared_ptr<DeviceCore>> cores_;
    std::once_flag initFlag_;
};

[[deprecated("Use DevicePool instead")]]
inline std::vector<std::shared_ptr<Device>>& CheckDevices() {
    static std::vector<std::shared_ptr<Device>> devices;
    static std::once_flag flag;
    std::call_once(flag, []() {
        dxrt_status_t st = dxrt_device_pool_init();
        if (st != DXRT_OK) {
            throw Exception(std::string("CheckDevices init failed: ") +
                            dxrt_last_error_message());
        }
        int count = 0;
        dxrt_device_get_count(&count);
        devices.reserve(count);
        for (int i = 0; i < count; ++i) {
            devices.push_back(std::make_shared<Device>(i));
        }
    });
    return devices;
}

/* ================================================================
 *  Profiler  (V2 — job-based metrics via dxrt_profiler_* / dxrt_job_metrics_*)
 * ================================================================ */

/** Per-device NPU timing breakdown (microseconds). Mirrors dxrt_npu_timing_t / dxrt::NpuDeviceMetrics. */
struct NpuDeviceMetrics
{
    double input_format_us       = 0.0;
    double h2d_us                = 0.0;
    double inference_core_all_us = 0.0;
    double inference_core_0_us   = 0.0;
    double inference_core_1_us   = 0.0;
    double inference_core_2_us   = 0.0;
    double d2h_us                = 0.0;
    double output_format_us      = 0.0;
    double total_us              = 0.0;
    bool   valid                 = false;  ///< true if populated from a real measurement
};

/** Metrics for one task within a job (NPU or CPU). */
struct TaskMetrics
{
    std::string                    task_name;
    std::map<int, NpuDeviceMetrics> devices;      // deviceId → metrics (NPU tasks)
    double                         cpu_task_us = 0.0; // CPU execution time (CPU tasks)
    bool                           valid = false;
};

/** All task metrics for one completed job. */
struct JobMetrics
{
    std::vector<TaskMetrics> tasks;
    bool valid = false;

    TaskMetrics GetTask(const std::string& name) const
    {
        for (const auto& t : tasks)
            if (t.task_name == name) return t;
        return TaskMetrics{};
    }
};

class Profiler
{
public:
    static Profiler& GetInstance()
    {
        static Profiler instance;
        return instance;
    }

    /** Get per-job metrics after engine Wait() returns. */
    JobMetrics GetJobMetrics(int jobId)
    {
        JobMetrics result;
        dxrt_job_metrics_t jm = nullptr;
        if (dxrt_profiler_get_job_metrics(jobId, &jm) != DXRT_OK || !jm)
            return result;

        result.valid = (dxrt_job_metrics_is_valid(jm) != 0);
        int taskCount = dxrt_job_metrics_task_count(jm);
        for (int ti = 0; ti < taskCount; ++ti) {
            TaskMetrics tm;
            const char* name = dxrt_job_metrics_task_name(jm, ti);
            tm.task_name = name ? name : "";
            tm.cpu_task_us = dxrt_job_metrics_task_cpu_us(jm, ti);
            tm.valid = true;

            int devCount = dxrt_job_metrics_task_device_count(jm, ti);
            for (int di = 0; di < devCount; ++di) {
                int devId = dxrt_job_metrics_task_device_id(jm, ti, di);
                NpuDeviceMetrics dm;
                dxrt_npu_timing_t t;
                dxrt_npu_timing_init(&t);
                if (dxrt_job_metrics_device_timing_ex(jm, ti, di, &t) == DXRT_OK) {
                    dm.input_format_us       = t.input_format_us;
                    dm.h2d_us                = t.h2d_us;
                    dm.inference_core_all_us = t.inference_core_all_us;
                    dm.inference_core_0_us   = t.inference_core_0_us;
                    dm.inference_core_1_us   = t.inference_core_1_us;
                    dm.inference_core_2_us   = t.inference_core_2_us;
                    dm.d2h_us                = t.d2h_us;
                    dm.output_format_us      = t.output_format_us;
                    dm.total_us              = t.total_us;
                    dm.valid                 = (t.valid != 0);
                } else {
                    // _ex error here implies a C-ABI invariant violation (devId
                    // came from dxrt_job_metrics_task_device_id). Silent zero-
                    // fill is intentional: GetJobMetrics has no exception path,
                    // and a downstream consumer can detect missing data via
                    // dm.valid == false.
                    dm.valid = false;
                }
                tm.devices[devId] = dm;
            }
            result.tasks.push_back(std::move(tm));
        }
        dxrt_job_metrics_destroy(jm);
        return result;
    }

    void Show()
    {
        dxrt_profiler_show();
    }

    void Save(const std::string& file)
    {
        dxrt_profiler_save(file.c_str());
    }

    void Clear()
    {
        dxrt_profiler_clear();
    }

    /** Start recording a user profiling event.
     *  \param name  Event name (e.g. "preprocess", "postprocess"). */
    void Start(const std::string& name)
    {
        dxrt_profiler_user_start(name.c_str());
    }

    /** End recording a user profiling event.
     *  \param name  Event name matching a previous Start() call. */
    void End(const std::string& name)
    {
        dxrt_profiler_user_end(name.c_str());
    }

    /** Clear all user-recorded profiling events. */
    void UserClear()
    {
        dxrt_profiler_user_clear();
    }

private:
    Profiler() = default;
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;
};

/* ================================================================
 *  RuntimeEventDispatcher  (dxrt_event_*)
 * ================================================================ */

class RuntimeEventDispatcher
{
public:
    enum class LEVEL
    {
        INFO     = 1,
        WARNING  = 2,
        ERROR    = 3,
        CRITICAL = 4
    };

    enum class TYPE
    {
        DEVICE_CORE   = 1000,
        DEVICE_STATUS = 1001,
        DEVICE_IO     = 1002,
        DEVICE_MEMORY = 1003,
        UNKNOWN       = 1004
    };

    enum class CODE
    {
        WRITE_INPUT          = 2000,
        READ_OUTPUT          = 2001,
        MEMORY_OVERFLOW      = 2002,
        MEMORY_ALLOCATION    = 2003,
        DEVICE_EVENT         = 2004,
        RECOVERY_OCCURRED    = 2005,
        TIMEOUT_OCCURRED     = 2006,
        THROTTLING_NOTICE    = 2007,
        THROTTLING_EMERGENCY = 2008,
        UNKNOWN              = 2009
    };

    static RuntimeEventDispatcher& GetInstance()
    {
        static RuntimeEventDispatcher instance;
        return instance;
    }

    void DispatchEvent(LEVEL level, TYPE type, CODE code, const std::string& message)
    {
        dxrt_event_dispatch(
            static_cast<dxrt_event_level_t>(level),
            static_cast<dxrt_event_type_t>(type),
            static_cast<dxrt_event_code_t>(code),
            message.c_str());
    }

    void RegisterEventHandler(
        const std::function<void(LEVEL, TYPE, CODE,
                                 const std::string&,
                                 const std::string&)>& handler)
    {
        handler_ = handler;
        if (handler_)
        {
            dxrt_event_register_handler(&callback_trampoline, this);
        }
    }

    void SetCurrentLevel(LEVEL level)
    {
        dxrt_event_set_level(static_cast<dxrt_event_level_t>(level));
    }

    LEVEL GetCurrentLevel() const
    {
        dxrt_event_level_t level = static_cast<dxrt_event_level_t>(0);
        dxrt_event_get_level(&level);
        return static_cast<LEVEL>(level);
    }

private:
    RuntimeEventDispatcher() = default;
    RuntimeEventDispatcher(const RuntimeEventDispatcher&) = delete;
    RuntimeEventDispatcher& operator=(const RuntimeEventDispatcher&) = delete;

    std::function<void(LEVEL, TYPE, CODE,
                       const std::string&,
                       const std::string&)> handler_;

    static void callback_trampoline(dxrt_event_level_t level,
                                    dxrt_event_type_t type,
                                    dxrt_event_code_t code,
                                    const char* message,
                                    const char* timestamp,
                                    void* user_data)
    {
        auto* self = static_cast<RuntimeEventDispatcher*>(user_data);
        if (self->handler_)
        {
            self->handler_(
                static_cast<LEVEL>(level),
                static_cast<TYPE>(type),
                static_cast<CODE>(code),
                message ? message : "",
                timestamp ? timestamp : "");
        }
    }
};

/* ================================================================
 *  Free functions
 * ================================================================ */

inline std::string VersionString()
{
    return dxrt_version_string();
}

inline bool is_service_running()
{
    return dxrt_is_service_running() != 0;
}

inline int get_task_max_load()
{
    return dxrt_get_task_max_load();
}

inline void set_skip_inference_io(bool enabled)
{
    dxrt_status_t st = dxrt_set_skip_inference_io(enabled ? 1 : 0);
    if (st != DXRT_OK)
        throw Exception(std::string("set_skip_inference_io failed: ") +
                        dxrt_last_error_message(), st);
}

inline bool get_skip_inference_io()
{
    int enabled = 0;
    dxrt_status_t st = dxrt_get_skip_inference_io(&enabled);
    if (st != DXRT_OK)
        throw Exception(std::string("get_skip_inference_io failed: ") +
                        dxrt_last_error_message(), st);
    return enabled != 0;
}

inline int service_main(int argc, char** argv)
{
    return dxrt_service_main(argc, argv);
}

/* ================================================================
 *  ParseModel / ParseOptions
 * ================================================================ */

struct ParseOptions {
    bool verbose = false;
    bool json_extract = false;
    bool no_color = false;       // not yet supported via C ABI
    std::string output_file;     // not yet supported via C ABI
};

inline int ParseModel(const std::string& file)
{
    detail::check(dxrt_parse_model(file.c_str()));
    return 0;
}

inline int ParseModel(const std::string& file, const ParseOptions& options)
{
    detail::check(dxrt_parse_model_with_options(
        file.c_str(), options.verbose ? 1 : 0, options.json_extract ? 1 : 0));
    return 0;
}

/* ================================================================
 *  Structured output types (BBOX, FACE, POSE)
 * ================================================================ */

typedef struct _DeviceBoundingBox {
    float x;
    float y;
    float w;
    float h;
    uint8_t grid_y;
    uint8_t grid_x;
    uint8_t box_idx;
    uint8_t layer_idx;
    float score;
    uint32_t label;
    char padding[4];
} DeviceBoundingBox_t;

typedef struct _DeviceFace {
    float x;
    float y;
    float w;
    float h;
    uint8_t grid_y;
    uint8_t grid_x;
    uint8_t box_idx;
    uint8_t layer_idx;
    float score;
    float kpts[5][2];
} DeviceFace_t;

typedef struct _DevicePose {
    float x;
    float y;
    float w;
    float h;
    uint8_t grid_y;
    uint8_t grid_x;
    uint8_t box_idx;
    uint8_t layer_idx;
    float score;
    uint32_t label;
    float kpts[17][3];
    char padding[24];
} DevicePose_t;

} // namespace dxrt

#endif /* DXRT_CXX_API_H */
