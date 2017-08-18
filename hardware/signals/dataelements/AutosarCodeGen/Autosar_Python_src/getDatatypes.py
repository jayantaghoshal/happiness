#!/bin/python

# Copyright 2017 Volvo Cars, All Rights Reserved.
# Volvo Cars Confidential

from optparse import OptionParser

import codecs
import autosar
import os
from datetime import datetime
from autosar.components import AR_RPort
import crc16

##import sys
#import generate_impl_datatypes as implDatatype

VERSION = 'v1.0'
DESCRIPTION = 'C++ code generator for AUTOSAR'
COPYRIGHT = 'Copyright 2017 Delphi Technologies, Inc., All Rights Reserved. Delphi Confidential'

OUTPUT_FILE_ENCODING = "utf-8"


GEN_JSONENCDEC_HPP_FROMTO_ARRAY_TEMPLATE = """// ==========================================
// {ARRAY}
template <> json toJson<{ARRAY}>(const {ARRAY}& v);
template <> {ARRAY} fromJson<{ARRAY}>(const json& j);

"""

GEN_JSONENCDEC_CPP_FROMTO_ARRAY_TEMPLATE = """// ==========================================
// {ARRAY}
template <>
json toJson<{ARRAY}>(const {ARRAY}& v) {
    json j;
    for (int i=0 ; i<{N}; ++i) {
        j.push_back(v[i]);
    }
    return j;
}
template <>
{ARRAY} fromJson<{ARRAY}>(const json& a) {
    if (!a.is_array()) {
        return {ARRAY}();
    }
    if (a.size()!={N}) {
        return {ARRAY}();
    }
    {ARRAY} result;
    for (int i=0 ; i<{N} ; ++i) {
        result[i] = a[i].get<{TYPE}>();
    }
    return result;
}
"""

# Note: function is not closed here
GEN_JSONENCDEC_CPP_FROMJSON_FUNC_OPENING = """template <>
{STRUCT} fromJson<{STRUCT}>(const json& object) {
    if (!object.is_object()) {
        return {STRUCT}();
    }
    {STRUCT} v;
"""

# Note: function is not closed here
GEN_JSONENCDEC_CPP_TOJSON_FUNC_OPENING = """// ==========================================
// {STRUCT}
template <>
json toJson<{STRUCT}>(const {STRUCT}& v) {
    json j;
"""

GEN_JSONENCDEC_HPP_CLASS_TEMPLATE = """// ==========================================
// {STRUCT}
template <> json toJson<{STRUCT}>(const {STRUCT}& v);
template <> {STRUCT} fromJson<{STRUCT}>(const json& j);

"""

GEN_DATAELEMENTS_HPP_CLASS_TEMPLATE = """%sstruct %s : %spublic DataElemInfo {
    using data_elem_type = %s;%s

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


"""

GEN_DATAELEMENTS_CPP_CLASS_TEMPLATE = """//==========================================
const char* {CLASS}::Name() { return \"%s\"; }
const char* {CLASS}::TypeName() { return \"%s\"; }
Dir {CLASS}::Direction() { return %s; }

const char* {CLASS}::name() const { return Name(); }
const char* {CLASS}::typeName() const { return TypeName(); }
Dir {CLASS}::direction() const { return Direction(); }


"""

# ******************************************************************
# VSM TEMPLATES
GEN_VSM_ALL_DATAELEMENTS = """new autosar::{CLASS}(),\n"""

GEN_VSM_INJECT_VARIABLE_TEMPLATE = """    static DEInjector<autosar::{DATAELEM}_info>* {DATAELEM}_de=nullptr;

"""

GEN_VSM_INJECT_INSTANCE_TEMPLATE = """    {DATAELEM}_de = new DEInjector<autosar::{DATAELEM}_info>();

"""

GEN_VSM_INJECT_SIGNALGROUP_SWITCH_OK = """    case SignalGroup|ComConf_ComSignalGroup_ig{RTETYPE}_mrx:
    {
        if (sizeof({RTETYPE}) == length) {
            log_verbose() << "Received {RTETYPE} (" << ComConf_ComSignalGroup_ig{RTETYPE}_mrx << ")";
            const {RTETYPE}& rteValue = *static_cast<const {RTETYPE}*>(buffer);
            autosar::{DATAELEM}_info::data_elem_type deValue;
{TRANSFER}
            {DATAELEM}_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for {RTETYPE} (" << ComConf_ComSignalGroup_ig{RTETYPE}_mrx << "). Got " << length << ", expected " << sizeof({RTETYPE});
        }
    }
    break;

"""

GEN_VSM_INJECT_SIGNAL_SWITCH_OK = """    case ComConf_ComSignal_is{RTETYPE}_mrx:
    {
        if (sizeof({RTETYPE}) == length) {
            log_verbose() << "Received {RTETYPE} (" << ComConf_ComSignal_is{RTETYPE}_mrx << ")";
            const {RTETYPE}& rteValue = *static_cast<const {RTETYPE}*>(buffer);
            autosar::{DATAELEM}_info::data_elem_type deValue;
            {TRANSFER};
            {DATAELEM}_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for {RTETYPE} (" << ComConf_ComSignal_is{RTETYPE}_mrx << "). Got " << length << ", expected " << sizeof({RTETYPE});
        }
    }
    break;

"""

GEN_VSM_INJECT_SIGNALGROUP_SWITCH_ERROR = """    case SignalGroup|ComConf_ComSignalGroup_ig{RTETYPE}_mrx:
        {DATAELEM}_de->error(errorCode);
        break;

"""

