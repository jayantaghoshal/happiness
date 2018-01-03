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
        if signal.name in rte_name_to_de_name:
            common_signals.append(signal)

    return (common_signals, rte_name_to_de_name, rte_name_to_de_type)


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

def render_signals(signals: List[fdx_description_file_parser.Item]):
    convstr = "\n"
    # wow, this is messy..
    convstr += "        name_to_item_map = { i.msg_or_namespace + '::' + i.name : i for i in self.signal_list }\n\n"
    for signal in signals:
        name = unambiguate_name(signal, signals)
        key = "'%s::' + %s.fdx_name" % (signal.msg_or_namespace, name)
        convstr += "        self.%s = %s(self, name_to_item_map[%s])\n" % (name, name, key)
    convstr += "\n"

    return convstr


def render(signals: List[fdx_description_file_parser.Item],
           rte_name_to_de_name : Dict[str, str],
           rte_name_to_de_type : Dict[str, model.DE_BaseType]):

    convstr = ""

    for signal in signals:
        name = unambiguate_name(signal, signals)
        type = rte_name_to_de_type[signal.name]

        convstr += py_comment(type.desc)
        convstr += "class %s:\n" % name
        convstr += "    de_name     = \"%s\"\n" % rte_name_to_de_name[signal.name]
        convstr += "    fdx_name    = \"%s\"\n" % signal.name
        convstr += "    fdx_groupid = %s\n\n" % signal.parent_group.group_id
        convstr += "    def __init__(self, signal_interface, item):\n"
        convstr += "        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None\n"
        convstr += "        self.signal_interface = signal_interface\n"
        convstr += "        self.item = item\n"

        if isinstance(type, model.DE_Identical):
            convstr += "    min = %s\n" % type.limit_min
            convstr += "    max = %s\n" % type.limit_max
            convstr += "    @classmethod\n"
            convstr += "    def r2p(cls, raw):\n"
            convstr += "        return raw\n"
            convstr += "    @classmethod\n"
            convstr += "    def p2r(cls, physical):\n"
            convstr += "        assert(cls.min <= physical <= cls.max)\n"
            convstr += "        return physical\n"
        elif isinstance(type, model.DE_Value):
            convstr += "    min    = %s\n" % type.limit_min
            convstr += "    max    = %s\n" % type.limit_max
            convstr += "    scale  = %s\n" % type.scale
            convstr += "    offset = %s\n" % type.offset
            convstr += "    @classmethod\n"
            convstr += "    def r2p(cls, raw):\n"
            convstr += "        return (raw * cls.scale) + cls.offset\n"
            convstr += "    @classmethod\n"
            convstr += "    def p2r(cls, physical):\n"
            convstr += "        assert(cls.min <= physical <= cls.max)\n"
            convstr += "        return (physical - cls.offset) / cls.scale\n"
        elif isinstance(type, model.DE_Enum):
            convstr += "    class map:\n        "
            convstr += "\n        ".join( "%s = %s" %(clean_variable_name(v.name), v.value)  for v in type.values)
            convstr += "\n"
        elif isinstance(type, model.DE_Boolean):
            convstr += "    @classmethod\n"
            convstr += "    def r2p(cls, raw):\n"
            convstr += "        return raw\n"
            convstr += "    @classmethod\n"
            convstr += "    def p2r(cls, physical):\n"
            convstr += "        return physical\n"
        elif isinstance(type, model.DE_Array):
            pass
        else:
            assert(False)

        if(isinstance(type, model.DE_Identical) or isinstance(type, model.DE_Value) or isinstance(type, model.DE_Boolean)):
            convstr+="""
    def set(self, value_physical):
        self.item.value_raw = self.p2r(value_physical)

    def send(self, value_physical):
        self.item.value_raw = self.p2r(value_physical)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group.group_id, self.item.parent_group.size, self.item.parent_group.build_data())
        self.signal_interface.logger.debug('send %s=%d',self.fdx_name, value_physical)

    def get(self):
        value = self.r2p(self.item.value_raw)
        self.signal_interface.logger.debug('get %s=%d',self.fdx_name, value)
        return value

"""
        elif isinstance(type, model.DE_Enum):
            convstr+="""
    def set(self, value_physical):
        self.item.value_raw = value_physical

    def send(self, value_physical):
        self.item.value_raw = value_physical
        self.signal_interface.connection.send_data_exchange(self.item.parent_group.group_id, self.item.parent_group.size, self.item.parent_group.build_data())
        self.signal_interface.logger.debug('send %s=%d',self.fdx_name, value_physical)

    def get(self):
        self.signal_interface.logger.debug('get %s=%d',self.fdx_name, self.item.value_raw)
        return self.item.value_raw
"""
        elif isinstance(type, model.DE_Array):
            convstr += """
    def set(self, value_physical):
        assert len(value_physical) == %d
        self.item.value_raw = value_physical

    def send(self, value_physical):
        assert len(value_physical) == %d
        self.item.value_raw = value_physical
        self.signal_interface.connection.send_data_exchange(self.item.parent_group.group_id, self.item.parent_group.size, self.item.parent_group.build_data())
        self.signal_interface.logger.debug('send %%s=%%d',self.fdx_name, value_physical)

    def get(self):
        self.signal_interface.logger.debug('get %%s=%%d',self.fdx_name, self.item.value_raw)
        return self.item.value_raw
            """ % (type.max_elements, type.max_elements)
        else:
            assert(False)
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
    #TODO: Need to increase the frequency of some send_free_running_request
    with open(args.outputname, "w", encoding="utf-8") as f:
        (common_signals, rte_name_to_de_name, rte_name_to_de_type) = get_common_signals(all_types, all_de_elements,
                                                                                        signals)

        code = """# Signal scaling database
# --- AUTO GENERATED ---
# Inputs: %(inputs)s

import os
import logging
import time
from fdx import fdx_client
from fdx import fdx_description_file_parser

# Dummy class used when no real FDX connection is used (debugging on host without any hardware)
class FDXDummyConnection:
    def send_data_exchange(self, a, b, c):
        pass
    def close(self):
        pass


ns_per_ms = 1000000
class FrSignalInterface:

    def __init__(self):

        self.connected = False
        self.logger = logging.getLogger(__name__)

        (self.groups, self.sysvar_list, self.signal_list) = fdx_description_file_parser.parse(
            os.environ.get('FDX_DESCRIPTION_FILE_PATH', os.path.dirname(__file__)+"/../../CANoe/SPA2210/FR_Body_LIN/FDXDescriptionFile.xml"))

        self.group_id_map = {g.group_id: g for g in self.groups}

        def data_exchange(group_id, data):
            group = self.group_id_map[group_id]
            group.receive_data(data)

        if "VECTOR_FDX_IP" in os.environ:
            try:
                self.connection = fdx_client.FDXConnection(
                    data_exchange,
                    os.environ['VECTOR_FDX_IP'],
                    int(os.environ.get('VECTOR_FDX_PORT', '2809')))
                self.connection.send_start()
                self.connection.confirmed_stop()    # Stop in case previous test failed to stop
                self.connection.confirmed_start()
                self.verify_simulation_version()
                groups_to_subscribe = [g for g in self.groups if "ihubackbone" in g.name.lower() or "ihulin19" in g.name.lower()]
                for g in groups_to_subscribe:
                    self.connection.send_free_running_request(g.group_id, fdx_client.kFreeRunningFlag.transmitCyclic, 500 * ns_per_ms, 0)
                self.connected = True
            except:
                self.connection.close()
                raise
        else:
            self.connection = FDXDummyConnection()
            self.logger.error("Environment variables VECTOR_FDX_PORT and/or VECTOR_FDX_IP not found, no connection to target")

        %(senders_and_receivers)s

    def verify_simulation_version(self):
        # SPA2210/FR_Body_LIN/SimulationDB/Simulation.vsysvar
        EXPECTED_VERSION = 2
        simulation_version = next((s for s in self.sysvar_list if s.name == "SimulationVersion"))
        deadline = time.time() + 20
        while simulation_version.value_raw != EXPECTED_VERSION and time.time() < deadline:
            self.connection.send_data_request(simulation_version.parent_group.group_id)
            time.sleep(0.5)
        if simulation_version.value_raw != EXPECTED_VERSION:
            raise Exception("Simulation version mismatch! CANoe simulation version=%%r, expected version=%%d" %% (simulation_version.value_raw, EXPECTED_VERSION))

    def close(self):
        if self.connected:
            self.connection.confirmed_stop()
            self.connection.close()

%(signal_classes)s
"""
        f.write(code % {
            "inputs": " \n#    ".join(sys.argv),
            "senders_and_receivers" : render_signals(common_signals),
            "signal_classes" : render(common_signals, rte_name_to_de_name, rte_name_to_de_type)
        })



if __name__ == "__main__":
    main()
