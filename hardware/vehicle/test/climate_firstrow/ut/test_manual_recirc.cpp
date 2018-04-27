/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "manual_recirc_logic.h"
#include "mock_dispatcher.h"

#include "carconfig_mock.h"

#include "enum_helper.h"

#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gtest/gtest.h>
#include <libsettings/setting.h>
#include <vcc/localconfig.h>
#include <memory>
#include <tuple>
#include "mock_settingsmanager.h"

using namespace ECDDataElement;
using namespace autosar;
using namespace testing;

using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using userSelectionGen = v0::org::volvocars::climate::UserSelection;
using CC174 = CarConfigParams::CC174_AirQualitySystemType;

namespace {
const int lcfgTimeout = 10;

void setClimaActv(OnOff1 onoff) {
    DEInjector<ClimaActv_info> climaActv;
    climaActv.inject(onoff);
}

void setVehicleMode(UsgModSts1 usgModeSts1, CarModSts1 carModSts1) {
    DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;
    VehModMngtGlbSafe1 drvMode;
    drvMode.UsgModSts = usgModeSts1;
    drvMode.CarModSts1_ = carModSts1;
    driverMode.inject(drvMode);
}
void setVehicleModeError() {
    DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;
    driverMode.error(12);
}

void setClimaActiveError() {
    DEInjector<autosar::ClimaActv_info> climaActv;
    climaActv.error(12);
}
}

namespace climate {

using AutoClimateState = FirstRowGen::AutoClimateState;
using ManualRecircState = FirstRowGen::ManualRecircState;

class RecircTest : public Test {
    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();

  public:
    RecircTest()
        : Test(),
          shareAirQualitySensor({userSelectionGen::StateType::NOT_PRESENT, userSelectionGen::OffOnType::OFF}),
          shareRecircTimer({userSelectionGen::StateType::NOT_PRESENT, userSelectionGen::OffOnType::OFF}),
          shareClimateReset{ClimateResetLogic::ClimateResetEvent::NO_REQ},
          shareMaxDefroster(FirstRowGen::MaxDefrosterState::OFF),
          shareFanLevelFront(FirstRowGen::FanLevelFrontValue::OFF),
          shareAutoClimate(AutoClimateLogic::AutoClimateEvent::NO_REQ),
          sManualRecirc(SettingId::TestSetting1, FirstRowGen::ManualRecircState::OFF, settingsManager),
          sManualRecircDyno(SettingId::TestSetting1, FirstRowGen::ManualRecircState::OFF, settingsManager),
          sManualRecircProxy(sManualRecirc, sManualRecircDyno),
          recirc(FirstRowGen::ManualRecircState::OFF),
          dispatcher(),
          localConfig(),
          carConfig_() {
        ON_CALL(carConfig_, getValue(to_undrl(CC174::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC174::Air_Quality_System)));

        ON_CALL(localConfig, GetIntMock("Climate_Manual_Recirc")).WillByDefault(Return(lcfgTimeout));

        setVehicleMode(UsgModSts1::UsgModInActv, CarModSts1::CarModNorm);
        setClimaActv(OnOff1::Off);
        setAutoClimateSetting(AutoClimateLogic::AutoClimateEvent::NO_REQ);
        shareClimateReset.set(ClimateResetLogic::ClimateResetEvent::NO_REQ);
        setAirQualitySensorSetting(userSelectionGen::StateType::NOT_PRESENT, userSelectionGen::OffOnType::OFF);
        setRecircTimerSetting(userSelectionGen::StateType::NOT_PRESENT, userSelectionGen::OffOnType::OFF);
        setMaxDefrostSetting(FirstRowGen::MaxDefrosterState::DISABLED);

        makeSut();

        setRecircSetting(FirstRowGen::ManualRecircState::DISABLED);
    }

