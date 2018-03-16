/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <vcc/localconfig.h>
#include "electric_defroster_windscreen_logic.h"

#include "carconfig_mock.h"

#include "enum_helper.h"
#include "mock_dispatcher.h"
#include "notifiable_property.h"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <cc_parameterlist.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ApplicationDataElement;
using namespace autosar;
using namespace ECDDataElement;
using namespace SettingsFramework;
using namespace testing;

using EDW = ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState;
using EDS = ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState;
using CC122 = CarConfigParams::CC122_HeatedWindscreenType;

namespace {
std::chrono::milliseconds toMilliseconds(double seconds) {
    return std::chrono::milliseconds{static_cast<int>(seconds * 1000)};
}
}

class ElectricDefrosterWindscreenTest : public ::testing::Test {
  public:
    ElectricDefrosterWindscreenTest()
        : electricDefrosterWindscreen_(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF),
          sAutoDefrosterFront({UserSelectionGen::StateType::NOT_PRESENT, UserSelectionGen::OffOnType::OFF}),
          lcfgTimeout(3.1) {
        ON_CALL(carConfig_, getValue(to_undrl(CC122::ParamNumber)))
                .WillByDefault(Return(to_undrl(CC122::Heated_Frontscreen)));

        ON_CALL(localConfig, GetDoubleMock("Climate_defroster_timeout")).WillByDefault(Return(lcfgTimeout));
        ON_CALL(dispatcher, IsRunning()).WillByDefault(Return(false));

        setHmiDefrostStatus(ActrDefrstSts::Off);
        resetSignals();
    }

    ~ElectricDefrosterWindscreenTest() {
        resetVehicleMode();
        resetSignals();
    }

  protected:
    void setElectricDefrosterWindscreen(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState frost) {
        electricDefrosterWindscreen_.set(frost);
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

    void setVehicleMode(CarModSts1 carModSts1, UsgModSts1 usgModeSts1) {
        VehModMngtGlbSafe1 drvMode;
        drvMode.CarModSts1_ = carModSts1;
        drvMode.UsgModSts = usgModeSts1;
        driverMode.inject(drvMode);
    }

    void setDriveMode(DrvModReqType1 drvModReqType1) { driveMode.send(drvModReqType1); }

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

    void resetSignals() {
        setDriveMode(DrvModReqType1::Undefd);
        setVehicleMode(CarModSts1::CarModFcy, UsgModSts1::UsgModAbdnd);
        setHmiDefrostStatus(ActrDefrstSts::Off);
    }

    void createSut() {
        sut_ = std::unique_ptr<ElectricDefrosterWindscreenLogic>(new ElectricDefrosterWindscreenLogic(
                &localConfig, electricDefrosterWindscreen_, sAutoDefrosterFront, dispatcher, hmiDefrstElecReq_));
    }

    NotifiableProperty<ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState> electricDefrosterWindscreen_;
    NotifiableProperty<UserSelectionGen::OffOnSelection> sAutoDefrosterFront;
    DEInjector<autosar::VehModMngtGlbSafe1_info> vehicleMode;
    DEInjector<autosar::HmiDefrstElecSts_info> hmiDefrosterStatus;
    DEInjector<autosar::VehModMngtGlbSafe1_info> driverMode;
    DESender<autosar::DrvModReq_info> driveMode;
    NiceMock<vcc::mocks::MockLocalConfigReader> localConfig;
    NiceMock<CarConfigMock> carConfig_;
    NiceMock<MockDispatcher> dispatcher;
    std::unique_ptr<ElectricDefrosterWindscreenLogic> sut_;
    HmiDefrstElecReq hmiDefrstElecReq_;

    const double lcfgTimeout;
};

TEST_F(ElectricDefrosterWindscreenTest, initWithoutInvalidCarConfig) {
    EXPECT_CALL(carConfig_, getValue(to_undrl(CC122::ParamNumber)))
            .WillRepeatedly(Return(to_undrl(CC122::InvalidValue)));

    createSut();

    EXPECT_TRUE(sut_->isInState(
            ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::CARCONFIG_INVALID));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::NOT_PRESENT,
              electricDefrosterWindscreen_.get());
}

//==============================================//
//               Simple error startup           //
//==============================================//
TEST_F(ElectricDefrosterWindscreenTest, testErrorStartup) {
    // Inject signal error
    setHmiDefrostStatus(ActrDefrstSts::On, true);

    createSut();

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::SYSTEM_ERROR));
}

