import glob
import os
import argparse
import warnings
import numpy as np
import subprocess
import shutil
from dx_engine import InferenceEngine
import json
import struct 
import re 

def parse_args():
    parser = argparse.ArgumentParser(description="Argparse")
    parser.add_argument("--model", "-m", type=str, required=True, help="model dir.")
    parser.add_argument("--log", "-l", type=str, required=True, help="log filename ex) log")
    parser.add_argument("--npu", "-n", type=int, required=False, help="1(on, defualt) or 0(off), npu validation only(w/o cpu offloading)", default=0)
    args = parser.parse_args()
    return args

def count_val_blocks(file_path):
    with open(file_path, 'r') as file:
        val_block_count = 0
        in_val_block = False

        for line in file:
            if line.strip().startswith("val {"):
                in_val_block = True
            elif in_val_block and line.strip() == "}":
                val_block_count += 1
                in_val_block = False
    return val_block_count

def int8tofloat32(arr):
    float_array=[]
    for i in range(0, len(arr), 4):
        byte_data = arr[i:i+4].tobytes()
        float_value = struct.unpack('f', byte_data)[0]
        float_array.append(float_value)
    return np.array(float_array)

def bit_match(mask_path, gt_path, rt_path, single_output):
    mask = np.array([])
    task_num = int(rt_path.split('_')[-3])
    if mask_path and os.path.exists(mask_path):
        mask = np.frombuffer(open(mask_path, "rb").read(), dtype=np.uint8)
        mask = np.unpackbits(mask, bitorder="little")
    elif 'npu' in gt_path:
        mask = single_output['masks'][task_num]
        if mask.nbytes > 0: 
            print(".dxnn mask, shape : ", mask.shape)

    if 'argmax' in gt_path:
        gt = np.frombuffer(open(gt_path, "rb").read(), dtype=np.uint16)
        rt = np.frombuffer(open(rt_path, "rb").read(), dtype=np.uint16)
        #rt = np.array([single_output['output']], dtype=np.uint16)
    else:
        gt = np.frombuffer(open(gt_path, "rb").read(), dtype=np.int8)
        rt = np.frombuffer(open(rt_path, "rb").read(), dtype=np.int8)

    if mask.nbytes > 0:
        if mask.shape != rt.shape:
            warnings.warn(f"size mismatch Runtime output : {rt.shape} vs mask : {mask.shape}")
        else:
            rt = np.where(mask, rt[:mask.shape[0]], 0)
        if mask.shape != gt.shape:
            warnings.warn(f"size mismatch GT output : {gt.shape} vs mask : {mask.shape}")
        else:
            gt = np.where(mask, gt[:mask.shape[0]], 0)
    elif mask.nbytes == 0 and gt.shape[0] != rt.shape[0]:
        warnings.warn(f"mask not found : rt.shape : {rt.shape}, gt.shape : {gt.shape}")
        # if the mask is neccesary but empty
        if gt.shape[0] > rt.shape[0]:
            padding_length = len(gt) - len(rt)
            rt = np.pad(rt, (0, padding_length), 'constant', constant_values=0)
        elif gt.shape[0] < rt.shape[0]:
            padding_length = len(rt) - len(gt)
            gt = np.pad(gt, (0, padding_length), 'constant', constant_values=0)

    print("rt : ", rt)
    print("gt : ", gt)

    if np.all(rt == gt):
        result = True
    else:
        result = False
    if not result and len(gt) != 1:
        rt_byte = rt[:4].tobytes()
        gt_byte = gt[:4].tobytes()
        rt_float_value = struct.unpack('f', rt_byte)[0]
        gt_float_value = struct.unpack('f', gt_byte)[0]
        if np.allclose(rt_float_value, gt_float_value, rtol=1e-05, atol=5e-7):
            rt = int8tofloat32(rt)
            gt = int8tofloat32(gt)
            if np.allclose(rt, gt, rtol=1e-05, atol=5e-07):
                result = True

    print(f"{rt_path} vs {gt_path} : (rt:{rt.shape}, gt:{gt.shape}) {result} ")
    print('\n')   
    return result

