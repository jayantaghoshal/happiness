# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import traceback
import datetime
import os
from typing import List
from .abstract_reporter import abstract_reporter
from handle_result import store_result, test_visualisation
from shipit.test_runner.test_types import IhuBaseTest, ResultData
from utilities.ihuhandler import FlashResult
from .influxdb_wrapper import insert_influx_data, query_influx_data
from . import mongodb_wrapper

logger = logging.getLogger(__name__)

class ci_database_reporter(abstract_reporter):

    def __init__(self):
        self.continue_report = True

    def plan_started(self) -> None:
        pass

    def plan_finished(self, test_results: List[ResultData]) -> None:
        #InfluxDB:
        failing_testcases = [x for x in test_results if not x.passed]
        passing_testcases = len(test_results) - len(failing_testcases)
        # NOTE: Gate test does not have top job name and build number yet, hard code the value to align with hourly and daily
        if "TOP_JOB_NUMBER" in os.environ and "TOP_JOB_JOBNAME" in os.environ and os.environ["TOP_JOB_NUMBER"] and os.environ["TOP_JOB_JOBNAME"]:
            top_job_jobname = os.environ["TOP_JOB_JOBNAME"]
            top_job_build_number = int(os.environ["TOP_JOB_NUMBER"])
        else:
            if os.environ["JOB_NAME"] == "ihu_gate_test":
                top_job_jobname = "ihu_gate"
            else:
                top_job_jobname = ""
            top_job_build_number = 0
        data = {
            'build_number' : os.environ["BUILD_NUMBER"],
            'gerrit_id' : os.environ["ZUUL_CHANGE"],
            'build_host' : os.environ["HOST_HOSTNAME"],
            'job_name' : os.environ["JOB_NAME"],
            'top_job_name': top_job_jobname,
            'top_job_build_number' : top_job_build_number,
            'passing_testcases' : passing_testcases,
            'failing_testcases' : len(failing_testcases),
            'total_testcases' : len(test_results),
        }

        tags = {
            "jobType": "ihu_test",
        }

        logger.debug("Storing: {}".format(data))
        try:
            insert_influx_data(str(os.environ["JOB_NAME"]), data, tags)
        except Exception as e:
            logger.error(str(e))

        kpidata = {
            'build_number' : os.environ["BUILD_NUMBER"],
            'build_host' : os.environ["HOST_HOSTNAME"],
            'top_job_name': top_job_jobname,
            'top_job_build_number' : top_job_build_number,
        }

        for r in test_results:
            if r.name != 'VtsCiSmokeTest':
                continue
            for kpi in r.test_kpis:
                kpidata[kpi] = r.test_kpis[kpi]

        kpitags = {
            "jobType": "ihu_test_kpi",
        }

        logger.debug("Storing: {}".format(data))
        try:
            insert_influx_data(str(os.environ["JOB_NAME"]) + "_kpi", kpidata, kpitags)
        except Exception as e:
            logger.error(str(e))


        # create overview measurement for hourly and daily, since they are currently divided in seperated sections
        self.regroup_data(passing_testcases, len(failing_testcases), len(test_results), top_job_jobname, top_job_build_number, top_job_jobname + "_overview")

        #MongoDB:
        #link_template = "https://reports-ihu-ci.cm.volvocars.biz/tests?top_job_name={}&build_number={}"
        #link = link_template.format(top_job_jobname, top_job_build_number)
        link = "https://reports-ihu-ci.cm.volvocars.biz"
        logging.info("Results and logs can be accessed from: {}".format(link))

    def regroup_data(self, passing_testcases, failing_testcases, total_testcases, top_job_jobname, top_job_build_number, measurement) -> None:
        fieldPass = "passing_testcases"
        fieldFail = "failing_testcases"
        fieldTotal = "total_testcases"
        fieldTime = "time"
        build_number = "top_job_build_number={}".format(top_job_build_number)
        try:
            results = query_influx_data(fieldPass, fieldFail, fieldTotal, fieldTime, measurement, build_number)
        except Exception as e:
            logger.error(str(e))
        points = results.get_points()
        logger.debug("Result: {0}".format(results))

        if len(results) > 0 :
            logger.debug("Has record, append to existing measurement")
            for item in points:
                passing_testcases += item[fieldPass]
                failing_testcases += item[fieldFail]
                total_testcases += item[fieldTotal]
                timestamp = item[fieldTime]
        else:
            logger.debug("No result yet, insert new measurement")
            timestamp = ""

        data = {
            'gerrit_id' : os.environ["ZUUL_CHANGE"],
            'top_job_name': top_job_jobname,
            'top_job_build_number' : top_job_build_number,
            'passing_testcases' : passing_testcases,
            'failing_testcases' : failing_testcases,
            'total_testcases' : total_testcases,
        }
        tags = {
            "jobType": "ihu_test_overview",
        }

        logger.info("Storing: {}".format(data))
        try:
            insert_influx_data(measurement, data, tags, timestamp)
        except Exception as e:
            logger.error(str(e))


    def module_started(self, test: IhuBaseTest, testrun_uuid: str) -> None:
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

    def module_finished(self, test: IhuBaseTest, test_result: ResultData, testrun_uuid: str) -> None:
        if not self.continue_report:
            return
        try:
            store_result.load_test_results(test, test_result, self.started_at, datetime.datetime.utcnow(), testrun_uuid)
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
        logger.debug("Storing: {}".format(data))

        try:
            insert_influx_data("flash_result", data, tags)
        except Exception as e:
            logger.error(str(e))

        try:
            mongodb_wrapper.insert_data(self.store_swdl_to_mongodb(result))

        except Exception as e:
            logger.error(str(e))

    def store_swdl_to_mongodb(self, result: FlashResult):
        mongo_data = {
                    'description' : "IHU fastboot flashing",
                    'module_name' : "Software_Download_fastboot",
                    'job_name' : os.environ["JOB_NAME"],
                    'test_job_build_number' : int(os.environ["BUILD_NUMBER"]),
                    'test_dir_name' : os.environ["PWD"],
                    'result' : bool(result.success),
                    'test_type' : 'SWDL',
                    'hostname' : os.environ["HOST_HOSTNAME"],
                    'runtime' : float(result.totaltime),
                    'flash_attempt' : result.attempts,
                    'result_stored_at' : datetime.datetime.utcnow()
                }
        if "TOP_JOB_NUMBER" in os.environ and "TOP_JOB_JOBNAME" in os.environ and os.environ["TOP_JOB_NUMBER"] and os.environ["TOP_JOB_JOBNAME"]:
            mongo_data["top_test_job_build_number"] = int(os.environ["TOP_JOB_NUMBER"])
            mongo_data["top_test_job_name"] = os.environ["TOP_JOB_JOBNAME"]
        else:
            mongo_data["top_test_job_name"] = ""
            mongo_data["top_test_job_build_number"] = 0

        logs = {}

        with open(os.path.join(os.environ["REPO_ROOT_DIR"], "ihu_update.log")) as f:
            log_entry = {}
            log_entry["name"] = f.name
            log_entry["contents"] = store_result.truncate_to_fit_mongo(f.read())
            log_insertion_result = mongodb_wrapper.insert_data(log_entry, "logs")
            logs[store_result.clean_mongo_key(f.name)] = {
                "id": log_insertion_result,
                "size": len(f.read())
            }

        mongo_data["logs"] = logs

        logger.debug("Mongo data: {}".format(mongo_data))

        return mongo_data
