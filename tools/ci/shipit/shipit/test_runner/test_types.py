from typing import Set
import datetime

class Capabilities:
    ihu_generic = "ihu-generic"
    flexray     = "flexray"
    adb         = "adb"
    mp_serial   = "mp-serial"
    vip_serial  = "vip-serial"

standard_caps = {
    Capabilities.ihu_generic,
    Capabilities.adb,
    Capabilities.mp_serial,
    Capabilities.vip_serial
}

class TestFailedException(Exception):
    pass


class IhuBaseTest:
    def __init__(self, require_capabilities):
        # type: (Set[str]) -> None
        self.require_capabilities = require_capabilities

class VTSTest(IhuBaseTest):
    def __init__(self, test_xml_path, require_capabilities):
        # type: (str, Set[str]) -> None
        super().__init__(require_capabilities)
        self.test_xml_path = test_xml_path  # Relative to AOSP root

    def __str__(self):
        return self.test_xml_path

class Disabled(IhuBaseTest):
    def __init__(self, disabled_test: IhuBaseTest, reason: str, jira_issue: str, deadline: str) -> None:
        super().__init__(disabled_test.require_capabilities)
        self.disabled_test = disabled_test
        self.reason = reason
        self.jira_issue = jira_issue
        self.deadline = datetime.datetime.strptime(deadline, "%Y-%m-%d")
