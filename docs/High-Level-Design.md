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
DXRT Software stack is partitioned to 3 layers.

* NN API layer is designed to provide pulic inference APIs, tensor processing APIs to user's application SW.  
* Library layer is designed for actual implementation of runtime framework in userspace, and partitioned to various C++ classes that abstracts resources for inference. It also plays an important role in exchanging data related to inference with device drivers.  
* Device driver layer is responsible for communicating with firmware, managing inference, and HW-dependent implementation of NPU or PCIe functions (Device driver repository will be split from DXRT in future).  
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
Memory class allocates required memory when both InferenceEngine is created and inference is requested.
In accelerator mode, NPU memory and host system memory is managed seperately. Memory class manage NPU Memory.
NPU Memory area is divided to feature Memory and model Memory. Feature Memory has input and output data. Model memory has model rmap and weight data.
Host system memory is provided by caller for inputs, and managed by Buffer class for outputs.
In standalone mode, contiguous reserved memory by linux CMA(Contiguous Memory Allocator) is used as NPU data interface.  
![image](/assets/images/memory_allocation.jpg)

