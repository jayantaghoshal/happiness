#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

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
import sys
sys.path.append('/usr/local/lib/python2.7/dist-packages')
import typing

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
            self.write_kpi("cpu_core_%d" % core, total_load[core], "%")
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
        shell_response = my_shell.Execute(["cat /proc/meminfo"])

        mem_free_kb = float(re.findall('MemFree:\s*(\d+)', shell_response[const.STDOUT][0])[0])
        buffers_kb = float(re.findall('Buffers:\s*(\d+)', shell_response[const.STDOUT][0])[0])
        cached_kb  = float(re.findall('Cached:\s*(\d+)', shell_response[const.STDOUT][0])[0])
        s_reclaimable_kb  = float(re.findall('SReclaimable:\s*(\d+)', shell_response[const.STDOUT][0])[0])
        total_mem_kb  = float(re.findall('MemTotal:\s*(\d+)', shell_response[const.STDOUT][0])[0])

        total_free_memory_kb = mem_free_kb + buffers_kb + cached_kb + s_reclaimable_kb
        free_percent = 100 * (total_free_memory_kb/total_mem_kb)

        asserts.assertLess(requirement, free_percent, "The free memory is less than" + str(requirement) + "%")

        used_mem_kb = total_mem_kb - total_free_memory_kb
        self.write_kpi("used_memory", used_mem_kb, "mb")
        self.write_kpi("total_memory", total_free_memory_kb, "kb")
        self.write_kpi("free_memory", free_percent, "%")
        logging.info("The free memory is: " + str(total_free_memory_kb) + " kB")
        logging.info("The used memory is: " + str(used_mem_kb) + " kB")
        logging.info("The total memory is: " + str(total_mem_kb) + " kB")
        logging.info("Free memory is: {:.2f}".format(free_percent) + "%")


    def testDisk(self):
        requirement = 95
        self.dut.shell.InvokeTerminal("my_shell5")
        my_shell = getattr(self.dut.shell, "my_shell5")
        shell_response = my_shell.Execute(["df"])
        number_of_disks = re.findall('dm\s*-\s*([^\n\r]*)', shell_response[const.STDOUT][0])
        disk_load = {}

        for disk in number_of_disks:
            disk_load[str(disk)] = {}
            disk_load[str(disk)]['usage'] = [x.strip() for x in str(disk).split()[1:]]

            #Size
            logging.info("Total size: " + str(disk_load[str(disk)]['usage'][0]))
            #Used
            logging.info("Used: " + str(disk_load[str(disk)]['usage'][1]))
            #Available
            logging.info("Available: " + str(disk_load[str(disk)]['usage'][2]))
            #Usage -%
            logging.info("Usage: " + str(disk_load[str(disk)]['usage'][3]))
            #Mount Location
            logging.info("Mount on: " + str(disk_load[str(disk)]['usage'][4]))

            if (float(disk_load[str(disk)]['usage'][3].strip('%'))) > requirement:
               asserts.assertLess(float(disk_load[str(disk)]['usage'][3].strip('%')), requirement, "The disk usage is over " + str(requirement) + "%")

    def get_data(self, cores):
        self.dut.shell.InvokeTerminal("data_shell")
        my_shell = getattr(self.dut.shell, "data_shell")
        shell_response = my_shell.Execute(["cat /proc/stat"])
        stats_data = io.StringIO(unicode(shell_response[const.STDOUT][0]))
        stats_data.readline() #remove first line with total cpu load
        core_load = {} # type: typing.Any

        for core in range(cores):
            core_load[str(core)] = {}
            core_load[str(core)]['stats'] = [float(column) for column in stats_data.readline().strip().split()[1:]]
            core_load[str(core)]['idle'] = core_load[str(core)]['stats'][3]
            core_load[str(core)]['total'] = sum(core_load[str(core)]['stats'])

        return core_load


    def calc_load(self, core_first_data, core_last_data, cores):
        utilisation = [] # type: typing.List[float]
        for core in range(cores):
            idle_delta = core_first_data[str(core)]['idle'] - core_last_data[str(core)]['idle']
            total_delta = core_first_data[str(core)]['total'] - core_last_data[str(core)]['total']
            utilisation.append(100.0 * (1.0 - idle_delta / total_delta))

        return utilisation



if __name__ ==  "__main__":
    test_runner.main()
