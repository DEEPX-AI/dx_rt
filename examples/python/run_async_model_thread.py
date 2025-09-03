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


if __name__ == "__main__":
    logger = Logger()
    
    DEFAULT_LOOP_COUNT = 1
    loop_count = DEFAULT_LOOP_COUNT
    modelPath = ""
    argc = len(sys.argv)
    if ( argc > 1 ) :
        modelPath = sys.argv[1];
        if ( argc > 2 ) :
            loop_count = int(sys.argv[2])
            
        if "--verbose" in sys.argv or "-v" in sys.argv:
            logger.set_level(LogLevel.DEBUG)
            
    else:
        logger.info("[Usage] run_async_model_thread [dxnn-file-path] [loop-count] [--verbose|-v]")
        exit(-1)
    
    logger.info(f"Start run_async_model_thread test for model: {modelPath}")
    result = -1

    try:
        
        # create inference engine instance with model
        with InferenceEngine(modelPath) as ie:

            # register call back function
            ie.register_callback(onInferenceCallbackFunc)
            
            # input
            input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
            start = time.perf_counter()
            
            t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, input, 0, loop_count))
            t2 = threading.Thread(target=inferenceThreadFunc, args=(ie, input, 1, loop_count))
            t3 = threading.Thread(target=inferenceThreadFunc, args=(ie, input, 2, loop_count))

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
        
    exit(result)