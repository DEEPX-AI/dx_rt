@echo off
setlocal enabledelayedexpansion

:: ============================================================================
:: csharp_package Build Script
:: Quick build for DxEngineNative + DxEngine NuGet package + examples
::
:: Prerequisites:
::   - C++ dx_rt must be built first (build.bat at root)
::   - .NET 8.0 SDK installed
::   - Visual Studio 2022 Build Tools (for DxEngineNative C++/CLI)
::
:: Usage:
::   build.bat              Build all (Release)
::   build.bat debug        Build all (Debug)
::   build.bat pack         Build + create NuGet package only (no examples)
::   build.bat examples     Build examples only (assumes DxEngine already built)
::   build.bat clean        Clean all build artifacts
:: ============================================================================

set "SCRIPT_DIR=%~dp0"
set "ROOT_DIR=%SCRIPT_DIR%.."
set "BUILD_TYPE=Release"
set "DO_NATIVE=1"
set "DO_CSHARP=1"
set "DO_PACK=1"
set "DO_EXAMPLES=1"
set "DO_CLEAN=0"

:: Parse arguments
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
if /i "%~1"=="pack" (
    set "DO_EXAMPLES=0"
    shift
    goto :parse_args
)
if /i "%~1"=="examples" (
    set "DO_NATIVE=0"
    set "DO_CSHARP=0"
    set "DO_PACK=0"
    shift
    goto :parse_args
)
if /i "%~1"=="clean" (
    set "DO_CLEAN=1"
    set "DO_NATIVE=0"
    set "DO_CSHARP=0"
    set "DO_PACK=0"
    set "DO_EXAMPLES=0"
    shift
    goto :parse_args
)
if /i "%~1"=="help" goto :show_help
if /i "%~1"=="-h" goto :show_help
if /i "%~1"=="/?" goto :show_help
echo Unknown option: %~1
goto :show_help
:after_parse

:: Read version from release.ver
set "VERSION=0.0.0"
if exist "%ROOT_DIR%\release.ver" (
    for /f "usebackq tokens=*" %%a in ("%ROOT_DIR%\release.ver") do set "VERSION=%%a"
)
if "!VERSION:~0,1!"=="v" set "VERSION=!VERSION:~1!"

:: Clean
if "%DO_CLEAN%"=="1" (
    echo.
    echo [INFO] Cleaning csharp_package build artifacts...
    if exist "packages" rmdir /s /q "packages"
    if exist "src\DxEngine\bin" rmdir /s /q "src\DxEngine\bin"
    if exist "src\DxEngine\obj" rmdir /s /q "src\DxEngine\obj"
    if exist "src\DxEngineNative\bin" rmdir /s /q "src\DxEngineNative\bin"
    if exist "src\DxEngineNative\Release" rmdir /s /q "src\DxEngineNative\Release"
    if exist "src\DxEngineNative\Debug" rmdir /s /q "src\DxEngineNative\Debug"
    if exist "examples\SimpleInference\bin" rmdir /s /q "examples\SimpleInference\bin"
    if exist "examples\SimpleInference\obj" rmdir /s /q "examples\SimpleInference\obj"
    if exist "examples\YoloV5\bin" rmdir /s /q "examples\YoloV5\bin"
    if exist "examples\YoloV5\obj" rmdir /s /q "examples\YoloV5\obj"
    echo [OK] Clean completed.
    goto :end
)

:: Banner
echo.
echo ============================================================
echo   DxEngine C# Package Build
echo ============================================================
echo   Version    : !VERSION!
echo   Build Type : !BUILD_TYPE!
echo   Root Dir   : !ROOT_DIR!
echo ============================================================
echo.

:: Check that C++ build exists
if not exist "%ROOT_DIR%\out\build\x64-%BUILD_TYPE%\bin" (
    echo [ERROR] C++ build not found at: %ROOT_DIR%\out\build\x64-%BUILD_TYPE%\bin
    echo         Run build.bat at the root first.
    exit /b 1
)

:: Setup Visual Studio environment if needed (for DxEngineNative msbuild)
if "%DO_NATIVE%"=="1" (
    if "%VSCMD_VER%"=="" (
        set "VS_PATH="
        if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community"
        if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Professional"
        if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
        if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools"
        if not defined VS_PATH (
            echo [ERROR] Visual Studio 2022 not found.
            exit /b 1
        )
        echo [INFO] Setting up Visual Studio environment...
        call "!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
    )
)

