/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vipcom_client.h"
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <unistd.h>
#include <iomanip>

#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "VipComClient"
constexpr uint8_t HISIP_APPLICATION_ID_CARCONFIG = 0x77u;

CarConfigVipCom::CarConfigVipCom() {
    vipReader = std::thread(&CarConfigVipCom::VipReader, this);
    versionRequest();
}

CarConfigVipCom::~CarConfigVipCom() {
    // Make sure we can exit thread functions and join threads
    DesipClient::setExitListen(true);
    android::IPCThreadState::self()->stopProcess();

    if (vipReader.joinable()) {
        vipReader.join();
    }
}

int32_t CarConfigVipCom::versionReport(void) {
    ParcelableDesipMessage msg;
    int8_t* data = msg.allocDataPtr(2);
    bool sendOK = false;

    data[0] = 0x01;  // catalog version
    data[1] = 0x07;  // catalog revision

    msg.setAid(HISIP_APPLICATION_ID_CARCONFIG);
    msg.setFid(hisipBytes::sysVersionReport);

    sendMsg(msg, &sendOK);
    return (sendOK ? 0 : -1);
}

int32_t CarConfigVipCom::versionRequest(void) {
    ParcelableDesipMessage msg;
    bool sendOK = false;

    msg.setAid(HISIP_APPLICATION_ID_CARCONFIG);
    msg.setFid(hisipBytes::sysVersionRequest);

    sendMsg(msg, &sendOK);
    return (sendOK ? 0 : -1);
}

int CarConfigVipCom::sendConfig(std::vector<int8_t>& values) {
    ParcelableDesipMessage msg;
    bool sendOK = false;

    msg.setAid(HISIP_APPLICATION_ID_CARCONFIG);
    msg.setFid(hisipBytes::sysSetCarConfigFid);
    msg.setDataPtr(&values[0], values.size());
    this->sendMsg(msg, &sendOK);
    usleep(300000);
    if (sendOK) {
        ALOGI("Carconfig values successfully sent to VIP");
        return 0;
    }
    ALOGE("Failed to send CarConfig values to VIP");
    return -1;
}

void CarConfigVipCom::VipReader() {
    android::ProcessState::self()->startThreadPool();
    DesipClient::listen<CC_Desip_Listener, CarConfigVipCom>(this);
    android::IPCThreadState::self()->joinThreadPool();
}

void CarConfigVipCom::setRxMsgID(ParcelableDesipMessage* msg) { msg->setAid(HISIP_APPLICATION_ID_CARCONFIG); }

void CarConfigVipCom::onMessage(const uint8_t& _fid, const int8_t _payload[35]) {
    // Accept messages with correct FID.
    if (_fid == hisipBytes::sysRepCarConfigFid) {
        vipAcknowledge = _payload[0];

        // Sys_Car_Config_Request not implemented. VIP will always return 0
        if (_payload[1] == 0 /*cc_175*/ && _payload[2] == 0 /*cc_181*/) {
            ALOGI("Received correct values form VIP");
        } else {
            ALOGI("FID: 0x%X, Received unexpected values from VIP with data: 0x%X, 0x%X",
                  _fid,
                  _payload[1],
                  _payload[2]);
        }

    } else if (_fid == hisipBytes::sysVersionReport) {
        ALOGI("Received version report from VIP");

    } else if (_fid == hisipBytes::sysVersionRequest) {
        ALOGI("Sending version report to VIP");
        versionReport();

    } else {
        ALOGE("Received a message with unexpected FID: %x", _fid);
    }
}

CC_Desip_Listener::CC_Desip_Listener(CarConfigVipCom* carConfigVipCom) { this->carConfigVipCom = carConfigVipCom; }

Status CC_Desip_Listener::deliverMessage(const ParcelableDesipMessage& msg, bool* _aidl_return) {
    vip_msg m;
    m.fid = msg.getFid();
    memcpy(static_cast<void*>(m.data), static_cast<void*>(msg.getDataPtr()), msg.getDataSize());
    *_aidl_return = true;
    carConfigVipCom->onMessage(m.fid, m.data);
    return android::binder::Status::ok();
}

String16 CC_Desip_Listener::getId() { return String16("Carconfig-updater"); }
