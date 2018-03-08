/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "air_conditioner_logic.h"
#include "fan_level_front_logic.h"

#include "carconfig_mock.h"

#include "enum_helper.h"
#include "max_defroster_logic.h"
#include "mock_dispatcher.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libsettings/setting.h>
#include <chrono>
#include <tuple>
#include <v0/org/volvocars/climate/FirstRow.hpp>
#include "mock_settingsmanager.h"

namespace {
const int lcfgTimeout = 100;
}

namespace TestFanLevelFront {

using namespace ApplicationDataElement;
using namespace autosar;
using namespace testing;
using namespace ECDDataElement;
using namespace SettingsFramework;

using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using CC175 = CarConfigParams::CC175_HvacVariantsType;

class FanLevelFrontTest : public ::testing::Test {
    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();

  public:
    FanLevelFrontTest()
        : settingLastAutoFanLevel(SettingId::TestSetting1, FirstRowGen::FanLevelFrontValue::OFF, settingsManager),
          settingLastAutoFanLevelDyno(SettingId::TestSetting2, FirstRowGen::FanLevelFrontValue::OFF, settingsManager),
          lastFanAutoFanLevelProxy(settingLastAutoFanLevel, settingLastAutoFanLevelDyno),
          settingLastFanLevel(SettingId::TestSetting3, FirstRowGen::FanLevelFrontValue::OFF, settingsManager),
          settingLastFanLevelDyno(SettingId::TestSetting4, FirstRowGen::FanLevelFrontValue::OFF, settingsManager),
          lastFanLevelFrontProxySetting(settingLastFanLevel, settingLastFanLevelDyno),
          autoClimate_{FirstRowGen::AutoClimateState::OFF},
          fanLevelFront_{FirstRowGen::FanLevelFrontValue::OFF},
          MaxDefrost_{FirstRowGen::MaxDefrosterState::OFF},
          climateReset_{ClimateResetLogic::ClimateResetEvent::NO_REQ} {
        DataElementFramework::instance().reset();

        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC175::Electronic_Climate_Control_2_zone)));

        resetVehicleMode();

        ON_CALL(localConfig_, GetIntMock("Climate_FanLevelFront_timeout")).WillByDefault(Return(lcfgTimeout));

        setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::OFF);
        setAutoClimateSetting(FirstRowGen::AutoClimateState::OFF);
        setClimateReset(ClimateResetLogic::ClimateResetEvent::NO_REQ);

        auto lastAutoFanLevel = FirstRowGen::FanLevelFrontValue::OFF;
        settingLastAutoFanLevel.set(lastAutoFanLevel);

        auto lastFanLevel = FirstRowGen::FanLevelFrontValue::OFF;
        lastFanLevelFrontProxySetting.set(lastFanLevel);
    }

  protected:
    std::unique_ptr<FanLevelFrontLogic> createFanLevelFrontLogic() {
        return std::make_unique<FanLevelFrontLogic>(&localConfig_, lastFanAutoFanLevelProxy,
                                                    lastFanLevelFrontProxySetting, fanLevelFront_, MaxDefrost_,
                                                    autoClimate_, climateReset_, dispatcher_);
    }

    void setVehicleMode(CarModSts1 carModSts1, UsgModSts1 usgModeSts1) {
        VehModMngtGlbSafe1 drvMode;
        drvMode.CarModSts1_ = carModSts1;
        drvMode.UsgModSts = usgModeSts1;
        driverMode.inject(drvMode);
    }

    void resetVehicleMode() {
        VehModMngtGlbSafe1 drvMode;
        drvMode.CarModSts1_ = CarModSts1::CarModFcy;
        drvMode.UsgModSts = UsgModSts1::UsgModAbdnd;
        drvMode.CarModSubtypWdCarModSubtyp = 0;
        drvMode.EgyLvlElecMai = 0;
        drvMode.EgyLvlElecSubtyp = 0;
        drvMode.PwrLvlElecMai = 0;
        drvMode.PwrLvlElecSubtyp = 0;
        drvMode.FltEgyCnsWdSts = FltEgyCns1::NoFlt;
        drvMode.Chks = 0;
        drvMode.Cntr = 0;
        driverMode.inject(drvMode);
    }

    DEInjector<autosar::ClimaActv_info> climateActve;
    void setClimateActive(OnOff1 value, bool signalError = false) {
        OnOff1 onOff;
        onOff = value;
        if (!signalError) {
            climateActve.inject(onOff);
        } else {
            climateActve.error(1);
        }
    }
    //  MaxDefrostValue {OFF, ON};
    void setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState frost) { MaxDefrost_.set(frost); }

    void setAutoClimateSetting(FirstRowGen::AutoClimateState autoClimate) { autoClimate_.set(autoClimate); }

    void setClimateReset(ClimateResetLogic::ClimateResetEvent climateEvent) { climateReset_.set(climateEvent); }

    void setFanLevelFront(FirstRowGen::FanLevelFrontValue fanLevel) {
        lastFanLevelFrontProxySetting.set(static_cast<FirstRowGen::FanLevelFrontValue::Literal>(fanLevel.value_));
    }

    DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;

    CompatSetting<FirstRowGen::FanLevelFrontValue::Literal, UserScope::USER> settingLastAutoFanLevel;
    CompatSetting<FirstRowGen::FanLevelFrontValue::Literal, UserScope::NOT_USER_RELATED> settingLastAutoFanLevelDyno;
    SettingsProxy<FirstRowGen::FanLevelFrontValue::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>
            lastFanAutoFanLevelProxy;
    CompatSetting<FirstRowGen::FanLevelFrontValue::Literal, UserScope::USER> settingLastFanLevel;
    CompatSetting<FirstRowGen::FanLevelFrontValue::Literal, UserScope::NOT_USER_RELATED> settingLastFanLevelDyno;
    SettingsProxy<FirstRowGen::FanLevelFrontValue::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>
            lastFanLevelFrontProxySetting;

    // Signals (needed here so we can be sure they are instantiated before the test start)
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> usgModSts_;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climateActiveSignal_;
    ApplicationDataElement::DESender<autosar::HmiCmptmtCoolgReq_info> hmiCmptmtCoolgReq_;

    NotifiableProperty<FirstRowGen::AutoClimateState> autoClimate_;
    NotifiableProperty<FirstRowGen::FanLevelFrontValue> fanLevelFront_;
    NotifiableProperty<FirstRowGen::MaxDefrosterState> MaxDefrost_;
    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> climateReset_;

    NiceMock<MockDispatcher> dispatcher_;
    NiceMock<vcc::mocks::MockLocalConfigReader> localConfig_;
    NiceMock<CarConfigMock> carConfig_;
};

