#!/bin/bash
# Build script for DxEngine C# package

echo "Building DxEngine NuGet package..."

cd "$(dirname "$0")/DxEngine"

# Restore dependencies
dotnet restore

# Build in Release mode
dotnet build -c Release

# Create NuGet package
dotnet pack -c Release -o ../packages

echo ""
echo "Building DxEngine CLI tools..."

cd "$(dirname "$0")/../cli"

# Restore dependencies
dotnet restore

# Build in Release mode
dotnet build -c Release

# Publish CLI as self-contained executable
dotnet publish -c Release -o ../bin

echo ""
echo "Build completed."
echo "- NuGet package is in the 'packages' folder."
echo "- CLI tools are in the 'bin' folder."
