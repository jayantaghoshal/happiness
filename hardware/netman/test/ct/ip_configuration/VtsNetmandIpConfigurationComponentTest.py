#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

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

    """ TODO: (Erik Dahlgren) Move to separate/external file?"""

    """Interface configurations: """

    APIX0="apix0"
    APIX0_IP_ADDRESS="198.18.18.1"
    APIX0_BROADCAST_ADDRESS="198.18.255.255"
    APIX0_NETMASK="255.255.0.0"
    APIX0_MTU=1500
    APIX0_MAC_ADDRESS="02:00:00:00:12:01"

    METH0="meth0"
    METH0_IP_ADDRESS="198.18.2.1"
    METH0_BROADCAST_ADDRESS="198.18.255.255"
    METH0_NETMASK="255.255.240.0"
    METH0_MTU=1500
    METH0_MAC_ADDRESS="02:00:00:01:12:01"

    TCAM0="tcam0"
    TCAM0_IP_ADDRESS="198.18.34.1"
    TCAM0_BROADCAST_ADDRESS="198.18.255.255"
    TCAM0_NETMASK="255.255.240.0"
    TCAM0_MTU=1500
    TCAM0_MAC_ADDRESS="02:00:00:02:12:01"

    TCAM0_AC ="tcam0_ac"
    TCAM0_AC_IP_ADDRESS="198.19.101.66"
    TCAM0_AC_BROADCAST_ADDRESS="198.19.101.95"
    TCAM0_AC_NETMASK="255.255.255.224"
    TCAM0_AC_MTU=1500
    TCAM0_AC_MAC_ADDRESS="02:00:00:02:12:01"
    TCAM0_AC_VLAN_ID=172
    TCAM0_AC_EGRESS_QOS = 3

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()
        self.target = nh.NetmanHelper(self.terminal)

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

    ## ----------------------------------------
    ## --/ Helper functions /--
    ## ---------------------------------------

    def ip_Addr_test(self, interface, valid_ip):
        # Act
        ip_address = self.target.get_ip_address(interface)

        # Assert
        asserts.assertEqual(ip_address, valid_ip)

    def broadcast_Test(self, interface, valid_broadcast_address):
        # Act
        broadcast_address = self.target.get_broadcast_address(interface)

        # Assert
        asserts.assertEqual(broadcast_address, valid_broadcast_address)

    def netmask_Test(self, interface, valid_netmask_address):
        # Act
        netmask = self.target.get_netmask(interface)

        # Assert
        asserts.assertEqual(netmask, valid_netmask_address)

    def mtu_Test(self, interface, valid_mtu):
        # Act
        mtu = self.target.get_mtu(interface)

        # Assert
        asserts.assertEqual(mtu, valid_mtu)

    def mac_Address_Test(self, interface, valid_mac_address):
        # Act
        mac_address = self.target.get_mac_address(interface)

        # Assert
        asserts.assertEqual(mac_address, valid_mac_address, "Incorrect MAC address")

    def vlan_Id_Test(self, interface, valid_vlan):
        # Act
        vlan_id = self.target.get_vlan_id(interface, True)

        # Assert
        asserts.assertEqual(valid_vlan, vlan_id, "VLAN id is not matching")

    def vlan_Egress_Qos_Test(self, interface, valid_qos):
        # Act
        vlan_qos = self.target.get_egress_qos(interface, True)

        #Assert
        asserts.assertEqual(valid_qos, vlan_qos)

    ## ----------------------------------------
    ## --/ TCAM (tcam0) Interface Ip Configuration Tests /--
    ## ---------------------------------------

    def testTcam0_Startup_IpAddress(self):
        self.ip_Addr_test(self.TCAM0, self.TCAM0_IP_ADDRESS)

    def testTcam0_Startup_BroadcastAddress(self):
        self.broadcast_Test(self.TCAM0, self.TCAM0_BROADCAST_ADDRESS)

    def testTcam0_Startup_Netmask(self):
        self.netmask_Test(self.TCAM0, self.TCAM0_NETMASK)

    def testTcam0_Startup_Mtu(self):
        self.mtu_Test(self.TCAM0, self.TCAM0_MTU)

    def testTcam0_Startup_MacAddress(self):
        self.mac_Address_Test(self.TCAM0, self.TCAM0_MAC_ADDRESS)

    ## ----------------------------------------
    ## --/ MOST (meth0) Interface Ip Configuration Tests /--
    ## ---------------------------------------

    def testMeth0_Startup_IpAddress(self):
        self.ip_Addr_test(self.METH0, self.METH0_IP_ADDRESS)

    def testMeth0_Startup_BroadcastAddress(self):
        self.broadcast_Test(self.METH0, self.METH0_BROADCAST_ADDRESS)

    def testMeth0_Startup_Netmask(self):
        self.netmask_Test(self.METH0, self.METH0_NETMASK)

    def testMeth0_Startup_Mtu(self):
        self.mtu_Test(self.METH0, self.METH0_MTU)

    def testMeth0_Startup_MacAddress(self):
        self.mac_Address_Test(self.METH0, self.METH0_MAC_ADDRESS)

    ## ----------------------------------------
    ## --/ APIX (apix0) Interface Ip Configuration Tests /--
    ## ---------------------------------------

    def testApix0_Startup_IpAddress(self):
        self.ip_Addr_test(self.APIX0, self.APIX0_IP_ADDRESS)

    def testApix0_Startup_BroadcastAddress(self):
        self.broadcast_Test(self.APIX0, self.APIX0_BROADCAST_ADDRESS)

    def testApix0_Startup_Netmask(self):
        self.netmask_Test(self.APIX0, self.APIX0_NETMASK)

    def testApix0_Startup_Mtu(self):
        self.mtu_Test(self.APIX0, self.APIX0_MTU)

    def testApix0_Startup_MacAddress(self):
        self.mac_Address_Test(self.APIX0, self.APIX0_MAC_ADDRESS)

    ## ----------------------------------------
    ## --/ VLAN AC(172) (tcam0_ac) Interface Ip Configuration Tests /--
    ## ----------------------------------------

    def testTcam0_AC_Startup_IpAddress(self):
        self.ip_Addr_test(self.TCAM0_AC, self.TCAM0_AC_IP_ADDRESS)

    def testTcam0_AC_Startup_BroadcastAddress(self):
        self.broadcast_Test(self.TCAM0_AC, self.TCAM0_AC_BROADCAST_ADDRESS)

    def testTcam0_AC_Startup_Netmask(self):
        self.netmask_Test(self.TCAM0_AC, self.TCAM0_AC_NETMASK)

    def testTcam0_AC_Startup_Mtu(self):
        self.mtu_Test(self.TCAM0_AC, self.TCAM0_AC_MTU)

    def testTcam0_AC_Startup_MacAddress(self):
        self.mac_Address_Test(self.TCAM0_AC, self.TCAM0_AC_MAC_ADDRESS)

    def testTCAM0_AC_Vlan_Id(self):
        self.vlan_Id_Test(self.TCAM0_AC, self.TCAM0_AC_VLAN_ID)

    def testTCAM0_AC_Vlan_Qos_Test(self):
        self.vlan_Egress_Qos_Test(self.TCAM0_AC, self.TCAM0_AC_EGRESS_QOS)

    ## ------------------------------------------
    ## --/ Netmand restart /--
    ## ------------------------------------------
    def test_Netman_Restart(self):
        # Arrange
        self.target.kill_netman()
        incorrect_ip = "198.18.34.6"
        resulting_ip_address = self.target.set_ip_address(self.TCAM0, incorrect_ip, True)

        # Act
        self.target.start_netman()

        # Assert
        asserts.assertEqual(incorrect_ip, resulting_ip_address, "setting new ip for interface failed")
        ip_address = self.target.get_ip_address(self.TCAM0, True)
        asserts.assertEqual(self.TCAM0_IP_ADDRESS, ip_address, "Incorrect ip configuration after netman restart")

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

if __name__ == "__main__":
    test_runner.main()
