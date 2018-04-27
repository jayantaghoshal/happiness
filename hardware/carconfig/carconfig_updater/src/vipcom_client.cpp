/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vipcom_client.h"
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <unistd.h>
#include <boost/crc.hpp>
#include <cstring>
#include <iomanip>

#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "carconfig_updater"
constexpr uint8_t HISIP_APPLICATION_ID_CARCONFIG = 0x77u;

#define CATALOG_VERSION 1
#define CATALOG_REVISION 0
#define DATA_REPORT_MAX_BLOCK_SIZE 50

CarConfigVipCom::CarConfigVipCom() {
    // Read the entire carconfig file into the array before
    // there is a posibility that we might get a new one.
    for (int i = 1; i <= Carconfig_base::cc_no_of_parameters; i++) {
        ccList[i - 1] = rd.getValue(i);
    }
    vipReader = std::thread(&CarConfigVipCom::VipReader, this);
    versionRequest();
}

CarConfigVipCom::~CarConfigVipCom() {
    // Make sure we can exit thread functions and join threads
    android::IPCThreadState::self()->stopProcess();
    if (vipReader.joinable()) {
        vipReader.join();
    }
}

bool CarConfigVipCom::sendHISIPMsg(HisipMessage& msg) {
    bool sendOK = true;
    msg.setAid(HISIP_APPLICATION_ID_CARCONFIG);
    HisipClient::sendToVip(msg);
    return sendOK;
}

int32_t CarConfigVipCom::calculateChecksum(char* data, int32_t calcLength) {
    boost::crc_32_type crc32;
    crc32.process_bytes(data, calcLength);
    return crc32.checksum();
}

void CarConfigVipCom::versionReport(void) {
    HisipMessage msg;
    int8_t* data = msg.allocDataPtr(2);

    data[0] = CATALOG_VERSION;
    data[1] = CATALOG_REVISION;

    msg.setFid(hisipBytes::carConfigVersionReport);

    ALOGI("Sending version report (%i.%i)", data[0], data[1]);
    sendHISIPMsg(msg);
}

void CarConfigVipCom::versionRequest(void) {
    HisipMessage msg;

    msg.setFid(hisipBytes::carConfigVersionRequest);

    ALOGI("Sending version request");
    sendHISIPMsg(msg);
}

void CarConfigVipCom::setTransfer(void) {
    HisipMessage msg;

    msg.setFid(hisipBytes::carConfigTransferCmd);

    ALOGI("Sending transfer command");
    sendHISIPMsg(msg);
}

void CarConfigVipCom::checksumCmd(const int8_t payload[35]) {
    HisipMessage msg;
    int8_t* data = msg.allocDataPtr(1);
    int16_t checkSumRangeSize = 0;
    int32_t vipChecksum = 0;
    bool checkSumMatch = false;
    bool sendOK = false;

    msg.setFid(hisipBytes::carConfigChecksumReport);

    // Copy and flip endianness
    checkSumRangeSize = (payload[0] << 8) | (payload[1] << 0 & 0xFF);
    vipChecksum =
            (payload[2] << 24) | (payload[3] << 16 & 0xFF0000) | (payload[4] << 8 & 0xFF00) | (payload[5] << 0 & 0xFF);

    // This shoudn't happen, but if the VIP asks us to calculate
    // on carconfig parameters than exists, we shouldn't try. Send
    // carConfigChecksumReportNok will result in copying new
    // parameters and new checksum size.
    if (checkSumRangeSize > Carconfig_base::cc_no_of_parameters) {
        ALOGE("Checksum calculation range large than carconfig");
        data[0] = carConfigChecksumReportNok;
    }
    // If we haven't received carconfig parameters from the CEM then we respond with
    // checksum ok so that we don't overwrite the default caconfig file on the VIP with
    // our own default file.
    else if ((calculateChecksum(ccList.data(), checkSumRangeSize) == vipChecksum) || (rd.usingDefaultFile())) {
        data[0] = carConfigChecksumReportOk;
        checkSumMatch = true;
    } else {
        ALOGI("VIP <-> MP Checksum mismatch, notifying VIP and requsting CC transfer");
        data[0] = carConfigChecksumReportNok;
    }

    sendOK = sendHISIPMsg(msg);
    if (sendOK && (checkSumMatch == false)) {
        setTransfer();
    }
}

