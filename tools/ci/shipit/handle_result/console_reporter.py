# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from typing import List
from .abstract_reporter import abstract_reporter
from shipit.testscripts import NamedTestResult
from shipit.test_runner.test_types import IhuBaseTest, ResultData
from utilities.ihuhandler import FlashResult

def _print_indented(self, s: str, indent="    ") -> None:
    lines = s.split("\n")
    for l in lines:
        print(indent + l)


class console_reporter(abstract_reporter):

    def __init__(self):
        pass

    def plan_started(self) -> None:
        pass

    def flash_started(self) -> None:
        pass

    def flash_finished(self, result: FlashResult) -> None:
        pass

    def plan_finished(self, test_results: List[NamedTestResult]) -> None:
        print("*** VTS Python Test summary ***")
        print("All tests completed")
        failing_testcases = [x for x in test_results if not x.result.passed]
        if len(failing_testcases) > 0:
            print("#####################################################################")
            print("Failing test cases:")
            for ntr in failing_testcases:
                print(" * %s" % ntr.name)
                _print_indented(ntr.result.console, "      >> ")
            print("#####################################################################")
        else:
            print("All %d tests successful" % len(test_results))

    def module_started(self, test: IhuBaseTest) -> None:
        pass

    def module_finished(self, test: IhuBaseTest, test_result: ResultData) -> None:
        pass
