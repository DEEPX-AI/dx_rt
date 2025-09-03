#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import numpy as np
import sys
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


if __name__ == "__main__":
    logger = Logger()
    
    DEFAULT_LOOP_COUNT = 1
    loop_count = DEFAULT_LOOP_COUNT
    modelPath = ""
    argc = len(sys.argv)
    if ( argc > 1 ) :
        modelPath = sys.argv[1]
        if ( argc > 2 ) :
            loop_count = int(sys.argv[2])
            
        if "--verbose" in sys.argv or "-v" in sys.argv:
            logger.set_level(LogLevel.DEBUG)
            
    else:
        logger.info("[Usage] run_async_model_wait [dxnn-file-path] [loop-count] [--verbose|-v]")
        exit(-1)
    
    logger.info(f"Start run_async_model_wait test for model: {modelPath}")
    
    try:
        # create inference engine instance with model
        with InferenceEngine(modelPath) as ie:

            # do not register call back function
            # ie.register_callback(onInferenceCallbackFunc)

            t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, loop_count))

            t1.start()

            input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
            start = time.perf_counter()
            
            # inference loop
            for i in range(loop_count):

                
                # inference asynchronously, use all npu cores
                # if device-load >= max-load-value, this function will block  
                jobId = ie.run_async(input, user_arg=0)

                q.put(jobId)

                logger.debug(f"Inference start (async) {i}")

            t1.join()
            
            end = time.perf_counter()
            total_time_ms = (end -start) * 1000
            avg_latency = total_time_ms / loop_count
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