/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Stable C API — Implementation
 *
 * This file wraps the internal C++ InferenceEngine with a stable
 * extern "C" interface.  Every exported function catches all C++
 * exceptions and converts them to error codes + thread-local message.
 */

#include "dxrt/dxrt_c_api.h"
#include "dxrt/inference_engine.h"
#include "dxrt/inference_option.h"
#include "dxrt/configuration.h"
#include "dxrt/device_info_status.h"
#include "dxrt/runtime_event_dispatcher.h"
#include "dxrt/profiler.h"
#include "dxrt/user_event_store.h"
#include "dxrt/service_util.h"
#include "dxrt/common.h"
#include "dxrt/model.h"
#include "dxrt/tensor.h"
#include "dxrt/exception/exception.h"
#include "dxrt/device_pool.h"
#include "device_shm/shared_memory_reader.h"
#include "device_shm/monitor_shared_memory.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <memory>
#include <new>

/* ── Thread-local error message ──────────────────────────────── */

static thread_local std::string g_last_error;

static void set_error(const char* msg)
{
    try {
        g_last_error = msg ? msg : "unknown error";
    } catch (...) {
        /* OOM while storing error — nothing we can do */
    }
}

static void clear_error()
{
    g_last_error.clear();
}

/* ── Internal engine wrapper ─────────────────────────────────── */

struct dxrt_engine_s {
    dxrt::InferenceEngine* engine;
};

static void fill_tensor_info(dxrt_tensor_info_t& info, const dxrt::Tensor& t)
{
    std::memset(&info, 0, sizeof(info));
    const auto& name = t.name();
    std::strncpy(info.name, name.c_str(), sizeof(info.name) - 1);
    info.type = static_cast<int>(t.type());
    info.elem_size = const_cast<dxrt::Tensor&>(t).elem_size();
    const auto& shape = t.shape();
    const int dims_total = static_cast<int>(shape.size());
    info.ndim = (std::min)(dims_total, static_cast<int>(DXRT_MAX_TENSOR_DIMS));
    for (int d = 0; d < info.ndim; ++d)
        info.shape[d] = shape[d];
    info.size_in_bytes = t.size_in_bytes();
}

