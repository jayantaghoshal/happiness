#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys
import glob
import re
import shutil

try:
    fn = sys.argv[1]
except Exception as e:
    print("Expects path of the vbf files as argument- Error:"+str(e))
    sys.exit(1)

vbf_tags = {
    "vip-pbl.VBF": "SXBL",
    "vip-sbl.VBF": "SWBL",
    "vip-ssbl.VBF": "SWBS",
    "vip-lcfg.VBF": "SWP2",
    "vip-ess.VBF": "SWL3",
    "vip-app.VBF": "SWLM"
}

if os.path.exists(fn):
    no_vbf = True
    for filename in glob.iglob(os.path.join(fn, "*.VBF")):
        sw_part_number = ""
        sw_version = ""
        with open(filename, mode='r') as file_handler:
            for line in file_handler:
                if "sw_part_number = " in line:
                    try:
                        sw_part_number = re.search(
                            '\"(.+?)\"', line).group(1)
                    except AttributeError:
                        print("Part number cannot be found.")
                        sys.exit(1)

                if "sw_version = " in line:
                    try:
                        sw_version = re.search(
                            '\"(.+?)\"', line).group(1)
                        sw_version = sw_version[-2:]   # get only the last two alphabets like (AA, AB, AC)
                    except AttributeError:
                        print("sw version cannot be found.")
                        sys.exit(1)
            new_file_name = sw_part_number+sw_version+".VBF"
        shutil.copyfile(filename, os.path.join(fn, new_file_name))
        basename=os.path.basename(filename)

        with open(os.path.join(fn, "vbf_tags"), mode="a+") as file_handler:
            if "mp-android" in basename:
                file_handler.write("SWL2,"+new_file_name+"\n")
            elif vbf_tags[basename] is not None:
                file_handler.write(vbf_tags[basename]+","+new_file_name+"\n")


        no_vbf = False
    if no_vbf:
        print("No VBFs in the given directory.")
        sys.exit(1)

else:
    raise("path does not exist")
