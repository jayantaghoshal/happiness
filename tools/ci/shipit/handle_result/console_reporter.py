# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from typing import List
from .abstract_reporter import abstract_reporter
from shipit.testscripts import NamedTestResult
from shipit.test_runner.test_types import IhuBaseTest, ResultData

def _print_indented(self, s: str, indent="    ") -> None:
    lines = s.split("\n")
    for l in lines:
        print(indent + l)


class console_reporter(abstract_reporter):

    def __init__(self):
        pass

    def plan_started(self) -> None:
        pass

    def plan_finished(self, test_results: List[NamedTestResult]) -> None:
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
        results = test_result.json_result
        if results is not None:
            print("Test name: " + str(test))
            print("Test class: " + results["Results"][0]["Test Class"])
            for result in results["Results"]:
                print("\tTest name: " + result["Test Name"])
                print("\tResult: " + result["Result"])

            for result in results["Results"]:
                if result["Result"] != "PASS":
                    print("Details: " + result["Details"])
                    print("Test failed! The result from " + result["Test Class"] + ", " + result["Test Name"] + " is " + result["Result"])

            print("Number of executed tests in JSON file: " + str(results["Summary"]["Executed"]))
        else:
            print("The current test case %s does not generate a JSON file" % str(test))
