#!/usr/bin/env python
import os

from vts.runners.host import test_runner
from vts.runners.host import base_test


"""The _writeResultsJsonString fix is needed to be able to see the number
of test cases that have been run without error. A virtual python 
environment is created and stores the test_run_summary.json, 
this modification copies it to /tmp outside of the virtual environment.
Patching test_runner to enable a correct test summary.
"""


def _writeResultsJsonString(self):
    """Writes out a json file with the test result info for easy parsing.
    """
    path2 = os.path.join(self.log_path, "test_run_summary.json")
    path = "/tmp/test_run_summary.json"
    with open(path, 'w') as f:
        f.write(self.results.jsonString())

    with open(path2, 'w') as f:
        f.write(self.results.jsonString())


class IhuBaseTestClass(base_test.BaseTestClass):
    def __init__(self, *args, **kwargs):
        super(IhuBaseTestClass, self).__init__(*args, **kwargs)
        try:
            test_runner.TestRunner._writeResultsJsonString = _writeResultsJsonString
        except Exception:
            print("*** Unable to patch test_runner.TestRunner._writeResultsJsonString ***")
