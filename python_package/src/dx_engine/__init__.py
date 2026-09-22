#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers
# who are supplied with DEEPX NPU (Neural Processing Unit).
# Unauthorized sharing or usage is strictly prohibited by law.
#

import os
import warnings

from dx_engine.version import __version__
from dx_engine.inference_engine import InferenceEngine
from dx_engine.inference_option import InferenceOption
from dx_engine.configuration import Configuration
from dx_engine.device_status import DeviceStatus
from dx_engine.runtime_event_dispatcher import RuntimeEventDispatcher
from dx_engine.profiler import Profiler, JobMetrics, TaskMetrics, NpuDeviceMetrics


def _major_minor(version: str):
    parts = version.strip().lstrip("vV").split(".")
    return tuple(parts[:2])


def _check_runtime_version() -> None:
    """Abort import when the loaded libdxrt.so does not match this package.

    The version is read from the shared library that ``_pydxrt`` is linked
    against, so a stale system-wide ``libdxrt.so`` is detected here instead of
    surfacing later as an obscure runtime failure.
    """
    if os.environ.get("DX_ENGINE_SKIP_VERSION_CHECK", "0") not in ("", "0"):
        return

    try:
        runtime_version = Configuration().get_version()
    except Exception as exc:  # version query must never hide the real import
        warnings.warn(
            f"dx_engine: could not determine the libdxrt runtime version ({exc}); "
            "skipping the version check.",
            RuntimeWarning,
            stacklevel=2,
        )
        return

    if _major_minor(runtime_version) != _major_minor(__version__):
        raise RuntimeError(
            "dx_engine and DX-RT versions do not match.\n"
            f"  - dx_engine (python package) : {__version__}\n"
            f"  - libdxrt.so (runtime)       : {runtime_version}\n"
            "Please install matching versions of dx_engine and libdxrt-bin.\n"
            "You can either:\n"
            f"  - install the dx_engine wheel that matches libdxrt-bin {runtime_version}, or\n"
            f"  - upgrade libdxrt-bin to version {__version__}.\n"
            "To skip this check, set DX_ENGINE_SKIP_VERSION_CHECK=1.\n"
            "Warning: Skipping the version check may cause compatibility issues."
        )


_check_runtime_version()
