import warnings
import os
import numpy as np
import argparse
from dx_engine import InferenceEngine
from enum import Enum

class RunModelMode(Enum):
    BENCHMARK_MODE = 0
    SINGLE_MODE = 1
    TARGET_FPS_MODE = 2

    def __str__(self):
        if self == RunModelMode.BENCHMARK_MODE:
            return "Benchmark Mode"
        elif self == RunModelMode.SINGLE_MODE:
            return "Single Mode"
        elif self == RunModelMode.TARGET_FPS_MODE:
            return "Target FPS Mode"
        else:
            return "Unknown Mode"

mode = RunModelMode.BENCHMARK_MODE

def parseArgs():
    parser = argparse.ArgumentParser(description="Argparse")
    parser.add_argument("--model", "-m", type=str, required=True, help="Model file (.dxnn)")
    parser.add_argument("--input", "-i", type=str, default="", help="Input data file")
    parser.add_argument("--output", "-o", type=str, default="output.bin.pyrt", help="Output data file")
    parser.add_argument("--benchmark", "-b", action="store_true", default=False, help="Perform a benchmark test (Maximum throughput)")
    parser.add_argument("--single", "-s", action="store_true", default=False, help="Perform a single run test (Sequential single-input inference on a single-core)")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Loops to test")

    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    if args.input and not os.path.exists(args.input):
        parser.error(f"Input file '{args.input}' does not exist.")
    return args

def SetRunModelMode(single):
    global mode
    if single:
        mode = RunModelMode.SINGLE_MODE
    else :
        mode = RunModelMode.BENCHMARK_MODE
    print(f"\nRun model target mode : {mode}")

if __name__ == "__main__":
    args = parseArgs()
    print(f"model file: {args.model}")
    print(f"input file: {args.input}")
    print(f"output file: {args.output}")

    ie = InferenceEngine(args.model)
    input_dtype = ie.input_dtype()
    output_dtype = ie.output_dtype()
    input_size = ie.input_size()
    output_size = ie.output_size()
    print(f"input dtype: {input_dtype}")
    print(f"output dtype: {output_dtype}")
    print(f"input size: {input_size}")
    print(f"output size: {output_size}")
    SetRunModelMode(args.single)
    if mode is RunModelMode.BENCHMARK_MODE:
        fps = ie.RunBenchMark(args.loops)
        if args.input:
            with open(args.input, "rb") as file:
                input = [np.frombuffer(file.read(), dtype=np.uint8)]
            outputs = ie.Run(input)
            with open(args.output, "wb") as file:
                for i, output in enumerate(outputs):
                    #print(f" output [{i}]: ", output.dtype, output.shape)
                    #output.tofile(f"{args.output}.{i}")
                    file.write(output.tobytes())
        print("=======================================")
        print("* Benchmark Result(3 Cores)")
        print(f"  - FPS                  : {fps}") 
        print("=======================================")
    else:
        if args.input:
            with open(args.input, "rb") as file:
                input = [np.frombuffer(file.read(), dtype=np.uint8)]
        else:
            input = [np.zeros(input_size, dtype=np.uint8)]
        for loop in range(args.loops):
            outputs = ie.Run(input)
            if args.input:
                with open(args.output, "wb") as file:
                    for i, output in enumerate(outputs):
                        #print(f" output [{i}]: ", output.dtype, output.shape)
                        #output.tofile(f"{args.output}.{i}")
                        file.write(output.tobytes())
                print(f"* Processing File : {args.input}")
                print(f"* Output Saved As : {args.output}")

            print("=======================================")
            print("* Benchmark Result(1 Core)")
            print(f"  - NPU Processing Time  : {ie.inference_time()/1000.} ms")
            print(f"  - Total Latency        : {ie.latency()/1000.} ms")
            print(f"  - FPS                  : {1/(ie.inference_time()/1000/1000)}")
            print("=======================================")
