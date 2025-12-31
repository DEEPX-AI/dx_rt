/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/parsers/v8_model_parser.h"

#include <fstream>
#include <cstring>
#include <string>

#include "dxrt/filesys_support.h"
#include "dxrt/exception/exception.h"
#include "dxrt/util.h"
#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/rapidjson.h"
#include "../resource/log_messages.h"
#include "dxrt/common.h"

// Add missing constants
#ifndef MAX_CHECKPOINT_COUNT
#define MAX_CHECKPOINT_COUNT 3
#endif

#ifndef DXRT_TASK_MAX_LOAD
#define DXRT_TASK_MAX_LOAD 6
#endif

using rapidjson::Document;
using rapidjson::Value;
using rapidjson::SizeType;
using std::string;

namespace dxrt {

std::string V8ModelParser::ParseModel(const std::string& filePath, ModelDataBase& modelData) {
    if (!fileExists(filePath) || getExtension(filePath) != "dxnn") {
        throw FileNotFoundException(EXCEPTION_MESSAGE("Invalid model path : " + filePath));
    }

    int fileSize = getFileSize(filePath);
    std::vector<char> vbuf(fileSize);
    char *buf = vbuf.data();

    FILE *fp = fopen(filePath.c_str(), "rb");
    if (!fp) {
        throw FileNotFoundException(EXCEPTION_MESSAGE("Failed to open file: " + filePath));
    }

    std::ignore = fread(static_cast<void*>(buf), fileSize, 1, fp);
    fclose(fp);

    return V8ModelParser::ParseModel((const uint8_t*)buf, fileSize, modelData);
}

std::string V8ModelParser::ParseModel(const uint8_t* modelBuffer, size_t modelSize, ModelDataBase& modelData) {
    LoadBinaryInfo(modelData.deepx_binary, (char*)modelBuffer, modelSize);

    LoadGraphInfo(modelData.deepx_graph, modelData);

    string modelCompileType = LoadRmapInfo(modelData.deepx_rmap, modelData);

    return modelCompileType;
}

int V8ModelParser::LoadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, char *buffer, int fileSize) {
    Document document;
    std::ignore = fileSize;

    int offset = 0, sizeInfo = 8192;
    string signInfo, headerInfo;

    signInfo = string(buffer, 4);
    offset += 8;
    if (signInfo != "DXNN") {
        throw InvalidModelException(EXCEPTION_MESSAGE(LogMessages::InvalidDXNNFileFormat()));
    }

    // dxnn file format version 4byte integer little-endian
    int32_t dxnnFileFormatVersion = (int32_t)(buffer[4] |
                buffer[5] << 8  |
                buffer[6] << 16 |
                buffer[7] << 24);
    param._dxnnFileFormatVersion = dxnnFileFormatVersion;

    if (dxnnFileFormatVersion < MIN_SINGLEFILE_VERSION || dxnnFileFormatVersion > MAX_SINGLEFILE_VERSION)
    {
        throw ModelParsingException(EXCEPTION_MESSAGE(LogMessages::NotSupported_ModelFileFormatVersion(dxnnFileFormatVersion, MIN_SINGLEFILE_VERSION, MAX_SINGLEFILE_VERSION)));
    }

    if (dxnnFileFormatVersion != 8) {
        throw ModelParsingException(EXCEPTION_MESSAGE("V8ModelParser can only parse version 8 files"));
    }

    sizeInfo = 8192;
    headerInfo = string(buffer+offset, sizeInfo-offset);
    offset = sizeInfo;

    document.Parse(headerInfo.c_str());
    if (document.HasParseError()) {
        throw ModelParsingException(
            EXCEPTION_MESSAGE(LogMessages::InvalidDXNNModelHeader(static_cast<int>(document.GetParseError())))
        );
    }

    if (document.HasMember("data") && document["data"].IsObject()) {
        const Value &dataObj = document["data"];

        // [Field] - cpu models
#ifdef USE_ORT
        if (dataObj.HasMember("cpu_models") && dataObj["cpu_models"].IsObject()) {
            const Value &cpuModelsObj = dataObj["cpu_models"];
            int i = 0;
            for (Value::ConstMemberIterator iter = cpuModelsObj.MemberBegin(); iter != cpuModelsObj.MemberEnd(); ++iter) {
                if (iter->name.IsString()) {
                    deepx_binaryinfo::Models model;
                    model.name() = iter->name.GetString();
                    const Value& value = iter->value;
                    if (value.HasMember("offset") && value["offset"].IsInt64())
                        model.offset() = value["offset"].GetInt64();
                    if (value.HasMember("size") && value["size"].IsInt64())
                        model.size() = value["size"].GetInt64();
                    param.cpu_models().push_back(model);
                }
                i++;
            }
        }
#endif

        // [Field] - compile config
        if (dataObj.HasMember("compile_config") && dataObj["compile_config"].IsObject()) {
            const Value &compileConfiglObj = dataObj["compile_config"];
            int64_t cc_offset = 0, cc_size = 0;
            if (compileConfiglObj.HasMember("offset")) {
                if (compileConfiglObj["offset"].IsInt64())
                    cc_offset = compileConfiglObj["offset"].GetInt64();
                else if (compileConfiglObj["offset"].IsInt())
                    cc_offset = compileConfiglObj["offset"].GetInt();
                else if (compileConfiglObj["offset"].IsString())
                    cc_offset = std::stoll(compileConfiglObj["offset"].GetString());
            }
            if (compileConfiglObj.HasMember("size")) {
                if (compileConfiglObj["size"].IsInt64())
                    cc_size = compileConfiglObj["size"].GetInt64();
                else if (compileConfiglObj["size"].IsInt())
                    cc_size = compileConfiglObj["size"].GetInt();
                else if (compileConfiglObj["size"].IsString())
                    cc_size = std::stoll(compileConfiglObj["size"].GetString());
            }

            Document compile_config_document;
            std::string compile_config_str = string(buffer+cc_offset+offset, cc_size);
            compile_config_document.Parse(compile_config_str.c_str());
            if (compile_config_document.HasMember("compile_version") && compile_config_document["compile_version"].IsString()) {
                const Value &compileVersionObj = compile_config_document["compile_version"];
                param._compilerVersion = compileVersionObj.GetString();
            }
            
            // v8: Read PPU type from compile_config.json
            if (compile_config_document.HasMember("ppu") && !compile_config_document["ppu"].IsNull()) {
                const Value &ppuObj = compile_config_document["ppu"];
                if (ppuObj.IsObject() && ppuObj.HasMember("type")) {
                    if (ppuObj["type"].IsInt()) {
                        param._ppuType = ppuObj["type"].GetInt();
                        LOG_DXRT_DBG << "V8: PPU type from compile_config: " << param._ppuType << std::endl;
                    }
                }
            }
        }

        // [Field] - graph info
        if (dataObj.HasMember("graph_info") && dataObj["graph_info"].IsObject()) {
            const Value &graphInfolObj = dataObj["graph_info"];
            if (graphInfolObj.HasMember("offset")) {
                if (graphInfolObj["offset"].IsInt64())
                    param.graph_info().offset() = graphInfolObj["offset"].GetInt64();
                else if (graphInfolObj["offset"].IsInt())
                    param.graph_info().offset() = graphInfolObj["offset"].GetInt();
                else if (graphInfolObj["offset"].IsString())
                    param.graph_info().offset() = std::stoll(graphInfolObj["offset"].GetString());
            }
            if (graphInfolObj.HasMember("size")) {
                if (graphInfolObj["size"].IsInt64())
                    param.graph_info().size() = graphInfolObj["size"].GetInt64();
                else if (graphInfolObj["size"].IsInt())
                    param.graph_info().size() = graphInfolObj["size"].GetInt();
                else if (graphInfolObj["size"].IsString())
                    param.graph_info().size() = std::stoll(graphInfolObj["size"].GetString());
            }
        }

        // [Field] - compiled data (v8: includes optional PPU binary)
        if (dataObj.HasMember("compiled_data") && dataObj["compiled_data"].IsObject()) {
            const Value &compiledData = dataObj["compiled_data"];
            for (Value::ConstMemberIterator iter = compiledData.MemberBegin(); iter != compiledData.MemberEnd(); ++iter) {
                if (iter->name.IsString()) {
                    deepx_binaryinfo::Models rmap;
                    deepx_binaryinfo::Models weight;
                    deepx_binaryinfo::Models rmap_info;
                    deepx_binaryinfo::Models bitmatch_mask;
                    deepx_binaryinfo::Models ppu;  // v8: new PPU binary
                    
                    rmap.npu() = weight.npu() = rmap_info.npu() = bitmatch_mask.npu() = ppu.npu() = iter->name.GetString();
                    const Value& value = iter->value;

                    for (Value::ConstMemberIterator iter2 = value.MemberBegin(); iter2 != value.MemberEnd(); ++iter2) {
                        if (iter2->name.IsString()) {
                            rmap.name() = weight.name() = rmap_info.name() = bitmatch_mask.name() = ppu.name() = iter2->name.GetString();
                            const Value& value2 = iter2->value;

                            // [Sub-Field] - rmap
                            if (value2.HasMember("rmap") && value2["rmap"].IsObject()) {
                                const Value &rmapObj = value2["rmap"];
                                if (rmapObj.HasMember("offset")) {
                                    if (rmapObj["offset"].IsInt64())
                                        rmap.offset() = rmapObj["offset"].GetInt64();
                                    else if (rmapObj["offset"].IsInt())
                                        rmap.offset() = rmapObj["offset"].GetInt();
                                    else if (rmapObj["offset"].IsString())
                                        rmap.offset() = std::stoll(rmapObj["offset"].GetString());
                                }
                                if (rmapObj.HasMember("size")) {
                                    if (rmapObj["size"].IsInt64())
                                        rmap.size() = rmapObj["size"].GetInt64();
                                    else if (rmapObj["size"].IsInt())
                                        rmap.size() = rmapObj["size"].GetInt();
                                    else if (rmapObj["size"].IsString())
                                        rmap.size() = std::stoll(rmapObj["size"].GetString());
                                }
                                param.rmap().push_back(rmap);
                            }

                            // [Sub-Field] - weight
                            if (value2.HasMember("weight") && value2["weight"].IsObject()) {
                                const Value &weightObj = value2["weight"];
                                if (weightObj.HasMember("offset")) {
                                    if (weightObj["offset"].IsInt64())
                                        weight.offset() = weightObj["offset"].GetInt64();
                                    else if (weightObj["offset"].IsInt())
                                        weight.offset() = weightObj["offset"].GetInt();
                                    else if (weightObj["offset"].IsString())
                                        weight.offset() = std::stoll(weightObj["offset"].GetString());
                                }
                                if (weightObj.HasMember("size")) {
                                    if (weightObj["size"].IsInt64())
                                        weight.size() = weightObj["size"].GetInt64();
                                    else if (weightObj["size"].IsInt())
                                        weight.size() = weightObj["size"].GetInt();
                                    else if (weightObj["size"].IsString())
                                        weight.size() = std::stoll(weightObj["size"].GetString());
                                }
                                param.weight().push_back(weight);
                            }

                            // [Sub-Field] - rmap info
                            if (value2.HasMember("rmap_info") && value2["rmap_info"].IsObject()) {
                                const Value &rmapInfoObj = value2["rmap_info"];
                                if (rmapInfoObj.HasMember("offset")) {
                                    if (rmapInfoObj["offset"].IsInt64())
                                        rmap_info.offset() = rmapInfoObj["offset"].GetInt64();
                                    else if (rmapInfoObj["offset"].IsInt())
                                        rmap_info.offset() = rmapInfoObj["offset"].GetInt();
                                    else if (rmapInfoObj["offset"].IsString())
                                        rmap_info.offset() = std::stoll(rmapInfoObj["offset"].GetString());
                                }
                                if (rmapInfoObj.HasMember("size")) {
                                    if (rmapInfoObj["size"].IsInt64())
                                        rmap_info.size() = rmapInfoObj["size"].GetInt64();
                                    else if (rmapInfoObj["size"].IsInt())
                                        rmap_info.size() = rmapInfoObj["size"].GetInt();
                                    else if (rmapInfoObj["size"].IsString())
                                        rmap_info.size() = std::stoll(rmapInfoObj["size"].GetString());
                                }
                                param.rmap_info().push_back(rmap_info);
                            }

                            // [Sub-Field] - bit match mask
                            if (value2.HasMember("bitmatch") && value2["bitmatch"].IsObject()) {
                                const Value &bitmatchObj = value2["bitmatch"];
                                if (bitmatchObj.HasMember("offset")) {
                                    if (bitmatchObj["offset"].IsInt64())
                                        bitmatch_mask.offset() = bitmatchObj["offset"].GetInt64();
                                    else if (bitmatchObj["offset"].IsInt())
                                        bitmatch_mask.offset() = bitmatchObj["offset"].GetInt();
                                    else if (bitmatchObj["offset"].IsString())
                                        bitmatch_mask.offset() = std::stoll(bitmatchObj["offset"].GetString());
                                }
                                if (bitmatchObj.HasMember("size")) {
                                    if (bitmatchObj["size"].IsInt64())
                                        bitmatch_mask.size() = bitmatchObj["size"].GetInt64();
                                    else if (bitmatchObj["size"].IsInt())
                                        bitmatch_mask.size() = bitmatchObj["size"].GetInt();
                                    else if (bitmatchObj["size"].IsString())
                                        bitmatch_mask.size() = std::stoll(bitmatchObj["size"].GetString());
                                }
                                param.bitmatch_mask().push_back(bitmatch_mask);
                            }

                            // [Sub-Field] - ppu (v8 new field, optional)
                            if (value2.HasMember("ppu") && value2["ppu"].IsObject()) {
                                const Value &ppuObj = value2["ppu"];
                                if (ppuObj.HasMember("offset")) {
                                    if (ppuObj["offset"].IsInt64())
                                        ppu.offset() = ppuObj["offset"].GetInt64();
                                    else if (ppuObj["offset"].IsInt())
                                        ppu.offset() = ppuObj["offset"].GetInt();
                                    else if (ppuObj["offset"].IsString())
                                        ppu.offset() = std::stoll(ppuObj["offset"].GetString());
                                }
                                if (ppuObj.HasMember("size")) {
                                    if (ppuObj["size"].IsInt64())
                                        ppu.size() = ppuObj["size"].GetInt64();
                                    else if (ppuObj["size"].IsInt())
                                        ppu.size() = ppuObj["size"].GetInt();
                                    else if (ppuObj["size"].IsString())
                                        ppu.size() = std::stoll(ppuObj["size"].GetString());
                                }
                                
                                // Only add to ppu vector if it has valid size
                                if (ppu.size() > 0) {
                                    param.ppu().push_back(ppu);
                                    LOG_DXRT_DBG << "V8: PPU binary found - NPU: " << ppu.npu() 
                                                 << ", Task: " << ppu.name() 
                                                 << ", Size: " << ppu.size() << " bytes" << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // [Buffer] - CPU Binary Data
    for (size_t i = 0; i < param.cpu_models().size(); i++) {
        param.cpu_models(i)._buffer.resize(param.cpu_models(i).size());
        memcpy(param.cpu_models(i)._buffer.data(), buffer + (offset + param.cpu_models(i).offset()), param.cpu_models(i).size());
    }

    // [Buffer] - Graph Info.
    std::unique_ptr<char[]> graphInfoBuf(new char[param.graph_info().size()]);
    memcpy(graphInfoBuf.get(), buffer + (offset + param.graph_info().offset()), param.graph_info().size());
    string graphInfoStr(&graphInfoBuf[0], param.graph_info().size());
    param.graph_info().str() = graphInfoStr;

    // [Buffer] - RMAP Binary Data
    for (size_t i = 0; i < param.rmap().size(); i++) {
        param.rmap(i)._buffer.resize(param.rmap(i).size());
        memcpy(param.rmap(i)._buffer.data(), buffer + (offset + param.rmap(i).offset()), param.rmap(i).size());
    }

    // [Buffer] - Weight Binary Data
    for (size_t i = 0; i < param.weight().size(); i++) {
        param.weight(i)._buffer.resize(param.weight(i).size());
        memcpy(param.weight(i)._buffer.data(), buffer + (offset + param.weight(i).offset()), param.weight(i).size());
    }

    // [Buffer] - RMAP Info.
    for (size_t i = 0; i < param.rmap_info().size(); i++) {
        std::unique_ptr<char[]> rmapInfoBuf(new char[param.rmap_info(i).size()]);
        memcpy(rmapInfoBuf.get(), buffer + (offset + param.rmap_info(i).offset()), param.rmap_info(i).size());
        string rmapInfoStr(&rmapInfoBuf[0], param.rmap_info(i).size());
        param.rmap_info(i).str() = rmapInfoStr;
    }

    // [Buffer] - Bitmatch Mask.
    for (size_t i = 0; i < param.bitmatch_mask().size(); i++) {
        param.bitmatch_mask(i)._buffer.resize(param.bitmatch_mask(i).size());
        memcpy(param.bitmatch_mask(i)._buffer.data(), buffer + (offset + param.bitmatch_mask(i).offset()), param.bitmatch_mask(i).size());
    }

    // [Buffer] - PPU Binary Data (v8 new)
    for (size_t i = 0; i < param.ppu().size(); i++) {
        param.ppu(i)._buffer.resize(param.ppu(i).size());
        memcpy(param.ppu(i)._buffer.data(), buffer + (offset + param.ppu(i).offset()), param.ppu(i).size());
        LOG_DXRT_DBG << "V8: PPU binary loaded - index: " << i 
                     << ", size: " << param.ppu(i).size() << " bytes" << std::endl;
    }

    return dxnnFileFormatVersion;
}

int V8ModelParser::LoadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, ModelDataBase& data) {
    Document document;
    string graphInfoBuffer;

    for (const auto& str : data.deepx_binary.graph_info().str())
        graphInfoBuffer += str;
    document.Parse(graphInfoBuffer.c_str());

    if (document.HasParseError()) {
        LOG_DXRT_ERR("No graphinfo (" << document.GetParseError() << ")");
        return -1;
    }

    // offloading
    if (document.HasMember("offloading") && document["offloading"].IsBool())
        param._use_offloading = document["offloading"].GetBool();

    if (document.HasMember("inputs") && document["inputs"].IsArray()) {
        const rapidjson::Value& inputsArray = document["inputs"];
        param.inputs().clear();
        for (rapidjson::SizeType i = 0; i < inputsArray.Size(); i++) {
            if (inputsArray[i].IsString())
                param.inputs().push_back(inputsArray[i].GetString());
        }
    }

    if (document.HasMember("outputs") && document["outputs"].IsArray()) {
        const rapidjson::Value& outputsArray = document["outputs"];
        param.outputs().clear();
        for (rapidjson::SizeType i = 0; i < outputsArray.Size(); i++) {
            if (outputsArray[i].IsString())
                param.outputs().push_back(outputsArray[i].GetString());
        }
    }

    if (document.HasMember("toposort_order") && document["toposort_order"].IsArray()) {
        const rapidjson::Value& orderArray = document["toposort_order"];
        param.topoSort_order().clear();
        for (rapidjson::SizeType i = 0; i < orderArray.Size(); i++) {
            if (orderArray[i].IsString())
                param.topoSort_order().push_back(orderArray[i].GetString());
        }
    }

    if (document.HasMember("graphs") && document["graphs"].IsArray()) {
        const rapidjson::Value& graphsArray = document["graphs"];
        param.subgraphs().clear();
        for (rapidjson::SizeType i = 0; i < graphsArray.Size(); i++) {
            const rapidjson::Value& subGraphObj = graphsArray[i];
            deepx_graphinfo::SubGraph subGraph;

            if (subGraphObj.HasMember("name") && subGraphObj["name"].IsString())
                subGraph.name() = subGraphObj["name"].GetString();

            if (subGraphObj.HasMember("device") && subGraphObj["device"].IsString())
                subGraph.device() = subGraphObj["device"].GetString();

            if (subGraphObj.HasMember("inputs") && subGraphObj["inputs"].IsArray()) {
                const rapidjson::Value& tensorArray = subGraphObj["inputs"];
                for (rapidjson::SizeType j = 0; j < tensorArray.Size(); j++) {
                    const rapidjson::Value& tensorObj = tensorArray[j];
                    deepx_graphinfo::Tensor tensor;

                    if (tensorObj.HasMember("name") && tensorObj["name"].IsString())
                        tensor.name() = tensorObj["name"].GetString();

                    if (tensorObj.HasMember("owner") && tensorObj["owner"].IsString())
                        tensor.owner() = tensorObj["owner"].GetString();

                    if (tensorObj.HasMember("users") && tensorObj["users"].IsArray()) {
                        const rapidjson::Value& usersArray = tensorObj["users"];
                        for (rapidjson::SizeType k = 0; k < usersArray.Size(); k++) {
                            if (usersArray[k].IsString())
                                tensor.users().push_back(usersArray[k].GetString());
                        }
                    }

                    subGraph.inputs().push_back(tensor);
                }
            }

            if (subGraphObj.HasMember("outputs") && subGraphObj["outputs"].IsArray()) {
                const rapidjson::Value& tensorArray = subGraphObj["outputs"];
                for (rapidjson::SizeType j = 0; j < tensorArray.Size(); j++) {
                    const rapidjson::Value& tensorObj = tensorArray[j];
                    deepx_graphinfo::Tensor tensor;

                    if (tensorObj.HasMember("name") && tensorObj["name"].IsString())
                        tensor.name() = tensorObj["name"].GetString();

                    if (tensorObj.HasMember("owner") && tensorObj["owner"].IsString())
                        tensor.owner() = tensorObj["owner"].GetString();

                    if (tensorObj.HasMember("users") && tensorObj["users"].IsArray()) {
                        const rapidjson::Value& usersArray = tensorObj["users"];
                        for (rapidjson::SizeType k = 0; k < usersArray.Size(); k++) {
                            if (usersArray[k].IsString())
                                tensor.users().push_back(usersArray[k].GetString());
                        }
                    }

                    subGraph.outputs().push_back(tensor);
                }
            }

            // Parse head and tail flags from graph_info
            if (subGraphObj.HasMember("head") && subGraphObj["head"].IsBool())
                subGraph.head() = subGraphObj["head"].GetBool();

            if (subGraphObj.HasMember("tail") && subGraphObj["tail"].IsBool())
                subGraph.tail() = subGraphObj["tail"].GetBool();

            param.subgraphs().push_back(subGraph);
        }
    }

    return 0;
}

std::string V8ModelParser::LoadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, ModelDataBase& data) {
    Document document;
    string modelCompileType;

    for (size_t i = 0; i < data.deepx_binary.rmap_info().size(); i++) {
        string rmapBuffer = "";
        for (const auto& str : data.deepx_binary.rmap_info(i).str())
            rmapBuffer += str;

        document.Parse(rmapBuffer.c_str());
        if (document.HasParseError()) {
            throw ModelParsingException(EXCEPTION_MESSAGE("rmapinfo parsing failed"));
        }

        deepx_rmapinfo::RegisterInfoDatabase regMap;

        // version
        if (document.HasMember("version") && document["version"].IsObject()) {
            const rapidjson::Value& versionObj = document["version"];
            if (versionObj.HasMember("npu") && versionObj["npu"].IsString())
                regMap.version().npu() = versionObj["npu"].GetString();
            if (versionObj.HasMember("rmap") && versionObj["rmap"].IsString())
                regMap.version().rmap() = versionObj["rmap"].GetString();
            if (versionObj.HasMember("rmapInfo") && versionObj["rmapInfo"].IsString())
                regMap.version().rmap_info() = versionObj["rmapInfo"].GetString();
            if (versionObj.HasMember("opt_level") && versionObj["opt_level"].IsString())
                regMap.version().opt_level() = versionObj["opt_level"].GetString();
        }

        // name
        if (document.HasMember("name") && document["name"].IsString())
            regMap.name() = document["name"].GetString();

        // mode
        if (document.HasMember("mode") && document["mode"].IsString()) {
            modelCompileType = document["mode"].GetString();
            regMap.mode() = modelCompileType;
        }

        // npu
        if (document.HasMember("npu") && document["npu"].IsObject()) {
            const rapidjson::Value& npuObj = document["npu"];
            if (npuObj.HasMember("mac") && npuObj["mac"].IsInt64())
                regMap.npu().mac() = npuObj["mac"].GetInt64();
        }

        // size
        if (document.HasMember("size") && document["size"].IsInt64()) {
            regMap.size() = document["size"].GetInt64();
        } else {
            regMap.size() = 0;
        }


        // counts
        if (document.HasMember("counts") && document["counts"].IsObject()) {
            const rapidjson::Value& countsObj = document["counts"];
            if (countsObj.HasMember("layer") && countsObj["layer"].IsInt64())
                regMap.counts().layer() = countsObj["layer"].GetInt64();
            if (countsObj.HasMember("cmd") && countsObj["cmd"].IsInt64())
                regMap.counts().cmd() = countsObj["cmd"].GetInt64();
            if (countsObj.HasMember("checkpoints") && countsObj["checkpoints"].IsArray()) {
                regMap.counts()._op_mode = 1;
                const Value& listObj = countsObj["checkpoints"];
                for (SizeType j = 0; j < MAX_CHECKPOINT_COUNT; j++) {
                    if (j >= listObj.Size()) break;
                    regMap.counts()._checkpoints[j] = listObj[j].GetUint64();
                }
            } else {
                regMap.counts()._op_mode = 0;
            }
        }

        // model memory - support both "model_memory" object and "memory" array formats
        if (document.HasMember("model_memory") && document["model_memory"].IsObject()) {
            const rapidjson::Value& modelMemObj = document["model_memory"];

            if (modelMemObj.HasMember("input") && modelMemObj["input"].IsObject()) {
                const rapidjson::Value& inputObj = modelMemObj["input"];
                deepx_rmapinfo::Memory mem;
                if (inputObj.HasMember("offset") && inputObj["offset"].IsInt64())
                    mem.offset() = inputObj["offset"].GetInt64();
                if (inputObj.HasMember("size") && inputObj["size"].IsInt64())
                    mem.size() = inputObj["size"].GetInt64();
                regMap.model_memory().input() = mem;
            }

            if (modelMemObj.HasMember("output") && modelMemObj["output"].IsObject()) {
                const rapidjson::Value& outputObj = modelMemObj["output"];
                deepx_rmapinfo::Memory mem;
                if (outputObj.HasMember("offset") && outputObj["offset"].IsInt64())
                    mem.offset() = outputObj["offset"].GetInt64();
                if (outputObj.HasMember("size") && outputObj["size"].IsInt64())
                    mem.size() = outputObj["size"].GetInt64();
                regMap.model_memory().output() = mem;
            }

            if (modelMemObj.HasMember("rmap") && modelMemObj["rmap"].IsObject()) {
                const rapidjson::Value& rmapObj = modelMemObj["rmap"];
                deepx_rmapinfo::Memory mem;
                if (rmapObj.HasMember("offset") && rmapObj["offset"].IsInt64())
                    mem.offset() = rmapObj["offset"].GetInt64();
                if (rmapObj.HasMember("size") && rmapObj["size"].IsInt64())
                    mem.size() = rmapObj["size"].GetInt64();
                regMap.model_memory().rmap() = mem;
            }

            if (modelMemObj.HasMember("weight") && modelMemObj["weight"].IsObject()) {
                const rapidjson::Value& weightObj = modelMemObj["weight"];
                deepx_rmapinfo::Memory mem;
                if (weightObj.HasMember("offset") && weightObj["offset"].IsInt64())
                    mem.offset() = weightObj["offset"].GetInt64();
                if (weightObj.HasMember("size") && weightObj["size"].IsInt64())
                    mem.size() = weightObj["size"].GetInt64();
                regMap.model_memory().weight() = mem;
            }
        }
        // V8: Support "memory" array format (alternative to "model_memory" object)
        else if (document.HasMember("memory") && document["memory"].IsArray()) {
            const rapidjson::Value& memoryArray = document["memory"];
            for (rapidjson::SizeType j = 0; j < memoryArray.Size(); j++) {
                const rapidjson::Value& memObj = memoryArray[j];
                if (!memObj.HasMember("name") || !memObj["name"].IsString())
                    continue;
                
                deepx_rmapinfo::Memory memory;
                memory.name() = memObj["name"].GetString();
                
                if (memObj.HasMember("offset") && memObj["offset"].IsInt64()) {
                    memory.offset() = memObj["offset"].GetInt64();
                    if (memory.offset() != 0 && memory.name() != "TEMP")
                        LOG_DXRT_ERR(LogMessages::ModelParser_OutputOffsetIsNotZero());
                }
                
                if (memObj.HasMember("size") && memObj["size"].IsInt64()) {
                    memory.size() = memObj["size"].GetInt64();
                }
                
                if (memObj.HasMember("type") && memObj["type"].IsString())
                    memory.type() = deepx_rmapinfo::GetMemoryTypeNum(memObj["type"].GetString());
                
                // Map array entries to model_memory fields with size calculation
                if (memory.name() == "RMAP") {
                    regMap.model_memory().rmap() = memory;
                    regMap.model_memory().model_memory_size() += memory.size();
                } else if (memory.name() == "WEIGHT") {
                    regMap.model_memory().weight() = memory;
                    regMap.model_memory().model_memory_size() += memory.size();
                } else if (memory.name() == "INPUT") {
                    regMap.model_memory().input() = memory;
                    regMap.model_memory().model_memory_size() += memory.size() * _taskBufferCount; // DXRT_TASK_MAX_LOAD;
                } else if (memory.name() == "OUTPUT") {
                    regMap.model_memory().output() = memory;
                    regMap.model_memory().model_memory_size() += memory.size() * _taskBufferCount; // DXRT_TASK_MAX_LOAD;
                } else if (memory.name() == "TEMP") {
                    regMap.model_memory().temp() = memory;
                    regMap.model_memory().model_memory_size() += memory.size();
                }
            }
        }

        // inputs
        if (document.HasMember("inputs") && document["inputs"].IsArray()) {
            const rapidjson::Value& inputsArray = document["inputs"];
            regMap.inputs().clear();
            for (rapidjson::SizeType j = 0; j < inputsArray.Size(); j++) {
                const rapidjson::Value& tensorObj = inputsArray[j];
                deepx_rmapinfo::TensorInfo tensor;

                if (tensorObj.HasMember("name") && tensorObj["name"].IsString())
                    tensor.name() = tensorObj["name"].GetString();
                
                if (tensorObj.HasMember("dtype") && tensorObj["dtype"].IsString()) {
                    tensor.dtype() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype"].GetString());
                    tensor.elem_size() = GetDataSize_Datatype(static_cast<DataType>(tensor.dtype()));
                }
                
                if (tensorObj.HasMember("shape") && tensorObj["shape"].IsArray()) {
                    const rapidjson::Value& shapeArr = tensorObj["shape"];
                    for (rapidjson::SizeType k = 0; k < shapeArr.Size(); k++) {
                        if (shapeArr[k].IsInt64())
                            tensor.shape().push_back(shapeArr[k].GetInt64());
                    }
                }
                
                if (tensorObj.HasMember("name_encoded") && tensorObj["name_encoded"].IsString())
                    tensor.name_encoded() = tensorObj["name_encoded"].GetString();
                
                if (tensorObj.HasMember("dtype_encoded") && tensorObj["dtype_encoded"].IsString())
                    tensor.dtype_encoded() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype_encoded"].GetString());
                
                if (tensorObj.HasMember("shape_encoded") && tensorObj["shape_encoded"].IsArray()) {
                    const rapidjson::Value& shapeArr = tensorObj["shape_encoded"];
                    for (rapidjson::SizeType k = 0; k < shapeArr.Size(); k++) {
                        if (shapeArr[k].IsInt64())
                            tensor.shape_encoded().push_back(shapeArr[k].GetInt64());
                    }
                }
                
                if (tensorObj.HasMember("layout") && tensorObj["layout"].IsString())
                    tensor.layout() = deepx_rmapinfo::GetLayoutNum(tensorObj["layout"].GetString());
                
                if (tensorObj.HasMember("align_unit") && tensorObj["align_unit"].IsInt())
                    tensor.align_unit() = tensorObj["align_unit"].GetInt();
                
                if (tensorObj.HasMember("transpose") && tensorObj["transpose"].IsString())
                    tensor.transpose() = deepx_rmapinfo::GetTransposeNum(tensorObj["transpose"].GetString());
                
                if (tensorObj.HasMember("scale") && tensorObj["scale"].IsFloat()) {
                    tensor.scale() = tensorObj["scale"].GetFloat();
                    if (tensorObj.HasMember("bias") && tensorObj["bias"].IsFloat()) {
                        tensor.bias() = tensorObj["bias"].GetFloat();
                        tensor.use_quantization() = true;
                    } else {
                        tensor.use_quantization() = false;
                    }
                }

                if (tensorObj.HasMember("memory") && tensorObj["memory"].IsObject()) {
                    const rapidjson::Value& memObj = tensorObj["memory"];
                    deepx_rmapinfo::Memory mem;
                    if (memObj.HasMember("name") && memObj["name"].IsString())
                        mem.name() = memObj["name"].GetString();
                    if (memObj.HasMember("offset") && memObj["offset"].IsInt64())
                        mem.offset() = memObj["offset"].GetInt64();
                    if (memObj.HasMember("size") && memObj["size"].IsInt64())
                        mem.size() = memObj["size"].GetInt64();
                    if (memObj.HasMember("type") && memObj["type"].IsString())
                        mem.type() = deepx_rmapinfo::GetMemoryTypeNum(memObj["type"].GetString());
                    tensor.memory() = mem;
                }

                regMap.inputs().push_back(tensor);
            }
        }

        // outputs
        if (document.HasMember("outputs") && document["outputs"].IsArray()) {
            const rapidjson::Value& outputsArray = document["outputs"];
            regMap.outputs().clear();
            for (rapidjson::SizeType j = 0; j < outputsArray.Size(); j++) {
                const rapidjson::Value& tensorObj = outputsArray[j];
                deepx_rmapinfo::TensorInfo tensor;

                if (tensorObj.HasMember("name") && tensorObj["name"].IsString())
                    tensor.name() = tensorObj["name"].GetString();
                
                if (tensorObj.HasMember("dtype") && tensorObj["dtype"].IsString()) {
                    tensor.dtype() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype"].GetString());
                    tensor.elem_size() = GetDataSize_Datatype(static_cast<DataType>(tensor.dtype()));
                }
                
                if (tensorObj.HasMember("shape") && tensorObj["shape"].IsArray()) {
                    const rapidjson::Value& shapeArr = tensorObj["shape"];
                    for (rapidjson::SizeType k = 0; k < shapeArr.Size(); k++) {
                        if (shapeArr[k].IsInt64())
                            tensor.shape().push_back(shapeArr[k].GetInt64());
                    }
                }
                
                if (tensorObj.HasMember("name_encoded") && tensorObj["name_encoded"].IsString())
                    tensor.name_encoded() = tensorObj["name_encoded"].GetString();
                
                if (tensorObj.HasMember("dtype_encoded") && tensorObj["dtype_encoded"].IsString())
                    tensor.dtype_encoded() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype_encoded"].GetString());
                
                if (tensorObj.HasMember("shape_encoded") && tensorObj["shape_encoded"].IsArray()) {
                    const rapidjson::Value& shapeArr = tensorObj["shape_encoded"];
                    for (rapidjson::SizeType k = 0; k < shapeArr.Size(); k++) {
                        if (shapeArr[k].IsInt64())
                            tensor.shape_encoded().push_back(shapeArr[k].GetInt64());
                    }
                }
                
                if (tensorObj.HasMember("layout") && tensorObj["layout"].IsString())
                    tensor.layout() = deepx_rmapinfo::GetLayoutNum(tensorObj["layout"].GetString());
                
                if (tensorObj.HasMember("align_unit") && tensorObj["align_unit"].IsInt())
                    tensor.align_unit() = tensorObj["align_unit"].GetInt();
                
                if (tensorObj.HasMember("transpose") && tensorObj["transpose"].IsString())
                    tensor.transpose() = deepx_rmapinfo::GetTransposeNum(tensorObj["transpose"].GetString());
                
                if (tensorObj.HasMember("scale") && tensorObj["scale"].IsFloat()) {
                    tensor.scale() = tensorObj["scale"].GetFloat();
                    if (tensorObj.HasMember("bias") && tensorObj["bias"].IsFloat()) {
                        tensor.bias() = tensorObj["bias"].GetFloat();
                        tensor.use_quantization() = true;
                    } else {
                        tensor.use_quantization() = false;
                    }
                }

                if (tensorObj.HasMember("memory") && tensorObj["memory"].IsObject()) {
                    const rapidjson::Value& memObj = tensorObj["memory"];
                    deepx_rmapinfo::Memory mem;
                    if (memObj.HasMember("name") && memObj["name"].IsString())
                        mem.name() = memObj["name"].GetString();
                    if (memObj.HasMember("offset") && memObj["offset"].IsInt64())
                        mem.offset() = memObj["offset"].GetInt64();
                    if (memObj.HasMember("size") && memObj["size"].IsInt64())
                        mem.size() = memObj["size"].GetInt64();
                    if (memObj.HasMember("type") && memObj["type"].IsString())
                        mem.type() = deepx_rmapinfo::GetMemoryTypeNum(memObj["type"].GetString());
                    tensor.memory() = mem;
                }

                if (tensor.memory().type() == deepx_rmapinfo::MemoryType::PPU) {
                    if (tensor.layout() == deepx_rmapinfo::Layout::PPU_YOLO)
                        tensor.name() = "BBOX";
                    else if (tensor.layout() == deepx_rmapinfo::Layout::PPU_FD)
                        tensor.name() = "FACE";
                    else if (tensor.layout() == deepx_rmapinfo::Layout::PPU_POSE)
                        tensor.name() = "POSE";
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
                else if (tensor.memory().type() == deepx_rmapinfo::MemoryType::ARGMAX)
                {
                    if (tensorObj.HasMember("shape_encoded") && tensorObj["shape_encoded"].IsArray()) {
                        const rapidjson::Value& shapeArr = tensorObj["shape_encoded"];
                        int64_t product = 1;
                        product *= shapeArr[0].GetInt64();

                        int elementSize = getElementSize(tensor.dtype_encoded());
                        product *= elementSize;

                        if (product != tensor.memory().size()) {
                            throw ModelParsingException(EXCEPTION_MESSAGE("invalid output shape in rmap_info"));
                        }
                    }
                }
                else
                {
                    if (tensorObj.HasMember("shape_encoded") && tensorObj["shape_encoded"].IsArray()) {
                        const rapidjson::Value& shapeArr = tensorObj["shape_encoded"];
                        int64_t product = 1;
                        for (rapidjson::SizeType k = 0; k < shapeArr.Size(); k++) {
                            int64_t value = shapeArr[k].GetInt64();
                            if (k == shapeArr.Size() - 1)
                                value = GetAlign(value, tensor.align_unit());
                            product *= value;
                        }
                        int elementSize = getElementSize(tensor.dtype_encoded());
                        product *= elementSize;

                        if (product != tensor.memory().size()) {
                            throw ModelParsingException(EXCEPTION_MESSAGE("invalid output shape in rmap_info"));
                        }
                    }
                }

                regMap.outputs().push_back(tensor);
            }
        }

        // v8: Copy PPU type from binary info to rmap info
        regMap.ppu_type() = data.deepx_binary._ppuType;

        param.rmap_info().push_back(regMap);
    }

    for (size_t i = 0; i < modelCompileType.length(); i++) {
        modelCompileType[i] = tolower(modelCompileType[i]);
    }
    return modelCompileType;
}

}  // namespace dxrt
