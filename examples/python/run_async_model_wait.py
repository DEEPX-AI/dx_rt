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

import queue
import threading

q = queue.Queue()

def inferenceThreadFunc(ie, loopCount):
    logger = Logger()
    count = 0

    while(True):
    
        # pop item from queue 
        jobId = q.get()

        # waiting for the inference to complete by jobId
        # ownership of the outputs is transferred to the user 
        outputs = ie.wait(jobId)

        # post processing
        # postProcessing(outputs);

        # something to do


        logger.debug(f"Inference outputs corresponding to jobId={jobId}, index={count}")

        count += 1
        if ( count >= loopCount ):
            break
   
    return 0


def parse_args():
    parser = argparse.ArgumentParser(description="Run asynchronous model inference with wait")
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
    
    logger.info(f"Start run_async_model_wait test for model: {args.model}")
    
    try:
        # create inference engine instance with model
        with InferenceEngine(args.model) as ie:

            # do not register call back function
            # ie.register_callback(onInferenceCallbackFunc)

            t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, args.loops))

            t1.start()

            input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
            start = time.perf_counter()
            
            # inference loop
            for i in range(args.loops):

                
                # inference asynchronously, use all npu cores
                # if device-load >= max-load-value, this function will block  
                jobId = ie.run_async(input, user_arg=0)

                q.put(jobId)

                logger.debug(f"Inference start (async) {i}")

            t1.join()
            
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

    exit(0)