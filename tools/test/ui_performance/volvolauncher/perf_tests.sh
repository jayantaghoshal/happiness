#!/bin/sh
set -ue
python ./performance_eval.py > temp.txt
python ./generate_performance_report.py temp.txt "$1"
