#!/bin/bash
# this file or some version of it should be moved to the common place
# SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd) is currently unused
# Set python path, needed for the python test case code to find vendor specific tests
export PYTHONPATH=$PYTHONPATH:$ANDROID_BUILD_TOP/vendor:$ANDROID_BUILD_TOP/vendor/volvocars/tools/testing/fdx_client

# Run component test as a standard python program
test/ct/ComponentTest.py