:: Detect .NET SDK
set "DOTNET_PATH="
where dotnet >nul 2>&1
if errorlevel 1 (
    if exist "C:\Program Files\dotnet\dotnet.exe" set "DOTNET_PATH=C:\Program Files\dotnet"
) else (
    for /f "delims=" %%i in ('where dotnet') do (
        if not defined DOTNET_PATH set "DOTNET_PATH=%%~dpi"
    )
)
if defined DOTNET_PATH (
    set "PATH=!DOTNET_PATH!;!PATH!"
    for /f "delims=" %%v in ('dir /b /o-n "!DOTNET_PATH!\sdk\8.*" 2^>nul') do (
        if not defined DOTNET_SDK_VER set "DOTNET_SDK_VER=%%v"
    )
    if defined DOTNET_SDK_VER (
        set "MSBuildSDKsPath=!DOTNET_PATH!\sdk\!DOTNET_SDK_VER!\Sdks"
    )
) else (
    echo [ERROR] .NET SDK not found. Install .NET 8.0 SDK.
    exit /b 1
)

cd /d "%SCRIPT_DIR%"

:: ---- Step 1: Build DxEngineNative (C++/CLI) ----
if "%DO_NATIVE%"=="1" (
    echo [INFO] Building DxEngineNative C++/CLI wrapper...
    if exist "src\DxEngineNative\DxEngineNative.vcxproj" (
        cd /d "%SCRIPT_DIR%src\DxEngineNative"
        msbuild DxEngineNative.vcxproj /p:Configuration=%BUILD_TYPE% /p:Platform=x64 /p:VcpkgEnableManifest=false /v:minimal
        if errorlevel 1 (
            echo [ERROR] DxEngineNative build failed!
            exit /b 1
        )
        echo [OK] DxEngineNative.dll built successfully.
    )
    cd /d "%SCRIPT_DIR%"
)

:: ---- Step 2: Build DxEngine (C#) ----
if "%DO_CSHARP%"=="1" (
    echo [INFO] Building DxEngine C# library ^(v!VERSION!^)...
    cd /d "%SCRIPT_DIR%src\DxEngine"
    dotnet build -c %BUILD_TYPE% /p:Version=%VERSION%
    if errorlevel 1 (
        echo [ERROR] DxEngine build failed!
        exit /b 1
    )
    echo [OK] DxEngine.dll built successfully.
    cd /d "%SCRIPT_DIR%"
)

:: ---- Step 3: Create NuGet package ----
if "%DO_PACK%"=="1" (
    echo [INFO] Creating NuGet package ^(v!VERSION!^)...
    cd /d "%SCRIPT_DIR%src\DxEngine"
    if exist "..\..\packages\*.nupkg" del /q "..\..\packages\*.nupkg"
    dotnet pack -c %BUILD_TYPE% /p:Version=%VERSION% -o ..\..\packages
    if errorlevel 1 (
        echo [ERROR] NuGet pack failed!
        exit /b 1
    )
    echo [OK] NuGet package created: packages\DxEngine.!VERSION!.nupkg
    cd /d "%SCRIPT_DIR%"
)

:: ---- Step 4: Build examples ----
if "%DO_EXAMPLES%"=="1" (
    echo [INFO] Building examples...
    cd /d "%SCRIPT_DIR%examples"

    if exist "SimpleInference\SimpleInference.csproj" (
        echo [INFO]   SimpleInference...
        dotnet build SimpleInference\SimpleInference.csproj -c %BUILD_TYPE% /p:Version=%VERSION%
        if errorlevel 1 echo [WARNING] SimpleInference build failed.
    )

    if exist "YoloV5\YoloV5Example.csproj" (
        echo [INFO]   YoloV5Example...
        dotnet build YoloV5\YoloV5Example.csproj -c %BUILD_TYPE% /p:Version=%VERSION%
        if errorlevel 1 echo [WARNING] YoloV5Example build failed.
    )

    echo [OK] Examples build completed.
    cd /d "%SCRIPT_DIR%"
)

echo.
echo ============================================================
echo   Build completed successfully!
echo   Version : !VERSION!
echo   Config  : !BUILD_TYPE!
if "%DO_PACK%"=="1" echo   Package : packages\DxEngine.!VERSION!.nupkg
echo ============================================================
goto :end

:show_help
echo.
echo Usage: build.bat [options]
echo.
echo Options:
echo   (none)       Build all (DxEngineNative + DxEngine + NuGet pack + examples)
echo   debug        Build in Debug configuration
echo   release      Build in Release configuration (default)
echo   pack         Build DxEngineNative + DxEngine + NuGet package only (no examples)
echo   examples     Build examples only (assumes DxEngine already built)
echo   clean        Remove all build artifacts
echo   help         Show this help
echo.

:end
endlocal
