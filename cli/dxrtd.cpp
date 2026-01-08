/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/common.h"
#include "dxrt/service_util.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#include <thread>
#include <windows.h>
#include <shellapi.h>
#endif

int dxrt_service_main(int argc, char* argv[]);

#ifdef _WIN32
// Windows service related global variables
static SERVICE_STATUS g_ServiceStatus = {0};
static SERVICE_STATUS_HANDLE g_StatusHandle = nullptr;
static HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;
static HANDLE g_ServiceMutex = nullptr;  // Mutex to indicate service is running
static const wchar_t* SERVICE_NAME = L"dxrtd";
static std::thread g_ServiceThread;

// Check if running as administrator
bool IsRunningAsAdmin()
{
    BOOL isAdmin = FALSE;
    PSID adminGroup = nullptr;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&ntAuthority, 2,
        SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &adminGroup))
    {
        CheckTokenMembership(nullptr, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin == TRUE;
}

// Relaunch with elevated privileges
bool RelaunchAsAdmin(int argc, char* argv[])
{
    wchar_t szPath[MAX_PATH];
    if (!GetModuleFileNameW(nullptr, szPath, MAX_PATH))
    {
        return false;
    }

    // Build command line arguments
    std::wstring args;
    for (int i = 1; i < argc; ++i)
    {
        if (i > 1) args += L" ";
        // Convert char* to wstring
        int len = MultiByteToWideChar(CP_UTF8, 0, argv[i], -1, nullptr, 0);
        std::wstring warg(len, 0);
        MultiByteToWideChar(CP_UTF8, 0, argv[i], -1, &warg[0], len);
        warg.resize(len - 1);  // Remove null terminator
        args += warg;
    }

    // Use ShellExecuteEx to request elevation
    SHELLEXECUTEINFOW sei = {0};
    sei.cbSize = sizeof(sei);
    sei.lpVerb = L"runas";
    sei.lpFile = szPath;
    sei.lpParameters = args.c_str();
    sei.nShow = SW_SHOWNORMAL;
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;

    if (!ShellExecuteExW(&sei))
    {
        DWORD error = GetLastError();
        if (error == ERROR_CANCELLED)
        {
            std::cerr << "User declined elevation request" << std::endl;
        }
        else
        {
            std::cerr << "Failed to elevate: " << error << std::endl;
        }
        return false;
    }

    // Wait for elevated process to complete
    if (sei.hProcess)
    {
        WaitForSingleObject(sei.hProcess, INFINITE);
        CloseHandle(sei.hProcess);
    }
    return true;
}

// Function to report service status
void ReportServiceStatus(DWORD currentState, DWORD exitCode, DWORD waitHint)
{
    static DWORD checkPoint = 1;

    g_ServiceStatus.dwCurrentState = currentState;
    g_ServiceStatus.dwWin32ExitCode = exitCode;
    g_ServiceStatus.dwWaitHint = waitHint;

    if (currentState == SERVICE_START_PENDING)
        g_ServiceStatus.dwControlsAccepted = 0;
    else
        g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

    if (currentState == SERVICE_RUNNING || currentState == SERVICE_STOPPED)
        g_ServiceStatus.dwCheckPoint = 0;
    else
        g_ServiceStatus.dwCheckPoint = checkPoint++;

    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

// Service control handler
void WINAPI ServiceCtrlHandler(DWORD ctrlCode)
{
    switch (ctrlCode)
    {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
            ReportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 3000);
            SetEvent(g_ServiceStopEvent);
            return;
        default:
            break;
    }
}

// Service main function
void WINAPI ServiceMain(DWORD argc, LPWSTR* argv)
{
    std::ignore = argc;
    std::ignore = argv;

    // Register service status handle
    g_StatusHandle = RegisterServiceCtrlHandlerW(SERVICE_NAME, ServiceCtrlHandler);
    if (g_StatusHandle == nullptr)
    {
        return;
    }

    // Initialize service status
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;

    // Report service start pending status
    ReportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    // Create service stop event
    g_ServiceStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (g_ServiceStopEvent == nullptr)
    {
        ReportServiceStatus(SERVICE_STOPPED, GetLastError(), 0);
        return;
    }

    // Create service mutex to indicate service is running
    g_ServiceMutex = dxrt::createServiceMutex();
    if (g_ServiceMutex == nullptr)
    {
        CloseHandle(g_ServiceStopEvent);
        ReportServiceStatus(SERVICE_STOPPED, ERROR_SERVICE_ALREADY_RUNNING, 0);
        return;
    }

    // Run service logic in a separate thread
    g_ServiceThread = std::thread([]() {
        char* args[] = { const_cast<char*>("dxrtd"), nullptr };
        dxrt_service_main(1, args);
    });

    // Report service running status
    ReportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);

    // Wait for service stop event
    WaitForSingleObject(g_ServiceStopEvent, INFINITE);

    // Cleanup
    if (g_ServiceThread.joinable())
    {
        g_ServiceThread.detach();  // Detach since dxrt_service_main runs indefinitely
    }
    
    // Release service mutex
    dxrt::releaseServiceMutex(g_ServiceMutex);
    g_ServiceMutex = nullptr;
    
    CloseHandle(g_ServiceStopEvent);
    ReportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

// Service installation function
bool InstallService()
{
    SC_HANDLE schSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
    if (schSCManager == nullptr)
    {
        std::cerr << "OpenSCManager failed: " << GetLastError() << std::endl;
        return false;
    }

    wchar_t szPath[MAX_PATH];
    if (!GetModuleFileNameW(nullptr, szPath, MAX_PATH))
    {
        std::cerr << "GetModuleFileName failed: " << GetLastError() << std::endl;
        CloseServiceHandle(schSCManager);
        return false;
    }

    // Append --run argument for service execution
    std::wstring servicePath = szPath;
    servicePath += L" --run";

    SC_HANDLE schService = CreateServiceW(
        schSCManager,
        SERVICE_NAME,
        SERVICE_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        servicePath.c_str(),
        nullptr, nullptr, nullptr, nullptr, nullptr);

    if (schService == nullptr)
    {
        std::cerr << "CreateService failed: " << GetLastError() << std::endl;
        CloseServiceHandle(schSCManager);
        return false;
    }

    std::cout << "Service installed successfully" << std::endl;
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return true;
}

// Service uninstallation function
bool UninstallService()
{
    SC_HANDLE schSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (schSCManager == nullptr)
    {
        std::cerr << "OpenSCManager failed: " << GetLastError() << std::endl;
        return false;
    }

    SC_HANDLE schService = OpenServiceW(schSCManager, SERVICE_NAME, SERVICE_ALL_ACCESS);
    if (schService == nullptr)
    {
        std::cerr << "OpenService failed: " << GetLastError() << std::endl;
        CloseServiceHandle(schSCManager);
        return false;
    }

    if (!DeleteService(schService))
    {
        std::cerr << "DeleteService failed: " << GetLastError() << std::endl;
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return false;
    }

    std::cout << "Service uninstalled successfully" << std::endl;
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return true;
}

// Start the Windows service
bool StartServiceCmd()
{
    SC_HANDLE schSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (schSCManager == nullptr)
    {
        std::cerr << "OpenSCManager failed: " << GetLastError() << std::endl;
        return false;
    }

    SC_HANDLE schService = OpenServiceW(schSCManager, SERVICE_NAME, SERVICE_START | SERVICE_QUERY_STATUS);
    if (schService == nullptr)
    {
        std::cerr << "OpenService failed: " << GetLastError() << std::endl;
        CloseServiceHandle(schSCManager);
        return false;
    }

    // Check current service status
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO,
        reinterpret_cast<LPBYTE>(&ssStatus), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
    {
        if (ssStatus.dwCurrentState == SERVICE_RUNNING)
        {
            std::cout << "Service is already running" << std::endl;
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return true;
        }
    }

    // Start the service
    if (!StartServiceW(schService, 0, nullptr))
    {
        DWORD error = GetLastError();
        if (error == ERROR_SERVICE_ALREADY_RUNNING)
        {
            std::cout << "Service is already running" << std::endl;
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return true;
        }
        std::cerr << "StartService failed: " << error << std::endl;
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return false;
    }

    // Wait for service to start
    std::cout << "Starting service";
    const DWORD timeout = 30000;  // 30 seconds timeout
    DWORD startTime = GetTickCount();
    while (true)
    {
        if (!QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO,
            reinterpret_cast<LPBYTE>(&ssStatus), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
        {
            break;
        }

        if (ssStatus.dwCurrentState == SERVICE_RUNNING)
        {
            std::cout << std::endl << "Service started successfully" << std::endl;
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return true;
        }

        if (GetTickCount() - startTime > timeout)
        {
            std::cerr << std::endl << "Timeout waiting for service to start" << std::endl;
            break;
        }

        std::cout << ".";
        Sleep(500);
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return false;
}

// Stop the Windows service
bool StopServiceCmd()
{
    SC_HANDLE schSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (schSCManager == nullptr)
    {
        std::cerr << "OpenSCManager failed: " << GetLastError() << std::endl;
        return false;
    }

    SC_HANDLE schService = OpenServiceW(schSCManager, SERVICE_NAME, SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (schService == nullptr)
    {
        std::cerr << "OpenService failed: " << GetLastError() << std::endl;
        CloseServiceHandle(schSCManager);
        return false;
    }

    // Check current service status
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO,
        reinterpret_cast<LPBYTE>(&ssStatus), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
    {
        if (ssStatus.dwCurrentState == SERVICE_STOPPED)
        {
            std::cout << "Service is already stopped" << std::endl;
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return true;
        }
    }

    // Send stop control
    SERVICE_STATUS status;
    if (!ControlService(schService, SERVICE_CONTROL_STOP, &status))
    {
        DWORD error = GetLastError();
        if (error == ERROR_SERVICE_NOT_ACTIVE)
        {
            std::cout << "Service is already stopped" << std::endl;
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return true;
        }
        std::cerr << "ControlService failed: " << error << std::endl;
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return false;
    }

    // Wait for service to stop
    std::cout << "Stopping service";
    const DWORD timeout = 30000;  // 30 seconds timeout
    DWORD startTime = GetTickCount();
    while (true)
    {
        if (!QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO,
            reinterpret_cast<LPBYTE>(&ssStatus), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
        {
            break;
        }

        if (ssStatus.dwCurrentState == SERVICE_STOPPED)
        {
            std::cout << std::endl << "Service stopped successfully" << std::endl;
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return true;
        }

        if (GetTickCount() - startTime > timeout)
        {
            std::cerr << std::endl << "Timeout waiting for service to stop" << std::endl;
            break;
        }

        std::cout << ".";
        Sleep(500);
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return false;
}

// Run in console mode
int RunConsoleMode(int argc, char* argv[])
{
    // Create service mutex to indicate service is running
    g_ServiceMutex = dxrt::createServiceMutex();
    if (g_ServiceMutex == nullptr)
    {
        std::cout << "Other instance of dxrtd is running" << std::endl;
        return -1;
    }

#ifdef USE_SERVICE
    int result = dxrt_service_main(argc, argv);
    
    // Release mutex on exit
    dxrt::releaseServiceMutex(g_ServiceMutex);
    g_ServiceMutex = nullptr;
    
    return result;
#else
    std::ignore = argc;
    std::ignore = argv;
    dxrt::releaseServiceMutex(g_ServiceMutex);
    g_ServiceMutex = nullptr;
    std::cout << "USE_SERVICE is not set, so dxrt_service will not work" << std::endl;
    return -1;
#endif
}

// Run as Windows service
int RunAsService()
{
    SERVICE_TABLE_ENTRYW ServiceTable[] = {
        { const_cast<LPWSTR>(SERVICE_NAME), ServiceMain },
        { nullptr, nullptr }
    };

    if (!StartServiceCtrlDispatcherW(ServiceTable))
    {
        DWORD error = GetLastError();
        if (error == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
        {
            std::cerr << "Failed to connect to service controller" << std::endl;
            std::cerr << "Use --run option only when started by Windows Service Manager" << std::endl;
        }
        return -1;
    }
    return 0;
}

// Print usage information
void PrintUsage()
{
    std::cout << "Usage: dxrtd [options]" << std::endl;
    std::cout << "  (no options)       Run in console mode" << std::endl;
    std::cout << "  --install, -i      Install as Windows service" << std::endl;
    std::cout << "  --uninstall, -u    Uninstall Windows service" << std::endl;
    std::cout << "  --start            Start the Windows service" << std::endl;
    std::cout << "  --stop             Stop the Windows service" << std::endl;
    std::cout << "  --run, -r          Run as Windows service (used by SCM)" << std::endl;
    std::cout << "  --help, -h         Show this help message" << std::endl;
}
#endif

int main(int argc, char* argv[])
{
#ifdef _WIN32
    // No arguments: run in console mode
    if (argc == 1)
    {
        return RunConsoleMode(argc, argv);
    }

    // Process command line arguments
    std::string arg = argv[1];
    if (arg == "--install" || arg == "-i")
    {
        // Request elevation if not running as admin
        if (!IsRunningAsAdmin())
        {
            std::cout << "Requesting administrator privileges..." << std::endl;
            return RelaunchAsAdmin(argc, argv) ? 0 : -1;
        }
        return InstallService() ? 0 : -1;
    }
    else if (arg == "--uninstall" || arg == "-u")
    {
        // Request elevation if not running as admin
        if (!IsRunningAsAdmin())
        {
            std::cout << "Requesting administrator privileges..." << std::endl;
            return RelaunchAsAdmin(argc, argv) ? 0 : -1;
        }
        return UninstallService() ? 0 : -1;
    }
    else if (arg == "--start")
    {
        // Request elevation if not running as admin
        if (!IsRunningAsAdmin())
        {
            std::cout << "Requesting administrator privileges..." << std::endl;
            return RelaunchAsAdmin(argc, argv) ? 0 : -1;
        }
        return StartServiceCmd() ? 0 : -1;
    }
    else if (arg == "--stop")
    {
        // Request elevation if not running as admin
        if (!IsRunningAsAdmin())
        {
            std::cout << "Requesting administrator privileges..." << std::endl;
            return RelaunchAsAdmin(argc, argv) ? 0 : -1;
        }
        return StopServiceCmd() ? 0 : -1;
    }
    else if (arg == "--run" || arg == "-r")
    {
        // Run as Windows service (called by SCM)
        return RunAsService();
    }
    else if (arg == "--help" || arg == "-h")
    {
        PrintUsage();
        return 0;
    }
    else
    {
        // Unknown option or additional arguments: run in console mode
        return RunConsoleMode(argc, argv);
    }
#else
    std::ignore = argc;
    std::ignore = argv;
    if (dxrt::isDxrtServiceRunning())
    {
        std::cout << "Other instance of dxrtd is running" << std::endl;
        return -1;
    }
#ifdef USE_SERVICE
    return dxrt_service_main(argc, argv);
#else
    std::cout << "USE_SERVICE is not set, so dxrt_service will not work" << std::endl;
    return -1;
#endif
#endif
}