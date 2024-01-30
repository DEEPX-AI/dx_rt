#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include "dxrt/model.h"
#include "dxrt/inference_engine.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <filesystem>
#include <cxxabi.h>

using namespace std;
using namespace rapidjson;
using namespace deepx_rmapinfo;

namespace dxrt
{

void PrintJson(const rapidjson::Value& value, const std::string& indent = "") {
    if (value.IsObject()) {
        std::cout << "{\n";
        for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it) {
            std::cout << indent << "  \"" << it->name.GetString() << "\": ";
            PrintJson(it->value, indent + "  ");
        }
        std::cout << indent << "}";
    } else if (value.IsArray()) {
        std::cout << "[\n";
        for (rapidjson::SizeType i = 0; i < value.Size(); ++i) {
            std::cout << indent << "  ";
            PrintJson(value[i], indent + "  ");
            if (i < value.Size() - 1) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << indent << "]";
    } else if (value.IsString()) {
        std::cout << "\"" << value.GetString() << "\"";
    } else if (value.IsNumber()) {
        if (value.IsInt()) {
            std::cout << value.GetInt();
        } else if (value.IsDouble()) {
            std::cout << value.GetDouble();
        }
    } else if (value.IsBool()) {
        std::cout << (value.GetBool() ? "true" : "false");
    } else if (value.IsNull()) {
        std::cout << "null";
    }
}

static string dataFormatTable[] =
{
	"NONE",
	"NCHW",
	"NHWC",
	"NHW"
};
int ParseModel(string file)
{
    auto ie = InferenceEngine(file);
    // cout << ie << endl;
    return 0;
}
deepx_modelinfo::ModelInfoDatabase LoadModelParam(string file)
{
    deepx_modelinfo::ModelInfoDatabase param;
    string rmapStr;

    filesystem::path filePath = filesystem::path(file);
    DXRT_ASSERT(
        (filesystem::exists(filePath) && filePath.extension()==".dxnn"), 
        "Invalid model path : " + filePath.string()
    );
    
    int fileSize = filesystem::file_size(file);
    vector<char> vbuf(fileSize, 'a');
    char *buf = vbuf.data();
    FILE *fp = fopen(file.c_str(), "rb");
	if (!fp) {
		cout << " file " << file.c_str() << " open failed(" << errno << ")" << endl;
		exit(-1);
	}
    fread((void*)buf, fileSize, 1, fp);
    fclose(fp);

    // Parse Binary Data Info. from Model File
    param.deepx_binary = LoadBinaryInfo(buf, fileSize);

    // Parse Graph Info.
    param.deepx_graph = LoadGraphInfo(param);

    // Parse RMAP Info.
    param.deepx_rmap = LoadRmapInfo(param);

    return param;
}

deepx_binaryinfo::BinaryInfoDatabase LoadBinaryInfo(char *buffer, int fileSize)
{
    deepx_binaryinfo::BinaryInfoDatabase param;
    Document document;

    int offset = 0, verInfo = 0, sizeInfo = 0;
    string signInfo, headerInfo;    

    signInfo = string(buffer, 4);
    offset += 8;
    if (signInfo != "DXNN")
    {
        cout << " The model is invalid. " << endl;
        exit (1);
    }

    verInfo = static_cast<int>(buffer[4]);
    cout << "DXNN Model Ver. : " << verInfo << endl;

    if (verInfo >= 2)
        sizeInfo = 8192;
    else
        sizeInfo = 1024;

    for(int i=0;i<fileSize;i++)
    {
        // LOG_VALUE((256+(int)buf[i]-22)%256);
        buffer[i] = (char)((256+(int)buffer[i]-22)%256);
    }

    headerInfo = string(buffer+offset,sizeInfo-offset);
    offset = sizeInfo;

    // cout << headerInfo << endl;
    document.Parse(headerInfo.c_str());    
    if (document.HasParseError()) {
        std::cerr << "headerinfo parsing failed. (" << document.GetParseError() << ")" << std::endl;
        exit(-1);
    }
    // PrintJson(document);

    if (document.HasMember("data") && document["data"].IsObject()) {
        const Value &dataObj = document["data"];

        // [Field] - merged model
        if (dataObj.HasMember("merged_model") && dataObj["merged_model"].IsObject()) {
            const Value &mergedModelObj = dataObj["merged_model"];
            if (mergedModelObj.HasMember("offset") && mergedModelObj["offset"].IsInt64())
                param.merged_model().offset() = mergedModelObj["offset"].GetInt64();
            if (mergedModelObj.HasMember("size") && mergedModelObj["size"].IsInt64())
                param.merged_model().size() = mergedModelObj["size"].GetInt64();
        }

        // [Field] - npu models
        if (dataObj.HasMember("npu_models") && dataObj["npu_models"].IsObject()) {
            const Value &npuModelsObj = dataObj["npu_models"];
            for (Value::ConstMemberIterator iter = npuModelsObj.MemberBegin(); iter != npuModelsObj.MemberEnd(); ++iter) {
                if (iter->name.IsString()) {
                    deepx_binaryinfo::Models model;
                    model.name() = iter->name.GetString();
                    const Value& value = iter->value;
                    if (value.HasMember("offset") && !value["offset"].IsInt64())
                        model.offset() = value["offset"].GetInt64();
                    if (value.HasMember("size") && value["size"].IsInt64())
                        model.size() = value["size"].GetInt64();    
                    param.npu_models().push_back(model);                                    
                }
            }
        }

        // [Field] - cpu models
        if (dataObj.HasMember("cpu_models") && dataObj["cpu_models"].IsObject()) {
            const Value &cpuModelsObj = dataObj["cpu_models"];
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
            }
        }

        // [Field] - graph info
        if (dataObj.HasMember("graph_info") && dataObj["graph_info"].IsObject()) {
            const Value &graphInfolObj = dataObj["graph_info"];
            if (graphInfolObj.HasMember("offset") && graphInfolObj["offset"].IsInt64())
                param.graph_info().offset() = graphInfolObj["offset"].GetInt64();
            if (graphInfolObj.HasMember("size") && graphInfolObj["size"].IsInt64())
                param.graph_info().size() = graphInfolObj["size"].GetInt64();
        }

        // [Field] - compiled data
        if (dataObj.HasMember("compiled_data") && dataObj["compiled_data"].IsObject()) {
            const Value &compiledData = dataObj["compiled_data"];
            for (Value::ConstMemberIterator iter = compiledData.MemberBegin(); iter != compiledData.MemberEnd(); ++iter) {
                if (iter->name.IsString()) {
                    deepx_binaryinfo::Models rmap;
                    deepx_binaryinfo::Models weight;                    
                    deepx_binaryinfo::Models rmap_info;
                    rmap.npu() = weight.npu() = rmap_info.npu() = iter->name.GetString();
                    const Value& value = iter->value;
                    for (Value::ConstMemberIterator iter2 = value.MemberBegin(); iter2 != value.MemberEnd(); ++iter2) {
                        if (iter2->name.IsString()) {
                            rmap.name() = weight.name() = rmap_info.name() = iter2->name.GetString();
                            const Value& value2 = iter2->value;
                            // [Sub-Field] - rmap
                            if (value2.HasMember("rmap") && value2["rmap"].IsObject()) {
                                const Value &rmapObj = value2["rmap"];
                                if (rmapObj.HasMember("offset") && rmapObj["offset"].IsInt64())
                                    rmap.offset() = rmapObj["offset"].GetInt64();
                                if (rmapObj.HasMember("size") && rmapObj["size"].IsInt64())
                                    rmap.size() = rmapObj["size"].GetInt64();
                                param.rmap().push_back(rmap);
                            }
                            // [Sub-Field] - weight
                            if (value2.HasMember("weight") && value2["weight"].IsObject()) {
                                const Value &weightObj = value2["weight"];
                                if (weightObj.HasMember("offset") && weightObj["offset"].IsInt64())
                                    weight.offset() = weightObj["offset"].GetInt64();
                                if (weightObj.HasMember("size") && weightObj["size"].IsInt64())
                                    weight.size() = weightObj["size"].GetInt64();
                                param.weight().push_back(weight);
                            }
                            // [Sub-Field] - rmap info
                            if (value2.HasMember("rmap_info") && value2["rmap_info"].IsObject()) {
                                const Value &rmapInfoObj = value2["rmap_info"];
                                if (rmapInfoObj.HasMember("offset") && rmapInfoObj["offset"].IsInt64())
                                    rmap_info.offset() = rmapInfoObj["offset"].GetInt64();
                                if (rmapInfoObj.HasMember("size") && rmapInfoObj["size"].IsInt64())
                                    rmap_info.size() = rmapInfoObj["size"].GetInt64();
                                param.rmap_info().push_back(rmap_info);
                            }
                        }
                    }
                }
            }
        }
    }

