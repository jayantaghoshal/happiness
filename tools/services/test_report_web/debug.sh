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
-e NODE_ENV=dev \
-e DEBUG="*" \
test_report_web
