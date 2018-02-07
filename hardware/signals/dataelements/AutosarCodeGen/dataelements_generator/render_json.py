# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from typing import List, Dict
import typing
from .model import DE_Array, DE_Struct, DE_Type_Key, DE_BaseType
from .render_cpp_basic import get_cpp_type

GEN_JSONENCDEC_CPP_FROMJSON_FUNC_OPENING = """template <>
{STRUCT} fromJson<{STRUCT}>(const json& object) {
    if (!object.is_object()) {
        return {STRUCT}();
    }
    {STRUCT} v;
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
GEN_JSONENCDEC_HPP_FROMTO_ARRAY_TEMPLATE = """// ==========================================
// {ARRAY}
template <> json toJson<{ARRAY}>(const {ARRAY}& v);
template <> {ARRAY} fromJson<{ARRAY}>(const json& j);

"""
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

def render_json(header,
                footer: str,
                arrays: List[DE_Array],
                structs: List[DE_Struct],
                all_types: Dict[DE_Type_Key, DE_BaseType]):
    gen_jsonenc_cpp_contents = header + """
#include "gen_jsonencdec.h"

namespace autosar {

"""


    gen_jsonenc_h_contents = header + """#ifndef _JSONENCDEC_H
#define _JSONENCDEC_H

#include <json_v211.hpp>
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

    #TODO: Is this really needed?
    already_rendered_enums = set()  # type: typing.Set[str]

    for e in sorted(all_types.values(), key=lambda x: x.de_type_name):
        if isinstance(e, DE_Array):
            subtype_name = get_cpp_type(all_types[e.arrayitem_type_id])
            arrayTypeStr = "std::array<%s,%d>" % (subtype_name, e.max_elements)
            if arrayTypeStr not in already_rendered_enums:
                already_rendered_enums.add(arrayTypeStr)
                gen_jsonenc_h_contents += GEN_JSONENCDEC_HPP_FROMTO_ARRAY_TEMPLATE.replace("{ARRAY}", arrayTypeStr)
                func_tojson_str = GEN_JSONENCDEC_CPP_FROMTO_ARRAY_TEMPLATE.replace("{ARRAY}", arrayTypeStr)
                func_tojson_str = func_tojson_str.replace("{N}", str(e.max_elements))
                func_tojson_str = func_tojson_str.replace("{TYPE}", subtype_name)
                gen_jsonenc_cpp_contents += func_tojson_str + "\n\n"
        if isinstance(e, DE_Struct):
            gen_jsonenc_h_contents += GEN_JSONENCDEC_HPP_CLASS_TEMPLATE.replace("{STRUCT}", e.de_type_name)
            func_tojson_str = GEN_JSONENCDEC_CPP_TOJSON_FUNC_OPENING.replace("{STRUCT}", e.de_type_name)
            func_fromjson_str = GEN_JSONENCDEC_CPP_FROMJSON_FUNC_OPENING.replace("{STRUCT}", e.de_type_name)

            for c in e.children:
                subtype_name = get_cpp_type(all_types[c.de_type_id])
                attrname = c.member_name
                if attrname == subtype_name:
                    attrname += "_"

                func_tojson_str += "    j[\"%s\"] = toJson<%s>(v.%s);\n" % (attrname, subtype_name, attrname)
                func_fromjson_str += "    v.%s = fromJson<%s>(object[\"%s\"]);\n" % (attrname, subtype_name, attrname)

            func_tojson_str += "    return j;\n}"
            func_fromjson_str += "    return v;\n}\n\n\n"

            gen_jsonenc_cpp_contents += func_tojson_str + "\n" + func_fromjson_str
    gen_jsonenc_h_contents += "} // end of namespace\n#endif"
    gen_jsonenc_cpp_contents += "} // end of namespace\n"
    gen_jsonenc_h_contents += footer
    gen_jsonenc_cpp_contents += footer

    return (gen_jsonenc_h_contents, gen_jsonenc_cpp_contents)
