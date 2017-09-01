#!/bin/bash
set -ue
mypy --show-column-numbers --check-untyped-defs --warn-no-return --ignore-missing-imports .
python3 -m flake8 --statistics --show-source --config=tox.ini .