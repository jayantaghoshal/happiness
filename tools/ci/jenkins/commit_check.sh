#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

python3 ./vendor/volvocars/tools/ci/shipit/bump.py . check "${ZUUL_BRANCH}"

# Disabled commit check because Zuul seems to confuse it by creating temporary
# merge-commits with "invalid" commit message.
#python3 ./vendor/volvocars/tools/ci/jenkins/verify_commit_message.py ./vendor/volvocars/

# Think 3 times and ask @krzysztof.wesolowski@volvocars.com if you want to add any check
# to the commit_check.sh (this script) instead of the commit_check_and_gate_common.sh (below).
# If sth slips through the gate and blocks commit check, we would be all very unhappy :)
"$SCRIPT_DIR"/commit_check_and_gate_common.sh