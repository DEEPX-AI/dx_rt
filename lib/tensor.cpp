/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/util.h"
#include "dxrt/exception/exception.h"
#include <algorithm>

#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif

#include <iostream>
#include <fstream>
using std::string;

namespace dxrt {

Tensor::Tensor(string name_, std::vector<int64_t> shape_, DataType type_, void *data_, int memory_type_)  // NOSONAR: S1238 due to legacy code structure, refactor needed
: _name(name_), _shape(shape_), _type(type_), _data(data_), _memoryType(memory_type_)
{
    _elemSize = GetDataSize_Datatype(_type);
    if (_shape.size() >= 4)
        _inc = static_cast<uint32_t>(_elemSize*_shape[3]);
}
Tensor::Tensor(const Tensor &tensor_, void* data_)  // NOSONAR: S1238 due to legacy code structure, refactor needed
:_name(tensor_._name), _shape(tensor_._shape), _type(tensor_._type),
    _phyAddr(tensor_._phyAddr), _inc(tensor_._inc), _elemSize(tensor_._elemSize), _memoryType(tensor_._memoryType)
{
    if (data_ == nullptr)
    {
        _data = tensor_._data;
    }
    else
    {
        _data = data_;
    }

#ifdef USE_ORT
    // Deep copy the shared_ptr through opaque pointer
    if (tensor_._ortValuePtr != nullptr) {
        _ortValuePtr = new std::shared_ptr<Ort::Value>(*static_cast<std::shared_ptr<Ort::Value>*>(tensor_._ortValuePtr));
    }
    _isOrtOwned = tensor_._isOrtOwned;
#endif
}

Tensor& Tensor::operator=(const Tensor& other)
{
    if (this == &other)
    {
        return *this;
    } 

    _name = other._name;
    _shape = other._shape;
    _type = other._type;
    _data = other._data;
    _phyAddr = other._phyAddr;
    _inc = other._inc;
    _elemSize = other._elemSize;
    _memoryType = other._memoryType;
    _dataReleaseFlag = other._dataReleaseFlag;
#ifdef USE_ORT
    // Clean up existing OrtValue
    if (_ortValuePtr != nullptr) 
    {
        delete static_cast<std::shared_ptr<Ort::Value>*>(_ortValuePtr);
        _ortValuePtr = nullptr;
    }
    // Deep copy: creates new shared_ptr, incrementing refcount
    if (other._ortValuePtr != nullptr) 
    {
        _ortValuePtr = new std::shared_ptr<Ort::Value>(
            *static_cast<std::shared_ptr<Ort::Value>*>(other._ortValuePtr));
    }

    _isOrtOwned = other._isOrtOwned;
#endif

    return *this;
}

Tensor::Tensor(Tensor&& other) noexcept
: _name(std::move(other._name)), _shape(std::move(other._shape)), _type(other._type),
  _data(other._data), _phyAddr(other._phyAddr), _inc(other._inc),
  _elemSize(other._elemSize), _memoryType(other._memoryType),
  _dataReleaseFlag(other._dataReleaseFlag)
#ifdef USE_ORT
  , _ortValuePtr(other._ortValuePtr)
  , _isOrtOwned(other._isOrtOwned)
#endif
{
    other._data = nullptr;
    other._phyAddr = 0;
    other._dataReleaseFlag = false;
#ifdef USE_ORT
    other._ortValuePtr = nullptr;
    other._isOrtOwned = false;
#endif
}

Tensor& Tensor::operator=(Tensor&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    } 
    _name = std::move(other._name);
    _shape = std::move(other._shape);
    _type = other._type;
    _data = other._data;
    _phyAddr = other._phyAddr;
    _inc = other._inc;
    _elemSize = other._elemSize;
    _memoryType = other._memoryType;
    _dataReleaseFlag = other._dataReleaseFlag;
    other._data = nullptr;
    other._phyAddr = 0;
    other._dataReleaseFlag = false;
#ifdef USE_ORT
    // Clean up existing OrtValue
    if (_ortValuePtr != nullptr) 
    {
        delete static_cast<std::shared_ptr<Ort::Value>*>(_ortValuePtr);
    }
    // Transfer ownership (no new/delete, no refcount change)
    _ortValuePtr = other._ortValuePtr;
    other._ortValuePtr = nullptr;
    _isOrtOwned = other._isOrtOwned;
    other._isOrtOwned = false;
#endif
    return *this;
}

