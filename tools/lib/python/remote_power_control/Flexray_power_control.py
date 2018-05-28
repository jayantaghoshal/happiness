#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

''' Prerequisite: IPM_MAP_FILE set through jenkins build environment (job specific).'''
class Flexray_power_control(CI_power_control):

    def __init__(self):
        super().__init__()
        flexray = self.test_machine["flexray"]
        self.ipm_port = self.ipm_machine[flexray]

''' Sample Usage
    ipm_instance = Flexray_power_control()
    ipm_instance.port_detail()
    ipm_instance.off()
    ipm_instance.on()
    ipm_instance.reboot(5)
    print(ipm_instance.state())
'''
