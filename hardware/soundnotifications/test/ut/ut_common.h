/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <inttypes.h>
#include <iostream>

#include <cutils/log.h>

#include <soundwrapper.h>
#include "audiomanagermock.h"

// DEInjector
#include <ECD_dataelement.h>

using namespace SoundNotifications;
using namespace ECDDataElement;
using namespace autosar;
using namespace testing;

class ut_common : public ::testing::Test {
  public:
    ut_common() = default;
    ~ut_common() override = default;
    ::android::hardware::Return<void> mockPlaySound(const ::android::hardware::hidl_string& soundType,
                                                    int32_t soundComp,
                                                    const AudioManagerMock::playSound_cb& _hidl_cb) {
        ALOG(LOG_INFO, "SoundNotificationsUT", "AudioManagerMock::mockPlaySound: %s %i", soundType.c_str(), soundComp);
        connectionID++;
        bool error = false;

        try {
            AudioTable::getSourceName(AudioTable::getSoundType(soundType.c_str()),
                                      static_cast<AudioTable::SoundComponent>(soundComp));
        } catch (std::invalid_argument& iaex) {
            ALOG(LOG_WARN,
                 "SoundNotificationsUT",
                 "AudioManagerMock::mockPlaySound. Invalid combination of Type and Component");
            error = true;
        }

        if (!error) {
            _hidl_cb(AMStatus::OK, connectionID);
            swrapper->onRampedIn(connectionID);
        } else {
            _hidl_cb(AMStatus::VALUE_OUT_OF_RANGE, 0);
        }
        return android::hardware::Status::fromStatusT(android::OK);
    }

    ::android::hardware::Return<AMStatus> mockStopSound(uint32_t connectionId) {
        ALOG(LOG_INFO, "SoundNotificationsUT", "AudioManagerMock::mockStopSound. connection ID: %d", connectionId);
        swrapper->onDisconnected(connectionID);
        return AMStatus::OK;
    }

    static void SetUpTestCase() {
        swrapper = SoundWrapper::instance();
        am_service = ::android::sp<AudioManagerMock>(new AudioManagerMock);
        swrapper->init(am_service);
    }

    void SetUp() override {
        SoundWrapper::clearAll();
        DataElementFramework::instance().reset();
        ON_CALL(*am_service, playSound(_, _, _)).WillByDefault(Invoke(this, &ut_common::mockPlaySound));
        ON_CALL(*am_service, stopSound(_)).WillByDefault(Invoke(this, &ut_common::mockStopSound));
    }

    void TearDown() override {}

    static SoundWrapper* swrapper;
    static ::android::sp<AudioManagerMock> am_service;
    uint32_t connectionID{0};
};
