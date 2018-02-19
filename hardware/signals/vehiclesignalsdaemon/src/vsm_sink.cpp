/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <ECD_dataelement.h>
#include <message_api.h>
#include <reuse.h>
#include <vsm_sink.h>
#include <vector>

#include <avmp.h>
#include <vipcomm/VipFramework.h>
#include "vfc_handler.h"
#include "vsm.h"
#include "vsm_inject.h"

#define LOG_TAG "VSD"

// This is the namespace for ARInjector and ARSink (found in ECD_dataelement.h) used for VFC
using namespace ECDDataElement;

// This dataelement is special since it is internal to the IHU (sent to the VIP but not Flexray)
static DESink<ActivateVfc_info>* ActivateVfc_sink;

static void validateAvmpVersionAndSinkMessage(Message_Send_T* message);
static void sendAvmpMessageToVip(const vipcomm::SignalID header, const void* buffer, const std::size_t buffSize);
static void initInternalDataelements(void);

static void validateAvmpVersionAndSinkMessage(Message_Send_T* message) {
    bool HACK = true;  // TODO(PSS370-17469): Remove hack once VIP has fixed checksum generation
    if (HACK || (avmpVersionCheckOk == VersionHandshakeStatus::Ok)) {
        messageSend(message);
    } else {
        // Actually it's not possible to reach this line as vipcomm::initializeSink() where sinks are initialized
        // is only called on successful version handshake.
        ALOGE("Mismatch in AVMP version/file CRC or check not yet made. Discarding sink message.");
    }
}

static void sendAvmpMessageToVip(const vipcomm::SignalID header, const void* buffer, const std::size_t buffSize) {
    Message_Send_T message;
    uint8_t* messageBuffer = static_cast<uint8_t*>(malloc(buffSize + avmp::avmpHeaderSize));

    if (nullptr != messageBuffer) {
        messageBuffer[0] = static_cast<uint8_t>(header & 0x00FF);
        messageBuffer[1] = static_cast<uint8_t>((header >> 8) & 0x00FF);
        memcpy(messageBuffer + avmp::avmpHeaderSize, buffer, buffSize);

        message.data_ptr = messageBuffer;
        message.data_size = buffSize + avmp::avmpHeaderSize;

        validateAvmpVersionAndSinkMessage(&message);  // change to avmp specific
        free(messageBuffer);
    } else {
        ALOGE("Unable to allocate memory for sink message.");
    }
}

static void initInternalDataelements(void) {
    static std::vector<uint8_t> data;  // This will be left empty for ActivateVfc

    ActivateVfc_sink = new DESink<ActivateVfc_info>;
    ActivateVfc_sink->subscribe([]() {
        const ActivateVfc value = ActivateVfc_sink->get().value();
        if (shallSendVfcToVIP(value))  // See if we shall send this request further
        {
            const uint8_t vfcId = static_cast<uint8_t>(value.vfcToActivate);
            const uint8_t seconds = value.secondsToKeepActive;
            ALOGD("ActivateVfc: %d-%d", vfcId, seconds);

            // Send via AVMP.
            const uint8_t vfcbuffer[2] = {vfcId, seconds};
            sendAvmpMessageToVip(avmp::controlMsgMask | avmp::activateVfcMsgId, vfcbuffer,
                                 avmp::activateVfcPayloadSize);
        }
    });
}

// Register callback lambda functions that will trigger when data is written
// to the dataelement that should be sent to VIP and Flexray bus.
void vsm_sink_init(void) {
    ALOGI("Initializing VSM sink handler.");

    // Initialize dataelements internal to the IHU (not sent on Flexray bus)
    initInternalDataelements();

    vipcomm::initializeSink(sendAvmpMessageToVip);
}
