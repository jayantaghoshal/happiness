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

HomeButtonModule::~HomeButtonModule() {
    // Make sure we can exit thread functions and join threads
    HisipClient::disconnectFromHisipService();

    if (reader_thread_.joinable()) {
        reader_thread_.join();
    }
}

void HomeButtonModule::init(HomeButtonCallback* listener) {
    home_button_listener_ = listener;
    reader_thread_ = std::thread(&HomeButtonModule::VIPReader, this);
}

void HomeButtonModule::VIPReader() {
    // Handle callback messages from HisipService
    HisipClient::connectToHisipService<VIPListener, void>(this);
    android::IPCThreadState::self()->joinThreadPool();
}

void HomeButtonModule::processMessage(vip_msg& msg) {
    switch (msg.fid) {
        case FID_vip_power_mode_rep:
            if (msg.data[6] & 0b10000000) {  // Home key down
                if (home_button_listener_) home_button_listener_->HomeButtonPressed(true);
            } else if (msg.data[6] & 0b01000000) {
                if (home_button_listener_) home_button_listener_->HomeButtonPressed(false);
            }
            break;
        default:
            break;
    }
}

std::vector<uint8_t> HomeButtonModule::getApplicationId() {
    std::vector<uint8_t> applicationId = {static_cast<uint8_t>(AID_power_synchronization)};
    return applicationId;
}

HomeButtonModule::VIPListener::VIPListener(void* hisip_client) {
    ALOGV("[%s] VIP listener created", __func__);
    home_button_module_ = static_cast<HomeButtonModule*>(hisip_client);
}

/* Messages handling from VIP */
bool HomeButtonModule::VIPListener::onMessageFromVip(const HisipMessage& msg) {
    // Forward Power Application related messages for processing
    vip_msg m;
    m.fid = msg.getFid();
    m.data_length = msg.getDataSize();
    memcpy(static_cast<void*>(m.data), static_cast<void*>(msg.getDataPtr()), msg.getDataSize());

    auto hisip_data = BytesToHex(m.data, m.data_length).c_str();
    ALOGV("[%s] AID: 0x%02X FID: 0x%02X HISIP data: %s", __func__, msg.getAid(), m.fid, hisip_data);

    home_button_module_->processMessage(m);

    return true;
}

std::string HomeButtonModule::VIPListener::getUserId() {
    return "HomeButtonModule";
}

}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
