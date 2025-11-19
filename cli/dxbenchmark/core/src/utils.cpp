#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <memory>
#include <chrono> 
#include <algorithm>
#include <utility> 
#include <ctime>      
#include <iomanip>
#include <set>

#ifdef __linux__
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <dirent.h> 
#include <sys/stat.h> 
#elif _WIN32
#include <windows.h>
#include <sstream>
#endif

#include "dxrt/dxrt_api.h"
#include "dxrt/extern/cxxopts.hpp"
#include "dxrt/filesys_support.h"
#include "dxrt/profiler.h"

#include "../include/utils.h"
#include "../include/runner.h"

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::string;

#ifdef __linux__

void getHostInform(HostInform& inform)
{
    std::ifstream cpuinfo("/proc/cpuinfo");
    string line;
    bool modelNameFound = false;
    bool cpuCoresFound = false;
    std::stringstream ss;

    if (cpuinfo.is_open()) 
    {
        while (getline(cpuinfo, line)) 
        {
            // Core model
            if (!modelNameFound && line.find("model name") != string::npos) 
            {
                inform.coreModel = line.substr(line.find(":") + 2);
                modelNameFound = true;
            }
            // number of CPU cores
            if (!cpuCoresFound && line.find("cpu cores") != string::npos) 
            {
                inform.numCore = line.substr(line.find(":") + 2);
                cpuCoresFound = true;
            }

            if (modelNameFound && cpuCoresFound) 
            {
                break;
            }
        }
        cpuinfo.close();
    }
    else 
    {
        inform.coreModel = "Undefined Model";
        inform.numCore = "Undefined Number";
    }

    struct utsname buffer;

    if (uname(&buffer) == 0) 
    {
        inform.arch = buffer.machine;
    }
    else 
    {
        inform.arch = "Undefined Architecture";
    }

    std::ifstream osFile("/etc/os-release");
    if (!osFile.is_open()) 
    {
        inform.os = "Undefined Operating System";
    }

    while (getline(osFile, line)) 
    {
        std::string key = "PRETTY_NAME=";
        if (line.rfind(key, 0) == 0) 
        { 
            std::string value = line.substr(key.length());
            
            if (value.length() >= 2 && value.front() == '"' && value.back() == '"') 
            {
                value = value.substr(1, value.length() - 2);
            }
            inform.os = value;
        }
    }

    struct sysinfo memInfo;

    if (sysinfo(&memInfo) == 0) 
    {
        long long totalPhysMem = static_cast<long long>(memInfo.totalram) * memInfo.mem_unit;
        ss << static_cast<double>(totalPhysMem) / (1024 * 1024 * 1024) << " GB";
        inform.memSize = ss.str();

    }
    else 
    {
        inform.memSize = "Undefined Memory Size";
    }
}

void printCpuInfo() 
{
    cout << "--- CPU Information ---" << endl;
    std::ifstream cpuinfo("/proc/cpuinfo");
    string line;
    bool modelNameFound = false;
    bool cpuCoresFound = false;
    bool vendorIdFound = false;

    if (cpuinfo.is_open()) 
    {
        while (getline(cpuinfo, line)) 
        {
            // model name
            if (!modelNameFound && line.find("model name") != string::npos) 
            {
                cout << "  Model Name: " << line.substr(line.find(":") + 2) << endl;
                modelNameFound = true;
            }
            // number of CPU cores
            if (!cpuCoresFound && line.find("cpu cores") != string::npos) 
            {
                cout << "  CPU Cores: " << line.substr(line.find(":") + 2) << endl;
                cpuCoresFound = true;
            }
            // vendor ID
            if (!vendorIdFound && line.find("vendor_id") != string::npos) 
            {
                cout << "  Vendor ID: " << line.substr(line.find(":") + 2) << endl;
                vendorIdFound = true;
            }

            if (modelNameFound && cpuCoresFound && vendorIdFound) 
            {
                break;
            }
        }
        cpuinfo.close();
    } 
    else 
    {
        // std::cerr << "Error: Could not open /proc/cpuinfo" << std::endl;
        std::cerr << "... No CPU Info." << endl;
    }
}

