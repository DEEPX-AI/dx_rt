@echo off
setlocal enabledelayedexpansion

:: ============================================================================
:: dx_rt Build Script for Windows
:: Usage: build.bat [options]
:: ============================================================================

set "SCRIPT_DIR=%~dp0"
set "BUILD_TYPE=Release"
set "DO_CONFIGURE=1"
set "DO_BUILD=1"
set "DO_INSTALL=0"
set "DO_CLEAN=0"
set "DO_REBUILD=0"
set "DO_DISTCLEAN=0"
set "DO_ALL=0"
set "DO_VS_PROJECT=0"
set "DO_INSTALL_TOOLS=0"
set "DO_SETUP=0"
set "DO_UNSETUP=0"

:: Visual Studio paths (checked in order)
:: Set VCPKG_DOWNLOADS to avoid SYSTEM account AppData path issues (CI/service)
if not defined VCPKG_DOWNLOADS set "VCPKG_DOWNLOADS=%SCRIPT_DIR%out\vcpkg-downloads"

:: Visual Studio paths (checked in order)
set "VS_COMMUNITY_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community"
set "VS_PROFESSIONAL_PATH=C:\Program Files\Microsoft Visual Studio\2022\Professional"
set "VS_ENTERPRISE_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
set "VS_BUILDTOOLS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools"
set "VS_PATH="

:: If no arguments, default to "all" (e.g. double-click)
set "NO_ARGS=0"
if "%~1"=="" (
    set "DO_ALL=1"
    set "NO_ARGS=1"
    goto :after_parse
)

:: Parse command line arguments
:parse_args
if "%~1"=="" goto :after_parse
if /i "%~1"=="debug" (
    set "BUILD_TYPE=Debug"
    shift
    goto :parse_args
)
if /i "%~1"=="release" (
    set "BUILD_TYPE=Release"
    shift
    goto :parse_args
)
if /i "%~1"=="vs" (
    set "DO_VS_PROJECT=1"
    set "DO_CONFIGURE=0"
    set "DO_BUILD=0"
    shift
    goto :parse_args
)
if /i "%~1"=="configure" (
    set "DO_BUILD=0"
    shift
    goto :parse_args
)
if /i "%~1"=="build" (
    set "DO_CONFIGURE=0"
    shift
    goto :parse_args
)
if /i "%~1"=="clean" (
    set "DO_CLEAN=1"
    set "DO_CONFIGURE=0"
    set "DO_BUILD=0"
    shift
    goto :parse_args
)
if /i "%~1"=="rebuild" (
    set "DO_REBUILD=1"
    shift
    goto :parse_args
)
if /i "%~1"=="install" (
    set "DO_INSTALL=1"
    shift
    goto :parse_args
)
if /i "%~1"=="distclean" (
    set "DO_DISTCLEAN=1"
    set "DO_CONFIGURE=0"
    set "DO_BUILD=0"
    shift
    goto :parse_args
)
if /i "%~1"=="all" (
    set "DO_ALL=1"
    shift
    goto :parse_args
)
if /i "%~1"=="install-tools" (
    set "DO_INSTALL_TOOLS=1"
    set "DO_CONFIGURE=0"
    set "DO_BUILD=0"
    shift
    goto :parse_args
)
if /i "%~1"=="setup" (
    set "DO_SETUP=1"
    set "DO_CONFIGURE=0"
    set "DO_BUILD=0"
    shift
    goto :parse_args
)
if /i "%~1"=="unsetup" (
    set "DO_UNSETUP=1"
    set "DO_CONFIGURE=0"
    set "DO_BUILD=0"
    shift
    goto :parse_args
)
if /i "%~1"=="help" goto :show_help
if /i "%~1"=="-h" goto :show_help
if /i "%~1"=="--help" goto :show_help
if /i "%~1"=="/?" goto :show_help

echo Unknown option: %~1
goto :show_help

:after_parse

:: If only build type was specified (no action), default to "all"
if "%DO_ALL%"=="0" if "%DO_CLEAN%"=="0" if "%DO_REBUILD%"=="0" if "%DO_DISTCLEAN%"=="0" if "%DO_VS_PROJECT%"=="0" if "%DO_INSTALL_TOOLS%"=="0" (
    if "%DO_INSTALL%"=="0" if "%DO_CONFIGURE%"=="1" if "%DO_BUILD%"=="1" (
        set "DO_ALL=1"
    )
)

:: Handle install-tools command first
if "%DO_INSTALL_TOOLS%"=="1" goto :install_build_tools

