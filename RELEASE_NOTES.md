# Changelog
### Do not edit. This document is generated automatically.
## [1.1.0] - 2024-05-22

### Features

- create release exclude file

### Fixes

- support onnxruntime library (using 1.12.1 pre-built library) (#175)
- *(CMakeLists.txt)* FAE-194 issue related to CMAKE_INSTALL_LIBDIR
- *(cpu_handle.h)* bug fix & add install script
- public repo release  action
- clean ci workspace

### Miscellaneous Tasks

- add github actions to check conventions, bump version, and draft release
- add .bumpversion.cfg
- change user
- config pre-commit
- modify release tag to, from
- change permission
- add release excluded files
- fixed syntax error in bump action
- fixed action scripts

## [1.0.0] - 2024-05-13

### Features

- *(inference_engine)* parse bitmatch mask (#128)
- add LICENSE
- add public release CI

### Fixes

- add index.md to fix 404 error on docs page
- change target branch to avoid overlap branch & tag
- change cmd for getting from/to tag

### Miscellaneous Tasks

- add CHANGELOG.md
- remove unused submodule config
- add codeowners
- add prefix v on release.ver

### cli

- fix fwugrade bug for absolute path

### hotfix

- add exclude mkdocs.yml
- change release SDK structure

## [0.6.0] - 2024-03-27

### Documentation

- modification Examples.md file
- modify Examples.md file
- modified inference option usage
- Examples.md: update multi channel detection

### Testing

- npu_validation: bitMatchType determined by outputDataInfo
- add ppu bit match & update M1A reg map(v0.10.1)
- add ppu bit match & update M1A reg map(v0.10.1)
- add ppu bit match & update M1A reg map(v0.10.1)
- add ppu bit match & update M1A reg map(v0.10.1)
- update npu_validataion test case for M1A
- dma_flush / start_again (M1A)
- update m1a default test cvg
- npu_validation: remove temp. logic for ppu bit match
- npu_validation: generalize long_aging T.C.
- update bit compare logic for M1A
- update ppu compare logic for exception case
- update dma_transfer for M1A
- add timestamp for M1A
- add timestamp for M1A
- ppu validation app
- ppu: add face format
- ppu: flip layer param temporarily to fix accuracy problem
- ppu: fix face detection
- ppu: add pose format
- ppu: improve performance
- ppu: refactoring
- ppu: add draw in parse
- update inference option of BitMatchTest
- Add irq_abnormal test
- ppu: cm4 f/w install script
- change ie option for scaling test
- add feature address tour
- add feature address tour
- add irq count testcase
- update test case
- add ppu firmware
- Apply npu 0/1/2 for haps fpga
- rapidjson
- update ppu compare logic

### Miscellaneous Tasks

- update ci

### app

- run_model: decrease number of inferences for standalone devices
- yolo: add opencv debug for bin. input
- Face ID add require parameter (model path)
- OSD&Box Blinking Bug Fix
- cli: add dumpmem
- cli: modify help
- refactoring run_model
- run_model: support model shuffle run
- run_model: support model shuffle run
- fix compile error (arm64 ubuntu20.04)
- run_model: support device shuffle run
- object_detection: add repeat parameter
- object_detection: fix err log
- hybrid: yolov7 + pidnet
- hybrid: yolov7 + pidnet
- hybrid: yolov7 + pidnet : fix color bug
- yolo(update for DX-L1)
- split yolo_l1 app
- split yolo_l1 app
- l1 demo double buffering
- l1 demo double buffering
- l1 demo double buffering

### app/lib

- add loop option in run_model

### app_temp

- fix invalid eyenix demo operation
- porting async. inference for eyenix app

### build

- cmake: add shared library builing system (defualt: static library)
- cmake: modify shared lib option name and release.sh
- cmake: modify USE_SHARED_DXRT_LIB option
- use shared library by default
- set CMAKE_RUNTIME_OUTPUT_DIRECTORY once
- fix invalid install path
- fix build error for cross compile case
- support dxrt find_package()

### cli

- add GET_INFO for board information
- update cli and voltage scaling format
- add command for npu freq & volt setting
- add command for set board id num
- modify set board id num command -> only use dev build option
- get_status
- device reset
- update fwconfig
- dump
- fw log
- add firmware update
- remove data dump txt
- change simple run condition
- sync-up run_model
- add loops option

### driver

- add npu
- npu bring-up
- npu irq
- add data in debug log
- connect cflags in build script
- prepare xdma driver
- interface to xdma driver
- pcie-dma api for xdma driver
- fix compile error
- fix build error for standalone device
- fix build error for fpga
- add external symbol for xdma
- fix RT hang for short npu task
- fix RT hang for short npu task
- Added single msi interrupt function to run npu
- remove log for inference response

### lib

- apply P/R comments
- add file pointer check, change dx_message func.
- prepare ppu data type
- update M1A reg map(v0.10.4) && Add cpu_reset
- Add cpu_reset & cpu_unreset
- dump debug registers when irq timeout
- npuif: update M1A rmap (v0.10.6)
- update cli implementation
- proto: update rmap.info
- M1A PPU integration
- memif: remove NpuType
- read last Message sequence ID
- command: hide NPU_SET for release build
- fix build error
- fix ppu output mismatch
- set runqueue size limit
- change pcie_start_addr for alveo fpga(0x1000000)
- stabilize thread operations
- fix seg. fault for delayed inference cases
- change reset api for haps
- add DVFS set/get msg type with cli update
- SET_DVFS only applies in dev mode
- apply scheduler thread
- move bit match test function to InferenceEngine
- split request units in parallel bit match test
- default double buffering for accerlator type
- support argmax model in data integrity check
- Add npu id member from devInfo
- ie: support ppu models
- remove input done irq
- ie: support ppu overflow cases
- add rapidJSON package
- npuif: m1a: split axi base per device
- update omitted memSize for m.2 b'd
- update default ddr usage
- Fixed axi base address setting error for M.2
- Fixed axi base address setting error for M.2
- fix DX-L1 npu hang
- skip input thread for standalone device
- modified the header size to change depending on .dxnn version
- modify warning message for no graph info case
- remove cpu onnx dump
- fix seg. fault for cpu->npu case
- fix sigabort for standalone device
- redesign cpu offloading
- fix seg. fault for ORT disabled case
- improve multi-input request concepts
- improve latency measurement for complex graph
- fix invalid output mem. offset
- sync-up DX-L3
- fix side effect for output mem. offset patch

### pcie

- update initial driver version
- update message lock
- Change message base address(0xD8100000)
- add api(dx_dev_get_list_size)
- update driver with response buffer pool
- update driver with response buffer pool
- update error handling
- modify pcie driver folder name
- modify pcie driver folder name

### python_package

- add bit match scripts
- add yolov7 face

### script

- update get_model.sh
- add default y option
- update get_model.sh
- add run_model script

### tool

- profiler: fix colors array overflow
- remove npu api gen

## [0.5.3] - 2023-06-14

### Testing

- npu_validation: dma_transfer SRAM oneShot
- npu_validation: update dma_transfer data type
- npu_validation: add bw scaling with dx_message
- npu_validation: add msg.h temporary
- npu_validation: add timestamp tc
- fix seg. fault for m1 8k timestamp tc

### app

- adjust imagenet demo
- add pose_ddrnet
- pose_ddrnet: fix waitKey hang
- ddrnet: fix waitKey hang
- pose_demo: update capture interval
- pose_ddrnet: increase weight for overlay frame
- add caption, remove accuracy
- add caption
- face_recognition: change name of unknown face
- yolo: improve postproc. perf. for anchorless model
- run_model: fix bit match
- yolo: update L1 ISP Phy addr
- add cli app
- cli: fix arguments
- add mlperf to app_temp build

### app_temp

- Modify to display only "npu inference" info on the monitor
- Modify to display npu inference time (GetNpuPerf) on the monitor
- update window
- L1: Fix LCD log npu name from m1 to l1
- check face position
- mlperf
- mlperf singlestream for resnet50
- revert temp. data dump logic

### lib

- npuif: npu clock info. from m1 f/w
- ie: add average inference time api
- ie: clear profiler for average api
- improve RT internal FPS perf.
- fix seg. fault for dx-m1 model
- npu timestamp for standalone devices
- set npu clock for fpga build
- device: disable npu timestamp measurement
- device: disable npu timestamp measurement
- modify npuif address type to 64bit
- improve device scheduling
- fix invalid param fowarding
- add message function w/ seq
- use prebuilt RuntimeParam. instances
- move runtimeparam shared_ptr to normal pointer
- cli implementations
- fix invalid datainfo name for multi-buffering
- fix build error for released package

### mlperf

- implment LoadSamplesToRam() in QSL
- prepare polymorphism for SUT
- multistream draft impl.

## [0.5.2] - 2023-05-10

### Testing

- npu_bit_match: rollback temp. patch
- npu_validation: change message ram addr.
- npu_validation: change default voltage of m1 8k
- npu_validation: fork scaling from stability
- npu_validation: update scaling table
- memif test
- npu_validation: update slew rate scaling
- fix build error
- rollback M1 npu voltage
- npu_validation: update scaling range from cfg
- npu_validation: add recv message cmd
- npu_validation: update recv message with seq
- fix build error
- npu_validation: support argmax match
- npu_validation: support argmax match
- add npu run-only tc
- npu_validation: dma_transfer with repeat cnt
- support both of dram/argmax compare

### app

- *(temp)* add pose_estimation (yolo-pose)
- yolo: DX-L2 ISP demo by linux framebuffer
- od: fix build error
- add pose estimation
- pose_estimation: modify name
- remove temp. app
- auto-sleep time for opencv waitKey()
- remove unused var.
- add temp. app for dev. build
- fix ddrnet

### app_temp

- add face_recognition2, imagenet_classification2, segmentation2
- add object_detection2
- imagenet classification demo
- update fix inference engine
- L1 yolo
- enable eyenix od

### build

- rollback riscv64 cross-compile path temporarily
- remove comment
- add fpga build optioin
- fix invalid build mode
- add bin dir.

### driver

- L1 npu driver update for release
- L1: change kernel dir.

### lib

- fix model find fail
- merge m1 bring-up
- improve pcie read perf.
- worker: remove unused loop
- m1 4k/8k simultaneous inference
- util: fix bit match fail for data reuse model
- ie: add benchmark
- optimize input worker latency
- avoid overlapping
- pcieif: debug log
- pcieif: improve device file open/close
- add device name for irq timeout log
- message interface to m1 firmware
- fix argmax model detection bug
- worker: add perf. debug
- fix invalid device scheduling for m1
- remove debug log
- fix memory leak

### sample

- pose estimation image sample

### script

- inference script generation for DX-M1 bring-up
- update release.sh
- install.sh draft

## [0.5.1] - 2023-04-03

### Testing

- ppu-based-yolo postproc. draft
- ppu: add detect2
- ppu: refactoring
- ppu: extend test data
- ppu: add decoding logic
- ppu: add skip logic for nms

### Miscellaneous Tasks

- settings for ci build
- settings for ci build
- update latest settings
- update latest settings
- update latest settings
- fix incorrect cp cmd
- remove prev build output
- remove prev build output - dxrt
- cleanup git repo after build
- remove container after run
- fix: missing docker command
- amend output file name with build no, commit hash
- setup for default script
- comment out some needless conda setup
- remove needless scripts
- fix: incorrect outfile refs(dxrt* -> dx_rt*)

### app

- yolo: add yolov5s_640
- packet_detection:
- add network packet classification
- packet_classification: update preproc
- *(object_detection)* ISP demo - set box colors according to labels
- yolo: improve async. inference
- fix compile error
- yolo: generalize async. inference for L1 isp demo
- *(evs)* add face detection (scrfd)
- *(evs)* add crowd counting (p2pnet)
- yolo: add tcp/ip input
- yolo: ethernet input working
- od_server: temp. port num
- yolo: draft for ethernet input scenario
- yolo: user input server IP addr for ethernet I/F
- move app_evs to app_temp
- yolo: yolov4_608 porting

### build

- change default cross-compile setting for riscv64

### driver

- remove ioctl log
- L1 npu driver update

### lib

- remove pcie log for release build
- improve job completed timing
- ie: add api : WaitForCompleted()
- improve cache flush performance for standalone device
- improve async. inference
- update transpose api

### sample

- add jpg files

### scirpt

- update file path in a53_flash

## [0.5.0] - 2023-02-24

### Documentation

- move changeLog to root dir.

### Testing

- set default xml for npu_validation
- fix TC fails for DX_L2
- fix TC fails for DX_L2
- repeated test for npu bit match test
- repeated test for npu bit match test
- dynamic memory allocation for data compare
- update dx-m1 test cfg
- fix segmentation fault
- remove exit() for bit match
- fix ie test bug
- devicemanager init
- fix tc fails for DX_L2
- enable device shuffle tc for DX_L2
- remove npu reset for timestamp test
- fix seg. fault for npu bit match test
- update model file path
- update model file path
- fix input data fail > 3GB
- fix npu long aging test bug
- fix seg. fault for argmax models

### app

- od: add yolov7, and improve postproc. simulation
- ssd: improve postproc. simulation
- add yolov7_512
- *(face_recognition)* modify get_gallary()
- *(face_recognition)* fix model path (face align model)
- re-enable all apps.
- fix order of l2 cache flush and write/read
- *(imagenet)* add imagenet classification demo (#1)
- *(imagenet_classification)* rename
- *(imagenet_classification)* visualize classification results with rect
- *(imagenet_classification)* fix default paths, add arguments
- run_model: remove default output file dump
- *(imagenet_classification)* update Examples.md

### cfg

- increase ddr usage for large model verification

### driver

- npu driver for DX_L2
- npu reset for DX_L2
- fix compile error for DX-L2
- optimize npu IRQ latency
- optimize npu IRQ latency

### lib

- prepare device auto-detection
- device auto-detection. for acc mode
- device auto-detection. for std mode
- prepare to unify npuif
- unify npuif (draft)
- unify npuif
- unify npuif
- unify npuif
- unify npuif - DX_L1
- unify npuif - DX_L1
- npuif: DX_L2 porting
- remove debug log
- fix seg. fault for DX-L1
- update rmapInfo: v0.5.1
- prepare DX_M1 4k/8k concurrent
- prepare DX_M1 4k/8k concurrent
- DX_M1 4k/8k concurrent
- disable debug memif
- ParseModel() for multiple model parameters
- fix runtime error for standalone mode
- disable npu wait time log

### npu_validation

- fix axi base tour fail for DX_L2

### script

- update release.sh
- npu bit match test helper script
- add ChangeLog md for release.sh

### tool

- minimize log for reg_rw

## [0.4.0] - 2023-01-05

### Testing

- wait for input done irq in npu bit match test
- ie: set_input

### app

- od: add ISP input interface
- od: isp debug by opencv
- od: isp debug by opencv

### driver

- add npu driver source code
- update pcie driver for interrupt

### lib

- onnxruntime integration
- workaround for input done irq timeout
- add tensor transform api
- remove task/tensor dependency, and DX_L2 npuif bring-up
- fix inference fail for cpu task
- dummy devParam for cpu task
- ie: add run() by input phy. addr.

### tool

- npu api generator md file

## [0.3.1] - 2022-12-12

### Documentation

- fix typo errors

### Testing

- fix dma tc fail for DX-L1

### app

- refactoring od app. postproc. param
- disable dump_data
- ssd512 : use_softmax true

### lib

- add temp memory region

### ver

- v0.3.1

## [0.3.0] - 2022-12-02

### Documentation

- update md
- add mkdocs
- generate README md
- generate README md
- improve examples
- fix output data format

### Testing

- fix npu validation fail in eyenix FPGA for DX_L1
- prepare ORT integration

### app

- add parse_model
- face recognition update
- *(face_recognition)* modify visualization

### build

- refactoring for unified release

### lib

- fix build error in release package
- support bytestring rmapinfo format in release build
- fix tensor api bug, and update m1 test cfg
- merge npu param format

### script

- clean release docs dir

## [0.0.1] - 2022-11-22

### Documentation

- hide undocumented classes, members

### Testing

- prepare release test
- fix build error. (disable some npu testcases temporarily)

### app

- add run_model
- fix bit match fail in riscv64
- improve OD postproc by using tensor API

### build

- remove HAL headers in release package
- separate build mode from use_prebuilt_dxrt flag
- fix invalid build mode print in release package
- remove lib. files in non-prebuilt bulid

### lib

- fix build error for gcc 7.5.0
- remove ReadOutputAll in InferenceEngine
- fix npu param format for dev. build

### sample

- remove dummy file

## [0.2.0] - 2022-11-18

### Documentation

- doxygen for cpp api (draft)
- doxygen for cpp api
- add md files

### Testing

- add CTest, googletest framework
- refactoring cmake structure
- update test cfg
- fix axi tour tc, and extend testcase definition
- apply protobuf format for testcase
- fix npu reg. write fail
- fix fail testcases for DX-M1
- add npu bit_match
- fix manual mode in A53
- fix fail testcases in A53
- ddr heavy traffic testcase
- modify test cfg naming rule
- fix test cfg. file path
- read npu output all from buffer
- increase compare buf. size
- fix data compare buf. problem
- change result xml filename
- fix bad_alloc problem in multiple pcie env.
- fix npu_validation fail
- fix DX-L1 npu validation fail
- add fail logs for npu bit match
- increase max device of dx-m1
- get inference mode from test cfg
- change npu reset case for bit-match tc
- change npu reset case for bit-match tc
- fix npu axi base tour fail
- fix npu validation fails
- fix npu validation fails

### Miscellaneous Tasks

- test commit to test ci trigger
- test commit to test ci trigger
- test commit to test ci trigger
- test commit to test ci trigger
- test commit to test ci trigger
- test commit to test ci trigger
- test commit to test ci trigger
- test commit to test ci trigger
- test commit to test ci trigger

### app

- npureg using /dev/mem
- update dxrt_comm
- object detection bring-up
- prepare e2e inference for dx-m1
- od: standalone simple test
- od: standalone e/e inference
- od: standalone e/e inference
- od: standalone e/e inference
- od: acc. e/e inference (polling)
- dmabuf write/read tool
- dmabuf write/read tool
- yolo: fix segmentation fault in aging test
- fix build error
- ssd porting
- add demo_bin
- fix demo_bin options
- add classification
- add dms yolov5s
- update yolo demo
- image segmentation demo
- hybrid - OD + Seg.
- OD + Seg. : improve perf.
- fix invalid operation in standalone mode
- hyundai mask detection demo
- face recognition
- change npu reg log setting
- add pause, stabilize
- face detection -> face alignment
- face warping
- face warping -> face recognition
- face similarity log
- fix -> convert color (BGR2RGB)
- async face detection
- async face recognition
- fix frame index
- face_recognition2
- *(face_recognition2)* modify warp function
- *(face_recognition2)* async
- *(face_recognition2)* refactor
- *(face_recognition2)* modify usage
- *(face_recognition2)* sync/async
- add yolo_pidnet

### build

- refactoring architecture
- add build.cfg, verbose
- fix typo, add api h
- fix verbose setting error
- integrate googletest, protobuf as built-in
- use ninja build
- riscv64-eyenix cross-compile
- USE_NPU_PARAM_TEXT_FORMAT=0
- refactoring 3rd party lib
- improve 3rdparty lib. integration
- opencv-integrated test verified in HAPS FPGA
- prepare dxrt release
- apply dev build for efficient release

### e/e

- improve host wait scheme
- refactoring signal threads for multi-npu
- refactoring io mode
- standalone with pcie irq

### inference

- remove allocateTensorBuffers in option
- single model per inference instance
- worker-based asynchronous inference

### lib

- implement Tensor, Model
- add Device, Task
- implementing memory, task, inf.engine
- implement
- apply shared_ptr
- split device structure
- initial impl. for task setup
- reduce debug log, impl. double buffering
- start tensor work
- expand meminfo to datainfo
- tensor: sync with datainfo
- device run interface
- inference option class
- add set_input
- remove set_output
- modify input/output connection
- beautify debug logs
- setup NPU backend integration
- npu backend: first build-up
- npuif, pcieif, memif
- npu simple inference
- fix invalid data output for npu inference
- fix sequential test bit-matching fail
- fix memif base addr. concept
- fix memif base addr. concept (arm)4
- npu driver bring-up, and fix apb fail
- connect npu irq to signal
- unified event handling
- unified event handling
- ie: add output queue
- change task/tensor instances from shared_ptr to normal pointer
- ie: add GetOutput()
- memif: move base to datainfo argument
- ie: change input setup concept
- ie: add inference mode: IO mode
- pcieif: revert default device
- move signal threads to devicenpu
- device: merge polling thread to output thread
- device: fix abnormal value change after sigwait
- npuif: fix segmentation fault
- modify e/e message concept
- refactoring thread interface
- specific device-based inference
- prepare runtime data offset
- refactoring for runtime data offset
- remove temp. node in rmap proto
- fix addr. base bug for standalone type
- fix seg. fault in task
- simplify Run()
- fix nput bit match fail
- fix invalid outputAllInfo setup
- use poll() for npu irq
- use poll() for npu irq: fix std mode bug
- npuif: fix compile error
- npuif: refacotring irq handling
- add RunFrame()

### lib/test

- revert multi-model
- prepare multi-device/model concept
- add memorymanager reset
- prepare multi-device/model concept
- prepare multi-device/model concept

### lib/tool

- refactoring NPU API structure for multiple NPU

### model

- change rmapinfo format

### npu

- DX-M1 setup
- dx-m1 v1p0 setup
- DX-L1 setup

### proto

- update
- add input mode in rmapinfo
- set protobuf ver. to 3.12.0

### script

- modify help

<!-- generated by git-cliff -->
