#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# TODO,
# pull latest hourly,
# get apk from Artifactory,
# install apk,
# target test,
# push apk to binary r epo if PASS

artifactory pull-latest ihu_hourly_build-eng out.tgz || die "Could not fetch archive from Artifactory."

tar xf "${OUT_ARCHIVE}" || die "Could not extract out archive."

ihu_update --profile --profile=ci-machinery || die "failed to flash "