    // [Buffer] - CPU Binary Data
    for (int i = 0; i < param.cpu_models().size(); i++) {
        unique_ptr<char[]> cpuModelsBuf(new char[param.cpu_models(i).size()]);
        memcpy(cpuModelsBuf.get(), buffer + (offset + param.cpu_models(i).offset()), param.cpu_models(i).size());
        param.cpu_models(i).buffer() = cpuModelsBuf.get();
        cpuModelsBuf.release();
    }

    // [Buffer] - Graph Info.
    unique_ptr<char[]> graphInfoBuf(new char[param.graph_info().size()]);
    memcpy(graphInfoBuf.get(), buffer + (offset + param.graph_info().offset()), param.graph_info().size());
    string graphInfoStr(&graphInfoBuf[0], param.graph_info().size());
    param.graph_info().str() = graphInfoStr;
    graphInfoBuf.release();

    // [Buffer] - RMAP Binary Data
    for (int i = 0; i < param.rmap().size(); i++) {
        unique_ptr<char[]> rmapBuf(new char[param.rmap(i).size()]);
        memcpy(rmapBuf.get(), buffer + (offset + param.rmap(i).offset()), param.rmap(i).size());
        param.rmap(i).buffer() = rmapBuf.get();
        rmapBuf.release();
    }

