/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifndef DXRT_PARSER_COMMON_UTILS_H
#define DXRT_PARSER_COMMON_UTILS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/rapidjson.h"

namespace dxrt {
namespace parser_common {

// Basic Utility Functions
/**
 * @brief Parse int64 from rapidjson Value supporting multiple types (Int64, Int, String)
 */
int64_t parseInt64FromValue(const rapidjson::Value& value);

/**
 * @brief Parse string array from JSON array and populate vector
 */
void parseStringArray(const rapidjson::Value& jsonArray, std::vector<std::string>& targetVector);

// Models and BinaryInfo Related Functions
/**
 * @brief Parse offset and size fields from JSON object to Models struct
 */
void parseModelsOffsetSize(const rapidjson::Value& obj, deepx_binaryinfo::Models& model);

/**
 * @brief Copy binary data from buffer to Models vector based on offset/size
 */
void copyBinaryDataToModels(std::vector<deepx_binaryinfo::Models>& models, const char* buffer, int offset);

/**
 * @brief Copy string data from buffer to Models vector
 */
void copyStringDataToModels(std::vector<deepx_binaryinfo::Models>& models, const char* buffer, int offset);

/**
 * @brief Parse cpu_models section from JSON and populate BinaryInfoDatabase
 */
void parseCpuModels(const rapidjson::Value& cpuModelsObj, deepx_binaryinfo::BinaryInfoDatabase& param);

/**
 * @brief Parse checkpoints array into Counts._checkpoints
 */
void parseCheckpoints(const rapidjson::Value& listObj, deepx_rmapinfo::Counts& counts);

/**
 * @brief Parse Memory object from JSON
 */
deepx_rmapinfo::Memory parseMemoryObject(const rapidjson::Value& memObj);

/**
 * @brief Assign memory to model_memory based on type (RMAP, WEIGHT, INPUT, OUTPUT, TEMP)
 */
void assignMemoryToModelMemory(deepx_rmapinfo::Memory& memory, 
                                deepx_rmapinfo::RegisterInfoDatabase& regMap,
                                size_t taskBufferCount);

// GraphInfo Related Functions
/**
 * @brief Parse GraphInfo Tensor fields (name, owner, users) from JSON
 */
void parseGraphInfoTensor(const rapidjson::Value& tensorObj, deepx_graphinfo::Tensor& tensor);

/**
 * @brief Parse tensor array and populate vector
 */
void parseTensorArray(const rapidjson::Value& tensorArray, std::vector<deepx_graphinfo::Tensor>& tensors);

} // namespace parser_common
} // namespace dxrt

#endif // DXRT_PARSER_COMMON_UTILS_H
