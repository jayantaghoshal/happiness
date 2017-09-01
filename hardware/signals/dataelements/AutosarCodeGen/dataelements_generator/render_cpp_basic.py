from .model import DE_BaseType, DE_Value, DE_Boolean, DE_Identical


def escape_cpp_comment(text):
    s=text.replace("*/", "* /")
    s=s.replace("\n", "\n * ")
    s=s.replace("\"", "")
    return s

def create_cpp_comment(text):
    return "/*" + escape_cpp_comment(text) + "*/"

def create_cpp_brief_comment(text):
    return "/*!<" + escape_cpp_comment(text) + "*/"


def get_cpp_type(t : DE_BaseType):
    if isinstance(t, DE_Value) or isinstance(t, DE_Identical):
        return t.underlying_type
    if isinstance(t, DE_Boolean):
        return "bool"
    return t.de_type_name


