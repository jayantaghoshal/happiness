#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)



export USE_CCACHE=false

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng


# Build image, vts & tradefed
time make -j32 droid vts tradefed-all

# Build vendor/volovcar tests (Unit and Component Tests) # Not relevant to daily build
# TODO: fix it for building daily tests
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py build --plan=nightly || die "Build Unit and Component tests failed"

# Workaround broken config file that prevents vts from running
# TODO: Remove when config file is fixed
rm -f out/host/linux-x86/vts/android-vts/testcases/VtsClimateComponentTest.config

# Create archive out.tgz
OUT_ARCHIVE=out.tgz
time tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            --directory="$REPO_ROOT_DIR" \
            ./out/target/product/ihu_vcc/fast_flashfiles \
            ./out/target/product/ihu_vcc/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed || die "Could not create out archive"

du -sh "$OUT_ARCHIVE"

# Upload to Artifactory
time artifactory push ihu_daily_build_vcc_eng "${BUILD_NUMBER}" "${OUT_ARCHIVE}" || die "Could not push out archive to Artifactory."

# Cleanup
rm ${OUT_ARCHIVE}
