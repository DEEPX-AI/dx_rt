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
    int cdiv(int a, int b);

    class NpuFormatHandler {
    public:
        // --- Existing Methods ---
        static int encode(Bytes& input, Bytes& output, int col, int unit);
        static int encode_preformatter(Bytes& input, Bytes& output);
        static int encode_preim2col(Bytes& input, Bytes& output, int width, int channel);
        static int encode_formatted(Bytes& input, Bytes& output, int channel);
        static int decode(Bytes& input, Bytes& output, int col, int unit);
        static int decode_aligned(Bytes& input, Bytes& output, int channel, deepx_rmapinfo::DataType dtype);
        static void bidirectional_transpose(void* src, void* dst, int row, int col, size_t element_size);
        static void bidirectional_transpose_inplace(void* src, int row, int col, size_t element_size);

        /**
         * @brief Encodes and transposes data in a single pass.
         * Equivalent to bidirectional_transpose(src, temp, row, col, element_size)
         * followed by encode_formatted(temp, dst, row). // Note: channel for encode becomes original row count
         * Assumes out-of-place operation (input.data != output.data).
         *
         * @param input Input Bytes struct containing the original matrix data (row x col).
         * input.size must be equal to row * col * element_size.
         * @param output Output Bytes struct. Must have pre-allocated data buffer.
         * The required size is calculated and output.size will be updated.
         * Required size = col * cdiv(row, unit) * unit * element_size.
         * @param row Number of rows in the original input matrix.
         * @param col Number of columns in the original input matrix.
         * @param element_size Size of a single data element in bytes.
         * @param unit The encoding unit size (typically 64).
         * @return 0 on success, -1 on error.
         */
        static int encode_formatted_transposed(
            Bytes& input, Bytes& output, int row, int col, size_t element_size, int unit = 64);

        /**
         * @brief Decodes aligned data from the NPU and transposes it according to the specified method,
         * storing the final result in the output buffer. (Combines Decode + Transpose)
         * @param input Encoded NPU data (Bytes struct).
         * @param output Buffer to store the final result (Bytes struct). The function sets the calculated size in output.size.
         * @param channel_for_decode The number of channels (columns) to be used during decoding. Typically the last dimension of shape_encoded.
         * @param dtype Data type (DataType enum).
         * @param shape_encoded Logical shape of the NPU encoded data (std::vector<int>). Used for transpose calculation.
         * @param transpose_type The type of transpose operation to perform (Transpose enum).
         * @return 0 on success, -1 on error.
         */
        static int decode_aligned_transposed(
            Bytes& input, Bytes& output, int channel_for_decode, deepx_rmapinfo::DataType dtype,
            std::vector<int64_t> shape_encoded,
            int transpose_type);

    private:
        NpuFormatHandler() = default; // Prevent instantiation
    };

} // namespace npu_format_handler