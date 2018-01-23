#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import json
import os
import xml.etree.ElementTree as ET



build_number = os.environ["BUILD_NUMBER"]
workspace = os.environ["WORKSPACE"]


def xmlToJSON_Tradefed(tradefed_result_dir: str, hostname: str):

    # create/empty the result json
    tradefed_result_file = os.path.join(
        tradefed_result_dir, 'tradefed_result.json')

    tradefed_content = os.listdir(tradefed_result_dir)
    tradefed_test_invocations = []
    for inv in tradefed_content:  # filter inv directories
        if os.path.isdir(os.path.join(tradefed_result_dir, inv)):
            tradefed_test_invocations.append(inv)

    tmp_count = 0
    for tmp in tradefed_test_invocations:  # append the path to inv dir
        tradefed_test_invocations[tmp_count] = os.path.join(
            tradefed_result_dir, tmp)
        tmp_count = tmp_count + 1

    dict_list = []
    for f in tradefed_test_invocations:  # iterate invocations
        data = {}
        tree = ET.parse(os.path.join(f, 'test_result_.xml'))
        root = tree.getroot()

        for child in root.iter():

            if child.tag == 'testsuite':
                data['tests'] = int(child.attrib.get('tests'))
                data['failures'] = int(child.attrib.get('failures'))
                data['hostname'] = hostname
                data['time'] = int(child.attrib.get('time'))
                data['timestamp'] = child.attrib.get('timestamp')
                data['errors'] = int(child.attrib.get('errors'))

            if child.tag == 'testcase':
                data['name'] = child.attrib.get('name')
                data['classname'] = child.attrib.get('classname')
                for failure_log in child.iter('failure'):
                    data['failure_log'] = failure_log.text

        logs_list = os.listdir(f)
        logs_list.remove('test_result_.xml')

        data['logs'] = []
        for log in logs_list:
            data['logs'].append(os.path.basename(f) + '/' + log)

        dict_list.append(data)

    # Writing JSON data
    with open(tradefed_result_file, 'w') as tmp:
        json.dump({"results": dict_list}, tmp, indent=4, sort_keys=True)


def xmlToJSON_VTS(vts_result_dir, hostname):

    # create/empty the result json
    vts_result_file = os.path.join(
        vts_result_dir, 'vts_result.json')

    vts_content = os.listdir(vts_result_dir)
    vts_test_invocations = []
    for inv in vts_content:  # filter inv directories
        if os.path.isdir(os.path.join(vts_result_dir, inv)):
            vts_test_invocations.append(inv)

    tmp_count = 0
    for tmp in vts_test_invocations:  # append the path to inv dir
        vts_test_invocations[tmp_count] = os.path.join(vts_result_dir, tmp)
        tmp_count = tmp_count + 1

    dict_list = []
    for f in vts_test_invocations:  # iterate invocations
        data = {}
        tree = ET.parse(os.path.join(f, 'test_result.xml'))
        root = tree.getroot()

        for summary in root.iter('Summary'):
            data['pass'] = int(summary.get('pass'))
            data['failed'] = int(summary.get('failed'))
            data['modules_done'] = int(summary.get('modules_done'))
            data['modules_total'] = int(summary.get('modules_total'))

        for module in root.iter('Module'):
            data['done'] = module.get('done')
            data['runtime'] = int(module.get('runtime'))
            data['name'] = module.get('name')
            data['abi'] = module.get('abi')

        data['testcases'] = []
        i = 0
        for testcase in root.iter('TestCase'):
            data['testcases'].append({})
            data['testcases'][i]['testcase_name'] = testcase.get('name')
            data['testcases'][i]['tests'] = []
            for test in testcase:
                if test.get('result') == 'pass':
                    data['testcases'][i]['tests'].append(
                        {"name": test.get('name'), "status": test.get('result')})
                elif test.get('result') == 'fail':
                    data['testcases'][i]['tests'].append({"name": test.get('name'), "status": test.get(
                        'result'), "failure_log": test.find('Failure').find('StackTrace').text})

            i = i + 1

        # get log list
        logs_list = os.listdir(f)
        logs_list.remove('test_result.xml')

        data['logs'] = []
        for log in logs_list:
            data['logs'].append(os.path.join(os.path.basename(f), log))

        data['hostname'] = hostname
        dict_list.append(data)

    # Writing JSON data
    with open(vts_result_file, 'w') as tmp:
        json.dump({"results": dict_list}, tmp, indent=4, sort_keys=True)


def main():
    top_dir = os.path.join(workspace, build_number)
    for f in os.listdir(top_dir):
        pathname = os.path.join(top_dir, f)
        if os.path.isdir(pathname):
            hostname = os.path.basename(pathname).split("_")[-1]
            xmlToJSON_Tradefed(os.path.join(pathname, "tradefed"), hostname)
            xmlToJSON_VTS(os.path.join(pathname, "vts"), hostname)


if __name__ == "__main__":
    main()
