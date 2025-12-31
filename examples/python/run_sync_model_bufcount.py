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
from dx_engine import InferenceEngine, InferenceOption
from logger import Logger, LogLevel


def parse_args():
    parser = argparse.ArgumentParser(description="Run synchronous model inference")
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
        
    logger.info(f"Start run_sync_model test for model: {args.model}")
    
    try:
        # create inference engine instance with model
        ie_1_option = InferenceOption()
        ie_1_option.buffer_count = 6
        logger.info(f"Buffer count for IE 1: {ie_1_option.buffer_count}")
        ie_2_option = InferenceOption()
        ie_2_option.buffer_count = 3
        logger.info(f"Buffer count for IE 2: {ie_2_option.buffer_count}")
        with InferenceEngine(args.model, ie_1_option) as ie_1, InferenceEngine(args.model, ie_2_option) as ie_2:

            input_1 = [np.zeros(ie_1.get_input_size(), dtype=np.uint8)]
            input_2 = [np.zeros(ie_2.get_input_size(), dtype=np.uint8)]

            start = time.perf_counter()
            # inference loop
            for i in range(args.loops):

                # inference synchronously 
                # use only one npu core 
                # ownership of the outputs is transferred to the user 
                outputs_1 = ie_1.run(input_1)
                outputs_2 = ie_2.run(input_2)

                # post processing 
                #postProcessing(outputs)
                logger.debug(f"Inference outputs {i}")
                
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