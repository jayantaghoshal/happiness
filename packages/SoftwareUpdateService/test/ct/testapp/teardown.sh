#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"

adb shell log "kill json-server"
kill -9 "$(pgrep 'node')"

# Save database used during test run for post test analysis
cp "${SCRIPT_DIR}/cloudsim/db.json" "${SCRIPT_DIR}/cloudsim/db.json.run"
cp "${SCRIPT_DIR}/cloudsim/db.json.bak" "${SCRIPT_DIR}/cloudsim/db.json"
rm "${SCRIPT_DIR}/cloudsim/db.json.bak"