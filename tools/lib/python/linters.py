# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import subprocess
import xml.parsers.expat

import android_logging_linter
from ihuutils import external_tool_finder


class LinterError(Exception):
    def __init__(self, violation, source_file=None):
        if source_file is None:
            super(LinterError, self).__init__(violation)
        else:
            super(LinterError, self).__init__("{} in file {}".format(violation, source_file))


class AndroidLoggingLinterError(LinterError):
    pass


class ShellcheckLinterError(LinterError):
    pass


class XmlLinterError(LinterError):
    pass


SHELLCHECK_BINARY = external_tool_finder.find_tool('shellcheck', 'SHELLCHECK_BINARY')

cpp_extensions = ['.cpp', '.c', '.hpp', '.h']
shellcheck_extensions = ['.sh']
xml_extension = ['.xml']

supported_extensions = cpp_extensions + shellcheck_extensions + xml_extension


def can_file_be_linted(path: str):
    filename, extension = os.path.splitext(path)
    return extension in supported_extensions


def run_for_file(path: str):
    filename, extension = os.path.splitext(path)

    if extension in shellcheck_extensions:
        try:
            subprocess.check_output([SHELLCHECK_BINARY, path])
        except subprocess.CalledProcessError as e:
            raise ShellcheckLinterError(e.output.decode())

    if extension in cpp_extensions:
        violations = list(android_logging_linter.check_file(path))
        if len(violations) > 0:
            violation = violations[0]
            raise AndroidLoggingLinterError(violation.violation, violation.file)

    if extension in xml_extension:
        try:
            parser = xml.parsers.expat.ParserCreate()
            with open(path, "rb") as xml_as_binary_file:
                parser.ParseFile(xml_as_binary_file)
        except Exception as e:
            raise XmlLinterError(e, path)
