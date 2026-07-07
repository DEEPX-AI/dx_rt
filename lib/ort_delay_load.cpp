/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * Delay-load hook for onnxruntime.dll
 *
 * When dxrt.dll is built with /DELAYLOAD:onnxruntime.dll, the OS does not
 * resolve the DLL at load time.  Instead, the first call into any ORT API
 * triggers the delay-load helper, which invokes this hook.
 *
 * The hook intercepts the "pre-load-library" notification and explicitly
 * loads onnxruntime.dll from the same directory as dxrt.dll.  This prevents
 * a stale or incompatible copy in C:\Windows\System32 (or elsewhere on PATH)
 * from being picked up first.
 *
 * LOAD_WITH_ALTERED_SEARCH_PATH ensures that onnxruntime.dll's own
 * dependencies (e.g. onnxruntime_providers_shared.dll) are also resolved
 * from that same directory.
 */

#include <dxrt/gen.h>

#if defined(_WIN32) && defined(USE_ORT)

#include <windows.h>
#include <delayimp.h>

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cwchar>

namespace
{

const char* DelayNotifyToString(unsigned dliNotify)
{
    switch (dliNotify)
    {
    case dliStartProcessing:
        return "dliStartProcessing";
    case dliNotePreLoadLibrary:
        return "dliNotePreLoadLibrary";
    case dliNotePreGetProcAddress:
        return "dliNotePreGetProcAddress";
    case dliFailLoadLib:
        return "dliFailLoadLib";
    case dliFailGetProc:
        return "dliFailGetProc";
    case dliNoteEndProcessing:
        return "dliNoteEndProcessing";
    default:
        return "unknown";
    }
}

void WideToUtf8(const wchar_t* src, char* dst, size_t dstSize)
{
    if (!dst || dstSize == 0)
        return;

    dst[0] = '\0';

    if (!src)
    {
        strcpy_s(dst, dstSize, "(null)");
        return;
    }

    int rc = WideCharToMultiByte(
        CP_UTF8,
        0,
        src,
        -1,
        dst,
        static_cast<int>(dstSize),
        nullptr,
        nullptr);

    if (rc == 0)
        strcpy_s(dst, dstSize, "(utf8-convert-failed)");
}

bool IsDelayLoadLoggingEnabled()
{
    char value[8];
    DWORD len = GetEnvironmentVariableA("DXRT_ORT_DELAY_LOG", value, static_cast<DWORD>(_countof(value)));
    return len > 0 && len < _countof(value) && strcmp(value, "0") != 0;
}

void DxrtOrtDelayLog(const char* fmt, ...)
{
    static const bool enabled = IsDelayLoadLoggingEnabled();
    if (!enabled)
        return;

    char payload[2048];
    va_list args;
    va_start(args, fmt);
    vsnprintf_s(payload, _countof(payload), _TRUNCATE, fmt, args);
    va_end(args);

    SYSTEMTIME st;
    GetLocalTime(&st);

    char line[2600];
    _snprintf_s(
        line,
        _countof(line),
        _TRUNCATE,
        "[dxrt][ort-delay][%04u-%02u-%02u %02u:%02u:%02u.%03u][pid:%lu tid:%lu] %s\r\n",
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond,
        st.wMilliseconds,
        static_cast<unsigned long>(GetCurrentProcessId()),
        static_cast<unsigned long>(GetCurrentThreadId()),
        payload);

    OutputDebugStringA(line);

    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    if (hStdErr != nullptr && hStdErr != INVALID_HANDLE_VALUE)
    {
        DWORD written = 0;
        WriteFile(hStdErr, line, static_cast<DWORD>(strlen(line)), &written, nullptr);
    }

    wchar_t tempPath[MAX_PATH];
    DWORD tempLen = GetTempPathW(MAX_PATH, tempPath);
    if (tempLen == 0 || tempLen >= MAX_PATH)
        return;

    const wchar_t logName[] = L"dxrt_ort_delay_load.log";
    if (wcslen(tempPath) + _countof(logName) >= MAX_PATH)
        return;

    wcscat_s(tempPath, logName);

    FILE* fp = nullptr;
    if (_wfopen_s(&fp, tempPath, L"a") == 0 && fp != nullptr)
    {
        fputs(line, fp);
        fclose(fp);
    }
}

void LogLastError(const char* context)
{
    DWORD errorCode = GetLastError();
    char message[512] = {0};

    DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    DWORD len = FormatMessageA(
        flags,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        message,
        static_cast<DWORD>(_countof(message)),
        nullptr);

    if (len > 0)
        DxrtOrtDelayLog("%s failed. GetLastError=%lu (%s)", context, static_cast<unsigned long>(errorCode), message);
    else
        DxrtOrtDelayLog("%s failed. GetLastError=%lu", context, static_cast<unsigned long>(errorCode));
}

void LogModulePathFromHandle(HMODULE moduleHandle, const char* label)
{
    if (!moduleHandle)
    {
        DxrtOrtDelayLog("%s: handle is null", label);
        return;
    }

    wchar_t modulePathW[MAX_PATH];
    DWORD len = GetModuleFileNameW(moduleHandle, modulePathW, MAX_PATH);
    if (len == 0 || len >= MAX_PATH)
    {
        DxrtOrtDelayLog("%s: failed to resolve module path from handle=%p", label, moduleHandle);
        LogLastError("GetModuleFileNameW");
        return;
    }

    char modulePathUtf8[MAX_PATH * 3];
    WideToUtf8(modulePathW, modulePathUtf8, _countof(modulePathUtf8));
    DxrtOrtDelayLog("%s: handle=%p, path=%s", label, moduleHandle, modulePathUtf8);
}

void LogModulePathIfLoaded(const wchar_t* moduleName, const char* label)
{
    HMODULE moduleHandle = GetModuleHandleW(moduleName);
    if (!moduleHandle)
    {
        char moduleNameUtf8[256];
        WideToUtf8(moduleName, moduleNameUtf8, _countof(moduleNameUtf8));
        DxrtOrtDelayLog("%s: %s is not loaded yet", label, moduleNameUtf8);
        return;
    }

    LogModulePathFromHandle(moduleHandle, label);
}

void LogStartupState()
{
    DxrtOrtDelayLog("ort_delay_load module initialized");
    LogModulePathFromHandle(GetModuleHandleW(nullptr), "Current process image");
    LogModulePathIfLoaded(L"onnxruntime.dll", "At dxrt load (pre-hook check)");
}

struct StartupLogger
{
    StartupLogger()
    {
        LogStartupState();
    }
};

static StartupLogger g_startupLogger;

} // namespace

