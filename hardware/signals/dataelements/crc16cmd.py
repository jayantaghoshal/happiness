import sys
import crc16

total = len(sys.argv)
if total!=3:
    sys.exit("Usage: crc16cmd.py <file> <def-name>")
else:
    file = open(sys.argv[1],'r')
    print "// "+sys.argv[1]
    print "#ifndef "+sys.argv[2]
    print "#define "+sys.argv[2]+" "+str(crc16.crc16(file.read()))
    print "#endif"

