/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <vcc/localconfig.h>
#include "defroster_windscreen_ccsm.h"

#include "carconfig_mock.h"

#include "enum_helper.h"
#include "mock_defroster.h"
#include "mock_dispatcher.h"

#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

const double lcfgDelay = 2.0;
}

using namespace autosar;
using namespace ECDDataElement;
using namespace testing;

using CC122 = CarConfigParams::CC122_HeatedWindscreenType;

class DefrosterWindscreenCCSMTest : public Test {
  public:
    DefrosterWindscreenCCSMTest(CC122 ccParam = CC122::No_Heated_Frontscreen)
        : localConfig_{},
          carConfig_{},
          dispatcher_{},
          sut_{},
          maxDefroster_{FirstRowGen::MaxDefrosterState::DISABLED},
          defrosterWindscreen_{FirstRowGen::ElectricDefrosterWindscreenState::DISABLED},
          electricDefrosterWindscreenLogic_{},
          maxDefrosterLogic_{} {
        ON_CALL(carConfig_, getValue(to_undrl(CC122::ParamNumber))).WillByDefault(Return(to_undrl(ccParam)));

        ON_CALL(localConfig_, GetDoubleMock("Climate_Defroster_delay")).WillByDefault(Return(lcfgDelay));

        frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::NotPsd);
    }

    void SetUp() override { createSut(); }

  protected:
    void createSut() {
        sut_ = std::make_unique<DefrosterWindscreenCCSM>(&localConfig_,
                                                         maxDefroster_,
                                                         defrosterWindscreen_,
                                                         dispatcher_,
                                                         electricDefrosterWindscreenLogic_,
                                                         electricDefrosterWindscreenPopupLogic_,
                                                         maxDefrosterLogic_);
    }

    static ECDDataElement::DEInjector<autosar::Btn4ForUsrSwtPanFrntReq_info> frontDefrosterButtonReq_;
    static ECDDataElement::DESink<autosar::LiForBtn4ForUsrSwtPanFrntCmd_info> frontDefrosterButtonLedReq_;

    NiceMock<vcc::mocks::MockLocalConfigReader> localConfig_;
    NiceMock<CarConfigMock> carConfig_;
    NiceMock<MockDispatcher> dispatcher_;
    std::unique_ptr<DefrosterWindscreenCCSM> sut_;

    NotifiableProperty<FirstRowGen::MaxDefrosterState> maxDefroster_;
    NotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState> defrosterWindscreen_;

    NiceMock<MockDefroster> electricDefrosterWindscreenLogic_;
    NiceMock<MockDefroster> electricDefrosterWindscreenPopupLogic_;
    NiceMock<MockDefroster> maxDefrosterLogic_;
};

ECDDataElement::DEInjector<autosar::Btn4ForUsrSwtPanFrntReq_info> DefrosterWindscreenCCSMTest::frontDefrosterButtonReq_;
ECDDataElement::DESink<autosar::LiForBtn4ForUsrSwtPanFrntCmd_info>
        DefrosterWindscreenCCSMTest::frontDefrosterButtonLedReq_;

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

TEST_F(DefrosterWindscreenCCSMTest, Constructor_WillInitialize) {
    EXPECT_EQ(autosar::OnOff1::Off, frontDefrosterButtonLedReq_.get().value());
}

class DefrosterWindscreenCCSMBrokenCarConfigTest : public DefrosterWindscreenCCSMTest {
  public:
    DefrosterWindscreenCCSMBrokenCarConfigTest() : DefrosterWindscreenCCSMTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC122::ParamNumber))).WillByDefault(Throw(std::out_of_range{""}));
    }

    void SetUp() override {}
};

TEST_F(DefrosterWindscreenCCSMBrokenCarConfigTest, Constructor_WhenCC122Broken_WillThrow) {
    EXPECT_THROW(createSut(), std::out_of_range);
}

class CarWithNoWindscreenDefrosterTest : public DefrosterWindscreenCCSMTest {
  public:
    CarWithNoWindscreenDefrosterTest() : DefrosterWindscreenCCSMTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC122::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC122::No_Heated_Frontscreen)));
        defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::OFF);
    }
};

TEST_F(CarWithNoWindscreenDefrosterTest,
       SetMaxDefroster_WhenCarHasNoWindscreenDefroster_WillNotBeAbleToRequestWindscreenDefroster) {
    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
    EXPECT_CALL(electricDefrosterWindscreenLogic_, request(_)).Times(0);
    EXPECT_CALL(electricDefrosterWindscreenPopupLogic_, request(_)).Times(0);
}

TEST_F(CarWithNoWindscreenDefrosterTest,
       SetMaxDefroster_WhenMaxDefrosterIsOffUponButtonPush_WillRequestMaxDefrosterOn) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    EXPECT_CALL(maxDefrosterLogic_, request({OnOff::On}));
    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
}

TEST_F(CarWithNoWindscreenDefrosterTest,
       SetMaxDefroster_WhenMaxDefrosterIsOnUponButtonPush_WillRequestMaxDefrosterOff) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    EXPECT_CALL(maxDefrosterLogic_, request({OnOff::Off}));
    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
}

TEST_F(CarWithNoWindscreenDefrosterTest,
       SetMaxDefroster_WhenMaxDefrosterIsOnAndButtonNotPushed_WillNotRequestMaxDefroster) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    EXPECT_CALL(maxDefrosterLogic_, request(_)).Times(0);
    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::NotPsd);
}

class CarWithWindscreenDefrosterTest : public DefrosterWindscreenCCSMTest {
  public:
    CarWithWindscreenDefrosterTest() : DefrosterWindscreenCCSMTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC122::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC122::Heated_Frontscreen)));
    }
};

