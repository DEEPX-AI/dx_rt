
### Run (Synchronous)

The synchronous Run method uses a single NPU core to perform inference in a blocking manner. It can be configured to utilize multiple NPU cores simultaneously by employing threads to run each core independently. (Refer to **Figure** in **Section 5.2. Inference Workflow**)  

**Inference Engine Run (Python)**  

`run_sync_model.py`

```
# DX-RT importes
from dx_engine import InferenceEngine
...


if __name__ == "__main__":
    ...    

    # create inference engine instance with model
    ie = InferenceEngine(modelPath)

    input = [np.zeros(ie.GetInputSize(), dtype=np.uint8)]

    # inference loop
    for i in range(loop_count):

        # inference synchronously 
        # use only one npu core 
        outputs = ie.Run(input)

        # post processing 
        postProcessing(outputs)

    exit(0)
```

### RunAsync (Asynchronous)

The asynchronous Run mode is a method that performs inference asynchronously while utilizing multiple NPU cores simultaneously. It can be implemented to maximize NPU resources through a callback function or a thread wait mechanism. 

Inference Engine RunAsync, Callback, User Argument  

- the outputs are guaranteed to be valid **only** within this callback function  
- processing this callback functions as quickly as possible is beneficial for improving inference performance  
- inference asynchronously, use all npu cores  
- if `device-load >= max-load-value`, this function will block  

The following is an example of asynchronous inference using a callback function. A user argument can be used to synchronize the input with the output of the callback.  

**Inference Engine RunAsync, Callback, User Argument (Python)**

`run_async_model.py`

```
from dx_engine import InferenceEngine
...

q = queue.Queue()
gLoopCount = 0

lock = threading.Lock()

def onInferenceCallbackFunc(outputs, user_arg):

    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance

    global gLoopCount

    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:
        # user data type casting
        index, loop_count = user_arg.value
    

        # post processing
        #postProcessing(outputs);

        # something to do

        print("Inference output (callback) index=", index)

        gLoopCount += 1
        if ( gLoopCount == loop_count ) :
            print("Complete Callback")
            q.put(0)

    return 0


if __name__ == "__main__":

    ...    

    # create inference engine instance with model
    ie = InferenceEngine(modelPath)

    # register call back function
    ie.register_callback(onInferenceCallbackFunc)


    input = [np.zeros(ie.GetInputSize(), dtype=np.uint8)]

    # inference loop
    for i in range(loop_count):

        # inference asynchronously, use all npu cores
        # if device-load >= max-load-value, this function will block  
        ie.RunAsync(input, user_arg=[i, loop_count])

        print("Inference start (async)", i)

    exit(q.get())
```

The following is an example where multiple threads start input and inference, and a single callback processes the output.

Inference Engine RunAsync, Callback, User Argument, Thread  

- the outputs are guaranteed to be valid **only** within this callback function  
- processing this callback functions as quickly as possible is beneficial for improving inference performance  
- inference asynchronously, use all npu cores  
- if `device-load >= max-load-value`, this function will block  

**Inference Engine RunAsync, Callback, User Argument, Thread (Python)**

`run_async_model_thread.py`
```
from dx_engine import InferenceEngine
...

THRAD_COUNT = 3
total_count = 0
q = queue.Queue()

lock = threading.Lock()


def inferenceThreadFunc(ie, threadIndex, loopCount):

    # input
    input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]
    
    # inference loop
    for i in range(loopCount):

        # inference asynchronously, use all npu cores
        # if device-load >= max-load-value, this function will block  
        ie.RunAsync(input,user_arg = [i, loopCount, threadIndex])
    
    return 0

def onInferenceCallbackFunc(outputs, user_arg):
    # the outputs are guaranteed to be valid only within this callback function
    # processing this callback functions as quickly as possible is beneficial 
    # for improving inference performance

    global total_count

    # Mutex locks should be properly adjusted 
    # to ensure that callback functions are thread-safe.
    with lock:
        # user data type casting
        index = user_arg.value[0]
        loop_count = user_arg.value[1]
        thread_index = user_arg.value[2]

        # post processing
        #postProcessing(outputs);

        # something to do

        total_count += 1

        if ( total_count ==  loop_count * THRAD_COUNT) :
            q.put(0)

    return 0


if __name__ == "__main__":
    ...    

    # create inference engine instance with model
    ie = InferenceEngine(modelPath)

    # register call back function
    ie.register_callback(onInferenceCallbackFunc)

   
    t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, 0, loop_count))
    t2 = threading.Thread(target=inferenceThreadFunc, args=(ie, 1, loop_count))
    t3 = threading.Thread(target=inferenceThreadFunc, args=(ie, 2, loop_count))

    # Start and join
    t1.start()
    t2.start()
    t3.start()


    # join
    t1.join()
    t2.join()
    t3.join()
        

    exit(q.get())
```

