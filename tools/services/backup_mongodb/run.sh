#!/usr/bin/env bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

docker run \
-d \
--restart unless-stopped \
--dns=10.244.0.15 \
--dns=10.244.0.20 \
--link=icup_android-mongo \
-e MONGODB_PASSWORD \
-e SWF_API_KEY \
-v /cm/services/backup_mongodb:/workspace \
backup_mongodb