GEN_VSM_INJECT_SIGNAL_SWITCH_ERROR = """    case ComConf_ComSignal_is{RTETYPE}_mrx:
        {DATAELEM}_de->error(errorCode);
        break;

"""

GEN_VSM_INJECT_TRANSFER_ARRAY_COPY = """for (unsigned int i=0; i<deValue.size(); ++i) deValue[i] = static_cast<autosar::{DATAELEM}_info::data_elem_type::value_type>( rteValue[i] )"""
GEN_VSM_INJECT_SIGNAL_TRANSFER = """deValue = static_cast<decltype(deValue)>(rteValue)"""
GEN_VSM_INJECT_SIGNAL_TRANSFER_LINEAR = """deValue = static_cast<decltype(deValue)>( {FUNC}<{SIZE}>(rteValue) )"""
GEN_VSM_INJECT_SIGNALGROUP_TRANSFER = """            deValue.{DEATTR} = static_cast<decltype(deValue.{DEATTR})>(rteValue.{RTEATTR})"""
GEN_VSM_INJECT_SIGNALGROUP_TRANSFER_LINEAR = """            deValue.{DEATTR} = static_cast<decltype(deValue.{DEATTR})>( {FUNC}<{SIZE}>(rteValue.{RTEATTR}) )"""


GEN_VSM_SINK_VARIABLE_TEMPLATE = """    static DESink<autosar::{DATAELEM}_info>* {DATAELEM}_de=nullptr;

"""

GEN_VSM_SINK_SIGNAL_SUBSCRIBE = """{DATAELEM}_de = new DESink<autosar::{DATAELEM}_info>();
{DATAELEM}_de->subscribe([]() {
    auto deValue = {DATAELEM}_de->get().value();
    {RTETYPE} rteValue;
    {TRANSFER};
    sendAvmpMessageToVip( ComConf_ComSignal_is{RTETYPE}_mtx, &rteValue, sizeof(rteValue) ); 
});

"""

GEN_VSM_SINK_SIGNALGROUP_SUBSCRIBE = """{DATAELEM}_de = new DESink<autosar::{DATAELEM}_info>();
{DATAELEM}_de->subscribe([]() {
    auto deValue = {DATAELEM}_de->get().value();
    {RTETYPE} rteValue;
{TRANSFER}
    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_ig{RTETYPE}_mtx, &rteValue, sizeof(rteValue) ); 
});

"""

GEN_VSM_SINK_SIGNAL_TRANSFER = """rteValue = static_cast<decltype(rteValue)>(deValue)"""
GEN_VSM_SINK_SIGNAL_TRANSFER_SCALE = """rteValue = round((deValue - {OFFSET}) / {SCALE})"""
GEN_VSM_SINK_SIGNALGROUP_TRANSFER = """    rteValue.{RTEATTR} = static_cast<decltype(rteValue.{RTEATTR})>(deValue.{DEATTR})"""
GEN_VSM_SINK_SIGNALGROUP_TRANSFER_SCALE = """    rteValue.{RTEATTR} = round((deValue.{DEATTR} - {OFFSET}) / {SCALE})"""
GEN_VSM_SINK_TRANSFER_ARRAY_COPY = """for (unsigned int i=0; i<deValue.size(); ++i) rteValue[i] = static_cast<std::remove_reference<decltype( *rteValue )>::type>( deValue[i] )"""


def escape_cpp_comment(text):
    s=text.replace("*/", "* /")
    s=s.replace("\n","\n * ")
    s=s.replace("\"","")
    return s


