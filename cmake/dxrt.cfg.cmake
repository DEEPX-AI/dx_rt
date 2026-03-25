option(ENABLE_DEBUG_INFO "Include debugging informations in build output" ON)
option(ENABLE_SHOW_MODEL_INFO "Show Model Info" OFF)
if(MSVC)
	option(USE_SHARED_DXRT_LIB "Build for DXRT Shared Library" ON)
	# Define an option to select between /MT and /MD
	option(USE_MT "Use /MT (static runtime) instead of /MD (dynamic runtime)" OFF)
else()
	option(USE_SHARED_DXRT_LIB "Build for DXRT Shared Library" ON)
endif()
option(USE_DXRT_TEST "Use DXRT Unit Test" ON)
option(USE_VNPU "Use VNPU" OFF)

option(USE_ORT "Use ONNX Runtime" ON)
option(USE_PYTHON "Use Python" ON)
option(USE_SERVICE "Use DXRT Service" ON)

# Acceleration options: build with acceleration support.
# When FORCE_* is OFF (default), actual activation is controlled at runtime via environment variables:
#   - NPU format conversion acceleration: DXRT_NFH_ACCEL=1
#   - CPU op acceleration: DXRT_CPU_ACCEL=1
option(USE_NPU_FORMAT_CONVERSION_ACCELERATION "Accelerate NPU data format conversion (transpose/padding)" OFF)
option(USE_CPU_OP_ACCELERATION "Accelerate CPU-side ONNX operations" OFF)

# Note: FORCE_* options only take effect when the corresponding USE_*_ACCELERATION option is ON.
# If USE_*_ACCELERATION is OFF, the FORCE_* flag is ignored (the compile definition is never added).
# When FORCE is ON, acceleration is always enabled (env var check is skipped).
option(FORCE_NPU_FORMAT_CONVERSION_ACCELERATION "Always enable NPU format conversion acceleration (skip env var check)" OFF)
option(FORCE_CPU_OP_ACCELERATION "Always enable CPU op acceleration (skip env var check)" OFF)