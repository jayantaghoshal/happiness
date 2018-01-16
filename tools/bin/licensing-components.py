#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""
licensing-components

Usage:
  licensing-components software-bom <root> <output_path>
  licensing-components list-copyright-headers <file_or_files_list>...
  licensing-components fix-copyright-headers <file_or_files_list>...
  licensing-components fix-all-copyright-headers <directory>
  licensing-components verify-copyright-headers <files>...
  licensing-components -h | --help
  licensing-components --version

Options:
  -h --help                         Show this screen.
  --version                         Show version.

Examples:
  licensing-components software-bom /directory

Help:
  This tool support tracking of component organization and listing for
  Volvo Car Corporation developed code, shared with partners.

  It gathers data from .vcc-component.yaml files, and uses it to label
  the files, create software bom etc.
"""

import typing
from ihuutils.docopt_dispatch import dispatch
from licensingcomponents import copyrightheader


@dispatch.on('software-bom')
def software_bom(root, output_path, **kwargs):
    software_bom(root, output_path)


@dispatch.on('list-copyright-headers')
def list_copyright_headers(file_or_files_list: typing.Union[str, typing.Iterable[str]], **kwargs):
    list_copyright_headers(file_or_files_list)


@dispatch.on('fix-copyright-headers')
def fix_copyright_headers(file_or_files_list: typing.Union[str, typing.Iterable[str]], **kwargs):
    copyrightheader.fix_copyright_headers(file_or_files_list)


@dispatch.on('fix-all-copyright-headers')
def fix_all_copyright_headers(directory, **kwargs):
    copyrightheader.fix_all_copyright_headers(directory)


@dispatch.on('verify-copyright-headers')
def verify_copyright_headers(files: typing.Iterable[str], **kwargs):
    copyrightheader.verify_copyright_headers(files)


if __name__ == "__main__" and __package__ is None:
    dispatch(__doc__)
