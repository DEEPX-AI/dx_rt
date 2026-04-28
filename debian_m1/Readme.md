# Debian Package Build Files (M1)

This directory contains the necessary files for building a Debian package (.deb) for the libdxrt project (DEEPX Runtime).

## Files Overview

### debian/ Directory Structure
- **control**: Package metadata (name, version, dependencies, description)
- **changelog**: Version history and release notes
- **compat**: Debhelper compatibility level (version 9)
- **rules**: Build instructions and installation logic (uses rsync to copy source)
- **install**: Build notes and instructions
- **postinst**: Post-installation script (builds C++ runtime and Python module via `build.sh`)
- **postrm**: Post-removal script (removes source directory)
- **prerm**: Pre-removal script (runs `build.sh --uninstall` before file removal)

## Prerequisites

Install Debian packaging tools:
```bash
sudo apt-get install dpkg-dev debhelper build-essential rsync cmake
```

## Build Process

### 1. Prepare the debian directory
From the root of the repository, rename `debian_m1` to `debian`:
```bash
mv debian_m1 debian
```

### 2. Build the Debian Package
```bash
dpkg-buildpackage -us -uc
```

This will:
- Copy the entire source tree (excluding `debian/`, `.git/`, `.github/`, `release/`) to the package staging area via rsync
- Create `libdxrt_[version]_all.deb`

## Package Contents

The generated .deb package includes the full source tree installed to `/usr/share/libdxrt/`. On installation, the `postinst` script automatically:

1. Runs `build.sh --clean` to compile the C++ runtime library and Python `dx_engine` module
2. Handles PEP 668 (externally-managed-environment) for Python >= 3.11 by using `--python-break-system-packages`
3. Copies `_pydxrt.so` to the system Python site-packages directory
4. Verifies the `dx_engine` module version

## Installation

```bash
sudo dpkg -i libdxrt_[version]_all.deb
sudo apt-get install -f  # Fix any dependency issues
```

### Post-Installation Verification

```bash
# Verify dx_engine Python module
python3 -c "import dx_engine; print(dx_engine.__version__)"

# Verify shared libraries are registered
ldconfig -p | grep libdxrt
```

### ldconfig (Shared Library Cache)

After installation, `ldconfig` updates the shared library cache so the system can find `libdxrt.so` at runtime.

- **What it does**: Scans `/usr/local/lib`, `/usr/lib`, and paths listed in `/etc/ld.so.conf`, then rebuilds `/etc/ld.so.cache`.
- **When to run manually**: If you see errors like `error while loading shared libraries: libdxrt.so: cannot open shared object file`, run:
  ```bash
  sudo ldconfig
  ```
- **Verify**: Check that the library is registered:
  ```bash
  ldconfig -p | grep libdxrt
  ```

### Python venv 
While Debian typically installs to the system Python, using a virtual environment (via python3-venv) is the recommended practice.

To set up a virtual environment and install the package, follow this step:

```
python3 -m venv --system-site-packages my_venv
```

## Uninstallation

```bash
sudo dpkg -r libdxrt
```

The uninstallation process:
1. **prerm**: Runs `build.sh --uninstall` to remove built artifacts (libraries, binaries, Python modules)
2. **postrm**: Removes the source directory `/usr/share/libdxrt/`

## Using a Virtual Environment (Recommended)

While the Debian package installs the Python module to system Python by default, using a virtual environment is recommended:

```bash
# Install python3-venv if not already installed
sudo apt-get install python3-venv

# Create a virtual environment with access to system site-packages
python3 -m venv --system-site-packages my_venv

# Activate the virtual environment
source my_venv/bin/activate
```

## Package Information

View package details:
```bash
dpkg-deb --info libdxrt_[version]_all.deb
```

View package contents:
```bash
dpkg-deb --contents libdxrt_[version]_all.deb
```

## Troubleshooting

### dpkg-buildpackage not found
```bash
sudo apt-get install dpkg-dev debhelper
```

### Build fails during postinst
The `postinst` script runs `build.sh --clean` which compiles from source. Ensure all build dependencies are installed:
```bash
sudo apt-get install build-essential cmake ninja-build python3-dev python3-pip
```

### dx_engine module not found after installation
Check that the `.so` file was copied correctly:
```bash
python3 -c "import dx_engine"
```
If it fails, check the postinst log and verify that the site-packages path is correct.

### Shared library not found (libdxrt.so)
Update the library cache:
```bash
sudo ldconfig
ldconfig -p | grep libdxrt
```

## Version Management

The package version is defined in `debian/changelog`. To release a new version, add a new entry at the top of the changelog file.

## Notes

- This package uses `Architecture: all` — the source is compiled on the target machine during installation via `postinst`
- Build time depends on the target machine's hardware (compiling C++ from source)
- Python >= 3.11 requires `--break-system-packages` flag for pip, which is handled automatically
