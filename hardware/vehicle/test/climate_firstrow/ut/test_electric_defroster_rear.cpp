/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vcc/localconfig.h>
#include <chrono>
#include <thread>
#include <v0/org/volvocars/climate/UserSelection.hpp>
#include "electric_defroster_rear_logic.h"
#include "ivi-logging.h"
#include "mock_dispatcher.h"

using ElectricDefrosterRearState = v0::org::volvocars::climate::FirstRow::ElectricDefrosterRearState;
using namespace testing;
using namespace autosar;

using namespace ApplicationDataElement;
using namespace autosar;
using namespace SettingsFramework;
using namespace ECDDataElement;

using EDR = ElectricDefrosterRearLogic::InternalElectricDefrosterRearState;
using EDS = ElectricDefrosterRearLogic::ElectricDefrosterRearState;

namespace {
std::chrono::milliseconds toMilliseconds(double seconds) {
    return std::chrono::milliseconds{static_cast<int>(seconds * 1000)};
}
}

class ElectricDefrosterRearTest : public ::testing::Test {
  public:
    ElectricDefrosterRearTest()
        : electricDefrosterRear_(ElectricDefrosterRearState::DISABLED),
          sAutoDefrosterRear({UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}),
          lcfgTimeout(3.1) {
        ON_CALL(localConfig, GetDoubleMock("Climate_defroster_timeout")).WillByDefault(Return(lcfgTimeout));
        ON_CALL(dispatcher, IsRunning()).WillByDefault(Return(false));

        resetSignals();
        resetVehicleMode();
        setDriveMode(DrvModReqType1::Undefd);
        setHmiDefrosterStatus(ActrDefrstSts::Off, ActrDefrstSts::Off);
        setButton5FrontRequest(PsdNotPsd::NotPsd);

        outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};

        hmiDefrosterRequest.subscribe([this]() { outSignal = hmiDefrosterRequest.get().value(); });
    }

    ~ElectricDefrosterRearTest() { resetVehicleMode(); }

    NotifiableProperty<ElectricDefrosterRearState> electricDefrosterRear_;
    NotifiableProperty<UserSelectionGen::OffOnSelection> sAutoDefrosterRear;

    DEInjector<autosar::VehModMngtGlbSafe1_info> vehicleMode;
    DESender<autosar::DrvModReq_info> driveMode;
    DEInjector<autosar::HmiDefrstElecSts_info> hmiDefrstSts;
    DEInjector<autosar::Btn5ForUsrSwtPanFrntReq_info> btn5FrntReq;

    NiceMock<vcc::mocks::MockLocalConfigReader> localConfig;
    NiceMock<MockDispatcher> dispatcher;

    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    HmiDefrstElecReq outSignal;
    std::unique_ptr<ElectricDefrosterRearLogic> sut_;
    HmiDefrstElecReq hmiDefrstElecReq_;

    const double lcfgTimeout;

    void setDriveMode(DrvModReqType1 drvModReqType1) { driveMode.send(drvModReqType1); }

    void setVehicleMode(CarModSts1 carModSts1, UsgModSts1 usgModeSts1) {
        VehModMngtGlbSafe1 drvMode;
        drvMode.UsgModSts = usgModeSts1;
        drvMode.CarModSts1_ = carModSts1;
        vehicleMode.inject(drvMode);
    }

    void setUsgMode(UsgModSts1 usgModeSts1) {
        VehModMngtGlbSafe1 drvMode;
        drvMode.UsgModSts = usgModeSts1;
        vehicleMode.inject(drvMode);
    }

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

    void setButton5FrontRequest(PsdNotPsd request) { btn5FrntReq.inject(request); }

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
        vehicleMode.inject(drvMode);
    }
    void setVehicleModeError() { vehicleMode.error(12); }

    void setLevelOfComfort(LvlOfClimaCmft lvlOfClimaCmft) {
        DEInjector<autosar::LvlOfClimaCmft_info> levelOfClimateComfort;
        levelOfClimateComfort.inject(lvlOfClimaCmft);
    }

    void resetSignals() {
        setDriveMode(DrvModReqType1::Undefd);
        setVehicleMode(CarModSts1::CarModFcy, UsgModSts1::UsgModAbdnd);
        setHmiDefrosterStatus(ActrDefrstSts::Off, ActrDefrstSts::Off);
    }

    void createSut() {
        sut_ = std::unique_ptr<ElectricDefrosterRearLogic>(new ElectricDefrosterRearLogic(
                &localConfig, electricDefrosterRear_, sAutoDefrosterRear, dispatcher, hmiDefrstElecReq_));
    }
};

