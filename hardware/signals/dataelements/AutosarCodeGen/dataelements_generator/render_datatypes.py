# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from typing import List, Dict, Set
import typing
from .model import DE_Enum, DE_Array, DE_Struct, DE_Type_Key, DE_BaseType, DE_Value, DE_Identical
from .render_cpp_basic import escape_cpp_comment_including_prefix_if_nonempty, get_cpp_type, create_cpp_brief_comment
import jinja2


def clean_member_name(member_name, type_name):
    if member_name == type_name:
        return member_name + "_"
    return member_name


def get_range_comment(member_type: DE_BaseType):
    if isinstance(member_type, DE_Value) or isinstance(member_type, DE_Identical):
        # Don't include the description of other types as that description
        # can be found by ctrl-clicking the type.
        # DE_Value and DE_Identical use simple types so they need the comment.
        return " " + create_cpp_brief_comment(member_type.desc)
    else:
        return ""

def sorted_types_by_typename(all_types: Dict[DE_Type_Key, DE_BaseType]):
    return list(sorted(all_types.values(), key=lambda x: x.de_type_name))


SHARED_TEMPLATEDATA = {
    "isinstance": isinstance,
    "get_cpp_type": get_cpp_type,
    "clean_member_name": clean_member_name,
    "escape_cpp_comment_including_prefix_if_nonempty": escape_cpp_comment_including_prefix_if_nonempty,
    "get_range_comment" : get_range_comment,
    # A bit ugly, these must be made available for isinstance inside templates
    "DE_Array": DE_Array,
    "DE_Struct": DE_Struct,
    "DE_Enum": DE_Enum,
}   # type: typing.Mapping[str, typing.Any]


def render_datatype(header: str,
                    footer: str,
                    enums : List[DE_Enum],
                    arrays: List[DE_Array],
                    structs: List[DE_Struct],
                    all_types: Dict[DE_Type_Key, DE_BaseType]):

    templatedata =  {
        **SHARED_TEMPLATEDATA,
        "all_types": all_types,
        "sorted_types": sorted_types_by_typename(all_types),
    }

    j2env = jinja2.Environment(
        loader=jinja2.FileSystemLoader('./dataelements_generator/templates/'),
        line_statement_prefix="$$"
    )
    return header + j2env.get_template("gen_datatypes.h.j2").render(**templatedata) + footer


def render_json(header,
                footer: str,
                arrays: List[DE_Array],
                structs: List[DE_Struct],
                all_types: Dict[DE_Type_Key, DE_BaseType]):

    # Can only have one template specialization of a specific toJson<std::array<T, N>>, hence we must prevent duplicates
    # This is also the reason we don't use the "using" alias of the std::array defined in gen_datatypes.h in the toJson<>
    already_rendered_arrays = set()  # type: Set[typing.Any]
    sorted_types_without_duplicate_arrays = []
    for t in sorted_types_by_typename(all_types):
        if isinstance(t, DE_Array):
            arraytype = (get_cpp_type(all_types[t.arrayitem_type_id]), t.max_elements)
            if arraytype in already_rendered_arrays:
                continue
            already_rendered_arrays.add(arraytype)

        sorted_types_without_duplicate_arrays.append(t)

    templatedata = {
        **SHARED_TEMPLATEDATA,
        "all_types": all_types,
        "sorted_types": sorted_types_without_duplicate_arrays,
    }

    j2env = jinja2.Environment(
        loader=jinja2.FileSystemLoader('./dataelements_generator/templates/'),
        line_statement_prefix="$$"
    )
    gen_jsonenc_h_contents =  header + j2env.get_template("gen_jsonencdec.h.j2").render(**templatedata) + footer
    gen_jsonenc_cpp_contents = header + j2env.get_template("gen_jsonencdec.cpp.j2").render(**templatedata) + footer
    return (gen_jsonenc_h_contents, gen_jsonenc_cpp_contents)

