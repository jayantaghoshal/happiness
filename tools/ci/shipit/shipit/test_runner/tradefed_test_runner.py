#!/usr/bin/env python3

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import re
from shipit.process_tools import check_output_logged
from shipit.test_runner.test_types import TestFailedException, ResultData
import xml.etree.cElementTree as ET
import concurrent.futures
import zipfile
import os
import subprocess
import tempfile

logger = logging.getLogger(__name__)

def tradefed_run(test_path: str, max_test_time_sec: int):
    xml_path = os.path.join(test_path, "AndroidTest.xml")
    logger.info("Running tradefed test from xml %s" % xml_path)
    tradefed_run_xml(xml_path, max_test_time_sec)
    save_files_dir = tempfile.mkdtemp()
    return ResultData(xml_path, True, [], "", "", dict(), dict(), [], save_files_dir)   #TODO: ...

def tradefed_run_xml(xml_path: str, max_test_time_sec: int):
    if not config_got_xml_reporter(xml_path):
        raise TestFailedException("Can't run test case, XmlResultReporter is missing from the test xml configuration %s" % xml_path)

    # TODO: I don't know how to handle adb not connected errors. Tradefed will reschedule them and try to rerun
    #       in the background.
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
