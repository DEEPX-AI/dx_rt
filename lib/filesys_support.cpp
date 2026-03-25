/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/filesys_support.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <cstring>
#include <array>
#ifdef __linux__
    #include <sys/stat.h>
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#endif  // __linux__

using std::cout;
using std::endl;
using std::string;



string dxrt::getCurrentPath()
{
#ifdef __linux__
    std::array<char, PATH_MAX> buffer;
    if (getcwd(buffer.data(), buffer.size()) != nullptr)
    {
        return std::string(buffer.data());
    } else {
        cout <<"getcwd() error" << endl;
        return "";
    }
#elif _WIN32
    std::array<TCHAR, MAX_PATH> buffer;
    GetCurrentDirectory(MAX_PATH, buffer.data());
    return std::string(buffer.data());
#endif
}

string dxrt::getPath(const string& path)
{
    if (path.length() < 1) return "";
#ifdef _WIN32
    return getAbsolutePath(path);
#endif
    if (path[0] == '/')
        return path;
    else if (path.substr(0, 2) == "./" || path.substr(0, 3) == "../")
    {
        std::array<char, 1024> cwd;
#ifdef __linux__
        if (getcwd(cwd.data(), cwd.size()) != nullptr)
            return string(cwd.data()) + '/' + path;
        else
            return path;
#elif _WIN32
        if (GetCurrentDirectory(sizeof(cwd), cwd.data()) != 0)
            return string(cwd.data()) + '/' + path;
        else
            return path;
#endif
    }
    return path;
}

string dxrt::getAbsolutePath(const string& path)
{
    if (path.length() < 1)return "";
#ifdef __linux__
    if (path[0] == '\\')return path;
    std::array<char, PATH_MAX> path_buffer;
    const char* resolvedPath = realpath(path.c_str(), path_buffer.data());
    if (resolvedPath == nullptr)
    {
        return "";
    }
    string absolutePath(resolvedPath);

    return absolutePath;

#elif _WIN32
    char* resolvedPath = _fullpath(NULL, path.c_str(), _MAX_PATH);
    if (resolvedPath == nullptr)
    {
        return "";
    }
    string absolutePath(resolvedPath);
    free(resolvedPath);
    return absolutePath;
#endif
}

string dxrt::getParentPath(const string& path)
{
#ifdef __linux__
    size_t pos = path.find_last_of("/\\");
    if (pos == string::npos)
    {
        return "";
    }
    return path.substr(0, pos);
#elif _WIN32
    size_t pos = path.find_last_of("\\");
    if (pos == string::npos)
    {
        return "";
    }
    return path.substr(0, pos);
#endif
}

uint64_t dxrt::getFileSize(const string& filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    if (rc != 0)
    {
        return -1;
    }
    return static_cast<uint64_t>(stat_buf.st_size);
}

bool dxrt::fileExists(const string& path)
{
#ifdef __linux__
    struct stat stat_buf;
    int rc = stat(path.c_str(), &stat_buf);
    if (rc != 0)
    {
        return false;
    }
#elif _WIN32
    HANDLE handle = CreateFile(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (handle == INVALID_HANDLE_VALUE) {
        return false;
    }
    CloseHandle(handle);
#endif
    return true;
}


string dxrt::getExtension(const string& path)
{
    size_t pos = path.find_last_of(".");
    if (pos == string::npos) return "";
    return path.substr(pos+1);
}
