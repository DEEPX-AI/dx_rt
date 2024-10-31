import warnings
import os
import numpy as np
import argparse
from dx_engine import InferenceEngine
import glob
import time
import threading
import struct
import queue  

callback_lock = threading.Lock()
pass_count = 0
total_count = 0
gt_outputs = []  # Global list to store ground truth outputs
masks = []
input_data_list = []
result_queue = queue.Queue()  
PPU_model_flag = False
ARGMAX_model_flag = False

all_pass_count = 0
all_total_count = 0
failed_models = []  # To store models that failed

def parseArgs():
    parser = argparse.ArgumentParser(description="Argparse")
    parser.add_argument("--model_path", "-m", type=str, help="Model directory containing .dxnn files")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Loops to test")
    parser.add_argument("--dir", "-d", type=str, help="Directory containing multiple model subdirectories")
    parser.add_argument("--use_ort", "-u", action='store_true', help="Specify whether the USE_ORT option is enabled")
    args = parser.parse_args()

    if args.dir and not os.path.isdir(args.dir):
        parser.error(f"Directory path '{args.dir}' does not exist.")
    
    if args.model_path and not os.path.isdir(args.model_path):
        parser.error(f"Model path '{args.model_path}' does not exist.")
    
    return args

def load_gt_outputs(model_path):
    global PPU_model_flag, ARGMAX_model_flag, gt_outputs
    gt_files = sorted(glob.glob(os.path.join(model_path, "gt", "npu_0_output_*.bin")))
    for gt_file in gt_files:
        if 'ppu' in gt_file:
            gt_outputs.append(np.frombuffer(open(gt_file, "rb").read(), dtype=np.int8))
            PPU_model_flag = True
        elif ('argmax' in gt_file) : 
            gt_outputs.append(np.frombuffer(open(gt_file, "rb").read(), dtype=np.int8))
            ARGMAX_model_flag = True
        elif not PPU_model_flag and not ARGMAX_model_flag:
            gt_outputs.append(np.frombuffer(open(gt_file, "rb").read(), dtype=np.int8))
    print("gt_outputs : ",gt_outputs)
    return gt_outputs

def int8tofloat32(arr):
    float_array=[]
    for i in range(0, len(arr), 4):
        byte_data = arr[i:i+4].tobytes()
        float_value = struct.unpack('f', byte_data)[0]
        float_array.append(float_value)
    return np.array(float_array)

def callbackWithArgs(outputs, user_arg):
    global pass_count, total_count, masks, gt_outputs, all_pass_count, all_total_count, failed_requests
    with callback_lock:
        gt_output = gt_outputs[user_arg.value] 

        ie_outputs_t = b""
        for ie_output in outputs:
            binary_data = ie_output.tobytes()
            ie_outputs_t = ie_outputs_t + binary_data
        ie_outputs = np.frombuffer(ie_outputs_t, dtype=np.int8)

        mask = masks[0]
        if mask.nbytes > 0:
            if mask.shape != ie_outputs.shape:
                warnings.warn(f"size mismatch Runtime output : {ie_outputs.shape} vs mask : {mask.shape}")
            ie_outputs = np.where(mask, ie_outputs[:mask.shape[0]], 0)
            if mask.shape != gt_output.shape:
                warnings.warn(f"size mismatch GT output : {gt_output.shape} vs mask : {mask.shape}")
            gt_output = np.where(mask, gt_output[:mask.shape[0]], 0)

        total_count += 1
        all_total_count += 1
        if np.all(ie_outputs == gt_output):
            pass_count += 1
            all_pass_count += 1
            result = "PASS"
        else:
            result = "FAIL"
            failed_requests.append(user_arg.value)  # Store the index of failed request

        if result == "FAIL" and len(gt_output) != 1:
            rt_byte = ie_outputs[:4].tobytes()
            gt_byte = gt_output[:4].tobytes()
            rt_float_value = struct.unpack('f', rt_byte)[0]
            gt_float_value = struct.unpack('f', gt_byte)[0]
            if np.allclose(rt_float_value, gt_float_value, rtol=1e-04):
                ie_outputs = int8tofloat32(ie_outputs)
                gt_output = int8tofloat32(gt_output)
                if np.allclose(ie_outputs, gt_output, rtol=1e-05):
                    pass_count += 1
                    all_pass_count += 1
                    result = "PASS"
                elif np.allclose(ie_outputs, gt_output, rtol=1e-04):
                    pass_count += 1
                    all_pass_count += 1
                    result = "PASS"
        print(f"user_arg : {user_arg.value} | Callback called, Result: {result}")
        result_queue.get(timeout=5)
        result_queue.task_done()
    return 0  

