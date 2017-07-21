from typing import List, Dict

from model import DE_Enum, DE_Array, DE_Struct, DE_Type_Key, DE_BaseType, DE_Value, DE_Identical
from render_cpp_basic import escape_cpp_comment, get_cpp_type, create_cpp_brief_comment


def render_datatype(header: str,
                    enums : List[DE_Enum],
                    arrays: List[DE_Array],
                    structs: List[DE_Struct],
                    all_types: Dict[DE_Type_Key, DE_BaseType]):
    dTStr = header + """#ifndef _DATATYPES_H
#define _DATATYPES_H

#include <array>

namespace autosar {\n
"""

    # TODO: Not sure why we output all enums first, i think it's legacy from when parsing and output was done in same phase
    #       I left it that way for easier diff of output files.

    for t in sorted(all_types.values(), key=lambda x:x.de_type_name):
        if isinstance(t, DE_Enum):
            if t.desc != "":
                dTStr += "/*!\n * \enum %s\n * %s\n */\n" % (t.de_type_name, escape_cpp_comment(t.desc.strip()))
            else:
                dTStr += "/*!\n * \enum %s\n */\n" % (t.de_type_name)
            dTStr += "enum class " + t.de_type_name + " {\n"
            for v in t.values:
                dTStr += "    " + v.name + "=" + str(v.value) + ",\n"
            dTStr += "};\n\n"

    for t in sorted(all_types.values(), key=lambda x: x.de_type_name):
        if isinstance(t, DE_Array):
            inner_type = all_types[t.arrayitem_type_id]
            if t.desc != "":
                dTStr += "/*!\n * \\brief array %s\n * %s\n */\n" % (t.de_type_name, escape_cpp_comment(t.desc.strip()))
            else:
                dTStr += "/*!\n * \\brief array %s\n */\n" % (t.de_type_name)

            arrayTypeStr = "std::array<%s,%d>" % (get_cpp_type(inner_type), t.max_elements)
            dTStr += "using " + t.de_type_name + " = " + arrayTypeStr + ";" + t.desc + "\n\n"
        if isinstance(t, DE_Struct):
            if t.desc != "":
                dTStr += "/*!\n * \struct %s\n * %s\n */\n" % (t.de_type_name, escape_cpp_comment(t.desc.strip()))
            else:
                dTStr += "/*!\n * \struct %s\n */\n" % (t.de_type_name)

            dTStr += "struct " + t.de_type_name + " {\n"

            for c in t.children:
                attrname = c.member_name
                member_type = all_types[c.de_type_id]
                member_type_name = get_cpp_type(member_type)
                if attrname == member_type_name:
                    attrname += "_"

                range_comment = ""
                if isinstance(member_type, DE_Value) or isinstance(member_type, DE_Identical):
                    # Don't include the description of other types as that description
                    # can be found by ctrl-clicking the type.
                    # DE_Value and DE_Identical use simple types so they need the comment.
                    range_comment = " " + create_cpp_brief_comment(member_type.desc)
                dTStr += "    " + member_type_name + " " + attrname + ";" + range_comment + "\n"

            dTStr += "};\n\n"

    dTStr += "} // end of namespace\n#endif"
    return dTStr