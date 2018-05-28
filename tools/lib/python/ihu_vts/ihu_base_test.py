#!/usr/bin/env python

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import json
import sys
from vts.runners.host import test_runner, base_test, const
import subprocess
import logging
sys.path.append('/usr/local/lib/python2.7/dist-packages')
from typing import Union, List, Dict#, Optional

_number = Union[int, float]
_kpi_value_type = Union[_number, List[Union[List[_number], _number]], str, List[str]]

"""The _writeResultsJsonString fix is needed to be able to see the number
of test cases that have been run without error. A virtual python
environment is created and stores the test_run_summary.json,
this modification copies it to /tmp outside of the virtual environment.
Patching test_runner to enable a correct test summary.
"""


def _writeResultsJsonString(self):
    """Writes out a json file with the test result info for easy parsing.
    """
    path2 = os.path.join(self.log_path, "test_run_summary.json")
    path = "/tmp/test_run_summary.json"
    with open(path, 'w') as f:
        f.write(self.results.jsonString())

    with open(path2, 'w') as f:
        f.write(self.results.jsonString())


class IhuBaseTestClass(base_test.BaseTestClass):
    def __init__(self, *args, **kwargs):
        self.vcc_kpis = {}  # type: Dict[str, _kpi_value_type]
        super(IhuBaseTestClass, self).__init__(*args, **kwargs)
        try:
            test_runner.TestRunner._writeResultsJsonString = _writeResultsJsonString    # type: ignore
        except Exception:
            print("*** Unable to patch test_runner.TestRunner._writeResultsJsonString ***")

    def setUpClass(self):
        self.dut = self.android_devices[0]
        self.dut.shell.InvokeTerminal("IhuBaseShell")

    def tearDownClass(self):
        try:
            if len(self.vcc_kpis) > 0:
                with open("/tmp/test_run_kpis.json", "w") as f:
                    json.dump(self.vcc_kpis, f)
        except:
            pass

        super(IhuBaseTestClass, self).tearDownClass()

    def executeInShell(self, command):
        return self.dut.shell.IhuBaseShell.Execute(command)

    def write_kpi(self, name, value, unit=None):
        # type: (str, _kpi_value_type, str) -> None
        self.vcc_kpis[name] = value

    def save_files(self, from_ihu_path, name):
        # files in this path will be picked up and and stored by the test runner later.
        to_ihu_path = os.path.join('/data/tmp_save_files/', name)

        command = 'mkdir -p {}'.format(to_ihu_path)
        self.dut.shell.IhuBaseShell.Execute(command)
        command = 'cp -r {} {}'.format(from_ihu_path, to_ihu_path)
        self.dut.shell.IhuBaseShell.Execute(command)

    def deviceReboot(self):
        self.executeInShell("reboot")
        self.dut.stopServices()
        self.dut.waitForBootCompletion()
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("IhuBaseShell")

    def set_carconfig_value(self, name, value):
        current = self.executeInShell("readcarconfig {}".format(name))[const.STDOUT][0].strip()
        try:
            int(current)
        except ValueError:
            raise Exception("Bad response from readcarconfig")
        if str(current) == str(value):
            return False
        else:
            self.executeInShell("changecarconfig {} {}".format(name, value))
            return True

    def set_carconfig(self, carconfig):
        change = False
        for config in carconfig:
            change |= self.set_carconfig_value(config[0], config[1])
        return change