#! /usr/bin/sh

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

echo "$(cat /proc/uptime) Test started" >> /data/switch-user-test.txt
while true
do
    sleep 1
    echo "$(cat /proc/uptime) current-user: $(am get-current-user)"  >> /data/switch-user-test.txt
    am switch-user 10
    CURRENT=$(am get-current-user)

    if [ "$CURRENT" = "10" ]; then
        echo "Matched current user" >> /data/switch-user-test.txt
        sleep 10000000
    fi;
done