//==============================================//
//               Simple error startup           //
//==============================================//
TEST_F(ElectricDefrosterRearTest, testErrorStartup) {
    // Inject signal error
    setHmiDefrosterStatus(ActrDefrstSts::Off, ActrDefrstSts::Off, true);

    createSut();

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::SYSTEM_ERROR));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
}

//==============================================//
//               Simple startup                 //
//==============================================//
TEST_F(ElectricDefrosterRearTest, startUp) {
    createSut();

    EXPECT_EQ(ElectricDefrosterRearState::DISABLED, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::NOT_ACTIVE));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
}

//===================================//
// Class for testing ActivationCheck //
//===================================//
class ElectricDefrosterRearActivationCheckTest : public ElectricDefrosterRearTest,
                                                 public WithParamInterface<std::tuple<CarModSts1, UsgModSts1>> {};

TEST_P(ElectricDefrosterRearActivationCheckTest, testActivationCheck) {
    auto carMode = std::get<0>(GetParam());
    auto usageMode = std::get<1>(GetParam());

    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setHmiDefrosterStatus(ActrDefrstSts::Off, ActrDefrstSts::Off);

    setDriveMode(DrvModReqType1::DrvMod2);
    setVehicleMode(carMode, usageMode);

    bool activationCheck = (usageMode == UsgModSts1::UsgModDrvg &&
                            (carMode == CarModSts1::CarModDyno || carMode == CarModSts1::CarModNorm));

    if (activationCheck)
        EXPECT_TRUE(sut_->isInState(EDR::ACTIVE));
    else {
        EXPECT_TRUE(sut_->isInState(EDR::NOT_ACTIVE));
        EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
    }
}

INSTANTIATE_TEST_CASE_P(testActivationCheck,
                        ElectricDefrosterRearActivationCheckTest,
                        Combine(Values(CarModSts1::CarModCrash,
                                       CarModSts1::CarModDyno,
                                       CarModSts1::CarModFcy,
                                       CarModSts1::CarModNorm,
                                       CarModSts1::CarModTrnsp),
                                Values(UsgModSts1::UsgModAbdnd,
                                       UsgModSts1::UsgModActv,
                                       UsgModSts1::UsgModCnvinc,
                                       UsgModSts1::UsgModDrvg,
                                       UsgModSts1::UsgModInActv)));

//===================================//
// Class for testing AutoInitCheck   //
//===================================//
class ElectricDefrosterRearAutoInitCheckCheckTest
        : public ElectricDefrosterRearTest,
          public WithParamInterface<std::tuple<DrvModReqType1, UserSelectionGen::OffOnType>> {};

TEST_P(ElectricDefrosterRearAutoInitCheckCheckTest, testAutoInitCheck) {
    auto driveMode = std::get<0>(GetParam());
    auto autoRear = std::get<1>(GetParam());

    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, autoRear});

    setHmiDefrosterStatus(ActrDefrstSts::Off, ActrDefrstSts::Off);

    setDriveMode(driveMode);
    setVehicleMode(CarModSts1::CarModDyno, UsgModSts1::UsgModDrvg);

    bool AutoInitCheck = (autoRear == UserSelectionGen::OffOnType::ON &&
                          (driveMode != DrvModReqType1::DrvMod1 && driveMode != DrvModReqType1::DrvMod8));

    if (AutoInitCheck) {
        EXPECT_TRUE(sut_->isInState(EDR::AUTO));
        EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.ReElecReq);
    } else
        EXPECT_TRUE(sut_->isInState(EDR::MANUAL));
}

INSTANTIATE_TEST_CASE_P(testAutoInitCheck,
                        ElectricDefrosterRearAutoInitCheckCheckTest,
                        Combine(Values(DrvModReqType1::DrvMod1,
                                       DrvModReqType1::DrvMod2,
                                       DrvModReqType1::DrvMod3,
                                       DrvModReqType1::DrvMod4,
                                       DrvModReqType1::DrvMod5,
                                       DrvModReqType1::DrvMod6,
                                       DrvModReqType1::DrvMod7,
                                       DrvModReqType1::DrvMod8,
                                       DrvModReqType1::DrvMod9,
                                       DrvModReqType1::DrvMod10,
                                       DrvModReqType1::DrvMod11,
                                       DrvModReqType1::DrvMod12,
                                       DrvModReqType1::DrvMod13,
                                       DrvModReqType1::DrvMod14,
                                       DrvModReqType1::Err,
                                       DrvModReqType1::Undefd),
                                Values(UserSelectionGen::OffOnType::OFF, UserSelectionGen::OffOnType::ON)));

