import numpy as np
import sys
from dx_engine import InferenceEngine, InferenceOption


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
    
    # inference option
    option = InferenceOption()

    print("Inference Options:")

    # select devices
    option.set_devices([0])
    print("   Devices=", option.get_devices())

    # NPU bound opion (NPU_ALL or NPU_0 or NPU_1 or NPU_2)
    option.set_bound_option(InferenceOption.BOUND_OPTION.NPU_ALL)
    print("   Option=", option.get_bound_option())

    # use ONNX Runtime (True or False)
    option.set_use_ort(False)
    print("   Use ORT =", option.get_use_ort())
   
    # create inference engine instance with model
    with InferenceEngine(modelPath, option) as ie:

        input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]

        # inference loop
        for i in range(loop_count):

            # inference synchronously 
            # use only one npu core 
            # ownership of the outputs is transferred to the user 
            outputs = ie.run(input)

            # post processing 
            #postProcessing(outputs)
            print("Inference outputs ", i)

    exit(0)