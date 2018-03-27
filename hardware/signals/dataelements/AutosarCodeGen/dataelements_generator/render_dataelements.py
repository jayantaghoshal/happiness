# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import typing
from typing import List, Dict
import jinja2

from .model import DE_Element, DE_Type_Key, DE_BaseType, DE_Value, DE_Identical, DE_Boolean, \
    DE_Struct, DE_Array
from .render_cpp_basic import escape_cpp_comment_including_prefix_if_nonempty, create_cpp_brief_comment, create_cpp_brief_comment_if_nonempty, get_cpp_type


def render_dataelments(header: str, footer: str, all_de_elements: List[DE_Element], all_types: Dict[DE_Type_Key, DE_BaseType]):
    templateelements = []

    for de_de in all_de_elements:
        de_type = all_types[de_de.de_type_id]
        transfer = get_vsm_transfer(all_types, de_de, de_type)
        range_comment = de_type.desc.strip() if isinstance(de_type, (DE_Value, DE_Identical)) else ""

        templateelements.append({
            "de_dataelementname" : de_de.de_dataelementname,
            "is_insignal"        : de_de.is_insignal,
            "is_internal"        : de_de.is_internal,
            "isSignal"           : de_de.isSignal,
            "description"        : de_de.description,
            "rtename"            : de_de.rtename,
            "range_comment"      : range_comment,
            "transfer"           : transfer,
            "cpp_type"           : get_cpp_type(all_types[de_de.de_type_id])
        })

    templatedata = {
        "all_dataelements" : templateelements,
        "escape_cpp_comment_including_prefix_if_nonempty": escape_cpp_comment_including_prefix_if_nonempty,
        "create_cpp_brief_comment_if_nonempty" : create_cpp_brief_comment_if_nonempty
    }

    j2env = jinja2.Environment(
        loader=jinja2.FileSystemLoader('./dataelements_generator/templates/'),
    )
    dataElemetsCppStr = header + j2env.get_template("gen_dataelements.cpp.j2").render(**templatedata) + footer
    dataElementsHeaderStr = header + j2env.get_template("gen_dataelements.h.j2").render(**templatedata) + footer
    gen_vsm_all_dataelements_cpp = header + j2env.get_template("vsm_all_dataelements.inc.j2").render(**templatedata) + footer
    gen_vsm_inject_instance_cpp = header +  j2env.get_template("vsm_inject_instances.inc.j2").render(**templatedata) + footer
    gen_vsm_inject_switch_error_cpp = header + j2env.get_template("vsm_inject_switch_error.inc.j2").render(**templatedata) + footer
    gen_vsm_inject_switch_ok_cpp = header +  j2env.get_template("vsm_inject_switch_ok.inc.j2").render(**templatedata) + footer
    gen_vsm_inject_variable_cpp = header + j2env.get_template("vsm_inject_variables.inc.j2").render(
        **templatedata) + footer
    gen_vsm_sink_subscribe_cpp = header + j2env.get_template("vsm_sink_subscribe.inc.j2").render(**templatedata) + footer
    gen_vsm_sink_variable_cpp = header +  j2env.get_template("vsm_sink_variables.inc.j2").render(**templatedata) + footer


    return (dataElementsHeaderStr,
            dataElemetsCppStr,
            gen_vsm_inject_variable_cpp,
            gen_vsm_inject_instance_cpp,
            gen_vsm_inject_switch_ok_cpp,
            gen_vsm_inject_switch_error_cpp,
            gen_vsm_sink_variable_cpp,
            gen_vsm_sink_subscribe_cpp,
            gen_vsm_all_dataelements_cpp
            )


def get_vsm_transfer(all_types : Dict[DE_Type_Key, DE_BaseType], de_de: DE_Element, de_type: DE_BaseType):
    transfer = None  # type: typing.Any
    if isinstance(de_type, DE_Value):
        transfer = {
            "type": "scale",
            "no_scale": de_type.no_scale,
            "nr_of_bits": de_type.nr_of_bits,
            "is_signed": de_type.is_signed,
            "offset": de_type.offset,
            "scale": de_type.scale
        }
    elif isinstance(de_type, DE_Identical):
        transfer = {
            "type": "identical"
        }
    elif isinstance(de_type, DE_Boolean):
        transfer = {
            "type": "identical"
        }
    elif isinstance(de_type, DE_Struct):
        if not de_de.is_internal:
            transfer = {
                "type": "struct",
                "members": []
            }

            for e in sorted(de_type.children, key=lambda x: x.member_name):
                subType = all_types[e.de_type_id]
                rteattr = de_de.rte_attr_map[e.member_name]
                member_name = e.member_name
                if member_name == subType.de_type_name:
                    member_name += "_"

                if isinstance(subType, DE_Value):
                    member_transfer = {
                        "type": "scale",
                        "no_scale": subType.no_scale,
                        "nr_of_bits": subType.nr_of_bits,
                        "is_signed": subType.is_signed,
                        "offset": subType.offset,
                        "scale": subType.scale
                    }
                else:
                    member_transfer = {
                        "type": "identical"
                    }

                transfer["members"].append({
                    "rteattr": rteattr,
                    "membername": member_name,
                    "transfer": member_transfer
                })

        else:
            pass  # ????
    elif isinstance(de_type, DE_Array):
        transfer = {
            "type": "array"
        }
    else:
        transfer = {
            "type": "identical"
        }
    return transfer