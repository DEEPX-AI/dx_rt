/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <tuple>
#include <vector>
#ifdef _WINDOWS
#include <limits>
    #ifdef max
        #undef max
        #undef min
    #endif  // max
#endif

#include "dxrt/datatype.h"
#include "dxrt/common.h"

#define MIN_COMPILER_VERSION "1.18.1"
#define MIN_SINGLEFILE_VERSION 6
#define MAX_SINGLEFILE_VERSION 7



namespace deepx_binaryinfo {
struct DXRT_API Models {
    std::string &npu()     { return _npu; }
    std::string &name()    { return _name; }  // npu task name
    std::string &str()     { return _str; }  // info json data
    std::vector<char> &buffer()   { return _buffer; }  // binary data
    int64_t &offset() { return _offset; }
    int64_t &size()   { return _size; }

    std::string _npu;
    std::string _name;
    std::string _str;
    std::vector<char> _buffer;
    int64_t _offset = 0;
    int64_t _size = 0;
};

struct DXRT_API BinaryInfoDatabase {
    Models &merged_model()       { return _merged_model; }
    std::vector<Models> &npu_models() { return _npu_models; }
    Models &npu_models(int i)    { return _npu_models[i]; }
    std::vector<Models> &cpu_models() { return _cpu_models; }
    Models &cpu_models(int i)    { return _cpu_models[i]; }
    Models &graph_info()         { return _graph_info; }
    std::vector<Models> &rmap()       { return _rmap; }
    Models &rmap(int i)          { return _rmap[i]; }
    std::vector<Models> &weight()     { return _weight; }
    Models &weight(int i)        { return _weight[i]; }
    std::vector<Models> &rmap_info()  { return _rmap_info; }
    Models &rmap_info(int i)     { return _rmap_info[i]; }
    std::vector<Models> &bitmatch_mask()  { return _bitmatch_mask; }
    Models &bitmatch_mask(int i)     { return _bitmatch_mask[i]; }

    Models _merged_model;
    std::vector<Models> _npu_models;
    std::vector<Models> _cpu_models;
    Models _graph_info;
    std::vector<Models> _rmap;
    std::vector<Models> _weight;
    std::vector<Models> _rmap_info;
    std::vector<Models> _bitmatch_mask;

    // version info (file-format & compiler)
    int32_t _dxnnFileFormatVersion;
    std::string _compilerVersion;
};
} /* namespace deepx_binaryinfo */

namespace deepx_graphinfo {
struct DXRT_API Tensor {
    std::string &name() { return _name; }
    const std::string &name() const { return _name; }
    std::string &owner() { return _owner; }
    const std::string &owner() const { return _owner; }
    std::vector<std::string> &users() { return _users; }
    const std::vector<std::string> &users() const { return _users; }

    std::string _name;
    std::string _owner;
    std::vector<std::string> _users;
};

struct DXRT_API SubGraph {
    std::string &name() { return _name; }
    const std::string &name() const { return _name; }
    std::string &device() { return _device; }
    const std::string &device() const { return _device; }
    std::vector<Tensor> &inputs()  { return _inputs; }
    const std::vector<Tensor> &inputs() const { return _inputs; }
    Tensor &inputs(int i)     { return _inputs[i]; }
    const Tensor &inputs(int i) const { return _inputs[i]; }
    std::vector<Tensor> &outputs() { return _outputs; }
    const std::vector<Tensor> &outputs() const { return _outputs; }
    Tensor &outputs(int i)    { return _outputs[i]; }
    const Tensor &outputs(int i) const { return _outputs[i]; }

    std::string _name;

    std::string _device;

    std::vector<Tensor> _inputs;
    std::vector<Tensor> _outputs;
};

struct DXRT_API GraphInfoDatabase {
    bool &use_offloading() { return _use_offloading; }
    std::vector<std::string> &topoSort_order()        { return _topoSort_order; }

    std::vector<SubGraph> &subgraphs() { return _subgraphs; }
    SubGraph &subgraphs(int i)    { return _subgraphs[i]; }
    std::vector<std::string> &inputs()         { return _inputs; }
    std::vector<std::string> &outputs()        { return _outputs; }

    bool _use_offloading;
    std::vector<std::string> _topoSort_order;

    std::vector<std::string> _inputs;
    std::vector<std::string> _outputs;
    std::vector<SubGraph> _subgraphs;
};

} /* namespace deepx_graphinfo */

namespace deepx_rmapinfo {
struct DXRT_API Version {
    std::string &npu()      { return _npu; }
    std::string &rmap()     { return _rmap; }
    std::string &rmap_info() { return _rmap_info; }
    std::string &opt_level() { return _opt_level; }

