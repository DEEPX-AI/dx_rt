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
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "dxrt/parsers/v7_model_parser.h"
#include "dxrt/filesys_support.h"
#include "dxrt/exception/exception.h"
#include "dxrt/util.h"
#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/rapidjson.h"
#include "dxrt/common.h"
#include "../resource/log_messages.h"

using rapidjson::Document;
using rapidjson::Value;
using rapidjson::SizeType;
using std::string;

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

    // Helper: Parse compiled data for a single NPU from JSON
    void parseCompiledDataForNpu(const Value& npuData, const string& npuName, deepx_binaryinfo::BinaryInfoDatabase& param)
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

            if (value2.HasMember("rmap") && value2["rmap"].IsObject()) 
            {
                parseModelsOffsetSize(value2["rmap"], rmap);
                param.rmap().push_back(rmap);
            }

            if (value2.HasMember("weight") && value2["weight"].IsObject()) 
            {
                parseModelsOffsetSize(value2["weight"], weight);
                param.weight().push_back(weight);
            }

            if (value2.HasMember("rmap_info") && value2["rmap_info"].IsObject()) 
            {
                parseModelsOffsetSize(value2["rmap_info"], rmap_info);
                param.rmap_info().push_back(rmap_info);
            }

            if (value2.HasMember("bitmatch") && value2["bitmatch"].IsObject()) 
            {
                parseModelsOffsetSize(value2["bitmatch"], bitmatch_mask);
                param.bitmatch_mask().push_back(bitmatch_mask);
            }
        }
    }

    // Helper: Parse single subgraph from JSON object
    void parseSubGraph(const Value& subGraphObj, deepx_graphinfo::SubGraph& subGraph)
    {
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

        if (subGraphObj.HasMember("head") && subGraphObj["head"].IsBool())
        {
            subGraph.head() = subGraphObj["head"].GetBool();
        }

        if (subGraphObj.HasMember("tail") && subGraphObj["tail"].IsBool())
        {
            subGraph.tail() = subGraphObj["tail"].GetBool();
        }
    }

    // Helper: Build producer/consumer count maps for tensor relationship analysis
    void buildTensorUsageMaps(const std::vector<deepx_graphinfo::SubGraph>& subgraphs,
                              std::unordered_map<std::string, int>& producerCount,
                              std::unordered_map<std::string, int>& consumerCount)
    {
        for (const auto& sg : subgraphs) 
        {
            for (const auto& t : sg.outputs()) 
            {
                producerCount[t.name()] += 1;
                for (const auto& u : t.users()) 
                {   
                    consumerCount[u] += 1;
                }
            }
        }
    }

    // Helper: Check if subgraph should be marked as head
    bool shouldBeHead(const deepx_graphinfo::SubGraph& sg,
                      const std::unordered_set<std::string>& modelInputs,
                      const std::unordered_map<std::string, int>& producerCount)
    {
        if (sg.head()) 
        {
            return false; // Already marked, don't override
        }

        bool anyInputModelLevel = false;
        bool allInputsNoProducer = true;
        
        for (const auto& t : sg.inputs()) 
        {
            if (modelInputs.count(t.name())) 
            {
                anyInputModelLevel = true;
            }

            if (!t.owner().empty()) 
            {
                allInputsNoProducer = false;
            } 
            else if (producerCount.count(t.name()) && producerCount.at(t.name()) > 0) 
            {
                allInputsNoProducer = false;
            }
        }

        return anyInputModelLevel || allInputsNoProducer;
    }

    // Helper: Check if subgraph should be marked as tail
    bool shouldBeTail(const deepx_graphinfo::SubGraph& sg,
                      const std::unordered_set<std::string>& modelOutputs,
                      const std::unordered_map<std::string, int>& consumerCount)
    {
        if (sg.tail()) 
        {
            return false; // Already marked, don't override
        }

        bool anyOutputModelLevel = false;
        bool allOutputsNoConsumer = true;
        
        for (const auto& t : sg.outputs()) 
        {
            if (modelOutputs.count(t.name()))   
            {
                anyOutputModelLevel = true;
            }

            if (consumerCount.count(t.name()) && consumerCount.at(t.name()) > 0) 
            {
                allOutputsNoConsumer = false;
            }

            if (!t.users().empty()) 
            {
                allOutputsNoConsumer = false;
            }
        }

        return anyOutputModelLevel || allOutputsNoConsumer;
    }

    // Helper: Infer and set head/tail flags for subgraphs (backward compatibility)
    void inferHeadTailFlags(deepx_graphinfo::GraphInfoDatabase& param)
    {
        if (param.subgraphs().empty()) 
        {
            return;
        }

        std::unordered_set<std::string> modelInputs(param.inputs().begin(), param.inputs().end());
        std::unordered_set<std::string> modelOutputs(param.outputs().begin(), param.outputs().end());

        std::unordered_map<std::string, int> producerCount;
        std::unordered_map<std::string, int> consumerCount;
        buildTensorUsageMaps(param.subgraphs(), producerCount, consumerCount);

        for (auto& sg : param.subgraphs()) 
        {
            if (shouldBeHead(sg, modelInputs, producerCount)) 
            {
                sg.head() = true;
            }

            if (shouldBeTail(sg, modelOutputs, consumerCount)) 
            {
                sg.tail() = true;
            }
        }
    }

    // ============================================================================
    // RmapInfo Related Functions
    // ============================================================================

    // Helper: Parse TensorInfo fields from JSON object (V7 specific)
    void parseTensorInfoFields(const Value& tensorObj, deepx_rmapinfo::TensorInfo& tensor)
    {
        if (tensorObj.HasMember("name") && tensorObj["name"].IsString())
        {
            tensor.name() = tensorObj["name"].GetString();
        }

        if (tensorObj.HasMember("dtype") && tensorObj["dtype"].IsString()) 
        {
            tensor.dtype() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype"].GetString());
            tensor.elem_size() = GetDataSize_Datatype(static_cast<DataType>(tensor.dtype()));
        }

        if (tensorObj.HasMember("shape") && tensorObj["shape"].IsArray()) 
        {
            const Value& shapeArr = tensorObj["shape"];
            for (SizeType j = 0; j < shapeArr.Size(); j++) 
            {
                tensor.shape().push_back(shapeArr[j].GetInt64());
            }
        }

        if (tensorObj.HasMember("name_encoded") && tensorObj["name_encoded"].IsString())
        {
            tensor.name_encoded() = tensorObj["name_encoded"].GetString();
        }

        if (tensorObj.HasMember("dtype_encoded") && tensorObj["dtype_encoded"].IsString())
        {
            tensor.dtype_encoded() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype_encoded"].GetString());
        }

        if (tensorObj.HasMember("shape_encoded") && tensorObj["shape_encoded"].IsArray()) 
        {
            const Value& shapeArr = tensorObj["shape_encoded"];
            for (SizeType j = 0; j < shapeArr.Size(); j++) 
            {
                tensor.shape_encoded().push_back(shapeArr[j].GetInt64());
            }
        }

        if (tensorObj.HasMember("layout") && tensorObj["layout"].IsString())
        {
            tensor.layout() = deepx_rmapinfo::GetLayoutNum(tensorObj["layout"].GetString());
        }

        if (tensorObj.HasMember("align_unit") && tensorObj["align_unit"].IsInt())
        {
            tensor.align_unit() = tensorObj["align_unit"].GetInt();
        }

        if (tensorObj.HasMember("transpose") && tensorObj["transpose"].IsString())
        {
            tensor.transpose() = deepx_rmapinfo::GetTransposeNum(tensorObj["transpose"].GetString());
        }

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

    // Helper: Process PPU output tensor
    void processPpuOutput(deepx_rmapinfo::TensorInfo& tensor)
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
        
        int dataType = DataType::BBOX;
        dataType += tensor.layout();
        dataType -= deepx_rmapinfo::Layout::PPU_YOLO;
        tensor.dtype() = dataType;
    }

    // Helper: Validate ARGMAX output shape
    void validateArgmaxOutputShape(const Value& tensorObj, deepx_rmapinfo::TensorInfo& tensor)
    {
        if (!tensorObj.HasMember("shape_encoded") || !tensorObj["shape_encoded"].IsArray()) 
        {
            return;
        }

        const rapidjson::Value& shapeArr = tensorObj["shape_encoded"];
        int64_t product = shapeArr[0].GetInt64();
        int elementSize = getElementSize(tensor.dtype_encoded());
        product *= elementSize;

        if (product != tensor.memory().size()) 
        {
            throw ModelParsingException(EXCEPTION_MESSAGE("invalid output shape in rmap_info"));
        }
    }

    // Helper: Validate default output shape
    void validateDefaultOutputShape(const Value& tensorObj, deepx_rmapinfo::TensorInfo& tensor)
    {
        if (!tensorObj.HasMember("shape_encoded") || !tensorObj["shape_encoded"].IsArray()) 
        {
            return;
        }

        const rapidjson::Value& shapeArr = tensorObj["shape_encoded"];
        int64_t product = 1;
        
        for (rapidjson::SizeType j = 0; j < shapeArr.Size(); j++) 
        {
            int64_t value = shapeArr[j].GetInt64();
            if (j == shapeArr.Size() - 1)
            {
                value = GetAlign(value, tensor.align_unit());
            }
            product *= value;
        }
        
        int elementSize = getElementSize(tensor.dtype_encoded());
        product *= elementSize;

        if (product != tensor.memory().size()) 
        {
            throw ModelParsingException(EXCEPTION_MESSAGE("invalid output shape in rmap_info"));
        }
    }

    // Helper: Process output tensor based on memory type
    void processOutputTensor(const Value& tensorObj, deepx_rmapinfo::TensorInfo& tensor)
    {
        const auto memType = tensor.memory().type();
        
        if (memType == deepx_rmapinfo::MemoryType::PPU) 
        {
            processPpuOutput(tensor);
            return;
        }
        
        if (memType == deepx_rmapinfo::MemoryType::ARGMAX)
        {
            validateArgmaxOutputShape(tensorObj, tensor);
            return;
        }
        
        validateDefaultOutputShape(tensorObj, tensor);
    }


    // ============================================================================
    // Top-level Parser Functions
    // ============================================================================

    // Helper: Parse data section from header document
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
            const Value &compiledData = dataObj["compiled_data"];
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

    // Helper: Parse single rmap_info document into RegisterInfoDatabase
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
                parseTensorInfoFields(tensorObj, tensor);
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
                parseTensorInfoFields(tensorObj, tensor);
                processOutputTensor(tensorObj, tensor);
                regMap.outputs().push_back(tensor);
            }
        }
    }
}

