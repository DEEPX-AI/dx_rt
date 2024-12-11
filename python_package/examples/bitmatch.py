import warnings
import os
import numpy as np
import argparse
from dx_engine import InferenceEngine
import glob
import time
import threading
import struct
import queue
import logging
from collections import defaultdict
from dataclasses import dataclass
from typing import List, Dict, Any, Optional, Tuple
from concurrent.futures import ThreadPoolExecutor

@dataclass
class TestConfig:
    use_ort: bool = False
    batch_mode: bool = False
    iterations: int = 1
    verbose: bool = True
    log_enabled: bool = True

@dataclass
class TestStatistics:
    pass_count: int = 0
    total_count: int = 0
    failed_jobs: List[int] = None
    execution_times: List[float] = None

    def __post_init__(self):
        self.failed_jobs = []
        self.execution_times = []

class BitMatchTester:
    def __init__(self, config: TestConfig):
        self.config = config
        self.stats = TestStatistics()
        self.callback_lock = threading.Lock()
        self.result_queue = queue.Queue()
        self.model_result_messages = []
        self.compile_type = "RELEASE"
        self._setup_logging()
        
    def _setup_logging(self):
        if self.config.log_enabled:
            log_format = "%(asctime)s - %(levelname)s - %(message)s"
            date_format = "%Y-%m-%d %H:%M:%S"
            logging.basicConfig(
                filename='bitmatch_test_async_result.log',
                level=logging.INFO,
                format=log_format,
                datefmt=date_format
            )

    def log(self, message: str, level: str = 'info', logskip=False):
        if self.config.verbose:
            print(f"[{level}] {message}")
        if self.config.log_enabled and not logskip:
            getattr(logging, level)(message)

    @staticmethod
    def int8tofloat32(arr: np.ndarray) -> np.ndarray:
        return np.frombuffer(arr.tobytes(), dtype=np.float32)

    def bitmatch_logic(self, ie_output: np.ndarray, gt_output: np.ndarray, mask: Optional[np.ndarray] = None) -> Tuple[str, np.ndarray, np.ndarray]:
        if mask is not None and mask.nbytes > 0:
            if mask.shape != ie_output.shape:
                warnings.warn(f"Size mismatch Runtime output: {ie_output.shape} vs mask: {mask.shape}, (gt : {gt_output.shape})")
            ie_output = np.where(mask, ie_output[:mask.shape[0]], 0)
            gt_output = np.where(mask, gt_output[:mask.shape[0]], 0)
        """    
        if mask is not None and mask.nbytes > 0:
            if mask.shape != ie_output.shape:
                warnings.warn(f"Size mismatch Runtime output: {ie_output.shape} vs mask: {mask.shape}")
            masked_indices = mask.astype(bool)
            ie_output = ie_output[masked_indices]
            gt_output = gt_output[masked_indices]
        """

        if np.array_equal(ie_output, gt_output):
            return "PASS", ie_output, gt_output

        """
        if len(gt_output) != 1:
            ie_float = self.int8tofloat32(ie_output[:4])
            gt_float = self.int8tofloat32(gt_output[:4])
            
            # Small value differences in floats are not allowed by default, as they are likely caused by DRAM bit errors
            if np.allclose(ie_float, gt_float, rtol=1e-05):
                ie_full_float = self.int8tofloat32(ie_output)
                gt_full_float = self.int8tofloat32(gt_output)
                if np.allclose(ie_full_float, gt_full_float, rtol=1e-05):
                    return "PASS", ie_output, gt_output
        """
        return "FAIL", ie_output, gt_output

    def _handle_mismatch(self, ie_outputs: np.ndarray, gt_output_masked: np.ndarray, loop_id: int):
        different_indices = np.where(ie_outputs != gt_output_masked)[0]
        first_10_diff_indices = different_indices[:10]
        first_10_diff_values = [(ie_outputs[i], gt_output_masked[i]) 
                               for i in first_10_diff_indices]

        self.log(f"Indices of differences (first 10): {first_10_diff_indices}")
        self.log(f"Total number of differences: {len(different_indices)}")
        self.log(f"Different values (first 10): {first_10_diff_values}")
        self.stats.failed_jobs.append(loop_id)

    def callback_handler(self, outputs: List[np.ndarray], user_arg: Any):
        with self.callback_lock:
            loop_id, input_idx = user_arg.value
            
            if not self.config.batch_mode:
                ie_outputs = np.frombuffer(b''.join(output.tobytes() for output in outputs), dtype=np.int8)
                gt_output = self.gt_outputs[input_idx]
                
                result, ie_outputs, gt_output_masked = self.bitmatch_logic(
                    ie_outputs, gt_output["LAST"], self.masks[0]
                )
                
                self.log(f"loop_id: {loop_id}, input_idx: {input_idx} | Result: {result}", logskip=True)
                
                self.stats.total_count += 1
                if result == "PASS":
                    self.stats.pass_count += 1
                else:
                    self._handle_mismatch(ie_outputs, gt_output_masked, loop_id)
                
                self.result_queue.get(timeout=5)
                self.result_queue.task_done()
            else:
                self.log(f"loop_id: {loop_id}, input_idx: {input_idx} | Callback called", logskip=True)
        
        return 0

    def process_model(self, model_path: str, loops: int) -> bool:
        self.stats = TestStatistics()  # Reset statistics
        
        model_files = glob.glob(os.path.join(model_path, "*.dxnn"))
        if not model_files:
            self.log(f"No .dxnn files found in '{model_path}'", 'error')
            return False

        ie = InferenceEngine(model_files[0])
        

        
        self.compile_type = ie.get_compile_type()
        self.log(f"COMPILE TYPE : {self.compile_type}", logskip=True)
        task_order = ie.task_order()
        self.config.use_ort = any("cpu" in task for task in task_order)
    
        #For Debug
        if ie.get_num_tails() > 1 :
            message = (f"{model_path} | MULT | {0} | "
                  f"{0} | {task_order} | {ie.get_num_tails()}"
                  f" | {0} | "
                  f"{0}")
            self.log(message)

            return False

        self._load_data(model_path, ie)

        if self.compile_type == "RELEASE" and (ie.output_size() < self.gt_outputs[0]["LAST"].size):
            if self.config.use_ort:
                self.log(f"The output size is not the same. ({model_files[0]})", 'error')
                return 0

        ie.RegisterCallBack(self.callback_handler)

        if self.compile_type == "RELEASE":
            '''
            if self.config.batch_mode:
                self._run_batch_mode(ie, loops)
            else:
                self._run_async_mode(ie, loops)
            '''
            self._run_async_mode(ie, loops)
        elif self.compile_type == "DEBUG":
            self._run_validation_mode(ie, loops)
        else:
            self.log(f"Unknown Compile Type : {self.compile_type} ('RELEASE' or 'DEBUG')", 'error')
            return False

        success = self.stats.pass_count == self.stats.total_count
        self._log_results(model_path, task_order, ie.get_num_tails())
        
        return success

    def _load_gt_outputs(self, model_path: str) -> List[Dict[str, np.ndarray]]:
        grouped_outputs = defaultdict(
            lambda: {'NORMAL': np.array([]), 'PPU': np.array([]), 
                    'ARGMAX': np.array([]), 'LAST': np.array([])}
        )

        self.log("GT file list:", logskip=True)
        if self.compile_type == "RELEASE" and self.config.use_ort:
            gt_pattern = "gt/output_*.bin"
        else:
            gt_pattern = "gt/npu_0_output_*.bin"
        
        gt_files = sorted(glob.glob(os.path.join(model_path, gt_pattern)))

        for gt_file in gt_files:
            filename = os.path.basename(gt_file)
            try:
                key = int(filename.split("_")[-1].split(".")[0])
            except ValueError:
                continue

            with open(gt_file, "rb") as f:
                data = np.frombuffer(f.read(), dtype=np.int8)
                
                if 'ppu' in gt_file:
                    grouped_outputs[key]["PPU"] = data
                    self.log(f" - {gt_file} (PPU) : {data.shape} {data[:10]}", logskip=True)
                elif 'argmax' in gt_file:
                    grouped_outputs[key]["ARGMAX"] = data
                    self.log(f" - {gt_file} (ARGMAX) : {data.shape} {data[:10]}", logskip=True)
                elif 'pu_' in gt_file:
                    grouped_outputs[key]["NORMAL"] = data
                    self.log(f" - {gt_file} (NORMAL) : {data.shape} {data[:10]}", logskip=True)
                    if self.compile_type == "DEBUG" and grouped_outputs[key]["LAST"].size == 0:
                        grouped_outputs[key]["LAST"] = data
                else:
                    grouped_outputs[key]["LAST"] = data
                    self.log(f" - {gt_file} (LAST) : {data.shape} {data[:10]}", logskip=True)

            if self.compile_type == "RELEASE" and grouped_outputs[key]["LAST"].size == 0:
                grouped_outputs[key]["LAST"] = data
                self.log(f" - {gt_file} (LAST) : {data.shape} {data[:10]}", logskip=True)
                
        return [grouped_outputs[key] for key in sorted(grouped_outputs.keys())]

    def _load_data(self, model_path: str, ie: InferenceEngine):
        if self.compile_type == "RELEASE" and self.config.use_ort:
            input_pattern = "gt/input_*.bin"
        else:
            input_pattern = "gt/npu_0_input_*.bin"
        if self.compile_type == "DEBUG" and self.config.use_ort:
            self.log(f"DEBUG.dxnn bitmatch with USE_ORT is not supported. (Bitmatch only for the NPU Task in DEBUG.dxnn.)", 'warning')

        self.input_files = sorted(glob.glob(os.path.join(model_path, input_pattern)))
        self.input_data_list = []
        
        for input_file in self.input_files:
            with open(input_file, "rb") as f:
                data = np.frombuffer(f.read(), dtype=np.int8)
                self.input_data_list.append([data])
                self.log(f"Loaded {input_file}: {data[:10]}", logskip=True)

        self.gt_outputs = self._load_gt_outputs(model_path)

        if self.compile_type == "RELEASE" and self.config.use_ort:
            self.masks = [np.array([])]
        else:
            self.masks = [ie.bitmatch_mask(0)]

    '''
    def _run_batch_mode(self, ie: InferenceEngine, loops: int):
        for batch_idx in range(0, loops, self.config.iterations):
            batch_size = min(self.config.iterations, loops - batch_idx)
            input_idxs = np.random.randint(len(self.input_data_list), size=batch_size)
            
            batch_input = [self.input_data_list[idx] for idx in input_idxs]
            batch_args = [(i + batch_idx, idx) for i, idx in enumerate(input_idxs)]
            
            start_time = time.time()
            batch_outputs = ie.run_batch(batch_input, user_args=batch_args)
            self.stats.execution_times.append((time.time() - start_time)*1000)
            
            self._process_batch_results(batch_outputs, input_idxs)
    '''
    def _run_async_mode(self, ie: InferenceEngine, loops: int):
        start_time = time.time()
        for loop in range(loops):
            input_idx = np.random.randint(len(self.input_data_list))
            input_data = self.input_data_list[input_idx]
            
            job_id = ie.RunAsync(input_data, user_arg=(loop, input_idx))
            self.log(f"Loop {loop} is requested with input {input_idx}", logskip=True)                
            self.result_queue.put(job_id)
            
        self.result_queue.join()
        self.stats.execution_times.append((time.time() - start_time)*1000)

    def _run_validation_mode(self, ie: InferenceEngine, loops: int):
        validation_outputs = []
        input_idxs = []
        start_time = time.time()
        
        for loop in range(loops):
            input_idx = np.random.randint(len(self.input_data_list))
            input_idxs.append(input_idx)
            input_data = self.input_data_list[input_idx]
            
            validation_outputs.append(ie.ValidateDevice(input_data, 0))
        self._process_batch_results(validation_outputs, input_idxs)
        self.stats.execution_times.append((time.time() - start_time)*1000)

    def _process_batch_results(self, batch_outputs: List[List[np.ndarray]], input_idxs: List[int]):
        for idx, outputs in enumerate(batch_outputs):
            input_idx = input_idxs[idx]
            ie_outputs = np.frombuffer(b''.join(output.tobytes() for output in outputs), dtype=np.int8)

            result, ie_outputs_masked, gt_output_masked = self.bitmatch_logic(
                                                        ie_outputs, 
                                                        self.gt_outputs[input_idx]["LAST"],
                                                        self.masks[0]
                                                    )
            if result == "PASS" and self.gt_outputs[input_idx]["PPU"].size > 0 :
                result, ie_outputs_masked_ppu, gt_output_masked_ppu = self.bitmatch_logic(
                                                            ie_outputs[-128*1024:-128*1024+self.gt_outputs[input_idx]["PPU"].size], 
                                                            self.gt_outputs[input_idx]["PPU"],
                                                            np.array([])
                                                        )   

            self.log(f"Loop {idx} | Input ID: {input_idx} | Result: {result}", logskip=True)
            
            self.stats.total_count += 1
            if result == "PASS":
                self.stats.pass_count += 1
            else:
                self._handle_mismatch(ie_outputs_masked, gt_output_masked, idx)

    def _log_results(self, model_path: str, task_order: List[str], num_tails: int):
        result = "PASS" if self.stats.pass_count == self.stats.total_count else "FAIL"
        latency = np.mean(self.stats.execution_times)
        message = (f"{model_path} | {result} | {self.stats.pass_count} | "
                  f"{self.stats.total_count} | {task_order} | {num_tails}"
                  f" | {latency} | "
                  f"{self.stats.total_count*1000/latency}")

        self.model_result_messages.append(message)
        self.log(message)

    def log_all_results(self,test_pass_count, test_total_count, failed_models):
        self.config.verbose = True
        self.log("========================================================================================")
        self.log("     MODEL PATH     | RESULT | PASS | TOTAL | TASK ORDER | TAIL | MEAN(ms) | FPS")
        self.log("----------------------------------------------------------------------------------------")
        for messeage in self.model_result_messages:
            self.log(messeage)
        self.log("========================================================================================")
        self.log(f"Pass Count: {test_pass_count} / Total Count: {test_total_count}")

        if failed_models:
            self.log("Failed Models:")
            for model in failed_models:
                self.log(f" - Model: {model['model']}")
                if model['fail_count'] == 0:
                    self.log(f" - Inference ERROR occurred\n")
                else:
                    self.log(f" - Failures: {model['fail_count']}/{model['total_count']}")
                    self.log(f" - Failed Jobs: {model['failed_jobs']}\n")
        else:
            self.log("All PASS")

