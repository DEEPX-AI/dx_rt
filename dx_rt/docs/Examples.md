Please find our demo applications implemented based on the Inference Guide.
## Parse Model
Parse model, and show detailed model information  
(from `app/basic/parse_model.cpp`)  
>  -m, --model     model path  
>  -h, --help      show help  
```
parse_model -m <model_dir>
```
```
$./parse_model -m yolov5s
modelPath: yolov5s
parse non-encryption format.
compiler version info : v0.5_transpose, 1.0, 0.0
model name : yolov5s_512
number of macs : 4096
number of layers : 71
total memory usage : 34628608 Bytes
  - weight param. : 7342592 Bytes
  - npu param. : 219648 Bytes
  - input : 786432 Bytes
  - output : 26279936 Bytes
input tensors
  type : IM2COL
  [0] "INPUT", UINT32, [1, 3, 512, 512, ], 786432 Bytes
output tensors
  [0] "378", FLOAT, [64, 64, 255, ], 4194304 Bytes
  [1] "439", FLOAT, [32, 32, 255, ], 1048576 Bytes
  [2] "500", FLOAT, [16, 16, 255, ], 262144 Bytes
```
## Run Model
Simple model run demo, which measure inference time, and check output data integrity  
(from `app/basic/run_model.cpp`)  
>  -c, --config    config json file  
>  -m, --model     model path  
>  -i, --input     input data file  
>  -o, --output    file to save output data  
>  -r, --ref       reference output data file to compare  
>  -l, --loop      loop test  
>  -h, --help      show help  
```
run_model -m <model_dir> -i <input bin.> -o <output bin.> -r <reference output bin.> -l <number of loops>
-----------------------------------
loops : 100
-----------------------------------
...
...
-----------------------------------
  Inference time : 13.872ms
  FPS : 72.0879
  Bit match test : SKIP
-----------------------------------
```
Multiple model run can be configured by using config json file.  
```
run_model -c <json file>
```
Refer to following json example.  
```
{
  "models": [
    {
      "model": {
        "path": "/dxrt/m1a/1705/yolov7_640_ppu",
        "data": [
          {
            "input": "/dxrt/m1a/1705/yolov7_640_ppu/npu_input_0.bin",
            "output": "/dxrt/m1a/1705/yolov7_640_ppu/npu_output_0.ppu.bin.rt",
            "refoutput": "/dxrt/m1a/1705/yolov7_640_ppu/npu_output_0.ppu.bin"
          },
          {
            "input": "/dxrt/m1a/1705/yolov7_640_ppu/npu_input_1.bin",
            "output": "/dxrt/m1a/1705/yolov7_640_ppu/npu_output_1.ppu.bin.rt",
            "refoutput": "/dxrt/m1a/1705/yolov7_640_ppu/npu_output_1.ppu.bin"
          }
        ]
      }
    },
    {
      "model": {
        "path": "/dxrt/m1a/1705/yolov4",
        "data": [
          {
            "input": "/dxrt/m1a/1705/yolov4/npu_input_0.bin",
            "output": "/dxrt/m1a/1705/yolov4/npu_output_0.bin.rt",
            "refoutput": "/dxrt/m1a/1705/yolov4/npu_output_0.bin"
          }
        ]
      }
    },
    {
      "model": {
        "path": "/dxrt/m1a/1705/eyenix_ssdlite320"
      }
    }
  ],
  "loops_benchmark": 30,
  "loops_datacheck": 20,
  "loops_aging": 50
}
```
## Firmware Interface DXRT-CLI tool  
Read device status, and handle them by commandline interface  
(only for accelerator device)  
>  -s, --status    get chip status  
>  -i, --info      get device information  
>  -r, --reset     reset chip  
>  -d, --dump      dump device memory  
>  -h, --help      show help  
```
dxrt-cli --status
...
...
NPU0, DX_M1_4K @ 1000MHz, 850mV, 43'C
NPU1, DX_M1_8K @ 666MHz, 750mV, 44'C
```
```
dxrt-cli --reset
...
...
Reset NPU0
Reset NPU1
```
## Image Classification
Classification Demo  
for details, see `app/classification/classification.cpp`    
>   -m, --model     define model path  
>   -i, --image     use image file input  
>   -a, --async     asynchronous inference  
>   -l, --loop      number of loops to repeat  
>   -h, --help      show help  
```
classification -m <model_dir> -i <image_dir> 
```
```
$ sudo ./classification -m /dxrt/models/efficientnet-b0_argmax/ -b efficientnet.input.0.bin  

modelPath: /dxrt/models/efficientnet-b0_argmax/
binFile: efficientnet.input.0.bin
asyncInference: 0
mode: 0
Found device : /dev/dx_dma0_npu_0
Detected device id : b1000705
Found device : /dev/dx_dma0_npu_1
Detected device id : b1000705
==== Accelerator mode ====
  Task[0] /dxrt/models/efficientnet-b0_argmax/ NPUs, 8976640bytes
            Device : NPU 0, /dev/dx_dma0_npu_0(IDLE), DX_M1_4K @ 1000MHz, PCIE type
              - MemIf : /dev/dx_dma0_h2c_0, /dev/dx_dma0_c2h_0, c01000000(0) + 5f800000
    [0]
      inputs
        Tensor: name INPUT, type UINT8 [1, 224, 224, 3,  ], data 0x556d114826a0, dest 0
      outputs
        Tensor: name argmax_output, type UINT16 [ ], data 0x556d11482460, dest 0
Top1 Result : class 809
```
## Object Detection
YOLO Demo (from `app/object_detection/yolo_demo.cpp`)  
SSD Demo (from `app/object_detection/ssd_demo.cpp`)  
>   -m, --model     define model path  
>   -i, --image     use image file input  
>   -v, --video     use video file input  
>   -w, --write     write result frames to a video file  
>   -c, --camera    use camera input  
>   -x, --isp       use ISP input  
>   -b, --bin       use binary file input  
>   -s, --sim       use pre-defined npu output binary file input( perform post-proc. only )  
>   -a, --async     asynchronous inference  
>   -e, --ethernet  use ethernet input  
>   -p, --param      pre/post-processing parameter selection  
>   -l, --loop      loop test  
>   -n, --numbuf    number of memory buffers for inference  
>   -h, --help      show help  

