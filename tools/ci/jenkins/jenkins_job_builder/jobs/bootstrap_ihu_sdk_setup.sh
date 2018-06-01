#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

bootstrap_docker_run "repo sync --no-clone-bundle --current-branch --force-sync --detach -q -j8 vendor/volvocars/hmi/MediaLibrary"
bootstrap_docker_run "repo sync --no-clone-bundle --current-branch --force-sync --detach -q -j8 vendor/volvocars/hmi/ComponentLibrary"

./vendor/volvocars/tools/ci/jenkins/ci_docker_run.sh --multiuser ./vendor/volvocars/tools/ci/jenkins/sdk_setup.sh