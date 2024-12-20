Please find our demo applications implemented based on the Inference Guide.
## Parse Model
Parse model, and show detailed model information  
(from `bin/parse_model.cpp`)  
>  -m, --model     model path  
>  -h, --help      show help  
```
parse_model -m <model_dir>
```
```
$./parse_model -m model.dxnn
modelPath: /.../model.dxnn
DXNN Model Ver. : 6
...
[  ] -> npu_0 -> [ ]
  inputs
     -> npu_0
      images
  outputs
    npu_0 ->
       ...
  Task[0] npu_0, NPU, 117389760 bytes (input 786432, output 131072)
    inputs
      images, UINT8, [1, 512, 512, 3 ], 0
    outputs
      DX_tensor_5288, BBOX, [UNKNOWN ], 0
```
## Run Model
Simple model run demo, which measure inference time, and check output data integrity  
(from `bin/run_model.cpp`)  
>  -c, --config    config json file  
>  -m, --model     model path  
>  -i, --input     input data file  
>  -o, --output    file to save output data  
>  -r, --ref       reference output data file to compare  
>  -l, --loop      loop test  
>  -h, --help      show help  
```
run_model -m <model_dir> -i <input bin.> -o <output bin.> -r <reference output bin.> -l <number of loops>
```
```
$ run_model -m /.../model.dxnn -i /.../input.bin -l 100

modelFile: /.../model.dxnn
inputFile: /.../input.bin
outputFile: output.bin
benchmark: 0
loops: 100
Run model target mode : Benchmark Mode
============================================================================================
* Processing File : /.../input.bin
* Output Saved As : output.bin
* Benchmark Result(3 Cores)
  - FPS : 323.331604
============================================================================================
```
## Firmware Interface DXRT-CLI tool  
Read device status, and handle them by commandline interface  
(only for accelerator device)  

```
dxrt-cli <option> <argument>
```
> -s, --status             Get device status  
> -i, --info               Get device info  
> -m, --monitor arg        Monitoring device status every [arg] seconds  
> -r, --reset arg          Reset device(0: reset only NPU, 1: reset entire device) (default: 0)  
> -d, --device arg         Device ID (if not specified, CLI commands will be sent to all devices.) (default: -1)  
> -u, --fwupdate arg       Update firmware with deepx firmware file. sub-option : [force:force update, unreset:device unreset(default:reset)]  
> -w, --fwupload arg       Upload firmware with deepx firmware file.[2nd_boot/rtos]  
> -g, --fwversion arg      Get firmware version with deepx firmware file  
> -p, --dump arg           Dump device internals to a file  
> -l, --fwlog arg          Extract firmware logs to a file  
> -h, --help               Print usage  

```
$ dxrt-cli --status
DXRT v2.0.x
=======================================================
 * Device 0: M1, Accelator type
---------------------   Version   ---------------------
 * RT Driver version   : v1.1.0
 * PCIe Driver version : v1.1.0
-------------------------------------------------------
 * FW version          : v1.5.5
--------------------- Device Info ---------------------
 * Memory : LPDDR5 5500 MHz, 2.98GiB
 * Board  : M.2, Rev 10.0
 * PCIe   : Gen3 X4 [04:00:00]

NPU 0: voltage 750 mV, clock 1000 MHz, temperature 40'C
NPU 1: voltage 750 mV, clock 1000 MHz, temperature 40'C
NPU 2: voltage 750 mV, clock 1000 MHz, temperature 40'C
dvfs Disabled
=======================================================
```
```
$ dxrt-cli --reset 0
DXRT v2.0.x
    Device 0 reset by option 0
```
```
** Please reboot when updating firmware. **
$ dxrt-cli --fwupdate fw.bin
DXRT v2.0.x
============ FW Binary Information ============
Signature   : DEEPX GENESIS-M
Total Image : 6
Board Type  : 2
DDR Type    : 2
Firmware Ver: 1.5.2
    Device 0 update firmware[1.5.2] by /.../1.5.2/fw.bin, SubCmd:0 : SUCCESS
```
```
$ dxrt-cli -c 750 -c 1000 -c 750 -c 1000 -c 750 -c 1000
DXRT v2.0.x
    Device 0 update firmware config by 6
```
```
$ dxrt-cli -c 750 -c 1000 -c 750 -c 1000 -c 750 -c 1000
DXRT v2.0.x
    Device 0 update firmware config by 6
```
```
$ dxrt-cli -C firmware_settings.json
DXRT v2.0.x
npuID[@0]:: voltage - 750 / freq - 1000
npuID[@1]:: voltage - 750 / freq - 1000
npuID[@2]:: voltage - 750 / freq - 1000
npu throttling threshold 1 - 60
npu throttling threshold 2 - 90
npu emergency threshold - 120
npu throttling enabled - 1
    Device 0 update firmware config by 10
```
```
$ dxrt-cli -m 1
DXRT v2.0.x
NPU 0: voltage 750 mV, clock 1000 MHz, temperature 39'C
NPU 1: voltage 750 mV, clock 1000 MHz, temperature 39'C
NPU 2: voltage 750 mV, clock 1000 MHz, temperature 38'C
dvfs Disabled
=======================================================
NPU 0: voltage 750 mV, clock 1000 MHz, temperature 39'C
NPU 1: voltage 750 mV, clock 1000 MHz, temperature 39'C
NPU 2: voltage 750 mV, clock 1000 MHz, temperature 39'C
dvfs Disabled
=======================================================
NPU 0: voltage 750 mV, clock 1000 MHz, temperature 39'C
NPU 1: voltage 750 mV, clock 1000 MHz, temperature 38'C
NPU 2: voltage 750 mV, clock 1000 MHz, temperature 38'C
dvfs Disabled
=======================================================
```