void printArchitectureInfo() 
{
    cout << "\n--- Architecture Information ---" << endl;
    struct utsname buffer;

    if (uname(&buffer) == 0) 
    {
        cout << "  System Name: " << buffer.sysname << endl;
        cout << "  Node Name:   " << buffer.nodename << endl;
        cout << "  Release:     " << buffer.release << endl;
        cout << "  Version:     " << buffer.version << endl;
        cout << "  Machine:     " << buffer.machine << endl;  // architecture information
        // perror("uname"); // error message if it fail
        // std::cerr << "Error: Could not get system architecture info." << std::endl;
    } 
    else 
    {
        std::cerr << "No System Architecture Info." << endl;
    }
}

void printMemoryInfo() 
{
    cout << "\n--- Memory Information ---" << endl;
    struct sysinfo memInfo;

    if (sysinfo(&memInfo) == 0) 
    {
        // total physical memory (bytes)
        long long totalPhysMem = static_cast<long long>(memInfo.totalram) * memInfo.mem_unit;
        // availabe physical memory (bytes)
        long long availPhysMem = static_cast<long long>(memInfo.freeram) * memInfo.mem_unit;
        // total swap space (bytes)
        long long totalSwap = static_cast<long long>(memInfo.totalswap) * memInfo.mem_unit;
        // available swap space (bytes)
        long long freeSwap = static_cast<long long>(memInfo.freeswap) * memInfo.mem_unit;

        // byte --> GB
        cout << std::fixed << std::setprecision(2);

        cout << "  Total Physical Memory: " << static_cast<double>(totalPhysMem) / (1024 * 1024 * 1024) << " GB" << endl;
        cout << "  Available Physical Memory: " << static_cast<double>(availPhysMem) / (1024 * 1024 * 1024) << " GB" << endl;
        cout << "  Total Swap Space: " << static_cast<double>(totalSwap) / (1024 * 1024 * 1024) << " GB" << endl;
        cout << "  Free Swap Space: " << static_cast<double>(freeSwap) / (1024 * 1024 * 1024) << " GB" << endl;
        cout << endl;

    } 
    else 
    {
        //perror("sysinfo"); 
        //std::cerr << "Error: Could not get system memory info." << std::endl;
        std::cerr << "No System Memory Info." << endl;
    }
}

void _getModelLinux(const string& dirPath, vector<std::pair<string, string>>& fileList, bool recursive) 
{
    DIR *dir;
    struct dirent *ent;
    const string extension = ".dxnn";

    if ((dir = opendir(dirPath.c_str())) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) 
        {
            std::string entryName = ent->d_name;

            if (entryName == "." || entryName == "..") 
            {
                continue;
            }

            std::string fullPath = (dirPath.back() == '/') ? (dirPath + entryName) : (dirPath + "/" + entryName);

            struct stat statBuf;
            if (stat(fullPath.c_str(), &statBuf) != 0) 
            {
                perror(("Could not stat file: " + fullPath).c_str());
                continue;
            }

            if (S_ISDIR(statBuf.st_mode)) 
            {
                if (recursive)
                {
                    _getModelLinux(fullPath, fileList, recursive);
                }
            }
            else if (S_ISREG(statBuf.st_mode)) 
            {
                if (entryName.length() >= extension.length() &&
                    entryName.compare(entryName.length() - extension.length(), extension.length(), extension) == 0) 
                {
                    fileList.push_back(std::make_pair(entryName, fullPath));
                }
            }
        }
        closedir(dir);
    } 
    else 
    {
        perror(("Could not open directory: " + dirPath).c_str());
    }
}

vector<std::pair<string, string>> getModelLinux(const string& startDir, bool recursive) 
{
    vector<std::pair<string, string>> fileList;
    _getModelLinux(startDir, fileList, recursive);
    return fileList;
}

#elif _WIN32