TEST_F(FanLevelFrontTest, testInit) {
    auto fan = createFanLevelFrontLogic();

    setVehicleMode(CarModSts1::CarModFcy, UsgModSts1::UsgModAbdnd);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::DISABLED, fanLevelFront_.get());
}

class FanLevelFrontCarConfigTest : public FanLevelFrontTest, public WithParamInterface<std::tuple<CC175>> {};

TEST_P(FanLevelFrontCarConfigTest, testCarConfig) {
    auto carconfigParam = std::get<0>(GetParam());

    EXPECT_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber))).WillRepeatedly(Return(to_undrl(carconfigParam)));

    auto fan = createFanLevelFrontLogic();

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModCnvinc);
    bool ccNOK = (carconfigParam == CC175::InvalidValue);

    if (ccNOK) {
        EXPECT_EQ(FirstRowGen::FanLevelFrontValue::DISABLED, fanLevelFront_.get());
        EXPECT_TRUE(fan->isInState(FanLevelFrontLogic::CARCONFIG_INVALID));
    } else {
        EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
        EXPECT_FALSE(fan->isInState(FanLevelFrontLogic::CARCONFIG_INVALID));
    }
}

INSTANTIATE_TEST_CASE_P(testCarConfig, FanLevelFrontCarConfigTest,
                        Values(CC175::Electronic_Climate_Control_2_zone, CC175::Electronic_Climate_Control_4_zone,
                               CC175::HVAC_Small_1_zone, CC175::HVAC_Small_2_zone, CC175::HVAC_Small_3_zone,
                               CC175::InvalidValue, CC175::Manual_AC));

TEST_F(FanLevelFrontTest, TestChangeToLevel1ECC) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::On);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL1);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);
}

