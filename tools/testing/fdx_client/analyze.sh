#!/bin/bash
export MYPYPATH=../../../hardware/signals/dataelements/AutosarCodeGen
mypy --py2 --show-column-numbers --check-untyped-defs --warn-no-return examples
mypy --py2 --show-column-numbers --check-untyped-defs --warn-no-return fdx
mypy       --show-column-numbers --check-untyped-defs --warn-no-return generate_signal_scaling.py
python  -m flake8 --statistics --count --show-source examples
python  -m flake8 --statistics --count --show-source fdx
python3 -m flake8 --statistics --count --show-source generate_signal_scaling.py
