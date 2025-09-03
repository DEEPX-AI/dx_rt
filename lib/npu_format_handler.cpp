/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
 
#include "dxrt/npu_format_handler.h"
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

namespace npu_format_handler {

// Helper function: Integer division rounding up
int cdiv(int a, int b) {
    if (b == 0) {
        LOG_DXRT_ERR("[cdiv] Error: Division by zero.");
        return 0; // Or handle error appropriately
    }
    return (a + (b - 1)) / b;
}

// --- Existing encode function (modified error handling) ---
int NpuFormatHandler::encode(Bytes& input, Bytes& output, int col, int unit) {
    if (col <= 0 || unit <= 0) {
         LOG_DXRT_ERR("[encode] Error: Column size (" << col << ") and unit size (" << unit << ") must be positive.");
         return -1;
    }
    if (input.size % col != 0) {
        LOG_DXRT_ERR("[encode] Error: Input size (" << input.size << ") is not a multiple of column size (" << col << ")");
        // perror("Reason"); // perror is for system errors, not logical errors
        return -1;
    }
    if (input.data == nullptr) {
         LOG_DXRT_ERR("[encode] Error: Input data buffer is null.");
         return -1;
    }


    int row = input.size / col;
    int aligned_col = cdiv(col, unit) * unit;
    uint32_t expected_size = (uint32_t)row * aligned_col;

    if (output.data == nullptr) {
        LOG_DXRT_ERR("[encode] Error: Output data buffer is null.");
        // perror("[encode] Error allocating memory"); // Misleading - memory should be pre-allocated
        return -1;
    }
     // It's generally better if the caller provides a sufficiently sized buffer.
     // Warn if provided size is different, then set the correct expected size.
    if (expected_size != output.size) {
        LOG_DXRT_ERR("[encode] Warning: Output size is different than expected. "
                  << "Expected size: " << expected_size << ", Provided size: " << output.size
                  << ". Output size will be set to expected.");
    }
    output.size = expected_size; // Set the correct size for the output

    uint8_t* data = output.data; // Use the provided output buffer


    if (input.data == output.data) { // In-place operation requires temporary buffer
        try {
            // Allocate temporary buffer only if really needed
            if (col == aligned_col) {
                 // No padding needed, data is already in the correct format (effectively a no-op)
                 // Size check already done.
                 return 0;
            }

            uint8_t* temp_buffer = new uint8_t[input.size];
            // No need to check for nullptr, new throws std::bad_alloc on failure

            memcpy(temp_buffer, input.data, input.size);

            // Clear the output area (especially padding) before copying back
            memset(data, 0, output.size);

            for (int i = 0; i < row; ++i) {
                memcpy(data + (size_t)i * aligned_col, temp_buffer + (size_t)i * col, col);
            }

            delete[] temp_buffer;
        }
        catch (const std::bad_alloc& e) {
            LOG_DXRT_ERR("[encode] Error: Failed to allocate temporary buffer for in-place operation: " << e.what());
            return -1;
        }
         catch (const std::exception& e) {
             LOG_DXRT_ERR("[encode] Error during in-place operation: " << e.what());
             return -1;
         }
    } else { // Out-of-place operation
        // Clear the output area (especially padding) before copying
        memset(data, 0, output.size);
        for (int i = 0; i < row; ++i) {
            memcpy(data + (size_t)i * aligned_col, input.data + (size_t)i * col, col);
        }
    }

    return 0;
}

// --- Existing encode_preformatter (no changes needed other than calling updated encode) ---
int NpuFormatHandler::encode_preformatter(Bytes& input, Bytes& output) {
    const int unit = 64;
    int col = input.size; // Assumes input is a flat vector, col = total size
    if (col == 0 && input.data == nullptr) { // Handle empty input case gracefully
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
    return encode(input, output, col, unit);
}

// --- Existing encode_preim2col (no changes needed other than calling updated encode) ---
int NpuFormatHandler::encode_preim2col(Bytes& input, Bytes& output, int width, int channel) {
    const int unit = 64;
     if (width <= 0 || channel <= 0) {
         LOG_DXRT_ERR("[encode_preim2col] Error: Width (" << width << ") and channel (" << channel << ") must be positive.");
         return -1;
     }
    int col = width * channel;
    return encode(input, output, col, unit);
}

// --- Existing encode_formatted (modified error handling) ---
int NpuFormatHandler::encode_formatted(Bytes& input, Bytes& output, int channel) {
    const int unit = 64;
    int col = channel; // In this context, col is the channel count

    if (col <= 0 || unit <= 0) {
         LOG_DXRT_ERR("[encode_formatted] Error: Channel size (" << col << ") and unit size (" << unit << ") must be positive.");
         return -1;
    }
     if (input.data == nullptr) {
         LOG_DXRT_ERR("[encode_formatted] Error: Input data buffer is null.");
         return -1;
     }
    if (input.size == 0) { // Handle empty input
         output.size = 0;
         return 0;
    }
    if (input.size % col != 0) {
        LOG_DXRT_ERR("[encode_formatted] Error: Input size (" << input.size << ") is not a multiple of channel size (" << col << ")");
        return -1;
    }

    int row = input.size / col; // Number of elements per channel? Or number of 'rows' in the logical view
    int col_group = cdiv(col, unit); // How many unit-sized groups fit in the columns (channels)
    int aligned_col = col_group * unit; // Total width after aligning channels to unit boundary
    uint32_t expected_size = (uint32_t)row * aligned_col; // Expected output size in bytes

    if (output.data == nullptr) {
        LOG_DXRT_ERR("[encode_formatted] Error: Output data buffer is null.");
        return -1;
    }
     if (expected_size != output.size) {
         LOG_DXRT_ERR("[encode_formatted] Warning: Output size is different than expected. "
                   << "Expected size: " << expected_size << ", Provided size: " << output.size
                   << ". Output size will be set to expected.");
     }
     output.size = expected_size;

    uint8_t* data = output.data;

    // Zero out the buffer initially to handle padding correctly
    memset(data, 0, output.size);

    if (input.data == output.data) { // In-place
        try {
             uint8_t* temp_buffer = new uint8_t[input.size];
             memcpy(temp_buffer, input.data, input.size);

             for (int g = 0; g < col_group; ++g) {
                 for (int i = 0; i < row; ++i) {
                     // Calculate addresses relative to the start of the buffers
                     size_t src_addr = (size_t)i * col + (size_t)g * unit;
                     size_t dst_addr = (size_t)g * row * unit + (size_t)i * unit;

                     // Calculate how many bytes to copy for this chunk
                     int remaining_cols = col - g * unit;
                     int copy_size = (remaining_cols < unit) ? remaining_cols : unit;

                     // Ensure copy_size is not negative if col < g*unit (shouldn't happen with cdiv)
                     if (copy_size > 0) {
                         // Check bounds before memcpy
                          if (src_addr + copy_size > input.size || dst_addr + copy_size > output.size) {
                               LOG_DXRT_ERR("[encode_formatted] Internal Error: Memory access out of bounds during in-place copy.");
                              delete[] temp_buffer; // Clean up memory
                              return -1;
                         }
                         memcpy(data + dst_addr, temp_buffer + src_addr, copy_size);
                     }
                 }
             }
             delete[] temp_buffer;
        } catch (const std::bad_alloc& e) {
             LOG_DXRT_ERR("[encode_formatted] Error: Failed to allocate temporary buffer for in-place operation: " << e.what());
             return -1;
        } catch (const std::exception& e) {
             LOG_DXRT_ERR("[encode_formatted] Error during in-place operation: " << e.what());
             return -1;
         }

    } else { // Out-of-place
        for (int g = 0; g < col_group; ++g) {
            for (int i = 0; i < row; ++i) {
                size_t src_addr = (size_t)i * col + (size_t)g * unit;
                size_t dst_addr = (size_t)g * row * unit + (size_t)i * unit;
                int remaining_cols = col - g * unit;
                int copy_size = (remaining_cols < unit) ? remaining_cols : unit;

                if (copy_size > 0) {
                     // Check bounds before memcpy
                     if (src_addr + copy_size > input.size || dst_addr + copy_size > output.size) {
                          LOG_DXRT_ERR("[encode_formatted] Internal Error: Memory access out of bounds during out-of-place copy.");
                          return -1;
                     }
                    memcpy(data + dst_addr, input.data + src_addr, copy_size);
                }
            }
        }
    }

    return 0;
}


// --- Existing decode function (modified error handling) ---
int NpuFormatHandler::decode(Bytes& input, Bytes& output, int col, int unit) {
     if (col <= 0 || unit <= 0) {
         LOG_DXRT_ERR("[decode] Error: Column size (" << col << ") and unit size (" << unit << ") must be positive.");
         return -1;
    }
    int aligned_col = cdiv(col, unit) * unit;
     if (aligned_col == 0) {
         LOG_DXRT_ERR("[decode] Error: Calculated aligned column size is zero.");
         return -1;
     }
     if (input.data == nullptr) {
         LOG_DXRT_ERR("[decode] Error: Input data buffer is null.");
         return -1;
     }
    if (input.size == 0) { // Handle empty input
         output.size = 0;
         return 0;
    }
     if (input.size % aligned_col != 0) {
         LOG_DXRT_ERR("[decode] Error: Input size (" << input.size << ") is not a multiple of aligned column size (" << aligned_col << ")");
         return -1;
    }

    int row = input.size / aligned_col;
    uint32_t expected_size = (uint32_t)row * col;

    if (output.data == nullptr) {
        LOG_DXRT_ERR("[decode] Error: Output data buffer is null.");
        return -1;
    }
    if (expected_size != output.size) {
         LOG_DXRT_ERR("[decode] Warning: Output size is different than expected. "
                   << "Expected size: " << expected_size << ", Provided size: " << output.size
                   << ". Output size will be set to expected.");
    }
    output.size = expected_size; // Set the correct size for the output

    uint8_t* data = output.data;


    if (input.data == output.data) { // In-place
        try {
             // If no padding was present, it's effectively a no-op (data is already dense)
             if (col == aligned_col) {
                  return 0;
             }

             // Need temporary buffer to handle overlapping regions correctly
             uint8_t* temp_buffer = new uint8_t[input.size];
             memcpy(temp_buffer, input.data, input.size); // Copy original aligned data

             // Copy back only the valid data portions
             for (int i = 0; i < row; ++i) {
                  // Check bounds before memcpy
                   if ((size_t)i * col + col > output.size || (size_t)i * aligned_col + col > input.size) {
                        LOG_DXRT_ERR("[decode] Internal Error: Memory access out of bounds during in-place copy.");
                        delete[] temp_buffer;
                        return -1;
                  }
                 memcpy(data + (size_t)i * col, temp_buffer + (size_t)i * aligned_col, col);
             }
             delete[] temp_buffer;
         } catch (const std::bad_alloc& e) {
             LOG_DXRT_ERR("[decode] Error: Failed to allocate temporary buffer for in-place operation: " << e.what());
             return -1;
         } catch (const std::exception& e) {
             LOG_DXRT_ERR("[decode] Error during in-place operation: " << e.what());
             return -1;
         }
    } else { // Out-of-place
        for (int i = 0; i < row; ++i) {
             // Check bounds before memcpy
             if ((size_t)i * col + col > output.size || (size_t)i * aligned_col + col > input.size) {
                   LOG_DXRT_ERR("[decode] Internal Error: Memory access out of bounds during out-of-place copy.");
                   return -1;
             }
            memcpy(data + (size_t)i * col, input.data + (size_t)i * aligned_col, col);
        }
    }

    return 0;
}

// --- Existing decode_aligned (no changes needed other than calling updated decode) ---
int NpuFormatHandler::decode_aligned(Bytes& input, Bytes& output, int channel, deepx_rmapinfo::DataType dtype) {
    int unit = 64; // Base unit size in elements
    int col = channel; // Number of columns in elements

     // Adjust unit and calculate byte-based col/unit for FLOAT32 if needed
    if (dtype == deepx_rmapinfo::DataType::FLOAT32) {
        // Check if channel count (elements) is less than 16
        if (col < unit) {
            unit = 16; // Use smaller unit for narrow float cases
        }
        // Scale unit and col to bytes for the underlying decode function
        unit *= 4;
        col *= 4;
    }
     // For other types (e.g., UINT8), element size is 1, so col and unit remain as element counts.

    return decode(input, output, col, unit);
}

// --- Existing bidirectional_transpose (modified error handling) ---
void NpuFormatHandler::bidirectional_transpose(void* src, void* dst, int row, int col, size_t element_size) {
    if (src == nullptr || dst == nullptr) {
        LOG_DXRT_ERR("[bidirectional_transpose] Error: Source or destination pointer is null.");
        return; // Keep void return for consistency? Or change API? For now, return.
    }
     if (row <= 0 || col <= 0 || element_size == 0) {
         LOG_DXRT_ERR("[bidirectional_transpose] Error: Invalid input parameters (row=" << row
                   << ", col=" << col << ", element_size=" << element_size << ").");
         return;
     }


    try {
        if (src == dst) {
            // Call inplace version which handles square/non-square appropriately
            bidirectional_transpose_inplace(src, row, col, element_size);
            return;
        }

        uint8_t* dst_ptr_base = static_cast<uint8_t*>(dst);
        const uint8_t* src_ptr_base = static_cast<const uint8_t*>(src);

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                size_t src_offset = ((size_t)i * col + j) * element_size;
                size_t dst_offset = ((size_t)j * row + i) * element_size; // Transposed index [j][i]
                // Basic bounds check (optional, assumes caller allocated enough space)
                // size_t src_total_size = (size_t)row * col * element_size;
                // size_t dst_total_size = (size_t)col * row * element_size;
                // if (src_offset + element_size > src_total_size || dst_offset + element_size > dst_total_size) {
                //     LOG_DXRT_ERR("[bidirectional_transpose] Warning: Potential out-of-bounds access during transpose.");
                //     // Continue carefully or return error
                // }
                memcpy(dst_ptr_base + dst_offset, src_ptr_base + src_offset, element_size);
            }
        }
    }
    catch (const std::exception& e) {
        // Catch potential exceptions from memory operations if any (though memcpy usually doesn't throw std::exception)
        LOG_DXRT_ERR("[bidirectional_transpose] Error: " << e.what());
        // Cannot return error code easily from void function.
    }
}

