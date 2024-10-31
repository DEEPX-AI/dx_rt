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
input_data_list = []

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
    gt_outputs=[]
    gt_files = sorted(glob.glob(os.path.join(model_path, "gt", "npu_0_output_*.bin")))
    for gt_file in gt_files:
        if ('argmax' not in gt_file) and ('ppu' not in gt_file): 
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

def bitmatch(gt,rt,masks):
    mask = masks[0]
    if mask.nbytes > 0:
        if mask.shape != rt.shape:
            warnings.warn(f"size mismatch Runtime output : {rt.shape} vs mask : {mask.shape}")
        rt = np.where(mask, rt[:mask.shape[0]], 0)
        if mask.shape != gt.shape:
            warnings.warn(f"size mismatch GT output : {gt.shape} vs mask : {mask.shape}")
        gt = np.where(mask, gt[:mask.shape[0]], 0)

    print("gt : ",gt[:10])
    print("rt : ",rt[:10])
    print("")
    if np.all(rt == gt):
        return True
    if len(gt) >= 4:
        rt_byte = rt[:4].tobytes()
        gt_byte = gt[:4].tobytes()
        rt_float_value = struct.unpack('f', rt_byte)[0]
        gt_float_value = struct.unpack('f', gt_byte)[0]
        if np.allclose(rt_float_value, gt_float_value, rtol=1e-04):
            rt = int8tofloat32(rt)
            gt = int8tofloat32(gt)
            if np.allclose(rt, gt, rtol=1e-04):
                return True
    return False

def process_model_dir(model_dir, loops, use_ort):
    global pass_count, total_count, input_data_list, failed_requests,all_pass_count,all_total_count

    masks=[]
    pass_count = 0
    total_count = 0
    input_data_list.clear()
    failed_requests = []  # List to track failed requests
    gt_outputs = []  

    print(f"Processing model in: {model_dir}")
    model_files = glob.glob(os.path.join(model_dir, "*.dxnn"))
    if not model_files:
        print(f"No .dxnn files found in the model directory '{model_dir}'")
        return

    ie = InferenceEngine(model_files[0])  # Initialize with the first model
    gt_outputs = load_gt_outputs(model_dir)  # Load ground truth outputs
    if len(gt_outputs) != 0:
        if use_ort :
            input_files = sorted(glob.glob(os.path.join(model_dir, "gt", "input_*.bin")))
        else :
            input_files = sorted(glob.glob(os.path.join(model_dir, "gt", "npu_0_input_*.bin")))

        for input_file in input_files:
            with open(input_file, "rb") as file:
                input_data_list.append([np.frombuffer(file.read(), dtype=np.int8)])

        masks.append(ie.get_bitmatch_mask(0)) # todo : multi npu task  

        for loop in range(loops):
            input_id = loop % len(input_data_list)
            input_data = input_data_list[input_id]
            outputs = ie.validate_device(input_data,0)
            result = bitmatch(gt_outputs[input_id],outputs[0],masks)
            if result :
                pass_count += 1 
                total_count += 1
                all_pass_count += 1
                all_total_count += 1
            else:
                total_count += 1 
                all_total_count += 1
                failed_requests.append(loop)  # Store the index of failed request

            print(f"input ID : {input_id} : {'pass' if result else 'false'}")

        if pass_count < total_count:
            # Log the failed model with failure details
            failed_models.append({
                'model': model_dir,
                'fail_count': total_count - pass_count,
                'failed_indices': input_id
            })
        
        print(f"Model Pass Count: {pass_count} / Total Count: {total_count}")
        print("-----------------------------------")

if __name__ == "__main__":
    args = parseArgs()

    if args.dir:
        # Process each subdirectory inside the directory
        model_dirs = [d for d in glob.glob(os.path.join(args.dir, "*")) if os.path.isdir(d)]
        for model_dir in model_dirs:
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
