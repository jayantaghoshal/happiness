#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import json
import sys

def main():
    mapping_file = os.environ['IPM_MAP_FILE']  # Set in jenkins build env
    try:
        lab_config_data=json.load(open(mapping_file))
    except json.JSONDecodeError as e:
        print("Error while parsing IPM_MAP_FILE: {}".format(mapping_file))
        print("Parsing error: {}".format(e))
        sys.exit(1)
    hostname = os.environ["HOST_HOSTNAME"]
    flexray_name = lab_config_data[hostname]["flexray"]
    print(lab_config_data[flexray_name]["ip"])

if __name__ == '__main__':
    main()
