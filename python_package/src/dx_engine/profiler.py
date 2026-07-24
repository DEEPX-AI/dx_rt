#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers
# who are supplied with DEEPX NPU (Neural Processing Unit).
# Unauthorized sharing or usage is strictly prohibited by law.
#

import dx_engine.capi._pydxrt as C


class NpuDeviceMetrics:
    """Timing breakdown for one NPU device within a task."""

    def __init__(self, c_obj: C.NpuDeviceMetrics):
        self._c = c_obj

    @property
    def input_format_us(self) -> float:
        """NPU input format handler time in microseconds."""
        return self._c.input_format_us

    @property
    def h2d_us(self) -> float:
        """Host-to-Device DMA time in microseconds."""
        return self._c.h2d_us

    @property
    def inference_core_all_us(self) -> float:
        """NPU inference time aggregated across all cores (microseconds)."""
        return self._c.inference_core_all_us

    @property
    def inference_core_0_us(self) -> float:
        """NPU inference time on core 0 (microseconds)."""
        return self._c.inference_core_0_us

    @property
    def inference_core_1_us(self) -> float:
        """NPU inference time on core 1 (microseconds)."""
        return self._c.inference_core_1_us

    @property
    def inference_core_2_us(self) -> float:
        """NPU inference time on core 2 (microseconds)."""
        return self._c.inference_core_2_us

    @property
    def d2h_us(self) -> float:
        """Device-to-Host DMA time in microseconds."""
        return self._c.d2h_us

    @property
    def output_format_us(self) -> float:
        """NPU output format handler time in microseconds."""
        return self._c.output_format_us

    @property
    def total_us(self) -> float:
        """End-to-end NPU task time in microseconds."""
        return self._c.total_us

    @property
    def valid(self) -> bool:
        """True if timing fields are populated from a real measurement."""
        return self._c.valid


class TaskMetrics:
    """Metrics for one registered task (NPU task or CPU task)."""

    def __init__(self, c_obj: C.TaskMetrics):
        self._c = c_obj

    @property
    def task_name(self) -> str:
        return self._c.task_name

    @property
    def devices(self) -> dict:
        """dict[int, NpuDeviceMetrics] — per-device NPU metrics (NPU tasks only)."""
        return {dev_id: NpuDeviceMetrics(d) for dev_id, d in self._c.devices.items()}

    @property
    def cpu_task_us(self) -> float:
        """CPU task execution time in microseconds (non-zero only for CPU tasks)."""
        return self._c.cpu_task_us

    @property
    def valid(self) -> bool:
        return self._c.valid


class JobMetrics:
    """All task metrics for one job."""

    def __init__(self, c_obj: C.JobMetrics):
        self._c = c_obj

    @property
    def tasks(self) -> list:
        """list[TaskMetrics] — one entry per task that participated in the job."""
        return [TaskMetrics(t) for t in self._c.tasks]

    @property
    def valid(self) -> bool:
        return self._c.valid

    def get_task(self, name: str) -> TaskMetrics:
        """Look up a task by name; returns TaskMetrics with valid=False if not found."""
        return TaskMetrics(self._c.get_task(name))


class Profiler:
    """Runtime profiler for per-job inference timing.

    Example::

        profiler = Profiler.get_instance()
        job_id = ie.run_async([input])
        outputs = ie.wait(job_id)
        jm = profiler.get_job_metrics(job_id)
        if jm.valid:
            npu = jm.get_task("npu_0")
            print(npu.devices[0].h2d_us)
    """

    def __init__(self):
        self._c: C.Profiler = C.Profiler.get_instance()

    @staticmethod
    def get_instance() -> "Profiler":
        """Return the singleton Profiler instance."""
        p = Profiler.__new__(Profiler)
        p._c = C.Profiler.get_instance()
        return p

    def show(self) -> None:
        """Print min/max/average/CoV statistics for all recorded events."""
        self._c.show()

    def save(self, filename: str) -> None:
        """Save timing data to a JSON file."""
        self._c.save(filename)

    def clear(self) -> None:
        """Clear all recorded timing data."""
        self._c.clear()

    def get_job_metrics(self, job_id: int) -> JobMetrics:
        """Return JobMetrics for the given job_id; valid=False if not found."""
        return JobMetrics(self._c.get_job_metrics(job_id))

    def start(self, name: str) -> None:
        """Start recording a user profiling event.

        Args:
            name: Event name (e.g. "preprocess", "postprocess").
        """
        self._c.start(name)

    def end(self, name: str) -> None:
        """End recording a user profiling event.

        Args:
            name: Event name matching a previous start() call.
        """
        self._c.end(name)

    def user_clear(self) -> None:
        """Clear all user-recorded profiling events."""
        self._c.user_clear()
