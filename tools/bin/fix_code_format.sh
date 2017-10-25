#!/bin/bash
set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)

"${SCRIPT_DIR}/fix_cpp_format.sh" "$@" &
"${SCRIPT_DIR}/fix_bp_format.sh" "$@" &

wait
echo "All formatting done".