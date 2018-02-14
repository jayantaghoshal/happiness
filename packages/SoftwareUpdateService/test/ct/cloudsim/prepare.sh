#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"

cp "${SCRIPT_DIR}/db.json" "${SCRIPT_DIR}/db.json.bak"
cp "${SCRIPT_DIR}/db.json.run" "${SCRIPT_DIR}/db.json"
adb shell log "push localconfig"
adb push "${SCRIPT_DIR}/localconfig_cloud.json" "data/local/tmp/localconfig_cloud.json"
adb shell log "Starting json-server"
node "${SCRIPT_DIR}/server.js" > "${SCRIPT_DIR}/cloudsim.log" 2>&1  &
disown $!
