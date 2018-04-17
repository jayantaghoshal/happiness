# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import gzip
import zipfile
import shutil
import xml.etree.ElementTree as ET
import json
import glob
from shipit.test_runner.test_types import VTSTest, TradefedTest, IhuBaseTest, Disabled, ResultData
from shipit.test_runner import vts_test_runner as vts_test_run
from shipit.test_runner.test_env import vcc_root, aosp_root, run_in_lunched_env
from . import mongodb_wrapper

import sys

def clean_old_results():
    result_dirs = []
    result_dirs.append(os.path.join(
        os.environ["ANDROID_HOST_OUT"], "vts/android-vts/results/"))
    result_dirs.append(os.path.join(
        os.environ["ANDROID_HOST_OUT"], "vts/android-vts/logs/"))
    result_dirs.append("/tmp/0/stub/")

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
        test_detail['runtime'] = int(module.get('runtime'))
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


def load_zip_logs(txt_zip_file: str):
    zip = zipfile.ZipFile(txt_zip_file)
    filename = zip.namelist()[0]
    with zip.open(filename) as f:
        # Incase of UnicodeDecodeError, the "backslashreplace" will ignore them by adding two backslashs '\\' to the front
        log_content = str(f.read().decode('utf-8', 'backslashreplace'))
        return log_content

def truncate_to_fit_mongo(log_content: str):
    # Mongodb supports to store max size of 16793598 bytes so we restrict each log shouldn't be more than 4 Mb
    if(sys.getsizeof(log_content) >= 4000000):
        print("Log file exceeds the limit")
        return "Log file exceeds the limit so trimmed " + str(sys.getsizeof(log_content) - 4000000) + " chars in the beginning of the file !!!!!!!!!! \n" + log_content[-4000000:]
    else:
        return log_content


def load_test_results(test, test_result: ResultData):

    test_detail = {}
    test_detail["test_dir_name"] = test.test_root_dir
    test_detail["job_name"] = os.environ["JOB_NAME"]
    test_detail["capabilities"] = str(test.require_capabilities)
    test_detail["test_job_build_number"] = int(os.environ["BUILD_NUMBER"])
    test_detail["hostname"] = os.environ["HOST_HOSTNAME"]

    try:
        test_detail["console_log"] = test_result.console
        test_detail["result"] = test_result.passed
    except Exception: # sometimes on test failures, "test_result" is not generated by the runner
        test_detail["console_log"] = ""
        test_detail["result"] = False

    if "TOP_JOB_NUMBER" in os.environ and "TOP_JOB_JOBNAME" in os.environ and os.environ["TOP_JOB_NUMBER"] and os.environ["TOP_JOB_JOBNAME"]:
        test_detail["top_test_job_build_number"] = int(os.environ["TOP_JOB_NUMBER"])
        test_detail["top_test_job_name"] = os.environ["TOP_JOB_JOBNAME"]
    else:
        test_detail["top_test_job_name"] = ""
        test_detail["top_test_job_build_number"] = 0

    if isinstance(test, VTSTest):
        test_detail["test_type"] = "vts"
        test_detail["module_name"] = vts_test_run.read_module_name(os.path.join(aosp_root,
                                                                                test.test_root_dir, "AndroidTest.xml"))

        result_dir = os.path.join(
            os.environ["ANDROID_HOST_OUT"], "vts/android-vts/results/")
        log_dir = os.path.join(
            os.environ["ANDROID_HOST_OUT"], "vts/android-vts/logs/")

        for filename in glob.iglob(log_dir + '**/*.gz', recursive=True):
            test_detail['file-' + str(os.path.splitext(os.path.splitext(
                os.path.basename(filename))[0])[0])] = truncate_to_fit_mongo(load_gz_logs(filename))

        for filename in glob.iglob(result_dir + '**/test_result.xml', recursive=True):
            test_detail = parse_vts_result_xml(filename, test_detail)

        try:
            test_detail["kpis"] = test_result.test_kpis
        except Exception:
            test_detail["kpis"] = ""

    elif isinstance(test, TradefedTest):

        test_detail["test_type"] = "tradefed"
        test_detail["module_name"] = os.path.basename(test.test_root_dir)
        result_dir = "/tmp/0/stub/"

        for filename in glob.iglob(result_dir + '**/test_result*', recursive=True):
            parse_tradefed_result_xml(truncate_to_fit_mongo(load_zip_logs(filename)), test_detail)

        for filename in glob.iglob(result_dir + '**/*.zip', recursive=True):
            test_detail['file-' + str(os.path.splitext(os.path.splitext(
                os.path.basename(filename))[0])[0])] = truncate_to_fit_mongo(load_zip_logs(filename))

    mongodb_wrapper.insert_data(test_detail)

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
