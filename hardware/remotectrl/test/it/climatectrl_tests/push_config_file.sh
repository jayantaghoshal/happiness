#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"

adb push "${SCRIPT_DIR}/vsomeip_client.json" "data/local/tmp/vsomeip_client.json"
