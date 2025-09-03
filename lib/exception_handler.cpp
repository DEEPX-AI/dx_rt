/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/dxrt_api.h"
#include "dxrt/exception_handler.h"
#include <csignal>
#include <cstdlib>
#include <cstdio>

#ifdef __linux__
    #include <execinfo.h>
#elif _WIN32
    #include <windows.h>
    #include <dbghelp.h>
    #pragma comment(lib, "dbghelp.lib")
#endif


namespace dxrt {

#ifdef __linux__
static void signalHandler(int signo)
{
    std::ignore = signo;
#ifdef DXRT_SHOW_STACKTRACE_ON_HANDLER
    void* array[22];
    size_t size;
    char** strings;
    size_t i;

    size = backtrace(array, 22);
    strings = backtrace_symbols(array, size);
    printf("Exception: Caught signal %d:\n", signo);
    for (i = 0; i < size; i++)
    {
        printf("[%lu] %s\n", i, strings[i]);
    }
    free(strings);
#endif
    exit(EXIT_FAILURE);
}
#elif _WIN32
void windows_print_stacktrace(CONTEXT* context)
{
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    SymInitialize(process, NULL, TRUE);

    STACKFRAME64 stack_frame;
    memset(&stack_frame, 0, sizeof(STACKFRAME64));

#if defined(_M_IX86)
    DWORD machine = IMAGE_FILE_MACHINE_I386;
    stack_frame.AddrPC.Offset = context->Eip;
    stack_frame.AddrPC.Mode = AddrModeFlat;
    stack_frame.AddrFrame.Offset = context->Ebp;
    stack_frame.AddrFrame.Mode = AddrModeFlat;
    stack_frame.AddrStack.Offset = context->Esp;
    stack_frame.AddrStack.Mode = AddrModeFlat;
#elif defined(_M_X64)
    DWORD machine = IMAGE_FILE_MACHINE_AMD64;
    stack_frame.AddrPC.Offset = context->Rip;
    stack_frame.AddrPC.Mode = AddrModeFlat;
    stack_frame.AddrFrame.Offset = context->Rsp;
    stack_frame.AddrFrame.Mode = AddrModeFlat;
    stack_frame.AddrStack.Offset = context->Rsp;
    stack_frame.AddrStack.Mode = AddrModeFlat;
#endif

    for (int i = 0; i < 22; i++)
    {
        if (!StackWalk64(machine, process, thread, &stack_frame, context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
            break;

        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64 displacement = 0;
        if (SymFromAddr(process, stack_frame.AddrPC.Offset, &displacement, symbol))
        {
            printf("[%d] %s - 0x%0llX\n", i, symbol->Name, symbol->Address);
        }
        else
        {
            printf("[%d] Unknown symbol\n", i);
        }
    }

    SymCleanup(process);
}

LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS* ExceptionInfo)
{
    printf("Exception: Caught signal %d:\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
    windows_print_stacktrace(ExceptionInfo->ContextRecord);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif 

ExceptionHandler::ExceptionHandler()
{
#ifdef __linux__
    signal(SIGSEGV, signalHandler);
    signal(SIGBUS,  signalHandler);
    signal(SIGABRT, signalHandler);
#elif _WIN32
    SetUnhandledExceptionFilter(windows_exception_handler);
#endif
}
ExceptionHandler exceptionHandler;

} // namespace dxrt
