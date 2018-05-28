#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# TODO, build and do host tests
set -uex
export ANDROID_HOME="$PWD/android-sdk-linux"

"$ZUUL_PROJECT"/gradlew artifactoryPublish -p "$ZUUL_PROJECT" -Partifactory_user=e9426001 -Partifactory_password="$ARTIFACTORY_API_KEY" -Partifactory_contextUrl=https://swf1.artifactory.cm.volvocars.biz/artifactory