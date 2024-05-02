import warnings
import os
import numpy as np
import argparse
from dx_engine import InferenceEngine

def parseArgs():
    parser = argparse.ArgumentParser(description="Argparse")
    parser.add_argument("--model", "-m", type=str, required=True, help="Model file (.dxnn)")
    parser.add_argument("--input", "-i", type=str, default="", help="Input data file")
    parser.add_argument("--output", "-o", type=str, default="output.bin.pyrt", help="Output data file")
    parser.add_argument("--benchmark", "-b", action="store_true", default=False, help="Perform benchmark test")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Loops to test")

    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    if args.input and not os.path.exists(args.input):
        parser.error(f"Input file '{args.input}' does not exist.")
    if args.input and not os.path.exists(args.input):
        parser.error(f"Input file '{args.input}' does not exist.")
    return args


if __name__ == "__main__":
    args = parseArgs()
    print("---------------------------------")
    print(f"Load model from: {args.model}")
    print("---------------------------------")

    ie = InferenceEngine(args.model)
    input_dtype = ie.input_dtype()
    output_dtype = ie.output_dtype()
    input_size = ie.input_size()
    output_size = ie.output_size()
    print(f"input dtype: {input_dtype}")
    print(f"output dtype: {output_dtype}")
    print(f"input size: {input_size}")
    print(f"output size: {output_size}")
    if args.benchmark is True:
        fps = ie.benchmark(args.loops)
        print(f"FPS: {fps}")
    else:
        if args.input:
            with open(args.input, "rb") as file:
                input = [np.frombuffer(file.read(), dtype=np.uint8)]
        else:
            input = [np.zeros(input_size, dtype=np.uint8)]
        for loop in range(args.loops):
            outputs = ie.run(input)
            with open(args.output, "wb") as file:
                for i, output in enumerate(outputs):
                    print(f" output [{i}]: ", output.dtype, output.shape)
                    output.tofile(f"{args.output}.{i}")
                    file.write(output.tobytes())
            print("-----------------------------------")
            print(f"{args.input} -> {args.output}")
            print(f"  Latency : {ie.latency()}")
            print(f"  Inference time : {ie.inf_time()}")
            print("-----------------------------------")