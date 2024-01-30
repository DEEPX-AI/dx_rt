// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/util.h"

using namespace std;

namespace dxrt {

Tensor::Tensor(string name_, std::vector<int64_t> shape_, DataType type_, void *data_)
: _name(name_), _shape(shape_), _type(type_), _data(data_)
{
    LOG_DXRT_DBG << "1: " << _name << ", " << this << endl;
    _elemSize = GetDataSize((deepx_rmapinfo::DataType)_type);
    if(_shape.size()>=4)
        _inc = _elemSize*( 64*( _shape[3]/64) + (int)(((_shape[3]%64)>0) ? 64 : 0) );
}
Tensor::Tensor(const Tensor &tensor_, void *data_)
:_name(tensor_._name), _shape(tensor_._shape), _type(tensor_._type), _inc(tensor_._inc), _elemSize(tensor_._elemSize), _phyAddr(tensor_._phyAddr)
{
    LOG_DXRT_DBG << "2: " << _name << ", " << this << endl;
    _data = (data_==nullptr) ? tensor_._data : data_;
}
Tensor::~Tensor()
{
    LOG_DXRT_DBG << this << endl;
    // LOG_DXRT_DBG << _name << endl;
}
string &Tensor::name()
{
    return _name;
}
vector<int64_t> &Tensor::shape()
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
    void *ptr = _data + h*_shape[2]*_inc + w*_inc + _elemSize*c;
    return ptr;
}

ostream& operator<<(ostream& os, const Tensor& tensor)
{
    os << dec << tensor._name << ", " << tensor._type << ", [";
    for(auto &s:tensor._shape) os << s << ", ";
    os << " ], " << hex << tensor._data;
    if(tensor._phyAddr!=0)
    {
        os << ", " << tensor._phyAddr;
    }
    os << dec;
    return os;
}

/* TODO : Devices */
/* TODO : Create Task MAP */

} // namespace dxrt