//===================================//
// Class for testing all carconfig   //
//===================================//
class ElectricDefrosterWindscreenCarconfigTest : public ElectricDefrosterWindscreenTest,
                                                 public WithParamInterface<std::tuple<CC122>> {};

TEST_P(ElectricDefrosterWindscreenCarconfigTest, testCarConfig) {
    auto carconfigParam = std::get<0>(GetParam());

    EXPECT_CALL(carConfig_, getValue(to_undrl(CC122::ParamNumber))).WillRepeatedly(Return(to_undrl(carconfigParam)));

    createSut();

    bool carConfigOK = (carconfigParam == CC122::Heated_Frontscreen);

    if (carConfigOK) {
        EXPECT_TRUE(
                sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::SYSTEM_OK));
        EXPECT_TRUE(sut_->isInState(
                ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::NOT_ACTIVE));
        EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::DISABLED,
                  electricDefrosterWindscreen_.get());
    } else {
        EXPECT_TRUE(sut_->isInState(
                ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::CARCONFIG_INVALID));
        EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::NOT_PRESENT,
                  electricDefrosterWindscreen_.get());
    }
}

INSTANTIATE_TEST_CASE_P(testCarConfig,
                        ElectricDefrosterWindscreenCarconfigTest,
                        Values(CC122::Heated_Frontscreen, CC122::InvalidValue, CC122::No_Heated_Frontscreen));

//===================================//
// Class for testing ActivationCheck //
//===================================//
class ElectricDefrosterWindscreenActivationCheckTest : public ElectricDefrosterWindscreenTest,
                                                       public WithParamInterface<std::tuple<CarModSts1, UsgModSts1>> {};

TEST_P(ElectricDefrosterWindscreenActivationCheckTest, testActivationCheck) {
    auto carMode = std::get<0>(GetParam());
    auto usageMode = std::get<1>(GetParam());

    createSut();

    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    setHmiDefrostStatus(ActrDefrstSts::Off);

    setDriveMode(DrvModReqType1::DrvMod2);
    setVehicleMode(carMode, usageMode);

    bool activationCheck = (usageMode == UsgModSts1::UsgModDrvg &&
                            (carMode == CarModSts1::CarModDyno || carMode == CarModSts1::CarModNorm));

    if (activationCheck)
        EXPECT_TRUE(sut_->isInState(EDW::ACTIVE));
    else
        EXPECT_TRUE(sut_->isInState(EDW::NOT_ACTIVE));
}

INSTANTIATE_TEST_CASE_P(testActivationCheck,
                        ElectricDefrosterWindscreenActivationCheckTest,
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
class ElectricDefrosterWindscreenAutoInitCheckCheckTest
        : public ElectricDefrosterWindscreenTest,
          public WithParamInterface<std::tuple<DrvModReqType1, UserSelectionGen::OffOnType>> {};

TEST_P(ElectricDefrosterWindscreenAutoInitCheckCheckTest, testAutoInitCheck) {
    auto driveMode = std::get<0>(GetParam());
    auto autoFront = std::get<1>(GetParam());

    createSut();

    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, autoFront});
    setHmiDefrostStatus(ActrDefrstSts::Off);

    setDriveMode(driveMode);
    setVehicleMode(CarModSts1::CarModDyno, UsgModSts1::UsgModDrvg);

    bool AutoInitCheck = (autoFront == UserSelectionGen::OffOnType::ON &&
                          (driveMode != DrvModReqType1::DrvMod1 && driveMode != DrvModReqType1::DrvMod8));

    if (AutoInitCheck)
        EXPECT_TRUE(sut_->isInState(EDW::AUTO));
    else
        EXPECT_TRUE(sut_->isInState(EDW::MANUAL));
}

