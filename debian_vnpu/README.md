# Debian Package Build Files

This directory contains the necessary files for building a Debian package (.deb) for the dx-vnpu-rt-ep project.

## Files Overview

### debian/ Directory Structure
- **control**: Package metadata (name, version, dependencies, description)
- **changelog**: Version history and release notes
- **compat**: Debhelper compatibility level (version 9)
- **rules**: Build instructions and installation logic
- **copyright**: License and copyright information
- **postinst**: Post-installation script (runs after package installation)
- **postrm**: Post-removal script (runs after package removal)

### Build Script
- **build_debian_package.sh**: Main script to create the .deb package

## Prerequisites

Install Debian packaging tools:
```bash
sudo apt-get install dpkg-dev debhelper build-essential
```

## Build Process

### 1. Cross-compile for ARM64
First, build the project for aarch64 architecture:
```bash
./build.sh --arch aarch64
```

This will create the `build_aarch64/` directory with compiled binaries.

### 2. Build the Debian Package
Run the packaging script:
```bash
./build_debian_package.sh
```

This script will:
- Verify that cross-compilation completed successfully
- Check for required binaries in `build_aarch64/bin/`
- Verify ONNX Runtime libraries in `util/onnxruntime_aarch64/lib/`
- Build the Debian package
- Create `dx-vnpu-rt-ep_[version]_arm64.deb`

## Package Contents

The generated .deb package includes:

### Binaries (installed to /usr/bin/):
- dxrt-cli
- parse_model
- run_model

### Libraries (installed to /usr/lib/aarch64-linux-gnu/):
- libdxrt.so* (from build_aarch64/lib/)
- libonnxruntime*.so* (from util/onnxruntime_aarch64/lib/)

### Headers (installed to /usr/include/dxrt/):
- Header files from build_aarch64/include/

### Documentation (installed to /usr/share/doc/dx-vnpu-rt-ep/):
- README.md
- LICENSE
- CHANGELOG.md

## Installation

On an ARM64 system, install the package:
```bash
sudo dpkg -i dx-vnpu-rt-ep_[version]_arm64.deb
sudo apt-get install -f  # Fix any dependency issues
```

## Uninstallation

Remove the package:
```bash
sudo dpkg -r dx-vnpu-rt-ep
```

## Package Information

View package details:
```bash
dpkg-deb --info dx-vnpu-rt-ep_[version]_arm64.deb
```

View package contents:
```bash
dpkg-deb --contents dx-vnpu-rt-ep_[version]_arm64.deb
```

## Troubleshooting

### Missing binaries
If the build fails due to missing binaries, ensure cross-compilation completed:
```bash
./build.sh --arch aarch64
ls -la build_aarch64/bin/
```

### Missing ONNX Runtime
If ONNX Runtime libraries are missing:
```bash
ls -la util/onnxruntime_aarch64/lib/
```

### dpkg-buildpackage not found
Install packaging tools:
```bash
sudo apt-get install dpkg-dev debhelper
```

## Version Management

The package version is automatically read from `release.ver` file. To change the version:
1. Update `release.ver`
2. The `build_debian_package.sh` script will automatically update `debian/changelog`

## Notes

- This package is built for **arm64** (aarch64) architecture only
- The package uses pre-compiled binaries from cross-compilation
- No compilation occurs during package building
- Library dependencies are automatically resolved by dpkg-shlibdeps
