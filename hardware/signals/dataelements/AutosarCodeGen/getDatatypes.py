#!/bin/python3
# Copyright 2017 Volvo Cars, All Rights Reserved.
# Volvo Cars Confidential
import codecs
import os
import uuid
from datetime import datetime
from argparse import ArgumentParser
from typing import Dict, List
import crc16
import autosar
import render_cpp_basic
import render_dataelements
import render_json
from autosar.components import AR_RPort
from model import DE_Type_Key, DE_BaseType, DE_Enum, DE_Struct, DE_Value, DE_Array, DE_StructMember, DE_Element, DE_Boolean, \
    DE_Identical, DE_EnumItem
from render_datatypes import render_datatype

VERSION = 'v1.0'
DESCRIPTION = 'C++ code generator for AUTOSAR'
COPYRIGHT = 'Copyright 2017 Delphi Technologies, Inc., All Rights Reserved. Delphi Confidential'

OUTPUT_FILE_ENCODING = "utf-8"



def new_key():
    return uuid.uuid4().hex


def parse_type(swc_data: autosar.arxml.ARXml,
               all_types: Dict[DE_Type_Key, DE_BaseType],
               typeT: autosar.components.ARDatatype,
               baseType: autosar.components.ARSwBaseType) -> DE_Type_Key:
    typeStr = typeT.shortname
    if typeStr in all_types:
        return typeStr

    if typeT.getCategory() == 'VALUE':
        if typeT.getCompuMethod().getCategory() == 'TEXTTABLE':
            return typeStr # Has been added in earlier step, TODO: Shall be done here instead
        elif typeT.getCompuMethod().getCategory() == 'LINEAR':
            numerator = typeT.getCompuMethod().getCompuNumerator()
            underlying_type = "double"
            unitref = typeT.getCompuMethod().getSubElem("UNIT-REF").text
            unitstr = swc_data.units[unitref].getSubElem("DISPLAY-NAME").text

            size = "0"
            is_signed = False
            if baseType is not None:
                if baseType.getBaseType() == '2C':
                    is_signed = True
                size = baseType.getBaseTypeLength()
            if size == 0 or size is None:
                raise Exception("Can this really happen???")

            lim = typeT.getDataConstraint().getLimits()
            if numerator[1] == '1.0':
                underlying_type = getIntTypeStr(lim)
                scale = 1
                offset = 0
            else:
                scale = numerator[1]
                offset = numerator[0]
            no_scale = (scale == "1.0" and offset == "0")
            t = DE_Value(new_key(), underlying_type, is_signed, int(size), float(scale), float(offset), lim[0], lim[1],
                         unitstr, no_scale)
            all_types[t.de_type_name] = t
            return t.de_type_name
        elif typeT.getCompuMethod().getCategory() == 'IDENTICAL':
            unitref = typeT.getCompuMethod().getSubElem("UNIT-REF").text
            unitstr = swc_data.units[unitref].getSubElem("DISPLAY-NAME").text
            lim = typeT.getDataConstraint().getLimits()
            underlying_type = getIntTypeStr(lim)

            t = DE_Identical(new_key(), underlying_type, lim[0], lim[1], unitstr)
            all_types[t.de_type_name] = t
            return t.de_type_name
        else:
            raise Exception(
                "Unhandled structure compuMethod category '" + typeT.getCompuMethod().getCategory() + "' for data-type/element " + d.shortname + "/" + r.shortname)
    elif typeT.getCategory() == 'BOOLEAN':
        return "bool"
    else:
        raise Exception(
            "Unhandled structure data-type category '" + r.getCategory() + "' for data-type/element " + d.shortname + "/" + r.shortname)


def parse(swc_data: autosar.arxml.ARXml, comfile_data):
    all_types = parse_datatypes(swc_data)
    all_de_elements = parse_dataelements(swc_data, comfile_data, all_types)
    return (all_types, all_de_elements)


