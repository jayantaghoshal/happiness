/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <message_api.h>
#include <reuse.h>
#include <vsm_inject.h>

#include <avmp.h>
#include <vipcomm/VipFramework.h>

#include <cutils/log.h>
#include <array>
#include <chrono>
#include <thread>
#include "vsm.h"
#undef LOG_TAG
#define LOG_TAG "VSD"
using namespace std::chrono_literals;

// variables used to monitor avmp communication status

std::atomic<std::chrono::steady_clock::time_point> lastAvmpSignalReceived;
std::atomic<std::chrono::steady_clock::time_point> lastAvmpHeartbeat;
std::atomic<VersionHandshakeStatus> avmpVersionCheckOk{VersionHandshakeStatus::NotReceived};

void vsm_inject_init(void) {
    ALOGI("Initializing VSM inject handler");

    // Create instances used to send (inject) data elements received from the VIP to application code
    vipcomm::initializeInject();
}
void sendAvmpVersionHandshake() {
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
    ALOGD("Sending AVMP version: %d", avmpVersionHandshake[2]);
    ALOGD("Sending COM CRC, MSB: %d", avmpVersionHandshake[4]);
    ALOGD("Sending COM CRC, LSB: %d", avmpVersionHandshake[3]);
    ALOGD("Sending SWC CRC, MSB: %d", avmpVersionHandshake[6]);
    ALOGD("Sending SWC CRC, LSB: %d", avmpVersionHandshake[5]);
    ALOGD("Sending RTE TYPE CRC, MSB: %d", avmpVersionHandshake[8]);
    ALOGD("Sending RTE TYPE CRC, LSB: %d", avmpVersionHandshake[7]);
    ALOGD("Sending COM CFG CRC, MSB: %d", avmpVersionHandshake[10]);
    ALOGD("Sending COM CFG CRC, LSB: %d", avmpVersionHandshake[9]);

    messageSend(&message);
}

void vsm_inject_start(void) {
    sendAvmpVersionHandshake();

    // Start error monitoring thread
    std::thread monitorThread([]() {
        lastAvmpHeartbeat = std::chrono::steady_clock::now();
        lastAvmpSignalReceived = std::chrono::steady_clock::now();

        bool avmpSignalErrorLogged = false;
        bool avmpHeartbeatErrorLogged = false;
        size_t versionRetryIndex = 0;
        const std::array<std::chrono::steady_clock::duration, 8> versionRetryTimes{
                {200ms, 500ms, 1s, 1s, 1s, 3s, 3s, 10s}};

        const auto avmpHeartBeatTimeout = 4s;
        const auto avmpSignalTimeout = 4s;
        std::chrono::steady_clock::time_point nextRetryVersionHandShake =
                std::chrono::steady_clock::now() + versionRetryTimes[0];

        ALOGI("AVMPMonitor thread started");
        while (true) {
            std::this_thread::sleep_for(100ms);  // TODO: Replace with dispatcher

            const auto now = std::chrono::steady_clock::now();
            const bool avmpHeartbeatReceived = (lastAvmpHeartbeat.load() > (now - avmpHeartBeatTimeout));
            const bool avmpSignalReceived = (lastAvmpSignalReceived.load() > (now - avmpSignalTimeout));

            {
                // Monitor received heartbeats
                if (!avmpHeartbeatReceived && !avmpHeartbeatErrorLogged) {
                    avmpHeartbeatErrorLogged = true;
                    ALOGE("AVMPMonitor: heartbeats stopped coming in from the VIP");
                } else if (avmpHeartbeatReceived && avmpHeartbeatErrorLogged) {
                    avmpHeartbeatErrorLogged = false;
                    ALOGE("AVMPMonitor: heartbeats started to come in from the VIP");
                }
            }
            {
                // Monitor received signals
                if (!avmpSignalReceived && !avmpSignalErrorLogged) {
                    avmpSignalErrorLogged = true;
                    ALOGI("AVMPMonitor: signals stopped coming in from the VIP");
                } else if (avmpSignalReceived && avmpSignalErrorLogged) {
                    avmpSignalErrorLogged = false;
                    ALOGI("AVMPMonitor: signals started to come in from the VIP");
                }
            }
            if (avmpVersionCheckOk == VersionHandshakeStatus::NotReceived && now > nextRetryVersionHandShake) {
                // WORKAROUND: This retry should actually be done by DESIP layer but some times
                // VIP acks and still drops the message  so we have to do it here in higher layers also.
                // [PSS370-3953]

                if (versionRetryIndex == 0) {
                    ALOGE("DESIP AVMP Version handshake was not received from VIP. Sending signals will not work. "
                          "Retrying forever");
                }
                sendAvmpVersionHandshake();
                nextRetryVersionHandShake = now + versionRetryTimes[versionRetryIndex];
                versionRetryIndex = std::min(versionRetryTimes.size() - 1, versionRetryIndex + 1);
            }
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

    lastAvmpSignalReceived = std::chrono::steady_clock::now();

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
