#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import os
import sys
import argparse
from dx_engine.utils import parse_model


def parseArgs():
    parser = argparse.ArgumentParser(
        prog="parse_model",
        description="parse model",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""Examples:
  parse_model -m model.dxnn
  parse_model -m model.dxnn -v
  parse_model -m model.dxnn -o output.txt
  parse_model -m model.dxnn -j    # Extracts model_graph_info.json, model_rmap_info_*.json"""
    )
    
    parser.add_argument("--model", "-m", type=str, required=True, help="model path (required)")
    parser.add_argument("--verbose", "-v", action="store_true", help="show detailed task dependencies and memory usage")
    parser.add_argument("--output", "-o", type=str, help="save the raw console output to a file (without color codes)")
    parser.add_argument("--json", "-j", action="store_true", help="extract JSON binary data (graph_info, rmap_info) to files")

    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    return args


if __name__ == "__main__":
    try:
        args = parseArgs()
        
        # Create parse options dictionary to match C++ ParseOptions structure
        options = {
            'verbose': args.verbose,
            'json_extract': args.json,
            'output_file': args.output if args.output else "",
            'no_color': bool(args.output)  # Disable color for file output
        }
        
        # Call parse_model with options
        result = parse_model(args.model, options)
        sys.exit(result if result != 0 else 0)
        
    except KeyboardInterrupt:
        print("\nInterrupted")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
