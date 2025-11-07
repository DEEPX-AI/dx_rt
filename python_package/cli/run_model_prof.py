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
import fcntl
import ctypes
import csv
import tempfile
import json
import io

try:
    from dx_engine import InferenceEngine, InferenceOption, Configuration
except ImportError:

    print(f"[FATAL ERR] dx_engine module or its components are not found. "
          f"Please ensure it is installed correctly and accessible in your PYTHONPATH.", file=sys.stderr)
    raise 

APP_NAME = "DXRT Python run_model_prof"

# Globals for TARGET_FPS_MODE callback synchronization
callback_completed_count = 0
callback_sync_lock = threading.Lock()


def parse_arguments():
    parser = argparse.ArgumentParser(prog="run_model.py", description=APP_NAME, formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("-m", "--model", type=str, required=True, help="Model file (.dxnn)")
    parser.add_argument("-i", "--input", type=str, default="", help="Input data file (optional)")
    parser.add_argument("-o", "--output", type=str, default="output.bin", help="Output data file (default: output.bin)")
    parser.add_argument("-n", "--npu", type=int, default=1,
                        help="NPU bounding option (default: 1 for NPU_0)\n"
                             "  0: NPU_ALL\n  1: NPU_0\n  2: NPU_1\n  3: NPU_2\n"
                             "  4: NPU_0/1\n  5: NPU_1/2\n  6: NPU_0/2")
    parser.add_argument("-l", "--loops", type=int, default=30, help="Number of inference loops to perform (default: 30)")
    parser.add_argument("--use-ort", action="store_true", default=False, help="Enable ONNX Runtime for CPU tasks in the model graph\nIf disabled, only NPU tasks operate")
    parser.add_argument("--csv-out", type=str, default="output.csv", help="Profiler CSV output file (tile,voltage)")
    parser.add_argument("--dev-node", type=str, default="/dev/dxrt0", help="DXRT device node for IOCTL read")
    parser.add_argument("--svg-out", type=str, default="profiler.svg", help="SVG plot output path (default: profiler.svg)")
    parser.add_argument("--density-line", action="store_true", help="Overlay median voltage per tile")
    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    if args.input and not os.path.exists(args.input):
        parser.error(f"Input file '{args.input}' does not exist.")
    
    return args

# ---- IOCTL / ctypes support (minimal) ----
_IOC_WRITE = 1
_IOC_NRSHIFT=0; _IOC_TYPESHIFT=8; _IOC_SIZESHIFT=16; _IOC_DIRSHIFT=30

def _IOW(type_char, nr, size):
    return (_IOC_WRITE << _IOC_DIRSHIFT) | (ord(type_char) << _IOC_TYPESHIFT) | (nr << _IOC_NRSHIFT) | (size << _IOC_SIZESHIFT)

DXRT_CMD_READ_MEM = 8  # enum order in driver.h
# Fixed profiler region constants (no longer configurable via CLI)
PROF_BASE = 0x600000000
PROF_ADDR = 0x04F00000
PROF_SIZE = 0x00100000

class DxrtMessage(ctypes.Structure):
    _fields_ = [
        ("cmd", ctypes.c_int32),
        ("sub_cmd", ctypes.c_int32),
        ("data", ctypes.c_void_p),
        ("size", ctypes.c_uint32),
    ]

class DxrtReqMeminfo(ctypes.Structure):
    _fields_ = [
        ("data", ctypes.c_uint64),
        ("base", ctypes.c_uint64),
        ("offset", ctypes.c_uint32),
        ("size", ctypes.c_uint32),
        ("ch", ctypes.c_uint32),
    ]

class AvcProf(ctypes.Structure):
    _fields_ = [
        ("npuVolt", ctypes.c_uint32 * 3),
        ("npuTile", ctypes.c_uint32 * 3),
        ("loop", ctypes.c_uint32),
        ("done", ctypes.c_uint32),
    ]


def read_profiler_region(dev_node: str, base: int, offset: int, size: int, channel: int = 0):
    """Read raw profiler region via IOCTL into Python and parse avcProf entries.
    Returns (entries:list of dict, entry_count:int, current_idx:int)."""
    # Prepare buffer
    buf = (ctypes.c_ubyte * size)()
    buf_ptr = ctypes.addressof(buf)

    req = DxrtReqMeminfo()
    req.data = buf_ptr
    req.base = base
    req.offset = offset
    req.size = size
    req.ch = channel

    msg = DxrtMessage()
    msg.cmd = DXRT_CMD_READ_MEM
    msg.sub_cmd = 0
    msg.data = ctypes.addressof(req)
    msg.size = ctypes.sizeof(req)

    IOCTL_MSG = _IOW('D', 0, ctypes.sizeof(DxrtMessage))

    with open(dev_node, 'rb', buffering=0) as fd:
        fcntl.ioctl(fd, IOCTL_MSG, msg)

    entry_sz = ctypes.sizeof(AvcProf)
    capacity = size // entry_sz
    entries = []
    AvcProfPtr = ctypes.POINTER(AvcProf)
    for i in range(capacity):
        e = ctypes.cast(buf_ptr + i*entry_sz, AvcProfPtr).contents
        entries.append({
            'voltages': [e.npuVolt[j] for j in range(3)],
            'tiles': [e.npuTile[j] for j in range(3)],
            'loop': e.loop,
            'done': e.done,
            'raw_index': i,
        })
    # heuristic same as C++: first done!=0
    entry_count = 0
    current_idx = 0
    for e in entries:
        if e['done'] != 0:
            entry_count = e['loop']
            current_idx = e['done']
            break
    return entries, entry_count, current_idx

def make_scatter_graph(args, tile_list, volt_list, model_path):
    try:
        import matplotlib.pyplot as plt
        import numpy as np
    except Exception as e:
        print(f"[WARN] matplotlib not available, skip plot ({e})")
        return
    if not tile_list:
        print("[WARN] No data to plot")
        return
    model_name = os.path.basename(model_path)
    orig_tiles = list(reversed(tile_list))
    volts = np.array(list(reversed(volt_list)), dtype=float)

    # Simplified: direct tile values (no normalization, spacing, jitter)
    tiles_numeric = np.array(orig_tiles, dtype=float)
    unique_tiles = sorted(set(orig_tiles))
    x_ticks_src = unique_tiles
    x_ticks_pos = unique_tiles

    # Dynamic figure width
    # Always auto-adjust width (formerly --auto-width)
    WIDTH_PER_TILE = 0.02  # formerly args.width_per_tile
    MAX_WIDTH = 160.0      # formerly args.max_width
    fig_width = min(MAX_WIDTH, max(10.0, len(unique_tiles) * WIDTH_PER_TILE))
    fig, ax = plt.subplots(figsize=(fig_width, 4.5))

    # Fixed scatter defaults (size=10, alpha=0.5)
    SCATTER_SIZE = 10
    SCATTER_ALPHA = 0.5
    sc = ax.scatter(tiles_numeric, volts, s=SCATTER_SIZE, alpha=SCATTER_ALPHA,
                    c=volts, cmap='viridis', linewidths=0)
    fig.colorbar(sc, ax=ax, label='voltage')
    ax.set_title(f'{model_name} - tile/voltage (scatter)')

    # Median line per tile (use underlying unique tile groups)
    if args.density_line:
        from collections import defaultdict
        groups = defaultdict(list)
        for x_val, orig_t, v in zip(tiles_numeric, orig_tiles, volts):
            groups[orig_t].append(v)
        med_tiles = sorted(groups.keys())
        med_x = []
        for t in med_tiles:
            med_x.append(t)
        med_vals = [float(np.median(groups[t])) for t in med_tiles]
        ax.plot(med_x, med_vals, color='red', linewidth=1.2, label='median')
        ax.legend(loc='best')

    ax.set_xlabel('tile')
    ax.set_ylabel('voltage')
    ax.set_ylim(710, 760)  # Fixed vertical axis as requested
    ax.grid(alpha=0.25, linestyle='--', linewidth=0.5)

    # Manage x ticks (avoid overcrowding)
    if len(x_ticks_pos) <= 50:
        ax.set_xticks(x_ticks_pos)
        ax.set_xticklabels(x_ticks_src, rotation=0, fontsize=8)
    else:
        step = int(np.ceil(len(x_ticks_pos)/50))
        sel_pos = x_ticks_pos[::step]
        sel_lbl = x_ticks_src[::step]
        ax.set_xticks(sel_pos)
        ax.set_xticklabels(sel_lbl, rotation=0, fontsize=7)

    fig.tight_layout()
    try:
        # Render once into memory (no metadata injection)
        buf = io.StringIO()
        fig.savefig(buf, format='svg')
        svg_text = buf.getvalue()
        if getattr(args, 'svg_out', ''):
             with open(args.svg_out, 'w', encoding='utf-8') as fsvg:
                 fsvg.write(svg_text)
             print(f"[INFO] Saved SVG plot: {args.svg_out} (width={fig_width})")
    except Exception as e:
        print(f"[WARN] Failed to save plot: {e}")
    plt.close(fig)

def main():
    global current_run_mode, callback_completed_count

    args = parse_arguments()
    print(f"Model file: {args.model}")
    if args.input:
        print(f"Input data file: {args.input}")
        print(f"Output data file: {args.output}")
    print(f"Loops: {args.loops}")

    io = InferenceOption()
    config = Configuration()

    # Build embedded firmware config JSON safely (avoid f-string brace issues)
    EMBEDDED_FWCONFIG_JSON = json.dumps({
        "voltage_monitor_settings": {
            "mode": 3,
            "loop": args.loops
        }
    })
    try:
        # Current API expects a file path; create ephemeral temp file
        with tempfile.NamedTemporaryFile('w', suffix='.json', delete=True) as tf:
            tf.write(EMBEDDED_FWCONFIG_JSON)
            tf.flush()
            config.set_fw_config_with_json(tf.name)
        print('[INFO] Applied embedded firmware JSON config (in-memory)')
    except Exception as e:
        print(f'[WARN] Failed to apply embedded firmware JSON: {e}')

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
    devices_list_for_op = [] 
    io.devices = devices_list_for_op
    
    try: 
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

        measured_fps = ie.run_benchmark(args.loops, input_buf_list)
        print(f"Model : {args.model}")
        print(f"FPS   : {measured_fps}")
        # Enable profiler (must be before run, but ensure here too)
        try:
            config.set_enable(Configuration.ITEM.PROFILER, True)
        except Exception:
            pass
        # After benchmark, read profiler
        try:
            entries, entry_count, current_idx = read_profiler_region(
                args.dev_node, PROF_BASE, PROF_ADDR, PROF_SIZE)
            if entry_count == 0:
                print("[INFO] No profiler entries (entry_count=0)")
            else:
                max_entries = len(entries)
                idx = (current_idx - 1 + max_entries) % max_entries
                with open(args.csv_out, 'w', newline='') as f:
                    w = csv.writer(f)
                    w.writerow(['tile','voltage'])
                    tile_series = []
                    volt_series = []
                    for _ in range(entry_count):
                        e = entries[idx]
                        t = e['tiles'][0]; v = e['voltages'][0]
                        w.writerow([t, v])
                        tile_series.append(t); volt_series.append(v)
                        idx = (idx - 1 + max_entries) % max_entries
                print(f"[INFO] Saved profiler CSV: {args.csv_out}")
                print(f"[INFO] Entries: {entry_count}")
                make_scatter_graph(args, tile_series, volt_series, args.model)
        except FileNotFoundError:
            print(f"[WARN] Device node not found: {args.dev_node} (skipping profiler dump)")
        except PermissionError:
            print(f"[WARN] Permission denied opening {args.dev_node}. Try sudo or adjust udev rules.")
        except OSError as oe:
            print(f"[WARN] IOCTL read failed: {oe}")
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