TEST_F(FanLevelFrontTest, TestChangeToLevel5ECC) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::On);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL5);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL5, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutPlusPlus, outvalue);
}

TEST_F(FanLevelFrontTest, TestChangeToLevelMaxECC) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::Off);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::MAX);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::MAX, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Max, outvalue);
}

TEST_F(FanLevelFrontTest, TestMaxDefrosterAndAutoClimateOnOff) {
    auto lastAutoFanLevel = FirstRowGen::FanLevelFrontValue::LVL2;
    settingLastAutoFanLevel.set(lastAutoFanLevel);

    auto lastFanLevel = FirstRowGen::FanLevelFrontValue::MAX;
    settingLastFanLevel.set(lastFanLevel);

    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::Off);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(lastFanLevel, fanLevelFront_.get());

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::ON);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL5, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutPlusPlus, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::MAX);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::MAX, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Max, outvalue);

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::OFF);
    EXPECT_EQ(lastFanLevel, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Max, outvalue);

    setAutoClimateSetting(FirstRowGen::AutoClimateState::ON);
    EXPECT_EQ(lastAutoFanLevel, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinus, outvalue);

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::ON);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL5, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutPlusPlus, outvalue);

    setAutoClimateSetting(FirstRowGen::AutoClimateState::OFF);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL5, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutPlusPlus, outvalue);

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::OFF);
    EXPECT_EQ(lastAutoFanLevel, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinus, outvalue);
}

TEST_F(FanLevelFrontTest, TestAutoClimateOnOff) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::On);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL1);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::MAX);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::MAX, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Max, outvalue);

    setAutoClimateSetting(FirstRowGen::AutoClimateState::ON);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    setAutoClimateSetting(FirstRowGen::AutoClimateState::OFF);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);
}

TEST_F(FanLevelFrontTest, TestClimateReset) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::On);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL1);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    setClimateReset(ClimateResetLogic::ClimateResetEvent::ACTIVATED);
    setClimateReset(ClimateResetLogic::ClimateResetEvent::NO_REQ);

    EXPECT_EQ(fanLevelFront_.get(), FirstRowGen::FanLevelFrontValue::LVL3);
    EXPECT_EQ(HmiHvacFanLvl::LvlAutoNorm, outvalue);
}

TEST_F(FanLevelFrontTest, TestClimateResetWhenMaxDefrosterIsOn) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::On);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL1);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::ON);

    EXPECT_EQ(fanLevelFront_.get(), FirstRowGen::FanLevelFrontValue::LVL5);
    EXPECT_EQ(HmiHvacFanLvl::LvlAutPlusPlus, outvalue);

    setClimateReset(ClimateResetLogic::ClimateResetEvent::ACTIVATED);
    setClimateReset(ClimateResetLogic::ClimateResetEvent::NO_REQ);

    EXPECT_EQ(fanLevelFront_.get(), FirstRowGen::FanLevelFrontValue::LVL3);
    EXPECT_EQ(HmiHvacFanLvl::LvlAutoNorm, outvalue);
}

TEST_F(FanLevelFrontTest, MaxDefrosterOn_WhenFanIsOff_WillSetFanToLvl5) {
    auto fan = createFanLevelFrontLogic();

    setClimateActive(OnOff1::Off);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());

    setMaxDefrosterSetting(FirstRowGen::MaxDefrosterState::ON);
    EXPECT_EQ(fanLevelFront_.get(), FirstRowGen::FanLevelFrontValue::LVL5);
}

TEST_F(FanLevelFrontTest, TestClimateActive) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    // Set Climate active signal as error
    setClimateActive(OnOff1::Off, true);

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::SYSTEM_ERROR, fanLevelFront_.get());

    setClimateActive(OnOff1::On);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL1);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::MAX);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::MAX, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Max, outvalue);

    setAutoClimateSetting(FirstRowGen::AutoClimateState::ON);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    setAutoClimateSetting(FirstRowGen::AutoClimateState::OFF);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModCnvinc);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    setClimateActive(OnOff1::Off);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);
}

