#!/usr/bin/env python3
"""
Test script to verify C++ exception translation to Python exceptions.
"""

import sys
import os

# Add the python package to the path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'python_package/src'))

try:
    import dx_engine.capi._pydxrt as C
    from dx_engine.inference_engine import InferenceEngine
    from dx_engine.inference_option import InferenceOption
except ImportError as e:
    print(f"Import error: {e}")
    sys.exit(1)

def test_invalid_model_path():
    """Test exception handling with invalid model path."""
    print("Testing invalid model path...")
    try:
        # Try to create InferenceEngine with non-existent model
        ie = InferenceEngine("non_existent_model.dxnn")
        print("ERROR: Should have raised an exception!")
    except Exception as e:
        print(f"✅ Caught exception: {type(e).__name__}: {e}")
        return True
    return False

def test_invalid_inference_option():
    """Test exception handling with invalid inference option."""
    print("\nTesting invalid inference option...")
    try:
        # Try to create InferenceEngine with invalid option
        option = InferenceOption()
        option.use_ort = True  # This might cause issues if ORT is not available
        
        # Use a valid model path but with problematic option
        ie = InferenceEngine("test_model.dxnn", option)
        print("ERROR: Should have raised an exception!")
    except Exception as e:
        print(f"✅ Caught exception: {type(e).__name__}: {e}")
        return True
    return False

def test_direct_cpp_exception():
    """Test direct C++ exception handling."""
    print("\nTesting direct C++ exception...")
    try:
        # Try to create C++ InferenceEngine directly with invalid path
        option = C.InferenceOption()
        ie = C.InferenceEngine("non_existent_model.dxnn", option)
        print("ERROR: Should have raised an exception!")
    except Exception as e:
        print(f"✅ Caught C++ exception: {type(e).__name__}: {e}")
        return True
    return False

def main():
    """Main test function."""
    print("Testing C++ exception translation to Python...")
    print("=" * 50)
    
    success_count = 0
    total_tests = 3
    
    # Test 1: Invalid model path
    if test_invalid_model_path():
        success_count += 1
    
    # Test 2: Invalid inference option
    if test_invalid_inference_option():
        success_count += 1
    
    # Test 3: Direct C++ exception
    if test_direct_cpp_exception():
        success_count += 1
    
    print("\n" + "=" * 50)
    print(f"Test Results: {success_count}/{total_tests} tests passed")
    
    if success_count == total_tests:
        print("✅ All tests passed! Exception translation is working correctly.")
        return 0
    else:
        print("❌ Some tests failed. Exception translation needs improvement.")
        return 1

if __name__ == "__main__":
    sys.exit(main()) 