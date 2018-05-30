/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "timerMain.h"
#define LOG_TAG "Adasis"
#include <log/log.h>

timerGoogleImpl::timerGoogleImpl(adasisFlexrayConverterImpl* googleAdasisConverterhal)
    : googleAdasisConverter(googleAdasisConverterhal) {
    IDispatcher::GetDefaultDispatcher().Enqueue([this] { this->timeMessages(); });
}

void timerGoogleImpl::timeMessages() {
    if (googleAdasisConverter->getConverterStarted()) {
        if (fourEightyCounter == FOUREIGTHYTIMER) {
            ALOGI("Adasis message: Converter started sending position");
            googleAdasisConverter->sendPositionMessage();
            fourEightyCounter = RESET;
        }
        if (twoHundredTimer == TWOHUNDREDTIMER) {
            ALOGI("Adasis message: Converter started sending shortHorizonn");
            googleAdasisConverter->sendShortHorizonMessage();
            googleAdasisConverter->sendLongHorizonMessage();
            twoHundredTimer = RESET;
        }
        if (twoFourtyTimer == TWOFOURTYTIMER) {
            ALOGI("Adasis message: Converter started sending MetaData");
            googleAdasisConverter->sendHorizonMetaData();
            googleAdasisConverter->sendStubMessage();
            googleAdasisConverter->sendSegmentMessage();
            twoFourtyTimer = RESET;
        }

        fourEightyCounter++;
        twoHundredTimer++;
        twoFourtyTimer++;
        seconds++;
    }

    IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(std::chrono::milliseconds(FLEXRAYFRAME),
                                                         [this] { timeMessages(); });
}