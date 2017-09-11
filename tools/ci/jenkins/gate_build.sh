#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Update the manifests based on the templates and download all other
# repositories. First time this will take a very long time but subsequent
# downloads are incremental and faster.
docker_run python3 ./vendor/volvocars/tools/ci/shipit/bump.py . local "${ZUUL_BRANCH}"

# Setup ccache
export USE_CCACHE=true

#TODO: Move this to commit check when the commit gate is ready
#TODO: Investigate Hidl tool generator for multilib flag before
#uncommenting the following line
# docker_run "64bit_sanity.py $REPO_ROOT_DIR/vendor/volvocars/" || die "64 bit build sanity check failed"

docker_run "cd vendor/volvocars/tools/ci/jenkins && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/ci/shipit && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/ci/shipit && python3 -m unittest"
docker_run "cd vendor/volvocars/hardware/signals/dataelements/AutosarCodeGen && ./analyze.sh"
docker_run "cd vendor/volvocars/tools/testing/fdx_client && ./analyze.sh"

docker_run "lunch ihu_vcc-eng && make -j16 droid" || die "Build image failed"

docker_run "lunch ihu_vcc-eng && make -j16 vts" || die "Build VTS failed"

docker_run "lunch ihu_vcc-eng && make -j16 tradefed-all" || die "Build Tradefed failed"

# Build vendor/volovcar tests (Unit and Component Tests)
# build_tests

# Push out files required for gate_test.sh to Artifactory.
OUT_ARCHIVE=out.tgz
docker_run "tar cvfz ${OUT_ARCHIVE} \
            ./out/target/product/ihu_vcc/fast_flashfiles \
            ./out/target/product/ihu_vcc/data \
            ./out/host/linux-x86/bin/fastboot \
            ./out/host/linux-x86/bin/adb \
            ./out/host/linux-x86/bin/aapt \
            ./out/host/linux-x86/bin/tradefed.sh \
            ./out/host/linux-x86/bin/vts-tradefed \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed" || die "Could not create out archive"

docker_run artifactory push ihu_gate_build "${ZUUL_CHANGE_IDS}" ${OUT_ARCHIVE} \
    || die "Could not push out archive to Artifactory."

rm ${OUT_ARCHIVE}
