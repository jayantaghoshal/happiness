/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ionizer_logic.h"

#include "carconfig_mock.h"
#include "enum_helper.h"
#include "notifiable_property.h"
#include "settings_proxy.h"
#include "v0/org/volvocars/climate/FirstRow.hpp"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gtest/gtest.h>
#include <libsettings/setting.h>
#include <memory>
#include "mock_settingsmanager.h"
using namespace testing;
using namespace autosar;

using UserScope = SettingsFramework::UserScope;
using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using IonizerState = FirstRowGen::IonizerState;
using CC349 = CarConfigParams::CC349_IonicInternalAirCleanerType;

namespace {

VehModMngtGlbSafe1 createVehMod(UsgModSts1 usgModeSts1, CarModSts1 carModSts1) {
    VehModMngtGlbSafe1 drvMode{};
    drvMode.UsgModSts = usgModeSts1;
    drvMode.CarModSts1_ = carModSts1;
    return drvMode;
}
}

namespace first_row {

class IonizerTest : public ::testing::Test {
  public:
    IonizerTest()
        : carConfig_{},
          ionizer_{FirstRowGen::IonizerState::NOT_PRESENT},
          ionizerSetting_{SettingId::TestSetting1, true, settingsManager},
          ionizerDynoSetting_{SettingId::TestSetting1, false, settingsManager},
          ionizerSettingProxy_{ionizerSetting_, ionizerDynoSetting_},
          vehicleModeSignal_{},
          climaActvSignal_{},
          sut_{} {
        ON_CALL(carConfig_, getValue(to_undrl(CC349::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC349::With_Ionic_internal_air_cleaner)));

        DataElementFramework::instance().reset();
    }

    void SetUp() override { createSut(); }

  protected:
    void createSut() { sut_ = std::make_unique<IonizerLogic>(ionizer_, ionizerSettingProxy_); }

    void resetVehMod() {
        const auto drvMode = createVehMod(UsgModSts1::UsgModAbdnd, CarModSts1::CarModFcy);
        vehicleModeSignal_.inject(drvMode);
    }

    NiceMock<CarConfigMock> carConfig_;

    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    NotifiableProperty<FirstRowGen::IonizerState> ionizer_;
    CompatSetting<bool, UserScope::NOT_USER_RELATED> ionizerSetting_;
    CompatSetting<bool, UserScope::NOT_USER_RELATED> ionizerDynoSetting_;
    SettingsProxy<bool, UserScope::NOT_USER_RELATED, UserScope::NOT_USER_RELATED> ionizerSettingProxy_;

    ECDDataElement::DEInjector<VehModMngtGlbSafe1_info> vehicleModeSignal_;
    ECDDataElement::DEInjector<ClimaActv_info> climaActvSignal_;
    ECDDataElement::DESink<AirClngReq_info> airClngReqSignal_;

    std::unique_ptr<IonizerLogic> sut_;
};

TEST_F(IonizerTest, Constructor_WhenVehModError_WillSetError) {
    vehicleModeSignal_.error(0);
    climaActvSignal_.inject(OnOff1::Off);
    EXPECT_EQ(FirstRowGen::IonizerState::SYSTEM_ERROR, ionizer_.get());
}

TEST_F(IonizerTest, Constructor_WhenClimaActvError_WillSetError) {
    resetVehMod();
    climaActvSignal_.error(0);
    EXPECT_EQ(FirstRowGen::IonizerState::SYSTEM_ERROR, ionizer_.get());
}

TEST_F(IonizerTest, Request_WillReturnNotReady) {
    climaActvSignal_.error(0);
    EXPECT_EQ(CommonTypesGen::ReturnCode::NOT_READY, sut_->request(FirstRowGen::IonizerRequest::OFF));
}

class IonizerLogicBrokenCarConfigTest : public IonizerTest {
  public:
    IonizerLogicBrokenCarConfigTest() : IonizerTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC349::ParamNumber))).WillByDefault(Throw(std::out_of_range{""}));
    }

    void SetUp() override {}
};

