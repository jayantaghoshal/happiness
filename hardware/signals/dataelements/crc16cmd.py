# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
import crc16

total = len(sys.argv)
if total!=3:
    sys.exit("Usage: crc16cmd.py <file> <def-name>")
else:
    with open(sys.argv[1],'rb') as file:
        print("// "+sys.argv[1])
        print("#ifndef "+sys.argv[2])
        print("#define "+sys.argv[2]+" "+str(crc16.crc16(file.read())))
        print("#endif")

