# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import subprocess
import sys
import json
import time

stdoutdata = subprocess.getoutput("adb shell pm list packages -f")
pk = {}
for l in stdoutdata.split('\n'):
    if l.startswith("package:"):
        parts = l.split(".apk=")
        if len(parts) != 2:
            print("Not 2 parts:", parts)
        else:
            version_name = subprocess.getoutput("adb shell dumpsys package %s | grep versionName" % parts[1])
            pk[parts[1]] = version_name

if len(sys.argv) == 2:
    if sys.argv[1] == "dump":
        fn = 'apk_version_%s.json' % time.strftime('%Y%m%d_%H%M%S')
        print("Writing version to %s" % fn)
        with open(fn, "w") as fo:
            json.dump(pk, fo, indent=2)
    else:
        with open(sys.argv[1]) as fi:
            print("Comparing versions from %s" % sys.argv[1])
            pk_in = json.load(fi)
            for k, v in pk_in.items():
                if k in pk:
                    if pk[k] != pk_in[k]:
                        print("Package %s: %s => %s" % (k, pk_in[k], pk[k]))
                    else:
                        print("Same version: %s" % k)
                else:
                    print("New package %s: %s" % (k, pk[k]))
            for k, v in pk.items():
                if k not in pk_in:
                    print("Package %s Added" % pk[k])
else:
    for k, v in pk.items():
        print(k, v)
