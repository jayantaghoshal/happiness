#!/usr/bin/eniv python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

from volvocars.hardware.netman.test.ct.helpers.execution_context import (ExecutionContext, NamespaceExecutionContext)
from volvocars.hardware.netman.test.ct.helpers.network_helpers import PingTest


class VtsEthernetConnectivity_PingTest(base_test.BaseTestClass):

    def setUpClass(self):
       self.dut = self.registerController(android_device)[0]

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()

    def test_ToPingEthernetConnectivity(self):
        try:
            # Arrange
            ping_test = PingTest(NamespaceExecutionContext("vcc", self.terminal))

            hosts_to_test = [
                             dict(name="MOST",ip_address="198.18.2.18"),
                             dict(name="APIX",ip_address="198.18.24.1"),
                             dict(name="TCAM",ip_address="198.18.32.1"),
                             dict(name="VGM",ip_address="198.18.32.2"),
                             dict(name="VLAN_TCAM_0xAC",ip_address="198.19.101.65"),
                             dict(name="VLAN_TCAM_0xBA",ip_address="192.168.15.1"),
                             dict(name="VLAN_TCAM_0xBB",ip_address="198.19.101.129"),
                             dict(name="VLAN_TCAM_0xBC",ip_address="198.19.101.161"),
                             dict(name="VLAN_TCAM_0xBD",ip_address="198.19.101.193"),
                             dict(name="VLAN_TCAM_0xBE",ip_address="198.19.102.1"),
                             dict(name="VLAN_TCAM_0xCA",ip_address="198.19.102.65"),
                             dict(name="VLAN_VGM_0xAC",ip_address="198.19.101.67"),
                             dict(name="VLAN_VGM_0xCA",ip_address="198.19.102.67"),
                           ]

            # Act
            for host in hosts_to_test:
                ping_test.ping(host["ip_address"])

                # Assert
                asserts.assertTrue(ping_test.is_destination_reachable, "Failed to ping IP/VLAN "+ host["name"] +": "+ host["ip_address"])

        except Exception as e:
                asserts.fail("Exception thrown: {}".format(e))

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