The following is an example of performing asynchronous inference by creating an inference wait thread. The main thread starts input and inference, and the inference wait thread retrieves the output data corresponding to the input.

Inference Engine RunAsync, Wait  

- inference asynchronously, use all npu cores  
- if `device-load >= max-load-value`, this function will block  

**Inference Engine RunAsync, Wait (Python)**

`run_async_model_wait.py`
```
# DX-RT imports
from dx_engine import InferenceEngine
...

q = queue.Queue()


def inferenceThreadFunc(ie, loopCount):

    count = 0

    while(True):
    
        # pop item from queue 
        jobId = q.get()

        # waiting for the inference to complete by jobId
        # ownership of the outputs is transferred to the user 
        outputs = ie.Wait(jobId)

        # post processing
        # postProcessing(outputs);

        # something to do

        count += 1
        if ( count >= loopCount ):
            break
   
    return 0


if __name__ == "__main__":
    ...

    # create inference engine instance with model
    with InferenceEngine(modelPath) as ie:

        # do not register call back function
        # ie.register_callback(onInferenceCallbackFunc)

        t1 = threading.Thread(target=inferenceThreadFunc, args=(ie, loop_count))

        t1.start()

        input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]

        # inference loop
        for i in range(loop_count):

            # inference asynchronously, use all npu cores
            # if device-load >= max-load-value, this function will block  
            jobId = ie.run_async(input, user_arg=0)

            q.put(jobId)

        t1.join()

    exit(0)
    
```

### Run (Batch)
The following is an example of batch inference with multiple inputs and multiple outputs.

`run_batch_model.py`

```
   
import numpy as np
import sys
from dx_engine import InferenceEngine
from dx_engine import InferenceOption


if __name__ == "__main__":
    ...
   
    # create inference engine instance with model
    with InferenceEngine(modelPath) as ie:

        input_buffers = []
        output_buffers = []
        index = 0
        for b in range(batch_count):
            input_buffers.append([np.array([np.random.randint(0, 255)],  dtype=np.uint8)])
            output_buffers.append([np.zeros(ie.get_output_size(), dtype=np.uint8)])
            index = index + 1

        # inference loop
        for i in range(loop_count):

            # batch inference
            # It operates asynchronously internally 
            # for the specified number of batches and returns the results
            results = ie.run_batch(input_buffers, output_buffers)

            # post processing 

    exit(0)

```

### Inference Option

The following inference options allow you to specify an NPU core for performing inference.

Inference Engine Run, Inference Option  

- select devices  
  : default device is `[]`  
  : Choose the device to utilize  (ex. `[0, 2]`)  
- select bound option per device  
  : `InferenceOption.BOUND_OPTION.NPU_ALL`  
  : `InferenceOption.BOUND_OPTION.NPU_0`  
  : `InferenceOption.BOUND_OPTION.NPU_1`  
  : `InferenceOption.BOUND_OPTION.NPU_2 ` 
- use onnx runtime library (`ORT`)  
  : `set_use_ort / get_use_ort`  

NPU_ALL / NPU_0 / NPU_1 / NPU_2
```
# DX-RT imports
from dx_engine import InferenceEngine, InferenceOption
...

if __name__ == "__main__":
    ...
    
    # inference option
    option = InferenceOption()

    print("Inference Options:")

    # select devices
    option.set_devices([0])

    # NPU bound opion (NPU_ALL or NPU_0 or NPU_1 or NPU_2)
    option.set_bound_option(InferenceOption.BOUND_OPTION.NPU_ALL)

    # use ONNX Runtime (True or False)
    option.set_use_ort(False)
   
    # create inference engine instance with model
    with InferenceEngine(modelPath, option) as ie:

        input = [np.zeros(ie.get_input_size(), dtype=np.uint8)]

        # inference loop
        for i in range(loop_count):

            # inference synchronously 
            # use only one npu core 
            # ownership of the outputs is transferred to the user 
            outputs = ie.run(input)

            # post processing 
            #postProcessing(outputs)
            print("Inference outputs ", i)

    exit(0)
```

### Examples

The examples provided earlier are actual code samples that can be executed. Please refer to them for practical use. (`examples/python`)  

- `run_async_model.py`  
  : A performance-optimized example using a callback function  
- `run_async_model_thread.py`  
  : An example using a single inference engine, callback function, and thread  
  : Usage method when there is a single AI model and multiple inputs  
- `run_async_model_wait.py`  
  : An example using threads and waits  
- `run_sync_model.py`  
  : An example using a single thread  
- `run_sync_model_thread.py`  
  : An example running an inference engine on multiple threads  
- `run_sync_model_bound.py`  
  : An example of specifying an NPU using the bound option  

---
