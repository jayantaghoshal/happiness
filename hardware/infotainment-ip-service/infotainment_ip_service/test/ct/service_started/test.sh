#!/bin/bash


# Determine the absolute path to this script
SCRIPT_PATH=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
INTEGRATION_TOOLS_PATH=$SCRIPT_PATH/../../../../test_utils/ihu_tools/integration/tools

# First ensure that docker image is built
# Note: If it is already build, this step takes a few ms.
pushd "$INTEGRATION_TOOLS_PATH"/docker-behave > /dev/null
./build.sh
popd > /dev/null

# We will put all logs in the same directory for easy extraction
# of artifacts in jenkins.
mkdir -p "$SCRIPT_PATH"/logs

"$INTEGRATION_TOOLS_PATH"/docker-behave/run.sh behave --junit --junit-directory ./logs
