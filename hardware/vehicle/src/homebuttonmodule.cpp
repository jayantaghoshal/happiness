/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <android/keycodes.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>
#include <iomanip>
#include <sstream>

#include "homebuttonmodule.h"

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "HomeButtonModule"

// Convenience function for getting number of elements in an array
#define Num_Elems(array) (sizeof(array) / sizeof(array[0]))

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;
using namespace android;

// Following data is from the HISIP Message Catalogue
const auto AID_power_synchronization = 0x04;
const auto FID_vip_power_mode_rep = 0x05;

int8_t hisip_version_message[] = {0x01, 0x00};

/**
 * Converts a series/array of bytes to a string with their
 * hexadecimal represantation
 * @param  bytes  bytes to convert
 * @param  length the amount of bytes to convert
 * @return        stringified bytes in hexadecimal
 */
std::string BytesToHex(const int8_t bytes[], uint32_t length) {
    std::stringstream stringified_data("");
    for (uint32_t i = 0; i < length; i++) {
        stringified_data << "0x" << std::hex << std::setfill('0') << std::setw(2) << (bytes[i] & 0xFF) << " ";
    }
    return stringified_data.str();
}

HomeButtonModule::HomeButtonModule(vhal20::impl::IVehicleHalImpl* vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal),
      keyboard_prop_config_{
              // Property to control key events
              .prop = toInt(vhal20::VehicleProperty::HW_KEY_INPUT),
              .access = vhal20::VehiclePropertyAccess::READ,
              .changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE,
              .supportedAreas = 0,
              .areaConfigs = std::vector<vhal20::VehicleAreaConfig>(1),
              .configFlags = 0,
              .configArray = std::vector<int>(1),
      },
      homekeylongpress_prop_config_{
              .prop = toInt(VehicleProperty::HOME_KEY_LONGPRESS),
              .access = VehiclePropertyAccess::READ,
              .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
              .supportedAreas = toInt(VehicleAreaZone::WHOLE_CABIN),
              .areaConfigs = std::vector<vhal20::VehicleAreaConfig>(1),
              .configFlags = 0,
              .configArray = std::vector<int>(1),
      } {}

HomeButtonModule::~HomeButtonModule() {
    // Make sure we can exit thread functions and join threads
    DesipClient::setExitListen(true);

    if (reader_thread_.joinable()) {
        reader_thread_.join();
    }
}

void HomeButtonModule::init() {
    ModuleBase::init();  // Register getProp() and setProp() callbacks.

    reader_thread_ = std::thread(&HomeButtonModule::VIPReader, this);
}

void HomeButtonModule::VIPReader() {
    android::ProcessState::self()->startThreadPool();
    // Handle callback messages from DesipService
    DesipClient::listen<VIPListener, HomeButtonModule>(this);
    android::IPCThreadState::self()->joinThreadPool();
}

