/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/util.h"
#include "dxrt/datatype.h"
#include "dxrt/inference_engine.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#if __cplusplus > 201103L
#include <random>
#endif
#include <locale>
#include <thread>
#include <chrono>


using std::string;
using std::vector;
using std::cout;
using std::endl;

namespace dxrt
{
unsigned int RandomValue()
{
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_int_distribution<unsigned int> distr;
    return distr(eng);
}

vector<int> RandomSequence(int n)
{
    vector<int> v(n);
    for (int i = 0; i < n; i++)
        v[i] = i;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);

    cout << __func__ << " : ";
    for (int i = 0; i < n; i++)
        cout << std::dec << v[i] << " ";
    cout << endl;
    return v;
}

#ifdef __linux__
#define STRTOK_DEFINE strtok_r
#elif _WIN32
#define STRTOK_DEFINE strtok_s
#endif

vector<string> StringSplit(string s, string divid)
{
    vector<string> v;

    if (!s.empty())
    {
        char* temp = new char[s.length()+1];
        strncpy(temp, s.c_str(), s.length()-1);
        char* save_pointer = nullptr;
        char* c = STRTOK_DEFINE(temp, divid.c_str(), &save_pointer);
        while (c) {
            v.push_back(c);
            c = STRTOK_DEFINE(NULL, divid.c_str(), &save_pointer);
        }
        delete[] temp;
    }
    else
    {
        v.emplace_back("");
    }
    return v;
}

std::string format_number_with_commas(int64_t num) {
    std::ostringstream oss;
    try {
        oss.imbue(std::locale(""));
    } catch (const std::exception& e) {
        LOG_DXRT_DBG << e.what() << std::endl;
    }
    oss << num;
    return oss.str();
}

///////////////////// Data Compare Logic using npu param file information /////////////////////////
int GetDataSize_rmapinfo_datatype(deepx_rmapinfo::DataType dType)
{
    int size = 0;
    switch (dType) {
        case deepx_rmapinfo::DataType::UINT8  :
        case deepx_rmapinfo::DataType::INT8   :
            size = 1;
            break;
        case deepx_rmapinfo::DataType::INT16  :
        case deepx_rmapinfo::DataType::UINT16 :
            size = 2;
            break;
        case deepx_rmapinfo::DataType::UINT32 :
        case deepx_rmapinfo::DataType::INT32  :
        case deepx_rmapinfo::DataType::FLOAT32:
            size = 4;
            break;
        case deepx_rmapinfo::DataType::UINT64 :
        case deepx_rmapinfo::DataType::INT64  :
            size = 8;
            break;
        default:
            cout << "Unwanted Data Type is inserted in GetDataSize_rmapinfo_datatype." << dType << endl;
            exit(0);
    }
    return size;
}
int GetDataSize_Datatype(DataType dType)
{
    int size = 0;
    switch (dType) {
        case DataType::UINT8  :
        case DataType::INT8   :
            size = 1;
            break;
        case DataType::INT16  :
        case DataType::UINT16 :
            size = 2;
            break;
        case DataType::UINT32 :
        case DataType::INT32  :
        case DataType::FLOAT:
            size = 4;
            break;
        case DataType::UINT64 :
        case DataType::INT64  :
            size = 8;
            break;
        case DataType::BBOX :
            size = 32;
            break;
        case DataType::FACE :
            size = 64;
            break;
        case DataType::POSE :
            size = 256;
            break;
        default:
            cout << "Unwanted Data Type is inserted in GetDataSize_Datatype." << dType << endl;
            exit(0);
    }
    return size;
}

int DataFromFile(string f, void *d)
{
    LOG_DXRT_DBG << f << " -> " << d << endl;
    std::ifstream in(f, std::ifstream::binary);
    if (in)
    {
        in.clear();
        in.seekg(0, in.end);
        unsigned int size = static_cast<int>(in.tellg());
        in.seekg(0, in.beg);
        in.read(static_cast<char*>(d), size);
        in.close();
        return size;
    }
    return 0;
}
void DataFromFile(string f, void *d, unsigned int size)
{
    FILE *fp;
    fp = fopen(f.c_str(), "rb");
    std::ignore = fread(d, size, 1, fp);
    fclose(fp);
}
uint32_t SizeFromFile(string f)
{
    std::ifstream in(f, std::ifstream::binary);
    uint32_t size = 0;
    if (in)
    {
        in.seekg(0, in.end);
        size = static_cast<int>(in.tellg());
        in.close();
    }
    return size;
}
void DataDumpBin(string filename, void *data, unsigned int size)
{
    FILE *fp;
    fp = fopen(filename.c_str(), "wb");
    fwrite(data, size, 1, fp);
    fclose(fp);
}
vector<string> GetFileList(string dir)
{
    vector<string> v;
#ifdef __linux__
    DIR* p = opendir(dir.c_str());
    struct dirent* dp;
    while ((dp = readdir(p)) != NULL)
    {
        // cout << dp->d_name << "===" << endl;
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
            v.emplace_back(dp->d_name);
    }
    closedir(p);
#elif _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((dir + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (strcmp(findFileData.cFileName, ".") != 0 &&
                strcmp(findFileData.cFileName, "..") != 0)
            {
                v.emplace_back(findFileData.cFileName);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
#endif

    return v;
}

