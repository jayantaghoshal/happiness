/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <IDispatcher.h>
#include <android/keycodes.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <carconfig.h>
#include <cc_parameterlist.h>
#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>
#include <iomanip>
#include <sstream>

#include "keymanagermodule.h"

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "KeyManagerModule"

// Convenience function for getting number of elements in an array
#define Num_Elems(array) (sizeof(array) / sizeof(array[0]))

/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/
// clang-format off

typedef enum
{
    HID_CMD_INPUT_DEVICE,
    HID_CMD_KEY_ID,
    HID_CMD_KEY_VALUE,
    HID_CMD_TIME_STAMP_H,
    HID_CMD_TIME_STAMP_L,
    HID_CMD_FRAME_SIZE
} hidCmd_T;


typedef enum
{
    INPUT_KEY  = 0x30,
    INPUT_KNOB = 0x48
} inputDevice_T;

typedef enum
{
    VIP_KEY_NO_KEY                                = 0x00,
    VIP_KEY_VOLUME_KNOB                           = 0x01,
    VIP_KEY_HOME_BTN                              = 0x01,
    VIP_KEY_LIN19_DMSM_KNOB                       = 0x02,
    VIP_KEY_LIN19_DEFROST_FRONT                   = 0x05,
    VIP_KEY_LIN19_NEXT                            = 0x09,
    VIP_KEY_LIN19_PLAY_PAUSE                      = 0x07,
    VIP_KEY_LIN19_DEFROST_REAR                    = 0x08,
    VIP_KEY_LIN19_PREVIOUS                        = 0x06,
    VIP_KEY_LIN19_CCSM_BTN6                       = 0x0A,
    VIP_KEY_LIN19_DMSM_ACCEPT                     = 0x0B,
    VIP_KEY_LIN19_CLIMA_HILO_SEC_ROW_LEFT         = 0x0C,
    VIP_KEY_LIN19_TEMPR_SETPOINT_ROW_SEC_RIGHT    = 0x0D,
    VIP_KEY_LIN19_CLIMA_HILO_SEC_ROW_RIGHT        = 0x0E,
    VIP_KEY_LIN19_UPDTEMPR_SETPOINT_ROW_SEC_RIGHT = 0x0F,
    VIP_KEY_LIN19_UPDTEMPR_SETPOINT_ROW_SEC_LEFT  = 0x10,
    VIP_KEY_LIN19_UPDSEAT_HEAT_LVL_ROW_SEC_RIGHT  = 0x11,
    VIP_KEY_LIN19_UPDSEAT_HEAT_LVL_ROW_SEC_LEFT   = 0x12,
    VIP_KEY_LIN19_UPDFAN_LVL_ROW_SEC              = 0x13,
    VIP_KEY_LIN19_SEAT_HEAT_LVL_ROW_SEC_LEFT      = 0x14,
    VIP_KEY_LIN19_TEMPR_SETPOINT_ROW_SEC_LEFT     = 0x15,
    VIP_KEY_LIN19_SEAT_HEAT_LVL_ROW_SEC_RIGHT     = 0x16,
    VIP_KEY_LIN19_SEAT_HEAT_LVL_LEFT              = 0x17,
    VIP_KEY_LIN19_SEAT_HEAT_LVL_RIGHT             = 0x18,
    VIP_KEY_LIN19_UPDSEAT_VENT_LVL_ROW_SEC_LEFT   = 0x19,
    VIP_KEY_LIN19_SEAT_VENT_LVL_ROW_SEC_LEFT      = 0x1A,
    VIP_KEY_LIN19_SEAT_VENT_LVL_ROW_SEC_RIGHT     = 0x1B,
    VIP_KEY_LIN19_UPDSEAT_VENT_LVL_ROW_SEC_RIGHT  = 0x1C,
    VIP_KEY_LIN19_FAN_LVL_ROW_SEC                 = 0x1D,
    VIP_KEY_FLRAY_SWC_LEFT_RIGHT                  = 0x50,
    VIP_KEY_FLRAY_SWC_SET                         = 0x51,
    VIP_KEY_FLRAY_SWC_MODE                        = 0x52,
    VIP_KEY_FLRAY_SWC_PUSH_TO_TALK                = 0x53,
    VIP_KEY_FLRAY_SWC_VOLUME                      = 0x54,
    VIP_KEY_FLRAY_SEAT_PASSENGER_AB               = 0x55,
    VIP_KEY_FLRAY_SEAT_PASSENGER_HORIZ            = 0x56,
    VIP_KEY_FLRAY_SEAT_PASSENGER_VERTI            = 0x57,
    VIP_KEY_FLRAY_SEAT_DRIVER_AB                  = 0x58,
    VIP_KEY_FLRAY_SEAT_DRIVER_HORIZ               = 0x59,
    VIP_KEY_FLRAY_SEAT_DRIVER_VERTI               = 0x60,
} keyID_T;