TEST_F(IonizerLogicBrokenCarConfigTest, Constructor_WhenCC349Broken_WillThrow) {
    EXPECT_THROW(createSut(), std::out_of_range);
}

class IonizerLogicDisabledCarConfigTest : public IonizerTest {
  public:
    IonizerLogicDisabledCarConfigTest() : IonizerTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC349::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC349::Without_Ionic_internal_air_cleaner)));
    }
};

TEST_F(IonizerLogicDisabledCarConfigTest, Constructor_WillBeNotPresent) {
    EXPECT_EQ(FirstRowGen::IonizerState::NOT_PRESENT, ionizer_.get());
}

TEST_F(IonizerLogicDisabledCarConfigTest, Constructor_WillAirClngReqNoReq) {
    EXPECT_EQ(OnOffNoReq::NoReq, airClngReqSignal_.get().value());
}

TEST_F(IonizerLogicDisabledCarConfigTest, Request_WillDoNothing) {
    sut_->request(FirstRowGen::IonizerRequest::ON);

    EXPECT_EQ(FirstRowGen::IonizerState::NOT_PRESENT, ionizer_.get());
}

TEST_F(IonizerLogicDisabledCarConfigTest, Request_WillReturnNotPresent) {
    EXPECT_EQ(CommonTypesGen::ReturnCode::FUNCTION_NOT_PRESENT, sut_->request(FirstRowGen::IonizerRequest::OFF));
}

class IonizerLogicActivationTest
        : public IonizerTest,
          public WithParamInterface<std::tuple<OnOff1, UsgModSts1, CarModSts1, FirstRowGen::IonizerState>> {
  public:
    IonizerLogicActivationTest() : IonizerTest() {
        auto const params = GetParam();

        ionizerSetting_.set(false);

        climaActvSignal_.inject(std::get<0>(params));
        vehicleModeSignal_.inject(createVehMod(std::get<1>(params), std::get<2>(params)));
    }
};

INSTANTIATE_TEST_CASE_P(
        , IonizerLogicActivationTest,
        Values(std::make_tuple(OnOff1::Off, UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm, IonizerState::OFF),
               std::make_tuple(OnOff1::Off, UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno, IonizerState::OFF),
               std::make_tuple(OnOff1::On, UsgModSts1::UsgModInActv, CarModSts1::CarModNorm, IonizerState::OFF),
               std::make_tuple(OnOff1::On, UsgModSts1::UsgModInActv, CarModSts1::CarModDyno, IonizerState::OFF),
               std::make_tuple(OnOff1::On, UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm, IonizerState::DISABLED),
               std::make_tuple(OnOff1::Off, UsgModSts1::UsgModInActv, CarModSts1::CarModFcy, IonizerState::DISABLED),
               std::make_tuple(OnOff1::Off, UsgModSts1::UsgModInActv, CarModSts1::CarModNorm, IonizerState::DISABLED),
               std::make_tuple(OnOff1::Off, UsgModSts1::UsgModAbdnd, CarModSts1::CarModNorm, IonizerState::DISABLED),
               std::make_tuple(OnOff1::Off, UsgModSts1::UsgModDrvg, CarModSts1::CarModFcy, IonizerState::DISABLED),
               std::make_tuple(OnOff1::Off, UsgModSts1::UsgModInActv, CarModSts1::CarModNorm, IonizerState::DISABLED)));

TEST_P(IonizerLogicActivationTest, Constructor_WhenValidConditions_WillBeActive) {
    auto const result = std::get<3>(GetParam());

    EXPECT_EQ(result, ionizer_.get());
}

class IonizerLogicNormalTest : public IonizerTest {
  public:
    IonizerLogicNormalTest() : IonizerTest() {
        ionizerSetting_.set(true);

        resetVehMod();
        climaActvSignal_.inject(OnOff1::Off);
    }
};

