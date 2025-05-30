#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <tuple>
#ifdef _WINDOWS
#include <limits>
    #ifdef max
        #undef max
        #undef min
    #endif // max
#endif
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"
#include "rapidjson/rapidjson.h"

#define MIN_COMPILER_VERSION "1.18.1"
#define MIN_SINGLEFILE_VERSION 6
using namespace std;
namespace deepx_binaryinfo {
    struct DXRT_API Models {
        string &npu()     { return _npu; }
        string &name()    { return _name; }
        string &str()     { return _str; }
        vector<char> &buffer()   { return _buffer; }
        int64_t &offset() { return _offset; }
        int64_t &size()   { return _size; }

        string _npu;
        string _name;
        string _str;
        vector<char> _buffer;
        int64_t _offset = 0;
        int64_t _size = 0;

    };

    struct DXRT_API BinaryInfoDatabase {
        Models &merged_model()       { return _merged_model; }
        vector<Models> &npu_models() { return _npu_models; }
        Models &npu_models(int i)    { return _npu_models[i]; }
        vector<Models> &cpu_models() { return _cpu_models; }
        Models &cpu_models(int i)    { return _cpu_models[i]; }
        Models &graph_info()         { return _graph_info; }
        vector<Models> &rmap()       { return _rmap; }
        Models &rmap(int i)          { return _rmap[i]; }
        vector<Models> &weight()     { return _weight; }
        Models &weight(int i)        { return _weight[i]; }
        vector<Models> &rmap_info()  { return _rmap_info; }
        Models &rmap_info(int i)     { return _rmap_info[i]; }
        vector<Models> &bitmatch_mask()  { return _bitmatch_mask; }
        Models &bitmatch_mask(int i)     { return _bitmatch_mask[i]; }

        Models _merged_model;
        vector<Models> _npu_models;
        vector<Models> _cpu_models;
        Models _graph_info;
        vector<Models> _rmap;
        vector<Models> _weight;
        vector<Models> _rmap_info;
        vector<Models> _bitmatch_mask;

        // version info (file-format & compiler)
        int32_t _dxnnFileFormatVersion;
        std::string _compilerVersion;
    };
} /* namespace deepx_binaryinfo */

namespace deepx_graphinfo {
    struct DXRT_API KeyValueInfo {
        string &key() { return _key; }
        string &val() { return _val; }

        string _key;
        string _val;
    };

    struct DXRT_API GraphBindingDatabase {
        string &name() { return _name; }
        string &type() { return _type; }
        string &output_type() { return _output_type; }
        vector<KeyValueInfo> &inputs()  { return _inputs; }
        KeyValueInfo &inputs(int i)     { return _inputs[i]; }
        vector<KeyValueInfo> &outputs() { return _outputs; }
        KeyValueInfo &outputs(int i)    { return _outputs[i]; }

        string _name;
        string _type;
        string _output_type;
        vector<KeyValueInfo> _inputs;
        vector<KeyValueInfo> _outputs;
        
    };

    struct DXRT_API GraphInfoDatabase {
        vector<GraphBindingDatabase> &m_graph() { return _m_graph; }
        GraphBindingDatabase &m_graph(int i)    { return _m_graph[i]; }
        vector<string> &origin_input()        { return _origin_input; }
        vector<string> &origin_output()        { return _origin_output; }
        vector<string> &topoSort_order()        { return _topoSort_order; }

        vector<GraphBindingDatabase> _m_graph;    
        vector<string> _origin_input;    
        vector<string> _origin_output;    
        vector<string> _topoSort_order;    
    };
} /* namespace deepx_graphinfo */

namespace deepx_rmapinfo {
    struct DXRT_API Version {
        string &npu()      { return _npu; }
        string &rmap()     { return _rmap; }
        string &rmapinfo() { return _rmapinfo; }
        
        string _npu;
        string _rmap;
        string _rmapinfo;
    };

    struct DXRT_API Npu {
        int64_t &mac() { return _mac; }
        int64_t _mac = 0;
    };

    struct DXRT_API Counts {
        int64_t &layer() { return _layer; }
        int64_t &cmd()   { return _cmd; }

        int64_t _layer = 0;
        int64_t _cmd = 0;
    };

    struct DXRT_API Memory {
        string  &name()   { return _name; }
        int64_t &offset() { return _offset; }
        int64_t &size()   { return _size; }
        int64_t &type()   { return _type; }

        string  _name;
        int64_t _offset = 0;
        int64_t _size = 0;
        int64_t _type = 0;
    };

