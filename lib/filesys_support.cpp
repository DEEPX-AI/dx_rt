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
#endif __linux__

using namespace std;

#ifdef __linux__
    #define PATH_MAX    (400)
#elif _WIN32
    #define MAX_PATH    (400)
#endif __linux__

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
    if(path.length() < 1) return "";
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
    char* resolvedPath = realpath(path.c_str(), NULL);
    if (resolvedPath == NULL)
    {
        return "";
    }
    string absolutePath(resolvedPath);
    free(resolvedPath);
    return absolutePath;

#elif _WIN32
    // Check if path is already absolute (starts with '\\')
    if (path[0] == '\\') return path;

    // Buffer to store resolved path
    char resolvedPath[MAX_PATH];
    DWORD len = GetFullPathName(path.c_str(), MAX_PATH, resolvedPath, nullptr);
    if (len == 0) {
        std::cerr << "Error getting full path: " << GetLastError() << std::endl;
        return "";
    }
    string absolutePath(resolvedPath);
    free(resolvedPath);
    return absolutePath;
#endif
}

string dxrt::getParentPath(const string& path)
{
    size_t pos = path.find_last_of("/\\");
    if(pos == string::npos) 
    {
        return "";
    }
    return path.substr(0,pos);
}

int dxrt::getFileSize(const string& filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(),&stat_buf);
    if(rc!=0)
    {
        return -1;
    }
    return stat_buf.st_size;
}

bool dxrt::fileExists(const string& path)
{
    struct stat stat_buf;
    int rc = stat(path.c_str(),&stat_buf);
    if(rc!=0)
    {
        return false;
    }
    return true;
}


string dxrt::getExtension(const string& path)
{
    size_t pos = path.find_last_of(".");
    if(pos == string::npos) return "";
    return path.substr(pos+1);
}