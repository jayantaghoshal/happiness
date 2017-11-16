#!/usr/bin/env python

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
import sys
import logging
import time
sys.path.append('/usr/local/lib/python2.7/dist-packages')
from generated.pyDataElements import \
    FrSignalInterface, \
    AccAutResuWarnReq, \
    ClimaTmrStsRqrd

logger = logging.getLogger('flexray_test.test')


def wait_for_signal(signal, expectedvalue, timeout_sec, message):
    end = time.time() + timeout_sec
    value = signal.get()
    while time.time() < end:
        value = signal.get()
        if value == expectedvalue:
            logger.info("Got expected signal %s=%d" % (signal.de_name, value))
            break
        time.sleep(0.2)
    logger.error("Assert Expected=%d Actual=%d" % (expectedvalue, value))
    asserts.assertEqual(value, expectedvalue,
                        "Expected signal %s to be %d within %d sec, got %d. %s)" %
                        (signal.de_name, expectedvalue, timeout_sec, signal.get(), message))


class ComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

    def setUp(self):
        logger.info("Init FR")
        self.flexray = FrSignalInterface()

    def tearDown(self):
        logger.info('Teardown')
        try:
            self.shell.Execute("pkill flexray_test")
        except Exception as e:
            logger.exception("ERROR In teardown: %r" % e)
        self.flexray.close()

    def _runPingPongTest(self, iteration):
        logger.info('starting test program: flexray_test')
        self.shell.Execute("/data/local/tmp/flexray_test&")

        self.flexray.AccAutResuWarnReq.send(AccAutResuWarnReq.map.On)
        wait_for_signal(self.flexray.ClimaTmrStsRqrd, ClimaTmrStsRqrd.map.On, 10, "Iteration %d" % iteration)

        self.flexray.AccAutResuWarnReq.send(AccAutResuWarnReq.map.Off)
        wait_for_signal(self.flexray.ClimaTmrStsRqrd, ClimaTmrStsRqrd.map.Off, 10, "Iteration %d" % iteration)
        self.shell.Execute("pkill flexray_test")


    def testFlexrayPingPong(self):
        for i in range(5):
            logger.info('Iteration %d' % i)
            self._runPingPongTest(i)
            time.sleep(1)

    def testFlexrayPingPongAfterRestartOfVSD(self):
        for i in range(200):
            logger.info('Iteration %d' % i)
            self._runPingPongTest(i)

            # Restart VSD to ensure VSD and VIP can recover desip handshake
            self.shell.Execute("stop vehicle-signals-daemon")
            if i < 100:
                pass
            elif i < 150:
                time.sleep(1)   # 50*1=50sec
            else:
                time.sleep(3)   # 50*3=150sec, => total 200sec = 3,3min
            self.shell.Execute("start vehicle-signals-daemon")


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()
