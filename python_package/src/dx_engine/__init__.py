#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers
# who are supplied with DEEPX NPU (Neural Processing Unit).
# Unauthorized sharing or usage is strictly prohibited by law.
#

# =============================================================================
# Pre-import matplotlib before loading _pydxrt
# =============================================================================
# pybind11's C API level PyImport_ImportModule() during _pydxrt loading
# corrupts partially-initialized Python modules (like matplotlib.ft2font with
# multi-phase init). By pre-importing matplotlib before _pydxrt loads, we
# ensure matplotlib is fully initialized and protected from corruption.
#
# This is safe: matplotlib is optional for dx_engine, and pre-importing has
# no side effects if not actually used by client code.
# =============================================================================
try:
    import matplotlib  # noqa: F401
    import matplotlib.pyplot  # noqa: F401
except Exception:
    # matplotlib not available, but that's okay - it's optional for dx_engine
    pass

from dx_engine.version import __version__
from dx_engine.inference_engine import InferenceEngine
from dx_engine.inference_option import InferenceOption
from dx_engine.configuration import Configuration
from dx_engine.device_status import DeviceStatus
from dx_engine.runtime_event_dispatcher import RuntimeEventDispatcher
