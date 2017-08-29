from typing import List, Dict

from .model import DE_Element, DE_Type_Key, DE_BaseType, DE_Value, DE_Identical, DE_Boolean, \
    DE_Struct, DE_Array
from .render_cpp_basic import escape_cpp_comment, create_cpp_brief_comment, get_cpp_type

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

GEN_VSM_INJECT_SIGNAL_SWITCH_OK = """    case ComConf_ComSignal_is{RTETYPE}_mrx:
    {
        if (sizeof({RTETYPE}) == length) {
            ALOGV("Received {RTETYPE} (%u)", ComConf_ComSignal_is{RTETYPE}_mrx);
            const {RTETYPE}& rteValue = *static_cast<const {RTETYPE}*>(buffer);
            autosar::{DATAELEM}_info::data_elem_type deValue;
            {TRANSFER};
            {DATAELEM}_de->inject(deValue);
        } else {
            ALOGE("Wrong buffer size received for {RTETYPE} (%u). Got %zu , expected %lu", ComConf_ComSignal_is{RTETYPE}_mrx, length, static_cast<unsigned long>(sizeof({RTETYPE})));
        }
    }
    break;

"""

GEN_VSM_SINK_TRANSFER_ARRAY_COPY = """for (unsigned int i=0; i<deValue.size(); ++i) rteValue[i] = static_cast<std::remove_reference<decltype( *rteValue )>::type>( deValue[i] )"""
GEN_VSM_SINK_SIGNALGROUP_TRANSFER_SCALE = """    rteValue.{RTEATTR} = round((deValue.{DEATTR} - {OFFSET}) / {SCALE})"""
GEN_VSM_SINK_SIGNALGROUP_TRANSFER = """    rteValue.{RTEATTR} = static_cast<decltype(rteValue.{RTEATTR})>(deValue.{DEATTR})"""
GEN_VSM_SINK_SIGNAL_TRANSFER_SCALE = """rteValue = round((deValue - {OFFSET}) / {SCALE})"""
GEN_VSM_SINK_SIGNAL_TRANSFER = """rteValue = static_cast<decltype(rteValue)>(deValue)"""
GEN_VSM_SINK_SIGNALGROUP_SUBSCRIBE = """{DATAELEM}_de = new DESink<autosar::{DATAELEM}_info>();
{DATAELEM}_de->subscribe([]() {
    auto deValue = {DATAELEM}_de->get().value();
    {RTETYPE} rteValue;
{TRANSFER}
    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_ig{RTETYPE}_mtx, &rteValue, sizeof(rteValue) ); 
});

"""
GEN_VSM_SINK_SIGNAL_SUBSCRIBE = """{DATAELEM}_de = new DESink<autosar::{DATAELEM}_info>();
{DATAELEM}_de->subscribe([]() {
    auto deValue = {DATAELEM}_de->get().value();
    {RTETYPE} rteValue;
    {TRANSFER};
    sendAvmpMessageToVip( ComConf_ComSignal_is{RTETYPE}_mtx, &rteValue, sizeof(rteValue) ); 
});

"""
GEN_VSM_SINK_VARIABLE_TEMPLATE = """    static DESink<autosar::{DATAELEM}_info>* {DATAELEM}_de=nullptr;

"""
GEN_VSM_INJECT_SIGNALGROUP_TRANSFER_LINEAR = """            deValue.{DEATTR} = static_cast<decltype(deValue.{DEATTR})>( {FUNC}<{SIZE}>(rteValue.{RTEATTR}) )"""
GEN_VSM_INJECT_SIGNALGROUP_TRANSFER = """            deValue.{DEATTR} = static_cast<decltype(deValue.{DEATTR})>(rteValue.{RTEATTR})"""
GEN_VSM_INJECT_SIGNAL_TRANSFER_LINEAR = """deValue = static_cast<decltype(deValue)>( {FUNC}<{SIZE}>(rteValue) )"""
GEN_VSM_INJECT_SIGNAL_TRANSFER = """deValue = static_cast<decltype(deValue)>(rteValue)"""
GEN_VSM_INJECT_TRANSFER_ARRAY_COPY = """for (unsigned int i=0; i<deValue.size(); ++i) deValue[i] = static_cast<autosar::{DATAELEM}_info::data_elem_type::value_type>( rteValue[i] )"""
GEN_VSM_INJECT_SIGNAL_SWITCH_ERROR = """    case ComConf_ComSignal_is{RTETYPE}_mrx:
        {DATAELEM}_de->error(errorCode);
        break;

"""
GEN_VSM_INJECT_SIGNALGROUP_SWITCH_ERROR = """    case SignalGroup|ComConf_ComSignalGroup_ig{RTETYPE}_mrx:
        {DATAELEM}_de->error(errorCode);
        break;

"""
GEN_VSM_INJECT_SIGNALGROUP_SWITCH_OK = """    case SignalGroup|ComConf_ComSignalGroup_ig{RTETYPE}_mrx:
    {
        if (sizeof({RTETYPE}) == length) {
            ALOGV("Received {RTETYPE} (%u)", ComConf_ComSignalGroup_ig{RTETYPE}_mrx);
            const {RTETYPE}& rteValue = *static_cast<const {RTETYPE}*>(buffer);
            autosar::{DATAELEM}_info::data_elem_type deValue;
{TRANSFER}
            {DATAELEM}_de->inject(deValue);
        } else {
            ALOGE("Wrong buffer size received for {RTETYPE} (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_ig{RTETYPE}_mrx, length, static_cast<unsigned long>(sizeof({RTETYPE})));        
        }
    }
    break;

"""


