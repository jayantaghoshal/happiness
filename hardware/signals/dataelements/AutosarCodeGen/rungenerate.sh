#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

python3 -tt generate.py \
    -s databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml \
    -c databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Com.arxml \
    -d generated