:: Handle setup/unsetup commands (no VS environment needed)
if "%DO_SETUP%"=="1" goto :do_setup
if "%DO_UNSETUP%"=="1" goto :do_unsetup

:: Detect Visual Studio or Build Tools installation
call :detect_vs_installation
if not defined VS_PATH (
    echo.
    echo ============================================================
    echo   dx_rt Build Script
    echo ============================================================
    echo.
    echo [ERROR] No Visual Studio 2022 or Build Tools found!
    echo.
    echo Checked locations:
    echo   - !VS_COMMUNITY_PATH!
    echo   - !VS_PROFESSIONAL_PATH!
    echo   - !VS_ENTERPRISE_PATH!
    echo   - !VS_BUILDTOOLS_PATH!
    echo.
    echo To install Visual Studio Build Tools 2022 ^(CLI only, no IDE^):
    echo   build.bat install-tools
    echo.
    echo Or download manually from:
    echo   https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo.
    exit /b 1
)

:: Show banner
echo.
echo ============================================================
echo   dx_rt Build Script
echo ============================================================
if "%DO_VS_PROJECT%"=="1" (
    echo   Action     : Generate Visual Studio Project
    echo   Output Dir : !SCRIPT_DIR!build_vs
) else (
    echo   Build Type : !BUILD_TYPE!
)
echo   Source Dir : !SCRIPT_DIR!
echo   VS Path    : !VS_PATH!
echo ============================================================
echo.

:: Setup Visual Studio environment if not already set
if "%VSCMD_VER%"=="" (
    echo [INFO] Setting up Visual Studio 2022 x64 environment...
    call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
    if errorlevel 1 (
        echo [ERROR] Failed to setup Visual Studio environment.
        exit /b 1
    )
    echo [OK] Visual Studio environment configured.
) else (
    echo [INFO] Visual Studio environment already configured ^(v%VSCMD_VER%^)
)

:: Change to script directory
cd /d "%SCRIPT_DIR%"

:: Generate Visual Studio Project
if "%DO_VS_PROJECT%"=="1" (
    echo.
    echo [INFO] Generating Visual Studio 2022 solution/project files...
    echo.
    cmake --preset x64-VS
    if errorlevel 1 (
        echo.
        echo [ERROR] Failed to generate Visual Studio project!
        exit /b 1
    )
    echo.
    echo [OK] Visual Studio project generated successfully!
    echo.
    echo Solution file: !SCRIPT_DIR!build_vs\dx_rt.sln
    echo.
    echo You can open it with:
    echo   start build_vs\dx_rt.sln
    goto :end
)

:: Distclean (full reset to source-only state)
if "%DO_DISTCLEAN%"=="1" (
    echo.
    echo [INFO] Full reset - removing all build artifacts...
    echo.
    call :stop_dxrtd_service
    if exist "out" (
        rmdir /s /q "out"
        echo [OK] Removed out\
    )
    if exist "build_vs" (
        rmdir /s /q "build_vs"
        echo [OK] Removed build_vs\
    )
    if exist "vcpkg_installed" (
        rmdir /s /q "vcpkg_installed"
        echo [OK] Removed vcpkg_installed\
    )
    echo [INFO] Cleaning csharp_package build artifacts...
    if exist "csharp_package\bin" (
        rmdir /s /q "csharp_package\bin"
        echo [OK] Removed csharp_package\bin
    )
    if exist "csharp_package\packages" (
        rmdir /s /q "csharp_package\packages"
        echo [OK] Removed csharp_package\packages
    )
    if exist "csharp_package\.vs" (
        rmdir /s /q "csharp_package\.vs"
        echo [OK] Removed csharp_package\.vs
    )
    if exist "csharp_package\src\DxEngine\bin" (
        rmdir /s /q "csharp_package\src\DxEngine\bin"
        echo [OK] Removed csharp_package\src\DxEngine\bin
    )
    if exist "csharp_package\src\DxEngine\obj" (
        rmdir /s /q "csharp_package\src\DxEngine\obj"
        echo [OK] Removed csharp_package\src\DxEngine\obj
    )
    if exist "csharp_package\src\DxEngineNative\bin" (
        rmdir /s /q "csharp_package\src\DxEngineNative\bin"
        echo [OK] Removed csharp_package\src\DxEngineNative\bin
    )
    if exist "csharp_package\src\DxEngineNative\Release" (
        rmdir /s /q "csharp_package\src\DxEngineNative\Release"
        echo [OK] Removed csharp_package\src\DxEngineNative\Release
    )
    if exist "csharp_package\src\DxEngineNative\Debug" (
        rmdir /s /q "csharp_package\src\DxEngineNative\Debug"
        echo [OK] Removed csharp_package\src\DxEngineNative\Debug
    )
    if exist "csharp_package\examples\.vs" (
        rmdir /s /q "csharp_package\examples\.vs"
        echo [OK] Removed csharp_package\examples\.vs
    )
    if exist "csharp_package\examples\SimpleInference\bin" (
        rmdir /s /q "csharp_package\examples\SimpleInference\bin"
        echo [OK] Removed csharp_package\examples\SimpleInference\bin
    )
    if exist "csharp_package\examples\SimpleInference\obj" (
        rmdir /s /q "csharp_package\examples\SimpleInference\obj"
        echo [OK] Removed csharp_package\examples\SimpleInference\obj
    )
    if exist "csharp_package\examples\YoloV5\bin" (
        rmdir /s /q "csharp_package\examples\YoloV5\bin"
        echo [OK] Removed csharp_package\examples\YoloV5\bin
    )
    if exist "csharp_package\examples\YoloV5\obj" (
        rmdir /s /q "csharp_package\examples\YoloV5\obj"
        echo [OK] Removed csharp_package\examples\YoloV5\obj
    )
    echo.
    echo [OK] Distclean completed. Workspace reset to source-only state.
    goto :end
)

