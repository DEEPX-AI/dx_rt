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

:: Visual Studio paths (checked in order)
set "VS_COMMUNITY_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community"
set "VS_PROFESSIONAL_PATH=C:\Program Files\Microsoft Visual Studio\2022\Professional"
set "VS_ENTERPRISE_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
set "VS_BUILDTOOLS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools"
set "VS_PATH="

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
if /i "%~1"=="help" goto :show_help
if /i "%~1"=="-h" goto :show_help
if /i "%~1"=="--help" goto :show_help
if /i "%~1"=="/?" goto :show_help

echo Unknown option: %~1
goto :show_help

:after_parse

:: Handle install-tools command first
if "%DO_INSTALL_TOOLS%"=="1" goto :install_build_tools

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
    echo Solution file: %SCRIPT_DIR%build_vs\dx_rt.sln
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
    echo This will remove:
    echo   - out\                  ^(all build and install files^)
    echo   - build_vs\             ^(Visual Studio project^)
    echo   - vcpkg_installed\      ^(vcpkg packages^)
    echo.
    set /p "CONFIRM_DISTCLEAN=Are you sure? [y/N] "
    if /i not "!CONFIRM_DISTCLEAN!"=="y" (
        echo Cancelled.
        goto :end
    )
    echo.
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
    echo.
    echo [OK] Distclean completed. Workspace reset to source-only state.
    goto :end
)

:: All = Clean + Configure + Build + Install (Release)
if "%DO_ALL%"=="1" (
    echo.
    echo [INFO] Full build: clean + configure + build + install ^(!BUILD_TYPE!^)
    echo.
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
    echo Output directory: %SCRIPT_DIR%out\build\x64-%BUILD_TYPE%\bin
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
echo Other Options:
echo   help, -h, --help, /?   Show this help message
echo.
echo Examples:
echo   build.bat                    Configure and build Release
echo   build.bat debug              Configure and build Debug
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
echo   build.bat install-tools     Install VS Build Tools 2022 (CLI only, no IDE)
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
endlocal
