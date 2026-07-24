#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import numpy as np
import os
import argparse
import time
from dx_engine import InferenceEngine
from logger import Logger, LogLevel


def parse_args():
    parser = argparse.ArgumentParser(description="Run benchmark for model performance measurement")
    parser.add_argument("--model", "-m", type=str, required=True, help="Path to model file (.dxnn)")
    parser.add_argument("--loops", "-l", type=int, default=30, help="Number of benchmark loops (default: 30)")
    parser.add_argument("--verbose", "-v", action="store_true", default=False, 
                        help="Shows NPU Processing Time and Latency")
    parser.add_argument("--warmup-runs", "-w", type=int, default=0, 
                        help="Number of warmup runs before actual measurement (default: 0)")
    parser.add_argument("--no-input", action="store_true", default=False,
                        help="Test API's None handling - do not pass input buffer (API will create internally)")
    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    
    return args


if __name__ == "__main__":
    args = parse_args()
    logger = Logger()
    
    # Set log level based on verbose flag
    if args.verbose:
        logger.set_level(LogLevel.DEBUG)
        
    logger.info(f"Start run_benchmark test for model: {args.model}")
    logger.info(f"Run model target mode : Benchmark Mode")
    logger.info(f"Inference by loops: count={args.loops}")
    
    try:
        # create inference engine instance with model
        with InferenceEngine(args.model) as ie:

            # Prepare input data based on --no-input flag
            if args.no_input:
                logger.info("Testing API's None handling - input_data=None")
                input_data = None
            else:
                # NOTE: np.zeros() uses COW zero pages — all virtual pages share one
                # physical page. PCIe DMA driver's get_user_pages() then sees duplicate
                # physical pages in the SG list and fails with EFAULT.
                # np.empty() + explicit fill forces unique physical page allocation.
                _buf = np.empty(ie.get_input_size(), dtype=np.uint8)
                _buf.fill(0)
                input_data = [_buf]

            # Perform warmup runs if specified
            if args.warmup_runs > 0:
                logger.info(f"Performing {args.warmup_runs} warmup run(s)...")
                for i in range(args.warmup_runs):
                    # For warmup, we need actual buffer (run() doesn't auto-create for None)
                    if input_data is None:
                        _warmup_buf = np.empty(ie.get_input_size(), dtype=np.uint8)
                        _warmup_buf.fill(0)
                        ie.run([_warmup_buf])
                    else:
                        ie.run(input_data)
                logger.info("Warmup completed.")

            # Run benchmark - internally loops num_loops times and returns average latency in ms
            # If input_data is None, run_benchmark will internally create the buffer
            fps = ie.run_benchmark(args.loops, input_data)
            
            # Get performance metrics
            npu_time_mean = ie.get_npu_inference_time_mean() / 1000.0  # Convert to ms
            latency_mean = ie.get_latency_mean() / 1000.0  # Convert to ms
            
            # Print results in run_model.cpp style
            def print_benchmark_result(npu_time_ms, latency_ms, fps_val, loops, verbose):
                lines = []
                
                desc_npu_time = "Actual NPU core computation time for a single request"
                desc_latency = "End-to-end time per request including data transfer and overheads"
                desc_fps = "Overall user-observed inference throughput (inputs/second), reflecting perceived speed"
                
                lines.append(f"* Benchmark Result ({loops} inputs)")
                
                if verbose:
                    lines.append(f"  - NPU Processing Time Average : {npu_time_ms:.3f} ms ({desc_npu_time})")
                    lines.append(f"  - Latency Average             : {latency_ms:.3f} ms ({desc_latency})")
                    lines.append(f"  - FPS                         : {fps_val:.2f} ({desc_fps})")
                else:
                    lines.append(f"  - FPS : {fps_val:.2f}")
                
                max_length = max(len(line) for line in lines)
                print("=" * max_length)
                for line in lines:
                    print(line)
                print("=" * max_length)
            
            print_benchmark_result(npu_time_mean, latency_mean, fps, args.loops, args.verbose)
                
    except Exception as e:
        logger.error(f"Exception: {str(e)}")
        exit(-1)
        
    exit(0)
