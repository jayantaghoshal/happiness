/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <inttypes.h>
#include <iostream>

#include <soundwrapper.h>
#include "audiomanagermock.h"
#include "parking_brake.h"

// DEInjector
#include <ECD_dataelement.h>

#undef LOG_TAG
#define LOG_TAG "ParkingBrakeUnitTest"

using namespace SoundNotifications;
using namespace ECDDataElement;
using namespace autosar;
using namespace testing;
/**
Unit test for EPB REQPROD:218654/MAIN;3	Audio request for EPB warning
*/

class ParkingBrakeTest : public ::testing::Test {
  public:
    ::android::hardware::Return<void> mockPlaySound(int32_t soundType,
                                                    int32_t soundComp,
                                                    AudioManagerMock::playSound_cb _hidl_cb) {
        ALOGI("AudioManagerMock::mockPlaySound: %i %i", soundType, soundComp);
        connectionID++;
        bool error = false;

        try {
            AudioTable::getSourceName(static_cast<AudioTable::SoundType>(soundType),
                                      static_cast<AudioTable::SoundComponent>(soundComp));
        } catch (std::invalid_argument iaex) {
            ALOGW("AudioManagerMock::mockPlaySound. Invalid combination of Type and Component");
            error = true;
        }

        if (!error) {
            _hidl_cb(AMStatus::OK, connectionID);
            swrapper->onRampedIn(static_cast<uint32_t>(connectionID));
        } else {
            _hidl_cb(AMStatus::VALUE_OUT_OF_RANGE, -1);
        }
        return android::hardware::Status::fromStatusT(android::OK);
    }

    ::android::hardware::Return<AMStatus> mockStopSound(int64_t connectionId) {
        ALOGI("AudioManagerMock::mockStopSound. connection ID: %" PRId64, connectionId);
        swrapper->onDisconnected(static_cast<uint32_t>(connectionID));
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
        ON_CALL(*am_service.get(), playSound(_, _, _)).WillByDefault(Invoke(this, &ParkingBrakeTest::mockPlaySound));
        ON_CALL(*am_service.get(), stopSound(_)).WillByDefault(Invoke(this, &ParkingBrakeTest::mockStopSound));
    }
    void TearDown() override {}
    static SoundWrapper* swrapper;
    static ::android::sp<AudioManagerMock> am_service;
    int64_t connectionID{0};
};

::android::sp<AudioManagerMock> ParkingBrakeTest::am_service = nullptr;
SoundWrapper* ParkingBrakeTest::swrapper = nullptr;

/**
Test Function : EpbLampSignal_Active_SpeedGtrThanMax_EpbSoundPlayed
Description: To test the audio sound play for the EPB warning activation as per the requirement.
Signal VehSpdLgtSafe > 10 km/h
AND
Signal EpbLampReq == "Flash3"
*/
TEST_F(ParkingBrakeTest, EpbLampSignal_Active_SpeedGtrThanMax_EpbSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::Chassis::ParkingBrake pb;

    DEInjector<autosar::EpbLampReq_info> sendEpblamp;
    EpbLampReqRec epb_lamp;
    DEInjector<autosar::VehSpdLgtSafe_info> sendEpbSpd;
    VehSpdLgtSafe1 veh_spd;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::ElectricalParkingBrakeEPB),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    epb_lamp.EpbLampReq = EpbLampReqType1::Flash3;
    sendEpblamp.inject(epb_lamp);
    // set the vehicle speed more than Max speed.
    veh_spd.VehSpdLgt = static_cast<double>(3);
    sendEpbSpd.inject(veh_spd);
}

