# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import zipfile
import sys
import xml.etree.ElementTree as ET
import json
import mongodb_wrapper
from typing import Any, Dict

def extract_files(path):
    found = False
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith('.zip'):
                if unzip(root, file):
                    found = True
    return found


def unzip(folder, file):
    try:
        print("Unpacking file: {}".format(os.path.join(folder, file)))
        zip_ref = zipfile.ZipFile(os.path.join(folder, file), 'r')
        zip_ref.extractall(folder)
        zip_ref.close()
        return True
    except Exception:
        print("Could not unpack file: " + str(os.path.join(folder, file)))
        return False


def get_result_file(path):
    for root, dirs, files in os.walk(path):
        for file in files:
            if not file.endswith('test_result.xml'):
                continue
            return os.path.join(root, file)
    return None


def upload_results(file):
    tree = ET.parse(file)
    root = tree.getroot()

    if root.tag == 'Result':
        print('Found results')
    else:
        sys.exit(1)

    build = root.find('Build')
    base = {} # type: Dict[str, Any]
    json_base = {**base, **build.attrib}
    json_base["job_name"] = os.environ["JOB_NAME"]
    json_base["capabilities"] = 'vts'
    json_base["test_job_build_number"] = int(os.environ["BUILD_NUMBER"])
    json_base["hostname"] = os.environ["HOST_HOSTNAME"]
    json_base["top_test_job_build_number"] = int(os.environ["TOP_JOB_NUMBER"])
    json_base["top_test_job_name"] = "ihu_vts"
    json_base["test_type"] = 'AndroidVTS'

    for module in root.findall('Module'):
        module_json = {**json_base, **module.attrib}
        module_json['test_dir_name'] = module.attrib['name']
        module_json['module_name'] = module.attrib['name']
        del module_json['name']
        testcase = module.find('TestCase')
        module_json['testcases'] = [{'tests': [], 'testcase_name': testcase.attrib['name']}]
        result = True
        for test in testcase.findall('Test'):
            module_json['testcases'][0]['tests'].append({'name': test.attrib['name'], 'status': test.attrib['result']})
            if not test.attrib['result'] == 'pass':
                result = False
        module_json["result"] = result

        print(module_json)
        mongodb_wrapper.insert_data(module_json)


def main():
    if len(sys.argv) < 2:
        print("Need one folder as argument, exit")
        sys.exit(1)
    folder = sys.argv[1]

    if not extract_files(folder):
        print("No zip files found in folder ({}), aborting upload".format(folder))
        sys.exit(1)

    file = get_result_file(folder)
    if file is None:
        print("No results found in folder ({}), aborting upload".format(folder))
        sys.exit(1)

    upload_results(get_result_file(folder))

    sys.exit(0)


if __name__ == "__main__":
    main()

