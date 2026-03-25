/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include <fstream>
#include <cstring>
#include <memory>
#include <set>

#include "dxrt/parsers/v6_model_parser.h"
#include "dxrt/common.h"
#include "dxrt/filesys_support.h"
#include "dxrt/exception/exception.h"
#include "dxrt/util.h"
#include "dxrt/extern/rapidjson/writer.h"
#include "dxrt/extern/rapidjson/stringbuffer.h"
#include "../resource/log_messages.h"

// <windows.h> defines GetObject as a macro (GetObjectA/GetObjectW) which conflicts
// with rapidjson's Value::GetObject(). Undefine it after windows.h is pulled in.
#ifdef _WIN32
#undef GetObject
#endif

using std::vector;
using std::string;
using rapidjson::Document;
using rapidjson::Value;
using rapidjson::StringBuffer;
using rapidjson::SizeType;
using rapidjson::Writer;
using rapidjson::kObjectType;
using rapidjson::kArrayType;

namespace dxrt {

namespace {
    
    // Basic Utility Functions
    using parser_common::parseInt64FromValue;
    using parser_common::parseStringArray;
    
    // Models and BinaryInfo Related Functions
    using parser_common::copyBinaryDataToModels;
    using parser_common::copyStringDataToModels;
    using parser_common::parseModelsOffsetSize;
    using parser_common::parseCpuModels;

    // RmapInfo Related Functions
    using parser_common::parseMemoryObject;
    using parser_common::parseCheckpoints;
    using parser_common::assignMemoryToModelMemory;

    // GraphInfo Related Functions
    using parser_common::parseGraphInfoTensor;
    using parser_common::parseTensorArray;

    // Helper: Parse TensorInfo fields from JSON object (V6 specific)
    // isOutput=false: inputs parse shape_encoded normally
    // isOutput=true: outputs apply GetAlign() to last shape_encoded dim, may override align_unit to 16
    void parseTensorInfoFields(const Value& tensorObj, deepx_rmapinfo::TensorInfo& tensor, bool isOutput)
    {
        // name
        if (tensorObj.HasMember("name") && tensorObj["name"].IsString())
        {
            tensor.name() = tensorObj["name"].GetString();
        }

        // dtype
        if (tensorObj.HasMember("dtype") && tensorObj["dtype"].IsString()) 
        {
            tensor.dtype() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype"].GetString());
            tensor.elem_size() = GetDataSize_Datatype(static_cast<DataType>(tensor.dtype()));
        }

        // shape
        if (tensorObj.HasMember("shape") && tensorObj["shape"].IsArray()) 
        {
            const Value& shapeArr = tensorObj["shape"];
            for (SizeType j = 0; j < shapeArr.Size(); j++) 
            {
                tensor.shape().push_back(shapeArr[j].GetInt64());
            }
        }

        // name_encoded
        if (tensorObj.HasMember("name_encoded") && tensorObj["name_encoded"].IsString())
        {
            tensor.name_encoded() = tensorObj["name_encoded"].GetString();
        }

        // dtype_encoded
        if (tensorObj.HasMember("dtype_encoded") && tensorObj["dtype_encoded"].IsString())
        {
            tensor.dtype_encoded() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype_encoded"].GetString());
        }

        // align_unit (V6: both inputs and outputs read this at same position)
        if (tensorObj.HasMember("align_unit") && tensorObj["align_unit"].IsInt())
        {
            tensor.align_unit() = tensorObj["align_unit"].GetInt();
        }

        // shape_encoded (V6: outputs apply GetAlign to last dimension)
        if (tensorObj.HasMember("shape_encoded") && tensorObj["shape_encoded"].IsArray()) 
        {
            const Value& shapeArr = tensorObj["shape_encoded"];
            for (SizeType j = 0; j < shapeArr.Size(); j++) 
            {
                int64_t value = shapeArr[j].GetInt64();
                // V6-specific: outputs apply GetAlign() to last dimension and may override align_unit
                if (isOutput && j == shapeArr.Size() - 1)
                {
                    value = GetAlign(value);
                    if (value < 64)
                    {
                        tensor.align_unit() = 16;
                    }
                }
                tensor.shape_encoded().push_back(value);
            }
        }

        // layout
        if (tensorObj.HasMember("layout") && tensorObj["layout"].IsString())
        {
            tensor.layout() = deepx_rmapinfo::GetLayoutNum(tensorObj["layout"].GetString());
        }

        // transpose
        if (tensorObj.HasMember("transpose") && tensorObj["transpose"].IsString())
        {
            tensor.transpose() = deepx_rmapinfo::GetTransposeNum(tensorObj["transpose"].GetString());
        }

        // scale and bias (quantization)
        if (tensorObj.HasMember("scale") && tensorObj["scale"].IsFloat()) 
        {
            tensor.scale() = tensorObj["scale"].GetFloat();
            if (tensorObj.HasMember("bias") && tensorObj["bias"].IsFloat()) 
            {
                tensor.bias() = tensorObj["bias"].GetFloat();
                tensor.use_quantization() = true;
            } 
            else
            {
                tensor.use_quantization() = false;
            }
        }

        // memory
        if (tensorObj.HasMember("memory") && tensorObj["memory"].IsObject()) 
        {
            const Value& memObj = tensorObj["memory"];
            deepx_rmapinfo::Memory mem;
            if (memObj.HasMember("name") && memObj["name"].IsString())
            {
                mem.name() = memObj["name"].GetString();
            }
            if (memObj.HasMember("offset") && memObj["offset"].IsInt64())
            {
                mem.offset() = memObj["offset"].GetInt64();
            }
            if (memObj.HasMember("size") && memObj["size"].IsInt64())
            {
                mem.size() = memObj["size"].GetInt64();
            }
            if (memObj.HasMember("type") && memObj["type"].IsString())
            {
                mem.type() = deepx_rmapinfo::GetMemoryTypeNum(memObj["type"].GetString());
            }
            tensor.memory() = mem;
        }
    }

    // ============================================================================
    // Top-level Parser Functions
    // ============================================================================

    // Helper: Parse compiled data for NPU from header (V6 specific)
    void parseCompiledDataForNpu(const Value& npuData, const std::string& npuName, deepx_binaryinfo::BinaryInfoDatabase& param)
    {
        for (Value::ConstMemberIterator iter = npuData.MemberBegin(); iter != npuData.MemberEnd(); ++iter) 
        {
            if (!iter->name.IsString()) 
            {
                continue;
            }

            const string modelName = iter->name.GetString();
            const Value& value2 = iter->value;

            deepx_binaryinfo::Models rmap;
            deepx_binaryinfo::Models weight;
            deepx_binaryinfo::Models rmap_info;
            deepx_binaryinfo::Models bitmatch_mask;
            
            rmap.npu() = weight.npu() = rmap_info.npu() = bitmatch_mask.npu() = npuName;
            rmap.name() = weight.name() = rmap_info.name() = bitmatch_mask.name() = modelName;

            // [Sub-Field] - rmap
            if (value2.HasMember("rmap") && value2["rmap"].IsObject()) 
            {
                parseModelsOffsetSize(value2["rmap"], rmap);
                param.rmap().push_back(rmap);
            }

            // [Sub-Field] - weight
            if (value2.HasMember("weight") && value2["weight"].IsObject()) 
            {
                parseModelsOffsetSize(value2["weight"], weight);
                param.weight().push_back(weight);
            }

            // [Sub-Field] - rmap info
            if (value2.HasMember("rmap_info") && value2["rmap_info"].IsObject()) 
            {
                parseModelsOffsetSize(value2["rmap_info"], rmap_info);
                param.rmap_info().push_back(rmap_info);
            }

            // [Sub-Field] - bit match mask
            if (value2.HasMember("bitmatch") && value2["bitmatch"].IsObject()) 
            {
                parseModelsOffsetSize(value2["bitmatch"], bitmatch_mask);
                param.bitmatch_mask().push_back(bitmatch_mask);
            }
        }
    }