    // [Buffer] - Weight Binary Data
    for (int i = 0; i < param.weight().size(); i++) {
        unique_ptr<char[]> weightBuf(new char[param.weight(i).size()]);
        memcpy(weightBuf.get(), buffer + (offset + param.weight(i).offset()), param.weight(i).size());
        param.weight(i).buffer() = weightBuf.get();
        weightBuf.release();
    }
    
    // [Buffer] - RMAP Info.
    for (int i = 0; i < param.rmap_info().size(); i++) {
        unique_ptr<char[]> rmapInfoBuf(new char[param.rmap_info(i).size()]);
        memcpy(rmapInfoBuf.get(), buffer + (offset + param.rmap_info(i).offset()), param.rmap_info(i).size());
        string rmapInfoStr(&rmapInfoBuf[0], param.rmap_info(i).size());
        param.rmap_info(i).str() = rmapInfoStr;
        rmapInfoBuf.release();
    }
    return param;
}

deepx_graphinfo::GraphInfoDatabase LoadGraphInfo(deepx_modelinfo::ModelInfoDatabase data)
{
    deepx_graphinfo::GraphInfoDatabase param;
    Document document;
    string graphInfoBuffer;

    for (const auto& str : data.deepx_binary.graph_info().str())
        graphInfoBuffer += str;
    document.Parse(graphInfoBuffer.c_str());

    // cout << graphInfoBuffer << endl;

    if (document.HasParseError()) {
        std::cerr << "graphinfo parsing failed. (" << document.GetParseError() << ")" << std::endl;
        // exit(-1);
        return param;
    }
    // PrintJson(document);
    if (document.HasMember("graphs") && document["graphs"].IsArray()) {
        const Value& graphsArray = document["graphs"];
        for (SizeType i = 0; i < graphsArray.Size(); i++) {
            deepx_graphinfo::GraphBindingDatabase graph;
            const Value& graphObj = graphsArray[i];
            if (graphObj.HasMember("name") && graphObj["name"].IsString())
                graph.name() = graphObj["name"].GetString();
            if (graphObj.HasMember("type") && graphObj["type"].IsString())
                graph.type() = graphObj["type"].GetString();

            // [field]-"inputs"
            if (graphObj.HasMember("inputs") && graphObj["inputs"].IsObject()) {
                const Value& inputsObj = graphObj["inputs"];
                for (Value::ConstMemberIterator iter = inputsObj.MemberBegin(); iter != inputsObj.MemberEnd(); ++iter) {
                    if (iter->name.IsString()) {
                        deepx_graphinfo::KeyValueInfo keyVal;
                        keyVal.key() = iter->name.GetString();       
                        const Value& value = iter->value;
                        if (value.HasMember("source") && !value["source"].IsNull())
                            keyVal.val() = value["source"].GetString();
                        graph.inputs().push_back(keyVal);
                    }
                }
            }

            // [field]-"outputs"
            if (graphObj.HasMember("outputs") && graphObj["outputs"].IsObject()) {
                const Value& outputsObj = graphObj["outputs"];
                for (Value::ConstMemberIterator iter = outputsObj.MemberBegin(); iter != outputsObj.MemberEnd(); ++iter) {
                    deepx_graphinfo::KeyValueInfo keyVal;
                    keyVal.key() = iter->name.GetString();
                    const Value& value = iter->value;
                    for (SizeType i = 0; i < value.Size(); i++) {
                        if (value[i].IsString())
                            keyVal.val() = value[i].GetString();
                    }
                    graph.outputs().push_back(keyVal);
                }
            }
            param.m_graph().push_back(graph);
        }
    }
    
    // [field]-"toposort order"
    if (document.HasMember("toposort_order") && document["toposort_order"].IsArray()) {
        const Value& toposortOrderArray = document["toposort_order"];
        for (SizeType i = 0; i < toposortOrderArray.Size(); i++)
            param.topoSort_order().push_back(toposortOrderArray[i].GetString());
    }

    return param;
}

