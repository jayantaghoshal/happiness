#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# TODO, build and do host tests
"$ZUUL_PROJECT"/gradlew artifactoryPublish -Partifactory_user=jenkins-icup_android -Partifactory_password="$ARTIFACTORY_API_KEY" -Partifactory_contextUrl=https://swf1.artifactory.cm.volvocars.biz/artifactory