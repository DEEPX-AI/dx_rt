@echo off
REM Build script for DxEngine C# package

echo Building DxEngine NuGet package...

cd /d "%~dp0DxEngine"

REM Restore dependencies
dotnet restore

REM Build in Release mode
dotnet build -c Release

REM Create NuGet package
dotnet pack -c Release -o ..\packages

echo.
echo Building DxEngine CLI tools...

cd /d "%~dp0cli"

REM Restore dependencies
dotnet restore

REM Build in Release mode
dotnet build -c Release

REM Publish CLI as self-contained executable
dotnet publish -c Release -o ..\bin

echo.
echo Build completed.
echo - NuGet package is in the 'packages' folder.
echo - CLI tools are in the 'bin' folder.
pause
