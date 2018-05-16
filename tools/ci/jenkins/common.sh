#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)
export PIP_CONFIG_FILE="/usr/local/pip/pip.conf"
export VTS_PYPI_PATH="${PIP_CONFIG_FILE}" # Will be used in Android P with the same purpose as the above

if [ -z "${WORKSPACE}" ]; then
  WORKSPACE=$(readlink -f "${REPO_ROOT_DIR}"/..)
fi


# Color codes which can be used when printing stuff to terminal
C_ERROR="\033[0;31m"
#C_OK="\033[0;32m"
C_OFF="\033[0m"


function die() {
    message=$1
    echo -e "${C_ERROR}${message}${C_OFF}"
    exit 1
}

function repo_sync() {
  repo sync --no-clone-bundle --current-branch --force-sync --detach -q -j8 "$@" || die "repo sync failed"
}

function storecimetrix() {
    buildFunc=$1
    buildTime=$2
    timeStamp=$(date +%s%N)

    if [[ "$buildFunc" == *"droid"* ]]; then
       buildFunc="makedroid"
    elif [[ "$buildFunc" == *"tradefed-all"* ]]; then
       buildFunc="maketradefed"
    elif [[ "$buildFunc" == *"tester.py"* ]]; then
       buildFunc="makecomponent"
    elif [[ "$buildFunc" == *"bump.py"* ]]; then
       buildFunc="reposync"
    elif [[ "$buildFunc" == *"mmma"* ]]; then
       buildFunc="mmma"
    elif [[ "$buildFunc" == *"artifactory"* ]]; then
       buildFunc="artifactorypush"
    elif [[ "$buildFunc" == *"scp"* ]]; then
       buildFunc="gateflowscp"
    fi

    set +e
    curl -i -XPOST 'http://gotsvl1416.got.volvocars.net:8086/write?db=ciflowtime' --data-binary "$buildFunc,build_stage=${JOB_NAME} build_number=${BUILD_NUMBER},build_time=$buildTime $timeStamp"
    set -e
}

function citime(){
  timer_start=$(date '+%s')
  "$@"
  timer_stop=$(date '+%s')
  elapsedTime=$((timer_stop - timer_start))
  echo $((elapsedTime / 3600))"h" $(((elapsedTime / 60) % 60))"m" $((elapsedTime % 60))"s"

  storecimetrix "$*" "$elapsedTime"
}


