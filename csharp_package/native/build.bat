@echo off
REM Build script for DxEngineNative C++/CLI wrapper

echo ============================================
echo Building DxEngineNative C++/CLI wrapper...
echo ============================================

REM Check if Visual Studio is available
where msbuild >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] MSBuild not found. Please run this script from Developer Command Prompt for VS.
    echo         Or add MSBuild to your PATH.
    pause
    exit /b 1
)

cd /d "%~dp0"

REM Build C++/CLI project
echo.
echo Building C++/CLI native wrapper...
msbuild DxEngineNative.vcxproj /p:Configuration=Release /p:Platform=x64 /v:minimal

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] C++/CLI build failed!
    pause
    exit /b 1
)

echo.
echo ============================================
echo Build completed successfully!
echo Output: bin\Release\DxEngineNative.dll
echo ============================================
pause
