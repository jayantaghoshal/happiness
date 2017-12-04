import subprocess
import sys
import re
import os
import itertools
import typing
import string
from collections import namedtuple


LoggingViolation = namedtuple("LoggingViolation", "violation file")

# TODO: Remove files from here once they are fixed
#       Used to be able to run scanner on new files in gate and avoid rebase-race to fix old files
temporary_exclusions = [
    "hardware/tarmac/eventloop/test/ct/eventloop_ct_test.cpp",
    "hardware/signals/vehiclesignalsdaemon/src/vsm_sink.cpp",
    "hardware/signals/dataelements/generated/vsm/vsm_inject_switch_ok_cpp.h",
    "hardware/signals/dataelements/AutosarCodeGen/generated/vsm/vsm_inject_switch_ok_cpp.h",
    "hardware/infotainmentIpBus/test/ut/testCodec.cpp",
    "hardware/gnssd/src/binderimpl/gnss/Gnss.cpp",
    "hardware/packet-injector/tiny-ipcommandbus/include/udp_packet_injector.h",
    "hardware/packet-injector/tiny-ipcommandbus/src/Pdu.cpp",
    "hardware/localconfig/src/local_config_file_reader_android_ihu_behavior.cpp",
    "hardware/localconfig/src/local_config_reader.cpp",
    "hardware/signals/desip/src/desip_main.cpp",
    "hardware/signals/desip/src/vibdrvHWlink.cpp",
    "hardware/signals/desip/src/desip_prot.cpp",
    "hardware/signals/dataelements/src/vipcomm/VipFramework.cpp",
    "hardware/signals/dataelements/test/ut/unittest.cpp",
    "hardware/infotainmentIpBus/src/type_conversion_helpers.cpp",
    "hardware/infotainmentIpBus/src/type_conversion_helpers.cpp",
    "hardware/dim/test/ct/apix_gate/apix_gate_ct_test.cpp",
    "hardware/iplmd/test/ct/src/LscMocker.h"]


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
    #TODO: Temp exclude infotainment-ip-service because its not being compiled into image, just used as reference, should be removed
    return (f for f in files if not os.path.relpath(f, dir_to_check).startswith("hardware/infotainment-ip-service"))


LOGTAG_PATTERN = re.compile("^#define\s+LOG_TAG\s+\"(.*?)\"", re.MULTILINE)
ALOGX_PATTERN = re.compile("ALOG[EWIDV]")


def check_file(filename: str)-> typing.Iterable[LoggingViolation]:
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
    last_include_cutils_log_index = file_contents.rfind("#include <cutils/log.h>")

    if last_include_index == last_include_cutils_log_index:
        last_include_index = file_contents.rfind("#include", 0, last_include_cutils_log_index)

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

        if 0 <= first_alogx_index < first_define_logtag_index:
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
        apply_exclusions = True
        violations = check_files_in_directory(arg)
    elif os.path.isfile(arg):
        basedir = os.path.dirname(arg)
        apply_exclusions = False
        violations = check_file(arg)
    else:
        print("Path " + arg + " is neither file nor directory");
        sys.exit(1)

    errors_found = False
    for e in violations:
        relname = os.path.relpath(e.file, basedir)

        if relname in temporary_exclusions and apply_exclusions:
            continue

        if not errors_found:
            print("--------------------------------------------")
            print("Log tag errors found")
            print("See https://c1.confluence.cm.volvocars.biz/display/IHUA/Logging for logging guidelines")
            errors_found = True

        print(" * " + relname )
        print("     " + e.violation)
        print()
    if errors_found:
        sys.exit(1)


if __name__ == "__main__":
    main()
