/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/util.h"
#include "dxrt/parsers/parser_common_utils.h"
#include "../resource/log_messages.h"

#include <algorithm>
#include <cstring>

using rapidjson::Value;
using rapidjson::SizeType;
using std::string;

namespace dxrt {
namespace parser_common {

constexpr size_t MAX_CHECKPOINT_COUNT = 3;

// ============================================================================
// Basic Utility Functions
// ============================================================================

    // Helper: Parse int64 from Value with multiple type support
    int64_t parseInt64FromValue(const Value& value) 
    {
        if (value.IsInt64()) 
        {
            return value.GetInt64();
        }
        if (value.IsInt()) 
        {
            return value.GetInt();
        }
        if (value.IsString()) 
        {
            return std::stoll(value.GetString());
        }
        return 0;
    }

    // Helper: Parse string array from JSON and populate vector
    void parseStringArray(const Value& jsonArray, std::vector<std::string>& targetVector)
    {
        targetVector.clear();
        for (SizeType i = 0; i < jsonArray.Size(); i++) 
        {
            if (jsonArray[i].IsString())
            {
                targetVector.emplace_back(jsonArray[i].GetString());
            }
        }
    }

    // ============================================================================
    // Models and BinaryInfo Related Functions
    // ============================================================================

    // Helper: Parse offset and size from JSON object to Models struct
    void parseModelsOffsetSize(const Value& obj, deepx_binaryinfo::Models& model)
    {
        if (obj.HasMember("offset")) 
        {
            model.offset() = parseInt64FromValue(obj["offset"]);
        }
        if (obj.HasMember("size")) 
        {
            model.size() = parseInt64FromValue(obj["size"]);
        }
    }

    // Helper: Copy binary data from buffer to Models vector
    void copyBinaryDataToModels(std::vector<deepx_binaryinfo::Models>& models, const char* buffer, int offset)
    {
        std::for_each(models.begin(), models.end(), [buffer, offset](auto& model) {
            model._buffer.resize(model.size());
            if (model.size() > 0)
            {
                memcpy(model._buffer.data(), buffer + (offset + model.offset()), model.size());
            }
        });
    }

    // Helper: Copy string data from buffer to Models vector
    void copyStringDataToModels(std::vector<deepx_binaryinfo::Models>& models, const char* buffer, int offset)
    {
        std::for_each(models.begin(), models.end(), [buffer, offset](auto& model) {
            const char* src = buffer + offset + model.offset();
            model.str() = string(src, model.size());
        }); 
    }

    // Helper: Parse cpu_models from JSON object
    void parseCpuModels(const Value& cpuModelsObj, deepx_binaryinfo::BinaryInfoDatabase& param)
    {
        for (Value::ConstMemberIterator iter = cpuModelsObj.MemberBegin(); iter != cpuModelsObj.MemberEnd(); ++iter) 
        {
            if (!iter->name.IsString()) 
            {
                continue;
            }
            
            deepx_binaryinfo::Models model;
            model.name() = iter->name.GetString();
            parseModelsOffsetSize(iter->value, model);
            param.cpu_models().push_back(model);
        }
    }

    void parseCheckpoints(const Value& listObj, deepx_rmapinfo::Counts& counts)
    {
        counts._op_mode = 1;
        for (SizeType j = 0; j < MAX_CHECKPOINT_COUNT; j++) 
        {
            if (j >= listObj.Size()) 
            {
                break;
            }
            counts._checkpoints[j] = static_cast<uint32_t>(listObj[j].GetUint64());
        }
    }

    deepx_rmapinfo::Memory parseMemoryObject(const Value& memObj)
    {
        deepx_rmapinfo::Memory memory;
        
        if (memObj.HasMember("name") && memObj["name"].IsString())
        {
            memory.name() = memObj["name"].GetString();
        }

        if (memObj.HasMember("offset") && memObj["offset"].IsInt64()) 
        {
            memory.offset() = memObj["offset"].GetInt64();
            if (memory.offset() != 0 && memory.name() != "TEMP")
            {
                LOG_DXRT_ERR(LogMessages::ModelParser_OutputOffsetIsNotZero());
            }
        }

        if (memObj.HasMember("size") && memObj["size"].IsInt64()) 
        {
            memory.size() = memObj["size"].GetInt64();
        }

        if (memObj.HasMember("type") && memObj["type"].IsString())
        {
            memory.type() = deepx_rmapinfo::GetMemoryTypeNum(memObj["type"].GetString());
        }

        return memory;
    }

    void assignMemoryToModelMemory(deepx_rmapinfo::Memory& memory, 
                                    deepx_rmapinfo::RegisterInfoDatabase& regMap,
                                    size_t taskBufferCount)
    {
        const string& memName = memory.name();
        
        if (memName == "RMAP")
        {
            regMap.model_memory().rmap() = memory;
            regMap.model_memory().model_memory_size() += memory.size();
            return;
        }
        
        if (memName == "WEIGHT")
        {
            regMap.model_memory().weight() = memory;
            regMap.model_memory().model_memory_size() += memory.size();
            return;
        }
        
        if (memName == "INPUT")
        {
            regMap.model_memory().input() = memory;
            regMap.model_memory().model_memory_size() += memory.size() * taskBufferCount;
            return;
        }
        
        if (memName == "OUTPUT")
        {
            regMap.model_memory().output() = memory;
            regMap.model_memory().model_memory_size() += memory.size() * taskBufferCount;
            return;
        }
        
        if (memName == "TEMP")
        {
            regMap.model_memory().temp() = memory;
            regMap.model_memory().model_memory_size() += memory.size();
        }
    }

    // ============================================================================
    // GraphInfo Related Functions
    // ============================================================================

    // Helper: Parse GraphInfo Tensor fields from JSON object
    void parseGraphInfoTensor(const Value& tensorObj, deepx_graphinfo::Tensor& tensor)
    {
        if (tensorObj.HasMember("name") && tensorObj["name"].IsString())
        {
            tensor.name() = tensorObj["name"].GetString();
        }

        if (tensorObj.HasMember("owner") && tensorObj["owner"].IsString())
        {
            tensor.owner() = tensorObj["owner"].GetString();
        }

        if (tensorObj.HasMember("users") && tensorObj["users"].IsArray()) 
        {
            const Value& usersArray = tensorObj["users"];
            for (SizeType k = 0; k < usersArray.Size(); k++) 
            {
                if (usersArray[k].IsString())
                {
                    tensor.users().emplace_back(usersArray[k].GetString());
                }
            }
        }
    }

    void parseTensorArray(const Value& tensorArray, std::vector<deepx_graphinfo::Tensor>& tensors)
    {
        for (rapidjson::SizeType j = 0; j < tensorArray.Size(); j++) 
        {
            const rapidjson::Value& tensorObj = tensorArray[j];
            deepx_graphinfo::Tensor tensor;
            parseGraphInfoTensor(tensorObj, tensor);
            tensors.emplace_back(tensor);
        }
    }

} // namespace parser_common
} // namespace dxrt
