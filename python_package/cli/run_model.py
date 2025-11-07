#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import warnings
import os
import numpy as np
import argparse
import time
import threading
from enum import Enum
from typing import List, Union, Any, Optional, Dict, Callable
import sys

try:
    from dx_engine import InferenceEngine, InferenceOption
except ImportError:

    print(f"[FATAL ERR] dx_engine module or its components are not found. "
          f"Please ensure it is installed correctly and accessible in your PYTHONPATH.", file=sys.stderr)
    raise 

APP_NAME = "DXRT Python run_model"

class RunModelMode(Enum):
    BENCHMARK_MODE = 0
    SINGLE_MODE = 1
    TARGET_FPS_MODE = 2

    def __str__(self): # For printing the mode name
        if self == RunModelMode.BENCHMARK_MODE: return "Benchmark Mode"
        elif self == RunModelMode.SINGLE_MODE: return "Single Mode"
        elif self == RunModelMode.TARGET_FPS_MODE: return "Target FPS Mode"
        return "Unknown Mode"

current_run_mode: RunModelMode = RunModelMode.BENCHMARK_MODE # Default mode

# Globals for TARGET_FPS_MODE callback synchronization
callback_completed_count = 0
callback_sync_lock = threading.Lock()


def print_inf_result(input_file: str, output_file: str, model_file: str,
                     latency_ms: float, inf_time_ms: float, fps_val: float,
                     loops: int, current_mode: RunModelMode, verbose: bool): # Renamed fps to fps_val, mode to current_mode
    lines = []
    if os.environ.get("DXRT_SHOW_PROFILE")=='1':
        verbose=True
    desc_npu_time = "Actual NPU core computation time for a single request"
    desc_latency = ("End-to-end time per request including data transfer and overheads")
    desc_fps = ("Overall user-observed inference throughput (inputs/second), "
                "reflecting perceived speed")

    description_parenthesis_start_column = 45

    def build_formatted_line(label: str,   
                             value_str: str,   
                             unit_str: str,  
                             description: str,
                             verbose: bool) -> str:
        value_with_unit = value_str + (f" {unit_str}" if unit_str else "")
        core_content = label + value_with_unit

        line = core_content
        current_length = len(core_content)

        spaces_to_add = description_parenthesis_start_column - current_length

        if spaces_to_add <= 0:
            spaces_to_add = 1 
        
        line += ' ' * spaces_to_add
        if verbose:
            line += f"({description})"  
        return line

    inf_time_str = f"{inf_time_ms:.3f}" # 3 decimal places for ms
    latency_str = f"{latency_ms:.3f}" # 3 decimal places for ms
    fps_str = f"{fps_val:.2f}"        # 2 decimal places for FPS

    # Disable until DX_SIM is supported
    if input_file:
        lines.append(f"* Processing File : {input_file}")
        lines.append(f"* Output Saved As : {output_file}")

    if current_mode == RunModelMode.SINGLE_MODE:
        lines.append("* Benchmark Result (single input)")
        if verbose:
            lines.append(build_formatted_line("  - NPU Processing Time  : ", inf_time_str, "ms", desc_npu_time, True))
            lines.append(build_formatted_line("  - Latency              : ", latency_str, "ms", desc_latency, True))
            lines.append(build_formatted_line("  - FPS                  : ", fps_str,     "",   desc_fps, True))
        else:
            lines.append(build_formatted_line("  - FPS : ", fps_str,     "",   desc_fps, False))
    else:
        lines.append(f"* Benchmark Result ({loops} inputs)")
        if verbose:
            lines.append(build_formatted_line("  - NPU Processing Time Average : ", inf_time_str, "ms", desc_npu_time, True))
            lines.append(build_formatted_line("  - Latency Average             : ", latency_str, "ms", desc_latency, True))
            lines.append(build_formatted_line("  - FPS                         : ", fps_str,     "",   desc_fps, True))
        else:
            lines.append(build_formatted_line("  - FPS : ", fps_str,     "",   desc_fps, False))

    max_line_len = 0
    for line_item in lines:
        max_line_len = max(max_line_len, len(line_item))

    print(f"\n{('=' * max_line_len)}")
    for line_item in lines:
        print(line_item)
    print(f"{('=' * max_line_len)}")


def set_run_model_mode(args_single: bool, args_target_fps: int):
    global current_run_mode
    if args_single:
        current_run_mode = RunModelMode.SINGLE_MODE
    elif args_target_fps > 0:
        current_run_mode = RunModelMode.TARGET_FPS_MODE
    else:
        current_run_mode = RunModelMode.BENCHMARK_MODE
    print(f"Run model target mode : {current_run_mode}")


