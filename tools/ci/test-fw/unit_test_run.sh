#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)

# Run component test
# TODO: We should move the test_run.py to this directory.
$SCRIPT_DIR/test_run.sh test/ut
