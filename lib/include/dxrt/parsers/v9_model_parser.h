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
 * @brief Parser for DXNN v9 format files
 * 
 * V9 format adds PPU (Post-Processing Unit) binary support for PPCPU model type.
 * This parser handles:
 * - All v9 features (rmap, weight, rmap_info, bitmatch)
 * - New PPU binary field in compiled_data (optional)
 * - PPCPU model type detection
 */
class V9ModelParser : public IModelParser {
public:
    V9ModelParser() = default;
    ~V9ModelParser() override = default;

    int GetSupportedVersion() const override { return MIN_COMPATIBILITY_V9_FMT_VERSION; }
    std::string GetParserName() const override { return "V9ModelParser"; }

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

private:
    /*
        V9 File Format specificaions
    */
    static constexpr size_t     PRELUDE_SIZE = 32;  // Size of the prelude section in bytes
    static constexpr size_t     PRELUDE_SIGNATURE_LEN = 4;  // 4 bytes
    static constexpr size_t     PRELUDE_RESERVED_LEN = 8;  // 8 bytes
    static constexpr uint32_t   PRELUDE_FLAG_EXPECTED_VAL = 0;
    static constexpr std::array<uint8_t, PRELUDE_RESERVED_LEN> PRELUDE_RESERVED_EXPECTED_VAL = {0};
    static constexpr uint64_t   MAX_JSON_HEADER_SIZE = 16 * 1024 * 1024;  // 16 MiB
    static constexpr int32_t    MIN_COMPATIBILITY_V9_FMT_VERSION = 9;  // Minimum DXNN file format version for V9 parser
    static constexpr int32_t    MAX_COMPATIBILITY_V9_FMT_VERSION = 9;  // Maximum DXNN file format version for V9 parser

};

}  // namespace dxrt
