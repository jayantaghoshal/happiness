#!/bin/bash
adb shell "mount -o rw,remount /vendor"
(croot && \
    cd out/target/product/ihu_vcc && \
    find vendor -type f \( -name "*signals*" -o -name "*hidl-server*" -o -name "*dataelem*" \) | \
    while read x; \
        do echo "$x" && \
        adb push "$x" "$x"; \
    done)