void getHostInform(HostInform& inform)
{
    // Get CPU information from registry
    HKEY hKey;
    DWORD bufferSize = 256;
    char buffer[256];
    
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
                      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, 
                            (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
            inform.coreModel = std::string(buffer);
        } else {
            inform.coreModel = "Undefined Model";
        }
        RegCloseKey(hKey);
    } else {
        inform.coreModel = "Undefined Model";
    }

    // Get number of CPU cores
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    inform.numCore = std::to_string(sysInfo.dwNumberOfProcessors);

    // Get architecture
    if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        inform.arch = "x86_64";
    } else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64) {
        inform.arch = "ARM64";
    } else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
        inform.arch = "x86";
    } else {
        inform.arch = "Unknown";
    }

    // Get OS version
    OSVERSIONINFOEXA osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
    
    // Note: GetVersionEx is deprecated but works for basic info
    #pragma warning(push)
    #pragma warning(disable: 4996)
    if (GetVersionExA((LPOSVERSIONINFOA)&osvi)) {
        std::stringstream ss;
        ss << "Windows " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
        inform.os = ss.str();
    } else {
        inform.os = "Windows";
    }
    #pragma warning(pop)

    // Get memory size
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        double totalGB = static_cast<double>(memInfo.ullTotalPhys) / (1024.0 * 1024.0 * 1024.0);
        std::stringstream ss;
        ss << totalGB << " GB";
        inform.memSize = ss.str();
    } else {
        inform.memSize = "Undefined Memory Size";
    }
}

void printCpuInfo() 
{
    cout << "--- CPU Information ---" << endl;
    
    HKEY hKey;
    DWORD bufferSize = 256;
    char buffer[256];
    
    // Processor name
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
                      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, 
                            (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
            cout << "  Model Name: " << buffer << endl;
        }
        
        // Vendor
        bufferSize = 256;
        if (RegQueryValueExA(hKey, "VendorIdentifier", NULL, NULL, 
                            (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
            cout << "  Vendor ID: " << buffer << endl;
        }
        
        RegCloseKey(hKey);
    }
    
    // CPU cores
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    cout << "  CPU Cores: " << sysInfo.dwNumberOfProcessors << endl;
}

void printArchitectureInfo() 
{
    cout << "\n--- Architecture Information ---" << endl;
    
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    
    cout << "  System Name: Windows" << endl;
    
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    if (GetComputerNameA(computerName, &size)) {
        cout << "  Node Name:   " << computerName << endl;
    }
    
    OSVERSIONINFOEXA osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
    
    #pragma warning(push)
    #pragma warning(disable: 4996)
    if (GetVersionExA((LPOSVERSIONINFOA)&osvi)) {
        cout << "  Release:     " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << endl;
        cout << "  Version:     Build " << osvi.dwBuildNumber << endl;
    }
    #pragma warning(pop)
    
    const char* arch = "Unknown";
    if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        arch = "x86_64";
    } else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64) {
        arch = "ARM64";
    } else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
        arch = "x86";
    }
    cout << "  Machine:     " << arch << endl;
}

void printMemoryInfo() 
{
    cout << "\n--- Memory Information ---" << endl;
    
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    
    if (GlobalMemoryStatusEx(&memInfo)) {
        double totalPhysMem = static_cast<double>(memInfo.ullTotalPhys) / (1024.0 * 1024.0 * 1024.0);
        double availPhysMem = static_cast<double>(memInfo.ullAvailPhys) / (1024.0 * 1024.0 * 1024.0);
        double totalPageFile = static_cast<double>(memInfo.ullTotalPageFile) / (1024.0 * 1024.0 * 1024.0);
        double availPageFile = static_cast<double>(memInfo.ullAvailPageFile) / (1024.0 * 1024.0 * 1024.0);
        
        cout << std::fixed << std::setprecision(2);
        cout << "  Total Physical Memory: " << totalPhysMem << " GB" << endl;
        cout << "  Available Physical Memory: " << availPhysMem << " GB" << endl;
        cout << "  Total Page File: " << totalPageFile << " GB" << endl;
        cout << "  Available Page File: " << availPageFile << " GB" << endl;
        cout << endl;
    } else {
        std::cerr << "No System Memory Info." << endl;
    }
}

