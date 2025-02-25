// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.


#include "dxrt/common.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
int dxrt_service_main();



#ifdef __linux__
#include <dirent.h>
#include <unistd.h>
bool hasOtherProcess() {
    const std::string& processName = "dxrtd";
    pid_t myPid = getpid(); // 현재 프로세스의 PID 가져오기

    // /proc 디렉토리 열기
    DIR* procDir = opendir("/proc");
    if (!procDir) {
        perror("opendir");
        return false;
    }

    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        // 디렉토리 이름이 숫자인지 확인 (프로세스 디렉토리)
        std::string dirName(entry->d_name);
        if (!std::all_of(dirName.begin(), dirName.end(), ::isdigit)) {
            continue;
        }

        pid_t pid = std::stoi(dirName);
        if (pid == myPid) {
            continue; // 자신의 프로세스는 제외
        }

        // /proc/<pid>/cmdline 파일 열기
        std::ifstream cmdlineFile("/proc/" + dirName + "/cmdline");
        if (!cmdlineFile.is_open()) {
            continue;
        }

        // cmdline 파일에서 명령어 이름 읽기
        std::string cmdline;
        std::getline(cmdlineFile, cmdline, '\0'); // NULL 문자로 구분된 첫 번째 항목 읽기
        cmdlineFile.close();

        // 프로세스 이름이 포함되어 있는지 확인
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
// 특정 이름의 다른 프로세스가 있는지 확인하는 함수
bool hasOtherProcess() {
    const std::string processName = "dxrtd.exe";
    DWORD myPid = GetCurrentProcessId(); // 현재 프로세스의 PID 가져오기

    // 프로세스 스냅샷 생성
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot.\n";
        return false;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    // 첫 번째 프로세스 가져오기
    if (!Process32First(hSnapshot, &processEntry)) {
        std::cerr << "Failed to retrieve first process.\n";
        CloseHandle(hSnapshot);
        return false;
    }

    // 프로세스 목록 순회
    do {
        // 현재 프로세스의 이름이 일치하고 PID가 다른 경우
        if (processName == processEntry.szExeFile && myPid != processEntry.th32ProcessID) {
            CloseHandle(hSnapshot);
            return true; // 다른 프로세스가 실행 중
        }
    } while (Process32Next(hSnapshot, &processEntry));

    CloseHandle(hSnapshot);
    return false; // 동일한 이름의 다른 프로세스가 없음
}
#endif

int main(int argc, char* argv[])
{
    std::ignore = argc;
    std::ignore = argv;
    if (hasOtherProcess())
    {
        std::cout << "Other instance of dxrtd is running" << std::endl;
        return -1;
    }
#ifdef USE_SERVICE
    return dxrt_service_main();
#else
    std::cout << "USE_SERVICE is not set, so dxrt_service will not work" << std::endl;
    return -1;
#endif
}