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
        logger.info("[Usage] run_sync_model [dxnn-file-path] [loop-count] [--verbose|-v]")
        exit(-1)
        
    logger.info(f"Start run_sync_model test for model: {modelPath}")
    
    try:
        # create inference engine instance with model
        with InferenceEngine(modelPath) as ie:

            input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]

            start = time.perf_counter()
            # inference loop
            for i in range(loop_count):

                # inference synchronously 
                # use only one npu core 
                # ownership of the outputs is transferred to the user 
                outputs = ie.run(input)

                # post processing 
                #postProcessing(outputs)
                logger.debug(f"Inference outputs {i}")
                
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