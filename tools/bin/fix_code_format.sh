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

# Blueprint (*.bp) files
echo "Canonical formatting for blueprint files in: $PROCESSED_DIR"
for f in ${PROCESSED_DIR}/**/*.bp
do
  "${REPO_ROOT_DIR}/prebuilts/build-tools/linux-x86/bin/bpfmt" -w "$f"
  echo "Formatted $f"
done
echo "Canonical formatting for blueprint files - Done."

# C++ sources (*.c/cpp/h/hpp)
echo "Canonical formatting for CPP source files in: $PROCESSED_DIR"

for f in ${PROCESSED_DIR}/**/*.c ${PROCESSED_DIR}/**/*.cpp ${PROCESSED_DIR}/**/*.h ${PROCESSED_DIR}/**/*.hpp
do
  "${REPO_ROOT_DIR}/prebuilts/clang/host/linux-x86/clang-3859424/bin/clang-format" -i "$f"
  echo "Formatted $f"
done
echo "Canonical formatting for CPP source files - Done."

# Python sources (*.py)
echo "Canonical formatting for Python files in: $PROCESSED_DIR"
for f in ${PROCESSED_DIR}/**/*.py
do
  "autopep8" -i "$f"
  echo "Formatted $f"
done
echo "Canonical formatting for Python files - Done."
