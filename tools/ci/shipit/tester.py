# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import argparse
import datetime
import glob
import json
import logging
import logging.config
import os
import shlex
import shutil
import sys
import traceback
import multiprocessing
import xml.etree.ElementTree as ET

from os.path import join as pathjoin
from typing import List, Set, Tuple

import shipit.test_runner.test_types
from shipit.test_runner import vts_test_runner as vts_test_run
from shipit.test_runner import tradefed_test_runner
from shipit.test_runner.test_types import AndroidVTS, VTSTest, TradefedTest, IhuBaseTest, Disabled, ResultData
from shipit.test_runner import test_types
from shipit.test_runner.test_types import TestFailedException
from shipit.test_runner.test_env import vcc_root, aosp_root, run_in_lunched_env
from handle_result import store_result, test_visualisation
from shipit.process_tools import check_output_logged
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
    def __init__(self, name: str, result: ResultData) -> None:
        self.name = name
        self.result = result

def is_test_supported(test: IhuBaseTest, machine_capabilities: Set[str]):
    missing_capabilities = test.require_capabilities - machine_capabilities
    if len(missing_capabilities) > 0:
        logger.debug("Skipping test %s because following capabilities missing: %r" % (test, missing_capabilities))
    return len(missing_capabilities) == 0


def run_test(test: IhuBaseTest) -> ResultData:
    try:
        if isinstance(test, AndroidVTS):
            print(test)
            return vts_test_run.vts_tradefed_run_module(test.module_name)
        elif isinstance(test, VTSTest):
            print(test)
            return vts_test_run.vts_tradefed_run_file(pathjoin(aosp_root, test.test_root_dir))
        elif isinstance(test, TradefedTest):
            print(test)
            return tradefed_test_runner.tradefed_run(pathjoin(aosp_root, test.test_root_dir))
        elif isinstance(test, shipit.test_runner.test_types.Disabled):
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


def check_result(test_results: List[NamedTestResult], abort_on_first_failure: bool) -> None:
    results = test_results[-1].result.json_result

    if results is not None:
        print("Test name: " + test_results[-1].name)
        print("Test class: " + results["Results"][0]["Test Class"])
        for result in results["Results"]:
            print("\tTest name: " + result["Test Name"])
            print("\tResult: " + result["Result"])

        for result in results["Results"]:
            if result["Result"] != "PASS":
                print("Details: " + result["Details"])
                print("Test failed! The result from " + result["Test Class"] + ", " + result["Test Name"] + " is " + result["Result"])
                if abort_on_first_failure:
                    print_test_summary(test_results)
                    sys.exit(1)

        print("Number of executed tests in JSON file: " + str(results["Summary"]["Executed"]))
    else:
        print("The current test case %s does not generate a JSON file" % test_results[-1].name)


def build_testcases(tests_to_run: List[IhuBaseTest], skip_build_vts):
    all_vts_tests = [t for t in tests_to_run if isinstance(t, VTSTest)]
    all_tradefed_tests = [t for t in tests_to_run if isinstance(t, TradefedTest)]
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


def print_indented(s: str, indent="    "):
    lines = s.split("\n")
    for l in lines:
        print(indent + l)


def dump_test_results_to_json(test_results: List[NamedTestResult]):
    def res(r: NamedTestResult):
        return {
            "name": r.name,
            "pass": r.result.passed,
            "kpis:" : r.result.test_kpis
        }
    to_dump = {
        "results" : [res(r) for r in test_results]
    }

    out = os.environ["ANDROID_HOST_OUT"]
    result_dir = os.path.join(out, "vcc_test_results", "current")
    if os.path.isdir(result_dir):
        shutil.rmtree(result_dir)   # TODO: Copy old results into some circular buffer
    print("\n\n")
    print("##################################################################################")
    print("##")
    print("## Dumping test results to %s" % result_dir)
    print("##")
    print()
    os.makedirs(result_dir, exist_ok=True)
    for r in test_results:
        clean_name = "tc_" + r.name.replace("/", "") + "_stdout"
        with open(os.path.join(result_dir, clean_name), "w", encoding="utf-8") as f:
            f.write(r.result.console)

        for log_type_name, value in r.result.logs.items():
            clean_name = "tc_" + r.name.replace("/", "") + "_" + log_type_name
            with open(os.path.join(result_dir, clean_name), "w", encoding="utf-8") as f:
                f.write(value)


    with open(os.path.join(result_dir, "test_result.json"), "w", encoding="utf-8") as f:
        json.dump(to_dump, f)



