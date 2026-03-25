/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
 
#pragma once

#include "dxrt/common.h"
#include "dxrt/model.h" 
#include "dxrt/util.h" 

#include <cstdint>
#include <vector>
#include <cstddef>



namespace npu_format_handler {

    // Bytes struct definition (ensure it's defined, either here or in common.h)
    struct Bytes {
        uint32_t size;
        uint8_t* data;
    };

    // Helper function (can remain public if needed elsewhere, or moved to .cpp as static)
    DXRT_API int cdiv(int a, int b);

    class DXRT_API NpuFormatHandler {
    public:
        // --- Existing Methods ---
        static int encode(const Bytes& input, Bytes& output, int col, int unit);
        static int encode_preformatter(const Bytes& input, Bytes& output, int align_unit = 64);
        static int encode_preim2col(const Bytes& input, Bytes& output, int width, int channel, int align_unit = 64);
        static int encode_formatted(const Bytes& input, Bytes& output, int channel, int align_unit = 64);
        static int decode(const Bytes& input, Bytes& output, int col, int unit);
        static int decode_aligned(const Bytes& input, Bytes& output, int channel, deepx_rmapinfo::DataType dtype, int align_unit = 64);
        static void bidirectional_transpose(void* src, void* dst, int row, int col, size_t element_size);
        static void bidirectional_transpose_inplace(void* src, int row, int col, size_t element_size);
        
        // Architecture-specific transpose implementations
#ifdef USE_IPP
        static void ipp_bidirectional_transpose(void* src, void* dst, int row, int col, size_t element_size);
#endif

#ifdef USE_NEON
        static void neon_bidirectional_transpose(void* src, void* dst, int row, int col, size_t element_size);
#endif

        // --- High-level NFH Processing Functions ---
        
        /**
         * @brief High-level input encoding function for NPU inference requests.
         * Performs appropriate format encoding based on tensor layout and transpose settings.
         * @param reqData Request data containing input tensors and task metadata.
         * @param threadIdForProfiling Thread ID for profiling tags (use -1 if not applicable).
         * @return 0 on success, -1 on error.
         */
        static int EncodeInputs(void* reqData, int threadIdForProfiling = -1);

        /**
         * @brief High-level output decoding function for NPU inference responses.
         * Performs appropriate format decoding and handles different model types (normal, argmax, ppu).
         * @param req Shared pointer to the request object (passed as const void*).
         * @param response NPU response structure.
         * @param threadIdForProfiling Thread ID for profiling tags (use -1 if not applicable).
         * @return 0 on success, -1 on error.
         */
        static int DecodeOutputs(const void* req, const void* response, int threadIdForProfiling = -1);

    private:
        NpuFormatHandler() = default; // Prevent instantiation
    };

} // namespace npu_format_handler