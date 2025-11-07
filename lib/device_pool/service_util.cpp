/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/service_util.h"
#include <algorithm>
#include "dxrt/common.h"

namespace dxrt {

#ifdef __linux__
#include <dirent.h>
#include <unistd.h>
bool DXRT_API isDxrtServiceRunning() {
    const std::string& processName = "dxrtd";
    pid_t myPid = getpid();  // Get Current Process PID

    // /proc Directory Open
    DIR* procDir = opendir("/proc");
    if (!procDir) {
        perror("opendir");
        return false;
    }

    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        // Check if directory name is digit(process)
        std::string dirName(entry->d_name);
        if (!std::all_of(dirName.begin(), dirName.end(), ::isdigit)) {
            continue;
        }

        pid_t pid = std::stoi(dirName);
        if (pid == myPid) {
            continue;  // Exclude self
        }

        // /proc/<pid>/cmdline file open
        std::ifstream cmdlineFile("/proc/" + dirName + "/cmdline");
        if (!cmdlineFile.is_open()) {
            continue;
        }

        // read from cmdline
        std::string cmdline;
        std::getline(cmdlineFile, cmdline, '\0');  // read first things seperated by NULL
        cmdlineFile.close();

        // Check for process name
        if (cmdline.find(processName) != std::string::npos) {
            closedir(procDir);
            return true;
        }
    }

    closedir(procDir);
    return false;
}
#elif _WIN32
#include <windows.h>
#include <tlhelp32.h>
// Check for other process with has same name
bool DXRT_API isDxrtServiceRunning() {
    const std::string processName = "dxrtd.exe";
    DWORD myPid = GetCurrentProcessId();  // Get current process PID

    // Get process snapshots
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        LOG_DXRT_ERR("Failed to create process snapshot.");
        return false;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    // Get first process
    if (!Process32First(hSnapshot, &processEntry)) {
        LOG_DXRT_ERR("Failed to retrieve first process.");
        CloseHandle(hSnapshot);
        return false;
    }

    // iterate process list
    do {
        // If name is equal and pid is not equal
        if (processName == processEntry.szExeFile && myPid != processEntry.th32ProcessID) {
            CloseHandle(hSnapshot);
            return true;  // Other instance running
        }
    } while (Process32Next(hSnapshot, &processEntry));

    CloseHandle(hSnapshot);
    return false;  // No Other instance
}
#endif

}  // namespace dxrt
