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
    config.set_enable(Configuration.ITEM.SHOW_MODEL_INFO, True)
    config.set_enable(Configuration.ITEM.SHOW_PROFILE, True)

    print('Runtime framework version:', config.get_version())
    print('Device driver version:', config.get_driver_version())
    print('PCIe driver version:', config.get_pcie_driver_version())

    if config.get_enable(Configuration.ITEM.SHOW_MODEL_INFO):
        print('SHOW_MODEL_INFO configuration is enabled')
    else:
        print('SHOW_MODEL_INFO configuration is disabled')

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


    device_count = DeviceStatus.get_device_count()
    for i in range(device_count):
        device_status = DeviceStatus.get_current_status(i)
        print('Device', device_status.get_id())

        for c in range(3):
            print('   NPU Core', c, 
                ' Temperature:', device_status.get_temperature(c),
                ' Voltage:', device_status.get_npu_voltage(c),
                ' Clock:', device_status.get_npu_clock(c))

    exit(result)