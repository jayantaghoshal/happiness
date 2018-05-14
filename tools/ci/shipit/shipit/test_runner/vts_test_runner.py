#!/usr/bin/env python3

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# TODO: We should move the vts_test_runner.py to ../test-fw directory.
#       But we want to discuss the structure with CI-guards first.

import glob
import logging
import json
import os
import re
import datetime
from shipit.test_runner.test_types import ResultData, VTSTest
from shipit.process_tools import check_output_logged, ProcessRunner
#from shipit.test_runner.test_types import VtsTestFailedException
import xml.etree.cElementTree as ET
import concurrent.futures
from typing import Dict, List, Optional, Set
from subprocess import check_output, CalledProcessError
import typing
#import zipfile

logger = logging.getLogger(__name__)

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


def vts_tradefed_run_file(test_module_dir: str,
                          tests_to_run: Optional[List[str]],
                          max_test_time_sec) -> ResultData:
    xml_path = os.path.join(test_module_dir, "AndroidTest.xml")
    module_name = read_module_name(xml_path)
    return vts_tradefed_run_module(module_name, tests_to_run, _create_env(test_module_dir), max_test_time_sec)


def vts_tradefed_run_module(module_name: str,
                            tests_to_run: Optional[List[str]],
                            env: Dict[str, str] = os.environ.copy(),
                            max_test_time_sec=60 * 60) -> ResultData:
    logger.info("Running test module {}".format(module_name))
    logger.debug("Environment %r" % (env))
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
        nr_tests = len(tests_to_run)
        assert nr_tests != 0
        tests_to_run_args = ["--test", ",".join(tests_to_run)]
    else:
        nr_tests = 0

    output = ""
    info = ""
    success = True
    try:
        test_result = check_output_logged(["vts-tradefed",
        #test_result = ProcessRunner().run(["vts-tradefed",
                                           "run",
                                           "commandAndExit",
                                           "vts-staging-default",
                                           "--skip-all-system-status-check",
                                           "--skip-preconditions",
                                           "--primary-abi-only",
                                           "--screenshot-on-failure",
                                           "--module",
                                           module_name] +
                                          tests_to_run_args,
                                          timeout_sec=max_test_time_sec,
                                          env=env)
        output = test_result.decode('UTF-8', 'backslashreplace').strip(" \n\r\t")
    except concurrent.futures.TimeoutError as te:
        info = "Test time out, maximum test time: %d sec" % max_test_time_sec
        success = False
        logger.error(info)
    except Exception as e:
        info = str(e)
        success = False
        logger.error("Could not run test, maybe you forgot to issue lunch before to setup environment? Reason: %r" % e)

    lines = output.splitlines()
    if len(lines) < 10: # Arbitrarily choosen limit
        for line in lines:
            logger.info("Console: " + str(line))

    if success:
        success, info = _check_output_for_failure(output, nr_tests)
    logdict = _check_output_for_logs(output)
    screenshots = _check_output_for_screenshots(output)

    results = _parse_xml(output)
    for result in results:
        logger.info("Test: {} result: {}".format(result['name'], result['result']))
        if 'message' in result:
            logger.error(result['message'])
        if 'stacktrace' in result:
            logger.error(result['stacktrace'])

    return ResultData(name=module_name,
                      passed=success,
                      console=output,
                      results=results,
                      info=info,
                      test_kpis=get_json_kpi_results(),
                      logs=logdict,
                      screenshot_paths=screenshots)


def _parse_xml(output: str):
    result_match = re.search(r"I\/ResultReporter:\s+Test Result:\s+(\S+)", output)
    if not result_match:
        logger.warning("Could not find test results")
        return []
    result_path = os.path.dirname(os.path.abspath(result_match.group(1)))
    tree = ET.parse(os.path.join(result_path, 'test_result.xml'))
    root = tree.getroot()
    results = []
    for test in root.iter('Test'):
        result = {'name': test.attrib['name'], 'result': test.attrib['result']}
        failure = test.find('Failure')
        if failure:
            try:
                result['message'] = failure.attrib['message']
            except Exception:
                pass
            stacktrace = failure.find('StackTrace')
            try:
                result['stacktrace'] = stacktrace.text
            except Exception:
                pass
        results.append(result)
    return results


def _check_output_for_screenshots(test_result: str) -> typing.List[str]:
    # example: "05-03 05:44:29 D/ResultReporter: Saved logs for VtsFlexraySignalingCT_OneSendOneReceive#testFlexray2-screenshot in /home/eelmeke/android/vcc/out/host/linux-x86/vts/android-vts/logs/2018.05.03_05.43.32/inv_16988131975858974672/VtsFlexraySignalingCT_OneSendOneReceive#testFlexray2-screenshot_2646366068102957666.png"
    screenshots = [m[1] for m in re.findall(r"./ResultReporter:\s+Saved logs for\s+(.*-screenshot)\s+in\s+(\S+)", test_result)]
    for screenshot in screenshots:
        logger.info("Screenshot: {}".format(screenshot))
    return screenshots


