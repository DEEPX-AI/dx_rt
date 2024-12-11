import numpy as np
import sys
from dx_engine import InferenceEngine

import queue
import threading

q = queue.Queue()


def inferenceThreadFunc(ie, loopCount):

    count = 0

    while(True):
    
        # pop item from queue 
        jobId = q.get()

        # waiting for the inference to complete by jobId
        # ownership of the outputs is transferred to the user 
        outputs = ie.Wait(jobId)

        # post processing
        # postProcessing(outputs);

        # something to do


        print("Inference outputs corresponding to jobId=", jobId, "index=", count)

        count += 1
        if ( count >= loopCount ):
            break
   
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
    

    # create inference engine instance with model
    ie = InferenceEngine(modelPath)

    # do not register call back function
    # ie.register_callback(onInferenceCallbackFunc)

    t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, loop_count))

    t1.start()

    input = [np.zeros(ie.input_size(), dtype=np.uint8)]

    # inference loop
    for i in range(loop_count):

        
        # inference asynchronously, use all npu cores
        # if device-load >= max-load-value, this function will block  
        jobId = ie.RunAsync(input, user_arg=0)

        q.put(jobId)

        print("Inference start (async)", i)

    t1.join()

    exit(0)