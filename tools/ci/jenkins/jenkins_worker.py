#!/usr/bin/env python3

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import textwrap
import os
import sys
import argparse


class ArgumentHandler(object):

    def parse(self):

        parser = argparse.ArgumentParser(
            prog="jenkins worker",
            formatter_class=argparse.RawDescriptionHelpFormatter,
            description=textwrap.dedent('''\
                Command line tool to interact for running jenkins jobs

                Commands:
                check   - Run the check stage
                build   - Run the gate-build stage
                test    - Runt the gate-test stage
                show    - View attributes
                ''')
            )
        subparser = parser.add_subparsers()

        check_parser = subparser.add_parser('check', description='Run the check stage')
        check_parser.add_argument('stage_type', metavar='STAGE-TYPE', help='Available options: sdk, system')
        check_parser.set_defaults(func=self.check)

        build_parser = subparser.add_parser('build', description='Run the build stage')
        build_parser.add_argument('stage_type', metavar='STAGE-TYPE', help='Available options: sdk, system')
        build_parser.set_defaults(func=self.build)

        test_parser = subparser.add_parser('test', description='Run the test stage')
        test_parser.add_argument('stage_type', metavar='STAGE-TYPE', help='Available options: sdk, system')
        test_parser.set_defaults(func=self.build)

        return parser.parse_args()

    def check(self, args):
        print("in check")

    def build(self, args):
        print("in build")

    def test(self, args):
        print("in test")

def main():
    '''Main function'''
    parser = ArgumentHandler().parse()
    args = parser.parse_args()
    if hasattr(args, 'func'):
        args.func(args)
    else:
        parser.print_help()
        sys.exit(1)