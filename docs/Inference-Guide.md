## Model file format
Original ONNX model file is converted as following by DXCOM SDK.
```
Model dir.
    └── graph.dxnn
```
* graph.dxnn : Deepx Single Artifact with NPU Command Data & metadata / Model Parameters
## Basic Flow
![inference_basic_r1](/assets/images/basic_flow.jpg)
### 0. Prepare a model
You can choose one by 2 way
1) Pick one prebuilt model from ModelZoo  
2) Compile a model to DXRT model format (Details are in DXCOM SDK manual)  
### 1. Set inference option
Create an `dxrt::InferenceOption`. (Refer to API Reference)  
`dxrt::InferenceOption` represents detailed options for inference engine.  
- This implementation is deprecated temporarily in current version, and will be ported in next version.  
### 2. Load model to inference engine
Create an `dxrt::InferenceEngine` from the model directory.  
Required initialization of HW resources will be done automatically by doing this.  
If `dxrt::InferenceOption` is not specified, default inference option is applied.  
```
auto ie = dxrt::InferenceEngine("yolov5s");
auto ie = dxrt::InferenceEngine("yolov5s", &option);
```
### 3. Connect input tensors
Prepare input buffer(s) to perform infernce on.
You can easily prepare an input buffer with minimal effort with the code below.  
```
vector<uint8_t> inputBuf(ie.input_size(), 0);
```
Guides for connecting the inference engine to various image sources are provided in dx-app and dx-demo, along with preprocessing examples.  
### 4. Inference
Run inference engine. `outputs` is a vector of shared_ptr of `dxrt::Tensor` returned from `dxrt::InferenceEngine::Run()`.
```
auto outputs = ie.Run(inputBuf.data());
```
If you want to perform inference in a non-blocking way, you can call `dxrt::InferenceEngine::RunAsync` as follows.  
```
auto jobId = ie.RunAsync(inputBuf.data());
```
Non-blocking API issues a request ID, and you can wait for the request to complete.  
```
auto outputs = ie.Wait(jobId);
```
If you don't want to wait to run your application efficiently, you can pipeline requests with callbacks.  
```
std::function<int(vector<shared_ptr<dxrt::Tensor>>, void*)> postProcCallBack = \
    [&](vector<shared_ptr<dxrt::Tensor>> outputs, void *arg)
    {
        /* Process output tensors here */
        ... ...
        return 0;
    };
ie.RegisterCallBack(postProcCallBack);
```
For more detailed information, please check the API document.  
### 5. Process output tensors
Since inference is done, process output tensors using `Tensor` APIs and custom post-processing logic.  
You can find templates and example code in dx-app, dx-demo to help you post-process smoothly.  
As mentioned before, using callbacks allows for more efficient post-processing.  
## Multiple Device Inference
This is not applicable for single-NPU devices.  
Basically, the inference engine schedules and manages multiple devices in real time.  
If inference option is set explicitly, inference engine may only use specific devices during real-time inference for the model.  
## Device Tensor Data Format
Compiled models have tensor shape as NHWC format basically.  
Input tensor data format of current devices consists of 2 types.  
For formatter type, [1, 3, 224, 224] will be [1, 224, 224, 3] in device input tensor.  
For im2col type, [1, 3, 224, 224] will be [1, 224, 224*3+32] in device input tensor.  

|name|Compiled model format|Device format|Data size|  
|---|---|---|---|  
|Formatter|[N, H, W, C]|[N, H, W, C]|8bit|  
|IM2COL|[N, H, W, C]|[N, H, align64(W*C)]|8bit|  

Output tensor data format of current devices is aligned NHWC format.  
For example, [1, 40, 52, 36] will be [1, 52, 36, 40+24] in device output tensor.  
By using ```Tensor``` APIs, post-processing can be done without converting formats.  

|name|Compiled model format|Device format|  
|---|---|---|  
|aligned NHWC|[N, H, W, C]|[N, H, W, align64(C)]|  

API to convert from device format to NCHW/NHWC format will be supported soon.  

## Runtime Linking Error  
```
yolo: error while loading shared libraries: libprotobuf.so.23:
      cannot open shared object file: No such file or directory
or

yolo: error while loading shared libraries: libdxrt_x86_64 <or riscv64/aarch64>.so: 
      cannot open shared object file: No such file or directory
```
If you get any library(DXRT lib., openCV, googleprotobuf) linking error while executing applications, please set library path as following.  

* dxrt lib. path is `build_<CMAKE_SYSTEM_PROCESSOR>/release/lib/` in the DXRT package.  
* openCV lib. path is `<CMAKE_INSTALL_PREFIX>/lib` in your openCV build repository.  
* googleprotobuf lib. path is `extern/lib/<CMAKE_SYSTEM_PROCESSOR>` in the DXRT package.  
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<lib. path>
```

## Profile application  
### 1. Gather timing data per events
You can profile events using ```Profiler``` APIs. Please refer to API reference.  
After application is terminated, `profiler.json` will be created.  
```
auto& profiler = dxrt::Profiler::GetInstance();
profiler.Start("1sec");
sleep(1);
profiler.End("1sec");
```
### 2. Visualize profiler's data
You can visualize events from `profiler.json` using as following.  
```
python3 tool/profiler/plot.py --input profiler.json
```
Then, generated image file `profiler.png` will show detailed profiling data.  
![image](/assets/images/profiler.jpg)  
Please refer to usage of `tool/profiler/plot.py`.  
```
usage: plot.py [-h] [-i INPUT] [-o OUTPUT] [-s START] [-e END] [-g]

Draw timing chart from profiler data.

optional arguments:
  -h, --help            show this help message and exit
  -i INPUT, --input INPUT
                        Input json file to plot
  -o OUTPUT, --output OUTPUT
                        Output image file to save the plot
  -s START, --start START
                        Starting point( > 0.0) when the entire interval is 1
  -e END, --end END     End point( < 1.0) when the entire interval is 1
  -g, --show_gap        Show time gap between starting points
```
