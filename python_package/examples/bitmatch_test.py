import glob
import os
import argparse
import warnings
import numpy as np
import subprocess
import shutil
from dx_engine import InferenceEngine
import struct 
import re 
import platform

USE_ORT=0

def parse_args():
    parser = argparse.ArgumentParser(description="Argparse")
    parser.add_argument("--model", "-m", type=str, required=True, help="model dir.")
    parser.add_argument("--log", "-l", type=str, required=True, help="log filename ex) log")
    parser.add_argument("--debug", "-d", type=str, required=False, help="RT debug options, 0: Do not generate RT binaries. 1: Generate RT binaries for debugging. 2(defualt): Generate RT binaries for release.",default="1")
    args = parser.parse_args()
    return args

def get_target_arch():
    machine = platform.machine().lower()
    if machine in ['x86_64', 'amd64']:
        return 'x86_64'
    elif machine in ['arm64', 'aarch64']:
        return 'aarch64'
    elif machine.startswith('arm'):
        return 'arm'
    elif machine in ['risc64']:
        return 'risc64'
    else:
        return 'unknown'
    
def find_dxrt_dir(target_arch):
    dxrt_dir = '/usr/local'

    if not os.path.exists(dxrt_dir):
        print(f"-- Error : {dxrt_dir} directory does not exist")
        exit(1)
    
    return dxrt_dir

def check_use_ort(dxrt_dir):
    gen_h_path = os.path.join(dxrt_dir, 'include', 'dxrt', 'gen.h')
    if os.path.exists(gen_h_path):
        with open(gen_h_path, 'r') as file:
            for line in file:
                if 'USE_ORT' in line:
                    return 1
    return 0


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
    #ie_outputs = single_output['ie_outputs']
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
        """
        if len(ie_outputs)>0:
            ie_outputs_t = b""
            for ie_output in ie_outputs:
                binary_data = ie_output.tobytes()
                ie_outputs_t = ie_outputs_t + binary_data
            ie_outputs = np.frombuffer(ie_outputs_t, dtype=np.uint16)
        """
    else:
        gt = np.frombuffer(open(gt_path, "rb").read(), dtype=np.int8)
        rt = np.frombuffer(open(rt_path, "rb").read(), dtype=np.int8)
        """
        if len(ie_outputs)>0:
            ie_outputs_t = b""
            for ie_output in ie_outputs:
                binary_data = ie_output.tobytes()
                ie_outputs_t = ie_outputs_t + binary_data
            ie_outputs = np.frombuffer(ie_outputs_t, dtype=np.int8)
        """

    if mask.nbytes > 0 and 'ppu' not in gt_path:
        if mask.shape != rt.shape:
            warnings.warn(f"size mismatch Runtime output : {rt.shape} vs mask : {mask.shape}")
        rt = np.where(mask, rt[:mask.shape[0]], 0)
        if mask.shape != gt.shape:
            warnings.warn(f"size mismatch GT output : {gt.shape} vs mask : {mask.shape}")
        gt = np.where(mask, gt[:mask.shape[0]], 0)
        print(gt.shape, rt.shape, mask.shape)
    '''
    elif mask.nbytes == 0 and gt.shape[0] != rt.shape[0]:
        warnings.warn(f"mask not found : rt.shape : {rt.shape}, gt.shape : {gt.shape}")
        # if the mask is neccesary but empty
        if gt.shape[0] > rt.shape[0]:
            padding_length = len(gt) - len(rt)
            rt = np.pad(rt, (0, padding_length), 'constant', constant_values=0)
        elif gt.shape[0] < rt.shape[0]:
            padding_length = len(rt) - len(gt)
            gt = np.pad(gt, (0, padding_length), 'constant', constant_values=0)
    '''


    #print("ie_outputs : ", ie_outputs[:np.min([len(ie_outputs),10])], ie_outputs.shape)
    print("rt(.bin) : ", rt[:np.min([len(rt),10])], rt.shape)
    print("gt(.bin) : ", gt[:np.min([len(gt),10])], gt.shape)

    if np.all(rt == gt):
        result = "PASS"
        #if np.all(gt==ie_outputs):
        #    result = "PASS(OOO)"
    else:
        result = "FAIL"
    if  result == "FAIL" and len(gt) != 1:
        rt_byte = rt[:4].tobytes()
        gt_byte = gt[:4].tobytes()
        rt_float_value = struct.unpack('f', rt_byte)[0]
        gt_float_value = struct.unpack('f', gt_byte)[0]
        print(rt_float_value,gt_float_value)
        if np.allclose(rt_float_value, gt_float_value, rtol=1e-04, atol=1e-06):
            rt = int8tofloat32(rt)
            gt = int8tofloat32(gt)
            if np.allclose(rt, gt, rtol=1e-05, atol=5e-07):
                result = "PASS"
            elif np.allclose(rt, gt, rtol=1e-04, atol=1e-06):
                result = "CLOSE"
                
    print(f"{rt_path} vs {gt_path} : (rt:{rt.shape}, gt:{gt.shape}) {result} ")
    print('\n')   
    return result

