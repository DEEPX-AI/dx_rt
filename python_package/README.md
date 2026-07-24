# dx-engine

Python bindings for the DEEPX Runtime (`dx_rt`). `dx-engine` exposes the
`libdxrt` inference stack to Python via a `pybind11` extension so you can
load DEEPX-compiled models (`.dxnn`), run inference on DEEPX NPUs, inspect
device status, and collect profiling data from Python.

- Package name (PyPI / pip): `dx-engine`
- Import name: `dx_engine`
- Supported Python: 3.8 – 3.14
- Requires: `libdxrt` runtime installed on the system (typically via the
  `libdxrt-bin` Debian package).

## Installation

### From PyPI
```bash
python -m pip install dx-engine
```

### From a prebuilt wheel (recommended)

Wheels are shipped inside the `libdxrt-bin` Debian package under
`/usr/share/libdxrt-bin/python/`. Install the wheel whose `cpXY` tag
matches your Python version:

```bash
# 1. Install the system runtime
sudo dpkg -i libdxrt-bin_<version>_<arch>.deb

# 2. Install the matching wheel into your venv/interpreter
PYTAG=cp$(python -c 'import sys;print(f"{sys.version_info[0]}{sys.version_info[1]}")')
pip install /usr/share/libdxrt-bin/python/dx_engine-*-${PYTAG}-${PYTAG}-*.whl
```

`sudo dpkg -i` automatically installs the wheel that matches the **system**
`python3` interpreter. Any user-owned venv must repeat the `pip install`
step because `dpkg` runs as root and cannot safely write into a venv.

### From source

Build wheels for one or more Python versions using isolated `uv`-managed
venvs. See [BUILD_WHEELS.md](BUILD_WHEELS.md) for the full procedure.

```bash
cd dx_rt/python_package
./build_all_wheels.sh              # 3.8 – 3.14
./build_all_wheels.sh 3.12         # single version
```

Produced wheels are written to `./wheelhouse/`.

## Quick Start

```python
import numpy as np
from dx_engine import InferenceEngine, InferenceOption

# 1. Load a compiled model
option = InferenceOption()
engine = InferenceEngine("model.dxnn", option)

# 2. Prepare inputs matching the model's expected shape/dtype
input_tensor = np.zeros(engine.get_input_shape(), dtype=np.float32)

# 3. Run inference
outputs = engine.run([input_tensor])

# 4. Inspect results
for i, out in enumerate(outputs):
    print(f"output[{i}]: shape={out.shape}, dtype={out.dtype}")
```

## Public API

Re-exported from `dx_engine` (`src/dx_engine/__init__.py`):

| Symbol | Purpose |
|---|---|
| `InferenceEngine` | Load a `.dxnn` model and run synchronous/asynchronous inference |
| `InferenceOption` | Runtime knobs applied when constructing an `InferenceEngine` |
| `Configuration` | Global runtime configuration (device selection, logging, ...) |
| `DeviceStatus` | Query NPU health, temperature, and utilization |
| `RuntimeEventDispatcher` | Subscribe to runtime events emitted by `dx_rt` |
| `Profiler`, `JobMetrics`, `TaskMetrics`, `NpuDeviceMetrics` | Collect and inspect per-job / per-task profiling data |
| `__version__` | Package version (kept in sync with `dx_rt/release.ver`) |

## Runtime Requirements

`dx_engine` links against `libdxrt.so` at import time. If you see
`ImportError: libdxrt.so.<major>: cannot open shared object file`, verify:

```bash
dpkg -l libdxrt-bin | grep ^ii
ldconfig -p | grep libdxrt.so
```

If the library is installed but not in the loader cache, run
`sudo ldconfig` once.

The `libdxrt` build linked at wheel build time and the one present at
runtime must share the same major version. Reinstall the matching wheel
whenever you upgrade `libdxrt-bin`.

## Verify the Installation

```bash
python -c "import dx_engine; print(dx_engine.__version__)"
```

## License

Proprietary — Copyright (C) 2018- DEEPX Ltd. All rights reserved.

This software is provided exclusively to customers supplied with a DEEPX
NPU. Unauthorized sharing or use is prohibited. See [LICENSE](LICENSE)
for the full terms; each wheel embeds the same file under
`dx_engine-<ver>.dist-info/licenses/LICENSE`.
