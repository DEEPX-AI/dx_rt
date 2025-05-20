
## v2.9.0 (May 2025)

- Modify the build.sh script according to cmake options.  
    : CMake option USE_ORT=ON, running build.sh --clean installs ONNX Runtime.  
    : CMake option USE_PYTHON=ON, running build.sh installs the Python package.  
    : CMake option USE_SERVICE=ON, running build.sh starts or restarts the service.  
- Addressed multithreading issues by implementing additional locks, improving stability under heavy load.  
- Fix crash on multi-device environments with more than 2 H1 cards. (>=8 devices)  
- Resolved data corruption errors that could occur in different scenarios, ensuring data integrity.  
- Fix profiler bugs.  
- Addressed issues identified by static analysis and other tools, enhancing code quality and reliability.  
- Add --use_ort flag to the run_model.py example for ONNX Runtime.  
- Add run batch function. (Python & C++)  
    : batch inference with multiple inputs and multiple outputs.  
- Minimum model file versions  
    : .dxnn file format version >= v6  
    : compiler version >= v1.15.2  
- Minimum Driver and Firmware versions  
    : RT Driver Version >= v1.5.0  
    : PCIe Driver Version >= v1.4.0  
    : Firmware Version >= v2.0.5  

## v2.8.2 (April 2025)

- Modify Inference Engine to be used with 'with' statements, and update relevant examples.  
- Add Python inference option interface with the following configurations  
- NPU Device Selection / NPU Bound Option / ORT Usage Flag  
- Display dxnn versions in parse_model (.dxnn file format version & compiler version)  
- Added instructions on how to retrieve device status information  
- Driver and Firmware versions  
    : RT Driver >= v1.3.3  
    : Firmware >= v1.6.3  

---