VehiclePropValue HomeButtonModule::convertToPropValue(HomeButtonState homekeystate) {
    VehiclePropValue prop_value;
    prop_value.timestamp = elapsedRealtimeNano();
    prop_value.areaId = 0;
    prop_value.prop = toInt(VehicleProperty::HOME_KEY_LONGPRESS);
    prop_value.value.int32Values.resize(1);
    prop_value.value.int32Values[0] = (int)homekeystate;
    return prop_value;
}
void HomeButtonModule::processMessage(vip_msg& msg) {
    switch (msg.fid) {
        case FID_vip_power_mode_rep:
            if (msg.data[6] & 0b10000000) {  // Home key down
                ALOGV("Home keydown");
                homekeyjobid_ = tarmac::eventloop::IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(
                        std::chrono::microseconds(3000000), [&]() {
                            HomeButtonState state = (homekeystate_ == HomeButtonState::kHomeButtonLongInactive)
                                                            ? HomeButtonState::kHomeButtonLongActive
                                                            : HomeButtonState::kHomeButtonLongInactive;
                            VehiclePropValue prop_value = convertToPropValue(state);
                            pushProp(prop_value);
                            homelongpress_ = true;
                            ALOGD("Home LONGPRESS, %d", state);
                        });
            } else if (msg.data[6] & 0b01000000) {
                ALOGV("start homekeyup homekeystate_:%d,homelongpress_: %d",
                      homekeystate_,
                      homelongpress_);  // Home key up
                tarmac::eventloop::IDispatcher::GetDefaultDispatcher().Cancel(homekeyjobid_);
                if (homekeystate_ == HomeButtonState::kHomeButtonLongInactive && !homelongpress_) {
                    handleButtonStateRequest(0x03, ButtonStateType::kButtonPressed);
                    handleButtonStateRequest(0x03, ButtonStateType::kButtonReleased);
                } else if (homelongpress_) {
                    // update to new homekeystate when actually release the key
                    homekeystate_ = (homekeystate_ == HomeButtonState::kHomeButtonLongInactive)
                                            ? HomeButtonState::kHomeButtonLongActive
                                            : HomeButtonState::kHomeButtonLongInactive;
                }
                homelongpress_ = false;  // always clear flag longpress
                ALOGV("End homekeyup homekeystate_:%d,homelongpress_: %d", homekeystate_, homelongpress_);
            }
            break;
        default:
            break;
    }
}

uint8_t HomeButtonModule::handleButtonStateRequest(int key_code, ButtonStateType req) {
    vhal20::VehiclePropValue prop_value;

    prop_value.timestamp = android::elapsedRealtimeNano();
    prop_value.areaId = 0;

    prop_value.prop = toInt(vhal20::VehicleProperty::HW_KEY_INPUT);
    prop_value.value.int32Values.resize(3);
    prop_value.value.int32Values[0] = (int)req;
    prop_value.value.int32Values[1] = key_code;
    prop_value.value.int32Values[2] = toInt(vhal20::VehicleDisplay::MAIN);

    return pushProp(prop_value);
}

void HomeButtonModule::setRxMsgID(ParcelableDesipMessage* msg) { msg->setAid(AID_power_synchronization); }

std::vector<vhal20::VehiclePropConfig> HomeButtonModule::listProperties() {
    ALOGV("[%s] Vehicle property configs returned.", __FUNCTION__);
    return {homekeylongpress_prop_config_, keyboard_prop_config_};
}
std::unique_ptr<VehiclePropValue> HomeButtonModule::getProp(const VehiclePropValue& requestedPropValue,
                                                            vhal20::impl::Status& /*status*/) {
    ALOGD("getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
    VehiclePropValue prop_value;
    switch (requestedPropValue.prop) {
        case toInt(VehicleProperty::HOME_KEY_LONGPRESS):
            prop_value = convertToPropValue(homekeystate_);
            ALOGD("get prophomekeystate_: %d", homekeystate_);
            break;
        default:
            ALOGW("Unknown getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
            return nullptr;
            break;
    }
    return std::make_unique<VehiclePropValue>(prop_value);
}

HomeButtonModule::VIPListener::VIPListener(HomeButtonModule* homebuttonModule) {
    ALOGV("[%s] VIP listener created", __func__);
    home_button_module_ = homebuttonModule;
}

/* Messages handling from VIP */
android::binder::Status HomeButtonModule::VIPListener::deliverMessage(const ParcelableDesipMessage& msg,
                                                                      bool* _aidl_return) {
    // Forward Power Application related messages for processing
    vip_msg m;
    m.fid = msg.getFid();
    m.data_length = msg.getDataSize();
    memcpy(static_cast<void*>(m.data), static_cast<void*>(msg.getDataPtr()), msg.getDataSize());

    auto hisip_data = BytesToHex(m.data, m.data_length).c_str();
    ALOGV("[%s] AID: 0x%02X FID: 0x%02X HISIP data: %s", __func__, msg.getAid(), m.fid, hisip_data);

    home_button_module_->processMessage(m);

    *_aidl_return = true;
    return android::binder::Status::ok();
}

String16 HomeButtonModule::VIPListener::getId() { return String16{"HomeButtonModule"}; }

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor