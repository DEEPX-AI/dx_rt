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

#if defined(_WIN32) && defined(USE_ORT)

#include <windows.h>
#include <delayimp.h>

static FARPROC WINAPI OrtDelayLoadHook(unsigned dliNotify, PDelayLoadInfo pdli)
{
    if (dliNotify != dliNotePreLoadLibrary)
        return nullptr;

    if (_stricmp(pdli->szDll, "onnxruntime.dll") != 0)
        return nullptr;

    // Obtain the directory that contains the current module (dxrt.dll).
    HMODULE hSelf = nullptr;
    if (!GetModuleHandleExW(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&OrtDelayLoadHook),
            &hSelf) ||
        !hSelf)
    {
        return nullptr;
    }

    wchar_t modulePath[MAX_PATH];
    DWORD len = GetModuleFileNameW(hSelf, modulePath, MAX_PATH);
    if (len == 0 || len >= MAX_PATH)
        return nullptr;

    // Replace the trailing filename with onnxruntime.dll.
    wchar_t* lastSlash = wcsrchr(modulePath, L'\\');
    if (!lastSlash)
        return nullptr;

    // Ensure there is enough space for "onnxruntime.dll" + null terminator.
    const wchar_t ortDll[] = L"onnxruntime.dll";
    if ((lastSlash - modulePath + 1) + _countof(ortDll) > MAX_PATH)
        return nullptr;

    wcscpy_s(lastSlash + 1, MAX_PATH - (lastSlash - modulePath + 1), ortDll);

    // Load from the explicit path.  LOAD_WITH_ALTERED_SEARCH_PATH makes the
    // loader resolve onnxruntime.dll's own dependencies (e.g.
    // onnxruntime_providers_shared.dll) from the same directory.
    HMODULE hOrt = LoadLibraryExW(modulePath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    return reinterpret_cast<FARPROC>(hOrt);
}

// Register the hook with the MSVC delay-load helper.
extern "C" const PfnDliHook __pfnDliNotifyHook2 = OrtDelayLoadHook;

#endif // _WIN32 && USE_ORT