def getDatatypes(file1, options):
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

    if os.path.isfile(datatypespath): os.remove(datatypespath)
    if os.path.isfile(dataelementshpath): os.remove(dataelementshpath)
    if os.path.isfile(dataelementscpppath): os.remove(dataelementscpppath)
    if os.path.isfile(jsonencdecpath): os.remove(jsonencdecpath)
    if os.path.isfile(jsonencdeccpppath): os.remove(jsonencdeccpppath)

    if os.path.isfile(vsminjectinstancespath): os.remove(vsminjectinstancespath)
    if os.path.isfile(vsminjectvariablespath): os.remove(vsminjectvariablespath)
    if os.path.isfile(vsminjectswitchokpath): os.remove(vsminjectswitchokpath)
    if os.path.isfile(vsminjectswitcherrorpath): os.remove(vsminjectswitcherrorpath)
    if os.path.isfile(vsmsinkvariablespath): os.remove(vsmsinkvariablespath)
    if os.path.isfile(vsmsinksubscribepath): os.remove(vsmsinksubscribepath)
    if os.path.isfile(vsmalldataelementspath): os.remove(vsmalldataelementspath)

    if os.path.isfile(swccrcpath): os.remove(swccrcpath)
    if os.path.isfile(comcrcpath): os.remove(comcrcpath)

    if not os.path.isdir(options.outputdirectory): os.mkdir(options.outputdirectory)
    if not os.path.isdir(options.outputdirectory+"/vsm"): os.mkdir(options.outputdirectory+"/vsm")

    header = "/*!\n * \\file\n * " + DESCRIPTION+" "+VERSION+"\n"
    header += " * " + COPYRIGHT + "\n"
    header += " * Generated at: " + datetime.now().isoformat() + "\n"
    header += " * Source: " + options.swcinputfile + "\n */\n"
    #header += "// Source 2: " + options.cominputfile + "\n\n\n"

    dTStr = header + """#ifndef _DATATYPES_H
#define _DATATYPES_H

#include <array>

namespace autosar {\n
"""

    gen_jsonenc_h_contents = header + """#ifndef _JSONENCDEC_H
#define _JSONENCDEC_H

#include <json.hpp>
#include <type_traits>

#include "gen_datatypes.h"

using json = nlohmann::json;

namespace autosar {

// handles all enum types
template<typename T> using AREnum = typename std::enable_if<std::is_enum<T>::value, T>::type;

// handles everything EXCEPT enum (and the explicits below)
template<typename T> using AllButAREnum = typename std::enable_if<!std::is_enum<T>::value, T>::type;

template <typename T>
json toJson(AREnum<T> e) {
    return json((int)e);
}

template <typename T>
AREnum<T> fromJson(const json& j) {
    if (!j.is_number()) {
        return T();
    }
    return static_cast<T>( j.get<int>() );
}

// handles everything EXCEPT enum (and the explicits below)
template <typename T>
json toJson(const AllButAREnum<T>& e) {
    return json(e);
}

template <typename T>
AllButAREnum<T> fromJson(const json& j) {
    if ( j.is_array() || j.is_null() || j.is_object() ) {
        return T();
    }
    return j.get<T>();
}


"""

    gen_jsonenc_cpp_contents = header + """
#include "gen_jsonencdec.h"

namespace autosar {

"""

    gen_vsm_inject_variable_cpp = header + """
"""
    gen_vsm_inject_instance_cpp = header + """
"""
    gen_vsm_inject_switch_ok_cpp = header + """
"""
    gen_vsm_inject_switch_error_cpp = header + """
"""
    gen_vsm_sink_variable_cpp = header + """
"""
    gen_vsm_sink_subscribe_cpp = header + """
"""
    print "Calculating CRC16 for Swc file"
    crcFileToDefine(file1, swccrcpath, "SWC_CRC")
    print "Calculating CRC16 for Com file"
    crcFileToDefine(options.cominputfile, comcrcpath, "COM_CRC")

    print "Get Datatypes:"
    f1 = autosar.arxml.load(file1)
    f2 = autosar.arxml.load(options.cominputfile)
