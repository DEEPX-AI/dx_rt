import os
import glob
import argparse
import time 
import subprocess

from module.config import TestConfig
from module.tester import BitMatchTester


def pcie_rescan():
    result = subprocess.run(
        "pcie_rescan",
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )


def main():
    parser = argparse.ArgumentParser(description="Bitmatch Test Script")
    parser.add_argument("--model_path", "-m", type=str, help="Model directory path")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Number of test loops")
    parser.add_argument("--dir", "-d", type=str, help="Directory with model subdirectories")
    parser.add_argument("--batch", "-b", action='store_true', help="Enable batch mode")
    parser.add_argument("--batch_itr", "-bi", type=int, default=1, help="Batch iterations")
    parser.add_argument("--test_iteration", "-ti", type=int, default=1, help="Test iterations")
    parser.add_argument("--verbose", "-v", action='store_true', help="Enable verbose output")
    parser.add_argument("--no-logging", action='store_true', help="Disable file logging")
    parser.add_argument("--debug", type=str, required=False, help="RT debug options, 0(defualt): Do not generate RT binaries. / 1: Generate RT binaries for debug. / 2: Generate RT binaries for release.", default="0")
    parser.add_argument("--model_filter", "-f", type=str, required=False, help="Path to model_filter.txt")  

    args = parser.parse_args()
    
    os.environ['DXRT_DEBUG_DATA'] = args.debug

    config = TestConfig(
        batch_mode=args.batch,
        iterations=args.batch_itr,
        verbose=args.verbose,
        log_enabled=not args.no_logging
    )
    
    tester = BitMatchTester(config)
    failed_models = []

    test_total_count = 0
    test_pass_count = 0 

    if args.model_filter:
        with open(args.model_filter, "r") as file:
            filtered_model_names = {line.strip() for line in file.readlines()} 

    if args.dir:
        model_dirs = [d for d in glob.glob(os.path.join(args.dir, "*")) if os.path.isdir(d)]
        if args.model_filter:
            model_dirs = [d for d in model_dirs if os.path.basename(d) in filtered_model_names] 
        
        for test_itr in range(args.test_iteration):
            for model_dir in model_dirs:
                if not tester.process_model(model_dir, args.loops):
                    failed_models.append({
                        'test_itr': test_itr,
                        'model': model_dir,
                        'total_count': tester.stats.total_count,
                        'fail_count': tester.stats.total_count - tester.stats.pass_count,
                        'failed_jobs': tester.stats.failed_jobs
                    })
                test_total_count += tester.stats.total_count
                test_pass_count += tester.stats.pass_count
                #time.sleep(2)
        print(f"{len(model_dirs)} models detected")
    elif args.model_path:
        args.model_path = os.path.dirname(args.model_path) + "/"
        for test_itr in range(args.test_iteration):
            if not tester.process_model(args.model_path, args.loops):
                failed_models.append({
                    'test_itr': test_itr,
                    'model': args.model_path,
                    'total_count': tester.stats.total_count,
                    'fail_count': tester.stats.total_count - tester.stats.pass_count,
                    'failed_jobs': tester.stats.failed_jobs
                })
            test_total_count += tester.stats.total_count
            test_pass_count += tester.stats.pass_count

    tester.log_all_results(test_pass_count, test_total_count, failed_models)

    if failed_models:
        exit(1)
    else:
        exit(0)

if __name__ == "__main__":
    main()
