#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
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
    interface_configs = [
            dict(name="apix0", ip_address="198.18.18.1", broadcast_address="198.18.255.255",
                    netmask="255.255.240.0", mtu=1500, mac_address="02:00:00:00:12:01"),
            dict(name="meth0", ip_address="198.18.2.1", broadcast_address="198.18.255.255",
                    netmask="255.255.240.0", mtu=1500, mac_address="02:00:00:01:12:01"),
            dict(name="tcam0", ip_address="198.18.34.1", broadcast_address="198.18.255.255",
                    netmask="255.255.240.0", mtu=1500, mac_address="02:00:00:02:12:01"),
            dict(name="tcam0_ac", ip_address="198.19.101.66", broadcast_address="198.19.101.95",
                    netmask="255.255.255.224", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=172,
                    vlan_egress_prio=3),
            dict(name="tcam0_ba", ip_address="192.168.15.2", broadcast_address="192.168.15.15",
                    netmask="255.255.255.240", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=186,
                    vlan_egress_prio=2),
            dict(name="tcam0_bb",  ip_address="198.19.101.130", broadcast_address="198.19.101.159",
                    netmask="255.255.255.224", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=187,
                    vlan_egress_prio=2),
            dict(name="tcam0_bc",  ip_address="198.19.101.162", broadcast_address="198.19.101.191",
                    netmask="255.255.255.224", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=188,
                    vlan_egress_prio=3),
            dict(name="tcam0_bd",  ip_address="198.19.101.194", broadcast_address="198.19.101.223",
                    netmask="255.255.255.224", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=189,
                    vlan_egress_prio=2),
            dict(name="tcam0_be", ip_address="198.19.102.2", broadcast_address="198.19.102.31",
                    netmask="255.255.255.224", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=190,
                    vlan_egress_prio=5),
            dict(name="tcam0_bf",  ip_address="198.19.102.33", broadcast_address="198.19.102.63",
                    netmask="255.255.255.224", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=191,
                    vlan_egress_prio=4),
            dict(name="tcam0_ca",  ip_address="198.19.102.66", broadcast_address="198.19.102.95",
                    netmask="255.255.255.224", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=202,
                    vlan_egress_prio=2),
            dict(name="tcam0_cd",  ip_address="198.19.102.161", broadcast_address="198.19.102.191",
                    netmask="255.255.255.224", mtu=1500, mac_address="02:00:00:02:12:01", vlan_id=205,
                    vlan_egress_prio=2)
        ]

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

    def test_Startup_InterfaceConfigs(self):
        for interface in self.interface_configs:
            self.ip_Addr_test(interface["name"], interface["ip_address"])
            self.broadcast_Test(interface["name"], interface["broadcast_address"])
            self.netmask_Test(interface["name"], interface["netmask"])
            self.mtu_Test(interface["name"], interface["mtu"])
            self.mac_Address_Test(interface["name"], interface["mac_address"])

    def test_Vlan_Id(self):
        vlan_interface_config = [interface for interface in self.interface_configs if "vlan_id" in interface.keys()]
        for interface in vlan_interface_config:
            self.vlan_Id_Test(interface["name"], interface["vlan_id"])

    def test_Vlan_Qos_Test(self):
        vlan_interface_config = [interface for interface in self.interface_configs if "vlan_id" in interface.keys()]
        for interface in vlan_interface_config:
            self.vlan_Egress_Qos_Test(interface["name"], interface["vlan_egress_prio"])

    ## ------------------------------------------
    ## --/ Netmand restart /--
    ## ------------------------------------------
    def test_Netman_Restart(self):
        # Arrange
        self.target.kill_netman()
        tcam0 = [item for item in self.interface_configs if item["name"] == "tcam0"]
        incorrect_ip = "198.18.34.6"
        if len(tcam0) > 0:
            resulting_ip_address = self.target.set_ip_address(tcam0[0]["name"], incorrect_ip, True)

            # Act
            self.target.start_netman()

            # Assert
            asserts.assertEqual(incorrect_ip, resulting_ip_address, "setting new ip for interface failed")
            ip_address = self.target.get_ip_address(tcam0[0]["name"], True)
            asserts.assertEqual(tcam0[0]["ip_address"], ip_address, "Incorrect ip configuration after netman restart")

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
