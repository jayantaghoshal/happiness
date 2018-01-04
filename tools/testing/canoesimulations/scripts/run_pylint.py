# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

""" Run pylint with the correct files and modules """

import os
import subprocess


def run_pylint(directory):
    """
    Find all .py files in directory and execute pylint with them as arguments

    :param directory: Top level directory that contain all .py files
    """
    all_py_files = []
    for root, _, files in os.walk(directory):
        if '__init__.py' in files:
            all_py_files += [root]
        else:
            py_files_in_root = [os.path.join(root, f) for f in files if f.endswith('.py')]
            all_py_files += py_files_in_root

    command = ['pylint'] + all_py_files
    subprocess.call(command)


if len(os.sys.argv) == 2:
    run_pylint(os.sys.argv[1])
else:
    run_pylint(os.getcwd())
