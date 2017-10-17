#!/usr/bin/env python3

import logging
import logging.config
import re
import sys
from shipit.process_tools import check_output_logged
from shipit.test_runner.test_types import TestFailedException
import xml.etree.cElementTree as ET
import concurrent.futures
import zipfile
import os
import subprocess
import typing

def find_test_xml_files(test_path: str) -> typing.List[str]:
    # https://source.android.com/devices/tech/test_infra/tradefed/full_example
    #     "Tradefed automatically recognizes all configurations placed in config folders on the classpath."
    # We enforce that convention a bit stricter by only looking in res/config
    print("test path is ", test_path)
    config_path = os.path.abspath(os.path.join(test_path, "res", "config"))
    print("config path is ", config_path)
    xml_files = [os.path.join(config_path, x) for x in os.listdir(config_path) if x.endswith(".xml")]
    if len(xml_files) == 0:
        raise TestFailedException("Failed to find any xml configurations in %s" % config_path)
    return xml_files

def tradefed_run(test_path: str):
    logging.info("Running tradefed test from xml %s" % test_path)
    for x in find_test_xml_files(test_path):
        tradefed_run_xml(x)

def tradefed_run_xml(xml_path: str):
    if not config_got_xml_reporter(xml_path):
        raise TestFailedException("Can't run test case, XmlResultReporter is missing from the test xml configuration %s" % xml_path)

    # TODO: I don't know how to handle adb not connected errors. Tradefed will reschedule them and try to rerun
    #       in the background.
    max_test_time_sec = 60 * 60
    try:
        test_result = check_output_logged(["tradefed.sh", "run", "commandAndExit", xml_path],
                                          timeout_sec=max_test_time_sec).decode().strip(" \n\r\t")
    except concurrent.futures.TimeoutError as te:
        raise TestFailedException("Test time out, maximum test time: %d sec" % max_test_time_sec)
    except subprocess.CalledProcessError as e:
        raise TestFailedException(
            "Could not run test, maybe you forgot to issue lunch before to setup environment? Reason: %r" % e.stderr.decode())

    assert_no_failures_in_test_result(test_result)

def config_got_xml_reporter(xml_path: str) -> bool:
    root = ET.parse(xml_path)
    result_reporters = root.findall("./result_reporter")
    for r in result_reporters:
        if r.attrib["class"].strip() == "com.android.tradefed.result.XmlResultReporter":
            return True
    return False

def assert_no_failures_in_test_result(tradefed_console_output: str):
    result_pattern = (
        # Date
        r"\S+\s+" +
        # Time
        r"\S+\s+" +
        # Tag
        r"I\/XmlResultReporter\s*:\s*" +
        # Message
        r"XML test result file generated at (.*?)\.\s*Total tests")
    m = re.search(result_pattern, tradefed_console_output)
    if m is None:
        raise TestFailedException("Failed to find XML test report, does your test xml contain an XmlResultReporter? Stdout of tradefed: %s" % tradefed_console_output)
    zip_location = m.group(1)
    result_zip = zipfile.ZipFile(zip_location)
    test_result_files_in_zipfile = [s for s in result_zip.namelist() if "test_result" in s]
    if len(test_result_files_in_zipfile) == 0:
        raise TestFailedException("Failed to read result xml, no test_results found in zipfile. Files found: %s" % result_zip.namelist())

    for n in test_result_files_in_zipfile:
        with result_zip.open(n) as f:
            result_xml_contents = f.read()
        root = ET.ElementTree(ET.fromstring(result_xml_contents))
        nr_of_tests = int(root.getroot().attrib["tests"])
        nr_of_fails = int(root.getroot().attrib["failures"])
        nr_of_errors = int(root.getroot().attrib["errors"])

        # TODO: Extract the failure tag. Will there be only one? Will there be an error tag?
        if nr_of_tests == 0:
            raise TestFailedException("No tests run, result xml is: %s" % result_xml_contents)
        if nr_of_fails > 0:
            raise TestFailedException("Found failed tests, result xml is: %s" % result_xml_contents)
        if nr_of_errors > 0:
            raise TestFailedException("Found tests in error, result xml is: %s" % result_xml_contents)

if __name__ == "__main__":
    tradefed_run(sys.argv[1])