# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os

import shutil


def find_tool(binary_name: str, environment_variable_with_path):
    tool_on_env = os.getenv(environment_variable_with_path, '')
    if tool_on_env != '':
        if os.path.isfile(tool_on_env):
            return os.path.abspath(tool_on_env)
        else:
            exc_msg = 'Provided path ({}={}) is not an existing file'.format(environment_variable_with_path,
                                                                             tool_on_env)
            raise Exception(exc_msg)

    tool_on_path = shutil.which(binary_name)
    if tool_on_path is not None:
        return tool_on_path
    else:
        raise Exception('Tool {} not found in PATH'.format(binary_name))
