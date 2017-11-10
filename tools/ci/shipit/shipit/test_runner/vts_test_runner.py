#!/usr/bin/env python3

# TODO: We should move the vts_test_runner.py to ../test-fw directory.
#       But we want to discuss the structure with CI-guards first.

import argparse
import logging
import logging.config
import json
import os
import re
from shipit.process_tools import check_output_logged
from shipit.test_runner.test_types import TestFailedException
import xml.etree.cElementTree as ET
import concurrent.futures
from typing import Dict

# Add pwd to the PYTHONPATH because VTS runs in its own environment with different working directory
# than where we launch it. It then translates the <option name="test-case-path" value="x/y/z" />
# into a python module import by replacing / with . To avoid cluttering the whole repo with
# __init__.py files we set the test-case-path relative to the AndroidTest.xml directory and add that
# directory to the PYTHONPATH here.
def _create_env(test_module_dir_path: str) -> Dict[str, str]:
    env = os.environ.copy()
    orig_value = env.get("PYTHONPATH")
    env['PYTHONPATH'] = orig_value + os.pathsep + test_module_dir_path if orig_value else test_module_dir_path
    return env

def vts_tradefed_run(test_module_dir_path: str):
    module_name = read_module_name(os.path.join(test_module_dir_path, "AndroidTest.xml"))
    logging.info("Running test module %s" % module_name)
    max_test_time_sec = 60 * 60
    try:
        test_result = check_output_logged(["vts-tradefed",
                                           "run",
                                           "commandAndExit",
                                           "vts",
                                           "--skip-all-system-status-check",
                                           "--skip-preconditions",
                                           "-l",
                                           "INFO",
                                           "--abi",
                                           "x86_64",
                                           "--module",
                                           module_name],
                                          timeout_sec=max_test_time_sec,
                                          env=_create_env(test_module_dir_path)).decode().strip(" \n\r\t")
    except concurrent.futures.TimeoutError as te:
        raise TestFailedException("Test time out, maximum test time: %d sec" % max_test_time_sec)
    except Exception as e:
        raise TestFailedException(
            "Could not run test, maybe you forgot to issue lunch before to setup environment? Reason: %r" % e)

    fail_pattern1 = "fail:|PASSED: 0"
    if re.search(fail_pattern1, test_result):
        raise TestFailedException("Test failed! This pattern in not allowed in the output: \"%s\"" % fail_pattern1)

    fail_pattern2 = "PASSED: [1-9][0-9]*"
    if not re.search(fail_pattern2, test_result):
        raise TestFailedException("Test failed! This pattern was missing in the output: \"%s\"" % fail_pattern2)

def read_module_name(android_test_xml_file: str):
    logging.info("Reading module name from %s" % android_test_xml_file)

    et = ET.parse(android_test_xml_file)

    module_name_option = et.find("./test/option/[@name='module-name']")
    if module_name_option is not None:
        return module_name_option.attrib["value"]

    test_module_name_option = et.find("./test/option/[@name='test-module-name']")
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

    vts_tradefed_run(getattr(parsed_args, "test-module-dir"))

    logging.info("Test completed")


if __name__ == "__main__":
    main()