def print_test_summary(test_results: List[NamedTestResult]):
    print("*** VTS Python Test summary ***")
    for test_result in test_results:
        results = test_result.result.json_result
        if results is not None:
            print("****************************************************")
            print("Test name: " +test_result.name)
            print("Test class: " + results["Results"][0]["Test Class"])

            print("\tError: " + str(results["Summary"]["Error"]))
            print("\tExecuted: " + str(results["Summary"]["Executed"]))
            print("\tFailed: " + str(results["Summary"]["Failed"]))
            print("\tPassed: " + str(results["Summary"]["Passed"]))
            print("\tRequested: " + str(results["Summary"]["Requested"]))
            print("\tSkipped: " + str(results["Summary"]["Skipped"]))
            print("\tJson creation time: " + str(test_result.result.json_change_time))
            print("****************************************************")
            print("")


def flash_ihu(max_attempts=3, power_cycle_length=120):
    logger.info("Updating ihu")
    for attempt in range(max_attempts):
        try:
            check_output_logged(['ihu_update', '--profile=ci-machinery'])
            logger.info("Updating ihu complete")
            break
        except Exception as e:
            logger.error("Updating ihu failed: {}".format(e))
            if attempt == max_attempts - 1: # The last attempt failed
                raise Exception("Updating ihu failed, reached max attempts: {}".format(max_attempts))
            else:
                logger.info("Power cycling ihu and retry")
                boot_script_path = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, 'jenkins', 'ihu_ipm_reboot.py'))
                power_result = os.system("python3 " + boot_script_path + " " + str(power_cycle_length))
                if power_result != 0:
                    raise Exception("Could not power cycle ihu")
    logger.info("Update of ihu done")


def run_testcases(tests_to_run: List[IhuBaseTest], vcc_dashboard_reporting: bool, report_results_to_ci_database: bool, abort_on_first_failure: bool):
    test_results = []  # type: List[NamedTestResult]
    if vcc_dashboard_reporting:  # initialise visualizing Testcases in VCC CI
        try:
            if "UPSTREAM_JOB_GIT_REVISION" in os.environ and os.environ["UPSTREAM_JOB_GIT_REVISION"]:
                change_id = test_visualisation.redis_con.get(
                    "icup_android.gerrit.commit_id." + os.environ["UPSTREAM_JOB_GIT_REVISION"] + ".change_id")
            elif "ZUUL_CHANGE" in os.environ and os.environ["ZUUL_CHANGE"]:
                print("ZUUL_CHANGE:" + os.environ["ZUUL_CHANGE"])
                change_id = ((test_visualisation.redis_con.get(
                    "icup_android.gerrit.change_number." + os.environ["ZUUL_CHANGE"] + ".change_id")))
            vccciproxy = test_visualisation.VCCCIProxy(
                change_id.decode("utf-8"))
        except Exception as e:
                print(traceback.format_exc())
                print(e)
                print("Initialization of VCC CI failed")
    for t in tests_to_run:
        if vcc_dashboard_reporting:
            try:
                vccciproxy.testcase_started(store_result.get_module_name(t))
            except Exception as e:
                print(traceback.format_exc())
                print(e)
                print("Testcase started message to VCC CI failed")
        if report_results_to_ci_database:
            try:
                store_result.clean_old_results()
            except Exception as e:
                print(traceback.format_exc())
                print(e)
                print("Cleaning old results failed")
                report_results_to_ci_database = False  # It will prohibit running rest of mongodb operation
        test_result = run_test(t)
        if vcc_dashboard_reporting and not isinstance(t, Disabled):
            try:
                vccciproxy.testcase_finished(store_result.get_module_name(
                    t), store_result.get_result(test_result))
            except Exception as e:
                print(traceback.format_exc())
                print(e)
                print("Testcase finished message to VCC CI failed")
        if report_results_to_ci_database and not isinstance(t, Disabled):
            try:
                store_result.load_test_results(t, test_result)
            except Exception as e:
                print(traceback.format_exc())
                print(e)
                print("Storing results to mongodb failed")
        test_results.append(NamedTestResult(str(t), test_result))
        check_result(test_results, abort_on_first_failure)


    try:
        dump_test_results_to_json(test_results)
    except Exception as e:
        logger.error("Failed to write json test report %r", e, exc_info=True)
        # Keep going...

    print_test_summary(test_results)
    print("All tests completed")
    failing_testcases = [x for x in test_results if not x.result.passed]
    if len(failing_testcases) > 0:
        print("#####################################################################")
        print("Failing test cases:")
        for ntr in failing_testcases:
            print(" * %s" % ntr.name)
            print_indented(ntr.result.console, "      >> ")
        print("#####################################################################")
        sys.exit(1)
    else:
        print("All %d tests successful" % len(test_results))