//====================================//
// Class for testing AutoToManualTest //
//====================================//
class ElectricDefrosterRearToManualTest
        : public ElectricDefrosterRearTest,
          public WithParamInterface<
                  std::tuple<FirstRowGen::ElectricDefrosterRearRequest, ActrDefrstSts, ActrDefrstSts, PsdNotPsd>> {};

TEST_P(ElectricDefrosterRearToManualTest, testGotoManual) {
    auto PA_Request = std::get<0>(GetParam());
    auto HmiRear = std::get<1>(GetParam());
    auto HmiMirror = std::get<2>(GetParam());
    auto Lin_Button = std::get<3>(GetParam());

    auto driveMode = DrvModReqType1::DrvMod2;
    auto usageMode = UsgModSts1::UsgModDrvg;
    auto carMode = CarModSts1::CarModDyno;

    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    setHmiDefrosterStatus(HmiRear, HmiMirror);
    setButton5FrontRequest(Lin_Button);

    setDriveMode(driveMode);
    setVehicleMode(carMode, usageMode);

    EXPECT_TRUE(sut_->isInState(EDR::SYSTEM_OK));

    if ((HmiRear != ActrDefrstSts::NotAvailable && HmiMirror != ActrDefrstSts::NotAvailable)) {
        EXPECT_TRUE(sut_->isInState(EDR::AUTO));
        EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.ReElecReq);
    } else
        EXPECT_TRUE(sut_->isInState(EDR::MANUAL));

    if (sut_->isInState(EDR::AUTO)) {
        bool validComp = (PA_Request != electricDefrosterRear_.get());

        bool AutoToManualCheck =
                ((PA_Request == ElectricDefrosterRearState::OFF &&
                  electricDefrosterRear_.get() == ElectricDefrosterRearState::ON) ||
                 (HmiRear == ActrDefrstSts::NotAvailable || HmiMirror == ActrDefrstSts::NotAvailable)) ||
                (PA_Request == ElectricDefrosterRearState::ON &&
                 electricDefrosterRear_.get() == ElectricDefrosterRearState::OFF) ||
                Lin_Button == PsdNotPsd::Psd;

        sut_->request(PA_Request);

        if (AutoToManualCheck && validComp) {
            std::cout << "AutoToManualCheck" << AutoToManualCheck << std::endl;
            EXPECT_TRUE(sut_->isInState(EDR::MANUAL));
        } else {
            EXPECT_TRUE(sut_->isInState(EDR::AUTO));
            EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.ReElecReq);
        }
    }
}

INSTANTIATE_TEST_CASE_P(testGotoManual,
                        ElectricDefrosterRearToManualTest,
                        Combine(Values(FirstRowGen::ElectricDefrosterRearRequest::ON,
                                       FirstRowGen::ElectricDefrosterRearRequest::OFF),
                                Values(ActrDefrstSts::AutoCdn,
                                       ActrDefrstSts::Limited,
                                       ActrDefrstSts::NotAvailable,
                                       ActrDefrstSts::Off,
                                       ActrDefrstSts::On,
                                       ActrDefrstSts::TmrOff),
                                Values(ActrDefrstSts::AutoCdn,
                                       ActrDefrstSts::Limited,
                                       ActrDefrstSts::NotAvailable,
                                       ActrDefrstSts::Off,
                                       ActrDefrstSts::On,
                                       ActrDefrstSts::TmrOff),
                                Values(PsdNotPsd::NotPsd, PsdNotPsd::Psd)));

//====================================//
//       Change to Auto On            //
//====================================//
TEST_F(ElectricDefrosterRearTest, changeToAutoOn) {
    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setButton5FrontRequest(PsdNotPsd::NotPsd);
    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    setHmiDefrosterStatus(ActrDefrstSts::AutoCdn, ActrDefrstSts::AutoCdn);

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::AUTO));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::AUTO_ON));
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.ReElecReq);
}

