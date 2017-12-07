# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
import subprocess
import sys

from subprocess import Popen, PIPE

print("Doing test")
result = subprocess.check_output("adb shell ls /proc/", stderr=subprocess.STDOUT, shell=True).rstrip()
regex = ur"([0-9]+).*"
lost_pids = 0
matches = re.finditer(regex, result)
print("         CPU    PID           PROCESS")
for matchNum, match in enumerate(matches):
    try:
        string_to_use = ("adb shell cat proc/{0}/stat".format(match.group(1)))
        stat_for_process = subprocess.check_output(string_to_use, stderr=subprocess.STDOUT, shell=True).rstrip()
        extraction_regex = ur"([0-9]+).\((.*)\)(\s+\S*){11}\s(\d*)\s(\d*)"
        internal_matches = re.finditer(extraction_regex, stat_for_process)
        for internalMatchNum, internalMatch in enumerate(internal_matches):
            print("{:>12}  {:>5}  {:>16}".format(int(internalMatch.group(4))+int(internalMatch.group(5)), internalMatch.group(1), internalMatch.group(2)))
    except subprocess.CalledProcessError as e:
        # the process was terminated stat was called, do nothing
        lost_pids += 1
        print("ERROR")
