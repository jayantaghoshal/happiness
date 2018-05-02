#!/bin/bash

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
source "${SCRIPT_DIR}/common.sh"
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../..)

# Setup ccache and put cache and Android out folder in a tmpfs.
# We need to set CC_WRAPPER and CXX_WRAPPER to explicitly use ccache version (3.2.4) in Docker
# image. The version currently shipped by Google in AOSP (3.1.9) is too old and causes build
# failures when building in parallel.
TMPFS=/mnt/ramdisk
export CCACHE_DIR=$TMPFS/ccache
export CCACHE_MAXSIZE=50G
export CC_WRAPPER=/usr/bin/ccache
export CXX_WRAPPER=/usr/bin/ccache
export USE_CCACHE=true

# Rerun commit check in case merge effect changed after the change was validated at the check step
python3 ./vendor/volvocars/tools/ci/shipit/bump.py . check "${ZUUL_BRANCH}"

# Update the manifests based on the templates and download all other
# repositories. First time this will take a very long time but subsequent
# downloads are incremental and faster.
citime python3 ./vendor/volvocars/tools/ci/shipit/bump.py . local sync "${ZUUL_PROJECT}"

# Unable to remove out folder when the server is in process of
# copying files at the same time.
rm -rf out || sleep 30 && rm -rf out  # Remove previous OUT_DIR for clean build.

source "$REPO_ROOT_DIR"/build/envsetup.sh
lunch ihu_vcc-eng

# Rerun commit check in case it changed after the change was validated at the check step
time "$SCRIPT_DIR"/commit_check_and_gate_common.sh
time python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/bin/gate_check.py

VTS_REPO_HASH=$(git -C "${SCRIPT_DIR}"/../../../../../test/vts/ rev-parse HEAD)
# Check vts package is up to date
time checkIfVtsPackageUpToDate "$VTS_REPO_HASH"

# Build image & tradefed
citime make -j64 droid

# Create a generic subdirectory for build meta data files
mkdir -p -m 755 out/vcc_build_metadata
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_droid || true
cp out/.build.trace.gz out/vcc_build_metadata/make_droid.trace.gz || true

citime make -j64 tradefed-all
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_tradefed_all || true
cp out/.build.trace.gz out/vcc_build_metadata/make_tradefedall.trace.gz || true

# Download VTS package from artifactory
# Note: Have tried tar xvkf --skip-old-files, tar xvkf --keep-old-files, they are not able to merge all files to make droid out/
if [ "$(checkIfVtsPackageUpToDate "$VTS_REPO_HASH")" == "rebuilt" ];then
    echo "VTS package has already been built"
else
    artifactory pull vts_build_package "$VTS_REPO_HASH" outVTS.tgz || die "Could not fetch out archive from Artifactory."
    mkdir -p -m 755 vtsPackage
    tar xvf outVTS.tgz -C vtsPackage/
    cp -r -n -a vtsPackage/out/* out/
fi

# Build vendor/volovcar tests (Unit and Component Tests)
citime python3 "$REPO_ROOT_DIR"/vendor/volvocars/tools/ci/shipit/tester.py build --plan gate --ciflow true || die "Build Unit and Component tests failed"
cp out/.ninja_log out/vcc_build_metadata/ninja_log_make_tester_build || true

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
time tar -c --use-compress-program='pigz -1' -f "${OUT_ARCHIVE}" \
            ./out/target/product/ihu_abl_car/fast_flashfiles \
            ./out/target/product/ihu_abl_car/data \
            ./out/host/linux-x86/bin \
            ./out/host/linux-x86/vts/android-vts \
            ./out/host/linux-x86/tradefed || die "Could not create out archive"

# Ensure our repo can be build with mma
# Must be done AFTER "make droid", otherwise we risk putting stuff into the image that are not present in device.mk
citime mmma vendor/volvocars
cp out/.ninja_log out/vcc_build_metadata/ninja_log_mmmma_vendor_volvocars || true
cp out/.build.trace.gz out/vcc_build_metadata/mmma_vendorvolvocars.trace.gz || true

ls -lh "$OUT_ARCHIVE"
time artifactory push ihu_gate_build "${ZUUL_COMMIT}" "${OUT_ARCHIVE}"

# Create build meta data metaData.tgz
BUILD_META_DATA=metaData.tgz
time tar -c --use-compress-program='pigz -1' -f "${BUILD_META_DATA}" \
            ./out/vcc_build_metadata || die "Could not create metadata archive"

ls -lh "$BUILD_META_DATA"
time artifactory push ihu_gate_build "${ZUUL_COMMIT}" "${BUILD_META_DATA}"

# Clean up vtsPackages
rm -rf vtsPackage/

rm -rf outVTS.tgz
