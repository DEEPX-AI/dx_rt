#include "dxrt/filesys_support.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

#define PATH_MAX    (400)
string dxrt::getCurrentPath()
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        // std::cout << "Current working directory: " << buffer << std::endl;
    } else {
        cout <<"getcwd() error" << endl;
        return "";
    }
    return string(buffer);
}

string dxrt::getAbsolutePath(const string& path)
{
    if(path.length() < 1)return "";
    if(path[0]=='\\')return path;
    char* resolvedPath = realpath(path.c_str(),NULL);
    if (resolvedPath == NULL)
    {
        return "";
    }
    string absolutePath(resolvedPath);
    free(resolvedPath);
    return absolutePath;
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