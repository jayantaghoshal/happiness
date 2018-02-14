#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"

cp "${SCRIPT_DIR}/db.json" "${SCRIPT_DIR}/../../cloudsim/db.json.run"

"${SCRIPT_DIR}/../../cloudsim/prepare.sh"