    std::string _npu;
    std::string _rmap;
    std::string _rmap_info;
    std::string _opt_level;
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
    uint32_t  _op_mode = 0;
    uint32_t  _checkpoints[3] = {0, 0, 0};

};

struct DXRT_API Memory {
    std::string  &name()   { return _name; }
    int64_t &offset() { return _offset; }
    int64_t &size()   { return _size; }
    int &type()   { return _type; }

    std::string  _name;
    int64_t _offset = 0;
    int64_t _size = 0;
    int _type = 0;
};

struct DXRT_API ModelMemory {
    int64_t &model_memory_size()   { return _model_memory_size; }
    Memory &rmap() { return _rmap; }
    Memory &weight() { return _weight; }
    Memory &input() { return _input; }
    Memory &output() { return _output; }
    Memory &temp() { return _temp; }

    int64_t _model_memory_size = 0;
    Memory _rmap;
    Memory _weight;
    Memory _input;
    Memory _output;
    Memory _temp;
};

struct TensorInfo {
    std::string& name() { return _name; }
    int& dtype() { return _dtype; }
    std::vector<int64_t>& shape() { return _shape; }
    std::string& name_encoded() { return _name_encoded; }
    int& dtype_encoded() { return _dtype_encoded; }
    std::vector<int64_t>& shape_encoded() { return _shape_encoded; }
    int& layout() { return _layout; }
    int& align_unit() { return _align_unit; }
    int& transpose() { return _transpose; }
    Memory& memory() { return _memory; }
    float& scale() { return _scale; }
    float& bias() { return _bias; }
    bool& use_quantization() { return _use_quantization; }
    int& elem_size() { return _elem_size; }

    std::string _name;             // Original ONNX tensor name
    int _dtype;            // Original data type (e.g., "INT8", "FLOAT32", etc.)
    std::vector<int64_t> _shape;       // Original tensor shape
    std::string _name_encoded;     // NPU encoded tensor name
    int _dtype_encoded;    // NPU encoded data type
    std::vector<int64_t> _shape_encoded;  // NPU encoded tensor shape
    int _layout = 0;           // Tensor layout (e.g., "PRE_IM2COL", "ALIGNED", etc.)
    int _align_unit = 0;           // Alignment unit (e.g., 16, 64, etc.)
    int _transpose = 0;        // Transpose direction (e.g., "CHANNEL_FIRST_TO_LAST")
    float _scale = 0.0;            // Quantization sclale
    float _bias = 0.0;             // Quantization bias
    bool _use_quantization = false;  // Whether to apply quantization
    Memory _memory;                // Tensor memory information
    int _elem_size = 0;
};

struct DXRT_API RegisterInfoDatabase {
    Version& version() { return _version; }
    std::string& name() { return _name; }
    std::string& mode() { return _mode; }
    Npu& npu() { return _npu; }
    int64_t& size() { return _size; }
    Counts& counts() { return _counts; }
    std::vector<TensorInfo>& inputs() { return _inputs; }
    std::vector<TensorInfo>& outputs() { return _outputs; }
    ModelMemory& model_memory() { return _model_memory; }
    bool is_initialized() const {
        return _size != -1;
    }
    Version  _version;
    std::string   _name;
    std::string   _mode;
    Npu      _npu;
    int64_t  _size = -1;
    Counts   _counts;
    std::vector<TensorInfo> _inputs;
    std::vector<TensorInfo> _outputs;
    ModelMemory  _model_memory;
};

struct DXRT_API rmapInfoDatabase {
    std::vector<RegisterInfoDatabase> &rmap_info() { return _rmap_info; }
    RegisterInfoDatabase &rmap_info(int i)    { return _rmap_info[i]; }

