#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"

# Update the manifests based on the templates and download all other
# repositories. First time this will take a very long time but subsequent
# downloads are incremental and faster.
docker_run "time python3 ./vendor/volvocars/tools/ci/shipit/bump.py . local \"${ZUUL_BRANCH}\""

# Setup ccache and put cache and Android out folder in a tmpfs.
# We need to set CC_WRAPPER and CXX_WRAPPER to explicitly use ccache version (3.2.4) in Docker
# image. The version currently shipped by Google in AOSP (3.1.9) is too old and causes build
# failures when building in parallel.
TMPFS=/dev/shm
JOB_TMPFS=$TMPFS/$JOB_NAME
export CCACHE_DIR=$TMPFS/ccache
export CCACHE_MAXSIZE=50G
export CC_WRAPPER=/usr/bin/ccache
export CXX_WRAPPER=/usr/bin/ccache
export OUT_DIR=$JOB_TMPFS/out
export USE_CCACHE=true

rm -rf "${OUT_DIR}"  # Remove previous OUT_DIR for clean build.

# Rerun commit check in case it changed before the change was validated first time
time "$SCRIPT_DIR"/commit_check_and_gate_common.sh

docker_run "lunch ihu_vcc-eng && time make -j32 droid vts tradefed-all" || die "Build failed"

# Build vendor/volovcar tests (Unit and Component Tests)
docker_run "time python3 $REPO_ROOT_DIR/vendor/volvocars/tools/ci/shipit/tester.py build --plan=gate" || die "Build Unit and Component tests failed"


# Push out files required for gate_test.sh to Artifactory.
#
# Some measurements of different compression algorithms
# Compression         Size     Time          Size Reduction rate
# None                2.3GB -  0m 10sec           N/A
# gzip -1             950MB -  0m 45sec         30 Mb/s
# gzip -2             933MB -  0m 50sec         27 Mb/s
# gzip -6 (Default)   877MB -  1m 45sec         14 Mb/s
# gzip -9             873MB -  4m 46sec          5 Mb/s
# pigz -1             943MB -  0m 12sec        113 Mb/s
# pigz -6             877MB -  0m 18sec         79 Mb/s
# pigz -9             873MB -  0m 47sec         30 Mb/s

OUT_ARCHIVE=out.tgz
docker_run "time tar -c --use-compress-program='pigz -1' -f ${OUT_ARCHIVE} \
            --directory=\"${JOB_TMPFS}\"
            ./out/target/product/ihu_vcc/fast_flashfiles \
            ./out/target/product/ihu_vcc/data \
            ./out/host/linux-x86/bin/fastboot \
            ./out/host/linux-x86/bin/adb \
            ./out/host/linux-x86/bin/aapt \
            ./out/host/linux-x86/bin/tradefed.sh \
            ./out/host/linux-x86/bin/vts-tradefed \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed" || die "Could not create out archive"

ls -lh "$OUT_ARCHIVE"
docker_run "time artifactory push ihu_gate_build \"${ZUUL_CHANGE_IDS}\" ${OUT_ARCHIVE}" \
    || die "Could not push out archive to Artifactory."

rm ${OUT_ARCHIVE}
