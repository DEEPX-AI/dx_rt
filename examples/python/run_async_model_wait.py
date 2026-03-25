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

def inference_thread_func(ie, loop_count):
    logger = Logger()
    count = 0

    while(True):

        # pop item from queue
        jobId = q.get() # NOSONAR

        # waiting for the inference to complete by jobId
        # ownership of the outputs is transferred to the user
        outputs = ie.wait(jobId) # NOSONAR:python:S1481

        # post processing
        # postProcessing(outputs);

        # something to do


        logger.debug(f"Inference outputs corresponding to jobId={jobId}, index={count}")

        count += 1
        if ( count >= loop_count ):
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
            # ie.register_callback(on_inference_callback_func)

            t1 = threading.Thread(target=inference_thread_func, args=(ie, args.loops))

            t1.start()

            # NOTE: np.zeros() uses COW zero pages — all virtual pages share one
            # physical page. PCIe DMA driver's get_user_pages() then sees duplicate
            # physical pages in the SG list and fails with EFAULT.
            # np.empty() + explicit fill forces unique physical page allocation.
            _buf = np.empty(ie.get_input_size(), dtype=np.uint8)
            _buf.fill(0)
            input = [_buf]
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
