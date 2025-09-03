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
from dx_engine import InferenceEngine, Configuration, DeviceStatus

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

    global gLoopCount

    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:

        # user data type casting
        index, loop_count = user_arg
    

        # post processing
        #postProcessing(outputs);

        # something to do

        print("Inference output (callback) index=", index)

        gLoopCount += 1
        if ( gLoopCount == loop_count ) :
            print("Complete Callback")
            q.put(0)

    return 0


if __name__ == "__main__":

    config = Configuration()
    config.set_enable(Configuration.ITEM.PROFILER, True)

    # print profiling infomation
    config.set_attribute(Configuration.ITEM.PROFILER, 
                            Configuration.ATTRIBUTE.PROFILER_SHOW_DATA, "ON")

    # save profiling infomation to file
    config.set_attribute(Configuration.ITEM.PROFILER, 
                            Configuration.ATTRIBUTE.PROFILER_SAVE_DATA, "ON")

    print('Runtime framework version:', config.get_version())
    print('Device driver version:', config.get_driver_version())
    print('PCIe driver version:', config.get_pcie_driver_version())

    if config.get_enable(Configuration.ITEM.PROFILER):
        print('PROFLIER configuration is enabled')
    else:
        print('PROFILER configuration is disabled')

    DEFAULT_LOOP_COUNT = 1
    loop_count = DEFAULT_LOOP_COUNT
    modelPath = ""
    argc = len(sys.argv)
    if ( argc > 1 ) :
        modelPath = sys.argv[1];
        if ( argc > 2 ) :
            loop_count = int(sys.argv[2])
    else:
        print("[Usage] run_async_model [dxnn-file-path] [loop-count]")
        exit(-1)
    
    result = -1

    # create inference engine instance with model
    with InferenceEngine(modelPath) as ie:

        # register call back function
        ie.register_callback(onInferenceCallbackFunc)

        input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]

        # inference loop
        for i in range(loop_count):

            # inference asynchronously, use all npu cores
            # if device-load >= max-load-value, this function will block  
            ie.run_async(input, user_arg=[i, loop_count])

            print("Inference start (async)", i)

        # wait until all callback data processing is completed
        result = q.get()

    exit(result)