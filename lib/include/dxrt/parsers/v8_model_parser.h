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
#include "dxrt/extern/rapidjson/document.h"
#include <string>

namespace dxrt {

/**
 * @brief Parser for DXNN v8 format files
 * 
 * V8 format adds PPU (Post-Processing Unit) binary support for PPCPU model type.
 * This parser handles:
 * - All v7 features (rmap, weight, rmap_info, bitmatch)
 * - New PPU binary field in compiled_data (optional)
 * - PPCPU model type detection
 */
class V8ModelParser : public IModelParser {
public:
    V8ModelParser() = default;
    ~V8ModelParser() override = default;
    
    /**
     * @brief Parse v8 DXNN file
     * @param filePath Path to .dxnn file
     * @param modelData Output structure to populate
     * @return Model compile type string
     */
    std::string ParseModel(const std::string& filePath, ModelDataBase& modelData) override;
    
    /**
     * @brief Get supported version number
     * @return 8
     */
    int GetSupportedVersion() const override { return 8; }
    
    /**
     * @brief Get parser name
     * @return "V8ModelParser"
     */
    std::string GetParserName() const override { return "V8ModelParser"; }

private:
    /**
     * @brief Load binary info from DXNN header (including PPU)
     * @param param Output binary database
     * @param buffer File buffer
     * @param fileSize File size in bytes
     * @return DXNN file format version
     */
    int LoadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, char *buffer, int fileSize);
    
    /**
     * @brief Load graph info from parsed binary data
     * @param param Output graph database
     * @param data Model data containing binary info
     * @return 0 on success, -1 on error
     */
    int LoadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, ModelDataBase& data);
    
    /**
     * @brief Load rmap info from parsed binary data
     * @param param Output rmap database
     * @param data Model data containing binary info
     * @return Model compile type string
     */
    std::string LoadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, ModelDataBase& data);
};

}  // namespace dxrt