/**
Test Function : EpbLampSignal_InActive_EpbSoundStopped
Description:To test the stop EPB warning as per the requirement.
Signal EpbLampReq != "On" AND Signal EpbLampReq != "Flash3"
OR
Audible warning is active AND Signal VehSpdLgtSafe changes value to < 5 km/h
*/
TEST_F(ParkingBrakeTest, EpbLampSignal_InActive_EpbSoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::Chassis::ParkingBrake pb;

    DEInjector<autosar::EpbLampReq_info> sendEpblamp;
    EpbLampReqRec epb_lamp;
    DEInjector<autosar::VehSpdLgtSafe_info> sendEpbSpd;
    VehSpdLgtSafe1 veh_spd;
    /*
   sending the EPB play signals to create the play state condition before stopping it.
   Expecting a call for playSound just to mitigate error "expected to be never called".
   */
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::ElectricalParkingBrakeEPB),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);
    epb_lamp.EpbLampReq = EpbLampReqType1::Flash3;
    sendEpblamp.inject(epb_lamp);
    // set the vehicle speed more than Max speed.
    veh_spd.VehSpdLgt = static_cast<double>(3);
    sendEpbSpd.inject(veh_spd);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    /*
    set epb lamp to OFF.
    */
    epb_lamp.EpbLampReq = EpbLampReqType1::Off;
    sendEpblamp.inject(epb_lamp);
}

/**
Test Function : EpbLampSignal_On_SpeedGtrThanMax_EpbSoundPlayed
Description: To test the play EPB warning activation as per the requirement.
Signal VehSpdLgtSafe > 10 km/h
AND
Signal EpbLampReq == "On"
*/
TEST_F(ParkingBrakeTest, EpbLampSignal_On_SpeedGtrThanMax_EpbSoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::Chassis::ParkingBrake pb;

    DEInjector<autosar::EpbLampReq_info> sendEpblamp;
    EpbLampReqRec epb_lamp;
    DEInjector<autosar::VehSpdLgtSafe_info> sendEpbSpd;
    VehSpdLgtSafe1 veh_spd;

    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::ElectricalParkingBrakeEPB),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);

    epb_lamp.EpbLampReq = EpbLampReqType1::On;
    sendEpblamp.inject(epb_lamp);
    // set the vehicle speed more than Max speed.
    veh_spd.VehSpdLgt = static_cast<double>(3);
    sendEpbSpd.inject(veh_spd);
}

/**
Test Function : EpbLampSignal_Active_SpeedLessThanMin_EpbSoundStopped
Description:To test the stop EPB warning as per the requirement.
Signal EpbLampReq != "On" AND Signal EpbLampReq != "Flash3"
OR
Audible warning is active AND Signal VehSpdLgtSafe changes value to < 5 km/h
*/
TEST_F(ParkingBrakeTest, EpbLampSignal_Active_SpeedLessThanMin_EpbSoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::Chassis::ParkingBrake pb;

    DEInjector<autosar::EpbLampReq_info> sendEpblamp;
    EpbLampReqRec epb_lamp;
    DEInjector<autosar::VehSpdLgtSafe_info> sendEpbSpd;
    VehSpdLgtSafe1 veh_spd;
    /*
    sending the EPB play signals to create the play state condition before stopping it.
    Expecting a call for playSound just to mitigate error "expected to be never called".
    */
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::ElectricalParkingBrakeEPB),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);
    epb_lamp.EpbLampReq = EpbLampReqType1::On;
    sendEpblamp.inject(epb_lamp);
    // set the vehicle speed more than Max speed.
    veh_spd.VehSpdLgt = static_cast<double>(3);
    sendEpbSpd.inject(veh_spd);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    /*
    set the vehicle speed less than Min speed.
    */
    veh_spd.VehSpdLgt = static_cast<double>(1.37);
    sendEpbSpd.inject(veh_spd);
}

/**
Test Function : EpbLampSignal_Active_SpeedEqlToMin_EpbSoundStopped
Description: stop EPB warning as per the requirement,
on the boundary values.
Signal EpbLampReq != "On" AND Signal EpbLampReq != "Flash3"
OR
Audible warning is active AND Signal VehSpdLgtSafe changes value to < 5 km/h
*/
TEST_F(ParkingBrakeTest, EpbLampSignal_Active_SpeedEqlToMin_EpbSoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::Chassis::ParkingBrake pb;

    DEInjector<autosar::EpbLampReq_info> sendEpblamp;
    EpbLampReqRec epb_lamp;
    DEInjector<autosar::VehSpdLgtSafe_info> sendEpbSpd;
    VehSpdLgtSafe1 veh_spd;
    /*
    sending the EPB play signals to create the play state condition before stopping it.
    Expecting a call for playSound just to mitigate error "expected to be never called".
    */
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::ElectricalParkingBrakeEPB),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);
    epb_lamp.EpbLampReq = EpbLampReqType1::On;
    sendEpblamp.inject(epb_lamp);
    // set the vehicle speed more than Max speed.
    veh_spd.VehSpdLgt = static_cast<double>(3);
    sendEpbSpd.inject(veh_spd);

    // Setup expectations first
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);
    /*
    set the vehicle speed equal to Min speed.
    */
    veh_spd.VehSpdLgt = static_cast<double>(1.38805);
    sendEpbSpd.inject(veh_spd);
}

