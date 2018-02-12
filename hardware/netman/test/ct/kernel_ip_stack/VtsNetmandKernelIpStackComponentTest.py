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

class VtsNetmandKernelIpStackComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()
        self.target = nh.NetmanHelper(self.terminal)

    ## ----------------------------------------
    ## --/ Firewall Configuration Tests /--
    ## ----------------------------------------

    # TODO Parameter not available in namspaces, kernel patch not integrated into Android yet
    # def test_TcpWindowScaling_ShouldBeZero(self):
    #     # Act
    #     tcp_window_scaling = \
    #         int(self.target.get_sys_ctl_parameters("ipv4/tcp_window_scaling"))
    #
    #     # Assert
    #     asserts.assertEqual(0, tcp_window_scaling)

    def test_TcpWindowScaling_ShouldBeOne(self):
        # Act
        icmp_echo_ignore_broadcasts = \
            int(self.target.get_sys_ctl_parameters("ipv4/icmp_echo_ignore_broadcasts"))

        # Assert
        asserts.assertEqual(1, icmp_echo_ignore_broadcasts)

    def test_IcmpIgnoreBogusErrorResponses_ShouldBeOne(self):
        # Act
        icmp_ignore_bogus_error_responses = \
            int(self.target.get_sys_ctl_parameters("ipv4/icmp_ignore_bogus_error_responses"))

        # Assert
        asserts.assertEqual(1, icmp_ignore_bogus_error_responses)

    def test_AllRpFilter_ShouldBeOne(self):
        all_rp_filter = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/all/rp_filter"))

        asserts.assertEqual(1, all_rp_filter)

    def test_DefaultRpFilter_ShouldBeOne(self):
        default_rp_filter = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/default/rp_filter"))

        asserts.assertEqual(1, default_rp_filter)

    def test_IpForward_ShouldBeOne(self):
        ip_forward = \
            int(self.target.get_sys_ctl_parameters("ipv4/ip_forward"))

        asserts.assertEqual(1, ip_forward)

    def test_AllForward_ShouldBeOne(self):
        all_forwarding = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/all/forwarding"))

        asserts.assertEqual(1, all_forwarding)

    def test_DefaultForwarding_ShouldBeOne(self):
        default_forwarding = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/default/forwarding"))

        asserts.assertEqual(1, default_forwarding)

    def test_AllAcceptRedirects_ShouldBeZero(self):
        all_accept_redirects = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/all/accept_redirects"))

        asserts.assertEqual(0, all_accept_redirects)

    def test_DefaultAcceptRedirects_ShouldBeZero(self):
        default_accept_redirects = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/default/accept_redirects"))

        asserts.assertEqual(0, default_accept_redirects)

    def test_AllSecureRedirects_ShouldBeZero(self):
        all_secure_redirects = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/all/secure_redirects"))

        asserts.assertEqual(0, all_secure_redirects)

    def test_DefaultSecureRedirects_ShouldBeZero(self):
        default_secure_redirects = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/default/secure_redirects"))

        asserts.assertEqual(0, default_secure_redirects)

    # TODO Parameter not available in namspaces, kernel patch not integrated into Android yet
    # def test_TcpTimeStamps_ShouldBeZero(self):
    #     tcp_timestamps = \
    #         int(self.target.get_sys_ctl_parameters("ipv4/tcp_timestamps"))
    #
    #     asserts.assertEqual(0, tcp_timestamps)

    def test_AllSendRedirects_ShouldBeZero(self):
        all_send_redirects = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/all/send_redirects"))

        asserts.assertEqual(0, all_send_redirects)

    def test_DefaultSendRedirets_ShouldBeZero(self):
        default_send_redirects = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/default/send_redirects"))

        asserts.assertEqual(0, default_send_redirects)

    def test_AllAcceptSourceRoute_ShouldBeZero(self):
        all_accept_source_route = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/all/accept_source_route"))

        asserts.assertEqual(0, all_accept_source_route)

    def test_DefaultAcceptSourceRoute_ShouldBeZero(self):
        default_accept_source_route = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/default/accept_source_route"))

        asserts.assertEqual(0, default_accept_source_route)

    def test_ProxyArpEth1_ShouldBeOne(self):
        # Force interface up
        output = self.target.execute_cmd("ip netns exec vcc ifconfig tcam0 up")
        asserts.assertEqual(0, output['return_codes'][0])
        proxy_arp_eth1 = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/tcam0/proxy_arp"))

        asserts.assertEqual(1, proxy_arp_eth1)

    def test_ProxyArpMeth0_ShouldBeOne(self):
        # Force interface up
        output = self.target.execute_cmd("ip netns exec vcc ifconfig meth0 up")
        asserts.assertEqual(0, output['return_codes'][0])
        proxy_arp_meth0 = \
            int(self.target.get_sys_ctl_parameters("ipv4/conf/meth0/proxy_arp"))

        asserts.assertEqual(1, proxy_arp_meth0)

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