# 	externalSignals = dict()
# 	for k,d in f2.sigtrig.iteritems():
# 		if externalSignals.has_key(d.de):
# 			if d.path[:29]=="/VehicleTopology/BackboneMAIN":
# 				externalSignals[ d.de ]=d.isOut
# 				s = "IN"
# 				if d.isOut: s="OUT"
# 				print "changing: " + d.de + " to " + s
# 		else:
# 			externalSignals[ d.de ]=d.isOut

    #if os.path.isfile(options.enumfile):
    #	os.remove(options.enumfile)
    #fenum = open(options.enumfile,'w')
    #enumStr = ""

    # dtBaseTypes = dict()

    print "Generating enums..."

    # VALUE
    # BOOLEAN
    for k, d in sorted(f1.datatypes.iteritems()):
        d = d
        """:type: autosar.components.ARDatatype""" # can also be subclasses ARRecordDatatype, ARArrayDatatype

        if d.getCategory() == 'VALUE':
            compuMethod = d.getCompuMethod()
            # constr = d.getDataConstraint()
            # print compuMethod.shortname
            if compuMethod.getCategory() == 'LINEAR':
                pass
            elif compuMethod.getCategory() == 'IDENTICAL':
                pass
            elif compuMethod.getCategory() == 'TEXTTABLE':
                #print compuMethod.getValues()

                desc = d.getDesc()
                if desc != "":
                    dTStr += "/*!\n * \enum %s\n * %s\n */\n" % (d.shortname,escape_cpp_comment(desc.strip()))
                else:
                    dTStr += "/*!\n * \enum %s\n */\n" % (d.shortname)

                dTStr += "enum class " + d.shortname + " {\n"
                enumerations = compuMethod.getEnumerations()
                inverted_dict = dict([[v[0],k] for k,v in enumerations.items()])
                for i in compuMethod.getValues():
                    ename = inverted_dict[str(i)].replace(compuMethod.shortname + '_','')
                    dTStr += "    " + ename + "=" + str(i) + ",\n"

                dTStr += "};\n\n"

            else:
                raise Exception("Unhandled compuMethod category '" + compuMethod.getCategory() + "' for data-type " + d.shortname)

        elif d.getCategory() == 'BOOLEAN':
            pass
            # dTStr = dTStr + "using " + d.shortname + " = bool\n\n"
        elif d.getCategory() == 'STRUCTURE':
            pass  # is handled in a second round below
        elif d.getCategory() == 'ARRAY':
            pass  # is handled in a second round below
        else:
            raise Exception("Unhandled data-type category '" + d.getCategory() + "' for data-type " + d.shortname)

    print "Generating structs and encoders/decoders..."

    typeInfoMap = dict()
    # STRUCTURE
    # ARRAY
    hist = dict()
    for k, d in sorted(f1.datatypes.iteritems()):
        if d.getCategory() == 'STRUCTURE':
            desc = d.getDesc()
            if desc != "":
                dTStr += "/*!\n * \struct %s\n * %s\n */\n" % (d.shortname,escape_cpp_comment(desc.strip()))
            else:
                dTStr += "/*!\n * \struct %s\n */\n" % (d.shortname)

            dTStr += "struct " + d.shortname + " {\n"
            typeInfo = dict()
            typeInfo["isarray"] = False
            gen_jsonenc_h_contents += GEN_JSONENCDEC_HPP_CLASS_TEMPLATE.replace("{STRUCT}", d.shortname)

            func_tojson_str = GEN_JSONENCDEC_CPP_TOJSON_FUNC_OPENING.replace("{STRUCT}", d.shortname)
            func_fromjson_str = GEN_JSONENCDEC_CPP_FROMJSON_FUNC_OPENING.replace("{STRUCT}", d.shortname)
            rE = d.getElements()
            for r in rE:
                typeT = r.getDataType()
                typeStr = typeT.shortname
                attrStr = r.shortname
                attrInfo = dict()
                attrInfo["realname"] = attrStr
                range_comment = ""
                if r.getCategory() == 'VALUE':
                    if typeT.getCompuMethod().getCategory() == 'TEXTTABLE':
                        if r.shortname == typeT.shortname:
                            attrStr += "_"
                        attrInfo["realname"] = attrStr
                    elif typeT.getCompuMethod().getCategory() == 'LINEAR':
                        numerator = typeT.getCompuMethod().getCompuNumerator()
                        typeStr = "double"
                        btstr = "raw is unsigned"
                        unitref = typeT.getCompuMethod().getSubElem("UNIT-REF").text
                        unitstr = f1.units[unitref].getSubElem("DISPLAY-NAME").text
                        
                        bt = r.getSwBaseType()
                        func = "toUnsignedFromRaw"
                        size = "0"
                        if bt is not None:
                            if bt.getBaseType() == '2C':
                                btstr = "raw is signed"
                                func = "toSignedFromRaw"
                            size = bt.getBaseTypeLength()
                        if size!="0":
                            btstr += ", "+size+" bits"
                        lim = typeT.getDataConstraint().getLimits()
                        attrInfo["func"] = func
                        attrInfo["size"] = size
                        if numerator[1] == '1.0':
                            typeStr = getIntTypeStr(lim)
                        else:
                            attrInfo["scale"] = numerator[1]
                            attrInfo["offset"] = numerator[0]
                        #range_comment = " // " + numerator[1] + "*x+" + numerator[0] + " (" + lim[0] + "->" + lim[1] + " " + unitstr + ") " + btstr
                        range_comment = " /*!< " + unitstr + " , " + lim[0] + "->" + lim[1] + " (" + numerator[1] + "*x+" + numerator[0] + ", " + btstr + " ) */"
                    elif typeT.getCompuMethod().getCategory() == 'IDENTICAL':
                        unitref = typeT.getCompuMethod().getSubElem("UNIT-REF").text
                        unitstr = f1.units[unitref].getSubElem("DISPLAY-NAME").text
                        lim = typeT.getDataConstraint().getLimits()
                        typeStr = getIntTypeStr(lim)
                        range_comment = " /*!< " + unitstr + " , " + lim[0] + "->" + lim[1] + " */"
                    else:
                        raise Exception("Unhandled structure compuMethod category '" + typeT.getCompuMethod().getCategory() + "' for data-type/element " + d.shortname+"/" + r.shortname)

                elif r.getCategory() == 'BOOLEAN':
                    typeStr = "bool"
                else:
                    raise Exception("Unhandled structure data-type category '" + r.getCategory() + "' for data-type/element " + d.shortname + "/" + r.shortname)
                dTStr += "    " + typeStr + " " + attrStr + ";" + range_comment + "\n"
                func_tojson_str += "    j[\"%s\"] = toJson<%s>(v.%s);\n" % (attrStr, typeStr, attrStr)
                func_fromjson_str += "    v.%s = fromJson<%s>(object[\"%s\"]);\n" % (attrStr, typeStr, attrStr)
                typeInfo[r.shortname] = attrInfo
            typeInfoMap[d.shortname] = typeInfo

            dTStr += "};\n\n"
            func_tojson_str += "    return j;\n}"
            func_fromjson_str += "    return v;\n}\n\n\n"
            gen_jsonenc_cpp_contents += func_tojson_str + "\n" + func_fromjson_str

        if d.getCategory() == 'ARRAY':
            desc = d.getDesc()
            if desc != "":
                dTStr += "/*!\n * \\brief array %s\n * %s\n */\n" % (d.shortname,escape_cpp_comment(desc.strip()))
            else:
                dTStr += "/*!\n * \\brief array %s\n */\n" % (d.shortname)

            typeInfo = dict()
            typeInfo["isarray"] = True

            et = d.getElementType()
            typeT = d.getDataType()
            typename = ""
            range_comment = ""
            if typeT.getCategory() == 'VALUE': ##or r.getCategory() == 'BOOLEAN':
                if typeT.getCompuMethod().getCategory() == 'TEXTTABLE':
                    typename = typeT.shortname
                elif typeT.getCompuMethod().getCategory() == 'LINEAR':
                    typename = "double"
                    btstr = "raw is unsigned"
                    bt = et.getSwBaseType()
                    func = "toUnsignedFromRaw"
                    size = "0"
                    if bt is not None:
                        if bt.getBaseType() == '2C':
                            btstr = "raw is signed"
                            func = "toSignedFromRaw"
                        size = bt.getBaseTypeLength()
                    if size!="0":
                        btstr += ", "+size+" bits"
                    numerator = typeT.getCompuMethod().getCompuNumerator()
                    lim = typeT.getDataConstraint().getLimits()
                    typeInfo["func"] = func
                    typeInfo["size"] = size
                    if numerator[1] == '1.0':
                        typename = getIntTypeStr(lim)
                    else:
                        typeInfo["scale"] = numerator[1]
                        typeInfo["offset"] = numerator[0]
                    range_comment = " /*!< " + numerator[1] + "*x+" + numerator[0] + " (" + lim[0] + "->" + lim[1] + ") "+ btstr + " */"

                elif typeT.getCompuMethod().getCategory() == 'IDENTICAL':
                    lim = typeT.getDataConstraint().getLimits()
                    range_comment = " /*!< " + lim[0] + "->" + lim[1] + " */"
                    typename = getIntTypeStr(lim)
                else:
                    raise Exception("Unhandled array compuMethod category '" + typeT.getCompuMethod().getCategory() + "' for data-type/element " + d.shortname+"/"+et["shortname"])
            elif typeT.getCategory() == 'BOOLEAN':
                typename = "bool"
            else:
                raise Exception("Unhandled array data-type category '" + typeT.getCategory() + "' for data-type/element " + d.shortname+"/"+et["shortname"])
            typeInfo["size"] = et["max_elems"]
            typeInfoMap[d.shortname] = typeInfo
            arrayTypeStr = "std::array<" + typename + "," + et["max_elems"] + ">"
            dTStr += "using " + d.shortname + " = " + arrayTypeStr + ";" + range_comment + "\n\n"
            if arrayTypeStr not in hist:
                hist[arrayTypeStr] = 1
                gen_jsonenc_h_contents += GEN_JSONENCDEC_HPP_FROMTO_ARRAY_TEMPLATE.replace("{ARRAY}", arrayTypeStr)
                func_tojson_str = GEN_JSONENCDEC_CPP_FROMTO_ARRAY_TEMPLATE.replace("{ARRAY}", arrayTypeStr)
                func_tojson_str = func_tojson_str.replace("{N}", et["max_elems"])
                func_tojson_str = func_tojson_str.replace("{TYPE}", typename)
                gen_jsonenc_cpp_contents += func_tojson_str + "\n\n"

    dTStr += "} // end of namespace\n#endif"
    gen_jsonenc_h_contents += "} // end of namespace\n#endif"
    gen_jsonenc_cpp_contents += "} // end of namespace\n"

    # only create file if everything went fine
    with codecs.open(datatypespath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(dTStr)

    with codecs.open(jsonencdecpath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_jsonenc_h_contents)

    with codecs.open(jsonencdeccpppath, 'w', encoding=OUTPUT_FILE_ENCODING) as fout:
        fout.write(gen_jsonenc_cpp_contents)

    # DATA-ELEMENTS
    print "Generating data-elements..."

    headerStr = header + """#ifndef _DATAELEMENTS_H
#define _DATAELEMENTS_H

#include \"gen_datatypes.h\"

namespace autosar {

enum class Dir {
    IN,
    OUT,
    INTERNAL
};

struct DataElemInfo {
    virtual const char* name() const=0;
    virtual const char* typeName() const=0;
    virtual Dir direction() const=0;
};

/*! 
 * \\brief Provide compile time info if a data-element is in or out
 */
class InTag {
};

class OutTag {
};

class InternalTag : public InTag, public OutTag {
};\n\n
"""

    cppStr = header + """#include \"gen_dataelements.h\"


namespace autosar {

"""
    gen_vsm_all_dataelements_cpp = ""

    # TODO: it is a bit unclear which ports we shall consider
    # now we handle all ports in all swcs:s
    hist = dict()
    for k, swcs in f1.swcs.iteritems():
        swcs = swcs
        """:type: autosar.components.ARSwc"""


        #print
        print "SWC: " + swcs.shortname
        if swcs.shortname != "IHU":
            continue
        for k2, port in sorted(swcs.ports.iteritems()):
            isInsignal = isinstance(port, AR_RPort)
            print ("   RPort: " if isInsignal else "   PPort: ") + port.shortname
            elems = port.getDataElements()
            port_interface = port.arxml.port_interfaces[port.port_interface.text]
            #pi = f1.port_interfaces.get(port.port_interface.text)
            #pi = port.port_interface
            #elems = pi.getDataElements()
            for de, dname in elems.iteritems():
                print "      " + de
                #isexternal = externalSignals.has_key(de)
                #isInsignal = isexternal and not externalSignals[de]
                # isInsignal = isinstance(port, AR_RPort)
                #inoutstr = "out: "
                #if isinstance(port,AR_RPort): inoutstr = "in:  "
                #if isexternal and (isInsignal == isinstance(port,AR_RPort)): inoutstr = inoutstr.upper()

                desc = port_interface.getDesc()
                isExternal = de in f2.groupmapping
                if de not in hist:
                    hist[de] = 1 # just a dummy value
                    d = f1.datatypes.get(dname)
                    if not isExternal:
                        dirtag = "InternalTag, "
                    else:
                        dirtag = "InTag, " if isInsignal else "OutTag, "
                    #if isinstance(port,AR_RPort): intag = "InTag, "
                    #if isexternal and isInsignal: dirtag = "InTag, "
                    #if isexternal and not isInsignal: dirtag = "OutTag, "
                    typename = d.shortname
                    range_comment = ""
                    tmpTransferInject = ''
                    tmpTransferSink = ''
                    if d.getCategory() == 'VALUE':
                        tmpTransferInject = GEN_VSM_INJECT_SIGNAL_TRANSFER
                        tmpTransferSink = GEN_VSM_SINK_SIGNAL_TRANSFER
                        if d.getCompuMethod().getCategory() == 'TEXTTABLE':
                            pass
                        elif d.getCompuMethod().getCategory() == 'LINEAR':
                            typename = "double"
                            btstr = "raw is unsigned"
                            bt = d.getSwBaseType()
                            func = "toUnsignedFromRaw"
                            size = "0"
                            if bt is not None:
                                if bt.getBaseType() == '2C':
                                    btstr = "raw is signed"
                                    func = "toSignedFromRaw"
                                size = getBaseTypeLength()
                            if size!="0":
                                btstr += ", "+size+" bits"
                            tmpTransferInject = GEN_VSM_INJECT_SIGNAL_TRANSFER_LINEAR.replace("{FUNC}",func).replace("{SIZE}",size);
                            numerator = d.getCompuMethod().getCompuNumerator()
                            unitref = d.getCompuMethod().getSubElem("UNIT-REF").text
                            unitstr = f1.units[unitref].getSubElem("DISPLAY-NAME").text
                            lim = d.getDataConstraint().getLimits()
                            if numerator[1] == '1.0':
                                typename = getIntTypeStr(lim)
                            else:
                                tmpTransferInject += " * "+ numerator[1] + " + " + numerator[0]
                                tmpTransferSink = GEN_VSM_SINK_SIGNAL_TRANSFER_SCALE.replace("{SCALE}", numerator[1]).replace("{OFFSET}", numerator[0])
                            #range_comment += " // " + numerator[1] + "*x+" + numerator[0] + " (" + lim[0] + "->" + lim[1] + ") " + btstr
                            range_comment = " /*!< " + unitstr + " , " + lim[0] + "->" + lim[1] + " (" + numerator[1] + "*x+" + numerator[0] + ", " + btstr + " ) */"

                        elif d.getCompuMethod().getCategory() == 'IDENTICAL':
                            lim = d.getDataConstraint().getLimits()
                            unitref = d.getCompuMethod().getSubElem("UNIT-REF").text
                            unitstr = f1.units[unitref].getSubElem("DISPLAY-NAME").text
                            range_comment = " /*!< " + unitstr + " , " + lim[0] + "->" + lim[1] + " */"
                            typename = getIntTypeStr(lim)
                        else:
                            raise Exception("Unhandled data-element data-type category '" + d.getCompuMethod().getCategory() + "' for data-type " + d.shortname)

                    elif d.getCategory() == 'BOOLEAN':
                        typename = "bool"
                        tmpTransferInject = GEN_VSM_INJECT_SIGNAL_TRANSFER
                        tmpTransferSink = GEN_VSM_SINK_SIGNAL_TRANSFER

                    elif d.getCategory() == 'STRUCTURE':
                        if isExternal:
                            rtegroup = f2.groupmapping[de]
                            for deattr,rteattr in rtegroup.groupsignalmap.items():
                                detype = typeInfoMap[typename]
                                derealattr = detype[deattr]
                                realname = derealattr["realname"]
                                if "scale" in derealattr:
                                    scale  = derealattr["scale"]
                                    offset = derealattr["offset"]
                                    func   = derealattr["func"]
                                    size   = derealattr["size"]
                                    tmpTransferInject += GEN_VSM_INJECT_SIGNALGROUP_TRANSFER_LINEAR.replace("{FUNC}",func).replace("{SIZE}",size).replace("{DEATTR}", realname).replace("{RTEATTR}", rteattr)+ " * " + scale + " + " + offset + ";\n"
                                    tmpTransferSink += GEN_VSM_SINK_SIGNALGROUP_TRANSFER_SCALE.replace("{DEATTR}", realname).replace("{RTEATTR}", rteattr).replace("{SCALE}",scale).replace("{OFFSET}", offset) + ";\n"
                                else:
                                    if "func" in derealattr:
                                        func = derealattr["func"]
                                        size = derealattr["size"]
                                        tmpTransferInject += GEN_VSM_INJECT_SIGNALGROUP_TRANSFER_LINEAR.replace("{FUNC}",func).replace("{SIZE}",size).replace("{DEATTR}", derealattr["realname"]).replace("{RTEATTR}", rteattr)+";\n"
                                    else:
                                        tmpTransferInject += GEN_VSM_INJECT_SIGNALGROUP_TRANSFER.replace("{DEATTR}", derealattr["realname"]).replace("{RTEATTR}", rteattr)+";\n"
                                    tmpTransferSink += GEN_VSM_SINK_SIGNALGROUP_TRANSFER.replace("{DEATTR}", derealattr["realname"]).replace("{RTEATTR}", rteattr)+";\n"
                    elif d.getCategory() == 'ARRAY':
                        rtegroup = f2.groupmapping[de]
                        tmpTransferInject = GEN_VSM_INJECT_TRANSFER_ARRAY_COPY.replace("{DATAELEM}", de)
                        if (rtegroup.isSignal): tmpTransferSink = GEN_VSM_SINK_TRANSFER_ARRAY_COPY
                        else: tmpTransferSink = "    "+GEN_VSM_SINK_TRANSFER_ARRAY_COPY+";"

                    descComment = ""
                    if desc != "":
                        descComment = "/*!\n * \\brief Signal %s\n * %s\n */\n" % (de,escape_cpp_comment(desc.strip()))
                    else:
                        descComment = "/*!\n * \\brief Signal %s\n */\n" % (de)
                    headerStr += GEN_DATAELEMENTS_HPP_CLASS_TEMPLATE % (descComment, de + "_info", dirtag, typename, range_comment)
                    if not isExternal:
                        direct = "Dir::INTERNAL"
                    else:
                        direct = "Dir::IN" if isInsignal else "Dir::OUT"
                    #if isinstance(port,AR_RPort): direct = "Dir::IN"
                    #if isexternal and isInsignal: direct = "Dir::IN"
                    #if isexternal and not isInsignal: direct = "Dir::OUT"

                    tmpStr = GEN_DATAELEMENTS_CPP_CLASS_TEMPLATE % (de, typename, direct)
                    cppStr += tmpStr.replace("{CLASS}", de+"_info")
                    gen_vsm_all_dataelements_cpp += GEN_VSM_ALL_DATAELEMENTS.replace("{CLASS}", de+"_info")
                    if isInsignal:
                        if isExternal:
                            rtegroup = f2.groupmapping[de]
                            gen_vsm_inject_variable_cpp += GEN_VSM_INJECT_VARIABLE_TEMPLATE.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de)
                            gen_vsm_inject_instance_cpp += GEN_VSM_INJECT_INSTANCE_TEMPLATE.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de)
                            if rtegroup.isSignal:
                                gen_vsm_inject_switch_ok_cpp += GEN_VSM_INJECT_SIGNAL_SWITCH_OK.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de).replace("{TRANSFER}", tmpTransferInject)
                                gen_vsm_inject_switch_error_cpp += GEN_VSM_INJECT_SIGNAL_SWITCH_ERROR.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de)
                            else:
                                gen_vsm_inject_switch_ok_cpp += GEN_VSM_INJECT_SIGNALGROUP_SWITCH_OK.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de).replace("{TRANSFER}", tmpTransferInject)
                                gen_vsm_inject_switch_error_cpp += GEN_VSM_INJECT_SIGNALGROUP_SWITCH_ERROR.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de)
                        else:
                            gen_vsm_inject_variable_cpp += "// ***************************************\n// "+de+" not found in Com file\n\n"
                            gen_vsm_inject_instance_cpp += "// ***************************************\n// "+de+" not found in Com file\n\n"
                    else:
                        if isExternal:
                            rtegroup = f2.groupmapping[de]
                            gen_vsm_sink_variable_cpp += GEN_VSM_SINK_VARIABLE_TEMPLATE.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de)
                            if rtegroup.isSignal:
                                gen_vsm_sink_subscribe_cpp += GEN_VSM_SINK_SIGNAL_SUBSCRIBE.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de).replace("{TRANSFER}", tmpTransferSink)
                            else:
                                gen_vsm_sink_subscribe_cpp += GEN_VSM_SINK_SIGNALGROUP_SUBSCRIBE.replace("{RTETYPE}", rtegroup.rtename).replace("{DATAELEM}", de).replace("{TRANSFER}", tmpTransferSink)


    headerStr += "} // end of namespace\n#endif"
    cppStr += "} // end of namespace\n"
    
    #print gen_vsm_inject_instance_cpp
    #print gen_vsm_inject_switch_ok_cpp
    #print gen_vsm_inject_switch_error_cpp
    #print gen_vsm_sink_subscribe_cpp

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

