#!/usr/bin/env python3
#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#
"""
Multi-Input Model Inference Examples

This script demonstrates various approaches to perform inference with multi-input models
using the DXRT Python API. It includes examples for:

1. Single inference with dictionary format
2. Single inference with vector format  
3. Auto-split single buffer inference
4. Batch inference with explicit batch format
5. Batch inference with flattened format
6. Asynchronous inference with callback
7. Simple async inference (run_async)

Usage:
    python multi_input_inference_example.py --model <model_path>
"""

import argparse
import os
import time
import threading
import queue
from typing import List, Dict, Any, Union
import numpy as np

from dx_engine import InferenceEngine


def create_dummy_input(size: int) -> np.ndarray:
    return np.random.randint(0, 256, size=size, dtype=np.uint8)

def print_model_info(ie: InferenceEngine) -> None:
    """Print comprehensive model information."""
    print("\n" + "="*60)
    print("                    MODEL INFORMATION")
    print("="*60)
    
    print(f"Multi-input model: {ie.is_multi_input_model()}")
    
    if ie.is_multi_input_model():
        print(f"Input tensor count: {ie.get_input_tensor_count()}")
        print(f"Total input size: {ie.get_input_size()} bytes")
        print(f"Total output size: {ie.get_output_size()} bytes")
        
        # Input tensor information
        input_names = ie.get_input_tensor_names()
        input_sizes = ie.get_input_tensor_sizes()
        input_info = ie.get_input_tensors_info()
        mapping = ie.get_input_tensor_to_task_mapping()
        
        print(f"\nInput tensor info: {input_info}")
        print("Input tensor details:")
        for i, name in enumerate(input_names):
            task_name = mapping.get(name, "Unknown")
            size = input_sizes[i] if i < len(input_sizes) else "Unknown"
            print(f"  {name}: {size} bytes -> Task: {task_name}")
        
        # Output tensor information
        output_names = ie.get_output_tensor_names()
        output_sizes = ie.get_output_tensor_sizes()
        output_info = ie.get_output_tensors_info()
        
        print(f"\nOutput tensor info: {output_info}")
        print("Output tensor details:")
        for i, name in enumerate(output_names):
            size = output_sizes[i] if i < len(output_sizes) else "Unknown"
            print(f"  {name}: {size} bytes")
    
    print("="*60)


