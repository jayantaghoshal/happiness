#!/usr/bin/env bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker run  -p 3001:3001 \
-it \
--rm \
--dns=10.244.0.15 \
--dns=10.244.0.20 \
--volume=/cm/services/log/:/cm/services/log/ \
--link=icup_android-mongo \
-e MONGODB_PASSWORD \
-e MONGODB_USER=jenkins-icup_android \
-e ICUP_ANDROID_MONGO_PORT_27017_TCP_ADDR \
-e NODE_ENV=dev \
-e DEBUG="*" \
test_report_web