# 	if os.path.isfile(options.dataelementfile):
# 		os.remove(options.dataelementfile)
# 	fdE = open(options.dataelementfile,'w')
# 	
# 	deMapStr = "if (~exist('dEhash'))\n	dEhash = java.util.Hashtable;\nend\n"
# 	
# 	for k,d in  f1.port_interfaces.iteritems():
# 		dataElem = d.data_elements
# 		for de,datatype in dataElem.iteritems():
# 			deMapStr = deMapStr + "if (~dEhash.containsKey('" + de + "'))\n" + "		dEhash.put('" + de + "','" + datatype.split('/')[-1] + "');\nelse\n	if ~strcmp(dEhash.get('" + de + "'),'" + datatype.split('/')[-1] + "')\n		disp(['# Warning: Datatype of dataelement " + de + " has been changed from ',dEhash.get('" + de + "'), ' to " + datatype.split('/')[-1] + ", this is not allowed. Please fix in elektra' ])\n	end\nend\n\n"
# 	fdE.write(deMapStr)
# 	
# 	if os.path.isfile(options.compfile):
# 		os.remove(options.compfile)
# 	fComps = open(options.compfile,'w')
# 	#compsStr = "compositions = {"
# 	portsStr = ""
# 	RootComp = f1.compositions.get(f1.system['/VehicleProject/SYSTEM'].root_type)# f1.compositions.get('/ComponentType/VCCGroup')
# 	
# 	portsStr = compInfoExtractor(f1,RootComp,portsStr,dtBaseTypes)
# 	#for k,swcs in  sorted(RootComp.swcs.iteritems()):
# 	#	comp = f1.compositions.get(swcs)
# 		#compsStr = compsStr + comp.shortname + ",...\n" 
# 	#	compInfoExtractor(f1,comp,portsStr,dtBaseTypes)
# 					
# 
# 				
# 				
# 	
# 	
# 	fComps.write(portsStr)
# 	#fComps.write(compsStr)
# 	
# 	
# 	
# 	
# 	return "0"


