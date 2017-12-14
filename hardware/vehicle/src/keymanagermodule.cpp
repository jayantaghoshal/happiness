#include "keymanagermodule.h"
#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "KeyManagerModule"

using namespace android::hardware::automotive::vehicle::V2_0;

#define HW_KEY_INPUT_SIZE 0x03

static constexpr int BUTTON_PRESSED = 0;
static constexpr int BUTTON_RELEASED = 1;

static constexpr int KEYCODE_VOLUME_UP = 24;
static constexpr int KEYCODE_VOLUME_DOWN = 25;
static constexpr int KEYCODE_VOICE_ASSIST = 231;

KeyManagerModule::KeyManagerModule(vhal20::impl::IVehicleHalImpl* vehicle_hal) : impl::ModuleBase(vehicle_hal) {}

void KeyManagerModule::StartSubscribe() {
    key_ptt_receiver_.subscribe([&]() {
        if (key_ptt_receiver_.get().isOk()) {
            auto v = key_ptt_receiver_.get().value();
            if (v == PsdNotPsd::NotPsd) {
                HandleButtonStateRequest(KEYCODE_VOICE_ASSIST, BUTTON_RELEASED);
            } else {
                HandleButtonStateRequest(KEYCODE_VOICE_ASSIST, BUTTON_PRESSED);
            }
        }
    });

    key_vol_up_down_receiver_.subscribe([&]() {
        if (key_vol_up_down_receiver_.get().isOk()) {
            auto btn_state = key_vol_up_down_receiver_.get().value();

            if (btn_state == last_vol_btn_state_) {
                ALOGE("Button state changed event received, but state had not changed!");
                return;
            }

            if (btn_state == BtnAudVolSts::NoBtnPsd) {
                HandleButtonStateRequest(
                        last_vol_btn_state_ == BtnAudVolSts::BtnVolUpPsd ? KEYCODE_VOLUME_UP : KEYCODE_VOLUME_DOWN,
                        BUTTON_RELEASED);
            } else {
                HandleButtonStateRequest(
                        btn_state == BtnAudVolSts::BtnVolUpPsd ? KEYCODE_VOLUME_UP : KEYCODE_VOLUME_DOWN,
                        BUTTON_PRESSED);
            }

            last_vol_btn_state_ = btn_state;
        }
    });

    roty_vol_dir_receiver_.subscribe([&]() {
        if (roty_vol_dir_receiver_.get().isOk()) {
            volume_roty_dir_ = roty_vol_dir_receiver_.get().value();
        }
    });

    roty_vol_pos_receiver_.subscribe([&]() {
        if (roty_vol_pos_receiver_.get().isOk()) {
            if ((volume_roty_dir_ == RotyDirUI1::CW) || (volume_roty_dir_ == RotyDirUI1::CCW)) {
                auto vol_steps = roty_vol_pos_receiver_.get().value();

                const int key = (volume_roty_dir_ == RotyDirUI1::CW) ? KEYCODE_VOLUME_UP : KEYCODE_VOLUME_DOWN;

                for (uint8_t i = vol_steps; i > 0; i--) {
                    HandleButtonStateRequest(key, BUTTON_PRESSED);
                    HandleButtonStateRequest(key, BUTTON_RELEASED);
                }
            }
        }
    });
}

uint8_t KeyManagerModule::HandleButtonStateRequest(int key_code, int req) {
    VehiclePropValue prop_value;

    prop_value.timestamp = android::elapsedRealtimeNano();
    prop_value.areaId = 0;

    prop_value.prop = toInt(VehicleProperty::HW_KEY_INPUT);
    prop_value.value.int32Values.resize(HW_KEY_INPUT_SIZE);
    prop_value.value.int32Values[0] = req;
    prop_value.value.int32Values[1] = key_code;
    prop_value.value.int32Values[2] = toInt(VehicleDisplay::MAIN);

    return pushProp(prop_value);
}

std::vector<VehiclePropConfig> KeyManagerModule::listProperties() {
    std::vector<VehiclePropConfig> prop_configs;
    {
        VehiclePropConfig prop_config;

        prop_config.prop = toInt(VehicleProperty::HW_KEY_INPUT);
        prop_config.access = VehiclePropertyAccess::READ;
        prop_config.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        prop_config.supportedAreas = 0;
        prop_config.areaConfigs = std::vector<VehicleAreaConfig>(1);
        prop_config.configFlags = 0;
        prop_config.configArray = std::vector<int>(1);

        prop_configs.push_back(prop_config);
    }

    return prop_configs;
}
