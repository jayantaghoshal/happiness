#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)

"${SCRIPT_DIR}/fix_cpp_format.sh" "$@" &
"${SCRIPT_DIR}/fix_bp_format.sh" "$@" &

wait
echo "All formatting done".