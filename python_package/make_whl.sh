#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# The wheel embeds LICENSE and NOTICE.md from this directory.
# They must be identical to the copies in the repository root.
for f in LICENSE NOTICE.md; do
    root_file="${SCRIPT_DIR}/../${f}"
    local_file="${SCRIPT_DIR}/${f}"

    if [ ! -f "${root_file}" ] && [ ! -f "${local_file}" ]; then
        echo "[make_whl][ERROR] ${f} not found in the repository root or python_package/." >&2
        echo "                  The wheel must not be built without it." >&2
        exit 1
    fi

    if [ ! -f "${local_file}" ]; then
        # Missing local copy: take it from the repository root.
        cp "${root_file}" "${local_file}"
        echo "[make_whl][INFO] Copied ${f} from the repository root to python_package/."
        continue
    fi

    if [ ! -f "${root_file}" ]; then
        # Standalone python_package/ tree (no repository root): use the local copy.
        continue
    fi

    if ! cmp -s "${root_file}" "${local_file}"; then
        echo "[make_whl][ERROR] python_package/${f} differs from the repository root ${f}." >&2
        echo "                  Run: cp ${f} python_package/${f}" >&2
        exit 1
    fi
done

python3 -m pip wheel .
