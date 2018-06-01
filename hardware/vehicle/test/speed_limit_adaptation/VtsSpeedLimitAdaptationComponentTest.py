#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import time
import sys

from vts.runners.host import asserts
from vts.runners.host import test_runner

sys.path.append('/usr/local/lib/python2.7/dist-packages')
from vf_common import vf_base_test
from vf_common.vf_vts_common import wait_for
from generated import datatypes as DE

timeout = 3

class VtsSpeedLimitAdaptationComponentTest(vf_base_test.VFBaseTest):

    # ----------------------------------------------------------------------------------------------------------
    # Car Config Unavailable
    # ----------------------------------------------------------------------------------------------------------
    def testSpeedLimitAdaptationCCDisabled(self):
        # Change Car Config 149 to 2, Car Config 36 to 2 and Car Config 23 to 3
        self.change_carconfig_and_reboot_if_needed([(149, 2),(36,2),(23,3)])

        self.fr.send_DrvrHmiSpdLimAdpnSts(DE.OnOff1.On)
        wait_for(lambda: self.fr.get_AccAdprSpdLimActv().Sts, DE.OnOff1.Off, timeout)

    # ----------------------------------------------------------------------------------------------------------
    # Car Config Available
    # ----------------------------------------------------------------------------------------------------------
    def testSpeedLimitAdaptationCCEnabled(self):
        # Change Car Config 149 to 2, Car Config 36 to 3 and Car Config 23 to 1
        self.change_carconfig_and_reboot_if_needed([(149, 2),(36,3),(23,1)])

        # ----------------------------------------------------------------------------------------------------------
        # Test DrvrHmiSpdLimnAdpnSts off
        # ----------------------------------------------------------------------------------------------------------

        self.fr.send_DrvrHmiSpdLimAdpnSts(DE.OnOff1.Off)
        wait_for(lambda: self.fr.get_AccAdprSpdLimActv().Sts, DE.OnOff1.Off, timeout)

        # ----------------------------------------------------------------------------------------------------------
        # Test DrvrHmiSpdLimnAdpnSts on
        # ----------------------------------------------------------------------------------------------------------

        self.fr.send_DrvrHmiSpdLimAdpnSts(DE.OnOff1.Off)
        wait_for(lambda: self.fr.get_AccAdprSpdLimActv().Sts, DE.OnOff1.Off, timeout)

if __name__ == "__main__":
    test_runner.main()