TEST_F(FanLevelFrontTest, TestTimeout) {
    int timeout = 3;

    EXPECT_CALL(localConfig_, GetIntMock("Climate_FanLevelFront_timeout")).WillRepeatedly(Return(timeout));

    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<autosar::HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFrnt;

    HmiHvacFanLvl outvalue = HmiHvacFanLvl::Off;
    hmiHvacFanLvlFrnt.subscribe([&hmiHvacFanLvlFrnt, &outvalue]() { outvalue = hmiHvacFanLvlFrnt.get().value(); });

    setClimateActive(OnOff1::Off);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    std::function<void(void)> func;
    EXPECT_CALL(dispatcher_, Start(_, _, _)).Times(AtLeast(4)).WillRepeatedly(SaveArg<1>(&func));

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL3);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL3, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutoNorm, outvalue);

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModCnvinc);

    // it is a timer before entering OFF mode
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL3, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutoNorm, outvalue);

    // Now we have a timeout
    func();

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    // Turn fan on again
    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL2);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL2, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinus, outvalue);

    // Now we have a timeout
    func();

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    // Turn fan on again, climateActive On
    setClimateActive(OnOff1::On);
    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL3);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL3, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutoNorm, outvalue);

    // Now we have a timeout
    func();

    // Fan should NOT turn off
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL3, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutoNorm, outvalue);
}

TEST_F(FanLevelFrontTest, Test_that_fan_go_to_dyno_setting_off_when_in_carmode_dyno) {
    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setVehicleMode(CarModSts1::CarModDyno, UsgModSts1::UsgModDrvg);

    setFanLevelFront(FirstRowGen::FanLevelFrontValue::OFF);
    settingLastAutoFanLevel.set(FirstRowGen::FanLevelFrontValue::OFF);

    setClimateActive(OnOff1::On);

    auto fan = createFanLevelFrontLogic();

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);
}

TEST_F(FanLevelFrontTest, Test_that_fan_go_to_dyno_setting_lvl1_when_in_carmode_dyno) {
    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setVehicleMode(CarModSts1::CarModDyno, UsgModSts1::UsgModDrvg);

    setFanLevelFront(FirstRowGen::FanLevelFrontValue::LVL1);
    settingLastAutoFanLevel.set(FirstRowGen::FanLevelFrontValue::OFF);

    setClimateActive(OnOff1::On);

    auto fan = createFanLevelFrontLogic();

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);
}

TEST_F(FanLevelFrontTest, Test_that_fan_go_to_dyno_setting_off_when_reciving_carmode_dyno) {
    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setFanLevelFront(FirstRowGen::FanLevelFrontValue::LVL2);
    settingLastAutoFanLevel.set(FirstRowGen::FanLevelFrontValue::OFF);

    setClimateActive(OnOff1::On);

    auto fan = createFanLevelFrontLogic();

    setVehicleMode(CarModSts1::CarModDyno, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);
}

TEST_F(FanLevelFrontTest, FanLevelFront_SavesBothNormalAndDynoSetting_WhenSwitchingBetweenModes) {
    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    setFanLevelFront(FirstRowGen::FanLevelFrontValue::LVL1);
    settingLastAutoFanLevel.set(FirstRowGen::FanLevelFrontValue::OFF);

    setClimateActive(OnOff1::On);

    auto fan = createFanLevelFrontLogic();

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    // Switch to dyno mode
    setVehicleMode(CarModSts1::CarModDyno, UsgModSts1::UsgModDrvg);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    // Set dyno mode to LVL2
    setFanLevelFront(FirstRowGen::FanLevelFrontValue::LVL2);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL2, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinus, outvalue);

    // Switch to normal mode
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutMinusMinus, outvalue);

    // Switch to dyno mode
    setVehicleMode(CarModSts1::CarModDyno, UsgModSts1::UsgModDrvg);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);
}

TEST_F(FanLevelFrontTest, Test_that_fan_switch_value_when_switching_profile) {
    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    setFanLevelFront(FirstRowGen::FanLevelFrontValue::LVL1);
    settingLastAutoFanLevel.set(FirstRowGen::FanLevelFrontValue::LVL1);

    setClimateActive(OnOff1::On);

    auto fan = createFanLevelFrontLogic();

    setFanLevelFront(FirstRowGen::FanLevelFrontValue::LVL3);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL3, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlAutoNorm, outvalue);

    setFanLevelFront(FirstRowGen::FanLevelFrontValue::MAX);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::MAX, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Max, outvalue);
}

