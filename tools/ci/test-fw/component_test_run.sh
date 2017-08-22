#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)

# Set python path, needed for VTS to find vendor specific tests
export PYTHONPATH=$PYTHONPATH:$ANDROID_BUILD_TOP/vendor

# Run component test
# TODO: We should move the test_run.py to this directory.
$SCRIPT_DIR/test_run.sh test/ct