:: All = Clean + Configure + Build + Install (Release)
if "%DO_ALL%"=="1" (
    echo.
    echo [INFO] Full build: clean + configure + build + install ^(!BUILD_TYPE!^)
    echo.
    call :stop_dxrtd_service
    if exist "out\build\x64-!BUILD_TYPE!" (
        rmdir /s /q "out\build\x64-!BUILD_TYPE!"
        echo [OK] Removed out\build\x64-!BUILD_TYPE!
    )
    if exist "out\install\x64-!BUILD_TYPE!" (
        rmdir /s /q "out\install\x64-!BUILD_TYPE!"
        echo [OK] Removed out\install\x64-!BUILD_TYPE!
    )
    set "DO_CONFIGURE=1"
    set "DO_BUILD=1"
    set "DO_INSTALL=1"
)

:: Clean
if "%DO_CLEAN%"=="1" (
    echo.
    echo [INFO] Cleaning build directories...
    if exist "out\build\x64-%BUILD_TYPE%" (
        rmdir /s /q "out\build\x64-%BUILD_TYPE%"
        echo [OK] Removed out\build\x64-%BUILD_TYPE%
    )
    if exist "vcpkg_installed" (
        echo [INFO] vcpkg_installed folder exists. Delete it too? [y/N]
        set /p "DEL_VCPKG="
        if /i "!DEL_VCPKG!"=="y" (
            rmdir /s /q "vcpkg_installed"
            echo [OK] Removed vcpkg_installed
        )
    )
    echo.
    echo [INFO] Cleaning csharp_package build artifacts...
    if exist "csharp_package\bin" (
        rmdir /s /q "csharp_package\bin"
        echo [OK] Removed csharp_package\bin
    )
    if exist "csharp_package\packages" (
        rmdir /s /q "csharp_package\packages"
        echo [OK] Removed csharp_package\packages
    )
    if exist "csharp_package\.vs" (
        rmdir /s /q "csharp_package\.vs"
        echo [OK] Removed csharp_package\.vs
    )
    if exist "csharp_package\src\DxEngine\bin" (
        rmdir /s /q "csharp_package\src\DxEngine\bin"
        echo [OK] Removed csharp_package\src\DxEngine\bin
    )
    if exist "csharp_package\src\DxEngine\obj" (
        rmdir /s /q "csharp_package\src\DxEngine\obj"
        echo [OK] Removed csharp_package\src\DxEngine\obj
    )
    if exist "csharp_package\src\DxEngineNative\bin" (
        rmdir /s /q "csharp_package\src\DxEngineNative\bin"
        echo [OK] Removed csharp_package\src\DxEngineNative\bin
    )
    if exist "csharp_package\src\DxEngineNative\Release" (
        rmdir /s /q "csharp_package\src\DxEngineNative\Release"
        echo [OK] Removed csharp_package\src\DxEngineNative\Release
    )
    if exist "csharp_package\src\DxEngineNative\Debug" (
        rmdir /s /q "csharp_package\src\DxEngineNative\Debug"
        echo [OK] Removed csharp_package\src\DxEngineNative\Debug
    )
    if exist "csharp_package\examples\.vs" (
        rmdir /s /q "csharp_package\DxEngineNative\Debug"
        echo [OK] Removed csharp_package\DxEngineNative\Debug
    )
    if exist "csharp_package\examples\.vs" (
        rmdir /s /q "csharp_package\examples\.vs"
        echo [OK] Removed csharp_package\examples\.vs
    )
    if exist "csharp_package\examples\SimpleInference\bin" (
        rmdir /s /q "csharp_package\examples\SimpleInference\bin"
        echo [OK] Removed csharp_package\examples\SimpleInference\bin
    )
    if exist "csharp_package\examples\SimpleInference\obj" (
        rmdir /s /q "csharp_package\examples\SimpleInference\obj"
        echo [OK] Removed csharp_package\examples\SimpleInference\obj
    )
    if exist "csharp_package\examples\YoloV5\bin" (
        rmdir /s /q "csharp_package\examples\YoloV5\bin"
        echo [OK] Removed csharp_package\examples\YoloV5\bin
    )
    if exist "csharp_package\examples\YoloV5\obj" (
        rmdir /s /q "csharp_package\examples\YoloV5\obj"
        echo [OK] Removed csharp_package\examples\YoloV5\obj
    )
    echo [OK] Clean completed.
    goto :end
)

