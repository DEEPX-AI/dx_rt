## Overview
Compiled NN model is imported as an ```InferenceEngine``` instance, just run it.  
```InferenceEngine``` will do :  
- initialization of DEEPX devices  
- memory management for inference  
- real-time inference job scheduling based on optimized NPU/CPU interaction  

Before running ```InferenceEngine```, ```InferenceOption``` should be established, which describes what device to use, how much resources to use.  
Also, example applications provide typical pre/post-processing attached to OpenCV for NN inference.  
![image](https://user-images.githubusercontent.com/79885630/199195923-ad8ebb22-c249-4f74-b4d7-b1177851ee6e.png)
## Static Architecture
DXRT Software stack is partitioned to 3 layers.

* NN API layer is designed to provide pulic inference APIs, tensor processing APIs to user's application SW.  
* Library layer is designed for actual implementation of runtime framework in userspace, and partitioned to various C++ classes that abstracts resources for inference. It also plays an important role in exchanging data related to inference with device drivers.  
* Device driver layer is responsible for communicating with firmware, managing inference, and HW-dependent implementation of NPU or PCIe functions (Device driver repository will be split from DXRT in future).  
![image](https://github.com/KOMOSYS/dx_rt/assets/79885630/5237730a-f8a2-4231-adfc-d15cf6f8baa1)

## Inference Concept
In point of high-level view, ```InferenceEngine``` abstracts all the steps of inference.  
When ```InferenceEngine``` is created, compiled model is loaded to one ```Model```, which is partitioned and converted to graph of ```Task```. ```Task``` represents NN operations information, and describes where(NPU or CPU) should it be done.  
Model parameters for each task will be written to proper device memory, and required memory allocation in both side(host, device) will be done automatically.  
Then, tensors are created based on the model parameters, and attached to each tasks. So, ```Worker``` instances will execute the task graph asynchronously internally. As a result, selected ```Device``` or ```CpuHandle``` by ```Worker``` will perform NN computations(if needed, memory write/read operations of tensors will be occured).  
## Compiler Interface
Compiled model consists of NPU task, and CPU task parameters as followings.  
![image](https://user-images.githubusercontent.com/79885630/199427311-32debba2-d013-4e29-bfcf-7a5e1f62e66c.png)
## Device Allocation
```Worker``` will allocate real-time available device for each requested task.  
## Memory Allocation for DEEPX devices
MemoryManager allocates required memory when InferenceEngine is created.  
In accelerator mode, non-contiguous userspace heap is used as NPU data interface.  
In standalone mode, contiguous reserved memory by linux CMA(Contiguous Memory Allocator) is used as NPU data interface.  
![image](https://user-images.githubusercontent.com/79885630/210707184-abf19711-5b58-431c-9a84-b0f5c3b17af2.png)

