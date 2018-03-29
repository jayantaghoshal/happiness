#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker run \
    --rm \
    -it \
    --dns=10.244.0.15 \
    --dns=10.244.0.20 \
    -v "${PWD}":"${PWD}" \
    -v "${HOME}":"${HOME}" \
    -e "DEBUG=1" \
    -e "LOG_DIR=${PWD}/log" \
    -e JENKINS_API_KEY \
    -e GERRIT_USER="${GERRIT_USER}" \
    -e USER \
    --entrypoint="" \
    gerrit-event-stream-listener /bin/bash -c /app/gerrit_event_stream_listener.py