typedef enum
{
    KEY_RELEASE,
    KEY_PRESS,
    KEY_PRESS_ALTERNATE
} keyValue_T;

typedef struct
{
   keyID_T       keyID;
   uint16_t      pressKeyCode;
   uint16_t      altPressKeyCode;
} keyMapTable_T;

const keyMapTable_T keyMapTable[] =
{
   /* keyID                              pressKeyCode                   altPressKeyCode */
   { VIP_KEY_LIN19_DEFROST_FRONT,        AKEYCODE_4,                    AKEYCODE_UNKNOWN },
   { VIP_KEY_LIN19_NEXT,                 AKEYCODE_MEDIA_NEXT,           AKEYCODE_UNKNOWN },
   { VIP_KEY_LIN19_PLAY_PAUSE,           AKEYCODE_MEDIA_PLAY_PAUSE,     AKEYCODE_UNKNOWN },
   { VIP_KEY_LIN19_DEFROST_REAR,         AKEYCODE_7,                    AKEYCODE_UNKNOWN },
   { VIP_KEY_LIN19_PREVIOUS,             AKEYCODE_MEDIA_PREVIOUS,       AKEYCODE_UNKNOWN },
   //Steering wheel controls
   { VIP_KEY_FLRAY_SWC_LEFT_RIGHT,       AKEYCODE_MEDIA_PREVIOUS,       AKEYCODE_MEDIA_NEXT },
   { VIP_KEY_FLRAY_SWC_SET,              AKEYCODE_UNKNOWN,              AKEYCODE_UNKNOWN },
   { VIP_KEY_FLRAY_SWC_MODE,             AKEYCODE_BUTTON_MODE,          AKEYCODE_UNKNOWN },
   { VIP_KEY_FLRAY_SWC_PUSH_TO_TALK,     AKEYCODE_VOICE_ASSIST,         AKEYCODE_UNKNOWN },
   { VIP_KEY_FLRAY_SWC_VOLUME,           AKEYCODE_VOLUME_UP,            AKEYCODE_VOLUME_DOWN }
};
// clang-format on

namespace {
// Following data is from the HISIP Message Catalogue
const auto AID_keyboard = 0x10;

const auto FID_hisip_version_request = 0x00;
const auto FID_hisip_version_response = 0x01;

// const auto FID_vip_mp_hid_command_request = 0x14;
const auto FID_mp_vip_hid_command_report = 0x15;

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

}  // namespace

KeyManagerModule::AutonomousDriveHandler::AutonomousDriveHandler() {
    aut_drive_status_receiver_.subscribe([this]() {
        const auto signal_value = aut_drive_status_receiver_.get();
        if (!signal_value.isOk()) {
            return;
        }
        // TODO real enum instead of door
        if (signal_value.value() == autosar::DoorSts2::Clsd) {
            autonomous_drive_status_ = AutonomousDriveStatus::kManual;
        } else {
            autonomous_drive_status_ = AutonomousDriveStatus::kAuto;
        }
    });

    veh_oper_mod_receiver_.subscribe([this]() {
        const auto signal_value = veh_oper_mod_receiver_.get();
        if (!signal_value.isOk()) {
            return;
        }
        // TODO real enum instead of door
        if (signal_value.value() == autosar::DoorSts2::Clsd) {
            vehicle_operator_mode_ = VehicleOperatorMode::kVO;
        } else {
            vehicle_operator_mode_ = VehicleOperatorMode::kNoVO;
        }
    });
    autonomous_drive_ =
            carconfig::checkValue(CarConfigParams::CC100_AutonomousDriveType::Autonomous_Vehicle_Preparation);
}

