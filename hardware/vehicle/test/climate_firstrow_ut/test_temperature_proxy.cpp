/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "temperature_proxy.h"

#include "carconfig_mock.h"
#include "enum_helper.h"

#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <tuple>
#include "mock_settingsmanager.h"

namespace TestTemperatureProxy {
using namespace testing;

using CC8 = CarConfigParams::CC8_SteeringWheelPositionType;
using CC175 = CarConfigParams::CC175_HvacVariantsType;

class TemperatureProxyTest : public Test, public testing::WithParamInterface<std::tuple<CC175, CC8>> {
  public:
    TemperatureProxyTest()
        : storedDriverTemp_{0},
          storedDriverTempHiLoN_{autosar::HmiCmptmtTSpSpcl::Norm},
          storedPassengerTemp_{0},
          storedPassengerTempHiLoN_{autosar::HmiCmptmtTSpSpcl::Norm},
          driverTemp_{0},
          driverTempHiLoN_{autosar::HmiCmptmtTSpSpcl::Norm},
          passengerTemp_{0},
          passengerTempHiLoN_{autosar::HmiCmptmtTSpSpcl::Norm},
          maxDefroster_{FirstRowGen::MaxDefrosterState::DISABLED},
          sTempDriver_{SettingId::TestSetting1, 22, settingsManager},
          sTempDynoDriver_{SettingId::TestSetting2, 18, settingsManager},
          sTemperatureDriverProxy_{sTempDriver_, sTempDynoDriver_},
          sTempPassenger_{SettingId::TestSetting3, 22, settingsManager},
          sTempDynoPassenger_{SettingId::TestSetting4, 18, settingsManager},
          sTemperaturePassengerProxy_{sTempPassenger_, sTempDynoPassenger_},
          sTempDriverHiLoN_{SettingId::TestSetting4, autosar::HmiCmptmtTSpSpcl::Norm, settingsManager},
          sTempDynoDriverHiLoN_{SettingId::TestSetting5, autosar::HmiCmptmtTSpSpcl::Norm, settingsManager},
          sTemperatureDriverHiLoNProxy_{sTempDriverHiLoN_, sTempDynoDriverHiLoN_},
          sTempPassengerHiLoN_{SettingId::TestSetting5, autosar::HmiCmptmtTSpSpcl::Norm, settingsManager},
          sTempDynoPassengerHiLoN_{SettingId::TestSetting6, autosar::HmiCmptmtTSpSpcl::Norm, settingsManager},
          sTemperaturePassengerHiLoNProxy_{sTempPassengerHiLoN_, sTempDynoPassengerHiLoN_} {
        auto ccParams = GetParam();

        ON_CALL(carConfig_, getValue(to_undrl(CC175::ParamNumber)))
                .WillByDefault(Return(to_undrl(std::get<0>(ccParams))));
        ON_CALL(carConfig_, getValue(to_undrl(CC8::ParamNumber)))
                .WillByDefault(Return(to_undrl(std::get<1>(ccParams))));

        setCarMod(autosar::CarModSts1::CarModNorm);

        resetSignals();
        sTempDriver_.set(22);
        sTempDriverHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
        sTempPassenger_.set(22);
        sTempPassengerHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
    }

    void SetUp() { createSut(); }

  protected:
    void createSut() {
        sut_ = std::unique_ptr<TemperatureProxy>(new TemperatureProxy(
                storedDriverTemp_, storedDriverTempHiLoN_, storedPassengerTemp_, storedPassengerTempHiLoN_, driverTemp_,
                driverTempHiLoN_, passengerTemp_, passengerTempHiLoN_, maxDefroster_, sTemperatureDriverProxy_,
                sTemperatureDriverHiLoNProxy_, sTemperaturePassengerProxy_, sTemperaturePassengerHiLoNProxy_,
                hmiCmptmtTSp));
    }

    static void setCarMod(autosar::CarModSts1 carModSts1) {
        autosar::VehModMngtGlbSafe1 mode;
        mode.CarModSts1_ = carModSts1;
        vehModMngtGlbSafe1_.inject(mode);
    }

    void resetSignals() {
        storedDriverTemp_.set(0.0);
        storedDriverTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
        storedPassengerTemp_.set(0.0);
        storedPassengerTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
        driverTemp_.set(0.0);
        driverTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
        passengerTemp_.set(0.0);
        passengerTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Norm);
        maxDefroster_.set(FirstRowGen::MaxDefrosterState::DISABLED);
    }

