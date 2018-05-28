# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import typing
from typing import Set, Mapping, Any, List
import datetime

class Capabilities:
    ihu_generic = "ihu-generic"
    # Access to CANoe simulation
    flexray     = "flexray"
    adb         = "adb"
    mp_serial   = "mp-serial"
    vip_serial  = "vip-serial"
    audio       = "audio"
    cem         = "cem"
    apix        = "apix"
    # Tests that are written with the python dataelements framework that can run in either
    # simulated mode through carsim_bridge or with canoe simulation.
    # They will be run twice, once on generic node with carsim and once on flexray node with canoe.
    flexray_or_carsim = "flexray_or_carsim"

standard_caps = {
    Capabilities.ihu_generic,
    Capabilities.adb,
    Capabilities.mp_serial,
    Capabilities.vip_serial
}

class TestFailedException(Exception):
    pass


class VtsTestFailedException(TestFailedException):
    def __init__(self, message):
        self.message = message


class IhuBaseTest:
    def __init__(self, require_capabilities):
        # type: (Set[str]) -> None
        self.require_capabilities = require_capabilities


class AndroidVTS(IhuBaseTest):
    def __init__(self, module_name, require_capabilities):
        super().__init__(require_capabilities)
        self.module_name = module_name

    def __str__(self):
        return self.module_name


class VTSTest(IhuBaseTest):
    def __init__(self, test_root_dir, require_capabilities, tests_to_run=None):
        # type: (str, Set[str], typing.Optional[typing.List[str]]) -> None
        super().__init__(require_capabilities)
        self.test_root_dir = test_root_dir  # Relative to AOSP root
        self.tests_to_run = tests_to_run

    def __str__(self):
        return self.test_root_dir


class TradefedTest(IhuBaseTest):
    def __init__(self, test_root_dir, require_capabilities):
        # type: (str, Set[str]) -> None
        super().__init__(require_capabilities)
        self.test_root_dir = test_root_dir  # Relative to AOSP root

    def __str__(self):
        return self.test_root_dir


class Disabled(IhuBaseTest):
    def __init__(self, disabled_test: IhuBaseTest, reason: str, jira_issue: str, deadline: str) -> None:
        super().__init__(disabled_test.require_capabilities)
        self.disabled_test = disabled_test
        self.reason = reason
        self.jira_issue = jira_issue
        self.deadline = datetime.datetime.strptime(deadline, "%Y-%m-%d")


class ResultData:
    def __init__(self,
                 name: str,
                 passed: bool,
                 results: List,
                 info: str,
                 console: str,
                 test_kpis: Mapping[str, Any],
                 logs: Mapping[str, str],
                 screenshot_paths: typing.List[str],
                 save_files_dir: typing.Optional[str]=None) -> None:
        self.name = name
        self.passed = passed
        self.results = results
        self.info = info
        self.console = console
        self.test_kpis = test_kpis
        self.logs = logs
        self.screenshot_paths = screenshot_paths
        self.save_files_dir = save_files_dir

    def __str__(self):
        if self.passed:
            return "PASSED"
        else:
            return "FAILED"
