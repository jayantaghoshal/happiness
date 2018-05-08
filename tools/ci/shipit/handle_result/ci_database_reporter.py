# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import traceback
import datetime
import os
from typing import List
from .abstract_reporter import abstract_reporter
from handle_result import store_result, test_visualisation
from shipit.testscripts import NamedTestResult
from shipit.test_runner.test_types import IhuBaseTest, ResultData
from utilities.ihuhandler import FlashResult
from .influxdb_wrapper import insert_influx_data

logger = logging.getLogger(__name__)

class ci_database_reporter(abstract_reporter):

    def __init__(self):
        self.continue_report = True

    def plan_started(self) -> None:
        pass

    def plan_finished(self, test_results: List[NamedTestResult]) -> None:
        pass

    def module_started(self, test: IhuBaseTest) -> None:
        if not self.continue_report:
            return
        self.started_at = datetime.datetime.utcnow()
        try:
            store_result.clean_old_results()
        except Exception as e:
            logger.error(str(traceback.format_exc()))
            logger.error(str(e))
            logger.error("Cleaning old results failed")
            self.continue_report = False

    def module_finished(self, test: IhuBaseTest, test_result: ResultData) -> None:
        if not self.continue_report:
            return
        try:
            store_result.load_test_results(test, test_result, self.started_at, datetime.datetime.utcnow())
        except Exception as e:
            logger.error(str(traceback.format_exc()))
            logger.error(str(e))
            logger.error("Storing results to mongodb failed")

    def flash_started(self):
        pass

    def flash_finished(self, result: FlashResult):
        data = {
            'build_number' : os.environ["BUILD_NUMBER"],
            'gerrit_id' : os.environ["ZUUL_CHANGE"],
            'build_host' : os.environ["HOST_HOSTNAME"],
            'job_name' : os.environ["JOB_NAME"],
            'flash_attempt' : result.attempts,
            'flash_time' : float(result.flashtime),
            'total_time' : float(result.totaltime),
            'info' : str(result.info),
            'flash_result' : bool(result.success),
        }
        tags = {
            "jobType": "ihu_flash",
        }
        logger.info("Storing: {}".format(data))
        try:
            insert_influx_data("flash_result", data, tags)
        except Exception as e:
            logger.info(str(e))