def validate_outputs(outputs: Union[List[np.ndarray], List[List[np.ndarray]]], expected_count: int, test_name: str) -> bool:
    """Validate inference outputs according to DXRT API implementation concepts."""
    print("")
    
    # Handle empty outputs case
    if outputs is None:
        print(f"[ERROR] {test_name}: Outputs is None")
        return False
    
    if not isinstance(outputs, list):
        print(f"[ERROR] {test_name}: Outputs is not a list, got {type(outputs)}")
        return False
    
    # Handle empty list case - this could be valid for some models
    if len(outputs) == 0:
        if expected_count == 0:
            print(f"[RESULT] {test_name}: Empty outputs (expected)")
            return True
        else:
            print(f"[ERROR] {test_name}: Empty outputs but expected {expected_count}")
            return False
    
    # Check if it's a batch output (List[List[np.ndarray]])
    # More robust check: if first element is a list and all elements are lists
    is_batch_output = (isinstance(outputs[0], list) and 
                      all(isinstance(item, list) for item in outputs))
    
    if is_batch_output:
        # Batch outputs - validate each sample
        for i, sample_outputs in enumerate(outputs):
            if not isinstance(sample_outputs, list):
                print(f"[ERROR] {test_name}: Sample {i} is not a list")
                return False
            
            if len(sample_outputs) != expected_count:
                print(f"[ERROR] {test_name}: Sample {i} expected {expected_count} outputs, got {len(sample_outputs)}")
                return False
            
            for j, output in enumerate(sample_outputs):
                if not isinstance(output, np.ndarray):
                    print(f"[ERROR] {test_name}: Sample {i} Output {j} is not numpy array, got {type(output)}")
                    return False
                
                # Validate tensor size (should not be empty)
                if output.size == 0:
                    print(f"[ERROR] {test_name}: Sample {i} Output {j} is empty (size=0)")
                    return False
                
                # Validate tensor shape (should have valid dimensions)
                if len(output.shape) == 0:
                    print(f"[ERROR] {test_name}: Sample {i} Output {j} has invalid shape {output.shape}")
                    return False
                
                # Additional validation: check for NaN or infinite values
                if np.any(np.isnan(output)):
                    print(f"[ERROR] {test_name}: Sample {i} Output {j} contains NaN values")
                    return False
                
                if np.any(np.isinf(output)):
                    print(f"[ERROR] {test_name}: Sample {i} Output {j} contains infinite values")
                    return False
        
        print(f"[RESULT] {test_name}: All batch outputs valid ({len(outputs)} samples)")
        return True
    else:
        # Single output (List[np.ndarray])
        if len(outputs) != expected_count:
            print(f"[ERROR] {test_name}: Expected {expected_count} outputs, got {len(outputs)}")
            return False
        
        for i, output in enumerate(outputs):
            if not isinstance(output, np.ndarray):
                print(f"[ERROR] {test_name}: Output {i} is not numpy array, got {type(output)}")
                return False
            
            # Validate tensor size (should not be empty)
            if output.size == 0:
                print(f"[ERROR] {test_name}: Output {i} is empty (size=0)")
                return False
            
            # Validate tensor shape (should have valid dimensions)
            if len(output.shape) == 0:
                print(f"[ERROR] {test_name}: Output {i} has invalid shape {output.shape}")
                return False
            
            # Additional validation: check for NaN or infinite values
            if np.any(np.isnan(output)):
                print(f"[ERROR] {test_name}: Output {i} contains NaN values")
                return False
            
            if np.any(np.isinf(output)):
                print(f"[ERROR] {test_name}: Output {i} contains infinite values")
                return False
        
        print(f"[RESULT] {test_name}: All outputs valid ({len(outputs)} tensors)")
        return True


def example1_single_inference_dictionary_no_buffer(ie: InferenceEngine) -> bool:
    """Example 1: Multi-Input Single Inference (Dictionary Format) - No Output Buffer"""
    print("\n1. Dictionary Format Single Inference (No Output Buffer)")
    print("   - Input: Dictionary mapping tensor names to data")
    print("   - API: ie.run_multi_input(input_dict) - auto-allocated output")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        print("\n","-"*60)
        return True
    
    try:
        # Get input tensor information
        input_names = ie.get_input_tensor_names()
        input_sizes = ie.get_input_tensor_sizes()
        
        # Create input data for each tensor
        input_tensors = {}
        for name, size in zip(input_names, input_sizes):
            input_tensors[name] = create_dummy_input(size)
        
        # Run inference without output buffers (auto-allocated)
        start_time = time.time()
        outputs = ie.run_multi_input(input_tensors)
        end_time = time.time()
        
        # Validate and report
        output_sizes = ie.get_output_tensor_sizes()
        expected_output_count = len(output_sizes)
        success = validate_outputs(outputs, expected_output_count, "Dictionary Format (No Buffer)")
        if success:
            print(f"         Inference time: {(end_time - start_time) * 1000:.2f} ms")
        print("\n","-"*60)
        return success
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


