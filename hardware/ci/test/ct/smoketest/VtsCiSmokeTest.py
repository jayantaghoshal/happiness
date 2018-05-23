#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import re
import io
import sys
from time import sleep
from vts.runners.host import asserts
from vts.runners.host import const
from vts.runners.host import test_runner
import ihu_base_test
sys.path.append('/usr/local/lib/python2.7/dist-packages')
import typing

# Any services listed here will not cause the test testCrashes to fail.
# All services, listed here or not will however be reported as kpi.
services_allowed_to_crash = [
    # None
]

class VtsCiSmokeTest(ihu_base_test.IhuBaseTestClass):
    """
       Smoke tests to run first to check the sanity of the device.
       The old smoke test requirements for earlier projects was at
       90 % load and 90 % free memory.
    """

    def testCarconfig(self):
        self.executeInShell("changecarconfig 148 2")
        self.deviceReboot()
        need_reboot = self.set_carconfig_value(148, 2)
        asserts.assertEqual(need_reboot, False, "Expected set_carconfig_value False on non-changed value.")
        need_reboot = self.set_carconfig_value(148, 3)
        asserts.assertEqual(need_reboot, True, "Expected set_carconfig_value True on changed value.")
        self.deviceReboot()
        set_value = self.executeInShell("readcarconfig 148")[const.STDOUT][0].strip()
        asserts.assertEqual(set_value, '3', "Failed to set carconfig value.")

    def testCpuLoadLong(self):
        requirement = 90
        #Before there is a clear indication of system boot completed,
        #give CPU load some time to avoid the load spike after ihu_update and boot.
        sleep(30) # wait a bit
        shell_response = self.executeInShell("cat /proc/cpuinfo")

        model_name = re.findall('model\s*name\s*:\s*([^\n\r]*)', shell_response[const.STDOUT][0])[0]
        number_of_cores = re.findall('cpu\s*cores\s*:\s*(\d+)', shell_response[const.STDOUT][0])[0]

        total_load = self.getCpuLoad(int(number_of_cores), 60)

        for core in range(int(number_of_cores)):
            logging.info("load in core" + str(core) + " = " + '%.1f%%' % total_load[core])
            self.write_kpi("cpu_core_%d" % core, total_load[core], "%")
            if total_load[core] > requirement:
                process_running = self.executeInShell(["top -n1"])
                logging.info("top -n1")
                logging.info(process_running[const.STDOUT][0])
                asserts.assertLess(total_load[core], requirement, "The load on the core is over " + str(requirement) + "%" + "\n" + process_running[const.STDOUT][0])

        logging.info("Cpu cores: " + number_of_cores)
        logging.info("model_name: " + model_name)

    def testBootchartBootTime(self):
        max_boot_time = 40  # s
        self.executeInShell("touch /data/bootchart/enabled")
        self.deviceReboot()
        shell_response = self.executeInShell("tail -20 /data/bootchart/proc_stat.log")
        boot_time = float(re.findall('\n(\d{3,5})\n', shell_response[const.STDOUT][0])[-1])
        boot_time = boot_time / 100.0
        logging.info("The bootchart boot time is: " + str(boot_time) + " s")

        self.write_kpi("bootchart_boot_time", boot_time, "s")

        self.saveFiles('/data/bootchart/', 'bootchart')

        asserts.assertLess(boot_time, max_boot_time, "Bootchart boot time longer than the maximum allowed {} s".format(max_boot_time / 100))
        self.executeInShell("rm /data/bootchart/*")

    def testCpuLoadShort(self):
        requirement = 90
        shell_response = self.executeInShell(["cat /proc/cpuinfo"])

        model_name = re.findall('model\s*name\s*:\s*([^\n\r]*)', shell_response[const.STDOUT][0])[0]
        number_of_cores = re.findall('cpu\s*cores\s*:\s*(\d+)', shell_response[const.STDOUT][0])[0]

        total_load = self.getCpuLoad(int(number_of_cores), 30)

        for core in range(int(number_of_cores)):
            logging.info("load in core" + str(core) + " = " + '%.1f%%' % total_load[core])
            self.write_kpi("cpu_core_%d" % core, total_load[core], "%")
            if total_load[core] > requirement:
                #Before there is a clear indication of system boot completed,
                #give CPU load another measurement to avoid the load spike after ihu_update and boot.

                total_load_secondtry = self.getCpuLoad(int(number_of_cores), 15)

                for core in range(int(number_of_cores)):
                    logging.info("load in core" + str(core) + " = " + '%.1f%%' % total_load_secondtry[core])
                    self.write_kpi("cpu_core_%d" % core, total_load_secondtry[core], "%")
                    if total_load_secondtry[core] > requirement:
                        process_running = self.executeInShell(["top -n1"])
                        logging.info("top -n1")
                        logging.info(process_running[const.STDOUT][0])
                        asserts.assertLess(total_load_secondtry[core], requirement, "The load on the core is over " + str(requirement) + "%" + "\n" + process_running[const.STDOUT][0])

        logging.info("Cpu cores: " + number_of_cores)
        logging.info("model_name: " + model_name)


    def testMemory(self):
        requirement = 10
        #requirement = 99
        shell_response = self.executeInShell(["cat /proc/meminfo"])

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
        requirement = 75
        number_of_disks = [] # type: typing.List[str]
        shell_response = self.executeInShell(["df","-h"])
        vendor = re.findall('([^\n]*/\s*vendor\s*)', shell_response[const.STDOUT][0])[0]
        number_of_disks.append(vendor)
        data = re.findall('([^\n]*/\s*data\s*)', shell_response[const.STDOUT][0])[0]
        number_of_disks.append(data)
        cache = re.findall('([^\n]*/\s*cache\s*)', shell_response[const.STDOUT][0])[0]
        number_of_disks.append(cache)
        disk_load = {}# type: typing.Dict[str, typing.Dict[str, typing.List[str]]]

        for disk in number_of_disks:
            disk_load[str(disk)] = {}
            disk_load[str(disk)]['usage'] = [x.strip() for x in str(disk).split()[1:]]
            #Logging
            logging.info("Size for " + str(disk_load[str(disk)]['usage'][4]) + ": "+ str(disk_load[str(disk)]['usage'][0])
                         + " Used: " + str(disk_load[str(disk)]['usage'][1])
                         + " Available: " + str(disk_load[str(disk)]['usage'][2])
                         + " Usage: " + str(disk_load[str(disk)]['usage'][3]))
            disk_usage_output = str(disk_load[str(disk)]['usage'][3])
            if disk_usage_output[-1] == "%":
                disk_usage_output = disk_usage_output[:-1]

            self.write_kpi("disk_usage" + str(disk_load[str(disk)]['usage'][4]), float(disk_usage_output), "%")

            if (float(disk_load[str(disk)]['usage'][3].strip('%'))) > requirement:
               asserts.assertLess(float(disk_load[str(disk)]['usage'][3].strip('%')), requirement, "The disk usage is over " + str(requirement) + "%")

    def testCrashes(self):
        def crash_allowed(process_name):
            if process_name in services_allowed_to_crash:
                return True
            return False

        shell_response = self.executeInShell(["ls /data/tombstones"])
        shell_response_stdout = shell_response[const.STDOUT][0].strip()
        tombstones_files = [x.strip() for x in shell_response_stdout.split("\n") if len(x) > 0]
        if len(tombstones_files) > 0:
            logging.info("Found tombstones")
        crashing_processes = []
        for t in tombstones_files:
            tombstone_output = self.executeInShell(["cat /data/tombstones/%s" %t ])[const.STDOUT][0]
            pidmatches = re.findall("^pid:.*>>>(.*)<<<.*$", tombstone_output, re.MULTILINE)
            if pidmatches:
                crashing_processes.extend(pidmatches)
            else:
                crashing_processes.append("???")
            logging.info("Tombstone: %s : %s" % (t, tombstone_output))

        self.write_kpi("crashing_services", crashing_processes)

        disallowed_crashes = [p for p in crashing_processes if not crash_allowed(p)]
        asserts.assertEqual(0, len(disallowed_crashes),
                            "No crashes are allowed, found tombstones in /data/tombstones on the device. "
                            "Crashing apps: [%s]. Full tombstone-details in logs" % ", ".join(crashing_processes))

    def getCpuLoad(self, number_of_cores, measure_time):
        first_cpu_data = self.get_data(int(number_of_cores))
        sleep(measure_time)
        second_cpu_data = self.get_data(int(number_of_cores))
        return self.calc_load(first_cpu_data, second_cpu_data, int(number_of_cores))


    def get_data(self, cores):
        shell_response = self.executeInShell(["cat /proc/stat"])
        stats_data = io.StringIO((shell_response[const.STDOUT][0]).decode('utf-8'))
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