def bit_match_dir(args, single_outputs):
    dir = args.model
    log_file_name = f'{args.log}.txt'
    rt_dir = os.path.join(dir, "rt")
    gt_dir = os.path.join(dir, "gt")
    
    with open(log_file_name, "a") as log_file:
        targets_rt = {os.path.basename(f) for f in glob.glob(os.path.join(rt_dir, '*'))}
        targets_gt = {os.path.basename(f) for f in glob.glob(os.path.join(gt_dir, '*'))}

        targets = [[rt, gt] for rt in targets_rt for gt in targets_gt if rt.split('.')[0] == gt.split('.')[0]]
        
        for rt_file, gt_file in targets:
            rt_file_path = os.path.join(rt_dir, rt_file)
            gt_file_path = os.path.join(gt_dir, gt_file)
            
            for suffix in ["_output_"]:
                if suffix in rt_file:
                    task = rt_file.split(suffix)[0]
                    idx = rt_file.split(suffix)[1].split('.bin')[0]
                    task_dir = os.path.join(dir, task)
                    task_mask_file = os.path.join(task_dir, 'bitmatch.bin') if "output" in suffix else None
                    result = bit_match(task_mask_file, gt_file_path, rt_file_path, single_outputs[idx])
                    log_file.write(f'{rt_file_path}, {gt_file_path}, {result}\n')

def inference(args):
    dir = args.model
    model_name = os.path.basename(dir)
    log_file_name = f'{args.log}.txt'
    rt_dir = os.path.join(dir, "rt")
    gt_dir = os.path.join(dir, "gt")
    
    connection_info_file = os.path.join(dir, "connection_info.json")
    toposort_order = []
    if os.path.exists(connection_info_file):
        with open(connection_info_file, "r") as json_file:
            connection_info = json.load(json_file)
            toposort_order = connection_info.get("toposort_order", [])

    bitmatch_cfg_file = os.path.join(dir, "bitmatch_cfg.txt")
    val_blocks = count_val_blocks(bitmatch_cfg_file) if os.path.exists(bitmatch_cfg_file) else -1

    indexes = [file.strip().split("input_")[1].split(".bin")[0] for file in glob.glob(os.path.join(gt_dir, 'input_*.bin'))]

    model_file = os.path.join(dir, "graph.dxnn")
    if not os.path.exists(model_file):
        model_file = os.path.join(dir, f"{model_name}.dxnn")

    with open(log_file_name, "a") as log_file:
        log_file.write(f'\n{model_file} engine setting,')
        log_file.write(",".join(toposort_order) + f",{val_blocks}, ...")

    ie = InferenceEngine(model_file)
    single_outputs = dict()

    with open(log_file_name, "a") as log_file:
        log_file.write(' - SUCCEEDED\n')
        for index in indexes:
            if args.npu: 
                input_file = os.path.join(gt_dir, f"npu_0_input_{index}.bin")
            else:
                input_file = os.path.join(gt_dir, f"input_{index}.bin")

            with open(input_file, "rb") as f:
                input_data = [np.frombuffer(f.read(), dtype=np.uint8)]
            outputs = ie.run(input_data)[0]

            log_file.write(f'{input_file} inference SUCCEEDED\n')

            subprocess.call("sync", shell=True)
            files = glob.glob("./cpu_*.bin") + glob.glob("./npu_*.bin")

            pattern = re.compile(r"./npu_(\d+)_input+.bin")
            npu_indices = []

            for file in files:
                match = pattern.search(file)
                if match:
                    npu_indices.append(int(match.group(1)))

            if npu_indices:
                npu_cnt = int(max(npu_indices))+1
            else:
                npu_cnt = -1
            
            for file in files:
                new_file = os.path.basename(file.replace(".bin", f"_{index}.bin"))
                shutil.move(file, os.path.join(rt_dir, new_file))
            
            masks=list()
            for idx in range(npu_cnt):
                masks.append(ie.get_bitmatch_mask(index=idx))
            
            if np.max(outputs.shape) == 1:
                single_outputs[index] = {'output': outputs[0], 'masks':masks}
            else:
                single_outputs[index] = {'output': None, 'masks': masks}
    return single_outputs

if __name__ == "__main__":
    args = parse_args()

    os.environ['DXRT_DEBUG_DATA'] = "2" 
    subprocess.call(f"rm -rf ./npu_*.bin ./cpu_*.bin {args.model}/rt/npu_*.bin {args.model}/rt/cpu_*.bin", shell=True)
    subprocess.call(f"mkdir -p {args.model}/rt", shell=True)

    single_outputs = inference(args)
    bit_match_dir(args, single_outputs)
    subprocess.call(f"rm -f ./dxrt.dump*", shell=True)
