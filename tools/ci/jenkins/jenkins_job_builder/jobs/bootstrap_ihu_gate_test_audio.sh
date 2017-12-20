#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker_killall
./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh ./vendor/volvocars/tools/ci/jenkins/gate_test_audio.sh
