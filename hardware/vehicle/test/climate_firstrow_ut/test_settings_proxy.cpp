/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "settings_proxy.h"

#include <ECD_dataelement.h>
#include <gtest/gtest.h>
#include "mock_settingsmanager.h"

namespace TestSettingsProxy {

enum class EnumClass { FIRST, SECOND, THIRD, FORTH, FIFTH, SIXTH };

class SettingsProxyTest : public ::testing::Test {
  public:
    SettingsProxyTest()
        : setting{SettingId::TestSetting1, EnumClass::FIRST, settingsManager},
          settingsDyno{SettingId::TestSetting2, EnumClass::FIFTH, settingsManager}

    {
        setting.set(EnumClass::FIRST);
        settingsDyno.set(EnumClass::FIFTH);
    }

    ~SettingsProxyTest() {}

    void setVehicleMode(autosar::UsgModSts1 usgModeSts1, autosar::CarModSts1 carModSts1) {
        autosar::VehModMngtGlbSafe1 drvMode;
        drvMode.UsgModSts = usgModeSts1;
        drvMode.CarModSts1_ = carModSts1;
        driverMode.inject(drvMode);
    }

    android::sp<SettingsFramework::SettingsManager> settingsManager = new MockSettingsManager();
    CompatSetting<EnumClass, SettingsFramework::UserScope::USER> setting;

    CompatSetting<EnumClass, SettingsFramework::UserScope::USER> settingsDyno;
    ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;
};

TEST_F(SettingsProxyTest, InitializeNormalMode) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    SettingsProxy<EnumClass, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::USER> sut(setting,
                                                                                                         settingsDyno);

    EXPECT_EQ(EnumClass::FIRST, sut.get());
}

TEST_F(SettingsProxyTest, InitializeDynoMode) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    SettingsProxy<EnumClass, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::USER> sut(setting,
                                                                                                         settingsDyno);

    EXPECT_EQ(EnumClass::FIFTH, sut.get());
}

TEST_F(SettingsProxyTest, TestChangeValueInDynoMode) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    SettingsProxy<EnumClass, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::USER> sut(setting,
                                                                                                         settingsDyno);

    sut.set(EnumClass::SECOND);

    EXPECT_EQ(EnumClass::SECOND, sut.get());
}

TEST_F(SettingsProxyTest, TestChangeValueInDynoModeAndGoBackToNormal) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);
    SettingsProxy<EnumClass, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::USER> sut(setting,
                                                                                                         settingsDyno);

    sut.set(EnumClass::SECOND);

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(EnumClass::FIRST, sut.get());
}

TEST_F(SettingsProxyTest, TestChangeBackAndForthBetweenNormalModeAndDynoModeAndChangeValue) {
    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);
    SettingsProxy<EnumClass, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::USER> sut(setting,
                                                                                                         settingsDyno);

    EXPECT_EQ(EnumClass::FIRST, sut.get());

    sut.set(EnumClass::SECOND);
    EXPECT_EQ(EnumClass::SECOND, sut.get());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);

    EXPECT_EQ(EnumClass::FIFTH, sut.get());

    sut.set(EnumClass::FORTH);
    EXPECT_EQ(EnumClass::FORTH, sut.get());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(EnumClass::SECOND, sut.get());

    sut.set(EnumClass::THIRD);
    EXPECT_EQ(EnumClass::THIRD, sut.get());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModDyno);

    // It should now use the defaultValue
    EXPECT_EQ(EnumClass::FIFTH, sut.get());

    setVehicleMode(autosar::UsgModSts1::UsgModDrvg, autosar::CarModSts1::CarModNorm);

    EXPECT_EQ(EnumClass::THIRD, sut.get());
}
}
