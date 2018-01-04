# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import argparse

__author__ = 'rchiarit'


def argument_parser():
    parser = argparse.ArgumentParser(
        description='Script for modifying CANoe configurations when created by model generation wizard. '
                    'CANoe needs to be started and have the configuration loaded.',
        usage='Errors in parameters use -h to know more details.',
        epilog='Exit codes: OK = {0}, '
               'Bad Argument = {1}, '
               'File Error = {2}, '.format(0, 1, 2))
    subparsers = parser.add_subparsers()

    parser_build = subparsers.add_parser('build', help='Builds a CANoe configurations in debug mode.')
    parser_build.set_defaults(which='build')
    req_args = parser_build.add_argument_group('required arguments')
    req_args.add_argument('-c', '--configuration_folder', type=str, help='CANoe Configuration folder', required=True)
    req_args.add_argument('-a', '--architecture', type=str,
                          help='Select architecture to build: legacy(default), icup', required=True)
    return parser.parse_args()
