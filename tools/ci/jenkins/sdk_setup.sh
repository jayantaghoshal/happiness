#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex
GIT_SSH="$HOME"/zuul_ssh_wrapper.sh zuul-cloner -v "${ZUUL_URL}" "${ZUUL_PROJECT}"
# Download the Android SDK Command Line Tools
# Version reference: https://developer.android.com/studio/
if [ -d "android-sdk-linux" ]; then
  echo "SDK already downloaded."
else
  wget https://dl.google.com/android/repository/sdk-tools-linux-3859397.zip
  unzip sdk-tools-linux-3859397.zip -d android-sdk-linux
fi

# Download the Android SDK
export ANDROID_HOME="$PWD/android-sdk-linux"

yes | "$ANDROID_HOME"/tools/bin/sdkmanager --licenses --proxy=http --proxy_host=10.244.0.55 --proxy_port=83