  protected:
    NiceMock<CarConfigMock> carConfig_;

    autosar::HmiCmptmtTSp hmiCmptmtTSp;

    static ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1_;
    static ECDDataElement::DESink<autosar::HmiCmptmtTSp_info> hmiCmptmtTSp_;

    NotifiableProperty<double> storedDriverTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> storedDriverTempHiLoN_;
    NotifiableProperty<double> storedPassengerTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> storedPassengerTempHiLoN_;
    NotifiableProperty<double> driverTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> driverTempHiLoN_;
    NotifiableProperty<double> passengerTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> passengerTempHiLoN_;
    NotifiableProperty<FirstRowGen::MaxDefrosterState> maxDefroster_;

    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    CompatSetting<double, SettingsFramework::UserScope::USER> sTempDriver_;
    CompatSetting<double, SettingsFramework::UserScope::NOT_USER_RELATED> sTempDynoDriver_;
    SettingsProxy<double, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>
            sTemperatureDriverProxy_;

    CompatSetting<double, SettingsFramework::UserScope::NOT_USER_RELATED> sTempPassenger_;
    CompatSetting<double, SettingsFramework::UserScope::NOT_USER_RELATED> sTempDynoPassenger_;
    SettingsProxy<double, SettingsFramework::UserScope::NOT_USER_RELATED,
                  SettingsFramework::UserScope::NOT_USER_RELATED>
            sTemperaturePassengerProxy_;

    CompatSetting<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::USER> sTempDriverHiLoN_;
    CompatSetting<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::NOT_USER_RELATED> sTempDynoDriverHiLoN_;
    SettingsProxy<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::USER,
                  SettingsFramework::UserScope::NOT_USER_RELATED>
            sTemperatureDriverHiLoNProxy_;

    CompatSetting<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::NOT_USER_RELATED> sTempPassengerHiLoN_;
    CompatSetting<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::NOT_USER_RELATED> sTempDynoPassengerHiLoN_;
    SettingsProxy<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::NOT_USER_RELATED,
                  SettingsFramework::UserScope::NOT_USER_RELATED>
            sTemperaturePassengerHiLoNProxy_;

    std::unique_ptr<TemperatureProxy> sut_;
};

ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> TemperatureProxyTest::vehModMngtGlbSafe1_;
ECDDataElement::DESink<autosar::HmiCmptmtTSp_info> TemperatureProxyTest::hmiCmptmtTSp_;

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

class TemperatureProxyCCSingleTest : public TemperatureProxyTest {};

TEST_P(TemperatureProxyCCSingleTest, Constructor_WillInitializeStoredDriverTempFromSetting) {
    auto const expected = 25.4;
    sTempDriver_.set(expected);

    sut_.reset();
    createSut();

    EXPECT_DOUBLE_EQ(expected, storedDriverTemp_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WillInitializeStoredDriverTempHiLoNFromSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;
    sTempDriverHiLoN_.set(expected);

    sut_.reset();
    createSut();

    EXPECT_EQ(expected, storedDriverTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WillInitializeLeftFromSetting) {
    auto const expected = 25.4;
    sTempDriver_.set(expected);

    sut_.reset();
    createSut();

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();
    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe);
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WillInitializeLeftHiLoNFromSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;
    sTempDriverHiLoN_.set(expected);

    sut_.reset();
    createSut();

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();
    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe);
}

TEST_P(TemperatureProxyCCSingleTest,
       Constructor_WhenDriverTempChangedAndMaxDefrosterOn_WillUpdateLeftAndNotUpdateSetting) {
    auto const expected = 24.0;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    driverTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe);
    EXPECT_DOUBLE_EQ(22, sTempDriver_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenDriverTempChangedAndCarModDyno_WillUpdateLeftAndNotUpdateSetting) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    auto const expected = 24.0;

    driverTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe);
    EXPECT_DOUBLE_EQ(22, sTempDriver_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenDriverTempChanged_WillUpdateLeftAndUpdateSetting) {
    auto const expected = 24.0;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    driverTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe);
    EXPECT_DOUBLE_EQ(expected, sTempDriver_.get());
}

TEST_P(TemperatureProxyCCSingleTest,
       Constructor_WhenDriverTempHiLoNChangedAndMaxDefrosterOn_WillUpdateLeftAndNotUpdateSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    driverTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, sTempDriverHiLoN_.get());
}