TEST_F(IonizerLogicNormalTest, Request_WhenNotActive_WillReturnDisabled) {
    EXPECT_EQ(CommonTypesGen::ReturnCode::FUNCTION_IS_DISABLED, sut_->request(FirstRowGen::IonizerRequest::OFF));
}

TEST_F(IonizerLogicNormalTest, Constructor_WhenGoingToActiveAndSettingOff_WillSetStateOff) {
    ionizerSetting_.set(false);

    vehicleModeSignal_.inject(createVehMod(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm));

    EXPECT_EQ(FirstRowGen::IonizerState::OFF, ionizer_.get());
}

TEST_F(IonizerLogicNormalTest, Constructor_WhenGoingToActiveAndSettingOn_WillSetStateOn) {
    ionizerSetting_.set(true);

    vehicleModeSignal_.inject(createVehMod(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm));

    EXPECT_EQ(FirstRowGen::IonizerState::ON, ionizer_.get());
}

TEST_F(IonizerLogicNormalTest, VehModSignalCB_WhenGoingToDynoAndSettingOn_WillUseDynoSettingAndSetOff) {
    EXPECT_TRUE(ionizerSetting_.get());

    vehicleModeSignal_.inject(createVehMod(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno));

    EXPECT_EQ(FirstRowGen::IonizerState::OFF, ionizer_.get());
}

TEST_F(IonizerLogicNormalTest, VehModSignalCB_WhenExitingDyno_WillRestoreState) {
    vehicleModeSignal_.inject(createVehMod(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno));
    vehicleModeSignal_.inject(createVehMod(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm));

    EXPECT_EQ(FirstRowGen::IonizerState::ON, ionizer_.get());
}

class IonizerLogicActiveTest : public IonizerLogicNormalTest {
  public:
    IonizerLogicActiveTest() : IonizerLogicNormalTest() {
        vehicleModeSignal_.inject(createVehMod(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm));
    }
};

TEST_F(IonizerLogicActiveTest, Constructor_WhenOn_WillSetAirClngReqOn) {
    EXPECT_EQ(OnOffNoReq::On, airClngReqSignal_.get().value());
}

TEST_F(IonizerLogicActiveTest, Constructor_WhenOff_WillSetAirClngReqOff) {
    ionizerSetting_.set(false);
    EXPECT_EQ(OnOffNoReq::Off, airClngReqSignal_.get().value());
}

TEST_F(IonizerLogicActiveTest, IonizerSettingCB_WhenIonizerSettingTrue_WillBeOn) {
    ionizerSetting_.set(false);
    ionizerSetting_.set(true);

    EXPECT_EQ(FirstRowGen::IonizerState::ON, ionizer_.get());
}

TEST_F(IonizerLogicActiveTest, IonizerSettingCB_WhenIonizerSettingFalse_WillBeOff) {
    ionizerSetting_.set(false);

    EXPECT_EQ(FirstRowGen::IonizerState::OFF, ionizer_.get());
}

TEST_F(IonizerLogicActiveTest, Request_WhenRequestOn_WillBeOn) {
    ionizerSetting_.set(false);

    sut_->request(FirstRowGen::IonizerRequest::ON);

    EXPECT_TRUE(ionizerSetting_.get());
    EXPECT_EQ(FirstRowGen::IonizerState::ON, ionizer_.get());
}

TEST_F(IonizerLogicActiveTest, Request_WhenRequestOff_WillBeOff) {
    sut_->request(FirstRowGen::IonizerRequest::OFF);

    EXPECT_FALSE(ionizerSetting_.get());
    EXPECT_EQ(FirstRowGen::IonizerState::OFF, ionizer_.get());
}

TEST_F(IonizerLogicActiveTest, Request_WillReturnSucess) {
    EXPECT_EQ(CommonTypesGen::ReturnCode::SUCCESS, sut_->request(FirstRowGen::IonizerRequest::OFF));
}
}
