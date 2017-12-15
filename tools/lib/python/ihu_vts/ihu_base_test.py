# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#!/usr/bin/env python
import os

from vts.runners.host import test_runner
from vts.runners.host import base_test
import json
import sys

sys.path.append('/usr/local/lib/python2.7/dist-packages')
import typing
from typing import Union, List, Optional, Dict

_number = Union[int, float]
_kpi_value_type = Union[_number, List[Union[List[_number], _number]]]

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
        self.vcc_kpis = {}  # type: Dict[str, _kpi_value_type]
        super(IhuBaseTestClass, self).__init__(*args, **kwargs)
        try:
            test_runner.TestRunner._writeResultsJsonString = _writeResultsJsonString    # type: ignore
        except Exception:
            print("*** Unable to patch test_runner.TestRunner._writeResultsJsonString ***")


    def tearDownClass(self):
        try:
            if len(self.vcc_kpis) > 0:
                with open("/tmp/test_run_kpis.json", "w") as f:
                    json.dump(self.vcc_kpis, f)
        except:
            pass

        super(IhuBaseTestClass, self).tearDownClass()


    def write_kpi(self, name, value, unit=None):
        # type: (str, _kpi_value_type, str) -> None
        self.vcc_kpis[name] = value
