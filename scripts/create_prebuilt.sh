#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DX_RT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
WORKSPACE_ROOT="$(cd "${DX_RT_ROOT}/.." && pwd)"

PREBUILT_DIR="${DX_RT_ROOT}/prebuilt"
PREBUILT_BIN_DIR="${PREBUILT_DIR}/bin"
PREBUILT_INCLUDE_DIR="${PREBUILT_DIR}/include"
PREBUILT_LIB_DIR="${PREBUILT_DIR}/lib"
PREBUILT_PYTHON_DIR="${PREBUILT_DIR}/python"
PREBUILT_SERVICE_DIR="${PREBUILT_DIR}/service"

SRC_BIN_DIR="${WORKSPACE_ROOT}/prebuilt_files/bin"
SRC_LIB_DIR="${WORKSPACE_ROOT}/prebuilt_files/lib"
SRC_INCLUDE_DXRT_DIR="/usr/local/include/dxrt"
SRC_WHEELHOUSE="${DX_RT_ROOT}/python_package/wheelhouse"
SRC_SERVICE_DIR="${DX_RT_ROOT}/service"
RELEASE_VER_FILE="${DX_RT_ROOT}/release.ver"

REQUIRED_BINS=(
  dxcli
  dxrun
  dxtop
  dxbenchmark
  dxrtd
  dxparse
)

log() {
  echo "[create_prebuilt] $*"
}

warn() {
  echo "[create_prebuilt][WARN] $*" >&2
}

require_file() {
  local file_path="$1"
  if [[ ! -f "${file_path}" ]]; then
    echo "[create_prebuilt][ERROR] Missing file: ${file_path}" >&2
    exit 1
  fi
}

create_dirs() {
  mkdir -p \
    "${PREBUILT_BIN_DIR}" \
    "${PREBUILT_INCLUDE_DIR}" \
    "${PREBUILT_LIB_DIR}" \
    "${PREBUILT_PYTHON_DIR}" \
    "${PREBUILT_SERVICE_DIR}"
}

copy_bins() {
  local bin_name
  for bin_name in "${REQUIRED_BINS[@]}"; do
    require_file "${SRC_BIN_DIR}/${bin_name}"
    cp -f "${SRC_BIN_DIR}/${bin_name}" "${PREBUILT_BIN_DIR}/${bin_name}"
    chmod +x "${PREBUILT_BIN_DIR}/${bin_name}"
  done
}

copy_include_dxrt() {
  if [[ ! -d "${SRC_INCLUDE_DXRT_DIR}" ]]; then
    echo "[create_prebuilt][ERROR] Missing include directory: ${SRC_INCLUDE_DXRT_DIR}" >&2
    exit 1
  fi

  rm -rf "${PREBUILT_INCLUDE_DIR}/dxrt"
  cp -a "${SRC_INCLUDE_DXRT_DIR}" "${PREBUILT_INCLUDE_DIR}/"
}

get_architecture() {
  local arch
  arch=$(uname -m)
  case "${arch}" in
    x86_64)
      echo "x86_64"
      ;;
    armv7l|armv6l|aarch64)
      echo "arm"
      ;;
    *)
      echo "unknown"
      ;;
  esac
}

copy_onnx_libs() {
  local arch
  arch=$(get_architecture)
  
  local onnx_version
  if [[ "${arch}" == "arm" ]]; then
    onnx_version="1.20.1"
  else
    onnx_version="1.22.0"
  fi
  
  local onnx_real="libonnxruntime.so.${onnx_version}"
  require_file "${SRC_LIB_DIR}/${onnx_real}"

  cp -a "${SRC_LIB_DIR}/${onnx_real}" "${PREBUILT_LIB_DIR}/${onnx_real}"

  rm -f "${PREBUILT_LIB_DIR}/libonnxruntime.so.1" "${PREBUILT_LIB_DIR}/libonnxruntime.so"
  ln -s "${onnx_real}" "${PREBUILT_LIB_DIR}/libonnxruntime.so.1"
  ln -s "libonnxruntime.so.1" "${PREBUILT_LIB_DIR}/libonnxruntime.so"
  
  log "Copied libonnxruntime.so.${onnx_version} for ${arch}"
}

copy_and_relink_libdxrt() {
  require_file "${SRC_LIB_DIR}/libdxrt.so"
  require_file "${RELEASE_VER_FILE}"

  local raw_version
  raw_version="$(tr -d '[:space:]' < "${RELEASE_VER_FILE}")"
  local version="${raw_version#v}"

  if [[ -z "${version}" ]]; then
    echo "[create_prebuilt][ERROR] Invalid version in ${RELEASE_VER_FILE}: '${raw_version}'" >&2
    exit 1
  fi

  local major="${version%%.*}"
  if [[ -z "${major}" ]]; then
    echo "[create_prebuilt][ERROR] Cannot parse major version from: '${version}'" >&2
    exit 1
  fi

  local versioned_lib="libdxrt.so.${version}"
  local major_link="libdxrt.so.${major}"

  cp -f "${SRC_LIB_DIR}/libdxrt.so" "${PREBUILT_LIB_DIR}/${versioned_lib}"

  ln -sfn "${versioned_lib}" "${PREBUILT_LIB_DIR}/${major_link}"
  ln -sfn "${major_link}" "${PREBUILT_LIB_DIR}/libdxrt.so"

  log "libdxrt links configured: libdxrt.so -> ${major_link} -> ${versioned_lib}"
}

copy_python_wheels() {
  if [[ ! -d "${SRC_WHEELHOUSE}" ]]; then
    warn "Missing python wheelhouse directory: ${SRC_WHEELHOUSE} (continuing)"
    return
  fi

  shopt -s nullglob
  local wheels=("${SRC_WHEELHOUSE}"/*.whl)
  shopt -u nullglob

  if [[ ${#wheels[@]} -eq 0 ]]; then
    warn "No .whl files found in ${SRC_WHEELHOUSE} (continuing)"
    return
  fi

  cp -f "${wheels[@]}" "${PREBUILT_PYTHON_DIR}/"
  log "Copied ${#wheels[@]} wheel file(s) to ${PREBUILT_PYTHON_DIR}"
}

copy_service_files() {
  if [[ ! -d "${SRC_SERVICE_DIR}" ]]; then
    warn "Missing service source directory: ${SRC_SERVICE_DIR} (continuing)"
    return
  fi

  shopt -s nullglob dotglob
  local entries=("${SRC_SERVICE_DIR}"/*)
  shopt -u nullglob dotglob

  if [[ ${#entries[@]} -eq 0 ]]; then
    warn "Service source directory is empty: ${SRC_SERVICE_DIR} (continuing)"
    return
  fi

  cp -a "${entries[@]}" "${PREBUILT_SERVICE_DIR}/"
  log "Copied service files from ${SRC_SERVICE_DIR}"
}

main() {
  log "Creating prebuilt directory structure"
  create_dirs

  log "Copying required binaries"
  copy_bins

  log "Copying dxrt include directory"
  copy_include_dxrt

  log "Copying ONNX Runtime libraries"
  copy_onnx_libs

  log "Copying and relinking libdxrt"
  copy_and_relink_libdxrt

  log "Copying python wheel artifacts"
  copy_python_wheels

  log "Copying service files"
  copy_service_files

  log "Done: ${PREBUILT_DIR}"
}

main "$@"