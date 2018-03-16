# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import keyword
import builtins
from .model import DE_BaseType, DE_Value, DE_Boolean, DE_Identical


def get_py_type(t: DE_BaseType):
    if isinstance(t, DE_Value) or isinstance(t, DE_Identical):
        # getIntTypeStr
        if "int" in t.underlying_type:
            return "int"
        else:
            return "float"
    if isinstance(t, DE_Boolean):
        return "bool"
    return t.de_type_name

def clean_member_name(member_name: str, typename: str):
    semi_clean_name = clean_variable_name(member_name)
    if semi_clean_name == typename:
        return semi_clean_name + "_"
    return semi_clean_name

def clean_variable_name(n: str):
    if keyword.iskeyword(n):
        return n + "_"
    if n in dir(builtins):
        return n + "_"
    return n
