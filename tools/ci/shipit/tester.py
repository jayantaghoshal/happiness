import argparse
import datetime
import glob
import json
import logging
import logging.config
import os
import shlex
import sys
import traceback
from os.path import join as pathjoin
from typing import List, Set, Tuple

import shipit.test_runner.test_types
from shipit.test_runner import vts_test_runner as vts_test_run
from shipit.test_runner import tradefed_test_runner
from shipit.test_runner.test_types import VTSTest, TradefedTest, IhuBaseTest
from shipit.test_runner import test_types
from shipit.test_runner.test_env import vcc_root, aosp_root, run_in_lunched_env

sys.path.append(vcc_root)
import test_plan    # NOQA

logger = logging.getLogger(__name__)


def is_test_supported(test: IhuBaseTest, machine_capabilities: Set[str]):
    missing_capabilities = test.require_capabilities - machine_capabilities
    if len(missing_capabilities) > 0:
        logger.debug("Skipping test %s because following capabilities missing: %r" % (test, missing_capabilities))
    return len(missing_capabilities) == 0


def run_test(test: IhuBaseTest):
    if isinstance(test, VTSTest):
        print(test)
        module_name = vts_test_run.read_module_name(pathjoin(aosp_root, test.test_xml_path, "AndroidTest.xml"))
        vts_test_run.vts_tradefed_run_module(module_name)
    elif isinstance(test, TradefedTest):
        print(test)
        tradefed_test_runner.tradefed_run(pathjoin(aosp_root, test.test_root_dir))
    elif isinstance(test, shipit.test_runner.test_types.Disabled):
        if datetime.datetime.now() > test.deadline:
            raise test_types.TestFailedException("Disabled test case has passed due date: %s, JIRA: %s, Reason: %s" %
                            (test.disabled_test, test.jira_issue, test.reason))
    else:
        raise Exception("Unknown test case: %s" % test)


def build_testcases(tests_to_run: List[IhuBaseTest]):
    all_vts_tests = [t for t in tests_to_run if isinstance(t, VTSTest)]
    all_tradefed_tests = [t for t in tests_to_run if isinstance(t, TradefedTest)]
    logging.debug("VTS tests: %r" % all_vts_tests)
    logging.debug("Tradefed tests: %r" % all_vts_tests)

    test_modules_to_build = (
        [t.test_xml_path for t in all_vts_tests] +
        [t.test_root_dir for t in all_tradefed_tests])

    if len(all_vts_tests) > 0:
        logger.info("Found VTS test cases, building VTS")
        test_modules_to_build.append("test/vts/runners/target/gtest")
        # TODO: Increase -j flag on build server
        run_in_lunched_env("make vts -j7", cwd=aosp_root)

    if len(all_tradefed_tests) > 0:
        logger.info("Found Tradefed test cases, building tradefed-all")
        # TODO: Increase -j flag on build server
        run_in_lunched_env("make tradefed-all -j7", cwd=aosp_root)

    if len(test_modules_to_build) > 0:
        print(test_modules_to_build)
        test_modules_space_separated = " ".join((shlex.quote(t) for t in test_modules_to_build))
        #TODO: Increase -j flag on build server
        run_in_lunched_env("mmma -j7 %s" % test_modules_space_separated, cwd=aosp_root)

def print_indented(s: str, indent="    "):
    lines = s.split("\n")
    for l in lines:
        print(indent + l)

def run_testcases(tests_to_run: List[IhuBaseTest]):
    failing_testcases = []  # type: List[Tuple[IhuBaseTest, str]]
    for t in tests_to_run:
        try:
            run_test(t)
        except test_types.TestFailedException as te:
            failing_testcases.append((t, str(te)))
            logger.error(str(te))
        except Exception as e:
            failing_testcases.append((t, str(e)))
            logger.exception(traceback.format_exc())

    print("All tests completed")
    if len(failing_testcases) > 0:
        print("#####################################################################")
        print("Failing test cases:")
        for (test, error) in failing_testcases:
            print(" * %s" % test)
            print_indented(error, "      >> ")
        print("#####################################################################")
        sys.exit(1)


def main():
    with open(pathjoin(os.path.dirname(__file__), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    root_parser = argparse.ArgumentParser()
    subparsers = root_parser.add_subparsers(dest="program")
    analyze_parser = subparsers.add_parser("analyze")  # NOQA
    build_parser = subparsers.add_parser("build")
    build_parser.add_argument('--plan', choices=['gate', 'hourly', 'nightly'])
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
    run_parser.add_argument('--plan', choices=['gate', 'hourly', 'nightly'])
    build_parser.add_argument('--test_component', default=None,
                            help="Run without a plan and test a specified directory only")
    run_parser.add_argument('--test_component', default=None,
                            help="Run without a plan and test a specified directory only")
    args = root_parser.parse_args()

    def get_plan():
        if args.plan == "gate":
            return test_plan.test_plan_gate
        elif args.plan == "hourly":
            return test_plan.test_plan_hourly
        elif args.plan == "nightly":
            return test_plan.test_plan_nightly
        else:
            if args.test_component:
                androidtest_xml_path = pathjoin(args.test_component, "AndroidTest.xml")
                if os.path.isfile(androidtest_xml_path):
                    return [VTSTest(os.path.relpath(args.test_component, aosp_root), set())]
                else:
                    return [TradefedTest(os.path.relpath(args.test_component, aosp_root), set())]
            else:
                run_parser.print_usage()
                sys.exit(1)

    if args.program == "analyze":
        detect_loose_test_cases()
    elif args.program == "build":
        plan = get_plan()
        build_testcases(plan)
    elif args.program == "run":
        plan = get_plan()
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
        run_testcases(selected_tests)
    else:
        root_parser.print_usage()
        sys.exit(1)


def detect_loose_test_cases():
    all_plans = test_plan.test_plan_gate + \
                test_plan.test_plan_hourly + \
                test_plan.test_plan_nightly

    disabled_subtests = [d.disabled_test for d in all_plans if isinstance(d, test_types.Disabled)]
    all_tests_including_disabled = all_plans + disabled_subtests
    all_testdirs_in_plans = {d.test_xml_path for d in all_tests_including_disabled if isinstance(d, VTSTest)}

    android_xmls = glob.glob(vcc_root + "/**/AndroidTest.xml", recursive=True)
    directories_with_androidtestxml = {os.path.relpath(os.path.dirname(p), aosp_root) for p in android_xmls}

    androidtestxmls_not_in_any_plan = directories_with_androidtestxml - all_testdirs_in_plans
    if len(androidtestxmls_not_in_any_plan) > 0:
        print("#############################################################################################")
        print("# ERROR: Found AndroidTest.xml not present in vendor/volvocars/test_plan.py")
        print("#  All VTS test cases must be included in the test_plan, or explicitly disabled in test_plan.")
        print("#  Please add your missing test into the test_plan")
        print("#  ")
        print("#  Files missing from test_plan.py:")
        for a in androidtestxmls_not_in_any_plan:
            print("#    - %s" % a)
        print("#############################################################################################")
        sys.exit(1)

    tests_in_plan_but_not_on_disk = all_testdirs_in_plans - directories_with_androidtestxml
    if len(tests_in_plan_but_not_on_disk) > 0:
        for a in tests_in_plan_but_not_on_disk:
            print("ERROR: vendor/volvocars/test_plan.py contains test not found in repo. Path: %s" % a)
        sys.exit(1)



if __name__ == "__main__":
    main()