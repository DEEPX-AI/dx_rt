#include <cstdio>
#include <errno.h>
#include <cstring>
#include <string>

std::string getErrorString(int error_code)
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    std::string error = "Error no " + std::to_string(error_code);
    char* str = strerror_r(error_code, buffer, sizeof(buffer));
    if (str != nullptr)
    {
        error += "(";
        error += std::string(str);
        error += ")";
    }
    else
    {
        error += "(strerror_r notfound "+ std::to_string(errno)+")";
    }
    return error;
}

std::string getString()
{
    return getErrorString(errno);
}
