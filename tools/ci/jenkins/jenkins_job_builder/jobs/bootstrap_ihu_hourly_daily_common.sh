#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -x
build_dir=vendor

if [[ -d $build_dir ]]; then
  rm -rf $build_dir;
fi

mkdir -p $build_dir
cd $build_dir

git clone ssh://jenkins-icup_android@gotsvl1415.got.volvocars.net:29421/vendor/volvocars

function docker_killall() {
  local containers
  containers=$(docker ps -q --format="{{.ID}} {{.Image}}" | grep vcc_aosp_build | cut -d " " -f 1 )
  if [ -n "$containers" ]; then
    #shellcheck disable=SC2086
    docker kill $containers
  fi
}

cd "$WORKSPACE"

# multiuser flag incase hourly and daily job uses docker at same time
"$build_dir"/volvocars/tools/ci/jenkins/ci_docker_run.sh --multiuser "$build_dir"/volvocars/tools/ci/jenkins/report_generation.sh

# deploy the result to the web server
if echo "$JOB_NAME" | grep -i "daily"
then
    cp -R "$BUILD_NUMBER" /cm/www/daily_test_report/
elif echo "$JOB_NAME" | grep -i "hourly"
then
    cp -R "$BUILD_NUMBER" /cm/www/hourly_test_report/
fi