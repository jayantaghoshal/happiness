/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <stdio.h>
#include <map>

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

int main(int argc, char* argv[]) {
    ::android::sp<vhal20::IVehicle> service = vhal20::IVehicle::getService();
    if (service == nullptr) {
        printf("Service is null, is android.hardware.automotive.vehicle.vcc@2.0-service running?\n");
        return 1;
    }

    vhal20::VehiclePropValue requestedPropValue;
    requestedPropValue.prop = static_cast<int32_t>(vccvhal10::VehicleProperty::CARCONFIG);

    service->get(requestedPropValue, [](vhal20::StatusCode status, const vhal20::VehiclePropValue& propValue) {
        std::map<vhal20::StatusCode, std::string> codesToString{{vhal20::StatusCode::OK, "OK"},
                                                                {vhal20::StatusCode::TRY_AGAIN, "TRY_AGAIN"},
                                                                {vhal20::StatusCode::INVALID_ARG, "INVALID_ARG"},
                                                                {vhal20::StatusCode::NOT_AVAILABLE, "NOT_AVAILABLE"},
                                                                {vhal20::StatusCode::ACCESS_DENIED, "ACCESS_DENIED"},
                                                                {vhal20::StatusCode::INTERNAL_ERROR, "INTERNAL_ERROR"}};

        printf("StatusCode: %s \n", codesToString[status].c_str());
        printf("Property id: %d\n", propValue.prop);
        int i = 1;
        for (const auto& ccVal : propValue.value.bytes) {
            printf("Carconfig %d: %d\n", i, ccVal);
            i++;
        }
    });
}
