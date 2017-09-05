#!/usr/bin/env python3

# TODO: We should move the test_run.py to ../test-fw directory.
#       But we want to discuss the structure with CI-guards first.

import argparse
import logging
import logging.config
import json
import os
import re
from shipit.process_tools import check_output_logged
import xml.etree.cElementTree as ET

logger = logging.getLogger(__name__)

def vts_tradefed_run_module(module_name: str):
    logging.info("Running test module %s" % module_name)
    try:
        test_result = check_output_logged(["vts-tradefed",
                                           "run",
                                           "commandAndExit",
                                           "vts",
                                           "--skip-all-system-status-check",
                                           "--skip-preconditions",
                                           "--abi",
                                           "x86_64",
                                           "--module",
                                           module_name],
                                           timeout_sec=120).decode().strip(" \n\r\t")
    except Exception as e:
        raise Exception(
            "Could not run test, maybe you forgot to issue lunch before to setup environment? Reason: %r" % e)

    fail_pattern1 = "fail:|PASSED: 0"
    if re.search(fail_pattern1, test_result):
        raise Exception("Test failed! This pattern in not allowed in the output: \"%s\"" % fail_pattern1)

    fail_pattern2 = "PASSED: [1-9][0-9]*"
    if not re.search(fail_pattern2, test_result):
        raise Exception("Test failed! This pattern was missing in the output: \"%s\"" % fail_pattern2)

def read_module_name(android_test_xml_file: str):
    logging.info("Reading module name from %s" % android_test_xml_file)
    et = ET.parse(android_test_xml_file)

    module_name_option = et.find("/test/option/[@name='module-name']")
    if module_name_option is not None:
        return module_name_option.attrib["value"]

    test_module_name_option = et.find("/test/option/[@name='test-module-name']")
    if test_module_name_option is not None:
        return test_module_name_option.attrib["value"]

    raise Exception("Did not find module-name with value in %s" % android_test_xml_file)

def main():
    parser = argparse.ArgumentParser(description="Run a VTS tradefed module")
    parser.add_argument("test-module-dir",
                        help="Directory where the AndroidTest.xml resides for the module to test")

    parsed_args = parser.parse_args()

    with open(os.path.dirname(__file__)+ "/logging.json", "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    android_test_xml_file = "%s/AndroidTest.xml" % getattr(parsed_args, "test-module-dir")
    test_module_name = read_module_name(android_test_xml_file)
    vts_tradefed_run_module(test_module_name)

    logging.info("Test completed")


if __name__ == "__main__":
    main()