TEST_P(TemperatureProxyCCSingleTest,
       Constructor_WhenDriverTempHiLoNChangedAndCarModDyno_WillUpdateLeftAndNotUpdateSetting) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    driverTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, sTempDriverHiLoN_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenDriverTempHiLoNChanged_WillUpdateLeftAndUpdateSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    driverTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe);
    EXPECT_EQ(expected, sTempDriverHiLoN_.get());
}

TEST_P(TemperatureProxyCCSingleTest,
       Constructor_WhenSettingTempDriverChangedAndCarModDyno_WillNotUpdateStoredDriverTemp) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    sTempDriver_.set(24);

    EXPECT_DOUBLE_EQ(sTempDynoDriver_.get(), storedDriverTemp_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenSettingTempDriverChanged_WillUpdateStoredDriverTemp) {
    auto const expected = 24.0;

    sTempDriver_.set(expected);

    EXPECT_DOUBLE_EQ(expected, storedDriverTemp_.get());
}

TEST_P(TemperatureProxyCCSingleTest,
       Constructor_WhenSettingDriverHiLoNChangedAndCarModDyno_WillNotUpdateStoredDriverTempHiLoN) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    sTempDriverHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    EXPECT_EQ(sTempDynoDriverHiLoN_.get(), storedDriverTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenSettingDriverHiLoNChanged_WillUpdateStoredDriverTempHiLoN) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    sTempDriverHiLoN_.set(expected);

    EXPECT_EQ(expected, storedDriverTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenCarModChangedToDyno_WillUpdateStoredDriver) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    EXPECT_DOUBLE_EQ(sTempDynoDriver_.get(), storedDriverTemp_.get());
    EXPECT_EQ(sTempDynoDriverHiLoN_.get(), storedDriverTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenCarModChangedFromDyno_WillUpdateStoredDriver) {
    setCarMod(autosar::CarModSts1::CarModDyno);
    setCarMod(autosar::CarModSts1::CarModNorm);

    EXPECT_DOUBLE_EQ(sTempDriver_.get(), storedDriverTemp_.get());
    EXPECT_EQ(sTempDriverHiLoN_.get(), storedDriverTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenPassengerTempChanged_WillNotUpdate) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    passengerTemp_.set(24.0);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(22, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe);
    EXPECT_DOUBLE_EQ(22, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi);
    EXPECT_DOUBLE_EQ(22, sTempDriver_.get());
    EXPECT_DOUBLE_EQ(22, sTempPassenger_.get());
}

TEST_P(TemperatureProxyCCSingleTest, Constructor_WhenPassengerTempHiLoNChanged_WillNotUpdate) {
    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    passengerTempHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, sTempDriverHiLoN_.get());
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, sTempPassengerHiLoN_.get());
}

INSTANTIATE_TEST_CASE_P(, TemperatureProxyCCSingleTest, Combine(Values(CC175::HVAC_Small_1_zone, CC175::Manual_AC),
                                                                Values(CC8::Left_Hand_Drive, CC8::Right_Hand_Drive)));

class TemperatureProxyCCLeftTest : public TemperatureProxyTest {};

TEST_P(TemperatureProxyCCLeftTest, Constructor_WillInitializeStoredPassengerTempFromSetting) {
    auto const expected = 25.4;
    sTempPassenger_.set(expected);

    sut_.reset();
    createSut();

    EXPECT_DOUBLE_EQ(expected, storedPassengerTemp_.get());
}

TEST_P(TemperatureProxyCCLeftTest, Constructor_WillInitializeStoredPassengerTempHiLoNFromSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;
    sTempPassengerHiLoN_.set(expected);

    sut_.reset();
    createSut();

    EXPECT_EQ(expected, storedPassengerTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCLeftTest, Constructor_WillInitializeRightFromSetting) {
    auto const expected = 25.4;
    sTempPassenger_.set(expected);

    sut_.reset();
    createSut();

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();
    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi);
}

TEST_P(TemperatureProxyCCLeftTest, Constructor_WillInitializeRightHiLoNFromSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;
    sTempPassengerHiLoN_.set(expected);

    sut_.reset();
    createSut();

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();
    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi);
}

TEST_P(TemperatureProxyCCLeftTest,
       Constructor_WhenPassengerTempChangedAndMaxDefrosterOn_WillUpdateRightAndNotUpdateSetting) {
    auto const expected = 24.0;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    passengerTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi);
    EXPECT_DOUBLE_EQ(22, sTempPassenger_.get());
}

