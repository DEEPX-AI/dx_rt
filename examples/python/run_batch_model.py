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
from dx_engine import InferenceOption
from logger import Logger, LogLevel

if __name__ == "__main__":
    logger = Logger()
    
    DEFAULT_LOOP_COUNT = 1
    DEFAULT_BATCH_COUNT = 1
    
    loop_count = DEFAULT_LOOP_COUNT
    batch_count = DEFAULT_BATCH_COUNT
    modelPath = ""
    argc = len(sys.argv)
    if ( argc > 1 ) :
        modelPath = sys.argv[1];
        if ( argc > 2 ) :
            loop_count = int(sys.argv[2])
        if ( argc > 3 ) :
            batch_count = int(sys.argv[3])
            
        if "--verbose" in sys.argv or "-v" in sys.argv:
            logger.set_level(LogLevel.DEBUG)
    else:
        logger.debug("[Usage] run_batch_model [dxnn-file-path] [loop-count] [batch-count] [--verbose|-v]")
        exit(-1)
   
    logger.debug(f"loop-count={loop_count}")
    logger.debug(f"batch-count={batch_count}")
    logger.info(f"Start run_batch_model test for model: {modelPath}")
    
    try:
        
        # create inference engine instance with model
        with InferenceEngine(modelPath) as ie:

            # register call back function
            #ie.register_callback(onInferenceCallbackFunc)

            input_buffers = []
            output_buffers = []
            index = 0
            for b in range(batch_count):
                input_buffers.append([np.zeros(ie.get_input_size(), dtype=np.uint8)])
                output_buffers.append([np.zeros(ie.get_output_size(), dtype=np.uint8)])
                index = index + 1

            start = time.perf_counter()
            # inference loop
            for i in range(loop_count):

                # batch inference
                # It operates asynchronously internally for the specified number of batches and returns the results
                results = ie.run_batch(input_buffers, output_buffers)

                # post processing 
                #postProcessing(outputs)
                logger.debug(f"Inference outputs {i}")
                logger.debug(f"Size of result: {len(results)}")
                for result in results:
                    logger.debug(f"Output (Result): {result}")
            
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