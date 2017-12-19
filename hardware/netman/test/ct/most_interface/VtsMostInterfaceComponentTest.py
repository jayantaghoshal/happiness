#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

from volvocars.hardware.netman.test.ct.helpers.execution_context import (ExecutionContext,
                                                                         NamespaceExecutionContext)
from volvocars.hardware.netman.test.ct.helpers.network_helpers import PingTest

class MostInterfaceComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()

    ## ----------------------------------------
    ## --/ MOST (meth0) Interface Ip Configuration Tests /--
    ## ----------------------------------------

    def test_WhenMeth0Configured_ShouldEnablePingToAUD(self):
        try:
            # Arrange
            ping_test = PingTest(NamespaceExecutionContext("vcc", self.terminal))
            aud_ip_address = "198.18.2.18"

            # Act
            ping_test.ping(aud_ip_address)

            # Assert
            asserts.assertTrue(ping_test.is_destination_reachable, "AUD unit not reachable by ping")
        except Exception as e:
            asserts.fail("Exception thrown: {}".format(e))

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
