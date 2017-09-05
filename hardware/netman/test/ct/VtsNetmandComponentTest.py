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

from helpers import netman_helper as nh

class VtsNetmandComponentTest(base_test.BaseTestClass):

    ETH1="eth1"
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

    def testEth1_Startup_IpAddress(self):
        # Arrange
        invalid_ip_address = "198.18.34.6"
        self.helper.kill_netman()
        resulting_ip_address = self.helper.set_ip_address(self.ETH1, "198.18.34.6")

        # Act
        self.helper.start_netman()

        # Assert
        asserts.assertEqual(resulting_ip_address, invalid_ip_address)

        ip_address = self.helper.get_ip_address(self.ETH1)
        asserts.assertEqual(ip_address, self.ETH1_IP_ADDRESS)

    def testEth1_Startup_BroadcastAddress(self):
        # Arrange
        self.helper.kill_netman()
        invalid_broadcast_address = "255.255.255.0"
        resulting_broadcast_address = self.helper.set_broadcast_address(self.ETH1, invalid_broadcast_address)

        # Act
        self.helper.start_netman()

        # Assert
        asserts.assertEqual(resulting_broadcast_address, invalid_broadcast_address)

        broadcast_address = self.helper.get_broadcast_address(self.ETH1)
        asserts.assertEqual(broadcast_address, self.ETH1_BROADCAST_ADDRESS)


    def testEth1_Startup_Netmask(self):
        # Arrange
        invalid_netmask = "255.255.255.0"
        self.helper.kill_netman()
        resulting_netmask = self.helper.set_netmask(self.ETH1, invalid_netmask)

        # Act
        self.helper.start_netman()

        # Assert
        asserts.assertEqual(resulting_netmask, invalid_netmask)

        netmask = self.helper.get_netmask(self.ETH1)
        asserts.assertEqual(netmask, self.ETH1_NETMASK)

    def testEth1_Startup_Mtu(self):
        # Arrange
        invalid_mtu = 1000
        self.helper.kill_netman()
        resulting_mtu = self.helper.set_mtu(self.ETH1, 1000)

        # Act
        self.helper.start_netman()

        # Assert
        asserts.assertEqual(resulting_mtu, invalid_mtu)

        mtu = self.helper.get_mtu(self.ETH1)
        asserts.assertEqual(mtu, self.ETH1_MTU)

    def testEth1_Startup_MacAddress(self):
        # Arrange
        invalid_mac_address = "02:00:00:02:12:05"
        self.helper.kill_netman()
        self.helper.interface_down(self.ETH1)
        resulting_mac_address = self.helper.set_mac_address(self.ETH1, invalid_mac_address)
        self.helper.interface_up(self.ETH1)

        # Act
        self.helper.start_netman()

        # Assert
        asserts.assertEqual(resulting_mac_address, invalid_mac_address)

        mac_address = self.helper.get_mac_address(self.ETH1)
        asserts.assertEqual(mac_address, self.ETH1_MAC_ADDRESS)

    def testEth1_Runtime_IpAddress(self):
        # Arrange
        invalid_ip_address = "192.18.34.5"
        self.helper.restart_netman()

        # Act
        self.helper.set_ip_address(self.ETH1, invalid_ip_address)

        # Assert
        ip_address = self.helper.get_ip_address(self.ETH1)
        asserts.assertEqual(ip_address, self.ETH1_IP_ADDRESS)

    def testEth1_RunTime_BroadcastAddress(self):
        # Arrange
        invalid_broadcast_address = "198.18.255.240"
        self.helper.restart_netman()

        # Act
        self.helper.set_broadcast_address(self.ETH1, invalid_broadcast_address)

        # Assert
        broadcast_address = self.helper.get_broadcast_address(self.ETH1)
        asserts.assertEqual(broadcast_address, self.ETH1_BROADCAST_ADDRESS)

    def testEth1_Runtime_Netmask(self):
        # Arrange
        self.helper.restart_netman()

        # Act
        self.helper.set_netmask(self.ETH1, "255.255.255.0")

        # Assert
        netmask = self.helper.get_netmask(self.ETH1)
        asserts.assertEqual(netmask, self.ETH1_NETMASK)

    def testEth1_Runtime_Mtu(self):
        # Arrange
        self.helper.restart_netman()

        # Act
        self.helper.set_mtu(self.ETH1, 1000)

        # Assert
        mtu = self.helper.get_mtu(self.ETH1)
        asserts.assertEqual(mtu, self.ETH1_MTU)

    def testEth1_Runtime_MacAddress(self):
        # Arrange
        self.helper.restart_netman()

        # Act
        self.helper.interface_down(self.ETH1)
        self.helper.set_mac_address(self.ETH1, "02:00:00:02:12:05")
        self.helper.interface_up(self.ETH1)

        # Assert
        mac_address = self.helper.get_mac_address(self.ETH1)
        asserts.assertEqual(mac_address, self.ETH1_MAC_ADDRESS)

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
