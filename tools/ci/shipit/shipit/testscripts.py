# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys
import glob
import traceback
import logging
import datetime
import shlex
from typing import List, Set
from os.path import join as pathjoin
import xml.etree.ElementTree as ET
from shipit.test_runner import vts_test_runner, tradefed_test_runner, test_types
from shipit.test_runner.test_env import vcc_root, aosp_root, run_in_lunched_env
import subprocess
import re
sys.path.append(vcc_root)
import test_plan    # NOQA

logger = logging.getLogger(__name__)


_PLAN_MAP = {
    "gate" : test_plan.test_plan_gate,
    "hourly": test_plan.test_plan_hourly,
    "nightly": test_plan.test_plan_nightly,
    "staging_hourly": test_plan.test_plan_staging_hourly,
    "staging_daily": test_plan.test_plan_staging_daily
}


'''class NamedTestResult():
    def __init__(self, name: str, result: test_types.ResultData) -> None:
        self.name = name
        self.result = result'''


def build_testcases(tests_to_run: List[test_types.IhuBaseTest]):
    all_vts_tests = [t for t in tests_to_run if isinstance(t, test_types.VTSTest)]
    all_tradefed_tests = [t for t in tests_to_run if isinstance(t, test_types.TradefedTest)]
    logger.debug("VTS tests: %r" % all_vts_tests)
    logger.debug("Tradefed tests: %r" % all_vts_tests)

    test_modules_to_build = (
        [t.test_root_dir for t in all_vts_tests] +
        [t.test_root_dir for t in all_tradefed_tests])

    if len(test_modules_to_build) > 0:
        logger.debug(str(test_modules_to_build))
        test_modules_space_separated = " ".join((shlex.quote(t) for t in test_modules_to_build))
        try:
            #NOTE: --verbose is important for this command to work in CI, see https://partnerissuetracker.corp.google.com/u/1/issues/79129588
            run_in_lunched_env(("atest " +
                                "--rebuild-module-info "
                                "--build " +
                                "--install " +
                                "--verbose " +
                                test_modules_space_separated), cwd=aosp_root)
        except subprocess.CalledProcessError as cpe:
            logger.error(cpe.output.decode())
            logger.error(cpe.stderr.decode())
            logger.error("%s Failed to build test cases" % (os.linesep * 3))
            sys.exit(-1)


def run_test(test: test_types.IhuBaseTest, max_testtime_sec: int) -> test_types.ResultData:
    try:
        if isinstance(test, test_types.AndroidVTS):
            logger.debug(str(test))
            return vts_test_runner.vts_tradefed_run_module(test.module_name, None, max_test_time_sec=max_testtime_sec)
        elif isinstance(test, test_types.VTSTest):
            logger.debug(str(test))
            return vts_test_runner.vts_tradefed_run_file(pathjoin(aosp_root, test.test_root_dir), test.tests_to_run, max_testtime_sec)
        elif isinstance(test, test_types.TradefedTest):
            logger.debug(str(test))
            return tradefed_test_runner.tradefed_run(pathjoin(aosp_root, test.test_root_dir), max_testtime_sec)
        elif isinstance(test, test_types.Disabled):
            if datetime.datetime.now() > test.deadline:
                raise test_types.TestFailedException("Disabled test case has passed due date: %s, JIRA: %s, Reason: %s" %
                                (test.disabled_test, test.jira_issue, test.reason))
            else:
                return test_types.ResultData(str(test), True, [], "DISABLED", "DISABLED", dict(), dict(), [])  #TODO: Introduce more values than pass/fail?

        raise Exception("Unknown test case: %s" % test)
    except test_types.VtsTestFailedException as exception:

        logger.error(str(exception.message))
        return test_types.ResultData(str(test), False, [], exception.message, exception.message, dict(), dict(), list())
    except test_types.TestFailedException as te:
        logger.error(str(te))
        return test_types.ResultData(str(test), False, [], str(te), str(te), dict(), dict(), [])
    except Exception as e:
        logger.exception(traceback.format_exc())
        return test_types.ResultData(str(test), False, [], str(e), str(e), dict(), dict(), [])


def assemble_plan(plan):
    if len(plan) == 0:
        return None
    else:
        complete_plan = []  # type: List[test_types.IhuBaseTest]
        for p in plan:
            complete_plan.extend(_PLAN_MAP[p])
        return complete_plan


def get_component(test_component):
    androidtest_xml_path = pathjoin(test_component, "AndroidTest.xml")
    if not os.path.isfile(androidtest_xml_path):
        logger.error("AndroidTest.xml not found")
        raise Exception("AndroidTest.xml not found")
    try:
        elem = ET.parse('AndroidTest.xml').getroot().find("test")
        value = elem.get('class').split('.')[-1]
    except Exception as e:
        logger.error("Unable to parse AndroidTest.xml, Error message: " + str(e))
        raise ValueError("Unable to parse AndroidTest.xml, Error message: " + str(e))
    if value == "VtsMultiDeviceTest" or value == "GTest":
        return [test_types.VTSTest(os.path.relpath(test_component, aosp_root), set())]
    else:
        return [test_types.TradefedTest(os.path.relpath(test_component, aosp_root), set())]


def _is_test_supported(test: test_types.IhuBaseTest, machine_capabilities: Set[str]):
    missing_capabilities = test.require_capabilities - machine_capabilities
    if len(missing_capabilities) > 0:
        logger.debug("Skipping test %s because following capabilities missing: %r" % (test, missing_capabilities))
    return len(missing_capabilities) == 0