/**
Test Function : EpbLampSignal_Active_SpeedLessThanMax_EpbNotPlayed
Description : check the negative condition of EPB sound play.
*/
TEST_F(ParkingBrakeTest, EpbLampSignal_Active_SpeedLessThanMax_EpbNotPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::Chassis::ParkingBrake pb;

    DEInjector<autosar::EpbLampReq_info> sendEpblamp;
    EpbLampReqRec epb_lamp;
    DEInjector<autosar::VehSpdLgtSafe_info> sendEpbSpd;
    VehSpdLgtSafe1 veh_spd;

    // Setup expectations first
    // Not expecting the call because conditions does not meet.
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::ElectricalParkingBrakeEPB),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(0);

    epb_lamp.EpbLampReq = EpbLampReqType1::On;
    sendEpblamp.inject(epb_lamp);
    veh_spd.VehSpdLgt = static_cast<double>(2.76);
    sendEpbSpd.inject(veh_spd);
}

/**
Test Function : EpbLampSignal_InActive_SpeedGtrThanMax_EpbNotPlayed
Description : test to check the negative condition of EPB sound play.
*/
TEST_F(ParkingBrakeTest, EpbLampSignal_InActive_SpeedGtrThanMax_EpbNotPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::Chassis::ParkingBrake pb;

    DEInjector<autosar::EpbLampReq_info> sendEpblamp;
    EpbLampReqRec epb_lamp;
    DEInjector<autosar::VehSpdLgtSafe_info> sendEpbSpd;
    VehSpdLgtSafe1 veh_spd;

    // Setup expectations first
    // Not expecting the call because conditions does not meet.
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::ElectricalParkingBrakeEPB),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(0);

    epb_lamp.EpbLampReq = EpbLampReqType1::Off;
    sendEpblamp.inject(epb_lamp);
    veh_spd.VehSpdLgt = static_cast<double>(2.78);
    sendEpbSpd.inject(veh_spd);
}

/**
Test Function : EpbLampSignal_Active_SpeedGtrThanMin_EpbNotStopped
Description : check the negative condition of EPB sound stop.
*/
TEST_F(ParkingBrakeTest, EpbLampSignal_Active_SpeedGtrThanMin_EpbNotStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::Chassis::ParkingBrake pb;

    DEInjector<autosar::EpbLampReq_info> sendEpblamp;
    EpbLampReqRec epb_lamp;
    DEInjector<autosar::VehSpdLgtSafe_info> sendEpbSpd;
    VehSpdLgtSafe1 veh_spd;

    /*
    sending the EPB play signals to create the play state condition before stopping it.
    Expecting a call for playSound just to mitigate error "expected to be never called".
    */
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::ElectricalParkingBrakeEPB),
                          static_cast<int32_t>(AudioTable::SoundComponent::NotAvailable),
                          testing::_))
            .Times(1);
    epb_lamp.EpbLampReq = EpbLampReqType1::On;
    sendEpblamp.inject(epb_lamp);
    veh_spd.VehSpdLgt = static_cast<double>(2.78);
    sendEpbSpd.inject(veh_spd);

    // Setup expectations first
    // Not expecting the call because conditions does not meet.
    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(0);

    // set the Lamp On
    epb_lamp.EpbLampReq = EpbLampReqType1::On;
    sendEpblamp.inject(epb_lamp);
    veh_spd.VehSpdLgt = static_cast<double>(1.78);
    sendEpbSpd.inject(veh_spd);
}
