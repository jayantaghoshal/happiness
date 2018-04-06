/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "beltreminder.h"
#include "ut_common.h"

#undef LOG_TAG
#define LOG_TAG "BeltReminderUnitTest"

/**
Unit test for REQPROD:446629/MAIN;0 Sound request for safety belt reminder
*/

class BeltReminderTest : public ut_common {};

/**
Test Function : BeltReminder_WarnUS_SoundPlayed
Description: To test the audio sound play for the WarnUS belt reminder.
*/
TEST_F(BeltReminderTest, BeltReminder_WarnUS_SoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::BeltReminder br;

    DEInjector<autosar::BltRmnSound1_info> sendBltRmndr;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::BeltReminder),
                          static_cast<int32_t>(AudioTable::SoundComponent::WarnUS),
                          testing::_))
            .Times(1);

    autosar::BltRmnSound1_info::data_elem_type value = autosar::Trig1::Trig;
    sendBltRmndr.inject(value);
}

/**
Test Function : BeltReminder_WarnUS_SoundStopped
Description:To test the stop Belt Reminder warning as per the requirement.
*/

TEST_F(BeltReminderTest, BeltReminder_WarnUS_SoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::BeltReminder br;

    DEInjector<autosar::BltRmnSound1_info> sendBltRmndr;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::BeltReminder),
                          static_cast<int32_t>(AudioTable::SoundComponent::WarnUS),
                          testing::_))
            .Times(1);

    autosar::BltRmnSound1_info::data_elem_type value = autosar::Trig1::Trig;
    sendBltRmndr.inject(value);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);

    value = autosar::Trig1::NoTrig;
    sendBltRmndr.inject(value);
}

/**
Test Function : BeltReminder_Warn1_SoundPlayed
Description: To test the audio sound play for the WarnUS belt reminder.
*/
TEST_F(BeltReminderTest, BeltReminder_Warn1_SoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::BeltReminder br;

    DEInjector<autosar::BltRmnSound2_info> sendBltRmndr;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::BeltReminder),
                          static_cast<int32_t>(AudioTable::SoundComponent::Warn1),
                          testing::_))
            .Times(1);

    autosar::BltRmnSound2_info::data_elem_type value = autosar::Trig1::Trig;
    sendBltRmndr.inject(value);
}

/**
Test Function : BeltReminder_Warn1_SoundStopped
Description:To test the stop Belt Reminder warning as per the requirement.
*/

TEST_F(BeltReminderTest, BeltReminder_Warn1_SoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::BeltReminder br;

    DEInjector<autosar::BltRmnSound2_info> sendBltRmndr;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::BeltReminder),
                          static_cast<int32_t>(AudioTable::SoundComponent::Warn1),
                          testing::_))
            .Times(1);

    autosar::BltRmnSound2_info::data_elem_type value = autosar::Trig1::Trig;
    sendBltRmndr.inject(value);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);

    value = autosar::Trig1::NoTrig;
    sendBltRmndr.inject(value);
}

/**
Test Function : BeltReminder_WarnFinal_SoundPlayed
Description: To test the audio sound play for the WarnUS belt reminder.
*/
TEST_F(BeltReminderTest, BeltReminder_WarnFinal_SoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::BeltReminder br;

    DEInjector<autosar::BltRmnSound3_info> sendBltRmndr;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::BeltReminder),
                          static_cast<int32_t>(AudioTable::SoundComponent::WarnFinal),
                          testing::_))
            .Times(1);

    autosar::BltRmnSound2_info::data_elem_type value = autosar::Trig1::Trig;
    sendBltRmndr.inject(value);
}

/**
Test Function : BeltReminder_WarnFinal_SoundStopped
Description:To test the stop Belt Reminder warning as per the requirement.
*/

TEST_F(BeltReminderTest, BeltReminder_WarnFinal_SoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::BeltReminder br;

    DEInjector<autosar::BltRmnSound3_info> sendBltRmndr;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::BeltReminder),
                          static_cast<int32_t>(AudioTable::SoundComponent::WarnFinal),
                          testing::_))
            .Times(1);

    autosar::BltRmnSound3_info::data_elem_type value = autosar::Trig1::Trig;
    sendBltRmndr.inject(value);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);

    value = autosar::Trig1::NoTrig;
    sendBltRmndr.inject(value);
}

/**
Test Function : BeltReminder_WarnRear_SoundPlayed
Description: To test the audio sound play for the WarnUS belt reminder.
*/
TEST_F(BeltReminderTest, BeltReminder_WarnRear_SoundPlayed) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::BeltReminder br;

    DEInjector<autosar::BltRmnSound4_info> sendBltRmndr;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::BeltReminder),
                          static_cast<int32_t>(AudioTable::SoundComponent::WarnRear),
                          testing::_))
            .Times(1);

    autosar::BltRmnSound4_info::data_elem_type value = autosar::Trig1::Trig;
    sendBltRmndr.inject(value);
}

/**
Test Function : BeltReminder_WarnRear_SoundStopped
Description:To test the stop Belt Reminder warning as per the requirement.
*/

TEST_F(BeltReminderTest, BeltReminder_WarnRear_SoundStopped) {
    ALOGI("Starting %s", test_info_->name());
    SoundNotifications::BeltReminder br;

    DEInjector<autosar::BltRmnSound4_info> sendBltRmndr;
    // Setup expectations first
    EXPECT_CALL(*am_service,
                playSound(static_cast<int32_t>(AudioTable::SoundType::BeltReminder),
                          static_cast<int32_t>(AudioTable::SoundComponent::WarnRear),
                          testing::_))
            .Times(1);

    autosar::BltRmnSound4_info::data_elem_type value = autosar::Trig1::Trig;
    sendBltRmndr.inject(value);

    EXPECT_CALL(*am_service, stopSound(testing::_)).Times(1);

    value = autosar::Trig1::NoTrig;
    sendBltRmndr.inject(value);
}
