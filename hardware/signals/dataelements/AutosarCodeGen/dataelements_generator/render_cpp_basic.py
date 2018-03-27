# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from .model import DE_BaseType, DE_Value, DE_Boolean, DE_Identical


def escape_cpp_comment(text: str):
    s = text.strip()
    s = s.replace("*/", "* /")
    s = s.replace("\"", "")
    s = "\n * ".join((s.rstrip() for s in s.splitlines()))
    return s

def escape_cpp_comment_including_prefix_if_nonempty(text: str, prefix="\n *"):
    s = text.strip()
    if len(s) == 0:
        return ""
    s = s.replace("*/", "* /")
    s = s.replace("\"", "")
    s = "\n * ".join((s.rstrip() for s in s.splitlines()))
    s = prefix + s
    return s

def create_cpp_comment(text):
    return "/*" + escape_cpp_comment(text) + "*/"

def create_cpp_brief_comment(text):
    return "/*!<" + escape_cpp_comment(text) + "*/"

def create_cpp_brief_comment_if_nonempty(text):
    if text.strip() == "":
        return ""
    return "/*!<" + escape_cpp_comment(text) + "*/"


def get_cpp_type(t : DE_BaseType):
    if isinstance(t, DE_Value) or isinstance(t, DE_Identical):
        return t.underlying_type
    if isinstance(t, DE_Boolean):
        return "bool"
    return t.de_type_name


