#!/bin/bash
python3 generate_signal_scaling.py \
    --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml \
    --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Com.arxml \
    --fdxdescriptionfile=FDXDescriptionFile.xml \
    --out=generated/pyDataElements.py