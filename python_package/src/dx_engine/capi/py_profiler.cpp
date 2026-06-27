/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 *
 * This file uses pybind11 (BSD 3-Clause License) - Copyright (c) 2016 Wenzel Jakob.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "dxrt/dxrt_cxx_api.h"

namespace dxrt
{
namespace py = pybind11;

void init_profiler(py::module_& m)
{
    // ── NpuDeviceMetrics ─────────────────────────────────────────────────────
    py::class_<NpuDeviceMetrics>(m, "NpuDeviceMetrics")
        .def_readonly("input_format_us", &NpuDeviceMetrics::input_format_us,
                      "NPU input format handler time (us)")
        .def_readonly("h2d_us",       &NpuDeviceMetrics::h2d_us,
                      "Host-to-Device DMA time (us)")
        .def_readonly("inference_core_all_us", &NpuDeviceMetrics::inference_core_all_us,
                      "NPU inference time aggregated across all cores (us)")
        .def_readonly("inference_core_0_us", &NpuDeviceMetrics::inference_core_0_us,
                      "NPU inference time on core 0 (us)")
        .def_readonly("inference_core_1_us", &NpuDeviceMetrics::inference_core_1_us,
                      "NPU inference time on core 1 (us)")
        .def_readonly("inference_core_2_us", &NpuDeviceMetrics::inference_core_2_us,
                      "NPU inference time on core 2 (us)")
        .def_readonly("d2h_us",       &NpuDeviceMetrics::d2h_us,
                      "Device-to-Host DMA time (us)")
        .def_readonly("output_format_us", &NpuDeviceMetrics::output_format_us,
                      "NPU output format handler time (us)")
        .def_readonly("total_us",     &NpuDeviceMetrics::total_us,
                      "End-to-end NPU task time (us)")
        .def_readonly("valid",        &NpuDeviceMetrics::valid,
                      "True if timing fields are populated from a real measurement");

    // ── TaskMetrics ──────────────────────────────────────────────────────────
    py::class_<TaskMetrics>(m, "TaskMetrics")
        .def_readonly("task_name",   &TaskMetrics::task_name,
                      "Task name (e.g. 'npu_0', 'cpu_0')")
        .def_readonly("devices",     &TaskMetrics::devices,
                      "dict[int, NpuDeviceMetrics] — per-device NPU metrics (NPU tasks only)")
        .def_readonly("cpu_task_us", &TaskMetrics::cpu_task_us,
                      "CPU task execution time in us (non-zero only for CPU tasks)")
        .def_readonly("valid",       &TaskMetrics::valid,
                      "True if this task had profiling data for the requested job");

    // ── JobMetrics ───────────────────────────────────────────────────────────
    py::class_<JobMetrics>(m, "JobMetrics")
        .def_readonly("tasks", &JobMetrics::tasks,
                      "list[TaskMetrics] — one entry per task that participated in the job")
        .def_readonly("valid", &JobMetrics::valid,
                      "True if at least one task had data for this job")
        .def("get_task", &JobMetrics::GetTask, py::arg("name"),
             "Look up a task by name; returns TaskMetrics with valid=False if not found");

    // ── Profiler ─────────────────────────────────────────────────────────────
    py::class_<Profiler>(m, "Profiler")
        .def_static("get_instance", &Profiler::GetInstance,
                    py::return_value_policy::reference,
                    "Returns the singleton Profiler instance")
        .def("show",  &Profiler::Show,
             "Print min/max/average/CoV statistics for all recorded events")
        .def("save",  &Profiler::Save, py::arg("filename"),
             "Save timing data to a JSON file")
        .def("clear", &Profiler::Clear,
             "Clear all recorded timing data")
        .def("get_job_metrics", &Profiler::GetJobMetrics, py::arg("job_id"),
             "Return JobMetrics for the given job_id; valid=False if not found")
        .def("start", &Profiler::Start, py::arg("name"),
             "Start recording a user profiling event")
        .def("end", &Profiler::End, py::arg("name"),
             "End recording a user profiling event")
        .def("user_clear", &Profiler::UserClear,
             "Clear all user-recorded profiling events");
}

} // namespace dxrt
