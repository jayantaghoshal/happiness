#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../)

export CLANG_FORMAT_BINARY="${REPO_ROOT_DIR}/prebuilts/clang/host/linux-x86/clang-3859424/bin/clang-format"
export BP_FORMAT_BINARY="${REPO_ROOT_DIR}/prebuilts/build-tools/linux-x86/bin/bpfmt"

commit-check.py "$@"