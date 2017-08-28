from argparse import ArgumentParser
from typing import List, Dict
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


def render(all_types: Dict[model.DE_Type_Key, model.DE_BaseType],
           all_de_elements: List[model.DE_Element],
           groups : List[fdx_description_file_parser.Group],
           signals: List[fdx_description_file_parser.Item]):

    rte_name_to_de_type = {}
    rte_name_to_de_name = {}

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
            assert(d.rtename is not None)
            assert(d.rte_attr_map is None)
            rte_name_to_de_type[d.rtename] = type
            rte_name_to_de_name[d.rtename] = d.de_dataelementname



    convstr = ""
    fr_and_lin19_signals = [s for s in signals if s.bus_name.lower() in ["backbone", "lin19"]]
    for signal in sorted(fr_and_lin19_signals,  key=lambda s: s.name.lower()):
        s = signal.name
        try:
            type = rte_name_to_de_type[s]
        except KeyError:
            # This is ok, because the FDXDescriptionFile contains signals from/to other ECUs
            continue
        if isinstance(type, model.DE_Array):
            continue # Unsupported, for now...

        convstr += py_comment(type.desc)
        convstr += "class %s:\n" % s
        convstr += "    de_name     = \"%s\"\n" % rte_name_to_de_name[s]
        convstr += "    fdx_name    = \"%s\"\n" % s
        convstr += "    fdx_groupid = %s\n" % signal.parent_group.group_id

        if isinstance(type, model.DE_Identical):
            convstr += "    min = %s\n" % type.limit_min
            convstr += "    max = %s\n" % type.limit_max
            convstr += "    def r2p(cls, raw):\n"
            convstr += "        return raw\n"
            convstr += "    def p2r(cls, physical):\n"
            convstr += "        assert(cls.min < physical < cls.max)\n"
            convstr += "        return physical\n"
        elif isinstance(type, model.DE_Value):
            convstr += "    min    = %s\n" % type.limit_min
            convstr += "    max    = %s\n" % type.limit_max
            convstr += "    scale  = %s\n" % type.scale
            convstr += "    offset = %s\n" % type.offset
            convstr += "    def r2p(cls, raw):\n"
            convstr += "        return (raw * cls.scale) + cls.offset\n"
            convstr += "    def p2r(cls, physical):\n"
            convstr += "        assert(cls.min < physical < cls.max)\n"
            convstr += "        return (physical - cls.offset) / cls.scale\n"
        elif isinstance(type, model.DE_Enum):
            convstr += "    class map:\n        "
            convstr += "\n        ".join( "%s = %s" %(clean_variable_name(v.name), v.value)  for v in type.values)
            convstr += "\n    \n"
        elif isinstance(type, model.DE_Boolean):
            convstr += "    def %s_r2p(raw):\n" % s
            convstr += "        return raw\n"
            convstr += "    def %s_p2r(physical):\n" % s
            convstr += "        return physical\n"
        else:
            assert(False)
        convstr+="\n"

    return convstr


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

    generated_output = render(all_types, all_de_elements, groups, signals)

    os.makedirs(os.path.dirname(args.outputname), exist_ok=True)
    with open(args.outputname, "w", encoding="utf-8") as f:
        f.write("# Signal scaling database\n")
        f.write("# --- AUTO GENERATED ---\n")
        f.write("# Inputs: %s\n" %  " \n#    ".join(sys.argv))
        f.write("\n\n")
        f.write(generated_output)



if __name__ == "__main__":
    main()