def parse_arguments():
    parser = argparse.ArgumentParser(prog="run_model.py", description=APP_NAME, formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("-m", "--model", type=str, required=True, help="Model file (.dxnn)")
    parser.add_argument("-i", "--input", type=str, default="", help=argparse.SUPPRESS)
    parser.add_argument("-o", "--output", type=str, default="output.bin", help=argparse.SUPPRESS)
    parser.add_argument("-b", "--benchmark", action="store_true", default=False, help="Perform a benchmark test (Maximum throughput)\n(This is the default mode,\n if --single or --fps > 0 are not specified)")
    parser.add_argument("-s", "--single", action="store_true", default=False, help="Perform a single run test\n(Sequential single-input inference on a single-core)")
    parser.add_argument("-v", "--verbose", action="store_true", default=False, help="Shows NPU Processing Time and Latency")
    parser.add_argument("-n", "--npu", type=int, default=0,
                        help="NPU bounding option (default: 0 for NPU_ALL)\n"
                             "  0: NPU_ALL\n  1: NPU_0\n  2: NPU_1\n  3: NPU_2\n"
                             "  4: NPU_0/1\n  5: NPU_1/2\n  6: NPU_0/2")
    parser.add_argument("-l", "--loops", type=int, default=30, help="Number of inference loops to perform (default: 30)")
    parser.add_argument("-w", "--warmup-runs", type=int, default=0, help="Number of warmup runs before actual measurement (default: 0)")
    parser.add_argument("-d", "--devices", type=str, default="all",
                        help="Specify target NPU devices (default: 'all')\nExamples:\n"
                             "  'all': Use all available/bound NPUs\n"
                             "  '0': Use NPU0 only\n"
                             "  '0,1,2': Use NPU0, NPU1, and NPU2\n"
                             "  'count:N': Use the first N NPUs\n  (e.g., 'count:2' for NPU0, NPU1)")
    parser.add_argument("-f", "--fps", type=int, default=0, help="Target FPS for TARGET_FPS_MODE\n(enables this mode if > 0 and --single is not set, default: 0)")
    parser.add_argument("--skip-io", action="store_true", default=False, help="Attempt to skip Inference I/O (Benchmark mode only)")
    parser.add_argument("--use-ort", action="store_true", default=False, help="Enable ONNX Runtime for CPU tasks in the model graph\nIf disabled, only NPU tasks operate")
    
    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    # Disable until DX_SIM is supported
    if args.input and not os.path.exists(args.input):
        parser.error(f"Input file '{args.input}' does not exist.")
    
    return args

def main():
    global current_run_mode, callback_completed_count

    args = parse_arguments()
    set_run_model_mode(args.single, args.fps) 

    if args.skip_io and current_run_mode != RunModelMode.BENCHMARK_MODE:
        print("[ERR] --skip-io option is only available in BENCHMARK_MODE.", file=sys.stderr)
        sys.exit(-1)
    
    if args.skip_io:
        print("[WARN] --skip-io is set. Actual I/O skipping depends on backend support via Python API.", file=sys.stderr)

    print(f"Model file: {args.model}")
    # Disable until DX_SIM is supported
    if args.input:
        print(f"Input data file: {args.input}")
        print(f"Output data file: {args.output}")
    print(f"Loops: {args.loops}")

    io = InferenceOption()
    # Respect build capability for ORT: if runtime does not support ORT, force-disable
    try:
        import dx_engine.capi._pydxrt as C
        if hasattr(C, 'is_ort_supported') and not C.is_ort_supported() and args.use_ort:
            print("[WARN] USE_ORT is disabled in this build. Ignoring --use-ort flag.", file=sys.stderr)
            io.use_ort = False
        else:
            io.use_ort = args.use_ort
    except Exception:
        # Fallback: set as requested; C++ layer will guard if needed
        io.use_ort = args.use_ort

    devices_list_for_op: List[int] = []
    devices_spec_str = args.devices.strip().lower()

    if devices_spec_str == "all" or not devices_spec_str:
        devices_list_for_op = [] 
        print("Device specification: 'all' (engine default)")
    elif devices_spec_str.startswith("count:"):
        try:
            count_str = devices_spec_str.split(":", 1)[1]
            count = int(count_str)
            if count > 0:
                devices_list_for_op = list(range(count))
                print(f"Device specification: First {count} NPU(s) {devices_list_for_op}")
            else:
                print(f"[ERR] Device count in '{args.devices}' must be positive.", file=sys.stderr)
                sys.exit(-1)
        except (IndexError, ValueError):
            print(f"[ERR] Invalid format for 'count:N' in --devices '{args.devices}'. Expected e.g., 'count:2'.", file=sys.stderr)
            sys.exit(-1)
    else: 
        try:
            devices_list_for_op = [int(x.strip()) for x in devices_spec_str.split(',') if x.strip()]
            if not devices_list_for_op and devices_spec_str: 
                 print(f"[WARN] No valid device IDs parsed from --devices string: '{args.devices}'. Using engine default for devices.", file=sys.stderr)
                 devices_list_for_op = []
            elif devices_list_for_op:
                print(f"Device specification: Specific NPU(s) {devices_list_for_op}")
        except ValueError:
            print(f"[ERR] Invalid device ID in --devices list '{args.devices}'. Expected comma-separated integers e.g., '0,1'.", file=sys.stderr)
            sys.exit(-1)
    io.devices = devices_list_for_op
    
    try: 
        if not hasattr(InferenceOption, 'BOUND_OPTION') or not hasattr(InferenceOption.BOUND_OPTION, 'NPU_ALL'):
             raise AttributeError("InferenceOption.BOUND_OPTION enum not found or incomplete in dx_engine module.")
        bound_enum_member = InferenceOption.BOUND_OPTION(args.npu)
        io.bound_option = bound_enum_member
    except ValueError:
        max_val_info = ""
        if hasattr(InferenceOption, 'BOUND_OPTION') and hasattr(InferenceOption.BOUND_OPTION, 'N_BOUND_INF_MAX'): # Check if N_BOUND_INF_MAX exists
            max_val_info = f" (max value: {InferenceOption.BOUND_OPTION.N_BOUND_INF_MAX.value -1})"
        print(f"[ERR] Invalid NPU bounding option: {args.npu}. Please use a valid enum value{max_val_info}.", file=sys.stderr)
        sys.exit(-1)
    except AttributeError as e:
        print(f"[ERR] Could not set NPU bounding option due to API issue: {e}", file=sys.stderr)
        sys.exit(-1)

    try:
        ie = InferenceEngine(args.model, io)
        if hasattr(ie, 'loops_for_mean') and args.loops > 0 : ie.loops_for_mean = args.loops # For mock class

        input_buf_list: List[np.ndarray]
        # Disable until DX_SIM is supported
        if args.input:
            expected_total_size = ie.get_input_size() 
            file_size = os.path.getsize(args.input)
            if expected_total_size != file_size:
                print(f"[ERR] Input file size mismatch. Expected {expected_total_size}, got {file_size}.", file=sys.stderr)
                sys.exit(-1)
            with open(args.input, "rb") as f:
                input_buf_list = [np.frombuffer(f.read(), dtype=np.uint8)]
        else:
            input_buf_list = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
        input_buf_list = [np.zeros(ie.get_input_size(), dtype=np.uint8)]


        # Perform warmup runs if specified
        if args.warmup_runs > 0:
            print(f"Performing {args.warmup_runs} warmup run(s)...")
            for i in range(args.warmup_runs):
                ie.run(input_buf_list)
            print("Warmup completed.")

        if current_run_mode == RunModelMode.SINGLE_MODE:
            total_latency_us_accumulator = 0.0
            total_npu_time_us_accumulator = 0.0
            total_wall_time_us_accumulator = 0.0

            for i in range(args.loops):
                start_time_wall = time.perf_counter()
                outputs = ie.run(input_buf_list)
                end_time_wall = time.perf_counter()

                loop_wall_time_us = (end_time_wall - start_time_wall) * 1_000_000
                total_wall_time_us_accumulator += loop_wall_time_us
                
                current_latency_us = float(ie.get_latency())
                current_npu_time_us = float(ie.get_npu_inference_time())
                
                total_latency_us_accumulator += current_latency_us
                total_npu_time_us_accumulator += current_npu_time_us
                loop_fps = 1_000_000.0 / loop_wall_time_us if loop_wall_time_us > 0 else 0

                # Disable until DX_SIM is supported
                if args.input and not args.skip_io:
                    with open(args.output, "wb") as f: 
                        f.write(outputs[0].tobytes())
                
                print_inf_result(args.input, args.output, args.model,
                                 current_latency_us / 1000.0,
                                 current_npu_time_us / 1000.0,
                                 loop_fps,
                                 1, 
                                 current_run_mode, args.verbose)
                print_inf_result("", "output.bin", args.model,
                                 current_latency_us / 1000.0,
                                 current_npu_time_us / 1000.0,
                                 loop_fps,
                                 1, 
                                 current_run_mode, args.verbose)
            if args.loops > 1:
                 avg_latency_ms = (total_latency_us_accumulator / args.loops) / 1000.0
                 avg_npu_ms = (total_npu_time_us_accumulator / args.loops) / 1000.0
                 avg_fps = (args.loops * 1_000_000.0) / total_wall_time_us_accumulator if total_wall_time_us_accumulator > 0 else 0
                 avg_summary_title = f"--- Average over {args.loops} loops (Single Mode) ---"
                 print(f"\n{avg_summary_title}")
                 print(f"  - Avg NPU Processing Time: {avg_npu_ms:.3f} ms")
                 print(f"  - Avg Latency          : {avg_latency_ms:.3f} ms")
                 print(f"  - Avg FPS              : {avg_fps:.2f}")
                 print("=" * len(avg_summary_title))


        elif current_run_mode == RunModelMode.TARGET_FPS_MODE:
            callback_completed_count = 0 
            job_ids: List[int] = []

            def py_post_proc_callback(outputs: List[np.ndarray], user_arg: Any):
                global callback_completed_count
                with callback_sync_lock:
                    callback_completed_count += 1
                return 0 

            ie.register_callback(py_post_proc_callback)
            overall_start_time = time.perf_counter()

            for i in range(args.loops):
                job_id = ie.run_async(input_buf_list, user_arg=i)
                job_ids.append(job_id)

                if args.fps > 0: 
                    target_time_per_frame_s = 1.0 / args.fps
                    expected_elapsed_s = (i + 1) * target_time_per_frame_s
                    actual_elapsed_s = time.perf_counter() - overall_start_time
                    
                    if actual_elapsed_s < expected_elapsed_s:
                        sleep_duration_s = expected_elapsed_s - actual_elapsed_s
                        if sleep_duration_s > 0:
                             time.sleep(sleep_duration_s)
            
            print(f"All {args.loops} async tasks submitted. Waiting for completions...")
            start_wait_time = time.perf_counter()
            max_wait_seconds = 60 + (args.loops * (1.0 / args.fps if args.fps > 0 and args.fps !=0 else 1.0)) 

            while True:
                with callback_sync_lock:
                    if callback_completed_count >= args.loops:
                        break
                if time.perf_counter() - start_wait_time > max_wait_seconds:
                    print(f"[WARN] Timeout waiting for all callbacks. Completed: {callback_completed_count}/{args.loops}", file=sys.stderr)
                    break
                time.sleep(0.001) 

            overall_end_time = time.perf_counter()
            total_wall_time_us = (overall_end_time - overall_start_time) * 1_000_000
            actual_fps = (args.loops * 1_000_000.0) / total_wall_time_us if total_wall_time_us > 0 else 0
            
            latency_mean_ms = ie.get_latency_mean() / 1000.0
            npu_time_mean_ms = ie.get_npu_inference_time_mean() / 1000.0

            # Disable until DX_SIM is supported
            print_inf_result(args.input, args.output, args.model,
                             latency_mean_ms, npu_time_mean_ms, actual_fps,
                             args.loops, current_run_mode, args.verbose)

            print_inf_result("", "output.bin", args.model,
                             latency_mean_ms, npu_time_mean_ms, actual_fps,
                             args.loops, current_run_mode, args.verbose)

        elif current_run_mode == RunModelMode.BENCHMARK_MODE:
            measured_fps = ie.run_benchmark(args.loops, input_buf_list)
            
            # Disable until DX_SIM is supported
            if args.input and not args.skip_io: 
                outputs = ie.run(input_buf_list)
                with open(args.output, "wb") as f:
                    f.write(outputs[0].tobytes())
            
            latency_mean_ms = ie.get_latency_mean() / 1000.0
            npu_time_mean_ms = ie.get_npu_inference_time_mean() / 1000.0

            # Disable until DX_SIM is supported
            print_inf_result(args.input, args.output, args.model,
                             latency_mean_ms, npu_time_mean_ms, measured_fps,
                             args.loops, current_run_mode, args.verbose)

            print_inf_result("", "output.bin", args.model,
                             latency_mean_ms, npu_time_mean_ms, measured_fps,
                             args.loops, current_run_mode, args.verbose)
        else:
            print(f"[ERR] Unknown run model mode: {current_run_mode}", file=sys.stderr)
            sys.exit(-1)

    except ImportError:
        print("[FATAL ERR] dx_engine module was not imported successfully. Please check installation.", file=sys.stderr)
        sys.exit(-1)
    except RuntimeError as e: 
        print(f"[ERR] DXRT Runtime Error: {e}", file=sys.stderr)
        sys.exit(-1)
    except Exception as e:
        print(f"[ERR] An unexpected error occurred: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        sys.exit(-1)

if __name__ == "__main__":
    main()