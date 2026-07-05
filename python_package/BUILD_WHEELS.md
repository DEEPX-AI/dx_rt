# dx_engine — Per-Python-Version Wheel Build Guide

This document describes how to build the `dx-engine` package in
`dx_rt_fork/python_package` for Python 3.10 / 3.11 / 3.12 / 3.13 using
isolated virtual environments (venv). Once the environment is set up, the
whole process can be repeated with a single command: `build_all_wheels.sh`.

---

## 1. Prerequisites

| Item | Requirement | Notes |
|---|---|---|
| OS | Ubuntu 22.04 (x86_64) | Other distributions work with the same steps |
| `libdxrt` runtime | `/usr/local/lib/libdxrt.so` present | Verify with `dpkg -l libdxrt-bin` |
| Headers | `dx_rt_fork/lib/include/dxrt/*.h` | Included in the source tree |
| CMake | ≥ 3.15 | `cmake --version` |
| Compiler | `g++` (C++14 support) | `build-essential` |
| Python | 3.10, 3.11, 3.12, 3.13 | Installed via `uv` (see below) |

Verification commands:

```bash
dpkg -l libdxrt-bin | grep ^ii
ldconfig -p | grep libdxrt.so
cmake --version
g++ --version
```

If `libdxrt.so` is not in the ldconfig cache, run `sudo ldconfig` once.

---

## 2. Multi-Python Setup with `uv`

Internet access is restricted in this environment and Ubuntu 22.04 does not
provide Python 3.12+ via apt. `uv` (from Astral) downloads privately-built
CPython distributions and installs them into the user area.

### 2-1. Install uv (one-time)

```bash
pip3 install --user uv
export PATH="$HOME/.local/bin:$PATH"
uv --version
```

Adding `export PATH="$HOME/.local/bin:$PATH"` to `~/.bashrc` makes this
persistent.

### 2-2. Download Python interpreters (one-time)

On the internal network, GitHub certificates are re-signed by a private CA,
so the system CA store must be used.

```bash
UV_SYSTEM_CERTS=true uv python install 3.10 3.11 3.12 3.13
uv python list --only-installed
```

They are installed under
`~/.local/share/uv/python/cpython-3.X.X-linux-x86_64-gnu/`.

> If you see `invalid peer certificate: UnknownIssuer`, export
> `UV_SYSTEM_CERTS=true` or
> `SSL_CERT_FILE=/etc/ssl/certs/ca-certificates.crt`.

---

## 3. How the Build Works

`pyproject.toml` declares `build-system.requires` as `scikit-build-core`,
`pybind11`, `setuptools`, and `wheel`. Running `pip wheel .` creates an
isolated PEP 517 build environment, and scikit-build-core invokes
`src/dx_engine/capi/CMakeLists.txt` to produce `_pydxrt.so`. It links
against the system-installed `/usr/local/lib/libdxrt.so`.

The resulting wheels are ABI-specific (e.g. `cp310-cp310-linux_x86_64.whl`)
and must be built separately for each Python version.

---

## 4. One-Shot Build — `build_all_wheels.sh`

`python_package/build_all_wheels.sh` automates the entire flow.

### 4-1. Usage

```bash
cd dx_rt_fork/python_package

# Default: build 3.10 3.11 3.12 3.13 → ./wheelhouse/
./build_all_wheels.sh

# Build specific versions only
./build_all_wheels.sh 3.11 3.12

# Change the output directory
./build_all_wheels.sh -o /tmp/my_wheels 3.13

# Recreate venvs from scratch
./build_all_wheels.sh --clean
```

### 4-2. What the Script Does

1. Prepends `~/.local/bin/uv` to `$PATH` automatically if `uv` is not found.
2. Creates a `.venvs/py3X/` venv per version (reused if it already exists).
3. Runs `pip install --upgrade pip wheel build scikit-build-core pybind11`
   inside each venv.
4. Runs `python -m pip wheel . --no-deps --wheel-dir <OUTPUT>` to produce
   the wheel.
   - Uses a dedicated `--build-option` temporary directory per build to
     avoid build-cache collisions.
5. Prints the list of produced wheels.

### 4-3. Example Output

```
wheelhouse/
├── dx_engine-<version>-cp310-cp310-linux_x86_64.whl
├── dx_engine-<version>-cp311-cp311-linux_x86_64.whl
├── dx_engine-<version>-cp312-cp312-linux_x86_64.whl
└── dx_engine-<version>-cp313-cp313-linux_x86_64.whl
```

`<version>` is taken from the package version declared in `pyproject.toml`
(kept in sync with `dx_rt_fork/release.ver`).

---

## 5. Manual Build (Reference)

To build a single version without the script:

```bash
cd dx_rt_fork/python_package
uv venv --python 3.12 .venvs/py312
source .venvs/py312/bin/activate
pip install --upgrade pip wheel scikit-build-core pybind11
pip wheel . --no-deps --wheel-dir ./wheelhouse
deactivate
```

`pip wheel .` performs the same action as `./make_whl.sh` but lets you
choose the output directory.

---

## 6. Install / Verify

```bash
# Install the freshly built wheel into the matching venv and import it
source .venvs/py312/bin/activate
pip install wheelhouse/dx_engine-<version>-cp312-*.whl
python -c "import dx_engine; print(dx_engine.__version__)"
deactivate
```

At import time `libdxrt.so` is required, so the runtime environment must
also have `libdxrt-bin` installed.

---

## 7. Common Issues

| Symptom | Cause / Fix |
|---|---|
| `Could not find pybind11` | `pybind11` is not installed in the venv's pip. Run `pip install pybind11` and retry. |
| `cannot find -ldxrt` | `libdxrt-bin` is not installed. Install with `sudo apt install libdxrt-bin` or install the `.deb` directly, then run `sudo ldconfig`. |
| `invalid peer certificate` (uv) | Private CA not applied. Export `UV_SYSTEM_CERTS=true`. |
| Repeated temp-dir collisions during wheel build | Run with `--clean` to wipe `.venvs` / `build/` and retry. |
| `undefined symbol` at import time | Mismatch between the `libdxrt` used at build time and at runtime. Align both to the same version. |

---

## 8. Directory Layout

```
python_package/
├── BUILD_WHEELS.md          # this document
├── build_all_wheels.sh      # multi-version build script
├── make_whl.sh              # single-version build using the active python (legacy)
├── pyproject.toml
├── src/dx_engine/...
├── .venvs/                  # per-version venvs (auto-created, recommended in .gitignore)
└── wheelhouse/              # build output (auto-created, recommended in .gitignore)
```
