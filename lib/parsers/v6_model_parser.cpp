/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/parsers/v6_model_parser.h"
#include <fstream>
#include <cstring>
#include <memory>
#include "dxrt/filesys_support.h"
#include "dxrt/exception/exception.h"
#include "dxrt/util.h"
#include "../resource/log_messages.h"
#include "dxrt/extern/rapidjson/writer.h"
#include "dxrt/extern/rapidjson/stringbuffer.h"
#include "dxrt/common.h"

using std::vector;
using std::string;
using rapidjson::Document;
using rapidjson::Value;
using rapidjson::StringBuffer;
using rapidjson::SizeType;
using rapidjson::Writer;
using rapidjson::kObjectType;
using rapidjson::kArrayType;


// Add missing constants
#ifndef MAX_CHECKPOINT_COUNT
#define MAX_CHECKPOINT_COUNT 3
#endif

#ifndef DXRT_TASK_MAX_LOAD
#define DXRT_TASK_MAX_LOAD 6
#endif

#undef GetObject

namespace dxrt {

std::string V6ModelParser::ParseModel(const std::string& filePath, ModelDataBase& modelData) {
    if (!fileExists(filePath) || getExtension(filePath) != "dxnn") {
        throw FileNotFoundException(EXCEPTION_MESSAGE("Invalid model path : " + filePath));
    }

    int fileSize = getFileSize(filePath);
    vector<char> vbuf(fileSize, 'a');
    char *buf = vbuf.data();

    FILE *fp = fopen(filePath.c_str(), "rb");
    if (!fp) {
        throw FileNotFoundException(EXCEPTION_MESSAGE("Failed to open file: " + filePath));
    }

    std::ignore = fread(static_cast<void*>(buf), fileSize, 1, fp);
    fclose(fp);

    LoadBinaryInfo(modelData.deepx_binary, buf, fileSize);

    // Store original v6 graph_info and rmap_info
    string v6GraphInfo = "";
    for (const auto& str : modelData.deepx_binary.graph_info().str())
        v6GraphInfo += str;

    vector<string> v6RmapInfos;
    for (size_t i = 0; i < modelData.deepx_binary.rmap_info().size(); i++) {
        string v6RmapInfo = "";
        for (const auto& str : modelData.deepx_binary.rmap_info(i).str())
            v6RmapInfo += str;
        v6RmapInfos.push_back(v6RmapInfo);
    }

    // v6_converter.py logic: Keep binary data as is, convert rmap_info and graph_info

    // Convert graph_info to v7 format
    string v7GraphInfo = ConvertGraphInfoV6ToV7(v6GraphInfo);
    modelData.deepx_binary.graph_info().str() = v7GraphInfo;

    // Convert rmap_info to v7 format
    for (size_t i = 0; i < modelData.deepx_binary.rmap_info().size(); i++) {
        string v7RmapInfo = ConvertRmapInfoV6ToV7(v6RmapInfos[i], v6GraphInfo);
        modelData.deepx_binary.rmap_info(i).str() = v7RmapInfo;
    }

    LoadGraphInfo(modelData.deepx_graph, modelData);
    string modelCompileType = LoadRmapInfo(modelData.deepx_rmap, modelData);

    return modelCompileType;
}

int V6ModelParser::LoadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& param, char *buffer, int fileSize) {
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

    if (dxnnFileFormatVersion != 6) {
        throw ModelParsingException(EXCEPTION_MESSAGE("V6ModelParser can only parse version 6 files"));
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
                    if (value.HasMember("offset")) {
                        if (value["offset"].IsInt64())
                            model.offset() = value["offset"].GetInt64();
                        else if (value["offset"].IsInt())
                            model.offset() = value["offset"].GetInt();
                        else if (value["offset"].IsString())
                            model.offset() = std::stoll(value["offset"].GetString());
                    }
                    if (value.HasMember("size")) {
                        if (value["size"].IsInt64())
                            model.size() = value["size"].GetInt64();
                        else if (value["size"].IsInt())
                            model.size() = value["size"].GetInt();
                        else if (value["size"].IsString())
                            model.size() = std::stoll(value["size"].GetString());
                    }
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

        // [Field] - compiled data
        if (dataObj.HasMember("compiled_data") && dataObj["compiled_data"].IsObject()) {
            const Value &compiledData = dataObj["compiled_data"];
            for (Value::ConstMemberIterator iter = compiledData.MemberBegin(); iter != compiledData.MemberEnd(); ++iter) {
                if (iter->name.IsString()) {
                    deepx_binaryinfo::Models rmap;
                    deepx_binaryinfo::Models weight;
                    deepx_binaryinfo::Models rmap_info;
                    deepx_binaryinfo::Models bitmatch_mask;
                    rmap.npu() = weight.npu() = rmap_info.npu() = bitmatch_mask.npu() = iter->name.GetString();
                    const Value& value = iter->value;

                    for (Value::ConstMemberIterator iter2 = value.MemberBegin(); iter2 != value.MemberEnd(); ++iter2) {
                        if (iter2->name.IsString()) {
                            rmap.name() = weight.name() = rmap_info.name() = bitmatch_mask.name() = iter2->name.GetString();
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

    return dxnnFileFormatVersion;
}

std::string V6ModelParser::ConvertGraphInfoV6ToV7(const std::string& v6GraphInfo) {

    Document v6Doc;
    v6Doc.Parse(v6GraphInfo.c_str());

    if (v6Doc.HasParseError()) {
        throw ModelParsingException(EXCEPTION_MESSAGE("Failed to parse V6 graph info"));
    }

    Document v7Doc;
    v7Doc.SetObject();
    Document::AllocatorType& allocator = v7Doc.GetAllocator();

    if (v6Doc.HasMember("offloading")) {
        v7Doc.AddMember("offloading", v6Doc["offloading"], allocator);
    }

    if (v6Doc.HasMember("origin_input")) {
        v7Doc.AddMember("inputs", v6Doc["origin_input"], allocator);
    }

    if (v6Doc.HasMember("origin_output")) {
        v7Doc.AddMember("outputs", v6Doc["origin_output"], allocator);
    }

    if (v6Doc.HasMember("toposort_order")) {
        v7Doc.AddMember("toposort_order", v6Doc["toposort_order"], allocator);
    }

    if (v6Doc.HasMember("graphs") && v6Doc["graphs"].IsArray()) {
        Value v7Graphs(kArrayType);

        for (auto& v6Graph : v6Doc["graphs"].GetArray()) {
            Value v7Graph(kObjectType);

            // name, device copy
            if (v6Graph.HasMember("name")) {
                v7Graph.AddMember("name", v6Graph["name"], allocator);
            }
            if (v6Graph.HasMember("type")) {
                v7Graph.AddMember("device", v6Graph["type"], allocator);
            }

            if (v6Graph.HasMember("inputs") && v6Graph["inputs"].IsObject()) {
                Value v7Inputs(kArrayType);

                for (auto& input : v6Graph["inputs"].GetObject()) {
                    Value inputObj(kObjectType);
                    Value nameVal;
                    nameVal.SetString(input.name.GetString(), allocator);
                    inputObj.AddMember("name", nameVal, allocator);

                    if (input.value.IsObject() && input.value.HasMember("source")) {
                        inputObj.AddMember("owner", input.value["source"], allocator);
                    } else {
                        Value emptyOwner("");
                        inputObj.AddMember("owner", emptyOwner, allocator);
                    }

                    Value users(kArrayType);
                    if (v6Graph.HasMember("name")) {
                        users.PushBack(v6Graph["name"], allocator);
                    }
                    inputObj.AddMember("users", users, allocator);

                    v7Inputs.PushBack(inputObj, allocator);
                }

                v7Graph.AddMember("inputs", v7Inputs, allocator);
            }

            if (v6Graph.HasMember("outputs") && v6Graph["outputs"].IsObject()) {
                Value v7Outputs(kArrayType);

                for (auto& output : v6Graph["outputs"].GetObject()) {
                    Value outputObj(kObjectType);
                    Value nameVal;
                    nameVal.SetString(output.name.GetString(), allocator);
                    outputObj.AddMember("name", nameVal, allocator);

                    if (v6Graph.HasMember("name")) {
                        outputObj.AddMember("owner", v6Graph["name"], allocator);
                    }

                    Value users(kArrayType);
                    if (output.value.IsObject() && output.value.HasMember("next_layers") && output.value["next_layers"].IsArray()) {
                        for (auto& nextLayer : output.value["next_layers"].GetArray()) {
                            users.PushBack(nextLayer, allocator);
                        }
                    }
                    outputObj.AddMember("users", users, allocator);

                    v7Outputs.PushBack(outputObj, allocator);
                }

                v7Graph.AddMember("outputs", v7Outputs, allocator);
            }

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

std::string V6ModelParser::ConvertRmapInfoV6ToV7(const std::string& v6RmapInfo, const std::string& v6GraphInfo) {
    // Convert V6 rmap info to V7 format.
    // This implements the logic of the `convert_rmap_info` function from v6_converter.py in C++.

    Document v6RmapDoc, v6GraphDoc;
    v6RmapDoc.Parse(v6RmapInfo.c_str());
    v6GraphDoc.Parse(v6GraphInfo.c_str());

    if (v6RmapDoc.HasParseError() || v6GraphDoc.HasParseError()) {
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
    if (v6RmapDoc.HasMember("version") && v6RmapDoc["version"].IsObject()) {
        const Value& v6Version = v6RmapDoc["version"];
        Value versionObj(kObjectType);

        if (v6Version.HasMember("npu") && v6Version["npu"].IsString()) {
            Value npuVal;
            npuVal.SetString(v6Version["npu"].GetString(), allocator);
            versionObj.AddMember("npu", npuVal, allocator);
        }
        if (v6Version.HasMember("rmap") && v6Version["rmap"].IsString()) {
            Value rmapVal;
            rmapVal.SetString(v6Version["rmap"].GetString(), allocator);
            versionObj.AddMember("rmap", rmapVal, allocator);
        }

        // Separate version and optimization level from rmapInfo
        if (v6Version.HasMember("rmapInfo") && v6Version["rmapInfo"].IsString()) {
            auto versionPair = ParseV6Version(v6Version["rmapInfo"].GetString());
            std::string cg_version = versionPair.first;
            std::string opt_level = versionPair.second;
            Value rmapInfoVal, optLevelVal;
            rmapInfoVal.SetString(cg_version.c_str(), allocator);
            optLevelVal.SetString(opt_level.c_str(), allocator);
            versionObj.AddMember("rmapInfo", rmapInfoVal, allocator);
            versionObj.AddMember("opt_level", optLevelVal, allocator);
        }

        v7Doc.AddMember("version", versionObj, allocator);
    }

    // 3. Basic information
    if (v6RmapDoc.HasMember("model") && v6RmapDoc["model"].IsString()) {
        Value nameVal;
        nameVal.SetString(v6RmapDoc["model"].GetString(), allocator);
        v7Doc.AddMember("name", nameVal, allocator);
    }
    if (v6RmapDoc.HasMember("mode") && v6RmapDoc["mode"].IsString()) {
        Value modeVal;
        modeVal.SetString(v6RmapDoc["mode"].GetString(), allocator);
        v7Doc.AddMember("mode", modeVal, allocator);
    }
    if (v6RmapDoc.HasMember("npu") && v6RmapDoc["npu"].IsObject()) {
        Value npuObj(kObjectType);
        const Value& v6Npu = v6RmapDoc["npu"];
        if (v6Npu.HasMember("mac") && v6Npu["mac"].IsInt64()) {
            rapidjson::Value macVal;
            macVal.SetInt64(v6Npu["mac"].GetInt64());
            npuObj.AddMember(rapidjson::StringRef("mac"), macVal, allocator);
        }
        v7Doc.AddMember("npu", npuObj, allocator);
    }

    if (v6RmapDoc.HasMember("size")) {
        if (v6RmapDoc["size"].IsString()) {
            int64_t sizeVal = std::stoll(v6RmapDoc["size"].GetString());
            v7Doc.AddMember("size", sizeVal, allocator);
        } else if (v6RmapDoc["size"].IsInt()) {
            v7Doc.AddMember("size", v6RmapDoc["size"], allocator);
        } else if (v6RmapDoc["size"].IsInt64()) {
            v7Doc.AddMember("size", v6RmapDoc["size"], allocator);
        }
    }

    if (v6RmapDoc.HasMember("counts") && v6RmapDoc["counts"].IsObject()) {
        Value countsVal;
        countsVal.CopyFrom(v6RmapDoc["counts"], allocator);
        v7Doc.AddMember("counts", countsVal, allocator);
    }

    // 4. Create Memory information (referencing v6_converter.py)
    Value memoryArray(kArrayType);

    // INPUT memory (from v6 input.memory)
    if (v6RmapDoc.HasMember("input") && v6RmapDoc["input"].HasMember("memory")) {
        Value inputMem(kObjectType);
        const Value& v6InputMem = v6RmapDoc["input"]["memory"];
        Value nameVal;
        nameVal.SetString("INPUT", allocator);
        inputMem.AddMember(rapidjson::StringRef("name"), nameVal, allocator);

        if (v6InputMem.HasMember("offset")) {
            rapidjson::Value offsetVal;
            if (v6InputMem["offset"].IsInt64())
                offsetVal.SetInt64(v6InputMem["offset"].GetInt64());
            else if (v6InputMem["offset"].IsInt())
                offsetVal.SetInt(v6InputMem["offset"].GetInt());
            else if (v6InputMem["offset"].IsString())
                offsetVal.SetInt64(std::stoll(v6InputMem["offset"].GetString()));
            inputMem.AddMember(rapidjson::StringRef("offset"), offsetVal, allocator);
        }
        if (v6InputMem.HasMember("size")) {
            rapidjson::Value sizeVal;
            if (v6InputMem["size"].IsInt64())
                sizeVal.SetInt64(v6InputMem["size"].GetInt64());
            else if (v6InputMem["size"].IsInt())
                sizeVal.SetInt(v6InputMem["size"].GetInt());
            else if (v6InputMem["size"].IsString())
                sizeVal.SetInt64(std::stoll(v6InputMem["size"].GetString()));
            inputMem.AddMember(rapidjson::StringRef("size"), sizeVal, allocator);
        }
        if (v6InputMem.HasMember("type")) {
            Value typeVal;
            typeVal.SetString(v6InputMem["type"].GetString(), allocator);
            inputMem.AddMember(rapidjson::StringRef("type"), typeVal, allocator);
        }
        memoryArray.PushBack(inputMem, allocator);
    }

    // OUTPUT memory (from v6 outputs.memory)
    if (v6RmapDoc.HasMember("outputs") && v6RmapDoc["outputs"].HasMember("memory")) {
        Value outputMem(kObjectType);
        const Value& v6OutputMem = v6RmapDoc["outputs"]["memory"];
        Value nameVal;
        nameVal.SetString("OUTPUT", allocator);
        outputMem.AddMember(rapidjson::StringRef("name"), nameVal, allocator);

        if (v6OutputMem.HasMember("offset")) {
            rapidjson::Value offsetVal;
            if (v6OutputMem["offset"].IsInt64())
                offsetVal.SetInt64(v6OutputMem["offset"].GetInt64());
            else if (v6OutputMem["offset"].IsInt())
                offsetVal.SetInt(v6OutputMem["offset"].GetInt());
            else if (v6OutputMem["offset"].IsString())
                offsetVal.SetInt64(std::stoll(v6OutputMem["offset"].GetString()));
            outputMem.AddMember(rapidjson::StringRef("offset"), offsetVal, allocator);
        }
        if (v6OutputMem.HasMember("size")) {
            rapidjson::Value sizeVal;
            if (v6OutputMem["size"].IsInt64())
                sizeVal.SetInt64(v6OutputMem["size"].GetInt64());
            else if (v6OutputMem["size"].IsInt())
                sizeVal.SetInt(v6OutputMem["size"].GetInt());
            else if (v6OutputMem["size"].IsString())
                sizeVal.SetInt64(std::stoll(v6OutputMem["size"].GetString()));
            outputMem.AddMember(rapidjson::StringRef("size"), sizeVal, allocator);
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
                rapidjson::Value offsetVal;
                if (v6Mem["offset"].IsInt64())
                    offsetVal.SetInt64(v6Mem["offset"].GetInt64());
                else if (v6Mem["offset"].IsInt())
                    offsetVal.SetInt(v6Mem["offset"].GetInt());
                else if (v6Mem["offset"].IsString())
                    offsetVal.SetInt64(std::stoll(v6Mem["offset"].GetString()));
                mem.AddMember(rapidjson::StringRef("offset"), offsetVal, allocator);
            }
            if (v6Mem.HasMember("size")) {
                rapidjson::Value sizeVal;
                if (v6Mem["size"].IsInt64())
                    sizeVal.SetInt64(v6Mem["size"].GetInt64());
                else if (v6Mem["size"].IsInt())
                    sizeVal.SetInt(v6Mem["size"].GetInt());
                else if (v6Mem["size"].IsString())
                    sizeVal.SetInt64(std::stoll(v6Mem["size"].GetString()));
                mem.AddMember(rapidjson::StringRef("size"), sizeVal, allocator);
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
        
        // Use input_shape from v6 graph_info instead of v6 input shapes, following v6_converter.py
        Value shapeVal, shapeEncodedVal;
        shapeVal.CopyFrom(input_shape, allocator);
        shapeEncodedVal.CopyFrom(input_shape, allocator);
        inputTensor.AddMember("shape", shapeVal, allocator);
        inputTensor.AddMember("shape_encoded", shapeEncodedVal, allocator);
        
        // Set to the same as original name, following v6_converter.py
        Value nameEncodedVal;
        nameEncodedVal.SetString(input_name.c_str(), allocator);
        inputTensor.AddMember("name_encoded", nameEncodedVal, allocator);

        Value layoutVal;
        layoutVal.SetString("NONE", allocator);
        inputTensor.AddMember("layout", layoutVal, allocator);

        inputTensor.AddMember("align_unit", 1, allocator);

        Value transposeVal;
        transposeVal.SetNull();
        inputTensor.AddMember("transpose", transposeVal, allocator);

        Value scaleVal, biasVal;
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
                if (v6InputMem["offset"].IsInt64())
                    inputOffset = v6InputMem["offset"].GetInt64();
                else if (v6InputMem["offset"].IsInt())
                    inputOffset = v6InputMem["offset"].GetInt();
                else if (v6InputMem["offset"].IsString())
                    inputOffset = std::stoll(v6InputMem["offset"].GetString());
            }
            if (v6InputMem.HasMember("size")) {
                if (v6InputMem["size"].IsInt64())
                    inputSize = v6InputMem["size"].GetInt64();
                else if (v6InputMem["size"].IsInt())
                    inputSize = v6InputMem["size"].GetInt();
                else if (v6InputMem["size"].IsString())
                    inputSize = std::stoll(v6InputMem["size"].GetString());
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
            
            // Extract output shape from v6 graph_info for this specific output, following v6_converter.py
            // shape = npu_graph["outputs"][value["name"]]["shape"]
            Value outputShapeVal = ExtractOutputShapeFromV6Graph(v6GraphDoc, outputName, allocator);
            Value outputShapeEncodedVal;
            outputShapeEncodedVal.CopyFrom(outputShapeVal, allocator);
            outputTensor.AddMember("shape", outputShapeVal, allocator);
            outputTensor.AddMember("shape_encoded", outputShapeEncodedVal, allocator);
            
            // Handle layout field - preserve PPU_* values, set others to NONE
            Value layoutVal;
            if (v6Output.HasMember("format") && v6Output["format"].IsString()) {
                std::string layoutStr = v6Output["format"].GetString();
                if (layoutStr.find("PPU_") == 0) {
                    layoutVal.SetString(layoutStr.c_str(), allocator);
                } else {
                    layoutVal.SetString("NONE", allocator);
                }
            } else {
                layoutVal.SetString("NONE", allocator);
            }
            outputTensor.AddMember("layout", layoutVal, allocator);

            outputTensor.AddMember("align_unit", 1, allocator);

            Value transposeVal;
            transposeVal.SetNull();
            outputTensor.AddMember("transpose", transposeVal, allocator);

            Value scaleVal, biasVal;
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
            if (v6Output.HasMember("memory") && v6Output["memory"].IsObject()) {
                const Value& v6OutputMem = v6Output["memory"];
                if (v6OutputMem.HasMember("offset")) {
                    if (v6OutputMem["offset"].IsInt64())
                        outputOffset = v6OutputMem["offset"].GetInt64();
                    else if (v6OutputMem["offset"].IsInt())
                        outputOffset = v6OutputMem["offset"].GetInt();
                    else if (v6OutputMem["offset"].IsString())
                        outputOffset = std::stoll(v6OutputMem["offset"].GetString());
                }
                if (v6OutputMem.HasMember("size")) {
                    if (v6OutputMem["size"].IsInt64())
                        outputSize = v6OutputMem["size"].GetInt64();
                    else if (v6OutputMem["size"].IsInt())
                        outputSize = v6OutputMem["size"].GetInt();
                    else if (v6OutputMem["size"].IsString())
                        outputSize = std::stoll(v6OutputMem["size"].GetString());
                }
                if (v6OutputMem.HasMember("type") && v6OutputMem["type"].IsString()) {
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
std::string V6ModelParser::ParseV6GraphInfo(const rapidjson::Document& v6GraphInfo) {
    // Parse V6 graph info
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    v6GraphInfo.Accept(writer);
    return buffer.GetString();
}

std::string V6ModelParser::ParseV6RmapInfo(const rapidjson::Document& v6RmapInfo, const rapidjson::Document& v6GraphInfo) {
    // Parse V6 rmap info
    string v6RmapStr, v6GraphStr;

    StringBuffer rmapBuffer, graphBuffer;
    Writer<StringBuffer> rmapWriter(rmapBuffer), graphWriter(graphBuffer);
    v6RmapInfo.Accept(rmapWriter);
    v6GraphInfo.Accept(graphWriter);

    v6RmapStr = rmapBuffer.GetString();
    v6GraphStr = graphBuffer.GetString();

    return ConvertRmapInfoV6ToV7(v6RmapStr, v6GraphStr);
}

std::string V6ModelParser::ExtractInputNameFromV6Graph(const rapidjson::Document& v6GraphInfo) {
    // Extract input name from V6 graph.
    // Python equivalent:
    // for graph in graph_info["graphs"]:
    //     if graph["name"] == "npu_0":
    //         input_name = list(graph["inputs"].keys())[0]

    if (v6GraphInfo.HasMember("graphs") && v6GraphInfo["graphs"].IsArray()) {
        const Value& graphs = v6GraphInfo["graphs"];
        for (SizeType i = 0; i < graphs.Size(); i++) {
            const Value& graph = graphs[i];
            if (graph.HasMember("name") && graph["name"].IsString() &&
                string(graph["name"].GetString()) == "npu_0") {
                if (graph.HasMember("inputs") && graph["inputs"].IsObject()) {
                    const Value& inputs = graph["inputs"];
                    for (auto iter = inputs.MemberBegin(); iter != inputs.MemberEnd(); ++iter) {
                        return iter->name.GetString();  // Return the first input name
                    }
                }
            }
        }
    }

    // Return a default value if not found
    return "input";
}

Value V6ModelParser::ExtractInputShapeFromV6Graph(const rapidjson::Document& v6GraphInfo, Document::AllocatorType& allocator) {
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
                string(graph["name"].GetString()) == "npu_0") {
                if (graph.HasMember("inputs") && graph["inputs"].IsObject()) {
                    const Value& inputs = graph["inputs"];
                    for (auto iter = inputs.MemberBegin(); iter != inputs.MemberEnd(); ++iter) {
                        const Value& inputTensor = iter->value;
                        if (inputTensor.HasMember("shape") && inputTensor["shape"].IsArray()) {
                            Value shape(kArrayType);
                            const Value& inputShape = inputTensor["shape"];
                            for (SizeType j = 0; j < inputShape.Size(); j++) {
                                Value val;
                                val.CopyFrom(inputShape[j], allocator);
                                shape.PushBack(val, allocator);
                            }
                            return shape;
                        }
                        break; // Return first input's shape
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

Value V6ModelParser::ExtractOutputShapeFromV6Graph(const rapidjson::Document& v6GraphInfo, const std::string& outputName, Document::AllocatorType& allocator) {
    // Extract output shape from V6 graph for specific output name.
    // Python equivalent:
    // shape = npu_graph["outputs"][value["name"]]["shape"]
    
    if (v6GraphInfo.HasMember("graphs") && v6GraphInfo["graphs"].IsArray()) {
        const Value& graphs = v6GraphInfo["graphs"];
        for (SizeType i = 0; i < graphs.Size(); i++) {
            const Value& graph = graphs[i];
            if (graph.HasMember("name") && graph["name"].IsString() && 
                string(graph["name"].GetString()) == "npu_0") {
                if (graph.HasMember("outputs") && graph["outputs"].IsObject()) {
                    const Value& outputs = graph["outputs"];
                    if (outputs.HasMember(outputName.c_str()) && outputs[outputName.c_str()].IsObject()) {
                        const Value& outputTensor = outputs[outputName.c_str()];
                        if (outputTensor.HasMember("shape") && outputTensor["shape"].IsArray()) {
                            Value shape(kArrayType);
                            const Value& outputShape = outputTensor["shape"];
                            for (SizeType j = 0; j < outputShape.Size(); j++) {
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

std::pair<std::string, std::string> V6ModelParser::ParseV6Version(const std::string& versionStr) {
    // Separate version and optimization level from a V6 version string.
    // e.g., "1.0.0(opt_level)" -> {"1.0.0", "opt_level"}

    auto pos = versionStr.find('(');
    if (pos != std::string::npos) {
        std::string version = versionStr.substr(0, pos);
        std::string optLevel = versionStr.substr(pos + 1);

        // Remove trailing ')'
        if (!optLevel.empty() && optLevel.back() == ')') {
            optLevel.pop_back();
        }

        return {version, optLevel};
    }

    return {versionStr, ""};
}

int V6ModelParser::LoadGraphInfo(deepx_graphinfo::GraphInfoDatabase& param, ModelDataBase& data) {
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

            if (subGraphObj.HasMember("inputs") && subGraphObj["inputs"].IsArray())
            {
                const rapidjson::Value& tensorArray = subGraphObj["inputs"];
                for (rapidjson::SizeType j = 0; j < tensorArray.Size(); j++)
                {
                    const rapidjson::Value& tensorObj = tensorArray[j];
                    deepx_graphinfo::Tensor tensor;

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

            param.subgraphs().push_back(subGraph);
        }
    }

    return 0;
}

std::string V6ModelParser::LoadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& param, ModelDataBase& data) {
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

        // memory: List[MemoryInfo]
        if (document.HasMember("memory") && document["memory"].IsArray()) {
            const rapidjson::Value& memArray = document["memory"];
            for (rapidjson::SizeType mi = 0; mi < memArray.Size(); mi++) {
                const rapidjson::Value& memObj = memArray[mi];
                if (memObj.HasMember("name") && memObj["name"].IsString()) {
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

                    if (memory.name() == "RMAP") {
                        regMap.model_memory().rmap() = memory;
                        regMap.model_memory().model_memory_size() += memory.size();
                    }
                    else if (memory.name() == "WEIGHT")
                    {
                        regMap.model_memory().weight() = memory;
                        regMap.model_memory().model_memory_size() += memory.size();
                    }
                    else if (memory.name() == "INPUT")
                    {
                        regMap.model_memory().input() = memory;
                        regMap.model_memory().model_memory_size() += memory.size() * DXRT_TASK_MAX_LOAD;
                    }
                    else if (memory.name() == "OUTPUT")
                    {
                        regMap.model_memory().output() = memory;
                        regMap.model_memory().model_memory_size() += memory.size() * DXRT_TASK_MAX_LOAD;
                    }
                    else if (memory.name() == "TEMP")
                    {
                        regMap.model_memory().temp() = memory;
                        regMap.model_memory().model_memory_size() += memory.size();
                    }
                }
            }
        }

        // inputs: List[TensorInfo]
        if (document.HasMember("inputs") && document["inputs"].IsArray()) {
            const rapidjson::Value& tensorArray = document["inputs"];
            regMap.inputs().clear();
            for (rapidjson::SizeType ti = 0; ti < tensorArray.Size(); ti++) {
                const rapidjson::Value& tensorObj = tensorArray[ti];
                deepx_rmapinfo::TensorInfo tensor;

                if (tensorObj.HasMember("name") && tensorObj["name"].IsString())\
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
                    const rapidjson::Value& shapeArr = tensorObj["shape"];
                    for (rapidjson::SizeType j = 0; j < shapeArr.Size(); j++) {
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
                    const rapidjson::Value& shapeArr = tensorObj["shape_encoded"];
                    for (rapidjson::SizeType j = 0; j < shapeArr.Size(); j++) {
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

        // outputs: List[TensorInfo]
        if (document.HasMember("outputs") && document["outputs"].IsArray())
        {
            const rapidjson::Value& tensorArray = document["outputs"];
            regMap.outputs().clear();
            for (rapidjson::SizeType ti = 0; ti < tensorArray.Size(); ti++)
            {
                const rapidjson::Value& tensorObj = tensorArray[ti];
                deepx_rmapinfo::TensorInfo tensor;

                if (tensorObj.HasMember("name") && tensorObj["name"].IsString())
                    tensor.name() = tensorObj["name"].GetString();

                if (tensorObj.HasMember("dtype") && tensorObj["dtype"].IsString()) {
                    tensor.dtype() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype"].GetString());
                    tensor.elem_size() = GetDataSize_Datatype(static_cast<DataType>(tensor.dtype()));
                }

                if (tensorObj.HasMember("shape") && tensorObj["shape"].IsArray()) {
                    const rapidjson::Value& shapeArr = tensorObj["shape"];
                    for (rapidjson::SizeType j = 0; j < shapeArr.Size(); j++) {
                        tensor.shape().push_back(shapeArr[j].GetInt64());
                    }
                }

                if (tensorObj.HasMember("name_encoded") && tensorObj["name_encoded"].IsString())
                    tensor.name_encoded() = tensorObj["name_encoded"].GetString();

                if (tensorObj.HasMember("dtype_encoded") && tensorObj["dtype_encoded"].IsString())
                    tensor.dtype_encoded() = deepx_rmapinfo::GetDataTypeNum(tensorObj["dtype_encoded"].GetString());

                if (tensorObj.HasMember("shape_encoded") && tensorObj["shape_encoded"].IsArray()) {
                    const rapidjson::Value& shapeArr = tensorObj["shape_encoded"];
                    for (rapidjson::SizeType j = 0; j < shapeArr.Size(); j++) {
                        tensor.shape_encoded().push_back(shapeArr[j].GetInt64());
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
                regMap.outputs().push_back(tensor);
            }
        }

        param.rmap_info().push_back(regMap);
    }

    for (size_t i = 0; i < modelCompileType.length(); i++) {
        modelCompileType[i] = tolower(modelCompileType[i]);
    }
    return modelCompileType;
}

}  // namespace dxrt
