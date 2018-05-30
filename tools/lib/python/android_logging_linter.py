# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import subprocess
import sys
import re
import os
import typing
import string
from collections import namedtuple

LoggingViolation = namedtuple("LoggingViolation", ["violation", "file"])


def get_invalid_chars_in_tag(tag):
    INVALID_LOGTAG_CHARACTERS = string.whitespace + ":;,*\\`\"'"
    invalid_chars = set()
    for c in tag:
        if ord(c) < 33 or ord(c) >= 126 or c in INVALID_LOGTAG_CHARACTERS:
            invalid_chars.add(c)
    return invalid_chars


def grep(args) -> str:
    try:
        out = subprocess.check_output(["grep"] + args)
        return out.decode("utf-8")
    except subprocess.CalledProcessError as cpe:
        # because grep considers no matches to be an error(1), and other errors(2) to be real error
        if cpe.returncode > 1:
            raise
        return ""


def get_files_with_ALOGx_macros_or_logtag_defined(dir_to_check: str) -> typing.Iterable[str]:
    files = grep([
        "--recursive",
        "--include=*.cpp",
        "--include=*.h",
        "--files-with-matches",
        "--extended-regex",
        "(#define\s+LOG_TAG|ALOG[EWIDV])",
        dir_to_check]).splitlines()

    return files


LOGTAG_PATTERN = re.compile("^#define\s+LOG_TAG\s+\"(.*?)\"", re.MULTILINE)
ALOGX_PATTERN = re.compile("ALOG[EWIDV]")


def check_file(filename: str) -> typing.Iterable[LoggingViolation]:
    try:
        with open(filename, encoding="utf-8") as f:
            file_contents = f.read()
    except UnicodeDecodeError as e:
        print("Failed to analyze file: %s due to %s" % (filename, e))
        return

    no_log_lint_index = file_contents.rfind("NOLOGLINT")
    if no_log_lint_index > 0:
        return

    last_include_index = file_contents.rfind("#include")
    last_include_logging_header_index = max(file_contents.rfind("#include <cutils/log.h>"),
                                            file_contents.rfind("#include <log/log.h>"),
                                            file_contents.rfind("#include <android/log.h>"),
                                            file_contents.rfind("#include <android-base/logging.h>"))

    if last_include_index == last_include_logging_header_index:
        last_include_index = file_contents.rfind("#include", 0, last_include_logging_header_index)

    first_define_logtag_index = file_contents.find("#define LOG_TAG")
    last_define_logtag_index = file_contents.rfind("#define LOG_TAG")
    alog_match = ALOGX_PATTERN.search(file_contents)
    first_alogx_index = -1 if alog_match is None else alog_match.start()

    if filename.endswith(".h"):
        if last_define_logtag_index >= 0:
            yield LoggingViolation("Do not #define LOG_TAG in header file", filename)
        if first_alogx_index >= 0:
            yield LoggingViolation(
                'Avoid using the ALOGx-macros in header file, use ALOG(LOG_xxxx, "tag", "message") instead.',
                filename)
    else:
        logtag_matches = LOGTAG_PATTERN.finditer(file_contents)
        for tm in logtag_matches:
            tagname = tm.group(1)
            if len(tagname) > 23:
                yield LoggingViolation("Logtag exceeds 23 characters: '%s' (%d characers)" % (tagname, len(tagname)),
                                       filename)

            invalid_characters_in_tag = get_invalid_chars_in_tag(tagname)
            if len(invalid_characters_in_tag) > 0:
                yield LoggingViolation("Logtag '%s' contains invalid characters: %s" % (
                    tagname,
                    ", ".join(["'%s'" % c for c in invalid_characters_in_tag])),
                                       filename)

        if 0 <= first_alogx_index:
            if first_define_logtag_index < 0 or first_alogx_index < first_define_logtag_index:
                yield LoggingViolation("ALOGx called before #define LOG_TAG (or no LOG_TAG defined)",
                                       filename)

        if last_include_index >= 0 and 0 <= last_define_logtag_index < last_include_index:
            yield LoggingViolation("LOG_TAG defined before #includes, this might overwrite your LOG_TAG",
                                   filename)


def check_files_in_directory(dir_to_check: str) -> typing.Iterable[LoggingViolation]:
    files_with_logtag = get_files_with_ALOGx_macros_or_logtag_defined(dir_to_check)

    for filename in files_with_logtag:
        yield from check_file(filename)


def main():
    arg = sys.argv[1] if len(sys.argv) > 1 else "."

    if os.path.isdir(arg):
        basedir = arg
        violations = check_files_in_directory(arg)
    elif os.path.isfile(arg):
        basedir = os.path.dirname(arg)
        violations = check_file(arg)
    else:
        print("Path " + arg + " is neither file nor directory");
        sys.exit(1)

    errors_found = False
    for e in violations:
        relname = os.path.relpath(e.file, basedir)

        if not errors_found:
            print("--------------------------------------------")
            print("Log tag errors found")
            print("See https://c1.confluence.cm.volvocars.biz/display/IHUA/Logging for logging guidelines")
            errors_found = True

        print(" * " + relname)
        print("     " + e.violation)
        print()
    if errors_found:
        sys.exit(1)


if __name__ == "__main__":
    main()
