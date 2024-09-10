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
$ run_model -m <model_dir> -i <input bin.> -o <output bin.> -r <reference output bin.> -l <number of loops>
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

## Firmware Interface DXRT-CLI tool  
Read device status, and handle them by commandline interface  
(only for accelerator device)  
>  -s, --status         Get device status
  -i, --info           Get device info
  -r, --reset arg      Reset device(0: reset only NPU, 1: reset entire
                       device) (default: 0)
  -d, --device arg     Device ID (if not specified, CLI commands will be
                       sent to all devices.) (default: -1)
  -p, --dump arg       Dump device internals to a file
  -c, --fwconfig arg   Update firmware settings from list of parameters
  -l, --fwlog arg      Extract firmware logs to a file
  -u, --fwupdate arg   Update firmware with deepx firmware file.
                       sub-option : [force:force update, reset:device
                       reset]
  -g, --fwversion arg  Get firmware version with deepx firmware file
  -h, --help           Print usage

```
$ dxrt-cli --status
...
...
======================================
Device 0: type ACC,variant M1,DMA_CH:2ch,fw_ver:2.0.2
Memory:[Type:LPDDR4, Addr:0xc01000000, size: 1.98GiB(2,130,706,432Byte), clock: 4200dMHz]
Board:[Type SOM,Revision:0.2],Interface:ASIC, fw_info:dxfw 2.0.2, 2024-04-26 15:35:51, GNU 10.3.1, aarch64
NPU 0: voltage 825 mV, clock 1000 MHz, temperature 50'C
NPU 1: voltage 800 mV, clock 600 MHz, temperature 51'C
dvfs Disabled, boot_state:0, cnt [0, 0, 0, 0],
======================================

```
```
$ dxrt-cli --reset
...
...
    Device 0 reset by option 1
Device 0 : 0
```
```
** Please reboot when updating firmware. **
$ dxrt-cli --fwupdate fw.bin --fwupdate force
...
...
============ FW Binary Information ============
Signature   : DEEPX GENESIS-M
Total Image : 5
Board Type  : 1
DDR Type    : 1
FIrmware Ver: 2.0.2
    Device 0 update firmware[2.0.2] by fw.bin, SubCmd:4 : SUCCESS
Device 0 : 0
```