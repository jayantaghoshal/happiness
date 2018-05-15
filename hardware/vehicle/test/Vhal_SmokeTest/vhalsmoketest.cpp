/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <cstdio>
#include <cstdlib>
#include <future>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "gtest/gtest.h"

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <android/hardware/automotive/vehicle/2.0/IVehicleCallback.h>
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

#undef LOG_TAG
#define LOG_TAG "VhalSmoketest"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;
using namespace ::android::hardware::automotive::vehicle::V2_0;
namespace andrhw = android::hardware;

class VhalListener : public vhal20::IVehicleCallback {
  public:
    andrhw::Return<void> onPropertyEvent(const andrhw::hidl_vec<vhal20::VehiclePropValue>& /* values */) override {
        ALOGI("onPropertyEvent");
        return andrhw::Return<void>();
    }

    andrhw::Return<void> onPropertySet(const vhal20::VehiclePropValue& /*value*/) override {
        ALOGI("Onpropertyset");
        return andrhw::Return<void>();
    }

    andrhw::Return<void> onPropertySetError(vhal20::StatusCode /* errorCode */,
                                            int32_t /* propId */,
                                            int32_t /* areaId */) override {
        ALOGI("Onpropertyseterror");
        return andrhw::Return<void>();
    }
};

class VHalSmokeTest : public ::testing::Test {
  public:
    VHalSmokeTest() {
        service = vhal20::IVehicle::tryGetService();
        EXPECT_TRUE(service != nullptr)
                << "Service is null, is android.hardware.automotive.vehicle.vcc@2.0-service running?";
    }
    ::android::sp<vhal20::IVehicle> service;
    android::sp<VhalListener> listener = new VhalListener();
    std::vector<int32_t> invalidIdList = {0x00000000, 1, -1};
};

// This tests verifies all the Vhal Properties available for applications
// If property with Ivalid Zone is accessed it shows that  property Id along with the zone requested by application.
TEST_F(VHalSmokeTest, Verify_all_Properties) {
    std::promise<andrhw::hidl_vec<vhal20::VehiclePropConfig>> propertiesPromise;
    auto propertiesFuture = propertiesPromise.get_future();

    const auto service_status =
            service->getAllPropConfigs([&](const andrhw::hidl_vec<vhal20::VehiclePropConfig>& prop_configs) {
                propertiesPromise.set_value(prop_configs);
            });
    ASSERT_EQ(service_status.isOk(), true) << "Error in getAllPropConfigs :" << service_status.description().c_str();
    ASSERT_EQ(std::future_status::ready, propertiesFuture.wait_for(std::chrono::milliseconds(1000)));
    auto propertyConfigList = propertiesFuture.get();

    ALOGD("Number of properties to read: %zu", propertyConfigList.size());

    for (const vhal20::VehiclePropConfig& property_config_list : propertyConfigList) {
        vhal20::VehiclePropValue prop_value;
        prop_value.prop = property_config_list.prop;

        for (const auto& area : property_config_list.areaConfigs) {
            std::function<void(StatusCode status, const VehiclePropValue& propValue)> test = [&](
                    StatusCode status, const vhal20::VehiclePropValue& prop_value2) {
                (void)status;
                ALOGD("Read property OK Prop=%d, Area=%d", prop_value2.prop, prop_value2.areaId);
            };
            prop_value.areaId = area.areaId;
            const auto get_status = service->get(prop_value, test);
            ASSERT_EQ(get_status.isOk(), true) << "Error in get()for Prop id:" << prop_value.prop << "\n"
                                               << "Zone:" << area.areaId << "\n"
                                               << get_status.description().c_str();
        }
    }

    {
        // Try if getPropConfigs works with an existing property
        const std::vector<int32_t> requested_properties = {propertyConfigList[0].prop};
        const auto getConfPropertiesStatus = service->getPropConfigs(
                requested_properties,
                [=](vhal20::StatusCode status, const andrhw::hidl_vec<vhal20::VehiclePropConfig>& prop_config_list) {
                    (void)prop_config_list;
                    ALOGI("GetPropConfig status: %s", toString(status).c_str());
                });
        ASSERT_EQ(getConfPropertiesStatus.isOk(), true) << "Error in getPropConfigs() on defined property  :"
                                                        << getConfPropertiesStatus.description().c_str();
    }
}
// Testing invalid property
TEST_F(VHalSmokeTest, GetInvalidProperty) {
    for (auto invalidId : invalidIdList) {
        vhal20::VehiclePropValue prop_value;
        prop_value.prop = invalidId;
        std::promise<StatusCode> propertiesPromise;
        auto propertiesFuture = propertiesPromise.get_future();

        const auto get_status2 =
                service->get(prop_value, [&](StatusCode status, const vhal20::VehiclePropValue& prop_value2) {
                    propertiesPromise.set_value(status);
                    (void)prop_value2;
                });

        ASSERT_EQ(get_status2.isOk(), true) << "Error in get() on undefined property :"
                                            << get_status2.description().c_str();
        ASSERT_EQ(std::future_status::ready, propertiesFuture.wait_for(std::chrono::milliseconds(1000)));
        EXPECT_EQ(propertiesFuture.get(), StatusCode::INVALID_ARG);
    }
}