void NpuFormatHandler::bidirectional_transpose_inplace(void* src, int row, int col, size_t element_size) {
    if (src == nullptr) {
        LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Source pointer is null.");
        return;
    }
    if (row <= 0 || col <= 0 || element_size == 0) {
        LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Invalid input parameters (row=" << row
                  << ", col=" << col << ", element_size=" << element_size << ").");
        return;
    }

    // Calculate total data size (consider potential overflow)
    size_t total_elements = (size_t)row * col;
    size_t total_size_bytes = total_elements * element_size;
    // Basic overflow check (if element_size > 0 and total_elements > 0, result should match)
    if (element_size > 0 && total_elements > 0 && total_size_bytes / element_size != total_elements) {
         LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Size calculation overflow detected.");
         return;
    }

    if (total_size_bytes == 0) {
         return; // Nothing to do if size is 0
    }


    if (row == col) {
        // --- Square matrix: Use existing in-place transpose logic ---
        try {
            uint8_t* src_ptr = static_cast<uint8_t*>(src);
            // Temporary buffer (size of one element)
            // std::vector<uint8_t> temp(element_size); // Option using vector
            uint8_t* temp = new uint8_t[element_size];

            for (int i = 0; i < row; ++i) {
                // Iterate through the upper triangle only (excluding diagonal)
                for (int j = i + 1; j < col; ++j)
                {
                    // Calculate offsets for elements (i, j) and (j, i)
                    size_t offset1 = ((size_t)i * col + j) * element_size;
                    size_t offset2 = ((size_t)j * col + i) * element_size;

                    // Bounds check on original pointer access (add if necessary)
                    // if (offset1 + element_size > total_size_bytes || offset2 + element_size > total_size_bytes) { ... }

                    uint8_t* ptr1 = src_ptr + offset1;
                    uint8_t* ptr2 = src_ptr + offset2;

                    // Swap elements
                    memcpy(temp, ptr1, element_size);
                    memcpy(ptr1, ptr2, element_size);
                    memcpy(ptr2, temp, element_size);
                    // If using vector:
                    // memcpy(temp.data(), ptr1, element_size);
                    // memcpy(ptr1, ptr2, element_size);
                    // memcpy(ptr2, temp.data(), element_size);
                }
            }
            delete[] temp; // Free allocated memory
        } catch (const std::bad_alloc& e) {
            LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Failed to allocate temporary buffer: " << e.what());
        } catch (const std::exception& e) {
            LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error during square transpose: " << e.what());
        }
    } else {
        // --- Rectangular matrix: Use temporary buffer ---
        uint8_t* temp_buffer = nullptr;
        try {
            // 1. Allocate temporary buffer (full size)
            temp_buffer = new uint8_t[total_size_bytes];

            // 2. Copy from src to temp_buffer with transpose (Out-of-place transpose)
            uint8_t* src_ptr = static_cast<uint8_t*>(src);
            for (int i = 0; i < row; ++i) { // Iterate through original rows
                for (int j = 0; j < col; ++j) { // Iterate through original columns
                    size_t src_offset = ((size_t)i * col + j) * element_size;
                    // Destination index in temp buffer is transposed (j, i)
                    // Transposed matrix is C x R, so stride is row
                    size_t temp_dst_offset = ((size_t)j * row + i) * element_size;

                    // Bounds check
                    if (src_offset + element_size > total_size_bytes) {
                         throw std::runtime_error("Source read out-of-bounds during temp transpose");
                    }
                    if (temp_dst_offset + element_size > total_size_bytes) {
                         throw std::runtime_error("Temp buffer write out-of-bounds during temp transpose");
                    }

                    memcpy(temp_buffer + temp_dst_offset, src_ptr + src_offset, element_size);
                }
            }

            // 3. Copy the transposed result from temp_buffer back to the original src buffer
            memcpy(src, temp_buffer, total_size_bytes);

            // 4. Free the temporary buffer
            delete[] temp_buffer;
            temp_buffer = nullptr; // Reset pointer (optional)

        } catch (const std::bad_alloc& e) {
            LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error: Failed to allocate temporary buffer for non-square transpose: " << e.what());
            // temp_buffer is already null or delete[] will handle null
            // delete[] temp_buffer; // DO NOT delete here - potential double-free
            return; // Return on error
        } catch (const std::exception& e) {
            LOG_DXRT_ERR("[bidirectional_transpose_inplace] Error during non-square transpose: " << e.what());
            delete[] temp_buffer; // Attempt to free in case it was allocated before exception
            return; // Return on error
        }
    }
}