#ifdef USE_ORT
Tensor::Tensor(string name_, std::vector<int64_t> shape_, DataType type_,
               void *data_, void* ortValuePtr)
: _name(name_), _shape(shape_), _type(type_), _data(data_), _isOrtOwned(true)
{
    _elemSize = GetDataSize_Datatype(_type);
    if (_shape.size()>=4)
        _inc = static_cast<uint32_t>(_elemSize*_shape[3]);

    // Store the shared_ptr through opaque pointer
    if (ortValuePtr != nullptr) {
        _ortValuePtr = new std::shared_ptr<Ort::Value>(*static_cast<std::shared_ptr<Ort::Value>*>(ortValuePtr));
    }
}
#endif
Tensor::~Tensor()
{
    // delete data buffer if _dataReleaseFlag is true and not ORT-owned
#ifdef USE_ORT
    if ( _data != nullptr && _dataReleaseFlag && !_isOrtOwned )
    {
        auto dataPtr = static_cast<uint8_t*>(_data);
        delete[] dataPtr;
        _data = nullptr;
    }
    // Delete the opaque pointer wrapper (shared_ptr will handle OrtValue cleanup)
    if (_ortValuePtr != nullptr) {
        delete static_cast<std::shared_ptr<Ort::Value>*>(_ortValuePtr);
        _ortValuePtr = nullptr;
    }
#else
    if ( _data != nullptr && _dataReleaseFlag )
    {
        auto dataPtr = static_cast<uint8_t*>(_data);
        delete[] dataPtr;
        _data = nullptr;
    }
#endif
}
const string &Tensor::name() const
{
    return _name;
}
std::vector<int64_t> &Tensor::shape()
{
    return _shape;
}
const std::vector<int64_t> &Tensor::shape() const
{
    return _shape;
}
DataType &Tensor::type()
{
    return _type;
}
const DataType &Tensor::type() const
{
    return _type;
}
void* &Tensor::data()
{
    return _data;
}
uint64_t &Tensor::phy_addr()
{
    return _phyAddr;
}
uint32_t &Tensor::elem_size()
{
    return _elemSize;
}
int &Tensor::memory_type()
{
    return _memoryType;
}
void* Tensor::data(int h, int w, int c)
{
    if (h < 0 || h >= _shape[1] || w < 0 || w >= _shape[2] || c < 0 || c >= _shape[3]) {
        throw std::out_of_range("Invalid tensor indices");
    }
    auto ptr = static_cast<uint8_t*>(_data) + h*_shape[2]*_inc + w*_inc + _elemSize*c;
    return static_cast<void*>(ptr);
}

void Tensor::update_dynamic_shape(const std::vector<int64_t>& new_shape, void* new_data, uint64_t new_size_bytes)
{
    (void)new_size_bytes;  // Unused parameter, reserved for future validation

    if (new_shape.empty()) {
        return;
    }

    // Log the dynamic shape update for debugging
    std::string shape_str = "[";
    for (size_t i = 0; i < new_shape.size(); ++i) {
        if (i > 0) shape_str += ", ";
        shape_str += std::to_string(new_shape[i]);
    }
    shape_str += "]";

    LOG_DXRT_DBG << "Tensor '" << _name << "' dynamic shape updated: " << shape_str
                 << ", size: " << size_in_bytes() << " bytes" << std::endl;

    // Release existing data if release flag is set
    if (_data != nullptr && _dataReleaseFlag && _data != new_data) {
        auto dataPtr = static_cast<uint8_t*>(_data);
        delete[] dataPtr;
    }

    // Set new shape and data
    _shape = new_shape;
    _data = new_data;

    // Recalculate inc value (for 4D or higher tensors)
    if (_shape.size() >= 4) {
        _inc = _elemSize * static_cast<uint32_t>(_shape[3]);
    }

    // The new data is allocated externally, so set the release flag to false
    _dataReleaseFlag = false;
}