    // Helper: Parse data section from header document (V6 specific)
    void parseDataSection(const Value& dataObj, deepx_binaryinfo::BinaryInfoDatabase& param, const char* buffer, int offset)
    {
        // [Field] - cpu models
#ifdef USE_ORT
        if (dataObj.HasMember("cpu_models") && dataObj["cpu_models"].IsObject()) 
        {
            parseCpuModels(dataObj["cpu_models"], param);
        }
#endif

        // [Field] - compile config
        if (dataObj.HasMember("compile_config") && dataObj["compile_config"].IsObject()) 
        {
            const Value &compileConfiglObj = dataObj["compile_config"];
            int64_t cc_offset = 0;
            int64_t cc_size = 0;

            if (compileConfiglObj.HasMember("offset")) 
            {
                cc_offset = parseInt64FromValue(compileConfiglObj["offset"]);
            }
            if (compileConfiglObj.HasMember("size")) 
            {
                cc_size = parseInt64FromValue(compileConfiglObj["size"]);
            }

            Document compile_config_document;
            std::string compile_config_str(buffer + cc_offset + offset, cc_size);
            compile_config_document.Parse(compile_config_str.c_str());
            if (compile_config_document.HasMember("compile_version") && compile_config_document["compile_version"].IsString()) 
            {
                const Value &compileVersionObj = compile_config_document["compile_version"];
                param._compilerVersion = compileVersionObj.GetString();
            }
        }

        // [Field] - graph info
        if (dataObj.HasMember("graph_info") && dataObj["graph_info"].IsObject()) 
        {
            const Value &graphInfolObj = dataObj["graph_info"];
            if (graphInfolObj.HasMember("offset")) 
            {
                param.graph_info().offset() = parseInt64FromValue(graphInfolObj["offset"]);
            }

            if (graphInfolObj.HasMember("size")) 
            {
                param.graph_info().size() = parseInt64FromValue(graphInfolObj["size"]);
            }
        }

        // [Field] - compiled data
        if (dataObj.HasMember("compiled_data") && dataObj["compiled_data"].IsObject()) 
        {
            const Value& compiledData = dataObj["compiled_data"];
            for (Value::ConstMemberIterator iter = compiledData.MemberBegin(); iter != compiledData.MemberEnd(); ++iter) 
            {
                if (!iter->name.IsString()) 
                {
                    continue;
                }
                parseCompiledDataForNpu(iter->value, iter->name.GetString(), param);
            }
        }
    }

