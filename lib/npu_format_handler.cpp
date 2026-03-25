/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/npu_format_handler.h"
#include "dxrt/configuration.h"
#include <vector>
#include <numeric>
#include <functional>
#include <stdexcept>
#include <iostream>
#include "dxrt/common.h"
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <memory>
#ifdef USE_IPP
#include <ipp.h>
#endif
#ifdef USE_NEON
#include "dxrt/xnn_kernel.h"
#endif
// High-level NFH function dependencies
#include "dxrt/request_data.h"
#include "dxrt/request.h"
#include "dxrt/device.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/driver.h"
#include "dxrt/exception/exception.h"

namespace npu_format_handler
{

// Helper function: Integer division rounding up
int cdiv(int a, int b)
{
    if (b == 0)
    {
        LOG_DXRT_ERR("[cdiv] Error: Division by zero.");
        return 0; // Or handle error appropriately
    }
    return (a + (b - 1)) / b;
}

// --- Existing encode function (modified error handling) ---
int NpuFormatHandler::encode(const Bytes& input, Bytes& output, int col, int unit)
{

#ifndef USE_VNPU
    if (input.size == output.size && input.data == output.data)
    {
        return 0;
    }
#endif

    if (col <= 0 || unit <= 0)
    {
         LOG_DXRT_ERR("[encode] Error: Column size (" << col << ") and unit size (" << unit << ") must be positive.");
         return -1;
    }
    if (input.size % col != 0)
    {
        std::string error_msg = "[encode] Error: Input size (" + std::to_string(input.size) +
                                ") is not a multiple of column size (" + std::to_string(col) + ")";
        LOG_DXRT_ERR(error_msg);
        return -1;
    }
    if (input.data == nullptr)
    {
         LOG_DXRT_ERR("[encode] Error: Input data buffer is null.");
         return -1;
    }


    int row = input.size / col;
    int aligned_col = cdiv(col, unit) * unit;
    uint32_t expected_size = (uint32_t)row * aligned_col;

    if (output.data == nullptr)
    {
        LOG_DXRT_ERR("[encode] Error: Output data buffer is null.");
        return -1;
    }
     // It's generally better if the caller provides a sufficiently sized buffer.
     // Warn if provided size is different, then set the correct expected size.
    if (expected_size != output.size)
    {
        LOG_DXRT_ERR("[encode] Warning: Output size is different than expected. "
                  << "Expected size: " << expected_size << ", Provided size: " << output.size
                  << ". Output size will be set to expected.");
    }
    output.size = expected_size; // Set the correct size for the output

    uint8_t* data = output.data; // Use the provided output buffer


    if (input.data == output.data)
    { // In-place operation requires temporary buffer
        try
        {
            // Allocate temporary buffer only if really needed
            if (col == aligned_col)
            {
                 // No padding needed, data is already in the correct format (effectively a no-op)
                 // Size check already done.
                 return 0;
            }

            std::vector<uint8_t> temp_buffer(input.size);
            // No need to check for nullptr, new throws std::bad_alloc on failure

            memcpy(temp_buffer.data(), input.data, input.size);

            // Clear the output area (especially padding) before copying back
            memset(data, 0, output.size);

            for (int i = 0; i < row; ++i)
            {
                memcpy(data + (size_t)i * aligned_col, temp_buffer.data() + (size_t)i * col, col);
            }
        }
        catch (const std::bad_alloc& e) // LCOV_EXCL_BR_LINE
        {
            LOG_DXRT_ERR("[encode] Error: Failed to allocate temporary buffer for in-place operation: " << e.what());
            return -1;
        }
    }
    else
    { // Out-of-place operation
        // Clear the output area (especially padding) before copying
        memset(data, 0, output.size);
        for (int i = 0; i < row; ++i)
        {
            memcpy(data + (size_t)i * aligned_col, input.data + (size_t)i * col, col);
        }
    }

    return 0;
}

// --- Existing encode_preformatter (no changes needed other than calling updated encode) ---
int NpuFormatHandler::encode_preformatter(const Bytes& input, Bytes& output, int align_unit)
{
    int col = input.size;  // Assumes input is a flat vector, col = total size
    if (col == 0 && input.data == nullptr)  // Handle empty input case gracefully
    {
        output.size = 0;
        // output.data should ideally be nullptr or managed by caller
        return 0;
    }
    // If input.size is 0 but data is not null, it might be ambiguous.
    // If col becomes 0, encode will return error. Check here.
     if (col <= 0) {
         LOG_DXRT_ERR("[encode_preformatter] Error: Input size must be positive.");
         return -1;
     }
    return encode(input, output, col, align_unit);
}

// --- Existing encode_preim2col (no changes needed other than calling updated encode) ---
int NpuFormatHandler::encode_preim2col(const Bytes& input, Bytes& output, int width, int channel, int align_unit)
{
     if (width <= 0 || channel <= 0)
     {
         std::string error_msg = "[encode_preim2col] Error: Width (" + std::to_string(width) +
                                 ") and channel (" + std::to_string(channel) + ") must be positive.";
         LOG_DXRT_ERR(error_msg);
         return -1;
     }
    int col = width * channel;
    return encode(input, output, col, align_unit);
}

// --- Existing encode_formatted (modified error handling) ---
int NpuFormatHandler::encode_formatted(const Bytes& input, Bytes& output, int channel, int align_unit)
{
    int col = channel;  // In this context, col is the channel count

    if (col <= 0 || align_unit <= 0)
    {
         std::string error_msg = "[encode_formatted] Error: Channel size (" + std::to_string(col) +
                                 ") and unit size (" + std::to_string(align_unit) + ") must be positive.";
         LOG_DXRT_ERR(error_msg);
         return -1;
    }
    if (input.data == nullptr)
    {
         LOG_DXRT_ERR("[encode_formatted] Error: Input data buffer is null.");
         return -1;
    }
    if (input.size == 0)  // Handle empty input
    {
         output.size = 0;
         return 0;
    }
    if (input.size % col != 0)
    {
        LOG_DXRT_ERR("[encode_formatted] Error: Input size (" << input.size << ") is not a multiple of channel size (" << col << ")");
        return -1;
    }

    int row = input.size / col; // Number of elements per channel? Or number of 'rows' in the logical view
    int col_group = cdiv(col, align_unit); // How many unit-sized groups fit in the columns (channels)
    int aligned_col = col_group * align_unit; // Total width after aligning channels to unit boundary
    uint32_t expected_size = (uint32_t)row * aligned_col; // Expected output size in bytes

    if (output.data == nullptr)
    {
        LOG_DXRT_ERR("[encode_formatted] Error: Output data buffer is null.");
        return -1;
    }
    if (expected_size != output.size)
    {
        LOG_DXRT_ERR("[encode_formatted] Warning: Output size is different than expected. "
                  << "Expected size: " << expected_size << ", Provided size: " << output.size
                  << ". Output size will be set to expected.");
    }
    output.size = expected_size;

    uint8_t* data = output.data;

    // Zero out the buffer initially to handle padding correctly
    memset(data, 0, output.size);

    if (input.data == output.data)
    { // In-place
        try
        {
             std::vector<uint8_t> temp_buffer(input.size);
             memcpy(temp_buffer.data(), input.data, input.size);

            for (int g = 0; g < col_group; ++g)
            {
                for (int i = 0; i < row; ++i)
                {
                    // Calculate addresses relative to the start of the buffers
                    size_t src_addr = static_cast<size_t>(i) * col + static_cast<size_t>(g) * align_unit;
                    size_t dst_addr = static_cast<size_t>(g) * row * align_unit + static_cast<size_t>(i) * align_unit;

                    // Calculate how many bytes to copy for this chunk
                    int remaining_cols = col - g * align_unit;
                    int copy_size = (remaining_cols < align_unit) ? remaining_cols : align_unit;

                    // Ensure copy_size is not negative if col < g*unit (shouldn't happen with cdiv)
                    if (copy_size > 0)
                    {
                        // LCOV_EXCL_BR_START — mathematically unreachable with valid cdiv
                        if (src_addr + copy_size > input.size || dst_addr + copy_size > output.size)
                        {
                             std::string error_msg = "[encode_formatted] Internal Error: "
                                                     "Memory access out of bounds during in-place copy.";
                             LOG_DXRT_ERR(error_msg);
                            return -1;
                        }
                        // LCOV_EXCL_BR_STOP
                        memcpy(data + dst_addr, temp_buffer.data() + src_addr, copy_size);
                    }
                }
            }
        }
        catch (const std::bad_alloc& e) // LCOV_EXCL_BR_LINE
        {
             std::string error_msg = "[encode_formatted] Error: Failed to allocate temporary buffer "
                                     "for in-place operation: " + std::string(e.what());
             LOG_DXRT_ERR(error_msg);
             return -1;
        }
    }
    else
    {  // Out-of-place
        for (int g = 0; g < col_group; ++g)
        {
            for (int i = 0; i < row; ++i)
            {
                size_t src_addr = static_cast<size_t>(i) * col + static_cast<size_t>(g) * align_unit;
                size_t dst_addr = static_cast<size_t>(g) * row * align_unit + static_cast<size_t>(i) * align_unit;
                int remaining_cols = col - g * align_unit;
                int copy_size = (remaining_cols < align_unit) ? remaining_cols : align_unit;

                if (copy_size > 0)
                {
                     // LCOV_EXCL_BR_START — mathematically unreachable with valid cdiv
                     if (src_addr + copy_size > input.size || dst_addr + copy_size > output.size)
                     {
                          std::string error_msg = "[encode_formatted] Internal Error: "
                                                  "Memory access out of bounds during out-of-place copy.";
                          LOG_DXRT_ERR(error_msg);
                          return -1;
                     }
                     // LCOV_EXCL_BR_STOP
                    memcpy(data + dst_addr, input.data + src_addr, copy_size);
                }
            }
        }
    }

    return 0;
}


// --- Existing decode function (modified error handling) ---
int NpuFormatHandler::decode(const Bytes& input, Bytes& output, int col, int unit)
{
    if (col <= 0 || unit <= 0)
    {
        LOG_DXRT_ERR("[decode] Error: Column size (" << col << ") and unit size (" << unit << ") must be positive.");
        return -1;
    }
    int aligned_col = cdiv(col, unit) * unit;
    if (aligned_col == 0)
    {
        LOG_DXRT_ERR("[decode] Error: Calculated aligned column size is zero.");
        return -1;
    }
    if (input.data == nullptr)
    {
        LOG_DXRT_ERR("[decode] Error: Input data buffer is null.");
        return -1;
    }
    if (input.size == 0)
    { // Handle empty input
         output.size = 0;
         return 0;
    }
    if (input.size % aligned_col != 0)
    {
        LOG_DXRT_ERR("[decode] Error: Input size (" << input.size << ") is not a multiple of aligned column size (" << aligned_col << ")");
        return -1;
    }

    int row = input.size / aligned_col;
    uint32_t expected_size = (uint32_t)row * col;

    if (output.data == nullptr)
    {
        LOG_DXRT_ERR("[decode] Error: Output data buffer is null.");
        return -1;
    }
    if (expected_size != output.size)
    {
         LOG_DXRT_ERR("[decode] Warning: Output size is different than expected. "
                   << "Expected size: " << expected_size << ", Provided size: " << output.size
                   << ". Output size will be set to expected.");
    }
    output.size = expected_size; // Set the correct size for the output

    uint8_t* data = output.data;


    if (input.data == output.data)
    { // In-place
        try
        {
             // If no padding was present, it's effectively a no-op (data is already dense)
             if (col == aligned_col)
             {
                  return 0;
             }

             // Need temporary buffer to handle overlapping regions correctly
             std::vector<uint8_t> temp_buffer(input.size);
             memcpy(temp_buffer.data(), input.data, input.size); // Copy original aligned data

             // Copy back only the valid data portions
             for (int i = 0; i < row; ++i)
             {
                  // LCOV_EXCL_BR_START — mathematically unreachable: row/col derived from input.size
                  if ((size_t)i * col + col > output.size || (size_t)i * aligned_col + col > input.size)
                  {
                       LOG_DXRT_ERR("[decode] Internal Error: Memory access out of bounds during in-place copy.");
                       return -1;
                  }
                  // LCOV_EXCL_BR_STOP
                 memcpy(data + i * col, temp_buffer.data() + i * aligned_col, col);
             }
         }
         catch (const std::bad_alloc& e) // LCOV_EXCL_BR_LINE
         {
             LOG_DXRT_ERR("[decode] Error: Failed to allocate temporary buffer for in-place operation: " << e.what());
             return -1;
         }
    }
    else
    { // Out-of-place
        for (int i = 0; i < row; ++i)
        {
            // LCOV_EXCL_BR_START — mathematically unreachable: row/col derived from input.size
            if ((size_t)i * col + col > output.size || (size_t)i * aligned_col + col > input.size)
            {
                  LOG_DXRT_ERR("[decode] Internal Error: Memory access out of bounds during out-of-place copy.");
                  return -1;
            }
            // LCOV_EXCL_BR_STOP
            memcpy(data + (size_t)i * col, input.data + (size_t)i * aligned_col, col);
        }
    }

    return 0;
}

// --- Existing decode_aligned (no changes needed other than calling updated decode) ---
int NpuFormatHandler::decode_aligned(const Bytes& input, Bytes& output, int channel, deepx_rmapinfo::DataType dtype, int align_unit)
{
    int unit = align_unit; // Use align_unit from tensor info
    int col = channel; // Number of columns in elements

     // Scale unit and col to bytes for FLOAT32 if needed
    if (dtype == deepx_rmapinfo::DataType::FLOAT32)
    {
        // Scale unit and col to bytes for the underlying decode function
        unit *= 4;
        col *= 4;
    }
     // For other types (e.g., UINT8), element size is 1, so col and unit remain as element counts.

    return decode(input, output, col, unit);
}

// --- Bidirectional transpose with architecture-specific acceleration ---
void NpuFormatHandler::bidirectional_transpose(void* src, void* dst, int row, int col, size_t element_size)
{
    if (src == nullptr || dst == nullptr)
    {
        LOG_DXRT_ERR("[bidirectional_transpose] Error: Source or destination pointer is null.");
        return; // Keep void return for consistency? Or change API? For now, return.
    }
    if (row <= 0 || col <= 0 || element_size == 0)
    {
        LOG_DXRT_ERR("[bidirectional_transpose] Error: Invalid input parameters (row=" << row
                  << ", col=" << col << ", element_size=" << element_size << ").");
        return;
    }

    // Check whether NFH acceleration is enabled
    bool nfh_accel_enabled = false;
#ifdef FORCE_NFH_ACCELERATION
    nfh_accel_enabled = true;
#else
    nfh_accel_enabled = dxrt::Configuration::GetInstance().IsNfhAccelerationEnabled();
#endif
    if (nfh_accel_enabled && (element_size == 1 || element_size == 4))
    {
#ifdef USE_IPP
        ipp_bidirectional_transpose(src, dst, row, col, element_size);
        return;
#elif defined(USE_NEON)
        neon_bidirectional_transpose(src, dst, row, col, element_size);
        return;
#endif
    }

    // Fallback: naive transpose implementation
    if (src == dst)
    {
        // Call inplace version which handles square/non-square appropriately
        bidirectional_transpose_inplace(src, row, col, element_size);
        return;
    }

    auto* dst_ptr_base = static_cast<uint8_t*>(dst);
    auto* src_ptr_base = static_cast<const uint8_t*>(src);

    for (size_t i = 0; i < static_cast<size_t>(row); i++)
    {
        for (size_t j = 0; j < static_cast<size_t>(col); j++)
        {
            size_t src_offset = (i * static_cast<size_t>(col) + j) * element_size;
            size_t dst_offset = (j * static_cast<size_t>(row) + i) * element_size;  // Transposed index [j][i]
            memcpy(dst_ptr_base + dst_offset, src_ptr_base + src_offset, element_size);
        }
    }
}

#ifdef USE_IPP
void NpuFormatHandler::ipp_bidirectional_transpose(void* src, void* dst, int row, int col, size_t element_size)
{
    // Note: Validation already done by caller (bidirectional_transpose)
    IppStatus status = ippStsNoErr;

    // IPP supports direct transpose for float32 (4 bytes) and uint8 (1 byte)
    if (element_size == 4)
    {
        // Float32 transpose using ippiTranspose_32f_C1R
        const auto* src_f32 = static_cast<const Ipp32f*>(src);
        auto* dst_f32 = static_cast<Ipp32f*>(dst);

        IppiSize srcRoi = {col, row};  // Width, Height in IPP convention
        int srcStep = col * sizeof(Ipp32f);
        int dstStep = row * sizeof(Ipp32f);

        if (src == dst)
        {
            // In-place transpose
            // IPP doesn't support in-place transpose directly, need temporary buffer
            size_t bufferSize = (size_t)row * col * element_size;
            Ipp32f* temp_buffer = ippsMalloc_32f(row * col);
            if (temp_buffer == nullptr)
            {
                LOG_DXRT_ERR("[ipp_bidirectional_transpose] Error: Failed to allocate IPP buffer.");
                return;
            }

            // Copy to temp buffer with transpose
            status = ippiTranspose_32f_C1R(src_f32, srcStep, temp_buffer, dstStep, srcRoi);
            if (status != ippStsNoErr)
            {
                LOG_DXRT_ERR("[ipp_bidirectional_transpose] Error: IPP transpose failed with status " << status);
                ippsFree(temp_buffer);
                return;
            }

            // Copy back to original buffer
            memcpy(dst_f32, temp_buffer, bufferSize);
            ippsFree(temp_buffer);
        }
        else
        {
            // Out-of-place transpose
            status = ippiTranspose_32f_C1R(src_f32, srcStep, dst_f32, dstStep, srcRoi);
            if (status != ippStsNoErr)
            {
                LOG_DXRT_ERR("[ipp_bidirectional_transpose] Error: IPP transpose failed with status " << status);
                return;
            }
        }

    }
    else if (element_size == 1)
    {
        // Uint8 transpose using ippiTranspose_8u_C1R
        const auto* src_u8 = static_cast<const Ipp8u*>(src);
        auto* dst_u8 = static_cast<Ipp8u*>(dst);

        IppiSize srcRoi = {col, row};  // Width, Height in IPP convention
        int srcStep = col * sizeof(Ipp8u);
        int dstStep = row * sizeof(Ipp8u);

        if (src == dst)
        {
            // In-place transpose
            size_t bufferSize = (size_t)row * col * element_size;
            Ipp8u* temp_buffer = ippsMalloc_8u(row * col);
            if (temp_buffer == nullptr)
            {
                LOG_DXRT_ERR("[ipp_bidirectional_transpose] Error: Failed to allocate IPP buffer.");
                return;
            }

            // Copy to temp buffer with transpose
            status = ippiTranspose_8u_C1R(src_u8, srcStep, temp_buffer, dstStep, srcRoi);
            if (status != ippStsNoErr)
            {
                LOG_DXRT_ERR("[ipp_bidirectional_transpose] Error: IPP transpose failed with status " << status);
                ippsFree(temp_buffer);
                return;
            }

            // Copy back to original buffer
            memcpy(dst_u8, temp_buffer, bufferSize);
            ippsFree(temp_buffer);
        }
        else
        {
            // Out-of-place transpose
            status = ippiTranspose_8u_C1R(src_u8, srcStep, dst_u8, dstStep, srcRoi);
            if (status != ippStsNoErr)
            {
                LOG_DXRT_ERR("[ipp_bidirectional_transpose] Error: IPP transpose failed with status " << status);
                return;
            }
        }

    }
}
#endif

#ifdef USE_NEON
// ARM NEON transpose implementation using XNNPACK optimized kernels
// Note: Only called when element_size is 1 or 4 (checked by caller)
void NpuFormatHandler::neon_bidirectional_transpose(void* src, void* dst, int row, int col, size_t element_size)
{
    // Note: Validation and element_size check already done by caller (bidirectional_transpose)

    if (src == dst)
    {
        // For in-place transpose, use temporary buffer
        size_t total_size = static_cast<size_t>(row) * col * element_size;
        std::vector<uint8_t> temp_buffer(total_size);
        memcpy(temp_buffer.data(), src, total_size);

        if (element_size == 1)
        {
            xnnpack_transpose<uint8_t>(
                static_cast<const uint8_t*>(static_cast<void*>(temp_buffer.data())),
                static_cast<uint8_t*>(dst),
                row, col
            );
        }
        else  // element_size == 4
        {
            xnnpack_transpose<float>(
                static_cast<const float*>(static_cast<void*>(temp_buffer.data())),
                static_cast<float*>(dst),
                row, col
            );
        }
        return;
    }

    // Out-of-place: Use XNNPACK optimized transpose based on element size
    if (element_size == 1)
    {
        // uint8_t transpose (16x16 NEON vzipq)
        xnnpack_transpose<uint8_t>(
            static_cast<const uint8_t*>(src),
            static_cast<uint8_t*>(dst),
            row, col
        );
    }
    else  // element_size == 4
    {
        // float/uint32 transpose (4x4 NEON vqtbl4q)
        xnnpack_transpose<float>(
            static_cast<const float*>(src),
            static_cast<float*>(dst),
            row, col
        );
    }
}
#endif

void NpuFormatHandler::bidirectional_transpose_inplace(void* src, int row, int col, size_t element_size)
{
    if (src == nullptr)
    {
        LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Source pointer is null.");
        return;
    }
    if (row <= 0 || col <= 0 || element_size == 0) {
        LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Invalid input parameters (row=" << row
                  << ", col=" << col << ", element_size=" << element_size << ").");
        return;
    }

    // Calculate total data size (consider potential overflow)
    size_t total_elements = static_cast<size_t>(row) * static_cast<size_t>(col);
    size_t total_size_bytes = total_elements * element_size;
    // Basic overflow check (if element_size > 0 and total_elements > 0, result should match)
    if (element_size > 0 && total_elements > 0 && total_size_bytes / element_size != total_elements)
    {
         LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Size calculation overflow detected.");
         return;
    }

    if (total_size_bytes == 0) {
         return; // Nothing to do if size is 0
    }

    auto row_size = static_cast<size_t>(row);
    auto col_size = static_cast<size_t>(col);

    if (row == col)
    {
        // --- Square matrix: Use existing in-place transpose logic ---
        try
        {
            auto src_ptr = static_cast<uint8_t*>(src);
            // Temporary buffer (size of one element)
            // std::vector<uint8_t> temp(element_size); // Option using vector
            std::vector<uint8_t> temp(element_size);

            for (size_t i = 0; i < static_cast<size_t>(row); ++i)
            {
                // Iterate through the upper triangle only (excluding diagonal)
                for (size_t j = i + 1; j < col_size; ++j)
                {
                    // Calculate offsets for elements (i, j) and (j, i)
                    size_t offset1 = (i * col_size + j) * element_size;
                    size_t offset2 = (j * col_size + i) * element_size;

                    // Bounds check on original pointer access (add if necessary)
                    // if (offset1 + element_size > total_size_bytes || offset2 + element_size > total_size_bytes) { ... }

                    uint8_t* ptr1 = src_ptr + offset1;
                    uint8_t* ptr2 = src_ptr + offset2;

                    // Swap elements
                    memcpy(temp.data(), ptr1, element_size);
                    memcpy(ptr1, ptr2, element_size);
                    memcpy(ptr2, temp.data(), element_size);
                }
            }
        }
        catch (const std::bad_alloc& e) // LCOV_EXCL_BR_LINE
        {
            LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Failed to allocate temporary buffer: " << e.what());
        }
    }
    else
    {
        try
        {
            // 1. Allocate temporary buffer (full size)
            std::vector<uint8_t> temp_buffer(total_size_bytes);

            // 2. Copy from src to temp_buffer with transpose (Out-of-place transpose)
            const auto* src_ptr = static_cast<const uint8_t*>(src);
            for (int i = 0; i < row; ++i)
            { // Iterate through original rows
                for (int j = 0; j < col; ++j)
                { // Iterate through original columns
                    size_t src_offset = ((size_t)i * col + j) * element_size;
                    // Destination index in temp buffer is transposed (j, i)
                    // Transposed matrix is C x R, so stride is row
                    size_t temp_dst_offset = (j * row_size + i) * element_size;

                    // LCOV_EXCL_BR_START — mathematically unreachable: offsets derived from row*col
                    if (src_offset + element_size > total_size_bytes)
                    {
                         LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Source read out-of-bounds during temp transpose");
                         return;
                    }
                    if (temp_dst_offset + element_size > total_size_bytes)
                    {
                         LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Temp buffer write out-of-bounds during temp transpose");
                         return;
                    }
                    // LCOV_EXCL_BR_STOP

                    memcpy(temp_buffer.data() + temp_dst_offset, src_ptr + src_offset, element_size);
                }
            }

            // 3. Copy the transposed result from temp_buffer back to the original src buffer
            memcpy(src, temp_buffer.data(), total_size_bytes);

        }
        catch (const std::bad_alloc& e) // LCOV_EXCL_BR_LINE
        {
            LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Failed to allocate temporary buffer for non-square transpose: " << e.what());
            return;
        }
    }
}

// --- High-level NFH Processing Functions Implementation ---


int NpuFormatHandler::EncodeInputs(void* reqDataPtr, int threadIdForProfiling)
{
    using namespace dxrt;

    auto reqData = static_cast<RequestData*>(reqDataPtr);
    if (!reqData || !reqData->taskData)
    {
        LOG_DXRT_ERR("EncodeInputs: invalid reqData");
        return -1;
    }

    if (!Configuration::_sNpuValidateOpt)
    {
        size_t input_count = reqData->inputs.size();
        size_t tensor_info_count = reqData->taskData->_npuInputTensorInfos.size();
        size_t encoded_sizes_count = reqData->taskData->_encodedInputSizes.size();

        if (input_count == 0)
        {
            return 0;
        }
        if (input_count > tensor_info_count || input_count > encoded_sizes_count)
        {
            LOG_DXRT_ERR("EncodeInputs: array size mismatch");
            return -1;
        }
        if (DEBUG_DATA > 0)
        {
            DataDumpBin(reqData->taskData->name() + "_encoder_input.bin", reqData->inputs);
        }

#ifdef USE_PROFILER
        auto& profiler = dxrt::Profiler::GetInstance();
        std::string profile_name = "NPU Input Format Handler[Job_" + std::to_string(reqData->jobId) +
                                   "][" + reqData->taskData->name() +
                                   "][Req_" + std::to_string(reqData->requestId) + "]" +
                                   (threadIdForProfiling >= 0 ? "(" + std::to_string(threadIdForProfiling) + ")" : "");
        profiler.Start(profile_name);
#endif

        for (size_t i = 0; i < input_count; i++)
        {
            if (reqData->encoded_input_ptrs.size() <= i || reqData->encoded_input_ptrs[i] == nullptr)
            {
                LOG_DXRT_ERR("EncodeInputs: encoded_input_ptrs[" << i << "] is nullptr or out of range");
                return -1;
            }

            Tensor& input_tensor = reqData->inputs[i];
            deepx_rmapinfo::TensorInfo tensor_info = reqData->taskData->_npuInputTensorInfos[i];
            auto shape_dims = static_cast<int>(tensor_info.shape_encoded().size());

#ifndef USE_VNPU
            if (input_count == 1 && input_tensor.size_in_bytes() == reqData->taskData->_encodedInputSizes[i])
            {
                reqData->encoded_inputs_ptr = static_cast<uint8_t*>(input_tensor.data());
                reqData->encoded_input_ptrs[i] = static_cast<uint8_t*>(input_tensor.data());
            }
#endif

            Bytes original_input = {
                static_cast<uint32_t>(input_tensor.size_in_bytes()),
                static_cast<uint8_t*>(input_tensor.data())
            };
            Bytes encoded_input = {
                reqData->taskData->_encodedInputSizes[i],
                static_cast<uint8_t*>(reqData->encoded_input_ptrs[i])
            };

            if (original_input.data == nullptr || encoded_input.data == nullptr)
            {
                LOG_DXRT_ERR("EncodeInputs: null data pointer at input " << i);
                return -1;
            }

            if (static_cast<deepx_rmapinfo::Layout>(tensor_info.layout()) == deepx_rmapinfo::Layout::PRE_FORMATTER)
            {
                NpuFormatHandler::encode_preformatter(original_input, encoded_input, tensor_info.align_unit());
            }
            else if (static_cast<deepx_rmapinfo::Layout>(tensor_info.layout()) == deepx_rmapinfo::Layout::PRE_IM2COL)
            {
                NpuFormatHandler::encode_preim2col(
                    original_input, encoded_input,
                    static_cast<int>(tensor_info.shape_encoded()[shape_dims - 2]),
                    static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]),
                    tensor_info.align_unit()
                );
            }
            else if (static_cast<deepx_rmapinfo::Layout>(tensor_info.layout()) == deepx_rmapinfo::Layout::FORMATTED)
            {
                if (tensor_info.transpose() == deepx_rmapinfo::Transpose::TRANSPOSE_NONE)
                {
                    NpuFormatHandler::encode_formatted(
                        original_input, encoded_input,
                        static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]),
                        tensor_info.align_unit()
                    );
                }
                else if (tensor_info.transpose() == deepx_rmapinfo::Transpose::CHANNEL_FIRST_TO_LAST)
                {
                    NpuFormatHandler::encode_formatted(
                        original_input, encoded_input,
                        static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]),
                        tensor_info.align_unit()
                    );

                    Bytes temp_input = {original_input.size, encoded_input.data};
                    auto row = static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]);
                    int col = 1;
                    for (int j = 0; j < shape_dims - 1; j++)
                    {
                        col *= static_cast<int>(tensor_info.shape_encoded()[j]);
                    }
                    auto dtype_encoded = static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded());
                    int elem_size = dxrt::GetDataSize_rmapinfo_datatype(dtype_encoded);
                    NpuFormatHandler::bidirectional_transpose(
                        temp_input.data, encoded_input.data, row, col, elem_size);
                }
                else
                {
                    memcpy(static_cast<void*>(encoded_input.data), static_cast<const void*>(original_input.data), original_input.size);
                }
            }
            else if (static_cast<deepx_rmapinfo::Layout>(tensor_info.layout()) == deepx_rmapinfo::Layout::ALIGNED)
            {
                // Handle ALIGNED layout with transpose options
                if (tensor_info.transpose() == deepx_rmapinfo::Transpose::TRANSPOSE_NONE)
                {
                    // Use encode function with channel parameter (same as decode_aligned uses)
                    auto channel = static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]);
                    int unit = tensor_info.align_unit(); // Use align_unit from tensor info
                    int col = channel; // Number of columns in elements

                    // Scale unit and col to bytes for FLOAT32 if needed
                    auto datatype_encoded = static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded());
                    if (datatype_encoded == deepx_rmapinfo::DataType::FLOAT32)
                    {
                        // Scale unit and col to bytes for the underlying encode function
                        unit *= 4;
                        col *= 4;
                    }

                    NpuFormatHandler::encode(original_input, encoded_input, col, unit);
                }
                else if (tensor_info.transpose() == deepx_rmapinfo::Transpose::CHANNEL_FIRST_TO_LAST)
                {
                    // First apply transpose, then encode with aligned format
                    auto row = static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]);
                    int transpose_col = 1;
                    for (int j = 0; j < shape_dims - 1; j++)
                    {
                        transpose_col *= static_cast<int>(tensor_info.shape_encoded()[j]);
                    }
                    int elem_size = dxrt::GetDataSize_rmapinfo_datatype(static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded()));

                    // Apply transpose first (from original_input to encoded_input buffer)

                    NpuFormatHandler::bidirectional_transpose(original_input.data, encoded_input.data, row, transpose_col, elem_size);

                    // Then encode with aligned format (in-place on encoded_input buffer)
                    Bytes temp_transposed = {original_input.size, encoded_input.data};
                    auto channel = static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]);
                    auto unit = tensor_info.align_unit(); // Use align_unit from tensor info
                    auto col = channel; // Number of columns in elements

                    // Scale unit and col to bytes for FLOAT32 if needed
                    auto datatype_encoded = static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded());
                    if (datatype_encoded == deepx_rmapinfo::DataType::FLOAT32)
                    {
                        // Scale unit and col to bytes for the underlying encode function
                        unit *= 4;
                        col *= 4;
                    }

                    NpuFormatHandler::encode(temp_transposed, encoded_input, col, unit);
                }
                else
                {
                    LOG_DXRT_ERR("Invalid transpose type for ALIGNED layout");
                    memcpy(static_cast<void*>(encoded_input.data),
                            static_cast<const void*>(original_input.data),
                            original_input.size);
                }
            }
            else
            {
                memcpy(static_cast<void*>(encoded_input.data), static_cast<const void*>(original_input.data), original_input.size);
            }
        }