>    If the inference time is longer than the input frame rate, the incorrect result can be displayed.   
>    In this case, increase the size of the buffer using `-n` option.(*e.g.* `-n 2`) 

### Pre/post-processing parameters
Pre/post-processing parameters not included in compiled model are required to execute the object detection application.  
Example parameters are described in `*_cfg.cpp`, and they all are listed as followings in `*_demo.cpp`.
From the list, you can select what parameter to use in runtime by using `-p` argument.  
To configure your own parameters, you can modify examples, or append a new one to the list.  
```
// pre/post parameter table
YoloParam yoloParams[] = {
    [0] = yolov5s_320,
    [1] = yolov5s_512,
    [2] = yolox_s_512,
    [3] = yolov5s_512_concat,
    [4] = yolov7_640,
    [5] = yolov7_512
};
```
```
// pre/post parameter table
SsdParam ssdParams[] = {
    [0] = mv1_ssd_300,
    [1] = mv2_ssd_320,
    [2] = mv1_ssd_512,
};
```
```
YoloParam yolov5s_512 = {
    .imgSize = 512,
    .confThreshold = 0.25,
    .scoreThreshold = 0.3,
    .iouThreshold = 0.4,
    .numClasses = 80,
    .layers = {
        {
            .numGridX = 64,
            .numGridY = 64,
            .numBoxes = 3,
            .anchorWidth = { 10.0, 16.0, 33.0 },
            .anchorHeight = { 13.0, 30.0, 23.0 },
            .tensorIdx = { 0 },
        },
    ...
    ...
};
```

* object detection for single image  
```
./yolo -m <model_dir> -i <image file> -p <param. index>
./ssd -m <model_dir> -i <image file> -p <param. index>
```
For example, if you want to execute compiled YOLO model `A` with parameter `yolov5s_512`,
```
./yolo -m A -i sample/1.jpg -p 1
```
For example, if you want to execute compiled SSD model `B` with parameter `mv1_ssd_300`,
```
./ssd -m B -i sample/1.jpg -p 0
```

