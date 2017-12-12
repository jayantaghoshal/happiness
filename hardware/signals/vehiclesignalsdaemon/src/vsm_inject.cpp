/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <message_api.h>
#include <reuse.h>
#include <vsm_inject.h>

#include <avmp.h>
#include <vipcomm/VipFramework.h>

#include <thread>

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "VSD"

// variables used to monitor avmp communication status
static bool avmpSignalErrorLogged = false;
bool avmpSignalReceived = true;
static bool avmpHeartbeatErrorLogged = false;
bool avmpHeartbeatReceived = true;

void vsm_inject_init(void) {
    ALOGI("Initializing VSM inject handler");

    // Create instances used to send (inject) data elements received from the VIP to application code
    vipcomm::initializeInject();
}

void vsm_inject_start(void) {
    uint8_t avmpVersionHandshake[avmp::avmpHeaderSize + avmp::versHandshakePayloadSize];
    Message_Send_T message;

    // Send a "version handshake" control message to the VIP to validate
    // the Autosar description and configuration files
    uint16_t comCrc, swcCrc, rteTypeCrc, comCfgCrc;
    vipcomm::getFileCrc(comCrc, swcCrc, rteTypeCrc, comCfgCrc);

    memset(&message, 0, sizeof(message));
    avmpVersionHandshake[0] = avmp::versionHandshakeMsgId;
    avmpVersionHandshake[1] = avmp::controlMsgByteMask;
    avmpVersionHandshake[2] = avmp::avmpVersion;
    avmpVersionHandshake[3] = static_cast<uint8_t>(comCrc & 0x00FF);
    avmpVersionHandshake[4] = static_cast<uint8_t>(comCrc >> 8);
    avmpVersionHandshake[5] = static_cast<uint8_t>(swcCrc & 0x00FF);
    avmpVersionHandshake[6] = static_cast<uint8_t>(swcCrc >> 8);
    avmpVersionHandshake[7] = static_cast<uint8_t>(rteTypeCrc & 0x00FF);
    avmpVersionHandshake[8] = static_cast<uint8_t>(rteTypeCrc >> 8);
    avmpVersionHandshake[9] = static_cast<uint8_t>(comCfgCrc & 0x00FF);
    avmpVersionHandshake[10] = static_cast<uint8_t>(comCfgCrc >> 8);
    message.data_ptr = avmpVersionHandshake;
    message.data_size = avmp::avmpHeaderSize + avmp::versHandshakePayloadSize;

    ALOGI("Sending AVMP version handshake");
    ALOGD("AVMP version: %d", avmpVersionHandshake[2]);
    ALOGD("COM CRC, MSB: %d", avmpVersionHandshake[4]);
    ALOGD("COM CRC, LSB: %d", avmpVersionHandshake[3]);
    ALOGD("SWC CRC, MSB: %d", avmpVersionHandshake[6]);
    ALOGD("SWC CRC, LSB: %d", avmpVersionHandshake[5]);
    ALOGD("RTE TYPE CRC, MSB: %d", avmpVersionHandshake[8]);
    ALOGD("RTE TYPE CRC, LSB: %d", avmpVersionHandshake[7]);
    ALOGD("COM CFG CRC, MSB: %d", avmpVersionHandshake[10]);
    ALOGD("COM CFG CRC, LSB: %d", avmpVersionHandshake[9]);

    messageSend(&message);

    // Start error monitoring thread
    std::thread monitorThread([]() {
        ALOGI("AVMPMonitor thread started");
        while (true) {
            // Evaluate each 4s if any heartbeats and signals have been received
            sleep(4);

            // Monitor received heartbeats
            if (!avmpHeartbeatReceived && !avmpHeartbeatErrorLogged) {  // We didn't receive any heartbeats during the
                                                                        // period and we didn't log it (yet)
                avmpHeartbeatErrorLogged = true;
                ALOGE("AVMPMonitor: heartbeats stopped coming in from the VIP");
            } else if (avmpHeartbeatReceived && avmpHeartbeatErrorLogged) {  // We did receive heartbeats during the
                                                                             // period and this was the first detection
                                                                             // of it
                avmpHeartbeatErrorLogged = false;
                ALOGE("AVMPMonitor: heartbeats started to come in from the VIP");
            }
            avmpHeartbeatReceived = false;

            // Monitor received signals
            if (!avmpSignalReceived &&
                !avmpSignalErrorLogged) {  // We didn't receive any signals during the period and we didn't log it (yet)
                avmpSignalErrorLogged = true;
                ALOGE("AVMPMonitor: signals stopped coming in from the VIP");
            } else if (avmpSignalReceived && avmpSignalErrorLogged) {  // We did receive signals during the period and
                                                                       // this was the first detection of it
                avmpSignalErrorLogged = false;
                ALOGI("AVMPMonitor: signals started to come in from the VIP");
            }
            avmpSignalReceived = false;
        }
    });
    monitorThread.detach();
}

void vsm_inject_inject(uint16_t signalId, void *buffer, bool injectError, const uint32_t length) {
    // Parameter check
    if (nullptr == buffer) {
        ALOGE("vsm_inject_inject called with bad parameter");
        return;
    }

    avmpSignalReceived = true;

    if (true == injectError) {
        if (avmp::errorPayloadSize != length) {
            ALOGE("Erroneous length received for error signal = %d, expected = %d, received = %d", signalId,
                  avmp::errorPayloadSize, length);
            return;
        }

        // The error reason is found in the first payload byte
        const int errorCode = static_cast<int>((static_cast<uint8_t *>(buffer))[0]);

        ALOGV("Error: %d received for rx signal %d", errorCode, signalId);

        vipcomm::injectIncomingErrorSignal(signalId, errorCode);
    } else {
        ALOGV("injectIncomingOkSignal");
        vipcomm::injectIncomingOkSignal(signalId, buffer, length);
    }
}
