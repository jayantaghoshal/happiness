#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)

# Build component test
"$SCRIPT_DIR"/test_build.sh test/ct
