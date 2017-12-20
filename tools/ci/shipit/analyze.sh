#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ue
mypy --show-column-numbers --check-untyped-defs --warn-no-return --ignore-missing-imports .
python3 -m flake8 --statistics --show-source --config=tox.ini .