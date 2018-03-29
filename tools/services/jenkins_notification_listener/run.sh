#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker run \
    -d \
    --restart unless-stopped \
    --dns=10.244.0.15 \
    --dns=10.244.0.20 \
    -p 3002:3002 \
    -v /cm/services:/cm/services \
    -e "DEBUG=0" \
    -e "LOG_DIR=${PWD}/log" \
    -e JENKINS_API_KEY \
    -e USER \
    jenkins-notification-listener
