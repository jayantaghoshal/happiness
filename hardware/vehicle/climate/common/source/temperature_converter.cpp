/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "temperature_converter.h"

#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>
#include <algorithm>
#include <array>
#include <utility>
#undef LOG_TAG
#define LOG_TAG "TemperatureConverter"
#include <log/log.h>

namespace {
double min = 17;
double max = 27;

// This should be changed if resolution is changed (ie. the other map is used, see below)
double mccLo = 1;
double mccHi = 23;

// constexpr std::array<const std::pair<double, double>, 11> celsiusFahrenheitMap{{
//     std::make_pair(17, 62),
//     std::make_pair(18, 64),
//     std::make_pair(19, 66),
//     std::make_pair(20, 68),
//     std::make_pair(21, 70),
//     std::make_pair(22, 72), // Default
//     std::make_pair(23, 74),
//     std::make_pair(24, 76),
//     std::make_pair(25, 78),
//     std::make_pair(26, 80),
//     std::make_pair(27, 82)
// }};

// constexpr std::array<const std::pair<double, double>, 11> celsiusMCCMap{{
//     std::make_pair(15, 1),
//     std::make_pair(16, 2),
//     std::make_pair(17, 3),
//     std::make_pair(18, 4),
//     std::make_pair(19, 5),
//     std::make_pair(20, 6), // Default
//     std::make_pair(21, 7),
//     std::make_pair(22, 8),
//     std::make_pair(23, 9),
//     std::make_pair(24, 10),
//     std::make_pair(25, 11)
// }};

// Old map then half-degrees was a thing, keep for if they change their minds.
constexpr std::array<const std::pair<double, double>, 21> celsiusFahrenheitMap{
        {std::make_pair(17, 62), std::make_pair(17.5, 63), std::make_pair(18, 64), std::make_pair(18.5, 65),
         std::make_pair(19, 66), std::make_pair(19.5, 67), std::make_pair(20, 68), std::make_pair(20.5, 69),
         std::make_pair(21, 70), std::make_pair(21.5, 71), std::make_pair(22, 72), std::make_pair(22.5, 73),
         std::make_pair(23, 74), std::make_pair(23.5, 75), std::make_pair(24, 76), std::make_pair(24.5, 77),
         std::make_pair(25, 78), std::make_pair(25.5, 79), std::make_pair(26, 80), std::make_pair(26.5, 81),
         std::make_pair(27, 82)}};

constexpr std::array<const std::pair<double, double>, 23> celsiusMCCMap{
        {std::make_pair(15, 1),    std::make_pair(16, 2),    std::make_pair(16.5, 3),  std::make_pair(17, 4),
         std::make_pair(18, 5),    std::make_pair(18.5, 6),  std::make_pair(19, 7),    std::make_pair(20, 8),
         std::make_pair(20.5, 9),  std::make_pair(21, 10),   std::make_pair(22, 11),   std::make_pair(22.5, 12),
         std::make_pair(23, 13),   std::make_pair(23.5, 14), std::make_pair(24.5, 15), std::make_pair(25, 16),
         std::make_pair(25.5, 17), std::make_pair(26.5, 18), std::make_pair(27, 19),   std::make_pair(27.5, 20),
         std::make_pair(28.5, 21), std::make_pair(29, 22),   std::make_pair(29.5, 23)}};

double rangeCheck(double celsius) {
    auto retval = celsius;

    if (celsius < min) {
        retval = celsiusFahrenheitMap.front().first;
    } else if (celsius > max) {
        retval = celsiusFahrenheitMap.back().first;
    }

    return retval;
}

double celsiusToFahrenheit(double celsius) {
    auto retval = 72.0;

    if (celsius < min) {
        retval = celsiusFahrenheitMap.front().second;
    } else if (celsius > max) {
        retval = celsiusFahrenheitMap.back().second;
    } else {
        auto foundPair =
                std::find_if(celsiusFahrenheitMap.begin(),
                             celsiusFahrenheitMap.end(),
                             [celsius](const std::pair<double, double>& item) { return item.first == celsius; });

        if (foundPair != celsiusFahrenheitMap.end()) {
            retval = foundPair->second;
        }
    }

    return retval;
}

double fahrenheitToCelsius(double fahrenheit) {
    auto retval = 22.0;

    if (fahrenheit < celsiusToFahrenheit(min)) {
        retval = celsiusFahrenheitMap.front().first;
    } else if (fahrenheit > celsiusToFahrenheit(max)) {
        retval = celsiusFahrenheitMap.back().first;
    } else {
        auto foundPair =
                std::find_if(celsiusFahrenheitMap.begin(),
                             celsiusFahrenheitMap.end(),
                             [fahrenheit](const std::pair<double, double>& item) { return item.second == fahrenheit; });

        if (foundPair != celsiusFahrenheitMap.end()) {
            retval = foundPair->first;
        }
    }

    return retval;
}

double celsiusToMCC(double celsius) {
    auto retval = 11.0;  // This should be changed if resolution is changed

    auto foundPair = std::find_if(celsiusMCCMap.begin(),
                                  celsiusMCCMap.end(),
                                  [celsius](const std::pair<double, double>& item) { return item.first == celsius; });

    if (foundPair != celsiusMCCMap.end()) {
        retval = foundPair->second;
    } else {
        if (celsius < celsiusMCCMap.front().first) {
            retval = celsiusMCCMap.front().second;
        } else if (celsius > celsiusMCCMap.back().first) {
            retval = celsiusMCCMap.back().second;
        }
    }

    return retval;
}

double mccToCelsius(double mcc) {
    auto retval = 22.0;

    auto foundPair = std::find_if(celsiusMCCMap.begin(),
                                  celsiusMCCMap.end(),
                                  [mcc](const std::pair<double, double>& item) { return item.second == mcc; });

    if (foundPair != celsiusMCCMap.end()) {
        retval = foundPair->first;
    } else {
        if (mcc < celsiusMCCMap.front().second) {
            retval = celsiusMCCMap.front().first;
        } else if (mcc > celsiusMCCMap.back().second) {
            retval = celsiusMCCMap.back().first;
        }
    }

    return retval;
}

bool isCarConfigMCC() {
    try {
        auto ccParam = carconfig::getValue<CarConfigParams::CC175_HvacVariantsType>();
        return ccParam == CarConfigParams::CC175_HvacVariantsType::Manual_AC;
    } catch (std::out_of_range& e) {
        log_error() << "CC175 not present in CarConfig!";
    }

    return false;
}
}  // namespace

