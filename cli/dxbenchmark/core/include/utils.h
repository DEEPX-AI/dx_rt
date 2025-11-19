#pragma once
#include <string>
#include <vector>

#include "../include/runner.h"

using std::vector;
using std::string;


struct HostInform{
    string arch;
    string coreModel;
    string numCore;
    string memSize;
    string os;
};

enum SORT
{
    NAME,
    FPS,
    INFTIME,
    LATENCY
};

void getHostInform(HostInform& inform);
void printCpuInfo();
void printArchitectureInfo();
void printMemoryInfo();
string float_to_string_fixed(float value, int precision);
void _getModelLinux(const string& dirPath, vector<std::pair<string, string>>& fileList, bool recursive);
vector<std::pair<string, string>> getModelLinux(const string& startDir, bool recursive);
void _getModelWindows(const string& dirPath, vector<std::pair<string, string>>& fileList, bool recursive);
vector<std::pair<string, string>> getModelWindows(const string& startDir, bool recursive);
string getCurrentTime();
void sortModels(vector<Result>& results, string& criteria, string& order);
bool findDuplicates(vector<std::pair<string, string>>& fileList);