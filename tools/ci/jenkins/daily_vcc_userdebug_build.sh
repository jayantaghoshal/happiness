#!/bin/bash
set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)



export USE_CCACHE=false

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-userdebug


# Build only image 
time make -j32 droid 

# Create archive out.tgz 
OUT_ARCHIVE=out.tgz
time tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            --directory="$REPO_ROOT_DIR/out" \
            ./out/target/product/ihu_vcc/fast_flashfiles \
            ./out/target/product/ihu_vcc/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed || die "Could not create out archive"

du -sh "$OUT_ARCHIVE"

# Upload to Artifactory
time artifactory push ihu_daily_vcc_userdebug_build "${BUILD_NUMBER}" "${OUT_ARCHIVE}" || die "Could not push out archive to Artifactory."

# Cleanup
rm ${OUT_ARCHIVE}
