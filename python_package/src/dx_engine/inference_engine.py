from typing import List, Any
import ctypes
import warnings
import numpy as np

import dx_engine.capi._pydxrt as C
from dx_engine.dtype import NumpyDataTypeMapper
from dx_engine.utils import ensure_contiguous
from dx_engine.inference_option import InferenceOption

class InferenceEngine:
    def __init__(
        self,
        model_path: str,
        inference_option=InferenceOption()
    ) -> None:
        self.engine = C.InferenceEngine(model_path, inference_option.instance)

    def run(self, input_feed_list: List[np.ndarray]) -> List[np.ndarray]:
        """Return normal inference result"""
        input_feed_list = ensure_contiguous(input_feed_list)
        return C.run(self.engine, input_feed_list)

    def Run(self, input_feed_list: List[np.ndarray]):
        """@deprecated: Use `run()` instead."""
        warnings.warn("Run() is deprecated. Use run() instead.", DeprecationWarning, stacklevel=2)
        return self.run(input_feed_list)

    def run_async(self, input_feed_list: List[np.ndarray], user_arg) -> int:
        """Run inference asynchronously."""
        input_feed_list = ensure_contiguous(input_feed_list)
        user_arg_capsule = ctypes.py_object(user_arg)
        return C.run_async(self.engine, input_feed_list, user_arg_capsule)

    def RunAsync(self, input_feed_list: List[np.ndarray], user_arg):
        """@deprecated: Use `run_async()` instead."""
        warnings.warn("RunAsync() is deprecated. Use run_async() instead.", DeprecationWarning, stacklevel=2)
        return self.run_async(input_feed_list, user_arg)

    def run_benchmark(self, loop_cnt, input_feed_list) -> float:
        """Return benchmark result."""
        input_feed_list = ensure_contiguous(input_feed_list)
        return C.run_benchmark(self.engine, loop_cnt, input_feed_list)

    def RunBenchMark(self, loop_cnt, input_feed_list):
        """@deprecated: Use `run_benchmark()` instead."""
        warnings.warn("RunBenchMark() is deprecated. Use run_benchmark() instead.", DeprecationWarning, stacklevel=2)
        return self.run_benchmark(loop_cnt, input_feed_list)

    def validate_device(self, input_feed_list: List[np.ndarray], device_id=0) -> List[np.ndarray]:
        """Return device validation run result."""
        input_feed_list = ensure_contiguous(input_feed_list)
        return C.validate_device(self.engine, input_feed_list, device_id)

    def ValidateDevice(self, input_feed_list: List[np.ndarray], device_id=0):
        """@deprecated: Use `validate_device()` instead."""
        warnings.warn("ValidateDevice() is deprecated. Use validate_device() instead.", DeprecationWarning, stacklevel=2)
        return self.validate_device(input_feed_list, device_id)

    def register_callback(self, callback) -> None:
        """Register user callback function to be called upon inference completion."""
        if not callable(callback):
            raise TypeError("Expected a callable function for the callback")
        C.register_callback(self.engine, callback)

    def RegisterCallBack(self, callback):
        """@deprecated: Use `register_callback()` instead."""
        warnings.warn("RegisterCallBack() is deprecated. Use register_callback() instead.", DeprecationWarning, stacklevel=2)
        return self.register_callback(callback)

    def wait(self, req_id) -> None:
        """Wait for asynchronous operations to complete."""
        return C.wait(self.engine, req_id)

    def Wait(self, req_id):
        """@deprecated: Use `wait()` instead."""
        warnings.warn("Wait() is deprecated. Use wait() instead.", DeprecationWarning, stacklevel=2)
        return self.wait(req_id)

    def get_input_size(self) -> int:
        """Get engine's input size."""
        return self.engine.get_input_size()

    def input_size(self):
        """@deprecated: Use `get_input_size()` instead."""
        warnings.warn("input_size() is deprecated. Use get_input_size() instead.", DeprecationWarning, stacklevel=2)
        return self.get_input_size()

    def get_output_size(self) -> int:
        """Get engine's output size."""
        return self.engine.get_output_size()

    def output_size(self):
        """@deprecated: Use `get_output_size()` instead."""
        warnings.warn("output_size() is deprecated. Use get_output_size() instead.", DeprecationWarning, stacklevel=2)
        return self.get_output_size()

    def get_task_order(self) -> np.ndarray:
        """Get task order array in .dxnn file"""
        task_order = np.array(C.get_task_order(self.engine))
        return task_order

    def task_order(self):
        """@deprecated: Use `get_task_order()` instead."""
        warnings.warn("task_order() is deprecated. Use get_task_order() instead.", DeprecationWarning, stacklevel=2)
        return self.get_task_order()

    def get_latency(self):
        """Get latency in microseconds."""
        return self.engine.get_latency()

    def latency(self):
        """@deprecated: Use `get_latency()` instead."""
        warnings.warn("latency() is deprecated. Use get_latency() instead.", DeprecationWarning, stacklevel=2)
        return self.get_latency()

    def get_npu_inference_time(self):
        """Get latency in microseconds."""
        return self.engine.get_npu_inference_time()

    def inference_time(self):
        """@deprecated: Use `get_npu_inference_time()` instead."""
        warnings.warn("inference_time() is deprecated. Use get_npu_inference_time() instead.", DeprecationWarning, stacklevel=2)
        return self.engine.get_npu_inference_time()

    def get_latency_list(self):
        """Get list of latency values."""
        return self.engine.get_latency_list()

    def get_npu_inference_time_list(self):
        """Get list of NPU inference times."""
        return self.engine.get_npu_inference_time_list()

    def get_latency_mean(self):
        """Get mean latency."""
        return self.engine.get_latency_mean()

    def get_npu_inference_time_mean(self):
        """Get mean NPU inference time."""
        return self.engine.get_npu_inference_time_mean()

    def get_latency_std(self):
        """Get standard deviation of latency."""
        return self.engine.get_latency_std()

    def get_npu_inference_time_std(self):
        """Get standard deviation of NPU inference time."""
        return self.engine.get_npu_inference_time_std()

    def get_latency_count(self):
        """Get count of latency measurements."""
        return self.engine.get_latency_count()

    def get_npu_inference_time_count(self):
        """Get count of NPU inference time measurements."""
        return self.engine.get_npu_inference_time_count()

    def get_input_data_type(self) -> List[str]:
            """Get required input data type as a list of strings."""
            return C.get_input_dtype(self.engine)

    def input_dtype(self):
        """@deprecated: Use `get_input_data_type()` instead."""
        warnings.warn("input_dtype() is deprecated. Use get_input_data_type() instead.", 
                      DeprecationWarning, stacklevel=2)
        return self.get_input_data_type()
    
    def get_output_data_type(self) -> List[str]:
        """Get required output data type as a list of strings."""
        return C.get_output_dtype(self.engine)
    
    def output_dtype(self):
        """@deprecated: Use `get_output_data_type()` instead."""
        warnings.warn("output_dtype() is deprecated. Use get_output_data_type() instead.", 
                      DeprecationWarning, stacklevel=2)
        return self.get_output_data_type()

    def get_all_task_outputs(self) -> List[List[np.ndarray]]:
        """Get outputs from all tasks in order."""
        return C.get_all_task_outputs(self.engine)

    def get_outputs(self):
        """@deprecated: Use `get_all_task_outputs()` instead."""
        warnings.warn("get_outputs() is deprecated. Use get_all_task_outputs() instead.", DeprecationWarning, stacklevel=2)
        return self.get_all_task_outputs()

    def get_bitmatch_mask(self, index=0) -> np.ndarray:
        """Get bit match mask array."""
        mask_list = np.array(self.engine.get_bitmatch_mask(index), dtype=np.uint8)
        return np.unpackbits(mask_list, bitorder="little")

    def bitmatch_mask(self, index=0):
        """@deprecated: Use `get_bitmatch_mask()` instead."""
        warnings.warn("bitmatch_mask() is deprecated. Use get_bitmatch_mask() instead.", DeprecationWarning, stacklevel=2)
        return self.get_bitmatch_mask(index)

    def get_num_tail_tasks(self):
        """Get the number of tail tasks."""
        return self.engine.get_num_tail_tasks()

    def get_num_tails(self):
        """@deprecated: Use `get_num_tail_tasks()` instead."""
        warnings.warn("get_num_tails() is deprecated. Use get_num_tail_tasks() instead.", DeprecationWarning, stacklevel=2)
        return self.get_num_tail_tasks()

    def get_compile_type(self):
        """Get model compile type."""
        return self.engine.get_compile_type()

    def is_ppu(self):
        """Check if PPU is utilized."""
        return self.engine.is_ppu()

    def is_PPU(self):
        """@deprecated: Use `is_ppu()` instead."""
        warnings.warn("is_ppu() is deprecated. Use is_ppu() instead.", DeprecationWarning, stacklevel=2)
        return self.is_ppu()
    
    def parse_model(self, file: str):
        """Show model information"""
        return C.parse_model(self.engine, file)
    
    # for using 'with' statement
    def __enter__(self):
        C.inference_engine_enter(self.engine)
        return self
    
    # for using 'with' statement
    def __exit__(self, exc_type, exc_value, traceback):
        C.inference_engine_exit(self.engine)
        return True