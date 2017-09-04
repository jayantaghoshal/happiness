#!/usr/bin/env python

import logging
import time
import sys
import os

import re

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.precondition import precondition_utils
from subprocess import call

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

    def testEth1_Startup_IpAddress(self):
        # Arrange
        invalid_ip_address = "198.18.34.6"
        self._kill_netman()
        resulting_ip_address = self._set_ip_address(self.ETH1, "198.18.34.6")

        # Act
        self._start_netman()

        # Assert
        asserts.assertEqual(resulting_ip_address, invalid_ip_address)

        ip_address = self._get_ip_address(self.ETH1)
        asserts.assertEqual(ip_address, self.ETH1_IP_ADDRESS)

    def testEth1_Startup_BroadcastAddress(self):
        # Arrange
        self._kill_netman()
        invalid_broadcast_address = "255.255.255.0"
        resulting_broadcast_address = self._set_broadcast_address(self.ETH1, invalid_broadcast_address)

        # Act
        self._start_netman()

        # Assert
        asserts.assertEqual(resulting_broadcast_address, invalid_broadcast_address)

        broadcast_address = self._get_broadcast_address(self.ETH1)
        asserts.assertEqual(broadcast_address, self.ETH1_BROADCAST_ADDRESS)


    def testEth1_Startup_Netmask(self):
        # Arrange
        invalid_netmask = "255.255.255.0"
        self._kill_netman()
        resulting_netmask = self._set_netmask(self.ETH1, invalid_netmask)

        # Act
        self._start_netman()

        # Assert
        asserts.assertEqual(resulting_netmask, invalid_netmask)

        netmask = self._get_netmask(self.ETH1)
        asserts.assertEqual(netmask, self.ETH1_NETMASK)

    def testEth1_Startup_Mtu(self):
        # Arrange
        invalid_mtu = 1000
        self._kill_netman()
        resulting_mtu = self._set_mtu(self.ETH1, 1000)

        # Act
        self._start_netman()

        # Assert
        asserts.assertEqual(resulting_mtu, invalid_mtu)

        mtu = self._get_mtu(self.ETH1)
        asserts.assertEqual(mtu, self.ETH1_MTU)

    def testEth1_Startup_MacAddress(self):
        # Arrange
        invalid_mac_address = "02:00:00:02:12:05"
        self._kill_netman()
        self._interface_down(self.ETH1)
        resulting_mac_address = self._set_mac_address(self.ETH1, invalid_mac_address)
        self._interface_up(self.ETH1)

        # Act
        self._start_netman()

        # Assert
        asserts.assertEqual(resulting_mac_address, invalid_mac_address)

        mac_address = self._get_mac_address(self.ETH1)
        asserts.assertEqual(mac_address, self.ETH1_MAC_ADDRESS)

    def testEth1_Runtime_IpAddress(self):
        # Arrange
        invalid_ip_address = "192.18.34.5"
        self._restart_netman()

        # Act
        self._set_ip_address(self.ETH1, invalid_ip_address)

        # Assert
        ip_address = self._get_ip_address(self.ETH1)
        asserts.assertEqual(ip_address, self.ETH1_IP_ADDRESS)

    def testEth1_RunTime_BroadcastAddress(self):
        # Arrange
        invalid_broadcast_address = "198.18.255.240"
        self._restart_netman()

        # Act
        self._set_broadcast_address(self.ETH1, invalid_broadcast_address)

        # Assert
        broadcast_address = self._get_broadcast_address(self.ETH1)
        asserts.assertEqual(broadcast_address, self.ETH1_BROADCAST_ADDRESS)

    def testEth1_Runtime_Netmask(self):
        # Arrange
        self._restart_netman()

        # Act
        self._set_netmask(self.ETH1, "255.255.255.0")

        # Assert
        netmask = self._get_netmask(self.ETH1)
        asserts.assertEqual(netmask, self.ETH1_NETMASK)

    def testEth1_Runtime_Mtu(self):
        # Arrange
        self._restart_netman()

        # Act
        self._set_mtu(self.ETH1, 1000)

        # Assert
        mtu = self._get_mtu(self.ETH1)
        asserts.assertEqual(mtu, self.ETH1_MTU)

    def testEth1_Runtime_MacAddress(self):
        # Arrange
        self._restart_netman()

        # Act
        self._interface_down(self.ETH1)
        self._set_mac_address(self.ETH1, "02:00:00:02:12:05")
        self._interface_up(self.ETH1)

        # Assert
        mac_address = self._get_mac_address(self.ETH1)
        asserts.assertEqual(mac_address, self.ETH1_MAC_ADDRESS)

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

    def _restart_netman(self):
        self._kill_netman()
        self._start_netman()

    def _kill_netman(self):
        self._execute_cmd("pkill netman")

    def _start_netman(self):
        self._execute_cmd("netman&")

    def _execute_cmd(self, cmd):
        output = self.terminal.Execute(cmd)
        logging.info(str(output[const.STDOUT]))
        return output

    def _get_interface_conf(self, interface):
        cmd = " ".join(["ifconfig", interface])
        ifconfig_output = self._execute_cmd(cmd)
        conf = {}
        conf["ip_address"] = self._parse_ip_address(ifconfig_output)
        conf["broadcast_address"] = self._parse_broadcast_address(ifconfig_output)
        conf["netmask"] = self._parse_netmask(ifconfig_output)
        conf["mtu"] = self._parse_mtu(ifconfig_output)
        conf["mac_address"] = self._parse_mac_address(ifconfig_output)
        logging.info(conf)
        return conf

    def _parse_ip_address(self, ifconfig_output):
        groups = re.search(r"inet addr:(\d{1,3}[.]\d{1,3}[.]\d{1,3}[.]\d{1,3})", str(ifconfig_output))
        return groups.group(1) if groups else ""

    def _parse_broadcast_address(self, ifconfig_output):
        groups = re.search(r"Bcast:(\d{1,3}[.]\d{1,3}[.]\d{1,3}[.]\d{1,3})", str(ifconfig_output))
        return groups.group(1) if groups else ""

    def _parse_netmask(self, ifconfig_output):
        groups = re.search(r"Mask:(\d{1,3}[.]\d{1,3}[.]\d{1,3}[.]\d{1,3})", str(ifconfig_output))
        return groups.group(1) if groups else ""

    def _parse_mtu(self, ifconfig_output):
        groups = re.search(r"MTU:(\d{1,4})", str(ifconfig_output))
        return int(groups.group(1)) if groups else None

    def _parse_mac_address(self, ifconfig_output):
        groups = re.search(r"HWaddr (\d{2}:\d{2}:\d{2}:\d{2}:\d{2}:\d{2})", str(ifconfig_output))
        return groups.group(1) if groups else ""

    def _set_broadcast_address(self, interface, broadcast_address):
        cmd = " ".join(["ifconfig", interface, "broadcast", broadcast_address])
        self._execute_cmd(cmd)
        return self._get_broadcast_address(interface)

    def _get_broadcast_address(self, interface):
        return self._get_interface_conf(interface)["broadcast_address"]

    def _set_netmask(self, interface, netmask):
        cmd = " ".join(["ifconfig", interface, "netmask", netmask])
        self._execute_cmd(cmd)
        return self._get_netmask(interface)

    def _get_netmask(self, interface):
        return self._get_interface_conf(interface)["netmask"]

    def _set_ip_address(self, interface, ip_address):
        cmd = " ".join(["ifconfig", interface, ip_address])
        self._execute_cmd(cmd)
        return self._get_ip_address(interface)

    def _get_ip_address(self, interface):
        return self._get_interface_conf(interface)["ip_address"]

    def _set_mtu(self, interface, mtu):
        cmd = " ".join(["ifconfig", interface, "mtu", str(mtu)])
        self._execute_cmd(cmd)
        return self._get_mtu(interface)

    def _get_mtu(self, interface):
        return self._get_interface_conf(interface)["mtu"]

    def _set_mac_address(self, interface, mac_address):
        cmd = " ".join(["ifconfig", interface, "hw ether", mac_address])
        self._execute_cmd(cmd)
        return self._get_mac_address(interface)

    def _get_mac_address(self, interface):
        return self._get_interface_conf(interface)["mac_address"]

    def _interface_down(self, interface):
        cmd = " ".join(["ifconfig", interface, "down"])
        self._execute_cmd(cmd)

    def _interface_up(self, interface):
        cmd = " ".join(["ifconfig", interface, "up"])
        self._execute_cmd(cmd)

if __name__ == "__main__":
    test_runner.main()