:: Rebuild = Clean + Configure + Build
if "%DO_REBUILD%"=="1" (
    echo.
    echo [INFO] Rebuild: Cleaning build directory...
    if exist "out\build\x64-%BUILD_TYPE%" (
        rmdir /s /q "out\build\x64-%BUILD_TYPE%"
        echo [OK] Removed out\build\x64-%BUILD_TYPE%
    )
    set "DO_CONFIGURE=1"
    set "DO_BUILD=1"
)

:: Configure
if "%DO_CONFIGURE%"=="1" (
    echo.
    echo [INFO] Running CMake configure with preset x64-%BUILD_TYPE%...
    echo.
    cmake --preset x64-%BUILD_TYPE%
    if errorlevel 1 (
        echo.
        echo [ERROR] CMake configure failed!
        exit /b 1
    )
    echo.
    echo [OK] CMake configure completed.
)

:: Build
if "%DO_BUILD%"=="1" (
    echo.
    echo [INFO] Building with preset x64-%BUILD_TYPE%...
    echo.
    cmake --build --preset x64-%BUILD_TYPE%
    if errorlevel 1 (
        echo.
        echo [ERROR] Build failed!
        exit /b 1
    )
    echo.
    echo [OK] Build completed successfully!
    echo.
    echo Output directory: !SCRIPT_DIR!out\build\x64-!BUILD_TYPE!\bin
)

:: Install
if "%DO_INSTALL%"=="1" (
    echo.
    echo [INFO] Installing to out\install\x64-!BUILD_TYPE!...
    echo.
    cmake --install out\build\x64-!BUILD_TYPE!
    if errorlevel 1 (
        echo.
        echo [ERROR] Install failed!
        exit /b 1
    )
    echo.
    echo [OK] Install completed successfully!
    echo.
    echo Install directory: !SCRIPT_DIR!out\install\x64-!BUILD_TYPE!
)

:: Build C# package (after install so dxrt.lib is available)
if "%DO_BUILD%"=="1" (
    call :build_csharp_package
)

:: Restart dxrtd service if it was stopped before build
if "!RESTART_DXRTD!"=="1" call :start_dxrtd_service

goto :end

:: ============================================================================
:: Function: Stop dxrtd service if running (sets RESTART_DXRTD=1 on success)
:: ============================================================================
:stop_dxrtd_service
sc query dxrtd >nul 2>&1
if errorlevel 1 exit /b 0
sc query dxrtd | findstr /i "RUNNING" >nul 2>&1
if errorlevel 1 exit /b 0

echo [INFO] Stopping dxrtd service...
:: Try net stop first (waits for service to fully stop)
net stop dxrtd >nul 2>&1
if not errorlevel 1 (
    echo [OK] dxrtd service stopped.
    set "RESTART_DXRTD=1"
    exit /b 0
)
:: net stop failed — likely not running as administrator. Try sc stop.
sc stop dxrtd >nul 2>&1
:: Wait briefly for the service to release file handles
ping -n 3 127.0.0.1 >nul 2>&1
sc query dxrtd | findstr /i "STOPPED" >nul 2>&1
if not errorlevel 1 (
    echo [OK] dxrtd service stopped.
    set "RESTART_DXRTD=1"
    exit /b 0
)
echo [WARNING] Failed to stop dxrtd service. Run as administrator if files are locked.
exit /b 1