void CarConfigVipCom::dataRequest(const int8_t payload[35]) {
    HisipMessage msg;
    int8_t block = payload[0];
    int8_t nextBlock = block + 1;
    int8_t blockSize = DATA_REPORT_MAX_BLOCK_SIZE;

    msg.setFid(hisipBytes::carConfigDataReport);

    // The VIP read request starts inside the carconfig range but ends outside
    // left to read.
    if ((nextBlock * DATA_REPORT_MAX_BLOCK_SIZE) > Carconfig_base::cc_no_of_parameters) {
        blockSize = (nextBlock * blockSize) - Carconfig_base::cc_no_of_parameters;
    }
    // The VIP is trying to start a read outside of the carconfig range,
    // according to the protocol then we should just return with block size
    // zero to indicate that we've sent the entire carconfig list.
    else if ((block * DATA_REPORT_MAX_BLOCK_SIZE) == Carconfig_base::cc_no_of_parameters) {
        blockSize = 0;
    }

    int8_t* data = msg.allocDataPtr(blockSize + 2);
    data[0] = block;
    data[1] = blockSize;
    std::memcpy(&data[2], ccList.data() + (block * blockSize), blockSize);
    sendHISIPMsg(msg);
}

void CarConfigVipCom::VipReader() {
    android::ProcessState::self()->startThreadPool();
    HisipClient::connectToHisipService<CC_HISIP_Listener, void>(this);
    android::IPCThreadState::self()->joinThreadPool();
}

void CarConfigVipCom::setRxMsgID(HisipMessage* msg) {
    msg->setAid(HISIP_APPLICATION_ID_CARCONFIG);
}

std::vector<uint8_t> CarConfigVipCom::getApplicationId() {
    std::vector<uint8_t> applicationId = {static_cast<uint8_t>(HISIP_APPLICATION_ID_CARCONFIG)};
    return applicationId;
}

void CarConfigVipCom::onMessage(const uint8_t& _fid, const int8_t _payload[35]) {
    if (_fid == hisipBytes::carConfigChecksumCmd) {
        ALOGI("Received Checksum report");
        checksumCmd(_payload);
    }

    else if (_fid == hisipBytes::carConfigDataRequest) {
        dataRequest(_payload);
    }

    else if (_fid == hisipBytes::carConfigTransferReport) {
        ALOGI("Received Data transfer report with status %s",
              (uint8_t(_payload[1]) == hisipBytes::carConfigTransferReportOk ? "Ok" : "Not ok"));
    }

    else if (_fid == hisipBytes::carConfigVersionReport) {
        ALOGI("Received version report (%i.%i)", _payload[0], _payload[1]);
        if ((_payload[0] != CATALOG_VERSION) || (_payload[1] != CATALOG_REVISION)) {
            ALOGE("HISIP catalog version mismatch");
        }
    }

    else if (_fid == hisipBytes::carConfigVersionRequest) {
        ALOGI("Received Version request");
        versionReport();
    }

    else {
        ALOGE("Received a message with unexpected FID: %x", _fid);
    }
}

CC_HISIP_Listener::CC_HISIP_Listener(void* hisip_client) {
    ALOGV("[%s] VIP listener created", __func__);
    carConfigVipCom = static_cast<CarConfigVipCom*>(hisip_client);
}

bool CC_HISIP_Listener::onMessageFromVip(const HisipMessage& msg) {
    vip_msg m;
    m.fid = msg.getFid();
    memcpy(static_cast<void*>(m.data), static_cast<void*>(msg.getDataPtr()), msg.getDataSize());
    carConfigVipCom->onMessage(m.fid, m.data);
    return true;
}

std::string CC_HISIP_Listener::getUserId() {
    return "Carconfig-updater";
}