#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys
import glob
import re
import shutil
import subprocess
import redis

redis_con = redis.Redis("gotsvl1416.got.volvocars.net")

IP = "012"
ECU = "IHU_4.0_HIGH"
USER = "jenkins-icup_android"
PROJECTS = ['519G', 'P319']
SW_TAGS = ['SXBL', 'SWBL', 'SWL3', 'SWLM', 'SWP2', 'SWL2']
HW_PART_NUMBERS = ['50877022000', '50902267000']
API_KEY = os.environ["ARTIFACTORY_API_KEY"]
CHAIN_ID = redis_con.get(
    "icup_android.gerrit.commit_id." + os.environ["UPSTREAM_JOB_GIT_REVISION"] + ".change_id")

def create_ip():
    result = subprocess.Popen(
        ['swat', 'create_ip', '--username='+USER, '--ecu='+ECU, '--ip='+IP], stdout=subprocess.PIPE)
    if result.communicate()[1] != None:
        print("Create IP failed in SWAT")
        sys.exit(1)


def set_meta_data_in_ip():
    result = subprocess.Popen(
        ['swat', 'set_meta_data_in_ip', '--username='+USER, '--ecu='+ECU, '--ip='+IP, '--projects '] + PROJECTS + ['--sw_tags '] + SW_TAGS + ['--hw_part_numbers '] + HW_PART_NUMBERS, stdout=subprocess.PIPE)
    if result.communicate()[1] != None:
        print("Set meta data failed in SWAT")
        sys.exit(1)

def add_file_to_ip(TAG: str, sw_tags_to_filename: dict):
    result = subprocess.Popen(['swat', 'add_file_to_ip', '--username='+USER, '--ecu='+ECU,
                               '--ip='+IP, '--file_path='+sw_tags_to_filename[TAG], '--sw_tag='+TAG, '--sw_details=\"Chain-Id: '+CHAIN_ID+'\"', '--api_key_swf1='+API_KEY], stdout=subprocess.PIPE)
    print(result.communicate()[0]) # print the response from response
    if result.communicate()[1] != None:
        print("Add files to IP failed in SWAT")
        sys.exit(1)


def publish_ip():
    result = subprocess.Popen(
        ['swat', 'publish_ip', '--username='+USER, '--ecu='+ECU, '--ip='+IP, '--publish_level=1', '--api_key_swf1='+API_KEY], stdout=subprocess.PIPE)
    if result.communicate()[1] != None:
        print("Publish IP failed in SWAT")
        sys.exit(1)


if __name__ == "__main__":

    try:
        fn = sys.argv[1]
    except Exception as e:
        print("Expects path of the vbf files as argument- Error:"+str(e))
        sys.exit(1)

    if os.path.exists(fn):
        no_vbf = True
        for filename in glob.iglob(os.path.join(fn, "*.VBF")):
            no_vbf = False
        if no_vbf:
            print("No VBFs in the given directory.")
            sys.exit(1)

        no_tag_file = True
        for filename in glob.iglob(os.path.join(fn, "vbf_tags")):
            sw_tags_to_filename = {}
            with open(filename,'r') as file_handler:
                for line in file_handler:
                    sw_tags_to_filename[line.strip().split(",")[0]] = line.strip().split(",")[1]
            no_tag_file = False
        if no_tag_file:
            print("No VBF tag file that gives the list of vbfs and corresponding SW tags")
            sys.exit(1)
    else:
        raise("path does not exist")

    create_ip()
    set_meta_data_in_ip()

    for TAG in SW_TAGS:
        add_file_to_ip(TAG, sw_tags_to_filename)

    publish_ip()