void _getModelWindows(const string& dirPath, vector<std::pair<string, string>>& fileList, bool recursive) 
{
    const string extension = ".dxnn";
    std::string search_path = dirPath;
    
    // Ensure path ends with backslash
    if (!search_path.empty() && search_path.back() != '\\' && search_path.back() != '/') {
        search_path += "\\";
    }
    search_path += "*";
    
    WIN32_FIND_DATAA find_data;
    HANDLE h_find = FindFirstFileA(search_path.c_str(), &find_data);

    if (h_find == INVALID_HANDLE_VALUE) {
        std::cerr << "Could not open directory: " << dirPath << std::endl;
        return;
    }

    do {
        std::string entryName = find_data.cFileName;
        
        // Skip . and ..
        if (entryName == "." || entryName == "..") {
            continue;
        }
        
        std::string fullPath = dirPath;
        if (!fullPath.empty() && fullPath.back() != '\\' && fullPath.back() != '/') {
            fullPath += "\\";
        }
        fullPath += entryName;
        
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Recursively search subdirectories if enabled
            if (recursive) {
                _getModelWindows(fullPath, fileList, recursive);
            }
        } else {
            // Check if file has .dxnn extension
            if (entryName.length() >= extension.length() &&
                entryName.compare(entryName.length() - extension.length(), 
                                 extension.length(), extension) == 0) {
                fileList.push_back(std::make_pair(entryName, fullPath));
            }
        }
    } while (FindNextFileA(h_find, &find_data) != 0);
    
    FindClose(h_find);
}

vector<std::pair<string, string>> getModelWindows(const string& startDir, bool recursive) 
{
    vector<std::pair<string, string>> fileList;
    _getModelWindows(startDir, fileList, recursive);
    return fileList;
}

#endif

string float_to_string_fixed(float value, int precision) 
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();

    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm = *std::localtime(&now_c);

    std::stringstream ss;
    ss << std::put_time(&local_tm, "%Y_%m_%d_%H%M%S");
    
    std::string formatted_time = ss.str();

    return formatted_time;
}


void sortModels(vector<Result>& results, string& criteria, string& order)
{
    SORT c;

    if(criteria == "name") c = NAME;
    else if (criteria == "fps") c = FPS;
    else if (criteria == "time") c = INFTIME;
    else if (criteria == "latency") c = LATENCY;
    else c = NAME;
    
    std::sort(results.begin(), results.end(), [c, order](const Result& a, const Result& b) 
    {
        if (order == "desc")
        {
            switch (c)
            {
                case NAME:    return a.modelName.first > b.modelName.first;
                case FPS:     return a.fps > b.fps;
                case INFTIME: return a.infTime.mean > b.infTime.mean;
                case LATENCY: return a.latency.mean > b.latency.mean;
                default:    return a.modelName.first > b.modelName.first;
            }
        }

        else
        {
            switch (c) 
            {
                case NAME:    return a.modelName.first < b.modelName.first;
                case FPS:     return a.fps < b.fps;
                case INFTIME: return a.infTime.mean < b.infTime.mean;
                case LATENCY: return a.latency.mean < b.latency.mean;
                default: return a.modelName.first < b.modelName.first;
            }
        }
        return false;
    });

}

bool findDuplicates(vector<std::pair<string, string>>& fileList)
{
    std::map<string, vector<string>> nameTracker;

    for (const auto& filePair : fileList) {
        nameTracker[filePair.first].push_back(filePair.second);
    }

    std::set<string> duplicateNames;
    bool hasDuplicates = false;

    for (const auto& entry : nameTracker) {
        if (entry.second.size() > 1) {
            hasDuplicates = true;
            duplicateNames.insert(entry.first);
        }
    }

    if (!hasDuplicates) {
        return false;
    }

    for (auto& filePair : fileList) {
        if (duplicateNames.count(filePair.first)) {
            filePair.first = filePair.second;
        }
    }

    return true;
}