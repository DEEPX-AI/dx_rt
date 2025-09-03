#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

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
        
        # If this build does not support ORT, force-disable it to avoid runtime exceptions
        try:
            if hasattr(C, 'is_ort_supported') and not C.is_ort_supported():
                if current_option_instance.use_ort:
                    warnings.warn(
                        "USE_ORT is disabled in this build. Forcing InferenceOption.use_ort to False.",
                        RuntimeWarning,
                    )
                    current_option_instance.use_ort = False
        except Exception:
            # If capability check is unavailable, proceed; C++ side will still guard.
            pass
        
        try:
            self.engine = C.InferenceEngine(model_path, current_option_instance.instance)
        except Exception as e:
            # Re-raise with more context
            raise RuntimeError(f"Failed to create InferenceEngine for model '{model_path}': {str(e)}") from e

        self._input_tensor_info_cache: Optional[List[Dict[str, Any]]] = None
        self._output_tensor_info_cache: Optional[List[Dict[str, Any]]] = None

    def _analyze_input_format(self, input_data: Any) -> Dict[str, Any]:
        """
        Analyzes input data format and determines processing method.
        
        Args:
            input_data: User input data
            
        Returns:
            Dict containing:
                - format_type: 'single_ndarray', 'list_ndarray', 'list_list_ndarray'
                - is_batch: Boolean indicating if this is batch processing
                - batch_size: Number of samples (None for single)
                - processed_inputs: Standardized input format for C++ binding
        """
        # 1. Single np.ndarray input
        if isinstance(input_data, np.ndarray):
            # Check for auto-split for multi-input models
            if self._should_auto_split_input(input_data):
                split_tensors = self._split_input_buffer(input_data)
                return {
                    'format_type': 'single_ndarray',
                    'is_batch': False,
                    'batch_size': None,
                    'processed_inputs': split_tensors,
                    'auto_split': True
                }
            
            return {
                'format_type': 'single_ndarray',
                'is_batch': False,
                'batch_size': None,
                'processed_inputs': [input_data],  # Convert to list for consistency
                'auto_split': False
            }
        
        # 2. Error if input is not a list
        if not isinstance(input_data, list):
            raise TypeError("Input data must be np.ndarray, List[np.ndarray], or List[List[np.ndarray]].")
        
        if not input_data:
            raise ValueError("Input data list cannot be empty.")
        
        # 3. List[List[np.ndarray]] - explicit batch format
        if isinstance(input_data[0], list):
            # Verify all items are lists
            if not all(isinstance(item, list) for item in input_data):
                raise TypeError("All items must be lists for List[List[np.ndarray]] format.")
            
            # Validate each batch item
            for i, batch_item in enumerate(input_data):
                if not all(isinstance(tensor, np.ndarray) for tensor in batch_item):
                    raise TypeError(f"All tensors in batch item {i} must be np.ndarray.")
                
                if not batch_item:
                    raise ValueError(f"Batch item {i} cannot be empty.")
                
                # For multi-input models, validate tensor count per batch item
                if self.is_multi_input_model():
                    expected_count = self.get_input_tensor_count()
                    if len(batch_item) == expected_count:
                        pass  # exact match OK
                    elif len(batch_item) == 1 and isinstance(batch_item[0], np.ndarray) and self._should_auto_split_input(batch_item[0]):
                        # single concatenated buffer for multi-input sample -> allowed
                        pass
                    else:
                        tensor_names = self.get_input_tensor_names()
                        raise ValueError(f"Multi-input model requires {expected_count} input tensors {tensor_names} per sample, "
                                         f"but batch item {i} has {len(batch_item)} inputs.")
                elif len(batch_item) != 1:
                    # For single-input models, each batch item should have exactly 1 tensor
                    raise ValueError(f"Single-input model requires 1 tensor per batch item, "
                                   f"but batch item {i} has {len(batch_item)} tensors.")
            
            return {
                'format_type': 'list_list_ndarray',
                'is_batch': True,
                'batch_size': len(input_data),
                'processed_inputs': input_data,
                'auto_split': False
            }
        
        # 4. List[np.ndarray] - most complex case
        if not all(isinstance(tensor, np.ndarray) for tensor in input_data):
            raise TypeError("All items in List[np.ndarray] must be np.ndarray.")
        
        input_count = len(input_data)
        
        # Check for auto-split for multi-input models
        if self._should_auto_split_input(input_data[0]):
            split_tensors = self._split_input_buffer(input_data[0])
            return {
                'format_type': 'list_ndarray',
                'is_batch': False,
                'batch_size': None,
                'processed_inputs': split_tensors,
                'auto_split': True
            }
        
        if self.is_multi_input_model():
            expected_count = self.get_input_tensor_count()
            
            if input_count == expected_count:
                # Exact match: Multi-input single inference (most intuitive)
                return {
                    'format_type': 'list_ndarray',
                    'is_batch': False,
                    'batch_size': None,
                    'processed_inputs': input_data,
                    'auto_split': False
                }
            elif input_count % expected_count == 0:
                # Multiple relationship: Multi-input batch inference
                batch_size = input_count // expected_count
                # Reconstruct as batch format
                grouped_inputs = []
                for i in range(batch_size):
                    start_idx = i * expected_count
                    end_idx = start_idx + expected_count
                    batch_item = input_data[start_idx:end_idx]
                    grouped_inputs.append(batch_item)
                
                return {
                    'format_type': 'list_ndarray',
                    'is_batch': True,
                    'batch_size': batch_size,
                    'processed_inputs': grouped_inputs,
                    'auto_split': False
                }
            else:
                # Invalid count -> Try special case: list[np.ndarray] where each ndarray is concatenated inputs per sample
                if all(isinstance(t, np.ndarray) and self._should_auto_split_input(t) for t in input_data):
                    batch_size = input_count
                    # Wrap each tensor into a list so downstream code treats as multi-input sample
                    grouped_inputs = [[t] for t in input_data]
                    return {
                        'format_type': 'list_ndarray',
                        'is_batch': True,
                        'batch_size': batch_size,
                        'processed_inputs': grouped_inputs,
                        'auto_split': True  # auto-split will happen per sample inside C++
                    }

                # If not special case, raise error
                tensor_names = self.get_input_tensor_names()
                raise ValueError(f"Multi-input model requires {expected_count} input tensors {tensor_names} per sample. "
                               f"Got {input_count} inputs, which is not a valid multiple.")
        else:
            # Single-input model: List[np.ndarray] is either single inference or batch
            if input_count == 1:
                # Single inference request
                return {
                    'format_type': 'list_ndarray', 
                    'is_batch': False,
                    'batch_size': None,
                    'processed_inputs': input_data,
                    'auto_split': False
                }
            else:
                # Batch inference request
                batch_inputs = [[tensor] for tensor in input_data]
                return {
                    'format_type': 'list_ndarray',
                    'is_batch': True, 
                    'batch_size': input_count,
                    'processed_inputs': batch_inputs,
                    'auto_split': False
                }

    def _prepare_output_buffers(self, output_buffers: Any, analysis_result: Dict[str, Any]) -> Any:
        """
        Prepares output buffers according to the analysis result.
        
        Args:
            output_buffers: User-provided output buffers
            analysis_result: Result from _analyze_input_format
            
        Returns:
            Processed output buffers for C++ binding
        """
        # If no output buffers provided, return None to maintain existing behavior
        if output_buffers is None:
            return None
        
        is_batch = analysis_result['is_batch']
        batch_size = analysis_result['batch_size']
        
        if is_batch:
            if analysis_result['format_type'] == 'list_list_ndarray':
                if not (isinstance(output_buffers, list) and
                        len(output_buffers) > 0 and
                        isinstance(output_buffers[0], list) and
                        all(isinstance(t, np.ndarray) for t_list in output_buffers for t in t_list)):
                    raise TypeError("For batch inference with List[List[np.ndarray]] input, "
                                  "output_buffers must be List[List[np.ndarray]].")
                
                if len(output_buffers) != batch_size:
                    raise ValueError(f"Output buffer batch size ({len(output_buffers)}) must match "
                                   f"input batch size ({batch_size}).")
                
                # Validate each batch item's output buffers
                expected_output_count = self.get_output_tensor_count()
                processed_batch_outputs = []
                
                for batch_idx, batch_output_buffers in enumerate(output_buffers):
                    if len(batch_output_buffers) == expected_output_count:
                        # Each buffer corresponds to one output tensor - validate individually
                        for tensor_idx, buffer in enumerate(batch_output_buffers):
                            self._validate_output_buffer_size(buffer, tensor_index=tensor_idx)
                        processed_batch_outputs.append(batch_output_buffers)
                    elif len(batch_output_buffers) == 1 and self._should_auto_split_output(batch_output_buffers[0]):
                        # Single concatenated buffer for multi-output model - auto-split
                        self._validate_output_buffer_size(batch_output_buffers[0])
                        split_buffers = self._split_output_buffer(batch_output_buffers[0])
                        processed_batch_outputs.append(split_buffers)
                    else:
                        raise ValueError(f"Batch item {batch_idx} has {len(batch_output_buffers)} output buffers. "
                                       f"Expected either {expected_output_count} individual buffers or "
                                       f"1 concatenated buffer that matches total output size.")
                
                return processed_batch_outputs
            else:
                if not (isinstance(output_buffers, list) and 
                        all(isinstance(ob, np.ndarray) for ob in output_buffers)):
                    raise TypeError("For batch inference from List[np.ndarray], "
                                  "output_buffers must be List[np.ndarray].")
                
                if len(output_buffers) != batch_size:
                    raise ValueError(f"Output buffer batch size ({len(output_buffers)}) must match "
                                   f"input batch size ({batch_size}).")
                
                # Check if each buffer in the batch should be auto-split for multi-output
                processed_batch_outputs = []
                for batch_idx, buffer in enumerate(output_buffers):
                    # Validate buffer size first
                    self._validate_output_buffer_size(buffer)
                    
                    if self._should_auto_split_output(buffer):
                        processed_batch_outputs.append(self._split_output_buffer(buffer))
                    else:
                        # For single buffer per batch item, ensure it matches the expected single output size
                        if self._is_multi_output_model():
                            raise ValueError(f"Multi-output model requires separate buffers for each output tensor "
                                           f"in batch item {batch_idx}, or a single concatenated buffer that matches total size")
                        processed_batch_outputs.append([buffer])
                
                return processed_batch_outputs
        else:
            # Single inference case - handle multi-output buffer splitting
            if isinstance(output_buffers, np.ndarray):
                # Validate buffer size first
                self._validate_output_buffer_size(output_buffers)
                
                # Check if this single buffer should be auto-split for multi-output model
                if self._should_auto_split_output(output_buffers):
                    return self._split_output_buffer(output_buffers)
                else:
                    # Single ndarray - wrap in list for consistency
                    return [output_buffers]
            elif isinstance(output_buffers, list):
                # Check for single-element list containing concatenated buffer
                if (len(output_buffers) == 1 and 
                    isinstance(output_buffers[0], np.ndarray) and 
                    self._should_auto_split_output(output_buffers[0])):
                    # Validate the concatenated buffer
                    self._validate_output_buffer_size(output_buffers[0])
                    return self._split_output_buffer(output_buffers[0])
                elif all(isinstance(ob, np.ndarray) for ob in output_buffers):
                    # Validate each buffer and check count
                    expected_count = self.get_output_tensor_count()
                    if len(output_buffers) != expected_count:
                        raise ValueError(f"Number of output buffers ({len(output_buffers)}) "
                                       f"does not match expected output tensor count ({expected_count})")
                    
                    # Validate each individual buffer
                    for i, buffer in enumerate(output_buffers):
                        self._validate_output_buffer_size(buffer, tensor_index=i)
                    
                    return output_buffers
                else:
                    raise TypeError("For single inference, output_buffers must be List[np.ndarray].")
            else:
                raise TypeError("For single inference, output_buffers must be np.ndarray or List[np.ndarray].")

    def _prepare_user_args(self, user_args: Any, analysis_result: Dict[str, Any]) -> Any:
        """
        Prepares user arguments according to the analysis result.
        
        Args:
            user_args: User-provided arguments
            analysis_result: Result from _analyze_input_format
            
        Returns:
            Processed user arguments for C++ binding
        """
        if user_args is None:
            return None
        
        is_batch = analysis_result['is_batch']
        batch_size = analysis_result['batch_size']
        
        if is_batch:
            if analysis_result['format_type'] == 'list_list_ndarray':
                # For explicit batch format, user_args must also be in batch format
                if isinstance(user_args, list) and len(user_args) == batch_size:
                    return user_args
                else:
                    raise ValueError(f"For batch inference with List[List[np.ndarray]] input, "
                                   f"user_args must be a list of size {batch_size} or None.")
            else:
                # For batch converted from List[np.ndarray]
                if isinstance(user_args, list) and len(user_args) == batch_size:
                    # User provided arguments per sample
                    return user_args
                else:
                    # Apply single argument to all batch items
                    return [user_args] * batch_size
        else:
            # For single inference
            return user_args

    def run(
        self,
        input_data: Union[np.ndarray, List[np.ndarray], List[List[np.ndarray]]],
        output_buffers: Optional[Union[List[np.ndarray], List[List[np.ndarray]]]] = None,
        user_args: Optional[Union[Any, List[Any]]] = None
    ) -> Union[List[np.ndarray], List[List[np.ndarray]]]:
        """
        Runs inference synchronously. Handles single and batch modes with unified logic.

        Args:
            input_data:
                - Single input: np.ndarray
                - Multi-input single: List[np.ndarray] (length = model input count)
                - Batch: List[np.ndarray] (length = multiple of model input count for multi-input)
                - Explicit batch: List[List[np.ndarray]] (outer list = batch, inner list = per-sample inputs)
            output_buffers (Optional):
                - Single: List[np.ndarray]
                - Batch: List[List[np.ndarray]] for explicit batch, List[np.ndarray] for implicit batch
                For batch mode, output_buffers are required.
            user_args (Optional):
                - Single: Any Python object
                - Batch: List[Any] (one per batch item) or single value (applied to all)

        Returns:
            - Single: List[np.ndarray]
            - Batch: List[List[np.ndarray]]
        """
        # 1. Analyze input format
        analysis_result = self._analyze_input_format(input_data)
        
        # 2. Prepare output buffers
        processed_outputs = self._prepare_output_buffers(output_buffers, analysis_result)
        
        # 3. Prepare user arguments
        processed_user_args = self._prepare_user_args(user_args, analysis_result)
        
        # 4. Ensure data contiguity
        if analysis_result['is_batch']:
            inputs_for_c = [ensure_contiguous(batch_item) for batch_item in analysis_result['processed_inputs']]
            outputs_for_c = [ensure_contiguous(batch_output) for batch_output in processed_outputs]
        else:
            inputs_for_c = ensure_contiguous(analysis_result['processed_inputs'])
            outputs_for_c = ensure_contiguous(processed_outputs) if processed_outputs is not None else None
        
        # 5. Call C++ engine
        raw_outputs = C.run(self.engine, inputs_for_c, outputs_for_c, processed_user_args)
        
        return raw_outputs

    def run_multi_input(
        self,
        input_tensors: Dict[str, np.ndarray],
        output_buffers: Optional[List[np.ndarray]] = None,
        user_arg: Any = None
    ) -> List[np.ndarray]:
        """
        Runs inference on a multi-input model using a dictionary of named input tensors.

        Args:
            input_tensors: Dictionary mapping tensor names to numpy arrays.
            output_buffers: Optional list of pre-allocated output arrays.
            user_arg: Optional user-defined argument.

        Returns:
            List of output numpy arrays.

        Raises:
            ValueError: If the model is not a multi-input model or tensor names don't match.
        """
        if not self.is_multi_input_model():
            raise ValueError("This model is not a multi-input model. Use run() instead.")

        expected_names = self.get_input_tensor_names()
        provided_names = set(input_tensors.keys())
        expected_names_set = set(expected_names)

        if provided_names != expected_names_set:
            missing = expected_names_set - provided_names
            extra = provided_names - expected_names_set
            error_msg = f"Input tensor names mismatch. Expected: {expected_names}"
            if missing:
                error_msg += f", Missing: {list(missing)}"
            if extra:
                error_msg += f", Extra: {list(extra)}"
            raise ValueError(error_msg)

        # Convert dictionary to list in the correct order
        input_list = [input_tensors[name] for name in expected_names]

        return self.run(input_list, output_buffers=output_buffers, user_args=user_arg)
    
    def run_async(self,
                  input_data: Union[np.ndarray, List[np.ndarray]],
                  user_arg: Any = None,
                  output_buffer: Optional[Union[np.ndarray, List[np.ndarray]]] = None
                 ) -> int:
        """
        Run inference asynchronously with unified input format handling.
        
        Args:
            input_data:
                - Single input: np.ndarray 
                - Multi-input: List[np.ndarray] (length = model input count)
            user_arg: Optional user-defined argument
            output_buffer: Optional output buffer (np.ndarray or List[np.ndarray])
            
        Returns:
            Job ID for the asynchronous operation
            
        Note:
            Async inference only supports single inference (not batch).
            For batch async, use multiple run_async calls.
        """

        analysis_result = self._analyze_input_format(input_data)
        
        if analysis_result['is_batch']:
            raise ValueError("Batch inference is not supported in run_async. "
                           "Use multiple run_async calls or use run() for batch processing.")
        
        inputs_for_c = ensure_contiguous(analysis_result['processed_inputs'])
        
        valid_output_arg: Any = None
        if output_buffer is not None:
            if isinstance(output_buffer, np.ndarray):
                # Validate buffer size before processing
                self._validate_output_buffer_size(output_buffer)
                
                # Check if this single buffer should be auto-split for multi-output model
                if self._should_auto_split_output(output_buffer):
                    valid_output_arg = ensure_contiguous(self._split_output_buffer(output_buffer))
                else:
                    valid_output_arg = ensure_contiguous([output_buffer])
            elif isinstance(output_buffer, list):
                if not output_buffer:
                    # Handle empty list case - treat as None (no user buffer)
                    valid_output_arg = None
                elif all(isinstance(x, np.ndarray) for x in output_buffer):
                    # Validate each buffer in the list
                    for i, buffer in enumerate(output_buffer):
                        self._validate_output_buffer_size(buffer, tensor_index=i)
                    
                    # Check for single-element list containing concatenated buffer
                    if (len(output_buffer) == 1 and self._should_auto_split_output(output_buffer[0])):
                        valid_output_arg = ensure_contiguous(self._split_output_buffer(output_buffer[0]))
                    else:
                        # Validate that the number of buffers matches expected output count
                        expected_count = self.get_output_tensor_count()
                        if len(output_buffer) != expected_count:
                            raise ValueError(f"Number of output buffers ({len(output_buffer)}) "
                                           f"does not match expected output tensor count ({expected_count})")
                        valid_output_arg = ensure_contiguous(output_buffer)
                else:
                    raise TypeError("All items in output buffer list must be np.ndarray.")
            else:
                raise TypeError("output_buffer for run_async must be np.ndarray, List[np.ndarray], or None.")
        
        return self.engine.run_async(inputs_for_c, user_arg, valid_output_arg)

    def run_async_multi_input(
        self,
        input_tensors: Dict[str, np.ndarray],
        user_arg: Any = None,
        output_buffer: Optional[List[np.ndarray]] = None
    ) -> int:
        """
        Runs asynchronous inference on a multi-input model using a dictionary of named input tensors.

        Args:
            input_tensors: Dictionary mapping tensor names to numpy arrays.
            user_arg: Optional user-defined argument.
            output_buffer: Optional list of pre-allocated output arrays.

        Returns:
            Job ID for the asynchronous operation.

        Raises:
            ValueError: If the model is not a multi-input model or tensor names don't match.
        """
        if not self.is_multi_input_model():
            raise ValueError("This model is not a multi-input model. Use run_async() instead.")

        expected_names = self.get_input_tensor_names()
        provided_names = set(input_tensors.keys())
        expected_names_set = set(expected_names)

        if provided_names != expected_names_set:
            missing = expected_names_set - provided_names
            extra = provided_names - expected_names_set
            error_msg = f"Input tensor names mismatch. Expected: {expected_names}"
            if missing:
                error_msg += f", Missing: {list(missing)}"
            if extra:
                error_msg += f", Extra: {list(extra)}"
            raise ValueError(error_msg)

        # Convert dictionary to list in the correct order
        input_list = [input_tensors[name] for name in expected_names]

        return self.run_async(input_list, user_arg=user_arg, output_buffer=output_buffer)

    def register_callback(self, callback: Optional[Callable[[List[np.ndarray], Any], int]]) -> None:
        """Register a user callback for asynchronous inference completion."""
        if callback is not None and not callable(callback):
            raise TypeError("Callback must be a callable function or None.")
        self.engine.register_callback(callback)

    def wait(self, job_id: int) -> List[np.ndarray]:
        """Wait for an asynchronous job to complete and retrieve its output."""
        if not isinstance(job_id, int):
            raise TypeError("job_id must be an integer.")
        return self.engine.wait(job_id)

    def run_benchmark(self, num_loops: int, input_data: Optional[List[np.ndarray]] = None) -> float:
        """Runs a benchmark for a specified number of loops."""
        if not isinstance(num_loops, int) or num_loops <= 0:
            raise ValueError("num_loops must be a positive integer.")
        
        contiguous_input: Optional[List[np.ndarray]] = None
        if input_data is not None:
            if not isinstance(input_data, list) or not all(isinstance(i, np.ndarray) for i in input_data):
                raise TypeError("input_data for benchmark must be a List[np.ndarray] or None.")
            contiguous_input = ensure_contiguous(input_data)
            
        return self.engine.run_benchmark(num_loops, contiguous_input)

    def validate_device(self, input_data: Union[np.ndarray, List[np.ndarray]], device_id: int = 0) -> List[np.ndarray]:
        """
        Validates an NPU device with unified input format handling.
        
        Args:
            input_data:
                - Single input: np.ndarray
                - Multi-input: List[np.ndarray] (length = model input count)
            device_id: ID of the NPU device to validate
            
        Returns:
            List of output numpy arrays from validation
            
        Note:
            Device validation only supports single inference (not batch).
        """
        if self.get_compile_type() != "debug":
            print("Models compiled in release mode from DX-COM are not supported in validate_device.")
            return []
        
        if not isinstance(device_id, int):
            raise TypeError("device_id must be an integer.")
        
        analysis_result = self._analyze_input_format(input_data)
        
        if analysis_result['is_batch']:
            raise ValueError("Batch inference is not supported in validate_device. "
                           "Use single inference only for device validation.")
        
        inputs_for_c = ensure_contiguous(analysis_result['processed_inputs'])
        
        result = self.engine.validate_device(inputs_for_c, device_id)
        return [np.copy(arr) for arr in result]

    def validate_device_multi_input(
        self,
        input_tensors: Dict[str, np.ndarray],
        device_id: int = 0
    ) -> List[np.ndarray]:
        """
        Validates an NPU device using a multi-input model with named input tensors.

        Args:
            input_tensors: Dictionary mapping tensor names to numpy arrays.
            device_id: ID of the NPU device to validate.

        Returns:
            List of output numpy arrays from validation.

        Raises:
            ValueError: If the model is not a multi-input model or tensor names don't match.
        """
        if not self.is_multi_input_model():
            raise ValueError("This model is not a multi-input model. Use validate_device() instead.")

        if not isinstance(device_id, int):
            raise TypeError("device_id must be an integer.")

        expected_names = self.get_input_tensor_names()
        provided_names = set(input_tensors.keys())
        expected_names_set = set(expected_names)

        if provided_names != expected_names_set:
            missing = expected_names_set - provided_names
            extra = provided_names - expected_names_set
            error_msg = f"Input tensor names mismatch. Expected: {expected_names}"
            if missing:
                error_msg += f", Missing: {list(missing)}"
            if extra:
                error_msg += f", Extra: {list(extra)}"
            raise ValueError(error_msg)

        # Convert dictionary to list in the correct order
        input_list = [input_tensors[name] for name in expected_names]

        return self.validate_device(input_list, device_id=device_id)

    def get_input_size(self) -> int:
        """Get the total expected size of all input tensors in bytes."""
        return self.engine.get_input_size()

    def get_input_tensor_sizes(self) -> List[int]:
        """Get individual input tensor sizes for multi-input models in bytes, in the order specified by GetInputTensorNames()."""
        return self.engine.get_input_tensor_sizes()

    def get_output_size(self) -> int:
        """Get the total size of all output tensors in bytes, if known by the engine."""
        return self.engine.get_output_size()

    def get_output_tensor_sizes(self) -> List[int]:
        """Get individual output tensor sizes for multi-output models in bytes, in the order specified by GetOutputTensorNames()."""
        return self.engine.get_output_tensor_sizes()

    def get_all_task_outputs(self) -> List[List[np.ndarray]]:
        """Retrieves the outputs of all internal tasks in their execution order."""
        return self.engine.get_all_task_outputs()

    def _fetch_input_tensors_info(self) -> List[Dict[str, Any]]:
        """Internal: Fetches and caches input tensor info."""
        if self._input_tensor_info_cache is None:
            # Use class method instead of module function
            self._input_tensor_info_cache = self.engine.get_inputs_info() 
            for tensor_info in self._input_tensor_info_cache:
                tensor_info["dtype"] = NumpyDataTypeMapper.from_string(tensor_info["dtype"])
        return self._input_tensor_info_cache

    def _fetch_output_tensors_info(self) -> List[Dict[str, Any]]:
        """Internal: Fetches and caches output tensor info."""
        if self._output_tensor_info_cache is None:
            # Use class method instead of module function
            self._output_tensor_info_cache = self.engine.get_outputs_info()
            for tensor_info in self._output_tensor_info_cache:
                tensor_info["dtype"] = NumpyDataTypeMapper.from_string(tensor_info["dtype"])
        return self._output_tensor_info_cache

    def _should_auto_split_input(self, input_data: np.ndarray) -> bool:
        """Internal: Check if single input buffer should be auto-split for multi-input models."""
        if not self.is_multi_input_model():
            return False
        
        # Auto-split only for single numpy array input that matches total input size
        if not isinstance(input_data, np.ndarray):
            return False
            
        expected_total_size = self.get_input_size()
        actual_size = input_data.nbytes
        
        return actual_size == expected_total_size

    def _split_input_buffer(self, input_data: np.ndarray) -> List[np.ndarray]:
        """Internal: Split single input buffer into multiple input tensors for multi-input models."""
        tensor_sizes = self.get_input_tensor_sizes()
        input_names = self.get_input_tensor_names()
        
        if len(tensor_sizes) != len(input_names):
            raise ValueError("Mismatch between tensor sizes and tensor names")
        
        split_tensors = []
        offset = 0
        
        for i, (tensor_size, tensor_name) in enumerate(zip(tensor_sizes, input_names)):
            # Calculate elements count for this tensor
            remaining_bytes = input_data.nbytes - offset
            if remaining_bytes < tensor_size:
                raise ValueError(f"Input buffer too small for tensor '{tensor_name}' at index {i}")
            
            # Extract data for this tensor (as bytes)
            tensor_bytes = input_data.view(dtype=np.uint8)[offset:offset + tensor_size]
            split_tensors.append(tensor_bytes)
            offset += tensor_size
        
        return split_tensors

    def _is_multi_output_model(self) -> bool:
        """Internal: Check if model has multiple output tensors."""
        return self.get_output_tensor_count() > 1

    def _should_auto_split_output(self, output_buffer: np.ndarray) -> bool:
        """Internal: Check if single output buffer should be auto-split for multi-output models."""
        if not self._is_multi_output_model():
            return False
        
        # Auto-split only for single numpy array output that matches total output size
        if not isinstance(output_buffer, np.ndarray):
            return False
            
        expected_total_size = self.get_output_size()
        actual_size = output_buffer.nbytes
        
        return actual_size == expected_total_size

    def _split_output_buffer(self, output_buffer: np.ndarray) -> List[np.ndarray]:
        """Internal: Split single output buffer into multiple output tensors for multi-output models."""
        tensor_sizes = self.get_output_tensor_sizes()
        output_names = self.get_output_tensor_names()
        
        if len(tensor_sizes) != len(output_names):
            raise ValueError("Mismatch between output tensor sizes and tensor names")
        
        if not tensor_sizes:
            raise ValueError("No output tensor information available")
        
        # Validate total buffer size matches expected total
        expected_total_size = sum(tensor_sizes)
        if output_buffer.nbytes != expected_total_size:
            raise ValueError(f"Output buffer size ({output_buffer.nbytes} bytes) does not match "
                           f"expected total size ({expected_total_size} bytes) for multi-output model")
        
        split_tensors = []
        offset = 0
        
        for i, (tensor_size, tensor_name) in enumerate(zip(tensor_sizes, output_names)):
            remaining_bytes = output_buffer.nbytes - offset
            if remaining_bytes < tensor_size:
                raise ValueError(f"Output buffer too small for tensor '{tensor_name}' at index {i}: "
                               f"need {tensor_size} bytes, but only {remaining_bytes} bytes remaining")
            
            # Extract view for this tensor (as bytes)
            tensor_bytes = output_buffer.view(dtype=np.uint8)[offset:offset + tensor_size]
            split_tensors.append(tensor_bytes)
            offset += tensor_size
        
        # Ensure we used the entire buffer
        if offset != output_buffer.nbytes:
            raise ValueError(f"Buffer splitting error: used {offset} bytes out of {output_buffer.nbytes} bytes")
        
        return split_tensors

    def _create_output_buffers(self, batch_size: int = 1) -> List[np.ndarray]:
        """Internal: Create appropriately sized output buffers for the model."""
        if self._is_multi_output_model():
            # Multi-output model - create separate buffers for each output tensor
            tensor_sizes = self.get_output_tensor_sizes()
            buffers = []
            for tensor_size in tensor_sizes:
                buffer = np.zeros(tensor_size * batch_size, dtype=np.uint8)
                buffers.append(buffer)
            return buffers
        else:
            # Single output model - create one buffer
            total_size = self.get_output_size()
            buffer = np.zeros(total_size * batch_size, dtype=np.uint8)
            return [buffer]

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
        return self.engine.get_input_tensor_count()

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
        # Use class method instead of module function
        dtype_strs = self.engine.get_input_dtype() 
        return [NumpyDataTypeMapper.from_string(dt) for dt in dtype_strs]

    def get_output_data_type(self) -> List[Union[type, str, None]]: # NumpyDataTypeMapper.from_string can return None
        """
        Get required output data type as a list of Python types using NumpyDataTypeMapper.
        @deprecated: Use `get_output_tensors_info()` and access the 'dtype' key.
        """
        warnings.warn(
            "Method get_output_data_type() is deprecated. "
            "Use get_output_tensors_info() and access the 'dtype' key, then map with NumpyDataTypeMapper if needed.",
            DeprecationWarning,
            stacklevel=2
        )
        # Use class method instead of module function
        dtype_strs = self.engine.get_output_dtype()
        return [NumpyDataTypeMapper.from_string(dt) for dt in dtype_strs]

    def get_task_order(self) -> np.ndarray:
        """Returns the execution order of tasks/subgraphs within the model."""
        task_order = np.array(self.engine.get_task_order())
        return task_order

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

    def get_model_version(self) -> str:
        """Returns the DXNN file format version of the model."""
        return self.engine.get_model_version()

    def is_ppu(self) -> bool:
        """Checks if the loaded model utilizes a Post-Processing Unit (PPU)."""
        return self.engine.is_ppu()

    def is_multi_input_model(self) -> bool:
        """Checks if the loaded model requires multiple input tensors."""
        return self.engine.is_multi_input_model()

    def get_input_tensor_names(self) -> List[str]:
        """Returns the names of all input tensors in the order they should be provided."""
        return self.engine.get_input_tensor_names()

    def get_output_tensor_names(self) -> List[str]:
        """Returns the names of all output tensors in the order they are produced."""
        return self.engine.get_output_tensor_names()

    def get_input_tensor_to_task_mapping(self) -> Dict[str, str]:
        """Returns the mapping from input tensor names to their target tasks."""
        return self.engine.get_input_tensor_to_task_mapping()

    def dispose(self) -> None:
        """Explicitly releases resources held by the inference engine."""
        if hasattr(self, 'engine') and self.engine is not None:
            self.engine.dispose()

    def __enter__(self):
        """Enter the runtime context for the inference engine (for `with` statement)."""
        return self 

    def __exit__(self, exc_type, exc_val, exc_tb):
        """Exit the runtime context, ensuring resources are released."""
        self.dispose() 
        return False


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
        warnings.warn("Method output_dtype() is deprecated. Use get_output_tensors_info() and parse 'dtype' string.", DeprecationWarning, stacklevel=2)
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

    def _validate_output_buffer_size(self, buffer: np.ndarray, tensor_index: Optional[int] = None) -> None:
        """
        Validates that an output buffer has the correct size for the model.
        
        Args:
            buffer: The output buffer to validate
            tensor_index: If provided, validates against a specific output tensor
        
        Raises:
            ValueError: If buffer size is incorrect
        """
        if not isinstance(buffer, np.ndarray):
            raise TypeError("Output buffer must be a numpy array")
            
        if tensor_index is not None:
            # Validate specific tensor buffer
            expected_sizes = self.get_output_tensor_sizes()
            if tensor_index >= len(expected_sizes):
                raise ValueError(f"Tensor index {tensor_index} out of range. "
                               f"Model has {len(expected_sizes)} output tensors.")
            
            expected_size = expected_sizes[tensor_index]
            if buffer.nbytes != expected_size:
                tensor_names = self.get_output_tensor_names()
                tensor_name = tensor_names[tensor_index] if tensor_index < len(tensor_names) else f"tensor_{tensor_index}"
                raise ValueError(f"Output buffer size mismatch for '{tensor_name}': "
                               f"expected {expected_size} bytes, got {buffer.nbytes} bytes")
        else:
            # Validate total buffer size
            if self._is_multi_output_model():
                # For multi-output, buffer should either match total size or be splittable
                expected_total_size = self.get_output_size()
                if buffer.nbytes != expected_total_size:
                    raise ValueError(f"Output buffer size mismatch: "
                                   f"expected {expected_total_size} bytes, got {buffer.nbytes} bytes")
            else:
                # For single output, validate against the single tensor size
                expected_sizes = self.get_output_tensor_sizes()
                if expected_sizes and buffer.nbytes != expected_sizes[0]:
                    raise ValueError(f"Output buffer size mismatch: "
                                   f"expected {expected_sizes[0]} bytes, got {buffer.nbytes} bytes")


def parse_model(model_path: str) -> str:
    """
    Parses a model file using the C++ backend and returns information about it.
    """
    if not isinstance(model_path, str):
        raise TypeError("model_path must be a string.")
    return C.parse_model(model_path)