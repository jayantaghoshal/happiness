#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Setup ccache
USE_CCACHE=true

docker_run "cd vendor/volvocars/hardware/signals/dataelements/AutosarCodeGen && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/testing/fdx_client && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/ci/shipit && python3 -m unittest"
docker_run "lunch ihu_vcc-eng && make -j16 droid"

# Push out files required for gate_test.sh to Artifactory.
OUT_ARCHIVE=out.tar.gz
tar cvzf ${OUT_ARCHIVE} \
    ./out/host/linux-x86/bin/{adb,fastboot} \
    ./out/target/product/ihu_vcc/fast_flashfiles \
    || die "Could not create out archive"

docker_run artifactory push ihu_gate_build "${ZUUL_CHANGE_IDS}" ${OUT_ARCHIVE} \
    || die "Could not push out archive to Artifactory."

rm ${OUT_ARCHIVE}
