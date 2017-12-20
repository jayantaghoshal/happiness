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
import subprocess

class GeneralPerformanceTest(base_test.BaseTestClass):
    """Running performance tests"""


    def testPerformance(self):
        d = os.path.dirname(__file__)

        abstemp = os.path.abspath("temp.txt")

        with open(abstemp, "w") as f:
            f.write(subprocess.check_output(
                ["python", "performance_eval.py"], cwd=d))
        subprocess.check_output(
                ["python", "generate_performance_report.py", abstemp], cwd=d)

if __name__ == "__main__":
    test_runner.main()
