# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import re


def load_source_code(path: str):
    with open(path, 'r', encoding='utf-8') as file:
        return file.read()


def sanitize_whitespace(text):
    trailing_strip_re = re.compile(r'[ \t]+(\n|\Z)')
    text = trailing_strip_re.sub(r"\1", text).lstrip('\r\n ').replace('\r', '')
    return text