def parse_datatypes(swc_data: autosar.arxml.ARXml) -> Dict[DE_Type_Key, DE_BaseType]:
    all_types = {}
    all_types["bool"] = DE_Boolean()

    def push_type(key, t):
        if key in all_types:
            raise Exception("Type already exists: %s", key)
        all_types[key] = t

    for k, d in sorted(swc_data.datatypes.items()):
        d = d
        """:type: autosar.components.ARDatatype""" # can also be subclasses ARRecordDatatype, ARArrayDatatype

        if d.getCategory() == 'VALUE':
            compuMethod = d.getCompuMethod()
            if compuMethod.getCategory() == 'LINEAR':
                pass
            elif compuMethod.getCategory() == 'IDENTICAL':
                pass
            elif compuMethod.getCategory() == 'TEXTTABLE':
                enumerations = compuMethod.getEnumerations()
                inverted_dict = dict([[v[0], k] for k, v in enumerations.items()])
                name_to_value_list = []
                for i in compuMethod.getValues():
                    ename = inverted_dict[str(i)].replace(compuMethod.shortname + '_','')
                    name_to_value_list.append(DE_EnumItem(ename, i))
                push_type(d.shortname, DE_Enum(d.shortname, d.getDesc(), name_to_value_list))
            else:
                raise Exception("Unhandled compuMethod category '" + compuMethod.getCategory() + "' for data-type " + d.shortname)
        elif d.getCategory() == 'BOOLEAN':
            pass
        elif d.getCategory() == 'STRUCTURE':
            d = d
            """:type: autosar.components.ARRecordDatatype"""

            struct_children = []
            for record_element in d.getElements():
                attrStr = record_element.shortname
                # This is tricky, the ARRecordElement has a SwBaseType but not the ArDatatype
                t = parse_type(swc_data, all_types, record_element.getDataType(), record_element.getSwBaseType())
                struct_children.append(DE_StructMember(t, attrStr))

            push_type(d.shortname, DE_Struct(d.shortname, d.getDesc(), struct_children))
        elif d.getCategory() == 'ARRAY':
            d = d
            """:type: autosar.components.ARArrayDatatype"""
            et = d.getElementType()
            baseType = None
            if d.getDataType().getCategory() == "VALUE" and d.getDataType().getCompuMethod() == "LINEAR":
                baseType = et.getSwBaseType()
            t = parse_type(swc_data, all_types, d.getDataType(), baseType)
            push_type(d.shortname, DE_Array(d.shortname, t, int(et["max_elems"]), d.getDesc()))
        else:
            raise Exception("Unhandled data-type category '" + d.getCategory() + "' for data-type " + d.shortname)
    return all_types


def parse_dataelements(swc_data : autosar.arxml.ARXml,
                       comfile_data,
                       all_types: Dict[DE_Type_Key, DE_BaseType]) -> List[DE_Element]:
    all_de_elements = []
    """:type: List[DE_Element]"""

    for k, swcs in swc_data.swcs.items():
        swcs = swcs
        """:type: autosar.components.ARSwc"""

        if swcs.shortname != "IHU":
            continue
        for k2, port in sorted(swcs.ports.items()):
            isInsignal = isinstance(port, AR_RPort)
            print("   RPort: " if isInsignal else "   PPort: " + port.shortname)
            elems = port.getDataElements()
            port_interface = port.arxml.port_interfaces[port.port_interface.text]
            for de, dname in elems.items():
                print("      " + de)

                desc = port_interface.getDesc()
                isExternal = de in comfile_data.groupmapping
                data_type = swc_data.datatypes.get(dname)

                rte_attr_map = None
                rte_is_signal = None
                rte_name = None
                if isExternal:
                    rtegroup = comfile_data.groupmapping[de]
                    rte_is_signal = rtegroup.isSignal
                    rte_name = rtegroup.rtename
                    if data_type.getCategory() == 'STRUCTURE':
                        rte_attr_map = {}
                        for deattr, rteattr in rtegroup.groupsignalmap.items():
                            print("          DEATTR: %r, RTEATTR: %r, RTENAME: %r" % (deattr, rteattr, rte_name))
                            rte_attr_map[deattr] = rteattr

                t = parse_type(swc_data, all_types, data_type, data_type.getSwBaseType())

                all_de_elements.append(DE_Element(t, de, isInsignal, not isExternal, desc, rte_is_signal, rte_name, rte_attr_map))

    return all_de_elements


