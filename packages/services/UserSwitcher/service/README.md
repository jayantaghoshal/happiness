# Purpose

Service that switches user and unlocks the user as soon as the device is booted.
Used to investigate how android behaves when switching user early in the boot process.


# Prerequisites:
Run from root of AOSP tree:

    make libconscrypt_openjdk_jni -j8
    export LD_LIBRARY_PATH=$ANDROID_HOST_OUT/lib64
    adb shell pm create-user User2

# Building
Run from this directory:

    mma -j8
    java -jar $ANDROID_HOST_OUT/framework/signapk.jar \
        $ANDROID_BUILD_TOP/build/target/product/security/platform.x509.pem \
        $ANDROID_BUILD_TOP/build/target/product/security/platform.pk8 \
        $ANDROID_BUILD_TOP/out/target/product/ihu_vcc/system/priv-app/VccUserSwitchService/VccUserSwitchService.apk ./output-app-signed.apk

# Installing

    adb uninstall com.volvocars.userswitch
    adb install ./output-app-signed.apk
    adb shell am start-service com.volvocars.userswitch


# Analyzing

    adb logcat -v monotonic -s VccUserSwitchService:V SystemServer:V SYSTEM_SERVER_TIMING_TAG:V SYSTEM_SERVER_TIMING_TAGAsync:V ActivityManager:V