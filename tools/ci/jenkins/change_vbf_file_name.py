#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys
import glob
import re
import shutil

def main(vbf_dir):

    vbf_tags = {
        "vip-pbl.VBF": "SXBL",
        "vip-sbl.VBF": "SWBL",
        "vip-ssbl.VBF": "SWBS",
        "vip-lcfg.VBF": "SWP2",
        "vip-ess.VBF": "SWL3",
        "vip-app.VBF": "SWLM"
    }

    if os.path.exists(vbf_dir):
        no_vbf = True
        for filename in glob.iglob(os.path.join(vbf_dir, "*.VBF")):
            sw_part_number = ""
            sw_version = ""
            with open(filename, mode='r') as file_handler:
                for line in file_handler:
                    if "sw_part_number = " in line:
                        try:
                            sw_part_number = re.search(
                                '\"(.+?)\"', line).group(1)
                        except AttributeError:
                            raise Exception("Part number cannot be found.")
                    if "sw_version = " in line:
                        try:
                            sw_version = re.search(
                                '\"(.+?)\"', line).group(1)
                        except AttributeError:
                            raise Exception("sw version cannot be found.")

                if sw_part_number != "" and sw_version != "":
                    new_file_name = sw_part_number + sw_version + ".VBF"
                else:
                    raise Exception("sw_part_number/sw_version value missing")
            shutil.copyfile(filename, os.path.join(vbf_dir, new_file_name))
            basename = os.path.basename(filename)

            with open(os.path.join(vbf_dir, "vbf_tags"), mode="a+") as file_handler:
                if "mp-android" in basename:
                    file_handler.write("SWL2," + new_file_name + "\n")
                elif vbf_tags[basename] is not None:
                    file_handler.write(
                        vbf_tags[basename] + "," + new_file_name + "\n")

            no_vbf = False
        if no_vbf:
            raise Exception("No VBFs in the given directory.")
    else:
        raise Exception("path does not exist")


if __name__ == "__main__":
    try:
        vbf_dir = sys.argv[1]
    except Exception as e:
        raise Exception(
            "Expects path of the vbf files as argument- Error:" + str(e))
    main(vbf_dir)