    // Helper: Parse single rmap_info document into RegisterInfoDatabase (V6 specific)
    void parseSingleRmapInfo(const Document& document, deepx_rmapinfo::RegisterInfoDatabase& regMap, 
                             string& modelCompileType, size_t taskBufferCount)
    {
        // version
        if (document.HasMember("version") && document["version"].IsObject()) 
        {
            const rapidjson::Value& versionObj = document["version"];
            if (versionObj.HasMember("npu") && versionObj["npu"].IsString())
            {
                regMap.version().npu() = versionObj["npu"].GetString();
            }
            if (versionObj.HasMember("rmap") && versionObj["rmap"].IsString())
            {
                regMap.version().rmap() = versionObj["rmap"].GetString();
            }
            if (versionObj.HasMember("rmapInfo") && versionObj["rmapInfo"].IsString())
            {
                regMap.version().rmap_info() = versionObj["rmapInfo"].GetString();
            }
            if (versionObj.HasMember("opt_level") && versionObj["opt_level"].IsString())
            {
                regMap.version().opt_level() = versionObj["opt_level"].GetString();
            }
        }

        // name
        if (document.HasMember("name") && document["name"].IsString())
        {
            regMap.name() = document["name"].GetString();
        }

        // mode
        if (document.HasMember("mode") && document["mode"].IsString()) 
        {
            modelCompileType = document["mode"].GetString();
            regMap.mode() = modelCompileType;
        }

        // npu
        if (document.HasMember("npu") && document["npu"].IsObject()) 
        {
            const rapidjson::Value& npuObj = document["npu"];
            if (npuObj.HasMember("mac") && npuObj["mac"].IsInt64())
            {
                regMap.npu().mac() = npuObj["mac"].GetInt64();
            }
        }

        // size
        if (document.HasMember("size") && document["size"].IsInt64()) 
        {
            regMap.size() = document["size"].GetInt64();
        } 
        else 
        {
            regMap.size() = 0;
        }

        // counts
        if (document.HasMember("counts") && document["counts"].IsObject()) 
        {
            const rapidjson::Value& countsObj = document["counts"];
            if (countsObj.HasMember("layer") && countsObj["layer"].IsInt64())
            {
                regMap.counts().layer() = countsObj["layer"].GetInt64();
            }

            if (countsObj.HasMember("cmd") && countsObj["cmd"].IsInt64())
            {
                regMap.counts().cmd() = countsObj["cmd"].GetInt64();
            }

            if (countsObj.HasMember("checkpoints") && countsObj["checkpoints"].IsArray()) 
            {
                parseCheckpoints(countsObj["checkpoints"], regMap.counts());
            } 
            else 
            {
                regMap.counts()._op_mode = 0;
            }
        }

        // memory: List[MemoryInfo]
        if (document.HasMember("memory") && document["memory"].IsArray()) 
        {
            const rapidjson::Value& memArray = document["memory"];
            for (rapidjson::SizeType mi = 0; mi < memArray.Size(); mi++) 
            {
                const rapidjson::Value& memObj = memArray[mi];
                if (!memObj.HasMember("name") || !memObj["name"].IsString()) 
                {
                    continue;
                }

                deepx_rmapinfo::Memory memory = parseMemoryObject(memObj);
                assignMemoryToModelMemory(memory, regMap, taskBufferCount);
            }
        }

        // inputs: List[TensorInfo]
        if (document.HasMember("inputs") && document["inputs"].IsArray()) 
        {
            const rapidjson::Value& tensorArray = document["inputs"];
            regMap.inputs().clear();
            for (rapidjson::SizeType ti = 0; ti < tensorArray.Size(); ti++) 
            {
                const rapidjson::Value& tensorObj = tensorArray[ti];
                deepx_rmapinfo::TensorInfo tensor;
                parseTensorInfoFields(tensorObj, tensor, false);
                regMap.inputs().push_back(tensor);
            }
        }

        // outputs: List[TensorInfo]
        if (document.HasMember("outputs") && document["outputs"].IsArray())
        {
            const rapidjson::Value& tensorArray = document["outputs"];
            regMap.outputs().clear();
            for (rapidjson::SizeType ti = 0; ti < tensorArray.Size(); ti++) 
            {
                const rapidjson::Value& tensorObj = tensorArray[ti];
                deepx_rmapinfo::TensorInfo tensor;
                parseTensorInfoFields(tensorObj, tensor, true);                // V6-specific: PPU output special handling
                if (tensor.memory().type() == deepx_rmapinfo::MemoryType::PPU) 
                {
                    if (tensor.layout() == deepx_rmapinfo::Layout::PPU_YOLO)
                    {
                        tensor.name() = "BBOX";
                    }
                    else if (tensor.layout() == deepx_rmapinfo::Layout::PPU_FD)
                    {
                        tensor.name() = "FACE";
                    }
                    else if (tensor.layout() == deepx_rmapinfo::Layout::PPU_POSE)
                    {
                        tensor.name() = "POSE";
                    }
                    else
                    {
                        throw ModelParsingException(EXCEPTION_MESSAGE("PPU Output format is invalid"));
                    }
                    tensor.shape().clear();
                    tensor.shape().push_back(1);
                    tensor.shape().push_back(-1);
                    tensor.shape_encoded().clear();
                    tensor.shape_encoded().push_back(1);
                    tensor.shape_encoded().push_back(-1);
                    int dataType = DataType::BBOX;
                    dataType += tensor.layout();
                    dataType -= deepx_rmapinfo::Layout::PPU_YOLO;
                    tensor.dtype() = dataType;
                }
                regMap.outputs().push_back(tensor);
            }
        }
    }
} // namespace

void V6ModelParser::PreProcessModel(ModelDataBase& modelData) const {
    // Store original v6 graph_info and rmap_info
    string v6GraphInfo = "";
    for (const auto& str : modelData.deepx_binary.graph_info().str())
    {
        v6GraphInfo += str;
    }

    vector<string> v6RmapInfos;
    for (size_t i = 0; i < modelData.deepx_binary.rmap_info().size(); i++) 
    {
        string v6RmapInfo = "";
        for (const auto& str : modelData.deepx_binary.rmap_info(static_cast<int>(i)).str())
        {
            v6RmapInfo += str;
        }
        v6RmapInfos.push_back(v6RmapInfo);
    }

    // v6_converter.py logic: Keep binary data as is, convert rmap_info and graph_info

    // Convert graph_info to v7 format
    string v7GraphInfo = ConvertGraphInfoV6ToV7(v6GraphInfo);
    modelData.deepx_binary.graph_info().str() = v7GraphInfo;

    // Convert rmap_info to v7 format
    for (size_t i = 0; i < modelData.deepx_binary.rmap_info().size(); i++) {
        string v7RmapInfo = ConvertRmapInfoV6ToV7(v6RmapInfos[i], v6GraphInfo);
        modelData.deepx_binary.rmap_info(static_cast<int>(i)).str() = v7RmapInfo;
    }
}

int V6ModelParser::loadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, const char *buffer, int fileSize) const {
    Document document;
    std::ignore = fileSize;

    int offset = 0;
    int sizeInfo = 8192;
    string signInfo;
    string headerInfo;

    signInfo = string(buffer, 4);
    offset += 8;
    if (signInfo != "DXNN") 
    {
        throw InvalidModelException(EXCEPTION_MESSAGE(LogMessages::InvalidDXNNFileFormat()));
    }

    // dxnn file format version 4byte integer little-endian
    int32_t dxnnFileFormatVersion = buffer[4] |
                buffer[5] << 8  |
                buffer[6] << 16 |
                buffer[7] << 24;
    param._dxnnFileFormatVersion = dxnnFileFormatVersion;

    if (dxnnFileFormatVersion != 6) 
    {
        throw ModelParsingException(EXCEPTION_MESSAGE("V6ModelParser can only parse version 6 files"));
    }

    sizeInfo = 8192;
    headerInfo = string(buffer+offset, sizeInfo-offset);
    offset = sizeInfo;

    document.Parse(headerInfo.c_str());
    if (document.HasParseError()) 
    {
        throw ModelParsingException(
            EXCEPTION_MESSAGE(LogMessages::InvalidDXNNModelHeader(static_cast<int>(document.GetParseError())))
        );
    }

    if (document.HasMember("data") && document["data"].IsObject()) 
    {
        parseDataSection(document["data"], param, buffer, offset);
    }

    // [Buffer] - CPU Binary Data
    copyBinaryDataToModels(param.cpu_models(), buffer, offset);

    // [Buffer] - Graph Info.
    string graphInfoStr(buffer + offset + param.graph_info().offset(), param.graph_info().size());
    param.graph_info().str() = graphInfoStr;

    // [Buffer] - RMAP Binary Data
    copyBinaryDataToModels(param.rmap(), buffer, offset);

    // [Buffer] - Weight Binary Data
    copyBinaryDataToModels(param.weight(), buffer, offset);

    // [Buffer] - RMAP Info.
    copyStringDataToModels(param.rmap_info(), buffer, offset);

    // [Buffer] - Bitmatch Mask.
    copyBinaryDataToModels(param.bitmatch_mask(), buffer, offset);

    return dxnnFileFormatVersion;
}