:: ============================================================================
:: Function: Start dxrtd service
:: ============================================================================
:start_dxrtd_service
echo.
echo [INFO] Restarting dxrtd service...
net start dxrtd >nul 2>&1
if not errorlevel 1 (
    echo [OK] dxrtd service restarted.
    exit /b 0
)
sc start dxrtd >nul 2>&1
if not errorlevel 1 (
    echo [OK] dxrtd service restarted.
    exit /b 0
)
echo [WARNING] Failed to restart dxrtd service. Start manually: net start dxrtd
exit /b 1

:: ============================================================================
:: Setup: Register install bin to PATH + install & start dxrtd service
:: ============================================================================
:do_setup
echo.
echo ============================================================
echo   dx_rt Setup (PATH + Service)
echo ============================================================
echo.

:: Check administrator privileges
net session >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Administrator privileges required.
    echo         Right-click and select "Run as administrator".
    exit /b 1
)

set "INSTALL_BIN=%SCRIPT_DIR%out\install\x64-!BUILD_TYPE!\bin"
if not exist "!INSTALL_BIN!\dxrtd.exe" (
    echo [ERROR] dxrtd.exe not found at: !INSTALL_BIN!
    echo         Run "build.bat all" or "build.bat install" first.
    exit /b 1
)

echo   Build Type : !BUILD_TYPE!
echo   Bin Path   : !INSTALL_BIN!
echo.

:: --- Step 1: Add to system PATH ---
echo [INFO] Registering bin directory to system PATH...

:: Read current system PATH
for /f "tokens=2*" %%A in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path 2^>nul') do set "SYS_PATH=%%B"

:: Check if already registered
echo !SYS_PATH! | findstr /i /c:"!INSTALL_BIN!" >nul 2>&1
if not errorlevel 1 (
    echo [INFO] PATH already contains: !INSTALL_BIN!
) else (
    :: Append to system PATH (remove trailing semicolons first)
    set "NEW_PATH=!SYS_PATH!"
    if "!NEW_PATH:~-1!"==";" set "NEW_PATH=!NEW_PATH:~0,-1!"
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path /t REG_EXPAND_SZ /d "!NEW_PATH!;!INSTALL_BIN!" /f >nul 2>&1
    if errorlevel 1 (
        echo [ERROR] Failed to update system PATH.
        exit /b 1
    )
    echo [OK] Added to system PATH: !INSTALL_BIN!
    echo [INFO] New terminal sessions will pick up the updated PATH.
)

:: Broadcast WM_SETTINGCHANGE so new cmd/explorer processes pick up the change
:: setx with a dummy user variable triggers the broadcast; we then remove it.
setx DXRT_PATH_UPDATED 1 >nul 2>&1
reg delete "HKCU\Environment" /v DXRT_PATH_UPDATED /f >nul 2>&1

:: --- Step 2: Install and start dxrtd service ---
echo.
echo [INFO] Registering dxrtd service...
"!INSTALL_BIN!\dxrtd.exe" --install
if errorlevel 1 (
    echo [ERROR] Service registration failed.
    exit /b 1
)
echo [OK] dxrtd service registered.

echo [INFO] Starting dxrtd service...
"!INSTALL_BIN!\dxrtd.exe" --start
if errorlevel 1 (
    echo [ERROR] Failed to start dxrtd service.
    exit /b 1
)
echo [OK] dxrtd service started.

echo.
echo ============================================================
echo   Setup completed successfully!
echo   - System PATH updated
echo   - dxrtd service installed and running
echo ============================================================
goto :end

:: ============================================================================
:: Unsetup: Stop & uninstall dxrtd service + remove PATH entry
:: ============================================================================
:do_unsetup
echo.
echo ============================================================
echo   dx_rt Unsetup (Service + PATH)
echo ============================================================
echo.

:: Check administrator privileges
net session >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Administrator privileges required.
    echo         Right-click and select "Run as administrator".
    exit /b 1
)

set "INSTALL_BIN=%SCRIPT_DIR%out\install\x64-!BUILD_TYPE!\bin"

echo   Build Type : !BUILD_TYPE!
echo   Bin Path   : !INSTALL_BIN!
echo.

:: --- Step 1: Stop and uninstall dxrtd service ---
if exist "!INSTALL_BIN!\dxrtd.exe" (
    echo [INFO] Stopping dxrtd service...
    "!INSTALL_BIN!\dxrtd.exe" --stop 2>nul
    echo [INFO] Uninstalling dxrtd service...
    "!INSTALL_BIN!\dxrtd.exe" --uninstall 2>nul
    echo [OK] dxrtd service removed.
) else (
    echo [WARNING] dxrtd.exe not found at: !INSTALL_BIN!
    echo          Attempting service removal via sc...
    sc stop dxrtd >nul 2>&1
    sc delete dxrtd >nul 2>&1
)