# def compInfoExtractor(f1,comp,portsStr,dtBaseTypes):
# 	if comp.swcs:
# 		for k,swcs in  sorted(comp.swcs.iteritems()):
# 			comp2 = f1.compositions.get(swcs)
# 			portsStr = compInfoExtractor(f1,comp2,portsStr,dtBaseTypes)
# 	
# 	for port, ARport in sorted(comp.ports.iteritems()):
# 		pi = f1.port_interfaces.get(ARport.port_interface.text)
# 		for de, datatype in sorted(pi.data_elements.iteritems()):
# 			portName = port + '_' + de
# 			if len(portName) > 63:
# 				portName = portName[0:63]
# 			
# 			dimension = ""
# 			ARdt = f1.datatypes.get(datatype)
# 			if ARdt.getCategory() == 'ARRAY':
# 				dimension = ARdt.getElementType()['max_elems']
# 			else:
# 				dimension = '1'
# 			
# 			portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".port = '" + port + "';\n"
# 			portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".dataelement = '" + de + "';\n"
# 			if ARdt.getCategory() == 'ARRAY':
# 				arrayType = ARdt.getElementType()
# 				
# 				portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".datatype = '" + arrayType['datatype'].split('/')[-1] + "';\n"
# 			else:
# 				portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".datatype = '" + datatype.split('/')[-1] + "';\n"  
# 			
# 			portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".dimension = '" + dimension + "';\n"
# 			portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".portInterface = '" + pi.shortname + "';\n"
# 			
# 			
# 			if ARport.__class__ == autosar.components.AR_PPort:
# 				portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".direction = 'outport';\n"
# 			elif ARport.__class__ == autosar.components.AR_RPort:
# 				portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".direction = 'inport';\n"
# 			else:
# 				portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".direction = 'unknown';\n"
# 			names, values = ARport.initvalue
# 			if ARdt.getCategory() == 'STRUCTURE':
# 				portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".category = 'STRUCTURE';\n" 
# 				
# 				if len(names) == 0:
# 					## initvalue missing in elektra using default 0
# 					elements = ARdt.getElements()
# 					for e in elements:
# 						portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".recordInitvalue." + e.shortname + " = '0';\n"
# 						
# 					
# 				else:
# 					## correct init value
# 					for i in range(0,len(names)):
# 						portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".recordInitvalue." + names[i] + " = '" + values[i] + "';\n"
# 			elif ARdt.getCategory() == 'ARRAY':
# 				portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".category = 'ARRAY';\n" 
# 				arrayinit = '[ '
# 				arrayBase = dtBaseTypes[arrayType['datatype'].split('/')[-1]]
# 				if len(names) == 0:
# 					## initvalue missing in elektra using default 0
# 					for i in range(0, int(dimension)):
# 						arrayinit = arrayinit + arrayBase + "(" + str(0) +") "
# 					arrayinit = arrayinit + ']'
# 					portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".initvalue = " + arrayinit + ";\n"
# 						
# 				else:
# 					## correct init value
# 					for i in range(0,len(values)):
# 						arrayinit = arrayinit + values[i] +","
# 					arrayinit = arrayinit[0:-1] + ']'
# 						
# 					portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".initvalue = " + arrayinit + ";\n"
# 			else:
# 				portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".category = 'VALUE';\n" 
# 				if values.__class__ == list:
# 					portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".initvalue  = '" + values[0] + "';\n"
# 				else:
# 					portsStr = portsStr + "port_compinfo." + comp.shortname + "." + portName + ".initvalue  = '" + values + "';\n"
# 	return portsStr


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
    file = open(infile,'r')
    s = "// "+infile+"\n"
    s += "#ifndef "+definename+"\n"
    s += "#define "+definename+" "+str(crc16.crc16(file.read()))+"\n"
    s += "#endif\n"
    file.close()
    file = open(outfile,'w')
    file.write(s)
    file.close()

