#!/usr/bin/env python3

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# TODO: We should move the vts_test_runner.py to ../test-fw directory.
#       But we want to discuss the structure with CI-guards first.

import argparse
import glob
import logging
import logging.config
import json
import os
import re
import datetime
from shipit.test_runner.test_types import ResultData
from shipit.process_tools import check_output_logged
from shipit.test_runner.test_types import VtsTestFailedException
import xml.etree.cElementTree as ET
import concurrent.futures
from typing import Dict, List, Optional
from subprocess import check_output

# Add pwd to the PYTHONPATH because VTS runs in its own environment with different working directory
# than where we launch it. It then translates the <option name="test-case-path" value="x/y/z" />
# into a python module import by replacing / with . To avoid cluttering the whole repo with
# __init__.py files we set the test-case-path relative to the AndroidTest.xml directory and add that
# directory to the PYTHONPATH here.
def _create_env(test_module: str) -> Dict[str, str]:
    env = os.environ.copy()
    orig_value = env.get("PYTHONPATH")
    env['PYTHONPATH'] = orig_value + os.pathsep + test_module if orig_value else test_module
    return env


def vts_tradefed_run_file(test_module_dir: str, tests_to_run: Optional[List[str]]) -> ResultData:
    xml_path = os.path.join(test_module_dir, "AndroidTest.xml")
    module_name = read_module_name(xml_path)
    return vts_tradefed_run_module(module_name, tests_to_run, _create_env(test_module_dir))


def vts_tradefed_run_module(module_name: str, tests_to_run: Optional[List[str]], env: Dict[str, str] = os.environ.copy()) -> ResultData:
    logging.info("Running test module %s with environment %r" % (module_name, env))
    try:
        os.unlink("/tmp/test_run_kpis.json")
    except FileNotFoundError:
        pass
    try:
        os.unlink("/tmp/test_run_summary.json")
    except FileNotFoundError:
        pass

    tests_to_run_args = []  # type: List[str]
    if tests_to_run is not None:
        assert len(tests_to_run) != 0
        tests_to_run_args = ["--test", ",".join(tests_to_run)]

    max_test_time_sec = 60 * 60
    try:
        test_result = check_output_logged(["vts-tradefed",
                                           "run",
                                           "commandAndExit",
                                           "vts-staging-default",
                                           "--skip-all-system-status-check",
                                           "--skip-preconditions",
                                           "--abi",
                                           "x86_64",
                                           "--module",
                                           module_name] +
                                          tests_to_run_args,
                                          timeout_sec=max_test_time_sec,
                                          env=env).decode().strip(" \n\r\t")
    except concurrent.futures.TimeoutError as te:
        raise VtsTestFailedException("Test time out, maximum test time: %d sec" % max_test_time_sec)
    except Exception as e:
        raise VtsTestFailedException(
            "Could not run test, maybe you forgot to issue lunch before to setup environment? Reason: %r" % e)

    fail_pattern1 = "fail:|PASSED: 0"
    if re.search(fail_pattern1, test_result):
        exception_string = ("Test failed! This pattern in not allowed in the output: \"%s\"" % fail_pattern1)
        raise VtsTestFailedException(exception_string, get_json_object(module_name), get_json_change_time(module_name))

    pass_pattern = "I\/ResultReporter:\s*Invocation finished in.*PASSED:\s*(\d*),\s*FAILED:\s*(\d*)"
    pass_match = re.search(pass_pattern, test_result)
    if pass_match is None:
        raise VtsTestFailedException("Test failed, did not find nr of PASSED/FAILED in resultreporter output",
                                     get_json_object(module_name), get_json_change_time(module_name))
    nr_of_pass = int(pass_match.group(1))
    nr_of_fail = int(pass_match.group(2))

    if nr_of_fail != 0:
        raise VtsTestFailedException("Test failed, Number of FAILED: %d != 0:" % nr_of_fail,
                                     get_json_object(module_name), get_json_change_time(module_name))

    if tests_to_run is not None and nr_of_pass != len(tests_to_run):
        raise VtsTestFailedException("Test failed, Number of PASSED not equal to number of requested test cases. " +
                                     "(Maybe you misspelled one of the requested test cases?)" +
                                     "Nr of pass: %d, Requested test-cases: [%s]" % (nr_of_pass, ", ".join(tests_to_run)),
                                     get_json_object(module_name), get_json_change_time(module_name))


    log_dir_match = re.search(r"I\/ResultReporter:\s+Test Logs:\s+(\S+)", test_result)
    logdict = dict()
    if log_dir_match:
        log_dir = os.path.abspath(log_dir_match.group(1))
        logging.info("Log dir is %s" % log_dir)
        assert("out/host/linux-x86/vts/android-vts/logs" in log_dir)

        def read(name, pattern):
            matches = glob.glob(pattern, recursive=True)
            logging.info("Attempting %s, found mathces: %r" % (pattern, matches))
            if len(matches) == 1:
                gzip_filename = matches[0]
                logdict[name] = check_output(["gzip", "-d", "-c", gzip_filename]).decode()
        read("device_logcat_setup", os.path.join(log_dir, "**", "*device_logcat_setup*.txt.gz"))
        read("device_logcat_teardown", os.path.join(log_dir, "**", "*device_logcat_teardown*.txt.gz"))
        read("device_logcat_test", os.path.join(log_dir, "**", "*device_logcat_test*.txt.gz"))
        read("host_log", os.path.join(log_dir, "**", "*host_log*.txt.gz"))


    return ResultData(True,
                      test_result,
                      get_json_object(module_name),
                      get_json_change_time(module_name),
                      get_json_kpi_results(),
                      logdict)

def get_json_kpi_results():
    try:
        with open('/tmp/test_run_kpis.json') as kpis:
            return json.load(kpis)
    except FileNotFoundError:
        logging.info("test_run_kpis.json not found, ignoring test kpis")
        return {}

def get_json_change_time(module_name):
    try:
        with open('/tmp/test_run_summary.json') as test_result:
            results = json.load(test_result)
            if module_name == results["Results"][0]["Test Class"]:
                time = os.path.getmtime("/tmp/test_run_summary.json")
                last_mod_date = datetime.datetime.fromtimestamp(time)
                return last_mod_date
            else:
                return None
    except (EnvironmentError, TypeError, IndexError) as error:
        print("Error: Cannot find JSON result file that matches running module.")
        return None


def get_json_object(module_name):
    try:
        with open('/tmp/test_run_summary.json') as test_result:
            results = json.load(test_result)
            if module_name == results["Results"][0]["Test Class"]:
                return results
            else:
                return None
    except (EnvironmentError, TypeError, IndexError) as error:
        print("Error: Cannot find JSON result file that matches running module.")
        return None


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

    vts_tradefed_run_file(getattr(parsed_args, "test-module-dir"), None)

    logging.info("Test completed")


if __name__ == "__main__":
    main()