:: --- Step 2: Remove from system PATH ---
echo.
echo [INFO] Removing bin directory from system PATH...

for /f "tokens=2*" %%A in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path 2^>nul') do set "SYS_PATH=%%B"

:: Check if our path is in the system PATH
echo !SYS_PATH! | findstr /i /c:"!INSTALL_BIN!" >nul 2>&1
if errorlevel 1 (
    echo [INFO] PATH does not contain: !INSTALL_BIN!
) else (
    :: Remove our path entry
    set "NEW_PATH=!SYS_PATH!"
    :: Handle both ";ourpath" and "ourpath;" patterns
    set "NEW_PATH=!NEW_PATH:;%INSTALL_BIN%=!"
    set "NEW_PATH=!NEW_PATH:%INSTALL_BIN%;=!"
    set "NEW_PATH=!NEW_PATH:%INSTALL_BIN%=!"
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path /t REG_EXPAND_SZ /d "!NEW_PATH!" /f >nul 2>&1
    if errorlevel 1 (
        echo [ERROR] Failed to update system PATH.
        exit /b 1
    )
    echo [OK] Removed from system PATH: !INSTALL_BIN!
)

:: Broadcast WM_SETTINGCHANGE so new cmd/explorer processes pick up the change
setx DXRT_PATH_UPDATED 1 >nul 2>&1
reg delete "HKCU\Environment" /v DXRT_PATH_UPDATED /f >nul 2>&1

echo.
echo ============================================================
echo   Unsetup completed successfully!
echo   - dxrtd service stopped and removed
echo   - System PATH entry removed
echo ============================================================
goto :end

:: ============================================================================
:show_help
echo.
echo Usage: build.bat [options]
echo.
echo Build Type Options:
echo   release     Build Release configuration (default)
echo   debug       Build Debug configuration
echo.
echo Action Options:
echo   configure   Only run CMake configure (skip build)
echo   build       Only run build (skip configure)
echo   install     Install after build (to out\install\)
echo   clean       Clean build directories
echo   rebuild     Clean and rebuild (configure + build)
echo   all         Full build: clean + configure + build + install
echo   distclean   Full reset (remove out/, build_vs/, vcpkg_installed/)
echo   vs          Generate Visual Studio 2022 solution/project files
echo.
echo Service/PATH Options:
echo   setup       Register install bin to system PATH + install and start dxrtd service
echo   unsetup     Stop and uninstall dxrtd service + remove PATH entry
echo.
echo Other Options:
echo   install-tools              Install VS Build Tools 2022 (CLI only, no IDE)
echo   help, -h, --help, /?       Show this help message
echo.
echo Examples:
echo   build.bat                    Full Release build (same as: build.bat all)
echo   build.bat debug              Full Debug build (same as: build.bat debug all)
echo   build.bat build              Build Release only (skip configure)
echo   build.bat debug build        Build Debug only (skip configure)
echo   build.bat configure          Configure Release only
echo   build.bat debug configure    Configure Debug only
echo   build.bat clean              Clean Release build directory
echo   build.bat debug clean        Clean Debug build directory
echo   build.bat rebuild            Clean and rebuild Release
echo   build.bat debug rebuild      Clean and rebuild Debug
echo   build.bat install            Build and install Release
echo   build.bat debug install      Build and install Debug
echo   build.bat all                Full Release: clean + config + build + install
echo   build.bat debug all          Full Debug: clean + config + build + install
echo   build.bat distclean          Reset to source-only state (remove all artifacts)
echo   build.bat vs                 Generate Visual Studio project (build_vs\)
echo   build.bat setup              Register PATH + install/start dxrtd service (Release)
echo   build.bat debug setup        Register PATH + install/start dxrtd service (Debug)
echo   build.bat unsetup            Stop/uninstall dxrtd service + remove PATH (Release)
echo   build.bat debug unsetup      Stop/uninstall dxrtd service + remove PATH (Debug)
echo   build.bat install-tools      Install VS Build Tools 2022 (CLI only, no IDE)
echo.
exit /b 0

:: ============================================================================
:: Function: Detect Visual Studio or Build Tools installation
:: ============================================================================
:detect_vs_installation
set "VS_PATH="

:: Use delayed expansion for paths with special characters like (x86)
set "CM_VCVARS=!VS_COMMUNITY_PATH!\VC\Auxiliary\Build\vcvars64.bat"
set "PR_VCVARS=!VS_PROFESSIONAL_PATH!\VC\Auxiliary\Build\vcvars64.bat"
set "EN_VCVARS=!VS_ENTERPRISE_PATH!\VC\Auxiliary\Build\vcvars64.bat"
set "BT_VCVARS=!VS_BUILDTOOLS_PATH!\VC\Auxiliary\Build\vcvars64.bat"

