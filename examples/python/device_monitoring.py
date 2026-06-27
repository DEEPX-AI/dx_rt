#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers
# who are supplied with DEEPX NPU (Neural Processing Unit).
# Unauthorized sharing or usage is strictly prohibited by law.
#

"""
Device Status Example

Demonstrates the DeviceStatus API to retrieve all device
information (spec, status, utilization, memory usage).
Runs inference while monitoring the device in a background thread.

Usage:
    python device_monitoring.py --model <model_path> [--loops N] [--poll-ms MS]
    python device_monitoring.py --no-inference
"""

import argparse
import os
import sys
import threading
import time

import numpy as np
from dx_engine import InferenceEngine
from dx_engine.device_status import DeviceStatus


def print_device_status(ds):
    """Print all fields from a DeviceStatus object."""
    print(f"=== Device Status (id={ds.get_id()}) ===")

    print(f"[STATUS] temp=["
          f"{ds.get_temperature(0)}, "
          f"{ds.get_temperature(1)}, "
          f"{ds.get_temperature(2)}]C, "
          f"voltage=["
          f"{ds.get_npu_voltage(0)}, "
          f"{ds.get_npu_voltage(1)}, "
          f"{ds.get_npu_voltage(2)}]mV, "
          f"clock=["
          f"{ds.get_npu_clock(0)}, "
          f"{ds.get_npu_clock(1)}, "
          f"{ds.get_npu_clock(2)}]MHz")

    print(f"[USAGE] util=["
          f"{ds.get_core_utilization(0):.1f}%, "
          f"{ds.get_core_utilization(1):.1f}%, "
          f"{ds.get_core_utilization(2):.1f}%], "
          f"mem_used={ds.get_memory_used()}, "
          f"mem_free={ds.get_memory_free()}")


def monitor_thread(device_count, poll_ms, stop_event):
    """Background thread that periodically queries device status."""
    while not stop_event.is_set():
        for device_id in range(device_count):
            try:
                ds = DeviceStatus.get_current_status(device_id)
                print_device_status(ds)
            except Exception as e:
                print(f"[Monitor] Error for device {device_id}: {e}")
        time.sleep(poll_ms / 1000.0)
    print("[Monitor] Stopped.")


def parse_args():
    parser = argparse.ArgumentParser(description="Device Status API example")
    parser.add_argument("--model", "-m", type=str, help="Path to model file (.dxnn)")
    parser.add_argument("--loops", "-l", type=int, default=5, help="Number of inference loops (default: 5)")
    parser.add_argument("--poll-ms", "-p", type=int, default=200, help="Status poll interval in ms (default: 200)")
    parser.add_argument("--no-inference", action="store_true", help="Only query status, no inference")
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()

    device_count = DeviceStatus.get_device_count()

    # --- No-inference mode ---
    if args.no_inference:
        print("Querying device status (no inference)...")
        for device_id in range(device_count):
            try:
                ds = DeviceStatus.get_current_status(device_id)
                print_device_status(ds)
            except Exception as e:
                print(f"Error: {e}", file=sys.stderr)
                sys.exit(1)
        sys.exit(0)

    # --- Inference + monitoring mode ---
    if not args.model or not os.path.exists(args.model):
        print(f"Error: Model path '{args.model}' does not exist.", file=sys.stderr)
        sys.exit(1)

    print("Starting inference with device monitoring")
    print(f"  Model: {args.model}")
    print(f"  Loops: {args.loops}")
    print(f"  Poll interval: {args.poll_ms} ms")
    print()

    # Start monitoring thread
    stop_event = threading.Event()
    monitor = threading.Thread(target=monitor_thread, args=(device_count, args.poll_ms, stop_event))
    monitor.start()

    # Run benchmark
    # NOTE: np.zeros() uses COW zero pages — all virtual pages share one
    # physical page. PCIe DMA driver's get_user_pages() then sees duplicate
    # physical pages in the SG list and fails with EFAULT.
    # np.empty() + explicit fill forces unique physical page allocation.
    with InferenceEngine(args.model) as ie:
        _buf = np.empty(ie.get_input_size(), dtype=np.uint8)
        _buf.fill(0)
        input_data = [_buf]
        fps = ie.run_benchmark(args.loops, input_data)

    # Stop monitor
    stop_event.set()
    monitor.join()

    print()
    print(f"=== Done: RunBenchmark {args.loops} loops, average {fps} FPS ===")
    sys.exit(0)
