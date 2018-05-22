# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import datetime
import os
import gzip
import zipfile
import shutil
import xml.etree.ElementTree as ET
import glob
from shipit.test_runner.test_types import VTSTest, TradefedTest, IhuBaseTest, Disabled, ResultData
from shipit.test_runner import vts_test_runner as vts_test_run
from shipit.test_runner.test_env import vcc_root, aosp_root, run_in_lunched_env
from pymongo import MongoClient
import bson
from typing import Dict, Any
import sys
import logging
sys.path.append(vcc_root)
from tools.ci.artifactory.artifactory.artifactory import Artifactory as Artifactory # NOQA

logger = logging.getLogger(__name__)

def clean_old_results():
    result_dirs = []
    result_dirs.append(os.path.join(
        os.environ["ANDROID_HOST_OUT"], "vts/android-vts/results/"))
    result_dirs.append(os.path.join(
        os.environ["ANDROID_HOST_OUT"], "vts/android-vts/logs/"))
    result_dirs.append("/tmp/0/stub/")
    result_dirs.append("/tmp/saved_files_from_tests/")

    for dir in result_dirs:
        if os.path.isdir(dir):
            shutil.rmtree(dir, ignore_errors=True)


def parse_vts_result_xml(vts_result_xml: str, test_detail: dict):

    tree = ET.parse(vts_result_xml)
    root = tree.getroot()

    for summary in root.iter('Summary'):
        test_detail['pass'] = int(summary.get('pass'))
        test_detail['failed'] = int(summary.get('failed'))
        test_detail['modules_done'] = int(summary.get('modules_done'))
        test_detail['modules_total'] = int(summary.get('modules_total'))

    for module in root.iter('Module'):
        test_detail['done'] = module.get('done')
        test_detail['runtime'] = int(module.get('runtime')) # milliseconds
        test_detail['abi'] = module.get('abi')

    test_detail['testcases'] = []
    i = 0
    for testcase in root.iter('TestCase'):
        test_detail['testcases'].append({})
        test_detail['testcases'][i]['testcase_name'] = testcase.get('name')
        test_detail['testcases'][i]['tests'] = []
        for test in testcase:
            if test.get('result') == 'pass':
                test_detail['testcases'][i]['tests'].append(
                    {"name": test.get('name'), "status": test.get('result')})
            elif test.get('result') == 'fail':
                test_detail['testcases'][i]['tests'].append({"name": test.get('name'), "status": test.get(
                    'result'), "failure_log": test.find('Failure').find('StackTrace').text})
        i = i + 1

    with open(vts_result_xml, "r") as f:
        test_detail['file-result_xml'] = f.read()

    return test_detail


def load_gz_logs(txt_gz_file: str):
    with gzip.open(txt_gz_file, 'rb') as f:
        # Incase of UnicodeDecodeError, the "backslashreplace" will ignore them by adding two backslashs '\\' to the front
        log_content = f.read().decode('UTF-8', 'backslashreplace')
        return log_content


def parse_tradefed_result_xml(tradefed_result_xml: str, test_detail: dict):

    root = ET.fromstring(tradefed_result_xml)

    test_detail['testcases'] = []
    i = 0
    for child in root.iter():

        if child.tag == 'testsuite':
            test_detail['tests'] = int(child.attrib.get('tests'))
            test_detail['failures'] = int(child.attrib.get('failures'))
            test_detail['runtime'] = int(child.attrib.get('time'))
            test_detail['errors'] = int(child.attrib.get('errors'))

        if child.tag == 'testcase':
            test_detail['testcases'].append({})
            test_detail['testcases'][i]['testcase_name'] = child.attrib.get(
                'name')
            test_detail['testcases'][i]['classname'] = child.attrib.get(
                'classname')
            for failure_log in child.iter('failure'):
                test_detail['testcases'][i]['failure_log'] = failure_log.text
            i = i + 1

    test_detail['file-result_xml'] = (
        (tradefed_result_xml.replace('\n', '')).replace('\r', '')).replace('"', "'")

    return test_detail

def read_module_details(android_test_xml_file: str):
    et = ET.parse(android_test_xml_file)
    return et.getroot().attrib["description"]

