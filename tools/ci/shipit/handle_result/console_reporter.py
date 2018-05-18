# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from typing import List
from .abstract_reporter import abstract_reporter
from shipit.test_runner.test_types import IhuBaseTest, ResultData
from utilities.ihuhandler import FlashResult
import logging
logger = logging.getLogger(__name__)

def _print_indented(s: str, indent="    ") -> None:
    lines = s.split("\n")
    for l in lines:
        print(indent + l)


class console_reporter(abstract_reporter):

    def __init__(self):
        pass

    def flash_started(self) -> None:
        logger.info("Started flashing IHU")

    def flash_finished(self, result: FlashResult) -> None:
        logger.info("Finished flashing IHU")
        logger.info("Attempts: {}".format(result.attempts))
        if result.success:
            success = "SUCCESS"
        else:
            success = "FAIL"
        logger.info("Result: {}".format(success))

    def plan_started(self) -> None:
        print("Starting test plan")

    def plan_finished(self, test_results: List[ResultData]) -> None:
        logger.info("Test plan finished")
        print("*** Test summary ***")
        failing_testcases = [x for x in test_results if not x.passed]
        if len(failing_testcases) > 0:
            print("#####################################################################")
            print("Failing test cases:")
            for ntr in failing_testcases:
                print(" * {}".format(ntr.name))
                if ntr.info:
                    print("   {}".format(ntr.info))
                for result in ntr.results:
                    print("    --{}, {}".format(result['name'], result['result']))
                    if 'message' in result:
                        print("      Message: {}".format(result['message']))
                    if 'stacktrace' in result:
                        if not ('message' in result and result['stacktrace'] == result['message']):
                            print("      StackTrace:")
                            _print_indented(result['stacktrace'], "      >> ")
                #if ntr.console:
                #    print("   Console:")
                #    _print_indented(ntr.console, "      >> ")
            print("#####################################################################")
        else:
            print("All %d tests successful" % len(test_results))

    def module_started(self, test: IhuBaseTest, testrun_uuid: str) -> None:
        logger.info("Module started: {}".format(str(test)))

    def module_finished(self, test: IhuBaseTest, test_result: ResultData, testrun_uuid: str) -> None:
        logger.info("Module finished: {}, Result: {}".format(str(test), str(test_result)))
