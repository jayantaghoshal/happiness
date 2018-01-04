# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from argparse import ArgumentParser
from typing import List, Dict, Tuple
import sys
import os
import keyword
import builtins
from fdx import fdx_description_file_parser

sys.path.append(os.path.join(
    os.path.realpath(os.path.dirname(__file__)),
    "..", "..", "..", "hardware", "signals", "dataelements", "AutosarCodeGen"))
from dataelements_generator import model
from dataelements_generator import getDatatypes
import autosar
import jinja2


def py_comment(s : str):
    s = s.strip()
    if len(s) == 0:
        return ""
    return "".join(("# " + line + "\n" for line in s.split("\n")))

def clean_variable_name(n:str):
    if keyword.iskeyword(n):
        return n + "_"
    if n in dir(builtins):
        return n + "_"
    return n

#Returns the common signals between the signalDB (all_de_elements) and fdx_description-file (signals)
def get_common_signals(all_types: Dict[model.DE_Type_Key, model.DE_BaseType],
                       all_de_elements: List[model.DE_Element],
                       signals: List[fdx_description_file_parser.Item]) -> Tuple[List[fdx_description_file_parser.Item], Dict[str, str], Dict[str, model.DE_BaseType]]:

    rte_name_to_de_type = {}  # type: Dict[str, model.DE_BaseType]
    rte_name_to_de_name = {}  # type: Dict[str, str]

    for d in all_de_elements:
        if d.is_internal:
            continue
        type = all_types[d.de_type_id]
        if isinstance(type, model.DE_Struct):
            for c in type.children:
                child_type = all_types[c.de_type_id]
                rtename = d.rte_attr_map[c.member_name]
                rte_name_to_de_type[rtename] = child_type
                rte_name_to_de_name[rtename] = d.de_dataelementname + "." + c.member_name
        else:
            assert (d.rtename is not None)
            assert (d.rte_attr_map is None)
            rte_name_to_de_type[d.rtename] = type
            rte_name_to_de_name[d.rtename] = d.de_dataelementname

    fr_and_lin19_signals = [s for s in signals if s.bus_name.lower() in ["backbone", "lin19"]]

    common_signals = [] # type: List[fdx_description_file_parser.Item]
    for signal in sorted(fr_and_lin19_signals, key=lambda s: s.name.lower()):
        name = clean_name(signal)
        if name in rte_name_to_de_name:
            common_signals.append(signal)

    return (common_signals, rte_name_to_de_name, rte_name_to_de_type)


def clean_name(signal: fdx_description_file_parser.Item):
    if signal.bus_name == "LIN19":
        return signal.name.replace("_LIN19", "")
    return signal.name

def unambiguate_name(s: fdx_description_file_parser.Item, all_signals):
    items_with_same_name = [x for x in all_signals if x.name == s.name]
    items_with_same_name_on_same_bus = [x for x in items_with_same_name if x.name == s.bus_name]
    # Don't add prefix for Backbone signals, we here declare that as the default. Unless there is ambiguity on that bus
    if len(items_with_same_name_on_same_bus) > 1:
        return s.name + "_" + s.msg_or_namespace
    elif s.bus_name != "Backbone" and len(items_with_same_name) > 1:
        return s.name + "_" + s.msg_or_namespace
    else:
        return s.name


def render(signals: List[fdx_description_file_parser.Item],
           rte_name_to_de_name : Dict[str, str],
           rte_name_to_de_type : Dict[str, model.DE_BaseType]):

    template_data = []

    for signal in signals:
        type = rte_name_to_de_type[clean_name(signal)]

        template_data.append({
            "unambiguous_name": unambiguate_name(signal, signals),
            "de_name" : rte_name_to_de_name[clean_name(signal)],
            "fdx_name" : signal.name,
            "fdx_groupid" : signal.parent_group.group_id,
            "fdx_message_or_namespace" : signal.msg_or_namespace,
            "type" : rte_name_to_de_type[clean_name(signal)],
            "is_boolean": isinstance(type, model.DE_Boolean),
            "is_array" : isinstance(type, model.DE_Array),
            "is_intvalue" : isinstance(type, model.DE_Identical),
            "is_floatvalue" : isinstance(type, model.DE_Value),
            "is_enum" : isinstance(type, model.DE_Enum)
        })
    return template_data



def main():
    parser = ArgumentParser()
    parser.add_argument('-o', '--out', dest='outputname', help='Output file name of generated file',
                        required=False, default="generated/pyDataElements.py")
    parser.add_argument('-s', '--swcinputfile', dest='swcinputfile', help='SWC input arxml, (Elektra export file)',
                        required=True)
    parser.add_argument('-c', '--cominputfile', dest='cominputfile', help='COM input arxml, (Elektra export file)',
                        required=True)
    parser.add_argument('-f', '--fdxdescriptionfile', dest='fdxdescriptionfile', help='FDX Description file',
                        required=True)
    args = parser.parse_args()

    swc_data = autosar.arxml.load(args.swcinputfile)
    comfile_data = autosar.arxml.load(args.cominputfile)
    (all_types, all_de_elements) = getDatatypes.parse(swc_data, comfile_data)

    (groups, sysvars, signals) = fdx_description_file_parser.parse(args.fdxdescriptionfile)

    os.makedirs(os.path.dirname(args.outputname), exist_ok=True)
    #TODO: Need to increase the frequency of some send_free_running_request


    fdx_path_relative_generated_script = os.path.relpath(args.fdxdescriptionfile, os.path.dirname(args.outputname))

    (common_signals, rte_name_to_de_name, rte_name_to_de_type) = get_common_signals(all_types, all_de_elements, signals)
    templateData = {
        "inputs": " \n#    ".join(sys.argv),
        "signal_list": render(common_signals, rte_name_to_de_name, rte_name_to_de_type),
        "fdx_description_file_path" : fdx_path_relative_generated_script
    }

    j2env = jinja2.Environment(
        loader=jinja2.FileSystemLoader('.'),
    )
    j2env.globals["clean_variable_name"] = clean_variable_name
    j2env.globals["py_comment"] = py_comment
    template = j2env.get_template("generator_template.py.j2")

    with open(args.outputname, "w", encoding="utf-8") as f:
        f.write(template.render(**templateData))


if __name__ == "__main__":
    main()