uint64_t GetAlign(uint64_t size)
{
    if (size < 64) {
        int remainder = size % 16;
        if (remainder != 0) {
            size += 16 - remainder;
        }
        return size;
    }
    else
    {
        int remainder = size % 64;
        if (remainder != 0) {
            size += 64 - remainder;
        }
        return size;
    }

}
uint64_t GetAlign(uint64_t size, int align)
{
    if (align <= 0) 
        return GetAlign(size);
    int remainder = size % align;
    if (remainder != 0) {
        size += align - remainder;
    }
    return size;
}

template<typename T>
int DataComparePpu(T* d1, T* d2, int size)
{
    int ret = -1;
    if (size < 32) return 0;
    for (int i = 0; i < (size / static_cast<int>(sizeof(T))); i++)
    {
        // cout << i << ", " << d1[i].score << ", " << d1[i].x << ", " << d1[i].y << ", " << d1[i].w << ", " << d1[i].h << endl;
        // cout << i << ", " << d2[i].score << ", " << d2[i].x << ", " << d2[i].y << ", " << d2[i].w << ", " << d2[i].h << endl;
        if (memcmp(&d1[i], &d2[i], sizeof(T)) != 0)
        {
            if (i == 0)
            {
                return -1;
            }
            else
            {
                return i*sizeof(T);
            }
        }
        else
        {
            ret = 0;
        }
    }
    return ret;
}
int(*compareBoundingBox)(DeviceBoundingBox_t*, DeviceBoundingBox_t*, int) = DataComparePpu<DeviceBoundingBox_t>;
int(*compareFace)(DeviceFace_t*, DeviceFace_t*, int) = DataComparePpu<DeviceFace_t>;
int(*comparePose)(DevicePose_t*, DevicePose_t*, int) = DataComparePpu<DevicePose_t>;
int(*comparePpuDataFunctions[])(void*, void*, int)  = {
    reinterpret_cast<int(*)(void*, void*, int)>(compareBoundingBox),
    reinterpret_cast<int(*)(void*, void*, int)>(compareFace),
    reinterpret_cast<int(*)(void*, void*, int)>(comparePose),
};

void* MemAlloc(size_t size, size_t align, int value)
{
    void *mem = NULL;
#ifdef __linux__
    int rc = posix_memalign(reinterpret_cast<void**>(&mem), align, size);
    if (rc == EINVAL)
    {
        cout << "Error: posix_memalign returned EINVAL." << endl;
        return mem;
    }
    else if (rc == ENOMEM)
    {
        cout << "Error: posix_memalign returned ENOMEM." << endl;
        return mem;
    }
#elif _WIN32
    mem = _aligned_malloc(size, align);
    if (!mem)
    {
        if (errno == EINVAL)
        {
            std::cout << "Error: _aligned_malloc returned EINVAL." << std::endl;
        }
        else
        {
            std::cout << "Error: _aligned_malloc failed to allocate memory." << std::endl;
        }
        return mem;
    }
#endif

    if (value != 0)
    {
        LOG_DBG("Default value of allocation memory:" << value);
    }
    if (mem == nullptr)
    {
        LOG_DBG("failed to allocate memory: " << size << ", " << align);
    }
    else
    {
        memset(mem, value, size);
    }
    return mem;
}
void MemFree(void **p)
{
    if (*p != nullptr)
    {
#ifdef __linux__
        free(*p);
#elif _WIN32
        _aligned_free(p);
#endif
        * p = nullptr;
    }
}

std::ostream& operator<<(std::ostream& os, const Processor& processor)
{
    switch (processor)
    {
        case NPU:
            os << "NPU";
            break;
        case CPU:
            os << "CPU";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}

void DisplayCountdown(int seconds, string str)
{
    cout.sync_with_stdio(false);
    while (seconds > 0) {
        cout << "\r" << str << "(" << seconds << " seconds remaining) " << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
//#ifdef __linux__
//        sleep(1);
//#elif _WIN32
//        Sleep(1);
//#endif
        --seconds;
    }
    cout << endl;
}

} // namespace dxrt