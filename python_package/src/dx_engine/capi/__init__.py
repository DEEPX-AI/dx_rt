#
# Copyright (C) 2018- DEEPX Ltd.
# All rights reserved.
#
# This software is the property of DEEPX and is provided exclusively to customers 
# who are supplied with DEEPX NPU (Neural Processing Unit). 
# Unauthorized sharing or usage is strictly prohibited by law.
#

import os
import sys

_ORT_DLL = "onnxruntime.dll"
_ORT_PROVIDERS_DLL = "onnxruntime_providers_shared.dll"


def _is_dll_debug_enabled() -> bool:
	return os.environ.get("DXRT_DEBUG_DLL_PATHS", "0") == "1"


def _debug_log(msg: str) -> None:
	if _is_dll_debug_enabled():
		print(f"[dx_engine][dll-debug] {msg}", file=sys.stderr)


def _get_process_image_path_win() -> str:
	import ctypes

	kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
	get_module_filename = kernel32.GetModuleFileNameW
	get_module_filename.argtypes = [ctypes.c_void_p, ctypes.c_wchar_p, ctypes.c_uint32]
	get_module_filename.restype = ctypes.c_uint32

	buf = ctypes.create_unicode_buffer(32768)
	length = get_module_filename(None, buf, len(buf))
	if length == 0:
		return f"<GetModuleFileNameW(None) failed: {ctypes.get_last_error()}>"

	return buf.value


def _get_loaded_module_path_win(module_name: str):
	import ctypes

	kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
	get_module_handle = kernel32.GetModuleHandleW
	get_module_handle.argtypes = [ctypes.c_wchar_p]
	get_module_handle.restype = ctypes.c_void_p

	get_module_filename = kernel32.GetModuleFileNameW
	get_module_filename.argtypes = [ctypes.c_void_p, ctypes.c_wchar_p, ctypes.c_uint32]
	get_module_filename.restype = ctypes.c_uint32

	hmodule = get_module_handle(module_name)
	if not hmodule:
		return None

	buf = ctypes.create_unicode_buffer(32768)
	length = get_module_filename(ctypes.c_void_p(hmodule), buf, len(buf))
	if length == 0:
		return f"<GetModuleFileNameW({module_name}) failed: {ctypes.get_last_error()}>"

	return buf.value


def _debug_dump_loaded_dlls(tag: str) -> None:
	if not _is_dll_debug_enabled():
		return

	_debug_log(f"dump tag={tag}")
	_debug_log(f"cwd={os.getcwd()}")
	_debug_log(f"__file__={__file__}")

	if os.name != "nt":
		_debug_log("non-Windows environment; skipping Win32 module path check")
		return

	try:
		process_path = _get_process_image_path_win()
		_debug_log(f"process-image={process_path}")

		for module_name in (
			"_pydxrt.pyd",
			"dxrt.dll",
			_ORT_DLL,
			_ORT_PROVIDERS_DLL,
		):
			path = _get_loaded_module_path_win(module_name)
			if path is None:
				_debug_log(f"{module_name}: not loaded")
			else:
				_debug_log(f"{module_name}: {path}")
	except Exception as ex:
		_debug_log(f"module-path dump failed: {ex}")


def _safe_normpath(path: str) -> str:
	return os.path.normcase(os.path.normpath(path))


def _ensure_windows_dll_search_path() -> None:
	if os.name != "nt":
		return

	capi_dir = os.path.dirname(__file__)

	if hasattr(os, "add_dll_directory"):
		try:
			os.add_dll_directory(capi_dir)
			_debug_log(f"add_dll_directory={capi_dir}")
		except Exception as ex:
			_debug_log(f"add_dll_directory failed: {ex}")


def _preload_local_ort_dlls() -> None:
	if os.name != "nt":
		return

	try:
		import ctypes
		from ctypes import wintypes

		kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
		set_last_error = kernel32.SetLastError
		set_last_error.argtypes = [wintypes.DWORD]
		set_last_error.restype = None

		capi_dir = os.path.dirname(__file__)
		ort_path = os.path.join(capi_dir, _ORT_DLL)
		providers_path = os.path.join(capi_dir, _ORT_PROVIDERS_DLL)

		if not os.path.exists(ort_path):
			_debug_log(f"local ORT DLL missing: {ort_path}")
			return

		loaded_before = _get_loaded_module_path_win(_ORT_DLL)
		if loaded_before is not None:
			if _safe_normpath(loaded_before) != _safe_normpath(ort_path):
				print(
					"[dx_engine][dll-debug] WARNING: onnxruntime.dll was already loaded from "
					f"{loaded_before} before local preload.",
					file=sys.stderr,
				)
			else:
				_debug_log(f"onnxruntime.dll already loaded from local path: {loaded_before}")

		# Load local ORT first so later delay-load lookups bind to this instance.
		set_last_error(0)
		ctypes.WinDLL(ort_path)
		loaded_after = _get_loaded_module_path_win(_ORT_DLL)
		if loaded_after is not None:
			_debug_log(f"onnxruntime.dll after local preload: {loaded_after}")

		if os.path.exists(providers_path):
			set_last_error(0)
			ctypes.WinDLL(providers_path)
			loaded_provider = _get_loaded_module_path_win(_ORT_PROVIDERS_DLL)
			if loaded_provider is not None:
				_debug_log(f"onnxruntime_providers_shared.dll after preload: {loaded_provider}")

	except Exception as ex:
		_debug_log(f"local ORT preload failed: {ex}")

_ensure_windows_dll_search_path()
_preload_local_ort_dlls()
_debug_dump_loaded_dlls("dx_engine.capi package import")
