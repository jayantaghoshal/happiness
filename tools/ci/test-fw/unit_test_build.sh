#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)

# Build unit test
$SCRIPT_DIR/test_build.sh test/ut
