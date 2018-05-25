#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# TODO, build and do host tests
"$ZUUL_PROJECT"/gradlew artifactoryPublish
