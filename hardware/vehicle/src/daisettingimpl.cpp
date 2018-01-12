/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "daisettingimpl.h"
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "vcc/localconfig.h"

#undef LOG_TAG
#define LOG_TAG "DAISettingImpl"

using namespace ApplicationDataElement;
using namespace autosar;
using namespace vcc;
using namespace CarConfigParams;

namespace {
static bool isActiveUsgMode(UsgModSts1 usgModSts) {
    if (usgModSts == UsgModSts1::UsgModCnvinc || usgModSts == UsgModSts1::UsgModActv ||
        usgModSts == UsgModSts1::UsgModDrvg) {
        return true;
    }
    return false;
}
}

bool isDriveAwayInfoEnabledInCarConfig() {
    auto cc_315_DAI = carconfig::getValue<CC315_DriveAwayInformationType>();
    if (cc_315_DAI == CC315_DriveAwayInformationType::Drive_Away_Information_level_1 ||
        cc_315_DAI == CC315_DriveAwayInformationType::Drive_Away_Information_level_2) {
        return true;
    }
    return false;
}

DAISettingImpl::DAISettingImpl(android::sp<SettingsFramework::SettingsManagerHidl> settings_manager)
    : sDAISetting_(SettingId::DriveAwayInfoSetting,
                   isDriveAwayInfoEnabledInCarConfig() ? DAISettingType::VISUAL_AND_SOUND : DAISettingType::NOTHING,
                   settings_manager) {
    auto* lcfg = vcc::LocalConfigDefault();
    const bool lcfg_DAI_enabled = lcfg->GetBool("vehiclefunctions.adas.LCFG_DAI_Enabled");

    // SYSTEM_OK
    if ((isDriveAwayInfoEnabledInCarConfig()) && lcfg_DAI_enabled == true) {
        sDAISetting_.setCallback([&](const auto& value) {
            auto settings_value = value.value;

            switch (settings_value) {
                case DAISettingType::NOTHING:
                    autosarInfoActv_ = OnOff1::Off;
                    autosarSoundWarnActv_ = OnOff1::Off;
                    break;

                case DAISettingType::VISUAL:
                    autosarInfoActv_ = OnOff1::On;
                    autosarSoundWarnActv_ = OnOff1::Off;
                    break;

                case DAISettingType::VISUAL_AND_SOUND:
                    autosarInfoActv_ = OnOff1::On;
                    autosarSoundWarnActv_ = OnOff1::On;
                    break;

                default:
                    ALOGE("Non-supported DAI setting: %d\n", settings_value);
                    return;
            }
            infoActvSender_.send(autosarInfoActv_);
            infoSoundWarnActvSender_.send(autosarSoundWarnActv_);
            daiSetting_.set(static_cast<int32_t>(settings_value));
        });

        vehModMngtGlbSafe1_.subscribe([&]() {
            auto vehModMngtGlbSafe1_signal = vehModMngtGlbSafe1_.get();
            if (vehModMngtGlbSafe1_signal.isOk()) {
                auto frame = vehModMngtGlbSafe1_signal.value();
                if (isActiveUsgMode(frame.UsgModSts)) {
                    // TODO: Set common status to Available
                    ALOGI("Status set to Available");
                } else {
                    // TODO: Set common status to Disabled
                    ALOGI("Status set to Disabled");
                }
            }
        });

    } else {
        // CARCONFIG_NOT_PRESENT
        // TODO: Set common status to NotPresent
        autosarInfoActv_ = OnOff1::Off;
        autosarSoundWarnActv_ = OnOff1::Off;
        infoActvSender_.send(autosarInfoActv_);
        infoSoundWarnActvSender_.send(autosarSoundWarnActv_);
    }
}

ReadOnlyNotifiableProperty<int32_t>* DAISettingImpl::DAISetting() { return &daiSetting_; }

void DAISettingImpl::setDAISetting(int settings_value) {
    auto vehModMngtGlbSafe1_signal = vehModMngtGlbSafe1_.get();
    if (vehModMngtGlbSafe1_signal.isOk()) {
        auto frame = vehModMngtGlbSafe1_signal.value();
        // ACTIVE
        if (isActiveUsgMode(frame.UsgModSts)) {
            sDAISetting_.set(static_cast<DAISettingType>(settings_value));
        }
    }
}
