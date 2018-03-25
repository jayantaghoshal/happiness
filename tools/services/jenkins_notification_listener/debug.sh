#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker run \
    --rm \
    -it \
    --dns=10.244.0.15 \
    --dns=10.244.0.20 \
    -p 3002:3002 \
    -v "${PWD}":"${PWD}" \
    -e "DEBUG=1" \
    -e "LOG_DIR=${PWD}/log" \
    -e JENKINS_API_KEY \
    -e USER \
    --entrypoint="" \
    jenkins-notification-listener flask run --host=0.0.0.0 --port=3002
