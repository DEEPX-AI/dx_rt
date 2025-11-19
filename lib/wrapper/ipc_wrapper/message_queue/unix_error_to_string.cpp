#include <cstdio>
#include <errno.h>
#include <cstring>
#include <string>

#if __linux__
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
#else // _WIN32
#include <windows.h>
std::string getErrorString(int error_code)
{
    LPVOID msgBuffer;
    std::string error = "Error no " + std::to_string(error_code);

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&msgBuffer,
        0, NULL);

    if (msgBuffer != NULL)
    {
        error += " (";
        error += std::string(static_cast<char*>(msgBuffer));
        error += ")";
        LocalFree(msgBuffer);
    }
    else
    {
        error += "(FormatMessage failed "+ std::to_string(GetLastError())+")";
    }
    return error;
}
#endif

std::string getString()
{
    return getErrorString(errno);
}