INSTANTIATE_TEST_CASE_P(testAutoInitCheck,
                        ElectricDefrosterWindscreenAutoInitCheckCheckTest,
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
class ElectricDefrosterWindscreenToManualTest
        : public ElectricDefrosterWindscreenTest,
          public WithParamInterface<std::tuple<FirstRowGen::ElectricDefrosterWindscreenRequest, ActrDefrstSts>> {};

TEST_P(ElectricDefrosterWindscreenToManualTest, testGotoManual) {
    auto PA_Request = std::get<0>(GetParam());
    auto HmiFront = std::get<1>(GetParam());

    auto driveMode = DrvModReqType1::DrvMod2;
    auto usageMode = UsgModSts1::UsgModDrvg;
    auto carMode = CarModSts1::CarModDyno;

    createSut();

    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    setHmiDefrostStatus(HmiFront);

    setDriveMode(driveMode);
    setVehicleMode(carMode, usageMode);

    EXPECT_TRUE(sut_->isInState(EDW::SYSTEM_OK));

    EXPECT_TRUE(sut_->isInState(EDW::AUTO));

    bool AutoToManualCheck =
            ((PA_Request == EDS::ON && electricDefrosterWindscreen_.get() == EDS::OFF) ||
             (PA_Request == EDS::OFF && electricDefrosterWindscreen_.get() == EDS::ON) ||
             /* TODO: CCSM */
             (HmiFront == ActrDefrstSts::NotAvailable && electricDefrosterWindscreen_.get() == EDS::ON));

    sut_->request(PA_Request);

    if (AutoToManualCheck) {
        EXPECT_TRUE(sut_->isInState(EDW::MANUAL));
    } else {
        EXPECT_TRUE(sut_->isInState(EDW::AUTO));
    }
}

INSTANTIATE_TEST_CASE_P(testGotoManual,
                        ElectricDefrosterWindscreenToManualTest,
                        Combine(Values(FirstRowGen::ElectricDefrosterWindscreenRequest::ON,
                                       FirstRowGen::ElectricDefrosterWindscreenRequest::OFF),
                                Values(ActrDefrstSts::AutoCdn,
                                       ActrDefrstSts::Limited,
                                       ActrDefrstSts::NotAvailable,
                                       ActrDefrstSts::Off,
                                       ActrDefrstSts::On,
                                       ActrDefrstSts::TmrOff)));

//===================================//
//          Trigger Error            //
//===================================//
TEST_F(ElectricDefrosterWindscreenTest, triggerSystemError) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    createSut();

    // Inject signal error
    setHmiDefrostStatus(ActrDefrstSts::On, true);

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::SYSTEM_ERROR));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);
}

TEST_F(ElectricDefrosterWindscreenTest, returnFromError) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    createSut();

    // Inject signal error
    setHmiDefrostStatus(ActrDefrstSts::On, true);

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::SYSTEM_ERROR));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);

    setHmiDefrostStatus(ActrDefrstSts::On);
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::SYSTEM_OK));
    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::NOT_ACTIVE));
}

TEST_F(ElectricDefrosterWindscreenTest, activate) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    setHmiDefrostStatus(ActrDefrstSts::Off);

    createSut();

    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::ACTIVE));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO_OFF));
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);
}

//=========================================================//
// Class for testing all drivemodes with AutoDefroster ON  //
//=========================================================//
class ElectricDefrosterWindscreenAutoDefrosterOnTest : public ElectricDefrosterWindscreenTest,
                                                       public WithParamInterface<std::tuple<DrvModReqType1, EDW>> {};

TEST_P(ElectricDefrosterWindscreenAutoDefrosterOnTest, testDriveMode) {
    auto driveMode = std::get<0>(GetParam());
    auto state = std::get<1>(GetParam());

    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    setHmiDefrostStatus(ActrDefrstSts::AutoCdn);

    createSut();

    setDriveMode(driveMode);
    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(sut_->isInState(state));

    if (state == EDW::MANUAL)
        EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);
    else {
        EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);
    }
}
INSTANTIATE_TEST_CASE_P(testDriveMode,
                        ElectricDefrosterWindscreenAutoDefrosterOnTest,
                        Values(std::tuple<DrvModReqType1, EDW>(DrvModReqType1::Undefd, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::Err, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod1, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod2, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod3, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod4, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod5, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod6, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod7, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod8, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod9, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod10, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod11, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod12, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod13, EDW::AUTO),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod14, EDW::AUTO)));

//=========================================================//
// Class for testing all drivemodes with AutoDefroster OFF //
//=========================================================//
class ElectricDefrosterWindscreenAutoDefrosterOffTest : public ElectricDefrosterWindscreenTest,
                                                        public WithParamInterface<std::tuple<DrvModReqType1, EDW>> {};