int NpuFormatHandler::encode_formatted_transposed(
    Bytes& input, Bytes& output, int row, int col, size_t element_size, int unit)
{
     // --- Input Validation ---
     if (input.data == nullptr || output.data == nullptr) {
         LOG_DXRT_ERR("[encode_formatted_transposed] Error: Input or output data buffer is null.");
         return -1;
     }
     if (row <= 0 || col <= 0 || element_size == 0 || unit <= 0) {
         LOG_DXRT_ERR("[encode_formatted_transposed] Error: Invalid input parameters (row=" << row
                   << ", col=" << col << ", element_size=" << element_size << ", unit=" << unit << ").");
         return -1;
     }
      if (input.size != (uint32_t)row * col * element_size) {
         LOG_DXRT_ERR("[encode_formatted_transposed] Error: Input size (" << input.size
                   << ") does not match row * col * element_size (" << (uint32_t)row * col * element_size << ").");
         return -1;
     }
     if (input.data == output.data) {
         LOG_DXRT_ERR("[encode_formatted_transposed] Error: In-place operation (input == output) is not supported for this function.");
         return -1;
     }

     // --- Calculate Output Layout (Based on Transposed + Encoded Structure) ---
     // Dimensions fed into the conceptual 'encode_formatted' step are swapped
     int enc_row = col; // Number of rows for encoding = original columns
     int enc_col = row; // Number of columns for encoding = original rows (this is the 'channel' for encode_formatted)

     int col_group = cdiv(enc_col, unit); // Grouping based on original rows, aligned to unit
     int aligned_enc_col = col_group * unit; // Aligned width in elements (based on original rows)

     // Expected output size in bytes
     uint32_t expected_output_size = (uint32_t)enc_row * aligned_enc_col * element_size;

     // Check and set output buffer size
      if (expected_output_size != output.size) {
         LOG_DXRT_ERR("[encode_formatted_transposed] Warning: Output size is different than expected. "
                   << "Expected size: " << expected_output_size << ", Provided size: " << output.size
                   << ". Output size will be set to expected.");
     }
     output.size = expected_output_size; // Update output size


     // --- Perform Transpose and Encode Simultaneously ---
     uint8_t* dst_data = static_cast<uint8_t*>(output.data);
     const uint8_t* src_data = static_cast<const uint8_t*>(input.data);

     // Zero out the destination buffer first to handle padding correctly
     memset(dst_data, 0, output.size);

     try {
         // Outer loops iterate through the structure of the *output* buffer
         for (int g = 0; g < col_group; ++g) { // Index for groups of 'unit' (originally rows)
             for (int i = 0; i < enc_row; ++i) { // Index within group (originally columns 'j')
                 // Calculate the base offset in the destination buffer for the start of this unit-chunk
                 // Structure is: group * rows_per_group * unit_width + row_index_in_group * unit_width
                 size_t base_dst_offset_elements = (size_t)g * enc_row * unit + (size_t)i * unit;

                 // Determine how many elements to copy in this chunk (handles boundary case)
                 int remaining_elements = enc_col - g * unit; // Remaining original rows
                 int elements_to_copy = (remaining_elements < unit) ? remaining_elements : unit;

                 if (elements_to_copy <= 0) continue; // Should not happen if col_group is calculated correctly

                 // Inner loop copies individual elements from source (transposed) to destination (formatted)
                 for (int k = 0; k < elements_to_copy; ++k) {
                     // Calculate the original matrix indices (orig_row, orig_col)
                     int orig_row = g * unit + k; // Row index in the original input matrix
                     int orig_col = i;           // Column index in the original input matrix

                     // Calculate source offset in input buffer (bytes)
                     size_t src_offset_bytes = ((size_t)orig_row * col + orig_col) * element_size;

                     // Calculate destination offset in output buffer (bytes)
                     size_t dst_offset_bytes = (base_dst_offset_elements + k) * element_size;

                     // Bounds check (essential for safety)
                     if (src_offset_bytes + element_size > input.size || dst_offset_bytes + element_size > output.size) {
                          LOG_DXRT_ERR("[encode_formatted_transposed] Internal Error: Memory access out of bounds.");
                          return -1;
                     }

                     // Copy the element
                     memcpy(dst_data + dst_offset_bytes, src_data + src_offset_bytes, element_size);
                 }
                  // Padding is implicitly handled because the destination buffer was zeroed out,
                  // and we only copy 'elements_to_copy' elements into the allocated 'unit' space.
             }
         }
     } catch (const std::exception& e) {
         // Catch potential memory-related exceptions, although unlikely with memcpy
         LOG_DXRT_ERR("[encode_formatted_transposed] Error during copy: " << e.what());
         return -1;
     }

     return 0; // Success
}

