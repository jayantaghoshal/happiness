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
from generated.pyDataElements import \
    FrSignalInterface, \
    UsgModSts

logger = logging.getLogger('flexray_test.test')


def wait_for_signal(signal, expectedvalue, timeout_sec):
    end = time.time() + timeout_sec
    value = signal.receive()
    while time.time() < end:
        value = signal.receive()
        if value == expectedvalue:
            log("Got expected signal %s=%d" % (signal.de_name, value))
            break
        time.sleep(0.2)
    log("Assert Expected=%d Actual=%d" % (expectedvalue, value))
    asserts.assertEqual(value, expectedvalue,
                        "Expected signal %s to be %d within %d sec, got %d)" %
                        (signal.de_name, expectedvalue, timeout_sec, signal.receive()))

#f = open("/tmp/vtsflexraytest", mode="w")
def log(s):
    logger.info(s)
    #f.write("%d - %s" % (time.time(), s) + "\n")
    #f.flush()

class ComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

    def setUp(self):
        self.dut.stopServices() #Important for startServices() to work further down

        # Starting CANoe simulation might trigger a reboot,
        # Set up required signals for unit to boot and ensure it is booted
        log("Init FR")
        self.flexray = FrSignalInterface()
        self.flexray.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        self.flexray.FRNetworkStatus.send(1)
        self.flexray.VehBattUSysU.send(12)
		
        log("Waiting for boot")
        self.dut.waitForBootCompletion()
        log("Boot completed")

        # After reboot VTS looses the connection to the shell, recreate it
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        log("Shell recreated")

    def tearDown(self):
        log('Teardown')
        try:
            self.shell.Execute("pkill flexray_test")
        except Exception as e:
            log("ERROR In teardown: %r" % e)
        self.flexray.close()

    '''test1: Send and receive a flexray signal
        Send one flexray signal, "signalToIhu" that will end up in a DEReceiver 
        in the test program flexray_test.cpp which will in turn send back "signalFromIhu" with the same value as the one sent.
    '''
    def testFlexray2(self):
        AccAutResuWarnReq  = self.flexray.AccAutResuWarnReq
        ClimaTmrStsRqrd = self.flexray.ClimaTmrStsRqrd

        log('starting test program: flexray_test')
        self.shell.Execute("/data/local/tmp/flexray_test&")


        AccAutResuWarnReq.send(AccAutResuWarnReq.map.On)
        wait_for_signal(ClimaTmrStsRqrd, ClimaTmrStsRqrd.map.On, timeout_sec=10)

        AccAutResuWarnReq.send(AccAutResuWarnReq.map.Off)
        wait_for_signal(ClimaTmrStsRqrd, ClimaTmrStsRqrd.map.Off, timeout_sec=10)

if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()
