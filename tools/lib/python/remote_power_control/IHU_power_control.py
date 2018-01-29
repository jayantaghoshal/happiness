#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from remote_power_control import ipm  # IP Power Management
import json
import os

''' Prerequisite: IPM_MAP_FILE set through jenkins build environment (job specific).'''
class IHU_power_control:

    def __init__(self):
        mapping_file = os.environ['IPM_MAP_FILE']  # Set in jenkins build env
        mapping = json.load(open(mapping_file))
        test_machine = os.environ['HOST_HOSTNAME'] # Set in run.sh
        ihu = mapping[test_machine]["ihu"]
        ipm_machine = mapping[test_machine]["remote-power-switch"]
        self.ipm_hostname = mapping[ipm_machine]["hostname"]
        self.ipm_port = mapping[ipm_machine][ihu]

    def port_detail(self):
        print(self.ipm_hostname)
        print(self.ipm_port)

    def on(self):
        out = ipm.Client(self.ipm_hostname).on(self.ipm_port)
        if out == False:
            raise RuntimeError('Failed to remotely turn ON the ihu')

    def off(self):
        out = ipm.Client(self.ipm_hostname).off(self.ipm_port)
        if out == False:
            raise RuntimeError('Failed to remotely turn OFF the ihu')

    def reboot(self, sleep_time=10):
        ipm.Client(self.ipm_hostname).reboot(self.ipm_port, sleep_time)

    ''' Returns True if the port is ON and False if it is OFF'''
    def state(self):
        return ipm.Client(self.ipm_hostname).get_power_state(self.ipm_port)


''' Sample Usage
    ipm_instance = IHU_power_control()
    ipm_instance.port_detail()
    ipm_instance.off()
    ipm_instance.on()
    ipm_instance.reboot(5)
    print(ipm_instance.state())
'''