def render_dataelments(header: str, footer: str, all_de_elements: List[DE_Element], all_types: Dict[DE_Type_Key, DE_BaseType]):

    # TODO: Split this into VIP and DataElements?


    dataElementsHeaderStr = header + """#ifndef _DATAELEMENTS_H
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

    dataElemetsCppStr = header + """#include \"gen_dataelements.h\"


namespace autosar {

"""
    gen_vsm_all_dataelements_cpp = header + "\n"
    gen_vsm_inject_variable_cpp = header + "\n"
    gen_vsm_inject_instance_cpp = header + "\n"
    gen_vsm_inject_switch_ok_cpp = header + "\n"
    gen_vsm_inject_switch_error_cpp = header + "\n"
    gen_vsm_sink_variable_cpp = header + "\n"
    gen_vsm_sink_subscribe_cpp = header + "\n"


    for de_de in all_de_elements:
        tmpTransferInject = ''
        tmpTransferSink = ''

        de_type = all_types[de_de.de_type_id]
        if isinstance(de_type, DE_Value):
            func = "toSignedFromRaw" if de_type.is_signed else "toUnsignedFromRaw"
            tmpTransferInject = GEN_VSM_INJECT_SIGNAL_TRANSFER_LINEAR.replace("{FUNC}", func).replace("{SIZE}", str(de_type.nr_of_bits))
            tmpTransferSink = GEN_VSM_SINK_SIGNAL_TRANSFER_SCALE.replace("{SCALE}", str(de_type.scale)).replace(
                "{OFFSET}", str(de_type.offset))

            if not de_type.no_scale:
                tmpTransferInject += " * " + str(de_type.scale) + " + " + str(de_type.offset)

        elif isinstance(de_type, DE_Identical):
            tmpTransferInject = GEN_VSM_INJECT_SIGNAL_TRANSFER
            tmpTransferSink = GEN_VSM_SINK_SIGNAL_TRANSFER
        elif isinstance(de_type, DE_Boolean):
            tmpTransferInject = GEN_VSM_INJECT_SIGNAL_TRANSFER
            tmpTransferSink = GEN_VSM_SINK_SIGNAL_TRANSFER
        elif isinstance(de_type, DE_Struct):
            if not de_de.is_internal:
                for e in sorted(de_type.children, key=lambda x: x.member_name):
                    subType = all_types[e.de_type_id]
                    rteattr = de_de.rte_attr_map[e.member_name]
                    member_name = e.member_name
                    if member_name == subType.de_type_name:
                        member_name += "_"

                    if isinstance(subType, DE_Value):
                        func = "toSignedFromRaw" if subType.is_signed else "toUnsignedFromRaw"

                        if subType.scale is not None:
                            tmpTransferInject += GEN_VSM_INJECT_SIGNALGROUP_TRANSFER_LINEAR.replace("{FUNC}", func).replace(
                                "{SIZE}", str(subType.nr_of_bits)).replace("{DEATTR}", member_name).replace("{RTEATTR}",
                                                                                      rteattr) + " * " + str(subType.scale) + " + " + str(subType.offset) + ";\n"
                            tmpTransferSink += GEN_VSM_SINK_SIGNALGROUP_TRANSFER_SCALE.replace("{DEATTR}",
                                                                                               member_name).replace(
                                "{RTEATTR}", rteattr).replace("{SCALE}", str(subType.scale)).replace("{OFFSET}", str(subType.offset)) + ";\n"
                        else:
                            tmpTransferInject += GEN_VSM_INJECT_SIGNALGROUP_TRANSFER_LINEAR.replace("{FUNC}",
                                                                                                    func).replace(
                                "{SIZE}", str(subType.nr_of_bits)).replace("{DEATTR}", member_name).replace("{RTEATTR}",
                                                                                                        rteattr) + ";\n"
                            tmpTransferSink += GEN_VSM_SINK_SIGNALGROUP_TRANSFER.replace("{DEATTR}", member_name).replace(
                                "{RTEATTR}", rteattr) + ";\n"
                    else:
                        tmpTransferInject += \
                            GEN_VSM_INJECT_SIGNALGROUP_TRANSFER.replace("{DEATTR}", member_name).replace("{RTEATTR}",
                                                                                                    rteattr) + ";\n"
                        tmpTransferSink += GEN_VSM_SINK_SIGNALGROUP_TRANSFER.replace("{DEATTR}", member_name).replace(
                            "{RTEATTR}", rteattr) + ";\n"
            else:
                pass #????
        elif isinstance(de_type, DE_Array):
            tmpTransferInject = GEN_VSM_INJECT_TRANSFER_ARRAY_COPY.replace("{DATAELEM}", de_de.de_dataelementname)
            if (de_de.isSignal):
                tmpTransferSink = GEN_VSM_SINK_TRANSFER_ARRAY_COPY
            else:
                tmpTransferSink = "    " + GEN_VSM_SINK_TRANSFER_ARRAY_COPY + ";"
        else:
            tmpTransferInject = GEN_VSM_INJECT_SIGNAL_TRANSFER
            tmpTransferSink = GEN_VSM_SINK_SIGNAL_TRANSFER

        if de_de.description != "":
            descComment = "/*!\n * \\brief Signal %s\n * %s\n */\n" % (de_de.de_dataelementname, escape_cpp_comment(de_de.description.strip()))
        else:
            descComment = "/*!\n * \\brief Signal %s\n */\n" % (de_de.de_dataelementname)

        if de_de.is_internal:
            dirtag = "InternalTag, "
        else:
            dirtag = "InTag, " if de_de.is_insignal else "OutTag, "

        range_comment = ""
        if isinstance(de_type, (DE_Value, DE_Identical)):
            range_comment = " " + create_cpp_brief_comment(de_type.desc.strip())

        dataElementsHeaderStr += GEN_DATAELEMENTS_HPP_CLASS_TEMPLATE % (descComment, de_de.de_dataelementname + "_info", dirtag, get_cpp_type(de_type), range_comment)
        if de_de.is_internal:
            direct = "Dir::INTERNAL"
        else:
            direct = "Dir::IN" if de_de.is_insignal else "Dir::OUT"

        tmpStr = GEN_DATAELEMENTS_CPP_CLASS_TEMPLATE % (de_de.de_dataelementname, get_cpp_type(de_type), direct)
        dataElemetsCppStr += tmpStr.replace("{CLASS}", de_de.de_dataelementname + "_info")
        gen_vsm_all_dataelements_cpp += GEN_VSM_ALL_DATAELEMENTS.replace("{CLASS}", de_de.de_dataelementname + "_info")
        if de_de.is_insignal:
            if not de_de.is_internal:
                gen_vsm_inject_variable_cpp += GEN_VSM_INJECT_VARIABLE_TEMPLATE.replace("{RTETYPE}",
                                                                                        de_de.rtename).replace(
                    "{DATAELEM}", de_de.de_dataelementname)
                gen_vsm_inject_instance_cpp += GEN_VSM_INJECT_INSTANCE_TEMPLATE.replace("{RTETYPE}",
                                                                                        de_de.rtename).replace(
                    "{DATAELEM}", de_de.de_dataelementname)
                if de_de.isSignal:
                    gen_vsm_inject_switch_ok_cpp += GEN_VSM_INJECT_SIGNAL_SWITCH_OK.replace("{RTETYPE}",
                                                                                            de_de.rtename).replace(
                        "{DATAELEM}", de_de.de_dataelementname).replace("{TRANSFER}", tmpTransferInject)
                    gen_vsm_inject_switch_error_cpp += GEN_VSM_INJECT_SIGNAL_SWITCH_ERROR.replace("{RTETYPE}",
                                                                                                  de_de.rtename).replace(
                        "{DATAELEM}", de_de.de_dataelementname)
                else:
                    gen_vsm_inject_switch_ok_cpp += GEN_VSM_INJECT_SIGNALGROUP_SWITCH_OK.replace("{RTETYPE}",
                                                                                                 de_de.rtename).replace(
                        "{DATAELEM}", de_de.de_dataelementname).replace("{TRANSFER}", tmpTransferInject)
                    gen_vsm_inject_switch_error_cpp += GEN_VSM_INJECT_SIGNALGROUP_SWITCH_ERROR.replace("{RTETYPE}",
                                                                                                       de_de.rtename).replace(
                        "{DATAELEM}", de_de.de_dataelementname)
            else:
                gen_vsm_inject_variable_cpp += "// ***************************************\n// " + de_de.de_dataelementname + " not found in Com file\n\n"
                gen_vsm_inject_instance_cpp += "// ***************************************\n// " + de_de.de_dataelementname + " not found in Com file\n\n"
        else:
            if not de_de.is_internal:
                gen_vsm_sink_variable_cpp += GEN_VSM_SINK_VARIABLE_TEMPLATE.replace("{RTETYPE}",
                                                                                    de_de.rtename).replace(
                    "{DATAELEM}", de_de.de_dataelementname)
                if de_de.isSignal:
                    gen_vsm_sink_subscribe_cpp += GEN_VSM_SINK_SIGNAL_SUBSCRIBE.replace("{RTETYPE}",
                                                                                        de_de.rtename).replace(
                        "{DATAELEM}", de_de.de_dataelementname).replace("{TRANSFER}", tmpTransferSink)
                else:
                    gen_vsm_sink_subscribe_cpp += GEN_VSM_SINK_SIGNALGROUP_SUBSCRIBE.replace("{RTETYPE}",
                                                                                             de_de.rtename).replace(
                        "{DATAELEM}", de_de.de_dataelementname).replace("{TRANSFER}", tmpTransferSink)

    dataElementsHeaderStr += "} // end of namespace\n#endif"
    dataElemetsCppStr += "} // end of namespace\n"

    dataElementsHeaderStr += footer
    dataElemetsCppStr += footer
    gen_vsm_all_dataelements_cpp += footer 
    gen_vsm_inject_variable_cpp += footer
    gen_vsm_inject_instance_cpp += footer
    gen_vsm_inject_switch_ok_cpp += footer
    gen_vsm_inject_switch_error_cpp += footer
    gen_vsm_sink_variable_cpp += footer
    gen_vsm_sink_subscribe_cpp += footer

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