#!/bin/sh

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ue
python ./performance_eval.py > temp.txt
python ./generate_performance_report.py temp.txt "$1"