#ifdef USE_PROFILER
        profiler.End(profile_name);
#endif
    }
    else
    {
        for (size_t i = 0; i < reqData->outputs.size(); i++)
        {
            reqData->encoded_input_ptrs[i] = reqData->inputs[i].data();
        }
    }

    return 0;
}


int NpuFormatHandler::DecodeOutputs(const void* reqPtr, const void* responsePtr, int threadIdForProfiling)
{
    using namespace dxrt;

    // Cast from const void* to const std::shared_ptr<Request>*
    const auto req_ptr = static_cast<const std::shared_ptr<Request>*>(reqPtr);
    const auto response = static_cast<const dxrt_response_t*>(responsePtr);

    if (!req_ptr || !(*req_ptr)) return -1;
    std::shared_ptr<Request> req = *req_ptr;

    bool is_decoding = (req->modelType() == ModelType::MODEL_TYPE_NORMAL);
    if (req->modelType() == ModelType::MODEL_TYPE_ARGMAX)
    {
        is_decoding = (req->taskData()->_isArgMax == false);
    }

    if (is_decoding)
    {
        auto* req_data = req->getData();
        if (!Configuration::_sNpuValidateOpt)
        {
#ifdef USE_PROFILER
            auto& profiler = dxrt::Profiler::GetInstance();
            std::string profile_name = "NPU Output Format Handler[Job_" + std::to_string(req->job_id()) +
                                       "][" + req->taskData()->name() +
                                       "][Req_" + std::to_string(req->id()) + "]" +
                                       (threadIdForProfiling >= 0 ? "(" + std::to_string(threadIdForProfiling) + ")" : "");
            profiler.Start(profile_name);
#endif
            for (size_t i = 0; i < req_data->outputs.size(); i++)
            {
                Tensor& output_tensor = req_data->outputs[i];

                LOG_DXRT_DBG << "output_tensor[" << i << "] name: " << output_tensor.name() << std::endl;
                LOG_DXRT_DBG << "output_tensor[" << i << "] memory_type: " << output_tensor.memory_type() << std::endl;
                LOG_DXRT_DBG << "output_tensor[" << i << "] data(): " << output_tensor.data() << std::endl;
                LOG_DXRT_DBG << "output_tensor[" << i << "] size_in_bytes: " << output_tensor.size_in_bytes() << std::endl;
                LOG_DXRT_DBG << "encoded_output_ptrs.size(): " << req_data->encoded_output_ptrs.size() << std::endl;
                if (i < req_data->encoded_output_ptrs.size()) {
                    LOG_DXRT_DBG << "encoded_output_ptrs[" << i << "]: " << req_data->encoded_output_ptrs[i] << std::endl;
                }

                if (output_tensor.memory_type() == static_cast<int>(deepx_rmapinfo::MemoryType::ARGMAX))
                {
                    LOG_DXRT_DBG << "Processing ARGMAX tensor: " << output_tensor.name() << std::endl;
                    if (output_tensor.data() == nullptr) {
                        LOG_DXRT_ERR("ARGMAX output tensor data is nullptr for tensor: " << output_tensor.name());
                        continue;
                    }
                    LOG_DXRT_DBG << "Writing argmax value " << response->argmax << " to output_tensor.data(): " << output_tensor.data() << std::endl;
                    *(static_cast<uint16_t *>(output_tensor.data())) = response->argmax;

                    // ARGMAX tensors don't use encoded_output_ptrs, so we skip that write
                    LOG_DXRT_DBG << "ARGMAX tensor written successfully" << std::endl;
                    if (DEBUG_DATA > 0)
                    {
                        DataDumpBin(req->taskData()->name() + "_output.argmax.bin", output_tensor.data(), static_cast<unsigned int>(output_tensor.size_in_bytes()));
                    }
                    continue;
                }

                deepx_rmapinfo::TensorInfo tensor_info = req_data->taskData->_npuOutputTensorInfos[i];
                auto shape_dims = static_cast<int>(tensor_info.shape_encoded().size());

                // Validate array bounds first
                if (i >= req_data->encoded_output_ptrs.size())
                {
                    LOG_DXRT_ERR("Encoded output pointer index out of bounds for tensor: " << output_tensor.name());
                    continue;
                }

                Bytes encoded_output = {req_data->taskData->_encodedOutputSizes[i], static_cast<uint8_t*>(req_data->encoded_output_ptrs[i])};
                Bytes decoded_output = {static_cast<uint32_t>(output_tensor.size_in_bytes()), static_cast<uint8_t*>(output_tensor.data())};

                // Validate pointers before processing
                if (encoded_output.data == nullptr)
                {
                    LOG_DXRT_ERR("Encoded output pointer is nullptr for tensor: " << output_tensor.name());
                    continue;
                }
                if (decoded_output.data == nullptr)
                {
                    LOG_DXRT_ERR("Decoded output pointer is nullptr for tensor: " << output_tensor.name());
                    continue;
                }

                if (tensor_info.layout() == deepx_rmapinfo::Layout::ALIGNED)
                {
                    if (tensor_info.transpose() == deepx_rmapinfo::Transpose::TRANSPOSE_NONE)
                    {
                        NpuFormatHandler::decode_aligned(encoded_output, decoded_output, static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]), static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded()), tensor_info.align_unit());
                    }
                    else if (tensor_info.transpose() == deepx_rmapinfo::Transpose::CHANNEL_LAST_TO_FIRST)
                    {
                        NpuFormatHandler::decode_aligned(encoded_output, decoded_output, static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]), static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded()), tensor_info.align_unit());
                        Bytes transposed_output = {encoded_output.size, decoded_output.data};
                        auto col = static_cast<int>(tensor_info.shape_encoded()[shape_dims - 1]);
                        int row = 1; for (int j = 0; j < shape_dims - 1; j++) row *= static_cast<int>(tensor_info.shape_encoded()[j]);
                        int elem_size = dxrt::GetDataSize_rmapinfo_datatype(static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded()));
                        NpuFormatHandler::bidirectional_transpose(transposed_output.data, encoded_output.data, row, col, elem_size);
                        output_tensor.data() = static_cast<void*>(encoded_output.data);
                    }
                    else
                    {
                        memcpy(static_cast<void*>(decoded_output.data), static_cast<const void*>(encoded_output.data), encoded_output.size);
                    }
                }
                else
                {
                    memcpy(static_cast<void*>(decoded_output.data), static_cast<const void*>(encoded_output.data), encoded_output.size);
                }
            }