TEST_P(ElectricDefrosterWindscreenAutoDefrosterOffTest, testDriveMode) {
    auto driveMode = std::get<0>(GetParam());
    auto state = std::get<1>(GetParam());

    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    setHmiDefrostStatus(ActrDefrstSts::AutoCdn);

    createSut();

    setDriveMode(driveMode);
    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::OFF});
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(sut_->isInState(state));

    if (state == EDW::MANUAL)
        EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);
    else {
        EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);
    }
}
INSTANTIATE_TEST_CASE_P(testDriveMode,
                        ElectricDefrosterWindscreenAutoDefrosterOffTest,
                        Values(std::tuple<DrvModReqType1, EDW>(DrvModReqType1::Undefd, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::Err, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod1, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod2, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod3, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod4, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod5, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod6, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod7, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod8, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod9, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod10, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod11, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod12, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod13, EDW::MANUAL),
                               std::tuple<DrvModReqType1, EDW>(DrvModReqType1::DrvMod14, EDW::MANUAL)));

//===================================//
//          Goto to Auto Off         //
//===================================//
TEST_F(ElectricDefrosterWindscreenTest, testGotoAutoOff) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    setHmiDefrostStatus(ActrDefrstSts::Limited);

    createSut();

    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});

    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);
}

//===================================//
//     Go from AutoOff to AutoOn     //
//===================================//
TEST_F(ElectricDefrosterWindscreenTest, testGotoAutoOffAfterAutoOn) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    createSut();

    setHmiDefrostStatus(ActrDefrstSts::Limited);
    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);

    setHmiDefrostStatus(ActrDefrstSts::AutoCdn);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO_ON));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);

    setHmiDefrostStatus(ActrDefrstSts::Limited);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);
}

//===================================//
//     Go from Auto to ManualOn      //
//===================================//
TEST_F(ElectricDefrosterWindscreenTest, testGoFromAutoToManualOn) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    createSut();

    setHmiDefrostStatus(ActrDefrstSts::Limited);
    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    sut_->request(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenRequest::ON);
    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);

    sut_->request(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenRequest::ON);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_ON));
    EXPECT_EQ(autosar::ActrReq::On, outSignal.FrntElecReq);
}

//===================================//
//     Go from Auto to ManualOff     //
//===================================//
TEST_F(ElectricDefrosterWindscreenTest, testGoFromAutoToManualOff) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    setHmiDefrostStatus(ActrDefrstSts::NotAvailable);

    createSut();

    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO));
    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::AUTO_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::AutOn, outSignal.FrntElecReq);

    sut_->request(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenRequest::ON);
    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_OFF));
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);
}

//===================================//
//        Goto Manual At Once        //
//===================================//
TEST_F(ElectricDefrosterWindscreenTest, testGotoManualOnAtOnce) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    createSut();

    setHmiDefrostStatus(ActrDefrstSts::Limited);
    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::OFF});
    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);
}

//===================================//
//   Go from ManualOn to ManualOff   //
//===================================//
TEST_F(ElectricDefrosterWindscreenTest, testGotoManualOnAfterManualOff) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    createSut();

    setHmiDefrostStatus(ActrDefrstSts::Limited);
    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::OFF});
    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);

    sut_->request(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenRequest::ON);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_ON));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::On, outSignal.FrntElecReq);
}

//===================================//
//   Go from ManualOff to ManualOn   //
//===================================//
TEST_F(ElectricDefrosterWindscreenTest, testGotoManualOffAfterManualOnStartingInAutoOff) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    createSut();

    setHmiDefrostStatus(ActrDefrstSts::Limited);
    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    sut_->request((ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenRequest::ON));

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_ON));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::On, outSignal.FrntElecReq);

    setHmiDefrostStatus(ActrDefrstSts::TmrOff);

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);
}

//====================================//
//    Recocver from Abandon Mode      //
//====================================//
TEST_F(ElectricDefrosterWindscreenTest, RecoverFromAbandonMode) {
    createSut();

    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::OFF});

    setDriveMode(autosar::DrvModReqType1::DrvMod8);
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);
    setHmiDefrostStatus(autosar::ActrDefrstSts::On);

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_OFF));

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModAbdnd);
    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::NOT_ACTIVE));

    setVehicleMode(CarModSts1::CarModNorm, UsgModSts1::UsgModDrvg);
    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_OFF));
}

