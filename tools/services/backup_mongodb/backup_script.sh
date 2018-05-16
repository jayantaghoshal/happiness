#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex

source /tmp/env.sh

export backup_name
backup_name=$(date +%Y-%m-%d)

mongodump --host "$ICUP_ANDROID_MONGO_PORT_27017_TCP_ADDR" --port 27017 --username E9426001 --password "$MONGODB_PASSWORD" --out /workspace/backups/"$backup_name" --gzip

cd /workspace/backups

tar -c -f "$backup_name".tgz "$backup_name"

export JFROG_CLI_OFFER_CONFIG
JFROG_CLI_OFFER_CONFIG=false

/jfrog rt u /workspace/backups/"$backup_name".tgz ICUP_ANDROID_CI/mongodb_backups-test_results/ --url https://swf1.artifactory.cm.volvocars.biz/artifactory --apikey "$SWF_API_KEY"

rm -rf /workspace/backups/"$backup_name".tgz /workspace/backups/"$backup_name"
