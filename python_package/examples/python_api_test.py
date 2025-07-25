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
        print(f"Callback triggered for inference with user_arg({user_arg})")
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
    input_tensors_info = ie.get_input_tensors_info()
    output_tensors_info = ie.get_output_tensors_info()
    input_size = ie.get_input_size()
    output_size = ie.get_output_size()

    print(f"Input data type: {input_tensors_info}")
    print("\n------------------------------------------\n")

    print(f"Output data type: {output_tensors_info}")
    print("\n------------------------------------------\n")

    print(f"Input size: {input_size}")
    print("\n------------------------------------------\n")

    print(f"Total output size: {output_size}")
    print("\n------------------------------------------\n")

    input_data_list = [[np.zeros(input_size, dtype=np.uint8)],[np.zeros(input_size, dtype=np.uint8)]]
    output_data_buffer = [[np.empty(output_size, dtype=np.uint8)],[np.empty(output_size, dtype=np.uint8)]]
    
    req_id = ie.run_async(input_data_list[0], user_arg=0)
    outputs = ie.wait(req_id)  
    print(f"run_async() => wait() => outputs[0].shape : {outputs[0].shape}")
    print("\n------------------------------------------\n")

    outputs = ie.run(input_data_list[0])
    print(f"run(single_input) => outputs[0].shape : {outputs[0].shape}")
    print("\n------------------------------------------\n")
    
    outputs = ie.run(input_data_list, output_data_buffer)
    print(f"run(batch_input) => return outputs[0][0].shape : {outputs[0][0].shape}, buffer outputs[0][0].shape : {output_data_buffer[0][0].shape}")
    print("\n------------------------------------------\n")

    fps = ie.run_benchmark(30, input_data_list[0])
    print(f"run_benchmark() => fps : {fps}")
    print("\n------------------------------------------\n")

    outputs = ie.validate_device(input_data_list[0], 0)
    print(outputs)
    if outputs != []:
        print(f"validate_device() => outputs[0].shape : {outputs[0].shape}")
        print("\n------------------------------------------\n")
    else:
        print("validate_device() => outputs[0].shape : None")
        print("\n------------------------------------------\n")
    
    ie.register_callback(callback_with_args)

    req_id = ie.run_async(input_data_list[0], user_arg=0)
    result_queue.put(req_id)
    result_queue.join()
    print(f"run_async(), register_callback(), req_id : {req_id}")
    print("\n------------------------------------------\n")

    mask = ie.get_bitmatch_mask(0)  
    print(f"get_bitmatch_mask() => mask.shape : {mask.shape}")
    print("\n------------------------------------------\n")

    task_order = ie.get_task_order()
    print(f"get_task_order() => task_order : {task_order}")
    print("\n------------------------------------------\n")

    outputs = ie.get_all_task_outputs()
    print(f"runAsync() => the number of outputs from all tasks : {len(outputs)}")
    print("\n------------------------------------------\n")
    
    latency = ie.get_latency()
    print(f"get_latency() => latency : {latency}")
    print("\n------------------------------------------\n")

    npu_inference_time = ie.get_npu_inference_time()
    print(f"get_npu_inference_time() => inference_time : {npu_inference_time}")
    print("\n------------------------------------------\n")

    latency_list = ie.get_latency_list()
    print(f"get_latency_list() => {latency_list}")
    print("\n------------------------------------------\n")

    npu_inference_time_list = ie.get_npu_inference_time_list()
    print(f"get_npu_inference_time_list() => {npu_inference_time_list}")
    print("\n------------------------------------------\n")

    latency_mean = ie.get_latency_mean()
    print(f"get_latency_mean() => {latency_mean}")
    print("\n------------------------------------------\n")

    npu_inference_time_mean = ie.get_npu_inference_time_mean()
    print(f"get_npu_inference_time_mean() => {npu_inference_time_mean}")
    print("\n------------------------------------------\n")

    latency_std = ie.get_latency_std()
    print(f"get_latency_std() => {latency_std}")
    print("\n------------------------------------------\n")

    npu_inference_time_std = ie.get_npu_inference_time_std()
    print(f"get_npu_inference_time_std() => {npu_inference_time_std}")
    print("\n------------------------------------------\n")
    
    latency_count = ie.get_latency_count()
    print(f"get_latency_count() => {latency_count}")
    print("\n------------------------------------------\n")

    npu_inference_time_count = ie.get_npu_inference_time_count()
    print(f"get_npu_inference_time_count() => {npu_inference_time_count}")
    print("\n------------------------------------------\n")
    
    exit(0)