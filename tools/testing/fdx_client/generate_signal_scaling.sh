#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -u

SCRIPT_DIR="$(cd "$(dirname "$(readlink -f "$0")")"; pwd)"


set -ue
python3 generate_signal_scaling.py \
    --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml \
    --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml \
    --fdxdescriptionfile=../CANoe/SPA2610/FDXDescriptionFile.xml \
    --out=generated/pyDataElements.py

#TODO: Fix enviornment by rearraning files and add to envsetup.sh
export PYTHONPATH="$SCRIPT_DIR"/../../../hardware/signals/dataelements/tools/simulation:$PYTHONPATH
export MYPYPATH=$PYTHONPATH
mypy       --show-column-numbers --check-untyped-defs --warn-no-return ./generated
mypy --py2 --show-column-numbers --check-untyped-defs --warn-no-return ./generated
