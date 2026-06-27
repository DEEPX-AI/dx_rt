#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers
# who are supplied with DEEPX NPU (Neural Processing Unit).
# Unauthorized sharing or usage is strictly prohibited by law.
#

"""Async inference with per-job profiling using the Wait pattern.

Follows the run_async_model_wait flow:
  Main thread  : run_async() × N  →  push job_id to queue
  Worker thread: pop job_id → ie.wait(job_id) → profiler.get_job_metrics(job_id) → print

This gives accurate per-job H2D / NPU / D2H / Task timings for each job.

Usage:
    python run_async_model_profiler.py -m <model.dxnn> [-l <loops>] [--use-ort]
"""

import argparse
import os
import queue
import threading
import time

import numpy as np
from dx_engine import Configuration, InferenceEngine, InferenceOption, Profiler

# ---------------------------------------------------------------------------
# Job-ID queue shared between main thread and worker thread
# ---------------------------------------------------------------------------
g_job_id_queue: queue.Queue = queue.Queue(maxsize=32)


# ---------------------------------------------------------------------------
# Per-job metric printing
# ---------------------------------------------------------------------------

def print_job_metrics(job_idx: int, job_id: int, jm) -> None:
    if not jm.valid:
        print(f"[Job #{job_idx} id={job_id}] No profiler data")
        return

    for task in jm.tasks:
        parts = [f"[Job #{job_idx} id={job_id}] task={task.task_name}"]

        for dev_id, d in task.devices.items():
            parts.append(
                f"  |  Dev{dev_id}:"
                f" InputNFH={d.input_format_us:.3f}us"
                f" H2D={d.h2d_us:.3f}us"
                f" Inference={d.inference_core_all_us:.3f}us"
                f" (Core0={d.inference_core_0_us:.3f}"
                f" Core1={d.inference_core_1_us:.3f}"
                f" Core2={d.inference_core_2_us:.3f})"
                f" D2H={d.d2h_us:.3f}us"
                f" OutputNFH={d.output_format_us:.3f}us"
                f" NPU Task={d.total_us:.3f}us"
            )

        if task.cpu_task_us > 0.0:
            parts.append(f"  |  CPU Task={task.cpu_task_us:.3f}us")

        print("".join(parts))


# ---------------------------------------------------------------------------
# Worker thread: Wait for each job and print profiling metrics
# ---------------------------------------------------------------------------

def wait_thread_func(ie: InferenceEngine, profiler: Profiler, loop_count: int) -> None:
    for idx in range(loop_count):
        job_id = g_job_id_queue.get()

        try:
            outputs = ie.wait(job_id)
            _ = outputs
        except Exception as e:
            print(f"[Worker] Exception: {e}")
            return

        jm = profiler.get_job_metrics(job_id)
        print_job_metrics(idx, job_id, jm)


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Async inference with per-job profiling (Wait + get_job_metrics)"
    )
    parser.add_argument("--model", "-m", type=str, required=True,
                        help="Path to model file (.dxnn)")
    parser.add_argument("--loops", "-l", type=int, default=10,
                        help="Number of inference loops (default: 10)")
    parser.add_argument("--use-ort", action="store_true",
                        help="Enable ORT (CPU post-processing) for the model")
    parser.add_argument("--verbose", "-v", action="store_true",
                        help="Enable verbose output")
    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")

    return args


if __name__ == "__main__":
    args = parse_args()

    print(f"Model  : {args.model}")
    print(f"Loops  : {args.loops}")
    print(f"UseORT : {args.use_ort}")

    # ── Enable profiler ───────────────────────────────────────────────────
    config = Configuration()
    config.set_enable(Configuration.ITEM.PROFILER, True)
    config.set_attribute(Configuration.ITEM.PROFILER,
                         Configuration.ATTRIBUTE.PROFILER_SHOW_DATA, "ON")
    config.set_attribute(Configuration.ITEM.PROFILER,
                         Configuration.ATTRIBUTE.PROFILER_SAVE_DATA, "OFF")

    profiler = Profiler.get_instance()

    # ── Create InferenceEngine ────────────────────────────────────────────
    opt = InferenceOption()
    opt.use_ort = args.use_ort
    with InferenceEngine(args.model, opt) as ie:

        # ── Start worker thread ───────────────────────────────────────────
        worker = threading.Thread(
            target=wait_thread_func,
            args=(ie, profiler, args.loops),
            daemon=True,
        )
        worker.start()

        # ── Submit all jobs ───────────────────────────────────────────────
        # NOTE: np.zeros() uses COW zero pages — all virtual pages share one
        # physical page. PCIe DMA driver's get_user_pages() then sees duplicate
        # physical pages in the SG list and fails with EFAULT.
        # np.empty() + explicit fill forces unique physical page allocation.
        buf = np.empty(ie.get_input_size(), dtype=np.uint8)
        buf.fill(0)

        start = time.perf_counter()

        for i in range(args.loops):
            job_id = ie.run_async([buf])
            g_job_id_queue.put(job_id)
            if args.verbose:
                print(f"Submitted job_id={job_id}")

        worker.join()

        end = time.perf_counter()

    total_ms = (end - start) * 1000.0
    avg_ms   = total_ms / args.loops
    fps      = 1000.0 / avg_ms

    print("-----------------------------------")
    print(f"Total   : {total_ms:.2f} ms")
    print(f"Average : {avg_ms:.2f} ms/job")
    print(f"FPS     : {fps:.2f}")
    print("-----------------------------------")



