/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include <functional>
#include <memory>
#include <vector>

#include <cutils/log.h>
#include "activeuserprofilemodule.h"

#undef LOG_TAG
#define LOG_TAG "ActiveUserProfileModule"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;

using namespace ApplicationDataElement;
using namespace autosar;
using namespace android;
using namespace ApplicationDataElement;

ActiveUserProfileHal::ActiveUserProfileHal(vhal20::impl::IVehicleHalImpl* vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal) {
    initHandledProperties();
    pushProp(active_user_profile_);

    prof_pen_sts1_receiver_.subscribe([
        this, active_user_profile = active_user_profile_
    ]() mutable {  // Copy active_user_profile_ by value due to multi threaded
        if (getUserProfileFromFlexray(active_user_profile)) {
            ALOGI("ActiveUserProfile changed to: %d", active_user_profile.value.int32Values[0]);
            pushProp(active_user_profile);  // Assuming pushProp in VHAL is thread safe
        }
    });
}

int ActiveUserProfileHal::setProp(const VehiclePropValue& propValue) {
    ALOGD("Setprop: %d", propValue.prop);
    switch (propValue.prop) {
        case toInt(VehicleProperty::ACTIVE_USER_PROFILE): {
            if ((static_cast<int32_t>(IdPen::Prof1) > propValue.value.int32Values[0]) ||
                (static_cast<int32_t>(IdPen::ProfAll) < propValue.value.int32Values[0])) {
                ALOGW("VehicleProperty::ACTIVE_USER_PROFILE request out of range: %d", propValue.value.int32Values[0]);
                return 1;
            }
            prof_chg_sender_.send(static_cast<IdPen>(propValue.value.int32Values[0]));
            return 0;
        }
        default: {
            ALOGE("Trying to set property not handled by activeuserprofilemodule: %d", propValue.prop);
            return 1;
        }
    }
}

std::unique_ptr<VehiclePropValue> ActiveUserProfileHal::getProp(const VehiclePropValue& requestedPropValue) {
    ALOGD("getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
    getUserProfileFromFlexray(active_user_profile_);
    return std::make_unique<VehiclePropValue>(active_user_profile_);
}

std::vector<VehiclePropValue> ActiveUserProfileHal::getAllPropValues() { return {active_user_profile_}; }

std::vector<VehiclePropConfig> ActiveUserProfileHal::listProperties() {
    std::vector<VehiclePropConfig> propConfigs;
    {
        VehiclePropConfig propConfig;

        // Config of ACTIVE_USER_PROFILE
        propConfig.prop = toInt(VehicleProperty::ACTIVE_USER_PROFILE);
        propConfig.access = VehiclePropertyAccess::READ_WRITE;
        propConfig.changeMode = VehiclePropertyChangeMode::ON_CHANGE;

        propConfigs.push_back(propConfig);
    }
    return propConfigs;
}

void ActiveUserProfileHal::initHandledProperties() {
    {
        // Config of ACTIVE_USER_PROFILE
        active_user_profile_.prop = toInt(VehicleProperty::ACTIVE_USER_PROFILE);
        active_user_profile_.value.int32Values.resize(1);
        getUserProfileFromFlexray(active_user_profile_);
    }
}

bool ActiveUserProfileHal::getUserProfileFromFlexray(vhal20::VehiclePropValue& active_user_profile) {
    if (prof_pen_sts1_receiver_.get().isOk()) {
        active_user_profile.value.int32Values[0] = static_cast<uint8_t>(prof_pen_sts1_receiver_.get().value());
        active_user_profile.timestamp = elapsedRealtimeNano();
        return true;
    } else {
        ALOGI("%s signal ProfPenSts1 is in error.", __FUNCTION__);
        return false;
    }
}

}  // impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor