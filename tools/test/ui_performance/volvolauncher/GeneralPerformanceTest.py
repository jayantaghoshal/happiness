#!/usr/bin/env python

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

class GeneralPerformanceTest(base_test.BaseTestClass):
    """Running performance tests"""

    def testPerformance(self):
        os.system("performance_eval.py > logoutput.txt")
        os.system("generate_performance_report.py logoutput.txt html")

if __name__ == "__main__":
    test_runner.main()
