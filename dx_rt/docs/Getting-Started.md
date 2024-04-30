## Prerequisite
To proceed with the details below, installation must first be completed.  
Ensure that the necessary dependencies and build tools are installed on the host and target systems.  
## 0. Build
### non-cross-compile case
To build DXRT for the host system, execute the build script with the appropriate installation path.  
It is recommended to set the installation path to a location commonly referenced in the OS, such as `/usr/local`.  
```
./build.sh --install /usr/local
```
### cross-compile case
To cross-compile DXRT for a target system, please refer to `Build` chapter.  
```
./build.sh --arch <target_cpu>
```
## 1. Prepare a model
Pick one prebuilt model from ModelZoo.  
Since ModelZoo is still not open, prebuilt model files will be provided separately.  
```
(You will see the file structure as below )  
yolov5s_640
├── rmap.bin
├── rmap.info
└── weight.bin
```
## 2. Run simple CLI to device
Check basic device interface as following.
```
sudo ./bin/dxrt-cli --status
```
```
      Memory @ c01000000 ~ c60400000(0 ~ 5f400000), 5f400000, cur c01000000, [0, 5f400000, 0],
      Memory @ c60400000 ~ c80000000(0 ~ 1fc00000), 1fc00000, cur c60400000, [0, 1fc00000, 0],
    Device 0: type 0, var 200, addr 0xc01000000, size 0x7f000000, dma_ch 2, fw_ver 113, board rev 2, board type 1, ddr freq 4200, ddr type 1
    Device 0: voltage [825, 800, 0, 0], clock [1000, 600, 0, 0], temperature [44, 46, 0, 0], dvfs [0, 0], boot_state 0
```
## 3. Run simple benchmark with the model
```
sudo ./bin/run_model -m <model directory>
```
If the application completes successfully, you can see detection result as following.  
```
Start benchmark.
Completed benchmark.
Data test: Sequential
-----------------------------------
model
  Inference time : 4.41787ms
  FPS : 226.354
  Data Integrity Check : SKIP (0 pass, 0 fail )
-----------------------------------
...
```
## 4. Run object detection demo with the model
```
sudo ./bin/yolo -m <model directory> -p 2 -i sample/1.jpg
```
If the demo application completes successfully, you can see detection result as following.(each result values will be different from this)  
```
  Detected 7 boxes.
    BBOX:person(0) 0.838961, (382.578, 171.134, 498.847, 460.018)
    BBOX:bowl(45) 0.761698, (31.2044, 450.033, 99.4646, 491.072)
    BBOX:oven(69) 0.626437, (486.406, 309.157, 619.971, 450.749)
    BBOX:bowl(45) 0.618453, (59.1502, 393.524, 135.132, 434.191)
    BBOX:bowl(45) 0.456878, (155.886, 274.543, 184.228, 291.046)
    BBOX:person(0) 0.441233, (0.316133, 369.499, 60.3764, 415.845)
    BBOX:banana(46) 0.340904, (-0.602713, 106.704, 61.4954, 256.591)
yolov5s_640 : latency 11548us, 5472us
```
