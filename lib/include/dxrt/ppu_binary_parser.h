/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdint>
#include <vector>
#include "dxrt/datatype.h"

namespace dxrt {

/**
 * @brief PPU binary header structure
 * 
 * This structure represents the header of PPU binary file.
 * The header contains metadata about the number of tensors and format information.
 */
#pragma pack(push, 1)
typedef struct {
    uint8_t version;      ///< PPU binary format version
    uint8_t tensor_num;   ///< Number of PPU tensor configurations
    uint8_t size;         ///< Size of each ppu_info_t structure
    uint8_t checksum;     ///< Checksum for validation
} ppu_info_header_t;

/**
 * @brief PPU tensor configuration structure
 * 
 * This structure contains configuration parameters for a single PPU tensor,
 * including grid dimensions, filtering parameters, and data format information.
 */
typedef struct {
    uint16_t _PPU_OUT_FEATURE_CHANNEL;  ///< Output feature channel count
    uint16_t _PPU_ARG_CLASS_NUM;        ///< Number of classes for argmax
    float    _PPU_FILTER_THR;           ///< Filtering threshold
    uint8_t  _PPU_CONV_NUM;             ///< Convolution number
    uint8_t  _PPU_ANCHOR_NUM;           ///< Number of anchors
    uint8_t  _PPU_FILTER_MODE;          ///< Filter mode
    uint8_t  _PPU_LABEL_ON;             ///< Label enable flag
    uint8_t  _PPU_DATA_SIZE;            ///< Data size per element
    uint8_t  _PPU_GRID_WIDTH;           ///< Grid width
    uint8_t  _PPU_GRID_HEIGHT;          ///< Grid height
    uint8_t  reserved;                  ///< Reserved for alignment
} ppu_info_t;
#pragma pack(pop)

/**
 * @brief PPU output size information
 * 
 * This structure contains the calculated maximum output size for PPU processing.
 */
struct PpuOutputSizeInfo {
    uint32_t max_box_count;      ///< Maximum number of boxes (sum of all grid_w * grid_h)
    uint32_t box_data_size;      ///< Size of data for one box (based on DataType)
    uint32_t total_output_size;  ///< Total output size (max_box_count * box_data_size)
};

/**
 * @brief Parse PPU binary to calculate optimal output size
 * 
 * This function parses the PPU binary data to determine the maximum number of
 * candidate boxes and calculates the optimal output buffer size.
 * 
 * @param ppuBinaryData The PPU binary data buffer
 * @param outputDataType The output data type (BBOX, FACE, or POSE)
 * @return PpuOutputSizeInfo containing size information, or nullopt if parsing fails
 */
PpuOutputSizeInfo CalculatePpuOutputSize(const std::vector<uint8_t>& ppuBinaryData, 
                                         DataType outputDataType);

/**
 * @brief Get the size of one box data based on DataType
 * 
 * @param dataType The output data type (BBOX, FACE, or POSE)
 * @return Size in bytes for one box
 */
uint32_t GetPpuBoxDataSize(DataType dataType);

} // namespace dxrt
