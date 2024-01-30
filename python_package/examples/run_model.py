import warnings
import os
import numpy as np
import argparse
from dx_engine import InferenceEngine


def parseArgs():
    parser = argparse.ArgumentParser(description="Argparse")
    parser.add_argument("--model", "-m", type=str, required=True, help="model path")
    parser.add_argument("--input", "-i", type=str, default="", help="input file")
    parser.add_argument("--output", "-o", type=str, default="output.bin.pyrt", help="output file")
    parser.add_argument("--reference", "-r", type=str, default="", help="reference output file")
    parser.add_argument("--mask", "-k", type=str, default="", help="bit match mask file")
    parser.add_argument("--show", "-s", action="store_true", help="show model info.")
    parser.add_argument("--benchmark", "-b", action="store_true", help="benchmark")

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
        fps = ie.benchmark()
        print(f"FPS: {fps}")
    if args.input:
        with open(args.input, "rb") as file:
            input = [np.frombuffer(file.read(), dtype=np.uint8)]
        outputs = ie.run(input)
        print(type(outputs[0]))
        with open(args.output, "wb") as file:
            for i, output in enumerate(outputs):
                print(f" output [{i}]: ", output.dtype, output.shape)
                output.tofile(f"output{i}.bin")
                file.write(output.tobytes())
        if args.reference and args.mask:
            mask = np.frombuffer(open(args.mask, "rb").read(), dtype=np.uint8)
            mask = np.unpackbits(mask, bitorder="little")
            gt = np.frombuffer(open(args.reference, "rb").read(), dtype=np.int8)
            npu = np.frombuffer(open(args.output, "rb").read(), dtype=np.int8)

            print(npu.shape)
            print(gt.shape)
            print(mask.shape)

            if mask.shape != npu.shape:
                warnings.warn("size mismatch NPU output : {} vs mask : {}".format(npu.shape, mask.shape))
            npu = np.where(mask, npu[: mask.shape[0]], 0)  # mask dummy values as 0
            # npu[3] = 1
            if mask.shape != gt.shape:
                warnings.warn("size mismatch GT output : {} vs mask : {}".format(gt.shape, mask.shape))
            gt = np.where(mask, gt[: mask.shape[0]], 0)  # mask dummy values as 0

            res = npu == gt

            print(all(res))  # check all equality
            print(np.where(res == False)[0].tolist())  # print compare failed indices (byte address in the memory)

            # print("----------------------")
            # outputs_concat = np.concatenate(
            #     [ output.reshape(-1) for output in outputs]
            # )
            # outputs_concat = outputs_concat.astype(np.int8)
            # gt = np.frombuffer(open(args.reference, "rb").read(), dtype=np.int8)
            # mask = np.frombuffer(open(args.mask, "rb").read(), dtype=np.uint8)
            # mask = np.unpackbits(mask, bitorder="little")
            # print(outputs_concat.shape)
            # print(gt.shape)
            # print(mask.shape)
                     
                # print(output[0][0])