    void makeSut() {
        _sut = std::unique_ptr<ManualRecircLogic>(new ManualRecircLogic(&localConfig,
                                                                        shareRecircTimer,
                                                                        shareAirQualitySensor,
                                                                        sManualRecircProxy,
                                                                        recirc,
                                                                        shareClimateReset,
                                                                        dispatcher,
                                                                        shareFanLevelFront,
                                                                        shareAutoClimate,
                                                                        shareMaxDefroster));
    }

  protected:
    void setFanLevelSetting(FirstRowGen::FanLevelFrontValue level) { shareFanLevelFront.set(level); }

    void setMaxDefrostSetting(FirstRowGen::MaxDefrosterState frost) { shareMaxDefroster.set(frost); }

    void setAutoClimateSetting(AutoClimateLogic::AutoClimateEvent autoClimate) { shareAutoClimate.set(autoClimate); }
    void setClimateResetSetting(ClimateResetLogic::ClimateResetEvent climateReset) {
        shareClimateReset.set(climateReset);
    }
    void setRecircSetting(FirstRowGen::ManualRecircState recirc) {
        sManualRecirc.set(static_cast<FirstRowGen::ManualRecircState::Literal>(recirc.value_));
    }

    void setRecircSettingProxy(FirstRowGen::ManualRecircState recircProxy) {
        sManualRecircProxy.set(static_cast<FirstRowGen::ManualRecircState::Literal>(recircProxy.value_));
    }

    void setRecircTimerSetting(userSelectionGen::StateType state, userSelectionGen::OffOnType onoff) {
        shareRecircTimer.set({state, onoff});
    }

    void setAirQualitySensorSetting(userSelectionGen::StateType state, userSelectionGen::OffOnType onoff) {
        shareAirQualitySensor.set({state, onoff});
    }

    void setRecircDynoSetting(FirstRowGen::ManualRecircState recirc) {
        sManualRecircDyno.overrideDefaultValue(static_cast<FirstRowGen::ManualRecircState::Literal>(recirc.value_));
    }

    userSelectionGen::OffOnSelection onOff;

    NotifiableProperty<userSelectionGen::OffOnSelection> shareAirQualitySensor;
    NotifiableProperty<userSelectionGen::OffOnSelection> shareRecircTimer;
    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> shareClimateReset;
    NotifiableProperty<FirstRowGen::MaxDefrosterState> shareMaxDefroster;
    NotifiableProperty<FirstRowGen::FanLevelFrontValue> shareFanLevelFront;
    NotifiableProperty<AutoClimateLogic::AutoClimateEvent> shareAutoClimate;

    CompatSetting<FirstRowGen::ManualRecircState::Literal, UserScope::USER> sManualRecirc;
    CompatSetting<FirstRowGen::ManualRecircState::Literal, UserScope::NOT_USER_RELATED> sManualRecircDyno;
    SettingsProxy<FirstRowGen::ManualRecircState::Literal, UserScope::USER, UserScope::NOT_USER_RELATED>
            sManualRecircProxy;

    static ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModSts_;
    static ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climateActiveSignal_;
    static ApplicationDataElement::DESender<autosar::HmiHvacRecircCmd_info> hmiHvacRecircCmd_;

    NotifiableProperty<FirstRowGen::ManualRecircState> recirc;
    NiceMock<MockDispatcher> dispatcher;
    NiceMock<vcc::mocks::MockLocalConfigReader> localConfig;
    NiceMock<CarConfigMock> carConfig_;