TEST_F(CarWithWindscreenDefrosterTest,
       SetMaxDefrosterAndDefrosterWindscreen_WhenMaxAndWindscreenDefrosterOffUponButtonPush_WillRequestWindscreenDefrosterOn) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::OFF);

    EXPECT_CALL(maxDefrosterLogic_, request(_)).Times(0);
    EXPECT_CALL(electricDefrosterWindscreenLogic_, request({OnOff::On}));
    EXPECT_CALL(electricDefrosterWindscreenPopupLogic_, request({OnOff::On}));
    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
}

TEST_F(CarWithWindscreenDefrosterTest,
       SetMaxDefrosterAndDefrosterWindscreen_WhenMaxOnAndWindscreenOffUponButtonPush_WillRequestMaxAndWindscreenDefrosterOff) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::OFF);

    EXPECT_CALL(maxDefrosterLogic_, request({OnOff::Off}));
    EXPECT_CALL(electricDefrosterWindscreenLogic_, request({OnOff::Off}));
    EXPECT_CALL(electricDefrosterWindscreenPopupLogic_, request({OnOff::Off}));
    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
}

TEST_F(CarWithWindscreenDefrosterTest,
       SetMaxDefrosterAndDefrosterWindscreen_WhenMaxOnAndWindscreenOnUponButtonPush_WillRequestMaxAndWindscreenDefrosterOff) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::ON);

    EXPECT_CALL(maxDefrosterLogic_, request({OnOff::Off}));
    EXPECT_CALL(electricDefrosterWindscreenLogic_, request({OnOff::Off}));
    EXPECT_CALL(electricDefrosterWindscreenPopupLogic_, request({OnOff::Off}));
    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
}

TEST_F(CarWithWindscreenDefrosterTest,
       SetMaxDefrosterAndDefrosterWindscreen_WhenMaxOffAndWindscreenOnUponButtonPush_WillRequestMaxAndWindscreenDefrosterOn) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::ON);

    EXPECT_CALL(maxDefrosterLogic_, request({OnOff::On}));
    EXPECT_CALL(electricDefrosterWindscreenLogic_, request({OnOff::On}));
    EXPECT_CALL(electricDefrosterWindscreenPopupLogic_, request({OnOff::On}));

    std::function<void(void)> func;
    EXPECT_CALL(dispatcher_, Start(std::chrono::milliseconds{static_cast<int>(lcfgDelay * 1000)}, _, _))
            .WillOnce(SaveArg<1>(&func));

    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);
    if (func) {
        func();
    } else {
        EXPECT_EQ(true, false);
    }
}

TEST_F(CarWithWindscreenDefrosterTest,
       SetMaxDefrosterAndDefrosterWindscreen_WhenMaxOffAndWindscreenOnUponButtonPush_WillNotRequestMaxAndWindscreenDefrosterOnBeforeTimerTimeout) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::ON);

    EXPECT_CALL(maxDefrosterLogic_, request(_)).Times(0);
    EXPECT_CALL(electricDefrosterWindscreenLogic_, request(_)).Times(0);
    EXPECT_CALL(electricDefrosterWindscreenPopupLogic_, request(_)).Times(0);

    std::function<void(void)> func;
    EXPECT_CALL(dispatcher_, Start(std::chrono::milliseconds{static_cast<int>(lcfgDelay * 1000)}, _, _))
            .WillOnce(SaveArg<1>(&func));

    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::Psd);

    // No timer timeout
}

TEST_F(CarWithWindscreenDefrosterTest,
       SetMaxDefrosterAndDefrosterWindscreen_WhenButtonNotPushed_WillNotRequestMaxAndWindscreenDefroster) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::OFF);

    EXPECT_CALL(maxDefrosterLogic_, request(_)).Times(0);
    EXPECT_CALL(electricDefrosterWindscreenLogic_, request(_)).Times(0);
    EXPECT_CALL(electricDefrosterWindscreenPopupLogic_, request(_)).Times(0);
    frontDefrosterButtonReq_.inject(autosar::PsdNotPsd::NotPsd);
}

class DefrosterWindscreenCCSMButtonLedTest : public DefrosterWindscreenCCSMTest {
  public:
    DefrosterWindscreenCCSMButtonLedTest() : DefrosterWindscreenCCSMTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC122::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC122::Heated_Frontscreen)));

        maxDefroster_.set(FirstRowGen::MaxDefrosterState::DISABLED);
        defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::DISABLED);
    }
};

TEST_F(DefrosterWindscreenCCSMButtonLedTest, SetButtonLed_WhenMaxOffAndWindscreenOff_WillSetLedOff) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::OFF);

    EXPECT_EQ(autosar::OnOff1::Off, frontDefrosterButtonLedReq_.get().value());
}

TEST_F(DefrosterWindscreenCCSMButtonLedTest, SetButtonLed_WhenMaxOnAndWindscreenOff_WillSetLedOn) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::OFF);

    EXPECT_EQ(autosar::OnOff1::On, frontDefrosterButtonLedReq_.get().value());
}

TEST_F(DefrosterWindscreenCCSMButtonLedTest, SetButtonLed_WhenMaxOnAndWindscreenOn_WillSetLedOn) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::ON);

    EXPECT_EQ(autosar::OnOff1::On, frontDefrosterButtonLedReq_.get().value());
}

TEST_F(DefrosterWindscreenCCSMButtonLedTest, SetButtonLed_WhenMaxOffAndWindscreenOn_WillSetLedOn) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    defrosterWindscreen_.set(FirstRowGen::ElectricDefrosterWindscreenState::ON);

    EXPECT_EQ(autosar::OnOff1::On, frontDefrosterButtonLedReq_.get().value());
}