int NpuFormatHandler::decode_aligned_transposed(
    Bytes& input,
    Bytes& output,
    int channel_for_decode,
    deepx_rmapinfo::DataType dtype,
    std::vector<int64_t> shape_encoded,
    int transpose_type)
{
    // --- 1. Input Validation ---
    if (input.data == nullptr) {
        LOG_DXRT_ERR("[decode_aligned_transposed] Error: Input data buffer is null.");
        return -1;
    }
     if (output.data == nullptr && input.size > 0) {
         // Only check output null if output is needed (if input size is 0, output can be 0)
         // It might be safer to forbid null output.data even if output.size is 0.
         LOG_DXRT_ERR("[decode_aligned_transposed] Error: Output data buffer is null.");
         return -1;
     }
    if (channel_for_decode <= 0) {
        LOG_DXRT_ERR("[decode_aligned_transposed] Error: channel_for_decode (" << channel_for_decode << ") must be positive.");
        return -1;
    }
     if (shape_encoded.empty()) {
         LOG_DXRT_ERR("[decode_aligned_transposed] Error: shape_encoded is empty.");
         return -1;
     }
    // Only handle supported transpose types
    if (transpose_type != deepx_rmapinfo::Transpose::CHANNEL_FIRST_TO_LAST &&
        transpose_type != deepx_rmapinfo::Transpose::CHANNEL_LAST_TO_FIRST) {
         LOG_DXRT_ERR("[decode_aligned_transposed] Error: Unsupported transpose_type provided (" << static_cast<int>(transpose_type) << ").");
         return -1;
    }
    if (input.data == output.data && input.size > 0) { // Allow if size is 0
        LOG_DXRT_ERR("[decode_aligned_transposed] Error: In-place operation (input == output) is not supported.");
        return -1;
    }

    // --- 2. Determine Element Size ---
    size_t element_size = dxrt::GetDataSize_rmapinfo_datatype(dtype);

    // --- 3. Calculate Decoding Parameters ---
    int decode_unit_elements = 64; // Base unit (element count)
    if (dtype == deepx_rmapinfo::DataType::FLOAT32) {
        // Use a smaller unit for float if the channel count is below a threshold (e.g., 16)
        if (channel_for_decode < decode_unit_elements) {
            decode_unit_elements = 16;
        }
    }
    // Ensure unit size is at least 1
    if (decode_unit_elements <= 0) decode_unit_elements = 1;

    // Calculate byte-based parameters
    int decode_byte_col = channel_for_decode * element_size;
    int decode_byte_unit = decode_unit_elements * element_size;

    if (decode_byte_unit <= 0) {
         LOG_DXRT_ERR("[decode_aligned_transposed] Error: Calculated byte unit is not positive.");
         return -1;
     }

    int decode_byte_aligned_col = 0;
     try {
         // Calculate the aligned column width in bytes
         decode_byte_aligned_col = cdiv(decode_byte_col, decode_byte_unit) * decode_byte_unit;
     } catch (const std::runtime_error& e) {
         LOG_DXRT_ERR("[decode_aligned_transposed] Error during cdiv calculation: " << e.what());
         return -1;
     }


    if (decode_byte_aligned_col <= 0) {
        // Allow if input size is 0 (result will also be 0)
        if (input.size == 0) {
             output.size = 0;
             return 0;
        }
        LOG_DXRT_ERR("[decode_aligned_transposed] Error: Calculated aligned column size is not positive ("<< decode_byte_aligned_col <<").");
        return -1;
    }

    // Validate input size
    if (input.size == 0) {
        output.size = 0;
        return 0; // Handle empty input processed
    }
    if (input.size % decode_byte_aligned_col != 0) {
        LOG_DXRT_ERR("[decode_aligned_transposed] Error: Input size (" << input.size
                  << ") is not a multiple of aligned byte column size (" << decode_byte_aligned_col << ")");
        return -1;
    }

    // Calculate the number of rows to be decoded
    uint64_t decoded_rows = input.size / decode_byte_aligned_col; // Use uint64_t for intermediate calculation

    // --- 4. Calculate Transpose Parameters ---
    size_t shape_dims = shape_encoded.size();
    uint64_t transpose_row = 0; // Number of rows in the final output matrix
    uint64_t transpose_col = 0; // Number of columns in the final output matrix

    // Lambda function for calculating product safely using uint64_t to prevent overflow
    auto calculate_product = [&](size_t start_idx, size_t end_idx) -> uint64_t {
        if (start_idx >= end_idx || start_idx >= shape_dims) {
            return 1ULL; // Product of an empty range is 1
        }
         // Adjust end_idx if it exceeds bounds
         if (end_idx > shape_dims) {
             end_idx = shape_dims;
         }

        uint64_t product = 1ULL;
        for (size_t k = start_idx; k < end_idx; ++k) {
            if (shape_encoded[k] < 0) {
                throw std::runtime_error("Negative dimension encountered in shape_encoded");
            }
            uint64_t dim_val = static_cast<uint64_t>(shape_encoded[k]);
            if (dim_val == 0) return 0ULL; // If any dimension is 0, the total product is 0

            // Overflow check (is product * dim_val > UINT64_MAX ?)
            // Check only if product is already large enough to potentially overflow
            if (product > UINT64_MAX / dim_val) {
                throw std::overflow_error("Overflow detected during dimension product calculation");
            }
            product *= dim_val;
        }
        return product;
    };

    try {
        // Calculate output dimensions based on transpose type and shape_encoded
        if (transpose_type == deepx_rmapinfo::Transpose::CHANNEL_FIRST_TO_LAST) {
            if (shape_dims < 1) throw std::runtime_error("shape_dims must be >= 1 for CHANNEL_FIRST_TO_LAST");
            transpose_row = static_cast<uint64_t>(shape_encoded[0]); // First dimension becomes rows
            transpose_col = calculate_product(1, shape_dims);        // Product of the rest becomes columns
             if (shape_encoded[0] < 0) throw std::runtime_error("Negative dimension in shape_encoded[0]");
        } else if (transpose_type == deepx_rmapinfo::Transpose::CHANNEL_LAST_TO_FIRST) {
            if (shape_dims < 1) throw std::runtime_error("shape_dims must be >= 1 for CHANNEL_LAST_TO_FIRST");
            transpose_col = static_cast<uint64_t>(shape_encoded[shape_dims - 1]); // Last dimension becomes columns
            transpose_row = calculate_product(0, shape_dims - 1);         // Product of the preceding dimensions becomes rows
             if (shape_encoded[shape_dims - 1] < 0) throw std::runtime_error("Negative dimension in shape_encoded[last]");
        }
         // Handle cases where calculated dimensions might be zero
         if (transpose_row == 0 || transpose_col == 0) {
              // Allow if both are zero (0-size tensor)
              if (transpose_row != 0 || transpose_col != 0) {
                   // If only one is zero, it might indicate an error in shape definition
                   LOG_DXRT_ERR("[decode_aligned_transposed] Warning: Calculated transpose dimension is zero ("
                             << transpose_row << "x" << transpose_col << "). Check shape_encoded.");
                   // Can proceed treating as 0-size tensor or return error
              }
         }
    } catch (const std::exception& e) {
        LOG_DXRT_ERR("[decode_aligned_transposed] Error calculating transpose dimensions: " << e.what());
        return -1;
    }


    // --- 5. Consistency Check ---
    uint64_t total_elements_decoded = decoded_rows * static_cast<uint64_t>(channel_for_decode);
    uint64_t total_elements_transposed = transpose_row * transpose_col;

    if (total_elements_decoded != total_elements_transposed) {
        LOG_DXRT_ERR("[decode_aligned_transposed] Error: Mismatch in total elements. Decoded: "
                  << total_elements_decoded << " (" << decoded_rows << "*" << channel_for_decode
                  << "), Transposed: " << total_elements_transposed << " ("
                  << transpose_row << "*" << transpose_col << "). Check parameters.");
        return -1;
    }

    // --- 6. Calculate Output Buffer Size and Check ---
    uint64_t expected_output_size_64 = total_elements_transposed * element_size;
    // Check for overflow since output.size is uint32_t
    if (expected_output_size_64 > UINT32_MAX) {
        LOG_DXRT_ERR("[decode_aligned_transposed] Error: Calculated output size exceeds UINT32_MAX.");
        return -1;
    }
    uint32_t expected_output_size = static_cast<uint32_t>(expected_output_size_64);

    // Handle output buffer null check and size mismatch
     if (output.data == nullptr && expected_output_size > 0) {
          LOG_DXRT_ERR("[decode_aligned_transposed] Error: Output data buffer is null for non-zero expected size.");
          return -1;
     }
    if (output.size != expected_output_size) {
         // Warn about size mismatch but proceed, assuming caller allocated enough space. Set the correct size.
         LOG_DXRT_ERR("[decode_aligned_transposed] Warning: Output buffer size mismatch. Provided: "
                   << output.size << ", Expected: " << expected_output_size
                   << ". Using expected size for operation, ensure buffer is large enough.");
         output.size = expected_output_size;
    }
    // If size is 0, no work needed.
    if (expected_output_size == 0) {
        return 0;
    }

    // --- 7. Initialize Output Buffer (Optional but recommended) ---
    // This ensures any padding areas or potentially unwritten elements are zero.
    // memset(output.data, 0, output.size);

    // --- 8. Core Loop: Perform Decode and Transpose Simultaneously ---
    uint8_t* dst_data = static_cast<uint8_t*>(output.data);
    const uint8_t* src_data = static_cast<const uint8_t*>(input.data);

    // Define source (decoded) and destination (transposed) logical dimensions for clarity
    uint64_t src_logical_rows = decoded_rows;
    uint64_t src_logical_cols = static_cast<uint64_t>(channel_for_decode);
    //uint64_t dst_logical_rows = transpose_row;
    uint64_t dst_logical_cols = transpose_col;

    try {
        // Iterate based on the indices (i, j) of the conceptually decoded matrix
        for (uint64_t i = 0; i < src_logical_rows; ++i) {       // Index for decoded rows
            for (uint64_t j = 0; j < src_logical_cols; ++j) {   // Index for decoded columns (channels)

                // 1. Calculate Source Offset (within the encoded input buffer)
                // Start of the i-th aligned row + offset for the j-th element
                size_t src_offset_bytes = (size_t)i * decode_byte_aligned_col + (size_t)j * element_size;

                // 2. Calculate Transposed Coordinates (based on standard matrix transpose)
                // Source logical coordinate (r, c) = (i, j) -> Transposed logical coordinate (r_t, c_t) = (j, i)
                uint64_t r_t = j; // Transposed row index = original column index
                uint64_t c_t = i; // Transposed column index = original row index

                // 3. Calculate Destination Offset (within the transposed output buffer)
                // Output buffer layout is dst_logical_rows x dst_logical_cols
                size_t dst_offset_bytes = (r_t * dst_logical_cols + c_t) * element_size;

                // 4. Boundary Checks (Crucial for safety)
                if (src_offset_bytes + element_size > input.size) {
                     LOG_DXRT_ERR("[decode_aligned_transposed] Internal Error: Source read out of bounds. "
                               << "Offset: " << src_offset_bytes << ", ElementSize: " << element_size << ", InputSize: " << input.size);
                     return -1;
                }
                if (dst_offset_bytes + element_size > output.size) {
                     LOG_DXRT_ERR("[decode_aligned_transposed] Internal Error: Destination write out of bounds. "
                               << "Offset: " << dst_offset_bytes << ", ElementSize: " << element_size << ", OutputSize: " << output.size);
                     return -1;
                }

                // 5. Copy Element
                memcpy(dst_data + dst_offset_bytes, src_data + src_offset_bytes, element_size);
            }
        }
    } catch (const std::exception& e) {
        // Catch potential exceptions during the copy loop (e.g., memory access errors)
        LOG_DXRT_ERR("[decode_aligned_transposed] Error during copy loop: " << e.what());
        return -1;
    }

    // --- 9. Return Success ---
    return 0;
}

} // namespace npu_format_handler