TEST_P(TemperatureProxyCCLeftTest,
       Constructor_WhenPassengerTempChangedAndCarModDyno_WillUpdateRightAndNotUpdateSetting) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    auto const expected = 24.0;

    passengerTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi);
    EXPECT_DOUBLE_EQ(22, sTempPassenger_.get());
}

TEST_P(TemperatureProxyCCLeftTest, Constructor_WhenPassengerTempChanged_WillUpdateRightAndUpdateSetting) {
    auto const expected = 24.0;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    passengerTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi);
    EXPECT_DOUBLE_EQ(expected, sTempPassenger_.get());
}

TEST_P(TemperatureProxyCCLeftTest,
       Constructor_WhenPassengerTempHiLoNChangedAndMaxDefrosterOn_WillUpdateRightAndNotUpdateSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    passengerTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, sTempPassengerHiLoN_.get());
}

TEST_P(TemperatureProxyCCLeftTest,
       Constructor_WhenPassengerTempHiLoNChangedAndCarModDyno_WillUpdateRightAndNotUpdateSetting) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    passengerTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, sTempPassengerHiLoN_.get());
}

TEST_P(TemperatureProxyCCLeftTest, Constructor_WhenPassengerTempHiLoNChanged_WillUpdateRightAndUpdateSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    passengerTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Hi, sTempPassengerHiLoN_.get());
}

TEST_P(TemperatureProxyCCLeftTest,
       Constructor_WhenSettingTempPassengerChangedAndCarModDyno_WillNotUpdateStoredPassengerTemp) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    sTempPassenger_.set(24);

    EXPECT_DOUBLE_EQ(sTempDynoPassenger_.get(), storedPassengerTemp_.get());
}

TEST_P(TemperatureProxyCCLeftTest, Constructor_WhenSettingTempPassengerChanged_WillUpdateStoredPassengerTemp) {
    auto const expected = 24.0;

    sTempPassenger_.set(expected);

    EXPECT_DOUBLE_EQ(expected, storedPassengerTemp_.get());
}

TEST_P(TemperatureProxyCCLeftTest,
       Constructor_WhenSettingTempPassengerHiLoNChangedAndCarModDyno_WillNotUpdateStoredPassengerTempHiLoN) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    sTempPassengerHiLoN_.set(autosar::HmiCmptmtTSpSpcl::Hi);

    EXPECT_EQ(sTempDynoPassengerHiLoN_.get(), storedPassengerTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCLeftTest,
       Constructor_WhenSettingTempPassengerHiLoNChanged_WillUpdateStoredPassengerTempHiLoN) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    sTempPassengerHiLoN_.set(expected);

    EXPECT_EQ(expected, storedPassengerTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCLeftTest, Constructor_WhenCarModChangedToDyno_WillUpdateStoredPassenger) {
    setCarMod(autosar::CarModSts1::CarModDyno);

    EXPECT_DOUBLE_EQ(sTempDynoPassenger_.get(), storedPassengerTemp_.get());
    EXPECT_EQ(sTempDynoPassengerHiLoN_.get(), storedPassengerTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCLeftTest, Constructor_WhenCarModChangedFromDyno_WillUpdateStoredPassenger) {
    setCarMod(autosar::CarModSts1::CarModDyno);
    setCarMod(autosar::CarModSts1::CarModNorm);

    EXPECT_DOUBLE_EQ(sTempPassenger_.get(), storedPassengerTemp_.get());
    EXPECT_EQ(sTempPassengerHiLoN_.get(), storedPassengerTempHiLoN_.get());
}

INSTANTIATE_TEST_CASE_P(, TemperatureProxyCCLeftTest,
                        Combine(Values(CC175::Electronic_Climate_Control_2_zone,
                                       CC175::Electronic_Climate_Control_4_zone, CC175::HVAC_Small_2_zone),
                                Values(CC8::Left_Hand_Drive)));

class TemperatureProxyCCRightTest : public TemperatureProxyTest {};

TEST_P(TemperatureProxyCCRightTest, Constructor_WillInitializeStoredPassengerTempFromSetting) {
    auto const expected = 25.4;
    sTempPassenger_.set(expected);

    sut_.reset();
    createSut();

    EXPECT_DOUBLE_EQ(expected, storedPassengerTemp_.get());
}

TEST_P(TemperatureProxyCCRightTest, Constructor_WillInitializeStoredPassengerTempHiLoNFromSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;
    sTempPassengerHiLoN_.set(expected);

    sut_.reset();
    createSut();

    EXPECT_EQ(expected, storedPassengerTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCRightTest, Constructor_WillInitializeLeftFromSetting) {
    auto const expected = 25.4;
    sTempPassenger_.set(expected);

    sut_.reset();
    createSut();

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();
    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe);
}

TEST_P(TemperatureProxyCCRightTest, Constructor_WillInitializeLeftHiLoNFromSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;
    sTempPassengerHiLoN_.set(expected);

    sut_.reset();
    createSut();

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();
    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe);
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenDriverTempChangedAndMaxDefrosterOn_WillUpdateRightAndNotUpdateSetting) {
    auto const expected = 24.0;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    driverTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi);
    EXPECT_DOUBLE_EQ(22, sTempDriver_.get());
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenDriverTempChangedAndMaxDefrosterOff_WillUpdateRightAndUpdateSetting) {
    auto const expected = 24.0;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    driverTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi);
    EXPECT_DOUBLE_EQ(expected, sTempDriver_.get());
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenDriverTempHiLoNChangedAndMaxDefrosterOff_WillUpdateRightAndUpdateSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    driverTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Hi, sTempDriverHiLoN_.get());
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenDriverTempHiLoNChangedAndMaxDefrosterOn_WillUpdateRightAndNotUpdateSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    driverTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, sTempDriverHiLoN_.get());
}