bool KeyManagerModule::AutonomousDriveHandler::shouldIgnoreKey(const vip_msg& msg) {
    // If not autonomous drive return immediately
    if (!autonomous_drive_) {
        return false;
    }

    auto input_device = msg.data[HID_CMD_INPUT_DEVICE];
    auto key = msg.data[HID_CMD_KEY_ID];

    // Center stack audio controls should always be disabled
    if (INPUT_KNOB == input_device && VIP_KEY_VOLUME_KNOB == key) {
        return true;
    }
    if (INPUT_KEY == input_device &&
        (VIP_KEY_LIN19_PLAY_PAUSE == key || VIP_KEY_LIN19_NEXT == key || VIP_KEY_LIN19_PREVIOUS == key)) {
        return true;
    }

    // Defroster keys should always be disabled in automatic mode
    if (INPUT_KEY == input_device && (VIP_KEY_LIN19_DEFROST_FRONT == key || VIP_KEY_LIN19_DEFROST_REAR == key)) {
        if (AutonomousDriveStatus::kAuto == autonomous_drive_status_) {
            return true;
        }
    }

    // Drive mode should always be disabled in automatic mode
    if ((INPUT_KNOB == input_device && VIP_KEY_LIN19_DMSM_KNOB == key) ||
        (INPUT_KEY == input_device && VIP_KEY_LIN19_DMSM_ACCEPT == key)) {
        if (AutonomousDriveStatus::kAuto == autonomous_drive_status_) {
            return true;
        }
    }

    // Steering wheel right hand buttons should only be disabled in fully automatic mode
    if (INPUT_KEY == input_device &&
        (VIP_KEY_FLRAY_SWC_LEFT_RIGHT == key || VIP_KEY_FLRAY_SWC_SET == key || VIP_KEY_FLRAY_SWC_MODE == key ||
         VIP_KEY_FLRAY_SWC_PUSH_TO_TALK == key || VIP_KEY_FLRAY_SWC_VOLUME == key)) {
        if (AutonomousDriveStatus::kAuto == autonomous_drive_status_ &&
            VehicleOperatorMode::kNoVO == vehicle_operator_mode_) {
            return true;
        }
    }

    return false;
}

KeyManagerModule::KeyManagerModule(vhal20::impl::IVehicleHalImpl* vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal),
      keyboard_prop_config_{{
                                    // Property to control key events
                                    .prop = vhal20::toInt(vhal20::VehicleProperty::HW_KEY_INPUT),
                                    .access = vhal20::VehiclePropertyAccess::READ,
                                    .changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE,
                                    .areaConfigs = std::vector<vhal20::VehicleAreaConfig>(1),
                                    .configFlags = 0,
                                    .configArray = std::vector<int>(1),
                            },
                            {
                                    .prop = vhal20::toInt(vccvhal10::VehicleProperty::HOME_KEY_LONGPRESS),
                                    .access = vhal20::VehiclePropertyAccess::READ,
                                    .changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE,
                                    .areaConfigs = std::vector<vhal20::VehicleAreaConfig>(1),
                                    .configFlags = 0,
                                    .configArray = std::vector<int>(1),
                            }} {}

KeyManagerModule::~KeyManagerModule() {
    // Make sure we can exit thread functions and join threads
    HisipClient::disconnectFromHisipService();

    if (reader_thread_.joinable()) {
        reader_thread_.join();
    }
}

void KeyManagerModule::init() {
    ModuleBase::init();  // Register getProp() and setProp() callbacks.

    reader_thread_ = std::thread(&KeyManagerModule::VIPReader, this);

    // Initialize HISIP version handshake with VIP
    SendVersionRequestMsg();

    home_button_handler_.init(this);
}

void KeyManagerModule::VIPReader() {
    // Handle callback messages from DesipService
    HisipClient::connectToHisipService<VIPListener, void>(this);
    android::IPCThreadState::self()->joinThreadPool();
}

void KeyManagerModule::ProcessMessage(vip_msg& msg) {
    switch (msg.fid) {
        case FID_hisip_version_response: {
            bool same_version = (msg.data[0] == hisip_version_message[0]) && (msg.data[1] == hisip_version_message[1]);
            if (!same_version) {
                ALOGE("[%s] Power HISIP versions not matching! VIP: 0x%02x 0x%02x - MP: 0x%02x 0x%02x",
                      __FUNCTION__,
                      msg.data[0],
                      msg.data[1],
                      hisip_version_message[0],
                      hisip_version_message[1]);
            }
            break;
        }

        case FID_hisip_version_request:  // VIP Requesting Power App version from MP
        {
            SendVersionResponseMsg();
            break;
        }
        case FID_mp_vip_hid_command_report:
            ALOGD("HID_COMMANDS_RPT_FID: HID_CMD_INPUT_DEVICE = 0x%02X, HID_CMD_KEY_ID = 0x%02X, "
                  "HID_CMD_KEY_VALUE = 0x%02X, HID_CMD_TIME_STAMP_H = 0x%02X, HID_CMD_TIME_STAMP_L = 0x%02X, "
                  "HID_CMD_FRAME_SIZE = 0x%02X",
                  msg.data[HID_CMD_INPUT_DEVICE],
                  msg.data[HID_CMD_KEY_ID],
                  msg.data[HID_CMD_KEY_VALUE],
                  msg.data[HID_CMD_TIME_STAMP_H],
                  msg.data[HID_CMD_TIME_STAMP_L],
                  msg.data[HID_CMD_FRAME_SIZE]);

            if (autonomous_drive_handler_.shouldIgnoreKey(msg)) {
                ALOGD("Key input disabled, ignoring key event");
                break;
            }

            if (INPUT_KNOB == msg.data[HID_CMD_INPUT_DEVICE]) {
                processKnob(msg.data);
            } else {
                processKey(msg.data);
            }
            break;
        default: {
            ALOGW("[%s] Unhandled VIP message FID 0x%x", __func__, msg.fid);
            break;
        }
    }  // Switch End
}