    std::unique_ptr<ManualRecircLogic> _sut;
};

TEST_F(RecircTest, Constructor_WillInitialize) {
    EXPECT_EQ(FirstRowGen::ManualRecircState::DISABLED, recirc.get());
}

TEST_F(RecircTest, Constructor_WillBeError_WhenVehModError) {
    setVehicleModeError();

    EXPECT_EQ(FirstRowGen::ManualRecircState::SYSTEM_ERROR, recirc.get());
}

TEST_F(RecircTest, Constructor_WillBeError_WhenClimateActiveError) {
    setClimaActiveError();

    EXPECT_EQ(FirstRowGen::ManualRecircState::SYSTEM_ERROR, recirc.get());
}

TEST_F(RecircTest, Constructor_WillBeError_WhenActiveAndClimateActiveError) {
    setVehicleMode(UsgModSts1::UsgModAbdnd, CarModSts1::CarModFcy);

    setClimaActv(OnOff1::Off);

    EXPECT_EQ(FirstRowGen::ManualRecircState::DISABLED, recirc.get());

    setClimaActiveError();

    EXPECT_EQ(FirstRowGen::ManualRecircState::SYSTEM_ERROR, recirc.get());
}

TEST_F(RecircTest, Constructor_WillBeDisabled_WhenNotActive) {
    setVehicleMode(UsgModSts1::UsgModAbdnd, CarModSts1::CarModFcy);

    setClimaActv(OnOff1::Off);

    EXPECT_EQ(FirstRowGen::ManualRecircState::DISABLED, recirc.get());
}

TEST_F(RecircTest, Constructor_WillBeDisabled_WhenActiveAndFanOff) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::OFF);
    setMaxDefrostSetting(FirstRowGen::MaxDefrosterState::OFF);

    EXPECT_EQ(FirstRowGen::ManualRecircState::DISABLED, recirc.get());
}

TEST_F(RecircTest, Constructor_WillBeDisabled_WhenActiveAndMaxDefrostOn) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setMaxDefrostSetting(FirstRowGen::MaxDefrosterState::ON);

    EXPECT_EQ(FirstRowGen::ManualRecircState::DISABLED, recirc.get());
}

TEST_F(RecircTest, Constructor_WillBeOff_WhenEnabledAndRecircOff) {
    DESink<HmiHvacRecircCmd_info> hmiHvacRecircCmd;

    auto outValue = HmiHvacRecircCmd::Aut;
    hmiHvacRecircCmd.subscribe([&hmiHvacRecircCmd, &outValue]() { outValue = hmiHvacRecircCmd.get().value(); });

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setMaxDefrostSetting(FirstRowGen::MaxDefrosterState::OFF);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircSetting(FirstRowGen::ManualRecircState::OFF);

    auto expectedValue = FirstRowGen::ManualRecircState::OFF;

    EXPECT_EQ(expectedValue, recirc.get());
    EXPECT_EQ(HmiHvacRecircCmd::Aut, outValue);
}

TEST_F(RecircTest, Constructor_WillBeOn_WhenEnabledAndRecircOn) {
    DESink<HmiHvacRecircCmd_info> hmiHvacRecircCmd;

    auto outValue = HmiHvacRecircCmd::Aut;
    hmiHvacRecircCmd.subscribe([&hmiHvacRecircCmd, &outValue]() { outValue = hmiHvacRecircCmd.get().value(); });

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);

    auto expectedValue = FirstRowGen::ManualRecircState::ON;

    EXPECT_EQ(expectedValue, recirc.get());
    EXPECT_EQ(HmiHvacRecircCmd::RecircFull, outValue);
}

TEST_F(RecircTest, Constructor_WillNotProcess_WhenActiveHasNotChanged) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    DESink<HmiHvacRecircCmd_info> hmiHvacRecircCmd;

    auto nOfUpdates = 0;
    hmiHvacRecircCmd.subscribe([&hmiHvacRecircCmd, &nOfUpdates]() { nOfUpdates++; });

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    EXPECT_EQ(1, nOfUpdates);
}

TEST_F(RecircTest, ManRecirc_WillProcess_WhenCarMod) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);

    DESink<HmiHvacRecircCmd_info> hmiHvacRecircCmd;

    auto nOfUpdates = 0;
    hmiHvacRecircCmd.subscribe([&hmiHvacRecircCmd, &nOfUpdates]() { nOfUpdates++; });

    EXPECT_EQ(1, nOfUpdates);  // Init
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    EXPECT_EQ(2, nOfUpdates);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(3, nOfUpdates);
}

