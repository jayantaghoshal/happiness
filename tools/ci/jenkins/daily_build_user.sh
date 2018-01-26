#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)



export USE_CCACHE=false

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-user


# Build only image
time make droid

# TODO: Make vbf files for user target