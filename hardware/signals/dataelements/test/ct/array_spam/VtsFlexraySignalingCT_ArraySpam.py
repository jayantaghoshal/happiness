# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
import sys
import logging
import time
sys.path.append('/usr/local/lib/python2.7/dist-packages')
from generated.pyDataElements import FrSignalInterface

logger = logging.getLogger('flexray_test.test')

class ComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        self.flexray = FrSignalInterface()

    def tearDownClass(self):
        try:
            self.shell.Execute("pkill flexray_test")
            self.flexray.close()
        except Exception as e:
            logging.exception("ERROR In teardown: %r" % e)


    def testArraySpam(self):
        test_time = 30
        end_time = time.time() + test_time

        self.shell.Execute("/data/nativetest64/flexray_array_spam %d &" % test_time)
        time.sleep(5)  # Wait for target binary to start

        nr_of_ok_frames = 0
        prev = None
        while time.time() < end_time:
            received_array = self.flexray.DrvrHmiForHmiCen.get()
            if received_array == prev or received_array == 0:
                time.sleep(0.05)
                continue

            asserts.assertEqual(254, len(received_array), "Length of data not 254, data is: %r" % received_array, received_array)

            for i in range(len(received_array)-1):
                left = received_array[i]
                right = received_array[i+1]
                asserts.assertEqual(
                    right,
                    ((left + 1) % 256),
                    "Bytes in data not in expected sequence, %d+1 != %d. Data is: %r. nr_of_ok_frames: %d" % (left, right, received_array, nr_of_ok_frames))

            prev = received_array
            nr_of_ok_frames += 1
        asserts.assertLess(100, nr_of_ok_frames, "Didnt receive enough tests")


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()