TEST_F(VHalSmokeTest, SetInvalidProperty) {
    for (auto invalidId : invalidIdList) {
        vhal20::VehiclePropValue prop_value;
        prop_value.prop = invalidId;
        std::promise<StatusCode> propertiesPromise;
        const andrhw::Return<StatusCode> set_status2 = service->set(prop_value);

        EXPECT_EQ(set_status2.isOk(), true) << "Error in set() on undefined property :"
                                            << set_status2.description().c_str();
        EXPECT_NE((StatusCode)set_status2, StatusCode::OK);
    }
}

TEST_F(VHalSmokeTest, SubscribeInvalidProperty) {
    for (auto invalidId : invalidIdList) {
        andrhw::hidl_vec<vhal20::SubscribeOptions> options = {
                {.propId = invalidId, .flags = vhal20::SubscribeFlags::DEFAULT},
        };
        const andrhw::Return<StatusCode> subscribe_status = service->subscribe(listener, options);
        EXPECT_EQ(subscribe_status.isOk(), true) << "Error in subscribe() on undefined property  :"
                                                 << subscribe_status.description().c_str();
        EXPECT_NE((StatusCode)subscribe_status, StatusCode::OK);
    }
}

// Try if getPropConfigs works with an non existing property
// From IVehicle.hal:
//    * If requested VehicleProperty wasn't found it must return
//    * StatusCode::INVALID_ARG, otherwise a list of vehicle property
//    * configurations with StatusCode::OK
TEST_F(VHalSmokeTest, GetPropConfigInvalidProperty) {
    for (auto invalidId : invalidIdList) {
        const std::vector<int32_t> requested_properties = {invalidId};
        std::promise<StatusCode> statusPromise;
        auto statusFuture = statusPromise.get_future();
        const auto getConfPropertiesStatus = service->getPropConfigs(
                requested_properties,
                [&](vhal20::StatusCode status, const andrhw::hidl_vec<vhal20::VehiclePropConfig>& prop_config_list) {
                    (void)prop_config_list;
                    statusPromise.set_value(status);
                    ALOGI("GetPropConfig status: %s", toString(status).c_str());
                });
        EXPECT_EQ(getConfPropertiesStatus.isOk(), true) << "Error in getPropConfigs() on undefined property  :"
                                                        << getConfPropertiesStatus.description().c_str();
        ASSERT_EQ(std::future_status::ready, statusFuture.wait_for(std::chrono::milliseconds(1000)));
        EXPECT_EQ(statusFuture.get(), StatusCode::INVALID_ARG);
    }
}
