#!/bin/bash
# Sanity Check script for the Debian-packaged deepx SDK layout.
# Verifies executables/libraries under /usr/local and Python wheels shipped
# under /usr/share/libdxrt-bin/python.

set -u

LOCAL_BIN_DIR="/usr/local/bin"
LOCAL_LIB_DIR="/usr/local/lib"
DEB_PY_DIR="/usr/share/libdxrt-bin/python"

REQUIRED_BINS=(
    dxcli
    dxrun
    dxtop
    dxbenchmark
    dxrtd
    dxparse
)

# Libraries required at /usr/local/lib. Symlink chains are treated as one
# entry (checked separately below).
REQUIRED_LIBS=(
    libdxrt.so
    libonnxruntime.so
)

# Python wheel tags required under /usr/share/libdxrt-bin/python.
REQUIRED_WHEEL_TAGS=(
    cp38
    cp39
    cp310
    cp311
    cp312
    cp313
    cp314
)

function ExecutableFileCheck() {
    echo "==== Executable File Check (${LOCAL_BIN_DIR}) ===="
    local err=0 bin path
    for bin in "${REQUIRED_BINS[@]}"; do
        path="${LOCAL_BIN_DIR}/${bin}"
        if [[ ! -f "$path" ]]; then
            echo "[ERROR] ${path} ...MISSING"
            err=1
            continue
        fi
        if [[ ! -x "$path" ]]; then
            echo "[ERROR] ${path} ...NOT EXECUTABLE"
            err=1
            continue
        fi
        # dxrtd is a daemon binary — invoke -h can start a listener, so just
        # confirm the file is present and executable.
        if [[ "$bin" == "dxrtd" ]]; then
            echo "[OK] ${path} ...OK (presence check)"
            continue
        fi
        if "$path" -h >/dev/null 2>&1; then
            echo "[OK] ${path} ...OK"
        else
            echo "[ERROR] ${path} ...FAILED (-h returned non-zero)"
            err=1
        fi
    done
    return $err
}

function LibraryFileCheck() {
    echo "==== Library File Check (${LOCAL_LIB_DIR}) ===="
    local err=0 lib path target
    for lib in "${REQUIRED_LIBS[@]}"; do
        path="${LOCAL_LIB_DIR}/${lib}"
        if [[ ! -e "$path" ]]; then
            echo "[ERROR] ${path} ...MISSING"
            err=1
            continue
        fi
        if [[ -L "$path" ]]; then
            target="$(readlink -f "$path" || true)"
            if [[ -z "$target" || ! -f "$target" ]]; then
                echo "[ERROR] ${path} ...BROKEN SYMLINK"
                err=1
                continue
            fi
            echo "[OK] ${path} -> ${target}"
        else
            echo "[OK] ${path}"
        fi
    done

    # ldconfig cache lookup — confirms the loader can resolve the libs.
    local cached_lib
    for lib in "${REQUIRED_LIBS[@]}"; do
        cached_lib="$(ldconfig -p 2>/dev/null | awk -v L="$lib" '$1==L {print $NF; exit}')"
        if [[ -n "$cached_lib" ]]; then
            echo "[OK] ldconfig cache: ${lib} -> ${cached_lib}"
        else
            echo "[WARN] ldconfig cache: ${lib} not registered (run 'sudo ldconfig')"
        fi
    done

    return $err
}

function PythonWheelCheck() {
    echo "==== Python Wheel Check (${DEB_PY_DIR}) ===="
    if [[ ! -d "$DEB_PY_DIR" ]]; then
        echo "[ERROR] ${DEB_PY_DIR} ...MISSING"
        return 1
    fi

    local err=0 tag matches wh
    for tag in "${REQUIRED_WHEEL_TAGS[@]}"; do
        shopt -s nullglob
        matches=("${DEB_PY_DIR}"/dx_engine-*-"${tag}"-"${tag}"-*.whl)
        shopt -u nullglob

        if [[ ${#matches[@]} -eq 0 ]]; then
            echo "[ERROR] ${tag} wheel ...MISSING"
            err=1
            continue
        fi

        for wh in "${matches[@]}"; do
            echo "[OK] $(basename "$wh")"
        done
    done

    return $err
}

echo "============================================================================"
echo "==== Debian Sanity Check Date : $(date) ===="
echo ""

ExecutableFileCheck
EXEC_STATUS=$?

LibraryFileCheck
LIB_STATUS=$?

PythonWheelCheck
PY_STATUS=$?

echo
echo "============================================================================"
if [[ $EXEC_STATUS -ne 0 || $LIB_STATUS -ne 0 || $PY_STATUS -ne 0 ]]; then
    echo "** Sanity check FAILED! (exec=$EXEC_STATUS lib=$LIB_STATUS python=$PY_STATUS)"
    echo "============================================================================"
    exit 1
else
    echo "** Sanity check PASSED!"
    echo "============================================================================"
    exit 0
fi
