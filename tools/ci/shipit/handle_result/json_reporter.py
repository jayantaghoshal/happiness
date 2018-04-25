# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import json
import os
import logging
import shutil
from typing import List
from shipit.testscripts import NamedTestResult
from shipit.test_runner.test_types import IhuBaseTest, ResultData
from .abstract_reporter import abstract_reporter

logger = logging.getLogger(__name__)


def dump_test_results_to_json(test_results: List[NamedTestResult]) -> None:
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


class json_reporter(abstract_reporter):

    def __init__(self):
        pass

    def plan_started(self) -> None:
        pass

    def plan_finished(self, test_results: List[NamedTestResult]) -> None:
        try:
            dump_test_results_to_json(test_results)
        except Exception as e:
            logger.error("Failed to write json test report.")

    def module_started(self, test: IhuBaseTest) -> None:
        pass

    def module_finished(self, test: IhuBaseTest, test_result: ResultData) -> None:
        pass

