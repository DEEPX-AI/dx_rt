// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/exception_handler.h"
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <execinfo.h>

using namespace std;

namespace dxrt {

static void signalHandler(int signo)
{
    void *array[22];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 22);
    strings = backtrace_symbols(array, size);
    printf("Exception: Caught signal %d:\n", signo);
    for (i = 0; i < size; i++)
    {
        printf("[%d] %s\n", i, strings[i]);
    }
    free(strings);
    exit(EXIT_FAILURE);    
}
ExceptionHandler::ExceptionHandler()
{
    signal(SIGSEGV, signalHandler);
    signal(SIGBUS, signalHandler);
    signal(SIGABRT, signalHandler);
}
ExceptionHandler exceptionHandler;

} // namespace dxrt