#ifdef USE_PROFILER
            profiler.End(profile_name);
#endif
        }
        else
        {
            for (size_t i = 0; i < req_data->outputs.size(); i++)
                req_data->outputs[i].data() = req_data->encoded_output_ptrs[i];
        }
        if (DEBUG_DATA > 0)
        {
            DataDumpBin(req->taskData()->name() + "_decoder_output.bin", req->outputs());
        }
    }
    else if (req->modelType() == ModelType::MODEL_TYPE_ARGMAX && req->taskData()->_isArgMax)
    {
        *(static_cast<uint16_t *>(req->outputs().front().data())) = response->argmax;

        if (DEBUG_DATA > 0)
        {
            DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
        }
    }
    else if (req->modelType() == ModelType::MODEL_TYPE_PPU)
    {
        RequestData* req_data = req->getData();
        if (!req_data->outputs.empty())
        {
            memcpy(req_data->outputs[0].data(), static_cast<const void*>(req_data->encoded_output_ptrs[0]), 128 * 1024);
            req_data->outputs[0].shape() = std::vector<int64_t>{1, static_cast<int64_t>(response->ppu_filter_num)};
        }

        if (DEBUG_DATA > 0)
        {
            DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
        }
    }
    else if (req->modelType() == ModelType::MODEL_TYPE_PPCPU)
    {
        // PPCPU: Output data is already in encoded_output_ptrs[0]
        // Just set the dynamic shape
        RequestData* req_data = req->getData();
        if (!req_data->outputs.empty() && response->ppu_filter_num > 0)
        {
            DataType dtype = req_data->outputs[0].type();
            size_t unit_size = GetDataSize_Datatype(dtype);
            req_data->outputs[0].shape() = std::vector<int64_t>{static_cast<int64_t>(response->ppu_filter_num),
                                                                 static_cast<int64_t>(unit_size)};
            LOG_DXRT_DBG << "PPCPU output shape set to [" << response->ppu_filter_num
                         << ", " << unit_size << "]" << std::endl;
        }
        else
        {
            LOG_DXRT_DBG << "PPCPU output is empty or ppu_filter_num is 0, req id: " << req->id() << std::endl;
            if (!req_data->outputs.empty())
            {
                req_data->outputs[0].shape() = std::vector<int64_t>{0, 0};
            }
        }

        if (DEBUG_DATA > 0)
        {
            DataDumpBin(req->taskData()->name() + "_output.ppcpu.bin", req->outputs());
        }
    }
    else
    {
        LOG_DXRT_ERR("Invalid model type (normal, argmax, ppu, ppcpu)");
        return -1;
    }

    return 0;
}

} // namespace npu_format_handler
