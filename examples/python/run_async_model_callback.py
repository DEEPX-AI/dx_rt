#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import os
import numpy as np
import argparse
from dx_engine import InferenceEngine
from logger import Logger, LogLevel

import time
import threading
import queue  

callback_cnt = 0
callback_lock = threading.Lock()
result_queue = queue.Queue()  
start_time = 0
end_time = 0
def parse_args():
    parser = argparse.ArgumentParser(description="Inference Engine Arguments")
    parser.add_argument("--model", "-m", type=str, required=True, help="Path to model file (.dxnn)")
    parser.add_argument("--input", "-i", type=str, default="", help="Path to input data file")
    parser.add_argument("--output", "-o", type=str, default="output.bin.pyrt", help="Path to output data file")
    parser.add_argument("--benchmark", "-b", action="store_true", default=False, help="Run benchmark test")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Number of inference loops")
    parser.add_argument("--verbose", "-v", action="store_true", default=False, help="Enable debug logging")
    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    if args.input and not os.path.exists(args.input):
        parser.error(f"Input file '{args.input}' does not exist.")
    if args.verbose:
        logger = Logger()
        logger.set_level(LogLevel.DEBUG)
    return args

def callback_with_args(outputs, user_arg):
    global callback_cnt
    logger = Logger()
    with callback_lock:
        logger.debug(f"Callback triggered for inference with user_arg({user_arg})")
        callback_cnt += 1
        result_queue.get(timeout=5) 
        result_queue.task_done() 
    return 0

if __name__ == "__main__":
    args = parse_args()
    logger = Logger()
    
    logger.info("---------------------------------")
    logger.info(f"Loading model from: {args.model}")

    try:
        
        # Initialize inference engine
        with InferenceEngine(args.model) as ie:
            
            input_dtype = ie.get_input_tensors_info()[0]['dtype']
            output_dtype = ie.get_output_tensors_info()[0]['dtype']
            input_size = ie.get_input_size()
            output_size = ie.get_output_size()

            logger.debug(f"Input data type: {input_dtype}")
            logger.debug(f"Output data type: {output_dtype}")
            logger.debug(f"Input size: {input_size}")
            logger.debug(f"Total output size: {output_size}")

            # Load input data if provided, otherwise use zeros
            if args.input:
                with open(args.input, "rb") as file:
                    input_data = [np.frombuffer(file.read(), dtype=np.uint8)]
            else:
                input_data = [np.zeros(input_size, dtype=np.uint8)]

            # Register callback function
            ie.register_callback(callback_with_args)

            start = time.perf_counter()

            # Run inference for the number of loops specified
            for loop in range(args.loops):
                req_id = ie.run_async(input_data, user_arg=loop)
                logger.debug(f"[{req_id}] Inference request #{req_id} submitted with user_arg({loop})")
                result_queue.put(req_id)

            # Wait for all callbacks to complete
            # Join the queue and wait for all tasks to be done
            result_queue.join()

            end = time.perf_counter()

            total_time_ms = (end - start) * 1000  # Convert to milliseconds
            avg_latency = total_time_ms / args.loops  # Average latency per request
            fps = 1000.0/ avg_latency if avg_latency > 0 else 0.0
            
            logger.info("-----------------------------------")
            logger.info(f"Total Time: {total_time_ms:.3f} ms")
            logger.info(f"Average Latency: {avg_latency:.3f} ms")
            logger.info(f"FPS: {fps:.2f} frame/sec")
            logger.info("Success")
            logger.info("-----------------------------------")
            
    except Exception as e:
        logger.error(f"Exception: {str(e)}")
        exit(-1)
        
    exit(0)
