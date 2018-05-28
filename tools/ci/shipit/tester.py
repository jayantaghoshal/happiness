# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import argparse
import json
import logging
import logging.config
import os
import sys
import uuid
from os.path import join as pathjoin
from typing import List, Set
from handle_result.abstract_reporter import abstract_reporter
from handle_result.ci_database_reporter import ci_database_reporter
from handle_result.vcc_dashboard_reporter import vcc_dashboard_reporter
from handle_result.console_reporter import console_reporter
from shipit.testscripts import get_test_set, assemble_plan, get_component, build_testcases, run_test, \
    detect_loose_test_cases, enforce_timeout_in_gate_tests, pre_static_analysis_on_all_testcases, \
    pre_static_analysis_on_testcases, pull_ihu_files
from shipit.test_runner.test_types import ResultData
from utilities import ihuhandler, artifact_handler
import bson
import subprocess
import shutil

class Tester:

    def __init__(self, logger=logging.getLogger(__name__)):
        self.logger = logger
        self.logger.setLevel(logging.DEBUG)
        self.reporter_list = set() #type: Set[abstract_reporter]

    def _plan_started(self):
        self.logger.debug("Plan started")
        for r in self.reporter_list:
            r.plan_started()

    def _plan_finished(self, test_results):
        self.logger.debug("Plan finished")
        for r in self.reporter_list:
            r.plan_finished(test_results)

    def _module_started(self, test, testrun_uuid: str):
        self.logger.debug("Module started: {}".format(str(test)))
        for r in self.reporter_list:
            r.module_started(test, testrun_uuid)

    def _module_finished(self, test, test_result, testrun_uuid: str):
        self.logger.debug("Module finished: {}, Result: {}".format(str(test), str(test_result)))
        for r in self.reporter_list:
            r.module_finished(test, test_result, testrun_uuid)

    def _pre_run(self):
        self.old_tombstones = self._list_tombstones()
        logging.info("old_tombstones: {}".format(self.old_tombstones))

    def _post_run(self, test_result):
        all_tombstones = self._list_tombstones()
        new_tombstones = list(set(all_tombstones) - set(self.old_tombstones))
        test_result.test_kpis['number_of_new_tombstones'] = len(new_tombstones)

        logging.info("all_tombstones: {}".format(all_tombstones))
        logging.info("new_tombstones: {}".format(new_tombstones))

        logging.info("tmp save_files_dir: {}".format(test_result.save_files_dir))

        if test_result.save_files_dir:
            # file_path is empty when there have been a exception
            if os.path.isdir(test_result.save_files_dir):
                tmp_folder = test_result.save_files_dir
                tmp_folder_on_ihu = '/data/tmp_save_files/'

                for new_tombstone in new_tombstones:
                    pull_ihu_files(os.path.join('/data/tombstones/', new_tombstone), tmp_folder, 'tombstones')

                try:
                    save_files = subprocess.check_output(['adb', 'shell', 'ls', tmp_folder_on_ihu]).decode("utf-8").split()
                    for save_file in save_files:
                        pull_ihu_files(os.path.join(tmp_folder_on_ihu, save_file), tmp_folder, save_file)
                    subprocess.check_call(['adb', 'shell', 'rm', '-rf', tmp_folder_on_ihu])
                except subprocess.CalledProcessError:
                    logging.info("Could't find any files to save")
            shutil.rmtree(tmp_folder, ignore_errors=True)
        return test_result

    def _flash_started(self):
        self.logger.debug("Flashing started")
        for r in self.reporter_list:
            r.flash_started()

    def _flash_finished(self, result):
        self.logger.debug("Flashing finished")
        for r in self.reporter_list:
            r.flash_finished(result)

    def run_testcases(self, tests_to_run: List, abort_on_first_failure: bool, max_testtime_sec: int):
        test_results = []  # type: List[ResultData]
        self._plan_started()
        for t in tests_to_run:
            test_id = str(bson.ObjectId())  #TODO: Quick hack to get valid objectid, should not have dependency to mongo bson?
            self._module_started(t, test_id)
            self._pre_run()
            test_result = run_test(t, max_testtime_sec)
            test_result = self._post_run(test_result)
            test_results.append(test_result)
            self._module_finished(t, test_result, test_id)
            if not test_result.passed and abort_on_first_failure:
                break
        self._plan_finished(test_results)
        return test_results

    def flash_ihu(self):
        self._flash_started()
        result = ihuhandler.flash_ihu()
        self._flash_finished(result)
        return result

    def _set_log_config(self, args):
        with open(pathjoin(os.path.dirname(__file__), "logging.json"), "rt") as f:
            log_config = json.load(f)
            if args.verbose:
                loglevel = 'DEBUG'
            else:
                loglevel = 'INFO'
            #if args.quiet:
            #    loglevel = 'WARNING'
            log_config['handlers']['console']['level'] = loglevel
            filename = 'tester.log'
            i = 0
            while os.path.isfile(filename):
                i += 1
                filename = 'tester_{}.log'.format(i)
            log_config['handlers']['file_handler']['filename'] = filename
            log_config['handlers']['file_handler']['mode'] = 'w'
            logging.config.dictConfig(log_config)

    def _parse_args(self):
        root_parser = argparse.ArgumentParser()
        subparsers = root_parser.add_subparsers(dest="program")
        analyze_parser = subparsers.add_parser("analyze")  # NOQA
        build_parser = subparsers.add_parser("build")
        analyze_testplan_parser = subparsers.add_parser("analyze_testplan")  # NOQA
        print_parser = subparsers.add_parser("print_plan")
        run_parser = subparsers.add_parser("run")

        for common_args in [analyze_parser, build_parser, run_parser, print_parser, analyze_testplan_parser]:
            common_args.add_argument('--verbose', action='store_true')

        for parser_dependent_on_plan in [print_parser, run_parser, build_parser, analyze_testplan_parser]:
            parser_dependent_on_plan.add_argument('--plan', nargs="+", default=[],
                                      choices=['gate', 'hourly', 'nightly', 'staging_hourly', 'staging_daily'])
            parser_dependent_on_plan.add_argument('--test_component', default=None,
                                    help="Run without a plan and test a specified directory only")

        for parser_dependent_on_capabilities in [print_parser, run_parser]:
            parser_dependent_on_capabilities.add_argument("-c", "--capabilities",
                            type=str,
                            nargs="*",
                            default=[],
                            help="The capabilities your rig has access to, select from shipit.test_types.Capabilites")
            parser_dependent_on_capabilities.add_argument("-o", "--only_matching",
                                type=str,
                                nargs="*",
                                default=[],
                                help="Only pick tests requiring the specified capabilities. "
                                     "Ignore other tests even though your rig has capabilities enough to run them. "
                                     "This flag is intended to be used to optimize the use of specialized rigs so that they "
                                     "dont run generic test cases")

        run_parser.add_argument('--vcc_dashboard_reporting', action='store_true')
        run_parser.add_argument('--report_results_to_ci_database', action='store_true')
        run_parser.add_argument('--update_ihu', action='store_true')
        run_parser.add_argument('--download', default=None)
        run_parser.add_argument('--download_latest', default=None)
        run_parser.add_argument(
            '--abort-on-first-failure', action='store_true', dest="abort_on_first_failure")

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
            self.logger.error(str(e))
            sys.exit(1)

    def _list_tombstones(self):
        return subprocess.check_output(['adb', 'shell', 'ls', '/data/tombstones/']).decode("utf-8").split()


    def main(self):
        args = self._parse_args()
        self._set_log_config(args)

        if args.program == "analyze_testplan":
            # Special case not used for normal "analyze" to avoid circular dependencies between repositories
            detect_loose_test_cases()

        if args.program == "analyze":
            pre_static_analysis_on_all_testcases()
            enforce_timeout_in_gate_tests()

        elif args.program == "build":
            plan = self._get_plan(args)
            build_testcases(plan)

        elif args.program == "print_plan":
            print(args.capabilities)
            print(args.only_matching)
            plan = self._get_plan(args)
            selected_tests = get_test_set(plan, set(args.capabilities), set(args.only_matching))
            for t in selected_tests:
                print(t)

        elif args.program == "run":
            plan = self._get_plan(args)
            capabilities = set(args.capabilities)
            if len(capabilities) == 0:
                print("ERROR: No capabilities specified, no tests will be able to run")
                sys.exit(1)

            selected_tests = get_test_set(plan, capabilities, set(args.only_matching))

            if len(selected_tests) == 0:
                self.logger.warning('No applicable tests found.')
                return

            self.reporter_list.add(console_reporter())
            if args.vcc_dashboard_reporting:
                self.reporter_list.add(vcc_dashboard_reporter())
            if args.report_results_to_ci_database:
                self.reporter_list.add(ci_database_reporter())

            if args.download:
                artifact_handler.download_out(args.download)
            elif args.download_latest:
                artifact_handler.download_latest(args.download_latest)

            if args.update_ihu:
                result = self.flash_ihu()
                if not result.success:
                    self.logger.error('IHU update failed')
                    sys.exit(1)

            max_testtime_sec = 60 * 60
            if args.plan == "gate":
                max_testtime_sec = 10 * 60

            if args.test_component:
                pre_static_analysis_on_testcases(plan, half_analysis_without_external_repos=False)

            results = self.run_testcases(selected_tests, args.abort_on_first_failure, max_testtime_sec)
            failing_testcases = [x for x in results if not x.passed]
            if len(failing_testcases) > 0:
                logger.error('Test case failed')
                sys.exit(1)

        else:
            self.logger.error("Unknown parameter: {}".format(args.program))
            sys.exit(1)


if __name__ == "__main__":
    logger = logging.getLogger("Tester")
    Tester(logger).main()

