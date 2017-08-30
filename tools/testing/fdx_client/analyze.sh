export MYPYPATH=../../../hardware/signals/dataelements/AutosarCodeGen
mypy --py2 --show-column-numbers --check-untyped-defs --warn-no-return examples
mypy --py2 --show-column-numbers --check-untyped-defs --warn-no-return fdx
mypy       --show-column-numbers --check-untyped-defs --warn-no-return generate_signal_scaling.py
