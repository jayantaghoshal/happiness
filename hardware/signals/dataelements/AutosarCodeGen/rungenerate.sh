#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ue
./analyze.sh

python3 -tt generate.py \
    -s databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml \
    -c databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml \
    -d ../generated

