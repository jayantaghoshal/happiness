/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "defroster_rear_ccsm.h"
#include "mock_defroster.h"

#include <ECD_dataelement.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace autosar;
using namespace ECDDataElement;
using namespace testing;

class DefrosterRearCCSMTest : public Test {
  public:
    DefrosterRearCCSMTest()
        : sut_{}, rearDefroster_{FirstRowGen::ElectricDefrosterRearState::DISABLED}, electricDefrosterRearLogic_{} {
        rearDefrosterButtonReq_.inject(autosar::PsdNotPsd::NotPsd);
    }

    void SetUp() override { createSut(); }

  protected:
    void createSut() {
        sut_ = std::make_unique<DefrosterRearCCSM>(
                rearDefroster_, electricDefrosterRearLogic_, electricDefrosterRearPopupLogic_);
    }

    static ECDDataElement::DEInjector<autosar::Btn5ForUsrSwtPanFrntReq_info> rearDefrosterButtonReq_;
    static ECDDataElement::DESink<autosar::LiForBtn5ForUsrSwtPanFrntCmd_info> rearDefrosterButtonLedReq_;

    std::unique_ptr<DefrosterRearCCSM> sut_;

    NotifiableProperty<FirstRowGen::ElectricDefrosterRearState> rearDefroster_;
    NiceMock<MockDefroster> electricDefrosterRearLogic_;
    NiceMock<MockDefroster> electricDefrosterRearPopupLogic_;
};

ECDDataElement::DEInjector<autosar::Btn5ForUsrSwtPanFrntReq_info> DefrosterRearCCSMTest::rearDefrosterButtonReq_;
ECDDataElement::DESink<autosar::LiForBtn5ForUsrSwtPanFrntCmd_info> DefrosterRearCCSMTest::rearDefrosterButtonLedReq_;

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

TEST_F(DefrosterRearCCSMTest, Constructor_WillInitialize) {
    EXPECT_EQ(autosar::OnOff1::Off, rearDefrosterButtonLedReq_.get().value());
}

TEST_F(DefrosterRearCCSMTest, SetButtonLed_WhenRearDefrosterIsOn_WillSetOn) {
    rearDefroster_.set(FirstRowGen::ElectricDefrosterRearState::ON);

    EXPECT_EQ(autosar::OnOff1::On, rearDefrosterButtonLedReq_.get().value());
}

TEST_F(DefrosterRearCCSMTest, SetButtonLed_WhenRearDefrosterIsNotOn_WillSetOff) {
    rearDefroster_.set(FirstRowGen::ElectricDefrosterRearState::ON);
    rearDefroster_.set(FirstRowGen::ElectricDefrosterRearState::DISABLED);

    EXPECT_EQ(autosar::OnOff1::Off, rearDefrosterButtonLedReq_.get().value());

    rearDefroster_.set(FirstRowGen::ElectricDefrosterRearState::ON);
    rearDefroster_.set(FirstRowGen::ElectricDefrosterRearState::OFF);

    EXPECT_EQ(autosar::OnOff1::Off, rearDefrosterButtonLedReq_.get().value());
}

TEST_F(DefrosterRearCCSMTest, SetRearDefroster_WhenButtonIsPushedAndDefrosterIsTurnedOn_WillRequestOff) {
    rearDefroster_.set(FirstRowGen::ElectricDefrosterRearState::ON);

    EXPECT_CALL(electricDefrosterRearLogic_, request({OnOff::Off}));
    rearDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
}

TEST_F(DefrosterRearCCSMTest, SetRearDefroster_WhenButtonIsPushedAndDefrosterIsNotTurnedOn_WillRequestOn) {
    rearDefroster_.set(FirstRowGen::ElectricDefrosterRearState::OFF);

    EXPECT_CALL(electricDefrosterRearLogic_, request({OnOff::On}));
    rearDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
}

TEST_F(DefrosterRearCCSMTest, SetRearDefroster_WhenButtonIsNotPushed_WillNotRequest) {
    EXPECT_CALL(electricDefrosterRearLogic_, request(_)).Times(0);
    rearDefrosterButtonReq_.inject(autosar::PsdNotPsd::NotPsd);
}