    struct DXRT_API Shapes {
        int64_t &shape_size()    { return _shape_size; }
        vector<int64_t> &shape() { return _shape; }
        int64_t &shape(int i)    { return _shape[i]; }

        int64_t _shape_size = 0;
        vector<int64_t> _shape;
    };

    struct DXRT_API InOutput {
        string  &name()   { return _name; }
        Shapes  &shapes() { return _shapes; }
        int64_t &type()   { return _type; }
        Memory  &memory() { return _memory; }
        int64_t &mode()   { return _mode; }
        int64_t &format() { return _format; }

        string  _name;
        Shapes  _shapes;
        int64_t _type = 0;
        Memory  _memory;
        int64_t _mode = 0;
        int64_t _format = 0;
    };

    struct DXRT_API OutputList {
        int64_t &output_size()     { return _output_size; }
        vector<InOutput> &output() { return _output; }
        InOutput &output(int i)    { return _output[i]; }

        int64_t _output_size = 0;
        vector<InOutput> _output;
    };

    struct DXRT_API Outputs {
        Memory  &memory()         { return _memory; }
        OutputList  &outputlist() { return _outputlist; }

        Memory _memory;
        OutputList _outputlist;
    };

    struct DXRT_API Memorys {
        int64_t &memory_size()   { return _memory_size; }
        vector<Memory> &memory() { return _memory; }
        Memory   &memory(int i)  { return _memory[i]; }
        
        int64_t _memory_size = 0;
        vector<Memory> _memory;
    };

    struct DXRT_API Number {
        int64_t &layer(){ return _layer; }
        int64_t &tile() { return _tile; }

        int64_t _layer = 0;
        int64_t _tile = 0;
    };

    struct DXRT_API Operators {
        int64_t &operator_size()   { return _operator_size; }
        inline vector<string> &anoperator() { return _anoperator; }
        string &anoperator(int i) { return _anoperator[i]; }
            
        int64_t _operator_size = 0;
        vector<string> _anoperator;
    };

    struct DXRT_API PreNextLayer {
        int64_t &layer_size()     { return _layer_size; }
        vector<int64_t> &number() { return _number; }
        int64_t &number(int i)    { return _number[i]; }

        int64_t _layer_size = 0;
        vector<int64_t> _number;
    };

    struct DXRT_API PreNextLayers {
        PreNextLayer &preLayer()  { return _preLayer; }
        PreNextLayer &nextLayer() { return _nextLayer; }

        PreNextLayer _preLayer;
        PreNextLayer _nextLayer;
    };

    struct DXRT_API Layer {
        Memory &memory()   { return _memory; }
        Number &number()   { return _number; }
        InOutput &input()  { return _input; }
        InOutput &output() { return _output; }
        Operators &operators() { return _operators; }
        PreNextLayers &preLayers() { return _preLayers; }
        PreNextLayers &nextLayers() { return _nextLayers; }

        Memory  _memory;
        Number  _number;
        InOutput _input;
        InOutput _output;
        Operators _operators;
        PreNextLayers _preLayers;
        PreNextLayers _nextLayers;
    };

    struct DXRT_API Layers {
        int64_t &layer_size()  { return _layer_size; }
        vector<Layer> &layer() { return _layer; }
        Layer   &layer(int i)  { return _layer[i]; }

        int64_t _layer_size = 0;
        vector<Layer> _layer;
    };

    struct DXRT_API RegisterInfoDatabase {
        Version  &version()  { return _version; }
        string   &model()    { return _model; }
        Npu      &npu()      { return _npu; }
        int64_t  &size()     { return _size; }
        int64_t  &traffic()  { return _traffic; }
        Counts   &counts()   { return _counts; }
        InOutput &input()    { return _input; }
        Outputs  &outputs()  { return _outputs; }
        Memorys  &memorys()  { return _memorys; }
        Layers   &layers()   { return _layers; }

        Version  _version;
        string   _model;
        Npu      _npu;
        int64_t  _size = 0;
        int64_t  _traffic = 0;
        Counts   _counts;
        InOutput _input;
        Outputs  _outputs;
        Memorys  _memorys;
        Layers   _layers;
    };

    struct DXRT_API rmapInfoDatabase {
        vector<RegisterInfoDatabase> &m_rmap() { return _m_rmap; }
        RegisterInfoDatabase &m_rmap(int i)    { return _m_rmap[i]; }

        vector<RegisterInfoDatabase> _m_rmap;
    };

