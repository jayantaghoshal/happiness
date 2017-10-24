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