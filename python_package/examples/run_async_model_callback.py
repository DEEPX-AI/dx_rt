import os
import numpy as np
import argparse
from dx_engine import InferenceEngine
import time
import threading
import queue  

callback_cnt = 0
callback_lock = threading.Lock()
result_queue = queue.Queue()  
start_time = 0
end_time = 0
def parse_args():
    parser = argparse.ArgumentParser(description="Inference Engine Arguments")
    parser.add_argument("--model", "-m", type=str, required=True, help="Path to model file (.dxnn)")
    parser.add_argument("--input", "-i", type=str, default="", help="Path to input data file")
    parser.add_argument("--output", "-o", type=str, default="output.bin.pyrt", help="Path to output data file")
    parser.add_argument("--benchmark", "-b", action="store_true", default=False, help="Run benchmark test")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Number of inference loops")

    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    if args.input and not os.path.exists(args.input):
        parser.error(f"Input file '{args.input}' does not exist.")
    return args

def callback_with_args(outputs, user_arg):
    global callback_cnt
    with callback_lock:
        print(f"Callback triggered for inference with user_arg({user_arg.value})")
        callback_cnt += 1
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
    print(f"Output data type: {output_dtype}")
    print(f"Input size: {input_size}")
    print(f"Total output size: {output_size}")

    # Load input data if provided, otherwise use zeros
    if args.input:
        with open(args.input, "rb") as file:
            input_data = [np.frombuffer(file.read(), dtype=np.uint8)]
    else:
        input_data = [np.zeros(input_size, dtype=np.uint8)]

    # Register callback function
    ie.RegisterCallBack(callback_with_args)

    start_time = time.time()

    # Run inference for the number of loops specified
    for loop in range(args.loops):
        req_id = ie.RunAsync(input_data, user_arg=loop)
        print(f"[{req_id}] Inference request #{req_id} submitted with user_arg({loop})")
        result_queue.put(req_id)

    # Wait for all callbacks to complete
    # Join the queue and wait for all tasks to be done
    result_queue.join()

    end_time = time.time()

    total_time = (end_time - start_time) * 1000  # Convert to milliseconds
    avg_latency = total_time / args.loops  # Average latency per request
    fps = 1000.0 / avg_latency  # FPS based on average latency

    print("-----------------------------------")
    print(f"  Total Time: {total_time:.2f} ms")
    print(f"  Average Latency: {avg_latency:.2f} ms")
    print(f"  FPS: {fps:.2f} frames/sec")
    print("-----------------------------------")
