# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys
import glob
import traceback
import logging
import datetime
import multiprocessing
import shlex
from typing import List, Set, Tuple
from os.path import join as pathjoin
import xml.etree.ElementTree as ET
from shipit.test_runner import vts_test_runner as vts_test_run
from shipit.test_runner import tradefed_test_runner
from shipit.test_runner import test_types
from shipit.test_runner.test_types import TestFailedException
from shipit.test_runner.test_env import vcc_root, aosp_root, run_in_lunched_env
sys.path.append(vcc_root)
import test_plan    # NOQA

logger = logging.getLogger(__name__)


_PLAN_MAP = {
    "gate" : test_plan.test_plan_gate,
    "hourly": test_plan.test_plan_hourly,
    "nightly": test_plan.test_plan_nightly,
    "incubator": test_plan.test_plan_incubator_hourly,
    "staging": test_plan.test_plan_staging_daily
}


class NamedTestResult():
    def __init__(self, name: str, result: test_types.ResultData) -> None:
        self.name = name
        self.result = result


def build_testcases(tests_to_run: List[test_types.IhuBaseTest], skip_build_vts):
    all_vts_tests = [t for t in tests_to_run if isinstance(t, test_types.VTSTest)]
    all_tradefed_tests = [t for t in tests_to_run if isinstance(t, test_types.TradefedTest)]
    logging.debug("VTS tests: %r" % all_vts_tests)
    logging.debug("Tradefed tests: %r" % all_vts_tests)

    test_modules_to_build = (
        [t.test_root_dir for t in all_vts_tests] +
        [t.test_root_dir for t in all_tradefed_tests])

    if len(all_vts_tests) > 0:
        logger.info("Found VTS test cases, building VTS")
        test_modules_to_build.append("test/vts/runners/target/gtest")
        if(skip_build_vts == "CI_FLOW"):
            print("CI flow, skip multi VTS build")
        else:
            run_in_lunched_env("make vts -j%d" % multiprocessing.cpu_count(), cwd=aosp_root)

    if len(all_tradefed_tests) > 0:
        logger.info("Found Tradefed test cases, building tradefed-all")
        if(skip_build_vts== "CI_FLOW"):
            print("CI flow, skip multi tradefed build")
        else:
            run_in_lunched_env("make tradefed-all -j%d" % multiprocessing.cpu_count(), cwd=aosp_root)

    if len(test_modules_to_build) > 0:
        print(test_modules_to_build)
        test_modules_space_separated = " ".join((shlex.quote(t) for t in test_modules_to_build))
        run_in_lunched_env("mmma -j%d %s" % (multiprocessing.cpu_count(), test_modules_space_separated), cwd=aosp_root)


def run_test(test: test_types.IhuBaseTest) -> test_types.ResultData:
    try:
        if isinstance(test, test_types.AndroidVTS):
            print(test)
            return vts_test_run.vts_tradefed_run_module(test.module_name)
        elif isinstance(test, test_types.VTSTest):
            print(test)
            return vts_test_run.vts_tradefed_run_file(pathjoin(aosp_root, test.test_root_dir))
        elif isinstance(test, test_types.TradefedTest):
            print(test)
            return tradefed_test_runner.tradefed_run(pathjoin(aosp_root, test.test_root_dir))
        elif isinstance(test, test_types.Disabled):
            if datetime.datetime.now() > test.deadline:
                raise test_types.TestFailedException("Disabled test case has passed due date: %s, JIRA: %s, Reason: %s" %
                                (test.disabled_test, test.jira_issue, test.reason))
            else:
                return test_types.ResultData(True, "DISABLED", None, None, dict(), dict())  #TODO: Introduce more values than pass/fail?

        raise Exception("Unknown test case: %s" % test)
    except test_types.VtsTestFailedException as exception:

        logger.error(str(exception.message))
        return test_types.ResultData(False, exception.message, exception.json_result, exception.json_change_time,
                                       dict(), dict())
    except test_types.TestFailedException as te:
        logger.error(str(te))
        return test_types.ResultData(False, str(te), None, None, dict(), dict())
    except Exception as e:
        logger.exception(traceback.format_exc())
        return test_types.ResultData(False, str(e), None, None, dict(), dict())


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


def get_test_set(plan, capabilities, only_matching):
    supported_tests = [t for t in plan if _is_test_supported(t, capabilities)]
    if len(only_matching) > 0:
        selected_caps = set(only_matching)

        def _is_all_selected_caps_in_required(t: test_types.IhuBaseTest):
            return selected_caps.issubset(t.require_capabilities)
        return [t for t in supported_tests if _is_all_selected_caps_in_required(t)]
    else:
        return supported_tests


def detect_loose_test_cases():
    all_plans = test_plan.test_plan_gate + \
                test_plan.test_plan_hourly + \
                test_plan.test_plan_nightly + \
                test_plan.test_plan_incubator_hourly + \
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


