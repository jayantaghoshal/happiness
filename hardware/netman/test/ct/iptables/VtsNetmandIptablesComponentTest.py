#!/usr/bin/env python

import logging
import time
import re
import sys
import os

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.precondition import precondition_utils

from volvocars.hardware.netman.test.ct.helpers import netman_helper as nh

class VtsNetmandIptablesComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()
        self.target = nh.NetmanHelper(self.terminal)

    ## ----------------------------------------
    ## --/ Iptables Configuration Tests /--
    ## ----------------------------------------

    def _parse_iptables_filter_num_packets(self):
        cmd = " ".join(["ip netns exec vcc iptables", "-L", "-n", "-v"])
        result = self.target.execute_cmd(cmd)
        groups = re.findall(r"\s+(\d+)\s+\d+K?\s+ACCEPT\s+tcp\s+--\s+[meth0|tcam0]", str(result[const.STDOUT]))
        if groups:
            return [int(x[0]) for x in groups]
        else:
            return None

    def _parse_iptables_nat_num_packet(self):
        cmd = " ".join(["ip netns exec vcc iptables", "-t", "nat", "-L", "-n", "-v"])
        result = self.target.execute_cmd(cmd)
        groups = re.findall(r"\s+(\d+)\s+\d+K?\s+RETURN\s+all\s+--\s+\*\s+tcam0", str(result))
        return int(groups[0][0]) if groups else None

    def test_WhenIptablesRulesApplied_ShouldEstablishMeth0Eth1Connection(self):
        # Arrange

        """
        TODO (Philip Werner) Add this test when most driver is operational.
        """
        #is_meth0_available = self.target.exists_interface("meth0")
        #asserts.assertTrue(is_meth0_available, "Interface meth0 not available")

        is_eth1_available = self.target.exists_interface("tcam0")
        asserts.assertTrue(is_eth1_available, "Interface tcam0 not available")

        # Act
        received_filter_packets = self._parse_iptables_filter_num_packets()
        received_nat_packet = self._parse_iptables_nat_num_packet()

        # Assert
        asserts.assertNotEqual(None, received_filter_packets)

        """
        TODO (Abhi) Add this test when most driver is operational.
        """
        #asserts.assertNotEqual(0, received_filter_packets[0])
        #asserts.assertNotEqual(0, received_filter_packets[1])

        #asserts.assertNotEqual(0, received_nat_packet)

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