def example2_single_inference_vector_no_buffer(ie: InferenceEngine) -> bool:
    """Example 2: Multi-Input Single Inference (Vector Format) - No Output Buffer"""
    print("\n2. Vector Format Single Inference (No Output Buffer)")
    print("   - Input: List of arrays in tensor name order")
    print("   - API: ie.run(input_list) - auto-allocated output")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        print("\n","-"*60)
        return True
    
    try:
        # Get input tensor information
        input_sizes = ie.get_input_tensor_sizes()
        
        # Create input data in the order of get_input_tensor_names()
        input_list = []
        for size in input_sizes:
            input_data = create_dummy_input(size)
            input_list.append(input_data)
        
        # Run inference without output buffers (auto-allocated)
        start_time = time.time()
        outputs = ie.run(input_list)
        end_time = time.time()
        
        # Validate and report
        output_sizes = ie.get_output_tensor_sizes()
        expected_output_count = len(output_sizes)
        success = validate_outputs(outputs, expected_output_count, "Vector Format (No Buffer)")
        if success:
            print(f"         Inference time: {(end_time - start_time) * 1000:.2f} ms")
        print("\n","-"*60)
        return success
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


def example3_auto_split_inference_no_buffer(ie: InferenceEngine) -> bool:
    """Example 3: Auto-Split Single Buffer Inference - No Output Buffer"""
    print("\n3. Auto-Split Single Buffer Inference (No Output Buffer)")
    print("   - Input: Single concatenated buffer (auto-split)")
    print("   - API: ie.run(concatenated_buffer) - auto-allocated output")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        print("\n","-"*60)
        return True
    
    try:
        # Create concatenated input buffer
        total_input_size = ie.get_input_size()
        concatenated_input = create_dummy_input(total_input_size)
        
        # Run inference without output buffers (auto-allocated)
        start_time = time.time()
        outputs = ie.run(concatenated_input)
        end_time = time.time()
        
        # Validate and report
        output_sizes = ie.get_output_tensor_sizes()
        expected_output_count = len(output_sizes)
        success = validate_outputs(outputs, expected_output_count, "Auto-Split (No Buffer)")
        if success:
            print(f"         Inference time: {(end_time - start_time) * 1000:.2f} ms")
        print("\n","-"*60)
        return success
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


def example4_single_inference_dictionary(ie: InferenceEngine) -> bool:
    """Example 4: Multi-Input Single Inference (Dictionary Format) - With Output Buffer"""
    print("\n4. Dictionary Format Single Inference (With Output Buffer)")
    print("   - Input: Dictionary mapping tensor names to data")
    print("   - API: ie.run_multi_input(input_dict, output_buffers)")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        print("\n","-"*60)
        return True
    
    try:
        # Get input tensor information
        input_names = ie.get_input_tensor_names()
        input_sizes = ie.get_input_tensor_sizes()
        
        # Create input data for each tensor
        input_tensors = {}
        for name, size in zip(input_names, input_sizes):
            input_tensors[name] = create_dummy_input(size)
        
        # Create output buffers
        output_sizes = ie.get_output_tensor_sizes()
        output_buffers = [np.zeros(size, dtype=np.uint8) for size in output_sizes]
        
        # Run inference
        start_time = time.time()
        outputs = ie.run_multi_input(input_tensors, output_buffers=output_buffers)
        end_time = time.time()
        
        # Validate and report
        expected_output_count = len(output_sizes)
        success = validate_outputs(outputs, expected_output_count, "Dictionary Format")
        if success:
            print(f"         Inference time: {(end_time - start_time) * 1000:.2f} ms")
        print("\n","-"*60)
        return success
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


def example5_single_inference_vector(ie: InferenceEngine) -> bool:
    """Example 5: Multi-Input Single Inference (Vector Format) - With Output Buffer"""
    print("\n5. Vector Format Single Inference (With Output Buffer)")
    print("   - Input: List of arrays in tensor name order")
    print("   - API: ie.run(input_list, output_buffers)")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        print("\n","-"*60)
        return True
    
    try:
        # Get input tensor information
        input_sizes = ie.get_input_tensor_sizes()
        
        # Create input data in the order of get_input_tensor_names()
        input_list = []
        for size in input_sizes:
            input_data = create_dummy_input(size)
            input_list.append(input_data)
        
        # Create output buffers
        output_sizes = ie.get_output_tensor_sizes()
        output_buffers = [np.zeros(size, dtype=np.uint8) for size in output_sizes]
        
        # Run inference
        start_time = time.time()
        outputs = ie.run(input_list, output_buffers=output_buffers)
        end_time = time.time()
        
        # Validate and report
        expected_output_count = len(output_sizes)
        success = validate_outputs(outputs, expected_output_count, "Vector Format")
        if success:
            print(f"         Inference time: {(end_time - start_time) * 1000:.2f} ms")
        print("\n","-"*60)
        return success
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


