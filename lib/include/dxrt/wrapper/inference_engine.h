/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Wrapper — InferenceEngine (prebuilt delivery).
 *
 * This is a header-only C++ class that wraps the stable C ABI.
 * It provides the same interface as the internal InferenceEngine class
 * so that user code compiles without source changes.
 */

#pragma once

/* ── ODR guard ───────────────────────────────────────────────────
 * The wrapper headers and dxrt_cxx_api.h each declare their own
 * dxrt::Exception (different layouts, different accessors). Mixing
 * them in the same translation unit is silently broken — fail the
 * build loudly instead.
 */
#ifndef DXRT_WRAPPER_HEADERS_INCLUDED
# define DXRT_WRAPPER_HEADERS_INCLUDED
#endif
#ifdef DXRT_CXX_API_H_INCLUDED
# error "dxrt/wrapper/*.h and dxrt_cxx_api.h cannot be included in the same translation unit (two dxrt::Exception classes — ODR violation risk). Use one or the other."
#endif

#include "dxrt/dxrt_c_api.h"
#include "dxrt/common.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <map>
#include <vector>

namespace dxrt {

namespace detail {
static InferenceOption s_default_option;
} // namespace detail

class InferenceEngine
{
public:
    explicit InferenceEngine(const std::string& modelPath,
                             InferenceOption& option = detail::s_default_option)
    {
        dxrt_options_t opts;
        dxrt_options_init(&opts);
        opts.bound_option = static_cast<int>(option.boundOption);
        opts.buffer_count = option.bufferCount;
        opts.use_ort = option.useORT ? 1 : 0;
        if (option.devices.size() > 1)
        {
            check(dxrt_engine_create_with_devices(
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
            check(dxrt_engine_create(modelPath.c_str(), &opts, &h_));
        }
    }

    InferenceEngine(const uint8_t* data, size_t size,
                    InferenceOption& option = detail::s_default_option)
    {
        dxrt_options_t opts;
        dxrt_options_init(&opts);
        opts.bound_option = static_cast<int>(option.boundOption);
        opts.buffer_count = option.bufferCount;
        opts.use_ort = option.useORT ? 1 : 0;
        if (option.devices.size() > 1)
        {
            check(dxrt_engine_create_from_memory_with_devices(
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
            check(dxrt_engine_create_from_memory(data, size, &opts, &h_));
        }
    }

    ~InferenceEngine()
    {
        if (h_)
        {
            dxrt_engine_destroy(h_);
        }
    }

    InferenceEngine(const InferenceEngine&) = delete;
    InferenceEngine& operator=(const InferenceEngine&) = delete;

    TensorPtrs Run(void* input, void* userArg = nullptr, void* output = nullptr)
    {
        (void)userArg;
        uint64_t out_sz = 0;
        dxrt_engine_get_output_size(h_, &out_sz);
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
        check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        check(dxrt_engine_run_with_tensor_info(
            h_, input, out_ptr, count > 0 ? infos.data() : nullptr, &count));
        infos.resize(static_cast<size_t>(count));
        return build_output_tensors_from_infos(out_ptr, out_sz, owns, infos);
    }

    int RunAsync(void* input, void* userArg = nullptr, void* output = nullptr)
    {
        int job_id = -1;
        check(dxrt_engine_run_async(h_, input, userArg, output, &job_id));
        return job_id;
    }

    TensorPtrs Wait(int jobId) const
    {
        uint64_t out_sz = 0;
        dxrt_engine_get_output_size(h_, &out_sz);
        std::vector<uint8_t> buf(static_cast<size_t>(out_sz));
        int count = 0;
        check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        check(dxrt_engine_wait_with_tensor_info(
            h_, jobId, buf.data(), count > 0 ? infos.data() : nullptr, &count));
        infos.resize(static_cast<size_t>(count));
        return build_output_tensors_from_infos(buf.data(), out_sz, true, infos);
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
        check(dxrt_engine_register_callback(h_, &callback_trampoline, this));
    }

    float RunBenchmark(int num, void* inputPtr = nullptr)
    {
        float fps = 0.0f;
        check(dxrt_engine_run_benchmark(h_, num, inputPtr, &fps));
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
        dxrt_engine_get_output_size(h_, &out_sz);

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
        check(dxrt_engine_get_output_tensor_info(h_, nullptr, &output_count));
        int info_count = batch * output_count;
        std::vector<dxrt_tensor_info_t> infos(static_cast<size_t>(info_count));
        check(dxrt_engine_run_batch_with_tensor_info(
            h_, in_ptrs.data(), out_ptrs.data(), batch,
            info_count > 0 ? infos.data() : nullptr, &info_count));
        if (info_count % batch != 0)
            throw Exception("runtime output metadata count is not divisible by batch size",
                            INVALID_ARGUMENT);
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
        dxrt_engine_get_output_size(h_, &out_sz);
        std::vector<uint8_t> buf;
        void* out_ptr = outputPtr;
        if (!out_ptr)
        {
            buf.resize(static_cast<size_t>(out_sz));
            out_ptr = buf.data();
        }

        int count = 0;
        check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        check(dxrt_engine_run_multi_input_with_tensor_info(
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
        dxrt_engine_get_output_size(h_, &out_sz);
        std::vector<uint8_t> buf;
        void* out_ptr = outputPtr;
        if (!out_ptr)
        {
            buf.resize(static_cast<size_t>(out_sz));
            out_ptr = buf.data();
        }

        int count = 0;
        check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        check(dxrt_engine_run_multi_input_vector_with_tensor_info(
            h_, in_ptrs.data(), num, out_ptr,
            count > 0 ? infos.data() : nullptr, &count));
        infos.resize(static_cast<size_t>(count));
        return build_output_tensors_from_infos(out_ptr, out_sz, outputPtr == nullptr, infos);
    }

    TensorPtrs ValidateDevice(void* inputPtr, int deviceId = 0)
    {
        check(dxrt_engine_validate_device(h_, inputPtr, deviceId));
        uint64_t out_sz = 0;
        dxrt_engine_get_output_size(h_, &out_sz);
        return build_output_tensors(nullptr, out_sz);
    }

    TensorPtrs ValidateDevice(const std::vector<void*>& inputPtrs, int deviceId = 0)
    {
        int num = static_cast<int>(inputPtrs.size());
        std::vector<const void*> in_ptrs(inputPtrs.begin(), inputPtrs.end());
        check(dxrt_engine_validate_device_vector(h_, in_ptrs.data(), num, deviceId));
        uint64_t out_sz = 0;
        dxrt_engine_get_output_size(h_, &out_sz);
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
        check(dxrt_engine_validate_device_multi_input(h_, names.data(), buffers.data(), num, deviceId));
        uint64_t out_sz = 0;
        dxrt_engine_get_output_size(h_, &out_sz);
        return build_output_tensors(nullptr, out_sz);
    }

    TensorPtrs ValidateDeviceMultiInput(const std::vector<void*>& inputPtrs, int deviceId = 0)
    {
        int num = static_cast<int>(inputPtrs.size());
        std::vector<const void*> in_ptrs(inputPtrs.begin(), inputPtrs.end());
        check(dxrt_engine_validate_device_multi_input_vector(h_, in_ptrs.data(), num, deviceId));
        uint64_t out_sz = 0;
        dxrt_engine_get_output_size(h_, &out_sz);
        return build_output_tensors(nullptr, out_sz);
    }

    /* ── Async Multi-Input ─────────────────────────────────── */

    int RunAsync(const std::vector<void*>& inputPtrs, void* userArg = nullptr, void* outputPtr = nullptr)
    {
        int num = static_cast<int>(inputPtrs.size());
        std::vector<const void*> in_ptrs(inputPtrs.begin(), inputPtrs.end());
        int job_id = -1;
        check(dxrt_engine_run_async_multi_input_vector(h_, in_ptrs.data(), num, userArg, outputPtr, &job_id));
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
        check(dxrt_engine_run_async_multi_input(h_, names.data(), buffers.data(), num, userArg, outputPtr, &job_id));
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
        check(dxrt_engine_register_release_callback(h_, &release_trampoline, this));
    }

    /* ── GetAllTaskOutputs ─────────────────────────────────── */

    std::vector<TensorPtrs> GetAllTaskOutputs()
    {
        int total = 0;
        int num_tasks = 0;
        check(dxrt_engine_get_all_task_outputs(h_, nullptr, &total, nullptr, &num_tasks));
        if (total == 0) return {};

        std::vector<dxrt_tensor_info_t> infos(total);
        std::vector<int> task_counts(num_tasks);
        check(dxrt_engine_get_all_task_outputs(h_, infos.data(), &total, task_counts.data(), &num_tasks));

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
        check(dxrt_engine_get_device_input_tensor_info(h_, devId, nullptr, &count));
        if (count == 0) return {};

        std::vector<dxrt_tensor_info_t> infos(count);
        check(dxrt_engine_get_device_input_tensor_info(h_, devId, infos.data(), &count));

        Tensors tensors;
        for (int i = 0; i < count; ++i)
        {
            auto& info = infos[i];
            std::vector<int64_t> shape(info.shape, info.shape + info.ndim);
            tensors.emplace_back(info.name, info.type, shape, nullptr, info.size_in_bytes);
        }
        return {tensors};
    }

    uint64_t GetInputSize()
    {
        uint64_t sz = 0;
        check(dxrt_engine_get_input_size(h_, &sz));
        return sz;
    }

    uint64_t GetOutputSize() const
    {
        uint64_t sz = 0;
        check(dxrt_engine_get_output_size(h_, &sz));
        return sz;
    }

    std::string GetModelName() const
    {
        char buf[1024];
        check(dxrt_engine_get_model_name(h_, buf, sizeof(buf)));
        return std::string(buf);
    }

    int GetInputTensorCount() const
    {
        int n = 0;
        check(dxrt_engine_get_input_count(h_, &n));
        return n;
    }

    int GetNumTailTasks() const
    {
        int n = 0;
        check(dxrt_engine_get_output_count(h_, &n));
        return n;
    }

    int GetLatency() const
    {
        int us = 0;
        check(dxrt_engine_get_latency(h_, &us));
        return us;
    }

    std::vector<std::string> GetInputTensorNames() const
    {
        int count = 0;
        check(dxrt_engine_get_input_tensor_names(h_, &count, nullptr, 0));
        std::vector<std::string> result(static_cast<size_t>(count));
        if (count > 0)
        {
            std::vector<char> storage(static_cast<size_t>(count) * 256);
            std::vector<char*> ptrs(static_cast<size_t>(count));
            for (int i = 0; i < count; ++i)
            {
                ptrs[static_cast<size_t>(i)] = &storage[static_cast<size_t>(i) * 256];
            }
            check(dxrt_engine_get_input_tensor_names(h_, &count, ptrs.data(), 256));
            for (int i = 0; i < count; ++i)
            {
                result[static_cast<size_t>(i)] = ptrs[static_cast<size_t>(i)];
            }
        }
        return result;
    }

    std::vector<std::string> GetOutputTensorNames() const
    {
        int count = 0;
        check(dxrt_engine_get_output_tensor_names(h_, &count, nullptr, 0));
        std::vector<std::string> result(static_cast<size_t>(count));
        if (count > 0)
        {
            std::vector<char> storage(static_cast<size_t>(count) * 256);
            std::vector<char*> ptrs(static_cast<size_t>(count));
            for (int i = 0; i < count; ++i)
            {
                ptrs[static_cast<size_t>(i)] = &storage[static_cast<size_t>(i) * 256];
            }
            check(dxrt_engine_get_output_tensor_names(h_, &count, ptrs.data(), 256));
            for (int i = 0; i < count; ++i)
            {
                result[static_cast<size_t>(i)] = ptrs[static_cast<size_t>(i)];
            }
        }
        return result;
    }

    std::vector<uint64_t> GetInputTensorSizes()
    {
        int count = 0;
        check(dxrt_engine_get_input_tensor_sizes(h_, nullptr, &count));
        std::vector<uint64_t> sizes(static_cast<size_t>(count));
        if (count > 0)
        {
            check(dxrt_engine_get_input_tensor_sizes(h_, sizes.data(), &count));
        }
        return sizes;
    }

    /* ── Performance Metrics ───────────────────────────────── */

    uint32_t GetNpuInferenceTime()
    {
        uint32_t us = 0;
        check(dxrt_engine_get_npu_inference_time(h_, &us));
        return us;
    }

    double GetLatencyMean() const
    {
        double us = 0.0;
        check(dxrt_engine_get_latency_mean(h_, &us));
        return us;
    }

    double GetLatencyStdDev() const
    {
        double us = 0.0;
        check(dxrt_engine_get_latency_stddev(h_, &us));
        return us;
    }

    double GetNpuInferenceTimeMean() const
    {
        double us = 0.0;
        check(dxrt_engine_get_npu_time_mean(h_, &us));
        return us;
    }

    double GetNpuInferenceTimeStdDev() const
    {
        double us = 0.0;
        check(dxrt_engine_get_npu_time_stddev(h_, &us));
        return us;
    }

    int GetLatencyCnt() const
    {
        int cnt = 0;
        check(dxrt_engine_get_latency_count(h_, &cnt));
        return cnt;
    }

    int GetNpuInferenceTimeCnt() const
    {
        int cnt = 0;
        check(dxrt_engine_get_npu_time_count(h_, &cnt));
        return cnt;
    }

    std::vector<uint64_t> GetOutputTensorSizes() const
    {
        int count = 0;
        dxrt_engine_get_output_tensor_sizes(h_, nullptr, &count);
        std::vector<uint64_t> sizes(static_cast<size_t>(count));
        if (count > 0)
        {
            dxrt_engine_get_output_tensor_sizes(h_, sizes.data(), &count);
        }
        return sizes;
    }

    size_t GetOutputTensorOffset(const std::string& tensorName) const
    {
        size_t offset = 0;
        check(dxrt_engine_get_output_tensor_offset(h_, tensorName.c_str(), &offset));
        return offset;
    }

    /* ── Model Metadata ────────────────────────────────────── */

    std::string GetCompileType() const
    {
        char buf[256];
        check(dxrt_engine_get_compile_type(h_, buf, sizeof(buf)));
        return std::string(buf);
    }

    std::string GetModelVersion() const
    {
        char buf[256];
        check(dxrt_engine_get_model_version(h_, buf, sizeof(buf)));
        return std::string(buf);
    }

    bool IsPPU() const
    {
        int val = 0;
        check(dxrt_engine_is_ppu(h_, &val));
        return val != 0;
    }

    bool HasDynamicOutput() const
    {
        int val = 0;
        check(dxrt_engine_has_dynamic_output(h_, &val));
        return val != 0;
    }

    bool IsOrtConfigured() const
    {
        int val = 0;
        check(dxrt_engine_is_ort_configured(h_, &val));
        return val != 0;
    }

    bool IsMultiInputModel() const
    {
        int val = 0;
        check(dxrt_engine_is_multi_input(h_, &val));
        return val != 0;
    }

    void Dispose()
    {
        check(dxrt_engine_dispose(h_));
    }

    /* ── Performance Data Vectors ──────────────────────────── */

    std::vector<int> GetLatencyVector()
    {
        size_t count = 0;
        check(dxrt_engine_get_latency_list(h_, nullptr, &count));
        std::vector<int> result(count);
        if (count > 0)
            check(dxrt_engine_get_latency_list(h_, result.data(), &count));
        return result;
    }

    std::vector<uint32_t> GetNpuInferenceTimeVector()
    {
        size_t count = 0;
        check(dxrt_engine_get_npu_time_list(h_, nullptr, &count));
        std::vector<uint32_t> result(count);
        if (count > 0)
            check(dxrt_engine_get_npu_time_list(h_, result.data(), &count));
        return result;
    }

    std::vector<uint8_t> GetBitmatchMask(int index = 0)
    {
        size_t size = 0;
        check(dxrt_engine_get_bitmatch_mask(h_, index, nullptr, &size));
        std::vector<uint8_t> result(size);
        if (size > 0)
            check(dxrt_engine_get_bitmatch_mask(h_, index, result.data(), &size));
        return result;
    }

    /* ── Task / Mapping Queries ──────────────────────────── */

    std::vector<std::string> GetTaskOrder()
    {
        int count = 0;
        check(dxrt_engine_get_task_order(h_, &count, nullptr, 0));
        if (count == 0) return {};
        std::vector<char> buf(count * 256);
        std::vector<char*> ptrs(count);
        for (int i = 0; i < count; ++i)
            ptrs[i] = buf.data() + i * 256;
        check(dxrt_engine_get_task_order(h_, &count, ptrs.data(), 256));
        std::vector<std::string> result(count);
        for (int i = 0; i < count; ++i)
            result[i] = ptrs[i];
        return result;
    }

    std::map<std::string, std::string> GetInputTensorToTaskMapping()
    {
        int count = 0;
        check(dxrt_engine_get_input_tensor_to_task_mapping(h_, &count,
              nullptr, nullptr, 0));
        if (count == 0) return {};
        std::vector<char> kbuf(count * 256), vbuf(count * 256);
        std::vector<char*> kptrs(count), vptrs(count);
        for (int i = 0; i < count; ++i) {
            kptrs[i] = kbuf.data() + i * 256;
            vptrs[i] = vbuf.data() + i * 256;
        }
        check(dxrt_engine_get_input_tensor_to_task_mapping(h_, &count,
              kptrs.data(), vptrs.data(), 256));
        std::map<std::string, std::string> result;
        for (int i = 0; i < count; ++i)
            result[kptrs[i]] = vptrs[i];
        return result;
    }

    /* ── Tensor Metadata ─────────────────────────────────── */

    Tensors GetInputs(void* ptr = nullptr, uint64_t phyAddr = 0)
    {
        (void)ptr; (void)phyAddr;
        int count = 0;
        check(dxrt_engine_get_input_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        if (count > 0)
            check(dxrt_engine_get_input_tensor_info(h_, infos.data(), &count));
        Tensors result;
        for (auto& info : infos) {
            std::vector<int64_t> shape(info.shape, info.shape + info.ndim);
            result.emplace_back(info.name, info.type, shape, nullptr,
                                info.size_in_bytes);
        }
        return result;
    }

    Tensors GetOutputs(void* ptr = nullptr, uint64_t phyAddr = 0)
    {
        (void)ptr; (void)phyAddr;
        int count = 0;
        check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        if (count > 0)
            check(dxrt_engine_get_output_tensor_info(h_, infos.data(), &count));
        Tensors result;
        for (auto& info : infos) {
            std::vector<int64_t> shape(info.shape, info.shape + info.ndim);
            result.emplace_back(info.name, info.type, shape, nullptr,
                                info.size_in_bytes);
        }
        return result;
    }

private:
    dxrt_engine_t h_ = nullptr;
    std::function<int(TensorPtrs&, void*)> cb_;
    std::function<void(void*, int)> release_cb_;

    static void check(dxrt_status_t st)
    {
        if (st != DXRT_OK)
        {
            const char* msg = dxrt_last_error_message();
            ERROR_CODE code = DEFAULT;
            switch (st) {
                case DXRT_ERR_NOT_FOUND:     code = FILE_NOT_FOUND; break;
                case DXRT_ERR_INVALID_ARG:   code = INVALID_ARGUMENT; break;
                case DXRT_ERR_DEVICE:        code = DEVICE_IO; break;
                case DXRT_ERR_IO:            code = FILE_IO; break;
                case DXRT_ERR_INVALID_MODEL: code = INVALID_MODEL; break;
                default:                     code = DEFAULT; break;
            }
            throw Exception(msg ? msg : "unknown dxrt error", code);
        }
    }

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
        check(dxrt_engine_get_output_tensor_info(h_, nullptr, &count));
        std::vector<dxrt_tensor_info_t> infos(count);
        if (count > 0)
            check(dxrt_engine_get_output_tensor_info(h_, infos.data(), &count));
        return build_output_tensors_from_infos(data, total_sz, own, infos);
    }

    TensorPtrs build_output_tensors_from_infos(
        const void* data, uint64_t /* total_sz */, bool own,
        const std::vector<dxrt_tensor_info_t>& infos) const
    {
        TensorPtrs result;
        const uint8_t* ptr = static_cast<const uint8_t*>(data);
        for (size_t i = 0; i < infos.size(); ++i)
        {
            std::vector<int64_t> shape(infos[i].shape, infos[i].shape + infos[i].ndim);
            size_t sz = static_cast<size_t>(infos[i].size_in_bytes);
            if (own)
                result.push_back(std::make_shared<Tensor>(
                    infos[i].name, infos[i].type, std::move(shape), ptr, sz, true));
            else
                result.push_back(std::make_shared<Tensor>(
                    infos[i].name, infos[i].type, std::move(shape), ptr, sz));
            if (ptr) ptr += sz;
        }
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

} // namespace dxrt
