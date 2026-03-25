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

namespace dxrt {

/**
 * @brief Parser for DXNN V6 format
 * 
 * This parser handles the legacy V6 format by converting it to V7 format
 * internally. It implements the conversion logic from v6_converter.py
 * in pure C++.
 */
class V6ModelParser : public IModelParser {
public:
    V6ModelParser() = default;
    ~V6ModelParser() override = default;

    int GetSupportedVersion() const override { return 6; }
    std::string GetParserName() const override { return "DXNN V6 Parser"; }

protected:
    // Override virtual methods from IModelParser
    /**
     * @brief Load binary information from V6 model file
     * @param param Output parameter for binary info
     * @param buffer File buffer
     * @param fileSize Size of the file
     * @return File format version
     */
    int loadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, const char* buffer, int fileSize) const override;

    /**
     * @brief Parse compiled data for a single NPU from V6 format
     * @param npuData RapidJSON value containing data for one NPU
     * @param npuName Name of the NPU
     * @param param Output parameter for binary info
     */
    void parseCompiledDataForNpu(const rapidjson::Value& npuData, const std::string& npuName, deepx_binaryinfo::BinaryInfoDatabase& param) const;

    /**
     * @brief Convert V6 graph info to V7 format
     * @param v6GraphInfo V6 graph info JSON string
     * @return V7 graph info JSON string
     */
    std::string ConvertGraphInfoV6ToV7(const std::string& v6GraphInfo) const;

    /**
     * @brief Convert V6 rmap info to V7 format
     * @param v6RmapInfo V6 rmap info JSON string
     * @param v6GraphInfo V6 graph info JSON string (for input name lookup)
     * @return V7 rmap info JSON string
     */
    std::string ConvertRmapInfoV6ToV7(const std::string& v6RmapInfo, const std::string& v6GraphInfo) const;

    /**
     * @brief Load graph information from converted V7 format
     * @param param Output parameter for graph info
     * @param data Model data containing binary info
     * @return 0 on success, -1 on failure
     */
    int loadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, ModelDataBase& data) const override;

    /**
     * @brief Load rmap information from converted V7 format
     * @param param Output parameter for rmap info
     * @param data Model data containing binary info
     * @return Compile type string
     */
    std::string loadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, ModelDataBase& data) const override;

    /**
     * @brief Convert V6 format to V7 format before parsing
     * 
     * This method:
     * 1. Extracts original V6 graph_info and rmap_info
     * 2. Converts them to V7 format
     * 3. Replaces the data in modelData.deepx_binary
     * 
     * @param modelData Model data to transform (modified in-place)
     */
    void PreProcessModel(ModelDataBase& modelData) const override;

private:

    /**
     * @brief Parse V6 graph info structure
     * @param v6GraphInfo V6 graph info JSON document
     * @return V7 graph info JSON string
     */
    std::string ParseV6GraphInfo(const rapidjson::Document& v6GraphInfo) const;

    /**
     * @brief Parse V6 rmap info structure
     * @param v6RmapInfo V6 rmap info JSON document
     * @param v6GraphInfo V6 graph info JSON document
     * @return V7 rmap info JSON string
     */
    std::string ParseV6RmapInfo(const rapidjson::Document& v6RmapInfo, const rapidjson::Document& v6GraphInfo) const;

    /**
     * @brief Extract input tensor name from V6 graph info
     * @param v6GraphInfo V6 graph info JSON document
     * @return Input tensor name
     */
    std::string ExtractInputNameFromV6Graph(const rapidjson::Document& v6GraphInfo) const;

    /**
     * @brief Extract input tensor shape from V6 graph info
     * @param v6GraphInfo V6 graph info JSON document
     * @param allocator JSON allocator for creating new values
     * @return Input tensor shape as JSON array
     */
    rapidjson::Value ExtractInputShapeFromV6Graph(const rapidjson::Document& v6GraphInfo, rapidjson::Document::AllocatorType& allocator) const; 

    /**
     * @brief Extract output tensor shape from V6 graph info
     * @param v6GraphInfo V6 graph info JSON document
     * @param outputName Name of the output tensor
     * @param allocator JSON allocator for creating new values
     * @return Output tensor shape as JSON array
     */
    rapidjson::Value ExtractOutputShapeFromV6Graph(const rapidjson::Document& v6GraphInfo, const std::string& outputName, rapidjson::Document::AllocatorType& allocator) const;

    /**
     * @brief Parse V6 version string to extract components
     * @param versionStr V6 version string (e.g., "1.0.0(opt_level)")
     * @return Pair of version and optimization level
     */
    std::pair<std::string, std::string> ParseV6Version(const std::string& versionStr) const;
};

} // namespace dxrt 