def example6_auto_split_inference(ie: InferenceEngine) -> bool:
    """Example 6: Auto-Split Single Buffer Inference - With Output Buffer"""
    print("\n6. Auto-Split Single Buffer Inference (With Output Buffer)")
    print("   - Input: Single concatenated buffer (auto-split)")
    print("   - API: ie.run(concatenated_buffer, output_buffers)")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        print("\n","-"*60)
        return True
    
    try:
        # Create concatenated input buffer
        total_input_size = ie.get_input_size()
        concatenated_input = create_dummy_input(total_input_size)
        
        # Create output buffers
        output_sizes = ie.get_output_tensor_sizes()
        output_buffers = [np.zeros(size, dtype=np.uint8) for size in output_sizes]
        
        # Run inference
        start_time = time.time()
        outputs = ie.run(concatenated_input, output_buffers=output_buffers)
        end_time = time.time()
        
        # Validate and report
        expected_output_count = len(output_sizes)
        success = validate_outputs(outputs, expected_output_count, "Auto-Split")
        if success:
            print(f"         Inference time: {(end_time - start_time) * 1000:.2f} ms")
        print("\n","-"*60)
        return success
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


def example7_batch_inference_explicit(ie: InferenceEngine, batch_size: int = 3) -> bool:
    """Example 7: Multi-Input Batch Inference (Explicit Batch Format)"""
    print(f"\n7. Batch Inference - Explicit Format (batch_size={batch_size})")
    print("   - Input: List[List[np.ndarray]] (nested structure)")
    print("   - API: ie.run(batch_inputs)")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        return True
    
    try:
        # Get input tensor information
        input_sizes = ie.get_input_tensor_sizes()
        output_sizes = ie.get_output_tensor_sizes()
        
        # Create batch input data
        batch_inputs = []
        batch_outputs = []
        user_args = []
        
        for i in range(batch_size):
            # Create input tensors for this sample
            sample_inputs = []
            for size in input_sizes:
                input_data = create_dummy_input(size) + (i * 10)
                sample_inputs.append(input_data)
            batch_inputs.append(sample_inputs)
            
            # Create output buffers for this sample
            sample_outputs = []
            for size in output_sizes:
                output_buffer = np.zeros(size, dtype=np.uint8)
                sample_outputs.append(output_buffer)
            batch_outputs.append(sample_outputs)
            
            user_args.append(f"sample_{i}")
        
        # Run batch inference
        start_time = time.time()
        results = ie.run(batch_inputs, output_buffers=batch_outputs, user_args=user_args)
        end_time = time.time()
        
        # Validate and report
        expected_output_count = len(output_sizes)
        success = validate_outputs(results, expected_output_count, "Batch Explicit")
        if success:
            total_time_ms = (end_time - start_time) * 1000
            avg_time_ms = total_time_ms / batch_size
            print(f"     Total time: {total_time_ms:.2f} ms")
            print(f"     Average per sample: {avg_time_ms:.2f} ms")
        print("\n","-"*60)
        return success
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        return False


