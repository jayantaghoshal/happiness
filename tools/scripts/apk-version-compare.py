#!/usr/bin/python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# This script used to get and compare apk-versions on the target
# It was written since it is somewhat difficult to know if an
# updated apk have made it into the image.
# Uses adb towards a real target ihu
#
# Write current apks with version to file apk_version_<date>_<time>.json
# $python3 apk-version-compare.py dump
#
# Compare current apks and versions with those in the file
# $python3 apk-version-compare.py apk_version_20180101_0101.json
#
# Print apk package name with version on stdout
# $python3 apk-version-compare.py

# Note: There is lot of information in the 'dumpsys package <name>' output.
#       It should be easy to extract other info in the same way as this script.

import subprocess
import sys
import json
import time

proc_res = subprocess.run(['adb', 'shell', 'pm', 'list', 'packages', '-f'], stdout=subprocess.PIPE, check=True, universal_newlines=True)
pkg_dict = {}  # dictionary with package name and version
for l in proc_res.stdout.split('\n'):
    if l.startswith("package:"):
        parts = l.split(".apk=")
        if len(parts) != 2:
            print("Not 2 parts:", parts)
        else:
            proc_res = subprocess.run(['adb', 'shell', 'dumpsys package', parts[1], '|', 'grep', 'versionName'],
                                      stdout=subprocess.PIPE, check=True, universal_newlines=True)
            #version_name = subprocess.getoutput("adb shell dumpsys package %s | grep versionName" % parts[1])
            pkg_dict[parts[1]] = proc_res.stdout.strip('\n')

if len(sys.argv) == 2:
    if sys.argv[1] == "dump":
        fn = 'apk_version_%s.json' % time.strftime('%Y%m%d_%H%M%S')
        print("Writing version to %s" % fn)
        with open(fn, "w") as fo:
            json.dump(pkg_dict, fo, indent=2)
    else:
        with open(sys.argv[1]) as fi:
            print("Comparing versions from %s" % sys.argv[1])
            pkg_from_file_dict = json.load(fi)
            for k, v in pkg_from_file_dict.items():
                if k in pkg_dict:
                    if pkg_dict[k] != pkg_from_file_dict[k]:
                        print("Package %s: %s => %s" % (k, pkg_from_file_dict[k], pkg_dict[k]))
                    else:
                        print("Same version: %s" % k)
                else:
                    print("New package %s: %s" % (k, pkg_dict[k]))
            for k, v in pkg_dict.items():
                if k not in pkg_from_file_dict:
                    print("Package %s Added" % pkg_dict[k])
else:
    print('PackageName VersionName')
    for k, v in pkg_dict.items():
        print(k, v)