static dxrt_status_t fill_output_infos(const dxrt::TensorPtrs& tensors,
                                       dxrt_tensor_info_t* infos,
                                       int* count)
{
    if (!count) {
        set_error("count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    const int actual = static_cast<int>(tensors.size());
    if (!infos && actual > 0) {
        *count = actual;
        set_error("infos must not be NULL when output tensors are present");
        return DXRT_ERR_INVALID_ARG;
    }
    if (*count < actual) {
        *count = actual;
        set_error("tensor info buffer capacity is too small");
        return DXRT_ERR_INVALID_ARG;
    }
    *count = actual;
    for (int i = 0; i < actual; ++i) {
        if (tensors[static_cast<size_t>(i)])
            fill_tensor_info(infos[i], *tensors[static_cast<size_t>(i)]);
        else
            std::memset(&infos[i], 0, sizeof(infos[i]));
    }
    return DXRT_OK;
}

static dxrt_status_t fill_batch_output_infos(
    const std::vector<dxrt::TensorPtrs>& batch_outputs,
    dxrt_tensor_info_t* infos,
    int* count)
{
    if (!count) {
        set_error("count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    int actual = 0;
    for (const auto& tensors : batch_outputs)
        actual += static_cast<int>(tensors.size());

    if (!infos && actual > 0) {
        *count = actual;
        set_error("infos must not be NULL when output tensors are present");
        return DXRT_ERR_INVALID_ARG;
    }
    if (*count < actual) {
        *count = actual;
        set_error("tensor info buffer capacity is too small");
        return DXRT_ERR_INVALID_ARG;
    }

    *count = actual;
    int idx = 0;
    for (const auto& tensors : batch_outputs) {
        for (const auto& tensor : tensors) {
            if (tensor)
                fill_tensor_info(infos[idx], *tensor);
            else
                std::memset(&infos[idx], 0, sizeof(infos[idx]));
            ++idx;
        }
    }
    return DXRT_OK;
}

static void copy_outputs_to_buffer(const dxrt::TensorPtrs& tensors, void* output)
{
    if (!output || tensors.empty()) return;
    uint8_t* dst = static_cast<uint8_t*>(output);
    for (const auto& tensor : tensors) {
        if (tensor && tensor->data()) {
            auto bytes = tensor->size_in_bytes();
            std::memcpy(dst, tensor->data(), bytes);
            dst += bytes;
        }
    }
}

/* ── Helper: map InferenceOption from C struct ───────────────── */

static dxrt::InferenceOption make_option(const dxrt_options_t* opts)
{
    dxrt::InferenceOption option;
    if (!opts) return option;

    /* Validate struct_size for forward compatibility.
       If the caller was built with an older (smaller) struct, only read
       the fields that fit within the declared size. */
    const uint32_t sz = opts->struct_size;
    if (sz == 0) return option; /* uninitialised struct — use defaults */

#define FIELD_ACCESSIBLE(field) \
    (sz >= offsetof(dxrt_options_t, field) + sizeof(opts->field))

    if (FIELD_ACCESSIBLE(buffer_count) && opts->buffer_count > 0)
        option.bufferCount = opts->buffer_count;

    if (FIELD_ACCESSIBLE(device_id) && opts->device_id >= 0)
        option.devices = {opts->device_id};

    if (FIELD_ACCESSIBLE(bound_option))
        option.boundOption = opts->bound_option;

    if (FIELD_ACCESSIBLE(use_ort)) {
        if (opts->use_ort == 0)
            option.useORT = false;
        else if (opts->use_ort == 1)
            option.useORT = true;
        /* -1 = keep default */
    }

#undef FIELD_ACCESSIBLE

    return option;
}

/* ── Catch-all macro ─────────────────────────────────────────── */

#define DXRT_C_TRY      clear_error(); try {
#define DXRT_C_CATCH     } catch (const dxrt::FileNotFoundException& e) {     \
                             set_error(e.what());                            \
                             return DXRT_ERR_NOT_FOUND;                      \
                         } catch (const dxrt::NullPointerException& e) {     \
                             set_error(e.what());                            \
                             return DXRT_ERR_INVALID_ARG;                    \
                         } catch (const dxrt::InvalidArgumentException& e) { \
                             set_error(e.what());                            \
                             return DXRT_ERR_INVALID_ARG;                    \
                         } catch (const dxrt::InvalidModelException& e) {    \
                             set_error(e.what());                            \
                             return DXRT_ERR_INVALID_MODEL;                  \
                         } catch (const dxrt::ModelParsingException& e) {    \
                             set_error(e.what());                            \
                             return DXRT_ERR_INVALID_MODEL;                  \
                         } catch (const dxrt::FileIOException& e) {          \
                             set_error(e.what());                            \
                             return DXRT_ERR_IO;                             \
                         } catch (const dxrt::ServiceIOException& e) {       \
                             set_error(e.what());                            \
                             return DXRT_ERR_IO;                             \
                         } catch (const dxrt::DeviceIOException& e) {        \
                             set_error(e.what());                            \
                             return DXRT_ERR_DEVICE;                         \
                         } catch (const dxrt::InvalidOperationException& e) {\
                             set_error(e.what());                            \
                             return DXRT_ERR_INTERNAL;                       \
                         } catch (const dxrt::Exception& e) {                \
                             set_error(e.what());                            \
                             return DXRT_ERR_INTERNAL;                       \
                         } catch (const std::bad_alloc&) {                   \
                             set_error("out of memory");                     \
                             return DXRT_ERR_OUT_OF_MEMORY;                  \
                         } catch (const std::invalid_argument& e) {          \
                             set_error(e.what());                            \
                             return DXRT_ERR_INVALID_ARG;                    \
                         } catch (const std::exception& e) {                 \
                             set_error(e.what());                            \
                             return DXRT_ERR_INTERNAL;                       \
                         } catch (...) {                                     \
                             set_error("unknown C++ exception");             \
                             return DXRT_ERR_INTERNAL;                       \
                         }

/* ══════════════════════════════════════════════════════════════
 *  Version
 * ══════════════════════════════════════════════════════════════ */

#define DXRT_STRINGIFY_IMPL(x) #x
#define DXRT_STRINGIFY(x) DXRT_STRINGIFY_IMPL(x)

DXRT_CAPI const char* dxrt_version_string(void)
{
    static const char ver[] =
        DXRT_STRINGIFY(DXRT_VERSION_MAJOR) "."
        DXRT_STRINGIFY(DXRT_VERSION_MINOR) "."
        DXRT_STRINGIFY(DXRT_VERSION_PATCH);
    return ver;
}

/* ══════════════════════════════════════════════════════════════
 *  Error Reporting
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI const char* dxrt_last_error_message(void)
{
    return g_last_error.c_str();
}

/* ══════════════════════════════════════════════════════════════
 *  Engine Lifecycle
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_create(
    const char*            model_path,
    const dxrt_options_t*  opts,
    dxrt_engine_t*         out)
{
    if (!model_path || !out) {
        set_error("model_path and out must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    *out = NULL;

    DXRT_C_TRY

    auto option = make_option(opts);
    std::unique_ptr<dxrt_engine_s> wrapper(new dxrt_engine_s());
    wrapper->engine = new dxrt::InferenceEngine(
        std::string(model_path), option);
    *out = wrapper.release();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_create_from_memory(
    const uint8_t*         model_data,
    size_t                 model_size,
    const dxrt_options_t*  opts,
    dxrt_engine_t*         out)
{
    if (!model_data || model_size == 0 || !out) {
        set_error("model_data, model_size, and out must be valid");
        return DXRT_ERR_INVALID_ARG;
    }
    *out = NULL;

    DXRT_C_TRY

    auto option = make_option(opts);
    std::unique_ptr<dxrt_engine_s> wrapper(new dxrt_engine_s());
    wrapper->engine = new dxrt::InferenceEngine(
        model_data, model_size, option);
    *out = wrapper.release();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_create_with_devices(
    const char*            model_path,
    const dxrt_options_t*  opts,
    const int*             device_ids,
    int                    device_count,
    dxrt_engine_t*         out)
{
    if (!model_path || !out) {
        set_error("model_path and out must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    if (!device_ids || device_count <= 0) {
        set_error("device_ids must be non-NULL and device_count must be >= 1");
        return DXRT_ERR_INVALID_ARG;
    }
    *out = NULL;

    DXRT_C_TRY

    auto option = make_option(opts);
    option.devices.assign(device_ids, device_ids + device_count);
    std::unique_ptr<dxrt_engine_s> wrapper(new dxrt_engine_s());
    wrapper->engine = new dxrt::InferenceEngine(
        std::string(model_path), option);
    *out = wrapper.release();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_create_from_memory_with_devices(
    const uint8_t*         model_data,
    size_t                 model_size,
    const dxrt_options_t*  opts,
    const int*             device_ids,
    int                    device_count,
    dxrt_engine_t*         out)
{
    if (!model_data || model_size == 0 || !out) {
        set_error("model_data, model_size, and out must be valid");
        return DXRT_ERR_INVALID_ARG;
    }
    if (!device_ids || device_count <= 0) {
        set_error("device_ids must be non-NULL and device_count must be >= 1");
        return DXRT_ERR_INVALID_ARG;
    }
    *out = NULL;

    DXRT_C_TRY

    auto option = make_option(opts);
    option.devices.assign(device_ids, device_ids + device_count);
    std::unique_ptr<dxrt_engine_s> wrapper(new dxrt_engine_s());
    wrapper->engine = new dxrt::InferenceEngine(
        model_data, model_size, option);
    *out = wrapper.release();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI void dxrt_engine_destroy(dxrt_engine_t engine)
{
    if (!engine) return;
    try {
        delete engine->engine;
        delete engine;
    } catch (...) {
        /* swallow — destructor should not throw */
    }
}

/* ══════════════════════════════════════════════════════════════
 *  Synchronous Inference
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_run(
    dxrt_engine_t  engine,
    const void*    input,
    void*          output)
{
    if (!engine || !input || !output) {
        set_error("engine, input, and output must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto results = engine->engine->Run(
        const_cast<void*>(input),
        /*userArg=*/nullptr,
        output);
    /* Output buffer is filled by the engine directly. */
    (void)results;
    return DXRT_OK;

    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Asynchronous Inference
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_run_async(
    dxrt_engine_t  engine,
    const void*    input,
    void*          user_arg,
    void*          output,
    int*           job_id)
{
    if (!engine || !input || !job_id) {
        set_error("engine, input, and job_id must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    int id = engine->engine->RunAsync(
        const_cast<void*>(input), user_arg, output);
    *job_id = id;
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_wait(
    dxrt_engine_t  engine,
    int            job_id,
    void*          output)
{
    if (!engine) {
        set_error("engine must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto results = engine->engine->Wait(job_id);

    /* v1: Wait blocks until async job completes.
       Copy all output tensors contiguously into the user buffer. */
    copy_outputs_to_buffer(results, output);
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_run_with_tensor_info(
    dxrt_engine_t engine,
    const void* input,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count)
{
    if (!engine || !input || !output || !count) {
        set_error("engine, input, output, and count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto results = engine->engine->Run(
        const_cast<void*>(input),
        /*userArg=*/nullptr,
        output);
    return fill_output_infos(results, infos, count);

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_wait_with_tensor_info(
    dxrt_engine_t engine,
    int job_id,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count)
{
    if (!engine || !count) {
        set_error("engine and count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto results = engine->engine->Wait(job_id);
    copy_outputs_to_buffer(results, output);
    return fill_output_infos(results, infos, count);

    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Metadata Queries
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_get_input_size(
    dxrt_engine_t  engine,
    uint64_t*      size)
{
    if (!engine || !size) {
        set_error("engine and size must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    *size = engine->engine->GetInputSize();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_output_size(
    dxrt_engine_t  engine,
    uint64_t*      size)
{
    if (!engine || !size) {
        set_error("engine and size must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    *size = engine->engine->GetOutputSize();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_output_tensor_offset(
    dxrt_engine_t  engine,
    const char*    tensor_name,
    size_t*        offset)
{
    if (!engine || !tensor_name || !offset) {
        set_error("engine, tensor_name and offset must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    *offset = engine->engine->GetOutputTensorOffset(std::string(tensor_name));
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_input_count(
    dxrt_engine_t  engine,
    int*           count)
{
    if (!engine || !count) {
        set_error("engine and count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    *count = engine->engine->GetInputTensorCount();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_output_count(
    dxrt_engine_t  engine,
    int*           count)
{
    if (!engine || !count) {
        set_error("engine and count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    *count = engine->engine->GetNumTailTasks();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_model_name(
    dxrt_engine_t  engine,
    char*          buf,
    size_t         buf_len)
{
    if (!engine || !buf || buf_len == 0) {
        set_error("engine, buf, and buf_len must be valid");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    std::string name = engine->engine->GetModelName();
    if (name.size() + 1 > buf_len) {
        set_error("buffer too small for model name");
        return DXRT_ERR_INVALID_ARG;
    }
    std::memcpy(buf, name.c_str(), name.size() + 1);
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_latency(
    dxrt_engine_t  engine,
    int*           latency_us)
{
    if (!engine || !latency_us) {
        set_error("engine and latency_us must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    *latency_us = engine->engine->GetLatency();
    return DXRT_OK;

    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Tensor Name Queries
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_get_input_tensor_names(
    dxrt_engine_t engine, int* count,
    char** names, size_t buf_len)
{
    if (!engine || !count) {
        set_error("engine and count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto vec = engine->engine->GetInputTensorNames();
    int actual = static_cast<int>(vec.size());
    // K3: *count is [in/out]. On input it carries caller capacity (number of
    // char* slots in `names`). Without honoring it we would write past the
    // caller-provided array when the model has more tensors than expected.
    int capacity = *count;
    *count = actual;
    if (names) {
        if (capacity < actual) {
            set_error("names array too small for tensor count "
                      "(*count is capacity on input, actual on output)");
            return DXRT_ERR_INVALID_ARG;
        }
        for (int i = 0; i < actual; ++i) {
            if (vec[i].size() + 1 > buf_len) {
                set_error("name buffer too small");
                return DXRT_ERR_INVALID_ARG;
            }
            std::memcpy(names[i], vec[i].c_str(), vec[i].size() + 1);
        }
    }
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_output_tensor_names(
    dxrt_engine_t engine, int* count,
    char** names, size_t buf_len)
{
    if (!engine || !count) {
        set_error("engine and count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto vec = engine->engine->GetOutputTensorNames();
    int actual = static_cast<int>(vec.size());
    // K3: see comment in dxrt_engine_get_input_tensor_names.
    int capacity = *count;
    *count = actual;
    if (names) {
        if (capacity < actual) {
            set_error("names array too small for tensor count "
                      "(*count is capacity on input, actual on output)");
            return DXRT_ERR_INVALID_ARG;
        }
        for (int i = 0; i < actual; ++i) {
            if (vec[i].size() + 1 > buf_len) {
                set_error("name buffer too small");
                return DXRT_ERR_INVALID_ARG;
            }
            std::memcpy(names[i], vec[i].c_str(), vec[i].size() + 1);
        }
    }
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_input_tensor_sizes(
    dxrt_engine_t engine, uint64_t* sizes, int* count)
{
    if (!engine || !count) {
        set_error("engine and count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto vec = engine->engine->GetInputTensorSizes();
    int actual = static_cast<int>(vec.size());
    if (sizes) {
        int n = (*count < actual) ? *count : actual;
        for (int i = 0; i < n; ++i)
            sizes[i] = vec[i];
    }
    *count = actual;
    return DXRT_OK;

    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Benchmark
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_run_benchmark(
    dxrt_engine_t engine, int num, void* input_ptr, float* fps)
{
    if (!engine || !fps || num <= 0) {
        set_error("engine and fps must not be NULL, num must be > 0");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    *fps = engine->engine->RunBenchmark(num, input_ptr);
    return DXRT_OK;

    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Callback Registration
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_register_callback(
    dxrt_engine_t engine, dxrt_callback_fn callback, void* user_data)
{
    if (!engine || !callback) {
        set_error("engine and callback must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto fn_ptr = callback;
    auto ud = user_data;
    engine->engine->RegisterCallback(
        [fn_ptr, ud](dxrt::TensorPtrs& outputs, void* userArg) -> int {
            // I-n + C3: wrap the entire lambda body in try/catch so neither
            // (a) std::bad_alloc from the slices vector nor (b) any exception
            // thrown by the user C callback can unwind through the dxrt worker
            // thread and call std::terminate. The C ABI contract requires that
            // callbacks never propagate exceptions across the boundary.
            try {
                // Zero-copy: build slice array referencing engine-owned buffers.
                // No tensor-data memcpy: slices carry pointer + size + metadata.
                std::vector<dxrt_tensor_slice_t> slices(outputs.size());
                for (size_t i = 0; i < outputs.size(); ++i) {
                    const auto& t = outputs[i];
                    dxrt_tensor_slice_init(&slices[i]);
                    slices[i].data          = (t && t->data()) ? t->data() : nullptr;
                    slices[i].size_in_bytes = (t && t->data()) ? t->size_in_bytes() : 0;
                    if (t) {
                        fill_tensor_info(slices[i].info, *t);
                        // Keep info.size_in_bytes in lock-step with the slice's
                        // reported size: consumers use it to size the tensor, so
                        // a null-data tensor must report 0 in both views.
                        slices[i].info.size_in_bytes = slices[i].size_in_bytes;
                    }
                }
                return fn_ptr(slices.data(),
                              static_cast<int>(outputs.size()),
                              userArg, ud);
            } catch (const std::exception& e) {
                LOG_DXRT_ERR(std::string("dxrt C-ABI callback threw std::exception: ")
                             + e.what() + " (callback aborted, job continues)");
                return -1;
            } catch (...) {
                LOG_DXRT_ERR("dxrt C-ABI callback threw unknown exception "
                             "(callback aborted, job continues)");
                return -1;
            }
        });
    return DXRT_OK;

    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Configuration
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_config_get_version(char* buf, size_t buf_len)
{
    if (!buf || buf_len == 0) {
        set_error("buf and buf_len must be valid");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto& config = dxrt::Configuration::GetInstance();
    std::string ver = config.GetVersion();
    if (ver.size() + 1 > buf_len) {
        set_error("buffer too small for version string");
        return DXRT_ERR_INVALID_ARG;
    }
    std::memcpy(buf, ver.c_str(), ver.size() + 1);
    return DXRT_OK;

    DXRT_C_CATCH
}

/* Helper: copy config string into caller buffer */
static dxrt_status_t config_string_to_buf(
    const std::string& str, char* buf, size_t buf_len)
{
    if (str.size() + 1 > buf_len) {
        set_error("buffer too small");
        return DXRT_ERR_INVALID_ARG;
    }
    std::memcpy(buf, str.c_str(), str.size() + 1);
    return DXRT_OK;
}

DXRT_CAPI dxrt_status_t dxrt_config_get_driver_version(char* buf, size_t buf_len)
{
    if (!buf || buf_len == 0) {
        set_error("buf and buf_len must be valid");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    return config_string_to_buf(
        dxrt::Configuration::GetInstance().GetDriverVersion(), buf, buf_len);
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_get_pcie_driver_version(char* buf, size_t buf_len)
{
    if (!buf || buf_len == 0) {
        set_error("buf and buf_len must be valid");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    return config_string_to_buf(
        dxrt::Configuration::GetInstance().GetPCIeDriverVersion(), buf, buf_len);
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_get_ort_version(char* buf, size_t buf_len)
{
    if (!buf || buf_len == 0) {
        set_error("buf and buf_len must be valid");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    return config_string_to_buf(
        dxrt::Configuration::GetInstance().GetONNXRuntimeVersion(), buf, buf_len);
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_get_firmware_versions(
    int* device_ids, char** versions, size_t ver_buf_len, int* count)
{
    if (!count) {
        set_error("count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto fws = dxrt::Configuration::GetInstance().GetFirmwareVersions();
    int actual = static_cast<int>(fws.size());
    if (device_ids && versions) {
        int n = (*count < actual) ? *count : actual;
        for (int i = 0; i < n; ++i) {
            device_ids[i] = fws[i].first;
            if (fws[i].second.size() + 1 > ver_buf_len) {
                set_error("version buffer too small");
                return DXRT_ERR_INVALID_ARG;
            }
            std::memcpy(versions[i], fws[i].second.c_str(),
                        fws[i].second.size() + 1);
        }
    }
    *count = actual;
    return DXRT_OK;

    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Device Status
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_device_get_count(int* count)
{
    if (!count) {
        set_error("count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    *count = dxrt::DeviceStatus::GetDeviceCount();
    return DXRT_OK;

    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_status(
    int device_id, char* buf, size_t buf_len)
{
    if (!buf || buf_len == 0) {
        set_error("buf and buf_len must be valid");
        return DXRT_ERR_INVALID_ARG;
    }

    DXRT_C_TRY

    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    std::string info = status.GetInfoString() + "\n" + status.GetStatusString();
    if (info.size() + 1 > buf_len) {
        set_error("buffer too small for status string");
        return DXRT_ERR_INVALID_ARG;
    }
    std::memcpy(buf, info.c_str(), info.size() + 1);
    return DXRT_OK;

    DXRT_C_CATCH
}

/* Helper: copy device status string into caller buffer */
static dxrt_status_t device_string_to_buf(
    const std::string& str, char* buf, size_t buf_len)
{
    if (str.size() + 1 > buf_len) {
        set_error("buffer too small");
        return DXRT_ERR_INVALID_ARG;
    }
    std::memcpy(buf, str.c_str(), str.size() + 1);
    return DXRT_OK;
}

#define DXRT_DEVICE_STRING_FUNC(func_name, method_call)                       \
DXRT_CAPI dxrt_status_t func_name(                                            \
    int device_id, char* buf, size_t buf_len)                                 \
{                                                                             \
    if (!buf || buf_len == 0) {                                               \
        set_error("buf and buf_len must be valid");                           \
        return DXRT_ERR_INVALID_ARG;                                          \
    }                                                                         \
    DXRT_C_TRY                                                                \
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);            \
    return device_string_to_buf(status.method_call, buf, buf_len);            \
    DXRT_C_CATCH                                                              \
}

DXRT_DEVICE_STRING_FUNC(dxrt_device_get_type_word,       DeviceTypeWord())
DXRT_DEVICE_STRING_FUNC(dxrt_device_get_variant,         DeviceVariantStr())
DXRT_DEVICE_STRING_FUNC(dxrt_device_get_board_type,      BoardTypeStr())
DXRT_DEVICE_STRING_FUNC(dxrt_device_get_memory_type,     MemoryTypeStr())
DXRT_DEVICE_STRING_FUNC(dxrt_device_get_memory_size,     MemorySizeStrBinaryPrefix())
DXRT_DEVICE_STRING_FUNC(dxrt_device_get_all_memory_info, AllMemoryInfoStr())
DXRT_DEVICE_STRING_FUNC(dxrt_device_get_ddr_bit_err,     DdrBitErrStr())
DXRT_DEVICE_STRING_FUNC(dxrt_device_get_type_str,        DeviceTypeStr())
DXRT_DEVICE_STRING_FUNC(dxrt_device_get_memory_size_comma, MemorySizeStrWithComma())

#undef DXRT_DEVICE_STRING_FUNC

#define DXRT_DEVICE_CH_STRING_FUNC(func_name, method_call)                    \
DXRT_CAPI dxrt_status_t func_name(                                            \
    int device_id, int channel, char* buf, size_t buf_len)                    \
{                                                                             \
    if (!buf || buf_len == 0) {                                               \
        set_error("buf and buf_len must be valid");                           \
        return DXRT_ERR_INVALID_ARG;                                          \
    }                                                                         \
    DXRT_C_TRY                                                                \
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);            \
    return device_string_to_buf(status.method_call, buf, buf_len);            \
    DXRT_C_CATCH                                                              \
}

DXRT_DEVICE_CH_STRING_FUNC(dxrt_device_get_npu_status, NpuStatusStr(channel))
DXRT_DEVICE_CH_STRING_FUNC(dxrt_device_get_ddr_status, DdrStatusStr(channel))

#undef DXRT_DEVICE_CH_STRING_FUNC

DXRT_CAPI dxrt_status_t dxrt_device_get_temperature(
    int device_id, int channel, int* temp_c)
{
    if (!temp_c) {
        set_error("temp_c must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *temp_c = status.Temperature(channel);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_npu_clock(
    int device_id, int channel, uint32_t* clock_mhz)
{
    if (!clock_mhz) {
        set_error("clock_mhz must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *clock_mhz = status.NpuClock(channel);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_voltage(
    int device_id, int channel, uint32_t* voltage_mv)
{
    if (!voltage_mv) {
        set_error("voltage_mv must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *voltage_mv = status.Voltage(channel);
    return DXRT_OK;
    DXRT_C_CATCH
}

// ── Device Info — Numeric accessors ──────────────────────────────

DXRT_CAPI dxrt_status_t dxrt_device_get_memory_size_bytes(
    int device_id, int64_t* bytes)
{
    if (!bytes) {
        set_error("bytes must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *bytes = status.MemorySize();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_memory_clock(
    int device_id, uint64_t* clock_mhz)
{
    if (!clock_mhz) {
        set_error("clock_mhz must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *clock_mhz = status.MemoryClock();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_dma_channel_count(
    int device_id, uint64_t* count)
{
    if (!count) {
        set_error("count must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *count = status.DmaChannel();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_type_enum(
    int device_id, uint32_t* device_type)
{
    if (!device_type) {
        set_error("device_type must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *device_type = static_cast<uint32_t>(status.GetDeviceType());
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_pcie_info(
    int device_id, char* buf, size_t buf_len)
{
    if (!buf || buf_len == 0) {
        set_error("buf and buf_len must be valid");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    auto devInfo = status.getDevInfo();
    auto& pcie = devInfo.pcie;
    std::string s = status.PcieInfoStr(pcie.speed, pcie.width, pcie.bus, pcie.dev, pcie.func);
    return device_string_to_buf(s, buf, buf_len);
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_firmware_version(
    int device_id, char* buf, size_t buf_len)
{
    if (!buf || buf_len == 0) {
        set_error("buf and buf_len must be valid");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    // Get all firmware versions and find the one matching device_id
    int count = 0;
    dxrt_config_get_firmware_versions(nullptr, nullptr, 0, &count);
    if (count <= 0) {
        return device_string_to_buf("", buf, buf_len);
    }
    std::vector<int> ids(count);
    std::vector<char*> ptrs(count);
    std::vector<std::array<char, 128>> storage(count);
    for (int i = 0; i < count; ++i)
        ptrs[i] = storage[i].data();
    int allocated = count;
    dxrt_config_get_firmware_versions(ids.data(), ptrs.data(), 128, &count);
    int search_count = (count < allocated) ? count : allocated;
    for (int i = 0; i < search_count; ++i) {
        if (ids[i] == device_id) {
            return device_string_to_buf(std::string(ptrs[i]), buf, buf_len);
        }
    }
    return device_string_to_buf("", buf, buf_len);
    DXRT_C_CATCH
}

/* ── Device Pool & Custom Commands ─────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_device_pool_init(void)
{
    DXRT_C_TRY
    dxrt::DevicePool::GetInstance().InitCores();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_custom_command(
    int device_id, void* data, uint32_t sub_cmd, uint32_t size)
{
    DXRT_C_TRY
    if (!data && size > 0) {
        set_error("data must not be null when size > 0");
        return DXRT_ERR_INVALID_ARG;
    }
    auto& pool = dxrt::DevicePool::GetInstance();
    pool.InitCores();
    int count = static_cast<int>(pool.GetDeviceCount());
    if (device_id < 0 || device_id >= count) {
        set_error("device_id out of range");
        return DXRT_ERR_INVALID_ARG;
    }
    auto device = pool.GetDeviceCores(device_id);
    device->DoCustomCommand(data, sub_cmd, size);
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  RuntimeEventDispatcher
 * ══════════════════════════════════════════════════════════════ */

static std::atomic<dxrt_event_handler_t> g_event_handler{nullptr};
static std::atomic<void*> g_event_user_data{nullptr};

DXRT_CAPI dxrt_status_t dxrt_event_dispatch(dxrt_event_level_t level,
                                            dxrt_event_type_t type,
                                            dxrt_event_code_t code,
                                            const char* message)
{
    if (!message) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto& dispatcher = dxrt::RuntimeEventDispatcher::GetInstance();
    dispatcher.DispatchEvent(
        static_cast<dxrt::RuntimeEventDispatcher::LEVEL>(level),
        static_cast<dxrt::RuntimeEventDispatcher::TYPE>(type),
        static_cast<dxrt::RuntimeEventDispatcher::CODE>(code),
        message);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_event_register_handler(dxrt_event_handler_t handler,
                                                     void* user_data)
{
    DXRT_C_TRY
    g_event_handler.store(handler, std::memory_order_release);
    g_event_user_data.store(user_data, std::memory_order_release);
    auto& dispatcher = dxrt::RuntimeEventDispatcher::GetInstance();
    if (handler)
    {
        std::function<void(dxrt::RuntimeEventDispatcher::LEVEL,
                           dxrt::RuntimeEventDispatcher::TYPE,
                           dxrt::RuntimeEventDispatcher::CODE,
                           const std::string&,
                           const std::string&)> wrapper =
            [](dxrt::RuntimeEventDispatcher::LEVEL l,
               dxrt::RuntimeEventDispatcher::TYPE t,
               dxrt::RuntimeEventDispatcher::CODE c,
               const std::string& msg,
               const std::string& ts)
            {
                auto h = g_event_handler.load(std::memory_order_acquire);
                if (h)
                {
                    h(static_cast<dxrt_event_level_t>(l),
                      static_cast<dxrt_event_type_t>(t),
                      static_cast<dxrt_event_code_t>(c),
                      msg.c_str(), ts.c_str(),
                      g_event_user_data.load(std::memory_order_acquire));
                }
            };
        dispatcher.RegisterEventHandler(wrapper);
    }
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_event_set_level(dxrt_event_level_t level)
{
    DXRT_C_TRY
    auto& dispatcher = dxrt::RuntimeEventDispatcher::GetInstance();
    dispatcher.SetCurrentLevel(
        static_cast<dxrt::RuntimeEventDispatcher::LEVEL>(level));
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_event_get_level(dxrt_event_level_t* out_level)
{
    if (!out_level) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto& dispatcher = dxrt::RuntimeEventDispatcher::GetInstance();
    *out_level = static_cast<dxrt_event_level_t>(dispatcher.GetCurrentLevel());
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Profiler  (V2 — job-based metrics)
 * ══════════════════════════════════════════════════════════════ */

// Opaque handle storing a copy of JobMetrics
struct dxrt_job_metrics_s {
    dxrt::JobMetrics data;
    // Cache task names as C strings for lifetime safety
    std::vector<std::string> task_names;
    // Cache sorted device IDs per task for indexed access
    std::vector<std::vector<int>> device_ids;
};

DXRT_CAPI dxrt_status_t dxrt_profiler_get_job_metrics(int job_id, dxrt_job_metrics_t* out)
{
    if (!out) return DXRT_ERR_INVALID_ARG;
    *out = nullptr;
    DXRT_C_TRY
    auto jm = std::make_unique<dxrt_job_metrics_s>();
    jm->data = dxrt::Profiler::GetInstance().GetJobMetrics(job_id);
    // Pre-cache task names and device IDs for stable C access (map order = ascending key)
    for (const auto& task : jm->data.tasks) {
        jm->task_names.push_back(task.task_name);
        std::vector<int> devIds;
        for (const auto& pair : task.devices)
            devIds.push_back(pair.first);
        jm->device_ids.push_back(std::move(devIds));
    }
    *out = jm.release();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI void dxrt_job_metrics_destroy(dxrt_job_metrics_t jm)
{
    delete jm;
}

DXRT_CAPI int dxrt_job_metrics_is_valid(dxrt_job_metrics_t jm)
{
    return (jm && jm->data.valid) ? 1 : 0;
}

DXRT_CAPI int dxrt_job_metrics_task_count(dxrt_job_metrics_t jm)
{
    return jm ? static_cast<int>(jm->data.tasks.size()) : 0;
}

DXRT_CAPI const char* dxrt_job_metrics_task_name(dxrt_job_metrics_t jm, int task_idx)
{
    if (!jm || task_idx < 0 || task_idx >= static_cast<int>(jm->task_names.size()))
        return nullptr;
    return jm->task_names[task_idx].c_str();
}

DXRT_CAPI double dxrt_job_metrics_task_cpu_us(dxrt_job_metrics_t jm, int task_idx)
{
    if (!jm || task_idx < 0 || task_idx >= static_cast<int>(jm->data.tasks.size()))
        return 0.0;
    return jm->data.tasks[task_idx].cpu_task_us;
}

DXRT_CAPI int dxrt_job_metrics_task_device_count(dxrt_job_metrics_t jm, int task_idx)
{
    if (!jm || task_idx < 0 || task_idx >= static_cast<int>(jm->device_ids.size()))
        return 0;
    return static_cast<int>(jm->device_ids[task_idx].size());
}

DXRT_CAPI int dxrt_job_metrics_task_device_id(dxrt_job_metrics_t jm, int task_idx, int dev_idx)
{
    if (!jm || task_idx < 0 || task_idx >= static_cast<int>(jm->device_ids.size()))
        return -1;
    const auto& devs = jm->device_ids[task_idx];
    if (dev_idx < 0 || dev_idx >= static_cast<int>(devs.size()))
        return -1;
    return devs[dev_idx];
}

// No-throw: all data is locally cached in the handle; only const map lookup occurs.
DXRT_CAPI dxrt_status_t dxrt_job_metrics_device_timing(
    dxrt_job_metrics_t jm, int task_idx, int dev_idx,
    double* h2d_us, double* inference_us, double* d2h_us, double* total_us)
{
    if (!jm || task_idx < 0 || task_idx >= static_cast<int>(jm->data.tasks.size()))
        return DXRT_ERR_INVALID_ARG;
    const auto& devIds = jm->device_ids[task_idx];
    if (dev_idx < 0 || dev_idx >= static_cast<int>(devIds.size()))
        return DXRT_ERR_INVALID_ARG;
    int deviceId = devIds[dev_idx];
    const auto& task = jm->data.tasks[task_idx];
    auto it = task.devices.find(deviceId);
    if (it == task.devices.end())
        return DXRT_ERR_NOT_FOUND;
    const auto& m = it->second;
    if (h2d_us)       *h2d_us       = m.h2d_us;
    if (inference_us) *inference_us = m.inference_core_all_us;
    if (d2h_us)       *d2h_us       = m.d2h_us;
    if (total_us)     *total_us     = m.total_us;
    return DXRT_OK;
}

DXRT_CAPI dxrt_status_t dxrt_job_metrics_device_timing_ex(
    dxrt_job_metrics_t jm, int task_idx, int dev_idx,
    dxrt_npu_timing_t* out)
{
    if (!out) return DXRT_ERR_INVALID_ARG;

    /* Caller-init negotiation: require at least struct_size + valid fields. */
    const uint32_t kMinSize =
        (uint32_t)(offsetof(dxrt_npu_timing_t, valid) + sizeof(((dxrt_npu_timing_t*)0)->valid));
    const uint32_t caller_size = out->struct_size;
    if (caller_size < kMinSize) return DXRT_ERR_INVALID_ARG;
    const uint32_t lib_size = (uint32_t)sizeof(dxrt_npu_timing_t);
    const uint32_t n = caller_size < lib_size ? caller_size : lib_size;

    /* Zero only the negotiated window — never overrun a smaller caller buffer. */
    std::memset(out, 0, n);

    if (!jm || task_idx < 0 || task_idx >= static_cast<int>(jm->data.tasks.size()))
        return DXRT_ERR_INVALID_ARG;
    const auto& devIds = jm->device_ids[task_idx];
    if (dev_idx < 0 || dev_idx >= static_cast<int>(devIds.size()))
        return DXRT_ERR_INVALID_ARG;
    int deviceId = devIds[dev_idx];
    const auto& task = jm->data.tasks[task_idx];
    auto it = task.devices.find(deviceId);
    if (it == task.devices.end())
        return DXRT_ERR_NOT_FOUND;

    /* Write only fields that fit within the negotiated window.
     * Each field is written if its tail offset <= n. */
    #define DXRT_WRITE_IF_FITS(field, value)                              \
        do {                                                              \
            if (offsetof(dxrt_npu_timing_t, field) + sizeof(out->field)   \
                    <= n) {                                               \
                out->field = (value);                                     \
            }                                                             \
        } while (0)

    const auto& m = it->second;
    out->struct_size = n;            /* signal negotiated size on success */
    out->valid       = m.valid ? 1 : 0;
    DXRT_WRITE_IF_FITS(input_format_us,       m.input_format_us);
    DXRT_WRITE_IF_FITS(h2d_us,                m.h2d_us);
    DXRT_WRITE_IF_FITS(inference_core_all_us, m.inference_core_all_us);
    DXRT_WRITE_IF_FITS(inference_core_0_us,   m.inference_core_0_us);
    DXRT_WRITE_IF_FITS(inference_core_1_us,   m.inference_core_1_us);
    DXRT_WRITE_IF_FITS(inference_core_2_us,   m.inference_core_2_us);
    DXRT_WRITE_IF_FITS(d2h_us,                m.d2h_us);
    DXRT_WRITE_IF_FITS(output_format_us,      m.output_format_us);
    DXRT_WRITE_IF_FITS(total_us,              m.total_us);
    #undef DXRT_WRITE_IF_FITS
    return DXRT_OK;
}

DXRT_CAPI dxrt_status_t dxrt_profiler_show(void)
{
    DXRT_C_TRY
    dxrt::Profiler::GetInstance().Show();
    dxrt::UserEventStore::GetInstance().Show();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_profiler_save(const char* file_path)
{
    if (!file_path) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto user_events = dxrt::UserEventStore::GetInstance().GetEvents();
    dxrt::Profiler::GetInstance().Save(file_path, user_events);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_profiler_clear(void)
{
    DXRT_C_TRY
    dxrt::Profiler::GetInstance().Clear();
    dxrt::UserEventStore::GetInstance().Clear();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_profiler_user_start(const char* event_name)
{
    if (!event_name) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    dxrt::UserEventStore::GetInstance().Start(event_name);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_profiler_user_end(const char* event_name)
{
    if (!event_name) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    dxrt::UserEventStore::GetInstance().End(event_name);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_profiler_user_clear(void)
{
    DXRT_C_TRY
    dxrt::UserEventStore::GetInstance().Clear();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_set_skip_inference_io(int enabled)
{
    DXRT_C_TRY
    clear_error();
    dxrt::SKIP_INFERENCE_IO = enabled ? 1 : 0;
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_get_skip_inference_io(int* out_enabled)
{
    DXRT_C_TRY
    clear_error();
    if (!out_enabled) {
        set_error("out_enabled must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    *out_enabled = dxrt::SKIP_INFERENCE_IO ? 1 : 0;
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Configuration — Extended
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_config_set_enable(dxrt_config_item_t item, int enabled)
{
    DXRT_C_TRY
    auto& cfg = dxrt::Configuration::GetInstance();
    cfg.SetEnable(static_cast<dxrt::Configuration::ITEM>(item), enabled != 0);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_get_enable(dxrt_config_item_t item, int* out_enabled)
{
    if (!out_enabled) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto& cfg = dxrt::Configuration::GetInstance();
    *out_enabled = cfg.GetEnable(static_cast<dxrt::Configuration::ITEM>(item)) ? 1 : 0;
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_set_attribute(dxrt_config_item_t item,
                                                   dxrt_config_attr_t attr,
                                                   const char* value)
{
    if (!value) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto& cfg = dxrt::Configuration::GetInstance();
    cfg.SetAttribute(static_cast<dxrt::Configuration::ITEM>(item),
                     static_cast<dxrt::Configuration::ATTRIBUTE>(attr),
                     value);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_get_attribute(dxrt_config_item_t item,
                                                   dxrt_config_attr_t attr,
                                                   char* buf, size_t buf_size)
{
    if (!buf || buf_size == 0) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto& cfg = dxrt::Configuration::GetInstance();
    std::string val = cfg.GetAttribute(static_cast<dxrt::Configuration::ITEM>(item),
                                       static_cast<dxrt::Configuration::ATTRIBUTE>(attr));
    if (val.size() + 1 > buf_size) {
        set_error("buffer too small for attribute value");
        return DXRT_ERR_INVALID_ARG;
    }
    std::strncpy(buf, val.c_str(), buf_size);
    buf[buf_size - 1] = '\0';
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_get_int_attribute(dxrt_config_item_t item,
                                                      dxrt_config_attr_t attr,
                                                      int* out_value)
{
    if (!out_value) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto& cfg = dxrt::Configuration::GetInstance();
    *out_value = cfg.GetIntAttribute(static_cast<dxrt::Configuration::ITEM>(item),
                                     static_cast<dxrt::Configuration::ATTRIBUTE>(attr));
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_lock_enable(dxrt_config_item_t item)
{
    DXRT_C_TRY
    auto& cfg = dxrt::Configuration::GetInstance();
    cfg.LockEnable(static_cast<dxrt::Configuration::ITEM>(item));
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_config_load_file(const char* file_path)
{
    if (!file_path) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto& cfg = dxrt::Configuration::GetInstance();
    cfg.LoadConfigFile(file_path);
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  InferenceEngine — Performance Metrics
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_inference_time(dxrt_engine_t engine, uint32_t* out_us)
{
    if (!engine || !out_us) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_us = engine->engine->GetNpuInferenceTime();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_latency_mean(dxrt_engine_t engine, double* out_us)
{
    if (!engine || !out_us) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_us = engine->engine->GetLatencyMean();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_latency_stddev(dxrt_engine_t engine, double* out_us)
{
    if (!engine || !out_us) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_us = engine->engine->GetLatencyStdDev();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_time_mean(dxrt_engine_t engine, double* out_us)
{
    if (!engine || !out_us) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_us = engine->engine->GetNpuInferenceTimeMean();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_time_stddev(dxrt_engine_t engine, double* out_us)
{
    if (!engine || !out_us) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_us = engine->engine->GetNpuInferenceTimeStdDev();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_latency_count(dxrt_engine_t engine, int* out_count)
{
    if (!engine || !out_count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_count = engine->engine->GetLatencyCnt();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_time_count(dxrt_engine_t engine, int* out_count)
{
    if (!engine || !out_count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_count = engine->engine->GetNpuInferenceTimeCnt();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_output_tensor_sizes(dxrt_engine_t engine,
                                                             uint64_t* sizes, int* count)
{
    if (!engine || !count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto vec = engine->engine->GetOutputTensorSizes();
    int n = static_cast<int>(vec.size());
    if (!sizes) {
        *count = n;
        return DXRT_OK;
    }
    int cap = *count;
    *count = n;
    int copy_n = (n < cap) ? n : cap;
    for (int i = 0; i < copy_n; ++i)
        sizes[i] = vec[i];
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  InferenceEngine — Model Metadata
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_get_compile_type(dxrt_engine_t engine,
                                                      char* buf, size_t buf_size)
{
    if (!engine || !buf || buf_size == 0) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    std::string val = engine->engine->GetCompileType();
    if (val.size() + 1 > buf_size) {
        set_error("buffer too small for compile type");
        return DXRT_ERR_INVALID_ARG;
    }
    std::strncpy(buf, val.c_str(), buf_size);
    buf[buf_size - 1] = '\0';
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_model_version(dxrt_engine_t engine,
                                                       char* buf, size_t buf_size)
{
    if (!engine || !buf || buf_size == 0) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    std::string val = engine->engine->GetModelVersion();
    if (val.size() + 1 > buf_size) {
        set_error("buffer too small for model version");
        return DXRT_ERR_INVALID_ARG;
    }
    std::strncpy(buf, val.c_str(), buf_size);
    buf[buf_size - 1] = '\0';
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_is_ppu(dxrt_engine_t engine, int* out_is_ppu)
{
    if (!engine || !out_is_ppu) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_is_ppu = engine->engine->IsPPU() ? 1 : 0;
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_has_dynamic_output(dxrt_engine_t engine, int* out_has)
{
    if (!engine || !out_has) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_has = engine->engine->HasDynamicOutput() ? 1 : 0;
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_is_ort_configured(dxrt_engine_t engine, int* out_is)
{
    if (!engine || !out_is) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_is = engine->engine->IsOrtConfigured() ? 1 : 0;
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_is_multi_input(dxrt_engine_t engine, int* out_is)
{
    if (!engine || !out_is) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    *out_is = engine->engine->IsMultiInputModel() ? 1 : 0;
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_dispose(dxrt_engine_t engine)
{
    if (!engine) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    engine->engine->Dispose();
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ══════════════════════════════════════════════════════════════
 *  Performance Data Vectors
 * ══════════════════════════════════════════════════════════════ */

DXRT_CAPI dxrt_status_t dxrt_engine_get_latency_list(
    dxrt_engine_t engine, int* out_values, size_t* inout_count)
{
    if (!engine || !inout_count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto vec = engine->engine->GetLatencyVector();
    if (!out_values) {
        *inout_count = vec.size();
        return DXRT_OK;
    }
    size_t n = (std::min)(*inout_count, vec.size());
    std::memcpy(out_values, vec.data(), n * sizeof(int));
    *inout_count = vec.size();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_time_list(
    dxrt_engine_t engine, uint32_t* out_values, size_t* inout_count)
{
    if (!engine || !inout_count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto vec = engine->engine->GetNpuInferenceTimeVector();
    if (!out_values) {
        *inout_count = vec.size();
        return DXRT_OK;
    }
    size_t n = (std::min)(*inout_count, vec.size());
    std::memcpy(out_values, vec.data(), n * sizeof(uint32_t));
    *inout_count = vec.size();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_bitmatch_mask(
    dxrt_engine_t engine, int index,
    uint8_t* out_mask, size_t* inout_size)
{
    if (!engine || !inout_size) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto vec = engine->engine->GetBitmatchMask(index);
    if (!out_mask) {
        *inout_size = vec.size();
        return DXRT_OK;
    }
    size_t n = (std::min)(*inout_size, vec.size());
    std::memcpy(out_mask, vec.data(), n);
    *inout_size = vec.size();
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Task / Mapping Queries ────────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_engine_get_task_order(
    dxrt_engine_t engine, int* count,
    char** names, size_t buf_len)
{
    if (!engine || !count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto vec = engine->engine->GetTaskOrder();
    int available = static_cast<int>(vec.size());
    if (names) {
        int n = (std::min)(*count, available);
        for (int i = 0; i < n; ++i) {
            if (vec[i].size() + 1 > buf_len) {
                set_error("name buffer too small");
                return DXRT_ERR_INVALID_ARG;
            }
            std::memcpy(names[i], vec[i].c_str(), vec[i].size() + 1);
        }
    }
    *count = available;
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_input_tensor_to_task_mapping(
    dxrt_engine_t engine, int* count,
    char** keys, char** values, size_t buf_len)
{
    if (!engine || !count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto m = engine->engine->GetInputTensorToTaskMapping();
    int available = static_cast<int>(m.size());
    if (keys && values) {
        int n = (std::min)(*count, available);
        int i = 0;
        for (auto& kv : m) {
            if (i >= n) break;
            if (kv.first.size() + 1 > buf_len || kv.second.size() + 1 > buf_len) {
                set_error("buffer too small for mapping entry");
                return DXRT_ERR_INVALID_ARG;
            }
            std::memcpy(keys[i], kv.first.c_str(), kv.first.size() + 1);
            std::memcpy(values[i], kv.second.c_str(), kv.second.size() + 1);
            ++i;
        }
    }
    *count = available;
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Configuration Extended ────────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_config_set_fw_config_json(const char* json_file)
{
    if (!json_file) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto& cfg = dxrt::Configuration::GetInstance();
    cfg.SetFWConfigWithJson(json_file);
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Model Parsing ─────────────────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_parse_model(const char* file_path)
{
    if (!file_path) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    int ret = dxrt::ParseModel(std::string(file_path));
    if (ret != 0) {
        set_error("ParseModel failed");
        return DXRT_ERR_INTERNAL;
    }
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Tensor Metadata ───────────────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_engine_get_input_tensor_info(
    dxrt_engine_t engine, dxrt_tensor_info_t* infos, int* count)
{
    if (!engine || !count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto tensors = engine->engine->GetInputs();
    *count = static_cast<int>(tensors.size());
    if (infos) {
        for (int i = 0; i < *count; ++i)
            fill_tensor_info(infos[i], tensors[i]);
    }
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_output_tensor_info(
    dxrt_engine_t engine, dxrt_tensor_info_t* infos, int* count)
{
    if (!engine || !count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto tensors = engine->engine->GetOutputs();
    *count = static_cast<int>(tensors.size());
    if (infos) {
        for (int i = 0; i < *count; ++i)
            fill_tensor_info(infos[i], tensors[i]);
    }
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_validate_output_count_and_get_info(
    dxrt_engine_t engine,
    int expected_count,
    dxrt_tensor_info_t* out_infos)
{
    if (!engine || (expected_count > 0 && !out_infos)) {
        set_error("engine and out_infos (when expected_count>0) must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto tensors = engine->engine->GetOutputs();
    const int actual = static_cast<int>(tensors.size());
    if (actual != expected_count) {
        std::string msg = "output tensor count mismatch: engine reports ";
        msg += std::to_string(actual);
        msg += " but caller expected ";
        msg += std::to_string(expected_count);
        set_error(msg.c_str());
        return DXRT_ERR_INTERNAL;
    }
    for (int i = 0; i < actual; ++i)
        fill_tensor_info(out_infos[i], tensors[i]);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_all_task_output_count(
    dxrt_engine_t engine, int* count)
{
    if (!engine || !count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto outputs = engine->engine->GetAllTaskOutputs();
    *count = static_cast<int>(outputs.size());
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Multi-Input / Batch Inference ──────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_engine_run_batch(
    dxrt_engine_t engine,
    const void** input_buffers,
    void** output_buffers,
    int batch_size)
{
    if (!engine || !input_buffers || !output_buffers || batch_size <= 0)
        return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    std::vector<void*> inputs(batch_size);
    std::vector<void*> outputs(batch_size);
    for (int i = 0; i < batch_size; ++i) {
        if (!input_buffers[i] || !output_buffers[i]) {
            set_error("input_buffers[i] and output_buffers[i] must not be NULL");
            return DXRT_ERR_INVALID_ARG;
        }
        inputs[i] = const_cast<void*>(input_buffers[i]);
        outputs[i] = output_buffers[i];
    }
    engine->engine->Run(inputs, outputs);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_run_batch_with_tensor_info(
    dxrt_engine_t engine,
    const void** input_buffers,
    void** output_buffers,
    int batch_size,
    dxrt_tensor_info_t* infos,
    int* count)
{
    if (!engine || !input_buffers || !output_buffers || batch_size <= 0 || !count) {
        set_error("engine, input_buffers, output_buffers, count must be valid and batch_size > 0");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    std::vector<void*> inputs(batch_size);
    std::vector<void*> outputs(batch_size);
    for (int i = 0; i < batch_size; ++i) {
        if (!input_buffers[i] || !output_buffers[i]) {
            set_error("input_buffers[i] and output_buffers[i] must not be NULL");
            return DXRT_ERR_INVALID_ARG;
        }
        inputs[i] = const_cast<void*>(input_buffers[i]);
        outputs[i] = output_buffers[i];
    }
    auto results = engine->engine->Run(inputs, outputs);
    return fill_batch_output_infos(results, infos, count);
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    void* output)
{
    if (!engine || !input_names || !input_buffers || !output || num_inputs <= 0) {
        set_error("engine, input_names, input_buffers, output must be valid and num_inputs > 0");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    std::map<std::string, void*> inputs;
    for (int i = 0; i < num_inputs; ++i) {
        if (!input_names[i]) {
            set_error("input_names[i] must not be NULL");
            return DXRT_ERR_INVALID_ARG;
        }
        inputs[input_names[i]] = const_cast<void*>(input_buffers[i]);
    }
    // K2: must be synchronous — header contract says output is filled on return.
    // RunMultiInput internally calls Wait(jobId). Previously this used
    // RunAsyncMultiInput and returned immediately, causing silent data loss.
    // I-b: set_error is now paired with every early return above.
    engine->engine->RunMultiInput(inputs, nullptr, output);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_with_tensor_info(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count)
{
    if (!engine || !input_names || !input_buffers || !output || num_inputs <= 0 || !count) {
        set_error("engine, input_names, input_buffers, output, count must be valid and num_inputs > 0");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    std::map<std::string, void*> inputs;
    for (int i = 0; i < num_inputs; ++i) {
        if (!input_names[i]) {
            set_error("input_names[i] must not be NULL");
            return DXRT_ERR_INVALID_ARG;
        }
        inputs[input_names[i]] = const_cast<void*>(input_buffers[i]);
    }
    auto results = engine->engine->RunMultiInput(inputs, nullptr, output);
    return fill_output_infos(results, infos, count);
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_vector(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    void* output)
{
    if (!engine || !input_buffers || !output || num_inputs <= 0) {
        set_error("engine, input_buffers, output must be valid and num_inputs > 0");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    std::vector<void*> inputs(num_inputs);
    for (int i = 0; i < num_inputs; ++i)
        inputs[i] = const_cast<void*>(input_buffers[i]);
    // K2: see comment in dxrt_engine_run_multi_input above.
    engine->engine->RunMultiInput(inputs, nullptr, output);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_vector_with_tensor_info(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count)
{
    if (!engine || !input_buffers || !output || num_inputs <= 0 || !count) {
        set_error("engine, input_buffers, output, count must be valid and num_inputs > 0");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    std::vector<void*> inputs(num_inputs);
    for (int i = 0; i < num_inputs; ++i)
        inputs[i] = const_cast<void*>(input_buffers[i]);
    auto results = engine->engine->RunMultiInput(inputs, nullptr, output);
    return fill_output_infos(results, infos, count);
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_validate_device(
    dxrt_engine_t engine,
    const void* input,
    int device_id)
{
    if (!engine) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    engine->engine->ValidateDevice(const_cast<void*>(input), device_id);
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Async Multi-Input ─────────────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_engine_run_async_multi_input(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    void* user_arg,
    void* output,
    int* job_id)
{
    if (!engine || !input_names || !input_buffers || num_inputs <= 0 || !job_id)
        return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    std::map<std::string, void*> inputs;
    for (int i = 0; i < num_inputs; ++i) {
        if (!input_names[i]) return DXRT_ERR_INVALID_ARG;
        inputs[input_names[i]] = const_cast<void*>(input_buffers[i]);
    }
    *job_id = engine->engine->RunAsyncMultiInput(inputs, user_arg, output);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_run_async_multi_input_vector(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    void* user_arg,
    void* output,
    int* job_id)
{
    if (!engine || !input_buffers || num_inputs <= 0 || !job_id)
        return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    std::vector<void*> inputs(num_inputs);
    for (int i = 0; i < num_inputs; ++i)
        inputs[i] = const_cast<void*>(input_buffers[i]);
    *job_id = engine->engine->RunAsyncMultiInput(inputs, user_arg, output);
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Validate Device Multi-Input ───────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_engine_validate_device_vector(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    int device_id)
{
    if (!engine || !input_buffers || num_inputs <= 0)
        return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    std::vector<void*> inputs(num_inputs);
    for (int i = 0; i < num_inputs; ++i)
        inputs[i] = const_cast<void*>(input_buffers[i]);
    engine->engine->ValidateDevice(inputs, device_id);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_validate_device_multi_input(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    int device_id)
{
    if (!engine || !input_names || !input_buffers || num_inputs <= 0)
        return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    std::map<std::string, void*> inputs;
    for (int i = 0; i < num_inputs; ++i) {
        if (!input_names[i]) return DXRT_ERR_INVALID_ARG;
        inputs[input_names[i]] = const_cast<void*>(input_buffers[i]);
    }
    engine->engine->ValidateDeviceMultiInput(inputs, device_id);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_validate_device_multi_input_vector(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    int device_id)
{
    if (!engine || !input_buffers || num_inputs <= 0)
        return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    std::vector<void*> inputs(num_inputs);
    for (int i = 0; i < num_inputs; ++i)
        inputs[i] = const_cast<void*>(input_buffers[i]);
    engine->engine->ValidateDeviceMultiInput(inputs, device_id);
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── GetAllTaskOutputs ─────────────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_engine_get_all_task_outputs(
    dxrt_engine_t engine,
    dxrt_tensor_info_t* infos,
    int* total_count,
    int* task_counts,
    int* num_tasks)
{
    if (!engine || !total_count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto all_outputs = engine->engine->GetAllTaskOutputs();
    int n_tasks = static_cast<int>(all_outputs.size());
    int total = 0;
    for (auto& task_out : all_outputs)
        total += static_cast<int>(task_out.size());

    if (num_tasks) *num_tasks = n_tasks;

    if (!infos)
    {
        *total_count = total;
        return DXRT_OK;
    }

    int idx = 0;
    for (int t = 0; t < n_tasks; ++t)
    {
        int task_tensor_count = static_cast<int>(all_outputs[t].size());
        if (task_counts) task_counts[t] = task_tensor_count;
        for (int i = 0; i < task_tensor_count && idx < *total_count; ++i, ++idx)
        {
            auto& tensor = all_outputs[t][i];
            std::memset(&infos[idx], 0, sizeof(dxrt_tensor_info_t));
            std::strncpy(infos[idx].name, tensor->name().c_str(), sizeof(infos[idx].name) - 1);
            infos[idx].type = static_cast<int>(tensor->type());
            infos[idx].size_in_bytes = tensor->size_in_bytes();
            auto shape = tensor->shape();
            const int dims_total = static_cast<int>(shape.size());
            infos[idx].ndim = dims_total > DXRT_MAX_TENSOR_DIMS ? DXRT_MAX_TENSOR_DIMS : dims_total;
            for (int d = 0; d < infos[idx].ndim; ++d)
                infos[idx].shape[d] = static_cast<int>(shape[d]);
        }
    }
    *total_count = idx;
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Device-specific inputs ────────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_engine_get_device_input_count(
    dxrt_engine_t engine,
    int device_id,
    int* count)
{
    if (!engine || !count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto inputs = engine->engine->GetInputs(device_id);
    *count = static_cast<int>(inputs.size());
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_engine_get_device_input_tensor_info(
    dxrt_engine_t engine,
    int device_id,
    dxrt_tensor_info_t* infos,
    int* count)
{
    if (!engine || !count) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    auto all_inputs = engine->engine->GetInputs(device_id);
    int total = 0;
    for (auto& tensors : all_inputs)
        total += static_cast<int>(tensors.size());

    if (!infos)
    {
        *count = total;
        return DXRT_OK;
    }

    int idx = 0;
    for (auto& tensors : all_inputs)
    {
        for (auto& tensor : tensors)
        {
            if (idx >= *count) break;
            std::memset(&infos[idx], 0, sizeof(dxrt_tensor_info_t));
            std::strncpy(infos[idx].name, tensor.name().c_str(), sizeof(infos[idx].name) - 1);
            infos[idx].type = tensor.type();
            infos[idx].size_in_bytes = tensor.size_in_bytes();
            auto shape = tensor.shape();
            const int dims_total = static_cast<int>(shape.size());
            infos[idx].ndim = dims_total > DXRT_MAX_TENSOR_DIMS ? DXRT_MAX_TENSOR_DIMS : dims_total;
            for (int d = 0; d < infos[idx].ndim; ++d)
                infos[idx].shape[d] = shape[d];
            ++idx;
        }
    }
    *count = idx;
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── ParseModel with options ───────────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_parse_model_with_options(
    const char* file_path,
    int verbose,
    int json_extract)
{
    if (!file_path) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
    dxrt::ParseOptions opts;
    opts.verbose = (verbose != 0);
    opts.json_extract = (json_extract != 0);
    int ret = dxrt::ParseModel(std::string(file_path), opts);
    if (ret != 0) return DXRT_ERR_INTERNAL;
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── User Input Release Callback ───────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_engine_register_release_callback(
    dxrt_engine_t engine,
    dxrt_release_callback_t callback,
    void* user_data)
{
    if (!engine || !callback) return DXRT_ERR_INVALID_ARG;
    DXRT_C_TRY
#ifdef USE_VNPU
    engine->engine->RegisterUserInputReleaseCallback(
        [callback, user_data](void* userArg, int jobId) {
            callback(user_data, userArg, jobId);
        });
#else
    (void)callback; (void)user_data;
    return DXRT_ERR_NOT_SUPPORTED;
#endif
    return DXRT_OK;
    DXRT_C_CATCH
}

/* ── Service Functions ─────────────────────────────────────────── */

// Internal dxrtd service v2 entry point. Defined in lib/dxrt_service/
// dxrt_service_v2_main.cpp with C++ linkage and hidden in libdxrt.so
// (version script exports dxrt_* only). The C ABI wrapper below is the
// single supported entry point for the dxrtd daemon.
int dxrt_service_v2_main(int argc, char** argv);

extern "C" DXRT_CAPI int dxrt_service_main(int argc, char** argv)
{
    return dxrt_service_v2_main(argc, argv);
}

extern "C" DXRT_CAPI int dxrt_is_service_running(void)
{
    return dxrt::isDxrtServiceRunning() ? 1 : 0;
}

extern "C" DXRT_CAPI int dxrt_get_task_max_load(void)
{
    return dxrt::GetTaskMaxLoad();
}

#ifdef _WIN32
extern "C" DXRT_CAPI void* dxrt_create_service_mutex(void)
{
    return static_cast<void*>(dxrt::createServiceMutex());
}

extern "C" DXRT_CAPI void dxrt_release_service_mutex(void* handle)
{
    dxrt::releaseServiceMutex(static_cast<HANDLE>(handle));
}
#endif

/* ── Device Monitoring (shared memory) ─────────────────────────── */

DXRT_CAPI dxrt_status_t dxrt_device_get_core_utilization(
    int device_id, int core_id, double* utilization)
{
    if (!utilization) {
        set_error("utilization must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *utilization = status.GetCoreUtilization(core_id);
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_memory_used(int device_id, uint64_t* bytes)
{
    if (!bytes) {
        set_error("bytes must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *bytes = status.GetMemoryUsed();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_get_memory_free(int device_id, uint64_t* bytes)
{
    if (!bytes) {
        set_error("bytes must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *bytes = status.GetMemoryFree();
    return DXRT_OK;
    DXRT_C_CATCH
}

DXRT_CAPI dxrt_status_t dxrt_device_is_monitoring_active(int device_id, int* active)
{
    if (!active) {
        set_error("active must not be NULL");
        return DXRT_ERR_INVALID_ARG;
    }
    DXRT_C_TRY
    auto status = dxrt::DeviceStatus::GetCurrentStatus(device_id);
    *active = status.IsValid() ? 1 : 0;
    return DXRT_OK;
    DXRT_C_CATCH
}
