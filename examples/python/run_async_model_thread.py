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
from dx_engine import InferenceEngine
from logger import Logger, LogLevel

import threading
import queue

THRAD_COUNT = 3
total_count = 0
q = queue.Queue()

lock = threading.Lock()

def inferenceThreadFunc(ie, input, threadIndex, loopCount):
    logger = Logger()
    # inference loop
    for i in range(loopCount):

        # inference asynchronously, use all npu cores
        # if device-load >= max-load-value, this function will block  
        ie.run_async(input, user_arg = [i, loopCount, threadIndex])

        logger.debug(f"inferenceThreadFunc thread-index={threadIndex}, loop-index={i}")

    return 0

def onInferenceCallbackFunc(outputs, user_arg):
    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance
    logger = Logger()
    global total_count

    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:
        # user data type casting
        index = user_arg[0]
        loop_count = user_arg[1]
        thread_index = user_arg[2]

        # post processing
        #postProcessing(outputs);

        # something to do

        total_count += 1
        logger.debug(f"Inference output (callback) thread-index={thread_index}, index={index}, total-count={total_count}")

        if ( total_count ==  loop_count * THRAD_COUNT) :
            logger.debug("Complete Callback")
            q.put(0)

    return 0


def parse_args():
    parser = argparse.ArgumentParser(description="Run asynchronous model inference with multiple threads")
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
    
    logger.info(f"Start run_async_model_thread test for model: {args.model}")
    result = -1

    try:
        
        # create inference engine instance with model
        with InferenceEngine(args.model) as ie:

            # register call back function
            ie.register_callback(onInferenceCallbackFunc)
            
            # input
            input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
            start = time.perf_counter()
            
            t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, input, 0, args.loops))
            t2 = threading.Thread(target=inferenceThreadFunc, args=(ie, input, 1, args.loops))
            t3 = threading.Thread(target=inferenceThreadFunc, args=(ie, input, 2, args.loops))

            # Start and join
            t1.start()
            t2.start()
            t3.start()


            # join
            t1.join()
            t2.join()
            t3.join()

            # wait until all callback data processing is completed
            result = q.get()
            
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