def load_zip_logs(txt_zip_file: str):
    zip = zipfile.ZipFile(txt_zip_file)
    filename = zip.namelist()[0]
    with zip.open(filename) as f:
        # Incase of UnicodeDecodeError, the "backslashreplace" will ignore them by adding two backslashs '\\' to the front
        log_content = str(f.read().decode('utf-8', 'backslashreplace'))
        return log_content

def clean_mongo_key(key: str):
    return key.replace(".", "_").replace("$", "_")

def zip_dir(basedir, archivename):
    with zipfile.ZipFile(archivename, 'w') as zip:
        if os.path.isfile(basedir):
            zip.write(basedir, os.path.split(basedir)[1], compress_type=zipfile.ZIP_DEFLATED)
        if os.path.isdir(basedir):
            for folder, subfolders, files in os.walk(basedir):
                for file in files:
                    zip.write(os.path.join(folder, file), os.path.relpath(os.path.join(folder, file), basedir), compress_type=zipfile.ZIP_DEFLATED)

def truncate_to_fit_mongo(log_content: str):
    # https://docs.mongodb.com/manual/reference/limits/#BSON-Document-Size
    # Mongodb supports to store max size of 16793598 bytes so we restrict each log shouldn't be more than that
    limit = 16000000
    if(sys.getsizeof(log_content) >= limit):
        logger.warning("Log file exceeds the limit")
        return "Log file exceeds the limit so trimmed " + str(sys.getsizeof(log_content) - limit) + " chars in the beginning of the file !!!!!!!!!! \n" + log_content[-limit:]
    else:
        return log_content


