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

    common_signals = []
    for signal in sorted(fr_and_lin19_signals, key=lambda s: s.name.lower()):
        if signal.name in rte_name_to_de_name and not isinstance(rte_name_to_de_type[signal.name], model.DE_Array):
            common_signals.append(signal)

    return (common_signals, rte_name_to_de_name, rte_name_to_de_type)


def render_signals(signals: List[fdx_description_file_parser.Item]):

    convstr = "        name_to_item_map = { i.name : i for i in self.signal_list }\n\n"
    for signal in signals:
        convstr += "        self.%s = %s(self, name_to_item_map[%s.fdx_name])\n" % (signal.name, signal.name, signal.name)
    convstr += "\n"

    return convstr


def render(signals: List[fdx_description_file_parser.Item],
           rte_name_to_de_name : Dict[str, str],
           rte_name_to_de_type : Dict[str, model.DE_BaseType]):

    convstr = ""

    for signal in signals:
        s = signal.name
        type = rte_name_to_de_type[s]

        if isinstance(type, model.DE_Array):
            continue # Unsupported, for now...

        convstr += py_comment(type.desc)
        convstr += "class %s:\n" % s
        convstr += "    de_name     = \"%s\"\n" % rte_name_to_de_name[s]
        convstr += "    fdx_name    = \"%s\"\n" % s
        convstr += "    fdx_groupid = %s\n\n" % signal.parent_group.group_id
        convstr += "    def __init__(self, signal_interface, item):\n"
        convstr += "        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None\n"
        convstr += "        self.signal_interface = signal_interface\n"
        convstr += "        self.item = item\n"

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
            convstr += "\n"
        elif isinstance(type, model.DE_Boolean):
            convstr += "    def r2p(raw):\n"
            convstr += "        return raw\n"
            convstr += "    def p2r(physical):\n"
            convstr += "        return physical\n"
        else:
            assert(False)

        if(isinstance(type, model.DE_Identical) or isinstance(type, model.DE_Value) or isinstance(type, model.DE_Boolean)):
            convstr+="""    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

"""
        elif isinstance(type, model.DE_Enum):
            convstr+="""
    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
"""
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

    os.makedirs(os.path.dirname(args.outputname), exist_ok=True)
    with open(args.outputname, "w", encoding="utf-8") as f:
        f.write("# Signal scaling database\n")
        f.write("# --- AUTO GENERATED ---\n")
        f.write("# Inputs: %s\n" %  " \n#    ".join(sys.argv))
        f.write("from fdx import fdx_client\n")
        f.write("from fdx import fdx_description_file_parser\n")
        f.write("from . import config\n")
        code = """
class FrSignalInterface:
    def __init__(self):
        (self.groups, self.sysvar_list, self.signal_list) = fdx_description_file_parser.parse(
                config.fdx_description_file_path)
        self.group_id_map = {g.group_id: g for g in self.groups}

        def data_exchange(self, group_id, data):
            group = self.group_id_map[group_id]
            group.receive_data(data)

        try:
            self.connection = fdx_client.FDXConnection(data_exchange, config.vector_fdx_ip, config.vector_fdx_port)
            self.connection.send_start()
            self.connection.confirmed_start()
        except:
            self.connection.close()
            raise


"""
        f.write(code)
        (common_signals, rte_name_to_de_name, rte_name_to_de_type) = get_common_signals(all_types, all_de_elements, signals)
        f.write(render_signals(common_signals))
        f.write(render(common_signals, rte_name_to_de_name, rte_name_to_de_type))


if __name__ == "__main__":
    main()