def main():
    parser = argparse.ArgumentParser(description="Bitmatch Test Script")
    parser.add_argument("--model_path", "-m", type=str, help="Model directory path")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Number of test loops")
    parser.add_argument("--dir", "-d", type=str, help="Directory with model subdirectories")
    parser.add_argument("--batch", "-b", action='store_true', help="Enable batch mode")
    parser.add_argument("--itr", "-i", type=int, default=1, help="Batch iterations")
    parser.add_argument("--verbose", "-v", action='store_true', help="Enable verbose output")
    parser.add_argument("--no-logging", action='store_true', help="Disable file logging")
    parser.add_argument("--debug", type=str, required=False, help="RT debug options, 0(defualt): Do not generate RT binaries. / 1: Generate RT binaries for debug. / 2: Generate RT binaries for release.", default="0")
    
    args = parser.parse_args()
    
    os.environ['DXRT_DEBUG_DATA'] = args.debug

    config = TestConfig(
        batch_mode=args.batch,
        iterations=args.itr,
        verbose=args.verbose,
        log_enabled=not args.no_logging
    )
    
    tester = BitMatchTester(config)
    failed_models = []

    test_total_count = 0
    test_pass_count = 0 

    if args.dir:
        model_dirs = [d for d in glob.glob(os.path.join(args.dir, "*")) 
                     if os.path.isdir(d)]
        for model_dir in model_dirs:
            if not tester.process_model(model_dir, args.loops):
                failed_models.append({
                    'model': model_dir,
                    'total_count': tester.stats.total_count,
                    'fail_count': tester.stats.total_count - tester.stats.pass_count,
                    'failed_jobs': tester.stats.failed_jobs
                })
            test_total_count += tester.stats.total_count
            test_pass_count += tester.stats.pass_count
    elif args.model_path:
        args.model_path = os.path.dirname(args.model_path) + "/"
        if not tester.process_model(args.model_path, args.loops):
            failed_models.append({
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