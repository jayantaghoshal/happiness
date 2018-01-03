#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ue
python3 generate_signal_scaling.py \
    --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml \
    --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Com.arxml \
    --fdxdescriptionfile=../CANoe/SPA2210/FR_Body_LIN/FDXDescriptionFile.xml \
    --out=generated/pyDataElements.py

mypy --py2 --show-column-numbers --check-untyped-defs --warn-no-return generated/pyDataElements.py