#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ue
echo "Analyzing DataElements generator"
mypy --show-column-numbers --check-untyped-defs --warn-no-return dataelements_generator
python3 -m flake8 --statistics --count --show-source .
(cd autosar && python3 -m flake8 --statistics --count --show-source .)
echo "Analyzing DataElements generator DONE"