void KeyManagerModule::processKey(int8_t* data) {
    uint8_t inx = 0x00;

    if (NULL != data) {
        while ((inx < Num_Elems(keyMapTable)) && (keyMapTable[inx].keyID != data[HID_CMD_KEY_ID])) {
            inx++;
        }

        if (inx < Num_Elems(keyMapTable)) {
            switch (data[HID_CMD_KEY_VALUE]) {
                case KEY_RELEASE: {
                    ALOGD("Writing key release");

                    // Check if the key release might originate from an alt key press
                    auto it = altPressKeys.find(data[HID_CMD_KEY_ID]);
                    if (it == altPressKeys.end()) {
                        // No saved alt key press found, it is probably a normal key press then
                        HandleButtonStateRequest(keyMapTable[inx].pressKeyCode, ButtonStateType::kButtonReleased);
                    } else {
                        // Saved alt key press found, release alt key instead of normal key
                        HandleButtonStateRequest(keyMapTable[inx].altPressKeyCode, ButtonStateType::kButtonReleased);
                        altPressKeys.erase(it);
                    }
                    break;
                }

                case KEY_PRESS:
                    ALOGD("Writing key press");
                    HandleButtonStateRequest(keyMapTable[inx].pressKeyCode, ButtonStateType::kButtonPressed);
                    break;

                case KEY_PRESS_ALTERNATE:
                    ALOGD("Writing key press");
                    HandleButtonStateRequest(keyMapTable[inx].altPressKeyCode, ButtonStateType::kButtonPressed);

                    // Save information about alt key being pressed
                    altPressKeys.insert(data[HID_CMD_KEY_ID]);

                    break;

                default:
                    ALOGW("Invalid key value received");
                    break;
            }
        } else {
            ALOGW("Invalid key received");
        }
    }
}

void KeyManagerModule::processKnob(int8_t* data) {
    if (VIP_KEY_VOLUME_KNOB == data[HID_CMD_KEY_ID]) {
        uint16_t key_code = data[HID_CMD_KEY_VALUE] > 0 ? AKEYCODE_VOLUME_UP : AKEYCODE_VOLUME_DOWN;
        int8_t num_commands = std::abs(data[HID_CMD_KEY_VALUE]);
        while (num_commands > 0) {
            HandleButtonStateRequest(key_code, ButtonStateType::kButtonPressed);
            HandleButtonStateRequest(key_code, ButtonStateType::kButtonReleased);
            num_commands--;
        }
    } else {
        ALOGW("Unhandled knob event received");
    }
}

