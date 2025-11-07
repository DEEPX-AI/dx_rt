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
from dx_engine import InferenceOption
from logger import Logger, LogLevel


def parse_args():
    parser = argparse.ArgumentParser(description="Run batch model inference")
    parser.add_argument("--model", "-m", type=str, required=True, help="Path to model file (.dxnn)")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Number of inference loops (default: 1)")
    parser.add_argument("--verbose", "-v", action="store_true", default=False, help="Enable debug logging")
    parser.add_argument("--batch", "-b", type=int, default=1, help="Batch count (default: 1)")
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
   
    logger.debug(f"loop-count={args.loops}")
    logger.debug(f"batch-count={args.batch}")
    logger.info(f"Start run_batch_model test for model: {args.model}")
    
    try:
        
        # create inference engine instance with model
        with InferenceEngine(args.model) as ie:

            # register call back function
            #ie.register_callback(onInferenceCallbackFunc)

            input_buffers = []
            output_buffers = []
            index = 0
            for b in range(args.batch):
                input_buffers.append([np.zeros(ie.get_input_size(), dtype=np.uint8)])
                output_buffers.append([np.zeros(ie.get_output_size(), dtype=np.uint8)])
                index = index + 1

            start = time.perf_counter()
            # inference loop
            for i in range(args.loops):

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