TEST_P(TemperatureProxyCCRightTest, Constructor_WhenSettingTempDriverChanged_WillUpdateStoredDriverTemp) {
    auto const expected = 24.0;

    sTempDriver_.set(expected);

    EXPECT_DOUBLE_EQ(expected, storedDriverTemp_.get());
}

TEST_P(TemperatureProxyCCRightTest, Constructor_WhenSettingDriverTempHiLoNChanged_WillUpdateStoredDriverTempHiLoN) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    sTempDriverHiLoN_.set(expected);

    EXPECT_EQ(expected, storedDriverTempHiLoN_.get());
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenPassengerTempChangedAndMaxDefrosterOn_WillUpdateLeftAndNotUpdateSetting) {
    auto const expected = 24.0;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    passengerTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe);
    EXPECT_DOUBLE_EQ(22, sTempPassenger_.get());
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenPassengerTempChangedAndMaxDefrosterOff_WillUpdateLeftAndUpdateSetting) {
    auto const expected = 24.0;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    passengerTemp_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_DOUBLE_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe);
    EXPECT_DOUBLE_EQ(expected, sTempPassenger_.get());
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenPassengerTempHiLoNChangedAndMaxDefrosterOff_WillUpdateLeftAndUpdateSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::OFF);
    passengerTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Hi, sTempPassengerHiLoN_.get());
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenPassengerTempHiLoNChangedAndMaxDefrosterOn_WillUpdateLeftAndNotUpdateSetting) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    maxDefroster_.set(FirstRowGen::MaxDefrosterState::ON);
    passengerTempHiLoN_.set(expected);

    auto const hmiCmptmtTSp = hmiCmptmtTSp_.get().value();

    EXPECT_EQ(expected, hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe);
    EXPECT_EQ(autosar::HmiCmptmtTSpSpcl::Norm, sTempPassengerHiLoN_.get());
}

TEST_P(TemperatureProxyCCRightTest, Constructor_WhenSettingTempPassengerChanged_WillUpdateStoredPassengerTemp) {
    auto const expected = 24.0;

    sTempPassenger_.set(expected);

    EXPECT_DOUBLE_EQ(expected, storedPassengerTemp_.get());
}

TEST_P(TemperatureProxyCCRightTest,
       Constructor_WhenSettingTempPassengerHiLoNChanged_WillUpdateStoredPassengerTempHiLoN) {
    auto const expected = autosar::HmiCmptmtTSpSpcl::Hi;

    sTempPassengerHiLoN_.set(expected);

    EXPECT_EQ(expected, storedPassengerTempHiLoN_.get());
}

INSTANTIATE_TEST_CASE_P(, TemperatureProxyCCRightTest,
                        Combine(Values(CC175::Electronic_Climate_Control_2_zone,
                                       CC175::Electronic_Climate_Control_4_zone, CC175::HVAC_Small_2_zone),
                                Values(CC8::Right_Hand_Drive)));
}