def get_test_set(plan, supported_capabilities: Set[str], ignore_tests_not_requiring_these_capabilities: Set[str]):
    supported_tests = [t for t in plan if _is_test_supported(t, supported_capabilities)]
    if len(ignore_tests_not_requiring_these_capabilities) > 0:
        # To avoid greedy running of all tests a node supports it's possible to filter out capabilities that must be
        # required by the test. This saves run time on nodes which support special capabilities but also generic ones.
        def _is_all_selected_caps_in_required(t: test_types.IhuBaseTest):
            return len(ignore_tests_not_requiring_these_capabilities.intersection(t.require_capabilities)) > 0
        return [t for t in supported_tests if _is_all_selected_caps_in_required(t)]
    else:
        return supported_tests


def pre_static_analysis_on_testcases(testcases: List[test_types.IhuBaseTest], half_analysis_without_external_repos: bool):
    vts_tests = [d for d in testcases if isinstance(d, test_types.VTSTest)]
    vts_test_dirs = [os.path.join(aosp_root, d.test_root_dir) for d in vts_tests]
    vts_test_runner.analyze_tests(set(vts_test_dirs), half_analysis_without_external_repos)


def pre_static_analysis_on_all_testcases():
    all_plans = test_plan.test_plan_gate + \
                test_plan.test_plan_hourly + \
                test_plan.test_plan_nightly + \
                test_plan.test_plan_staging_hourly + \
                test_plan.test_plan_staging_daily
    pre_static_analysis_on_testcases(all_plans, half_analysis_without_external_repos=True)


def detect_loose_test_cases():
    all_plans = test_plan.test_plan_gate + \
                test_plan.test_plan_hourly + \
                test_plan.test_plan_nightly + \
                test_plan.test_plan_staging_hourly + \
                test_plan.test_plan_staging_daily

    disabled_subtests = [d.disabled_test for d in all_plans if isinstance(d, test_types.Disabled)]
    all_tests_including_disabled = all_plans + disabled_subtests
    all_testdirs_in_plans = {d.test_root_dir for d in all_tests_including_disabled if not (isinstance(d, test_types.Disabled) or isinstance(d, test_types.AndroidVTS))}

    android_xmls = glob.glob(vcc_root + "/**/AndroidTest.xml", recursive=True)
    directories_with_androidtestxml = {os.path.relpath(os.path.dirname(p), aosp_root) for p in android_xmls}

    androidtestxmls_not_in_any_plan = directories_with_androidtestxml - all_testdirs_in_plans
    if len(androidtestxmls_not_in_any_plan) > 0:
        logger.error("Found AndroidTest.xml not present in vendor/volvocars/test_plan.py")
        for a in androidtestxmls_not_in_any_plan:
            logger.error(str(a))
        raise Exception("Found AndroidTest.xml not present in vendor/volvocars/test_plan.py")

    tests_in_plan_but_not_on_disk = all_testdirs_in_plans - directories_with_androidtestxml
    if len(tests_in_plan_but_not_on_disk) > 0:
        for a in tests_in_plan_but_not_on_disk:
            logger.error("vendor/volvocars/test_plan.py contains test not found in repo. Path: %s" % a)
        raise Exception("vendor/volvocars/test_plan.py contains test not found in repo")


def enforce_timeout_in_gate_tests():
    def parse_timeout_seconds(t: str) -> int:
        time, unit = re.match(r"(\d+)\s*(\S+)", t).groups()
        seconds_per_unit = {
            "s": 1,
            "m": 60,
            "h": 3600
        }
        return int(time) * seconds_per_unit[unit]

    LIMIT_SEC = 7 * 60

    errors = []
    all_testdirs_in_plans = {d.test_root_dir for d in test_plan.test_plan_gate if
                             not (isinstance(d, test_types.Disabled) or isinstance(d, test_types.AndroidVTS))}
    for testdir in all_testdirs_in_plans:
        android_xml_path = pathjoin(testdir, "AndroidTest.xml")
        root = ET.parse(pathjoin(aosp_root, android_xml_path)).getroot()
        test_element = root.find("./test")
        test_type = test_element.attrib["class"].split(".")[-1]
        if test_type == "VtsMultiDeviceTest":
            timeout_element = root.find("./test/option/[@name='test-timeout']")
            if timeout_element is None:
                errors.append("%s is missing option test-timeout" % android_xml_path)
                continue
            timeout_sec = parse_timeout_seconds(timeout_element.attrib["value"])
        elif test_type == "GTest":
            timeout_element = root.find("./test/option/[@name='native-test-timeout']")
            if timeout_element is None:
                errors.append("%s is missing option native-test-timeout" % android_xml_path)
                continue
            timeout_sec = int(float(timeout_element.attrib["value"]) / 1000.0)
        else:
            raise Exception("Unknown test type in gate test, can not parse timeout.")

        if timeout_sec > LIMIT_SEC:
            errors.append("%s has a timeout of %d sec, maximum allowed test time in commit gate is %ds" %
                          (android_xml_path, timeout_sec, LIMIT_SEC))

    if len(errors) > 0:
        raise Exception("Test cases in commit gate test plan must adhere to timeout constraints." + os.linesep + "* " +
                        (os.linesep + "* ").join(errors))



