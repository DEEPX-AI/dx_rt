/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/ppu_binary_parser.h"
#include "dxrt/common.h"
#include <cstring>
#include <stdexcept>

namespace dxrt {

uint32_t GetPpuBoxDataSize(DataType dataType)
{
    switch (dataType) {
        case DataType::BBOX:
            return sizeof(DeviceBoundingBox_t);  // 32 bytes
        case DataType::FACE:
            return sizeof(DeviceFace_t);          // 64 bytes
        case DataType::POSE:
            return sizeof(DevicePose_t);          // 256 bytes
        default:
            LOG_DXRT_ERR("Invalid PPU data type: " << static_cast<int>(dataType));
            return 0;
    }
}

PpuOutputSizeInfo CalculatePpuOutputSize(const std::vector<uint8_t>& ppuBinaryData, 
                                         DataType outputDataType)
{
    PpuOutputSizeInfo sizeInfo = {0, 0, 0};
    
    // Validate input buffer size
    if (ppuBinaryData.size() < sizeof(ppu_info_header_t)) {
        LOG_DXRT_ERR("PPU binary data too small: " << ppuBinaryData.size() 
                     << " bytes (minimum: " << sizeof(ppu_info_header_t) << ")");
        return sizeInfo;
    }
    
    // Parse PPU header
    const ppu_info_header_t* header = reinterpret_cast<const ppu_info_header_t*>(ppuBinaryData.data());
    
    LOG_DXRT_DBG << "[PPU Parser] Header - version: " << static_cast<int>(header->version)
                 << ", tensor_num: " << static_cast<int>(header->tensor_num)
                 << ", size: " << static_cast<int>(header->size)
                 << ", checksum: " << static_cast<int>(header->checksum) << std::endl;
    
    // Validate tensor count
    if (header->tensor_num == 0) {
        LOG_DXRT_WARN("PPU binary has no tensors");
        return sizeInfo;
    }
    
    // Validate buffer size for all tensors
    size_t requiredSize = sizeof(ppu_info_header_t) + (header->tensor_num * sizeof(ppu_info_t));
    if (ppuBinaryData.size() < requiredSize) {
        LOG_DXRT_ERR("PPU binary data insufficient for " << static_cast<int>(header->tensor_num) 
                     << " tensors. Required: " << requiredSize 
                     << " bytes, Available: " << ppuBinaryData.size() << " bytes");
        return sizeInfo;
    }
    
    // Parse PPU tensor configurations
    const ppu_info_t* tensors = reinterpret_cast<const ppu_info_t*>(
        ppuBinaryData.data() + sizeof(ppu_info_header_t)
    );
    
    // Calculate total maximum box count by accumulating grid dimensions
    uint32_t totalMaxBoxCount = 0;
    
    for (int idx = 0; idx < header->tensor_num; idx++) {
        const ppu_info_t* tensor = &tensors[idx];
        
        // Calculate maximum boxes for this tensor (grid_width * grid_height)
        uint32_t tensorBoxCount = static_cast<uint32_t>(tensor->_PPU_GRID_WIDTH) * 
                                  static_cast<uint32_t>(tensor->_PPU_GRID_HEIGHT);
        
        totalMaxBoxCount += tensorBoxCount;
        
        LOG_DXRT_DBG << "[PPU Parser] Tensor " << idx 
                     << ": grid_w=" << static_cast<int>(tensor->_PPU_GRID_WIDTH)
                     << ", grid_h=" << static_cast<int>(tensor->_PPU_GRID_HEIGHT)
                     << ", boxes=" << tensorBoxCount
                     << " (accumulated: " << totalMaxBoxCount << ")" << std::endl;
    }
    
    // Get box data size based on output data type
    uint32_t boxDataSize = GetPpuBoxDataSize(outputDataType);
    if (boxDataSize == 0) {
        LOG_DXRT_ERR("Invalid box data size for data type: " << static_cast<int>(outputDataType));
        return sizeInfo;
    }
    
    // Calculate total output size
    uint32_t totalOutputSize = totalMaxBoxCount * boxDataSize;
    
    sizeInfo.max_box_count = totalMaxBoxCount;
    sizeInfo.box_data_size = boxDataSize;
    sizeInfo.total_output_size = totalOutputSize;
    
    LOG_DXRT_DBG << "[PPU Parser] Result - max_boxes: " << totalMaxBoxCount
                 << ", box_size: " << boxDataSize << " bytes"
                 << ", total_output_size: " << totalOutputSize << " bytes" << std::endl;
    
    return sizeInfo;
}

} // namespace dxrt
