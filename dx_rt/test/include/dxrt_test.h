#pragma once

#include "dxrt/common.h"

extern std::string testModelPath;
extern int testNum;

template<typename T>
void fillStructIncreasingValues(T& var) {
    uint8_t *ptr = reinterpret_cast<uint8_t*>(&var);
    for(int i=0; i<static_cast<int>(sizeof(var)); i++)
    {
        ptr[i] = i;
    }
}