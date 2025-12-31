/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/model_parser.h"
#include "dxrt/model.h"

namespace dxrt {

/**
 * @brief Parser for DXNN V7 format
 * 
 * This parser handles the current V7 format which is the standard format
 * used by the inference engine. It wraps the existing parsing logic
 * from model.cpp.
 */
class V7ModelParser : public IModelParser {
public:
    V7ModelParser() = default;
    ~V7ModelParser() override = default;
    
    std::string ParseModel(const std::string& filePath, ModelDataBase& modelData) override;
    std::string ParseModel(const uint8_t* modelBuffer, size_t modelSize, ModelDataBase& modelData) override;
    int GetSupportedVersion() const override { return 7; }
    std::string GetParserName() const override { return "DXNN V7 Parser"; }

private:
    /**
     * @brief Load binary information from the model file
     * @param param Output parameter for binary info
     * @param buffer File buffer
     * @param fileSize Size of the file
     * @return File format version
     */
    int LoadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, char* buffer, int fileSize);
    
    /**
     * @brief Load graph information from the model data
     * @param param Output parameter for graph info
     * @param data Model data containing binary info
     * @return 0 on success, -1 on failure
     */
    int LoadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, ModelDataBase& data);
    
    /**
     * @brief Load rmap information from the model data
     * @param param Output parameter for rmap info
     * @param data Model data containing binary info
     * @return Compile type string
     */
    std::string LoadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, ModelDataBase& data);
};

} // namespace dxrt 