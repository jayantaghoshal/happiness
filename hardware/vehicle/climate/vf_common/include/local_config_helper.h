/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <vcc/localconfig.h>

#include <chrono>
#include <stdexcept>
#include <type_traits>

namespace util
{

namespace detail
{
template <typename T>
constexpr bool is_localconfig_type_v
    = std::is_same<T, char>::value; //TODO(climateport): char=hack to make this always false to avoid GetValue<T>

template <typename T, bool IsEnum = std::is_enum<T>::value>
struct get_localconfig_type
{
    using type = typename std::underlying_type<T>::type;
};

template <typename T>
struct get_localconfig_type<T, false>
{
    using type = T;
};

template <typename T>
using get_localconfig_type_t = typename get_localconfig_type<T>::type;

template <typename T, bool IsLocalConfigType = is_localconfig_type_v<T> || std::is_enum<T>::value>
struct readLocalConfig
{
    static T apply(std::string const& parameter)
    {
        static_assert(is_localconfig_type_v<get_localconfig_type_t<T>>, "Not a Local Config type");
        const auto value = vcc::detail::GetValue<get_localconfig_type_t<T>>(parameter);
        return static_cast<T>(value);        
    }
};

template<>
struct readLocalConfig<int, true>
{
    static int apply(std::string const& parameter)
    {
        auto * lcfg = vcc::LocalConfigDefault();
        return lcfg->GetInt(parameter); 
    }
};
template<>
struct readLocalConfig<bool, false>
{
    static bool apply(std::string const& parameter)
    {
        auto * lcfg = vcc::LocalConfigDefault();
        return lcfg->GetBool(parameter); 
    }
};
template<>
struct readLocalConfig<double, false>
{
    static double apply(std::string const& parameter)
    {
        auto * lcfg = vcc::LocalConfigDefault();
        return lcfg->GetDouble(parameter); 
    }
};


template <>
struct readLocalConfig<std::chrono::seconds, false>
{
    using Type = std::chrono::seconds;
    static Type apply(std::string const& parameter)
    {
        auto * lcfg = vcc::LocalConfigDefault();
        const auto value = lcfg->GetInt(parameter);
        return Type{ value };        
    }
};

template <>
struct readLocalConfig<std::chrono::minutes, false>
{
    using Type = std::chrono::minutes;
    static Type apply(std::string const& parameter)
    {
        auto * lcfg = vcc::LocalConfigDefault();
        const auto value = lcfg->GetInt(parameter);
        return Type{ value };        
    }
};

template <>
struct readLocalConfig<std::chrono::milliseconds, false>
{
    using Type = std::chrono::milliseconds;
    static Type apply(std::string const& parameter)
    {
        auto * lcfg = vcc::LocalConfigDefault();
        const auto value = lcfg->GetInt(parameter);
        return Type{ value };        
    }
};
}

template <typename T>
T readLocalConfig(std::string const& parameter)
{
    return detail::readLocalConfig<T>::apply(parameter);
}
}
