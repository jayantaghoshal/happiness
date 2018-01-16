# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import subprocess
from ihuutils import external_tool_finder
from ihuutils.sourcecode import load_source_code, sanitize_whitespace


class CodeFormatInvalidError(Exception):
    def __init__(self, violation, source_file):
        # Call the base class constructor with the parameters it needs
        super(CodeFormatInvalidError, self).__init__("{} in file {}".format(violation, source_file))


CLANG_FORMAT_BINARY = external_tool_finder.find_tool('clang-format', 'CLANG_FORMAT_BINARY')
BP_FORMAT_BINARY = external_tool_finder.find_tool('bpfmt', 'BP_FORMAT_BINARY')

clang_extensions = ['.cpp', '.c', '.hpp', '.h']
bp_extensions = ['.bp']
whitespace_extensions = ['.mk', '.py', '.sh', '.rc', '.te']

supported_extensions = clang_extensions + bp_extensions + whitespace_extensions


def can_file_be_formatted(path: str):
    filename, extension = os.path.splitext(path)
    return extension in supported_extensions


def get_formatted_file_contents(path: str):
    filename, extension = os.path.splitext(path)
    if extension in clang_extensions:
        return subprocess.check_output([CLANG_FORMAT_BINARY, path]).decode()
    if extension in bp_extensions:
        return subprocess.check_output([BP_FORMAT_BINARY, path]).decode()
    if extension in whitespace_extensions:
        return sanitize_whitespace(load_source_code(path))


def apply_source_file_format(path: str):
    if can_file_be_formatted(path):
        current_content = load_source_code(path)
        formatted_content = get_formatted_file_contents(path)
        if current_content != formatted_content:
            with open(path, 'w', encoding='utf-8') as file:
                file.write(formatted_content)
            print("Fixed code formatting in {}".format(path))


def verify_source_file_format(path: str):
    if can_file_be_formatted(path):
        current_content = load_source_code(path)
        formatted_content = get_formatted_file_contents(path)
        if current_content != formatted_content:
            raise CodeFormatInvalidError("Invalid formatting", path)