def main():
    print '%s (%s)\n%s\n'%(DESCRIPTION, VERSION, COPYRIGHT)
    usage = "Usage: %prog [options] <file>"
    parser = OptionParser(version="%%prog %s (%s)"%(VERSION, autosar.VERSION), usage=usage)

    parser.add_option('-v', '--verbose', dest='verbose', action='store_true', help='Enable verbose output')
    #parser.add_option('-e', '--enumfile', dest='enumfile', help='Output file for enumerations')
    #parser.add_option('-d', '--datatypefile', dest='datatypefile', help='Output file for data types')
    parser.add_option('-d', '--directory', dest='outputdirectory', help='Output directory for generated files')
    #parser.add_option('-c', '--compfile', dest='compfile', help='Output file for composition struct')
    #parser.add_option('-m', '--dataelementfile', dest='dataelementfile', help='Output file for dataelement map')
    parser.add_option('-s', '--swcinputfile', dest='swcinputfile', help='SWC input arxml, (Elektra export file)')
    parser.add_option('-c', '--cominputfile', dest='cominputfile', help='COM input arxml, (Elektra export file)')
    (options, args) = parser.parse_args()

    autosar.arxml.options.verbose = options.verbose

    if options.outputdirectory is None or options.swcinputfile is None:
        parser.error('Missing file(s) on input')
    else:
        if os.path.isfile(options.swcinputfile):
            getDatatypes(options.swcinputfile, options)
        else:
            raise Exception("ERROR: File not found %s" % (options.swcinputfile))
    print "done!"

if __name__ == "__main__":
    main()