TEST_F(ElectricDefrosterWindscreenTest, testHandleCCSMRequest) {
    createSut();

    setHmiDefrostStatus(ActrDefrstSts::On);
    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::OFF});
    setDriveMode(autosar::DrvModReqType1::DrvMod2);
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());

    sut_->request(OnOff::On);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());

    sut_->request(OnOff::Off);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
}

TEST_F(ElectricDefrosterWindscreenTest,
       ElectricDefrosterShallRemainOff_WhenMovingBackToStatusOn_AfterAttemptToEnableInStatusNotAvailable) {
    createSut();

    setHmiDefrostStatus(ActrDefrstSts::NotAvailable);
    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());

    sut_->request(OnOff::On);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());

    setHmiDefrostStatus(ActrDefrstSts::On);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
}

TEST_F(ElectricDefrosterWindscreenTest,
       ElectricDefrosterShallBeSentOff_WhenSettingNotAvailable_AfterTurnedOnInDrivingByAutoSetting) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequestSink;
    hmiDefrosterRequestSink.subscribe(
            [&hmiDefrosterRequestSink, &outSignal]() { outSignal = hmiDefrosterRequestSink.get().value(); });

    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModInActv);

    createSut();

    sAutoDefrosterFront.set({UserSelectionGen::StateType::AVAILABLE, UserSelectionGen::OffOnType::ON});
    setHmiDefrostStatus(ActrDefrstSts::AutoCdn);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::DISABLED,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(ActrReq::Off, outSignal.FrntElecReq);

    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(ActrReq::AutOn, outSignal.FrntElecReq);

    setHmiDefrostStatus(ActrDefrstSts::NotAvailable);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(ActrReq::Off, outSignal.FrntElecReq);

    setHmiDefrostStatus(ActrDefrstSts::AutoCdn);

    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(ActrReq::Off, outSignal.FrntElecReq);
}

TEST_F(ElectricDefrosterWindscreenTest, ElectricDefrosterDispatcher_ShallBeTurnedOn_AndGoToManualOffAfterTimeout) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    std::function<void(void)> func;

    createSut();

    EXPECT_CALL(dispatcher, Start(toMilliseconds(lcfgTimeout), _, _)).WillOnce(SaveArg<1>(&func));

    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    ON_CALL(dispatcher, IsRunning()).WillByDefault(Return(true));

    sut_->request((ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenRequest::ON));
    setHmiDefrostStatus(ActrDefrstSts::On);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_ON));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::On, outSignal.FrntElecReq);

    setHmiDefrostStatus(ActrDefrstSts::Off);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_ON));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::On, outSignal.FrntElecReq);

    // Invoke dispatcher timeout
    ON_CALL(dispatcher, IsRunning()).WillByDefault(Return(false));
    if (func) {
        func();
    }

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);
}

TEST_F(ElectricDefrosterWindscreenTest,
       ElectricDefroster_ShallGoToManualOff_WhenDefrosterStatusIsSetToOffAfterTimerHasTimedOut) {
    HmiDefrstElecReq outSignal = {static_cast<ActrReq>(-1), static_cast<ActrReq>(-1), static_cast<ActrReq>(-1)};
    DESink<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;
    hmiDefrosterRequest.subscribe(
            [&hmiDefrosterRequest, &outSignal]() { outSignal = hmiDefrosterRequest.get().value(); });

    std::function<void(void)> func;

    createSut();

    EXPECT_CALL(dispatcher, Start(toMilliseconds(lcfgTimeout), _, _)).WillOnce(SaveArg<1>(&func));

    setVehicleMode(autosar::CarModSts1::CarModNorm, autosar::UsgModSts1::UsgModDrvg);

    sut_->request((ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenRequest::ON));
    setHmiDefrostStatus(ActrDefrstSts::On);

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_ON));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::On, outSignal.FrntElecReq);

    // Invoke dispatcher timeout
    if (func) func();

    EXPECT_TRUE(sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_ON));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::ON,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::On, outSignal.FrntElecReq);

    setHmiDefrostStatus(ActrDefrstSts::Off);

    EXPECT_TRUE(
            sut_->isInState(ElectricDefrosterWindscreenLogic::InternalElectricDefrosterWindscreenState::MANUAL_OFF));
    EXPECT_EQ(ElectricDefrosterWindscreenLogic::ElectricDefrosterWindscreenState::OFF,
              electricDefrosterWindscreen_.get());
    EXPECT_EQ(autosar::ActrReq::Off, outSignal.FrntElecReq);
}