def getDatatypes(swc_input_file, options):
    datatypespath = options.outputdirectory+"/gen_datatypes.h"
    dataelementshpath = options.outputdirectory+"/gen_dataelements.h"
    dataelementscpppath = options.outputdirectory+"/gen_dataelements.cpp"
    jsonencdecpath = options.outputdirectory+"/gen_jsonencdec.h"
    jsonencdeccpppath = options.outputdirectory+"/gen_jsonencdec.cpp"
    
    vsminjectinstancespath = options.outputdirectory+"/vsm/vsm_inject_instances_cpp.h"
    vsminjectvariablespath = options.outputdirectory+"/vsm/vsm_inject_variables_cpp.h"
    vsminjectswitchokpath = options.outputdirectory+"/vsm/vsm_inject_switch_ok_cpp.h"
    vsminjectswitcherrorpath = options.outputdirectory+"/vsm/vsm_inject_switch_error_cpp.h"
    vsmsinkvariablespath = options.outputdirectory+"/vsm/vsm_sink_variables_cpp.h"
    vsmsinksubscribepath = options.outputdirectory+"/vsm/vsm_sink_subscribe_cpp.h"
    vsmalldataelementspath = options.outputdirectory+"/vsm/vsm_all_dataelements_cpp.h"

    swccrcpath = options.outputdirectory+"/vsm/swc_crc.h"
    comcrcpath = options.outputdirectory+"/vsm/com_crc.h"

    for f in [
        datatypespath,
        dataelementshpath,
        dataelementscpppath,
        jsonencdecpath,
        jsonencdeccpppath,
        vsminjectinstancespath,
        vsminjectvariablespath,
        vsminjectswitchokpath,
        vsminjectswitcherrorpath,
        vsmsinkvariablespath,
        vsmsinksubscribepath,
        vsmalldataelementspath,
        swccrcpath,
        comcrcpath
            ]:
        if os.path.isfile(f):
            os.remove(f)

    if not os.path.isdir(options.outputdirectory): os.mkdir(options.outputdirectory)
    if not os.path.isdir(options.outputdirectory+"/vsm"): os.mkdir(options.outputdirectory+"/vsm")

    header = "/*!\n * \\file\n * " + DESCRIPTION+" "+VERSION+"\n"
    header += " * " + COPYRIGHT + "\n"
    header += " * Generated at: " + datetime.now().isoformat() + "\n"
    header += " * Source: " + options.swcinputfile + "\n */\n"


    print("Calculating CRC16 for Swc file")
    crcFileToDefine(swc_input_file, swccrcpath, "SWC_CRC")
    print("Calculating CRC16 for Com file")
    crcFileToDefine(options.cominputfile, comcrcpath, "COM_CRC")

    print("Get Datatypes:")
    swc_data = autosar.arxml.load(swc_input_file)
    comfile_data = autosar.arxml.load(options.cominputfile)

    (all_types, all_de_elements) = parse(swc_data, comfile_data)

    all_enums = [e for e in all_types.values() if isinstance(e, DE_Enum)]
    all_structs = [e for e in all_types.values() if isinstance(e, DE_Struct)]
    all_arrays = [e for e in all_types.values() if isinstance(e, DE_Array)]

    (headerStr,
     cppStr,
     gen_vsm_inject_variable_cpp,
     gen_vsm_inject_instance_cpp,
     gen_vsm_inject_switch_ok_cpp,
     gen_vsm_inject_switch_error_cpp,
     gen_vsm_sink_variable_cpp,
     gen_vsm_sink_subscribe_cpp,
     gen_vsm_all_dataelements_cpp
     ) = render_dataelements.render_dataelments(header, all_de_elements, all_types)
    (gen_jsonenc_h_contents, gen_jsonenc_cpp_contents) = render_json.render_json(header, all_arrays, all_structs, all_types)

    with codecs.open(datatypespath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(render_datatype(header, all_enums, all_arrays, all_structs, all_types))

    with codecs.open(jsonencdecpath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_jsonenc_h_contents)

    with codecs.open(jsonencdeccpppath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_jsonenc_cpp_contents)

    with codecs.open(dataelementshpath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(headerStr)

    with codecs.open(dataelementscpppath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(cppStr)

    with codecs.open(vsminjectvariablespath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_vsm_inject_variable_cpp)

    with codecs.open(vsminjectinstancespath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_vsm_inject_instance_cpp)

    with codecs.open(vsminjectswitchokpath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_vsm_inject_switch_ok_cpp)

    with codecs.open(vsminjectswitcherrorpath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_vsm_inject_switch_error_cpp)

    with codecs.open(vsmsinkvariablespath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_vsm_sink_variable_cpp)

    with codecs.open(vsmsinksubscribepath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_vsm_sink_subscribe_cpp)

    with codecs.open(vsmalldataelementspath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_vsm_all_dataelements_cpp)



def getIntTypeStr(limits):
    (minValueStr, maxValueStr) = limits
    minValue = int(minValueStr)
    maxValue = int(maxValueStr)
    if minValue >= 0:
        # unsigned int type
        if maxValue <= 0xFF: return "uint8_t"
        if maxValue <= 0xFFFF: return "uint16_t"
        if maxValue <= 0xFFFFFFFF: return "uint32_t"
        return "int64_t"
    else:
        # signed int
        if minValue >= -0x80 and maxValue <= 0x7F: return "int8_t"
        if minValue >= -0x8000 and maxValue <= 0x7FFF: return "int16_t"
        if minValue >= -0x80000000 and maxValue <= 0x7FFFFFFF: return "int32_t"
        return "int64_t"

def crcFileToDefine(infile, outfile, definename):
    s = "// "+infile+"\n"
    with open(infile,'r') as file:

        #TODO: This is incredibly slow (up to 20 sec), can we use another algorithm?
        #      Maybe call out to another process... linux crc32 takes 0,07sec on the same file.
        contents = file.read()
        crc = crc16.crc16(contents)


        s += "#define "+definename+" "+str(crc)+"\n"
        s += "#endif\n"
    s += "#ifndef "+definename+"\n"
    with open(outfile,'w') as file:
        file.write(s)

def main():
    print('%s (%s)\n%s\n'%(DESCRIPTION, VERSION, COPYRIGHT))

    parser = ArgumentParser()
    parser.add_argument('--version', action='version', version="%%(prog)s %s (%s)"%(VERSION, autosar.VERSION))
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true', help='Enable verbose output')
    parser.add_argument('-d', '--directory', dest='outputdirectory', help='Output directory for generated files', required=True)
    parser.add_argument('-s', '--swcinputfile', dest='swcinputfile', help='SWC input arxml, (Elektra export file)', required=True)
    parser.add_argument('-c', '--cominputfile', dest='cominputfile', help='COM input arxml, (Elektra export file)', required=True)
    args = parser.parse_args()

    autosar.arxml.options.verbose = args.verbose

    if not os.path.exists(args.swcinputfile):
        parser.error("swcinputfile \"%s\" does not exist" % args.swcinputfile)
    if not os.path.exists(args.cominputfile):
        parser.error("cominputfile  \"%s\" does not exist" % args.cominputfile)

    getDatatypes(args.swcinputfile, args)
    print("done!")

if __name__ == "__main__":
    main()