static FARPROC WINAPI OrtDelayLoadHook(unsigned dliNotify, PDelayLoadInfo pdli)
{
    const char* dllName = (pdli && pdli->szDll) ? pdli->szDll : "(null)";
    DxrtOrtDelayLog(
        "Delay-load notify=%s(%u), dll=%s, pdli=%p",
        DelayNotifyToString(dliNotify),
        dliNotify,
        dllName,
        pdli);

    if (dliNotify != dliNotePreLoadLibrary)
        return nullptr;

    if (!pdli || !pdli->szDll)
        return nullptr;

    if (_stricmp(pdli->szDll, "onnxruntime.dll") != 0)
        return nullptr;

    LogModulePathIfLoaded(L"onnxruntime.dll", "Before explicit load");

    // Obtain the directory that contains the current module (dxrt.dll).
    HMODULE hSelf = nullptr;
    if (!GetModuleHandleExW(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&OrtDelayLoadHook),
            &hSelf) ||
        !hSelf)
    {
        LogLastError("GetModuleHandleExW");
        return nullptr;
    }
    LogModulePathFromHandle(hSelf, "dxrt module path");

    wchar_t modulePath[MAX_PATH];
    DWORD len = GetModuleFileNameW(hSelf, modulePath, MAX_PATH);
    if (len == 0 || len >= MAX_PATH)
    {
        LogLastError("GetModuleFileNameW(hSelf)");
        return nullptr;
    }

    // Replace the trailing filename with onnxruntime.dll.
    wchar_t* lastSlash = wcsrchr(modulePath, L'\\');
    if (!lastSlash)
    {
        DxrtOrtDelayLog("Failed to locate path separator in dxrt module path");
        return nullptr;
    }

    // Ensure there is enough space for "onnxruntime.dll" + null terminator.
    const wchar_t ortDll[] = L"onnxruntime.dll";
    if ((lastSlash - modulePath + 1) + _countof(ortDll) > MAX_PATH)
    {
        DxrtOrtDelayLog("Target path buffer is too small for onnxruntime.dll");
        return nullptr;
    }

    wcscpy_s(lastSlash + 1, MAX_PATH - (lastSlash - modulePath + 1), ortDll);

    char explicitPathUtf8[MAX_PATH * 3];
    WideToUtf8(modulePath, explicitPathUtf8, _countof(explicitPathUtf8));
    DxrtOrtDelayLog("Trying explicit ORT load from path=%s", explicitPathUtf8);

    DWORD attrs = GetFileAttributesW(modulePath);
    if (attrs == INVALID_FILE_ATTRIBUTES)
        LogLastError("GetFileAttributesW(onnxruntime.dll)");
    else
        DxrtOrtDelayLog("Explicit ORT path exists. fileAttributes=0x%08lx", static_cast<unsigned long>(attrs));

    // Load from the explicit path.  LOAD_WITH_ALTERED_SEARCH_PATH makes the
    // loader resolve onnxruntime.dll's own dependencies (e.g.
    // onnxruntime_providers_shared.dll) from the same directory.
    SetLastError(ERROR_SUCCESS);
    HMODULE hOrt = LoadLibraryExW(modulePath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!hOrt)
    {
        LogLastError("LoadLibraryExW(onnxruntime.dll explicit path)");
        LogModulePathIfLoaded(L"onnxruntime.dll", "After failed explicit load");
        return nullptr;
    }

    LogModulePathFromHandle(hOrt, "Explicit LoadLibraryExW returned handle");
    LogModulePathIfLoaded(L"onnxruntime.dll", "After explicit load");
    return reinterpret_cast<FARPROC>(hOrt);
}

static FARPROC WINAPI OrtDelayLoadFailureHook(unsigned dliNotify, PDelayLoadInfo pdli)
{
    if (dliNotify == dliFailLoadLib || dliNotify == dliFailGetProc)
    {
        const char* dllName = (pdli && pdli->szDll) ? pdli->szDll : "(null)";
        DxrtOrtDelayLog(
            "Delay-load failure notify=%s(%u), dll=%s, pdli=%p, hmodCur=%p, pfnCur=%p, dwLastError=%lu",
            DelayNotifyToString(dliNotify),
            dliNotify,
            dllName,
            pdli,
            pdli ? pdli->hmodCur : nullptr,
            pdli ? pdli->pfnCur : nullptr,
            pdli ? static_cast<unsigned long>(pdli->dwLastError) : 0UL);
        LogLastError("Delay-load failure hook");
        LogModulePathIfLoaded(L"onnxruntime.dll", "At failure hook");
    }

    return nullptr;
}

// Register the hooks with the MSVC delay-load helper.
extern "C" const PfnDliHook __pfnDliNotifyHook2 = OrtDelayLoadHook;
extern "C" const PfnDliHook __pfnDliFailureHook2 = OrtDelayLoadFailureHook;

#endif // _WIN32 && USE_ORT
