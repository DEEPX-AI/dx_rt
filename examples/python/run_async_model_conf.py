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
from dx_engine import InferenceEngine, Configuration, DeviceStatus
from logger import Logger, LogLevel

import threading
import queue
from threading import Thread

q = queue.Queue()
gLoopCount = 0

lock = threading.Lock()

def onInferenceCallbackFunc(outputs, user_arg):
    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance
    logger = Logger()
    global gLoopCount

    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:

        # user data type casting
        index, loop_count = user_arg
    
        # post processing
        #postProcessing(outputs);

        # something to do

        logger.debug(f"Inference output (callback) index={index}")

        gLoopCount += 1
        if ( gLoopCount == loop_count ) :
            logger.debug("Complete Callback")
            q.put(0)

    return 0


if __name__ == "__main__":

    logger = Logger()
    config = Configuration()
    config.set_enable(Configuration.ITEM.SHOW_MODEL_INFO, True)
    config.set_enable(Configuration.ITEM.SHOW_PROFILE, True)

    logger.info('Runtime framework version: ' + config.get_version())
    logger.info('Device driver version: ' + config.get_driver_version())
    logger.info('PCIe driver version: ' + config.get_pcie_driver_version())

    if config.get_enable(Configuration.ITEM.SHOW_MODEL_INFO):
        logger.info('SHOW_MODEL_INFO configuration is enabled')
    else:
        logger.info('SHOW_MODEL_INFO configuration is disabled')

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
        logger.info("[Usage] run_async_model_conf [dxnn-file-path] [loop-count] [--verbose|-v]")
        exit(-1)
    
    logger.info(f"Start run_async_model_conf test for model: {modelPath}")
    result = -1

    try:
        
        # create inference engine instance with model
        with InferenceEngine(modelPath) as ie:

            # register call back function
            ie.register_callback(onInferenceCallbackFunc)

            input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
            start = time.perf_counter()
            
            # inference loop
            for i in range(loop_count):

                # inference asynchronously, use all npu cores
                # if device-load >= max-load-value, this function will block  
                ie.run_async(input, user_arg=[i, loop_count])

                logger.debug(f"Inference start (async) {i}")

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
            
            device_count = DeviceStatus.get_device_count()
            for i in range(device_count):
                device_status = DeviceStatus.get_current_status(i)
                logger.info(f'Device {device_status.get_id()}')

                for c in range(3):
                    logger.info(
                        f'   NPU Core {c} '
                        f'Temperature: {device_status.get_temperature(c)} '
                        f'Voltage: {device_status.get_npu_voltage(c)} '
                        f'Clock: {device_status.get_npu_clock(c)}'
                    )
    
    except Exception as e:
        logger.error(f"Exception: {str(e)}")
        exit(-1)
        
    exit(result)