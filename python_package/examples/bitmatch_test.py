import glob
import os
import argparse
import warnings
import numpy as np
import subprocess
import shutil
from dx_engine import InferenceEngine

def parse_args():
    parser = argparse.ArgumentParser(description="Argparse")
    parser.add_argument("--model", "-m", type=str, required=True, help="model dir.")
    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"{args.model} does not exist.")
    return args

def bit_match(mask_path, gt_path, rt_path):
    if mask_path!=None:
        if os.path.exists(mask_path):
            mask = np.frombuffer(open(mask_path, "rb").read(), dtype=np.uint8)
            mask = np.unpackbits(mask, bitorder="little")
        else:
            print("no mask")
            mask = np.array([])
    else:
        print("no mask")
        mask = np.array([])
    
    gt = np.frombuffer(open(gt_path, "rb").read(), dtype=np.int8)
    rt = np.frombuffer(open(rt_path, "rb").read(), dtype=np.int8)

    if mask.nbytes>0:
        if mask.shape != rt.shape:
            warnings.warn("size mismatch Runtime output : {} vs mask : {}".format(rt.shape, mask.shape))
        rt = np.where(mask, rt[: mask.shape[0]], 0)  # mask dummy values as 0
        if mask.shape != gt.shape:
            warnings.warn("size mismatch GT output : {} vs mask : {}".format(gt.shape, mask.shape))
        gt = np.where(mask, gt[: mask.shape[0]], 0)  # mask dummy values as 0

    res = rt == gt

    # print(all(res))  # check all equality
    # print(np.where(res == False)[0].tolist())  # print compare failed indices (byte address in the memory)
    if all(res)==True:
        result = "PASS"
    else:
        result = "FAIL"
    print(f"{rt_path} vs {gt_path} : {result}")
    return all(res)

def bit_match_dir(dir):
    rt_dir = os.path.join(dir, "rt")
    gt_dir = os.path.join(dir, "gt")
    targets = set([os.path.basename(f) for f in glob.glob(os.path.join(rt_dir, '*'))]) \
                & set([os.path.basename(f) for f in glob.glob(os.path.join(gt_dir, '*'))])
    for file in targets:
        rt_file = os.path.join(rt_dir, file)
        gt_file = os.path.join(gt_dir, file)
        # print(rt_file, gt_file)
        # for str in ["_input_", "_output_"]:
        for str in ["_output_"]:
            if str in file:
                tmp = file.split(str)
                task = tmp[0]
                idx = tmp[1].split(".bin")[0]
                task_dir = os.path.join(dir, task)
                if "output" in str:
                    task_mask_file = os.path.join(task_dir, 'bitmatch.bin')
                else:
                    task_mask_file = None
                # print(task_mask_file, gt_file, rt_file)
                bit_match(task_mask_file, gt_file, rt_file)

def inference(dir):
    rt_dir = os.path.join(dir, "rt")
    gt_dir = os.path.join(dir, "gt")
    indexes = []
    # find inputs
    # for f in glob.glob(gt_dir, '*_input_*.bin'):
    input_files = glob.glob(os.path.join(gt_dir, 'input_*.bin'))
    for file in input_files:
        index = file.split("input_")[1].split(".bin")[0]
        indexes.append(index)
    print(indexes)    
    model_file = os.path.join(dir, "graph.dxnn")
    ie = InferenceEngine(model_file)
    for index in indexes:
        input_file = os.path.join(gt_dir, f"input_{index}.bin")
        print(f"-------- {input_file}  start")
        with open(input_file, "rb") as f:
            input = [np.frombuffer(f.read(), dtype=np.uint8)]
        outputs = ie.run(input)
        # print(f"  ------ {input_file}  inference done")
        subprocess.call("sync", shell=True)
        files = glob.glob(os.path.join("./", "cpu_*.bin")) + glob.glob(os.path.join("./", "npu_*.bin"))
        print(files)
        for file in files:
            new_file = os.path.basename(file.replace(".bin", f"_{index}.bin"))
            print(f"       {file} -> {rt_dir}/{new_file}")
            shutil.move(file, f"{rt_dir}/{new_file}")
        print(f"-------- {input_file}  end")

if __name__ == "__main__":
    args = parse_args()

    # os.environ['DXRT_DEBUG_DATA']="1"

    subprocess.call(f"rm -rf ./npu_*.bin ./cpu_*.bin {args.model}/rt/npu_*.bin {args.model}/rt/cpu_*.bin", shell=True)
    subprocess.call(f"mkdir -p {args.model}/rt", shell=True)

    inference(args.model)
    bit_match_dir(args.model)