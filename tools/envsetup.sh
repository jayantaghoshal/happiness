#!/bin/bash
# Note use of BASH_SOURCE[0] instead of $0 to allow for sourcing.
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"; pwd)

export PATH=$SCRIPT_DIR/bin:$PATH

# TODO: export PYTHONPATH=$SCRIPT_PATH/python/site-packages:$PYTHONPATH
#       (or wherever we decide to move Python modules later on)
