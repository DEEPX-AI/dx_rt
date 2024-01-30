// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include "dxrt/model.h"
#include <string.h>
#include <cmath>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>

#define data_align(x, a) ( a*( x/a) + (int)(((x%a)>0) ? a : 0) )

namespace dxrt {
std::vector<int> RandomSequence(int n);
unsigned int RandomValue();
std::vector<std::string> StringSplit(std::string s, std::string divid);    
int DataCompare(DataType type, void *d1, void *d2, int size); /* 0 : data same, -1 : different at offset zero, n(>0) : different at offset n */
int DataCompare(void *d1, void *d2, int size); /* 0 : data same, -1 : different at offset zero, n(>0) : different at offset n */
int DataCompare(std::string f1, std::string f2);
int DataCompare(std::string f1, std::string f2, std::string path, std::string *logBuf=nullptr, int log_en=0, int type=1);
int DataCompare(std::string f1, std::string f2, deepx_rmapinfo::RegisterInfoDatabase Cfg, std::string *logBuf=nullptr, int log_en=0, int type=0);
int DataCompare(char *f1, char *f2, deepx_rmapinfo::RegisterInfoDatabase Cfg, std::string *logBuf=nullptr, int log_en=0, int type=0);
int GetDataSize(deepx_rmapinfo::DataType dType);
int DataFromFile(std::string f, void *d);
void DataFromFile(std::string f, void *d, unsigned int size);
void DataDumpBin(std::string filename, void *data, unsigned int size);
uint32_t SizeFromFile(std::string f);
std::vector<std::string> GetFileList(std::string dir);
uint64_t GetAlign64(uint64_t size);
void* MemAlloc(size_t size, size_t align=8, int value=0);
void MemFree(void **p);

template <typename T>
void DataDumpTxt(std::string filename, T *data, size_t ch, size_t row, size_t col, bool showHex=false)
{	
	std::ofstream f_out(filename, std::ios::out);
    if(showHex)
    {
        f_out << std::hex << std::showbase;
    }
    for(size_t c=0; c<ch;c++)
    {
        for(size_t h=0; h<row;h++)
        {
            for(size_t w=0; w<col;w++)
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

static inline void get_start_time(struct timeval *s)
{
	gettimeofday(s, NULL);
}
static inline uint64_t get_elapsed_time(struct timeval s)
{
	struct timeval e;
	uint64_t elapsed;

	gettimeofday(&e, NULL);

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

} // namespace dxrt
