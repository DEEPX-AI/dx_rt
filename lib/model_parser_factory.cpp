/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
 
#include "dxrt/model_parser.h"
#include "dxrt/parsers/v6_model_parser.h"
#include "dxrt/parsers/v7_model_parser.h"
#include "dxrt/exception/exception.h"
#include "dxrt/filesys_support.h"
#include "resource/log_messages.h"
#include <fstream>
#include <unordered_map>

namespace dxrt {

std::unique_ptr<IModelParser> ModelParserFactory::CreateParser(const std::string& filePath) {
    int version = GetFileFormatVersion(filePath);
    return CreateParser(version);
}

std::unique_ptr<IModelParser> ModelParserFactory::CreateParser(int version) {
    switch (version) {
        case 6:
            return std::make_unique<V6ModelParser>();
        case 7:
            return std::make_unique<V7ModelParser>();
        default:
            throw InvalidModelException(EXCEPTION_MESSAGE(
                LogMessages::NotSupported_ModelFileFormatVersion(version, MIN_SINGLEFILE_VERSION)
            ));
    }
}

int ModelParserFactory::GetFileFormatVersion(const std::string& filePath) {
    if (!fileExists(filePath)) {
        throw FileNotFoundException(EXCEPTION_MESSAGE("Invalid model path : " + filePath));
    }
    
    if (getExtension(filePath) != "dxnn") {
        throw InvalidModelException(EXCEPTION_MESSAGE("Invalid model path : " + filePath));
    }
    
    // DXNN header: "DXNN" (4 bytes) + 4-byte little-endian int version
    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs) {
        throw FileNotFoundException(EXCEPTION_MESSAGE("Invalid model path : " + filePath));
    }
    
    char header[8] = {0};
    ifs.read(header, 8);
    if (ifs.gcount() != 8) {
        throw ModelParsingException(EXCEPTION_MESSAGE("Failed to read DXNN header: " + filePath));
    }
    
    if (std::string(header, 4) != "DXNN") {
        throw InvalidModelException(EXCEPTION_MESSAGE(LogMessages::InvalidDXNNFileFormat()));
    }
    
    int32_t version = static_cast<int32_t>(static_cast<unsigned char>(header[4]) |
                                           static_cast<unsigned char>(header[5]) << 8 |
                                           static_cast<unsigned char>(header[6]) << 16 |
                                           static_cast<unsigned char>(header[7]) << 24);
    return version;
}

bool ModelParserFactory::IsVersionSupported(int version) {
    return version >= MIN_SINGLEFILE_VERSION && version <= MAX_SINGLEFILE_VERSION;
}

std::vector<int> ModelParserFactory::GetSupportedVersions() {
    return {6, 7}; // Currently supported versions
}

} // namespace dxrt 