TEST_F(RecircTest, RequestRecircState_WillBeOn_WhenEnabledAndRecircOn) {
    DESink<HmiHvacRecircCmd_info> hmiHvacRecircCmd;

    auto outValue = HmiHvacRecircCmd::Aut;
    hmiHvacRecircCmd.subscribe([&hmiHvacRecircCmd, &outValue]() { outValue = hmiHvacRecircCmd.get().value(); });

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    _sut->requestManualRecirc(FirstRowGen::ManualRecircRequest::ON);

    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());
    EXPECT_EQ(HmiHvacRecircCmd::RecircFull, outValue);
}

TEST_F(RecircTest, RequestRecircState_WillBeOff_WhenEnabledAndRecircOff) {
    DESink<HmiHvacRecircCmd_info> hmiHvacRecircCmd;

    auto outValue = HmiHvacRecircCmd::Aut;
    hmiHvacRecircCmd.subscribe([&hmiHvacRecircCmd, &outValue]() { outValue = hmiHvacRecircCmd.get().value(); });

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);

    _sut->requestManualRecirc(FirstRowGen::ManualRecircRequest::OFF);

    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
    EXPECT_EQ(HmiHvacRecircCmd::Aut, outValue);
}

TEST_F(RecircTest, RequestRecircState_WillBeOffAndAutWithAirQly_WhenEnabledAndRecircOffAndAirQualitySensorOn) {
    DESink<HmiHvacRecircCmd_info> hmiHvacRecircCmd;

    auto outValue = HmiHvacRecircCmd::Aut;
    hmiHvacRecircCmd.subscribe([&hmiHvacRecircCmd, &outValue]() { outValue = hmiHvacRecircCmd.get().value(); });

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setMaxDefrostSetting(FirstRowGen::MaxDefrosterState::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::OFF);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);

    auto expectedValue = FirstRowGen::ManualRecircState::OFF;

    EXPECT_EQ(expectedValue, recirc.get());
    EXPECT_EQ(HmiHvacRecircCmd::AutWithAirQly, outValue);
}

TEST_F(RecircTest, RequestRecircState_WillBeOffAndAut_WhenEnabledAndRecircOffAndCarConfigNotOkAndAirQualitySensorOn) {
    EXPECT_CALL(carConfig_, getValue(to_undrl(CC174::ParamNumber)))
            .WillRepeatedly(Return(to_undrl(CC174::InvalidValue)));

    makeSut();

    DESink<HmiHvacRecircCmd_info> hmiHvacRecircCmd;

    auto outValue = HmiHvacRecircCmd::Aut;
    hmiHvacRecircCmd.subscribe([&hmiHvacRecircCmd, &outValue]() { outValue = hmiHvacRecircCmd.get().value(); });

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircSetting(FirstRowGen::ManualRecircState::OFF);

    auto expectedValue = FirstRowGen::ManualRecircState::OFF;

    EXPECT_EQ(expectedValue, recirc.get());
    EXPECT_EQ(HmiHvacRecircCmd::Aut, outValue);
}

TEST_F(RecircTest, RequestRecircState_WillBeOff_WhenOnAndAutoClimateActivated) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);

    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());

    setAutoClimateSetting(AutoClimateLogic::AutoClimateEvent::ACTIVATED);

    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, RequestRecircState_WillBeOff_WhenOnAndClimateResetActivated) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);

    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());

    setClimateResetSetting(ClimateResetLogic::ClimateResetEvent::ACTIVATED);

    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, RequestRecircState_WillTurnOffAfterTimeout_WhenOnAndTimerOn) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircTimerSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);

    std::function<void(void)> func;

    // Convert from minutes to milliseconds
    EXPECT_CALL(dispatcher, Start(std::chrono::milliseconds{lcfgTimeout * 60 * 1000}, _, _))
            .WillOnce(SaveArg<1>(&func));

    _sut->requestManualRecirc(FirstRowGen::ManualRecircRequest::ON);

    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());

    if (func) func();

    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, RequestRecircState_WillNotTurnOffAfterTimeout_WhenOnAndTimerOff) {
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircTimerSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::OFF);

    std::function<void(void)> func;

    EXPECT_CALL(dispatcher, Start(_, _, _)).Times(0);

    _sut->requestManualRecirc(FirstRowGen::ManualRecircRequest::ON);
}

