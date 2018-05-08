#!/usr/bin/env bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker run -p 3001:3001 \
-d \
--restart unless-stopped \
--dns=10.244.0.15 \
--dns=10.244.0.20 \
--volume=/cm/services/log/:/app/logs/ \
--link=icup_android-mongo \
-e MONGODB_PASSWORD \
-e MONGODB_USER=jenkins-icup_android \
-e NODE_ENV=prod \
test_report_web
