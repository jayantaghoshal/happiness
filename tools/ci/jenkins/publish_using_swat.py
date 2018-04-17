#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys
import glob
import re
import subprocess
import redis
import logging

redis_con = redis.Redis("gotsvl1416.got.volvocars.net")

IP = "013"
ECU = "IHU_4.0_HIGH"
USER = "e9426001"  # case sensitive
PROJECTS = ['519G', 'P319']
SW_TAGS = ['SXBL', 'SWBL', 'SWL3', 'SWLM', 'SWP2', 'SWL2']
HW_PART_NUMBERS = ['50877022000', '50902267000']
API_KEY = os.environ["ARTIFACTORY_API_KEY"]
CHAIN_ID = redis_con.get(
    "icup_android.gerrit.commit_id." + os.environ["UPSTREAM_JOB_GIT_REVISION"] + ".change_id").decode("utf-8")


def create_ip():
    try:
        subprocess.check_call(
            ['swat', 'create_ip', '--username='+USER, '--ecu='+ECU, '--ip='+IP])

    except subprocess.CalledProcessError as e:
        if e.returncode == 10:
            logging.debug("IP already exist")
        else:
            logging.error(e)
            raise Exception("Failed in creating IP")


def set_meta_data_in_ip():
    try:
        subprocess.check_call(
            ['swat', 'set_meta_data_in_ip', '--username='+USER, '--ecu='+ECU, '--ip='+IP, '--projects'] + PROJECTS + ['--sw_tags'] + SW_TAGS + ['--hw_part_numbers'] + HW_PART_NUMBERS)

    except subprocess.CalledProcessError as e:
        logging.error(e)
        raise Exception("Set meta data failed in SWAT")


def add_file_to_ip(TAG, sw_tags_to_filename, file_path):
    try:
        subprocess.check_call(['swat', 'add_file_to_ip', '--username='+USER, '--ecu='+ECU,
                                        '--ip='+IP, '--file_path='+os.path.join(file_path, sw_tags_to_filename[TAG]), '--sw_tag='+TAG, '--sw_details=\"Chain-Id: '+CHAIN_ID+'\"', '--api_key_swf1='+API_KEY])

    except subprocess.CalledProcessError as e:
        logging.error(e)
        raise Exception("Add files to IP failed in SWAT")


def publish_ip():
    try:
        subprocess.check_call(
            ['swat', 'publish_ip', '--username='+USER, '--ecu='+ECU, '--ip='+IP, '--publish_level=1', '--api_key_swf1='+API_KEY], stderr=subprocess.STDOUT)

    except subprocess.CalledProcessError as e:
        logging.error(e)
        raise Exception("Publish IP failed in SWAT")


def main(vbf_dir):
    logging.getLogger().setLevel(logging.DEBUG)

    if os.path.exists(vbf_dir):
        no_vbf = True
        for filename in glob.iglob(os.path.join(vbf_dir, "*.VBF")):
            no_vbf = False
        if no_vbf:
            raise Exception("No VBFs in the given directory.")

        no_tag_file = True
        for filename in glob.iglob(os.path.join(vbf_dir, "vbf_tags")):
            sw_tags_to_filename = {}
            with open(filename, 'r') as file_handler:
                for line in file_handler:
                    sw_tags_to_filename[line.strip().split(
                        ",")[0]] = line.strip().split(",")[1]
            no_tag_file = False
        if no_tag_file:
            raise Exception(
                "No VBF tag file that gives the list of vbfs and corresponding SW tags")
    else:
        raise("path does not exist")

    create_ip()
    set_meta_data_in_ip()

    for TAG in SW_TAGS:
        add_file_to_ip(TAG, sw_tags_to_filename, vbf_dir)

    publish_ip()


if __name__ == "__main__":
    try:
        vbf_dir = sys.argv[1]
    except Exception as e:
        raise Exception(
            "Expects path of the vbf files as argument- Error:" + str(e))
    main(vbf_dir)
