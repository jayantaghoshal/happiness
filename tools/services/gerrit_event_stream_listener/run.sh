#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker run \
    -d \
    --dns=10.244.0.15 \
    --dns=10.244.0.20 \
    --restart unless-stopped \
    -v /cm/services:/cm/services \
    -v "${HOME}":"${HOME}" \
    -e "DEBUG=0" \
    -e "LOG_DIR=${PWD}/log" \
    -e JENKINS_API_KEY \
    -e GERRIT_USER \
    -e USER \
    gerrit-event-stream-listener