def bit_match_dir(args, single_outputs):
    dir = args.model
    log_file_name = f'{args.log}.txt'
    rt_dir = os.path.join(dir, "rt")
    gt_dir = os.path.join(dir, "gt")
    
    print(f"rt_dir : {rt_dir}")
    print(f"gt_dir : {gt_dir}",end="\n\n")
    with open(log_file_name, "a") as log_file:
        targets_rt = {os.path.basename(f) for f in glob.glob(os.path.join(rt_dir, '*'))}
        targets_gt = {os.path.basename(f) for f in glob.glob(os.path.join(gt_dir, '*'))}
        #print(targets_gt)
        #print(targets_rt)
        targets = [[rt, gt] for rt in targets_rt for gt in targets_gt if rt == gt]
        
        for rt_file, gt_file in targets:
            rt_file_path = os.path.join(rt_dir, rt_file)
            gt_file_path = os.path.join(gt_dir, gt_file)
            
            for suffix in ["_output_"]:
                if suffix in rt_file:
                    task = rt_file.split(suffix)[0]
                    idx = rt_file.split(suffix)[1].split('.')[0]
                    task_dir = os.path.join(dir, task)
                    task_mask_file = os.path.join(task_dir, 'bitmatch.bin') if "output" in suffix else None
                    result = bit_match(task_mask_file, gt_file_path, rt_file_path, single_outputs[idx])
                    log_file.write(f'{rt_file_path}, {gt_file_path}, {result}\n')

def add_index_to_filename(filename, index):
    pattern = re.compile(r"(./\w+_\d+_output)(\w+)?(\.\w+)?\.bin")
    match = pattern.match(filename)
    if match:
        base_name = match.group(1)
        extension1 = match.group(2) or ""
        extension2 = match.group(3) or ""
        new_file = f"{base_name}_{index}{extension1}{extension2}.bin"
        return new_file
    return None

def inference(args):
    dir = args.model
    model_name = os.path.basename(dir)
    log_file_name = f'{args.log}.txt'
    rt_dir = os.path.join(dir, "rt")
    gt_dir = os.path.join(dir, "gt")

    indexes = [file.strip().split("input_")[1].split(".bin")[0] for file in glob.glob(os.path.join(gt_dir, 'input_*.bin'))]

    model_file = os.path.join(dir, "graph.dxnn")
    if not os.path.exists(model_file):
        model_file = os.path.join(dir, f"{model_name}.dxnn")

    ie = InferenceEngine(model_file)
    toposort_order = ie.get_task_order()
    print("task order : ",toposort_order)

    with open(log_file_name, "a") as log_file:
        log_file.write(f'\n{model_file} engine setting,')
        log_file.write(",".join(toposort_order) + ", ...")

    single_outputs = dict()

    with open(log_file_name, "a") as log_file:
        log_file.write(' - SUCCEEDED\n')
        for index in indexes:
            if USE_ORT==0: 
                input_file = os.path.join(gt_dir, f"npu_0_input_{index}.bin")
            else:
                input_file = os.path.join(gt_dir, f"input_{index}.bin")
            with open(input_file, "rb") as f:
                input_data = [np.frombuffer(f.read(), dtype=np.int8)]
            outputs = ie.run(input_data)
            

            #all_outputs = ie.get_outputs()

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
                new_file = add_index_to_filename(file,index)
                if new_file is not None:
                    shutil.move(file, os.path.join(rt_dir, new_file))
            
            masks=list()
            for idx in range(npu_cnt):
                masks.append(ie.get_bitmatch_mask(index=idx))
            if len(outputs) == 1 and np.max(outputs[0].shape) == 1:
                single_outputs[index] = {'output': outputs[0], 'masks':masks,'ie_outputs': outputs}
            else:
                single_outputs[index] = {'output': None, 'masks': masks, 'ie_outputs':outputs}
    return single_outputs

if __name__ == "__main__":
    args = parse_args()
    if args.model[-1] == "/":
        args.model = args.model[:-1]
    
    target_arch = get_target_arch()
    if target_arch == 'unknown':
        print("-- Error: Unknown target architecture")
        exit(1)
    dxrt_dir = find_dxrt_dir(target_arch)
    USE_ORT = check_use_ort(dxrt_dir)
    print(f"USE_ORT = {USE_ORT}")

    os.environ['DXRT_DEBUG_DATA'] = args.debug
    subprocess.call(f"rm -f ./npu_*.bin ./cpu_*.bin {args.model}/rt/*.bin", shell=True)
    subprocess.call(f"mkdir -p {args.model}/rt", shell=True)

    single_outputs = inference(args)
    bit_match_dir(args, single_outputs)
    subprocess.call(f"rm -f ./npu_*.bin ./cpu_*.bin ./dxrt.dump*", shell=True)