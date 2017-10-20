#!/bin/bash
set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../..)

echo "REPO_ROOT_DIR = $REPO_ROOT_DIR"

if [ -z "$1" ]
  then
    PROCESSED_DIR="$PWD"
  else
    PROCESSED_DIR="$1"
fi

# Configure glob pattern behavior
shopt -s globstar
shopt -s nullglob

# Python sources (*.py)
echo "Canonical formatting for Python files in: $PROCESSED_DIR"
for f in ${PROCESSED_DIR}/**/*.py
do
  "autopep8" -i "$f"
  echo "Formatted $f"
done
echo "Canonical formatting for Python files - Done."