std::string V6ModelParser::ConvertGraphInfoV6ToV7(const std::string& v6GraphInfo) const {

    Document v6Doc;
    v6Doc.Parse(v6GraphInfo.c_str());

    if (v6Doc.HasParseError()) 
    {
        throw ModelParsingException(EXCEPTION_MESSAGE("Failed to parse V6 graph info"));
    }

    Document v7Doc;
    v7Doc.SetObject();
    Document::AllocatorType& allocator = v7Doc.GetAllocator();

    if (v6Doc.HasMember("offloading")) 
    {
        v7Doc.AddMember("offloading", v6Doc["offloading"], allocator);
    }

    std::set<std::string> modelInputs;
    std::set<std::string> modelOutputs;
    if (v6Doc.HasMember("origin_input") && v6Doc["origin_input"].IsArray()) 
    {
        for (const auto& inp : v6Doc["origin_input"].GetArray()) 
        {
            if (inp.IsString())
            {
                modelInputs.emplace(inp.GetString());
                LOG_DXRT_DBG << "[V6→V7] Model input: " << inp.GetString() << std::endl;
            }
        }
    }
    if (v6Doc.HasMember("origin_output") && v6Doc["origin_output"].IsArray()) 
    {
        for (const auto& out : v6Doc["origin_output"].GetArray()) 
        {
            if (out.IsString()) 
            {
                modelOutputs.emplace(out.GetString());
                LOG_DXRT_DBG << "[V6→V7] Model output: " << out.GetString() << std::endl;
            }
        }
    }
    LOG_DXRT_DBG << "[V6→V7] Collected " << modelInputs.size() << " model inputs, " << modelOutputs.size() << " model outputs" << std::endl;

    if (v6Doc.HasMember("origin_input")) 
    {
        v7Doc.AddMember("inputs", v6Doc["origin_input"], allocator);
    }

    if (v6Doc.HasMember("origin_output")) {
        v7Doc.AddMember("outputs", v6Doc["origin_output"], allocator);
    }

    if (v6Doc.HasMember("toposort_order")) {
        v7Doc.AddMember("toposort_order", v6Doc["toposort_order"], allocator);
    }

    if (v6Doc.HasMember("graphs") && v6Doc["graphs"].IsArray()) 
    {
        Value v7Graphs(kArrayType);

        for (auto& v6Graph : v6Doc["graphs"].GetArray()) 
        {
            Value v7Graph(kObjectType);

            // name, device copy
            std::string taskName;
            if (v6Graph.HasMember("name")) 
            {
                taskName = v6Graph["name"].GetString();
                v7Graph.AddMember("name", v6Graph["name"], allocator);
            }

            if (v6Graph.HasMember("type")) 
            {
                v7Graph.AddMember("device", v6Graph["type"], allocator);
            }
            
            // Determine if this task is head or tail
            bool isHead = false;
            bool isTail = false;
            LOG_DXRT_DBG << "[V6→V7] Processing task: " << taskName << std::endl;

            if (v6Graph.HasMember("inputs") && v6Graph["inputs"].IsObject()) 
            {
                Value v7Inputs(kArrayType);

                for (auto& input : v6Graph["inputs"].GetObject()) 
                {
                    Value inputObj(kObjectType);
                    Value nameVal;
                    nameVal.SetString(input.name.GetString(), allocator);
                    inputObj.AddMember("name", nameVal, allocator);

                    std::string inputName = input.name.GetString();
                    bool hasOwner = false;
                    std::string ownerStr = "";

                    if (input.value.IsObject() && input.value.HasMember("source") && input.value["source"].IsString()) 
                    {
                        ownerStr = input.value["source"].GetString();
                        hasOwner = !ownerStr.empty();
                    }
                    
                    Value ownerVal;
                    ownerVal.SetString(ownerStr.c_str(), allocator);
                    inputObj.AddMember("owner", ownerVal, allocator);
                    
                    // If input has no owner and is a model input, this task is a head
                    bool isTensorHead = (modelInputs.count(inputName) > 0);
                    if (!hasOwner && isTensorHead) 
                    {
                        isHead = true;
                        LOG_DXRT_DBG << "[V6→V7]   Input '" << inputName << "' is model input -> task is HEAD" << std::endl;
                    }
                    
                    // Add head flag to tensor (V7+ format)
                    inputObj.AddMember("head", isTensorHead, allocator);
                    LOG_DXRT_DBG << "[V6→V7]   Input '" << inputName << "' head=" << (isTensorHead ? "true" : "false") << std::endl;
                    
                    // Add tail flag to tensor (always false for inputs)
                    inputObj.AddMember("tail", false, allocator);

                    Value users(kArrayType);
                    if (v6Graph.HasMember("name") && v6Graph["name"].IsString()) 
                    {
                        Value taskNameVal;
                        taskNameVal.SetString(v6Graph["name"].GetString(), allocator);
                        users.PushBack(taskNameVal, allocator);
                    }
                    inputObj.AddMember("users", users, allocator);

                    v7Inputs.PushBack(inputObj, allocator);
                }

                v7Graph.AddMember("inputs", v7Inputs, allocator);
            }

            if (v6Graph.HasMember("outputs") && v6Graph["outputs"].IsObject()) 
            {
                Value v7Outputs(kArrayType);

                for (auto& output : v6Graph["outputs"].GetObject()) 
                {
                    Value outputObj(kObjectType);
                    Value nameVal;
                    std::string outputName = output.name.GetString();
                    nameVal.SetString(outputName.c_str(), allocator);
                    outputObj.AddMember("name", nameVal, allocator);

                    if (v6Graph.HasMember("name") && v6Graph["name"].IsString()) 
                    {
                        Value ownerTaskVal;
                        ownerTaskVal.SetString(v6Graph["name"].GetString(), allocator);
                        outputObj.AddMember("owner", ownerTaskVal, allocator);
                    }
                    
                    // Check if this output is a model output
                    bool isModelOutput = (modelOutputs.count(outputName) > 0);
                    if (isModelOutput) 
                    {
                        isTail = true;
                        LOG_DXRT_DBG << "[V6→V7]   Output '" << outputName << "' is model output -> task is TAIL" << std::endl;
                    }
                    
                    // Add head flag to tensor (always false for outputs)
                    outputObj.AddMember("head", false, allocator);
                    
                    // Add tail flag to tensor (V7+ format)
                    outputObj.AddMember("tail", isModelOutput, allocator);
                    LOG_DXRT_DBG << "[V6→V7]   Output '" << outputName << "' tail=" << (isModelOutput ? "true" : "false") << std::endl;

                    Value users(kArrayType);
                    if (output.value.IsObject() && output.value.HasMember("next_layers") && output.value["next_layers"].IsArray()) 
                    {
                        for (const auto& nextLayer : output.value["next_layers"].GetArray()) 
                        {
                            if (nextLayer.IsString()) 
                            {
                                Value nextLayerVal;
                                nextLayerVal.SetString(nextLayer.GetString(), allocator);
                                users.PushBack(nextLayerVal, allocator);
                            }
                        }
                    }
                    outputObj.AddMember("users", users, allocator);

                    v7Outputs.PushBack(outputObj, allocator);
                }

                v7Graph.AddMember("outputs", v7Outputs, allocator);
            }
            
            // Add head/tail flags to subgraph (V7+ format)
            v7Graph.AddMember("head", isHead, allocator);
            v7Graph.AddMember("tail", isTail, allocator);
            LOG_DXRT_DBG << "[V6→V7] Task '" << taskName << "' head=" << (isHead ? "true" : "false") << " tail=" << (isTail ? "true" : "false") << std::endl;

            v7Graphs.PushBack(v7Graph, allocator);
        }

        v7Doc.AddMember("graphs", v7Graphs, allocator);
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    v7Doc.Accept(writer);

    std::string result = buffer.GetString();

    return result;
}

std::string V6ModelParser::ConvertRmapInfoV6ToV7(const std::string& v6RmapInfo, const std::string& v6GraphInfo) const {
    // Convert V6 rmap info to V7 format.
    // This implements the logic of the `convert_rmap_info` function from v6_converter.py in C++.

    Document v6RmapDoc;
    Document v6GraphDoc;
    v6RmapDoc.Parse(v6RmapInfo.c_str());
    v6GraphDoc.Parse(v6GraphInfo.c_str());

    if (v6RmapDoc.HasParseError() || v6GraphDoc.HasParseError()) 
    {
        throw ModelParsingException(EXCEPTION_MESSAGE("Failed to parse V6 rmap/graph info"));
    }

    // Create a V7 format JSON
    Document v7Doc;
    v7Doc.SetObject();
    Document::AllocatorType& allocator = v7Doc.GetAllocator();
    
    // 1. Extract input_name and input_shape from v6 graph_info (following v6_converter.py logic)
    string input_name = ExtractInputNameFromV6Graph(v6GraphDoc);
    Value input_shape = ExtractInputShapeFromV6Graph(v6GraphDoc, allocator);
    
    // 2. Convert version information
    if (v6RmapDoc.HasMember("version") && v6RmapDoc["version"].IsObject()) 
    {
        const Value& v6Version = v6RmapDoc["version"];
        Value versionObj(kObjectType);

        if (v6Version.HasMember("npu") && v6Version["npu"].IsString()) 
        {
            Value npuVal;
            npuVal.SetString(v6Version["npu"].GetString(), allocator);
            versionObj.AddMember("npu", npuVal, allocator);
        }
        if (v6Version.HasMember("rmap") && v6Version["rmap"].IsString()) 
        {
            Value rmapVal;
            rmapVal.SetString(v6Version["rmap"].GetString(), allocator);
            versionObj.AddMember("rmap", rmapVal, allocator);
        }

        // Separate version and optimization level from rmapInfo
        if (v6Version.HasMember("rmapInfo") && v6Version["rmapInfo"].IsString()) 
        {
            auto versionPair = ParseV6Version(v6Version["rmapInfo"].GetString());
            std::string cg_version = versionPair.first;
            std::string opt_level = versionPair.second;
            Value rmapInfoVal;
            Value optLevelVal;
            rmapInfoVal.SetString(cg_version.c_str(), allocator);
            optLevelVal.SetString(opt_level.c_str(), allocator);
            versionObj.AddMember("rmapInfo", rmapInfoVal, allocator);
            versionObj.AddMember("opt_level", optLevelVal, allocator);
        }

        v7Doc.AddMember("version", versionObj, allocator);
    }

    // 3. Basic information
    if (v6RmapDoc.HasMember("model") && v6RmapDoc["model"].IsString()) 
    {
        Value nameVal;
        nameVal.SetString(v6RmapDoc["model"].GetString(), allocator);
        v7Doc.AddMember("name", nameVal, allocator);
    }
    if (v6RmapDoc.HasMember("mode") && v6RmapDoc["mode"].IsString()) 
    {
        Value modeVal;
        modeVal.SetString(v6RmapDoc["mode"].GetString(), allocator);
        v7Doc.AddMember("mode", modeVal, allocator);
    }
    if (v6RmapDoc.HasMember("npu") && v6RmapDoc["npu"].IsObject()) 
    {
        Value npuObj(kObjectType);
        const Value& v6Npu = v6RmapDoc["npu"];
        if (v6Npu.HasMember("mac") && v6Npu["mac"].IsInt64()) 
        {
            rapidjson::Value macVal;
            macVal.SetInt64(v6Npu["mac"].GetInt64());
            npuObj.AddMember(rapidjson::StringRef("mac"), macVal, allocator);
        }
        v7Doc.AddMember("npu", npuObj, allocator);
    }

    if (v6RmapDoc.HasMember("size")) {
        if (v6RmapDoc["size"].IsString()) 
        {
            int64_t sizeVal = std::stoll(v6RmapDoc["size"].GetString());
            v7Doc.AddMember("size", sizeVal, allocator);
        } 
        else if (v6RmapDoc["size"].IsInt()) 
        {
            v7Doc.AddMember("size", v6RmapDoc["size"], allocator);
        } 
        else if (v6RmapDoc["size"].IsInt64()) 
        {
            v7Doc.AddMember("size", v6RmapDoc["size"], allocator);
        }
    }

    if (v6RmapDoc.HasMember("counts") && v6RmapDoc["counts"].IsObject()) 
    {
        Value countsVal;
        countsVal.CopyFrom(v6RmapDoc["counts"], allocator);
        v7Doc.AddMember("counts", countsVal, allocator);
    }

    // 4. Create Memory information (referencing v6_converter.py)
    Value memoryArray(kArrayType);

    // INPUT memory (from v6 input.memory)
    if (v6RmapDoc.HasMember("input") && v6RmapDoc["input"].HasMember("memory")) 
    {
        Value inputMem(kObjectType);
        const Value& v6InputMem = v6RmapDoc["input"]["memory"];
        Value nameVal;
        nameVal.SetString("INPUT", allocator);
        inputMem.AddMember(rapidjson::StringRef("name"), nameVal, allocator);

        if (v6InputMem.HasMember("offset")) 
        {
            inputMem.AddMember(rapidjson::StringRef("offset"), parseInt64FromValue(v6InputMem["offset"]), allocator);
        }
        if (v6InputMem.HasMember("size")) 
        {
            inputMem.AddMember(rapidjson::StringRef("size"), parseInt64FromValue(v6InputMem["size"]), allocator);
        }
        if (v6InputMem.HasMember("type"))
        {
            Value typeVal;
            typeVal.SetString(v6InputMem["type"].GetString(), allocator);
            inputMem.AddMember(rapidjson::StringRef("type"), typeVal, allocator);
        }
        memoryArray.PushBack(inputMem, allocator);
    }

    // OUTPUT memory (from v6 outputs.memory)
    if (v6RmapDoc.HasMember("outputs") && v6RmapDoc["outputs"].HasMember("memory")) 
    {
        Value outputMem(kObjectType);
        const Value& v6OutputMem = v6RmapDoc["outputs"]["memory"];
        Value nameVal;
        nameVal.SetString("OUTPUT", allocator);
        outputMem.AddMember(rapidjson::StringRef("name"), nameVal, allocator);

        if (v6OutputMem.HasMember("offset")) {
            outputMem.AddMember(rapidjson::StringRef("offset"), parseInt64FromValue(v6OutputMem["offset"]), allocator);
        }
        if (v6OutputMem.HasMember("size")) {
            outputMem.AddMember(rapidjson::StringRef("size"), parseInt64FromValue(v6OutputMem["size"]), allocator);
        }
        if (v6OutputMem.HasMember("type")) {
            Value typeVal;
            typeVal.SetString(v6OutputMem["type"].GetString(), allocator);
            outputMem.AddMember(rapidjson::StringRef("type"), typeVal, allocator);
        }
        memoryArray.PushBack(outputMem, allocator);
    }

    // Other memories (from v6 memorys.memory array)
    if (v6RmapDoc.HasMember("memorys") && v6RmapDoc["memorys"].HasMember("memory")
        && v6RmapDoc["memorys"]["memory"].IsArray()) {
        const Value& v6MemArray = v6RmapDoc["memorys"]["memory"];
        for (SizeType i = 0; i < v6MemArray.Size(); i++) {
            const Value& v6Mem = v6MemArray[i];
            Value mem(kObjectType);

            if (v6Mem.HasMember("name")) {
                Value nameVal;
                nameVal.SetString(v6Mem["name"].GetString(), allocator);
                mem.AddMember(rapidjson::StringRef("name"), nameVal, allocator);
            }
            if (v6Mem.HasMember("offset")) {
                mem.AddMember(rapidjson::StringRef("offset"), parseInt64FromValue(v6Mem["offset"]), allocator);
            }
            if (v6Mem.HasMember("size")) {
                mem.AddMember(rapidjson::StringRef("size"), parseInt64FromValue(v6Mem["size"]), allocator);
            }
            Value typeVal;
            typeVal.SetString("DRAM", allocator);
            mem.AddMember(rapidjson::StringRef("type"), typeVal, allocator);
            memoryArray.PushBack(mem, allocator);
        }
    }

    v7Doc.AddMember("memory", memoryArray, allocator);

    // 5. Create Inputs information (from v6 input)
    Value inputsArray(kArrayType);
    if (v6RmapDoc.HasMember("input")) {
        const Value& v6Input = v6RmapDoc["input"];
        Value inputTensor(kObjectType);

        Value nameVal;
        nameVal.SetString(input_name.c_str(), allocator);
        inputTensor.AddMember("name", nameVal, allocator);

        if (v6Input.HasMember("type")) {
            Value dtypeVal;
            dtypeVal.SetString(v6Input["type"].GetString(), allocator);
            inputTensor.AddMember("dtype", dtypeVal, allocator);

            // Set to the same as original type, following v6_converter.py
            Value dtypeEncodedVal;
            dtypeEncodedVal.SetString(v6Input["type"].GetString(), allocator);
            inputTensor.AddMember("dtype_encoded", dtypeEncodedVal, allocator);
        }
        
        // Use input_shape from v6 graph_info (extracted at the beginning) and apply alignment
        Value shapeVal;
        Value shapeEncodedVal;
        shapeVal.CopyFrom(input_shape, allocator);
        shapeEncodedVal.CopyFrom(input_shape, allocator);
        
        
        // Align both shape and shape_encoded based on last dimension and compute align_unit
        int alignUnit = 64;
        if (input_shape.IsArray() && input_shape.Size() > 0) {
            int64_t lastDim = input_shape[input_shape.Size() - 1].GetInt64();
            if (lastDim < 64) {
                alignUnit = 16;
                // Round up to multiple of 16
                //int64_t alignedLastDim = ((lastDim + 15) / 16) * 16;
                //shapeVal[shapeVal.Size() - 1].SetInt64(alignedLastDim);
                //shapeEncodedVal[shapeEncodedVal.Size() - 1].SetInt64(alignedLastDim)
            } else {
                alignUnit = 64;
                // Round up to multiple of 64
                //int64_t alignedLastDim = ((lastDim + 63) / 64) * 64;
                //shapeVal[shapeVal.Size() - 1].SetInt64(alignedLastDim);
                //shapeEncodedVal[shapeEncodedVal.Size() - 1].SetInt64(alignedLastDim)
            }
        }
        
        inputTensor.AddMember("shape", shapeVal, allocator);
        inputTensor.AddMember("shape_encoded", shapeEncodedVal, allocator);
        
        // Set to the same as original name, following v6_converter.py
        Value nameEncodedVal;
        nameEncodedVal.SetString(input_name.c_str(), allocator);
        inputTensor.AddMember("name_encoded", nameEncodedVal, allocator);

        Value layoutVal;
        layoutVal.SetString("NONE", allocator);
        inputTensor.AddMember("layout", layoutVal, allocator);

        inputTensor.AddMember("align_unit", alignUnit, allocator);

        Value transposeVal;
        transposeVal.SetNull();
        inputTensor.AddMember("transpose", transposeVal, allocator);

        Value scaleVal;
        Value biasVal;
        scaleVal.SetNull();
        biasVal.SetNull();
        inputTensor.AddMember("scale", scaleVal, allocator);
        inputTensor.AddMember("bias", biasVal, allocator);

        // Set Input memory info (from v6 input.memory)
        Value inputMemory(kObjectType);
        Value inputNameVal;
        inputNameVal.SetString("INPUT", allocator);
        inputMemory.AddMember("name", inputNameVal, allocator);

        int64_t inputOffset = 0;
        int64_t inputSize = 0;
        const char* inputTypeStr = "DRAM";
        if (v6Input.HasMember("memory") && v6Input["memory"].IsObject()) {
            const Value& v6InputMem = v6Input["memory"];
            if (v6InputMem.HasMember("offset")) {
                inputOffset = parseInt64FromValue(v6InputMem["offset"]);
            }
            if (v6InputMem.HasMember("size")) {
                inputSize = parseInt64FromValue(v6InputMem["size"]);
            }
            if (v6InputMem.HasMember("type") && v6InputMem["type"].IsString()) {
                inputTypeStr = v6InputMem["type"].GetString();
            }
        }

        inputMemory.AddMember("offset", inputOffset, allocator);
        inputMemory.AddMember("size", inputSize, allocator);

        Value inputTypeVal;
        inputTypeVal.SetString(inputTypeStr, allocator);
        inputMemory.AddMember("type", inputTypeVal, allocator);

        inputTensor.AddMember("memory", inputMemory, allocator);
        inputsArray.PushBack(inputTensor, allocator);
    }

    v7Doc.AddMember("inputs", inputsArray, allocator);

    // 6. Create Outputs information (from v6 outputs.outputList.output array)
    Value outputsArray(kArrayType);
    if (v6RmapDoc.HasMember("outputs") && v6RmapDoc["outputs"].HasMember("outputList")
        && v6RmapDoc["outputs"]["outputList"].HasMember("output")
        && v6RmapDoc["outputs"]["outputList"]["output"].IsArray()) {

        const Value& v6OutputArray = v6RmapDoc["outputs"]["outputList"]["output"];
        for (SizeType i = 0; i < v6OutputArray.Size(); i++) {
            const Value& v6Output = v6OutputArray[i];
            Value outputTensor(kObjectType);
            
            string outputName;
            if (v6Output.HasMember("name")) {
                outputName = v6Output["name"].GetString();
                Value nameVal;
                nameVal.SetString(outputName.c_str(), allocator);
                outputTensor.AddMember("name", nameVal, allocator);

                // Set to the same as original name, following v6_converter.py
                Value nameEncodedVal;
                nameEncodedVal.SetString(outputName.c_str(), allocator);
                outputTensor.AddMember("name_encoded", nameEncodedVal, allocator);
            }

            if (v6Output.HasMember("type")) {
                Value dtypeVal;
                dtypeVal.SetString(v6Output["type"].GetString(), allocator);
                outputTensor.AddMember("dtype", dtypeVal, allocator);

                // Set to the same as original type, following v6_converter.py
                Value dtypeEncodedVal;
                dtypeEncodedVal.SetString(v6Output["type"].GetString(), allocator);
                outputTensor.AddMember("dtype_encoded", dtypeEncodedVal, allocator);
            }
            
            // Handle layout field - preserve PPU_* values, set others to NONE
            Value layoutVal;
            bool isPPUOutput = false;
            if (v6Output.HasMember("format") && v6Output["format"].IsString()) {
                std::string layoutStr = v6Output["format"].GetString();
                if (layoutStr.find("PPU_") == 0) {
                    isPPUOutput = true;
                    layoutVal.SetString(layoutStr.c_str(), allocator);
                } else {
                    layoutVal.SetString("NONE", allocator);
                }
            } else {
                layoutVal.SetString("NONE", allocator);
            }
            outputTensor.AddMember("layout", layoutVal, allocator);
            
            // Extract output shape from v6 graph_info for this specific output, following v6_converter.py
            // For PPU outputs (dynamic size), use [1, -1] shape instead of graph_info shape
            Value outputShapeVal(kArrayType);
            Value outputShapeEncodedVal(kArrayType);
            int outputAlignUnit = 64;
            if (isPPUOutput) 
            {
                // PPU outputs have dynamic size, represented as [1, -1]
                outputShapeVal.PushBack(1, allocator);
                outputShapeVal.PushBack(-1, allocator);
                outputShapeEncodedVal.PushBack(1, allocator);
                outputShapeEncodedVal.PushBack(-1, allocator);
            } else {
                // shape = npu_graph["outputs"][value["name"]]["shape"]
                outputShapeVal = ExtractOutputShapeFromV6Graph(v6GraphDoc, outputName, allocator);
                outputShapeEncodedVal.CopyFrom(outputShapeVal, allocator);
                
                // Align both shape and shape_encoded based on last dimension and compute align_unit
                if (outputShapeVal.IsArray() && outputShapeVal.Size() > 0) {
                    int64_t lastDim = outputShapeVal[outputShapeVal.Size() - 1].GetInt64();
                    if (lastDim < 64) 
                    {
                        outputAlignUnit = 16;
                        // Round up to multiple of 16
                        //int64_t alignedLastDim = ((lastDim + 15) / 16) * 16;
                        //outputShapeVal[outputShapeVal.Size() - 1].SetInt64(alignedLastDim);
                        //outputShapeEncodedVal[outputShapeEncodedVal.Size() - 1].SetInt64(alignedLastDim)
                    } 
                    else 
                    {
                        outputAlignUnit = 64;
                        // Round up to multiple of 64
                        //int64_t alignedLastDim = ((lastDim + 63) / 64) * 64;
                        //outputShapeVal[outputShapeVal.Size() - 1].SetInt64(alignedLastDim);
                        //outputShapeEncodedVal[outputShapeEncodedVal.Size() - 1].SetInt64(alignedLastDim)
                    }
                }
            }
            outputTensor.AddMember("shape", outputShapeVal, allocator);
            outputTensor.AddMember("shape_encoded", outputShapeEncodedVal, allocator);

            outputTensor.AddMember("align_unit", outputAlignUnit, allocator);

            Value transposeVal;
            transposeVal.SetNull();
            outputTensor.AddMember("transpose", transposeVal, allocator);

            Value scaleVal;
            Value biasVal;
            scaleVal.SetNull();
            biasVal.SetNull();
            outputTensor.AddMember("scale", scaleVal, allocator);
            outputTensor.AddMember("bias", biasVal, allocator);

            // Set Output memory info (from v6 output.memory)
            Value outputMemory(kObjectType);
            Value outputNameVal;
            outputNameVal.SetString("OUTPUT", allocator);
            outputMemory.AddMember("name", outputNameVal, allocator);

            int64_t outputOffset = 0;
            int64_t outputSize = 0;
            const char* outputTypeStr = "DRAM";
            if (v6Output.HasMember("memory") && v6Output["memory"].IsObject()) 
            {
                const Value& v6OutputMem = v6Output["memory"];
                if (v6OutputMem.HasMember("offset")) 
                {
                    outputOffset = parseInt64FromValue(v6OutputMem["offset"]);
                }
                if (v6OutputMem.HasMember("size")) 
                {
                    outputSize = parseInt64FromValue(v6OutputMem["size"]);
                }
                if (v6OutputMem.HasMember("type") && v6OutputMem["type"].IsString()) 
                {
                    outputTypeStr = v6OutputMem["type"].GetString();
                }
            }

            outputMemory.AddMember("offset", outputOffset, allocator);
            outputMemory.AddMember("size", outputSize, allocator);

            Value outputTypeVal;
            outputTypeVal.SetString(outputTypeStr, allocator);
            outputMemory.AddMember("type", outputTypeVal, allocator);

            outputTensor.AddMember("memory", outputMemory, allocator);

            outputsArray.PushBack(outputTensor, allocator);
        }
    }

    v7Doc.AddMember("outputs", outputsArray, allocator);

    // Convert JSON to string
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    v7Doc.Accept(writer);

    std::string result = buffer.GetString();

    return result;
}

// Helper function implementations
std::string V6ModelParser::ParseV6GraphInfo(const rapidjson::Document& v6GraphInfo) const {
    // Parse V6 graph info
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    v6GraphInfo.Accept(writer);
    return buffer.GetString();
}

std::string V6ModelParser::ParseV6RmapInfo(const rapidjson::Document& v6RmapInfo, const rapidjson::Document& v6GraphInfo) const {
    // Parse V6 rmap info
    string v6RmapStr;
    string v6GraphStr;

    StringBuffer rmapBuffer;
    StringBuffer graphBuffer;
    Writer<StringBuffer> rmapWriter(rmapBuffer);
    Writer<StringBuffer> graphWriter(graphBuffer);
    v6RmapInfo.Accept(rmapWriter);
    v6GraphInfo.Accept(graphWriter);

    v6RmapStr = rmapBuffer.GetString();
    v6GraphStr = graphBuffer.GetString();

    return ConvertRmapInfoV6ToV7(v6RmapStr, v6GraphStr);
}

std::string V6ModelParser::ExtractInputNameFromV6Graph(const rapidjson::Document& v6GraphInfo) const {
    // Extract input name from V6 graph.
    // Python equivalent:
    // for graph in graph_info["graphs"]:
    //     if graph["name"] == "npu_0":
    //         input_name = list(graph["inputs"].keys())[0]

    if (v6GraphInfo.HasMember("graphs") && v6GraphInfo["graphs"].IsArray()) 
    {
        const Value& graphs = v6GraphInfo["graphs"];

        for(const auto& graph : graphs.GetArray()) 
        {
            if (graph.HasMember("name") && graph["name"].IsString() &&
                string(graph["name"].GetString()) == "npu_0" && graph.HasMember("inputs") && graph["inputs"].IsObject())
            {
                const Value& inputs = graph["inputs"];
                if (inputs.MemberCount() > 0)
                {
                    return inputs.MemberBegin()->name.GetString(); // Return the first input name
                }
            }
        }
    }

    // Return a default value if not found
    return "input";
}

Value V6ModelParser::ExtractInputShapeFromV6Graph(const rapidjson::Document& v6GraphInfo, Document::AllocatorType& allocator) const {
    // Extract input shape from V6 graph.
    // Python equivalent:
    // for graph in graph_info["graphs"]:
    //     if graph["name"] == "npu_0":
    //         npu_graph = graph
    //         break
    // input_name, input_tensor = list(npu_graph["inputs"].items())[0]
    // input_shape = input_tensor["shape"]
    
    if (v6GraphInfo.HasMember("graphs") && v6GraphInfo["graphs"].IsArray()) {
        const Value& graphs = v6GraphInfo["graphs"];
        for (SizeType i = 0; i < graphs.Size(); i++) {
            const Value& graph = graphs[i];
            if (graph.HasMember("name") && graph["name"].IsString() &&
                string(graph["name"].GetString()) == "npu_0" && graph.HasMember("inputs") && graph["inputs"].IsObject()) {
                const Value& inputs = graph["inputs"];
                if (inputs.MemberCount() > 0)
                {
                    const Value& inputTensor = inputs.MemberBegin()->value;
                    if (inputTensor.HasMember("shape") && inputTensor["shape"].IsArray()) 
                    {
                        Value shape(kArrayType);
                        const Value& inputShape = inputTensor["shape"];
                        for (SizeType j = 0; j < inputShape.Size(); j++) 
                        {
                            Value val;
                            val.CopyFrom(inputShape[j], allocator);
                            shape.PushBack(val, allocator);
                        }
                        return shape;
                    }
                }
            }
        }
    }
    
    // Return a default shape if not found
    Value defaultShape(kArrayType);
    defaultShape.PushBack(1, allocator);
    return defaultShape;
}

Value V6ModelParser::ExtractOutputShapeFromV6Graph(const rapidjson::Document& v6GraphInfo, const std::string& outputName, Document::AllocatorType& allocator) const {
    // Extract output shape from V6 graph for specific output name.
    // Python equivalent:
    // shape = npu_graph["outputs"][value["name"]]["shape"]
    
    if (v6GraphInfo.HasMember("graphs") && v6GraphInfo["graphs"].IsArray()) {
        const Value& graphs = v6GraphInfo["graphs"];
        for (SizeType i = 0; i < graphs.Size(); i++) {
            const Value& graph = graphs[i];
            if (graph.HasMember("name") && graph["name"].IsString() && 
                string(graph["name"].GetString()) == "npu_0") {
                if (graph.HasMember("outputs") && graph["outputs"].IsObject()) 
                {
                    const Value& outputs = graph["outputs"];
                    if (outputs.HasMember(outputName.c_str()) && outputs[outputName.c_str()].IsObject()) 
                    {
                        const Value& outputTensor = outputs[outputName.c_str()];
                        if (outputTensor.HasMember("shape") && outputTensor["shape"].IsArray()) 
                        {
                            Value shape(kArrayType);
                            const Value& outputShape = outputTensor["shape"];
                            for (SizeType j = 0; j < outputShape.Size(); j++) 
                            {
                                Value val;
                                val.CopyFrom(outputShape[j], allocator);
                                shape.PushBack(val, allocator);
                            }
                            return shape;
                        }
                    }
                }
                break; // Found npu_0, no need to continue
            }
        }
    }
    
    // Return a default shape if not found
    Value defaultShape(kArrayType);
    defaultShape.PushBack(1, allocator);
    return defaultShape;
}

std::pair<std::string, std::string> V6ModelParser::ParseV6Version(const std::string& versionStr) const {
    // Separate version and optimization level from a V6 version string.
    // e.g., "1.0.0(opt_level)" -> {"1.0.0", "opt_level"}

    auto pos = versionStr.find('(');
    if (pos != std::string::npos) 
    {
        std::string version = versionStr.substr(0, pos);
        std::string optLevel = versionStr.substr(pos + 1);

        // Remove trailing ')'
        if (!optLevel.empty() && optLevel.back() == ')') 
        {
            optLevel.pop_back();
        }

        return {version, optLevel};
    }

    return {versionStr, ""};
}

int V6ModelParser::loadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, ModelDataBase& data) const {
    Document document;
    string graphInfoBuffer;

    for (const auto& str : data.deepx_binary.graph_info().str())
    {
        graphInfoBuffer += str;
    }
    document.Parse(graphInfoBuffer.c_str());

    if (document.HasParseError()) 
    {
        LOG_DXRT_ERR("No graphinfo (" << document.GetParseError() << ")");
        return -1;
    }

    // offloading
    if (document.HasMember("offloading") && document["offloading"].IsBool())
    {
        param._use_offloading = document["offloading"].GetBool();
    }
    
    if (document.HasMember("inputs") && document["inputs"].IsArray()) 
    {
        parseStringArray(document["inputs"], param.inputs());
    }

    if (document.HasMember("outputs") && document["outputs"].IsArray()) 
    {
        parseStringArray(document["outputs"], param.outputs());
    }

    if (document.HasMember("toposort_order") && document["toposort_order"].IsArray()) 
    {
        parseStringArray(document["toposort_order"], param.topoSort_order());
    }

    if (document.HasMember("graphs") && document["graphs"].IsArray()) 
    {
        const rapidjson::Value& graphsArray = document["graphs"];
        param.subgraphs().clear();
        for (rapidjson::SizeType i = 0; i < graphsArray.Size(); i++) 
        {
            const rapidjson::Value& subGraphObj = graphsArray[i];
            deepx_graphinfo::SubGraph subGraph;

            if (subGraphObj.HasMember("name") && subGraphObj["name"].IsString())
            {
                subGraph.name() = subGraphObj["name"].GetString();
            }

            if (subGraphObj.HasMember("device") && subGraphObj["device"].IsString())
            {
                subGraph.device() = subGraphObj["device"].GetString();
            }

            if (subGraphObj.HasMember("inputs") && subGraphObj["inputs"].IsArray())
            {
                parseTensorArray(subGraphObj["inputs"], subGraph.inputs());
            }

            if (subGraphObj.HasMember("outputs") && subGraphObj["outputs"].IsArray()) 
            {
                parseTensorArray(subGraphObj["outputs"], subGraph.outputs());
            }

            param.subgraphs().push_back(subGraph);
        }
    }

    bool hasTailFlag = false;
    for (auto &sg : param.subgraphs()) 
    { 
        if (sg.tail()) 
        { 
            hasTailFlag = true; 
            break; 
        }
    }
    if (!hasTailFlag) 
    {
        std::set<std::string> modelInputs(param.inputs().begin(), param.inputs().end());
        std::set<std::string> modelOutputs(param.outputs().begin(), param.outputs().end());
        for (auto &sg : param.subgraphs()) 
        {
            for (auto &t : sg.outputs()) 
            {
                if (modelOutputs.count(t.name()) > 0) 
                { 
                    sg.tail() = true; 
                    break;
                }
            }
            for (auto &t : sg.inputs()) 
            {
                if (t.owner().empty() && modelInputs.count(t.name()) > 0) 
                { 
                    sg.head() = true; 
                    break; 
                }
            }
        }
    }

    return 0;
}

std::string V6ModelParser::loadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, ModelDataBase& data) const 
{
    Document document;
    string modelCompileType;

    for (size_t i = 0; i < data.deepx_binary.rmap_info().size(); i++) 
    {
        string rmapBuffer = "";
        for (const auto& str : data.deepx_binary.rmap_info(static_cast<int>(i)).str())
        {
            rmapBuffer += str;
        }

        document.Parse(rmapBuffer.c_str());
        if (document.HasParseError()) 
        {
            throw ModelParsingException(EXCEPTION_MESSAGE("rmapinfo parsing failed"));
        }

        deepx_rmapinfo::RegisterInfoDatabase regMap;
        parseSingleRmapInfo(document, regMap, modelCompileType, GetTaskBufferCount());
        param.rmap_info().push_back(regMap);
    }

    for(char& c: modelCompileType) 
    {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    
    return modelCompileType;
}

}  // namespace dxrt
