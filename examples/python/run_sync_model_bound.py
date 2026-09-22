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
from contextlib import ExitStack
from dx_engine import InferenceEngine, InferenceOption
from logger import Logger, LogLevel


def parse_args():
    parser = argparse.ArgumentParser(description="Run synchronous model inference with core/device binding")
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

    if args.verbose:
        logger.set_level(LogLevel.DEBUG)

    logger.info(f"Start run_sync_model_bound test for model: {args.model}")

    bound_configs = [
        ("NPU_0", InferenceOption.BOUND_OPTION.NPU_0),
        ("NPU_1", InferenceOption.BOUND_OPTION.NPU_1),
        ("NPU_012", InferenceOption.BOUND_OPTION.NPU_ALL),
    ]

    logger.debug("Inference Options for 3 IEs:")

    try:
        with ExitStack() as stack:
            engines = []
            inputs_per_engine = []

            for name, bound_option in bound_configs:
                option = InferenceOption()
                option.devices = [0]
                option.bound_option = bound_option
                option.use_ort = False

                logger.debug(f"   [{name}] Devices = {option.devices}")
                logger.debug(f"   [{name}] Option  = {option.bound_option}")
                logger.debug(f"   [{name}] Use ORT = {option.use_ort}")

                ie = stack.enter_context(InferenceEngine(args.model, option))
                engines.append((name, ie))

                # NOTE: np.zeros() uses COW zero pages — all virtual pages share one
                # physical page. PCIe DMA driver's get_user_pages() then sees duplicate
                # physical pages in the SG list and fails with EFAULT.
                # np.empty() + explicit fill forces unique physical page allocation.
                _buf = np.empty(ie.get_input_size(), dtype=np.uint8)
                _buf.fill(0)
                inputs_per_engine.append([_buf])

            start = time.perf_counter()

            for i in range(args.loops):
                for idx, (name, ie) in enumerate(engines):
                    ie.run(inputs_per_engine[idx])
                    logger.debug(f"Inference outputs loop={i}, engine={name}")

            end = time.perf_counter()
            total_time_ms = (end - start) * 1000
            total_requests = args.loops * len(engines)
            avg_latency = total_time_ms / total_requests if total_requests > 0 else 0.0
            fps = (1000.0 * total_requests / total_time_ms) if total_time_ms > 0 else 0.0

            logger.info("-----------------------------------")
            logger.info(f"Total Time: {total_time_ms:.3f} ms")
            logger.info(f"Total Inference Requests: {total_requests}")
            logger.info(f"Average Latency per Inference: {avg_latency:.3f} ms")
            logger.info(f"FPS: {fps:.2f} frame/sec")
            logger.info("Success: 3 InferenceEngines with different bound options completed")
            logger.info("-----------------------------------")

    except Exception as e:
        logger.error(f"Exception: {str(e)}")
        exit(-1)

    exit(0)
