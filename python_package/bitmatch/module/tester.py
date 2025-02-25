import os
import glob
import time
import queue
import logging
import warnings
import threading
from collections import defaultdict
from typing import List, Dict, Any, Optional, Tuple
from concurrent.futures import ThreadPoolExecutor
import ctypes

import numpy as np

# dx_engine 모듈은 원래 코드에 의존하므로 그대로 사용합니다.
from dx_engine import InferenceEngine

from .config import TestConfig
from .statistics import TestStatistics
from .utils import int8tofloat32, get_dxrt_info

class BitMatchTester:
    def __init__(self, config: TestConfig):
        self.config = config
        self.stats = TestStatistics()
        self.callback_lock = threading.Lock()
        self.result_queue = queue.Queue()
        self.model_result_messages = []
        self.compile_type = "RELEASE"
        self._setup_logging()
        self.match_executor = ThreadPoolExecutor(max_workers=4)

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

    def log(self, message: str, level: str = 'info', logskip: bool = False):
        if self.config.verbose or level == 'error':
            print(f"[{level}] {message}")
        if self.config.log_enabled and not logskip:
            getattr(logging, level)(message)

    def bitmatch_logic(self, ie_output: np.ndarray, gt_output: np.ndarray, mask: Optional[np.ndarray] = None) -> Tuple[str, np.ndarray, np.ndarray]:
        if mask is not None and mask.nbytes > 0:
            if mask.shape != ie_output.shape:
                warnings.warn(f"Size mismatch Runtime output: {ie_output.shape} vs mask: {mask.shape}, (gt : {gt_output.shape})")
            ie_output = np.where(mask, ie_output[:mask.shape[0]], 0)
            gt_output = np.where(mask, gt_output[:mask.shape[0]], 0)

        if np.array_equal(ie_output, gt_output):
            return "PASS", ie_output, gt_output

        return "FAIL", ie_output, gt_output

    def _handle_mismatch(self, ie_outputs_masked: np.ndarray, gt_output_masked: np.ndarray, loop_id: int): 
        if gt_output_masked.shape != ie_outputs_masked.shape:
            self.log(f"Size mismatch ie_outputs: {ie_outputs_masked.shape} vs gt_output_masked: {gt_output_masked.shape}")
            self.stats.failed_jobs.append(loop_id)
            return 1
        if len(self.stats.failed_jobs) < 10:
            different_indices = np.where(ie_outputs_masked != gt_output_masked)[0]
            first_10_diff_indices = different_indices[:10]
            first_10_diff_values = [(ie_outputs_masked[i], gt_output_masked[i]) 
                                    for i in first_10_diff_indices]

            self.log(f"Indices of differences (first 10): {first_10_diff_indices}")
            self.log(f"Total number of differences: {len(different_indices)}")
            self.log(f"Different values (first 10): {first_10_diff_values}")
        self.stats.failed_jobs.append(loop_id)
    '''
    def callback_handler(self, outputs: List[np.ndarray], user_arg: Any):
        with self.callback_lock:
            # user_arg.value를 사용하는 부분은 원래 코드 커버리지를 유지하기 위한 것으로 가정합니다.
            loop_id, input_idx = user_arg.value
            if not self.config.batch_mode:
                ie_outputs = np.frombuffer(b''.join(output.tobytes() for output in outputs), dtype=np.int8)
                gt_output = self.gt_outputs[input_idx]

                result, ie_outputs_masked, gt_output_masked = self.bitmatch_logic(
                    ie_outputs, gt_output["LAST"], self.masks[0]
                )
                
                self.log(f"loop_id: {loop_id}, input_idx: {input_idx} | Result: {result}", logskip=True)
                
                self.stats.total_count += 1
                if result == "PASS":
                    self.stats.pass_count += 1
                else:
                    self._handle_mismatch(ie_outputs_masked, gt_output_masked, loop_id)
                
                self.result_queue.get(timeout=5)
                self.result_queue.task_done()
            else:
                self.log(f"loop_id: {loop_id}, input_idx: {input_idx} | Callback called", logskip=True)
        
        return 0
    '''
    def callback_handler(self, outputs: List[np.ndarray], user_arg: Any):
        # user_arg.value에는 (loop_id, input_idx)가 들어있다고 가정
        loop_id, input_idx = user_arg.value
        #self.log(f"loop_id: {loop_id}, input_idx: {input_idx} | Callback called ({hex(id(outputs))} - {hex(outputs[0].ctypes.data)})", logskip=True)
        self.log(f"loop_id: {loop_id}, input_idx: {input_idx})", logskip=True)

        # 별도 쓰레드풀에 비트매치 작업을 제출
        self.match_executor.submit(self._process_output, outputs, user_arg)
        return 0
    
    def _process_output(self, outputs: List[np.ndarray], user_arg: Any):
        loop_id, input_idx = user_arg.value
        #self.log(f"Processe bitmatch for loop_id: {loop_id}, input_idx: {input_idx} ({hex(id(outputs))} - {hex(outputs[0].ctypes.data)})", logskip=True)
        self.log(f"Processe bitmatch for loop_id: {loop_id}, input_idx: {input_idx}", logskip=True)

        # np.frombuffer는 byte-level 복사가 아니라 재해석이므로, 출력 버퍼는 이미 복사되었으므로 안전함
        ie_outputs = np.frombuffer(b''.join(output.tobytes() for output in outputs), dtype=np.int8)

        gt_output = self.gt_outputs[input_idx]
        result, ie_outputs_masked, gt_output_masked = self.bitmatch_logic(
            ie_outputs, gt_output["LAST"], self.masks[0]
        )
        self.log(f"Processed bitmatch for loop_id: {loop_id}, input_idx: {input_idx} | Result: {result}", logskip=True)
        self.stats.total_count += 1
        if result == "PASS":
            self.stats.pass_count += 1
        else:
            self._handle_mismatch(ie_outputs_masked, gt_output_masked, loop_id)
        # result_queue 항목 소비 (각 요청에 대해 반드시 하나씩 소비되어야 함)
        try:
            self.result_queue.get(timeout=5)
            self.result_queue.task_done()
        except queue.Empty:
            self.log(f"Timeout waiting on result_queue for loop_id: {loop_id}", level="error")

    def process_model(self, model_path: str, loops: int) -> bool:
        self.stats = TestStatistics()  # 통계 리셋

        model_files = glob.glob(os.path.join(model_path, "*.dxnn"))
        if not model_files:
            self.log(f"No .dxnn files found in '{model_path}'", 'error')
            message = (f"{model_path} | NOTFOUND | {0} | "
                       f"{0} | - | {0}"
                       f" | {0} | "
                       f"{0}")
            self.log(message)
            self.model_result_messages.append(message)
            return False

        ie = InferenceEngine(model_files[0])
        self.compile_type = ie.get_compile_type()
        self.log(f"COMPILE TYPE : {self.compile_type}", logskip=True)
        task_order = ie.get_task_order()
        self.config.use_ort = any("cpu" in task for task in task_order)
    
        # 데이터 로드
        self._load_data(model_path, ie)
        if not ie.is_ppu():
            if self.compile_type == "RELEASE" and (ie.get_output_size() < self.gt_outputs[0]["LAST"].size):
                if self.config.use_ort:
                    self.log(f"The output size is not the same. ({model_files[0]}) - ie.output_size : {ie.get_output_size()}, gt : {self.gt_outputs[0]['LAST'].size}", 'error')
                    message = (f"{model_path} | OUTSIZE | {0} | "
                               f"{0} | {task_order} | {ie.get_num_tail_tasks()}"
                               f" | {0} | "
                               f"{0}")
                    self.log(message)
                    self.model_result_messages.append(message)
                    return False

        ie.register_callback(self.callback_handler)

        if self.compile_type == "RELEASE":
            # 배치 모드 주석 처리되어 있으므로 async 모드만 호출
            self._run_async_mode(ie, loops)
        elif self.compile_type == "DEBUG":
            self._run_validation_mode(ie, loops)
        else:
            self.log(f"Unknown Compile Type : {self.compile_type} ('RELEASE' or 'DEBUG')", 'error')
            message = (f"{model_path} | COMTYPE | {0} | "
                       f"{0} | {task_order} | {ie.get_num_tail_tasks()}"
                       f" | {0} | "
                       f"{0}")
            self.log(message)
            self.model_result_messages.append(message)
            return False

        success = self.stats.pass_count == self.stats.total_count
        self._log_results(model_path, task_order, ie.get_num_tail_tasks())
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
                        self.log(f" - {gt_file} (LAST) : {data.shape} {data[:10]}", logskip=True)
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
            self.masks = [ie.get_bitmatch_mask(0)]

    '''
    def _run_async_mode(self, ie: InferenceEngine, loops: int):
        start_time = time.time()
        for loop in range(loops):
            input_idx = np.random.randint(len(self.input_data_list))
            input_data = self.input_data_list[input_idx]
            
            job_id = ie.run_async(input_data, user_arg=(loop, input_idx))
            self.log(f"Loop {loop} is requested with input {input_idx}", logskip=True)                
            self.result_queue.put(job_id)
            
        self.result_queue.join()
        if len(self.stats.execution_times) > 1:
            print("ERROR execution_times size > 1")
            exit(1)
        self.stats.execution_times.append((time.time() - start_time)*1000)
        
        latency_mean = ie.GetLatencyMean()
        latency_std = ie.get_latency_std()
        latency_CV = latency_std/latency_mean
        
        inf_time_mean = ie.get_npu_inference_time_mean()
        inf_time_std = ie.get_npu_inference_time_std()
        inf_time_CV = inf_time_std/inf_time_mean
        
        self.stats.latency_mean = latency_mean
        self.stats.latency_stddev = latency_std
        self.stats.latency_CV = latency_CV

        self.stats.inf_time_mean = inf_time_mean
        self.stats.inf_time_stddev = inf_time_std
        self.stats.inf_time_CV = inf_time_CV
        
        if int(os.environ.get('DXRT_DEBUG_DATA', '0')) > 0:
            print("Waiting to save data")
            time.sleep(3)
    '''

    def _run_async_mode(self, ie: InferenceEngine, loops: int):
        start_time = time.time()
        for loop in range(loops):
            input_idx = np.random.randint(len(self.input_data_list))
            input_data = self.input_data_list[input_idx]
            
            job_id = ie.run_async(input_data, user_arg=(loop, input_idx))
            self.log(f"Loop {loop} is requested with input {input_idx}", logskip=True)                
            self.result_queue.put(job_id)
            
        # wait for all async jobs to complete (각 callback에서 task_done을 호출)
        self.result_queue.join()
        if len(self.stats.execution_times) > 1:
            print("ERROR execution_times size > 1")
            exit(1)
        self.stats.execution_times.append((time.time() - start_time)*1000)
        
        latency_mean = ie.get_latency_mean()
        latency_std = ie.get_latency_std()
        latency_CV = latency_std / latency_mean if latency_mean else 0
        
        inf_time_mean = ie.get_npu_inference_time_mean()
        inf_time_std = ie.get_npu_inference_time_std()
        inf_time_CV = inf_time_std / inf_time_mean if inf_time_mean else 0
        
        self.stats.latency_mean = latency_mean
        self.stats.latency_stddev = latency_std
        self.stats.latency_CV = latency_CV

        self.stats.inf_time_mean = inf_time_mean
        self.stats.inf_time_stddev = inf_time_std
        self.stats.inf_time_CV = inf_time_CV
        
        if int(os.environ.get('DXRT_DEBUG_DATA', '0')) > 0:
            print("Waiting to save data")
            time.sleep(3)

    def _run_validation_mode(self, ie: InferenceEngine, loops: int):
        validation_outputs = []
        input_idxs = []
        start_time = time.time()
        
        for loop in range(loops):
            input_idx = np.random.randint(len(self.input_data_list))
            input_idxs.append(input_idx)
            input_data = self.input_data_list[input_idx]
            
            validation_outputs.append(ie.validate_device(input_data, 0))
        self._process_batch_results(validation_outputs, input_idxs)
        self.stats.execution_times.append((time.time() - start_time)*1000)

    def _process_batch_results(self, batch_outputs: List[List[np.ndarray]], input_idxs: List[int]):
        for idx, outputs in enumerate(batch_outputs):
            input_idx = input_idxs[idx]
            ie_outputs = np.frombuffer(b''.join(output.tobytes() for output in outputs), dtype=np.int8)
            gt_output = self.gt_outputs[input_idx]

            if gt_output["PPU"].size > 0:
                all_mask = np.concatenate([
                    self.masks[0],
                    np.ones(len(gt_output["PPU"]), dtype=np.int8),
                    np.zeros((128*1024 - len(gt_output["PPU"])), dtype=np.int8)
                ])
                all_gt = np.concatenate([
                    gt_output["LAST"],
                    gt_output["PPU"],
                    np.zeros((128*1024 - len(gt_output["PPU"])), dtype=np.int8)
                ])
                
                result, ie_outputs_masked, gt_output_masked = self.bitmatch_logic(
                    ie_outputs, all_gt, all_mask
                )
            else:
                result, ie_outputs_masked, gt_output_masked = self.bitmatch_logic(
                    ie_outputs, 
                    gt_output["LAST"],
                    self.masks[0]
                )

            self.log(f"Loop {idx} | Input ID: {input_idx} | Result: {result}", logskip=True)
            
            self.stats.total_count += 1
            if result == "PASS":
                self.stats.pass_count += 1
            else:
                self._handle_mismatch(ie_outputs_masked, gt_output_masked, idx)

    def _log_results(self, model_path: str, task_order: List[str], num_tails: int):
        result = "PASS" if self.stats.pass_count == self.stats.total_count else "FAIL"
        duration = self.stats.execution_times[-1]
        latency_mean = self.stats.latency_mean
        latency_std = self.stats.latency_stddev
        latecny_cv = self.stats.latency_CV
        inf_time_mean = self.stats.inf_time_mean
        inf_time_std = self.stats.inf_time_stddev
        inf_time_cv = self.stats.inf_time_CV

        dxrt_data = get_dxrt_info()
        #print(dxrt_data)

        model_name = model_path.split("/")[-1] 
        message = (f"{model_path} | {model_name} | {result} | {self.stats.pass_count} | "
                   f"{self.stats.total_count} | {task_order} | {num_tails} | "
                   f"{duration:.2f} | "
                   f"{self.stats.total_count*1000/duration:.2f} | "
                   f"{latency_mean:.2f} | "
                   f"{latecny_cv:.2f} | "
                   f"{latency_std:.2f} | "
                   f"{inf_time_mean:.2f} | "
                   f"{inf_time_cv:.2f} | "
                   f"{inf_time_std:.2f} | "
                   f"{dxrt_data['memory_speed_MHz']:.2f} | "
                   f"{dxrt_data['pcie']} | "
                   f"{dxrt_data['npus'][0]['voltage_mV']:.2f} | "
                   f"{dxrt_data['npus'][0]['clock_MHz']:.2f} | "
                   f"{dxrt_data['npus'][0]['temperature_C']:.2f} | "
                   f"{dxrt_data['npus'][1]['voltage_mV']:.2f} | "
                   f"{dxrt_data['npus'][1]['clock_MHz']:.2f} | "
                   f"{dxrt_data['npus'][1]['temperature_C']:.2f} | "
                   f"{dxrt_data['npus'][2]['voltage_mV']:.2f} | "
                   f"{dxrt_data['npus'][2]['clock_MHz']:.2f} | "
                   f"{dxrt_data['npus'][2]['temperature_C']:.2f}")

        self.model_result_messages.append(message)
        self.log(message)
        print(message)
        self.stats.execution_times = []

    def log_all_results(self, test_pass_count: int, test_total_count: int, failed_models: List[Dict[str, Any]]):
        self.config.verbose = True
        self.log("=" * 150)
        self.log(" MODEL PATH | MODEL NAME | RESULT | PASS | TOTAL | TASK ORDER | TAIL | DURATION (ms) | FPS |\
                  LATENCY MEAN (us) | LATENCY CV | LATENCY STD | NPU INF MEAN (us) | NPU INF CV | NPU INF STD |\
                  MEM_CLK_MHz | PCIe | NPU_0_VOL_mV | NPU_0_CLK_MHz | NPU_0_TEMP_C | NPU_1_VOL_mV | NPU_1_CLK_MHz | NPU_1_TEMP_C | NPU_2_VOL_mV | NPU_2_CLK_MHz | NPU_2_TEMP_C |")
        self.log("-" * 150)

        model_summary = defaultdict(lambda: {
            "model_path": "",
            "result": [],
            "pass_count": 0,
            "total_count": 0,
            "task_order": None,
            "num_tails": None,
            "dur_sum": 0.0,
            "fps_sum": 0.0,
            "latency_mean_sum": 0.0,
            "latency_cv_sum": 0.0,
            "latency_std_sum": 0.0,
            "inf_time_mean_sum": 0.0,
            "inf_time_cv_sum": 0.0,
            "inf_time_std_sum": 0.0,
            "mem_clk": 0.0,
            "pcie":"",
            "n0_vol": 0.0,
            "n0_clk": 0.0,
            "n0_temp_sum": 0.0,
            "n1_vol": 0.0,
            "n1_clk": 0.0,
            "n1_temp_sum": 0.0,
            "n2_vol": 0.0,
            "n2_clk": 0.0,
            "n2_temp_sum": 0.0,
            "count": 0
        })

        for message in self.model_result_messages:
            parts = message.split(" | ")
            (model_path, model_name, result, pass_count, total_count, task_order,
             num_tails, dur, fps, l_mean, l_cv, l_std, i_mean, i_cv, i_std,
             mem_clk, pcie, n0_vol, n0_clk, n0_temp, n1_vol, n1_clk, n1_temp, n2_vol, n2_clk, n2_temp) = parts
            
            pass_count = int(pass_count)
            total_count = int(total_count)
            dur = float(dur)
            fps = float(fps)
            l_mean = float(l_mean)
            l_cv = float(l_cv)
            l_std = float(l_std)
            i_mean = float(i_mean)
            i_cv = float(i_cv)
            i_std = float(i_std)
            mem_clk = float(mem_clk)
            n0_vol = float(n0_vol)
            n0_clk = float(n0_clk)
            n0_temp = float(n0_temp)
            n1_vol = float(n1_vol)
            n1_clk = float(n1_clk)
            n1_temp = float(n1_temp)
            n2_vol = float(n2_vol)
            n2_clk = float(n2_clk)
            n2_temp = float(n2_temp)

            model_summary[model_name]["model_path"] = model_path
            model_summary[model_name]["result"].append(result)
            model_summary[model_name]["pass_count"] += pass_count
            model_summary[model_name]["total_count"] += total_count
            model_summary[model_name]["task_order"] = task_order
            model_summary[model_name]["num_tails"] = num_tails
            model_summary[model_name]["dur_sum"] += dur
            model_summary[model_name]["fps_sum"] += fps
            model_summary[model_name]["latency_mean_sum"] += l_mean
            model_summary[model_name]["latency_cv_sum"] += l_cv
            model_summary[model_name]["latency_std_sum"] += l_std
            model_summary[model_name]["inf_time_mean_sum"] += i_mean
            model_summary[model_name]["inf_time_cv_sum"] += i_cv
            model_summary[model_name]["inf_time_std_sum"] += i_std
            model_summary[model_name]["mem_clk"] = mem_clk
            model_summary[model_name]["pcie"] = pcie
            model_summary[model_name]["n0_vol"] = n0_vol
            model_summary[model_name]["n0_clk"] = n0_clk
            model_summary[model_name]["n0_temp_sum"] += n0_temp
            model_summary[model_name]["n1_vol"] = n1_vol
            model_summary[model_name]["n1_clk"] = n1_clk
            model_summary[model_name]["n1_temp_sum"] += n1_temp
            model_summary[model_name]["n2_vol"] = n2_vol
            model_summary[model_name]["n2_clk"] = n2_clk
            model_summary[model_name]["n2_temp_sum"] += n2_temp
            model_summary[model_name]["count"] += 1

        for model_name, data in model_summary.items():
            fail_count = data["result"].count("FAIL")
            pass_count = data["result"].count("PASS")
            if fail_count == 0:
                result_summary = "PASS"
            elif pass_count == 0:
                result_summary = "FAIL"
            else:
                result_summary = f"FAIL {fail_count} / PASS {pass_count}"

            dur_mean = data["dur_sum"] / data["count"]
            fps_mean = data["fps_sum"] / data["count"]
            latency_mean = data["latency_mean_sum"] / data["count"]
            latency_cv = data["latency_cv_sum"] / data["count"]
            latency_std = data["latency_std_sum"] / data["count"]
            inf_time_mean = data["inf_time_mean_sum"] / data["count"]
            inf_time_cv = data["inf_time_cv_sum"] / data["count"]
            inf_time_std = data["inf_time_std_sum"] / data["count"]
            n0_temp_mean = data["n0_temp_sum"] / data["count"]
            n1_temp_mean = data["n1_temp_sum"] / data["count"]
            n2_temp_mean = data["n2_temp_sum"] / data["count"]

            message = (f"{data['model_path']} | {model_name} | {result_summary} | {data['pass_count']} | {data['total_count']} | "
                       f"{data['task_order']} | {data['num_tails']} | {dur_mean:.2f} | {fps_mean:.2f} | "
                       f"{latency_mean:.2f} | {latency_cv:.2f} | {latency_std:.2f} | {inf_time_mean:.2f} | {inf_time_cv:.2f} | {inf_time_std:.2f} | "
                       f"{data['mem_clk']:.2f} | {data['pcie']} | {data['n0_vol']:.2f} | {data['n0_clk']:.2f} | {n0_temp_mean:.2f} | {data['n1_vol']:.2f} | {data['n1_clk']:.2f} | {n1_temp_mean:.2f} | {data['n2_vol']:.2f} | {data['n2_clk']:.2f} | {n2_temp_mean:.2f} ")
            self.log(message)

        self.log("=" * 110)
        self.log(f"Pass Count: {test_pass_count} / Total Count: {test_total_count}")

        if failed_models:
            self.log("Failed Models:")
            
            model_summary = defaultdict(lambda: {"fail_count": 0, "total_count": 0, "failed_jobs": []})

            for model in failed_models:
                model_name = model["model"]
                model_summary[model_name]["fail_count"] += model["fail_count"]
                model_summary[model_name]["total_count"] += model["total_count"]
                model_summary[model_name]["failed_jobs"].append(model["failed_jobs"])

            for model_name, data in model_summary.items():
                self.log(f"\n🔹 Model: {model_name}")
                if data["fail_count"] == 0:
                    self.log(f"   Inference ERROR occurred\n")
                else:
                    self.log(f"   Failures: {data['fail_count']} / {data['total_count']}")
                    self.log(f"   Failed Jobs:")
                    for itr, job in enumerate(data["failed_jobs"]):
                        self.log("   " + "-" * 40)
                        self.log(f"   - Test Itr {itr}: {job}")
            
            self.log("\n")
        else:
            self.log("All PASS")