TEST_F(RecircTest, Will_use_dyno_setting_off_when_reciving_carmode_dyno) {
    makeSut();

    setRecircDynoSetting(FirstRowGen::ManualRecircState::OFF);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircTimerSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);

    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, Will_use_dyno_setting_off_when_carmode_dyno_at_start) {
    setRecircDynoSetting(FirstRowGen::ManualRecircState::OFF);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircTimerSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);

    makeSut();

    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, Will_use_dyno_setting_on_when_reciving_carmode_dyno) {
    makeSut();

    setRecircDynoSetting(FirstRowGen::ManualRecircState::ON);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircTimerSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);

    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());
}

TEST_F(RecircTest, Will_use_dyno_setting_on_when_carmode_dyno_at_start) {
    setRecircDynoSetting(FirstRowGen::ManualRecircState::ON);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setAirQualitySensorSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::ON);
    setRecircTimerSetting(userSelectionGen::StateType::AVAILABLE, userSelectionGen::OffOnType::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);

    makeSut();

    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());
}

TEST_F(RecircTest, ManRecirc_WillEnable_WhenRequestInCarModDyno) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircDynoSetting(FirstRowGen::ManualRecircState::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::OFF);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());  // init

    setRecircSettingProxy(FirstRowGen::ManualRecircState::ON);
    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());

    setRecircSettingProxy(FirstRowGen::ManualRecircState::OFF);
    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, ManRecirc_WillEnable_WhenRequestInCarModDyno_DifferentInit) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircDynoSetting(FirstRowGen::ManualRecircState::ON);
    setRecircSetting(FirstRowGen::ManualRecircState::OFF);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());
}

TEST_F(RecircTest, ManRecirc_WillNotEnable_WhenRequestInCarModDyno_DifferentInit) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircDynoSetting(FirstRowGen::ManualRecircState::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, ManRecirc_WillEnable_WhenRequestInCarModNorm) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircDynoSetting(FirstRowGen::ManualRecircState::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::OFF);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());

    setRecircSetting(FirstRowGen::ManualRecircState::ON);
    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());

    setRecircSetting(FirstRowGen::ManualRecircState::OFF);
    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, ManRecirc_WillEnable_WhenRequestInCarModNorm_DifferentInit) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircDynoSetting(FirstRowGen::ManualRecircState::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());
}

TEST_F(RecircTest, ManRecirc_WillNotEnable_WhenRequestInCarModNorm_DifferentInit) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircDynoSetting(FirstRowGen::ManualRecircState::ON);
    setRecircSetting(FirstRowGen::ManualRecircState::OFF);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());
}

TEST_F(RecircTest, ManRecirc_SwitchingBetween_CarModeNormalAndDyno) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircDynoSetting(FirstRowGen::ManualRecircState::OFF);
    setRecircSetting(FirstRowGen::ManualRecircState::ON);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());
}

TEST_F(RecircTest, ManRecirc_SwitchingBetween_CarModeDynoAndNormal) {
    setClimaActv(OnOff1::On);
    setFanLevelSetting(FirstRowGen::FanLevelFrontValue::LVL1);
    setRecircDynoSetting(FirstRowGen::ManualRecircState::ON);
    setRecircSetting(FirstRowGen::ManualRecircState::OFF);
    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModNorm);
    EXPECT_EQ(FirstRowGen::ManualRecircState::OFF, recirc.get());

    setVehicleMode(UsgModSts1::UsgModDrvg, CarModSts1::CarModDyno);
    EXPECT_EQ(FirstRowGen::ManualRecircState::ON, recirc.get());
}
}
