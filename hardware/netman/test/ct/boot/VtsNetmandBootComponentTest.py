#!/usr/bin/env python

import logging
import time
import sys
import os

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.precondition import precondition_utils
from subprocess import call

from ..helpers import netman_helper as nh

class VtsNetmandBootComponentTest(base_test.BaseTestClass):

    ETH1="tcam0"
    ETH1_IP_ADDRESS="198.18.34.1"
    ETH1_BROADCAST_ADDRESS="198.18.255.255"
    ETH1_NETMASK="255.255.240.0"
    ETH1_MTU=1500
    ETH1_MAC_ADDRESS="02:00:00:02:12:01"

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()
        self.helper = nh.NetmanHelper(self.terminal)

    def test_OnBoot_NetmandIsRunning(self):
        # Arrange

        # Act
        result = self.terminal.Execute("ps -A | grep netman")

        # Assert
        asserts.assertEqual(0, result[output.EXIT_CODE][0])

    def test_OnBoot_NetboydIsRunning(self):
        # Arrange

        # Act
        result = self.terminal.Execute("ps -A | grep netboyd")

        # Assert
        asserts.assertEqual(0, result[output.EXIT_CODE][0])

    ## ----------------------------------------
    ## --/ TCAM (eth1) Interface Ip Configuration Tests /--
    ## ----------------------------------------

    def test_OnBoot_Eth1HasCorrectIpAddress(self):
        # Assert
        ip_address = self.helper.get_ip_address(self.ETH1)
        asserts.assertEqual(ip_address, self.ETH1_IP_ADDRESS)

    def test_OnBoot_Eth1HasCorrectBroadcastAddress(self):
        # Assert
        broadcast_address = self.helper.get_broadcast_address(self.ETH1)
        asserts.assertEqual(broadcast_address, self.ETH1_BROADCAST_ADDRESS)

    def test_OnBoot_Eth1HasCorrectNetmask(self):
        # Assert
        netmask = self.helper.get_netmask(self.ETH1)
        asserts.assertEqual(netmask, self.ETH1_NETMASK)

    def test_OnBoot_Eth1HasCorrectMtu(self):
        # Assert
        mtu = self.helper.get_mtu(self.ETH1)
        asserts.assertEqual(mtu, self.ETH1_MTU)

    def test_OnBoot_Eth1HasCorrectMacAddress(self):
        # Assert
        mac_address = self.helper.get_mac_address(self.ETH1)
        asserts.assertEqual(mac_address, self.ETH1_MAC_ADDRESS)

    ## ----------------------------------------
    ## --/ Test properties on boot /--
    ## ----------------------------------------

    def test_OnBoot_NetmandShouldSetProperty(self):
        # Assert
        startup_completed = self.helper.get_prop("netmand.startup_completed")
        asserts.assertEqual(startup_completed, "1")

    def test_OnBoot_NetboydShouldSetProperty(self):
        # Assert
        startup_completed = self.helper.get_prop("netboyd.startup_completed")
        asserts.assertEqual(startup_completed, "1")

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
