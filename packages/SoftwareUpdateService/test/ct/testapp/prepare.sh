#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"

cp "${SCRIPT_DIR}/cloudsim/db.json" "${SCRIPT_DIR}/cloudsim/db.json.bak"
adb shell log "push localconfig"
adb push "${SCRIPT_DIR}/localconfig_cloud.json" "data/local/tmp/localconfig_cloud.json"
adb shell log "Starting json-server"
node "${SCRIPT_DIR}/cloudsim/server.js" > "${SCRIPT_DIR}/cloud_sim_logs.log" 2>&1  &
disown $!