deepx_rmapinfo::rmapInfoDatabase LoadRmapInfo(deepx_modelinfo::ModelInfoDatabase data)
{
    deepx_rmapinfo::rmapInfoDatabase param;
    Document document;
    string rmapBuffer;

    for (int i = 0; i < data.deepx_binary.rmap_info().size(); i++)
    {
        rmapBuffer = "";
        for (const auto& str : data.deepx_binary.rmap_info(i).str())
            rmapBuffer += str;

        // cout << rmapBuffer << endl;

        document.Parse(rmapBuffer.c_str());
        if (document.HasParseError()) {
            std::cerr << "rmapinfo parsing failed. (" << document.GetParseError() << ")" << std::endl;
            exit(-1);
        }

        deepx_rmapinfo::RegisterInfoDatabase regMap;

        // [field]-"verion"
        if (document.HasMember("version") && document["version"].IsObject()) {
            const Value& versionObj = document["version"];
            if (versionObj.HasMember("npu") && versionObj["npu"].IsString())
                regMap.version().npu() = versionObj["npu"].GetString();
            if (versionObj.HasMember("rmap") && versionObj["rmap"].IsString())
                regMap.version().rmap() = versionObj["rmap"].GetString();
            if (versionObj.HasMember("rmapInfo") && versionObj["rmapInfo"].IsString())
                regMap.version().rmapinfo() = versionObj["rmapInfo"].GetString();
        }

        // [field]-"model"
        if (document.HasMember("model") && document["model"].IsString())
            regMap.model() = document["model"].GetString();

        // [field]-"npu"
        if (document.HasMember("npu") && document["npu"].IsObject()) {
            const Value& npuObj = document["npu"];
            if (npuObj.HasMember("mac") && npuObj["mac"].IsInt64())
                regMap.npu().mac() = npuObj["mac"].GetInt64();
        }

        // [field]-"size"
        if (document.HasMember("size") && document["size"].IsString())
            regMap.size() = stoi(document["size"].GetString());

        // [field]-"traffic"
        if (document.HasMember("traffic") && document["traffic"].IsString())
            regMap.traffic() = stoi(document["traffic"].GetString());

        // [field]-"counts"
        if (document.HasMember("counts") && document["counts"].IsObject()) {
            const Value& countsObj = document["counts"];
            if (countsObj.HasMember("layer") && countsObj["layer"].IsInt64())
                regMap.counts().layer() = countsObj["layer"].GetInt64();
            if (countsObj.HasMember("cmd") && countsObj["cmd"].IsInt64())
                regMap.counts().cmd() = countsObj["cmd"].GetInt64();
        }

        // [field]-"input"
        if (document.HasMember("input") && document["input"].IsObject()) {
            const Value& inputObj = document["input"];
            if (inputObj.HasMember("shapes") && inputObj["shapes"].IsObject()) {
                const Value& shapesObj = inputObj["shapes"];
                if (shapesObj.HasMember("shape") && shapesObj["shape"].IsArray()) {
                    const Value& shapeArr = shapesObj["shape"];
                    regMap.input().shapes().shape_size() = shapeArr.Size();
                    for (SizeType i = 0; i < regMap.input().shapes().shape_size(); i++)
                        regMap.input().shapes().shape().push_back(shapeArr[i].GetInt64());
                }
            }
            if (inputObj.HasMember("type") && inputObj["type"].IsString())
                regMap.input().type() = GetDataTypeNum(inputObj["type"].GetString());
            if (inputObj.HasMember("mode") && inputObj["mode"].IsString())
                regMap.input().mode() = GetInputModeNum(inputObj["mode"].GetString());
            if (inputObj.HasMember("memory") && inputObj["memory"].IsObject()) {
                const Value& memoryObj = inputObj["memory"];
                if (memoryObj.HasMember("name") && memoryObj["name"].IsString())
                    regMap.input().memory().name() = memoryObj["name"].GetString();
                if (memoryObj.HasMember("offset") && memoryObj["offset"].IsString())
                    regMap.input().memory().offset() = stoi(memoryObj["offset"].GetString());
                if (memoryObj.HasMember("size") && memoryObj["size"].IsString())
                    regMap.input().memory().size() = stoi(memoryObj["size"].GetString());
                if (memoryObj.HasMember("type") && memoryObj["type"].IsString())
                    regMap.input().memory().type() = GetMemoryTypeNum(memoryObj["type"].GetString());
            }
            if (inputObj.HasMember("format") && inputObj["format"].IsString())
                regMap.input().format() = GetDataFormatNum(inputObj["format"].GetString());
        }

        // [field]-"outputs"
        if (document.HasMember("outputs") && document["outputs"].IsObject()) {
            const Value& outputsObj = document["outputs"];
            if (outputsObj.HasMember("memory") && outputsObj["memory"].IsObject()) {
                const Value& memoryObj = outputsObj["memory"];
                if (memoryObj.HasMember("name") && memoryObj["name"].IsString())
                    regMap.outputs().memory().name() = memoryObj["name"].GetString();
                if (memoryObj.HasMember("offset") && memoryObj["offset"].IsString())
                    regMap.outputs().memory().offset() = stoi(memoryObj["offset"].GetString());
                if (memoryObj.HasMember("size") && memoryObj["size"].IsString())
                    regMap.outputs().memory().size() = stoi(memoryObj["size"].GetString());
                if (memoryObj.HasMember("type") && memoryObj["type"].IsString())
                    regMap.outputs().memory().type() = GetMemoryTypeNum(memoryObj["type"].GetString());
            }
            if (outputsObj.HasMember("outputList") && outputsObj["outputList"].IsObject()) {
                const Value& outputListObj = outputsObj["outputList"];
                if (outputListObj.HasMember("output") && outputListObj["output"].IsArray()) {
                    const Value& outputArr = outputListObj["output"];
                    regMap.outputs().outputlist().output_size() = outputArr.Size();
                    for (SizeType i = 0; i < regMap.outputs().outputlist().output_size(); i++) {
                        const Value &outputObj = outputArr[i];
                        deepx_rmapinfo::InOutput output;
                        if (outputObj.HasMember("name") && outputObj["name"].IsString())
                            output.name() = outputObj["name"].GetString();
                        if (outputObj.HasMember("shapes") && outputObj["shapes"].IsObject()) {
                            const Value& shapesObj = outputObj["shapes"];
                            if (shapesObj.HasMember("shape") && shapesObj["shape"].IsArray()) {
                                const Value& shapeArr = shapesObj["shape"];
                                output.shapes().shape_size() = shapeArr.Size();
                                for (SizeType j = 0; j < output.shapes().shape_size(); j++) {
                                    output.shapes().shape().push_back(shapeArr[j].GetInt64());
                                }
                            }
                        }
                        if (outputObj.HasMember("type") && outputObj["type"].IsString())
                            output.type() = GetDataTypeNum(outputObj["type"].GetString());
                        if (outputObj.HasMember("memory") && outputObj["memory"].IsObject()) {
                            const Value &memoryObj = outputObj["memory"];
                            if (memoryObj.HasMember("name") && memoryObj["name"].IsString())
                                output.memory().name() = memoryObj["name"].GetString();
                            if (memoryObj.HasMember("offset") && memoryObj["offset"].IsString())
                                output.memory().offset() = stoi(memoryObj["offset"].GetString());
                            if (memoryObj.HasMember("size") && memoryObj["size"].IsString())
                                output.memory().size() = stoi(memoryObj["size"].GetString());
                            if (memoryObj.HasMember("type") && memoryObj["type"].IsString())
                                output.memory().type() = GetMemoryTypeNum(memoryObj["type"].GetString());
                        }
                        if (outputObj.HasMember("format") && outputObj["format"].IsString())
                            output.format() = GetDataFormatNum(outputObj["format"].GetString());
                        regMap.outputs().outputlist().output().push_back(output);
                    }
                }
            }
        }

        // [field]-"momorys"
        if (document.HasMember("memorys") && document["memorys"].IsObject()) {
            const Value& memorysObj = document["memorys"];
            if (memorysObj.HasMember("memory") && memorysObj["memory"].IsArray()) {
                const Value& memoryArr = memorysObj["memory"];
                regMap.memorys().memory_size() = memoryArr.Size();
                for (SizeType i = 0; i < regMap.memorys().memory_size(); i++) {
                    const Value& memoryObj = memoryArr[i];
                    deepx_rmapinfo::Memory memory;
                    if (memoryObj.HasMember("name") && memoryObj["name"].IsString())
                        memory.name() = memoryObj["name"].GetString();
                    if (memoryObj.HasMember("size") && memoryObj["size"].IsString())
                        memory.size() = stoi(memoryObj["size"].GetString());
                    if (memoryObj.HasMember("offset") && memoryObj["offset"].IsString())
                        memory.offset() = stoi(memoryObj["offset"].GetString());
                    regMap.memorys().memory().push_back(memory);
                }
            }
        }

        // [field]-"layers"
        if (document.HasMember("layers") && document["layers"].IsObject()) {
            const Value& layersObj = document["layers"];
            if (layersObj.HasMember("layer") && layersObj["layer"].IsArray()) {
                const Value& layerArr = layersObj["layer"];
                regMap.layers().layer_size() = layerArr.Size();
                for (SizeType i = 0; i < regMap.layers().layer_size(); i++) {
                    const Value& layerObj = layerArr[i];
                    deepx_rmapinfo::Layer layer;
                    if (layerObj.HasMember("memory") && layerObj["memory"].IsObject()) {
                        const Value& memoryObj = layerObj["memory"];
                        if (memoryObj.HasMember("name") && memoryObj["name"].IsString())
                            layer.memory().name() = memoryObj["name"].GetString();
                        if (memoryObj.HasMember("offset") && memoryObj["offset"].IsString())
                            layer.memory().offset() = stoi(memoryObj["offset"].GetString());
                        if (memoryObj.HasMember("size") && memoryObj["size"].IsString())
                            layer.memory().size() = stoi(memoryObj["size"].GetString());
                        if (memoryObj.HasMember("type") && memoryObj["type"].IsString())
                            layer.memory().type() = GetMemoryTypeNum(memoryObj["type"].GetString());
                    }
                    if (layerObj.HasMember("number") && layerObj["number"].IsObject()) {
                        const Value &memoryObj = layerObj["number"];
                        if (memoryObj.HasMember("tile") && memoryObj["tile"].IsInt64())
                            layer.number().layer() = memoryObj["tile"].GetInt64();
                        if (memoryObj.HasMember("layer") && memoryObj["layer"].IsInt64())
                            layer.number().tile() = memoryObj["layer"].GetInt64();
                    }
                    if (layerObj.HasMember("input") && layerObj["input"].IsObject()) {
                        const Value& inputObj = layerObj["input"];
                        if (inputObj.HasMember("shapes") && inputObj["shapes"].IsObject()) {
                            const Value& shapesObj = inputObj["shapes"];
                            if (shapesObj.HasMember("shape") && shapesObj["shape"].IsArray()) {
                                const Value& shapeArr = shapesObj["shape"];
                                layer.input().shapes().shape_size() = shapeArr.Size();
                                for (SizeType j = 0; j < layer.input().shapes().shape_size(); j++) {
                                    layer.input().shapes().shape().push_back(shapeArr[j].GetInt64());
                                }
                            }
                        }
                        if (inputObj.HasMember("type") && inputObj["type"].IsString())
                            layer.input().type() = GetDataTypeNum(inputObj["type"].GetString());
                    }
                    if (layerObj.HasMember("output") && layerObj["output"].IsObject()) {
                        const Value& outputObj = layerObj["output"];
                        if (outputObj.HasMember("shapes") && outputObj["shapes"].IsObject()) {
                            const Value& shapesObj = outputObj["shapes"];
                            if (shapesObj.HasMember("shape") && shapesObj["shape"].IsArray()) {
                                const Value& shapeArr = shapesObj["shape"];
                                layer.output().shapes().shape_size() = shapeArr.Size();
                                for (SizeType j = 0; j < layer.output().shapes().shape_size(); j++) {
                                    layer.output().shapes().shape().push_back(shapeArr[j].GetInt64());
                                }
                            }
                        }
                        if (outputObj.HasMember("type") && outputObj["type"].IsString())
                            layer.output().type() = GetDataTypeNum(outputObj["type"].GetString());
                    }
                    if (layerObj.HasMember("operators") && layerObj["operators"].IsObject()) {
                        const Value& operatorsObj = layerObj["operators"];
                        if (operatorsObj.HasMember("operator") && operatorsObj["operator"].IsArray()) {
                            const Value& operatorArr = operatorsObj["operator"];
                            layer.operators().operator_size() = operatorArr.Size();
                            for (SizeType j = 0; j < layer.operators().operator_size(); j++) {
                                const Value& operatorObj = operatorArr[j];
                                layer.operators().anoperator().push_back(operatorObj.GetString());
                            }
                        }
                    }
                    if (layerObj.HasMember("preLayers") && layerObj["preLayers"].IsObject()) {
                        const Value& preLayersObj = layerObj["preLayers"];
                        if (preLayersObj.HasMember("preLayer") && preLayersObj["preLayer"].IsArray()) {
                            const Value& preLayerArr = preLayersObj["preLayer"];
                            layer.preLayers().preLayer().layer_size() = preLayerArr.Size();
                            for (SizeType j = 0; j < layer.preLayers().preLayer().layer_size(); j++) {
                                const Value &numberObj = preLayerArr[j];
                                if (numberObj.HasMember("number") && numberObj["number"].IsInt64())
                                    layer.preLayers().preLayer().number().push_back(numberObj["number"].GetInt64());
                            }
                        }
                    }
                    if (layerObj.HasMember("nextLayers") && layerObj["nextLayers"].IsObject()) {
                        const Value& nextLayersObj = layerObj["nextLayers"];
                        if (nextLayersObj.HasMember("nextLayer") && nextLayersObj["nextLayer"].IsArray()) {
                            const Value& nextLayerArr = nextLayersObj["nextLayer"];
                            layer.nextLayers().nextLayer().layer_size() = nextLayerArr.Size();
                            for (SizeType j = 0; j < layer.nextLayers().nextLayer().layer_size(); j++) {
                                const Value &numberObj = nextLayerArr[j];
                                if (numberObj.HasMember("number")  && numberObj["number"].IsInt64())
                                    layer.nextLayers().nextLayer().number().push_back(numberObj["number"].GetInt64());
                            }
                        }
                    }
                    regMap.layers().layer().push_back(layer);
                }
            }
        }
        param.m_rmap().push_back(regMap);
    }
    return param;
}
}/* namespace dxrt */