:: Check Community Edition
if exist "!CM_VCVARS!" (
    set "VS_PATH=!VS_COMMUNITY_PATH!"
    exit /b 0
)

:: Check Professional Edition
if exist "!PR_VCVARS!" (
    set "VS_PATH=!VS_PROFESSIONAL_PATH!"
    exit /b 0
)

:: Check Enterprise Edition
if exist "!EN_VCVARS!" (
    set "VS_PATH=!VS_ENTERPRISE_PATH!"
    exit /b 0
)

:: Check Build Tools (CLI only)
if exist "!BT_VCVARS!" (
    set "VS_PATH=!VS_BUILDTOOLS_PATH!"
    exit /b 0
)

exit /b 0

:: ============================================================================
:: Function: Install Visual Studio Build Tools 2022
:: ============================================================================
:install_build_tools
echo.
echo ============================================================
echo   Visual Studio Build Tools 2022 Installer
echo ============================================================
echo.

:: Check if Build Tools already installed (path has parentheses, need special handling)
set "BT_VCVARS=%VS_BUILDTOOLS_PATH%\VC\Auxiliary\Build\vcvars64.bat"
if exist "!BT_VCVARS!" (
    echo [INFO] Visual Studio Build Tools 2022 is already installed at:
    echo        !VS_BUILDTOOLS_PATH!
    echo.
    exit /b 0
)

:: Check if VS Community is installed
set "CM_VCVARS=%VS_COMMUNITY_PATH%\VC\Auxiliary\Build\vcvars64.bat"
if exist "!CM_VCVARS!" (
    echo [INFO] Visual Studio 2022 Community is already installed.
    echo Build Tools installation is not necessary.
    exit /b 0
)

:: Check if VS Professional is installed
set "PR_VCVARS=%VS_PROFESSIONAL_PATH%\VC\Auxiliary\Build\vcvars64.bat"
if exist "!PR_VCVARS!" (
    echo [INFO] Visual Studio 2022 Professional is already installed.
    echo Build Tools installation is not necessary.
    exit /b 0
)

:: Check if VS Enterprise is installed
set "EN_VCVARS=%VS_ENTERPRISE_PATH%\VC\Auxiliary\Build\vcvars64.bat"
if exist "!EN_VCVARS!" (
    echo [INFO] Visual Studio 2022 Enterprise is already installed.
    echo Build Tools installation is not necessary.
    exit /b 0
)

:: Check for winget
where winget >nul 2>&1
if errorlevel 1 (
    echo [ERROR] winget is not available on this system.
    echo.
    echo Please install Build Tools manually:
    echo   1. Download from: https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo   2. Run the installer
    echo   3. Select "Desktop development with C++" workload
    echo.
    exit /b 1
)

echo [INFO] Installing Visual Studio Build Tools 2022 via winget...
echo.
echo This will install:
echo   - MSVC C++ compiler
echo   - Windows SDK
echo   - CMake
echo   - vcpkg
echo.
echo The installation may take 10-30 minutes depending on your internet speed.
echo.
set /p "CONFIRM=Do you want to continue? [y/N] "
if /i not "%CONFIRM%"=="y" (
    echo Installation cancelled.
    exit /b 0
)

echo.
echo [INFO] Starting installation...
echo.

:: Install Build Tools with C++ workload
winget install Microsoft.VisualStudio.2022.BuildTools --silent --accept-package-agreements --accept-source-agreements --override "--wait --quiet --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22621 --add Microsoft.VisualStudio.Component.VC.CMake.Project --add Microsoft.Component.VC.Runtime.UCRTSDK --includeRecommended"

if errorlevel 1 (
    echo.
    echo [ERROR] Installation failed!
    echo.
    echo Please try manual installation:
    echo   1. Download from: https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo   2. Run the installer
    echo   3. Select "Desktop development with C++" workload
    echo.
    exit /b 1
)

echo.
echo [OK] Visual Studio Build Tools 2022 installed successfully!
echo.
echo [IMPORTANT] Please restart your terminal/command prompt before building.
echo.
echo After restart, run:
echo   build.bat
echo.
exit /b 0

:end
echo.
if "%NO_ARGS%"=="1" (
    echo Press any key to exit...
    pause >nul
)
endlocal
exit /b 0

