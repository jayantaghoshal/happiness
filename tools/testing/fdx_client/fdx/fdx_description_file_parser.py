# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import struct
import threading
import xml.etree.ElementTree as ET
from typing import List, Tuple, Dict, Union


#https://docs.python.org/2/library/struct.html#format-characters
fdx_type_to_struct_map = {
    "int32": "i",
    "float": "d"    # TODO: There seems to be an error in the FDXDescriptionFileParser FDXWriter.py,
                    #       float should be 4 byte and double 8 byte
                    #       See CANoe_FDX_Protocol_EN.pdf section 2.2.2 Data Groups and Data Types
}

class Group():
    def __init__(self,
                 group_id, # type: int
                 name,     # type: str
                 size      # type: int
                 ):
        # type: (...) -> None
        self.group_id = group_id
        self.name = name
        self.size = size
        self.items = [] # type: List[Item]
        # Mutex used here because fdx-receiver thread calling receive_data usually not same as reader thread,
        # which is usually a test case calling Item.value()
        # Recursive because build_data reads i.value_raw which also grabs a lock
        self.mutex_lock = threading.RLock()

    def validate(self):
        summed_size = sum(i.size for i in self.items)
        if summed_size != self.size:
            raise Exception("Size of group not equals to sum(items in group), groupsize=%d, itemsize=%d"% (self.size, summed_size))

    def build_data(self):
        # type: () -> str
        data = bytearray(self.size)
        with self.mutex_lock:
            for i in self.items:
                if i.type == "bytearray":
                    len_bytes = struct.pack("I", len(i.value_raw))
                    to_send = len_bytes + bytearray(i.value_raw)
                    data[i.offset:(i.offset + i.size)] = to_send
                else:
                    item_data = struct.pack(fdx_type_to_struct_map[i.type], i.value_raw)
                    data[i.offset:(i.offset + i.size)] = item_data
        return str(data)

    def receive_data(self, data):
        # type: (List[int]) -> None
        with self.mutex_lock:
            for i in self.items:
                if i.type == "bytearray":
                    raw = data[i.offset:(i.offset + i.size)]
                    array_length =  struct.unpack("I", bytearray(raw[0:4]))[0]
                    i.value_raw = raw[4:4 + array_length]

                else:
                    (i.value_raw, ) = struct.unpack(fdx_type_to_struct_map[i.type], bytearray(data[i.offset:(i.offset + i.size)]))


class Item():
    def __init__(self,
                 parent_group,      # type: Group
                 name,              # type: str
                 msg_or_namespace,  # type: str
                 offset,            # type: int
                 size,              # type: int
                 datatype,          # type: str
                 is_raw,            # type: bool
                 bus_name           # type: str
                 ):
        # type: (...) -> None
        self.parent_group = parent_group
        self.name = name
        self.msg_or_namespace = msg_or_namespace
        self.offset = offset
        self.size = size
        self.type = datatype
        self.is_raw = is_raw   # If is_raw is false, then CANoe will handle scaling conversion
        # NOTE: Value can be either raw or physical depending on the FDXDescriptionFile
        self._value = 0        # type: Union[float, int, List[int]]
        self.bus_name = bus_name

    @property
    def value_raw(self):
        assert self.is_raw
        with self.parent_group.mutex_lock:
            return self._value

    @value_raw.setter
    def value_raw(self, value):
        assert self.is_raw
        with self.parent_group.mutex_lock:
            self._value = value

    @property
    def value_physical(self):
        assert not self.is_raw
        with self.parent_group.mutex_lock:
            return self._value

    @value_physical.setter
    def value_physical(self, value):
        assert not self.is_raw
        with self.parent_group.mutex_lock:
            self._value = value


def parse(filename):
    # type: (str) -> Tuple[List[Group], List[Item], List[Item]]
    all_groups = []
    signal_list = []
    sysvar_list = []

    root = ET.parse(filename)

    data_groups = root.findall("datagroup")
    for d in data_groups:
        g = Group(int(d.attrib["groupID"]), d.find("identifier").text, int(d.attrib["size"]))
        all_groups.append(g)

        items = d.findall("item")
        for i in items:
            sysvar = i.find("sysvar")
            signal = i.find("signal")

            if sysvar is not None:
                name = sysvar.attrib["name"]
                namespace = sysvar.attrib["namespace"]
                s = Item(g, name, namespace, int(i.attrib["offset"]), int(i.attrib["size"]), i.attrib["type"], True, None)
                g.items.append(s)
                sysvar_list.append(s)
            if signal is not None:
                name = signal.attrib["name"]
                msg = signal.attrib["msg"]
                scaling = signal.attrib["value"]
                is_raw = scaling == "raw"
                if not is_raw:
                    assert scaling == "physical", "Unrecognized value scaling %s " % scaling
                s = Item(g, name, msg, int(i.attrib["offset"]), int(i.attrib["size"]), i.attrib["type"], is_raw, signal.attrib["bus"])
                g.items.append(s)
                signal_list.append(s)
        g.validate()
    return (all_groups, sysvar_list, signal_list)
