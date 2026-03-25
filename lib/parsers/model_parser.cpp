/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/model_parser.h"
#include "dxrt/filesys_support.h"
#include "dxrt/exception/exception.h"
#include "../resource/log_messages.h"
#include <fstream>
#include <vector>

namespace dxrt {

// ============================================================================
// IModelParser - Base Class Implementation (Template Method Pattern)
// ============================================================================

std::string IModelParser::ParseModel(const std::string& filePath, ModelDataBase& modelData) const{
    // Validate file exists and has correct extension
    if (!fileExists(filePath) || getExtension(filePath) != "dxnn") {
        throw FileNotFoundException(EXCEPTION_MESSAGE("Invalid model path : " + filePath));
    }

    // Read file into memory
    uint64_t fileSize = getFileSize(filePath);
    std::vector<char> vbuf(fileSize);
    char* buf = vbuf.data();

    FILE* fp = fopen(filePath.c_str(), "rb");
    if (!fp) {
        throw FileNotFoundException(EXCEPTION_MESSAGE("Failed to open file: " + filePath));
    }

    std::ignore = fread(static_cast<void*>(buf), fileSize, 1, fp);
    fclose(fp);

    // Delegate to buffer-based parsing
    return ParseModel((const uint8_t*)buf, fileSize, modelData);
}

std::string IModelParser::ParseModel(const uint8_t* modelBuffer, size_t modelSize, ModelDataBase& modelData) const {
    // Step 1: Load binary info (header parsing)
    loadBinaryInfo(modelData.deepx_binary, (const char*)modelBuffer, static_cast<int>(modelSize));
    
    // Step 2: Pre-process (version-specific transformations, e.g., V6→V7 conversion)
    PreProcessModel(modelData);
    
    // Step 3: Load graph info (model structure)
    loadGraphInfo(modelData.deepx_graph, modelData);
    
    // Step 4: Load rmap info (register mapping) and return compile type
    return loadRmapInfo(modelData.deepx_rmap, modelData);
}

} // namespace dxrt
