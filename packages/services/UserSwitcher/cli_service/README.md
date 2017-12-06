# Purpose

Test application to investigate how android behaves when switching user early in the boot process.



# Installation instructions:

1. adb push test_user_switch.rc /vendor/etc/init
2. adb push test_user_switch.sh /vendor/bin/
3. adb shell pm create-user User2
4. Reboot device
5. adb shell tail -f /data/switch-user-test.txt