//====================================//
//    Go from Auto On to Auto Off     //
//====================================//
TEST_F(ElectricDefrosterRearTest, changeFromAutoOnToAutoOff) {
    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setButton5FrontRequest(PsdNotPsd::NotPsd);
    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    setHmiDefrosterStatus(ActrDefrstSts::AutoCdn, ActrDefrstSts::AutoCdn);

    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::ON);

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::AUTO));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::AUTO_ON));
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.ReElecReq);

    setHmiDefrosterStatus(ActrDefrstSts::On, ActrDefrstSts::On);

    EXPECT_EQ(ElectricDefrosterRearState::OFF, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::ACTIVE));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::AUTO));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::AUTO_OFF));
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.ReElecReq);
}

//====================================//
//       Change to Manual On          //
//====================================//
TEST_F(ElectricDefrosterRearTest, changeToManualOn) {
    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setDriveMode(autosar::DrvModReqType1::DrvMod8);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    setHmiDefrosterStatus(ActrDefrstSts::On, ActrDefrstSts::On);
    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::ON);

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);
}

//====================================//
//       Change to Manual Off         //
//====================================//
TEST_F(ElectricDefrosterRearTest, changeToManualOff) {
    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setDriveMode(autosar::DrvModReqType1::DrvMod1);
    setButton5FrontRequest(PsdNotPsd::NotPsd);
    setHmiDefrosterStatus(ActrDefrstSts::NotAvailable, ActrDefrstSts::NotAvailable);
    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::OFF);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(ElectricDefrosterRearState::OFF, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_OFF));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
}

//========================================//
//  Change from Manual On to manual Off   //
//========================================//
TEST_F(ElectricDefrosterRearTest, changeFromManualOnToManualOff) {
    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setDriveMode(autosar::DrvModReqType1::DrvMod8);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    setHmiDefrosterStatus(ActrDefrstSts::On, ActrDefrstSts::On);
    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::ON);

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);

    setHmiDefrosterStatus(ActrDefrstSts::NotAvailable, ActrDefrstSts::NotAvailable);

    EXPECT_EQ(ElectricDefrosterRearState::OFF, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_OFF));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
}

//====================================//
//         Test Error State           //
//====================================//
TEST_F(ElectricDefrosterRearTest, toErrorStateAndBackToArbitraryState) {
    createSut();

    // Pre-Condition
    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::OFF);
    setHmiDefrosterStatus(ActrDefrstSts::NotAvailable, ActrDefrstSts::NotAvailable);
    setButton5FrontRequest(PsdNotPsd::Psd);

    setDriveMode(autosar::DrvModReqType1::DrvMod1);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    EXPECT_EQ(ElectricDefrosterRearState::OFF, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_OFF));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);

    setVehicleModeError();

    EXPECT_EQ(ElectricDefrosterRearState::DISABLED, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::SYSTEM_ERROR));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);

    EXPECT_EQ(ElectricDefrosterRearState::OFF, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::SYSTEM_OK));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_OFF));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
}

TEST_F(ElectricDefrosterRearTest, RecoverFromAbandonMode) {
    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setDriveMode(autosar::DrvModReqType1::DrvMod8);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    setHmiDefrosterStatus(ActrDefrstSts::On, ActrDefrstSts::On);
    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::ON);

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModAbdnd);
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::NOT_ACTIVE));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_OFF));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
}

//====================================//
//    Simulate CCSM Button            //
//====================================//
TEST_F(ElectricDefrosterRearTest, SimulateCallFromCCSMButton) {
    createSut();

    sAutoDefrosterRear.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setDriveMode(autosar::DrvModReqType1::DrvMod8);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    setHmiDefrosterStatus(ActrDefrstSts::On, ActrDefrstSts::On);
    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::OFF);

    EXPECT_EQ(ElectricDefrosterRearState::OFF, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_OFF));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);

    sut_->request(OnOff::On);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);
}

//======================//
//    Timeout test      //
//======================//

class ElectricDefrosterRearTimerFlankConditionTest
        : public ElectricDefrosterRearTest,
          public WithParamInterface<std::tuple<ActrDefrstSts, ActrDefrstSts>> {};

