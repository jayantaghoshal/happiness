/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <carconfig.h>
#include <cc_parameterlist.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "notifiable_property.h"

#include "electric_defroster_windscreen_popup_logic.h"

namespace TestElectricDefrosterWindscreenPopup {

using namespace ApplicationDataElement;
using namespace autosar;
using namespace ECDDataElement;
using namespace testing;

using EDS = ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState;

class ElectricDefrosterWindscreenPopUpTest : public ::testing::Test {
  public:
    ElectricDefrosterWindscreenPopUpTest()
        : shareWindscreenPopupAttribute__(
                  ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::OFF),
          shareElectricDefrosterWindscreen_(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF) {
        setHmiDefrostStatus(ActrDefrstSts::Off);
        resetSignals();
    }

    ~ElectricDefrosterWindscreenPopUpTest() { resetSignals(); }

    NotifiableProperty<ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState>
            shareWindscreenPopupAttribute__;
    NotifiableProperty<ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState>
            shareElectricDefrosterWindscreen_;

    DEInjector<autosar::HmiDefrstElecSts_info> hmiDefrosterStatus;

    void setElectricDefrosterWindscreen(
            ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState windscreenPopup) {
        shareElectricDefrosterWindscreen_.set(windscreenPopup);
    }

    void setHmiDefrostStatus(ActrDefrstSts newStatus, bool signalError = false) {
        HmiDefrstElecSts status;
        status.Mirrr = ActrDefrstSts::Off;
        status.Re = ActrDefrstSts::Off;
        status.Frnt = newStatus;
        if (!signalError) {
            hmiDefrosterStatus.inject(status);
        } else {
            hmiDefrosterStatus.error(1);
        }
    }

    void resetSignals() { setHmiDefrostStatus(ActrDefrstSts::Off); }
};

// Initialisation
TEST_F(ElectricDefrosterWindscreenPopUpTest, initialisation) {
    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);

    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::OFF,
              shareWindscreenPopupAttribute__.get().value_);
}

TEST_F(ElectricDefrosterWindscreenPopUpTest, requestPopUpOnAndOff) {
    setHmiDefrostStatus(autosar::ActrDefrstSts::NotAvailable);

    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);

    edwp.request(OnOff::On);

    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::ON,
              shareWindscreenPopupAttribute__.get().value_);

    edwp.requestElectricDefrosterWindscreenPopup(FirstRowGen::ElectricDefrosterPopupRequest::OFF);
    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::OFF,
              shareWindscreenPopupAttribute__.get().value_);
}

TEST_F(ElectricDefrosterWindscreenPopUpTest, requestElectricDefrosterWindscreenOnAndOff) {
    setHmiDefrostStatus(autosar::ActrDefrstSts::NotAvailable);

    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);

    edwp.request(OnOff::Off);

    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::OFF,
              shareWindscreenPopupAttribute__.get().value_);

    edwp.requestElectricDefrosterWindscreen(FirstRowGen::ElectricDefrosterWindscreenRequest::ON);

    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::ON,
              shareWindscreenPopupAttribute__.get().value_);
}

// HMI Defrost Front status flexray signal as Limited
TEST_F(ElectricDefrosterWindscreenPopUpTest, HMIDefrostFrontStsasLimited) {
    setHmiDefrostStatus(autosar::ActrDefrstSts::Limited);
    setElectricDefrosterWindscreen(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF);
    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);
    EXPECT_NE(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::ON,
              shareWindscreenPopupAttribute__.get().value_);
}

// HMI Defrost Front status flexray signal as Avaialable
TEST_F(ElectricDefrosterWindscreenPopUpTest, HMIDefrostFrontStsasNotAvaialable) {
    setHmiDefrostStatus(autosar::ActrDefrstSts::NotAvailable);
    setElectricDefrosterWindscreen(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF);
    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);
    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::OFF,
              shareWindscreenPopupAttribute__.get().value_);
}

// CCSM request to turn CCSM defrost to ON
TEST_F(ElectricDefrosterWindscreenPopUpTest, Set_WindscreenStatusasOFF) {
    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);
    edwp.request(OnOff::On);
    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::OFF,
              shareWindscreenPopupAttribute__.get().value_);
}

// CCSM request to turn defrost to ON and HMI defrost front status as NotAvailable
TEST_F(ElectricDefrosterWindscreenPopUpTest, Set_WindscreenStatusasOFF_HMIDefrostFrontStsasNotAvaialable) {
    setHmiDefrostStatus(autosar::ActrDefrstSts::NotAvailable);
    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);
    edwp.request(OnOff::On);
    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::ON,
              shareWindscreenPopupAttribute__.get().value_);
}

TEST_F(ElectricDefrosterWindscreenPopUpTest, showPopup) {
    setHmiDefrostStatus(autosar::ActrDefrstSts::NotAvailable);
    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);

    shareElectricDefrosterWindscreen_.set(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON);
    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::OFF,
              shareWindscreenPopupAttribute__.get().value_);

    shareElectricDefrosterWindscreen_.set(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF);
    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::ON,
              shareWindscreenPopupAttribute__.get().value_);
}

TEST_F(ElectricDefrosterWindscreenPopUpTest,
       ShowPopup_WhenElectricWindscreenDefrosterIsOn_WhenSettingStatusNotAvaiable) {
    setHmiDefrostStatus(autosar::ActrDefrstSts::On);
    ElectricDefrosterWindscreenPopupLogic edwp(shareWindscreenPopupAttribute__, shareElectricDefrosterWindscreen_);

    shareElectricDefrosterWindscreen_.set(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON);

    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::OFF,
              shareWindscreenPopupAttribute__.get().value_);

    setHmiDefrostStatus(autosar::ActrDefrstSts::NotAvailable);
    // The electric windscreen defroster will be OFF when going to NotAvailable
    shareElectricDefrosterWindscreen_.set(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF);

    EXPECT_EQ(ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupState::ON,
              shareWindscreenPopupAttribute__.get().value_);
}

}  // namespace TestElectricDefrosterWindscreenPopup
