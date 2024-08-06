#include "dxrt/device_info_status.h"
#include "dxrt/device.h"
#include<map>
#include<iostream>
#include<sstream>

using std::string;
using std::endl;
using std::map;

map<int, string> device_types {{0,"ACC"},{1,"STD"}};
map<int, string> device_type_words {{0,"Accelator"},{1,"Standalone"}};
map<int, string> device_variants{{100,"L1"},{101,"L2"},{102,"L3"},{103,"L4"},
    {200,"M1"},{202,"M1"}};
map<int, string> board_types{{1,"SOM"},{2,"M.2"},{3,"H1"}};
map<int, string> memory_types{{1,"LPDDR4"},{2,"LPDDR5"}};
map<int, string> interface_types{{0,"ASIC"},{1,"FPGA"}};

constexpr int CHARBUFFER_SIZE = 128;

// internal helper functions
template<typename T>
static string map_lookup(const map<int, string>& m, T n)
{
    auto it = m.find(static_cast<int>(n));
    if(it == m.end())
    {
        char buf[CHARBUFFER_SIZE];
        snprintf(buf,CHARBUFFER_SIZE,"-ERROR(%d)-",n);
        return string(buf);
    }
    return it->second;
}
string convert_capacity(uint64_t n)
{
    constexpr uint64_t killo = 1024;
    constexpr uint64_t mega = killo *killo;
    constexpr uint64_t giga = mega*killo;
    constexpr uint64_t tera = giga*killo;
    double value = n;
    string postfix = "B";
    if(n >= tera)
    {
        value = static_cast<double>(n)/static_cast<double>(tera);
        postfix = "TiB";
    }
    else if (n>=giga)
    {
        value = static_cast<double>(n)/static_cast<double>(giga);
        postfix = "GiB";
    }
    else if (n>=mega)
    {
        value = static_cast<double>(n)/static_cast<double>(mega);
        postfix = "MiB";
    }
    else if (n>=killo)
    {
        value = static_cast<double>(n)/static_cast<double>(killo);
        postfix = "KiB";
    }
    char buffer[CHARBUFFER_SIZE];
    snprintf(buffer,CHARBUFFER_SIZE,"%.3g",value);
    return string(buffer)+postfix;
}
static string insert_comma(const string& str)
{
    int str_len = str.length();
    string ret = "";
    ret.reserve(str_len * 1.5);
    for(int i=0;i<str_len;i++)
    {
        ret.push_back(str[i]);
        int divisor = str_len - i;
        if((divisor>1)&&(divisor%3==1))
            ret.push_back(',');
    }
    return ret;
}

namespace dxrt {

DxrtDeviceInfoWithStatus::DxrtDeviceInfoWithStatus(int id, dxrt_device_info_t info, dxrt_device_status_t status)
:_id(id),_info(info), _status(status)
{

}


DxrtDeviceInfoWithStatus DxrtDeviceInfoWithStatus::getStatusInfo(DevicePtr device)
{
    int deviceId = device->id();
    auto info = device->info();
    auto status = device->status();
    return DxrtDeviceInfoWithStatus(deviceId,info,status);
}

string DxrtDeviceInfoWithStatus::dvfsStateInfoStr() const
{
    char buf[CHARBUFFER_SIZE];
    if (_status.dvfs_enable==1)
    {
        snprintf(buf,CHARBUFFER_SIZE,"dvfs Enabled(Max Frequency:%ud)", _status.dvfs_maxfreq);
    }
    else
    {
        snprintf(buf, CHARBUFFER_SIZE, "dvfs Disabled");
    }
    return string(buf);
}

string DxrtDeviceInfoWithStatus::npuStatusStr(int no) const
{
    char buf[CHARBUFFER_SIZE];
    snprintf(buf,CHARBUFFER_SIZE,"NPU %d: voltage %u mV, clock %u MHz, temperature %u'C", 
        no, _status.voltage[no], _status.clock[no], _status.temperature[no]);
    return string(buf);

}

string DxrtDeviceInfoWithStatus::deviceTypeStr() const
{
    return map_lookup(device_types,_info.type);
}
string DxrtDeviceInfoWithStatus::deviceTypeWord() const
{
    return map_lookup(device_type_words,_info.type);
}
string DxrtDeviceInfoWithStatus::deviceVariantStr() const
{
    return map_lookup(device_variants,_info.variant);
}
string DxrtDeviceInfoWithStatus::boardTypeStr() const
{
    return map_lookup(board_types,_info.bd_type);
}
string DxrtDeviceInfoWithStatus::memoryTypeStr() const
{
    return map_lookup(memory_types,_info.ddr_type);
}
string DxrtDeviceInfoWithStatus::interfaceTypeStr() const
{
#ifdef __linux__
    return map_lookup(interface_types, _info.interface);
#elif _WIN32
    return map_lookup(interface_types, _info.interface_value);
#endif
}

string DxrtDeviceInfoWithStatus::memorySizeStrBinaryPrefix() const
{
    return convert_capacity(_info.mem_size);
}

string DxrtDeviceInfoWithStatus::memorySizeStrWithComma() const
{
    return insert_comma(std::to_string(_info.mem_size))+"Byte";
}

string DxrtDeviceInfoWithStatus::allMemoryInfoStr() const
{
     char buffer[CHARBUFFER_SIZE];
     snprintf(buffer, CHARBUFFER_SIZE, "Type:%s, Addr:%p, size: %s(%s), clock: %udMHz", 
      memoryTypeStr().c_str(), reinterpret_cast<void*>(_info.mem_addr),
      memorySizeStrBinaryPrefix().c_str(), memorySizeStrWithComma().c_str(), _info.ddr_freq);
     return string(buffer);
}


string DxrtDeviceInfoWithStatus::fwVersionStr() const
{
    int version = _info.fw_ver;
    int v1 = version / 100;
    int v2 = (version%100)/10;
    int v3 = version%10;
    char buf[64];
    snprintf(buf,64,"%d.%d.%d",v1,v2,v3);
    return string(buf);
}

std::ostream& DxrtDeviceInfoWithStatus::infoToStream(std::ostream& os) const
{

    os << std::showbase << std::dec << "Device " << getId()
      << ": " << deviceVariantStr()<< ", "<< deviceTypeWord()<<" type" << endl;
    os << "Memory: " << memoryTypeStr() << " " << _info.ddr_freq <<" MHz, " 
      << memorySizeStrBinaryPrefix() << endl;
    os << "Board: "<< boardTypeStr() << " " << interfaceTypeStr() 
      << ", Rev " << static_cast<double>(info().bd_rev)/10.0 << endl;
    os << "FW v"<<fwVersionStr() << endl;
    return os;
}

string DxrtDeviceInfoWithStatus::getInfoString() const
{
    std::ostringstream os;
    infoToStream(os);
    return os.str();
}

std::ostream&DxrtDeviceInfoWithStatus::statusToStream(std::ostream& os) const
{
    os << std::showbase << std::dec;
    for(int i=0;i<static_cast<int>(info().num_dma_ch);i++)
    {
        os << npuStatusStr(i)<< endl;
    }
    os << dvfsStateInfoStr() << endl;
    return os;
}

string DxrtDeviceInfoWithStatus::getStatusString() const
{
    std::ostringstream os;
    statusToStream(os);
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const DxrtDeviceInfoWithStatus& d)
{
    d.infoToStream(os);
    os << endl;
    d.statusToStream(os);
    return os;
}

} // namespace dxrt

