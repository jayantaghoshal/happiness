#!/bin/bash

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

adb shell "mount -o rw,remount /vendor"
(croot && \
    cd out/target/product/ihu_vcc && \
    find vendor -type f \( -name "*signals*" -o -name "*hidl-server*" -o -name "*dataelem*" \) | \
    while read x; \
        do echo "$x" && \
        adb push "$x" "$x"; \
    done)
