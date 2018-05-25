#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# TODO, build and do host tests
set -uex
pwd
ls -l
echo "$ZUUL_PROJECT"

"$ZUUL_PROJECT"/gradlew tasks
"$ZUUL_PROJECT"/gradlew tasks -Partifactory_user=kkarls28 -Partifactory_password=AKCp5aTGrS6sFjkDahJ4xHTXSPg13UKKkXW1zam8b3WMitn3jFsUwYewCLTnikJk8v8gecuSZ -Partifactory_contextUrl=https://swf1.artifactory.cm.volvocars.biz/artifactory

#"$ZUUL_PROJECT"/gradlew artifactoryPublish -Partifactory_user=jenkins-icup_android -Partifactory_password="$ARTIFACTORY_API_KEY" -Partifactory_contextUrl=https://swf1.artifactory.cm.volvocars.biz/artifactory
"$ZUUL_PROJECT"/gradlew artifactoryPublish -Partifactory_user=kkarls28 -Partifactory_password=AKCp5aTGrS6sFjkDahJ4xHTXSPg13UKKkXW1zam8b3WMitn3jFsUwYewCLTnikJk8v8gecuSZ -Partifactory_contextUrl=https://swf1.artifactory.cm.volvocars.biz/artifactory