def example8_batch_inference_flattened(ie: InferenceEngine, batch_size: int = 3) -> bool:
    """Example 8: Multi-Input Batch Inference (Flattened Format)"""
    print(f"\n8. Batch Inference - Flattened Format (batch_size={batch_size})")
    print("   - Input: List[np.ndarray] (flattened structure)")
    print("   - API: ie.run(flattened_inputs)")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        return True
    
    try:
        # Get input tensor information
        input_sizes = ie.get_input_tensor_sizes()
        input_count = len(input_sizes)
        
        # Create flattened input data
        flattened_inputs = []
        for i in range(batch_size):
            for size in input_sizes:
                input_data = create_dummy_input(size) + (i * 10)
                flattened_inputs.append(input_data)
        
        # Create flattened output buffers
        output_sizes = ie.get_output_tensor_sizes()
        flattened_output_buffers = []
        for i in range(batch_size):
            for size in output_sizes:
                output_buffer = np.zeros(size, dtype=np.uint8)
                flattened_output_buffers.append(output_buffer)
        
        # Run batch inference
        start_time = time.time()
        results = ie.run(flattened_inputs, output_buffers=flattened_output_buffers)
        end_time = time.time()
        
        # Validate and report
        expected_output_count = len(output_sizes)
        success = validate_outputs(results, expected_output_count, "Batch Flattened")
        if success:
            total_time_ms = (end_time - start_time) * 1000
            avg_time_ms = total_time_ms / batch_size
            print(f"     Total time: {total_time_ms:.2f} ms")
            print(f"     Average per sample: {avg_time_ms:.2f} ms")
        print("\n","-"*60)
        return success
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


class AsyncInferenceHandler:
    """Handler for asynchronous inference callbacks"""
    
    def __init__(self, total_count: int):
        self.total_count = total_count
        self.completed_count = 0
        self.results = {}
        self.lock = threading.Lock()
        self.completion_queue = queue.Queue()
        self.validation_errors = []
    
    def callback(self, outputs: List[np.ndarray], user_arg: Any) -> int:
        """Callback function for async inference completion"""
        with self.lock:
            sample_id = user_arg
            self.results[sample_id] = outputs
            self.completed_count += 1
            
            # Validate outputs in callback according to DXRT API concepts
            try:
                if not isinstance(outputs, list):
                    self.validation_errors.append(f"Sample {sample_id}: outputs is not list, got {type(outputs)}")
                elif len(outputs) == 0:
                    self.validation_errors.append(f"Sample {sample_id}: empty outputs")
                else:
                    # Validate each output tensor
                    for i, output in enumerate(outputs):
                        if not isinstance(output, np.ndarray):
                            self.validation_errors.append(f"Sample {sample_id}: Output {i} is not numpy array, got {type(output)}")
                        elif output.size == 0:
                            self.validation_errors.append(f"Sample {sample_id}: Output {i} is empty (size=0)")
                        elif len(output.shape) == 0:
                            self.validation_errors.append(f"Sample {sample_id}: Output {i} has invalid shape {output.shape}")
                        elif np.any(np.isnan(output)):
                            self.validation_errors.append(f"Sample {sample_id}: Output {i} contains NaN values")
                        elif np.any(np.isinf(output)):
                            self.validation_errors.append(f"Sample {sample_id}: Output {i} contains infinite values")
                        else:
                            # Valid output tensor
                            pass
            except Exception as e:
                self.validation_errors.append(f"Sample {sample_id}: Validation error - {e}")
            
            print(f"   Async callback: {sample_id} completed ({self.completed_count}/{self.total_count})")
            
            if self.completed_count >= self.total_count:
                self.completion_queue.put("done")
        
        return 0
    
    def wait_for_completion(self, timeout: float = 30.0) -> bool:
        """Wait for all async inferences to complete"""
        try:
            self.completion_queue.get(timeout=timeout)
            return True
        except queue.Empty:
            return False
    
    def get_validation_errors(self) -> List[str]:
        """Get validation errors from callbacks"""
        return self.validation_errors.copy()


