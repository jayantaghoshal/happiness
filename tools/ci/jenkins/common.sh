#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)
export PIP_CONFIG_FILE="/usr/local/pip/pip.conf"

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

function ihu_update() {
  python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/ihu_update.py \
    --aosp_root_dir "$REPO_ROOT_DIR" \
    --vip_port /dev/ttyVIP \
    --mp_port /dev/ttyMP
}

function repo_sync() {
  repo sync --no-clone-bundle --current-branch -q -j8 "$@" || die "repo sync failed"
}

function clean_old_test_result_files() {
  set +e # unset exiting on error returns

  #clean results of old tests
  rm -rf /tmp/0/stub/*
  rm -rf "$ANDROID_HOST_OUT"/vts/android-vts/logs/*
  rm -rf "$ANDROID_HOST_OUT"/vts/android-vts/results/*

  #mkdir if not present
  mkdir  -p "$ANDROID_HOST_OUT"/vcc_test_results/
  mkdir  -p "$ANDROID_HOST_OUT"/vcc_test_results/tradefed
  mkdir  -p "$ANDROID_HOST_OUT"/vcc_test_results/vts

  #clean old collected results
  rm -rf "$ANDROID_HOST_OUT"/vcc_test_results/tradefed/*
  rm -rf "$ANDROID_HOST_OUT"/vcc_test_results/vts/*

  set -e # set exiting on error returns

}

function collect_test_result_files() {
  set +e # unset exiting on error returns
  #collect the results
  cp -R /tmp/0/stub/* "$ANDROID_HOST_OUT"/vcc_test_results/tradefed
  ls "$ANDROID_HOST_OUT"/vts/android-vts/logs/
  cp -R "$ANDROID_HOST_OUT"/vts/android-vts/logs/* "$ANDROID_HOST_OUT"/vcc_test_results/vts/
  cp -R "$ANDROID_HOST_OUT"/vts/android-vts/results/* "$ANDROID_HOST_OUT"/vcc_test_results/vts/

  #unzip archieves and rm zips (tradefed)
  find "$ANDROID_HOST_OUT"/vcc_test_results/tradefed -name '*.zip' -exec sh -c 'unzip -d `dirname {}` {}' ';'
  find "$ANDROID_HOST_OUT"/vcc_test_results/tradefed -name '*.zip' -exec sh -c 'rm  {}' ';'

  #unzip archieves and rm zips (vts)
  find "$ANDROID_HOST_OUT"/vcc_test_results/vts -name '*.gz' -exec sh -c 'gunzip -d {}' ';'
  find "$ANDROID_HOST_OUT"/vcc_test_results/vts -maxdepth 2 -type f \! -name 'test_result.xml' -delete
  find "$ANDROID_HOST_OUT"/vcc_test_results/vts -type d -name 'config' -exec sh -c 'rm -rf {}' ';'
  find "$ANDROID_HOST_OUT"/vcc_test_results/vts -type f -name 'test_result.xml' -exec sh -c 'mv {} `dirname {}`/inv_*/' ';'
  find "$ANDROID_HOST_OUT"/vcc_test_results/vts -type d -name 'inv_*' -exec sh -c 'mv {} `dirname {}`/../' ';'
  find "$ANDROID_HOST_OUT"/vcc_test_results/vts -maxdepth 1 -type d \! -name 'inv_*' -delete

  tar -czvf "$ANDROID_HOST_OUT"/vcc_test_results/vcc_test_results_"$JOB_NAME"_"$NODE_NAME".tar.gz -C "$ANDROID_HOST_OUT"/vcc_test_results/ vts tradefed
  set -e # set exiting on error returns

}