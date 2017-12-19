#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
import os

from os import listdir
from os.path import join, isdir

from vts.runners.host import const

import re

from base_helper import BaseHelper

class NetmanHelper(BaseHelper):

    def __init__(self, terminal):
        BaseHelper.__init__(self, terminal)
        self.ip_netns_exec_vcc = ["ip netns exec vcc"]

    def start_netman(self):
        cmd = self._format_cmd_line(["netmand&"], True)
        self.execute_cmd(cmd)
        # TODO (Philip Werner):
        # Remove sleep in favour of a more reliable method when waiting for interface
        # to be ready
        time.sleep(2)

    def kill_netman(self):
        cmd = self._format_cmd_line(["pkill netmand"], True)
        self.execute_cmd(cmd)

    def restart_netman(self):
        self.kill_netman()
        self.start_netman()

    def get_interface_conf(self, interface, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["ifconfig", interface], run_in_vcc_namespace)
        ifconfig_output = self.execute_cmd(cmd)
        conf = {}
        conf["ip_address"] = self._parse_ip_address(ifconfig_output)
        conf["broadcast_address"] = self._parse_broadcast_address(ifconfig_output)
        conf["netmask"] = self._parse_netmask(ifconfig_output)
        conf["mtu"] = self._parse_mtu(ifconfig_output)
        conf["mac_address"] = self._parse_mac_address(ifconfig_output)
        logging.info(conf)
        return conf

    def set_broadcast_address(self, interface, broadcast_address, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["ifconfig", interface, "broadcast", broadcast_address], run_in_vcc_namespace)
        self.execute_cmd(cmd)
        return self.get_broadcast_address(interface)

    def get_broadcast_address(self, interface, run_in_vcc_namespace=True):
        return self.get_interface_conf(interface, run_in_vcc_namespace)["broadcast_address"]

    def set_netmask(self, interface, netmask, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["ifconfig", interface, "netmask", netmask], run_in_vcc_namespace)
        self.execute_cmd(cmd)
        return self.get_netmask(interface)

    def get_netmask(self, interface, run_in_vcc_namespace=True):
        return self.get_interface_conf(interface, run_in_vcc_namespace)["netmask"]

    def set_ip_address(self, interface, ip_address, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["ifconfig", interface, ip_address], run_in_vcc_namespace)
        self.execute_cmd(cmd)
        return self.get_ip_address(interface)

    def get_ip_address(self, interface, run_in_vcc_namespace=True):
        return self.get_interface_conf(interface, run_in_vcc_namespace)["ip_address"]

    def set_mtu(self, interface, mtu, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["ifconfig", interface, "mtu", str(mtu)], run_in_vcc_namespace)
        self.execute_cmd(cmd)
        return self.get_mtu(interface)

    def get_mtu(self, interface, run_in_vcc_namespace=True):
        return self.get_interface_conf(interface, run_in_vcc_namespace)["mtu"]

    def set_mac_address(self, interface, mac_address, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["ifconfig", interface, "hw ether", mac_address], run_in_vcc_namespace)
        self.execute_cmd(cmd)
        # TODO (Philip Werner):
        # Remove sleep in favour of a more reliable method when waiting for interface
        # to be ready
        time.sleep(5)
        return self.get_mac_address(interface)

    def get_mac_address(self, interface, run_in_vcc_namespace=True):
        return self.get_interface_conf(interface, run_in_vcc_namespace)["mac_address"]

    def interface_down(self, interface, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["ifconfig", interface, "down"], run_in_vcc_namespace)
        self.execute_cmd(cmd)

    def interface_up(self, interface, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["ifconfig", interface, "up"], run_in_vcc_namespace)
        self.execute_cmd(cmd)

    def is_interface_up(self, interface, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["cat", "/sys/class/net/{}/operstate".format(interface)], run_in_vcc_namespace)
        return self.execute_cmd(cmd) == "up"

    def get_sys_ctl_parameters(self, parameter, run_in_vcc_namespace=True):
        cmd = self._format_cmd_line(["cat", "/proc/sys/net/{}".format(parameter)], run_in_vcc_namespace)
        return int(self.execute_cmd(cmd)[const.STDOUT][0].rstrip())

    def get_prop(self, property_name):
        cmd = " ".join(["getprop", property_name])
        return self.execute_cmd(cmd)[const.STDOUT][0].rstrip()

    def set_prop(self, property_name, value):
        cmd = " ".join(["setprop", property_name, value])
        return self.execute_cmd(cmd)

    def get_tcam_interface_name(self, run_in_vcc_namespace=True):
        """
        """
        #
        # TODO: Hard coded device path for now
        #
        cmd = self._format_cmd_line(["ls", "/sys/devices/pci0000:00/0000:00:13.0/0000:01:00.0/net"], run_in_vcc_namespace)
        output = self.execute_cmd(cmd)
        return output[const.STDOUT][0].rstrip() # return first line with eol stripped

    def rename_interface(self, old_interface_name, new_interface_name, run_in_vcc_namespace=True):
        if self.is_interface_up(old_interface_name, run_in_vcc_namespace):
            raise ValueError("Error: Cannot rename {} when interface is up.".format(old_interface_name))
        cmd = self._format_cmd_line(["ip", "link", "set", old_interface_name, "name", new_interface_name], run_in_vcc_namespace)
        self.execute_cmd(cmd)

    def namespace_exists(self, namespace):
        cmd ="ip netns show"
        return namespace in self.execute_cmd(cmd)[const.STDOUT][0].split("\n")

    def _format_cmd_line(self, cmd, run_in_vcc_namespace):
        if run_in_vcc_namespace:
            return " ".join(self.ip_netns_exec_vcc + cmd)
        else:
            return cmd

    def exists_interface(self, interface_name):
        cmd = " ".join(["ip netns exec vcc grep", interface_name, "/proc/net/dev"])
        result = self.execute_cmd(cmd)
        return result[const.EXIT_CODE][0] == 0

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
