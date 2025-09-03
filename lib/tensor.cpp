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

#include <iostream>
#include <fstream>
using std::string;

namespace dxrt {

Tensor::Tensor(string name_, std::vector<int64_t> shape_, DataType type_, void *data_)
: _name(name_), _shape(shape_), _type(type_), _data(data_)
{
    _elemSize = GetDataSize_Datatype(static_cast<DataType>(_type));
    // if(_shape.size()>=4)
    //    _inc = _elemSize*( 64*( _shape[3]/64) + (int)(((_shape[3]%64)>0) ? 64 : 0) );
    if (_shape.size()>=4)
        _inc = _elemSize*_shape[3];
}
Tensor::Tensor(const Tensor &tensor_, void *data_)
:_name(tensor_._name), _shape(tensor_._shape), _type(tensor_._type),
    _phyAddr(tensor_._phyAddr), _inc(tensor_._inc), _elemSize(tensor_._elemSize)
{
    if (data_ == nullptr)
    {
        _data = tensor_._data;
    }
    else
    {
        _data = data_;
    }
}
Tensor::~Tensor()
{
    // delete data buffer if _dataReleaseFlag is true
    if ( _data != nullptr && _dataReleaseFlag )
    {
        // std::cout << "Tensor::~Tensor() ptr=" << _data << std::endl;
        delete[] reinterpret_cast<uint8_t*>(_data);
        _data = nullptr;
    }
}
const string &Tensor::name() const
{
    return _name;
}
std::vector<int64_t> &Tensor::shape()
{
    return _shape;
}
DataType &Tensor::type()
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
void* Tensor::data(int h, int w, int c)
{
    if (h < 0 || h >= _shape[1] || w < 0 || w >= _shape[2] || c < 0 || c >= _shape[3]) {
        throw std::out_of_range("Invalid tensor indices");
    }
    uint8_t *ptr = static_cast<uint8_t*>(_data) + h*_shape[2]*_inc + w*_inc + _elemSize*c;
    return static_cast<void*>(ptr);
}

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
    // os << " ], " << std::hex << tensor._data;
    if (tensor._phyAddr != 0)
    {
        os << ", " << tensor._phyAddr;
    }
    os << std::dec;
    return os;
}

void DataDumpBin(std::string filename, std::vector<std::shared_ptr<dxrt::Tensor>> tensors)
{
    // DXRT_ASSERT(!filename.empty(), string(__func__)+": filename is empty");
    if ( filename.empty() )
        throw InvalidArgumentException(EXCEPTION_MESSAGE("filename is empty"));

    std::ofstream out(filename, std::ios::binary);
    // DXRT_ASSERT(out.is_open(), "Failed to open " + filename);
    if ( !out.is_open() )
        throw  InvalidOperationException(EXCEPTION_MESSAGE("Failed to open " + filename));

    for (auto &tensor : tensors)
    {
        // cout << "dump " << tensor->name() << " to " << filename << endl;
        uint8_t *bytes = reinterpret_cast<uint8_t*>(tensor->data());
        auto size = vectorProduct(tensor->shape())*tensor->elem_size();
        out.write(reinterpret_cast<const char*>(bytes), size);
    }
    out.close();
}

void DataDumpBin(std::string filename, std::vector<Tensor> tensors)
{
    // DXRT_ASSERT(!filename.empty(), string(__func__)+": filename is empty");
    if ( filename.empty() )
        throw InvalidArgumentException(EXCEPTION_MESSAGE("filename is empty"));


    std::ofstream out(filename, std::ios::binary);
    //DXRT_ASSERT(out.is_open(), "Failed to open " + filename);
    if ( !out.is_open() )
        throw InvalidArgumentException(EXCEPTION_MESSAGE("Failed to open " + filename));

    for (auto &tensor : tensors)
    {
        uint8_t *bytes = reinterpret_cast<uint8_t*>(tensor.data());
        auto size = vectorProduct(tensor.shape())*tensor.elem_size();
        out.write(reinterpret_cast<const char*>(bytes), size);
    }
    out.close();
}

/* TODO : Devices */
/* TODO : Create Task MAP */

}  // namespace dxrt
