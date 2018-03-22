/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "temperature_sync.h"

#include "carconfig_mock.h"
#include "enum_helper.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include <tuple>
#include "mock_settingsmanager.h"

using namespace testing;
using namespace SettingsFramework;

using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using CC175 = CarConfigParams::CC175_HvacVariantsType;

namespace {
autosar::VehModMngtGlbSafe1 createVehMod(autosar::UsgModSts1 usgModeSts1, autosar::CarModSts1 carModSts1) {
    autosar::VehModMngtGlbSafe1 drvMode{};
    drvMode.UsgModSts = usgModeSts1;
    drvMode.CarModSts1_ = carModSts1;
    return drvMode;
}
}

class TemperatureSyncTest : public ::testing::Test {
  public:
    TemperatureSyncTest()
        : carConfig_{},
          temperatureSync_{},
          driverTemp_{},
          driverTempHiLoN_{},
          passengerTemp_{},
          passengerTempHiLoN_{},
          rearLeftTemp_{},
          rearLeftTempHiLoN_{},
          rearRightTemp_{},
          rearRightTempHiLoN_{},
          maxDefroster_{},
          temperatureSyncSetting_{},
          temperatureSyncNormal_{SettingId::TestSetting1, false, settingsManager},
          temperatureSyncDyno_{SettingId::TestSetting2, false, settingsManager} {
        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC175::Electronic_Climate_Control_4_zone)));

        DataElementFramework::instance().reset();

        temperatureSyncSetting_ =
                new SettingsProxy<bool, UserScope::USER, UserScope::USER>(temperatureSyncNormal_, temperatureSyncDyno_);
    }

    void SetUp() override { createSut(); }

  protected:
    void createSut() {
        sut_ = std::make_unique<TemperatureSyncLogic>(
                temperatureSync_,
                driverTemp_,
                driverTempHiLoN_,
                passengerTemp_,
                passengerTempHiLoN_,
                rearLeftTemp_,
                rearLeftTempHiLoN_,
                rearRightTemp_,
                rearRightTempHiLoN_,
                maxDefroster_,
                std::unique_ptr<SettingsProxy<bool, UserScope::USER, UserScope::USER>>(temperatureSyncSetting_));
    }

    void resetVehMod() {
        auto drvMode = createVehMod(autosar::UsgModSts1::UsgModAbdnd, autosar::CarModSts1::CarModFcy);
        drvMode.FltEgyCnsWdSts = autosar::FltEgyCns1::NoFlt;
        vehModeInjector_.inject(drvMode);
    }

    NiceMock<CarConfigMock> carConfig_;

    NotifiableProperty<FirstRowGen::TemperatureSyncState> temperatureSync_;
    NotifiableProperty<double> driverTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> driverTempHiLoN_;
    NotifiableProperty<double> passengerTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> passengerTempHiLoN_;
    NotifiableProperty<double> rearLeftTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> rearLeftTempHiLoN_;
    NotifiableProperty<double> rearRightTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> rearRightTempHiLoN_;
    NotifiableProperty<FirstRowGen::MaxDefrosterState> maxDefroster_;

    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    SettingsProxy<bool, UserScope::USER, UserScope::USER>* temperatureSyncSetting_;

    CompatSetting<bool, UserScope::USER> temperatureSyncNormal_;
    CompatSetting<bool, UserScope::USER> temperatureSyncDyno_;

    ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> vehModeInjector_;
    ECDDataElement::DEInjector<autosar::ClimaActv_info> climaActvInjector_;

    std::unique_ptr<TemperatureSyncLogic> sut_;
};

class TemperatureSyncBrokenCarConfigTest : public TemperatureSyncTest

{
  public:
    TemperatureSyncBrokenCarConfigTest() : TemperatureSyncTest() {
        ON_CALL(carConfig_, getValue(_)).WillByDefault(Throw(std::out_of_range{""}));
    }

    void SetUp() override {}
};
TEST_F(TemperatureSyncBrokenCarConfigTest, Constructor_WhenCC175Broken_WillThrow) {
    EXPECT_THROW(createSut(), std::out_of_range);
}

