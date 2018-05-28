#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import json
# IP Power Management
from remote_power_control.Flexray_power_control import Flexray_power_control
import sys

def main():
    try:
        power_cycle_length = int(sys.argv[1])
    except Exception:
        power_cycle_length = 30 # seconds
    ipm_instance = Flexray_power_control()
    ipm_instance.port_detail()
    ipm_instance.reboot(power_cycle_length) # Turns off power, waits <power_cycle_length> and powers on again
    print("State: " + str(ipm_instance.state()))

if __name__ == '__main__':
    main()
