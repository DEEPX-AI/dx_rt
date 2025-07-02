from typing import List, Union, Any, Optional, Dict, Callable
import warnings
import numpy as np

try:
    import dx_engine.capi._pydxrt as C
except ImportError:
    raise ImportError(
        "Failed to import the C++ extension `_pydxrt`. "
        "Ensure it's compiled and in the Python path."
    ) from None

from dx_engine.dtype import NumpyDataTypeMapper 
from dx_engine.utils import ensure_contiguous
from dx_engine.inference_option import InferenceOption

class InferenceEngine:
    """
    DXRT Inference Engine Python wrapper.

    This class provides an interface to load a compiled model and perform
    inference tasks, either synchronously or asynchronously.
    It supports both single and batch inference.
    """
    def __init__(
        self,
        model_path: str,
        inference_option: Optional[InferenceOption] = None
    ) -> None:
        """
        Initializes the InferenceEngine.

        Args:
            model_path: Path to the compiled model file (e.g., *.dxnn).
            inference_option (Optional): Configuration options for inference.
                                         If None, a default InferenceOption instance is created.
        """
        if not isinstance(model_path, str):
            raise TypeError("model_path must be a string.")

        current_option_instance: InferenceOption
        if inference_option is None:
            current_option_instance = InferenceOption() # Create a default Python InferenceOption
        else:
            if not isinstance(inference_option, InferenceOption):
                raise TypeError("inference_option must be an instance of dx_engine.InferenceOption or None.")
            current_option_instance = inference_option
        
        self.engine = C.InferenceEngine(model_path, current_option_instance.instance)

        self._input_tensor_info_cache: Optional[List[Dict[str, Any]]] = None
        self._output_tensor_info_cache: Optional[List[Dict[str, Any]]] = None

    def run(
        self,
        input_data: Union[List[np.ndarray], List[List[np.ndarray]]],
        output_buffers: Optional[Union[List[np.ndarray], List[List[np.ndarray]]]] = None,
        user_args: Optional[Union[Any, List[Any]]] = None
    ) -> Union[List[np.ndarray], List[List[np.ndarray]]]:
        """
        Runs inference synchronously. Handles both single and batch modes.

        Args:
            input_data:
                - Single inference: `List[np.ndarray]` (one list of tensors for one inference).
                - Batch inference: `List[List[np.ndarray]]` (list of (list of tensors)).
            output_buffers (Optional):
                - Single: `List[np.ndarray]`.
                - Batch: `List[List[np.ndarray]]`.
                If provided, results are written here. Otherwise, new arrays are returned.
                For batch mode, `output_buffers` are typically required.
            user_args (Optional):
                - Single: Any Python object.
                - Batch: `List[Any]`, one per batch item.

        Returns:
            If `output_buffers` were provided, returns those same buffer(s) (now filled).
            Otherwise, returns newly allocated `List[np.ndarray]` (single) or
            `List[List[np.ndarray]]` (batch).
        """
        if not isinstance(input_data, list):
            raise TypeError("Input data must be a list.")
        if not input_data:
            raise ValueError("Input data list cannot be empty.")

        is_batch = False
        if isinstance(input_data[0], list):
            # Handles List[List[np.ndarray]] or List[[]] (batch of empty inputs)
            if not input_data[0] or isinstance(input_data[0][0], np.ndarray): 
                is_batch = True
            else: 
                raise TypeError("For batch inference, input_data should be List[List[np.ndarray]].")
        elif isinstance(input_data[0], np.ndarray): 
            is_batch = False
        else: 
            raise TypeError("Input data elements must be np.ndarray (for single) or List[np.ndarray] (for batch).")

        inputs_for_c: Union[List[np.ndarray], List[List[np.ndarray]]]
        outputs_for_c: Optional[Union[List[np.ndarray], List[List[np.ndarray]]]] = None
        user_args_for_c: Optional[Union[Any, List[Any]]] = user_args

        if is_batch:
            inputs_for_c = [ensure_contiguous(batch_item_input) for batch_item_input in input_data] # type: ignore
            if output_buffers is None:
                raise ValueError("output_buffers must be provided for batch synchronous inference.")
            if not (isinstance(output_buffers, list) and
                    len(output_buffers) > 0 and
                    isinstance(output_buffers[0], list) and
                    all(isinstance(t, np.ndarray) for t_list in output_buffers for t in t_list)): # type: ignore
                raise TypeError("For batch inference, output_buffers must be List[List[np.ndarray]].")
            if len(inputs_for_c) != len(output_buffers): # type: ignore
                raise ValueError("Number of input batches must match number of output buffer sets.")
            outputs_for_c = [ensure_contiguous(batch_item_output) for batch_item_output in output_buffers] # type: ignore

            if user_args is not None:
                if not isinstance(user_args, list) or len(user_args) != len(inputs_for_c):
                    raise ValueError("For batch inference, user_args must be a list matching the batch size, or None.")
        else: # Single inference
            inputs_for_c = ensure_contiguous(input_data) # type: ignore
            if output_buffers is not None:
                if not (isinstance(output_buffers, list) and all(isinstance(ob, np.ndarray) for ob in output_buffers)): # type: ignore
                    raise TypeError("For single inference, output_buffers must be List[np.ndarray] if provided.")
                outputs_for_c = ensure_contiguous(output_buffers) # type: ignore
        
        raw_outputs = C.run(self.engine, inputs_for_c, outputs_for_c, user_args_for_c)

        '''
        # Python Address Check (Debug)
        if output_buffers is not None and outputs_for_c is not None: # Checks if output buffers were involved
            print("--- Memory Address Check (After C.run) ---")
            if is_batch:
                for i in range(len(outputs_for_c)):
                    for j in range(len(outputs_for_c[i])):
                        prepared_ptr = outputs_for_c[i][j].ctypes.data
                        raw_ptr = raw_outputs[i][j].ctypes.data
                        prep_size = outputs_for_c[i][j].nbytes
                        raw_size = raw_outputs[i][j].nbytes
                        
                        is_view = (raw_ptr >= prepared_ptr and
                                   (raw_ptr + raw_size) <= (prepared_ptr + prep_size))
                        
                        print(f"Batch[{i}] Tensor[{j}]:")
                        print(f"  outputs_for_c: ptr={prepared_ptr}, shape={outputs_for_c[i][j].shape}, nbytes={prep_size}")
                        print(f"  raw_outputs:            ptr={raw_ptr}, shape={raw_outputs[i][j].shape}, nbytes={raw_size}, owner={raw_outputs[i][j].flags}")
                        print(f"  Pointers exactly match: {prepared_ptr == raw_ptr}")
                        print(f"  Raw_output is view of/within outputs_for_c: {is_view}")
            else: # Single inference
                for j in range(len(outputs_for_c)):
                    prepared_ptr = outputs_for_c[j].ctypes.data
                    raw_ptr = raw_outputs[j].ctypes.data
                    prep_size = outputs_for_c[j].nbytes
                    raw_size = raw_outputs[j].nbytes

                    is_view = (raw_ptr >= prepared_ptr and
                               (raw_ptr + raw_size) <= (prepared_ptr + prep_size))

                    print(f"Single Tensor[{j}]:")
                    print(f"  outputs_for_c: ptr={prepared_ptr}, shape={outputs_for_c[j].shape}, nbytes={prep_size}")
                    print(f"  raw_outputs:            ptr={raw_ptr}, shape={raw_outputs[j].shape}, nbytes={raw_size}")
                    print(f"  Pointers exactly match: {prepared_ptr == raw_ptr}")
                    print(f"  Raw_output is view of/within outputs_for_c: {is_view}")
        '''
        return raw_outputs
    
    def run_async(self,
                  input_data: List[np.ndarray],
                  user_arg: Any = None,
                  output_buffer: Optional[Union[np.ndarray, List[np.ndarray]]] = None
                 ) -> int:
        """Run inference asynchronously."""
        if not isinstance(input_data, list) or not all(isinstance(i, np.ndarray) for i in input_data):
            raise TypeError("input_data for run_async must be a List[np.ndarray].")
        
        contiguous_input = ensure_contiguous(input_data)
        
        valid_output_arg: Any = None
        if output_buffer is not None:
            if isinstance(output_buffer, np.ndarray):
                valid_output_arg = ensure_contiguous([output_buffer])[0] if isinstance(output_buffer, np.ndarray) else ensure_contiguous(output_buffer)

            elif isinstance(output_buffer, list) and all(isinstance(x, np.ndarray) for x in output_buffer):
                if not output_buffer: 
                    pass 
                else:
                    valid_output_arg = ensure_contiguous(output_buffer)
            else:
                raise TypeError("output_buffer for run_async must be np.ndarray, List[np.ndarray], or None.")
        
        return C.run_async(self.engine, contiguous_input, user_arg, valid_output_arg)

    def register_callback(self, callback: Optional[Callable[[List[np.ndarray], Any], int]]) -> None:
        """Register a user callback for asynchronous inference completion."""
        if callback is not None and not callable(callback):
            raise TypeError("Callback must be a callable function or None.")
        C.register_callback(self.engine, callback)

    def wait(self, job_id: int) -> List[np.ndarray]:
        """Wait for an asynchronous job to complete and retrieve its output."""
        if not isinstance(job_id, int):
            raise TypeError("job_id must be an integer.")
        return C.wait(self.engine, job_id)

    def run_benchmark(self, num_loops: int, input_data: Optional[List[np.ndarray]] = None) -> float:
        """Runs a benchmark for a specified number of loops."""
        if not isinstance(num_loops, int) or num_loops <= 0:
            raise ValueError("num_loops must be a positive integer.")
        
        contiguous_input: Optional[List[np.ndarray]] = None
        if input_data is not None:
            if not isinstance(input_data, list) or not all(isinstance(i, np.ndarray) for i in input_data):
                raise TypeError("input_data for benchmark must be a List[np.ndarray] or None.")
            contiguous_input = ensure_contiguous(input_data)
            
        return C.run_benchmark(self.engine, num_loops, contiguous_input)

    def validate_device(self, input_data: List[np.ndarray], device_id: int = 0) -> List[np.ndarray]:
        """Validates an NPU device by running an inference task on it."""
        if not isinstance(input_data, list) or not all(isinstance(i, np.ndarray) for i in input_data):
            raise TypeError("input_data for validate_device must be a List[np.ndarray].")
        if not isinstance(device_id, int):
            raise TypeError("device_id must be an integer.")

        contiguous_input = ensure_contiguous(input_data)
        result = C.validate_device(self.engine, contiguous_input, device_id)
        return [np.copy(arr) for arr in result]

    def get_input_size(self) -> int:
        """Get the total expected size of all input tensors in bytes."""
        return self.engine.get_input_size()

    def get_output_size(self) -> int:
        """Get the total size of all output tensors in bytes, if known by the engine."""
        return self.engine.get_output_size()

    def _fetch_input_tensors_info(self) -> List[Dict[str, Any]]:
        """Internal: Fetches and caches input tensor info."""
        if self._input_tensor_info_cache is None:
            # C.get_inputs_info is bound to pyGetInputsInfo(InferenceEngine &ie, ...)
            self._input_tensor_info_cache = C.get_inputs_info(self.engine) 
            for tensor_info in self._input_tensor_info_cache:
                tensor_info["dtype"] = NumpyDataTypeMapper.from_string(tensor_info["dtype"])
        return self._input_tensor_info_cache

    def _fetch_output_tensors_info(self) -> List[Dict[str, Any]]:
        """Internal: Fetches and caches output tensor info."""
        if self._output_tensor_info_cache is None:
            # C.get_outputs_info is bound to pyGetOutputsInfo(InferenceEngine &ie, ...)
            self._output_tensor_info_cache = C.get_outputs_info(self.engine)
            for tensor_info in self._output_tensor_info_cache:
                tensor_info["dtype"] = NumpyDataTypeMapper.from_string(tensor_info["dtype"])
        return self._output_tensor_info_cache

    def get_input_tensors_info(self) -> List[Dict[str, Any]]:
        """
        Returns a list of dictionaries, each detailing an input tensor info.
        Keys: 'name' (str), 'shape' (List[int]), 'dtype' (str), 'elem_size' (int).
        """
        return self._fetch_input_tensors_info()

    def get_output_tensors_info(self) -> List[Dict[str, Any]]:
        """
        Returns a list of dictionaries, each detailing an output tensor info.
        Keys: 'name' (str), 'shape' (List[int]), 'dtype' (str), 'elem_size' (int).
        """
        return self._fetch_output_tensors_info()

    def get_input_tensor_count(self) -> int:
        """Returns the number of input tensors required by the model."""
        return len(self._fetch_input_tensors_info())

    def get_output_tensor_count(self) -> int:
        """Returns the number of output tensors produced by the model."""
        return len(self._fetch_output_tensors_info())

    def get_input_data_type(self) -> List[Union[type, str, None]]: # NumpyDataTypeMapper.from_string can return None
        """
        Get required input data type as a list of Python types using NumpyDataTypeMapper.
        @deprecated: Use `get_input_tensor_info()` and access the 'dtype' key.
        """
        warnings.warn(
            "Method get_input_data_type() is deprecated. "
            "Use get_input_tensor_info() and access the 'dtype' key, then map with NumpyDataTypeMapper if needed.",
            DeprecationWarning,
            stacklevel=2
        )
        # C.get_input_dtype is bound to pyGetInputDataType(InferenceEngine& ie)
        dtype_strs = C.get_input_dtype(self.engine) 
        return [NumpyDataTypeMapper.from_string(dt) for dt in dtype_strs]

    def get_output_data_type(self) -> List[Union[type, str, None]]: # NumpyDataTypeMapper.from_string can return None
        """
        Get required output data type as a list of Python types using NumpyDataTypeMapper.
        @deprecated: Use `get_output_tensor_info()` and access the 'dtype' key.
        """
        warnings.warn(
            "Method get_output_data_type() is deprecated. "
            "Use get_output_tensor_info() and access the 'dtype' key, then map with NumpyDataTypeMapper if needed.",
            DeprecationWarning,
            stacklevel=2
        )
        # C.get_output_dtype is bound to pyGetOutputDataType(InferenceEngine& ie)
        dtype_strs = C.get_output_dtype(self.engine)
        return [NumpyDataTypeMapper.from_string(dt) for dt in dtype_strs]

    def get_task_order(self) -> np.ndarray:
        """Returns the execution order of tasks/subgraphs within the model."""
        task_order = np.array(C.get_task_order(self.engine))
        return task_order

    def get_all_task_outputs(self) -> List[List[np.ndarray]]:
        """Retrieves the outputs of all internal tasks in their execution order."""
        return C.get_all_task_outputs(self.engine) # Bound to pyGetAllTaskOutputs(InferenceEngine& ie)

    def get_latency(self) -> int:
        """Returns the latency of the most recent inference in microseconds."""
        return self.engine.get_latency()

    def get_npu_inference_time(self) -> int:
        """Returns the NPU processing time for the most recent inference in microseconds."""
        return self.engine.get_npu_inference_time()

    def get_latency_list(self) -> List[int]:
        """Returns a list of recent latency measurements (microseconds)."""
        return self.engine.get_latency_list()

    def get_npu_inference_time_list(self) -> List[int]:
        """Returns a list of recent NPU inference time measurements (microseconds)."""
        return self.engine.get_npu_inference_time_list()

    def get_latency_mean(self) -> float:
        """Returns the mean of collected latency values."""
        return self.engine.get_latency_mean()

    def get_npu_inference_time_mean(self) -> float:
        """Returns the mean of collected NPU inference times."""
        return self.engine.get_npu_inference_time_mean()

    def get_latency_std(self) -> float:
        """Returns the standard deviation of collected latency values."""
        return self.engine.get_latency_std()

    def get_npu_inference_time_std(self) -> float:
        """Returns the standard deviation of collected NPU inference times."""
        return self.engine.get_npu_inference_time_std()

    def get_latency_count(self) -> int:
        """Returns the count of latency values collected."""
        return self.engine.get_latency_count()

    def get_npu_inference_time_count(self) -> int:
        """Returns the count of NPU inference times collected."""
        return self.engine.get_npu_inference_time_count()

    def get_bitmatch_mask(self, index: int = 0) -> np.ndarray:
        """Retrieves a bitmatch mask for a specific NPU task."""
        mask_bytes_list = self.engine.get_bitmatch_mask(index)
        mask_bytes = np.array(mask_bytes_list, dtype=np.uint8)
        return np.unpackbits(mask_bytes, bitorder='little').astype(bool)

    def get_num_tail_tasks(self) -> int:
        """Returns the number of 'tail' tasks in the model graph."""
        return self.engine.get_num_tail_tasks()

    def get_compile_type(self) -> str:
        """Returns the compilation type/strategy of the loaded model."""
        return self.engine.get_compile_type()

    def is_ppu(self) -> bool:
        """Checks if the loaded model utilizes a Post-Processing Unit (PPU)."""
        return self.engine.is_ppu()

    def dispose(self) -> None:
        """Explicitly releases resources held by the inference engine."""
        if hasattr(self, 'engine') and self.engine is not None:
            self.engine.dispose()

    def __enter__(self):
        """Enter the runtime context for the inference engine (for `with` statement)."""
        C.inference_engine_enter(self.engine) # Calls the bound C++ function
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """Exit the runtime context, ensuring resources are released."""
        C.inference_engine_exit(self.engine) # Calls the bound C++ function
        # To match original user's py_inference_engine.cpp's intent for `__exit__`
        # which returned True (suppressing exceptions). If that's desired:
        # return True 
        # Otherwise, to propagate exceptions (current behavior by not returning True):
        return False # Or simply no return statement (None is returned, propagating exceptions)


    # --- Deprecated Methods ---
    def Run(self, input_feed_list: List[np.ndarray], user_arg: object = None):
        warnings.warn("Method Run() is deprecated. Use run() instead.", DeprecationWarning, stacklevel=2)
        return self.run(input_feed_list, user_args=user_arg)

    def run_batch(self, input_buffers: List[List[np.ndarray]], output_buffers: List[List[np.ndarray]], user_args: Optional[List[object]] = None):
        warnings.warn("Method run_batch() is deprecated. Use run() with batch-formatted inputs/outputs.", DeprecationWarning, stacklevel=2)
        return self.run(input_buffers, output_buffers=output_buffers, user_args=user_args)

    def RunBatch(self, input_buffers: List[List[np.ndarray]], output_buffers: List[List[np.ndarray]], user_args: Optional[List[object]] = None):
        warnings.warn("Method RunBatch() is deprecated. Use run() with batch-formatted inputs/outputs.", DeprecationWarning, stacklevel=2)
        return self.run(input_buffers, output_buffers=output_buffers, user_args=user_args)

    def RunAsync(self, input_feed_list: List[np.ndarray], user_arg: Any = None):
        warnings.warn("Method RunAsync() is deprecated. Use run_async() instead.", DeprecationWarning, stacklevel=2)
        return self.run_async(input_feed_list, user_arg=user_arg)

    def RunBenchMark(self, loop_cnt: int, input_feed_list: Optional[List[np.ndarray]] = None):
        warnings.warn("Method RunBenchMark() is deprecated. Use run_benchmark() instead.", DeprecationWarning, stacklevel=2)
        return self.run_benchmark(loop_cnt, input_feed_list)

    def ValidateDevice(self, input_feed_list: List[np.ndarray], device_id: int = 0):
        warnings.warn("Method ValidateDevice() is deprecated. Use validate_device() instead.", DeprecationWarning, stacklevel=2)
        return self.validate_device(input_feed_list, device_id)

    def RegisterCallBack(self, callback: Optional[Callable[[List[np.ndarray], Any], int]]):
        warnings.warn("Method RegisterCallBack() is deprecated. Use register_callback() instead.", DeprecationWarning, stacklevel=2)
        return self.register_callback(callback)

    def Wait(self, req_id: int):
        warnings.warn("Method Wait() is deprecated. Use wait() instead.", DeprecationWarning, stacklevel=2)
        return self.wait(req_id)

    def input_size(self) -> int:
        warnings.warn("Method input_size() is deprecated. Use get_input_size() instead.", DeprecationWarning, stacklevel=2)
        return self.get_input_size()

    def output_size(self) -> int:
        warnings.warn("Method output_size() is deprecated. Use get_output_size() instead.", DeprecationWarning, stacklevel=2)
        return self.get_output_size()
    
    def input_dtype(self) -> List[Union[type, None]]: # Updated return type
        warnings.warn("Method input_dtype() is deprecated. Use get_input_tensor_info() and parse 'dtype' string.", DeprecationWarning, stacklevel=2)
        return self.get_input_data_type() # Calls the new internal logic

    def output_dtype(self) -> List[Union[type, None]]: # Updated return type
        warnings.warn("Method output_dtype() is deprecated. Use get_output_tensor_info() and parse 'dtype' string.", DeprecationWarning, stacklevel=2)
        return self.get_output_data_type() # Calls the new internal logic

    def task_order(self) -> List[str]:
        warnings.warn("Method task_order() is deprecated. Use get_task_order() instead.", DeprecationWarning, stacklevel=2)
        return self.get_task_order()

    def latency(self) -> int:
        warnings.warn("Method latency() is deprecated. Use get_latency() instead.", DeprecationWarning, stacklevel=2)
        return self.get_latency()

    def inference_time(self) -> int:
        warnings.warn("Method inference_time() is deprecated. Use get_npu_inference_time() instead.", DeprecationWarning, stacklevel=2)
        return self.get_npu_inference_time()

    def get_outputs(self) -> List[List[np.ndarray]]:
        warnings.warn("Method get_outputs() is deprecated. Use get_all_task_outputs() instead.", DeprecationWarning, stacklevel=2)
        return self.get_all_task_outputs()

    def bitmatch_mask(self, index: int = 0) -> np.ndarray:
        warnings.warn("Method bitmatch_mask() is deprecated. Use get_bitmatch_mask() instead.", DeprecationWarning, stacklevel=2)
        return self.get_bitmatch_mask(index)

    def get_num_tails(self) -> int:
        warnings.warn("Method get_num_tails() is deprecated. Use get_num_tail_tasks() instead.", DeprecationWarning, stacklevel=2)
        return self.get_num_tail_tasks()

    def is_PPU(self) -> bool: 
        warnings.warn("Method is_PPU() is deprecated. Use is_ppu() instead.", DeprecationWarning, stacklevel=2)
        return self.is_ppu()


def parse_model(model_path: str) -> str:
    """
    Parses a model file using the C++ backend and returns information about it.
    """
    if not isinstance(model_path, str):
        raise TypeError("model_path must be a string.")
    return C.parse_model(model_path)