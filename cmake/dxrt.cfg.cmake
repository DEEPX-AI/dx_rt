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

option(USE_ORT "Use ONNX Runtime" ON)
option(USE_PYTHON "Use Python" ON)
option(USE_SERVICE "Use DXRT Service" ON)
