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
#include "dxrt/parsers/parser_common_utils.h"
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

    int GetSupportedVersion() const override { return 8; }
    std::string GetParserName() const override { return "V8ModelParser"; }

protected:
    // Override virtual methods from IModelParser
    /**
     * @brief Load binary info from DXNN header (including PPU)
     * @param param Output binary database
     * @param buffer File buffer
     * @param fileSize File size in bytes
     * @return DXNN file format version
     */
    int loadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, const char *buffer, int fileSize) const override;

    /**
     * @brief Load graph info from parsed binary data
     * @param param Output graph database
     * @param data Model data containing binary info
     * @return 0 on success, -1 on error
     */
    int loadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, ModelDataBase& data) const override;

    /**
     * @brief Load rmap info from parsed binary data
     * @param param Output rmap database
     * @param data Model data containing binary info
     * @return Model compile type string
     */
    std::string loadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, ModelDataBase& data) const override;
};

}  // namespace dxrt