class TemperatureSyncCarConfigNotOkTest : public TemperatureSyncTest,
                                          public WithParamInterface<CarConfigParams::CC175_HvacVariantsType> {
  public:
    TemperatureSyncCarConfigNotOkTest() : TemperatureSyncTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber))).WillByDefault(Return(to_undrl(GetParam())));
    }
};

TEST_P(TemperatureSyncCarConfigNotOkTest, CarConfigCheck_WhenCarConfigNotOk_WillSetSyncStateNotPresent) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::NOT_PRESENT, temperatureSync_.get());
}

INSTANTIATE_TEST_CASE_P(CarConfigCheck_WhenCarConfigNotOk_WillSetSyncStateNotVisible,
                        TemperatureSyncCarConfigNotOkTest,
                        Values(CarConfigParams::CC175_HvacVariantsType::HVAC_Small_1_zone,
                               CarConfigParams::CC175_HvacVariantsType::HVAC_Small_3_zone,
                               CarConfigParams::CC175_HvacVariantsType::Manual_AC,
                               CarConfigParams::CC175_HvacVariantsType::InvalidValue));

class TemperatureSyncCarConfigkOkTest : public TemperatureSyncCarConfigNotOkTest {};

TEST_P(TemperatureSyncCarConfigkOkTest, CarConfigCheck_WhenCarConfigOk_WillSetSyncStateDefaultDisabled) {
    // This is to run the a lambda function that sets the output signal to DISABLED
    climaActvInjector_.inject(autosar::OnOff1::Off);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::DISABLED, temperatureSync_.get());
}

INSTANTIATE_TEST_CASE_P(CarConfigCheck_WhenCarConfigNotOk_WillSetSyncStateNotVisible,
                        TemperatureSyncCarConfigkOkTest,
                        Values(CarConfigParams::CC175_HvacVariantsType::HVAC_Small_2_zone,
                               CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_2_zone,
                               CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_4_zone));

class TemperatureSyncActivationCheckTest
        : public TemperatureSyncTest,
          public WithParamInterface<std::tuple<autosar::UsgModSts1, autosar::CarModSts1>> {
  public:
    TemperatureSyncActivationCheckTest() : TemperatureSyncTest() {
        auto const params = GetParam();
        auto const usageMode = std::get<0>(params);
        auto const carMode = std::get<1>(params);
        vehModeInjector_.inject(createVehMod(usageMode, carMode));
    }
};

TEST_P(TemperatureSyncActivationCheckTest, Initialization_WhenNotActive_WillSetSyncDisabled) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::DISABLED, temperatureSync_.get());
}

INSTANTIATE_TEST_CASE_P(,
                        TemperatureSyncActivationCheckTest,
                        Combine(Values(autosar::UsgModSts1::UsgModAbdnd,
                                       autosar::UsgModSts1::UsgModInActv,
                                       autosar::UsgModSts1::UsgModCnvinc,
                                       autosar::UsgModSts1::UsgModActv),
                                Values(autosar::CarModSts1::CarModTrnsp,
                                       autosar::CarModSts1::CarModFcy,
                                       autosar::CarModSts1::CarModCrash)));

class TemperatureSyncSettingTest : public TemperatureSyncTest

{
  public:
    TemperatureSyncSettingTest() : TemperatureSyncTest() {
        climaActvInjector_.inject(autosar::OnOff1::Off);
        maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    }

    void SetUp() override {}
};

TEST_F(TemperatureSyncSettingTest, Setting_WhenSettingSetToSync_WillSetSyncOn) {
    vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm));
    temperatureSyncSetting_->set(true);

    createSut();

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());
}

TEST_F(TemperatureSyncSettingTest, Setting_WhenSettingSetToNotSync_WillSetSyncOff) {
    vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm));
    temperatureSyncSetting_->set(false);

    createSut();

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncSettingTest, Setting_WhenSwitchingCarMode_WillRememberSetting) {
    vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm));
    temperatureSyncSetting_->set(true);

    createSut();

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno));

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());

    temperatureSyncSetting_->set(true);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm));

    temperatureSyncSetting_->set(false);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());

    vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno));

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

