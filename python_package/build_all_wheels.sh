#!/usr/bin/env bash
# Build dx-engine wheels for multiple Python versions in isolated venvs.
# See BUILD_WHEELS.md for details.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

DEFAULT_VERSIONS=(3.8 3.9 3.10 3.11 3.12 3.13 3.14)
OUTPUT_DIR="$SCRIPT_DIR/wheelhouse"
VENV_ROOT="$SCRIPT_DIR/.venvs"
CLEAN=0
VERSIONS=()

usage() {
    cat <<EOF
Usage: $(basename "$0") [options] [PY_VER ...]

Build dx-engine wheels for the given Python versions (default: ${DEFAULT_VERSIONS[*]}).

Options:
  -o, --output DIR     output directory for wheels (default: ./wheelhouse)
      --clean          remove existing venvs and build cache before building
  -h, --help           show this help

Examples:
  $(basename "$0")
  $(basename "$0") 3.11 3.12
  $(basename "$0") -o /tmp/wh 3.13
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help) usage; exit 0 ;;
        -o|--output) OUTPUT_DIR="$2"; shift 2 ;;
        --clean) CLEAN=1; shift ;;
        3.*|cp3*) VERSIONS+=("$1"); shift ;;
        *) echo "Unknown argument: $1" >&2; usage; exit 1 ;;
    esac
done

if [[ ${#VERSIONS[@]} -eq 0 ]]; then
    VERSIONS=("${DEFAULT_VERSIONS[@]}")
fi

# --- Ensure uv is available -------------------------------------------------
if ! command -v uv >/dev/null 2>&1; then
    if [[ -x "$HOME/.local/bin/uv" ]]; then
        export PATH="$HOME/.local/bin:$PATH"
    else
        echo "uv not found. Install with: pip3 install --user uv" >&2
        exit 1
    fi
fi
echo "uv: $(uv --version)"

# Use system CA bundle for corporate-proxy environments.
export UV_SYSTEM_CERTS=true
export SSL_CERT_FILE="${SSL_CERT_FILE:-/etc/ssl/certs/ca-certificates.crt}"

# --- Optional clean ---------------------------------------------------------
if [[ $CLEAN -eq 1 ]]; then
    echo "Cleaning $VENV_ROOT and build caches..."
    rm -rf "$VENV_ROOT" "$SCRIPT_DIR/build" "$SCRIPT_DIR/_skbuild"
fi

mkdir -p "$OUTPUT_DIR" "$VENV_ROOT"

# --- Refresh LICENSE next to pyproject.toml ---------------------------------
# python_package/LICENSE is a committed copy of the repo-root LICENSE.
# Re-copy on every build so drift from ../LICENSE is impossible; PEP 639
# requires the file to live under the project root, and scikit-build-core
# then embeds it in each wheel's .dist-info/licenses/ directory via
# `license-files = ["LICENSE"]`.
REPO_LICENSE="$SCRIPT_DIR/../LICENSE"
LOCAL_LICENSE="$SCRIPT_DIR/LICENSE"
if [[ -f "$REPO_LICENSE" ]]; then
    cp -f "$REPO_LICENSE" "$LOCAL_LICENSE"
elif [[ ! -f "$LOCAL_LICENSE" ]]; then
    echo "WARNING: ../LICENSE not found; wheels will lack LICENSE metadata." >&2
fi

# --- libdxrt sanity check ---------------------------------------------------
# ldconfig lives in /usr/sbin which is not always on PATH for non-root users.
# Avoid `grep -q` here: under `set -o pipefail`, ldconfig gets SIGPIPE (141)
# when grep short-circuits, producing a false negative.
LDCONFIG="$(command -v ldconfig || echo /usr/sbin/ldconfig)"
DXRT_HIT="$("$LDCONFIG" -p 2>/dev/null | grep "libdxrt\.so" || true)"
if [[ -z "$DXRT_HIT" ]]; then
    echo "WARNING: libdxrt.so not found in ldconfig cache." >&2
    echo "         Install libdxrt-bin and run 'sudo ldconfig' before building." >&2
fi

# --- Build per version ------------------------------------------------------
SUCCESS=()
FAILURES=()

for ver in "${VERSIONS[@]}"; do
    tag="py${ver//./}"
    venv="$VENV_ROOT/$tag"

    echo
    echo "============================================================"
    echo "[Python $ver] venv: $venv"
    echo "============================================================"

    if [[ ! -x "$venv/bin/python" ]]; then
        # Make uv download the requested CPython if absent.
        uv python install "$ver" >/dev/null
        # --seed installs pip into the venv so we can use standard pip workflows.
        uv venv --seed --python "$ver" "$venv"
    fi

    py="$venv/bin/python"
    actual="$("$py" -c 'import sys;print("%d.%d"%sys.version_info[:2])')"
    echo "Interpreter: $py ($actual)"

    "$py" -m pip install --upgrade --quiet \
        pip wheel scikit-build-core 'pybind11>=2.11,<3'

    # Per-version build dir avoids stale cmake cache across versions.
    build_tmp="$SCRIPT_DIR/build/$tag"
    rm -rf "$build_tmp"
    mkdir -p "$build_tmp"

    set +e
    "$py" -m pip wheel . \
        --no-deps \
        --wheel-dir "$OUTPUT_DIR" \
        --config-settings=build-dir="$build_tmp"
    rc=$?
    set -e

    if [[ $rc -eq 0 ]]; then
        SUCCESS+=("$ver")
    else
        FAILURES+=("$ver")
        echo "[FAIL] Python $ver wheel build failed (exit $rc)" >&2
    fi
done

# --- Summary ----------------------------------------------------------------
echo
echo "============================================================"
echo "Build summary"
echo "  Output dir : $OUTPUT_DIR"
echo "  Success    : ${SUCCESS[*]:-(none)}"
echo "  Failed     : ${FAILURES[*]:-(none)}"
echo "============================================================"
ls -1 "$OUTPUT_DIR"/*.whl 2>/dev/null || echo "(no wheels)"

[[ ${#FAILURES[@]} -eq 0 ]]
