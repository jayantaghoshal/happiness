#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex

# TODO: Run in sandbox and verify the output folder location and archive/result storage
"$ZUUL_PROJECT"/gradlew clean assemble check -p "$ZUUL_PROJECT"