using FanLevelFrontValue = FirstRowGen::FanLevelFrontValue;

class FanLevelFrontActiveEntryTest
        : public FanLevelFrontTest,
          public WithParamInterface<std::tuple<OnOff1, UsgModSts1, FirstRowGen::FanLevelFrontValue>> {
  public:
    FanLevelFrontActiveEntryTest() : FanLevelFrontTest() {
        sut_ = createFanLevelFrontLogic();

        settingLastAutoFanLevel.set(FirstRowGen::FanLevelFrontValue::LVL3);

        auto const params = GetParam();

        auto const climateActive = std::get<0>(params);
        auto const usgMode = std::get<1>(params);

        setClimateActive(climateActive);
        setVehicleMode(CarModSts1::CarModNorm, usgMode);
    }

  private:
    std::unique_ptr<FanLevelFrontLogic> sut_;
};

INSTANTIATE_TEST_CASE_P(, FanLevelFrontActiveEntryTest,
                        Values(std::make_tuple(OnOff1::Off, UsgModSts1::UsgModDrvg, FanLevelFrontValue::OFF),
                               std::make_tuple(OnOff1::Off, UsgModSts1::UsgModCnvinc, FanLevelFrontValue::OFF),
                               std::make_tuple(OnOff1::Off, UsgModSts1::UsgModActv, FanLevelFrontValue::OFF),
                               std::make_tuple(OnOff1::On, UsgModSts1::UsgModCnvinc, FanLevelFrontValue::LVL3),
                               std::make_tuple(OnOff1::On, UsgModSts1::UsgModActv, FanLevelFrontValue::LVL3)));

TEST_P(FanLevelFrontActiveEntryTest, Constructor_WhenValidConditions_WillBeActive) {
    EXPECT_EQ(std::get<2>(GetParam()), fanLevelFront_.get());
}

class FanLevelFrontMCCTest : public FanLevelFrontTest {
  public:
    FanLevelFrontMCCTest() : FanLevelFrontTest() {
        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber))).WillByDefault(Return(to_undrl(CC175::Manual_AC)));
    }
};

TEST_F(FanLevelFrontMCCTest, TestChangeToLevelMaxMCC) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::Off);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::MAX);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::MAX, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Max, outvalue);
}

TEST_F(FanLevelFrontMCCTest, TestActivationAndDeActivation) {
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;

    setClimateActive(OnOff1::Off);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModAbdnd);
    auto fan = createFanLevelFrontLogic();
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::DISABLED, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, hmiHvacFanLvlFront.get().value());

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModCnvinc);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, hmiHvacFanLvlFront.get().value());

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL1);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlMan1, hmiHvacFanLvlFront.get().value());

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModInActv);
    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::DISABLED, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, hmiHvacFanLvlFront.get().value());
}

TEST_F(FanLevelFrontMCCTest, TestChangeToLevelOff) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::On);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL1);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlMan1, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::OFF);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);
}

TEST_F(FanLevelFrontMCCTest, TestChangeToLevel1MCC) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::On);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL1);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL1, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlMan1, outvalue);
}

TEST_F(FanLevelFrontMCCTest, TestChangeToLevel5MCC) {
    auto fan = createFanLevelFrontLogic();

    // Listen to outgoing dataelement
    DESink<HmiHvacFanLvlFrnt_info> hmiHvacFanLvlFront;
    HmiHvacFanLvl outvalue = {HmiHvacFanLvl::Off};
    hmiHvacFanLvlFront.subscribe([&hmiHvacFanLvlFront, &outvalue]() { outvalue = hmiHvacFanLvlFront.get().value(); });

    setClimateActive(OnOff1::On);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::OFF, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::Off, outvalue);

    fan->requestFanLevel(FirstRowGen::FanLevelFrontRequest::LVL5);

    EXPECT_EQ(FirstRowGen::FanLevelFrontValue::LVL5, fanLevelFront_.get());
    EXPECT_EQ(HmiHvacFanLvl::LvlMan5, outvalue);
}

}  // namespace TestFanLevelFront