def main():
    with open(pathjoin(os.path.dirname(__file__), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    root_parser = argparse.ArgumentParser()
    subparsers = root_parser.add_subparsers(dest="program")
    analyze_parser = subparsers.add_parser("analyze")  # NOQA
    build_parser = subparsers.add_parser("build")
    build_parser.add_argument('--plan', nargs="+", default=[], choices=['gate', 'hourly', 'nightly', 'staging', 'incubator'])
    build_parser.add_argument('--ciflow', choices=['true', 'false'])
    run_parser = subparsers.add_parser("run")
    run_parser.add_argument("-c", "--capabilities",
                        type=str,
                        nargs="*",
                        default=[],
                        help="The capabilities your rig has access to, select from shipit.test_types.Capabilites")
    run_parser.add_argument("-o", "--only_matching",
                            type=str,
                            nargs="*",
                            default=[],
                            help="Only pick tests requiring the specified capabilities. "
                                 "Ignore other tests even though your rig has capabilities enough to run them. "
                                 "This flag is intended to be used to optimize the use of specialized rigs so that they "
                                 "dont run generic test cases")
    run_parser.add_argument('--plan', nargs="+", default=[], choices=['gate', 'hourly', 'nightly', 'staging', 'incubator'])
    run_parser.add_argument('--vcc_dashboard_reporting', action='store_true')
    run_parser.add_argument('--report_results_to_ci_database', action='store_true')
    run_parser.add_argument('--update_ihu', action='store_true')
    run_parser.add_argument(
        '--abort-on-first-failure', action='store_true', dest="abort_on_first_failure")
    build_parser.add_argument('--test_component', default=None,
                            help="Run without a plan and test a specified directory only")
    run_parser.add_argument('--test_component', default=None,
                            help="Run without a plan and test a specified directory only")
    args = root_parser.parse_args()



    def assemble_plan():
        if len(args.plan) == 0:
            if args.test_component:
                androidtest_xml_path = pathjoin(args.test_component, "AndroidTest.xml")
                if not os.path.isfile(androidtest_xml_path):
                    print("ERROR: AndroidTest.xml not found")
                    sys.exit(1)

                try:
                    elem = ET.parse('AndroidTest.xml').getroot().find("test")
                    value = elem.get('class').split('.')[-1]
                except Exception as e:
                    print("Unable to parse AndroidTest.xml")
                    print("Error message: " + str(e))
                    sys.exit(1)

                if value == "VtsMultiDeviceTest" or value == "GTest":
                    return [VTSTest(os.path.relpath(args.test_component, aosp_root), set())]
                else:
                    return [TradefedTest(os.path.relpath(args.test_component, aosp_root), set())]
            else:
                run_parser.print_usage()
                sys.exit(1)
        else:
            complete_plan = []  # type: List[IhuBaseTest]
            for p in args.plan:
                complete_plan.extend(_PLAN_MAP[p])
            return complete_plan


    def get_option():
        if args.ciflow == "true":
            return "CI_FLOW"
        elif args.ciflow == "false":
            return "BUILD"

    if args.program == "analyze":
        detect_loose_test_cases()
    elif args.program == "build":
        plan = assemble_plan()
        skip_build_vts = get_option()
        build_testcases(plan, skip_build_vts)
    elif args.program == "run":
        plan = assemble_plan()
        vcc_dashboard_reporting = args.vcc_dashboard_reporting
        report_results_to_ci_database = args.report_results_to_ci_database
        capabilities = set(args.capabilities)
        if len(args.capabilities) == 0:
            print("ERROR: No capabilities specified, no tests will be able to run")
            run_parser.print_usage()
            sys.exit(1)

        supported_tests = [t for t in plan if is_test_supported(t, capabilities)]
        if len(args.only_matching) > 0:
            selected_caps = set(args.only_matching)

            def is_all_selected_caps_in_required(t: IhuBaseTest):
                return selected_caps.issubset(t.require_capabilities)
            selected_tests = [t for t in supported_tests if is_all_selected_caps_in_required(t)]
        else:
            selected_tests = supported_tests

        if len(selected_tests) == 0:
            logger.info('No applicable tests found.')
            return

        if args.update_ihu:
            flash_ihu()

        run_testcases(selected_tests, vcc_dashboard_reporting,
                      report_results_to_ci_database, args.abort_on_first_failure)
    else:
        root_parser.print_usage()
        sys.exit(1)


def detect_loose_test_cases():
    all_plans = test_plan.test_plan_gate + \
                test_plan.test_plan_hourly + \
                test_plan.test_plan_nightly + \
                test_plan.test_plan_incubator_hourly + \
                test_plan.test_plan_staging_daily

    disabled_subtests = [d.disabled_test for d in all_plans if isinstance(d, test_types.Disabled)]
    all_tests_including_disabled = all_plans + disabled_subtests
    all_testdirs_in_plans = {d.test_root_dir for d in all_tests_including_disabled if not (isinstance(d, Disabled) or isinstance(d, AndroidVTS))}

    android_xmls = glob.glob(vcc_root + "/**/AndroidTest.xml", recursive=True)
    directories_with_androidtestxml = {os.path.relpath(os.path.dirname(p), aosp_root) for p in android_xmls}

    androidtestxmls_not_in_any_plan = directories_with_androidtestxml - all_testdirs_in_plans
    if len(androidtestxmls_not_in_any_plan) > 0:
        print("#############################################################################################")
        print("# ERROR: Found AndroidTest.xml not present in vendor/volvocars/test_plan.py")
        print("#  All test cases must be included in the test_plan, or explicitly disabled in test_plan.")
        print("#  Please add your missing test into the test_plan")
        print("#  ")
        print("#  Files missing from test_plan.py:")
        for a in androidtestxmls_not_in_any_plan:
            print("#    - %s" % a)
        print("#####################################################################git ########################")
        sys.exit(1)

    tests_in_plan_but_not_on_disk = all_testdirs_in_plans - directories_with_androidtestxml
    if len(tests_in_plan_but_not_on_disk) > 0:
        for a in tests_in_plan_but_not_on_disk:
            print("ERROR: vendor/volvocars/test_plan.py contains test not found in repo. Path: %s" % a)
        sys.exit(1)

if __name__ == "__main__":
    main()

