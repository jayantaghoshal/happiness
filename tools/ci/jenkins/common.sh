#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
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

function buildNewVtsPackage() {
    #Build latest vts
    cd "$REPO_ROOT_DIR"; make vts

    OUT_ARCHIVE=outVTS.tgz
    tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            ./out/target/product/ihu_abl_car/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed || die "Could not create out archive"

    #Push latest vts build package to artifactory
    artifactory push vts_build_package "$VTS_HASH" "${OUT_ARCHIVE}" || die "Could not push archive to Artifactory."
}

function checkIfVtsPackageUpToDate() {
    VTS_HASH=$1
        if [ "$(artifactory show vts_build_package "$VTS_HASH" outVTS.tgz)" ]; then
            echo "exists"
        else
            buildNewVtsPackage
            echo "rebuilt"
        fi
}

function die() {
    message=$1
    echo -e "${C_ERROR}${message}${C_OFF}"
    exit 1
}

function ihu_update() {
  python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/ihu_update.py \
    --vip_port /dev/ttyVIP \
    --mp_port /dev/ttyMP
}

function repo_sync() {
  repo sync --no-clone-bundle --current-branch --force-sync --detach -q -j8 "$@" || die "repo sync failed"
}
