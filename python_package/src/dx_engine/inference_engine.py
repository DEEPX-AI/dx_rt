from typing import List, Any
import ctypes

import numpy as np

import dx_engine.capi._pydxrt as C
from dx_engine.dtype import NumpyDataTypeMapper


def raise_not_implemented_error() -> None:
    NOT_IMPLEMENTED_ERROR_MSG = "Not Pybinded Yet"
    raise NotImplementedError(NOT_IMPLEMENTED_ERROR_MSG)


class InferenceEngine:
    def __init__(
        self,
        model_path: str,
        inference_option=None,  # TODO: Will be used after pybind complete
    ) -> None:
        self.model_path = model_path
        self.inference_option = inference_option
        self.engine = C.InferenceEngine(model_path)

    def Run(self, input_feed_list: List[np.ndarray]) -> List[np.ndarray]:
        """Return Normal Inference Result.

        Args:
            input_feed_list (List[np.ndarray]): List of Numpy array.

        Returns:
            List[np.ndarray]
        """
        return C.run_engine(self.engine, input_feed_list)
    
    def RunAsync(self, input_feed_list: List[np.ndarray], user_arg) -> int:
        """Run inference asynchronously."""
        user_arg_capsule = ctypes.py_object(user_arg)
        reqId = C.run_async_engine(self.engine, input_feed_list, user_arg_capsule)
        return reqId
    
    def RunBenchMark(self, loop_cnt=30) -> float:
        """Retrun Benchmark result.

        Returns:
            float: fps
        """
        return self.engine.benchmark(loop_cnt)
    
    def ValidateDevice(self, input_feed_list: List[np.ndarray], device_id=0) -> List[np.ndarray]:
        """Return Device validation run result.

        Args:
            input_feed_list (List[np.ndarray]): List of Numpy array.

        Returns:
            List[np.ndarray]
        """
        return C.validate_device(self.engine, input_feed_list, device_id)

    def RegisterCallBack(self, callback) -> None:
        """Register user callback function to be called by inference completion."""
        if not callable(callback):
            raise TypeError("Expected a callable function for the callback")
        self.engine.register_callback(callback) 

    def Wait(self, reqId) -> None:
        """Wait for asynchronous operations to complete."""
        return C.wait(self.engine,reqId)

    def input_size(self) -> List[int]:
        """Get engine's input size."""
        return self.engine.input_size()

    def output_size(self) -> List[int]:
        """Get engine's output size."""
        return self.engine.output_size()

    def input_dtype(self) -> List[str]:
        """Get required input data-type as string"""
        return C.input_dtype(self.engine)

    def output_dtype(self) -> List[str]:
        """Get required output data-type as string"""
        return C.output_dtype(self.engine)

    def task_order(self) -> np.ndarray:
        """Get task order array in single file"""
        task_order = np.array(C.task_order(self.engine))
        return task_order

    def latency(self):
        return self.engine.latency()

    def inference_time(self):
        return self.engine.inf_time()
    
    def get_outputs(self) -> List[List[np.ndarray]]:
        """Get outputs from all tasks in order.

        Returns:
            List[List[np.ndarray]]: List of outputs, each output is a list of numpy arrays.
        """
        return C.get_outputs(self.engine)

    def bitmatch_mask(self, index=0) -> np.ndarray:
        """Get bit match mask array in single file"""
        mask_list = np.array(self.engine.bitmatch_mask(index), dtype=np.uint8)
        mask = np.unpackbits(mask_list, bitorder="little")
        return mask

    def get_num_tails(self):
        return self.engine.get_num_tails()
    
    def get_compile_type(self):
        return self.engine.get_compile_type()