def example9_async_inference_callback(ie: InferenceEngine, async_count: int = 3) -> bool:
    """Example 9: Multi-Input Async Inference with Callback"""
    print(f"\n9. Async Inference with Callback (async_count={async_count})")
    print("   - Input: Dictionary format with callback")
    print("   - API: ie.run_async_multi_input(input_dict, callback)")
    
    if not ie.is_multi_input_model():
        print("   [WARNING]  Skipped: Not a multi-input model")
        return True
    
    try:
        # Create async handler
        handler = AsyncInferenceHandler(async_count)
        
        # Register callback
        ie.register_callback(handler.callback)
        
        # Get input tensor information
        input_names = ie.get_input_tensor_names()
        input_sizes = ie.get_input_tensor_sizes()
        
        # Submit async requests
        job_ids = []
        for i in range(async_count):
            # Create input tensors for this request
            input_tensors = {}
            for name, size in zip(input_names, input_sizes):
                input_data = create_dummy_input(size) + (i * 15)
                input_tensors[name] = input_data
            
            # Submit async inference
            user_arg = f"async_sample_{i}"
            job_id = ie.run_async_multi_input(input_tensors, user_arg=user_arg)
            job_ids.append(job_id)
        
        print(f"   Submitted {async_count} async requests (job IDs: {job_ids})")
        
        # Wait for completion
        success = handler.wait_for_completion(timeout=30.0)
        
        # Clear callback
        ie.register_callback(None)
        
        if success and len(handler.results) == async_count:
            # Check for validation errors
            validation_errors = handler.get_validation_errors()
            if validation_errors:
                print(f"   [ERROR] Async inference validation errors:")
                for error in validation_errors:
                    print(f"     {error}")
                print("\n","-"*60)
                return False
            else:
                print(f"   [RESULT] All async inferences completed successfully")
                print("\n","-"*60)
                return True
        else:
            print(f"   [ERROR] Async inference failed or timed out")
            print("\n","-"*60)
            return False
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


def example10_simple_async_inference(ie: InferenceEngine, loop_count: int = 3) -> bool:
    """Example 10: Simple Async Inference (run_async style)"""
    print(f"\n10. Simple Async Inference (loop_count={loop_count})")
    print("   - Input: Single buffer with simple async")
    print("   - API: ie.run_async(input_buffer)")
    
    try:
        # Global variables for callback
        global_loop_count = 0
        completion_queue = queue.Queue()
        lock = threading.Lock()
        
        def simple_callback(outputs, user_arg):
            nonlocal global_loop_count
            with lock:
                index, total_count = user_arg
                global_loop_count += 1
                
                # Validate outputs in callback according to DXRT API concepts
                try:
                    if not isinstance(outputs, list):
                        print(f"   [ERROR] Simple async callback {index}: outputs is not list, got {type(outputs)}")
                    elif len(outputs) == 0:
                        print(f"   [ERROR] Simple async callback {index}: empty outputs")
                    else:
                        # Validate each output tensor
                        for i, output in enumerate(outputs):
                            if not isinstance(output, np.ndarray):
                                print(f"   [ERROR] Simple async callback {index}: Output {i} is not numpy array, got {type(output)}")
                            elif output.size == 0:
                                print(f"   [ERROR] Simple async callback {index}: Output {i} is empty (size=0)")
                            elif len(output.shape) == 0:
                                print(f"   [ERROR] Simple async callback {index}: Output {i} has invalid shape {output.shape}")
                            elif np.any(np.isnan(output)):
                                print(f"   [ERROR] Simple async callback {index}: Output {i} contains NaN values")
                            elif np.any(np.isinf(output)):
                                print(f"   [ERROR] Simple async callback {index}: Output {i} contains infinite values")
                            else:
                                # Valid output tensor
                                pass
                        else:
                            # All outputs are valid
                            print(f"   Simple async callback: index={index} ({global_loop_count}/{total_count})")
                except Exception as e:
                    print(f"   [ERROR] Simple async callback {index}: validation error - {e}")
                
                if global_loop_count == total_count:
                    completion_queue.put(0)
            return 0
        
        # Register callback
        ie.register_callback(simple_callback)
        
        # Create input buffer
        input_buffer = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
        
        # Submit async inferences
        for i in range(loop_count):
            ie.run_async(input_buffer, user_arg=[i, loop_count])
        
        print(f"   Submitted {loop_count} simple async requests")
        
        # Wait for completion
        try:
            result = completion_queue.get(timeout=30.0)
            ie.register_callback(None)  # Clear callback
            
            print(f"   [RESULT] All simple async inferences completed")
            print("\n","-"*60)
            return True
            
        except queue.Empty:
            ie.register_callback(None)  # Clear callback
            print(f"   [ERROR] Simple async inference timed out")
            print("\n","-"*60)
            return False
        
    except Exception as e:
        print(f"   [ERROR] Error: {e}")
        print("\n","-"*60)
        return False


