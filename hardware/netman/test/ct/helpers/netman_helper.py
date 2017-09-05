#!/usr/bin/env python

import logging
import time
import sys
import os

import re

from base_helper import BaseHelper

class NetmanHelper(BaseHelper):

    def __init__(self, terminal):
        BaseHelper.__init__(self, terminal)

    def start_netman(self):
        self.execute_cmd("netman&")

    def kill_netman(self):
        self.execute_cmd("pkill netman")

    def restart_netman(self):
        self.kill_netman()
        self.start_netman()

    def get_interface_conf(self, interface):
        cmd = " ".join(["ifconfig", interface])
        ifconfig_output = self.execute_cmd(cmd)
        conf = {}
        conf["ip_address"] = self._parse_ip_address(ifconfig_output)
        conf["broadcast_address"] = self._parse_broadcast_address(ifconfig_output)
        conf["netmask"] = self._parse_netmask(ifconfig_output)
        conf["mtu"] = self._parse_mtu(ifconfig_output)
        conf["mac_address"] = self._parse_mac_address(ifconfig_output)
        logging.info(conf)
        return conf

    def set_broadcast_address(self, interface, broadcast_address):
        cmd = " ".join(["ifconfig", interface, "broadcast", broadcast_address])
        self.execute_cmd(cmd)
        return self.get_broadcast_address(interface)

    def get_broadcast_address(self, interface):
        return self.get_interface_conf(interface)["broadcast_address"]

    def set_netmask(self, interface, netmask):
        cmd = " ".join(["ifconfig", interface, "netmask", netmask])
        self.execute_cmd(cmd)
        return self.get_netmask(interface)

    def get_netmask(self, interface):
        return self.get_interface_conf(interface)["netmask"]

    def set_ip_address(self, interface, ip_address):
        cmd = " ".join(["ifconfig", interface, ip_address])
        self.execute_cmd(cmd)
        return self.get_ip_address(interface)

    def get_ip_address(self, interface):
        return self.get_interface_conf(interface)["ip_address"]

    def set_mtu(self, interface, mtu):
        cmd = " ".join(["ifconfig", interface, "mtu", str(mtu)])
        self.execute_cmd(cmd)
        return self.get_mtu(interface)

    def get_mtu(self, interface):
        return self.get_interface_conf(interface)["mtu"]

    def set_mac_address(self, interface, mac_address):
        cmd = " ".join(["ifconfig", interface, "hw ether", mac_address])
        self.execute_cmd(cmd)
        return self.get_mac_address(interface)

    def get_mac_address(self, interface):
        return self.get_interface_conf(interface)["mac_address"]

    def interface_down(self, interface):
        cmd = " ".join(["ifconfig", interface, "down"])
        self.execute_cmd(cmd)

    def interface_up(self, interface):
        cmd = " ".join(["ifconfig", interface, "up"])
        self.execute_cmd(cmd)

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

