#!/usr/bin/env python

import logging
import time
import sys
import os
import re
import subprocess
import shlex
import io
from time import sleep

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
import ihu_base_test

class VtsCiSmokeTest(ihu_base_test.IhuBaseTestClass):
    """
       Smoke tests to run first to check the sanity of the device.
       The old smoke test requirements for earlier projects was at
       90 % load and 90 % free memory.
    """
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]


    def testCpuLoad(self):
        #requirement = 1 
        requirement = 90
        self.dut.shell.InvokeTerminal("my_shell3")
        my_shell = getattr(self.dut.shell, "my_shell3")
        shell_response = my_shell.Execute(["cat /proc/cpuinfo"])

        model_name = re.findall('model\s*name\s*:\s*([^\n\r]*)', shell_response[const.STDOUT][0])[0]
        number_of_cores = re.findall('cpu\s*cores\s*:\s*(\d+)', shell_response[const.STDOUT][0])[0]

        first_cpu_data = self.get_data(int(number_of_cores))
        sleep(30)
        second_cpu_data = self.get_data(int(number_of_cores))
        total_load = self.calc_load(first_cpu_data, second_cpu_data, int(number_of_cores))

        for core in range(int(number_of_cores)):
            logging.info("load in core" + str(core) + " = " + '%.1f%%' % total_load[core])
            if total_load[core] > requirement:
                process_running = my_shell.Execute(["top -n1"])
                logging.info("top -n1")
                logging.info(process_running[const.STDOUT][0])
                asserts.assertLess(total_load[core], requirement, "The load on the core is over " + str(requirement) + "%")

        logging.info("Cpu cores: " + number_of_cores)
        logging.info("model_name: " + model_name)


    def testMemory(self):
        requirement = 10
        #requirement = 99
        self.dut.shell.InvokeTerminal("my_shell4")
        my_shell = getattr(self.dut.shell, "my_shell4")
        shell_response = my_shell.Execute(["free -m"])

        memory_list = re.findall('.*Mem:\s*([^\n\r]*)', shell_response[const.STDOUT][0])[0].split( )

        total_memory = float(memory_list[0])
        used_memory = float(memory_list[1])
        free = 100 * (1 - used_memory/total_memory)

        asserts.assertLess(requirement, free, "The free memory is less than" + str(requirement) + "%")

        logging.info("The used memory is: " + str(used_memory) + " M")
        logging.info("The total memory is: " + str(total_memory) + " M")
        logging.info("Free memory is: {:.2f}".format(free) + "%")


    def get_data(self, cores):
        self.dut.shell.InvokeTerminal("data_shell")
        my_shell = getattr(self.dut.shell, "data_shell")
        shell_response = my_shell.Execute(["cat /proc/stat"])
        stats_data = io.StringIO(unicode(shell_response[const.STDOUT][0]))
        core_load = {}

        for core in range(cores):
            core_load[str(core)] = {}
            core_load[str(core)]['stats'] = [float(column) for column in stats_data.readline().strip().split()[1:]]
            core_load[str(core)]['idle'] = core_load[str(core)]['stats'][3]
            core_load[str(core)]['total'] = sum(core_load[str(core)]['stats'])

        return core_load


    def calc_load(self, core_first_data, core_last_data, cores):
        load = {}
        utilisation = []
        for core in range(cores):
            load[str(core)] = {}
            load[str(core)]['idle_delta'] = core_first_data[str(core)]['idle'] - core_last_data[str(core)]['idle']
            load[str(core)]['total_delta'] = core_first_data[str(core)]['total'] - core_last_data[str(core)]['total']

            utilisation.append(100.0 * (1.0 - load[str(core)]['idle_delta'] / load[str(core)]['total_delta']))

        return utilisation


if __name__ ==  "__main__":
    test_runner.main()
