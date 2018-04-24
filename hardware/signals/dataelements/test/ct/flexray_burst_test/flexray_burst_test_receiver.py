#!/usr/bin/env python

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys
import logging
import time
from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

sys.path.append('/usr/local/lib/python2.7/dist-packages')
from generated.pyDataElements import FrSignalInterface

logger = logging.getLogger('flexray_test.test')
import vehiclehalcommon


nr_of_iterations = 1

class ComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        self.flexray = FrSignalInterface()
        logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')

    def tearDownClass(self):
        try:
            self.shell.Execute("pkill flexray_burst_sender")
            self.flexray.close()
        except Exception as e:
            logging.error("ERROR In teardown: %r" % e)

    def testFlexrayBurstSend(self):
        fr = self.flexray

        signals = [
            fr.RoadFricIndcnActvSts,
            fr.RoadSgnInfoActvSts,
            fr.SpdAlrmActvForRoadSgnInfoSts,
            fr.SpdCameraInfoSeldForRoadSgnInfoSts,
            fr.SteerAssiActvForAutDrvSts,
            fr.TrfcRglnSeldForRoadSgnInfoSts,
            fr.TrlrLampChkAutReqSts,
            fr.UnlckFbVisReqSts,
            fr.WiprReAutReqSts, # Note: This signal only has 320ms frequency, but good enought for this test
            fr.AccAdprSpdLimActvSts,
            fr.AccAdprTurnSpdActvSts,
            fr.CllsnFwdWarnActvSts,
        ]

        sleep_buckets = [20, 10, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0]
        nr_of_signals_buckets = range(1, len(signals))
        results = {t: {} for t in sleep_buckets}  # 2d dict with 1st key=burst_sleep_ms, 2nd key=nr_of_signals_to_burst

        all_fails = []
        for burst_sleep_ms in sleep_buckets:
            for nr_of_signals_to_burst in nr_of_signals_buckets:
                signals_to_test_in_this_iteration = signals[:nr_of_signals_to_burst]

                for i in range(nr_of_iterations):
                    logging.info('Burst sleep time: %d ms. Iteration %d/%d' % (burst_sleep_ms, i, nr_of_iterations))
                    logging.info('starting test program: flexray_test')
                    self.shell.Execute("/data/nativetest64/flexray_burst_test_sender %d 150 %d&" % (nr_of_signals_to_burst, burst_sleep_ms))


                    vehiclehalcommon.wait_for_signal(fr, fr.AntithftRednReq, 2, timeout_sec=5)    # Trigger signal that all signals reset to Off done
                    for s in signals_to_test_in_this_iteration:
                        asserts.assertEqual(s.get(), fr.RoadFricIndcnActvSts.map.Off, "%s was not set to 0 during init" % s.de_name)
                    # Target binary will sleep for 2 seconds here

                    vehiclehalcommon.wait_for_signal(fr, fr.AntithftRednReq, 3, timeout_sec=5)    # Trigger signal that all signals set to On done
                    logging.info("Signals after burst:")
                    fails = []
                    for s in signals_to_test_in_this_iteration:
                        received_value = s.get()
                        if received_value != fr.RoadFricIndcnActvSts.map.On:
                            fails.append("%s was not set to 1 during burst with burst_sleep=%d, nr_of_signals=%d , received value=%d" %
                                         (s.de_name, burst_sleep_ms, nr_of_signals_to_burst,  received_value))
                    all_fails.extend(fails)
                    results[burst_sleep_ms][nr_of_signals_to_burst] = len(fails)


        errorTable = " NUMBER OF FAILS:" + os.linesep
        errorTable += "".rjust(120, "-") + os.linesep
        errorTable += "burst_sleep_ms | ".rjust(20) + " | ".join("%3d" % t for t in sleep_buckets) + os.linesep
        errorTable += "nr_of_signals |".rjust(20).ljust(120, "-") + os.linesep
        for n in nr_of_signals_buckets:
            errorTable += (("%3d" % n) + " | ").rjust(20) + " | ".join("%3d" % results[t][n] for t in sleep_buckets) + os.linesep
        errorTable += "".rjust(120, "-") + os.linesep
        for f in all_fails:
            errorTable += f + os.linesep

        logger.info(errorTable)
        asserts.assertFalse(len(all_fails), errorTable)




if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()
