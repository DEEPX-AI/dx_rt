import os
import numpy as np
import argparse
from dx_engine import InferenceEngine
import time
import threading
import queue  

callback_lock = threading.Lock()
result_queue = queue.Queue()  
def parse_args():
    parser = argparse.ArgumentParser(description="Inference Engine Arguments")
    parser.add_argument("--model", "-m", type=str, required=True, help="Path to model file (.dxnn)")
    parser.add_argument("--benchmark", "-b", action="store_true", default=False, help="Run benchmark test")

    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    return args

def callback_with_args(outputs, user_arg):
    with callback_lock:
        print(f"Callback triggered for inference with user_arg({user_arg.value})")
        result_queue.get(timeout=5) 
        result_queue.task_done() 
    return 0

if __name__ == "__main__":
    args = parse_args()

    print("---------------------------------")
    print(f"Loading model from: {args.model}")
    print("---------------------------------")

    # Initialize inference engine
    ie = InferenceEngine(args.model)
    input_dtype = ie.input_dtype()
    output_dtype = ie.output_dtype()
    input_size = ie.input_size()
    output_size = ie.output_size()

    print(f"Input data type: {input_dtype}")
    print("\n------------------------------------------\n")

    print(f"Output data type: {output_dtype}")
    print("\n------------------------------------------\n")

    print(f"Input size: {input_size}")
    print("\n------------------------------------------\n")

    print(f"Total output size: {output_size}")
    print("\n------------------------------------------\n")

    input_data_list = [[np.zeros(input_size, dtype=np.uint8)],[np.zeros(input_size, dtype=np.uint8)]]

    
    outputs = ie.run(input_data_list[0])
    print(f"run() => outputs[0].shape : {outputs[0].shape}")
    print("\n------------------------------------------\n")
    
    outputs = ie.validate_device(input_data_list[0], 0)
    print(outputs)
    print(f"validate_device() => outputs[0].shape : {outputs[0].shape}")
    print("\n------------------------------------------\n")
    
    
    fps = ie.benchmark()
    print(f"benchmark() => fps : {fps}")
    print("\n------------------------------------------\n")

    req_id = ie.run_async(input_data_list[0], user_arg=0)
    outputs = ie.wait(req_id)  
    print(f"wait() => outputs[0].shape : {outputs[0].shape}")
    print("\n------------------------------------------\n")

    mask = ie.get_bitmatch_mask(0)  
    print(f"get_bitmatch_mask() => mask.shape : {mask.shape}")
    print("\n------------------------------------------\n")

    outputs = ie.arun_batch(input_data_list[0], 5)
    print(f"arun_batch() => outputs[0][0].shape : {outputs[0][0].shape}")
    print("\n------------------------------------------\n")

    task_order = ie.get_task_order()
    print(f"get_task_order() => task_order : {task_order}")
    print("\n------------------------------------------\n")

    outputs = ie.get_outputs()
    print(f"run() => the number of outputs : {len(outputs)}, outputs[0].shape : {outputs[0][0].shape}")
    print("\n------------------------------------------\n")
    
    latency = ie.latency()
    print(f"latency() => latency : {latency}")
    print("\n------------------------------------------\n")

    inf_time = ie.inf_time()
    print(f"inf_time() => inf_time : {inf_time}")
    print("\n------------------------------------------\n")

    ie.register_callback(callback_with_args)

    req_id = ie.run_async(input_data_list[0], user_arg=0)
    result_queue.put(req_id)
    result_queue.join()
    print(f"run_async(), register_callback(), req_id : {req_id}")
    print("\n------------------------------------------\n")