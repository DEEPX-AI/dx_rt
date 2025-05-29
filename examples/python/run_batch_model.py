import numpy as np
import sys
from dx_engine import InferenceEngine
from dx_engine import InferenceOption


if __name__ == "__main__":
    DEFAULT_LOOP_COUNT = 1
    DEFAULT_BATCH_COUNT = 1
    
    loop_count = DEFAULT_LOOP_COUNT
    batch_count = DEFAULT_BATCH_COUNT
    modelPath = ""
    argc = len(sys.argv)
    if ( argc > 1 ) :
        modelPath = sys.argv[1];
        if ( argc > 2 ) :
            loop_count = int(sys.argv[2])
        if ( argc > 3 ) :
            batch_count = int(sys.argv[3])
    else:
        print("[Usage] run_batch_model [dxnn-file-path] [loop-count] [batch-count]")
        exit(-1)
   
    print('loop-count=', loop_count)
    print('batch-count=', batch_count)
   
    # create inference engine instance with model
    with InferenceEngine(modelPath) as ie:

        # register call back function
        #ie.register_callback(onInferenceCallbackFunc)

        input_buffers = []
        output_buffers = []
        index = 0
        for b in range(batch_count):
            input_buffers.append([np.zeros(ie.get_input_size(), dtype=np.uint8)])
            output_buffers.append([np.zeros(ie.get_output_size(), dtype=np.uint8)])
            index = index + 1

        # inference loop
        for i in range(loop_count):

            # batch inference
            # It operates asynchronously internally for the specified number of batches and returns the results
            results = ie.run_batch(input_buffers, output_buffers)

            # post processing 
            #postProcessing(outputs)
            print("Inference outputs ", i)
            print("Size of result", len(results))
            for result in results:
                print("Output (Result)", result)

    exit(0)