    std::vector<RegisterInfoDatabase> _rmap_info;
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
    DataType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int>::min(),
    DataType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int>::max()
};

enum DXRT_API MemoryType : int {
    MEMORYTYPE_NONE = 0,
    DRAM = 1,
    ARGMAX = 2,
    PPU = 3,
    MemoryType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int>::min(),
    MemoryType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int>::max()
};

enum DXRT_API Layout : int {
    LAYOUT_NONE = 0,
    PRE_FORMATTER = 1,
    PRE_IM2COL = 2,
    FORMATTED = 3,
    ALIGNED = 4,
    PPU_YOLO = 5,
    PPU_FD = 6,
    PPU_POSE = 7,
    Layout_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int>::min(),
    Layout_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int>::max()
};

inline const char* LayoutToString(Layout layout) {
    switch (layout) {
        case LAYOUT_NONE: return "LAYOUT_NONE";
        case PRE_FORMATTER: return "PRE_FORMATTER";
        case PRE_IM2COL: return "PRE_IM2COL";
        case FORMATTED: return "FORMATTED";
        case ALIGNED: return "ALIGNED";
        case PPU_YOLO: return "PPU_YOLO";
        case PPU_FD: return "PPU_FD";
        case PPU_POSE: return "PPU_POSE";
        default: return "UNKNOWN_LAYOUT";
    }
}

enum DXRT_API Transpose : int {
    TRANSPOSE_NONE = 0,
    CHANNEL_FIRST_TO_LAST = 1,
    CHANNEL_LAST_TO_FIRST = 2,
    Transpose_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int>::min(),
    Transpose_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int>::max()
};

inline const char* TransposeToString(Transpose transpose) {
    switch (transpose) {
        case TRANSPOSE_NONE: return "TRANSPOSE_NONE";
        case CHANNEL_FIRST_TO_LAST: return "CHANNEL_FIRST_TO_LAST";
        case CHANNEL_LAST_TO_FIRST: return "CHANNEL_LAST_TO_FIRST";
        default: return "UNKNOWN_TRANSPOSE";
    }
}

inline DataType GetDataTypeNum(const std::string& str) {
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

inline MemoryType GetMemoryTypeNum(const std::string& str) {
    if (str == "MEMORYTYPE_NONE") return MemoryType::MEMORYTYPE_NONE;
    if (str == "DRAM")            return MemoryType::DRAM;
    if (str == "ARGMAX")          return MemoryType::ARGMAX;
    if (str == "PPU")             return MemoryType::PPU;
    return MEMORYTYPE_NONE;
}

inline Layout GetLayoutNum(const std::string& str) {
    if (str == "LAYOUT_NONE")     return Layout::LAYOUT_NONE;
    if (str == "PRE_FORMATTER")       return Layout::PRE_FORMATTER;
    if (str == "PRE_IM2COL")       return Layout::PRE_IM2COL;
    if (str == "FORMATTED")       return Layout::FORMATTED;
    if (str == "ALIGNED")       return Layout::ALIGNED;
    if (str == "PPU_YOLO")       return Layout::PPU_YOLO;
    if (str == "PPU_FD")       return Layout::PPU_FD;
    if (str == "PPU_POSE")       return Layout::PPU_POSE;
    return Layout::LAYOUT_NONE;
}

inline Transpose GetTransposeNum(const std::string& str) {
    if (str == "TRANSPOSE_NONE")           return Transpose::TRANSPOSE_NONE;
    if (str == "CHANNEL_FIRST_TO_LAST")    return Transpose::CHANNEL_FIRST_TO_LAST;
    if (str == "CHANNEL_LAST_TO_FIRST")    return Transpose::CHANNEL_LAST_TO_FIRST;
    return Transpose::TRANSPOSE_NONE;
}
} /* namespace deepx_rmapinfo */

namespace dxrt {
inline int getElementSize(int dataTypeEncoded) {
    if (dataTypeEncoded == static_cast<int>(DataType::UINT8) || dataTypeEncoded == static_cast<int>(DataType::INT8) || dataTypeEncoded == static_cast<int>(DataType::NONE_TYPE)) return 1;
    if (dataTypeEncoded == static_cast<int>(DataType::UINT16) || dataTypeEncoded == static_cast<int>(DataType::INT16)) return 2;
    if (dataTypeEncoded == static_cast<int>(DataType::UINT32) || dataTypeEncoded == static_cast<int>(DataType::INT32) || dataTypeEncoded == static_cast<int>(DataType::FLOAT)) return 4;
    if (dataTypeEncoded == static_cast<int>(DataType::UINT64) || dataTypeEncoded == static_cast<int>(DataType::INT64)) return 8;
    LOG_DXRT_ERR("Invalid type : " << dataTypeEncoded);
    return 1;
}
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
DXRT_API std::string LoadModelParam(ModelDataBase& modelDB, std::string file);
DXRT_API int LoadGraphInfo(deepx_graphinfo::GraphInfoDatabase& graphInfo, ModelDataBase& data);
DXRT_API int LoadBinaryInfo(deepx_binaryinfo::BinaryInfoDatabase& binInfo,char *buffer, int fileSize);
DXRT_API std::string LoadRmapInfo(deepx_rmapinfo::rmapInfoDatabase& rampInfo, ModelDataBase& data);
bool isSupporterModelVersion(const std::string& vers);
}