This chapter introduces **DX-RT** command-line tools for model inspection, execution, and device management.

## Parse Model

This tool is used to parse and inspect a compiled model file (`.dxnn`), printing model structure and metadata.  

**Source:** `bin/parse_model.cpp`

**Usage**  
```
parse_model -m <model_dir>
```

**Option**  

- `-m, --model`: Path to the compiled model file  (`.dxnn`)  
- `-h, --help`:	Show help message  

**Example**
```
$./parse_model -m model.dxnn
```

---

## Run Model

This tool runs a compiled model and performs a basic inference test. It measures inference time, validates output data against a reference, and optionally runs in a loop for stress testing.  

**Source:** `bin/run_model.cpp`

**Usage**  
```
run_model -m <model_dir> -i <input_bin> -o <output_bin> -r <reference output_bin> -l <number of loops>
```

**Option**  

- `-c, --config`: Path to a JSON configuration file  
- `-m, --model`: Path to the compiled model file (`.dxnn`)  
- `-i, --input`: Input binary file  
- `-o, --output`: Output file to save inference results  
- `-r, --ref`: Reference output file to compare results  
- `-l, --loop`:	Number of inference iteration to run (loop test)
- `--use-ort `: use ONNX Runtime
- `-h, --help`:	Show help message  

**Example**
```
$ run_model -m /.../model.dxnn -i /.../input.bin -l 100
```

---

## DX-RT CLI Tool (Firmware Interface)

This tool provides a command-line interface to interact with DX-RT accelerator devices. It supports querying device status, resetting hardware, updating firmware, and more.  

**Note.** This tool is applicable **only** for accelerator devices.  

**Usage**  
```
dxrt-cli <option> <argument>
```

**Option**  

- `-s, --status`: Get current device status  
- `-i, --info`:	Display basic device information  
- `-m, --monitor`: Monitoring device status every [arg] seconds (arg > 0)  
- `-r, --reset`: Reset device (0: NPU only, 1: full device) (default: 0)  
- `-d, --device`: Specify device ID (default: -1 for all device)  
- `-u, --fwupdate`: Update firmware with a Deepx firmware file (options: force:, unreset)  
- `-w, --fwupload`: Update firmware file (2nd_boot or rtos)  
- `-g, --fwversion`: Check firmware version from a firmware file  
- `-p, --dump`: Dump initial device state to a file  
- `-l, --fwlog`: Extract firmware logs to a file  
- `-h, --help`: Show help message  

**Example**
```
$ dxrt-cli --status

$ dxrt-cli --reset 0

$ dxrt-cli --fwupdate fw.bin

$ dxrt-cli -m 1
```

---
