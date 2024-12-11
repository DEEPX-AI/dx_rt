import numpy as np
import sys
from dx_engine import InferenceEngine


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
        print("[Usage] run_sync_model [dxnn-file-path] [loop-count]")
        exit(-1)
    

    # create inference engine instance with model
    ie = InferenceEngine(modelPath)

    input = [np.zeros(ie.input_size(), dtype=np.uint8)]

    # inference loop
    for i in range(loop_count):

        # inference synchronously 
        # use only one npu core 
        # ownership of the outputs is transferred to the user 
        outputs = ie.Run(input)

        # post processing 
        #postProcessing(outputs)
        print("Inference outputs ", i)

    exit(0)