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

docker_killall
"$build_dir"/volvocars/tools/ci/jenkins/ci_docker_run.sh "$build_dir"/volvocars/tools/ci/jenkins/report_generation.sh