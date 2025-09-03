#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

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
