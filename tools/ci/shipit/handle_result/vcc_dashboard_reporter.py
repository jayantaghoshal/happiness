# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import os
import traceback
from typing import List
from .abstract_reporter import abstract_reporter
from shipit.test_runner.test_types import IhuBaseTest, ResultData
from utilities.ihuhandler import FlashResult
from handle_result import store_result, test_visualisation

logger = logging.getLogger(__name__)

class vcc_dashboard_reporter(abstract_reporter):

    def __init__(self):
        self.continue_report = True

    def flash_started(self) -> None:
        pass

    def flash_finished(self, result: FlashResult) -> None:
        pass

    def plan_started(self) -> None:
        logger.info("Initialise visualizing Testcases in VCC CI")
        try:
            if "UPSTREAM_JOB_GIT_REVISION" in os.environ and os.environ["UPSTREAM_JOB_GIT_REVISION"]:
                change_id = test_visualisation.redis_con.get(
                    "icup_android.gerrit.commit_id." + os.environ["UPSTREAM_JOB_GIT_REVISION"] + ".change_id")
            elif "ZUUL_CHANGE" in os.environ and os.environ["ZUUL_CHANGE"]:
                logger.info("ZUUL_CHANGE:" + os.environ["ZUUL_CHANGE"])
                change_id = ((test_visualisation.redis_con.get(
                    "icup_android.gerrit.change_number." + os.environ["ZUUL_CHANGE"] + ".change_id")))
            self.vccciproxy = test_visualisation.VCCCIProxy(
                change_id.decode("utf-8"))
        except Exception as e:
            logger.error(str(traceback.format_exc()))
            logger.error(str(e))
            logger.error("Initialization of VCC CI failed")
            self.continue_report = False

    def plan_finished(self, test_results: List[ResultData]) -> None:
        pass

    def module_started(self, test: IhuBaseTest) -> None:
        if not self.continue_report:
            return
        try:
            self.vccciproxy.testcase_started(store_result.get_module_name(test))
        except Exception as e:
            logger.error(str(traceback.format_exc()))
            logger.error(str(e))
            logger.error("Testcase started message to VCC CI failed")

    def module_finished(self, test: IhuBaseTest, test_result: ResultData) -> None:
        if not self.continue_report:
            return
        try:
            self.vccciproxy.testcase_finished(store_result.get_module_name(
                test), store_result.get_result(test_result))
        except Exception as e:
            logger.error(str(traceback.format_exc()))
            logger.error(str(e))
            logger.error("Testcase finished message to VCC CI failed")
