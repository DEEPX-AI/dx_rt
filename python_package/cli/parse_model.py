import os
import argparse
from dx_engine.utils import parse_model


def parseArgs():
    parser = argparse.ArgumentParser(description="Argparse")
    parser.add_argument("--model", "-m", type=str, required=True, help="model path")

    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    return args


if __name__ == "__main__":
    args = parseArgs()
    parse_model(args.model)
