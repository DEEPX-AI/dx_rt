import numpy as np
import sys
from dx_engine import InferenceEngine

import threading
import queue

THRAD_COUNT = 3
total_count = 0
q = queue.Queue()

lock = threading.Lock()

def inferenceThreadFunc(ie, input, threadIndex, loopCount):


    # inference loop
    for i in range(loopCount):

        # inference asynchronously, use all npu cores
        # if device-load >= max-load-value, this function will block  
        ie.run_async(input, user_arg = [i, loopCount, threadIndex])

        print("inferenceThreadFunc thread-index=", threadIndex, "loop-index=", i)
    
    return 0

def onInferenceCallbackFunc(outputs, user_arg):
    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance

    global total_count

    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:
        # user data type casting
        index = user_arg.value[0]
        loop_count = user_arg.value[1]
        thread_index = user_arg.value[2]

        # post processing
        #postProcessing(outputs);

        # something to do

        total_count += 1
        print("Inference output (callback) thread-index=", thread_index, "index=", index, "total-count=", total_count)

        if ( total_count ==  loop_count * THRAD_COUNT) :
            print("Complete Callback")
            q.put(0)

    return 0


if __name__ == "__main__":
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
        
        # input
        input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
    
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

    exit(result)