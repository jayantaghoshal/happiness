#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from pymongo import MongoClient
import json
import os
import glob


def mongo_client(job: str):
    client = MongoClient("mongodb://gotsvl1416.got.volvocars.net:27017")
    db = client['test_results']
    collection = db[job]
    return collection


def collect_all_result_data(result_dir: str):

    all_result_data = []
    for builds in glob.glob(os.path.join(result_dir, '*')):
        data = {}
        changelog = glob.glob(os.path.join(builds, 'changelog.json'))

        # fetch json data of changelog
        with open(changelog.pop(), 'r') as f:
            changelog = json.load(f)

        data['build_number'] = changelog['id']
        data['changeSet'] = changelog['changeSet']['items']
        data['subBuilds'] = changelog['subBuilds']

        for vts_json in glob.glob(os.path.join(builds, '**/vts/vts_result.json')):
            with open(vts_json, 'r') as f:
                vts_json_data = json.load(f)
            if 'vts_results' in data:
                data['vts_results'].append(vts_json_data['results'])
            else:
                data['vts_results'] = vts_json_data['results']

        for tradefed_json in glob.glob(os.path.join(builds, '**/tradefed/tradefed_result.json')):
            with open(tradefed_json, 'r') as f:
                tradefed_json_data = json.load(f)
            if 'tradefed_results' in data:
                data['tradefed_results'].append(tradefed_json_data['results'])
            else:
                data['tradefed_results'] = tradefed_json_data['results']

        all_result_data.append(data)

    return all_result_data


def collect_result_data(result_dir: str):

    data = {}
    changelog = glob.glob(os.path.join(result_dir, 'changelog.json'))

    # fetch json data of changelog
    with open(changelog.pop(), 'r') as f:
        changelog = json.load(f)

    data['build_number'] = changelog['id']
    data['changeSet'] = changelog['changeSet']['items']
    data['subBuilds'] = changelog['subBuilds']

    for vts_json in glob.glob(os.path.join(result_dir, '**/vts/vts_result.json')):
        with open(vts_json, 'r') as f:
            vts_json_data = json.load(f)
        if 'vts_results' in data:
            data['vts_results'].append(vts_json_data['results'])
        else:
            data['vts_results'] = vts_json_data['results']

    for tradefed_json in glob.glob(os.path.join(result_dir, '**/tradefed/tradefed_result.json')):
        with open(tradefed_json, 'r') as f:
            tradefed_json_data = json.load(f)
        if 'tradefed_results' in data:
            data['tradefed_results'].append(tradefed_json_data['results'])
        else:
            data['tradefed_results'] = tradefed_json_data['results']

    return data


def insert_document(collection, data):

    if collection.find_one({"build_number": data["build_number"]}):
        print("data exist for build number: "+data["build_number"])
        print("Try updating instead of insert")
        return
    else:
        collection.insert(data)


def load_old_data_to_db(job: str):
    collection = mongo_client(job)
    if job == "hourly":
        all_result_data = collect_all_result_data("/cm/www/hourly_test_report")
    elif job == "daily":
        all_result_data = collect_all_result_data("/cm/www/daily_test_report")
    for data in all_result_data:
        insert_document(collection, data)


def drop_old_data(job: str):
    collection = mongo_client(job)
    print(str(collection.count()) + " documents deleted from collection '"+job+"'")
    collection.remove({})


def load_current_build_data(job: str):
    collection = mongo_client(job)
    workspace = os.environ["WORKSPACE"]
    build_number = os.environ["BUILD_NUMBER"]
    data = collect_result_data(os.path.join(workspace, build_number))
    insert_document(collection, data)
    print("Results from this build are stored in mongoDB.")


if __name__ == '__main__':

    JOB_NAME = os.environ["JOB_NAME"]
    if JOB_NAME == "ihu_hourly":
        load_current_build_data('hourly')
    elif JOB_NAME == "ihu_daily":
        load_current_build_data('daily')
    else:
        raise("JOB_NAME not found")