// ============================================================================
// Main Parsing Functions
// ============================================================================

int V7ModelParser::loadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, const char *buffer, int fileSize) const {
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

    if (dxnnFileFormatVersion < MIN_SINGLEFILE_VERSION || dxnnFileFormatVersion > MAX_SINGLEFILE_VERSION)
    {
        throw ModelParsingException(EXCEPTION_MESSAGE(LogMessages::NotSupported_ModelFileFormatVersion(dxnnFileFormatVersion, MIN_SINGLEFILE_VERSION, MAX_SINGLEFILE_VERSION)));
    }

    if (dxnnFileFormatVersion != 7) 
    {
        throw ModelParsingException(EXCEPTION_MESSAGE("V7ModelParser can only parse version 7 files"));
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

int V7ModelParser::loadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, ModelDataBase& data) const {
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
            deepx_graphinfo::SubGraph subGraph;
            parseSubGraph(graphsArray[i], subGraph);
            param.subgraphs().push_back(subGraph);
        }
    }

    // Fallback inference for missing head/tail flags (backward compatibility)
    inferHeadTailFlags(param);

    return 0;
}

std::string V7ModelParser::loadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, ModelDataBase& data) const
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

    for (char& c : modelCompileType) 
    {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return modelCompileType;
}

}  // namespace dxrt
