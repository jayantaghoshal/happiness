# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import argparse
from Parser import FolderParser
from FDXWriter import FDXWriter
import os
import sys

__author__ = 'rchiarit'

__version__ = '$Revision$'


# $Source$


def main():
    """Main function."""
    writer = FDXWriter(args.start_group_id, args.output_directory)
    if os.path.isfile(args.file_or_directory):
        folder_parser = FolderParser(writer)
        folder_parser.parse_single_file(args.file_or_directory, args.skip_read_sysvar, args.add_io)
    elif os.path.isdir(args.file_or_directory):
        folder_parser = FolderParser(writer)
        folder_parser.parse_multiple_files(args.file_or_directory, args.skip_read_sysvar, args.add_io)
    else:
        print '{} is not a valid file or directory'.format(args.file_or_directory)
        sys.exit(1)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Parses one or multiple .vsysvar-files into an FDX Description file.')
    parser.add_argument('-f', '--file_or_directory', type=str, help='Input file or directory (default is the current'
                                                                    ' directory).', required=False,
                        default=os.path.dirname(os.path.abspath(__file__)))
    parser.add_argument('-i', '--start_group_id', type=int, help='Start group ID (default is 1100).', required=False,
                        default=1100)
    parser.add_argument('-s', '--skip_read_sysvar', type=bool, help='Decide if we should have read access to '
                                                                    'system variables.', required=False, default=False)
    parser.add_argument('-o', '--output_directory', type=str,
                        help='Output directory of the generated files (default is the \'output\' directory).',
                        required=False, default=os.path.abspath('output'))
    parser.add_argument('-a', '--add_io', type=bool, help='Add variables to write onto Vector I/O system variables.',
                        required=False, default=False)
    args = parser.parse_args()
    main()
