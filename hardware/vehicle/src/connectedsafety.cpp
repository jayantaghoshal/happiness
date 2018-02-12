/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <vhal_v2_0/VehicleUtils.h>

#include "carconfig.h"
#include "cc_parameterlist.h"
#include "connectedsafety.h"

#undef LOG_TAG
#define LOG_TAG "ConnectedSafetySetting"

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
using namespace SettingsFramework;

using namespace CarConfigParams;

static bool isConnectedSafetyEnabledInCarConfig() {
    auto cc_147_ConnSafety = carconfig::getValue<CC147_ConnectedSafetycsType>();
    if (cc_147_ConnSafety == CC147_ConnectedSafetycsType::With_Connected_Safety_level_1 ||
        cc_147_ConnSafety == CC147_ConnectedSafetycsType::With_Connected_Safety_level_2) {
        return true;
    }
    return false;
}

static bool isActiveUsgMode(UsgModSts1 usgModSts) {
    if (usgModSts == UsgModSts1::UsgModCnvinc || usgModSts == UsgModSts1::UsgModActv ||
        usgModSts == UsgModSts1::UsgModDrvg) {
        return true;
    }
    return false;
}

ConnectedSafety::ConnectedSafety(vhal20::impl::IVehicleHalImpl* vehicleHal,
                                 android::sp<SettingsManagerHidl> settings_manager)
    : vhal20::impl::ModuleBase(vehicleHal),
      sConnectedSafety_(SettingId::ConnectedSafety_On, OnOff1::Off, std::move(settings_manager)) {
    connectedSafetyValue_ = OnOff1::Off;
    activeProfile_ = ProfileIdentifier::None;

    if (isConnectedSafetyEnabledInCarConfig()) {
        connectedSafetyPropValue_.areaId = static_cast<int32_t>(VehicleArea::GLOBAL);
        connectedSafetyPropValue_.prop = toInt(VehicleProperty::CONNECTED_SAFETY_ON);
        connectedSafetyPropValue_.value.int32Values.resize(1);
        connectedSafetyPropValue_.value.int32Values[0] = static_cast<int32_t>(connectedSafetyValue_);

        vehModMngtGlbSafe1_.subscribe([this]() { mainLoop(); });

        roadFricIndcnSts_.subscribe([this]() { mainLoop(); });

        sConnectedSafety_.setCallback([this](const auto& value) {
            connectedSafetyValue_ = value.value;
            activeProfile_ = value.profileId;
            mainLoop();
        });

    } else {
        // CARCONFIG_NOT_PRESENT
        // TODO (Joel Gustafsson) Set common status to NotPresent
        autosarConnSafety_.Sts = OnOff1::Off;
        autosarConnSafety_.Pen = static_cast<IdPen>(activeProfile_);
        roadFricIndcnActv_.send(autosarConnSafety_);
    }
}

void ConnectedSafety::mainLoop() {
    isAvailable_ = false;
    auto const roadFricIndcnSts_signal = roadFricIndcnSts_.get();
    auto const vehModMngtGlbSafe1_signal = vehModMngtGlbSafe1_.get();
    if (roadFricIndcnSts_signal.isOk() && vehModMngtGlbSafe1_signal.isOk()) {
        auto fctSts2 = roadFricIndcnSts_signal.value();
        auto vehModMngtGlbSafe1 = vehModMngtGlbSafe1_signal.value();
        if (fctSts2 != FctSts2::Off && fctSts2 != FctSts2::On && isActiveUsgMode(vehModMngtGlbSafe1.UsgModSts)) {
            // SYSTEM_ERROR
            // TODO (Joel Gustafsson) Set common status to SystemError
            ALOGI("Status set to SystemError");
            autosarConnSafety_.Sts = OnOff1::Off;
            autosarConnSafety_.Pen = static_cast<IdPen>(activeProfile_);
            roadFricIndcnActv_.send(autosarConnSafety_);
            connectedSafetyPropValue_.value.int32Values[0] = static_cast<int32_t>(OnOff1::Off);
        } else {
            // SYSTEM_OK
            if (isActiveUsgMode(vehModMngtGlbSafe1.UsgModSts)) {
                // ACTIVE
                // TODO (Joel Gustafsson) Set common status to Available
                isAvailable_ = true;
                ALOGI("Status set to Available");
                ALOGI("Settings value set to: %d", static_cast<int32_t>(connectedSafetyValue_));
                autosarConnSafety_.Sts = connectedSafetyValue_;
                autosarConnSafety_.Pen = static_cast<IdPen>(activeProfile_);
                roadFricIndcnActv_.send(autosarConnSafety_);
                connectedSafetyPropValue_.value.int32Values[0] = static_cast<int32_t>(connectedSafetyValue_);
            } else {
                // NOT_ACTIVE
                // TODO (Joel Gustafsson) Set common status to Disabled
                ALOGI("Status set to Disabled");
                autosarConnSafety_.Sts = OnOff1::Off;
                autosarConnSafety_.Pen = static_cast<IdPen>(activeProfile_);
                roadFricIndcnActv_.send(autosarConnSafety_);
                connectedSafetyPropValue_.value.int32Values[0] = static_cast<int32_t>(OnOff1::Off);
            }
        }
    }
    pushProp(connectedSafetyPropValue_);
}

int ConnectedSafety::setProp(const VehiclePropValue& propValue) {
    // Only allow changes when setting is Available
    if (isAvailable_) sConnectedSafety_.set(static_cast<OnOff1>(propValue.value.int32Values[0]));
    return 0;
}

std::unique_ptr<VehiclePropValue> ConnectedSafety::getProp(const VehiclePropValue& requestedPropValue,
                                                           vhal20::impl::Status&) {
    ALOGV("getProp: 0x%0x", requestedPropValue.prop);
    if (requestedPropValue.prop == toInt(VehicleProperty::CONNECTED_SAFETY_ON)) {
        return std::make_unique<VehiclePropValue>(connectedSafetyPropValue_);
    } else {
        ALOGW("Unknown getProp: 0x%0x", requestedPropValue.prop);
        return nullptr;
    }
}

std::vector<vhal20::VehiclePropValue> ConnectedSafety::getAllPropValues() { return {connectedSafetyPropValue_}; }

std::vector<VehiclePropConfig> ConnectedSafety::listProperties() {
    if (isConnectedSafetyEnabledInCarConfig()) {
        VehiclePropConfig connectedsafetyconfig;
        connectedsafetyconfig.prop = toInt(VehicleProperty::CONNECTED_SAFETY_ON);
        connectedsafetyconfig.access = VehiclePropertyAccess::READ_WRITE;
        connectedsafetyconfig.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        connectedsafetyconfig.areaConfigs.resize(1);
        connectedsafetyconfig.areaConfigs[0].areaId = static_cast<int32_t>(VehicleArea::GLOBAL);
        connectedsafetyconfig.areaConfigs[0].minInt32Value = 0;  // Off
        connectedsafetyconfig.areaConfigs[0].maxInt32Value = 1;  // On

        return {connectedsafetyconfig};
    }
    return {};
}

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
