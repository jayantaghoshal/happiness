#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import json
# IP Power Management
from remote_power_control.IHU_power_control import IHU_power_control
import sys

def main():
    power_cycle_length = 120
    try:
        power_cycle_length = int(sys.argv[1])
    except Exception:
        pass
    try:
        mapping_file = os.environ['IPM_MAP_FILE']  # Set in jenkins build env
        json.load(open(mapping_file))
    except Exception:
        raise RuntimeError('Failed due to the IPM map file')
    ipm_instance = IHU_power_control()
    ipm_instance.port_detail()
    ipm_instance.reboot(power_cycle_length) # Turns off power, waits <power_cycle_length> and powers on again
    print("State: "+str(ipm_instance.state()))

if __name__ == '__main__':
    main()
