#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# shellcheck disable=SC2029
ssh -o StrictHostKeyChecking=no -i /var/lib/keys/id_rsa "$@"