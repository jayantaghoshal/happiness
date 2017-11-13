#!/bin/bash
# Note use of BASH_SOURCE[0] instead of $0 to allow for sourcing.
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"; pwd)

export PATH=$SCRIPT_DIR/bin:$PATH

export PYTHONPATH=$PYTHONPATH:${SCRIPT_DIR}/lib/python/ihu_vts
export PYTHONPATH=$PYTHONPATH:${SCRIPT_DIR}/lib/python
export PYTHONPATH=$PYTHONPATH:$ANDROID_BUILD_TOP/vendor
export PYTHONPATH=$PYTHONPATH:$ANDROID_BUILD_TOP/vendor/volvocars/tools/testing/fdx_client