* object detection for single video file  
```
./yolo -m <model_dir> -v <video file> -a -p <param. index>
./ssd -m <model_dir> -v <video file> -a -p <param. index>
```
* object detection for camera input (by OpenCV VideoCapture)  
```
./yolo -m <model_dir> -c -a -p <param. index>
./ssd -m <model_dir> -c -a -p <param. index>
```
* multi channel object detection for camera and video file input (must supported OpenCV Library)   
  support up to 4 video sources  
```
./yolo_multi -c <config_json_file>
```  
```
$ ./yolo_multi_camera -h
  yolo demo  
  -c, --config    use config json file for run application
                  e.g. sudo yolo_multi -c _multi_od_.json
  -n, --numbuf    number of memory buffers for inference
  -h, --help      show help
```   
Refer to following configuration json file. `test_od_demo.json`   
```
{
    "usage": "multi",
    "model_path": "/model_path/yolov5s_512",
    "model_name": "yolov5s_512",
    
    "video_sources": [
                    ["/video_path/video_sample0.mp4", "realtime"],
                    ["/dev/video0", "camera"],
                    ["/dev/video2", "camera"],
                    ["/video_path/video_sample1.mp4", "offline", 400]
    ], 
    
    "display_config": {
                    "display_label": "output",
                    "output_width": 1920,
                    "output_height": 1080,
                    "show_fps": true
    }
}
```    
Set "model_name" for mapping pre/post-processing parameters. :  
```
// pre/post parameter table
YoloParam yoloParams[] = {
    [0] = yolov5s_320,
    [1] = yolov5s_512, 
    [2] = yolox_s_512,
    [3] = yolov5s_512_concat,
    [4] = yolov7_640,
    [5] = yolov7_512
};
```    
```
...
    "model_path": "/model_path/yolov5s_512",
    "model_name": "yolov5s_512",
...   
```      
"realtime" for real-time capture video frames. "offline" for pre-saved frames to reducing latency of capture.    
Optional for "offline" next value, indicating number of frames to pre-saved. If you want to all frames of video, you can skip the value. :    
```
...
                    ["/video_path/video_sample0.mp4", "realtime"],
                    ["/video_path/video_sample1.mp4", "offline", 400],
                    ["/video_path/video_sample2.mp4", "offline"]
...   
```      
Camera devices should have the source type set to "camera". :    
```
...
                    ["/dev/video0", "camera"],
                    ["/dev/video2", "camera"],
...
```    
Set display windows label and display resolution. :        
```
...
    "display_config": {
                    "display_label": "output",
                    "output_width": 1920,
                    "output_height": 1080,
                    "show_fps": true
    }
...
```    

