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

from volvocars.hardware.netman.test.ct.helpers import netman_helper as nh

class VtsNetmandIpConfigurationComponentTest(base_test.BaseTestClass):

    ETH1="tcam0"
    ETH1_IP_ADDRESS="198.18.34.1"
    ETH1_BROADCAST_ADDRESS="198.18.255.255"
    ETH1_NETMASK="255.255.240.0"
    ETH1_MTU=1500
    ETH1_MAC_ADDRESS="02:00:00:02:12:01"

    METH0="meth0"
    METH0_IP_ADDRESS="198.18.2.1"
    METH0_BROADCAST_ADDRESS="198.18.255.255"
    METH0_NETMASK="255.255.240.0"
    METH0_MTU=1500
    METH0_MAC_ADDRESS="02:00:00:01:12:01"

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()
        self.target = nh.NetmanHelper(self.terminal)

    ## ----------------------------------------
    ## --/ TCAM (eth1) Interface Ip Configuration Tests /--
    ## ----------------------------------------

    def testEth1_Startup_IpAddress(self):
        # Arrange
        invalid_ip_address = "198.18.34.6"
        self.target.kill_netman()
        resulting_ip_address = self.target.set_ip_address(self.ETH1, "198.18.34.6")

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(resulting_ip_address, invalid_ip_address)

        ip_address = self.target.get_ip_address(self.ETH1)
        asserts.assertEqual(ip_address, self.ETH1_IP_ADDRESS)

    def testEth1_Startup_BroadcastAddress(self):
        # Arrange
        self.target.kill_netman()
        invalid_broadcast_address = "255.255.255.0"
        resulting_broadcast_address = self.target.set_broadcast_address(self.ETH1, invalid_broadcast_address)

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(resulting_broadcast_address, invalid_broadcast_address)

        broadcast_address = self.target.get_broadcast_address(self.ETH1)
        asserts.assertEqual(broadcast_address, self.ETH1_BROADCAST_ADDRESS)

    def testEth1_Startup_Netmask(self):
        # Arrange
        invalid_netmask = "255.255.255.0"
        self.target.kill_netman()
        resulting_netmask = self.target.set_netmask(self.ETH1, invalid_netmask)

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(resulting_netmask, invalid_netmask)

        netmask = self.target.get_netmask(self.ETH1)
        asserts.assertEqual(netmask, self.ETH1_NETMASK)

    def testEth1_Startup_Mtu(self):
        # Arrange
        invalid_mtu = 1000
        self.target.kill_netman()
        resulting_mtu = self.target.set_mtu(self.ETH1, 1000)

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(resulting_mtu, invalid_mtu)
        mtu = self.target.get_mtu(self.ETH1)
        asserts.assertEqual(mtu, self.ETH1_MTU)

    def testEth1_Startup_MacAddress(self):
        # Arrange
        invalid_mac_address = "02:00:00:02:12:05"
        logging.info("Setting incorrect mac address on tcam0 to: {}".format(invalid_mac_address))
        self.target.kill_netman()
        self.target.interface_down(self.ETH1)
        resulting_mac_address = self.target.set_mac_address(self.ETH1, invalid_mac_address)
        self.target.interface_up(self.ETH1)

        # Act
        logging.info("Starting netman")
        self.target.start_netman()

        # Assert
        logging.info("Asserting incorrect mac address was set to: {}".format(invalid_mac_address))
        asserts.assertEqual(resulting_mac_address, invalid_mac_address, "Incorrect MAC address")

        mac_address = self.target.get_mac_address(self.ETH1)
        logging.info("Asserting netman set correct mac address")
        asserts.assertEqual(mac_address, self.ETH1_MAC_ADDRESS, "Incorrect MAC address")

    ## ------------------------------------------
    ## --/ Netmand startup completed property /--
    ## ------------------------------------------

    def test_WhenStartupComplete_ShouldSetProperty(self):
        # Arrange
        self.target.kill_netman()
        self.target.set_prop("netmand.startup_completed", "0")

        # Act
        self.target.start_netman()

        # Assert
        startup_completed = self.target.get_prop("netmand.startup_completed")
        asserts.assertEqual(startup_completed, "1")

    ## ----------------------------------------
    ## --/ MOST (meth0) Interface Ip Configuration Tests /--
    ## ----------------------------------------

    ##
    ## TODO (Philip Werner): Enable these tests once the Most driver is operational
    ##
    """
    def testMeth0_Startup_IpAddress(self):
        # Arrange
        invalid_ip_address = "198.18.34.6"
        self.target.kill_netman()
        resulting_ip_address = self.target.set_ip_address(self.METH0, "198.18.34.6")

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(resulting_ip_address, invalid_ip_address)

        ip_address = self.target.get_ip_address(self.METH0)
        asserts.assertEqual(ip_address, self.METH0_IP_ADDRESS)


    def testMeth0_Startup_BroadcastAddress(self):
        # Arrange
        self.target.kill_netman()
        invalid_broadcast_address = "255.255.255.0"
        resulting_broadcast_address = self.target.set_broadcast_address(self.METH0, invalid_broadcast_address)

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(resulting_broadcast_address, invalid_broadcast_address)

        broadcast_address = self.target.get_broadcast_address(self.METH0)
        asserts.assertEqual(broadcast_address, self.METH0_BROADCAST_ADDRESS)


    def testMeth0_Startup_Netmask(self):
        # Arrange
        invalid_netmask = "255.255.255.0"
        self.target.kill_netman()
        resulting_netmask = self.target.set_netmask(self.METH0, invalid_netmask)

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(resulting_netmask, invalid_netmask)

        netmask = self.target.get_netmask(self.METH0)
        asserts.assertEqual(netmask, self.METH0_NETMASK)

    def testMeth0_Startup_Mtu(self):
        # Arrange
        invalid_mtu = 1000
        self.target.kill_netman()
        resulting_mtu = self.target.set_mtu(self.METH0, 1000)

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(resulting_mtu, invalid_mtu)

        mtu = self.target.get_mtu(self.METH0)
        asserts.assertEqual(mtu, self.METH0_MTU)

    def testMeth0_Startup_MacAddress(self):
        # Arrange
        invalid_mac_address = "02:00:00:02:12:05"
        self.target.kill_netman()
        self.target.interface_down(self.METH0)
        resulting_mac_address = self.target.set_mac_address(self.METH0, invalid_mac_address)
        self.target.interface_up(self.METH0)

        # Act
        self.target.start_netman()

        time.delay(5)
        # Assert
        asserts.assertEqual(resulting_mac_address, invalid_mac_address)

        mac_address = self.target.get_mac_address(self.METH0)
        asserts.assertEqual(mac_address, self.METH0_MAC_ADDRESS)

    def testMeth0_Runtime_IpAddress(self):
        # Arrange
        invalid_ip_address = "192.18.34.5"
        self.target.restart_netman()

        # Act
        self.target.set_ip_address(self.METH0, invalid_ip_address)

        # Assert
        ip_address = self.target.get_ip_address(self.METH0)
        asserts.assertEqual(ip_address, self.METH0_IP_ADDRESS)

    def testMeth0_RunTime_BroadcastAddress(self):
        # Arrange
        invalid_broadcast_address = "198.18.255.240"
        self.target.restart_netman()

        # Act
        self.target.set_broadcast_address(self.METH0, invalid_broadcast_address)

        # Assert
        broadcast_address = self.target.get_broadcast_address(self.METH0)
        asserts.assertEqual(broadcast_address, self.METH0_BROADCAST_ADDRESS)

    def testMeth0_Runtime_Netmask(self):
        # Arrange
        self.target.restart_netman()

        # Act
        self.target.set_netmask(self.METH0, "255.255.255.0")

        # Assert
        netmask = self.target.get_netmask(self.METH0)
        asserts.assertEqual(netmask, self.METH0_NETMASK)

    def testMeth0_Runtime_Mtu(self):
        # Arrange
        self.target.restart_netman()

        # Act
        self.target.set_mtu(self.METH0, 1000)

        # Assert
        mtu = self.target.get_mtu(self.METH0)
        asserts.assertEqual(mtu, self.METH0_MTU)

    def testMeth0_Runtime_MacAddress(self):
        # Arrange
        self.target.restart_netman()

        # Act
        self.target.interface_down(self.METH0)
        self.target.set_mac_address(self.METH0, "02:00:00:02:12:05")
        self.target.interface_up(self.METH0)

        # Assert
        mac_address = self.target.get_mac_address(self.METH0)
        asserts.assertEqual(mac_address, self.METH0_MAC_ADDRESS)
    """

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
