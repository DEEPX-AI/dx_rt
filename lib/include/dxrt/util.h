/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include "dxrt/model.h"
#include "dxrt/tensor.h"
#include <string.h>
#include <cmath>
#ifdef __linux__
    #include <sys/time.h>
#elif _WIN32
    #include <windows.h>
#endif
#include <time.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>

#define data_align(x, a) ( a*( x/a) + (int)(((x%a)>0) ? a : 0) )

namespace dxrt {
DXRT_API std::vector<int> RandomSequence(int n);
DXRT_API unsigned int RandomValue();
DXRT_API std::vector<std::string> StringSplit(std::string s, std::string divid);
DXRT_API std::string format_number_with_commas(int64_t num);
// int GetDataSize(deepx_rmapinfo::DataType dType);
DXRT_API int DataFromFile(std::string f, void *d);
DXRT_API void DataFromFile(std::string f, void *d, unsigned int size);
DXRT_API void DataDumpBin(std::string filename, void *data, unsigned int size);
DXRT_API uint32_t SizeFromFile(std::string f);
DXRT_API std::vector<std::string> GetFileList(std::string dir);
DXRT_API uint64_t GetAlign(uint64_t size, int align);
DXRT_API uint64_t GetAlign(uint64_t size);
DXRT_API void* MemAlloc(size_t size, size_t align = 8, int value = 0);
DXRT_API void MemFree(void **p);
DXRT_API void DisplayCountdown(int seconds, std::string str);

template< typename T >
std::string int_to_hex(T i )
{
    std::stringstream stream;
    stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(T)*2)
         << std::hex << i;
    return stream.str();
}
template <typename T>
void DataDumpTxt(std::string filename, T *data, size_t ch, size_t row, size_t col, bool showHex = false)
{
    std::ofstream f_out(filename, std::ios::out);
    if (showHex)
    {
        f_out << std::hex << std::showbase;
    }
    for (size_t c = 0; c < ch; c++)
    {
        for (size_t h = 0; h < row; h++)
        {
            for (size_t w = 0; w < col; w++)
            {
                f_out << (T)*data << " ";
                data++;
            }
            f_out << std::endl;
        }
        f_out << std::endl;
    }
    f_out.close();
}

template <typename T>
std::vector<T> SelectElements(const std::vector<T>& org, const std::vector<int>& indices)
{
    if (indices.empty())
        return org;
    std::vector<T> ret;
    for (const auto &index : indices)
    {
        ret.emplace_back(org[index]);
    }
    return ret;
}

static inline void get_start_time(struct timeval *s)
{
#ifdef __linux__
    gettimeofday(s, NULL);
#elif _WIN32
    LARGE_INTEGER counter, freq;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&freq);
    s->tv_sec = counter.QuadPart / freq.QuadPart;
    s->tv_usec = static_cast<int64_t>((counter.QuadPart % freq.QuadPart) * 1000000 / freq.QuadPart);
#endif
}
static inline uint64_t get_elapsed_time(struct timeval s)
{
    struct timeval e;
    uint64_t elapsed;

    get_start_time(&e);

    e.tv_sec = e.tv_sec - s.tv_sec;
    if (e.tv_usec < s.tv_usec) {
        e.tv_sec--;
        e.tv_usec = (1000000 - s.tv_usec) + e.tv_usec;
    } else {
        e.tv_usec = e.tv_usec - s.tv_usec;
    }

    elapsed = (e.tv_sec * 1000000) + e.tv_usec;

    return elapsed;
}

// TODO: enum refactorying required
DXRT_API int GetDataSize_rmapinfo_datatype(deepx_rmapinfo::DataType dType);
DXRT_API int GetDataSize_Datatype(DataType dType);
}  // namespace dxrt
