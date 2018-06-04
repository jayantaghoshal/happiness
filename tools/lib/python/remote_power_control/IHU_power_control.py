#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from remote_power_control.CI_power_control import CI_power_control

''' Prerequisite: IPM_MAP_FILE set through jenkins build environment (job specific).'''
class IHU_power_control(CI_power_control):

    def __init__(self):
        super().__init__()
        ihu = self.test_machine["ihu"]
        self.ipm_port = self.ipm_machine[ihu]

''' Sample Usage
    ipm_instance = IHU_power_control()
    ipm_instance.port_detail()
    ipm_instance.off()
    ipm_instance.on()
    ipm_instance.reboot(5)
    print(ipm_instance.state())
'''
