# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from typing import List, Dict
DE_Type_Key = str

"""
    Classes representing the bare minimum information required to generate DataElements.
    It is supposed to be agnostic from the language to generate for and independent from the arxml parsing.

    Types are represented by the DE_BaseType and its sub-classes.
    Each type has an string-ID of type DE_Type_Key which is usually its name,
    for simple float/int-types a UUID is generated as type name.

    When referring to other types, such as when a struct-type contains several members of other types, these references
    are always represented as ID instead of using pointers/reference.
    This is to simplify incremental parsing when the type that is a part of the struct might be generated later.

    The DE_Element class is representing the DataElement.
"""


class DE_EnumItem:
    def __init__(self, name: str, value: int) -> None:
        self.name = name
        self.value = value


class DE_StructMember:
    def __init__(self, type_id: DE_Type_Key, name: str) -> None:
        self.de_type_id = type_id
        self.member_name = name


class DE_BaseType:
    def __init__(self, de_type_name : DE_Type_Key) -> None:
        self.de_type_name = de_type_name

    @property
    def desc(self):
        return ""


class DE_Enum(DE_BaseType):
    def __init__(self, name: DE_Type_Key, description: str, values: List[DE_EnumItem]) -> None:
        super().__init__(name)
        self._description = description
        self.values = values

    @property
    def desc(self):
        return self._description


# Integer value without scaling/offset
class DE_Identical(DE_BaseType):
    def __init__(self,
                 name_uuid: DE_Type_Key,
                 underlying_type: str,
                 limit_min: float,
                 limit_max: float,
                 unitstr: str) -> None:
        super().__init__("IDENTICALTYPE:" + name_uuid)
        self.underlying_type = underlying_type
        self.limit_min = limit_min
        self.limit_max = limit_max
        self.unitstr = unitstr

    @property
    def desc(self):
        return "Unit: " + self.unitstr + ",  Range:"  + str(self.limit_min) + "->" + str(self.limit_max)


# Float value with scale/offset
class DE_Value(DE_BaseType):
    def __init__(self,
                 name_uuid: DE_Type_Key,
                 underlying_type: str,
                 is_signed: bool,
                 nr_of_bits: int,
                 scale: float,
                 offset: float,
                 limit_min: float,
                 limit_max: float,
                 unitstr: str,
                 no_scale: bool) -> None:
        super().__init__("VALUETYPE:" + name_uuid)
        self.underlying_type = underlying_type
        self.is_signed = is_signed
        self.nr_of_bits = nr_of_bits

        # TODO: Don't know if we should store this as string or as float (might loose precision if roundtripping str-float-str)?
        self.scale = scale
        self.offset = offset
        self.limit_min = limit_min
        self.limit_max = limit_max
        self.unitstr = unitstr
        self.no_scale = no_scale

    @property
    def desc(self):
        btstr = "raw is unsigned"
        if self.is_signed:
            btstr = "raw is signed"
        if self.nr_of_bits != "0":
            btstr += ", " + str(self.nr_of_bits) + " bits"

        return "Unit: " + self.unitstr + ",  Range:" + str(self.limit_min) + "->" + str(self.limit_max) + \
                        ", Resolution: (" + str(self.scale) + "*x+" + str(self.offset) + ", " + btstr + " )"


class DE_Boolean(DE_BaseType):
    def __init__(self) -> None:
        super().__init__("bool")


class DE_Array(DE_BaseType):
    def __init__(self, typename: DE_Type_Key, item_type: DE_Type_Key, max_elements: int, desc: str) -> None:
        super().__init__(typename)
        self.arrayitem_type_id = item_type
        self.max_elements = max_elements
        self._description = desc

    @property
    def desc(self):
        return self._description

class DE_Struct(DE_BaseType):
    def __init__(self, name: DE_Type_Key, desc: str, children: List[DE_StructMember]) -> None:
        super().__init__(name)
        self._description = desc
        self.children = children

    @property
    def desc(self):
        return self._description

class DE_Element:
    def __init__(self, type: DE_Type_Key, name: str, is_insignal: bool, is_internal: bool, description: str,
                 is_rte_signal: bool,
                 rte_name: str,
                 rte_attr_map: Dict[str, str]) -> None:
        self.de_type_id = type
        self.de_dataelementname = name      # This is the name you usually use to refer to the DataElement
        self.is_insignal = is_insignal
        self.is_internal = is_internal
        self.description = description
        self.isSignal = is_rte_signal
        self.rtename = rte_name
        self.rte_attr_map = rte_attr_map    # Map DE_Member.member_name to RTE-name
