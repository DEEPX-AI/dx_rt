/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Stable C API — ABI-stable interface for prebuilt delivery.
 *
 * This header is safe to include from C (gcc -std=c11) and C++.
 * All functions use the C calling convention and never throw exceptions.
 * Error information is communicated via return codes (dxrt_status_t)
 * and the thread-local dxrt_last_error_message().
 *
 * Typical usage (C):
 *
 *   #include "dxrt/dxrt_c_api.h"
 *
 *   dxrt_options_t opts;
 *   dxrt_options_init(&opts);
 *
 *   dxrt_engine_t engine = NULL;
 *   if (dxrt_engine_create("model.dxnn", &opts, &engine) != DXRT_OK) {
 *       fprintf(stderr, "create failed: %s\n", dxrt_last_error_message());
 *       return 1;
 *   }
 *
 *   uint64_t in_sz, out_sz;
 *   dxrt_engine_get_input_size(engine, &in_sz);
 *   dxrt_engine_get_output_size(engine, &out_sz);
 *
 *   void* input  = calloc(1, in_sz);
 *   void* output = calloc(1, out_sz);
 *   dxrt_engine_run(engine, input, output);
 *
 *   free(output);
 *   free(input);
 *   dxrt_engine_destroy(engine);
 */

#ifndef DXRT_C_API_H
#define DXRT_C_API_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* ── Visibility / Export ─────────────────────────────────────── */

#ifdef _WIN32
#  ifdef DXRT_STATIC
#    define DXRT_CAPI
#  elif defined(DXRT_C_BUILDING)
#    define DXRT_CAPI __declspec(dllexport)
#  else
#    define DXRT_CAPI __declspec(dllimport)
#  endif
#else
#  define DXRT_CAPI __attribute__((visibility("default")))
#endif

/* ── Version ─────────────────────────────────────────────────── */

#define DXRT_VERSION_MAJOR 3
#define DXRT_VERSION_MINOR 3
#define DXRT_VERSION_PATCH 0

/* ── Opaque Handle Types ─────────────────────────────────────── */

/** Opaque handle to an inference engine instance. */
typedef struct dxrt_engine_s* dxrt_engine_t;

/* ── Error Codes ─────────────────────────────────────────────── */

typedef enum {
    DXRT_OK                 =  0,
    DXRT_ERR_INVALID_ARG    = -1,
    DXRT_ERR_NOT_FOUND      = -2,   /**< model file not found */
    DXRT_ERR_DEVICE         = -3,   /**< NPU device I/O error */
    DXRT_ERR_TIMEOUT        = -4,
    DXRT_ERR_OUT_OF_MEMORY  = -5,
    DXRT_ERR_NOT_SUPPORTED  = -6,
    DXRT_ERR_IO             = -7,   /**< file/service I/O error */
    DXRT_ERR_INVALID_MODEL  = -8,   /**< invalid or corrupt model */
    DXRT_ERR_NOT_AVAILABLE  = -9,   /**< resource not available (e.g. monitoring inactive) */
    DXRT_ERR_INTERNAL       = -99   /**< catch-all for unexpected errors */
} dxrt_status_t;

/* ── Tensor Metadata ───────────────────────────────────────── */

/** Maximum dimensions for tensor shape in C API. */
#define DXRT_MAX_TENSOR_DIMS 8

/** Tensor metadata structure (flat, no pointers). */
typedef struct {
    char     name[256];       /**< Tensor name */
    int      type;            /**< DataType enum value */
    uint32_t elem_size;       /**< Element size in bytes */
    int      ndim;            /**< Number of dimensions */
    int64_t  shape[DXRT_MAX_TENSOR_DIMS]; /**< Shape (unused dims are 0) */
    uint64_t size_in_bytes;   /**< Total size in bytes */
} dxrt_tensor_info_t;

/* ── Options (versioned struct) ──────────────────────────────── */

/**
 * Inference options passed to dxrt_engine_create().
 *
 * Always initialise with dxrt_options_init() to ensure forward-
 * compatible defaults and correct struct_size.
 */
typedef struct {
    uint32_t struct_size;       /**< must be sizeof(dxrt_options_t) */
    int      buffer_count;      /**< number of inference buffers (0 = default) */
    int      device_id;         /**< target device ID (-1 = auto) */
    uint32_t bound_option;      /**< NPU core binding (0 = all) */
    int      use_ort;           /**< 1 = enable ORT tasks, 0 = NPU only, -1 = default */
    uint32_t reserved[6];       /**< zero-initialised, reserved for future use */
} dxrt_options_t;

/** Helper: initialise options with safe defaults. */
static inline void dxrt_options_init(dxrt_options_t* opts)
{
    if (!opts) return;
    memset(opts, 0, sizeof(*opts));     /* zero all fields + padding */
    opts->struct_size   = (uint32_t)sizeof(dxrt_options_t);
    opts->buffer_count  = 0;      /* 0 = library default */
    opts->device_id     = -1;     /* -1 = auto */
    opts->bound_option  = 0;      /* 0 = NPU_ALL */
    opts->use_ort       = -1;     /* -1 = library default */
}