namespace common {
namespace daemon {

TemperatureConverter::TemperatureConverter() : isMCC_(isCarConfigMCC()) {}

TemperatureConverter::~TemperatureConverter() = default;

double TemperatureConverter::toSingle(autosar::AmbTIndcdUnit unit, double temp) const {
    if (isMCC_) {
        return celsiusToMCC(temp);
    }

    switch (unit) {
        case autosar::AmbTIndcdUnit::Fahrenheit:
            temp = celsiusToFahrenheit(temp);
            break;
        case autosar::AmbTIndcdUnit::UkwnUnit:
        case autosar::AmbTIndcdUnit::Celsius:
            break;
    }

    return temp;
}

double TemperatureConverter::fromSingle(autosar::AmbTIndcdUnit unit, double temp) const {
    if (isMCC_) {
        temp = roundToClosestWholeDegree(temp);
        if (temp < mccLo) {
            ALOGI("temp request below range");
            temp = mccLo;
        } else if (temp > mccHi) {
            ALOGI("temp request above range");
            temp = mccHi;
        }
        return mccToCelsius(temp);
    }

    switch (unit) {
        case autosar::AmbTIndcdUnit::Fahrenheit:
            temp = roundToClosestWholeDegree(temp);
            temp = fahrenheitToCelsius(temp);
            break;
        case autosar::AmbTIndcdUnit::UkwnUnit:
        case autosar::AmbTIndcdUnit::Celsius:
            temp = roundToClosestHalfDegree(temp);
            temp = rangeCheck(temp);
            break;
    }

    return temp;
}

std::pair<double, double> TemperatureConverter::range(autosar::AmbTIndcdUnit unit) const {
    if (isMCC_) {
        return std::make_pair(1, 23);
    }

    switch (unit) {
        case autosar::AmbTIndcdUnit::Fahrenheit:
            return std::make_pair(celsiusFahrenheitMap.front().second, celsiusFahrenheitMap.back().second);
            break;
        case autosar::AmbTIndcdUnit::UkwnUnit:
        case autosar::AmbTIndcdUnit::Celsius:
            break;
    }

    return std::make_pair(celsiusFahrenheitMap.front().first, celsiusFahrenheitMap.back().first);
}

double TemperatureConverter::roundToClosestWholeDegree(double temp) const { return std::round(temp); }

double TemperatureConverter::roundToClosestHalfDegree(double temp) const { return std::round(temp * 2) / 2; }

}  // namespace daemon
}  // namespace common
