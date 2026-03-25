#!/bin/bash

# Script to build Debian package for dx-vnpu-rt-ep (ARM64)
# This script creates a .deb package from cross-compiled binaries

set -e

SCRIPT_DIR=$(realpath "$(dirname "$0")")
cd "$SCRIPT_DIR"

# Color definitions
COLOR_RED='\033[0;31m'
COLOR_GREEN='\033[0;32m'
COLOR_YELLOW='\033[1;33m'
COLOR_BLUE='\033[0;34m'
COLOR_RESET='\033[0m'

TAG_INFO="${COLOR_BLUE}[INFO]${COLOR_RESET}"
TAG_SUCCESS="${COLOR_GREEN}[SUCCESS]${COLOR_RESET}"
TAG_ERROR="${COLOR_RED}[ERROR]${COLOR_RESET}"
TAG_WARN="${COLOR_YELLOW}[WARN]${COLOR_RESET}"

# Read version from release.ver
VERSION=$(cat release.ver | tr -d 'v' | tr -d '\n')
if [ -z "$VERSION" ]; then
    echo -e "${TAG_ERROR} Failed to read version from release.ver"
    exit 1
fi

echo -e "${TAG_INFO} Building Debian package for dx-vnpu-rt-ep version ${VERSION}"

# Check if build_aarch64 directory exists
if [ ! -d "build_aarch64" ]; then
    echo -e "${TAG_ERROR} build_aarch64 directory not found!"
    echo -e "${TAG_INFO} Please run cross-compilation first:"
    echo -e "          ./build.sh --arch aarch64"
    exit 1
fi

# Check if required binaries exist
REQUIRED_BINS=(
    "build_aarch64/bin/dxrt-cli"
    "build_aarch64/bin/parse_model"
    "build_aarch64/bin/run_model"
)

for bin in "${REQUIRED_BINS[@]}"; do
    if [ ! -f "$bin" ]; then
        echo -e "${TAG_ERROR} Required binary not found: $bin"
        echo -e "${TAG_INFO} Please ensure cross-compilation completed successfully"
        exit 1
    fi
done

# Check if ONNX Runtime libraries exist
if [ ! -d "util/onnxruntime_aarch64/lib" ]; then
    echo -e "${TAG_ERROR} ONNX Runtime libraries not found in util/onnxruntime_aarch64/lib"
    exit 1
fi

# Check for required Debian tools
if ! command -v dpkg-buildpackage &> /dev/null; then
    echo -e "${TAG_ERROR} dpkg-buildpackage not found!"
    echo -e "${TAG_INFO} Please install: sudo apt-get install dpkg-dev debhelper"
    exit 1
fi

# Clean previous debian build artifacts
echo -e "${TAG_INFO} Cleaning previous build artifacts..."
rm -rf debian_vnpu/dx-vnpu-rt-ep
rm -f debian_vnpu/files
rm -f debian_vnpu/*.log
rm -f debian_vnpu/*.substvars
rm -f debian_vnpu/*.debhelper

# Make debian scripts executable
chmod +x debian_vnpu/rules
chmod +x debian_vnpu/postinst
chmod +x debian_vnpu/postrm

# Update version in debian_vnpu/changelog if needed
CHANGELOG_VERSION=$(head -n 1 debian_vnpu/changelog | grep -oP '\(\K[^)]+')
if [ "$CHANGELOG_VERSION" != "${VERSION}-1" ]; then
    echo -e "${TAG_INFO} Updating debian_vnpu/changelog with version ${VERSION}"
    cat > debian_vnpu/changelog << EOF
dx-vnpu-rt-ep (${VERSION}-1) stable; urgency=medium

  * Release version ${VERSION}
  * Cross-compiled binaries from build_aarch64
  * Includes ONNX Runtime libraries for aarch64

 -- DeepX <support@deepx.ai>  $(date -R)
EOF
fi

# Backup existing debian folder if it exists and create symlink for dpkg-buildpackage
DEBIAN_BACKUP=false
if [ -e "debian" ]; then
    echo -e "${TAG_INFO} Backing up existing debian folder..."
    mv debian debian_backup_temp
    DEBIAN_BACKUP=true
fi

echo -e "${TAG_INFO} Creating temporary debian symlink..."
ln -sf debian_vnpu debian

# Build the package
echo -e "${TAG_INFO} Building Debian package..."
dpkg-buildpackage -Zgzip -us -uc -b -aarm64

# Remove temporary symlink and restore backup if needed
rm -f debian
if [ "$DEBIAN_BACKUP" = true ]; then
    echo -e "${TAG_INFO} Restoring original debian folder..."
    mv debian_backup_temp debian
fi

# Check if package was created
DEB_FILE="../dx-vnpu-rt-ep_${VERSION}-1_arm64.deb"
if [ -f "$DEB_FILE" ]; then
    # Move to current directory
    mv "$DEB_FILE" "./dx-vnpu-rt-ep_${VERSION}_arm64.deb"
    DEB_FILE="./dx-vnpu-rt-ep_${VERSION}_arm64.deb"
    
    echo -e "${TAG_SUCCESS} Debian package created successfully!"
    echo -e "${TAG_INFO} Package: ${COLOR_GREEN}${DEB_FILE}${COLOR_RESET}"
    echo -e ""
    echo -e "${TAG_INFO} Package information:"
    dpkg-deb --info "$DEB_FILE"
    echo -e ""
    echo -e "${TAG_INFO} Package contents:"
    dpkg-deb --contents "$DEB_FILE"
    echo -e ""
    echo -e "${TAG_INFO} To install the package on an ARM64 system:"
    echo -e "          sudo dpkg -i ${DEB_FILE}"
    echo -e "          sudo apt-get install -f  # Fix dependencies if needed"
else
    echo -e "${TAG_ERROR} Failed to create Debian package!"
    exit 1
fi

# Clean build artifacts
echo -e "${TAG_INFO} Cleaning build artifacts..."
rm -rf debian_vnpu/dx-vnpu-rt-ep
rm -f debian_vnpu/files
rm -f debian_vnpu/*.log
rm -f debian_vnpu/*.substvars
rm -f debian_vnpu/*.debhelper
rm -f ../dx-vnpu-rt-ep_${VERSION}-1_arm64.buildinfo
rm -f ../dx-vnpu-rt-ep_${VERSION}-1_arm64.changes

echo -e "${TAG_SUCCESS} Done!"