#ifdef __cplusplus
extern "C" {
#endif

/* ══════════════════════════════════════════════════════════════
 *  Version
 * ══════════════════════════════════════════════════════════════ */

/** Return version string, e.g. "3.3.0".  Never returns NULL. */
DXRT_CAPI const char* dxrt_version_string(void);

/* ══════════════════════════════════════════════════════════════
 *  Error Reporting
 * ══════════════════════════════════════════════════════════════ */

/**
 * Return a human-readable message for the most recent error
 * that occurred **on the calling thread**.  Returns "" if no error.
 *
 * @warning The returned pointer is valid only until the next
 *          dxrt_* call on the same thread.  Copy the string
 *          if you need to keep it.
 */
DXRT_CAPI const char* dxrt_last_error_message(void);

/* ══════════════════════════════════════════════════════════════
 *  Engine Lifecycle
 * ══════════════════════════════════════════════════════════════ */

/**
 * Create an inference engine from a compiled model file (.dxnn).
 *
 * @param model_path  Path to .dxnn file (UTF-8, null-terminated).
 * @param opts        Options, or NULL for defaults.
 * @param[out] out    Receives the engine handle on success.
 * @return DXRT_OK on success.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_create(
    const char*            model_path,
    const dxrt_options_t*  opts,
    dxrt_engine_t*         out);

/**
 * Create an inference engine from an in-memory model buffer.
 *
 * @param model_data  Pointer to model data.
 * @param model_size  Size in bytes.
 * @param opts        Options, or NULL for defaults.
 * @param[out] out    Receives the engine handle on success.
 * @return DXRT_OK on success.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_create_from_memory(
    const uint8_t*         model_data,
    size_t                 model_size,
    const dxrt_options_t*  opts,
    dxrt_engine_t*         out);

/**
 * Create an inference engine pinned to a specific set of devices.
 *
 * Use this overload when the caller needs to bind execution to more
 * than one device (e.g. {0, 2}); the single-device case is already
 * covered by dxrt_engine_create() via dxrt_options_t::device_id.
 *
 * @param model_path    Path to .dxnn file (UTF-8, null-terminated).
 * @param opts          Options, or NULL for defaults.
 *                      opts->device_id is ignored — device selection
 *                      is taken exclusively from device_ids/device_count.
 * @param device_ids    Non-NULL array of device IDs (length device_count).
 * @param device_count  Number of device IDs (>= 1).
 * @param[out] out      Receives the engine handle on success.
 * @return DXRT_OK on success.
 *         DXRT_ERR_INVALID_ARG if device_ids is NULL or
 *         device_count <= 0.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_create_with_devices(
    const char*            model_path,
    const dxrt_options_t*  opts,
    const int*             device_ids,
    int                    device_count,
    dxrt_engine_t*         out);

/**
 * Memory-buffer variant of dxrt_engine_create_with_devices().
 * Same semantics as that function, but loads the model from
 * an in-memory buffer instead of a file path.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_create_from_memory_with_devices(
    const uint8_t*         model_data,
    size_t                 model_size,
    const dxrt_options_t*  opts,
    const int*             device_ids,
    int                    device_count,
    dxrt_engine_t*         out);

/**
 * Destroy an engine and free all associated resources.
 * Passing NULL is a safe no-op.
 */
DXRT_CAPI void dxrt_engine_destroy(dxrt_engine_t engine);

/* ══════════════════════════════════════════════════════════════
 *  Synchronous Inference
 * ══════════════════════════════════════════════════════════════ */

/**
 * Run synchronous inference.
 *
 * @param engine   Engine handle.
 * @param input    Input buffer (must be at least input_size bytes).
 * @param output   Output buffer (must not be NULL; must be at least
 *                 output_size bytes).
 * @return DXRT_OK on success.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run(
    dxrt_engine_t  engine,
    const void*    input,
    void*          output);

/**
 * Run synchronous inference, delivering `user_arg` to the callback registered
 * via dxrt_engine_register_callback() (if any).
 *
 * Behaviorally identical to dxrt_engine_run(), except `user_arg` is forwarded
 * to the completion callback instead of being discarded.
 *
 * @param engine    Engine handle.
 * @param input     Input buffer (must be at least input_size bytes).
 * @param user_arg  Opaque pointer forwarded to the completion callback, or NULL.
 * @param output    Output buffer (must not be NULL; must be at least
 *                  output_size bytes).
 * @return DXRT_OK on success.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_with_user_arg(
    dxrt_engine_t  engine,
    const void*    input,
    void*          user_arg,
    void*          output);

/* ══════════════════════════════════════════════════════════════
 *  Asynchronous Inference
 * ══════════════════════════════════════════════════════════════ */

/**
 * Submit an asynchronous inference job.
 *
 * @param engine      Engine handle.
 * @param input       Input buffer.
 * @param user_arg    Opaque per-job pointer returned in the completion callback.
 * @param output      Pre-allocated output buffer, or NULL to let the runtime allocate.
 * @param[out] job_id Receives the job identifier.
 * @return DXRT_OK on success.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_async(
    dxrt_engine_t  engine,
    const void*    input,
    void*          user_arg,
    void*          output,
    int*           job_id);

/**
 * Wait for an asynchronous job to complete and retrieve output.
 *
 * @param engine   Engine handle.
 * @param job_id   Job identifier from dxrt_engine_run_async().
 * @param output   Output buffer (must be at least output_size bytes),
 *                 or NULL to discard output.
 *
 * @note v1 limitation: for multi-output models, all tensors are copied
 *       contiguously into the output buffer. Ensure it is large enough
 *       (use dxrt_engine_get_output_size()).
 * @return DXRT_OK on success.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_wait(
    dxrt_engine_t  engine,
    int            job_id,
    void*          output);

/* ══════════════════════════════════════════════════════════════
 *  Metadata Queries
 * ══════════════════════════════════════════════════════════════ */

/** Get total input buffer size in bytes. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_input_size(
    dxrt_engine_t  engine,
    uint64_t*      size);

/** Get total output buffer size in bytes. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_output_size(
    dxrt_engine_t  engine,
    uint64_t*      size);

/** Get byte offset for a named output tensor within the concatenated output buffer. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_output_tensor_offset(
    dxrt_engine_t  engine,
    const char*    tensor_name,
    size_t*        offset);

/** Get the number of input tensors. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_input_count(
    dxrt_engine_t  engine,
    int*           count);

/** Get the number of output tensors (tail tasks). */
DXRT_CAPI dxrt_status_t dxrt_engine_get_output_count(
    dxrt_engine_t  engine,
    int*           count);

/**
 * Copy the model name into a caller-provided buffer.
 *
 * @param engine    Engine handle.
 * @param buf       Destination buffer.
 * @param buf_len   Size of buf (including NUL terminator space).
 * @return DXRT_OK, or DXRT_ERR_INVALID_ARG if buf is too small.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_model_name(
    dxrt_engine_t  engine,
    char*          buf,
    size_t         buf_len);

/** Get latency of the last inference in microseconds. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_latency(
    dxrt_engine_t  engine,
    int*           latency_us);

/* ══════════════════════════════════════════════════════════════
 *  Tensor Name Queries
 * ══════════════════════════════════════════════════════════════ */

/**
 * Get input tensor names. Caller provides array of char* buffers.
 * @param count [in/out] On input: capacity (number of char* slots in `names`).
 *                       On output: actual number of input tensors.
 *                       Pass *count=0 with names=NULL to query the actual count.
 *                       If names != NULL and *count < actual on input, returns
 *                       DXRT_ERR_INVALID_ARG and writes actual to *count.
 * @param names [out] Array of string buffers (each buf_len bytes). NULL to query count only.
 * @param buf_len  Size of each name buffer.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_input_tensor_names(
    dxrt_engine_t engine, int* count,
    char** names, size_t buf_len);

DXRT_CAPI dxrt_status_t dxrt_engine_get_output_tensor_names(
    dxrt_engine_t engine, int* count,
    char** names, size_t buf_len);

/**
 * Get per-tensor input sizes.
 * @param sizes [out] Array of uint64_t, must have at least *count elements.
 * @param count [in/out] On input: array capacity. On output: actual count.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_input_tensor_sizes(
    dxrt_engine_t engine, uint64_t* sizes, int* count);

/* ══════════════════════════════════════════════════════════════
 *  Benchmark
 * ══════════════════════════════════════════════════════════════ */

/**
 * Run benchmark and return FPS.
 * @param num   Number of iterations.
 * @param fps   [out] Frames per second.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_benchmark(
    dxrt_engine_t engine, int num, void* input_ptr, float* fps);

/* ══════════════════════════════════════════════════════════════
 *  Callback Registration
 * ══════════════════════════════════════════════════════════════ */

/**
 * Zero-copy view of one output tensor across the C ABI boundary, including
 * runtime tensor metadata (`info`) needed to interpret structured outputs
 * (e.g. PPU BBOX/FACE/POSE).
 * `data` points into engine-owned memory and is valid only for the duration
 * of the callback invocation. Callers must copy it out before returning if
 * the data is needed beyond the callback.
 *
 * Always initialise with dxrt_tensor_slice_init() to zero `info`/`reserved`
 * and set `struct_size`. This struct is passed to the callback as a packed
 * array, so its size is fixed: a future field addition changes the array
 * stride and therefore requires a new callback entry point, not an in-place
 * append. `struct_size` is provided only as a defensive sanity check.
 */
typedef struct {
    uint32_t    struct_size;     /**< must be sizeof(dxrt_tensor_slice_t) */
    const void* data;            /**< tensor data pointer (engine-owned, callback-scope) */
    uint64_t    size_in_bytes;
    dxrt_tensor_info_t info;     /**< runtime tensor metadata for this slice */
    uint32_t    reserved[4];     /**< zero-initialised padding for layout/alignment
                                      stability; NOT for forward-compatible field
                                      growth (see note above) */
} dxrt_tensor_slice_t;

/** Helper: initialise a slice with safe defaults. */
static inline void dxrt_tensor_slice_init(dxrt_tensor_slice_t* slice)
{
    if (!slice) return;
    memset(slice, 0, sizeof(*slice));
    slice->struct_size = (uint32_t)sizeof(dxrt_tensor_slice_t);
}

/**
 * Register an async inference completion callback.
 * @param callback  Function pointer:
 *        int callback(const dxrt_tensor_slice_t* tensors, int tensor_count,
 *                     void* user_arg, void* user_data)
 *        tensors:      array of `tensor_count` slices, one per output tensor.
 *                      Each slice's `data` is valid only during this call.
 *                      If the engine produces zero outputs, `tensors` may be
 *                      NULL and `tensor_count` will be 0; the callback is
 *                      still invoked so that the user_arg / user_data
 *                      bookkeeping runs.
 *        tensor_count: number of output tensors.
 *        user_arg:     user argument passed to RunAsync.
 *        user_data:    opaque pointer from registration.
 * @param user_data Opaque pointer passed to every callback invocation.
 *
 * NOTE: This signature replaces the earlier byte-buffer form. C++ users
 *       routing through `dxrt::InferenceEngine::RegisterCallback` see no
 *       source change; direct C consumers must match the new prototype.
 */
typedef int (*dxrt_callback_fn)(const dxrt_tensor_slice_t* tensors,
                                int tensor_count,
                                void* user_arg, void* user_data);

DXRT_CAPI dxrt_status_t dxrt_engine_register_callback(
    dxrt_engine_t engine, dxrt_callback_fn callback, void* user_data);

/* ══════════════════════════════════════════════════════════════
 *  Configuration
 * ══════════════════════════════════════════════════════════════ */

/** Get runtime version string. buf must be at least buf_len bytes. */
DXRT_CAPI dxrt_status_t dxrt_config_get_version(char* buf, size_t buf_len);

/** Get device driver version string. */
DXRT_CAPI dxrt_status_t dxrt_config_get_driver_version(char* buf, size_t buf_len);

/** Get PCIe driver version string. */
DXRT_CAPI dxrt_status_t dxrt_config_get_pcie_driver_version(char* buf, size_t buf_len);

/** Get ONNX Runtime version string. */
DXRT_CAPI dxrt_status_t dxrt_config_get_ort_version(char* buf, size_t buf_len);

/**
 * Get firmware versions for all detected devices.
 * @param device_ids [out] Array of device IDs (may be NULL to query count only).
 * @param versions   [out] Array of char* buffers (each ver_buf_len bytes).
 * @param ver_buf_len Size of each version buffer.
 * @param count      [in/out] On input: array capacity. On output: actual count.
 */
DXRT_CAPI dxrt_status_t dxrt_config_get_firmware_versions(
    int* device_ids, char** versions, size_t ver_buf_len, int* count);

/* ══════════════════════════════════════════════════════════════
 *  Device Status
 * ══════════════════════════════════════════════════════════════ */

/** Get number of available devices. */
DXRT_CAPI dxrt_status_t dxrt_device_get_count(int* count);

/** Get device status info as human-readable string. */
DXRT_CAPI dxrt_status_t dxrt_device_get_status(
    int device_id, char* buf, size_t buf_len);

/** Get device type word (e.g. "DX-M1A"). */
DXRT_CAPI dxrt_status_t dxrt_device_get_type_word(
    int device_id, char* buf, size_t buf_len);

/** Get device variant string. */
DXRT_CAPI dxrt_status_t dxrt_device_get_variant(
    int device_id, char* buf, size_t buf_len);

/** Get board type string. */
DXRT_CAPI dxrt_status_t dxrt_device_get_board_type(
    int device_id, char* buf, size_t buf_len);

/** Get memory type string. */
DXRT_CAPI dxrt_status_t dxrt_device_get_memory_type(
    int device_id, char* buf, size_t buf_len);

/** Get memory size as human-readable string (binary prefix). */
DXRT_CAPI dxrt_status_t dxrt_device_get_memory_size(
    int device_id, char* buf, size_t buf_len);

/** Get all memory info as a string. */
DXRT_CAPI dxrt_status_t dxrt_device_get_all_memory_info(
    int device_id, char* buf, size_t buf_len);

/** Get temperature for a channel (0-2) in degrees Celsius. */
DXRT_CAPI dxrt_status_t dxrt_device_get_temperature(
    int device_id, int channel, int* temp_c);

/** Get NPU clock for a channel (0-2) in MHz. */
DXRT_CAPI dxrt_status_t dxrt_device_get_npu_clock(
    int device_id, int channel, uint32_t* clock_mhz);

/** Get voltage for a channel (0-2) in mV. */
DXRT_CAPI dxrt_status_t dxrt_device_get_voltage(
    int device_id, int channel, uint32_t* voltage_mv);

/** Get NPU status string for a channel. */
DXRT_CAPI dxrt_status_t dxrt_device_get_npu_status(
    int device_id, int channel, char* buf, size_t buf_len);

/** Get DDR status string for a channel. */
DXRT_CAPI dxrt_status_t dxrt_device_get_ddr_status(
    int device_id, int channel, char* buf, size_t buf_len);

/** Get DDR bit error string. */
DXRT_CAPI dxrt_status_t dxrt_device_get_ddr_bit_err(
    int device_id, char* buf, size_t buf_len);

/* ──────────────────────────────────────────────────────────────
 *  Device Info — Numeric accessors
 * ────────────────────────────────────────────────────────────── */

/** Get total device memory size in bytes. */
DXRT_CAPI dxrt_status_t dxrt_device_get_memory_size_bytes(
    int device_id, int64_t* bytes);

/** Get memory clock frequency in MHz. */
DXRT_CAPI dxrt_status_t dxrt_device_get_memory_clock(
    int device_id, uint64_t* clock_mhz);

/** Get number of DMA channels. */
DXRT_CAPI dxrt_status_t dxrt_device_get_dma_channel_count(
    int device_id, uint64_t* count);

/** Get device type as enum (0=Accelerator, 1=Standalone). */
DXRT_CAPI dxrt_status_t dxrt_device_get_type_enum(
    int device_id, uint32_t* device_type);

/** Get device type as short abbreviation ("ACC" or "STD"). */
DXRT_CAPI dxrt_status_t dxrt_device_get_type_str(
    int device_id, char* buf, size_t buf_len);

/** Get PCIe information string (speed, width, bus/dev/func). */
DXRT_CAPI dxrt_status_t dxrt_device_get_pcie_info(
    int device_id, char* buf, size_t buf_len);

/** Get memory size as comma-formatted string (e.g. "2,130,706,432 Byte"). */
DXRT_CAPI dxrt_status_t dxrt_device_get_memory_size_comma(
    int device_id, char* buf, size_t buf_len);

/** Get firmware version string for this device. */
DXRT_CAPI dxrt_status_t dxrt_device_get_firmware_version(
    int device_id, char* buf, size_t buf_len);

/* ──────────────────────────────────────────────────────────────
 *  Device Monitoring (runtime data from shared memory / dxrtd)
 * ────────────────────────────────────────────────────────────── */

/** Get core utilization percentage for a specific core (0.0–100.0).
 *  Returns 0.0 if monitoring data is unavailable, -1.0 if core_id out of range. */
DXRT_CAPI dxrt_status_t dxrt_device_get_core_utilization(
    int device_id, int core_id, double* utilization);

/** Get NPU memory currently in use (bytes). */
DXRT_CAPI dxrt_status_t dxrt_device_get_memory_used(
    int device_id, uint64_t* bytes);

/** Get NPU memory currently free (bytes). */
DXRT_CAPI dxrt_status_t dxrt_device_get_memory_free(
    int device_id, uint64_t* bytes);

/** Check if device monitoring (dxrtd shared memory) is active.
 *  @param active  Output: 1 if active, 0 if not. */
DXRT_CAPI dxrt_status_t dxrt_device_is_monitoring_active(
    int device_id, int* active);

/* ──────────────────────────────────────────────────────────────
 *  Device Pool & Custom Commands
 * ────────────────────────────────────────────────────────────── */

/** Initialize the device pool (discovers and enumerates device cores). */
DXRT_CAPI dxrt_status_t dxrt_device_pool_init(void);

/**
 * Execute a custom command on a device core.
 *
 * @param device_id  Device index (0-based).
 * @param data       Command-specific data buffer (e.g. otp_info_t for OTP read).
 * @param sub_cmd    Sub-command identifier (e.g. DX_GET_OTP=2, DX_SET_LED=4).
 * @param size       Size of data buffer (0 = auto-detect by command type).
 */
DXRT_CAPI dxrt_status_t dxrt_device_custom_command(
    int device_id, void* data, uint32_t sub_cmd, uint32_t size);

/* ══════════════════════════════════════════════════════════════
 *  RuntimeEventDispatcher
 * ══════════════════════════════════════════════════════════════ */

/** Event severity levels (mirrors dxrt::RuntimeEventDispatcher::LEVEL). */
typedef enum {
    DXRT_EVENT_LEVEL_INFO     = 1,
    DXRT_EVENT_LEVEL_WARNING  = 2,
    DXRT_EVENT_LEVEL_ERROR    = 3,
    DXRT_EVENT_LEVEL_CRITICAL = 4
} dxrt_event_level_t;

/** Event type categories (mirrors dxrt::RuntimeEventDispatcher::TYPE). */
typedef enum {
    DXRT_EVENT_TYPE_DEVICE_CORE   = 1000,
    DXRT_EVENT_TYPE_DEVICE_STATUS = 1001,
    DXRT_EVENT_TYPE_DEVICE_IO     = 1002,
    DXRT_EVENT_TYPE_DEVICE_MEMORY = 1003,
    DXRT_EVENT_TYPE_UNKNOWN       = 1004
} dxrt_event_type_t;

/** Specific event codes (mirrors dxrt::RuntimeEventDispatcher::CODE). */
typedef enum {
    DXRT_EVENT_CODE_WRITE_INPUT          = 2000,
    DXRT_EVENT_CODE_READ_OUTPUT          = 2001,
    DXRT_EVENT_CODE_MEMORY_OVERFLOW      = 2002,
    DXRT_EVENT_CODE_MEMORY_ALLOCATION    = 2003,
    DXRT_EVENT_CODE_DEVICE_EVENT         = 2004,
    DXRT_EVENT_CODE_RECOVERY_OCCURRED    = 2005,
    DXRT_EVENT_CODE_TIMEOUT_OCCURRED     = 2006,
    DXRT_EVENT_CODE_THROTTLING_NOTICE    = 2007,
    DXRT_EVENT_CODE_THROTTLING_EMERGENCY = 2008,
    DXRT_EVENT_CODE_UNKNOWN              = 2009
} dxrt_event_code_t;

/** Callback: void handler(level, type, code, message, timestamp, user_data) */
typedef void (*dxrt_event_handler_t)(dxrt_event_level_t level,
                                     dxrt_event_type_t type,
                                     dxrt_event_code_t code,
                                     const char* message,
                                     const char* timestamp,
                                     void* user_data);

/** Dispatch a runtime event. */
DXRT_CAPI dxrt_status_t dxrt_event_dispatch(dxrt_event_level_t level,
                                            dxrt_event_type_t type,
                                            dxrt_event_code_t code,
                                            const char* message);

/** Register a custom event handler callback. */
DXRT_CAPI dxrt_status_t dxrt_event_register_handler(dxrt_event_handler_t handler,
                                                     void* user_data);

/** Set the minimum event level threshold. */
DXRT_CAPI dxrt_status_t dxrt_event_set_level(dxrt_event_level_t level);

/** Get the current event level threshold. */
DXRT_CAPI dxrt_status_t dxrt_event_get_level(dxrt_event_level_t* out_level);

/* ══════════════════════════════════════════════════════════════
 *  Profiler  (V2 — job-based metrics)
 * ══════════════════════════════════════════════════════════════ */

/** Opaque handle to job metrics returned by dxrt_profiler_get_job_metrics(). */
typedef struct dxrt_job_metrics_s* dxrt_job_metrics_t;

/** Get per-job profiling metrics for a completed job.
 *  Call after engine Wait() returns. Caller must free with dxrt_job_metrics_destroy().
 *  @param job_id  Job ID returned by dxrt_engine_run_async().
 *  @param out     Output handle. Set to NULL if job_id not found (returns DXRT_OK with invalid handle). */
DXRT_CAPI dxrt_status_t dxrt_profiler_get_job_metrics(int job_id, dxrt_job_metrics_t* out);

/** Destroy job metrics handle and free associated memory. Safe to call with NULL. */
DXRT_CAPI void dxrt_job_metrics_destroy(dxrt_job_metrics_t jm);

/** Check if job metrics contain valid data. Returns 1 if valid, 0 otherwise. */
DXRT_CAPI int dxrt_job_metrics_is_valid(dxrt_job_metrics_t jm);

/** Get number of tasks in the job metrics. Returns 0 if jm is NULL or invalid. */
DXRT_CAPI int dxrt_job_metrics_task_count(dxrt_job_metrics_t jm);

/** Get task name by index. Returns NULL if index out of range or jm is NULL.
 *  The returned string is valid until dxrt_job_metrics_destroy() is called. */
DXRT_CAPI const char* dxrt_job_metrics_task_name(dxrt_job_metrics_t jm, int task_idx);

/** Get CPU task execution time in microseconds. Returns 0.0 for NPU tasks. */
DXRT_CAPI double dxrt_job_metrics_task_cpu_us(dxrt_job_metrics_t jm, int task_idx);

/** Get number of NPU devices for a task. Returns 0 for CPU tasks or invalid index. */
DXRT_CAPI int dxrt_job_metrics_task_device_count(dxrt_job_metrics_t jm, int task_idx);

/** Get device ID by device index within a task. Returns -1 if invalid. */
DXRT_CAPI int dxrt_job_metrics_task_device_id(dxrt_job_metrics_t jm, int task_idx, int dev_idx);

/** Get NPU timing breakdown for a specific device within a task.
 *  All output values are in microseconds. Any output pointer may be NULL to skip.
 *
 *  Output parameters:
 *    - h2d_us:       Host-to-Device DMA transfer time.
 *    - inference_us: NPU inference time aggregated across all cores
 *                    (mirrors dxrt::NpuDeviceMetrics::inference_core_all_us).
 *                    Per-core breakdown is not exposed via this function; use
 *                    dxrt_job_metrics_device_timing_ex() for the full breakdown.
 *    - d2h_us:       Device-to-Host DMA transfer time.
 *    - total_us:     End-to-end NPU task duration (H2D + inference + D2H + overhead).
 *
 *  @return DXRT_ERR_INVALID_ARG if jm is NULL or indices out of range;
 *          DXRT_ERR_NOT_FOUND if no metrics recorded for that device. */
DXRT_CAPI dxrt_status_t dxrt_job_metrics_device_timing(
    dxrt_job_metrics_t jm, int task_idx, int dev_idx,
    double* h2d_us, double* inference_us, double* d2h_us, double* total_us);

/** Full NPU timing breakdown for one device within a task.
 *  Mirrors dxrt::NpuDeviceMetrics public timing fields. All values in microseconds.
 *
 *  ABI versioning (caller-init pattern, same as dxrt_options_t):
 *    1. Caller MUST call dxrt_npu_timing_init() before passing to
 *       dxrt_job_metrics_device_timing_ex(). _init sets
 *       `struct_size = sizeof(dxrt_npu_timing_t)` as known to the *caller*.
 *    2. The library negotiates `n = min(caller_size, lib_size)` and writes
 *       only the first `n` bytes — never overrunning a smaller caller buffer.
 *    3. On success the library sets `out->struct_size = n` so the caller can
 *       tell which trailing fields are valid (vs. left at _init zero).
 *
 *  `valid` is 1 only when timing fields were populated from a real measurement
 *  (0 on error path or when the underlying measurement did not fire).
 */
typedef struct dxrt_npu_timing_s {
    uint32_t struct_size;          /**< caller: sizeof(dxrt_npu_timing_t) via _init; lib overwrites on success with negotiated size */
    int32_t  valid;                /**< 1 = fields populated from a real measurement; 0 = zero-init / error */
    double input_format_us;        /**< NPU input format handler time */
    double h2d_us;                 /**< Host-to-Device DMA transfer time */
    double inference_core_all_us;  /**< NPU inference time aggregated across all cores */
    double inference_core_0_us;    /**< NPU inference time on core 0 */
    double inference_core_1_us;    /**< NPU inference time on core 1 */
    double inference_core_2_us;    /**< NPU inference time on core 2 */
    double d2h_us;                 /**< Device-to-Host DMA transfer time */
    double output_format_us;       /**< NPU output format handler time */
    double total_us;               /**< End-to-end NPU task duration */
    uint32_t reserved[2];          /**< zero-initialised, reserved for future use */
} dxrt_npu_timing_t;

/** Helper: initialise timing struct (zero fields + caller-side struct_size).
 *  REQUIRED before passing to dxrt_job_metrics_device_timing_ex(). */
static inline void dxrt_npu_timing_init(dxrt_npu_timing_t* t)
{
    if (!t) return;
    memset(t, 0, sizeof(*t));
    t->struct_size = (uint32_t)sizeof(dxrt_npu_timing_t);
}

/** Extended NPU timing getter. Populates fields of dxrt_npu_timing_t using
 *  caller-init struct_size negotiation (see dxrt_npu_timing_t doc).
 *
 *  Contract:
 *    - On any error: only the first `out->struct_size` bytes of *out are
 *      zeroed (valid = 0, struct_size = 0 if it fit). Trailing bytes beyond
 *      what the caller declared are untouched.
 *    - On success: `out->struct_size = min(caller_size, lib_size)`, `valid`
 *      reflects the underlying measurement, timing fields up to negotiated
 *      size are populated.
 *
 *  @return DXRT_ERR_INVALID_ARG if jm or out is NULL, indices out of range,
 *          or out->struct_size is smaller than the minimal supported size
 *          (must include struct_size + valid).
 *          DXRT_ERR_NOT_FOUND if no metrics recorded for that device. */
DXRT_CAPI dxrt_status_t dxrt_job_metrics_device_timing_ex(
    dxrt_job_metrics_t jm, int task_idx, int dev_idx,
    dxrt_npu_timing_t* out);

/** Print profiler summary statistics to stdout. */
DXRT_CAPI dxrt_status_t dxrt_profiler_show(void);

/** Save profiler data to file. */
DXRT_CAPI dxrt_status_t dxrt_profiler_save(const char* file_path);

/** Clear all profiler data. */
DXRT_CAPI dxrt_status_t dxrt_profiler_clear(void);

/** Enable or disable inference I/O skipping (benchmark mode). */
DXRT_CAPI dxrt_status_t dxrt_set_skip_inference_io(int enabled);

/** Get the current inference I/O skipping state. */
DXRT_CAPI dxrt_status_t dxrt_get_skip_inference_io(int* out_enabled);
/* ── User Event Profiling ──────────────────────────────────────────── */

/** Start recording a user profiling event.
 *  @param event_name  Event name (e.g. "preprocess", "postprocess").
 *  @return DXRT_OK on success. */
DXRT_CAPI dxrt_status_t dxrt_profiler_user_start(const char* event_name);

/** End recording a user profiling event.
 *  @param event_name  Event name matching a previous dxrt_profiler_user_start() call.
 *  @return DXRT_OK on success. */
DXRT_CAPI dxrt_status_t dxrt_profiler_user_end(const char* event_name);

/** Clear all user-recorded profiling events.
 *  @return DXRT_OK on success. */
DXRT_CAPI dxrt_status_t dxrt_profiler_user_clear(void);

/* ══════════════════════════════════════════════════════════════
 *  Configuration — Extended
 * ══════════════════════════════════════════════════════════════ */

/** Configuration items (mirrors dxrt::Configuration::ITEM). */
typedef enum {
    DXRT_CONFIG_ITEM_DEBUG               = 1,
    DXRT_CONFIG_ITEM_PROFILER            = 2,
    DXRT_CONFIG_ITEM_SERVICE             = 3,
    DXRT_CONFIG_ITEM_DYNAMIC_CPU_THREAD  = 4,
    DXRT_CONFIG_ITEM_TASK_FLOW           = 5,
    DXRT_CONFIG_ITEM_SHOW_THROTTLING     = 6,
    DXRT_CONFIG_ITEM_SHOW_PROFILE        = 7,
    DXRT_CONFIG_ITEM_SHOW_MODEL_INFO     = 8,
    DXRT_CONFIG_ITEM_CUSTOM_INTRA_OP_THREADS = 9,
    DXRT_CONFIG_ITEM_CUSTOM_INTER_OP_THREADS = 10,
    DXRT_CONFIG_ITEM_NFH_ASYNC           = 11
} dxrt_config_item_t;

/** Configuration attribute keys (mirrors dxrt::Configuration::ATTRIBUTE). */
typedef enum {
    DXRT_CONFIG_ATTR_PROFILER_SHOW_DATA  = 1001,
    DXRT_CONFIG_ATTR_PROFILER_SAVE_DATA  = 1002,
    DXRT_CONFIG_ATTR_CUSTOM_INTRA_OP_NUM = 1003,
    DXRT_CONFIG_ATTR_CUSTOM_INTER_OP_NUM = 1004
} dxrt_config_attr_t;

/** Enable or disable a configuration item. */
DXRT_CAPI dxrt_status_t dxrt_config_set_enable(dxrt_config_item_t item, int enabled);

/** Get the enabled status of a configuration item. */
DXRT_CAPI dxrt_status_t dxrt_config_get_enable(dxrt_config_item_t item, int* out_enabled);

/** Set a string attribute for a configuration item. */
DXRT_CAPI dxrt_status_t dxrt_config_set_attribute(dxrt_config_item_t item,
                                                   dxrt_config_attr_t attr,
                                                   const char* value);

/** Get a string attribute. Copies into caller-provided buffer. */
DXRT_CAPI dxrt_status_t dxrt_config_get_attribute(dxrt_config_item_t item,
                                                   dxrt_config_attr_t attr,
                                                   char* buf, size_t buf_size);

/** Get an integer attribute value. */
DXRT_CAPI dxrt_status_t dxrt_config_get_int_attribute(dxrt_config_item_t item,
                                                      dxrt_config_attr_t attr,
                                                      int* out_value);

/** Lock a configuration item (make read-only). */
DXRT_CAPI dxrt_status_t dxrt_config_lock_enable(dxrt_config_item_t item);

/** Load configuration from file. */
DXRT_CAPI dxrt_status_t dxrt_config_load_file(const char* file_path);

/* ══════════════════════════════════════════════════════════════
 *  InferenceEngine — Performance Metrics
 * ══════════════════════════════════════════════════════════════ */

/** Get last NPU inference time in microseconds. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_inference_time(dxrt_engine_t engine, uint32_t* out_us);

/** Get latency mean across accumulated samples (microseconds). */
DXRT_CAPI dxrt_status_t dxrt_engine_get_latency_mean(dxrt_engine_t engine, double* out_us);

/** Get latency standard deviation (microseconds). */
DXRT_CAPI dxrt_status_t dxrt_engine_get_latency_stddev(dxrt_engine_t engine, double* out_us);

/** Get NPU inference time mean (microseconds). */
DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_time_mean(dxrt_engine_t engine, double* out_us);

/** Get NPU inference time standard deviation (microseconds). */
DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_time_stddev(dxrt_engine_t engine, double* out_us);

/** Get count of accumulated latency samples. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_latency_count(dxrt_engine_t engine, int* out_count);

/** Get count of accumulated NPU inference time samples. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_time_count(dxrt_engine_t engine, int* out_count);

/**
 * Get per-tensor output sizes.
 * @param sizes [out] Array of uint64_t (must have at least *count elements).
 * @param count [in/out] On input: array capacity. On output: actual count.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_output_tensor_sizes(dxrt_engine_t engine,
                                                             uint64_t* sizes, int* count);

/* ══════════════════════════════════════════════════════════════
 *  InferenceEngine — Model Metadata
 * ══════════════════════════════════════════════════════════════ */

/** Get compile type string. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_compile_type(dxrt_engine_t engine,
                                                      char* buf, size_t buf_size);

/** Get model version string. */
DXRT_CAPI dxrt_status_t dxrt_engine_get_model_version(dxrt_engine_t engine,
                                                       char* buf, size_t buf_size);

/** Check if model uses PPU (Pre/Post Processing Unit). */
DXRT_CAPI dxrt_status_t dxrt_engine_is_ppu(dxrt_engine_t engine, int* out_is_ppu);

/** Check if model has dynamic-sized output. */
DXRT_CAPI dxrt_status_t dxrt_engine_has_dynamic_output(dxrt_engine_t engine, int* out_has);

/** Check if ORT (ONNX Runtime) is configured for this engine. */
DXRT_CAPI dxrt_status_t dxrt_engine_is_ort_configured(dxrt_engine_t engine, int* out_is);

/** Check if this is a multi-input model. */
DXRT_CAPI dxrt_status_t dxrt_engine_is_multi_input(dxrt_engine_t engine, int* out_is);

/** Dispose engine resources without destroying the handle. */
DXRT_CAPI dxrt_status_t dxrt_engine_dispose(dxrt_engine_t engine);

/* ══════════════════════════════════════════════════════════════
 *  InferenceEngine — Performance Data Vectors
 * ══════════════════════════════════════════════════════════════ */

/**
 * Get vector of recent latency measurements (microseconds).
 * Call with out_values=NULL to query count only.
 * @param out_values [out] Caller-allocated buffer, or NULL for size query.
 * @param inout_count [in/out] In: buffer capacity. Out: actual count.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_latency_list(
    dxrt_engine_t engine, int* out_values, size_t* inout_count);

/**
 * Get vector of recent NPU inference time measurements (microseconds).
 * Call with out_values=NULL to query count only.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_npu_time_list(
    dxrt_engine_t engine, uint32_t* out_values, size_t* inout_count);

/**
 * Get bitmatch mask for a given NPU task index.
 * Call with out_mask=NULL to query size only.
 * @param index NPU task index.
 * @param out_mask [out] Caller-allocated buffer, or NULL for size query.
 * @param inout_size [in/out] In: buffer capacity. Out: actual size.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_bitmatch_mask(
    dxrt_engine_t engine, int index,
    uint8_t* out_mask, size_t* inout_size);

/* ── Task / Mapping Queries ─────────────────────────────────── */

/**
 * Get task execution order (string array).
 * Call with names=NULL to query count only.
 * @param count [in/out] In: array capacity (ignored on size query). Out: task count.
 * @param names [out] Array of char* buffers, or NULL for count query.
 * @param buf_len Size of each name buffer.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_task_order(
    dxrt_engine_t engine, int* count,
    char** names, size_t buf_len);

/**
 * Get input-tensor-to-task mapping as key-value string pairs.
 * Call with keys=NULL to query count only.
 * @param count [in/out] In: array capacity. Out: number of entries.
 * @param keys [out] Array of char* buffers for keys, or NULL for count query.
 * @param values [out] Array of char* buffers for values, or NULL for count query.
 * @param buf_len Size of each key/value buffer.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_input_tensor_to_task_mapping(
    dxrt_engine_t engine, int* count,
    char** keys, char** values, size_t buf_len);

/* ── Configuration Extended ────────────────────────────────── */

/** Set firmware configuration from a JSON file path. */
DXRT_CAPI dxrt_status_t dxrt_config_set_fw_config_json(const char* json_file);

/* ── Model Parsing ─────────────────────────────────────────── */

/**
 * Parse a model file and return basic information.
 * @param file_path Path to the model file.
 * @return DXRT_OK on success, error code on failure.
 */
DXRT_CAPI dxrt_status_t dxrt_parse_model(const char* file_path);

/**
 * Get input tensor metadata.
 * Call with infos=NULL to query count only.
 * @param infos [out] Array of dxrt_tensor_info_t, or NULL for count query.
 * @param count [in/out] In: array capacity. Out: actual tensor count.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_input_tensor_info(
    dxrt_engine_t engine, dxrt_tensor_info_t* infos, int* count);

/**
 * Get output tensor metadata.
 * Call with infos=NULL to query count only.
 * @param infos [out] Array of dxrt_tensor_info_t, or NULL for count query.
 * @param count [in/out] In: array capacity. Out: actual tensor count.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_output_tensor_info(
    dxrt_engine_t engine, dxrt_tensor_info_t* infos, int* count);

/**
 * Run synchronous inference and return runtime output tensor metadata.
 * @param infos [out] Array of dxrt_tensor_info_t.
 * @param count [in/out] In: array capacity. Out: actual tensor count.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_with_tensor_info(
    dxrt_engine_t engine,
    const void* input,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Run synchronous inference, return runtime output tensor metadata, and
 * deliver `user_arg` to the callback registered via
 * dxrt_engine_register_callback() (if any).
 *
 * Behaviorally identical to dxrt_engine_run_with_tensor_info(), except
 * `user_arg` is forwarded to the completion callback instead of being
 * discarded.
 *
 * @param user_arg Opaque pointer forwarded to the completion callback, or NULL.
 * @param infos [out] Array of dxrt_tensor_info_t.
 * @param count [in/out] In: array capacity. Out: actual tensor count.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_with_tensor_info_and_user_arg(
    dxrt_engine_t engine,
    const void* input,
    void* user_arg,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Wait for an async job, copy output, and return runtime output tensor metadata.
 * @param infos [out] Array of dxrt_tensor_info_t.
 * @param count [in/out] In: array capacity. Out: actual tensor count.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_wait_with_tensor_info(
    dxrt_engine_t engine,
    int job_id,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Validate output tensor count and fetch metadata in one call.
 *
 * Used by the wrapper headers (dxrt_cxx_api.h, wrapper/inference_engine.h)
 * to centralize the count-mismatch check on the C-ABI callback path
 * (zero-copy). Without this, each wrapper would inline its own copy of
 * the validation, risking silent drift between wrappers.
 *
 * Behavior:
 *  - Returns DXRT_ERR_INTERNAL with descriptive error message via
 *    dxrt_last_error_message() if the engine's actual output tensor
 *    count differs from `expected_count` (engine bug — never silently
 *    truncate).
 *  - On success, fills `out_infos[0 .. expected_count-1]` with metadata.
 *
 * @param engine          Engine handle.
 * @param expected_count  The count the caller has (e.g. slice array size).
 * @param out_infos       Caller-allocated buffer of size >= expected_count.
 *                        May be NULL only if expected_count == 0.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_validate_output_count_and_get_info(
    dxrt_engine_t engine,
    int expected_count,
    dxrt_tensor_info_t* out_infos);

/**
 * Get number of task outputs (for multi-task models).
 * @param count [out] Number of tasks that produced outputs.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_all_task_output_count(
    dxrt_engine_t engine, int* count);

/* ── Multi-Input / Batch Inference ─────────────────────────── */

/**
 * Run batch inference with multiple input/output buffer pairs.
 * @param input_buffers Array of input data pointers (one per batch element).
 * @param output_buffers Array of output data pointers (one per batch element).
 * @param batch_size Number of elements in the batch.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_batch(
    dxrt_engine_t engine,
    const void** input_buffers,
    void** output_buffers,
    int batch_size);

DXRT_CAPI dxrt_status_t dxrt_engine_run_batch_with_tensor_info(
    dxrt_engine_t engine,
    const void** input_buffers,
    void** output_buffers,
    int batch_size,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Run batch inference, delivering each item's `user_args[i]` to the callback
 * registered via dxrt_engine_register_callback() (if any) when that item's
 * job completes.
 *
 * Behaviorally identical to dxrt_engine_run_batch(), except `user_args` is
 * forwarded to the completion callback instead of being discarded.
 *
 * @param input_buffers  Array of input data pointers (one per batch element).
 * @param user_args      Array of per-item opaque pointers forwarded to the
 *                       completion callback (one per batch element), or NULL
 *                       to opt out for the whole batch. If non-NULL, must have
 *                       at least `batch_size` valid entries (same implicit-
 *                       length contract as input_buffers/output_buffers).
 * @param output_buffers Array of output data pointers (one per batch element).
 * @param batch_size     Number of elements in the batch.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_batch_with_user_arg(
    dxrt_engine_t engine,
    const void** input_buffers,
    const void** user_args,
    void** output_buffers,
    int batch_size);

/**
 * Run batch inference, return runtime output tensor metadata, and deliver
 * each item's `user_args[i]` to the registered callback. See
 * dxrt_engine_run_batch_with_user_arg() for the `user_args` contract.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_batch_with_tensor_info_and_user_arg(
    dxrt_engine_t engine,
    const void** input_buffers,
    const void** user_args,
    void** output_buffers,
    int batch_size,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Run inference with multiple named input tensors.
 * @param input_names Array of input tensor name strings.
 * @param input_buffers Array of corresponding input data pointers.
 * @param num_inputs Number of inputs.
 * @param output Output buffer.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    void* output);

DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_with_tensor_info(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Run inference with multiple input buffers (ordered by tensor index).
 * @param input_buffers Array of input data pointers.
 * @param num_inputs Number of inputs.
 * @param output Output buffer.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_vector(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    void* output);

DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_vector_with_tensor_info(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Run inference with multiple named input tensors, delivering `user_arg` to
 * the callback registered via dxrt_engine_register_callback() (if any).
 *
 * Behaviorally identical to dxrt_engine_run_multi_input(), except `user_arg`
 * is forwarded to the completion callback instead of being discarded.
 *
 * @param user_arg  Opaque pointer forwarded to the completion callback, or NULL.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_with_user_arg(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    void* user_arg,
    void* output);

/**
 * Run inference with multiple named input tensors, return runtime output
 * tensor metadata, and deliver `user_arg` to the registered callback.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_with_tensor_info_and_user_arg(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    void* user_arg,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Run inference with multiple input buffers (ordered by tensor index),
 * delivering `user_arg` to the callback registered via
 * dxrt_engine_register_callback() (if any).
 *
 * Behaviorally identical to dxrt_engine_run_multi_input_vector(), except
 * `user_arg` is forwarded to the completion callback instead of being
 * discarded.
 *
 * @param user_arg  Opaque pointer forwarded to the completion callback, or NULL.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_vector_with_user_arg(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    void* user_arg,
    void* output);

/**
 * Run inference with multiple input buffers (ordered by tensor index), return
 * runtime output tensor metadata, and deliver `user_arg` to the registered
 * callback.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_multi_input_vector_with_tensor_info_and_user_arg(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    void* user_arg,
    void* output,
    dxrt_tensor_info_t* infos,
    int* count);

/**
 * Validate device by running inference and comparing outputs.
 * @param input Input data pointer.
 * @param device_id Device index to validate.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_validate_device(
    dxrt_engine_t engine,
    const void* input,
    int device_id);

/* ── Async Multi-Input ─────────────────────────────────────── */

/**
 * Submit asynchronous multi-input inference using named tensors.
 * @param engine   Engine handle.
 * @param input_names   Array of tensor name strings.
 * @param input_buffers Array of pointers to input data.
 * @param num_inputs    Number of inputs.
 * @param user_arg      Opaque per-job pointer returned in the completion callback.
 * @param output        Optional pre-allocated output buffer (may be NULL).
 * @param job_id        [out] Job identifier for Wait().
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_async_multi_input(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    void* user_arg,
    void* output,
    int* job_id);

/**
 * Submit asynchronous multi-input inference using vector of pointers.
 * @param engine   Engine handle.
 * @param input_buffers Array of pointers to input data.
 * @param num_inputs    Number of inputs.
 * @param user_arg      Opaque per-job pointer returned in the completion callback.
 * @param output        Optional pre-allocated output buffer (may be NULL).
 * @param job_id        [out] Job identifier for Wait().
 */
DXRT_CAPI dxrt_status_t dxrt_engine_run_async_multi_input_vector(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    void* user_arg,
    void* output,
    int* job_id);

/* ── Validate Device Multi-Input ───────────────────────────── */

/**
 * Validate device with multiple inputs (vector format).
 */
DXRT_CAPI dxrt_status_t dxrt_engine_validate_device_vector(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    int device_id);

/**
 * Validate device with named multi-input tensors.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_validate_device_multi_input(
    dxrt_engine_t engine,
    const char** input_names,
    const void** input_buffers,
    int num_inputs,
    int device_id);

/**
 * Validate device with multi-input (vector format).
 */
DXRT_CAPI dxrt_status_t dxrt_engine_validate_device_multi_input_vector(
    dxrt_engine_t engine,
    const void** input_buffers,
    int num_inputs,
    int device_id);

/* ── GetAllTaskOutputs ─────────────────────────────────────── */

/**
 * Get all task output tensor metadata.
 * Call with infos=NULL to query total count, then call again with buffer.
 * @param engine       Engine handle.
 * @param infos        [out] Array of dxrt_tensor_info_t (or NULL for count query).
 * @param total_count  [in/out] On input: capacity. On output: actual count.
 * @param task_counts  [out] Optional array of per-task tensor counts (may be NULL).
 * @param num_tasks    [out] Optional number of tasks (may be NULL).
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_all_task_outputs(
    dxrt_engine_t engine,
    dxrt_tensor_info_t* infos,
    int* total_count,
    int* task_counts,
    int* num_tasks);

/* ── Device-specific inputs ────────────────────────────────── */

/**
 * Get input tensor count for a specific device.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_device_input_count(
    dxrt_engine_t engine,
    int device_id,
    int* count);

/**
 * Get input tensor info for a specific device.
 * Call with infos=NULL to query count only.
 */
DXRT_CAPI dxrt_status_t dxrt_engine_get_device_input_tensor_info(
    dxrt_engine_t engine,
    int device_id,
    dxrt_tensor_info_t* infos,
    int* count);

/* ── ParseModel with options ───────────────────────────────── */

/**
 * Parse model with options.
 * @param file_path   Path to compiled model file.
 * @param verbose     Show detailed info (1) or not (0).
 * @param json_extract Extract JSON data (1) or not (0).
 */
DXRT_CAPI dxrt_status_t dxrt_parse_model_with_options(
    const char* file_path,
    int verbose,
    int json_extract);

/* ── User Input Release Callback (Internal — VNPU only) ────── */

/** @internal VNPU-only. Not part of the public API. */
typedef void (*dxrt_release_callback_t)(void* user_data, void* user_arg, int job_id);
DXRT_CAPI dxrt_status_t dxrt_engine_register_release_callback(
    dxrt_engine_t engine,
    dxrt_release_callback_t callback,
    void* user_data);

/* ── Service Functions ─────────────────────────────────────────── */

/**
 * Run the dxrtd service main loop.
 * This is the entry point for the dxrtd daemon.
 * Returns 0 on success, non-zero on error.
 */
DXRT_CAPI int dxrt_service_main(int argc, char** argv);

/** Check if a dxrtd instance is already running. Returns 1 if running, 0 if not. */
DXRT_CAPI int dxrt_is_service_running(void);

/** Get the maximum task load value for NPU scheduling. */
DXRT_CAPI int dxrt_get_task_max_load(void);

#ifdef _WIN32
/** Create a service mutex to prevent multiple dxrtd instances (Windows).
 *  Returns opaque handle on success, NULL if another instance is running. */
DXRT_CAPI void* dxrt_create_service_mutex(void);

/** Release a service mutex previously created by dxrt_create_service_mutex (Windows). */
DXRT_CAPI void dxrt_release_service_mutex(void* handle);
#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* DXRT_C_API_H */
