This chapter introduces the **DX-RT** command-line tools for model inspection, inference testing, and device management. These tools allow developers to validate models, measure performance, and control hardware directly from the terminal.  

## Parse Model

This tool parses a compiled `.dxnn` model file and displays detailed information such as model structure, input/output tensors, and metadata.  

**Source**  
```
cli/parse_model.cpp
```

**Usage**  
```
parse_model -m <model_dir>
```

**Option**  
```
  - m, --model     model path
  - h, --help      show help
```

**Example**  
```
$./parse_model -m model.dxnn
```

---

## Run Model

This tool executes a compiled model to verify basic functionality, measure inference time, and optionally perform repeated runs for stress testing.


**Source**  
```
cli/run_model.cpp
```

**Usage**  
```
run_model -m <model_dir> -i <input_bin> -o <output_bin> -r <reference output_bin> -l <number of loops>
```

**Option**  
```
  -m, --model arg    Model file (.dxnn)
  -i, --input arg    Input data file
  -o, --output arg   Output data file (default: output.bin)
  -b, --benchmark    Perform a benchmark test (Maximum throughput)
                     (This is the default mode,
                      if --single or --fps > 0 are not specified)
  -s, --single       Perform a single run test
                     (Sequential single-input inference on a single-core)
  -v, --verbose      Shows NPU Processing Time and Latency
  -n, --npu arg      NPU bounding (default:0)
                       0: NPU_ALL
                       1: NPU_0
                       2: NPU_1
                       3: NPU_2
                       4: NPU_0/1
                       5: NPU_1/2
                       6: NPU_0/2
  -l, --loops arg    Number of inference loops to perform (default: 30)
  -d, --devices arg  Specify target NPU devices.
                     Examples:
                       'all' (default): Use all available/bound NPUs
                       '0': Use NPU0 only
                       '0,1,2': Use NPU0, NPU1, and NPU2
                       'count:N': Use the first N NPUs
                       (e.g., 'count:2' for NPU0, NPU1) (default: all)
  -f, --fps arg      Target FPS for TARGET_FPS_MODE (default: 0)
                     (enables this mode if > 0 and --single is not set)
      --skip-io      Attempt to skip Inference I/O (Benchmark mode only)
      --use-ort      Enable ONNX Runtime for CPU tasks in the model graph
                     If disabled, only NPU tasks operate
  -h, --help         Print usage
```

**Example**  
```
$ run_model -m /.../model.dxnn -i /.../input.bin -l 100
```

---

## DX-RT CLI Tool (Firmware Interface)

This tool provides a command-line interface for interacting with **DX-RT** accelerator devices, supporting device status queries, hardware resets, and firmware updates.

> **NOTE.**  
> This tool is applicable **only** for accelerator devices.  

**Usage**  
```
dxrt-cli <option> <argument>
```

**Option**  
```
  -s, --status             Get device status
  -i, --info               Get device info
  -m, --monitor arg        Monitoring device status every [arg] seconds 
                           (arg > 0)
  -r, --reset arg          Reset device(0: reset only NPU, 1: reset entire 
                           device) (default: 0)
  -d, --device arg         Device ID (if not specified, CLI commands will 
                           be sent to all devices.) (default: -1)
  -u, --fwupdate arg       Update firmware with deepx firmware file.
                           sub-option : [force:force update, unreset:device 
                           unreset(default:reset)]
  -w, --fwupload arg       Upload firmware with deepx firmware 
                           file.[2nd_boot/rtos]
  -g, --fwversion arg      Get firmware version with deepx firmware file
  -p, --dump arg           Dump device internals to a file
  -C, --fwconfig_json arg  Update firmware settings from [JSON]
  -v, --version            Print minimum versions
      --errorstat          show internal error status
      --ddrerror           show ddr error count
  -h, --help               Print usage
```

**Example**
```
$ dxrt-cli --status

$ dxrt-cli --reset 0

$ dxrt-cli --fwupdate fw.bin

$ dxrt-cli -m 1
```

---
