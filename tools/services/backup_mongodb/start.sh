#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -e

printenv | sed 's/^\(.*\)$/export \1/g' > /tmp/env.sh

echo 'Starting cron'
cron -f
