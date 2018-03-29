/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <carconfig.h>
#include <cc_parameterlist.h>
#include <cutils/properties.h>
#include <cstdlib>
#include <string>

using namespace CarConfigParams;

void handleProp(int paramNumber, std::string propertyName) {
    std::string cc_value;
    cc_value = std::to_string(carconfig::getValue(paramNumber));
    property_set(propertyName.c_str(), cc_value.c_str());
}

int main() {
    // Carconfig parameter 100 - Autonomous Drive
    handleProp(static_cast<int>(CC100_AutonomousDriveType::ParamNumber), "vendor.carconfig.CC100_AutonomousDriveType");

    return EXIT_SUCCESS;
}