TEST_P(ElectricDefrosterRearTimerFlankConditionTest,
       ElectricDefrosterDispatcher_ShallBeTurnedOn_AndGoToManualOffAfterTimeout) {
    auto rearDefrosterStatus = std::get<0>(GetParam());
    auto mirrorDefrosterStatus = std::get<1>(GetParam());

    std::function<void(void)> func;

    EXPECT_CALL(dispatcher, Start(toMilliseconds(lcfgTimeout), _, _)).WillOnce(SaveArg<1>(&func));

    createSut();

    ON_CALL(dispatcher, IsRunning()).WillByDefault(Return(true));

    setDriveMode(autosar::DrvModReqType1::DrvMod8);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    setHmiDefrosterStatus(ActrDefrstSts::On, ActrDefrstSts::On);
    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::ON);

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);

    setHmiDefrosterStatus(rearDefrosterStatus, mirrorDefrosterStatus);

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);

    // Invoke dispatcher timeout
    ON_CALL(dispatcher, IsRunning()).WillByDefault(Return(false));
    if (func) func();

    if (rearDefrosterStatus != ActrDefrstSts::Limited && rearDefrosterStatus != ActrDefrstSts::On &&
        mirrorDefrosterStatus != ActrDefrstSts::Limited && mirrorDefrosterStatus != ActrDefrstSts::On) {
        EXPECT_EQ(ElectricDefrosterRearState::OFF, electricDefrosterRear_.get());
        EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
        EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_OFF));
        EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
    } else {
        EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
        EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
        EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
        EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);
    }
}

INSTANTIATE_TEST_CASE_P(
        ElectricDefrosterDispatcher_ShallBeTurnedOn_AndGoToManualOffAfterTimeout,
        ElectricDefrosterRearTimerFlankConditionTest,
        Combine(Values(ActrDefrstSts::AutoCdn, ActrDefrstSts::Limited, ActrDefrstSts::Off, ActrDefrstSts::On),
                Values(ActrDefrstSts::AutoCdn, ActrDefrstSts::Limited, ActrDefrstSts::Off, ActrDefrstSts::On)));

class ElectricDefrosterRearTimerConditionTest : public ElectricDefrosterRearTest,
                                                public WithParamInterface<std::tuple<ActrDefrstSts, ActrDefrstSts>> {};

TEST_P(ElectricDefrosterRearTimerConditionTest,
       ElectricDefroster_ShallGoToManualOff_WhenDefrosterStatusIsSetToOffAfterTimerHasTimedOut) {
    auto rearDefrosterStatus = std::get<0>(GetParam());
    auto mirrorDefrosterStatus = std::get<1>(GetParam());

    std::function<void(void)> func;

    EXPECT_CALL(dispatcher, Start(toMilliseconds(lcfgTimeout), _, _)).WillOnce(SaveArg<1>(&func));

    createSut();

    ON_CALL(dispatcher, IsRunning()).WillByDefault(Return(true));

    setDriveMode(autosar::DrvModReqType1::DrvMod8);
    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    setHmiDefrosterStatus(ActrDefrstSts::On, ActrDefrstSts::On);
    sut_->request(FirstRowGen::ElectricDefrosterRearRequest::ON);

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);

    // Invoke dispatcher timeout
    ON_CALL(dispatcher, IsRunning()).WillByDefault(Return(false));
    if (func) func();

    EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);

    setHmiDefrosterStatus(rearDefrosterStatus, mirrorDefrosterStatus);

    if (rearDefrosterStatus != ActrDefrstSts::Limited && rearDefrosterStatus != ActrDefrstSts::On &&
        mirrorDefrosterStatus != ActrDefrstSts::Limited && mirrorDefrosterStatus != ActrDefrstSts::On) {
        EXPECT_EQ(ElectricDefrosterRearState::OFF, electricDefrosterRear_.get());
        EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
        EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_OFF));
        EXPECT_EQ(autosar::ActrReq::Off, outSignal.ReElecReq);
    } else {
        EXPECT_EQ(ElectricDefrosterRearState::ON, electricDefrosterRear_.get());
        EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL));
        EXPECT_TRUE(sut_->isInState(ElectricDefrosterRearLogic::InternalElectricDefrosterRearState::MANUAL_ON));
        EXPECT_EQ(autosar::ActrReq::On, outSignal.ReElecReq);
    }
}

INSTANTIATE_TEST_CASE_P(
        ElectricDefroster_ShallGoToManualOff_WhenDefrosterStatusIsSetToOffAfterTimerHasTimedOut,
        ElectricDefrosterRearTimerConditionTest,
        Combine(Values(ActrDefrstSts::AutoCdn, ActrDefrstSts::Limited, ActrDefrstSts::Off, ActrDefrstSts::On),
                Values(ActrDefrstSts::AutoCdn, ActrDefrstSts::Limited, ActrDefrstSts::Off, ActrDefrstSts::On)));