def parse_args():
    parser = argparse.ArgumentParser(description="Multi-input model inference examples")
    parser.add_argument("--model", "-m", type=str, required=True, help="Path to model file (.dxnn)")
    parser.add_argument("--loops", "-l", type=int, default=1, help="Number of inference loops (default: 1)")
    args = parser.parse_args()

    if not os.path.exists(args.model):
        parser.error(f"Model path '{args.model}' does not exist.")
    
    return args


def main():
    """Main function to run all examples"""
    args = parse_args()
    
    print("Multi-Input Model Inference Examples")
    print(f"Model: {args.model}")
    
    # Track test results
    test_results = []
    
    try:
        # Create inference engine
        with InferenceEngine(args.model) as ie:
            # Print model information once
            print_model_info(ie)
            
            # Run all examples and collect results
            print("\n" + "="*60)
            print("                    RUNNING TESTS")
            print("="*60)
            
            # Single inference tests without output buffers (auto-allocated)
            test_results.append(("Dictionary Format (No Buffer)", example1_single_inference_dictionary_no_buffer(ie)))
            test_results.append(("Vector Format (No Buffer)", example2_single_inference_vector_no_buffer(ie)))
            test_results.append(("Auto-Split (No Buffer)", example3_auto_split_inference_no_buffer(ie)))
            
            # Single inference tests with output buffers
            test_results.append(("Dictionary Format (With Buffer)", example4_single_inference_dictionary(ie)))
            test_results.append(("Vector Format (With Buffer)", example5_single_inference_vector(ie)))
            test_results.append(("Auto-Split (With Buffer)", example6_auto_split_inference(ie)))
            
            # Batch inference tests (output buffers required)
            test_results.append(("Batch Explicit", example7_batch_inference_explicit(ie, batch_size=3)))
            test_results.append(("Batch Flattened", example8_batch_inference_flattened(ie, batch_size=3)))
            
            # Async inference tests
            test_results.append(("Async Callback", example9_async_inference_callback(ie, async_count=args.loops)))
            test_results.append(("Simple Async", example10_simple_async_inference(ie, loop_count=args.loops)))
            
    except Exception as e:
        print(f"[ERROR] Critical Error: {e}")
        return 1
    
    # Print test summary
    print("\n" + "="*60)
    print("                    TEST SUMMARY")
    print("="*60)
    
    passed = 0
    failed = 0
    
    for test_name, result in test_results:
        status = "* PASS" if result else "* FAIL"
        print(f"{status} | {test_name}")
        if result:
            passed += 1
        else:
            failed += 1
    
    print("-" * 60)
    print(f"Total: {len(test_results)} | Passed: {passed} | Failed: {failed}")
    
    if failed == 0:
        print(" *** All tests passed successfully! ***")
        return 0
    else:
        print("[WARNING]  Some tests failed!")
        return 1


if __name__ == "__main__":
    exit(main()) 