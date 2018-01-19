#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""
To speed up the build process it is possible patch library.go to turn off the
header-abi-dumper. This should not be checked in to the code, so this is a
check to safe guard library.go
"""

import os
import sys
import re

def check_code():
    abi_boolean_path = os.environ['REPO_ROOT_DIR'] + "/build/soong/cc/library.go"
    with open(abi_boolean_path) as abi_file:
        abi_readout = abi_file.read()
        if re.search(r'flags.SAbiDump\s*=\s*true', abi_readout, re.DOTALL):
            print("header-abi-dump is set to true")
        else:
            print("header-abi-dump is set to false, not allowed")
            sys.exit(1)

def main():
    check_code()

if __name__ == "__main__":
    main()