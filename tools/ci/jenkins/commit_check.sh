#!/bin/bash
set -e
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"


# Disabled commit check because Zuul seems to confuse it by creating temporary
# merge-commits with "invalid" commit message.
#python3 ./vendor/volvocars/tools/ci/jenkins/verify_commit_message.py ./vendor/volvocars/