def process_model_dir(model_dir, loops, use_ort):
    global pass_count, total_count, gt_outputs, input_data_list, masks, failed_requests, PPU_model_flag, ARGMAX_model_flag

    pass_count = 0
    total_count = 0
    gt_outputs.clear()
    input_data_list.clear()
    masks.clear()
    failed_requests = []  

    print(f"Processing model in: {model_dir}")
    model_files = glob.glob(os.path.join(model_dir, "*.dxnn"))
    if not model_files:
        print(f"No .dxnn files found in the model directory '{model_dir}'")
        return

    ie = InferenceEngine(model_files[0]) 
    load_gt_outputs(model_dir) 

    if use_ort :
        input_files = sorted(glob.glob(os.path.join(model_dir, "gt", "input_*.bin")))
    else :
        input_files = sorted(glob.glob(os.path.join(model_dir, "gt", "npu_0_input_*.bin")))

    for input_file in input_files:
        with open(input_file, "rb") as file:
            input_data_list.append([np.frombuffer(file.read(), dtype=np.int8)])

    ie.register_callback(callbackWithArgs)

    masks.append(ie.get_bitmatch_mask(0))
    if (ie.output_size() < gt_outputs[0].size) and (not (PPU_model_flag and gt_outputs[0].size<131072)) and (not ARGMAX_model_flag):
        print(f"rt size [{ie.output_size()}] < gt size [{gt_outputs[0].size}], masks [{masks[0].size}]")
        print("Asynch. Bitmatch with DEBUG.dxnn is not supported, You can use \"npu_validation.py\"")
        exit(1)

    for loop in range(loops):
        input_id = loop % len(input_data_list)
        input_data = input_data_list[input_id]
        req_id = ie.run_async(input_data, user_arg=input_id)
        print(f"Request ID: {req_id}, input ID : {input_id}")
        result_queue.put(req_id)

    result_queue.join()
    if pass_count < total_count:
        
        failed_models.append({
            'model': model_dir,
            'fail_count': total_count - pass_count,
            'failed_indices': failed_requests
        })
    print(f"Model Pass Count: {pass_count} / Total Count: {total_count}")
    print("-----------------------------------")

if __name__ == "__main__":
    args = parseArgs()

    if args.dir:
        model_dirs = [d for d in glob.glob(os.path.join(args.dir, "*")) if os.path.isdir(d)]
        for model_dir in model_dirs:
            PPU_model_flag = False
            ARGMAX_model_flag = False
            process_model_dir(model_dir, args.loops, args.use_ort)

        print(f"-----------------------------------")
        print(f"Overall Pass Count: {all_pass_count} / Total Count: {all_total_count}")
        print(f"-----------------------------------")

        if failed_models:
            print("\nFailed Models:")
            for model in failed_models:
                print(f"Model: {model['model']}, Failures: {model['fail_count']}, Failed Indices: {model['failed_indices']}")
        else:
            print("All models passed!")
    elif args.model_path:
        process_model_dir(args.model_path, args.loops, args.use_ort)

    print(f"Final Pass Count: {all_pass_count} / Total Count: {all_total_count}")
