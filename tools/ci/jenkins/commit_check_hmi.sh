#!/bin/bash

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

set -ex

# Download the Android SDK Command Line Tools
# Version reference: https://developer.android.com/studio/
if [ -d "android-sdk-linux" ]; then
  echo "SDK already downloaded."
else
  mkdir android-sdk-linux
  (cd android-sdk-linux)
  wget https://dl.google.com/android/repository/sdk-tools-linux-3859397.zip
  unzip sdk-tools-linux-3859397.zip
fi

# Download the Android SDK
export ANDROID_HOME="$PWD/android-sdk-linux"
export PATH="$ANDROID_HOME/tools:$ANDROID_HOME/platform-tools:$PATH"

"$ANDROID_HOME/tools/bin/sdkmanager --update"
"$ANDROID_HOME/tools/bin/sdkmanager "platforms;android-27" "build-tools;27.0.3" "extras;google;m2repository" "extras;android;m2repository""
"yes | $ANDROID_HOME/tools/bin/sdkmanager --licenses"
"$ANDROID_HOME/tools/bin/sdkmanager "platforms;android-P" "build-tools;28.0.0-rc1" "extras;google;m2repository" "extras;android;m2repository""
"yes | $ANDROID_HOME/tools/bin/sdkmanager --licenses"
"$ANDROID_HOME/tools/bin/sdkmanager --update"
"yes | $ANDROID_HOME/tools/bin/sdkmanager --licenses"

# Set the SDK dir for Gradle
if ! grep -q "sdk.dir=$ANDROID_HOME" "$ZUUL_PROJECT/local.properties"; then
  echo sdk.dir="$ANDROID_HOME" >> "$ZUUL_PROJECT/local.properties"
fi

# TODO: Run in sandbox and verify the output folder location and archive/result storage
"$ZUUL_PROJECT"/gradlew clean assemble check -p "$ZUUL_PROJECT"