def load_test_results(test: IhuBaseTest,
                      test_result: ResultData,
                      started_at: datetime.datetime,
                      finished_at: datetime.datetime,
                      testrun_uuid: str):
    client = MongoClient(
        "mongodb://jenkins-icup_android:" + os.environ[
            "MONGODB_PASSWORD"] + "@gotsvl1416.got.volvocars.net:27017/admin?authMechanism=SCRAM-SHA-1")
    db = client['test_results']
    records_collection = db['records']
    screenshots_collection = db['screenshots']
    logs_collection = db['logs']


    common_identifiers = {}  # type: Dict[str, Any]
    common_identifiers["testrun_id"] = bson.ObjectId(testrun_uuid)
    if isinstance(test, VTSTest):
        common_identifiers["test_dir_name"] = test.test_root_dir
    elif isinstance(test, TradefedTest):
        common_identifiers["test_dir_name"] = test.test_root_dir
    common_identifiers["job_name"] = os.environ["JOB_NAME"]
    common_identifiers["test_job_build_number"] = int(os.environ["BUILD_NUMBER"])
    if "TOP_JOB_NUMBER" in os.environ and "TOP_JOB_JOBNAME" in os.environ and os.environ["TOP_JOB_NUMBER"] and os.environ["TOP_JOB_JOBNAME"]:
        common_identifiers["top_test_job_build_number"] = int(os.environ["TOP_JOB_NUMBER"])
        common_identifiers["top_test_job_name"] = os.environ["TOP_JOB_JOBNAME"]
    else:
        common_identifiers["top_test_job_name"] = ""
        common_identifiers["top_test_job_build_number"] = 0

    test_detail = {}
    test_detail.update(common_identifiers)
    del test_detail["testrun_id"]
    test_detail["_id"] = bson.ObjectId(testrun_uuid)
    screenshot_dict = {}
    if len(test_result.screenshot_paths) > 0:
        def load_sreenshots():
            for screenshot_path in test_result.screenshot_paths:
                screnshot = {}
                screnshot.update(common_identifiers)
                screnshot["name"] = os.path.split(screenshot_path)[-1]
                with open(screenshot_path, "rb") as f:
                    screnshot["data"] = f.read()
                yield screnshot
        screenshot_insertion_results = screenshots_collection.insert_many(load_sreenshots())
        for path, inserted_id in zip(test_result.screenshot_paths, screenshot_insertion_results.inserted_ids):
            screenshot_dict[clean_mongo_key(os.path.split(path)[-1])] = inserted_id

    test_detail["screenshots"] = screenshot_dict
    test_detail["job_name"] = os.environ["JOB_NAME"]
    test_detail["capabilities"] = list(test.require_capabilities)
    test_detail["hostname"] = os.environ["HOST_HOSTNAME"]
    test_detail["started_at"] = started_at
    test_detail["finished_at"] = finished_at
    test_detail["result_stored_at"] = datetime.datetime.utcnow()

    logs = {}

    def store_log(name, contents):
        log_entry = {}
        log_entry.update(common_identifiers)
        log_entry["name"] = name
        log_entry["contents"] = truncate_to_fit_mongo(contents)
        log_insertion_result = logs_collection.insert_one(log_entry)
        logs[clean_mongo_key(name)] = {
            "id": log_insertion_result.inserted_id,
            "size": len(contents)
        }

    try:
        store_log("console_log", test_result.console)
        test_detail["result"] = test_result.passed
    except Exception: # sometimes on test failures, "test_result" is not generated by the runner
        test_detail["result"] = False

    # TODO: This parsing should be handled inside vts_test_runner and abstracted through TestResult
    if isinstance(test, VTSTest):
        test_detail["test_type"] = "vts"
        test_detail["module_name"] = vts_test_run.read_module_name(os.path.join(aosp_root,
                                                                                test.test_root_dir, "AndroidTest.xml"))
        test_detail["description"] = read_module_details(os.path.join(aosp_root, test.test_root_dir, "AndroidTest.xml"))

        result_dir = os.path.join(os.environ["ANDROID_HOST_OUT"], "vts/android-vts/results/")
        log_dir = os.path.join(os.environ["ANDROID_HOST_OUT"], "vts/android-vts/logs/")

        test_detail["files"] = {}
        zip_folder = '/tmp/saved_files_from_tests/zip_files'
        save_file_folder = '/tmp/saved_files_from_tests/files'


        if os.path.isdir(save_file_folder):
            if not os.path.isdir(zip_folder):
                os.makedirs(zip_folder)
            for name in os.listdir(save_file_folder):
                zip_path = name + '.zip'
                zip_dir(os.path.join(save_file_folder, name), os.path.join(zip_folder, zip_path))
                uri = "ICUP_ANDROID_CI/%s/%s/%s/%s" % (os.environ["JOB_NAME"], os.environ["BUILD_NUMBER"], test_detail["module_name"], zip_path)
                Artifactory().deploy_artifact(uri, os.path.join(zip_folder, zip_path))
                test_detail["files"][name] = {'location': 'artifactory', 'uri': uri}


        for filename in glob.iglob(log_dir + '**/*.gz', recursive=True):
            name = str(os.path.splitext(os.path.splitext(os.path.basename(filename))[0])[0])
            store_log(name, load_gz_logs(filename))

        for filename in glob.iglob(result_dir + '**/test_result.xml', recursive=True):
            # TODO: overwriting in every loop?
            test_detail = parse_vts_result_xml(filename, test_detail)

        try:
            test_detail["kpis"] = test_result.test_kpis
        except Exception:
            test_detail["kpis"] = ""

        if 'file-result_xml' in test_detail:
            store_log("result_xml", test_detail['file-result_xml'])
            del test_detail['file-result_xml']

    elif isinstance(test, TradefedTest):

        test_detail["test_type"] = "tradefed"
        test_detail["module_name"] = os.path.basename(test.test_root_dir)
        test_detail["description"] = read_module_details(os.path.join(aosp_root,
                                                                      test.test_root_dir, "AndroidTest.xml"))

        result_dir = "/tmp/0/stub/"
        for filename in glob.iglob(result_dir + '**/test_result*', recursive=True):
            # TODO: overwriting in every loop?
            parse_tradefed_result_xml(load_zip_logs(filename), test_detail)

        for filename in glob.iglob(result_dir + '**/*.zip', recursive=True):
            name = str(os.path.splitext(os.path.splitext(os.path.basename(filename))[0])[0])
            store_log(name, load_zip_logs(filename))

        store_log("result_xml", test_detail['file-result_xml'])
        del test_detail['file-result_xml']

    test_detail["logs"] = logs

    records_collection.insert_one(test_detail)

def get_module_name(test):
    if isinstance(test, VTSTest):
        return vts_test_run.read_module_name(os.path.join(aosp_root, test.test_root_dir, "AndroidTest.xml"))
    elif isinstance(test, TradefedTest):
        return os.path.basename(test.test_root_dir)


def get_result(test_result: ResultData):
    try:
        return test_result.passed
    except Exception:  # sometimes on test failures, "test_result" is not generated by the runner
        return False
