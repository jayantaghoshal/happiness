# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import re


def is_assigned_variable_invalid(name):
    return (name.startswith("PRODUCT_") or
            name.startswith("ANDROID_"))

def lint_android_mk_file(filename):
    if "vendor/volvocars/device" in filename:
        return

    with open(filename, "r", encoding="utf-8") as f:
        file_contents = f.read()

    for match in re.finditer(r"([\w\{\}\$]+)\s*:=", file_contents):
        assigned_variable_name = match.group(1).strip()
        if is_assigned_variable_invalid(assigned_variable_name):
            yield "Error: %s. Not allowed to assign to variable %s" % (filename, assigned_variable_name)