* object detection for ISP input (by standalone device ISP)  
This is only available for standalone devices (e.g. DX-L1, DX_L2)
```
./yolo -m <model_dir> -x -l 10000000 -p <param. index>
```
* object detection for ethernet input  
In this mode, device will work as a inference server by TCP/IP interface.  
So, "od_client" application should be executed from client side. Please refer to following.  
Server IP address is configured by '-p' option, and port number is set to 8080 currently.  
```
Server side
./yolo -m <model_dir> -e -a -p <param. index>
```
```
Client side
./od_client -m <model_dir> -c -p <IP address of server>
./od_client -m <model_dir> -v <video file> -p <IP address of server>
```
* object detection for single binary file  
```
./yolo -m <model_dir> -b <binary file> -p <param. index>
./ssd -m <model_dir> -b <binary file> -p <param. index>
```
```
./yolo -m /dxrt/models/DX_M1/v2p0/yolov5s -i sample/1.jpg -p 1
modelPath: /dxrt/models/DX_M1/v2p0/yolov5s
videoFile:
imgFile: sample/1.jpg
binFile:
simFile:
cameraInput: 0
asyncInference: 0
mode: 0
parse non-encryption format.
            Device : NPU 0(0x55ae03311cc0), 1
----------------------------------------------
PCIE device 0 : /dev/xdma0_user, /dev/xdma0_c2h_0, /dev/xdma0_h2c_0
character device /dev/xdma0_user opened.
Memory mapped at address 0x7f88a8dd7000.
        Task[0] @ 2106400 (/dxrt/models/DX_M1/v2p0/yolov5s) : 1 NPUs, 34628608bytes
          datainfo : rmap, filename /dxrt/models/DX_M1/v2p0/yolov5s/rmap.bin, type NONE [ ]
          datainfo : weight, filename /dxrt/models/DX_M1/v2p0/yolov5s/weight.bin, type NONE [ ]
            Device : NPU 0(0x55ae03311cc0), 1
            input tensors
              Tensor: name INPUT, type UINT32 [1, 3, 512, 512,  ], data 0x55ae03e6bec0, dest 0
              Tensor: name INPUT, type UINT32 [1, 3, 512, 512,  ], data 0x55ae049ac480, dest 0
            output tensors
              Tensor: name 378, type FLOAT [64, 64, 255,  ], data 0x55ae0392bc10, dest 0
              Tensor: name 439, type FLOAT [32, 32, 255,  ], data 0x55ae03d2bc20, dest 0
              Tensor: name 500, type FLOAT [16, 16, 255,  ], data 0x55ae03e2bd70, dest 0
              Tensor: name 378, type FLOAT [64, 64, 255,  ], data 0x55ae0446c090, dest 0
              Tensor: name 439, type FLOAT [32, 32, 255,  ], data 0x55ae0486c1e0, dest 0
              Tensor: name 500, type FLOAT [16, 16, 255,  ], data 0x55ae0496c330, dest 0
YOLO created : 16128 boxes, 80 classes, 3 layers.
  YoloParam:
    - conf_threshold: 0.25, score_threshold: 0.3, iou_threshold: 0.4, num_classes: 80, num_layers: 3
    - AnchorBoxParam: [ 16 x 16 x 3], width [116, 156, 373, ], height [90, 198, 326, ]
    - AnchorBoxParam: [ 64 x 64 x 3], width [10, 16, 33, ], height [13, 30, 23, ]
    - AnchorBoxParam: [ 32 x 32 x 3], width [30, 62, 59, ], height [61, 45, 119, ]
    - classes: [person, bicycle, car, motorcycle, airplane, bus, train, truck, boat, trafficlight, firehydrant, stopsign, parkingmeter, bench, bird, cat, dog, horse, sheep, cow, elephant, bear, zebra, giraffe, backpack, umbrella, handbag, tie, suitcase, frisbee, skis, snowboard, sportsball, kite, baseballbat, baseballglove, skateboard, surfboard, tennisracket, bottle, wineglass, cup, fork, knife, spoon, bowl, banana, apple, sandwich, orange, broccoli, carrot, hotdog, pizza, donut, cake, chair, couch, pottedplant, bed, diningtable, toilet, tv, laptop, mouse, remote, keyboard, cellphone, microwave, oven, toaster, sink, refrigerator, book, clock, vase, scissors, teddybear, hairdrier, toothbrush, ]
  OutputLayer: 64x64, 3 boxes, data align 0, data offset 0x0
  OutputLayer: 32x32, 3 boxes, data align 0, data offset 0x0
  OutputLayer: 16x16, 3 boxes, data align 0, data offset 0x0
  OutputLayer: 64x64, 3 boxes, data align 0, data offset 0x0
  OutputLayer: 32x32, 3 boxes, data align 0, data offset 0x0
  OutputLayer: 16x16, 3 boxes, data align 0, data offset 0x0
  Detected 10 boxes.
    BBOX:person(0) 0.871739, (307.538, 139.761, 401.063, 363.528)
    BBOX:bowl(45) 0.753045, (25.5786, 359.366, 78.6683, 392.692)
    BBOX:bowl(45) 0.736314, (46.0351, 315.148, 107.061, 346.346)
    BBOX:oven(69) 0.6947, (-0.0565262, 223.763, 153.919, 325.907)
    BBOX:person(0) 0.635725, (-0.137587, 294.905, 48.2289, 331.544)
    BBOX:bowl(45) 0.622758, (-0.811382, 328.491, 69.9861, 380.01)
    BBOX:oven(69) 0.583807, (391.249, 245.999, 494.725, 359.424)
    BBOX:bottle(39) 0.454198, (173.13, 269.961, 198.605, 321.687)
    BBOX:pottedplant(58) 0.440339, (-0.421099, 86.0271, 51.0569, 206.691)
    BBOX:bowl(45) 0.375233, (123.547, 219.058, 146.488, 233.043)
[*** Name :                           main| min :        36222us| max :        36222us| avg :        36222us ***]
*Max List* [36222,]
[*** Name :                           post| min :          259us| max :          259us| avg :          259us ***]
*Max List* [259,]
[*** Name :                            pre| min :         1431us| max :         1431us| avg :         1431us ***]
*Max List* [1431,]
```
![image](https://user-images.githubusercontent.com/79885630/204202825-074f6391-5fd2-41b6-9cee-5591b83f29eb.png)

## Pose estimation
Pose estimation Demo with YOLO (from `app/pose_estimation/pose_demo.cpp`)  
>   -m, --model     define model path  
>   -i, --image     use image file input  
>   -v, --video     use video file input  
>   -w, --write     write result frames to a video file  
>   -c, --camera    use camera input  
>   -x, --isp       use ISP input  
>   -b, --bin       use binary file input  
>   -s, --sim       use pre-defined npu output binary file input( perform post-proc. only )  
>   -a, --async     asynchronous inference  
>   -e, --ethernet  use ethernet input  
>   -p, --param      pre/post-processing parameter selection  
>   -l, --loop      loop test  
>   -n, --numbuf    number of memory buffers for inference  
>   -h, --help      show help  

### Pre/post-processing parameters
Pre/post-processing parameters follows same convention as in object detection.  

* pose estimation for single image  
```
./pose -m <model_dir> -i <image file> -p <param. index>
```
For example, if you want to execute compiled YOLO model `A` with parameter `yolov5s_512`,
```
./pose -m A -i sample/8.jpg -p 1
```
* pose estimation for single video file  
```
./pose -m <model_dir> -v <video file> -a -p <param. index>
```
* pose estimation for camera input (by OpenCV VideoCapture)  
```
./pose -m <model_dir> -c -a -p <param. index>
```
* pose estimation for ISP input (by standalone device ISP)  
This is only available for standalone devices (e.g. DX-L1, DX_L2)
```
./pose -m <model_dir> -x -l 10000000 -p <param. index>
```
![image](https://user-images.githubusercontent.com/79885630/233523884-02790d54-ada5-4c5b-afae-89026818c684.png)


## Image Segmentation
DDRNet demo (from `app/segmentation/ddrnet.cpp`)  
PIDNet demo (from `app/segmentation/pidnet.cpp`)  
>   -m, --model     define model path  
>   -i, --image     use image file input  
>   -v, --video     use video file input  
>   -c, --camera    use camera input  
>   -b, --bin       use binary file input  
>   -s, --sim       use pre-defined npu output binary file input( perform post-proc. only )  
>   -a, --async     asynchronous inference  
>   -o, --iomode    I/O only mode (not perform inference directly)  
>   -p, --pcie      Standalone mode: use pcie input  
>   -l, --loop      number of loops to repeat  
>   -h, --help      show help  

* segmentation for single image  
```
./ddrnet -m <model_dir> -i <image file>
./pidnet -m <model_dir> -i <image file>
```
* segmentation for single video file  
```
./ddrnet -m <model_dir> -v <video file> -a
./pidnet -m <model_dir> -v <video file> -a
```
* segmentation for camera input  
```
./ddrnet -m <model_dir> -c -a
./pidnet -m <model_dir> -c -a
```
* segmentation for single binary file  
```
./ddrnet -m <model_dir> -b <binary file>
./pidnet -m <model_dir> -b <binary file>
```
```
./pidnet -m /dxrt/models/DX_M1/v2p0/pidnet -i sample/7.png
inputWidth: 1024
inputHeight: 512
modelPath: /dxrt/models/DX_M1/v2p0/pidnet
videoFile:
binFile:
simFile:
cameraInput: 0
asyncInference: 0
mode: 0
            Device : NPU 0(0x55955bf416f0), 1
----------------------------------------------
PCIE device 0 : /dev/xdma0_user, /dev/xdma0_c2h_0, /dev/xdma0_h2c_0
character device /dev/xdma0_user opened.
Memory mapped at address 0x7f1a6a535000.
            Device : NPU 1(0x55955bf281e0), 1
----------------------------------------------
PCIE device 1 : /dev/xdma1_user, /dev/xdma1_c2h_0, /dev/xdma1_h2c_0
character device /dev/xdma1_user opened.
Memory mapped at address 0x7f1a4de01000.
        Task[0] @ 19eb0840 (/dxrt/models/DX_M1/v2p0/pidnet) : 2 NPUs, 69975552bytes
          datainfo : rmap, filename /dxrt/models/DX_M1/v2p0/pidnet/rmap.bin, type NONE [ ]
          datainfo : weight, filename /dxrt/models/DX_M1/v2p0/pidnet/weight.bin, type NONE [ ]
            Device : NPU 0(0x55955bf416f0), 1
            Device : NPU 1(0x55955bf281e0), 1
            input tensors
              Tensor: name INPUT, type UINT32 [1, 3, 512, 1024,  ], data 0x55955c26bfc0, dest 0
              Tensor: name INPUT, type UINT32 [1, 3, 512, 1024,  ], data 0x55955c5ec2e0, dest 0
              Tensor: name INPUT, type UINT32 [1, 3, 512, 1024,  ], data 0x55955c96c600, dest 0
              Tensor: name INPUT, type UINT32 [1, 3, 512, 1024,  ], data 0x55955ccec920, dest 0
              Tensor: name INPUT, type UINT32 [1, 3, 512, 1024,  ], data 0x55955d06cc40, dest 0
              Tensor: name INPUT, type UINT32 [1, 3, 512, 1024,  ], data 0x55955d3ecfe0, dest 0
              Tensor: name INPUT, type UINT32 [1, 3, 512, 1024,  ], data 0x55955d76d300, dest 0
              Tensor: name INPUT, type UINT32 [1, 3, 512, 1024,  ], data 0x55955daed620, dest 0
            output tensors
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955c16be70, dest 0
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955c4ec190, dest 0
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955c86c4b0, dest 0
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955cbec7d0, dest 0
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955cf6caf0, dest 0
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955d2ece90, dest 0
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955d66d1b0, dest 0
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955d9ed4d0, dest 0
outputs.size(): 1
              Tensor: name argmax_output, type UINT16 [1, 512, 1024,  ], data 0x55955c16be70, dest 0
1024x512 <- 676x338
[*** Name :                           main| min :        71527us| max :        71527us| avg :        71527us ***]
*Max List* [71527,]
[*** Name :                     post-blend| min :          490us| max :          490us| avg :          490us ***]
*Max List* [490,]
[*** Name :                   post-segment| min :          413us| max :          413us| avg :          413us ***]
*Max List* [413,]
[*** Name :                            pre| min :         1548us| max :         1548us| avg :         1548us ***]
*Max List* [1548,]
```
![image](https://user-images.githubusercontent.com/79885630/204205950-74d98cdf-ba6a-4b7e-817b-d0482ec8cf39.png)

## Face Detection/Recognition
Face Recognition Demo Including Face Detection (`app/face_recognition/face_recognition_demo.cpp`)   
>  -t, --threshold        Similarity Threshold  
>  -m0, --fd_modelpath    face detection model include path  
>  -m1, --lm_modelpath    face align model include path  
>  -m2, --fr_modelpath    face recognition model include path  
>  -p, --dbpath           face database directory  
>  -l, --left             first image file to compare  
>  -r, --right            second image file to compare  
>  -i, --image            use image file input  
>  -v, --video            use video file input  
>  -c, --camera           use camera input  
>  -a, --async            asynchronous inference  
>  -d, --detect           include face detection (for dual image test)  
>  -h, --help             show help  

* face recognition for single image  
```
./face_recognition -m0 <fd_model_dir> -m1 <lm_model_dir> -m2 <fr_model_dir> -p <Face DB dir> -i sample.jpg
```
* face recognition for single video file  
```
./face_recognition -m0 <fd_model_dir> -m1 <lm_model_dir> -m2 <fr_model_dir> -p <Face DB dir> -v sample.mp4 -a
```
* face recognition for camera input  
```
./face_recognition -m0 <fd_model_dir> -m1 <lm_model_dir> -m2 <fr_model_dir> -p <Face DB dir> -c -a
```
* face recognition to compare right image to left image. you can use option -d for face detection  
```
./face_recognition -m0 <fd_model_dir> -m1 <lm_model_dir> -m2 <fr_model_dir> -r <image_path> -l <image_path>
```
```
./face_recognition -m0 /fd_model -m1 /lm_model -m2 /fr_model -p /dxrt/face_db_2person -v /dxrt/videos/test.mp4
```
![image](https://user-images.githubusercontent.com/79885630/204216552-c2126f51-3a7a-49fc-a5d9-e210f42dc9c0.png)
![image](https://user-images.githubusercontent.com/79885630/204216579-b45af123-6f34-4b17-99ae-36d2be30de70.png)

## Multi-Model
Object Detection With Image Segmentation Demo (from `app/hybrid/`)  
Currently, model path is configured as in `OD_MODEL_PATH`, `SEG_MODEL_PATH` in source code.  
>   -i, --image     use image file input  
>   -v, --video     use video file input  
>   -c, --camera    use camera input  
>   -a, --async     asynchronous inference  
>   -h, --help      show help  

* YOLO + Segmentation for single image  
```
./yolo_ddrnet -m <model_dir> -i <image file>
./yolo_pidnet -m <model_dir> -i <image file>
```
* YOLO + Segmentation for single video file  
```
./yolo_ddrnet -m <model_dir> -v <video file> -a
./yolo_pidnet -m <model_dir> -v <video file> -a
```
* YOLO + Segmentation for camera input  
```
./yolo_ddrnet -m <model_dir> -c -a
./yolo_pidnet -m <model_dir> -c -a
```
```
./yolo_pidnet -a -v /dxrt/videos/stuttgart-00.avi
```
![image](https://user-images.githubusercontent.com/79885630/204217136-3c1d7175-b454-49b4-b8d9-2adc15b70676.png)

## ImageNet Classification
Image Classification Demo for ImageNet Dataset (from `app/imagenet_classification/imagenet_classification_demo.cpp`)    
Currently, default model/image/label/grid paths are configured in source code.  
(`DEFAULT_MODEL_PATH`, `DEFAULT_IMAGE_PATH`, `DEFAULT_LABEL_PATH`, `DEFAULT_GRID_PATH`)  

>  -m, --model        define model path  
>  -i, --image        ImageNet image path  
>  -l, --label        ImageNet label path  
>  -g, --grid         ImageNet grid path  
>  -h, --help         show help  
```bash
./imagenet_classification -m /dxrt/models/DX_M1/v2p0/efficientnet-b0_argmax/
```
The label file is a JSON file that contains image names and their corresponding labels, as shown below.
```json
{
  "ILSVRC2012_val_00000001": 65,
  "ILSVRC2012_val_00000002": 970,
  "ILSVRC2012_val_00000003": 230,
  ...
}
```

## Packet Classification
Packet Classification Demo (from `app/packet_classification/packet_classification.cpp`)

>  -m, --model        define model path
>  -a, --async        asynchronous inference
>  -b, --bin          use binary file input with preprocessed data
>  -h, --hazard       enable hazard mode

* packet classification for test packet
```bash
./packet_classification -m <model_dir> -a -h
```
* packet classification for single binary file
```
./packet_classification -m <model_dir> -b sample/packet/attack_packet_hazard.bin
```
Result
```
Initialize device /dev/dx_dma0_npu_0
Memory mapped at address 0x7f25f9bac000(100000)
parse non-encryption format.
  Task[0] /dxrt/m1_4k/khu-dasan-packet-224/1 NPUs, 9200000bytes
            Device : NPU 0, /dev/dx_dma0_npu_0(IDLE), DX_M1_4K, PCIE type
              - MemIf : /dev/dx_dma0_h2c_0, /dev/dx_dma0_c2h_0, 100000(0) + e0000000
    [0]
      inputs
        Tensor: name INPUT, type UINT8 [1, 224, 224, 1,  ], data 0x557a3cded080, dest 0
      outputs
        Tensor: name 250, type INT32 [1, 1, 1, 2,  ], data 0x557a3ccb6250, dest 0
[Packet Classification] result :: 0 [0:normal packet, 1:attack packet]
```
