/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <gtest/gtest.h>
#include "electric_defroster_rear_logic.h"
#include "electric_defroster_rear_popup_logic.h"
#include "notifiable_property.h"
#include "v0/org/volvocars/climate/FirstRow.hpp"

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

using namespace testing;
using namespace autosar;
using namespace ECDDataElement;

namespace TestElectricDefrosterRearPopup {

class ElectricDefrosterRearPopupTest : public ::testing::Test {
  public:
    ElectricDefrosterRearPopupTest()
        : electricDefrosterRear_{ElectricDefrosterRearLogic::ElectricDefrosterRearState::DISABLED},
          electricDefrosterRearPopup_{FirstRowGen::ElectricDefrosterPopupState::OFF}

    {
        electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::OFF);
    }
    ~ElectricDefrosterRearPopupTest() {}

    std::unique_ptr<ElectricDefrosterRearPopupLogic> createLogic() {
        electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::OFF);
        return std::make_unique<ElectricDefrosterRearPopupLogic>(electricDefrosterRear_, electricDefrosterRearPopup_);
    }

    DEInjector<HmiDefrstElecSts_info> hmiDefrstSts;

    NotifiableProperty<FirstRowGen::ElectricDefrosterRearState> electricDefrosterRear_;
    NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState> electricDefrosterRearPopup_;

    void setHmiDefrosterStatus(ActrDefrstSts rear, ActrDefrstSts mirror, bool injectError = false) {
        HmiDefrstElecSts defrstSts;
        defrstSts.Frnt = ActrDefrstSts::Off;
        defrstSts.Re = rear;
        defrstSts.Mirrr = mirror;
        if (injectError)
            hmiDefrstSts.error(1);
        else
            hmiDefrstSts.inject(defrstSts);
    }
};

//==============================================//
//               Simple error startup           //
//==============================================//

TEST_F(ElectricDefrosterRearPopupTest, startUp) { auto erdp = createLogic(); }

TEST_F(ElectricDefrosterRearPopupTest, callRequest) {
    auto edrp = createLogic();
    setHmiDefrosterStatus(ActrDefrstSts::Off, ActrDefrstSts::Off);
    edrp->request(FirstRowGen::ElectricDefrosterPopupRequest::OFF);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
}

TEST_F(ElectricDefrosterRearPopupTest, electricDefrosterRearGoesFromOnToOffWhenRearStatusIsNotAvailable) {
    auto edrp = createLogic();
    setHmiDefrosterStatus(ActrDefrstSts::NotAvailable, ActrDefrstSts::NotAvailable);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
    electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::ON);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
    electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::OFF);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::ON, electricDefrosterRearPopup_.get());
}

TEST_F(ElectricDefrosterRearPopupTest, pushOffWhenPopupIsOn) {
    auto edrp = createLogic();
    setHmiDefrosterStatus(ActrDefrstSts::NotAvailable, ActrDefrstSts::NotAvailable);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
    electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::ON);
    electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::OFF);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::ON, electricDefrosterRearPopup_.get());
    edrp->request(FirstRowGen::ElectricDefrosterPopupRequest::OFF);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
}

TEST_F(ElectricDefrosterRearPopupTest, HmiDefrostGoesToAvailableWhenPopupIsOn) {
    auto edrp = createLogic();
    setHmiDefrosterStatus(ActrDefrstSts::NotAvailable, ActrDefrstSts::NotAvailable);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
    electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::ON);
    electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::OFF);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::ON, electricDefrosterRearPopup_.get());
    setHmiDefrosterStatus(ActrDefrstSts::Off, ActrDefrstSts::Off);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
}

TEST_F(ElectricDefrosterRearPopupTest, HmiDefrosterStatusGoesFromNotAvailableToOff) {
    auto edrp = createLogic();
    setHmiDefrosterStatus(ActrDefrstSts::NotAvailable, ActrDefrstSts::NotAvailable);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
    electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::ON);
    electricDefrosterRear_.set(FirstRowGen::ElectricDefrosterRearState::OFF);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::ON, electricDefrosterRearPopup_.get());
    setHmiDefrosterStatus(ActrDefrstSts::Off, ActrDefrstSts::Off);
    EXPECT_EQ(FirstRowGen::ElectricDefrosterPopupState::OFF, electricDefrosterRearPopup_.get());
}

}  // namespace TestElectricDefrosterRearPopup
