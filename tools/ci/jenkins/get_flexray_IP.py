#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import json

def main():
    try:
        mapping_file = os.environ['IPM_MAP_FILE']  # Set in jenkins build env
        lab_config_data=json.load(open(mapping_file))
        hostname = os.environ["HOST_HOSTNAME"]
        flexray_name = lab_config_data[hostname]["flexray"]
        return lab_config_data[flexray_name]["ip"]
    except Exception:
        return ""

if __name__ == '__main__':
    main()