class TemperatureSyncRequestTest : public TemperatureSyncTest

{
  public:
    TemperatureSyncRequestTest() : TemperatureSyncTest() {
        vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm));
        climaActvInjector_.inject(autosar::OnOff1::Off);
        maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    }
};

TEST_F(TemperatureSyncRequestTest, Request_WhenRequestingOn_WillSetSyncOn) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::ON);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());
}

TEST_F(TemperatureSyncRequestTest, Request_WhenRequestingOff_WillSetSyncOff) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::ON);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    sut_->request(FirstRowGen::TemperatureSyncRequest::OFF);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncRequestTest, Request_WhenSyncIsOn_WillSetSettingToOn) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::ON);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());
    EXPECT_EQ(temperatureSyncNormal_.get(), true);
}

TEST_F(TemperatureSyncRequestTest, Request_WhenSyncIsOff_WillSetSettingToOff) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::OFF);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
    EXPECT_EQ(temperatureSyncNormal_.get(), false);
}

class TemperatureSyncTemperatureOverrideTest : public TemperatureSyncTest

{
  public:
    TemperatureSyncTemperatureOverrideTest() : TemperatureSyncTest() {
        vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm));
        climaActvInjector_.inject(autosar::OnOff1::Off);
        maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);

        temperatureSyncSetting_->set(true);

        driverTemp_.set(22.0);
        driverTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
        passengerTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
    }
};

TEST_F(TemperatureSyncTemperatureOverrideTest, PassengerTempCB_WhenSyncIsOn_WillSetToOff) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    passengerTemp_.set(25.0);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, PassengerTempSpecialCB_WhenSyncIsOn_WillSetToOff) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    passengerTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, PassengerTempCB_WhenSyncIsOff_WillRemainOff) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::OFF);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());

    passengerTemp_.set(19.0);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, PassengerTempSpecialCB_WhenSyncIsOff_WillRemainOff) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::OFF);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());

    passengerTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, RearLeftTempCB_WhenSyncIsOn_WillSetToOff) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    rearLeftTemp_.set(25.0);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, RearLeftTempSpecialCB_WhenSyncIsOn_WillSetToOff) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    rearLeftTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, RearLeftTempCB_WhenSyncIsOff_WillRemainOff) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::OFF);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());

    rearLeftTemp_.set(25.0);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, RearLeftTempSpecialCB_WhenSyncIsOff_WillRemainOff) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::OFF);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());

    rearLeftTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, RearRightTempCB_WhenSyncIsOn_WillSetToOff) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    rearRightTemp_.set(25.0);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, RearRightTempSpecialCB_WhenSyncIsOn_WillSetToOff) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    rearRightTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, RearRightTempCB_WhenSyncIsOff_WillRemainOff) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::OFF);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());

    rearRightTemp_.set(25.0);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

TEST_F(TemperatureSyncTemperatureOverrideTest, RearRightTempSpecialCB_WhenSyncIsOff_WillRemainOff) {
    sut_->request(FirstRowGen::TemperatureSyncRequest::OFF);
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());

    rearRightTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::OFF, temperatureSync_.get());
}

class TemperatureSyncMaxDefrosterTest : public TemperatureSyncTest

{
  public:
    TemperatureSyncMaxDefrosterTest() : TemperatureSyncTest() {
        vehModeInjector_.inject(createVehMod(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm));
        climaActvInjector_.inject(autosar::OnOff1::Off);
        maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);

        temperatureSyncSetting_->set(true);
    }
};

TEST_F(TemperatureSyncMaxDefrosterTest, MaxDefroster_WhenMaxDefrosterIsSet_WillSetSyncToDisabled) {
    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::DISABLED, temperatureSync_.get());

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);

    EXPECT_EQ(FirstRowGen::TemperatureSyncState::ON, temperatureSync_.get());
}
