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
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        // std::cout << "Current working directory: " << buffer << std::endl;
        return string(buffer);
    } else {
        cout <<"getcwd() error" << endl;
        return "";
    }
#elif _WIN32
    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);
    return std::string(buffer);
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
        char cwd[1024];
#ifdef __linux__
        if (getcwd(cwd, sizeof(cwd)) != nullptr)
            return string(cwd) + '/' + path;
        else
            return path;
#elif _WIN32
        if (GetCurrentDirectory(sizeof(cwd), cwd) != 0)
            return string(cwd) + '/' + path;
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
    char path_buffer [PATH_MAX];
    char* resolvedPath = realpath(path.c_str(), path_buffer);
    if (resolvedPath == NULL)
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

int dxrt::getFileSize(const string& filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    if (rc != 0)
    {
        return -1;
    }
    return stat_buf.st_size;
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