uint8_t KeyManagerModule::HandleButtonStateRequest(int key_code, ButtonStateType req) {
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

void KeyManagerModule::SendVersionRequestMsg() {
    int8_t data[] = {0};
    SendToVIP(FID_hisip_version_request, data, sizeof(data));
}

void KeyManagerModule::SendVersionResponseMsg() {
    SendToVIP(FID_hisip_version_response, hisip_version_message, sizeof(hisip_version_message));
}

std::vector<uint8_t> KeyManagerModule::getApplicationId() {
    std::vector<uint8_t> applicationId = {static_cast<uint8_t>(AID_keyboard)};
    return applicationId;
}

std::vector<vhal20::VehiclePropConfig> KeyManagerModule::listProperties() {
    ALOGV("[%s] Vehicle property configs returned.", __FUNCTION__);
    return keyboard_prop_config_;
}

std::unique_ptr<vhal20::VehiclePropValue> KeyManagerModule::getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                                    vhal20::impl::Status& status) {
    ALOGD("getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
    vhal20::VehiclePropValue prop_value;
    switch (requestedPropValue.prop) {
        case vhal20::toInt(vccvhal10::VehicleProperty::HOME_KEY_LONGPRESS):
            status = vhal20::impl::Status::SUCCESS;
            prop_value = convertToLongpressPropValue(homekeystate_);
            ALOGD("get prophomekeystate_: %d", homekeystate_);
            break;
        default:
            ALOGW("Unknown getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
            status = vhal20::impl::Status::PERMISSION_ERROR;
            return nullptr;
            break;
    }
    return std::make_unique<vhal20::VehiclePropValue>(prop_value);
}

void KeyManagerModule::SendToVIP(uint8_t fid, int8_t data[], uint32_t size) {
    ALOGV("[%s] AID: 0x%02X FID: 0x%02X HISIP data: %s", __func__, AID_keyboard, fid, BytesToHex(data, size).c_str());

    HisipMessage msg;
    msg.setAid(AID_keyboard);
    msg.setFid(fid);
    msg.setDataPtr(data, size);

    HisipClient::sendToVip(msg);
}

KeyManagerModule::VIPListener::VIPListener(void* desip_client) {
    ALOGV("[%s] VIP listener created", __func__);
    key_manager_module_ = static_cast<KeyManagerModule*>(desip_client);
}

/* Messages handling from VIP */
bool KeyManagerModule::VIPListener::onMessageFromVip(const HisipMessage& msg) {
    // Forward Power Application related messages for processing
    vip_msg m;
    m.fid = msg.getFid();
    m.data_length = msg.getDataSize();
    memcpy(static_cast<void*>(m.data), static_cast<void*>(msg.getDataPtr()), msg.getDataSize());

    auto hisip_data = BytesToHex(m.data, m.data_length).c_str();
    ALOGV("[%s] AID: 0x%02X FID: 0x%02X HISIP data: %s", __func__, msg.getAid(), m.fid, hisip_data);

    key_manager_module_->ProcessMessage(m);

    return true;
}

std::string KeyManagerModule::VIPListener::getUserId() { return "KeyManagerModule"; }

vhal20::VehiclePropValue KeyManagerModule::convertToLongpressPropValue(HomeButtonState homekeystate) {
    vhal20::VehiclePropValue prop_value;
    prop_value.timestamp = android::elapsedRealtimeNano();
    prop_value.areaId = 0;
    prop_value.prop = vhal20::toInt(vccvhal10::VehicleProperty::HOME_KEY_LONGPRESS);
    prop_value.value.int32Values.resize(1);
    prop_value.value.int32Values[0] = (int)homekeystate;
    return prop_value;
}

void KeyManagerModule::HomeButtonPressed(bool pressed) {
    if (pressed) {  // Home key down
        ALOGD("Home keydown");
        homekeyjobid_ = tarmac::eventloop::IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(
                std::chrono::microseconds(3000000), [&]() {
                    HomeButtonState state = (homekeystate_ == HomeButtonState::kHomeButtonLongInactive)
                                                    ? HomeButtonState::kHomeButtonLongActive
                                                    : HomeButtonState::kHomeButtonLongInactive;
                    vhal20::VehiclePropValue prop_value = convertToLongpressPropValue(state);
                    pushProp(prop_value);
                    homelongpress_ = true;
                    ALOGD("Home LONGPRESS, %d", state);
                });
    } else {                                                                                          // Home key up
        ALOGD("start homekeyup homekeystate_:%d,homelongpress_: %d", homekeystate_, homelongpress_);  // Home key up
        tarmac::eventloop::IDispatcher::GetDefaultDispatcher().Cancel(homekeyjobid_);
        if (homekeystate_ == HomeButtonState::kHomeButtonLongInactive && !homelongpress_) {
            HandleButtonStateRequest(0x03, ButtonStateType::kButtonPressed);
            HandleButtonStateRequest(0x03, ButtonStateType::kButtonReleased);
        } else if (homelongpress_) {
            // update to new homekeystate when actually release the key
            homekeystate_ = (homekeystate_ == HomeButtonState::kHomeButtonLongInactive)
                                    ? HomeButtonState::kHomeButtonLongActive
                                    : HomeButtonState::kHomeButtonLongInactive;
        }
        homelongpress_ = false;  // always clear flag longpress
        ALOGD("End homekeyup homekeystate_:%d,homelongpress_: %d", homekeystate_, homelongpress_);
    }
}
