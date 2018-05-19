#! /bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

err_report() {
    echo "Error on line $1"
}

trap 'err_report $LINENO' ERR

SCRIPT_DIR=$(cd "$(dirname "$(readlink -f "$0")")"; pwd)
REPO_ROOT_DIR=$(readlink -f "${SCRIPT_DIR}"/../../../../)

AUTOMOTIVE_2_0_JAR="${REPO_ROOT_DIR}/out/soong/.intermediates/hardware/interfaces/automotive/vehicle/2.0/android.hardware.automotive.vehicle-V2.0-java/android_common/javac/android.hardware.automotive.vehicle-V2.0-java.jar"
IREMOTE_CLIMATE_CTRL_SERVICE_JAR="${REPO_ROOT_DIR}/out/target/product/ihu_abl_car/obj/JAVA_LIBRARIES/com.volvocars.remotectrlservices.climatectrl.IRemoteClimateCtrlService_intermediates/javalib.jar"
REMOTE_CTRL_HIDL_JAR="${REPO_ROOT_DIR}/out/soong/.intermediates/vendor/volvocars/interfaces/remotectrl/1.0/vendor.volvocars.hardware.remotectrl-V1.0-java/android_common/javac/vendor.volvocars.hardware.remotectrl-V1.0-java.jar"
HW_BINDER_JAR="${REPO_ROOT_DIR}/out/soong/.intermediates/frameworks/base/hwbinder/android_common/javac/hwbinder.jar"
HIDL_BASE_JAR="${REPO_ROOT_DIR}/out/soong/.intermediates/system/libhidl/transport/base/1.0/android.hidl.base-V1.0-java/android_common/javac/android.hidl.base-V1.0-java.jar"
CLIMATE_SUPPORT_LIBRARY="${REPO_ROOT_DIR}/out/target/product/ihu_abl_car/obj/JAVA_LIBRARIES/remote-climate-support-lib_intermediates/javalib.jar"

# Support-lib
SUPPORT_SHARED_LIBS_PATH="${REPO_ROOT_DIR}/vendor/volvocars/packages/RemoteCtrlServices/support-lib/app/libs/"
rm -rf "${SUPPORT_SHARED_LIBS_PATH}"
mkdir "${SUPPORT_SHARED_LIBS_PATH}"
cp "${AUTOMOTIVE_2_0_JAR}" "${SUPPORT_SHARED_LIBS_PATH}"
cp "${REMOTE_CTRL_HIDL_JAR}" "${SUPPORT_SHARED_LIBS_PATH}"
cp "${IREMOTE_CLIMATE_CTRL_SERVICE_JAR}" "${SUPPORT_SHARED_LIBS_PATH}/IRemoteClimateCtrlService.jar"

# Gateway
GATEWAY_SHARED_LIBS_PATH="${REPO_ROOT_DIR}/vendor/volvocars/packages/RemoteCtrlServices/climatectrl/gateway/app/libs/"
rm -rf "${GATEWAY_SHARED_LIBS_PATH}"
mkdir "${GATEWAY_SHARED_LIBS_PATH}"
cp "${REMOTE_CTRL_HIDL_JAR}" "${GATEWAY_SHARED_LIBS_PATH}"
cp "${IREMOTE_CLIMATE_CTRL_SERVICE_JAR}" "${GATEWAY_SHARED_LIBS_PATH}/IRemoteClimateCtrlService.jar"
cp "${AUTOMOTIVE_2_0_JAR}" "${GATEWAY_SHARED_LIBS_PATH}"
cp "${HW_BINDER_JAR}" "${GATEWAY_SHARED_LIBS_PATH}"
cp "${CLIMATE_SUPPORT_LIBRARY}" "${GATEWAY_SHARED_LIBS_PATH}/remote-climate-support-lib.jar"
cp "${HIDL_BASE_JAR}" "${GATEWAY_SHARED_LIBS_PATH}"

# Service
SERVICE_SHARED_LIBS_PATH="${REPO_ROOT_DIR}/vendor/volvocars/packages/RemoteCtrlServices/climatectrl/service/app/libs/"
rm -rf "${SERVICE_SHARED_LIBS_PATH}"
mkdir "${SERVICE_SHARED_LIBS_PATH}"
cp "${AUTOMOTIVE_2_0_JAR}" "${SERVICE_SHARED_LIBS_PATH}"
cp "${IREMOTE_CLIMATE_CTRL_SERVICE_JAR}" "${SERVICE_SHARED_LIBS_PATH}/IRemoteClimateCtrlService.jar"
cp "${CLIMATE_SUPPORT_LIBRARY}" "${SERVICE_SHARED_LIBS_PATH}/remote-climate-support-lib.jar"

