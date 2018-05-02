# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import argparse
import json
import logging
import logging.config
import os
import sys
from os.path import join as pathjoin
from typing import List, Set
from handle_result.abstract_reporter import abstract_reporter
from handle_result.ci_database_reporter import ci_database_reporter
from handle_result.vcc_dashboard_reporter import vcc_dashboard_reporter
from handle_result.console_reporter import console_reporter
from shipit.testscripts import get_test_set, assemble_plan, get_component, build_testcases, run_test, detect_loose_test_cases, NamedTestResult
from utilities import ihuhandler


logger = logging.getLogger(__name__)


class tester:

    def __init__(self):
        self.reporter_list = set() #type: Set[abstract_reporter]

    def _plan_started(self):
        logger.info("Plan started")
        for r in self.reporter_list:
            r.plan_started()

    def _plan_finished(self, test_results):
        logger.info("Plan finished")
        for r in self.reporter_list:
            r.plan_finished(test_results)

    def _module_started(self, test):
        logger.info("Module started: {}".format(str(test)))
        for r in self.reporter_list:
            r.module_started(test)

    def _module_finished(self, test, test_result):
        logger.info("Module finished: {}, Result: {}".format(str(test), str(test_result)))
        for r in self.reporter_list:
            r.module_finished(test, test_result)

    def run_testcases(self, tests_to_run: List, abort_on_first_failure: bool):
        test_results = []  # type: List[NamedTestResult]
        self._plan_started()
        for t in tests_to_run:
            self._module_started(t)
            test_result = run_test(t)
            test_results.append(NamedTestResult(str(t), test_result))
            self._module_finished(t, test_result)
            if not self._check_result(test_result) and abort_on_first_failure:
                break
        self._plan_finished(test_results)
        return test_results

    def _check_result(self, test_result) -> bool:
        passed_all_tests = True
        results = test_result.json_result
        if results is not None:
            for result in results["Results"]:
                if result["Result"] != "PASS":
                    passed_all_tests = False
        return passed_all_tests

    def _set_log_config(self):
        with open(pathjoin(os.path.dirname(__file__), "logging.json"), "rt") as f:
            log_config = json.load(f)
        logging.config.dictConfig(log_config)

    def _parse_args(self):
        root_parser = argparse.ArgumentParser()
        subparsers = root_parser.add_subparsers(dest="program")
        analyze_parser = subparsers.add_parser("analyze")  # NOQA
        build_parser = subparsers.add_parser("build")
        build_parser.add_argument('--plan', nargs="+", default=[], choices=['gate', 'hourly', 'nightly', 'staging_hourly', 'staging_daily'])
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
        run_parser.add_argument('--plan', nargs="+", default=[], choices=['gate', 'hourly', 'nightly', 'staging_hourly', 'staging_daily'])
        run_parser.add_argument('--vcc_dashboard_reporting', action='store_true')
        run_parser.add_argument('--report_results_to_ci_database', action='store_true')
        run_parser.add_argument('--update_ihu', action='store_true')
        run_parser.add_argument(
            '--abort-on-first-failure', action='store_true', dest="abort_on_first_failure")
        build_parser.add_argument('--test_component', default=None,
                                help="Run without a plan and test a specified directory only")
        run_parser.add_argument('--test_component', default=None,
                                help="Run without a plan and test a specified directory only")
        return root_parser.parse_args()


    def _get_plan(self, args):
        try:
            if args.test_component:
                return get_component(args.test_component)
            else:
                plan = assemble_plan(args.plan)
                if plan:
                    return plan
                else:
                    raise ValueError('Could not get any plan')
        except ValueError as e:
            logger.error(str(e))
            sys.exit(1)

    def main(self):
        self._set_log_config()
        args = self._parse_args()

        if args.program == "analyze":
            detect_loose_test_cases()

        elif args.program == "build":
            plan = self._get_plan(args)
            build_testcases(plan)

        elif args.program == "run":
            plan = self._get_plan(args)
            capabilities = set(args.capabilities)
            if len(capabilities) == 0:
                print("ERROR: No capabilities specified, no tests will be able to run")
                sys.exit(1)

            selected_tests = get_test_set(plan, capabilities, args.only_matching)

            if len(selected_tests) == 0:
                logger.warning('No applicable tests found.')
                return

            self.reporter_list.add(console_reporter())
            if args.vcc_dashboard_reporting:
                self.reporter_list.add(vcc_dashboard_reporter())
            if args.report_results_to_ci_database:
                self.reporter_list.add(ci_database_reporter())

            if args.update_ihu:
                ihuhandler.flash_ihu()

            results = self.run_testcases(selected_tests, args.abort_on_first_failure)
            failing_testcases = [x for x in results if not x.result.passed]
            if len(failing_testcases) > 0:
                sys.exit(1)

        else:
            #root_parser.print_usage() # This wont work...
            print("Unknown parameter: {}".format(args.program))
            sys.exit(1)


if __name__ == "__main__":
    tester().main()

