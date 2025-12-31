#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import numpy as np
import argparse
import os
import time
from dx_engine import InferenceEngine, InferenceOption
from logger import Logger, LogLevel

import threading
import queue
from threading import Thread

q_1 = queue.Queue()
q_2 = queue.Queue()
gLoopCount_1 = 0
gLoopCount_2 = 0

lock = threading.Lock()

def onInferenceCallbackFunc_1(outputs, user_arg):
    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance

    global gLoopCount_1
    logger = Logger()
    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:

        # user data type casting
        index, loop_count = user_arg
    

        # post processing
        #postProcessing(outputs);

        # something to do

        logger.debug(f"Inference_1 output (callback) index={index}")

        gLoopCount_1 += 1
        if ( gLoopCount_1 == loop_count ) :
            logger.debug("Complete Callback")
            q_1.put(0)

    return 0

def onInferenceCallbackFunc_2(outputs, user_arg):
    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance

    global gLoopCount_2
    logger = Logger()
    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:

        # user data type casting
        index, loop_count = user_arg
    

        # post processing
        #postProcessing(outputs);

        # something to do

        logger.debug(f"Inference_2 output (callback) index={index}")

        gLoopCount_2 += 1
        if ( gLoopCount_2 == loop_count ) :
            logger.debug("Complete Callback")
            q_2.put(0)

    return 0

def parse_args():
    parser = argparse.ArgumentParser(description="Run asynchronous model inference with callback")
    parser.add_argument("--model", "-m", type=str, required=True, help="Path to model file (.dxnn)")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Number of inference loops (default: 1)")
    parser.add_argument("--verbose", "-v", action="store_true", default=False, help="Enable debug logging")
    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    
    if args.verbose:
        logger = Logger()
        logger.set_level(LogLevel.DEBUG)
    
    return args


if __name__ == "__main__":
    args = parse_args()
    logger = Logger()
    
    result = -1
    logger.info(f"Start run_async_model test for model: {args.model}")
    # create inference engine instance with model
    
    try:
        ie_1_option = InferenceOption()
        ie_1_option.buffer_count = 6
        logger.info(f"Buffer count for IE 1: {ie_1_option.buffer_count}")
        ie_2_option = InferenceOption()
        ie_2_option.buffer_count = 3
        logger.info(f"Buffer count for IE 2: {ie_2_option.buffer_count}")
        with InferenceEngine(args.model, ie_1_option) as ie_1, InferenceEngine(args.model, ie_2_option) as ie_2:

            # register call back function
            ie_1.register_callback(onInferenceCallbackFunc_1)
            ie_2.register_callback(onInferenceCallbackFunc_2)

            input_1 = [np.zeros(ie_1.get_input_size(), dtype=np.uint8)]
            input_2 = [np.zeros(ie_2.get_input_size(), dtype=np.uint8)]

            start = time.perf_counter()
            # inference loop
            for i in range(args.loops):

                # inference asynchronously, use all npu cores
                # if device-load >= max-load-value, this function will block  
                ie_1.run_async(input_1, user_arg=[i, args.loops])
                ie_2.run_async(input_2, user_arg=[i, args.loops])

                logger.debug(f"Inference start (async) {i}")

            # wait until all callback data processing is completed
            result_1 = q_1.get()
            result_2 = q_2.get()
            
            end = time.perf_counter()
            total_time_ms = (end -start) * 1000
            avg_latency = total_time_ms / args.loops
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

    exit(result)