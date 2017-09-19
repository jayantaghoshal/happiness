#!/usr/bin/env python

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
import sys
import logging
from time import sleep
import time
sys.path.append('/usr/local/lib/python2.7/dist-packages')
from generated.pyDataElements import ClimaTmrStsRqrd, AccAutResuWarnReq, FrSignalInterface

logger = logging.getLogger('flexray_test.test')


def wait_for_signal(signal, expectedvalue, timeout_sec):
    end = time.time() + timeout_sec
    value = signal.receive()
    while time.time() < end:
        print("Value is %d", value)
        value = signal.receive()
        if value == expectedvalue:
            logger.info("Got expected signal %s=%d" % (signal.de_name, value))
            break
        time.sleep(0.2)
    asserts.assertEqual(value, expectedvalue,
                        "Expected signal %s to be %d within %d sec, got %d)" %
                        (signal.de_name, expectedvalue, timeout_sec, signal.receive()))


class ComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

    def setUp(self):
        # The environment variables VECTOR_FDX_PORT and VECTOR_FDX_IP must be set
        # if a real connection to the flexray bus is what you want
        # (which is needed to run these tests for real)
        self.flexray = FrSignalInterface()

    def tearDown(self):
        logger.info('Killing test program: flexray_test')
        self.shell.Execute("pkill flexray_test")
        self.flexray.close()

    '''test1: Send and receive a flexray signal
        Send one flexray signal, "signalToIhu" that will end up in a DEReceiver 
        in the test program flexray_test.cpp which will in turn send back "signalFromIhu" with the same value as the one sent.
    '''
    def testFlexray(self):
        signal_from_ihu = self.flexray.ClimaTmrStsRqrd
        signal_to_ihu = self.flexray.AccAutResuWarnReq
        
        logger.info('starting test program: flexray_test')
        self.shell.Execute("/data/local/tmp/flexray_test&")


        signal_to_ihu.send(AccAutResuWarnReq.map.Off)
        sleep(1)
        signal_to_ihu.send(AccAutResuWarnReq.map.On)
        wait_for_signal(signal_from_ihu, ClimaTmrStsRqrd.map.On, timeout_sec=5)

        signal_to_ihu.send(AccAutResuWarnReq.map.Off)
        wait_for_signal(signal_from_ihu, ClimaTmrStsRqrd.map.Off, timeout_sec=5)

if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()