    enum DXRT_API DataType : int {
        DATA_TYPE_NONE = 0,
        FLOAT32 = 1,
        UINT8 = 2,
        INT8 = 3,
        UINT16 = 4,
        INT16 = 5,
        INT32 = 6,
        INT64 = 7,
        UINT32 = 8,
        UINT64 = 9,
        DataType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
        DataType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
    };

    enum DXRT_API DataFormat : int {
        DATAFORMAT_NONE = 0,
        NCHWd = 1,
        NHWCd = 2,
        NHWd = 3,
        PPU_YOLO = 4,
        PPU_FD = 5,
        PPU_POSE = 6,
        DataFormat_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
        DataFormat_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
    };

    enum DXRT_API MemoryType : int {
        MEMORYTYPE_NONE = 0,
        DRAM = 1,
        ARGMAX = 2,
        PPU = 3,
        MemoryType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
        MemoryType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
    };

    enum DXRT_API InputMode : int {
        INPUTMODE_NONE = 0,
        FORMATTER = 1,
        IM2COL = 2,
        InputMode_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
        InputMode_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
    };

    inline DataType GetDataTypeNum (const string& str) {
        if (str == "TYPE_NONE") return DataType::DATA_TYPE_NONE;
        if (str == "UINT8")     return DataType::UINT8;
        if (str == "UINT16")    return DataType::UINT16;
        if (str == "UINT32")    return DataType::UINT32;
        if (str == "UINT64")    return DataType::UINT64;
        if (str == "INT8")      return DataType::INT8;
        if (str == "INT16")     return DataType::INT16;
        if (str == "INT32")     return DataType::INT32;
        if (str == "INT64")     return DataType::INT64;
        if (str == "FLOAT32")   return DataType::FLOAT32;
        return DataType::DATA_TYPE_NONE;
    };

    inline DataFormat GetDataFormatNum (const string& str) {
        if (str == "DATAFORMAT_NONE") return DataFormat::DATAFORMAT_NONE;
        if (str == "NCHWd")           return DataFormat::NCHWd;
        if (str == "NHWCd")           return DataFormat::NHWCd;
        if (str == "PPU_YOLO")        return DataFormat::PPU_YOLO;
        if (str == "PPU_FD")          return DataFormat::PPU_FD;
        if (str == "PPU_POSE")        return DataFormat::PPU_POSE;
        return DataFormat::DATAFORMAT_NONE;
    };

    inline MemoryType GetMemoryTypeNum (const string& str) {
        if (str == "MEMORYTYPE_NONE") return MemoryType::MEMORYTYPE_NONE;
        if (str == "DRAM")            return MemoryType::DRAM;
        if (str == "ARGMAX")          return MemoryType::ARGMAX;
        if (str == "PPU")             return MemoryType::PPU;
        return MEMORYTYPE_NONE;
    }

    inline InputMode GetInputModeNum (const string& str) {
        if (str == "INPUTMODE_NONE")  return InputMode::INPUTMODE_NONE;
        if (str == "FORMATTER")       return InputMode::FORMATTER;
        if (str == "IM2COL")          return InputMode::IM2COL;
        return InputMode::INPUTMODE_NONE;
    }
} /* namespace deepx_rmapinfo */

namespace dxrt {
struct DXRT_API ModelDataBase {
    deepx_graphinfo::GraphInfoDatabase deepx_graph;
    deepx_binaryinfo::BinaryInfoDatabase deepx_binary;
    deepx_rmapinfo::rmapInfoDatabase deepx_rmap;
};
DXRT_API std::ostream& operator<<(std::ostream&, const ModelDataBase&);
/** \brief parse a model, and show information
 * \return return 0 if model parsing is done successfully, 
           return -1 if failed to parse model
*/
DXRT_API int ParseModel(std::string file);
DXRT_API ModelDataBase LoadModelParam(std::string file);
DXRT_API string LoadModelParam(ModelDataBase& modelDB, std::string file);
//DXRT_API deepx_graphinfo::GraphInfoDatabase LoadGraphInfo(ModelDataBase data);
DXRT_API int LoadGraphInfo(deepx_graphinfo::GraphInfoDatabase& graphInfo, ModelDataBase& data);
//DXRT_API deepx_binaryinfo::BinaryInfoDatabase LoadBinaryInfo(char *buffer, int fileSize);
DXRT_API int LoadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& binInfo,char *buffer, int fileSize);
//DXRT_API deepx_rmapinfo::rmapInfoDatabase LoadRmapInfo(ModelDataBase data);
DXRT_API string LoadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& rampInfo, ModelDataBase& data);
bool isSupporterModelVersion(const string& vers);
}
