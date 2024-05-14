## v0.1.0 / 2022-06-30  
- Initial release for DX-L1 (eyenix FPGA)  

## v0.2.0 / 2022-11-22  
- Common framework for devices  
- Support OpenCV for riscv64, arm64  
- Support documentation as markdown format  
- Added doxygen for API reference  
- Separate dev-build, release-build  
- Support encrypted NPU parameters  

## v0.3.0 / 2022-12-05  
- Add parse_model application  
- Refactor face recognition application  
- Change build architecture for auto-release  
- Improve docs.  

## v0.3.1 / 2022-12-12  
- Improve object detection pre/post parameter  
- Unified post-processing for yolov5, yolox  
- Support documents generation  

## v0.4.0 / 2023-01-05  
- Add ISP interface for object detection demo  
- Add ONNX runtime interface for CPU task (verified only x86_64)  
- Refactor device parameters in runtime lib.  
  
## v0.5.0 / 2023-02-22  
- Device variant/type setting is removed from build script (Device auto-detection is applied).  
- Added yolov7 configurations in object detection app.  
- Added PCIe driver (only for DX-M1 FPGA)  
- Reduced interrupt latency for standalone device  

## v0.5.1 / 2023-04-03  
- Added post-processing callback API  
- Added ethernet input scenario for yolo demo  
- Added tensor transpose API  
- Expand model image size parameter for yolo  
- Added network packet classification application  
- Added source code of L1 NPU driver  

## v0.5.2 / 2023-05-10  
- Support DX-M1 ASIC  
- Added PCIe driver build environment for DX-M1  
- Added pose estimation application  
- Added FPS estimation for run_model application  
 
## v0.5.3 / 2023-06-14  
- Support DX-H1 ASIC  
- Added firmware CLI tool for DX-M1, DX-H1  
- Improve YOLOX postproc. performance  

## v0.5.4 / 2023-07-17  
- Changed classification demo name 
- Support model configuration for real time face recognition demo  
- Support to receive device information and dump device memory from commandline interface application 

## v1.0.0 / 2024-04-29
### 1. Changed
- DXNN Version2 architecture sdk
- Remove driver folder
  Please refer to "dx_rt_npu_linux_driver".
### 2. Fixed
- None
### 3. Added
- Pybind11 support
  DXRT supports some Python APIs.
