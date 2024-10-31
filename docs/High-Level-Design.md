## Overview
Compiled NN model is imported as an ```InferenceEngine``` instance, just run it.  
```InferenceEngine``` will do :  
- initialization of DEEPX devices  
- memory management for inference  
- real-time inference job scheduling based on optimized NPU/CPU interaction  

Before running ```InferenceEngine```, ```InferenceOption``` should be established, which describes what device to use, how much resources to use.  
Also, example applications provide typical pre/post-processing attached to OpenCV for NN inference.  
  
![image](/assets/images/overview.jpg)
  
## Static Architecture
DXRT Software stack is partitioned to 4 layers.

* The NN API layer is designed to provide public inference APIs, tensor processing APIs for user applications, and Python APIs.
* The Library layer is designed for actual implementation of runtime framework in userspace, and partitioned to various C++ classes that abstracts resources for inference. It also plays an important role in exchanging data related to inference with device drivers and service.
* The Service layer is designed to support multiple processes and multiple devices, and is configured to communicate with each process to allocate inference to the most suitable device.
* The Device driver layer is responsible for communicating with firmware, managing inference, and HW-dependent implementation of NPU or PCIe functions (Device driver repository will be split from DXRT in future).  
  
![image](/assets/images/static_architecture.jpg)
  
## Inference Concept
In point of high-level view, ```InferenceEngine``` abstracts all the steps of inference.  
When ```InferenceEngine``` is created, compiled model is loaded to one ```Model```, which is partitioned and converted to graph of ```Task```. ```Task``` represents NN operations information, and describes where(NPU or CPU) should it be done.  
Model parameters for each task will be written to proper device memory, and required memory allocation in both side(host, device) will be done automatically.  
Then, tensors are created based on the model parameters, and attached to each tasks. So, ```Worker``` instances will execute the task graph asynchronously internally. As a result, selected ```Device``` or ```CpuHandle``` by ```Worker``` will perform NN computations(if needed, memory write/read operations of tensors will be occured).  
## Compiler Interface
Compiled model consists of NPU task, and CPU task parameters as followings.  
  
![image](/assets/images/inference_concept.jpg)
  
## Device Allocation
```Worker``` will allocate real-time available device for each requested task.  
## Memory Allocation for DEEPX devices
When the InferenceEngine was created, memory class allocation was required on the system. 
In case of accelerator mode, each NPU memory and host memory is managed separately by the memory class allocator. NPU-side memory is divided into the feature area and the model area. The feature area is allocated for input and output data, and the model area is allocated for the compiled model. In host-side memory, the input feature is provided by the caller, and the output as inference result is managed by the buffer class. 
In case of standalone mode, contiguous reserved memory is used for the NPU data interface by Linux CMA (Contiguous Memory Allocator).  
  
![image](/assets/images/memory_allocation.jpg)
  