#ifdef USE_ORT
void Tensor::update_with_ort_value(const std::vector<int64_t>& new_shape, void* new_data,
                                   void* ortValuePtr)
{
    if (new_shape.empty()) {
        return;
    }

    // Log the dynamic shape update with ORT details
    std::string shape_str = "[";
    for (size_t i = 0; i < new_shape.size(); ++i) {
        if (i > 0) shape_str += ", ";
        shape_str += std::to_string(new_shape[i]);
    }
    shape_str += "]";

    LOG_DXRT_DBG << "Tensor '" << _name << "' updated with OrtValue: " << shape_str
                 << ", size: " << size_in_bytes() << " bytes, ORT-managed: true" << std::endl;

    // Release existing data if release flag is set (but not if it's ORT-owned)
    if (_data != nullptr && _dataReleaseFlag && !_isOrtOwned && _data != new_data) {
        auto dataPtr = static_cast<uint8_t*>(_data);
        delete[] dataPtr;
    }

    // Delete old opaque pointer if exists
    if (_ortValuePtr != nullptr) {
        delete static_cast<std::shared_ptr<Ort::Value>*>(_ortValuePtr);
        _ortValuePtr = nullptr;
    }

    // Set new shape, data, and OrtValue through opaque pointer
    _shape = new_shape;
    _data = new_data;
    if (ortValuePtr != nullptr) {
        _ortValuePtr = new std::shared_ptr<Ort::Value>(*static_cast<std::shared_ptr<Ort::Value>*>(ortValuePtr));
    }
    _isOrtOwned = true;

    // Recalculate inc value (for 4D or higher tensors)
    if (_shape.size() >= 4) {
        _inc = _elemSize * static_cast<uint32_t>(_shape[3]);
    }

    // ORT manages memory, so don't set release flag
    _dataReleaseFlag = false;
}
#endif



// private functions
// set data release flag, if flag is true, the data is deleted in the destructor
void Tensor::setDataReleaseFlag(bool flag)
{
    _dataReleaseFlag = flag;
}

std::ostream& operator<<(std::ostream& os, const Tensor& tensor)
{
    os << std::dec << tensor._name << ", " << tensor._type << ", [";
    for (size_t i = 0; i < tensor._shape.size(); ++i)
    {
        if (tensor._shape[i] == -1)
        {
            os << "unknown";
        }
        else
        {
            os << tensor._shape[i];
        }

        if (i < tensor._shape.size() - 1)
        {
            os << ", ";
        }
    }
    os << " ]";
    if (tensor._phyAddr != 0)
    {
        os << ", " << tensor._phyAddr;
    }
    os << std::dec;
    return os;
}

void DataDumpBin(std::string filename, std::vector<std::shared_ptr<dxrt::Tensor>> tensors)  // NOSONAR: S1238 due to legacy code structure, refactor needed
{
    if ( filename.empty() )
        throw InvalidArgumentException(EXCEPTION_MESSAGE("filename is empty"));

    std::ofstream out(filename, std::ios::binary);
    if ( !out.is_open() )
        throw  InvalidOperationException(EXCEPTION_MESSAGE("Failed to open " + filename));

    for (const auto& tensor : tensors)
    {
        auto bytes = static_cast<const uint8_t*>(tensor->data());
        auto size = vectorProduct(tensor->shape())*tensor->elem_size();
        out.write(static_cast<const char*>(static_cast<const void*>(bytes)), size);
    }
    out.close();
}

void DataDumpBin(std::string filename, std::vector<Tensor> tensors)  // NOSONAR: S1238 due to legacy code structure, refactor needed
{
    if ( filename.empty() )
        throw InvalidArgumentException(EXCEPTION_MESSAGE("filename is empty"));


    std::ofstream out(filename, std::ios::binary);
    if ( !out.is_open() )
        throw InvalidArgumentException(EXCEPTION_MESSAGE("Failed to open " + filename));

    for (auto &tensor : tensors)
    {
        auto bytes = static_cast<const uint8_t*>(tensor.data());
        auto size = vectorProduct(tensor.shape())*tensor.elem_size();
        out.write(static_cast<const char*>(static_cast<const void*>(bytes)), size);
    }
    out.close();
}

}  // namespace dxrt