:: ============================================================================
:: Function: Build C# package (csharp_package)
:: ============================================================================
:build_csharp_package
echo.
echo [INFO] Building C# package (csharp_package)...
echo.
if not exist "!SCRIPT_DIR!csharp_package\src\DxEngine\DxEngine.csproj" (
    echo [INFO] C# project not found, skipping csharp_package build.
    exit /b 0
)

:: Detect .NET SDK and add to PATH/environment if needed
set "DOTNET_PATH="
where dotnet >nul 2>&1
if errorlevel 1 (
    if exist "C:\Program Files\dotnet\dotnet.exe" (
        set "DOTNET_PATH=C:\Program Files\dotnet"
    )
) else (
    for /f "delims=" %%i in ('where dotnet') do (
        if not defined DOTNET_PATH set "DOTNET_PATH=%%~dpi"
    )
)
if defined DOTNET_PATH (
    set "PATH=!DOTNET_PATH!;!PATH!"
    echo [INFO] .NET SDK found at: !DOTNET_PATH!
    :: Set MSBuildSDKsPath so VS Build Tools MSBuild can resolve Microsoft.NET.Sdk
    for /f "delims=" %%v in ('dir /b /o-n "!DOTNET_PATH!\sdk\8.*" 2^>nul') do (
        if not defined DOTNET_SDK_VER set "DOTNET_SDK_VER=%%v"
    )
    if defined DOTNET_SDK_VER (
        set "MSBuildSDKsPath=!DOTNET_PATH!\sdk\!DOTNET_SDK_VER!\Sdks"
        echo [INFO] MSBuildSDKsPath set to: !MSBuildSDKsPath!
    )
) else (
    echo [WARNING] .NET SDK not found, skipping C# package build.
    exit /b 0
)

pushd "!SCRIPT_DIR!csharp_package"

:: Read version from release.ver
set "CSHARP_VERSION=0.0.0"
if exist "!SCRIPT_DIR!release.ver" (
    for /f "usebackq tokens=*" %%a in ("!SCRIPT_DIR!release.ver") do set "CSHARP_VERSION=%%a"
)
:: Strip leading 'v' if present (e.g., v3.3.0 -> 3.3.0)
if "!CSHARP_VERSION:~0,1!"=="v" set "CSHARP_VERSION=!CSHARP_VERSION:~1!"
echo [INFO] Package version: !CSHARP_VERSION! ^(from release.ver^)

echo [INFO] Building DxEngineNative C++/CLI wrapper...
if exist "!SCRIPT_DIR!csharp_package\src\DxEngineNative\DxEngineNative.vcxproj" (
    cd /d "!SCRIPT_DIR!csharp_package\src\DxEngineNative"
    msbuild DxEngineNative.vcxproj /p:Configuration=!BUILD_TYPE! /p:Platform=x64 /p:VcpkgEnableManifest=false /v:minimal
    if errorlevel 1 (
        echo [WARNING] DxEngineNative build failed, native DLL will not be available.
    ) else (
        echo [OK] DxEngineNative.dll built successfully.
    )
) else (
    echo [INFO] DxEngineNative.vcxproj not found, skipping native build.
)

echo [INFO] Building DxEngine NuGet package...
cd /d "!SCRIPT_DIR!csharp_package\src\DxEngine"
dotnet restore
if errorlevel 1 (
    echo [WARNING] C# DxEngine restore failed, skipping C# build.
    popd
    exit /b 0
)
dotnet build -c !BUILD_TYPE! /p:Version=!CSHARP_VERSION!
if errorlevel 1 (
    echo [WARNING] C# DxEngine build failed.
    popd
    exit /b 0
)
if exist "..\..\packages\*.nupkg" del /q "..\..\packages\*.nupkg"
dotnet pack -c !BUILD_TYPE! /p:Version=!CSHARP_VERSION! -o ..\..\packages
echo [OK] DxEngine NuGet package created.

echo [INFO] Building C# examples...
cd /d "!SCRIPT_DIR!csharp_package\examples"

if exist "SimpleInference\SimpleInference.csproj" (
    echo [INFO]   Building SimpleInference...
    dotnet build SimpleInference\SimpleInference.csproj -c !BUILD_TYPE!
    if errorlevel 1 echo [WARNING] SimpleInference build failed.
)

if exist "YoloV5\YoloV5Example.csproj" (
    echo [INFO]   Building YoloV5Example...
    dotnet build YoloV5\YoloV5Example.csproj -c %BUILD_TYPE%
    if errorlevel 1 echo [WARNING] YoloV5Example build failed.
)

echo [OK] C# examples build completed.

popd
echo.
echo [OK] C# package build completed successfully!
echo     - NuGet package: csharp_package\packages\
echo     - Examples:      csharp_package\examples\
exit /b 0