def _check_output_for_logs(test_result: str) -> typing.Mapping[str, str]:
    log_dir_match = re.search(r"I\/ResultReporter:\s+Test Logs:\s+(\S+)", test_result)
    logdict = dict()
    if log_dir_match:
        log_dir = os.path.abspath(log_dir_match.group(1))
        logger.debug("Log dir is %s" % log_dir)
        assert("out/host/linux-x86/vts/android-vts/logs" in log_dir)

        def read(name, pattern):
            matches = glob.glob(pattern, recursive=True)
            logger.debug("Attempting %s, found mathces: %r" % (pattern, matches))
            if len(matches) == 1:
                gzip_filename = matches[0]
                logdict[name] = check_output(["gzip", "-d", "-c", gzip_filename]).decode('UTF-8', 'backslashreplace')
        read("device_logcat_setup", os.path.join(log_dir, "**", "*device_logcat_setup*.txt.gz"))
        read("device_logcat_teardown", os.path.join(log_dir, "**", "*device_logcat_teardown*.txt.gz"))
        read("device_logcat_test", os.path.join(log_dir, "**", "*device_logcat_test*.txt.gz"))
        read("host_log", os.path.join(log_dir, "**", "*host_log*.txt.gz"))
    return logdict


def _check_output_for_failure(test_result: str, nr_tests: int) -> typing.Tuple[bool, str]:
    if not test_result:
        info = "No test results"
        logger.warning(info)
        return False, info

    fail_pattern1 = "fail:|PASSED: 0"
    if re.search(fail_pattern1, test_result):
        info = "Test failed! This pattern in not allowed in the output: \"%s\"" % fail_pattern1
        logger.warning(info)
        return False, info

    pass_pattern = "I\/ResultReporter:\s*Invocation finished in.*PASSED:\s*(\d*),\s*FAILED:\s*(\d*)"
    pass_match = re.search(pass_pattern, test_result)
    if pass_match is None:
        info = "Test failed, did not find nr of PASSED/FAILED in resultreporter output"
        logger.warning(info)
        return False, info

    nr_of_pass = int(pass_match.group(1))
    nr_of_fail = int(pass_match.group(2))

    if nr_of_fail != 0:
        info = "Test failed, Number of FAILED != 0"
        logger.warning(info)
        return False, info

    if nr_of_pass == 0:
        info = "Test failed, Number of PASSED == 0"
        logger.warning(info)
        return False, info

    if nr_tests and nr_of_pass != nr_tests:
        info = "Test failed, Number of PASSED not equal to number of requested test cases."
        logger.warning(info)
        return False, info

    return True, ""


def get_json_kpi_results():
    try:
        with open('/tmp/test_run_kpis.json') as kpis:
            return json.load(kpis)
    except FileNotFoundError:
        logger.info("test_run_kpis.json not found, ignoring test kpis")
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
        logger.error("Error: Cannot find JSON result file that matches running module.")
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
        logger.error("Error: Cannot find JSON result file that matches running module.")
        return None


def read_module_name(android_test_xml_file: str):
    logger.debug("Reading module name from %s" % android_test_xml_file)

    et = ET.parse(android_test_xml_file)

    module_name_option = et.find("./test/option/[@name='module-name']")
    if module_name_option is not None:
        return module_name_option.attrib["value"]

    test_module_name_option = et.find("./test/option/[@name='test-module-name']")
    if test_module_name_option is not None:
        return test_module_name_option.attrib["value"]

    raise Exception("Did not find module-name with value in %s" % android_test_xml_file)

def analyze_tests(test_root_dirs: Set[str], half_analysis_without_external_repos):
    py_files_in_dir = []
    mypy_config_file = os.path.join(os.path.dirname(__file__), "mypy_component_test_run.ini")
    env = os.environ.copy()

    for d in test_root_dirs:
        py_files_in_dir.extend([os.path.join(d, f) for f in os.listdir(d) if f.endswith(".py")])
        env["PYTHONPATH"] = env["PYTHONPATH"] + os.pathsep + d
    if len(py_files_in_dir) == 0:
        return


    env["MYPYPATH"] = env["PYTHONPATH"] + os.pathsep + os.path.expandvars("$ANDROID_BUILD_TOP/test")
    logging.info("Mypy version: %s, environment: %r",
                 check_output_logged(["mypy", "--version"]),
                 env)

    extra_args = []
    if half_analysis_without_external_repos:
        # In the commit_check stage there is no test/vts repository available so ignore missing imports
        extra_args.append("--ignore-missing-imports")

    try:
        check_output_logged(
            [
                "mypy",
                "--py2"
            ] +
            extra_args +
            ["--config-file", mypy_config_file] +
            py_files_in_dir,
            env=env)
    except CalledProcessError as cpe:
        raise Exception("mypy static analyis failed: " + os.linesep + cpe.output.decode() + os.linesep +  cpe.stderr.decode())


