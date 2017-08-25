import os
from argparse import ArgumentParser
from dataelements_generator import getDatatypes
import autosar

def main():
    print('%s (%s)\n%s\n'%(getDatatypes.DESCRIPTION, getDatatypes.VERSION, getDatatypes.COPYRIGHT))

    parser = ArgumentParser()
    parser.add_argument('--version', action='version', version="%%(prog)s %s (%s)"%(getDatatypes.VERSION, autosar.VERSION))
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true', help='Enable verbose output')
    parser.add_argument('-d', '--directory', dest='outputdirectory', help='Output directory for generated files', required=True)
    parser.add_argument('-s', '--swcinputfile', dest='swcinputfile', help='SWC input arxml, (Elektra export file)', required=True)
    parser.add_argument('-c', '--cominputfile', dest='cominputfile', help='COM input arxml, (Elektra export file)', required=True)
    args = parser.parse_args()

    autosar.arxml.options.verbose = args.verbose

    if not os.path.exists(args.swcinputfile):
        parser.error("swcinputfile \"%s\" does not exist" % args.swcinputfile)
    if not os.path.exists(args.cominputfile):
        parser.error("cominputfile  \"%s\" does not exist" % args.cominputfile)

    getDatatypes.generate(args.swcinputfile, args)
    print("done!")

if __name__ == "__main__":
    main()
