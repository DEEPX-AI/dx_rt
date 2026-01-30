@echo off
setlocal enabledelayedexpansion

REM Build wheels for multiple Python versions on Windows
REM Supported Python versions: 3.10, 3.11, 3.12, 3.13, 3.14
REM
REM Usage: build_wheels.bat
REM        build_wheels.bat --cibuildwheel
REM        build_wheels.bat 3.11 3.12 3.13

set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

set "OUTPUT_DIR=wheelhouse"
set "USE_CIBUILDWHEEL=0"
set "PYTHON_VERSIONS=3.10 3.11 3.12 3.13 3.14"

REM Parse arguments
set "CUSTOM_VERSIONS="
:parse_args
if "%~1"=="" goto :done_args
if /i "%~1"=="--cibuildwheel" (
    set "USE_CIBUILDWHEEL=1"
    shift
    goto :parse_args
)
if /i "%~1"=="-c" (
    set "USE_CIBUILDWHEEL=1"
    shift
    goto :parse_args
)
if /i "%~1"=="--output" (
    set "OUTPUT_DIR=%~2"
    shift
    shift
    goto :parse_args
)
if /i "%~1"=="-o" (
    set "OUTPUT_DIR=%~2"
    shift
    shift
    goto :parse_args
)
REM Assume it's a Python version
set "CUSTOM_VERSIONS=!CUSTOM_VERSIONS! %~1"
shift
goto :parse_args
:done_args

REM Use custom versions if provided
if not "!CUSTOM_VERSIONS!"=="" (
    set "PYTHON_VERSIONS=!CUSTOM_VERSIONS:~1!"
)

REM Create output directory
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

echo ============================================
echo Building wheels for Python versions: %PYTHON_VERSIONS%
echo Output directory: %OUTPUT_DIR%
echo ============================================

if "%USE_CIBUILDWHEEL%"=="1" (
    call :build_with_cibuildwheel
) else (
    call :build_manual
)

REM List built wheels
echo.
echo ============================================
echo Built wheels in %OUTPUT_DIR%:
for %%f in ("%OUTPUT_DIR%\*.whl") do (
    echo   %%~nxf
)
echo ============================================

endlocal
exit /b 0

REM ============================================
REM Build using cibuildwheel
REM ============================================
:build_with_cibuildwheel
echo.
echo Using cibuildwheel...

REM Check if cibuildwheel is installed
pip show cibuildwheel >nul 2>&1
if errorlevel 1 (
    echo Installing cibuildwheel...
    python -m pip install cibuildwheel
)

REM Build version string for cibuildwheel (e.g., "cp310-* cp311-* cp312-*")
set "BUILD_VERSIONS="
for %%v in (%PYTHON_VERSIONS%) do (
    set "ver=%%v"
    set "ver=!ver:.=!"
    set "BUILD_VERSIONS=!BUILD_VERSIONS! cp!ver!-*"
)
set "BUILD_VERSIONS=!BUILD_VERSIONS:~1!"

set "CIBW_BUILD=%BUILD_VERSIONS%"
set "CIBW_SKIP=*-win32"
set "CIBW_TEST_COMMAND=python -c \"import dx_engine; print(dx_engine.__version__)\""

echo CIBW_BUILD: %CIBW_BUILD%

cibuildwheel --platform windows --output-dir "%OUTPUT_DIR%"
exit /b 0

REM ============================================
REM Manual build for each Python version
REM ============================================
:build_manual
echo.
echo Using py launcher for manual builds...

REM Check if py launcher is available
where py >nul 2>&1
if errorlevel 1 (
    echo Error: Python Launcher ^(py^) not found. Please install Python from python.org
    exit /b 1
)

REM Check if winget is available for installing Python
set "WINGET_AVAILABLE=0"
where winget >nul 2>&1
if not errorlevel 1 (
    set "WINGET_AVAILABLE=1"
)

set "SUCCESS_COUNT=0"
set "FAIL_COUNT=0"

for %%v in (%PYTHON_VERSIONS%) do (
    echo.
    echo --------------------------------------------
    echo Building wheel for Python %%v...

    REM Check if this Python version is installed
    py -%%v --version >nul 2>&1
    if errorlevel 1 (
        echo   [INFO] Python %%v is not installed
        call :install_python %%v
        
        REM Check again after installation
        py -%%v --version >nul 2>&1
        if errorlevel 1 (
            echo   [SKIP] Python %%v installation failed or not available
            set /a FAIL_COUNT+=1
        ) else (
            call :build_wheel %%v
        )
    ) else (
        call :build_wheel %%v
    )
)

goto :build_summary

REM ============================================
REM Install Python using winget
REM ============================================
:install_python
set "PY_VER=%~1"
echo   Attempting to install Python %PY_VER%...

if "!WINGET_AVAILABLE!"=="0" (
    echo   [WARN] winget not available. Please install Python %PY_VER% manually from python.org
    exit /b 1
)

REM Map version to winget package ID
REM winget package IDs: Python.Python.3.10, Python.Python.3.11, etc.
set "WINGET_ID=Python.Python.%PY_VER%"

echo   Running: winget install %WINGET_ID% --silent --accept-package-agreements --accept-source-agreements
winget install %WINGET_ID% --silent --accept-package-agreements --accept-source-agreements

if errorlevel 1 (
    echo   [WARN] Failed to install Python %PY_VER% via winget
    exit /b 1
)

echo   [INFO] Python %PY_VER% installed successfully
echo   [INFO] Refreshing environment variables...

REM Refresh PATH by calling refreshenv or manually updating
REM Since we can't refresh in the same session easily, we try to find the new Python
for /f "tokens=*" %%p in ('where /r "%LOCALAPPDATA%\Programs\Python" python.exe 2^>nul ^| findstr /i "Python%PY_VER:.=%"') do (
    echo   [INFO] Found newly installed Python at: %%p
)

exit /b 0

REM ============================================
REM Build wheel for a specific Python version
REM ============================================
:build_wheel
set "PY_VER=%~1"

for /f "tokens=*" %%a in ('py -%PY_VER% --version 2^>^&1') do (
    echo   Found: %%a
)

REM Ensure pip and build tools are available
echo   Installing build dependencies...
py -%PY_VER% -m pip install --upgrade pip wheel build scikit-build-core pybind11 >nul 2>&1

REM Build wheel using python -m build
echo   Building wheel...
py -%PY_VER% -m build --wheel --outdir "%OUTPUT_DIR%" 2>&1

if errorlevel 1 (
    echo   [FAIL] Failed to build wheel for Python %PY_VER%
    set /a FAIL_COUNT+=1
) else (
    echo   [SUCCESS] Wheel built for Python %PY_VER%
    set /a SUCCESS_COUNT+=1
)

exit /b 0

REM ============================================
REM Build summary
REM ============================================
:build_summary

:build_summary
echo.
echo ============================================
echo Build Summary:
echo   Success: %SUCCESS_COUNT%
echo   Failed/Skipped: %FAIL_COUNT%

exit /b 0
