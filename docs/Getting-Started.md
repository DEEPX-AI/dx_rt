# Prerequisite
To proceed with the details below, installation must first be completed.  
Ensure that the necessary dependencies and build tools are installed on the host and target systems.  
# 0. Build
## non-cross-compile case
To build DXRT for the host system, execute the build script with the appropriate installation path.  
It is recommended to set the installation path to a location commonly referenced in the OS, such as `/usr/local`.  
```
./build.sh --install /usr/local
```
## cross-compile case
To cross-compile DXRT for a target system, please refer to `Build` chapter.  
```
./build.sh --arch <target_cpu>
```
# 1. Prepare a model
Pick one prebuilt model from ModelZoo.  
Since ModelZoo is still not open, prebuilt model files will be provided separately.  
```
(You will see the file structure as below )  
yolov5s_640
└── graph.dxnn
```
# 2. Run simple CLI to device
Check basic device interface as following.
```
./bin/dxrt-cli --status
```
```
DXRT v2.0.3
Device 0: M1, Accelator type
Memory: LPDDR5 5500 MHz, 2.98GiB
Board: M.2, Rev 10.0
FW v1.5.1

NPU 0: voltage 750 mV, clock 1000 MHz, temperature 39'C
NPU 1: voltage 750 mV, clock 1000 MHz, temperature 39'C
NPU 2: voltage 750 mV, clock 1000 MHz, temperature 39'C
```
# 3. Run simple benchmark with the model
```
./bin/run_model -m <model directory>
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
