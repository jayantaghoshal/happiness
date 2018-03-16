/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cstdio>
#include <cstdlib>
#include <future>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "gtest/gtest.h"

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;
using namespace ::android::hardware::automotive::vehicle::V2_0;

::android::sp<vhal20::IVehicle> service = vhal20::IVehicle::getService();
// This tests verifies all the Vhal Properties available for applications
// If property with Ivalid Zone is accessed it shows that  property Id along with the zone requested by application.
TEST(VHalSmokeTest, Verify_all_Properties) {
    if (service == nullptr) {
        printf("Service is null, is android.hardware.automotive.vehicle.vcc@2.0-service running?\n");
    }
    std::promise<::android::hardware::hidl_vec<vhal20::VehiclePropConfig>> propertiesPromise;
    auto propertiesFuture = propertiesPromise.get_future();

    const auto service_status = service->getAllPropConfigs(
            [&](const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& prop_configs) {
                propertiesPromise.set_value(prop_configs);
            });
    ASSERT_EQ(service_status.isOk(), true) << "Error in getAllPropConfigs :" << service_status.description().c_str();
    ASSERT_EQ(std::future_status::ready, propertiesFuture.wait_for(std::chrono::milliseconds(1000)));

    for (const vhal20::VehiclePropConfig& property_config_list : propertiesFuture.get()) {
        vhal20::VehiclePropValue prop_value;
        int32_t mask = 1;
        prop_value.prop = property_config_list.prop;

        for (int i = 1; i < 32; i++) {
            if ((mask & property_config_list.supportedAreas) != 0) {
                std::function<void(StatusCode status, const VehiclePropValue& propValue)> test = [&](
                        StatusCode status, const vhal20::VehiclePropValue& prop_value2) {
                    (void)status;
                    (void)prop_value2;
                };
                const auto get_status = service->get(prop_value, test);
                ASSERT_EQ(get_status.isOk(), true) << "Error in get()for Prop id:" << prop_value.prop << "\n"
                                                   << "Zone:" << mask << "\n"
                                                   << get_status.description().c_str() << "\n";
            }
            mask = 1 << i;
        }
    }
}
// Testing invalid property
TEST(VHalSmokeTest, CheckInvalidProperty) {
    vhal20::VehiclePropValue prop_value;
    std::promise<StatusCode> propertiesPromise;
    auto propertiesFuture = propertiesPromise.get_future();
    // Pass invalid property id
    prop_value.prop = 512;

    const auto get_status2 =
            service->get(prop_value, [&](StatusCode status, const vhal20::VehiclePropValue& prop_value2) {
                propertiesPromise.set_value(status);
                (void)prop_value2;
            });

    ASSERT_EQ(get_status2.isOk(), true) << "Error in get() :" << get_status2.description().c_str() << "\n";
    ASSERT_EQ(std::future_status::ready, propertiesFuture.wait_for(std::chrono::milliseconds(1000)));
    // Expect INVALID_ARG from future.get()
    EXPECT_EQ(propertiesFuture.get(), StatusCode::INVALID_ARG);
}
