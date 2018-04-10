# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import typing
from typing import Set, Mapping, Any
import datetime

class Capabilities:
    ihu_generic = "ihu-generic"
    flexray     = "flexray"
    adb         = "adb"
    mp_serial   = "mp-serial"
    vip_serial  = "vip-serial"
    audio       = "audio"
    cem         = "cem"
    apix        = "apix"

standard_caps = {
    Capabilities.ihu_generic,
    Capabilities.adb,
    Capabilities.mp_serial,
    Capabilities.vip_serial
}

class TestFailedException(Exception):
    pass


class VtsTestFailedException(TestFailedException):
    def __init__(self, message, json_result=None, json_change_time=None):
        self.message = message
        self.json_result = json_result
        self.json_change_time = json_change_time


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
    def __init__(self, test_root_dir, require_capabilities):
        # type: (str, Set[str]) -> None
        super().__init__(require_capabilities)
        self.test_root_dir = test_root_dir  # Relative to AOSP root

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
                 passed: bool,
                 console: str,
                 json_result,
                 json_change_time,
                 test_kpis: Mapping[str, Any],
                 logs: Mapping[str, str]) -> None:
        self.passed = passed
        self.console = console
        self.json_result = json_result
        self.json_change_time = json_change_time
        self.test_kpis = test_kpis
        self.logs = logs
