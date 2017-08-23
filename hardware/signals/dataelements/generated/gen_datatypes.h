/*!
 * \file
 * C++ code generator for AUTOSAR v1.0
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved. Delphi Confidential
 * Generated at: 2017-08-23T17:26:58.440282
 * Source: databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml
 */
#ifndef _DATATYPES_H
#define _DATATYPES_H

#include <array>

namespace autosar {

/*!
 * \enum AbsWarnIndcnReq
 * Controls the ABS warning indication to the driver.
 */
enum class AbsWarnIndcnReq {
    AbsWarnIndcnOnReq=0,
    AbsWarnIndcnFlsgReq=1,
    Resd2=2,
    AbsWarnIndcnOffReq=3,
};

/*!
 * \enum AccSts1
 */
enum class AccSts1 {
    Off=1,
    Stb=2,
    Stop=3,
    StopTmp=4,
    Actv=5,
};

/*!
 * \enum ActrDefrstSts
 */
enum class ActrDefrstSts {
    Off=0,
    On=1,
    Limited=2,
    NotAvailable=3,
    TmrOff=4,
    AutoCdn=5,
};

/*!
 * \enum ActrReq
 */
enum class ActrReq {
    Off=0,
    On=1,
    AutOn=2,
};

/*!
 * \enum ActvInActv
 */
enum class ActvInActv {
    Active=0,
    Inactive=1,
};

/*!
 * \enum ActvnAvl1
 */
enum class ActvnAvl1 {
    NotAvl=0,
    Avl=1,
    Deactvd=2,
    Actvd=3,
};

/*!
 * \enum ActvnOfPrkgAut
 */
enum class ActvnOfPrkgAut {
    Off=0,
    PrkgTypAutSeln=1,
    PrkgTypPara=2,
    PrkgTypPerp=3,
    PrkgManvCncl=4,
    ParkOutManv=5,
    Resd1=6,
    Resd2=7,
};

/*!
 * \enum ActvtAutnmsPrkgCtrl
 */
enum class ActvtAutnmsPrkgCtrl {
    Idle=0,
    Activate=1,
    Resume=2,
};

/*!
 * \enum AdjSpdLimnSts2
 */
enum class AdjSpdLimnSts2 {
    Off=1,
    Stb=2,
    Actv=3,
    Ovrd=4,
};

/*!
 * \enum AdjSpdLimnWarnCoding
 */
enum class AdjSpdLimnWarnCoding {
    NoWarn=0,
    SoundWarn=1,
    VisWarn=2,
    SoundAndVisWarn=3,
};

/*!
 * \enum AdprTurnSpdMod2
 * Identifier     Description
 * CmftMod     CSA Comfort Mode
 * NormMod    CSA Normal Mode
 * DynMod      CSA Dynamic Mode
 */
enum class AdprTurnSpdMod2 {
    CmftMod=0,
    NormMod=1,
    DynMod=2,
};

/*!
 * \enum AmbLiTintg
 * Amibiance LIgt color setting:
 * 0 = Temperature
 * 1 = Off
 * 2 - 8 = Colour 1 - 7
 */
enum class AmbLiTintg {
    T=0,
    Off=1,
    Tintg1=2,
    Tintg2=3,
    Tintg3=4,
    Tintg4=5,
    Tintg5=6,
    Tintg6=7,
    Tintg7=8,
};

/*!
 * \enum AmbTIndcdUnit
 */
enum class AmbTIndcdUnit {
    Celsius=0,
    Fahrenheit=1,
    UkwnUnit=2,
};

/*!
 * \enum AprvdSts
 * Pairing status information for Out of band NFC pairing
 */
enum class AprvdSts {
    NoData=0,
    Setup=1,
    Request=2,
    Trusted=3,
    NotKnown=4,
    Disable=5,
};

/*!
 * \enum AsscSts
 */
enum class AsscSts {
    Idle=0,
    AsscDataTrfInin=1,
    AsscDataTrf=2,
    AsscReqActv=3,
    AsscReqNotAprvd=4,
};

/*!
 * \enum AsyLaneChgPsbl1
 */
enum class AsyLaneChgPsbl1 {
    NotPsbl=0,
    Le=1,
    Ri=2,
    Both=3,
};

/*!
 * \enum AsyLaneChgSts1
 */
enum class AsyLaneChgSts1 {
    Stb=0,
    Planned=1,
    ShowInt=2,
    Resd3=3,
    Executing=4,
    Resd5=5,
    Finshd=6,
    Aborted=7,
    Resd8=8,
    Resd9=9,
    Resd10=10,
};

/*!
 * \enum AsyLaneChgTyp1
 */
enum class AsyLaneChgTyp1 {
    NoIntent=0,
    LaneChgLe=1,
    LaneChgRi=2,
    SafeStopInLane=3,
    SafeStopToLe=4,
    SafeStopToRi=5,
    NotAvl=6,
    Resd7=7,
    Resd8=8,
    Resd9=9,
    Resd10=10,
};

/*!
 * \enum AudWarnReq1
 */
enum class AudWarnReq1 {
    Off=0,
    PrepareToLeave=1,
    LeaveNow=2,
};

/*!
 * \enum Aut1
 */
enum class Aut1 {
    Aut=0,
    HalfAut=1,
};

/*!
 * \enum AutnmsPrkgActvDirectPrkg
 */
enum class AutnmsPrkgActvDirectPrkg {
    DirectParkingNotActive=0,
    DirectParkingSlot=1,
    DirectParkingGarage=2,
};

/*!
 * \enum AutnmsPrkgActvPrkgDir
 */
enum class AutnmsPrkgActvPrkgDir {
    Inactive=0,
    FrontFirst=1,
    RearFirst=2,
};

/*!
 * \enum AutnmsPrkgActvPrkgScenario
 */
enum class AutnmsPrkgActvPrkgScenario {
    Inactive=0,
    ParkIn=1,
    ParkOut=2,
};

/*!
 * \enum AutnmsPrkgActvPrkgSide
 */
enum class AutnmsPrkgActvPrkgSide {
    Inactive=0,
    Left=1,
    Right=2,
    StraightForward=3,
};

/*!
 * \enum AutnmsPrkgActvRem
 */
enum class AutnmsPrkgActvRem {
    Inactive=0,
    NonRemote=1,
    Remote=2,
};

/*!
 * \enum AutnmsPrkgActvSlotTyp
 */
enum class AutnmsPrkgActvSlotTyp {
    Inactive=0,
    Parallel=1,
    Perpendicular=2,
    Garage=3,
    Fishbone=4,
    Resd1=5,
    Resd2=6,
    Resd3=7,
};

/*!
 * \enum AutnmsPrkgActvSt
 * Indicates which AP state is active
 */
enum class AutnmsPrkgActvSt {
    Off=0,
    PassiveScan=1,
    PassiveScanSlotFound=2,
    ParkInHighSpeed=3,
    ParkInScan=4,
    ParkInSlotFoundContinueForward=5,
    ParkInStopToPark=6,
    SideAndDirectionSelection=7,
    DirectParkInInstruction=8,
    ParkOutScanAndChooseScenario=9,
    ParkOutConfirmDirection=10,
    ParkOutStandInZone2=11,
    ParkOutStartRemoteParkOut=12,
    LookAround=13,
    RemoteManeuvre=14,
    EnterVehicle=15,
    ExitVehicle=16,
    WrongGear=17,
    StandStillAndConfirm=18,
    LookAroundAndStartManeuvre=19,
    RearwardMove=20,
    ForwardMove=21,
    ChangeGearForward=22,
    ChangeGearRearward=23,
    AutoBrakeInfo=24,
    MoveAwayFromObject=25,
    RemoteFinished=26,
    Finished=27,
    Pause=28,
    Cancel=29,
    Resd1=30,
    Resd2=31,
    Resd3=32,
    Resd4=33,
    Resd5=34,
};

/*!
 * \enum AutnmsPrkgAvlDirectPrkg
 */
enum class AutnmsPrkgAvlDirectPrkg {
    Unavailable=0,
    Available=1,
};

/*!
 * \enum AutnmsPrkgAvlPrkgDir
 */
enum class AutnmsPrkgAvlPrkgDir {
    NoSlotDetected=0,
    RearFirst=1,
    FrontFirst=2,
    RearAndFrontFirst=3,
};

/*!
 * \enum AutnmsPrkgAvlPrkgScenario
 */
enum class AutnmsPrkgAvlPrkgScenario {
    NotAvailable=0,
    ParkIn=1,
    ParkOut=2,
    ParkInAndParkOut=3,
};

/*!
 * \enum AutnmsPrkgAvlPrkgSide
 */
enum class AutnmsPrkgAvlPrkgSide {
    NoSlotDetected=0,
    Left=1,
    Right=2,
    StraightForward=3,
};

/*!
 * \enum AutnmsPrkgAvlRem
 */
enum class AutnmsPrkgAvlRem {
    NoSlotDetected=0,
    RemoteUnavailable=1,
    RemoteAvailable=2,
};

/*!
 * \enum AutnmsPrkgAvlSlotTyp
 */
enum class AutnmsPrkgAvlSlotTyp {
    NoSlotDetected=0,
    Parallel=1,
    Perpendicular=2,
    ParallelOrPerpendicular=3,
    DirectParking=4,
    Fishbone=5,
    ParallelOrFishbone=6,
    ParallelOrPerpendicularOrFishbone=7,
    Resd1=8,
    Resd2=9,
};

/*!
 * \enum AutnmsPrkgCnclSts
 */
enum class AutnmsPrkgCnclSts {
    Inactive=0,
    Active=1,
    DriverDeactivation=2,
    ABSInterference=3,
    ESCInterference=4,
    TrailerAttached=5,
    EngineStall=6,
    Malfunction=7,
    PathNotFound=8,
    MaxNumberOfMoves=9,
    MaxLongSpeedExceeded=10,
    MaxRemoteLongSpeedExceeded=11,
    PauseTimeout=12,
    UndefinedCancel=13,
    Resd1=14,
    Resd2=15,
};

/*!
 * \enum AutnmsPrkgPauseSts
 */
enum class AutnmsPrkgPauseSts {
    Inactive=0,
    Active=1,
    VehicleImpactWithObstacle=2,
    VehicleEmergencyIntervention=3,
    OpenDoor=4,
    SteeringOverride=5,
    StandstillDueToBrake=6,
    GearOverride=7,
    DriverInZone1=8,
    SafetySwitchReleased=9,
    DriverOutOfRange=10,
    UndefinedPause=11,
    Resd1=12,
    Resd2=13,
    Resd3=14,
    Resd4=15,
};

/*!
 * \enum AutnmsPrkgRecmndPrkgDir
 */
enum class AutnmsPrkgRecmndPrkgDir {
    NoSlotDetected=0,
    RearFirst=1,
    FrontFirst=2,
};

/*!
 * \enum AutnmsPrkgRecmndPrkgScenario
 */
enum class AutnmsPrkgRecmndPrkgScenario {
    NoSlotDetected=0,
    ParkIn=1,
    ParkOut=2,
};

/*!
 * \enum AutnmsPrkgRecmndPrkgSide
 */
enum class AutnmsPrkgRecmndPrkgSide {
    NoSlotDetected=0,
    Left=1,
    Right=2,
    StraightForward=3,
};

/*!
 * \enum AutnmsPrkgRecmndRem
 */
enum class AutnmsPrkgRecmndRem {
    NoSlotDetected=0,
    NonRemote=1,
    Remote=2,
};

/*!
 * \enum AutnmsPrkgRecmndSlotTyp
 */
enum class AutnmsPrkgRecmndSlotTyp {
    NoSlotDetected=0,
    Parallel=1,
    Perpendicular=2,
    DirectParking=3,
    Fishbone=4,
    Resd1=5,
    Resd2=6,
};

/*!
 * \enum AutnmsPrkgSeldDirectPrkg
 */
enum class AutnmsPrkgSeldDirectPrkg {
    DirectParkingNotSelected=0,
    DirectParkingSelected=1,
};

/*!
 * \enum AutnmsPrkgSeldPrkgDir
 */
enum class AutnmsPrkgSeldPrkgDir {
    Inactive=0,
    FrontFirst=1,
    RearFirst=2,
};

/*!
 * \enum AutnmsPrkgSeldPrkgScenario
 */
enum class AutnmsPrkgSeldPrkgScenario {
    Inactive=0,
    ParkIn=1,
    ParkOut=2,
};

/*!
 * \enum AutnmsPrkgSeldPrkgSide
 */
enum class AutnmsPrkgSeldPrkgSide {
    Inactive=0,
    Left=1,
    Right=2,
    StraightForward=3,
};

/*!
 * \enum AutnmsPrkgSeldRem
 */
enum class AutnmsPrkgSeldRem {
    Inactive=0,
    NonRemote=1,
    Remote=2,
};

/*!
 * \enum AutnmsPrkgSeldSlotTyp
 */
enum class AutnmsPrkgSeldSlotTyp {
    Inactive=0,
    Parallel=1,
    Perpendicular=2,
    Fishbone=3,
    Garage=4,
    HomeZone=5,
    ParkLaunch=6,
    Resd=7,
};

/*!
 * \enum AvlSts2
 * Availability status
 * 0= not available
 * 1= available
 */
enum class AvlSts2 {
    NotAvl=0,
    Avl=1,
};

/*!
 * \enum AxleWarn
 */
enum class AxleWarn {
    NoWarn=0,
    SoftWarn=1,
    HardWarn=2,
    Resd=3,
};

/*!
 * \enum BltLockSt1
 */
enum class BltLockSt1 {
    Unlock=0,
    Lock=1,
};

/*!
 * \enum BtnAudVolSts
 * SImple representation of  a virtual (multistep) button for adjusting audio volume.
 * 0 = button not pressed. No request to adjust volume
 * 1 = button pressed for volume up
 * 2 = buttono pressed for volume down
 * 
 * Receiver side controls how the button status is used.
 */
enum class BtnAudVolSts {
    NoBtnPsd=0,
    BtnVolUpPsd=1,
    BtnVolDwnPsd=2,
};

/*!
 * \enum BtnMmedLeRiSts
 * Represents status of a (multilevel) button Left/Right. 
 * 
 * Semantic defined by receiver side. (Previou/Next, FFBW/FFWD, etc)
 */
enum class BtnMmedLeRiSts {
    NoBtnPsd=0,
    BtnLePsd=1,
    BtnRiPsd=2,
};

/*!
 * \enum BtnMmedSetSts
 * Represents status of a button Set. 
 * 
 * Semantic defined by receiver side. (Activate/Deactivate, Play/Pause, Mute/UnMute etc)
 */
enum class BtnMmedSetSts {
    BtnSetNotPsd=0,
    BtnSetPsd=1,
};

/*!
 * \enum CalPsbl
 * Warning status	                                
 * == 0	Calibration status OK (Normal IHU behavior) 
 * == 1	Calibration not possible (All 4 rolling wheels shall be grey, menu item for recalibration of iTPMS system shall be unavailable and gray).
 */
enum class CalPsbl {
    CalPsbl=0,
    CalNoPsbl=1,
};

/*!
 * \enum CallSts
 * Carries call presence information. Defined for CallSts range [0-1]
 * 
 * 0 = Inactive. There is no active call present with any telephony solution in the system.
 * 1 = Active. Atleast one active call is present in the system.
 */
enum class CallSts {
    Inactive=0,
    Active=1,
};

/*!
 * \enum CarModSts1
 */
enum class CarModSts1 {
    CarModNorm=0,
    CarModTrnsp=1,
    CarModFcy=2,
    CarModCrash=3,
    CarModDyno=5,
};

/*!
 * \enum ChrgrWirelsSts
 * Signal for the current status of the Wireless Phone charger
 */
enum class ChrgrWirelsSts {
    Stopped=0,
    Charging=1,
    Missaligned=2,
    UnknownObj=3,
    Completed=4,
    SwError=5,
    HwError=6,
    Reserved=7,
};

/*!
 * \enum ClimaWarn
 * 0=No Warning
 * 1=Fuel Low
 * 2=Battery Low
 * 3=Fuel and Battery Low
 * 4=Temperature Low
 * 5=Temperature High
 */
enum class ClimaWarn {
    NoWarn=0,
    FuLo=1,
    BattLo=2,
    FuAndBattLo=3,
    TLo=4,
    THi=5,
    Error=6,
    HVError=7,
    ActvnLimd=8,
};

/*!
 * \enum ClkFmt
 * 24h or 12h indication of clock
 */
enum class ClkFmt {
    Hr24=0,
    Hr12=1,
    UkwnClkFmt=2,
};

/*!
 * \enum CllsnWarn1
 */
enum class CllsnWarn1 {
    NoWarn=0,
    WarnLvl1=1,
    WarnLvl2=2,
    Resd3=3,
};

/*!
 * \enum CmptmtTFrntQf
 * Interpretation:
 * SnsrDataUndefd - Accuracy of data undefinable
 * FanNotRunning - Aspiration fan not running
 * SnsrDataNotOk - Data accuracy not within specification
 * SnsrDataOk - Data is calculated within specified accuracy
 */
enum class CmptmtTFrntQf {
    SnsrDataUndefd=0,
    FanNotRunning=1,
    SnsrDataNotOk=2,
    SnsrDataOk=3,
};

/*!
 * \enum ConSftyWarnLvl1
 */
enum class ConSftyWarnLvl1 {
    NoWarn=0,
    LoLvl=1,
    MedLvl=2,
    HiLvl=3,
};

/*!
 * \enum CoolgReqForDisp
 * Interpretation:
 * NoReq - No active cooling requested
 * LoReq - Low cooling need
 * HiReq - High cooling need
 */
enum class CoolgReqForDisp {
    NoReq=0,
    LoReq=1,
    HiReq=2,
};

/*!
 * \enum CoolgStsForDisp
 * Interpretation:
 * NoCoolgActv - No cooling request received, no actions taken
 * CoolgLvlStrtd1 - Activated cooling in level 1 started
 * CoolgLvlStrtd2 - Activated cooling in level 2 started
 */
enum class CoolgStsForDisp {
    NoCoolgActv=0,
    CoolgLvlStrtd1=1,
    CoolgLvlStrtd2=2,
    Resd=3,
};

/*!
 * \enum CptEquid
 * Description if a component is equipped or not.
 */
enum class CptEquid {
    Equid=0,
    NotEquid=1,
};

/*!
 * \enum CrsCtrlrSts1
 */
enum class CrsCtrlrSts1 {
    Off=1,
    Stb=2,
    Actv=3,
};

/*!
 * \enum CtraIndcn1
 */
enum class CtraIndcn1 {
    NoCtraWarn=0,
    CtraWarn=1,
};

/*!
 * \enum CtrlDirOfTrfcLi1
 */
enum class CtrlDirOfTrfcLi1 {
    NoDirection=0,
    Straight=1,
    Left=2,
    LeftStraight=3,
    Right=4,
    StraightRight=5,
    LeftRight=6,
    All=7,
};

/*!
 * \enum CurtActvnTyp1
 */
enum class CurtActvnTyp1 {
    BtnReld=0,
    BtnPsd=1,
    Spare1=2,
    Spare2=3,
};

/*!
 * \enum DateFmt
 * DateFmt1 = YMD
 * DateFmt2 = DMY
 * DateFmt3 = MDY
 */
enum class DateFmt {
    DateFmt1=0,
    DateFmt2=1,
    DateFmt3=2,
    UkwnFmt=3,
};

/*!
 * \enum DateOrTi
 */
enum class DateOrTi {
    None=0,
    Date=1,
    Ti=2,
    DateTi=3,
};

/*!
 * \enum DevErrSts2
 */
enum class DevErrSts2 {
    NoFlt=0,
    Flt=1,
};

/*!
 * \enum DiagcForPanCenCtrl2
 */
enum class DiagcForPanCenCtrl2 {
    NoFlt=0,
    FanErr=1,
    OutdURng=2,
    TmrErr=3,
    MemErr=4,
    Spare6=5,
    Spare7=6,
    SnrFltT=7,
};

/*!
 * \enum DiagcForRCSM2
 */
enum class DiagcForRCSM2 {
    NoFlt=0,
    SpiErr=1,
    OutdURng=2,
    TmrErr=3,
    MemErr=4,
    ADErr=5,
    Spare1=6,
    Spare2=7,
};

/*!
 * \enum DiagcStsTypDMSM
 */
enum class DiagcStsTypDMSM {
    OK=0,
    CmnFailr=1,
    Spare1=2,
    Spare2=3,
};

/*!
 * \enum DispOfPrpsnMod3
 */
enum class DispOfPrpsnMod3 {
    NotRdy=0,
    ChrgnWithOnBdChrgr=1,
    PrpsnMotElecAndPrpsnEng=2,
    PrpsnMotElecAndPrpsnEngAndChrgn=3,
    OnlyEngPrpsn=4,
    OnlyEngPrpsnAndChrgn=5,
    OnlyPrpsnMotElec=6,
    NoPrpsnMotElecAndEngOff=7,
    NoPrpsnMotElecAndOnEng=8,
    NoPrpsnMotElecAndOnEngAndChrgn=9,
    BrkgRgnAndEngOff=10,
    BrkgRgnAndOnEng=11,
    BrkgRgnAndEngOnAndChrgn=12,
    NotOfUse2=13,
    NotOfUse3=14,
    NotOfUse4=15,
};

/*!
 * \enum DoorSts2
 * Status of the door.
 */
enum class DoorSts2 {
    Ukwn=0,
    Opend=1,
    Clsd=2,
};

/*!
 * \enum DrvModReqType1
 */
enum class DrvModReqType1 {
    Undefd=0,
    DrvMod1=1,
    DrvMod2=2,
    DrvMod3=3,
    DrvMod4=4,
    DrvMod5=5,
    DrvMod6=6,
    DrvMod7=7,
    DrvMod8=8,
    DrvMod9=9,
    DrvMod10=10,
    DrvMod11=11,
    DrvMod12=12,
    DrvMod13=13,
    DrvMod14=14,
    Err=15,
};

/*!
 * \enum DrvgSide1
 */
enum class DrvgSide1 {
    DrvgSideLe=0,
    DrvgSideRi=1,
};

/*!
 * \enum DrvrDesDir1
 * Describes the desired vehicle movement direction from the driver.
 * Contains only the drivers longitudinal direction intent. In which direction shall the car move.
 */
enum class DrvrDesDir1 {
    Undefd=0,
    Fwd=1,
    Rvs=2,
    Neut=3,
    Resd0=4,
    Resd1=5,
    Resd2=6,
    Resd3=7,
};

/*!
 * \enum DrvrDispSetg
 */
enum class DrvrDispSetg {
    Cmft=0,
    Eco=1,
    Dyn=2,
};

/*!
 * \enum DrvrHmiBackGndInfo
 */
enum class DrvrHmiBackGndInfo {
    DrvrHmiBackGndInfoSetg0=0,
    DrvrHmiBackGndInfoSetg1=1,
    DrvrHmiBackGndInfoSetg2=2,
    DrvrHmiBackGndInfoSetg3=3,
    DrvrHmiBackGndInfoSetg4=4,
    DrvrHmiBackGndInfoSetg5=5,
};

/*!
 * \enum DrvrHmiDispdMod1
 */
enum class DrvrHmiDispdMod1 {
    DispDftMod=0,
    DispAudMod=1,
    DispNavMod=2,
};

/*!
 * \enum DrvrHmiUsrIfSetg
 */
enum class DrvrHmiUsrIfSetg {
    UsrIfDft=0,
    UsrIfVrnt1=1,
    UsrIfVrnt2=2,
    UsrIfVrnt3=3,
    UsrIfVrnt4=4,
    UsrIfVrnt5=5,
    UsrIfVrnt6=6,
    UsrIfVrnt7=7,
    UsrIfVrnt8=8,
    UsrIfVrnt9=9,
};

/*!
 * \enum DrvrPfmncLvl2
 */
enum class DrvrPfmncLvl2 {
    Ukwn=0,
    DrvrLvl1=1,
    DrvrLvl2=2,
    DrvrLvl3=3,
    DrvrLvl4=4,
    DrvrLvl5=5,
};

/*!
 * \enum DrvrPfmncWarnReq1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name			Description
 * 0			0			NoWarn			No Warning			No Warning
 * 1			1			WarnForDrvrNotActv	Warning For Driver Not Active	Warning for Inattentive Driver
 * 2			2			WarnForDrvrTire		Warning For Driver Tire		Warning for Sleepy Driver
 * 3			3			NotInUse			Not In Use			Not used
 */
enum class DrvrPfmncWarnReq1 {
    NoWarn=0,
    WarnForDrvrNotActv=1,
    WarnForDrvrTire=2,
    NotInUse=3,
};

/*!
 * \enum DstLong
 */
enum class DstLong {
    km=0,
    miles=1,
    UkwnUnit=2,
};

/*!
 * \enum DstOfSnsr1
 */
enum class DstOfSnsr1 {
    DistanceInfinityOrNotUsed=0,
    Distance150cm=1,
    Distance146cm=2,
    Distance142cm=3,
    Distance138cm=4,
    Distance134cm=5,
    Distance130cm=6,
    Distance126cm=7,
    Distance122cm=8,
    Distance118cm=9,
    Distance114cm=10,
    Distance110cm=11,
    Distance106cm=12,
    Distance102m=13,
    Distance98cm=14,
    Distance94cm=15,
    Distance90cm=16,
    Distance86cm=17,
    Distance82cm=18,
    Distance78cm=19,
    Distance74cm=20,
    Distance70cm=21,
    Distance66cm=22,
    Distance62cm=23,
    Distance58cm=24,
    Distance54cm=25,
    Distance50cm=26,
    Distance46cm=27,
    Distance42cm=28,
    Distance38cm=29,
    Distance34cm=30,
    Distance30cm=31,
};

/*!
 * \enum DstSho
 */
enum class DstSho {
    Mtr=0,
    Ft=1,
    Yards=2,
    UkwnUnit=3,
};

/*!
 * \enum DstUnit
 */
enum class DstUnit {
    km=0,
    miles=1,
};

/*!
 * \enum EnableDisableCoding
 */
enum class EnableDisableCoding {
    Disabled=0,
    Enabled=1,
};

/*!
 * \enum EngCooltIndcn
 */
enum class EngCooltIndcn {
    MsgNoWarn=0,
    EngTHiToSpdRedn=1,
    EngTHiToStopSafe=2,
    EngTHiToEngStop=3,
    EngTHiToMan=4,
    CooltLvlLoToEngStop=5,
    CooltLvlLoToStopSafe=6,
    CooltLvlLoToMan=7,
    CooltLvlLoToLvlChk=8,
    MsgSrvRqrd=9,
};

/*!
 * \enum EngOilLvlSts1
 * Engine oil level warning information
 */
enum class EngOilLvlSts1 {
    OilLvlOk=0,
    OilLvlLo1=1,
    OilLvlLo2=2,
    OilLvlHi=3,
    OilLvlSrvRqrd=4,
    Resd=5,
};

/*!
 * \enum EngOilPWarn1
 * Engine oil pressure warning information
 */
enum class EngOilPWarn1 {
    EngOilPOk=0,
    EngOilPNotOk=1,
};

/*!
 * \enum EpbLampReqType1
 */
enum class EpbLampReqType1 {
    On=0,
    Off=1,
    Flash2=2,
    Flash3=3,
};

/*!
 * \enum Err1
 */
enum class Err1 {
    NoErr=0,
    Err=1,
};

/*!
 * \enum EscSt1
 * Information on activation/ deactivation of Electronic Stability Control (ESC)
 */
enum class EscSt1 {
    Inin=0,
    Ok=1,
    TmpErr=2,
    PrmntErr=3,
    UsrOff=4,
};

/*!
 * \enum EscWarnIndcnReq
 */
enum class EscWarnIndcnReq {
    EscWarnIndcnOnReq=0,
    EscWarnIndcnFlsgReq=1,
    Resd2=2,
    EscWarnIndcnOffReq=3,
};

/*!
 * \enum FailrNoFailr1
 */
enum class FailrNoFailr1 {
    NoFailr=0,
    Failr=1,
};

/*!
 * \enum FcnAvlSts1
 */
enum class FcnAvlSts1 {
    NoData=0,
    NotAvl=1,
    Avl=2,
};

/*!
 * \enum FctSts2
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			Ukwn		Unknown			Unknown
 * 1			1			Off		Off			Off
 * 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
 * 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
 * 4			4			NotAvl		Not Available		Unavailable
 * 5			5			SrvRqrd		Service Required		Service required
 * 6			6			On		On			On
 * 7			7			NotInUse		Not In Use		Not used
 */
enum class FctSts2 {
    Ukwn=0,
    Off=1,
    SpdLoStb=2,
    SpdHiStb=3,
    NotAvl=4,
    SrvRqrd=5,
    On=6,
    NotInUse=7,
};

/*!
 * \enum FldLvl
 * 0= High fluid level
 * 1= Low fluid level
 */
enum class FldLvl {
    FldLvlHi=0,
    FldLvlLo=1,
};

/*!
 * \enum Flg1
 * Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
 * 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
 */
enum class Flg1 {
    Rst=0,
    Set=1,
};

/*!
 * \enum FltCfmd1
 */
enum class FltCfmd1 {
    NoFltCfmd=0,
    FltCfmd=1,
};

/*!
 * \enum FltEgyCns1
 */
enum class FltEgyCns1 {
    NoFlt=0,
    Flt=1,
};

/*!
 * \enum FrntWiprLvrCmd1
 */
enum class FrntWiprLvrCmd1 {
    FrntWiprOff=0,
    FrntWiprSngStk=1,
    FrntWiprIntm=2,
    FrntWiprContnsLoSpd=3,
    FrntWiprContnsHiSpd=4,
};

/*!
 * \enum FuCnsIndcdUnit
 * LperHectoKiloM = L/100Km
 * KiloMPerL = Km/L
 * MilePerGallonEngland = MPG (UK)
 * MilePerGallonUsa = MPG (US)
 */
enum class FuCnsIndcdUnit {
    LperHectoKiloM=0,
    KiloMPerL=1,
    MilePerGallonEngland=2,
    MilePerGallonUsa=3,
    UkwnUnit=4,
};

/*!
 * \enum FuLvlLoIndcnToNav1
 */
enum class FuLvlLoIndcnToNav1 {
    Off=0,
    FuLvlLoIndcn=1,
    FuLvlLoWarn=2,
};

/*!
 * \enum GearIndcnTyp2
 */
enum class GearIndcnTyp2 {
    NoIndcn=0,
    Gear1=1,
    Gear2=2,
    Gear3=3,
    Gear4=4,
    Gear5=5,
    Gear6=6,
    Gear7=7,
    Gear8=8,
    Gear9=9,
    Gear10=10,
    Gear11=11,
    Gear2468=12,
    Gear13579=13,
    Neut=14,
    Rvs=15,
};

/*!
 * \enum GearShiftIndcn
 */
enum class GearShiftIndcn {
    NoShiftIndcn=0,
    ShiftUpIndcn=1,
    CoolShiftIndcn=2,
    ShiftDwnIndcn=3,
};

/*!
 * \enum GenQf1
 */
enum class GenQf1 {
    UndefindDataAccur=0,
    TmpUndefdData=1,
    DataAccurNotWithinSpcn=2,
    AccurData=3,
};

/*!
 * \enum GlbRst
 * Specifies global reset
 */
enum class GlbRst {
    NoRst=0,
    CarSetgRst=1,
    PrsnlSetgActvRst=2,
    PrsnlSetgAllRst=3,
    DataAndSetgAllRst=4,
    Resd1=5,
    Resd2=6,
    Resd3=7,
};

/*!
 * \enum HeatrPreCdngTyp
 */
enum class HeatrPreCdngTyp {
    NoReq=0,
    Aut=1,
    Fu=2,
    Elec=3,
};

/*!
 * \enum HmiCmptmtAirDistbnFrnt
 * Interpretation:
 * 0 - Flr - Floor
 * 1 - Vent - Vent
 * 2 - Defrst - Defrost
 * 3 - FlrDefrst - Floor / defrost
 * 4 - FlrVent - Floor / vent
 * 5 - VentDefrst - Vent / defrost
 * 6 - FlrVentDefrst - Floor / vent / defrost
 * 7 - Aut - Auto
 */
enum class HmiCmptmtAirDistbnFrnt {
    Flr=0,
    Vent=1,
    Defrst=2,
    FlrDefrst=3,
    FlrVent=4,
    VentDefrst=5,
    FlrVentDefrst=6,
    Aut=7,
};

/*!
 * \enum HmiCmptmtCoolgReq
 */
enum class HmiCmptmtCoolgReq {
    Off=0,
    Auto=1,
};

/*!
 * \enum HmiCmptmtTSpSpcl
 */
enum class HmiCmptmtTSpSpcl {
    Norm=0,
    Lo=1,
    Hi=2,
};

/*!
 * \enum HmiHvacFanLvl
 */
enum class HmiHvacFanLvl {
    Off=0,
    Min=1,
    LvlMan1=2,
    LvlMan2=3,
    LvlMan3=4,
    LvlMan4=5,
    LvlMan5=6,
    LvlMan6=7,
    LvlMan7=8,
    LvlAutMinusMinus=9,
    LvlAutMinus=10,
    LvlAutoNorm=11,
    LvlAutPlus=12,
    LvlAutPlusPlus=13,
    Max=14,
};

/*!
 * \enum HmiHvacReCtrl
 */
enum class HmiHvacReCtrl {
    Off=0,
    OffWithNoOccpt=1,
    On=2,
};

/*!
 * \enum HmiHvacRecircCmd
 * Interpretation:
 * 0 - OsaFull - Full OSA
 * 1 - RecircFull - Full recirculation
 * 2 - RecircFullWithTiOut - Full REC with timeout
 * 3 - AutWithAirQly - AUTO with AQS
 * 4 - Aut - Auto
 * 5 - Resd - Reserved
 */
enum class HmiHvacRecircCmd {
    Aut=0,
    AutWithAirQly=1,
    RecircFull=2,
};

/*!
 * \enum HudStsForHmi
 */
enum class HudStsForHmi {
    HudStsAvl=0,
    HudStsCalMod=1,
    HudStsTmpNotAvl=2,
    HudStsErr=3,
    Resd1=4,
    Resd2=5,
    Resd3=6,
};

/*!
 * \enum HvBattUsgBlkdTyp
 * HV battery usage function blocked types
 */
enum class HvBattUsgBlkdTyp {
    NotBlkd=0,
    BlkdByBattLvl=1,
    BlkdBySysFlt=2,
    NotOfUse=3,
};

/*!
 * \enum HvBattUsgType
 * Propulsion HV Battery usage mode type
 */
enum class HvBattUsgType {
    NoReq=0,
    Norm=1,
    Hld=2,
    HldSmt=3,
    Chrg=4,
    NotOfUse1=5,
    NotOfUse2=6,
    NotOfUse3=7,
};

/*!
 * \enum HznBrdg1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			SegNotPartOfBrdg	Segment Not Part Of Bridge	Segment is not part of a bridge.
 * 1			1			SegPartOfBrdg	Segment Part Of Bridge	Segment is part of a bridge.
 * 2			2			Ukwn		Unknown			Unknown
 */
enum class HznBrdg1 {
    SegNotPartOfBrdg=0,
    SegPartOfBrdg=1,
    Ukwn=2,
};

/*!
 * \enum HznIntscnCmplx1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name				Description
 * 0			0			SegNotPartOfIntscnCmplx	Segment Not Part Of Intersection Complex	Segment is not part of complex intersection
 * 1			1			SegPartOfIntscnCmplx	Segment Part Of Intersection Complex		Segment is part of a complex intersection
 * 2			2			Ukwn			Unknown					Unknown
 */
enum class HznIntscnCmplx1 {
    SegNotPartOfIntscnCmplx=0,
    SegPartOfIntscnCmplx=1,
    Ukwn=2,
};

/*!
 * \enum HznLanePrsnt2
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
 * 0			0			Ukwn		Unknown		Unknown
 * 1			1			EmgyLane	Emergency Lane	Emergency lane
 * 2			2			RoadLaneSng	Road Lane Single	Single-lane road
 * 3			3			LaneLe		Lane Left		Left-most lane
 * 4			4			LaneRi		Lane Right	Right-most lane
 * 5			5			LaneMid		Lane Middle	One of middle lanes on road with three or more lanes
 * 6			6			Resd		Reserved		Reserved
 * 7			7			Ukwn2		Unknown 2	Unknown 2
 */
enum class HznLanePrsnt2 {
    Ukwn=0,
    EmgyLane=1,
    RoadLaneSng=2,
    LaneLe=3,
    LaneRi=4,
    LaneMid=5,
    Resd=6,
    Ukwn2=7,
};

/*!
 * \enum HznMapSrc2
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
 * 0			0			Ukwn		Unknown		Unknown
 * 1			1			Src1		Source 1		NAVTEQ
 * 2			2			Src2		Source 2		TeleAtlas
 * 3			3			Src3		Source 3		Zenrin
 * 4			4			Resd1		Reserved 1	reserved
 * 5			5			Resd2		Reserved 2	reserved
 * 6			6			Resd3		Reserved 3	reserved
 * 7			7			Ukwn2		Unknown 2	Unknown 2
 */
enum class HznMapSrc2 {
    Ukwn=0,
    Src1=1,
    Src2=2,
    Src3=3,
    Resd1=4,
    Resd2=5,
    Resd3=6,
    Ukwn2=7,
};

/*!
 * \enum HznMsgTyp1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
 * 0			0			Resd1		Reserved 1	Reserved
 * 1			1			Posn		Position		POSITION
 * 2			2			Seg		Segment		SEGMENT
 * 3			3			Edge		Edge		STUB
 * 4			4			ProfSho		Profile Short	PROFILE SHORT
 * 5			5			ProfLong		Profile Long	PROFILE LONG
 * 6			6			Data		Data		META-DATA
 * 7			7			Resd2		Reserved 2	Reserved
 */
enum class HznMsgTyp1 {
    Resd1=0,
    Posn=1,
    Seg=2,
    Edge=3,
    ProfSho=4,
    ProfLong=5,
    Data=6,
    Resd2=7,
};

/*!
 * \enum HznNrOfLaneInDrvgDir1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name				Description
 * 0			0			WrgDirInRoadSngDir	Wrong Direction In Road Single Direction	One-direction road driven in wrong direction
 * 1			1			Lane1			Lane 1					1 lane
 * 2			2			Lane2			Lane 2					2 lanes
 * 3			3			Lane3			Lane 3					3 lanes
 * 4			4			Lane4			Lane 4					4 lanes
 * 5			5			Lane5			Lane 5					5 lanes
 * 6			6			LaneEquOrExcd6		Lane Equal Or Exceed 6			6 or more lanes
 */
enum class HznNrOfLaneInDrvgDir1 {
    WrgDirInRoadSngDir=0,
    Lane1=1,
    Lane2=2,
    Lane3=3,
    Lane4=4,
    Lane5=5,
    LaneEquOrExcd6=6,
    Ukwn=7,
};

/*!
 * \enum HznNrOfLaneInSecDir1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			RoadSngDir	Road Single Direction	One-direction road
 * 1			1			LaneSng		Lane Single		One lane
 * 2			2			LaneEquOrExcd2	Lane Equal Or Exceed 2	2 or more lanes
 */
enum class HznNrOfLaneInSecDir1 {
    RoadSngDir=0,
    LaneSng=1,
    LaneEquOrExcd2=2,
    Ukwn=3,
};

/*!
 * \enum HznPartOfCourseCalcd1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name			Description
 * 0			0			SegNotPartOfPahCalcd	Segment Not Part Of Path Calculated	Segment is not part of Calculated Route
 * 1			1			SegPartOfPahCalcd	Segment 	Part Of Path Calculated		Segment is part of Calculated Route
 * 2			2			Ukwn			Unknown				Unknown
 */
enum class HznPartOfCourseCalcd1 {
    SegNotPartOfPahCalcd=0,
    SegPartOfPahCalcd=1,
    Ukwn=2,
};

/*!
 * \enum HznProfLongTyp
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			NotInUse		Not In Use		N/A
 * 1			1			Lgt		Longitudinal		Longitude
 * 2			2			Lat		Lateral			Latitude
 * 3			3			Alti		Altitude			Altitude
 * 4			4			NodCtrlForLgt	Node Control For Longitudinal	(Bézier) Control Point Longitude
 * 5			5			NodCtrlForLat	Node Control For Lateral	(Bézier) Control Point Latitude
 * 6			6			NodCtrlForAlti	Node Control For Altitude	(Bézier) Control Point Altitude
 * 7			7			Id		Identifier			Link Identifier
 * 8			15								Reserved for standard types
 * 16			16			SpdLim1		Speed limits 1
 * 17			17			SpdLim2		Speed limits 2
 * 18			18			SpdLim3		Speed limits 3
 * 19			19			CamSpd		Speed camera
 * 20			20			RoadWInfo	Road Work Information
 * 21			21			TrfcLi		Traffic lights
 * 22			30								Reserved for system specific types
 * 31			31			Ukwn		Unknown
 */
enum class HznProfLongTyp {
    NotInUse=0,
    Lgt=1,
    Lat=2,
    Alti=3,
    NodCtrlForLgt=4,
    NodCtrlForLat=5,
    NodCtrlForAlti=6,
    Id=7,
    SpdLim1=16,
    SpdLim2=17,
    SpdLim3=18,
    CamSpd=19,
    RoadWInfo=20,
    TrfcLi=21,
    Ukwn=31,
};

/*!
 * \enum HznProfLongTypExtd1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			NotInUse		Not In Use		N/A
 * 1			1			Lgt		Longitudinal		Longitude
 * 2			2			Lat		Lateral			Latitude
 * 3			3			Alti		Altitude			Altitude
 * 4			4			NodCtrlForLgt	Node Control For Longitudinal	(Bézier) Control Point Longitude
 * 5			5			NodCtrlForLat	Node Control For Lateral	(Bézier) Control Point Latitude
 * 6			6			NodCtrlForAlti	Node Control For Altitude	(Bézier) Control Point Altitude
 * 7			7			Id		Identifier			Link Identifier
 * 8			15								Reserved for standard types
 * 16			16			SpdLim1		Speed limits 1
 * 17			17			SpdLim2		Speed limits 2
 * 18			18			SpdLim3		Speed limits 3
 * 19			19			CamSpd		Speed camera
 * 20			20			RoadWInfo	Road Work Information
 * 21			21			TrfcLi		Traffic lights
 * 22			30								Reserved for system specific types
 * 31			31			Ukwn		Unknown
 */
enum class HznProfLongTypExtd1 {
    NotInUse=0,
    Lgt=1,
    Lat=2,
    Alti=3,
    NodCtrlForLgt=4,
    NodCtrlForLat=5,
    NodCtrlForAlti=6,
    Id=7,
    SpdRng=16,
    AltiAvg=17,
    DestOrInterDest=18,
    Ukwn=31,
};

/*!
 * \enum HznProfShoTyp
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name			Description
 * 0			0			NotInUse			Not In Use			N/A
 * 1			1			Crvt			Curvature			Curvature
 * 2			2			PahNrTyp			Path Number Type			Route Number Types
 * 3			3			Slop1			Slop 1				Slope
 * 4			4			Slop2			Slop 2				Slope
 * 5			5			RoadAcs			Road Access			Road accessibility
 * 6			6			RoadCdn			Road Condition			Road condition
 * 7			7			PosnOfSpdSgnNotStat	Position Of Speed Sign Not Static	Variable Speed Sign Position
 * 8			8			ChgOfDir			Change Of Direction		Heading Change
 * 9			15										Reserved for standard types
 * 16			17										Reserved for system specific types
 * 18			18			TrfcSgn			Traffic Signs
 * 19			19										Reserved for system specific types
 * 20			20			CrashAr			Black Spot
 * 21			30										Reserved for system specific types
 * 31			31			Ukwn			Unknown
 */
enum class HznProfShoTyp {
    NotInUse=0,
    Crvt=1,
    PahNrTyp=2,
    Slop1=3,
    Slop2=4,
    RoadAcs=5,
    RoadCdn=6,
    PosnOfSpdSgnNotStat=7,
    ChgOfDir=8,
    TrfcSgn=18,
    CrashAr=20,
    Ukwn=31,
};

/*!
 * \enum HznProfTypQly1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
 * 0			0			HiQly		High Quality	means highest accuracy.
 * 1			1			NotInUse		Not In Use	Not used
 * 2			2			LoQly		Low Quality	mean lowest accuracy.
 * 3			3			UkwnQly		Unknown Quality	indicate that accuracy is unknown.
 */
enum class HznProfTypQly1 {
    HiQly=0,
    NotInUse=1,
    LoQly=2,
    UkwnQly=3,
};

/*!
 * \enum HznRiOfWay1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			RiOfWayOfHdPah	Right Of Way Of Head Path	Parent path has right-of-way over sub-path
 * 1			1			RiOfWayOfChdPah	Right Of Way Of Child Path	Sub-path has right-of-way over parent path
 * 2			2			Ukwn		Unknown			Unknown
 */
enum class HznRiOfWay1 {
    RiOfWayOfHdPah=0,
    RiOfWayOfChdPah=1,
    Ukwn=2,
};

/*!
 * \enum HznRoadLiAr1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name			Description
 * 0			0			SegNotPartOfTownOrCty	Segment Not Part Of Town Or City	Segment is not part of a built-up area.
 * 1			1			SegPartOfTownOrCty	Segment Part Of Town Or City	Segment is part of a built-up area.
 * 2			2			Ukwn			Unknown				Unknown
 */
enum class HznRoadLiAr1 {
    SegNotPartOfTownOrCty=0,
    SegPartOfTownOrCty=1,
    Ukwn=2,
};

/*!
 * \enum HznRoadMpl1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name			Description
 * 0			0			SegNotPartOfRoadMpl	Segment Not Part Of Road Multiple	Segment is not part of a divided road/dual carriageway.
 * 1			1			SegPartOfRoadMpl		Segment Part Of Road Multiple	Segment is part of a divided road/dual carriageway.
 * 2			2			Ukwn			Unknown				Unknown
 */
enum class HznRoadMpl1 {
    SegNotPartOfRoadMpl=0,
    SegPartOfRoadMpl=1,
    Ukwn=2,
};

/*!
 * \enum HznSpdLimEfc1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
 * 0			0			Ukwn		Unknown		Unknown
 * 1			1			Val5OrLowr	Value 5 Or Lower	<= 5 km/h [mph]
 * 2			2			Val7		Value 7		7 km/h [mph]
 * 3			3			Val10		Value 10		10 km/h [mph]
 * 4			4			Val15		Value 15		15 km/h [mph]
 * 5			5			Val20		Value 20		20 km/h [mph]
 * 6			6			Val25		Value 25		25 km/h [mph]
 * 7			7			Val30		Value 30		30 km/h [mph]
 * 8			8			Val35		Value 35		35 km/h [mph]
 * 9			9			Val40		Value 40		40 km/h [mph]
 * 10			10			Val45		Value 45		45 km/h [mph]
 * 11			11			Val50		Value 50		50 km/h [mph]
 * 12			12			Val55		Value 55		55 km/h [mph]
 * 13			13			Val60		Value 60		60 km/h [mph]
 * 14			14			Val65		Value 65		65 km/h [mph]
 * 15			15			Val70		Value 70		70 km/h [mph]
 * 16			16			Val75		Value 75		75 km/h [mph]
 * 17			17			Val80		Value 80		80 km/h [mph]
 * 18			18			Val85		Value 85		85 km/h [mph]
 * 19			19			Val90		Value 90		90 km/h [mph]
 * 20			20			Val95		Value 95		95 km/h [mph]
 * 21			21			Val100		Value 100		100 km/h [mph]
 * 22			22			Val105		Value 105		105 km/h [mph]
 * 23			23			Val110		Value 110		110 km/h [mph]
 * 24			24			Val115		Value 115		115 km/h [mph]
 * 25			25			Val120		Value 120		120 km/h [mph]
 * 26			26			Val130		Value 130		130 km/h [mph]
 * 27			27			Val140		Value 140		140 km/h [mph]
 * 28			28			Val150		Value 150		150 km/h [mph]
 * 29			29			Val160		Value 160		160 km/h [mph]
 * 30			30			NoLim		No Limit		Unlimited
 */
enum class HznSpdLimEfc1 {
    Ukwn=0,
    Val5OrLowr=1,
    Val7=2,
    Val10=3,
    Val15=4,
    Val20=5,
    Val25=6,
    Val30=7,
    Val35=8,
    Val40=9,
    Val45=10,
    Val50=11,
    Val55=12,
    Val60=13,
    Val65=14,
    Val70=15,
    Val75=16,
    Val80=17,
    Val85=18,
    Val90=19,
    Val95=20,
    Val100=21,
    Val105=22,
    Val110=23,
    Val115=24,
    Val120=25,
    Val130=26,
    Val140=27,
    Val150=28,
    Val160=29,
    NoLim=30,
};

/*!
 * \enum HznSpdLimTypEfc1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name			Description
 * 0			0			SecInfo		Second Information			Implicit (for instance, default speed limit in the cities)
 * 1			1			PrimInfoOnTrfcSgn	Primary Information On Traffic Sign	Explicit - on traffic sign
 * 2			2			PrimInfoByNight	Primary Information By Night		Explicit - by night
 * 3			3			PrimInfoByDay	Primary Information By Day		Explicit - by day
 * 4			4			PrimInfoByTiOfDay	Primary Information By Time Of Day	Explicit - time of day
 * 5			5			PrimInfoAtRain	Primary Information At Rain		Explicit - rain
 * 6			6			PrimInfoAtSnow	Primary Information At Snow		Explicit - snow
 * 7			7			Ukwn		Unknown				Unknown
 */
enum class HznSpdLimTypEfc1 {
    SecInfo=0,
    PrimInfoOnTrfcSgn=1,
    PrimInfoByNight=2,
    PrimInfoByDay=3,
    PrimInfoByTiOfDay=4,
    PrimInfoAtRain=5,
    PrimInfoAtSnow=6,
    Ukwn=7,
};

/*!
 * \enum HznSplyElectcSts1
 * EH (Electronic Horizon) provider status and support.
 */
enum class HznSplyElectcSts1 {
    Ukwn=0,
    NoSpprt=1,
    Failr=2,
    Spprt=3,
};

/*!
 * \enum HznTnl1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			SegNotPartOfTnl	Segment Not Part Of Tunnel	Segment is not part of a tunnel
 * 1			1			SegPartOfTnl	Segment Part Of Tunnel	Segment is part of a tunnel
 * 2			2			Ukwn		Unknown			Unknown
 */
enum class HznTnl1 {
    SegNotPartOfTnl=0,
    SegPartOfTnl=1,
    Ukwn=2,
};

/*!
 * \enum HznTypOfWay1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name		Description
 * 0			0			Ukwn			Unknown			Unknown
 * 1			1			FreeWay			Free Way			Freeway or Controlled Access road that is not a slip road/ramp
 * 2			2			RoadMpl			Raod Multiple		Multiple Carriageway or Multiply Digitized Road
 * 3			3			RoadSng			Road Single		Single Carriageway (default)
 * 4			4			Roty			Rotary			Roundabout Circle
 * 5			5			TrfcSq			Traffic Square		Traffic Square/Special Traffic Figure
 * 6			6			Resd1			Reserved 1		Reserved
 * 7			7			Resd2			Reserved 2		Reserved
 * 8			8			RampInEquDir		Ramp In Equal Direction	Parallel Road (as special type of a slip road/ramp)
 * 9			9			FreeWayRamp		Free Way Ramp		Slip Road/Ramp on a Freeway or Controlled Access road
 * 10			10			RampNotOnFreeWay	Ramp Not On Free Way	Slip Road/Ramp (not on a Freeway or Controlled Access road)
 * 11			11			RoadForSrvAndAcs		Road For Service And Access	Service Road or Frontage Road
 * 12			12			EntryToCarPark		Entry To Car Park		Entrance to or exit of a Car Park
 * 13			13			EntryToSrv		Entry To Service		Entrance to or exit to Service
 * 14			14			PedAr			Pedestrian Area		Pedestrian Zone
 */
enum class HznTypOfWay1 {
    Ukwn=0,
    FreeWay=1,
    RoadMpl=2,
    RoadSng=3,
    Roty=4,
    TrfcSq=5,
    Resd1=6,
    Resd2=7,
    RampInEquDir=8,
    FreeWayRamp=9,
    RampNotOnFreeWay=10,
    RoadForSrvAndAcs=11,
    EntryToCarPark=12,
    EntryToSrv=13,
    PedAr=14,
};

/*!
 * \enum IdPen
 * Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
 * 
 * Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
 * Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
 */
enum class IdPen {
    ProfUkwn=0,
    Prof1=1,
    Prof2=2,
    Prof3=3,
    Prof4=4,
    Prof5=5,
    Prof6=6,
    Prof7=7,
    Prof8=8,
    Prof9=9,
    Prof10=10,
    Prof11=11,
    Prof12=12,
    Prof13=13,
    Resd14=14,
    ProfAll=15,
};

/*!
 * \enum IndOfPrkgAutSts
 */
enum class IndOfPrkgAutSts {
    Off=0,
    PrkgTypAutSeln=1,
    PrkgTypPara=2,
    PrkgTypPerp=3,
    PrkgOutManv=4,
};

/*!
 * \enum IndcrSts1
 */
enum class IndcrSts1 {
    Off=0,
    LeOn=1,
    RiOn=2,
    LeAndRiOn=3,
};

/*!
 * \enum InterruptionSrc1
 */
enum class InterruptionSrc1 {
    Off=0,
    TiOut=1,
    ResuPsd=2,
    BrkPedl=3,
    AccrPedlPsd=4,
    Resd5=5,
    Resd6=6,
    Resd7=7,
};

/*!
 * \enum IntrLiInten
 */
enum class IntrLiInten {
    Off=0,
    IntenLo=1,
    IntenHi=2,
};

/*!
 * \enum KeyId1
 */
enum class KeyId1 {
    Key0=0,
    Key1=1,
    Key2=2,
    Key3=3,
    Key4=4,
    Key5=5,
    Key6=6,
    Key7=7,
    Key8=8,
    Key9=9,
    Key10=10,
    Key11=11,
};

/*!
 * \enum KeyLocn1
 * Data-Type for Key Search Location
 * KeyLocnAll  -  All zones
 * KeyLocnAllExt  -  All external zones
 * KeyLocnDrvrExt  -  External driver side
 * KeyLocnPassExt - External passenger side
 * KeyLocnTrExt - External behind trunk
 * KeyLocnAllInt - All internal zones
 * KeyLocnDrvrInt - Internal driver side
 * KeyLocnPassInt - Internal passenger side
 * KeyLocnResvInt - Internal backup reader (front central position)
 * KeyLocnResvIntSimple - Quick/simple internal backup reader request
 */
enum class KeyLocn1 {
    KeyLocnIdle=0,
    KeyLocnAll=1,
    KeyLocnAllExt=2,
    KeyLocnDrvrExt=3,
    KeyLocnPassExt=4,
    KeyLocnTrExt=5,
    KeyLocnAllInt=6,
    KeyLocnDrvrInt=7,
    KeyLocnPassInt=8,
    KeyLocnResvInt=9,
    KeyLocnResvIntSimple=10,
};

/*!
 * \enum KeyLostWarnIndcn
 */
enum class KeyLostWarnIndcn {
    KeyLostNoMsg=0,
    KeyMsgAndSoundLost=1,
    KeyMsgLost=2,
};

/*!
 * \enum LaneDetnSts1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			NoLaneDetd	No Lane Detedcted		No lane tracked
 * 1			1			LaneLeDetd	Lane Left Detected		Left lane marking tracked
 * 2			2			LaneRiDetd	Lane Right Detected	Right lane marking tracked
 * 3			3			LaneLeAndRiDetd	Lane Left And Right Detected	Left and right lane marking tracked
 */
enum class LaneDetnSts1 {
    NoLaneDetd=0,
    LaneLeDetd=1,
    LaneRiDetd=2,
    LaneLeAndRiDetd=3,
};

/*!
 * \enum LangTyp
 * Uknw = Unknown
 * Lang1 = Arabic
 * Lang2 = Bulgarian
 * Lang3 = Chinese Cantonese Traditional 
 * Lang4 = Chinese Mandarin Simplifed
 * Lang5 = Chinese Mandarin Traditional
 * Lang6 = Czech
 * Lang7 = Danish
 * Lang8 = Dutch
 * Lang9 = English AU
 * Lang10 = English UK
 * Lang11 = English US
 * Lang12 = Estonian
 * Lang13 = Finnish
 * Lang14 = Flemmish
 * Lang15 = French CAN
 * Lang16 = French EU
 * Lang17 = German
 * Lang18 = Greek
 * Lang19 = Hungarian
 * Lang20 = Italian
 * Lang21 = Japanese
 * Lang22 = Korean 
 * Lang23 = Latvian
 * Lang24 = Lithuanian
 * Lang25 = Norwegian
 * Lang26 = Polish
 * Lang27 = Portuguese BRA
 * Lang28 = Portuguese EU
 * Lang29 = Romanian
 * Lang30 = Russian
 * Lang31 = Slovak
 * Lang32 = Slovene
 * Lang33 = Spanish EU
 * Lang34 = Spanish US
 * Lang35 = Swedish
 * Lang36 = Thai
 * Lang37 = Turkish
 */
enum class LangTyp {
    Ukwn=0,
    Lang1=1,
    Lang2=2,
    Lang3=3,
    Lang4=4,
    Lang5=5,
    Lang6=6,
    Lang7=7,
    Lang8=8,
    Lang9=9,
    Lang10=10,
    Lang11=11,
    Lang12=12,
    Lang13=13,
    Lang14=14,
    Lang15=15,
    Lang16=16,
    Lang17=17,
    Lang18=18,
    Lang19=19,
    Lang20=20,
    Lang21=21,
    Lang22=22,
    Lang23=23,
    Lang24=24,
    Lang25=25,
    Lang26=26,
    Lang27=27,
    Lang28=28,
    Lang29=29,
    Lang30=30,
    Lang31=31,
    Lang32=32,
    Lang33=33,
    Lang34=34,
    Lang35=35,
    Lang36=36,
    Lang37=37,
};

/*!
 * \enum LcmaCtraOn
 */
enum class LcmaCtraOn {
    StrtUpOn=0,
    On=1,
    Off=2,
    TrlrOff=3,
};

/*!
 * \enum LiAutTran1
 * SntvyLo = Sensitivity Low, light transition happens later
 * SntvyLo = Sensitivity Norma 
 * SntvyLo = Sensitivity High, light transition happens earlier
 */
enum class LiAutTran1 {
    SnvtyLo=0,
    SnvtyNorm=1,
    SnvtyHi=2,
    Resd=3,
};

/*!
 * \enum LiTi2
 * Enumeration for intervals in tenths of seconds.
 * E.g 
 * 3 = 30 seconds
 * 4 = 40 seconds
 */
enum class LiTi2 {
    Sec0=0,
    Sec10=1,
    Sec20=2,
    Sec30=3,
    Sec40=4,
    Sec50=5,
    Sec60=6,
    Sec70=7,
    Sec80=8,
    Sec90=9,
    Sec100=10,
    Sec110=11,
    Sec120=12,
    Resd1=13,
    Resd2=14,
    Resd3=15,
};

/*!
 * \enum LiTrfcSide1
 * TrfcSideTmpLe = Traffic Side Temporary Left. Vehicle drives on left side of road
 * TrfcSideTmpRi =  Traffic Side Temporary Right. Vehicle drives on right side of road
 */
enum class LiTrfcSide1 {
    Off=0,
    TrfcSideTmpLe=1,
    TrfcSideTmpRi=2,
    Resd=3,
};

/*!
 * \enum LockSt2
 */
enum class LockSt2 {
    Undefd=0,
    Opend=1,
    Clsd=2,
    Lockd=3,
    Safe=4,
};

/*!
 * \enum LockSt3
 */
enum class LockSt3 {
    LockUndefd=0,
    LockUnlckd=1,
    LockTrUnlckd=2,
    LockLockd=3,
};

/*!
 * \enum LockSts2
 */
enum class LockSts2 {
    LockStsUkwn=0,
    Unlckd=1,
    Lockd=2,
    SafeLockd=3,
};

/*!
 * \enum LockTrigSrc2
 */
enum class LockTrigSrc2 {
    NoTrigSrc=0,
    KeyRem=1,
    Keyls=2,
    IntrSwt=3,
    SpdAut=4,
    TmrAut=5,
    Slam=6,
    Telm=7,
    Crash=8,
};

/*!
 * \enum Lvl2
 */
enum class Lvl2 {
    Std=0,
    Lo=1,
};

/*!
 * \enum Lvl30
 */
enum class Lvl30 {
    NoLvl=0,
    LoLvl=1,
    MidLvl=2,
    HiLvl=3,
};

/*!
 * \enum LvlCtrlSetgTyp
 */
enum class LvlCtrlSetgTyp {
    Cmft=0,
    Norm=1,
    Dyn=2,
    XC=3,
};

/*!
 * \enum LvlOfClimaCmft
 */
enum class LvlOfClimaCmft {
    Off=0,
    Lvl1=1,
    Lvl2=2,
    Lvl3=3,
    Lvl4=4,
    Lvl5=5,
    Lvl6=6,
    Lvl7=7,
};

/*!
 * \enum LvlWarn1
 */
enum class LvlWarn1 {
    NoWarn=0,
    Lvl1=1,
    Lvl2=2,
};

/*!
 * \enum ManAdpv1
 */
enum class ManAdpv1 {
    Man=0,
    Adpv=1,
};

/*!
 * \enum MassgIntenLvl
 */
enum class MassgIntenLvl {
    IntenLo=0,
    IntenNorm=1,
    IntenHi=2,
};

/*!
 * \enum MassgProgTyp
 */
enum class MassgProgTyp {
    Prog1=0,
    Prog2=1,
    Prog3=2,
    Prog4=3,
    Prog5=4,
};

/*!
 * \enum MirrDimLvlTyp
 */
enum class MirrDimLvlTyp {
    Normal=0,
    Dark=1,
    Light=2,
    Inhibit=3,
};

/*!
 * \enum MirrDwnStsTyp
 */
enum class MirrDwnStsTyp {
    MirrTiltUndefd=0,
    MirrUpPosn=1,
    MirrTiltPosn=2,
    MirrMovgToUpPosn=3,
    MirrMovgToTiltPosn=4,
};

/*!
 * \enum MirrFoldStsTyp
 */
enum class MirrFoldStsTyp {
    MirrFoldPosnUndefd=0,
    MirrNotFoldPosn=1,
    MirrFoldPosn=2,
    MirrMovgToNotFold=3,
    MirrMovgToFold=4,
};

/*!
 * \enum MmedHmiModStd2
 */
enum class MmedHmiModStd2 {
    InfModeOff=0,
    InfModeOn=1,
    InfModePartial=2,
    InfModeWelcome=3,
};

/*!
 * \enum MmedMaiPwrMod
 */
enum class MmedMaiPwrMod {
    IHUStateSleep=0,
    IHUStateStandby=1,
    IHUStatePartial=2,
    IHUStateOn=3,
};

/*!
 * \enum MmedTvmPwerMod
 */
enum class MmedTvmPwerMod {
    Sleep=0,
    Switch=1,
    Standby=2,
    TPEG=3,
    On=4,
};

/*!
 * \enum MtrlSnsrTFacQly
 */
enum class MtrlSnsrTFacQly {
    SnsrDataNotOK=0,
    SnsrDataOK=1,
};

/*!
 * \enum NoReqOffReq
 */
enum class NoReqOffReq {
    NoReq=0,
    OffReq=1,
};

/*!
 * \enum NoYes1
 */
enum class NoYes1 {
    No=0,
    Yes=1,
};

/*!
 * \enum NoYesCrit1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
 * 0			0			NotVld1		NotValid1		Not valid
 * 1			1			No		No		No
 * 2			2			Yes		Yes		Yes
 * 3			3			NotVld2		NotValid2		Not valid
 */
enum class NoYesCrit1 {
    NotVld1=0,
    No=1,
    Yes=2,
    NotVld2=3,
};

/*!
 * \enum NormSptType
 */
enum class NormSptType {
    Norm=0,
    Spt=1,
};

/*!
 * \enum NotAvlSts1
 */
enum class NotAvlSts1 {
    Avl=0,
    NotAvl=1,
};

/*!
 * \enum OffOnAut1
 */
enum class OffOnAut1 {
    Off=0,
    On=1,
    Aut=2,
};

/*!
 * \enum OffOnNotAvlSrvRqrd
 */
enum class OffOnNotAvlSrvRqrd {
    Off=0,
    On=1,
    NotAvl=2,
    SrvRqrd=3,
};

/*!
 * \enum OnOff1
 */
enum class OnOff1 {
    Off=0,
    On=1,
};

/*!
 * \enum OnOff2
 * PLILJESS:
 * Autosar AI Tables Standard DATA-TYPE.
 * Please consider using OnOff1 first hand to avoid confusion.
 */
enum class OnOff2 {
    On=0,
    Off=1,
};

/*!
 * \enum OnOffIdle1
 */
enum class OnOffIdle1 {
    Off=0,
    OffOrSpare1=1,
    On=2,
    OffOrSpare2=3,
};

/*!
 * \enum OnOffNoReq
 */
enum class OnOffNoReq {
    NoReq=0,
    On=1,
    Off=2,
};

/*!
 * \enum OnOffSafe1
 */
enum class OnOffSafe1 {
    OnOffSafeInvld1=0,
    OnOffSafeOn=1,
    OnOffSafeOff=2,
    OnOffSafeInvld2=3,
};

/*!
 * \enum PUnit
 */
enum class PUnit {
    Pa=0,
    Psi=1,
    Bar=2,
    Atm=3,
    UkwnUnit=4,
};

/*!
 * \enum ParkAssiLeRi1
 */
enum class ParkAssiLeRi1 {
    None=0,
    Left=1,
    Right=2,
    LeftAndRight=3,
};

/*!
 * \enum PassSeatSts1
 */
enum class PassSeatSts1 {
    Empty=0,
    Fmale=1,
    OccptLrg=2,
    Ukwn=3,
};

/*!
 * \enum PosnFromNavQly
 */
enum class PosnFromNavQly {
    QlyOfLvl0=0,
    QlyOfLvl1=1,
    QlyOfLvl2=2,
    QlyOfLvl3=3,
    QlyOfLvl4=4,
    QlyOfLvl5=5,
    QlyOfLvl6=6,
    QlyOfLvl7=7,
};

/*!
 * \enum PreClngNotif
 */
enum class PreClngNotif {
    NoReq=0,
    NoWarn=1,
    Done=2,
    Err=3,
    Intrpt=4,
    Spare1=5,
    Spare2=6,
    Spare3=7,
};

/*!
 * \enum PrioHighNormal
 */
enum class PrioHighNormal {
    PrioNormal=0,
    PrioHigh=1,
};

/*!
 * \enum PrkgAssiFailr2
 */
enum class PrkgAssiFailr2 {
    NoFailr=0,
    SgnlFailr=1,
    SnsrFailr=2,
    DgrSnsr=3,
};

/*!
 * \enum PrkgAssiSts3
 */
enum class PrkgAssiSts3 {
    SnsrAllOff=0,
    FourReSnsrActv=1,
    FourFrntFourReSnsrAct=2,
    SixFrntSnsrTwoReActv=3,
    Resd7=4,
    SnsrAllStb=5,
    Resd3=6,
    Resd4=7,
    Resd5=8,
    SnsrFailrAllOff=9,
    SnsrAllDeactvd=10,
    SnsrTrlrOff4=11,
    SnsrTrlrOff12=12,
    SixFrntSnsrAndSixReActv=13,
    Resd1=14,
    Resd2=15,
};

/*!
 * \enum PrkgAutSts3
 */
enum class PrkgAutSts3 {
    SAPNotActive=0,
    SAPPassiveScan=1,
    OverspeedSlowDown=2,
    ScanLeft=3,
    ScanRight=4,
    SAPPSlotFoundLeftGoForward=5,
    SAPPSlotFoundRightGoForward=6,
    PPASlotFoundLeftGoForward=7,
    PPASlotFoundRightGoForward=8,
    SAPStopToParkLeft=9,
    SAPStopToParkRight=10,
    PPAStopToParkLeft=11,
    PPAStopToParkRight=12,
    SAPPStopAndReverseGearLeft=13,
    SAPPStopAndReverseGearRight=14,
    PPAStopAndReverseGearLeft=15,
    PPAStopAndReverseGearRight=16,
    SAPPLookAroundLeft=17,
    SAPPLookAroundRight=18,
    PPALookAroundLeft=19,
    PPALookAroundRight=20,
    SAPPFirstMoveLeft=21,
    SAPPFirstMoveRight=22,
    PPAFirstMoveLeft=23,
    PPAFirstMoveRight=24,
    SAPPChangeToForwardLeft=25,
    SAPPChangeToForwardRight=26,
    PPAChangeToForwardLeft=27,
    PPAChangeToForwardRight=28,
    SAPPMoveForwardLeft=29,
    SAPPMoveForwardRight=30,
    PPAMoveForwardLeft=31,
    PPAMoveForwardRight=32,
    SAPPChangeToReverseLeft=33,
    SAPPChangeToReverseRight=34,
    PPAChangeToReverseLeft=35,
    PPAChangeToReverseRight=36,
    SAPPMoveRearwardLeft=37,
    SAPPMoveRearwardRight=38,
    PPAMoveRearwardLeft=39,
    PPAMoveRearwardRight=40,
    Finished=41,
    ParkingFail=42,
    Off=43,
    TemporaryNotAvailable=44,
    CancelHighSpeed=45,
    CancelSlipperyRoad=46,
    CancelTrailerConnect=47,
    CancelEngineStall=48,
    CancelSteringIntervention=49,
    CancelDriverDeactivation=50,
    CancelManyAttempts=51,
    CancelWrongGear=52,
    Resume=53,
    SelectParkOutSide=54,
    POAScannig=55,
    POAPathNotFound=56,
    POAChanageToForwardGearLeft=57,
    POAChanageToForwardGearRight=58,
    POAChanageToReverseGearLeft=59,
    POAChanageToReverseGearRight=60,
    POAStopAndChangeToForwardGearLeft=61,
    POAStopAndChangeToForwardGearRight=62,
    POAStopAndChangeToReverseGearLeft=63,
    POAStopAndChangeToReverseGearRight=64,
    POARearwardMoveLeft=65,
    POARearwardMoveRight=66,
    POAForwardMoveLeft=67,
    POAForwardMoveRight=68,
    POAFinishedGoLeft=69,
    POAFinishedGoRight=70,
    POALookAroundLeft=71,
    POALookAroundRight=72,
    POAFailr=73,
    EmgyBrkDsbld=74,
    EmgyBrkActv=75,
    EmgyBrkUsrActn=76,
};

/*!
 * \enum PrkgTypVld1
 */
enum class PrkgTypVld1 {
    NoPrkgDetd=0,
    ParaPrkgDetd=1,
    PerpPrkgDetd=2,
    ParaAndPerpPrkgDetd=3,
};

/*!
 * \enum PrpsnDrvMod
 */
enum class PrpsnDrvMod {
    Undefd=0,
    DrvMod1=1,
    DrvMod2=2,
    DrvMod3=3,
    DrvMod4=4,
    DrvMod5=5,
    DrvMod6=6,
    DrvMod7=7,
    DrvMod8=8,
    DrvMod9=9,
    DrvMod10=10,
    DrvMod11=11,
    DrvMod12=12,
    DrvMod13=13,
    DrvMod14=14,
    Err=15,
};

/*!
 * \enum PsdNotPsd
 */
enum class PsdNotPsd {
    NotPsd=0,
    Psd=1,
};

/*!
 * \enum PtDrvrModSetg2
 */
enum class PtDrvrModSetg2 {
    Undefd=0,
    Cmft=1,
    Eco=2,
    Dyn=3,
    Pure=4,
    Hyb=5,
    Pwr=6,
    AWD=7,
};

/*!
 * \enum PtGearAct1
 */
enum class PtGearAct1 {
    Neut=0,
    Gear1=1,
    Gear2=2,
    Gear3=3,
    Gear4=4,
    Gear5=5,
    Gear6=6,
    Gear7=7,
    Gear8=8,
    Gear9=9,
    Gear10=10,
    Gear11=11,
    Gear12=12,
    Gear13=13,
    Ukwn=14,
    Rvs=15,
};

/*!
 * \enum PwrSplyErrSts1
 * Status of power supply components
 */
enum class PwrSplyErrSts1 {
    SysOk=0,
    AltFltMecl=1,
    AltFltElec=2,
    AltFltT=3,
    AltFltCom=4,
    UHiDurgDrvg=5,
    BattSwtFltCom=6,
    BattSwtHwFlt=7,
    ULoDurgDrvg=8,
    BattSnsrFltCom=9,
    BattSnsrHwFlt=10,
    SpprtBattFltChrgn=11,
    FltComDcDc=12,
    FltElecDcDc=13,
    FltTDcDc=14,
};

/*!
 * \enum Qf1
 */
enum class Qf1 {
    DevOfDataUndefd=0,
    DataTmpUndefdAndEvlnInProgs=1,
    DevOfDataNotWithinRngAllwd=2,
    DataCalcdWithDevDefd=3,
};

/*!
 * \enum Qly2
 * Main usage shall be to indicate quality for vehicle state estimations, not to be used for failure code setting and function degradation unless absolutely necassary.
 */
enum class Qly2 {
    Flt=0,
    NoInfo=1,
    Vld=2,
};

/*!
 * \enum Qly3
 * General DATA-TYPE to indicate quality. Each quality deviation value, De0-De7, shall be specified on the DATA-ELEMENT using this DATA-TYPE.
 */
enum class Qly3 {
    De0=0,
    De1=1,
    De2=2,
    De3=3,
    De4=4,
    De5=5,
    De6=6,
    De7=7,
};

/*!
 * \enum QualityFactor
 */
enum class QualityFactor {
    QfUndefd=0,
    QfInProgs=1,
    QfNotSpc=2,
    QfSnsrDataOk=3,
};

/*!
 * \enum ReqSrc1
 */
enum class ReqSrc1 {
    NoReq=0,
    TtgReq=1,
    Resd2=2,
    Resd3=3,
    Resd4=4,
    Resd5=5,
    Resd6=6,
    Resd7=7,
};

/*!
 * \enum RngbdIllmnCmd1
 * For running board illumination setting value
 */
enum class RngbdIllmnCmd1 {
    Di=0,
    Aut=1,
    Resd2=2,
    Resd3=3,
};

/*!
 * \enum RoadFricWarnReq1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name		Description
 * 0			0			NoWarn		No Warning		No warning
 * 1			1			FricLoPsbl	Friction 	Low Possible		Potentially low friction
 * 2			2			FricLoCfmd	Friction Low Confirmed	Confirmed low friction
 */
enum class RoadFricWarnReq1 {
    NoWarn=0,
    FricLoPsbl=1,
    FricLoCfmd=2,
};

/*!
 * \enum RotyDirUI
 */
enum class RotyDirUI {
    Idle=0,
    CW=1,
    CCW=2,
    Failr=3,
};

/*!
 * \enum RotyDirUI1
 * Signal for CCSM rotary knob
 */
enum class RotyDirUI1 {
    Idle=0,
    CW=1,
    CCW=2,
    Spare=3,
};

/*!
 * \enum RsdsMstSt
 */
enum class RsdsMstSt {
    Cfg=0,
    StrtUp=1,
    Runng=2,
    Blkd=3,
    Faulty=4,
    Shutdown=5,
    Hot=6,
    Cal=7,
};

/*!
 * \enum SatltPosnSts
 * 0: No navigation solution
 * 1: 3 satelites solution (2D)
 * 2: more than 3 satellites solution (3D)
 * 3: CSF (Cold Start Fix)
 */
enum class SatltPosnSts {
    PosnStsOfLvl1=0,
    PosnStsOfLvl2=1,
    PosnStsOfLvl3=2,
    PosnStsOfLvl4=3,
};

/*!
 * \enum SeatActvSpplFct1
 */
enum class SeatActvSpplFct1 {
    NotAvl=0,
    LumExtnAndLumHei=1,
    BackBlster=2,
    CushExtn=3,
    HdrestHeiAndHdrestTilt=4,
    MassgFct=5,
};

/*!
 * \enum SeatClimaLvl
 */
enum class SeatClimaLvl {
    Off=0,
    Lvl1=1,
    Lvl2=2,
    Lvl3=3,
};

/*!
 * \enum SeatHeatDurgClimaEnad
 */
enum class SeatHeatDurgClimaEnad {
    SeatHeatOff=0,
    SeatDrvOn=1,
    SeatPassOn=2,
    SeatDrvrAndPass=3,
};

/*!
 * \enum SetVol
 * 0 = No max volume set
 * 1 = Max volume is set. Sound is muted
 */
enum class SetVol {
    SoundNotSet=0,
    SoundLo=1,
};

/*!
 * \enum SftyCchCntxt1
 */
enum class SftyCchCntxt1 {
    NoInfo=0,
    Fog=1,
    Rain=2,
    Darkness=3,
    DenseTrfc=4,
    FricLo=5,
    HighwayDrvg=6,
    CtyDrvg=7,
    RoadW=8,
    Misc9=9,
    Misc10=10,
    Misc11=11,
    Misc12=12,
    Misc13=13,
    Misc14=14,
    Misc15=15,
};

/*!
 * \enum SftyCchFb1
 */
enum class SftyCchFb1 {
    NoMsg=0,
    Msg1=1,
    Msg2=2,
    Msg3=3,
    Msg4=4,
    Msg5=5,
    Msg6=6,
    Msg7=7,
    Msg8=8,
    Msg9=9,
    Msg10=10,
    Msg11=11,
    Msg12=12,
    Msg13=13,
    Msg14=14,
    Msg15=15,
    Msg16=16,
    Msg17=17,
    Msg18=18,
    Msg19=19,
    Msg20=20,
    Msg21=21,
    Msg22=22,
    Msg23=23,
    Msg24=24,
    Msg25=25,
    Msg26=26,
    Msg27=27,
    Msg28=28,
    Msg29=29,
    Msg30=30,
    Msg31=31,
    Msg32=32,
    Msg33=33,
    Msg34=34,
    Msg35=35,
    Msg36=36,
    Msg37=37,
    Msg38=38,
    Msg39=39,
    Msg40=40,
    Msg41=41,
    Msg42=42,
    Msg43=43,
    Msg44=44,
    Msg45=45,
    Msg46=46,
    Msg47=47,
    Msg48=48,
    Msg49=49,
    Msg50=50,
    Msg51=51,
    Msg52=52,
    Msg53=53,
    Msg54=54,
    Msg55=55,
    Msg56=56,
    Msg57=57,
    Msg58=58,
    Msg59=59,
    Msg60=60,
    Msg61=61,
    Msg62=62,
    Msg63=63,
};

/*!
 * \enum SftyCchPostTripSts1
 */
enum class SftyCchPostTripSts1 {
    NoData=0,
    Drvg=1,
    PostTrip=2,
};

/*!
 * \enum SlowNormFast
 */
enum class SlowNormFast {
    Ukwn=0,
    Slow=1,
    Norm=2,
    Fast=3,
};

/*!
 * \enum Snvty1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
 * 0			0			NotInUse		Not In Use	Not used
 * 1			1			LoSnvty		Low Sensitivity	Low Sensitivity
 * 2			2			NormSnvty	Normal Sensitivity	Normal Sensitivity
 * 3			3			HiSnvty		High Sensitivity	High Sensitivity
 */
enum class Snvty1 {
    NotInUse=0,
    LoSnvty=1,
    NormSnvty=2,
    HiSnvty=3,
};

/*!
 * \enum Snvty4
 */
enum class Snvty4 {
    CmftMod=0,
    NormMod=1,
    SptMod=2,
    CrvtMod=3,
};

/*!
 * \enum SoundHptc1
 */
enum class SoundHptc1 {
    Sound=0,
    Hptc=1,
};

/*!
 * \enum SpdLimWarnReq1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name			Description
 * 0			0			NoWarn			No Warning			No warning
 * 1			1			SpdHi			Speed High			Too high speed compared to speed limit
 * 2			2			SpdHiAndSpdCtrlPassd	Speed High And Speed Control Passed	Too high speed and passing speed camera
 */
enum class SpdLimWarnReq1 {
    NoWarn=0,
    SpdHi=1,
    SpdHiAndSpdCtrlPassd=2,
};

/*!
 * \enum SpdUnit1
 * Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
 * 0			0			KiloMtrPerHr	Kilo Meter Per Hour	km/h
 * 1			1			MilePerHr		Mile Per Hour	mph
 */
enum class SpdUnit1 {
    KiloMtrPerHr=0,
    MilePerHr=1,
};

/*!
 * \enum SrvSts
 * Used be Connected service booking to inform Driver information about the current status of a service booking.
 */
enum class SrvSts {
    NoSrv=0,
    Srv=1,
    CfmdSrv=2,
    SrvPassd=3,
};

/*!
 * \enum SteerAsscLvl
 */
enum class SteerAsscLvl {
    Ukwn=0,
    Lvl1=1,
    Lvl2=2,
    Lvl3=3,
    Lvl4=4,
    Resd5=5,
    Resd6=6,
    Resd7=7,
};

/*!
 * \enum SteerMod
 * Mod1 = Comfort
 * Mod2 = Dynamic/Sport
 * Mod3 = Reserved
 * Mod4 = Reserved
 */
enum class SteerMod {
    Ukwn=0,
    Mod1=1,
    Mod2=2,
    Mod3=3,
    Mod4=4,
    Resd5=5,
    Resd6=6,
    Resd7=7,
};

/*!
 * \enum SteerStsForAutDrv3
 */
enum class SteerStsForAutDrv3 {
    Off=0,
    Stb=1,
    Actv=2,
    OvrdByDrvr=3,
    PndStb=4,
};

/*!
 * \enum SteerWarnReqForAutDrv1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name				Description
 * 0			0			NoWarn			No Warning				No warning
 * 1			1			DispWarnForDrvrSteerLe	Display Warning For Driver Steering Left	Show warning that driver shall steer left
 * 2			2			DispWarnForDrvrSteerRi	Display Warning For Driver Steering Right	Show warning that driver shall steer right
 * 3			3			DispGenWarn		Display Generic Warning			Show general warning
 */
enum class SteerWarnReqForAutDrv1 {
    NoWarn=0,
    DispWarnForDrvrSteerLe=1,
    DispWarnForDrvrSteerRi=2,
    DispGenWarn=3,
};

/*!
 * \enum SteerWhlHeatgOnCmdTyp
 */
enum class SteerWhlHeatgOnCmdTyp {
    Off=0,
    Lo=1,
    Med=2,
    Hi=3,
};

/*!
 * \enum StrtInProgs1
 */
enum class StrtInProgs1 {
    StrtStsOff=0,
    StrtStsImminent=1,
    StrtStsStrtng=2,
    StrtStsRunng=3,
};

/*!
 * \enum StrtStopReq1
 * Used for sending a Start or a Stop/Cancel request from HMI to a core function
 */
enum class StrtStopReq1 {
    Idle=0,
    StrtReq=1,
    StopReq=2,
    Resd=3,
};

/*!
 * \enum SurrndgsLiLvl
 * 0 = Full
 * 1 = Reduced
 */
enum class SurrndgsLiLvl {
    LvlFull=0,
    LvlReduced=1,
};

/*!
 * \enum SuspCtrlSetgTyp
 */
enum class SuspCtrlSetgTyp {
    Cmft=0,
    Norm=1,
    Dyn=2,
};

/*!
 * \enum SwtHozlSts1
 */
enum class SwtHozlSts1 {
    Idle=0,
    Fwd=1,
    Backw=2,
};

/*!
 * \enum SwtPush
 */
enum class SwtPush {
    NotPsd=0,
    Psd=1,
};

/*!
 * \enum SwtVertSts1
 */
enum class SwtVertSts1 {
    Idle=0,
    Up=1,
    Dwn=2,
};

/*!
 * \enum TelmSts
 */
enum class TelmSts {
    NoErr=0,
    TelmErr=1,
    TelmDi=2,
    TelmEnd=3,
};

/*!
 * \enum TirePAbsltValQf
 */
enum class TirePAbsltValQf {
    Norm=0,
    OldVal=1,
    UpdVal=2,
    SnsrFlt=3,
};

/*!
 * \enum TirePCalSts2
 */
enum class TirePCalSts2 {
    Norm=0,
    Cal=1,
    CalNotCmpl=2,
    CalCmpl=3,
};

/*!
 * \enum TirePMonSts1
 */
enum class TirePMonSts1 {
    NoWarn=0,
    SysFlt=1,
    FourMiss=2,
    Resd1=3,
    GmnSoftWarn=4,
    GmnHardWarn=5,
    Resd2=6,
    Resd3=7,
    Resd4=8,
    Resd5=9,
};

/*!
 * \enum TirePPosn
 */
enum class TirePPosn {
    NoPReadingOrPReadingOkWithOutPosn=0,
    PReadingWithPosn=1,
};

/*!
 * \enum TirePWarn
 */
enum class TirePWarn {
    NoWarn=0,
    SoftWarn=1,
    HardWarn=2,
    ResdWarn=3,
};

/*!
 * \enum Trig1
 */
enum class Trig1 {
    NoTrig=0,
    Trig=1,
};

/*!
 * \enum TripModSeln
 * Selects trip trip meter type
 * Trip1 - Reset by driver
 * Trip2 - Reset by timer (new trip condition)
 * Resd - Reserved for future Trip indication modes
 */
enum class TripModSeln {
    Ukwn=0,
    Trip1=1,
    Trip2=2,
    Resd=3,
};

/*!
 * \enum TrlrLampChkSts1
 * 0 = Trailer Lamp Check not possible to execute
 * 1 = Trailer Lamp Check possible to execute
 */
enum class TrlrLampChkSts1 {
    TrlrLampChkNotAvl=0,
    TrlrLampChkAvl=1,
};

/*!
 * \enum TrlrLampChkSts2
 * 0 = No status
 * 1 = Trailer Lamp Check started
 * 2 = Trailer Lamp Check completed
 * 3 = Operation canceled
 * 4 = Trailer Lamp Check requesting engine off to driver
 * 5 = Reserved
 * 6 = Reserved
 * 7 = Reserved
 */
enum class TrlrLampChkSts2 {
    Idle=0,
    TrlrLampChkStrtd=1,
    TrlrLampChkCmpl=2,
    CnclOper=3,
    TrlrLampEngOffReqd=4,
    Resd1=5,
    Resd2=6,
    Resd3=7,
};

/*!
 * \enum TrlrM
 */
enum class TrlrM {
    Lvl0=0,
    Lvl1=1,
    Lvl2=2,
    Lvl3=3,
};

/*!
 * \enum TrlrPrsntSts
 * 0 = Trailer unconnected
 * 1 = Trailer connected to the vehicle
 */
enum class TrlrPrsntSts {
    TrlrNotPrsnt=0,
    TrlrPrsnt=1,
};

/*!
 * \enum TurnSpdWarnReq1
 * Implementation Value Min	Implementation Value Max	Physical Value		Long name			Description
 * 0			0			NoWarn			No Warning			No warning
 * 1			1			WarnForCmngCrvtLe	Warning For Coming Curvature Left	Warning for upcoming left curve
 * 2			2			WarnForCmngCrvtRi	Warning For Coming Curvature Right	Warning for upcoming right curve
 */
enum class TurnSpdWarnReq1 {
    NoWarn=0,
    WarnForCmngCrvtLe=1,
    WarnForCmngCrvtRi=2,
};

/*!
 * \enum TwliBriSts1
 */
enum class TwliBriSts1 {
    Night=0,
    Day=1,
};

/*!
 * \enum Typ1
 * Basic type list
 * 8 types
 */
enum class Typ1 {
    Typ0=0,
    Typ1=1,
    Typ2=2,
    Typ3=3,
    Typ4=4,
    Typ5=5,
    Typ6=6,
    Typ7=7,
};

/*!
 * \enum UnlckKeylsCfg2
 */
enum class UnlckKeylsCfg2 {
    UnlckCen=0,
    UnlckInd=1,
};

/*!
 * \enum UnlckRemCfg1
 */
enum class UnlckRemCfg1 {
    UnlckCen=0,
    UnlckStep2=1,
};

/*!
 * \enum UsgModSts1
 * UsageMode Data
 */
enum class UsgModSts1 {
    UsgModAbdnd=0,
    UsgModInActv=1,
    UsgModCnvinc=2,
    UsgModActv=11,
    UsgModDrvg=13,
};

/*!
 * \enum UsrSwtDispTUnit
 */
enum class UsrSwtDispTUnit {
    Celcius=0,
    Farenheit=1,
};

/*!
 * \enum VehActv1
 * 0  No text
 * 1  Engine is running
 * 2  Ignition on
 * 3  TBD
 */
enum class VehActv1 {
    NoMsg=0,
    Msg1=1,
    Msg2=2,
    Msg3=3,
};

/*!
 * \enum VehMtnSt2
 */
enum class VehMtnSt2 {
    Ukwn=0,
    StandStillVal1=1,
    StandStillVal2=2,
    StandStillVal3=3,
    RollgFwdVal1=4,
    RollgFwdVal2=5,
    RollgBackwVal1=6,
    RollgBackwVal2=7,
};

/*!
 * \enum VehSpdIndcdUnit
 */
enum class VehSpdIndcdUnit {
    Kmph=0,
    Mph=1,
    UkwnUnit=2,
};

/*!
 * \enum VisWarnReq1
 */
enum class VisWarnReq1 {
    Off=0,
    Countdown=1,
    LeaveNow=2,
};

/*!
 * \enum VisnImgAgWideInUse
 */
enum class VisnImgAgWideInUse {
    NoImg=0,
    Img1=1,
    Img2=2,
    Img3=3,
    Img4=4,
    Img5=5,
    Img6=6,
    Img7=7,
    Img8=8,
    Img9=9,
    Img10=10,
    Img11=11,
    Img12=12,
    Img13=13,
    Img14=14,
    Img15=15,
};

/*!
 * \enum VolUnit
 * Gallon 1 = UK gallons
 * Gallon 2 = US gallons
 */
enum class VolUnit {
    litre=0,
    gallon1=1,
    gallon2=2,
    UkwnUnit=3,
};

/*!
 * \enum Warn2
 */
enum class Warn2 {
    NoWarn=0,
    Warn=1,
};

/*!
 * \enum WarnAndIntv1
 */
enum class WarnAndIntv1 {
    WarnAndIntv=0,
    Intv=1,
    Warn=2,
};

/*!
 * \enum WarnDir1
 */
enum class WarnDir1 {
    NoWarn=0,
    LeftSide=1,
    OuterLeft=2,
    InnerLeft=3,
    InnerRight=4,
    OuterRight=5,
    RightSide=6,
};

/*!
 * \enum WarnLeRi
 */
enum class WarnLeRi {
    NoWarn=0,
    WarnLe=1,
    WarnRi=2,
};

/*!
 * \enum WarnLeRi1
 */
enum class WarnLeRi1 {
    NoWarn=0,
    WarnLe=1,
    WarnRi=2,
};

/*!
 * \enum WinAndRoofAndCurtPosnTyp
 */
enum class WinAndRoofAndCurtPosnTyp {
    PosnUkwn=0,
    ClsFull=1,
    PercOpen4=2,
    PercOpen8=3,
    PercOpen12=4,
    PercOpen16=5,
    PercOpen20=6,
    PercOpen24=7,
    PercOpen28=8,
    PercOpen32=9,
    PercOpen36=10,
    PercOpen40=11,
    PercOpen44=12,
    PercOpen48=13,
    PercOpen52=14,
    PercOpen56=15,
    PercOpen60=16,
    PercOpen64=17,
    PercOpen68=18,
    PercOpen72=19,
    PercOpen76=20,
    PercOpen80=21,
    PercOpen84=22,
    PercOpen88=23,
    PercOpen92=24,
    PercOpen96=25,
    OpenFull=26,
    Resd1=27,
    Resd2=28,
    Resd3=29,
    Resd4=30,
    Movg=31,
};

/*!
 * \enum WipgSpdInfo
 * The signal reflects the actual wiping speed or mode regardless of activation source. Both rain sensor and wiper stalk info are used to create the signal. When in interval mode the wipes are carried out in low speed.
 */
enum class WipgSpdInfo {
    Off=0,
    IntlLo=1,
    IntlHi=2,
    WipgSpd4045=3,
    WipgSpd4650=4,
    WipgSpd5155=5,
    WipgSpd5660=6,
    WiprErr=7,
};

/*!
 * \enum WiprFrntSrvModReq
 */
enum class WiprFrntSrvModReq {
    NoActn=0,
    ActvtSrvPosn=1,
    DeActvtSrvPosn=2,
};

/*!
 * \enum YesNo2
 */
enum class YesNo2 {
    No=0,
    Yes=1,
};

/*!
 * \enum iTPMSTirePCalSts
 * Calibration status
 */
enum class iTPMSTirePCalSts {
    NoCalReq=0,
    CalOn=1,
    CalCmpl=2,
    CalFaild=3,
    Resd1=4,
    Resd2=5,
    Resd3=6,
};

/*!
 * \enum iTPMSTirePMSts
 * iTPMS system status and warnings
 */
enum class iTPMSTirePMSts {
    NoWarn=0,
    CmnWarn=1,
    WarnFL=2,
    WarnFR=3,
    WarnRL=4,
    WarnRR=5,
    SysUnAvi=6,
    SysFailr=7,
};

/*!
 * \struct ADataRawSafe1
 */
struct ADataRawSafe1 {
    double ALat; /*!<Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )*/
    Qf1 ALat1Qf;
    double ALgt; /*!<Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )*/
    Qf1 ALgt1Qf;
    double AVert; /*!<Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )*/
    Qf1 AVertQf;
    uint8_t Chks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Cntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct ActvnOfCrsEco
 */
struct ActvnOfCrsEco {
    OnOff1 Sts;
    IdPen Pen;
};

/*!
 * \struct AdprTurnSpdModPen1
 */
struct AdprTurnSpdModPen1 {
    AdprTurnSpdMod2 Sts;
    IdPen Pen;
};

/*!
 * \struct AgDataRawSafe1
 */
struct AgDataRawSafe1 {
    double RollRate; /*!<Unit: rad/s,  Range:-6->6, Resolution: (0.000244140625*x+0.0, raw is signed, 16 bits )*/
    Qf1 RollRateQf;
    double YawRate; /*!<Unit: rad/s,  Range:-6->6, Resolution: (0.000244140625*x+0.0, raw is signed, 16 bits )*/
    Qf1 YawRateQf;
    uint8_t Chks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Cntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct AmbTIndcdWithUnit
 * Indicated ambient temp with indicated unit and QF
 */
struct AmbTIndcdWithUnit {
    double AmbTIndcd; /*!<Unit: Unitless,  Range:-100->309.5, Resolution: (0.1*x+-100.0, raw is unsigned, 12 bits )*/
    AmbTIndcdUnit AmbTIndcdUnit_;
    GenQf1 QF;
};

/*!
 * \struct AmbTWithQly
 * The door module with a connected ambient temp sensor will send a temp value and QF, the door module without a connected ambient temp sensor will transmitt a temp value of 0x00 and QF = 0x00.
 */
struct AmbTWithQly {
    double AmbTVal; /*!<Unit: degC,  Range:-70->134.7, Resolution: (0.1*x+-70.0, raw is unsigned, 11 bits )*/
    GenQf1 Qly;
};

/*!
 * \brief array Array8ByteU8
 */
using Array8ByteU8 = std::array<uint8_t,8>;

/*!
 * \struct AsyLaneChg1
 */
struct AsyLaneChg1 {
    AsyLaneChgTyp1 Typ;
    AsyLaneChgSts1 Sts;
    AsyLaneChgPsbl1 Psbl;
};

/*!
 * \struct AutnmsPrkgActvScenario
 */
struct AutnmsPrkgActvScenario {
    AutnmsPrkgActvSt APActiveState;
    AutnmsPrkgActvPrkgSide APActiveParkingSide;
    AutnmsPrkgActvSlotTyp APActiveSlotType;
    AutnmsPrkgActvPrkgScenario APActiveParkingScenario;
    AutnmsPrkgActvPrkgDir APActiveParkingDirection;
    AutnmsPrkgActvRem APActiveRemote;
    AutnmsPrkgPauseSts APPauseStatus;
    AutnmsPrkgCnclSts APCancelStatus;
    AutnmsPrkgActvDirectPrkg APActiveDirectParking;
};

/*!
 * \struct AutnmsPrkgAvlScenario
 */
struct AutnmsPrkgAvlScenario {
    AutnmsPrkgAvlPrkgSide APAvailableParkingSide;
    AutnmsPrkgAvlPrkgScenario APAvailableParkingScenario;
    AutnmsPrkgAvlSlotTyp APAvailableSlotType;
    AutnmsPrkgAvlPrkgDir APAvailableParkingDirection;
    AutnmsPrkgAvlRem APAvailableRemote;
    AutnmsPrkgRecmndPrkgSide APRecommendedParkingSide;
    AutnmsPrkgRecmndSlotTyp APRecommendedSlotType;
    AutnmsPrkgRecmndPrkgDir APRecommendedParkingDirection;
    AutnmsPrkgRecmndRem APRecommendedRemote;
    AutnmsPrkgRecmndPrkgScenario APRecommendedParkingScenario;
    AutnmsPrkgAvlDirectPrkg APAvailableDirectParking;
};

/*!
 * \struct AutnmsPrkgSeldScenario
 */
struct AutnmsPrkgSeldScenario {
    AutnmsPrkgSeldSlotTyp AutnmsPrkgSeldSlotTyp_;
    AutnmsPrkgSeldPrkgScenario AutnmsPrkgSeldPrkgScenario_;
    AutnmsPrkgSeldPrkgDir AutnmsPrkgSeldPrkgDir_;
    AutnmsPrkgSeldPrkgSide AutnmsPrkgSeldPrkgSide_;
    AutnmsPrkgSeldRem AutnmsPrkgSeldRem_;
    AutnmsPrkgSeldDirectPrkg AutnmsPrkgSeldDirectPrkg_;
};

/*!
 * \struct BltLockStFrnt
 * Belt lock state for front seats.
 */
struct BltLockStFrnt {
    BltLockSt1 BltLockSt1_;
    DevErrSts2 BltLockSts;
};

/*!
 * \struct BltLockStRe
 * Belt lock state for rear seats (both 2nd and 3rd row).
 */
struct BltLockStRe {
    BltLockSt1 BltLockSt1_;
    DevErrSts2 BltLockSts;
    CptEquid BltLockEquid;
};

/*!
 * \struct BrkAndAbsWarnIndcnReqRec1
 */
struct BrkAndAbsWarnIndcnReqRec1 {
    OnOff2 BrkWarnIndcnReq;
    AbsWarnIndcnReq AbsWarnIndcnReq_;
    uint8_t BrkAndAbsWarnIndcnReqChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t BrkAndAbsWarnIndcnReqCntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct BrkFricTqAtWhlActRec
 */
struct BrkFricTqAtWhlActRec {
    uint16_t BrkFricTqAtWhlFrntLeAct; /*!<Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )*/
    uint16_t BrkFricTqAtWhlFrntRiAct; /*!<Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )*/
    uint16_t BrkFricTqAtWhlReLeAct; /*!<Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )*/
    uint16_t BrkFricTqAtWhlReRiAct; /*!<Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )*/
};

/*!
 * \struct CamIndReq
 */
struct CamIndReq {
    OnOff1 CamFrnt;
    OnOff1 CamRe;
    OnOff1 CamLe;
    OnOff1 CamRi;
};

/*!
 * \struct CchForFuEco1
 * Eco signals for DIM.
 */
struct CchForFuEco1 {
    double BarForFuEco; /*!<Unit: %,  Range:0->102.3, Resolution: (0.1*x+0.0, raw is unsigned, 10 bits )*/
    double GrdForFuEco; /*!<Unit: Unitless,  Range:0->102.3, Resolution: (0.1*x+0.0, raw is unsigned, 10 bits )*/
};

/*!
 * \struct CllsnWarnSide1
 */
struct CllsnWarnSide1 {
    CllsnWarn1 Le;
    CllsnWarn1 Ri;
};

/*!
 * \struct CmptmtAirTEstimdExtd
 * Estimated compartment temperature with quality flag
 */
struct CmptmtAirTEstimdExtd {
    double ComptmtT; /*!<Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )*/
    GenQf1 QlyFlg;
};

/*!
 * \struct CmptmtTFrnt
 */
struct CmptmtTFrnt {
    double CmptmtTFrnt; /*!<Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )*/
    CmptmtTFrntQf CmptmtTFrntQf_;
    Flg1 FanForCmptmtTRunng;
};

/*!
 * \struct ConSftyWarn1
 */
struct ConSftyWarn1 {
    uint8_t ConSftyWarnId; /*!<Unit: Unitless,  Range:0->15*/
    OnOff1 ConSftyWarnSnd;
    uint8_t ConSftyWarnTyp; /*!<Unit: Unitless,  Range:0->15*/
    ConSftyWarnLvl1 ConSftyWarnLvl;
    uint16_t DistanceToWarning; /*!<Unit: m,  Range:0->65535*/
};

/*!
 * \struct DataSpclTyp
 */
struct DataSpclTyp {
    uint8_t DataIdn; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t DataNrSpcl1; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t DataNrSpcl2; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t DataNrSpcl3; /*!<Unit: Unitless,  Range:0->255*/
};

/*!
 * \struct DateTi1ForSet
 */
struct DateTi1ForSet {
    DateOrTi DateOrTi_;
    uint8_t Year; /*!<Unit: Year,  Range:0->99*/
    uint8_t Month; /*!<Unit: Month,  Range:1->12*/
    uint8_t Day; /*!<Unit: Days,  Range:1->31*/
    uint8_t Hour; /*!<Unit: hours,  Range:0->23*/
    uint8_t Minute; /*!<Unit: Mins,  Range:0->59*/
    uint8_t Second; /*!<Unit: s,  Range:0->59*/
};

/*!
 * \struct DateTi30
 */
struct DateTi30 {
    uint8_t Yr1; /*!<Unit: Year,  Range:0->99*/
    uint8_t Mth1; /*!<Unit: Month,  Range:1->12*/
    uint8_t Day; /*!<Unit: Days,  Range:1->31*/
    uint8_t Hr1; /*!<Unit: hours,  Range:0->23*/
    uint8_t Mins1; /*!<Unit: Mins,  Range:0->59*/
    uint8_t Sec1; /*!<Unit: s,  Range:0->59*/
    NoYes1 DataValid;
};

/*!
 * \struct DriveAwayInfoWarnReq1
 */
struct DriveAwayInfoWarnReq1 {
    ReqSrc1 ReqSrc;
    uint8_t TiToDrvCntDwnTi; /*!<Unit: Unitless,  Range:0->255*/
    CtrlDirOfTrfcLi1 CtrlDirOfTrfcLi;
    VisWarnReq1 VisWarnReq;
    AudWarnReq1 AudWarnReq;
    InterruptionSrc1 InterruptionSrc;
};

/*!
 * \struct DrvrDispSetgReq
 */
struct DrvrDispSetgReq {
    IdPen Pen;
    DrvrDispSetg Sts;
};

/*!
 * \struct DrvrHmiBackGndInfoSetg
 */
struct DrvrHmiBackGndInfoSetg {
    IdPen Pen;
    DrvrHmiBackGndInfo Setg;
};

/*!
 * \struct DrvrHmiDispdModPen
 */
struct DrvrHmiDispdModPen {
    DrvrHmiDispdMod1 Sts;
    IdPen Pen;
};

/*!
 * \brief array DrvrHmiForHmiCen
 */
using DrvrHmiForHmiCen = std::array<uint8_t,254>;

/*!
 * \struct DrvrHmiUserIfSetgReq
 */
struct DrvrHmiUserIfSetgReq {
    IdPen Pen;
    DrvrHmiUsrIfSetg Setg;
};

/*!
 * \struct DrvrSeatSwtSts1
 */
struct DrvrSeatSwtSts1 {
    SwtHozlSts1 DrvrSeatSwtSldSts;
    SwtVertSts1 DrvrSeatSwtHeiSts;
    SwtVertSts1 DrvrSeatSwtHeiFrntSts;
    SwtHozlSts1 DrvrSeatSwtInclSts;
    SwtHozlSts1 DrvrSeatSwtAdjmtOfSpplFctHozlSts;
    SwtVertSts1 DrvrSeatSwtAdjmtOfSpplFctVertSts;
    SwtVertSts1 DrvrSeatSwtSelnOfSpplFctSts;
};

/*!
 * \struct DstToEmptyWithUnit
 */
struct DstToEmptyWithUnit {
    uint16_t DstToEmpty; /*!<Unit: Unitless,  Range:0->2000*/
    DstUnit DstUnit_;
};

/*!
 * \struct DstToManvLocnByNav
 */
struct DstToManvLocnByNav {
    uint16_t DstToManv; /*!<Unit: m,  Range:0->4000*/
    NoYes1 CntDwnToManvStrt;
    PosnFromNavQly PosnQly;
    NoYes1 SpprtForFct;
};

/*!
 * \struct EngIdleEco1
 */
struct EngIdleEco1 {
    OnOff1 Sts;
    IdPen Pen;
};

/*!
 * \struct EngNSafe
 */
struct EngNSafe {
    double EngN; /*!<Unit: rpm,  Range:0->16383, Resolution: (0.5*x+0.0, raw is unsigned, 15 bits )*/
    int16_t EngNGrdt; /*!<Unit: RpmPerSec,  Range:-30000->30000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )*/
    uint8_t EngNChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t EngNCntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct EpbLampReqRec
 */
struct EpbLampReqRec {
    EpbLampReqType1 EpbLampReq;
    uint8_t EpbLampReqChks; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t EpbLampReqCntr; /*!<Unit: Unitless,  Range:0->15, Resolution: (1.0*x+0.0, raw is unsigned, 4 bits )*/
};

/*!
 * \struct EscSptModReqdByDrvrRec1
 */
struct EscSptModReqdByDrvrRec1 {
    NoYes1 EscSptModReqdByDrvr;
    IdPen Pen;
};

/*!
 * \struct EscStSafe1
 */
struct EscStSafe1 {
    EscSt1 EscSt;
    uint8_t EscStChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t EscStCntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct EscWarnIndcnReqRec1
 */
struct EscWarnIndcnReqRec1 {
    EscWarnIndcnReq EscWarnIndcnReq_;
    uint8_t EscWarnIndcnReqChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t EscWarnIndcnReqCntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct ExtrMirrTiltSetg
 */
struct ExtrMirrTiltSetg {
    OnOff2 MirrDrvr;
    OnOff2 MirrPass;
    IdPen IdPen_;
};

/*!
 * \struct FrntWiprLvrReq2
 */
struct FrntWiprLvrReq2 {
    FrntWiprLvrCmd1 FrntWiprLvrCmd1_;
    Qf1 FrntWiprLvrQf;
    uint8_t FrntWiprLvrCrc; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t FrntWiprLvrCntr; /*!<Unit: Unitless,  Range:0->3*/
};

/*!
 * \struct FuLvlValWithQly
 */
struct FuLvlValWithQly {
    double FuLvlValFromFuTbl; /*!<Unit: Litre,  Range:0->204.6, Resolution: (0.2*x+0.0, raw is unsigned, 10 bits )*/
    GenQf1 GenQF;
};

/*!
 * \struct GearIndcnRec2
 */
struct GearIndcnRec2 {
    GearIndcnTyp2 GearIndcn;
    GearShiftIndcn GearShiftIndcn_;
    GearIndcnTyp2 GearTarIndcn;
};

/*!
 * \struct GlbRstForSetgAndData
 * Global reset activation critera together with profile information
 */
struct GlbRstForSetgAndData {
    GlbRst GlbRst_;
    IdPen Pen;
};

/*!
 * \brief array HmiCenForDrvrHmi
 */
using HmiCenForDrvrHmi = std::array<uint8_t,254>;

/*!
 * \struct HmiCmptmtTSp
 */
struct HmiCmptmtTSp {
    double HmiCmptmtTSpForRowFirstLe; /*!<Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )*/
    HmiCmptmtTSpSpcl HmiCmptmtTSpSpclForRowFirstLe;
    double HmiCmptmtTSpForRowFirstRi; /*!<Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )*/
    HmiCmptmtTSpSpcl HmiCmptmtTSpSpclForRowFirstRi;
    double HmiCmptmtTSpForRowSecLe; /*!<Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )*/
    HmiCmptmtTSpSpcl HmiCmptmtTSpSpclForRowSecLe;
    double HmiCmptmtTSpForRowSecRi; /*!<Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )*/
    HmiCmptmtTSpSpcl HmiCmptmtTSpSpclForRowSecRi;
};

/*!
 * \struct HmiDefrstElecReq
 */
struct HmiDefrstElecReq {
    ActrReq FrntElecReq;
    ActrReq ReElecReq;
    ActrReq MirrElecReq;
};

/*!
 * \struct HmiDefrstElecSts
 */
struct HmiDefrstElecSts {
    ActrDefrstSts Frnt;
    ActrDefrstSts Re;
    ActrDefrstSts Mirrr;
};

/*!
 * \struct HmiSeatClima
 */
struct HmiSeatClima {
    SeatClimaLvl HmiSeatHeatgForRowFirstLe;
    SeatClimaLvl HmiSeatHeatgForRowFirstRi;
    SeatClimaLvl HmiSeatHeatgForRowSecLe;
    SeatClimaLvl HmiSeatHeatgForRowSecRi;
    SeatClimaLvl HmiSeatVentnForRowFirstLe;
    SeatClimaLvl HmiSeatVentnForRowFirstRi;
};

/*!
 * \struct HmiSeatClimaExtd
 * Seat ventilation request, second row.
 */
struct HmiSeatClimaExtd {
    SeatClimaLvl HmiSeatVentnForRowSecLe;
    SeatClimaLvl HmiSeatVentnForRowSecRi;
};

/*!
 * \struct HudDiagc
 */
struct HudDiagc {
    Err1 HudTSts;
    Err1 HudCircShoSts;
    Err1 HudCricOpenSts;
    NotAvlSts1 ImgHudTmpNotAvlSts;
    Err1 ImgHudErrSts;
};

/*!
 * \struct HudVisFctSetgReq
 */
struct HudVisFctSetgReq {
    IdPen Pen;
    OnOff1 HudFct00;
    OnOff1 HudFct01;
    OnOff1 HudFct02;
    OnOff1 HudFct03;
    OnOff1 HudFct04;
    OnOff1 HudFct05;
    OnOff1 HudFct06;
    OnOff1 HudFct07;
    OnOff1 HudFct08;
    OnOff1 HudFct09;
    OnOff1 HudFct10;
    OnOff1 HudFct11;
    OnOff1 HudFct12;
    OnOff1 HudFct13;
    OnOff1 HudFct14;
    OnOff1 HudFct15;
    OnOff1 HudFct16;
    OnOff1 HudFct17;
    OnOff1 HudFct18;
    OnOff1 HudFct19;
};

/*!
 * \struct HwAprvdWirelsAdr1
 * Status for Bluetooth pairing
 */
struct HwAprvdWirelsAdr1 {
    uint8_t HwOffsAdr1; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t HwOffsAdr2; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t HwOffsAdr3; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t HwOffsAdr4; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t HwOffsAdr5; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t HwOffsAdr6; /*!<Unit: Unitless,  Range:0->255*/
    AprvdSts AprvdSts_;
};

/*!
 * \struct HznDataGroup2
 */
struct HznDataGroup2 {
    HznMsgTyp1 MsgTyp;
    uint8_t CycCntr; /*!<Unit: Unitless,  Range:0->3*/
    uint16_t CtryCod; /*!<Unit: Unitless,  Range:0->1023*/
    uint16_t RegnCod; /*!<Unit: Unitless,  Range:0->32767*/
    DrvgSide1 DrvgSide;
    SpdUnit1 SpdUnit;
    uint8_t HdTxVers; /*!<Unit: Unitless,  Range:0->3*/
    uint8_t TxVers; /*!<Unit: Unitless,  Range:0->15*/
    uint8_t ChdTxVers; /*!<Unit: Unitless,  Range:0->7*/
    uint16_t HwVers; /*!<Unit: Unitless,  Range:0->511*/
    HznMapSrc2 MapSrc;
    uint8_t YrVersOfMap; /*!<Unit: Unitless,  Range:0->63*/
    uint8_t PartOfYrVersOfMap; /*!<Unit: Unitless,  Range:0->3*/
};

/*!
 * \struct HznEdgeGroup2
 */
struct HznEdgeGroup2 {
    HznMsgTyp1 MsgTyp;
    uint8_t CycCntr; /*!<Unit: Unitless,  Range:0->3*/
    NoYes1 TxPrev;
    uint8_t PahIdx; /*!<Unit: Unitless,  Range:0->63*/
    uint16_t Offs; /*!<Unit: m,  Range:0->8191*/
    NoYes1 Upd;
    uint8_t PahIdxNew; /*!<Unit: Unitless,  Range:0->63*/
    double TurnAg; /*!<Unit: Deg,  Range:0->360, Resolution: (1.4173228346456692*x+0.0, raw is unsigned, 8 bits )*/
    double RelProblty; /*!<Unit: %,  Range:0->103.33333323, Resolution: (3.33333333*x+0.0, raw is unsigned, 5 bits )*/
    HznTypOfWay1 TypOfWay;
    HznNrOfLaneInDrvgDir1 NrOfLaneInDrvgDir;
    HznNrOfLaneInSecDir1 NrOfLaneInSecDir;
    HznIntscnCmplx1 IntscnCmplx;
    HznRiOfWay1 RiOfWay;
    uint8_t RoadClass; /*!<Unit: Unitless,  Range:0->7*/
    HznPartOfCourseCalcd1 PartOfCourseCalcd;
    NoYes1 LstEdgeAtOffs;
};

/*!
 * \struct HznPosnExtdGroup1
 */
struct HznPosnExtdGroup1 {
    HznMsgTyp1 MsgTyp;
    uint8_t PahIdx; /*!<Unit: Unitless,  Range:0->63*/
    uint8_t PosnIdx; /*!<Unit: Unitless,  Range:0->3*/
    double PosnTiDif; /*!<Unit: ms,  Range:0->2555, Resolution: (5.0*x+0.0, raw is unsigned, 9 bits )*/
    double Spd; /*!<Unit: m/s,  Range:-12.8->89.4, Resolution: (0.2*x+-12.8, raw is unsigned, 9 bits )*/
    double RelDir; /*!<Unit: Deg,  Range:0->360, Resolution: (1.4173228346456692*x+0.0, raw is unsigned, 8 bits )*/
    double PosnProblty; /*!<Unit: %,  Range:0->103.33333323, Resolution: (3.33333333*x+0.0, raw is unsigned, 5 bits )*/
    uint8_t PosnQly; /*!<Unit: Unitless,  Range:0->7*/
    HznLanePrsnt2 LanePrsnt;
};

/*!
 * \struct HznPosnExtdOffs
 */
struct HznPosnExtdOffs {
    double Offs; /*!<Unit: m,  Range:0->1048448, Resolution: (128.0*x+0.0, raw is unsigned, 13 bits )*/
    uint8_t CycCntr; /*!<Unit: Unitless,  Range:0->3*/
};

/*!
 * \struct HznPosnGroup3
 */
struct HznPosnGroup3 {
    HznMsgTyp1 MsgTyp;
    uint8_t CycCntr; /*!<Unit: Unitless,  Range:0->3*/
    uint8_t PahIdx; /*!<Unit: Unitless,  Range:0->63*/
    uint16_t Offs; /*!<Unit: m,  Range:0->8191*/
    uint8_t PosnIdx; /*!<Unit: Unitless,  Range:0->3*/
    double PosnTiDif; /*!<Unit: ms,  Range:0->2555, Resolution: (5.0*x+0.0, raw is unsigned, 9 bits )*/
    double Spd; /*!<Unit: m/s,  Range:-12.8->89.4, Resolution: (0.2*x+-12.8, raw is unsigned, 9 bits )*/
    double RelDir; /*!<Unit: Deg,  Range:0->360, Resolution: (1.4173228346456692*x+0.0, raw is unsigned, 8 bits )*/
    double PosnProblty; /*!<Unit: %,  Range:0->103.33333323, Resolution: (3.33333333*x+0.0, raw is unsigned, 5 bits )*/
    uint8_t PosnQly; /*!<Unit: Unitless,  Range:0->7*/
    HznLanePrsnt2 LanePrsnt;
};

/*!
 * \struct HznProfLongExtdGroup1
 */
struct HznProfLongExtdGroup1 {
    HznMsgTyp1 MsgTyp;
    uint8_t CycCntr; /*!<Unit: Unitless,  Range:0->3*/
    NoYes1 TxPrev;
    uint8_t PahIdx; /*!<Unit: Unitless,  Range:0->63*/
    double Offs; /*!<Unit: m,  Range:0->1048448, Resolution: (128.0*x+0.0, raw is unsigned, 13 bits )*/
    NoYes1 Upd;
    HznProfLongTypExtd1 ProfTyp;
    NoYes1 NodCtrl;
    uint32_t Val; /*!<Unit: Unitless,  Range:0->4294967295*/
};

/*!
 * \struct HznProfLongGroup3
 */
struct HznProfLongGroup3 {
    HznMsgTyp1 MsgTyp;
    uint8_t CycCntr; /*!<Unit: Unitless,  Range:0->3*/
    NoYes1 TxPrev;
    uint8_t PahIdx; /*!<Unit: Unitless,  Range:0->63*/
    uint16_t Offs; /*!<Unit: m,  Range:0->8191*/
    NoYes1 Upd;
    HznProfLongTyp ProfTyp;
    NoYes1 NodCtrl;
    uint32_t Val; /*!<Unit: Unitless,  Range:0->4294967295*/
};

/*!
 * \struct HznProfSho2
 */
struct HznProfSho2 {
    HznMsgTyp1 MsgTyp;
    uint8_t CycCntr; /*!<Unit: Unitless,  Range:0->3*/
    NoYes1 TxPrev;
    uint8_t PahIdx; /*!<Unit: Unitless,  Range:0->63*/
    uint16_t Offs; /*!<Unit: m,  Range:0->8191*/
    NoYes1 Upd;
    HznProfShoTyp ProfTyp;
    NoYes1 NodCtrl;
    uint16_t Val0; /*!<Unit: Unitless,  Range:0->1023*/
    uint16_t Dst1; /*!<Unit: m,  Range:0->1023*/
    uint16_t Val1; /*!<Unit: Unitless,  Range:0->1023*/
    HznProfTypQly1 ProfTypQly;
};

/*!
 * \struct HznSegGroup2
 */
struct HznSegGroup2 {
    HznMsgTyp1 MsgTyp;
    uint8_t CycCntr; /*!<Unit: Unitless,  Range:0->3*/
    NoYes1 TxPrev;
    uint8_t PahIdx; /*!<Unit: Unitless,  Range:0->63*/
    uint16_t Offs; /*!<Unit: m,  Range:0->8191*/
    NoYes1 Upd;
    uint8_t RoadClass; /*!<Unit: Unitless,  Range:0->7*/
    HznTypOfWay1 TypOfWay;
    HznSpdLimEfc1 SpdLimEfc;
    HznSpdLimTypEfc1 SpdLimTypEfc;
    HznNrOfLaneInDrvgDir1 NrOfLaneInDrvgDir;
    HznNrOfLaneInSecDir1 NrOfLaneInSecDir;
    HznTnl1 Tnl;
    HznBrdg1 Brdg;
    HznRoadMpl1 RoadMpl;
    HznRoadLiAr1 RoadLiAr;
    HznIntscnCmplx1 IntscnCmplx;
    double RelProblty; /*!<Unit: %,  Range:0->103.33333323, Resolution: (3.33333333*x+0.0, raw is unsigned, 5 bits )*/
    HznPartOfCourseCalcd1 PartOfCourseCalcd;
};

/*!
 * \struct IndcnUnit
 * Settings for units and formats that shall be used for indication in the vehicle
 */
struct IndcnUnit {
    AmbTIndcdUnit TUnit;
    VehSpdIndcdUnit SpdUnit;
    DstLong DstLong_;
    DstSho DstSho_;
    FuCnsIndcdUnit FuCnsUnit;
    VolUnit VolUnit_;
    IdPen IdPenForUnit;
    ClkFmt ClkFmt_;
    PUnit PUnit_;
    DateFmt DateFmt_;
};

/*!
 * \struct IntrLiAmbLiSetg
 */
struct IntrLiAmbLiSetg {
    IntrLiInten LiInten;
    AmbLiTintg LiTintg;
    IdPen Pen;
};

/*!
 * \struct IntrLiSurrndgsLiSetg
 */
struct IntrLiSurrndgsLiSetg {
    IntrLiInten LiInten;
    SurrndgsLiLvl LiLvl;
    IdPen Pen;
};

/*!
 * \struct KeyProfMpgUpd1
 */
struct KeyProfMpgUpd1 {
    IdPen KeyProfMpgUpdForIdPen;
    bool KeyProfMpgUpdOff;
};

/*!
 * \struct KeyReadStsToProfCtrl
 */
struct KeyReadStsToProfCtrl {
    KeyId1 KeyId;
    bool Boolean;
};

/*!
 * \struct LiAutTranPen1
 */
struct LiAutTranPen1 {
    LiAutTran1 Sts;
    IdPen Pen;
};

/*!
 * \struct LiTiPen2
 */
struct LiTiPen2 {
    LiTi2 Sts;
    IdPen Pen;
};

/*!
 * \struct LockgCenSts3
 */
struct LockgCenSts3 {
    LockSt3 LockSt;
    LockTrigSrc2 TrigSrc;
    bool UpdEve;
};

/*!
 * \struct LvlSeldForSpdLimAdpvPen1
 */
struct LvlSeldForSpdLimAdpvPen1 {
    Aut1 Sts;
    IdPen Pen;
};

/*!
 * \struct MassgFctActv
 */
struct MassgFctActv {
    OnOff1 DrvrMassgFctActv;
    OnOff1 PassMassgFctActv;
};

/*!
 * \struct MirrDimPen
 */
struct MirrDimPen {
    MirrDimLvlTyp MirrDimLvl;
    IdPen Pen;
};

/*!
 * \struct MtrlSnsrT
 */
struct MtrlSnsrT {
    double MtrlSnsrT; /*!<Unit: degC,  Range:-256->255.9, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )*/
    MtrlSnsrTFacQly MtrlSnsrTFacQly_;
};

/*!
 * \struct NetActvtRec1
 */
struct NetActvtRec1 {
    uint8_t ResourceGroup; /*!<Unit: Unitless,  Range:0->255*/
    PrioHighNormal Prio;
};

/*!
 * \struct NormSptPen
 */
struct NormSptPen {
    NormSptType NormSpt;
    IdPen Pen;
};

/*!
 * \struct NrSerlNodLIN
 */
struct NrSerlNodLIN {
    uint8_t Nr1; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Nr2; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Nr3; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Nr4; /*!<Unit: Unitless,  Range:0->255*/
};

/*!
 * \struct OffsForSpdWarnSetgPen
 */
struct OffsForSpdWarnSetgPen {
    uint8_t Sts; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    IdPen Pen;
};

/*!
 * \struct OnOffPen
 */
struct OnOffPen {
    OnOff1 Sts;
    IdPen Pen;
};

/*!
 * \struct PartNrNodLIN
 */
struct PartNrNodLIN {
    uint8_t Nr1; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Nr2; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Nr3; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Nr4; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t EndSgn1; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t EndSgn2; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t EndSgn3; /*!<Unit: Unitless,  Range:0->255*/
};

/*!
 * \struct PassSeatSwtSts2
 * Describes PSMP switch pack information.
 */
struct PassSeatSwtSts2 {
    SwtHozlSts1 PassSeatSwtSldSts;
    SwtVertSts1 PassSeatSwtHeiSts;
    SwtVertSts1 PassSeatSwtHeiFrntSts;
    SwtHozlSts1 PassSeatSwtInclSts;
    SwtHozlSts1 PassSeatSwtAdjmtOfSpplFctHozlSts;
    SwtVertSts1 PassSeatSwtAdjmtOfSpplFctVerSts;
    SwtVertSts1 PassSeatSwtSelnOfSpplFctStsSts;
};

/*!
 * \struct PinionSteerAg1Rec
 */
struct PinionSteerAg1Rec {
    double PinionSteerAg1; /*!<Unit: rad,  Range:-14.5->14.5, Resolution: (0.0009765625*x+0.0, raw is signed, 15 bits )*/
    GenQf1 PinionSteerAg1Qf;
};

/*!
 * \struct PosnFromNav
 */
struct PosnFromNav {
    double PosnLat; /*!<Unit: Deg,  Range:-90->90, Resolution: (2.7777777777777776e-07*x+0.0, raw is unsigned, 0 bits )*/
    double PosnLgt; /*!<Unit: Deg,  Range:-180->180, Resolution: (2.7777777777777776e-07*x+0.0, raw is unsigned, 0 bits )*/
    double PosnAlti; /*!<Unit: m,  Range:-100->6000, Resolution: (0.1*x+-100.0, raw is unsigned, 0 bits )*/
    double PosnSpd; /*!<Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 0 bits )*/
    double PosnDir; /*!<Unit: Deg,  Range:0->359.99, Resolution: (0.01*x+0.0, raw is unsigned, 0 bits )*/
    PosnFromNavQly PosnQly;
};

/*!
 * \struct PosnFromSatlt
 */
struct PosnFromSatlt {
    double PosnLat; /*!<Unit: Deg,  Range:-90->90, Resolution: (2.7777777777777776e-07*x+0.0, raw is signed, 30 bits )*/
    double PosnLgt; /*!<Unit: Deg,  Range:-180->180, Resolution: (2.7777777777777776e-07*x+0.0, raw is signed, 31 bits )*/
    double PosnAlti; /*!<Unit: m,  Range:-100->6000, Resolution: (0.1*x+-100.0, raw is unsigned, 16 bits )*/
    double PosnSpd; /*!<Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 17 bits )*/
    double PosnVHozl; /*!<Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 17 bits )*/
    double PosnVVert; /*!<Unit: m/s,  Range:-100->100, Resolution: (0.001*x+0.0, raw is signed, 18 bits )*/
    double PosnDir; /*!<Unit: Deg,  Range:0->359.99, Resolution: (0.01*x+0.0, raw is unsigned, 16 bits )*/
    uint8_t TiForYr; /*!<Unit: Year,  Range:0->99*/
    uint8_t TiForMth; /*!<Unit: Month,  Range:1->12*/
    uint8_t TiForDay; /*!<Unit: Days,  Range:1->31*/
    uint8_t TiForHr; /*!<Unit: hours,  Range:0->23*/
    uint8_t TiForMins; /*!<Unit: Mins,  Range:0->59*/
    uint8_t TiForSec; /*!<Unit: s,  Range:0->59*/
    NoYes1 SatltSysNo1InUse;
    NoYes1 SatltSysNo2InUse;
    NoYes1 SatltSysNo3InUse;
    NoYes1 SatltSysNo4InUse;
    NoYes1 SatltSysNo5InUse;
    NoYes1 SatltSysNo6InUse;
    SatltPosnSts SatltPosnStsPrm1;
    bool SatltPosnStsPrm2;
    bool SatltPosnStsPrm3;
    uint8_t NoOfSatltForSysNo1; /*!<Unit: Unitless,  Range:0->31*/
    uint8_t NoOfSatltForSysNo2; /*!<Unit: Unitless,  Range:0->31*/
    uint8_t NoOfSatltForSysNo3; /*!<Unit: Unitless,  Range:0->31*/
    uint8_t NoOfSatltForSysNo4; /*!<Unit: Unitless,  Range:0->31*/
    uint8_t NoOfSatltForSysNo5; /*!<Unit: Unitless,  Range:0->31*/
    uint8_t NoOfSatltForSysNo6; /*!<Unit: Unitless,  Range:0->31*/
    double PrePosnDil; /*!<Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )*/
    double PreHozlDil; /*!<Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )*/
    double PreVertDil; /*!<Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )*/
    double PreTiDil; /*!<Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )*/
};

/*!
 * \struct PrkgAssiAudWarn
 * New record type to hold audio warning data types for PAS.
 */
struct PrkgAssiAudWarn {
    WarnDir1 AudWarnDir;
    ParkAssiLeRi1 AudSideWarn;
};

/*!
 * \struct ProfAct1
 */
struct ProfAct1 {
    bool Prof1;
    bool Prof2;
    bool Prof3;
    bool Prof4;
    bool Prof5;
    bool Prof6;
    bool Prof7;
    bool Prof8;
    bool Prof9;
    bool Prof10;
    bool Prof11;
    bool Prof12;
};

/*!
 * \struct ProfLimd1
 */
struct ProfLimd1 {
    bool Prof1;
    bool Prof2;
    bool Prof3;
    bool Prof4;
    bool Prof5;
    bool Prof6;
    bool Prof7;
    bool Prof8;
    bool Prof9;
    bool Prof10;
    bool Prof11;
    bool Prof12;
};

/*!
 * \struct PtCluTq1
 */
struct PtCluTq1 {
    int16_t PtCluTq; /*!<Unit: NewtonMeter,  Range:-1024->1023, Resolution: (1.0*x+0.0, raw is unsigned, 11 bits )*/
    int16_t PtCluTqDyn; /*!<Unit: NewtonMeter,  Range:-1024->1023, Resolution: (1.0*x+0.0, raw is unsigned, 11 bits )*/
    Qly3 PtCluTqQly;
};

/*!
 * \struct PtDrvrSetg2
 */
struct PtDrvrSetg2 {
    IdPen Pen;
    PtDrvrModSetg2 Sts;
};

/*!
 * \struct PtTqAtWhlFrntActRec1
 */
struct PtTqAtWhlFrntActRec1 {
    int16_t PtTqAtWhlFrntLeAct; /*!<Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )*/
    int16_t PtTqAtWhlFrntRiAct; /*!<Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )*/
    int16_t PtTqAtAxleFrntAct; /*!<Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )*/
    Qly3 PtTqAtWhlsFrntQly;
};

/*!
 * \struct RngBdIllmnCmdPen1
 */
struct RngBdIllmnCmdPen1 {
    RngbdIllmnCmd1 Cmd;
    IdPen Pen;
};

/*!
 * \struct RsdsSysSts
 * Status of the different internal statemachines.
 * 
 * Master Statemachine
 * RsdsMstSt (Enum):
 * Cfg
 * StrtUp
 * Runng
 * Blkd
 * Faulty
 * Shutdown
 * Hot
 * Cal
 * 
 * Lcma:
 * OnOff1
 * ActvInActv
 * 
 * Ctra:
 * OnOff1
 * ActvInActv
 * 
 * Rcw:
 * OnOff1
 * 
 * Rcm:
 * OnOff1
 */
struct RsdsSysSts {
    RsdsMstSt RsdsSts;
    OnOff1 LcmaSts;
    ActvInActv LcmaSts1;
    OnOff1 CtraSts;
    ActvInActv CtraSts1;
    OnOff1 RcwSts;
    OnOff1 RcmSts;
};

/*!
 * \struct SeatBackUnlckd
 */
struct SeatBackUnlckd {
    bool SeatBackUnlckdLe;
    bool SeatBackUnlckdRi;
};

/*!
 * \struct SeatDispBtnPsd
 */
struct SeatDispBtnPsd {
    bool DrvrSeatDispBtnPsd;
    bool PassSeatDispBtnPsd;
};

/*!
 * \struct SeatMassgFct
 */
struct SeatMassgFct {
    OnOff1 OnOff;
    MassgProgTyp MassgProg;
    MassgIntenLvl MassgInten;
    MassgIntenLvl MassgSpdLvl;
};

/*!
 * \struct SetOfLang
 */
struct SetOfLang {
    IdPen IdPen_;
    LangTyp LangTyp_;
};

/*!
 * \struct SftyCchActvnSts1
 * Current status of function and also time since function was last active in Years, months and days.
 */
struct SftyCchActvnSts1 {
    NoYes1 Actv;
    uint8_t Yr; /*!<Unit: Unitless,  Range:0->15*/
    uint8_t Mth; /*!<Unit: Unitless,  Range:0->15*/
    uint8_t Day; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct SftyCchIdx1
 */
struct SftyCchIdx1 {
    double Tot; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 TotAvl;
    double DstToVeh; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 DstToVehAvl;
    double LaneKeep; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 LaneKeepAvl;
    double Attention; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 AttentionAvl;
    double KeepSpdLim; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 KeepSpdLimAvl;
};

/*!
 * \struct SftyCchIdx2
 */
struct SftyCchIdx2 {
    double Tot; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 TotAvl;
    SftyCchCntxt1 TotCntxt;
    double DstToVeh; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 DstToVehAvl;
    SftyCchCntxt1 DstToVehCntxt;
    double LaneKeep; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 LaneKeepAvl;
    SftyCchCntxt1 LaneKeepCntxt;
    double Attention; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 AttentionAvl;
    SftyCchCntxt1 AttentionCntxt;
    double KeepSpdLim; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 KeepSpdLimAvl;
    SftyCchCntxt1 KeepSpdLimCntxt;
    double FldTot; /*!<Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    FcnAvlSts1 FldTotAvl;
};

/*!
 * \struct SftySigGroupFromAudSafe1
 */
struct SftySigGroupFromAudSafe1 {
    NoYesCrit1 SftySigFaildDetdByAud;
    NoYesCrit1 SftyAudDend;
    NoYesCrit1 SftyAudEna;
    uint8_t SftySigGroupFromAudSafeChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t SftySigGroupFromAudSafeCntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct SftyWarnGroupFromAsySafe1
 */
struct SftyWarnGroupFromAsySafe1 {
    NoYesCrit1 CnclWarnLatForAutDrv;
    NoYesCrit1 SteerOvrdWarnReqForAutDrv;
    SteerStsForAutDrv3 SteerStsForAutDrv;
    uint8_t SftyWarnGroupFromAsySafeChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t SftyWarnGroupFromAsySafeCntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct SlowNormFastPen
 */
struct SlowNormFastPen {
    SlowNormFast SlowNormFast_;
    IdPen IdPen_;
};

/*!
 * \struct SnsrParkAssi1
 */
struct SnsrParkAssi1 {
    DstOfSnsr1 DstOfSnsrInsdLe;
    DstOfSnsr1 DstOfSnsrOutdLe;
    DstOfSnsr1 DstOfSnsrLeSide;
    DstOfSnsr1 DstOfSnsrInsdRi;
    DstOfSnsr1 DstOfSnsrOutdRi;
    DstOfSnsr1 DstOfSnsrRiSide;
    WarnDir1 AudWarnDir;
    ParkAssiLeRi1 AudSideWarn;
};

/*!
 * \struct SnsrPrkgAssi2
 * New record type to hold distances using new data type for longer range ultrasound sensors (DstOfSnsr2). Rest of content (audio warning data types) is the same as in (older) SnsrParkAssi1 record type.
 */
struct SnsrPrkgAssi2 {
    double SnsrDstInsdLe; /*!<Unit: cm,  Range:0->1023, Resolution: (-1.0*x+1023.0, raw is unsigned, 10 bits )*/
    double SnsrDstOutdLe; /*!<Unit: cm,  Range:0->1023, Resolution: (-1.0*x+1023.0, raw is unsigned, 10 bits )*/
    double SnsrDstSideLe; /*!<Unit: cm,  Range:0->511, Resolution: (-1.0*x+511.0, raw is unsigned, 9 bits )*/
    double SnsrDstInsdRi; /*!<Unit: cm,  Range:0->1023, Resolution: (-1.0*x+1023.0, raw is unsigned, 10 bits )*/
    double SnsrDstOutdRi; /*!<Unit: cm,  Range:0->1023, Resolution: (-1.0*x+1023.0, raw is unsigned, 10 bits )*/
    double SnsrDstSideRi; /*!<Unit: cm,  Range:0->511, Resolution: (-1.0*x+511.0, raw is unsigned, 9 bits )*/
    WarnDir1 AudWarnDir;
    ParkAssiLeRi1 AudSideWarn;
};

/*!
 * \struct SnsrPrkgAssi3
 * New record type to hold distances using new data type for longer range ultrasound sensors (DstOfSnsr2 and DstOfSnsr3).
 */
struct SnsrPrkgAssi3 {
    double SnsrDstInsdLe; /*!<Unit: cm,  Range:0->1023, Resolution: (-1.0*x+1023.0, raw is unsigned, 0 bits )*/
    double SnsrDstOutdLe; /*!<Unit: cm,  Range:0->1023, Resolution: (-1.0*x+1023.0, raw is unsigned, 0 bits )*/
    double SnsrDstSideLe; /*!<Unit: cm,  Range:0->511, Resolution: (-1.0*x+511.0, raw is unsigned, 0 bits )*/
    double SnsrDstInsdRi; /*!<Unit: cm,  Range:0->1023, Resolution: (-1.0*x+1023.0, raw is unsigned, 0 bits )*/
    double SnsrDstOutdRi; /*!<Unit: cm,  Range:0->1023, Resolution: (-1.0*x+1023.0, raw is unsigned, 0 bits )*/
    double SnsrDstSideRi; /*!<Unit: cm,  Range:0->511, Resolution: (-1.0*x+511.0, raw is unsigned, 0 bits )*/
};

/*!
 * \struct SnvtyPen1
 */
struct SnvtyPen1 {
    Snvty1 Sts;
    IdPen Pen;
};

/*!
 * \struct SnvtyPen3
 */
struct SnvtyPen3 {
    Snvty4 Sts;
    IdPen Pen;
};

/*!
 * \struct SteerAssiLvlForAutDrvPen1
 */
struct SteerAssiLvlForAutDrvPen1 {
    IdPen Pen;
    Lvl2 Sts;
};

/*!
 * \struct SteerSetg
 */
struct SteerSetg {
    SteerAsscLvl SteerAsscLvl_;
    SteerMod SteerMod_;
    IdPen Pen;
};

/*!
 * \struct SteerWhlSnsrSafe1
 */
struct SteerWhlSnsrSafe1 {
    double SteerWhlAg; /*!<Unit: rad,  Range:-14.5->14.5, Resolution: (0.0009765625*x+0.0, raw is signed, 15 bits )*/
    double SteerWhlAgSpd; /*!<Unit: rad/s,  Range:-50->50, Resolution: (0.0078125*x+0.0, raw is signed, 14 bits )*/
    GenQf1 SteerWhlSnsrQf;
    uint8_t SteerWhlSnsrChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t SteerWhlSnsrCntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct SuspSetgRec
 */
struct SuspSetgRec {
    IdPen SuspIdPen;
    LvlCtrlSetgTyp LvlCtrlSetg;
    SuspCtrlSetgTyp SuspCtrlSetg;
};

/*!
 * \struct SysU
 * System voltage represented by the CEM node Voltage
 */
struct SysU {
    double SysU; /*!<Unit: Volt,  Range:0->25, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )*/
    GenQf1 SysUQf;
};

/*!
 * \struct TiCorrn
 */
struct TiCorrn {
    int8_t HrCorrn; /*!<Unit: hours,  Range:-15->15*/
    int8_t MinsCorrn; /*!<Unit: Unitless,  Range:-59->59*/
    int8_t DayLiSaveTi; /*!<Unit: hours,  Range:-15->15*/
    NoYes1 SpprtForFct;
};

/*!
 * \struct TiGapAdpvSeldPen1
 */
struct TiGapAdpvSeldPen1 {
    ManAdpv1 Sts;
    IdPen Pen;
};

/*!
 * \struct TiGapPen1
 */
struct TiGapPen1 {
    double Sts; /*!<Unit: s,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )*/
    IdPen Pen;
};

/*!
 * \struct TireCircumCalByNav
 */
struct TireCircumCalByNav {
    uint16_t TireCircum; /*!<Unit: Unitless,  Range:0->4095*/
    NoYes1 HiQly;
    NoYes1 SpprtForFct;
};

/*!
 * \struct TirePAbsltValFrntLe3
 */
struct TirePAbsltValFrntLe3 {
    double TirepabsltVal1; /*!<Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )*/
    TirePAbsltValQf TirePabsltValQF;
    TirePPosn TirePPosn_;
};

/*!
 * \struct TirePAbsltValFrntRi1
 */
struct TirePAbsltValFrntRi1 {
    double TirePAbsltVal1; /*!<Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )*/
    TirePAbsltValQf TirePAbsltValQF;
    TirePPosn TirePPosn_;
};

/*!
 * \struct TirePAbsltValReLe1
 */
struct TirePAbsltValReLe1 {
    double TirePAbsltVal1; /*!<Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )*/
    TirePAbsltValQf TirePAbsltValQF;
    TirePPosn TirePPosn_;
};

/*!
 * \struct TirePAbsltValReRi1
 */
struct TirePAbsltValReRi1 {
    double TirePAbsltVal1; /*!<Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )*/
    TirePAbsltValQf TirePAbsltValQF;
    TirePPosn TirePPosn_;
};

/*!
 * \struct TirePMonData1
 */
struct TirePMonData1 {
    TirePMonSts1 TirePMonSts1_;
    TirePWarn TirePReRi;
    TirePWarn TirePReLe;
    TirePWarn TirePFrntLe;
    TirePWarn TirePFrntRe;
};

/*!
 * \struct TqSafe2
 */
struct TqSafe2 {
    double TqAct; /*!<Unit: NewtonMeter,  Range:-8188->8192, Resolution: (4.0*x+-8188.0, raw is unsigned, 12 bits )*/
    uint8_t TqActChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t TqActCntr; /*!<Unit: Unitless,  Range:0->15*/
    QualityFactor TqActQlyFac;
    double SpdLimn; /*!<Unit: rpm,  Range:0->2550, Resolution: (10.0*x+0.0, raw is unsigned, 8 bits )*/
};

/*!
 * \struct TwliBriRaw
 */
struct TwliBriRaw {
    uint16_t TwliBriRaw1; /*!<Unit: Unitless,  Range:0->10000*/
    GenQf1 TwliBriRawQf;
};

/*!
 * \struct UInt64Rec
 */
struct UInt64Rec {
    uint8_t Byte0; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Byte1; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Byte2; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Byte3; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Byte4; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Byte5; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Byte6; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Byte7; /*!<Unit: Unitless,  Range:0->255*/
};

/*!
 * \struct UnlckKeylsCfgPen2
 */
struct UnlckKeylsCfgPen2 {
    IdPen IdPen_;
    UnlckKeylsCfg2 KeylsCfg;
};

/*!
 * \struct UnlckRemCfgPen1
 */
struct UnlckRemCfgPen1 {
    UnlckRemCfg1 Sts;
    IdPen Pen;
};

/*!
 * \struct UsrSetSpdForKeySpdLimn
 */
struct UsrSetSpdForKeySpdLimn {
    double Sts; /*!<Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )*/
    IdPen Pen;
};

/*!
 * \struct UsrSetSpdForKeySpdWarn
 * Conveys the 6 personalized speed thresholds for Restricted Key Speed Alert function.
 * 
 * Value 0 = the speed threshold is not used
 * 
 * Personal ID shows to which Profile the settings belongs.
 */
struct UsrSetSpdForKeySpdWarn {
    double UsrSetSpdForKeySpdWarn1; /*!<Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )*/
    double UsrSetSpdForKeySpdWarn2; /*!<Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )*/
    double UsrSetSpdForKeySpdWarn3; /*!<Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )*/
    double UsrSetSpdForKeySpdWarn4; /*!<Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )*/
    double UsrSetSpdForKeySpdWarn5; /*!<Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )*/
    double UsrSetSpdForKeySpdWarn6; /*!<Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )*/
    IdPen UsrSetSpdForKeySpdWarnPen;
};

/*!
 * \struct UsrSetVolMaxForKeyVolLimn
 */
struct UsrSetVolMaxForKeyVolLimn {
    IdPen Pen;
    SetVol SetVol_;
};

/*!
 * \struct UsrSwtDispClimaReqForRowSec
 */
struct UsrSwtDispClimaReqForRowSec {
    double UsrSwtDispClimaReqForTSpForRowSecLe; /*!<Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )*/
    bool UsrSwtDispUpdClimaReqForTSpForRowSecLe;
    double UsrSwtDispClimaReqForTSpForRowSecRi; /*!<Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )*/
    bool UsrSwtDispUpdClimaReqForTSpForRowSecRi;
    SeatClimaLvl UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe;
    bool UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe;
    bool UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi;
    SeatClimaLvl UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi;
    HmiHvacFanLvl UsrSwtDispReqForFanLvlForRowSec;
    bool UsrSwtDispUpdReqForFanLvlForRowSec;
    HmiCmptmtTSpSpcl UsrSwtDispClimaReqForTSpSpclForRowSecLe;
    HmiCmptmtTSpSpcl UsrSwtDispClimaReqForTSpSpclForRowSecRi;
};

/*!
 * \struct UsrSwtDispClimaSts
 */
struct UsrSwtDispClimaSts {
    double UsrSwtDispClimaTSpForRowSecLe; /*!<Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )*/
    double UsrSwtDispClimaTSpForRowSecRi; /*!<Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )*/
    SeatClimaLvl UsrSwtDispSeatHeatLvlForRowSecLe;
    SeatClimaLvl UsrSwtDispSeatHeatLvlForRowSecRi;
    HmiHvacFanLvl UsrSwtDispFanLvlForRowSec;
    HmiCmptmtTSpSpcl UsrSwtDispClimaTSpSpclForRowSecLe;
    HmiCmptmtTSpSpcl UsrSwtDispClimaTSpSpclForRowSecRi;
};

/*!
 * \struct UsrSwtDispForSecRowSeatVentn
 */
struct UsrSwtDispForSecRowSeatVentn {
    SeatClimaLvl UsrSwtDispForSecRowSeatVentnLe;
    SeatClimaLvl UsrSwtDispForSecRowSeatVentnRi;
};

/*!
 * \struct UsrSwtDispReqForSecRowSeatVentn
 */
struct UsrSwtDispReqForSecRowSeatVentn {
    SeatClimaLvl UsrSwtDispReqForSecRowSeatVentnLe;
    SeatClimaLvl UsrSwtDispReqForSecRowSeatVentnRi;
    bool usrSwtDispUpdReqForSecRowSeatVentnLe;
    bool usrSwtDispUpdReqForSecRowSeatVentnRi;
};

/*!
 * \struct UsrSwtDispReqVrnt
 */
struct UsrSwtDispReqVrnt {
    OnOff1 UsrSwtDispSeatHeatFct;
    UsrSwtDispTUnit UsrSwtDispTUnit_;
};

/*!
 * \struct VFCGrp
 */
struct VFCGrp {
    uint16_t Grp1; /*!<Unit: Unitless,  Range:0->65535*/
    uint16_t Grp2; /*!<Unit: Unitless,  Range:0->65535*/
    uint16_t Grp3; /*!<Unit: Unitless,  Range:0->65535*/
};

/*!
 * \struct VehCfgPrm
 */
struct VehCfgPrm {
    uint8_t BlkIDBytePosn1; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t CCPBytePosn2; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t CCPBytePosn3; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t CCPBytePosn4; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t CCPBytePosn5; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t CCPBytePosn6; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t CCPBytePosn7; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t CCPBytePosn8; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
};

/*!
 * \struct VehMNomRec1
 */
struct VehMNomRec1 {
    uint16_t VehM; /*!<Unit: kg,  Range:0->10000, Resolution: (1.0*x+0.0, raw is unsigned, 14 bits )*/
    Qly2 VehMQly;
    TrlrM TrlrM_;
};

/*!
 * \struct VehModMngtGlbSafe1
 */
struct VehModMngtGlbSafe1 {
    UsgModSts1 UsgModSts;
    CarModSts1 CarModSts1_;
    uint8_t CarModSubtypWdCarModSubtyp; /*!<Unit: Unitless,  Range:0->7*/
    uint8_t EgyLvlElecMai; /*!<Unit: Unitless,  Range:0->15*/
    uint8_t EgyLvlElecSubtyp; /*!<Unit: Unitless,  Range:0->15*/
    uint8_t PwrLvlElecMai; /*!<Unit: Unitless,  Range:0->15*/
    uint8_t PwrLvlElecSubtyp; /*!<Unit: Unitless,  Range:0->15*/
    FltEgyCns1 FltEgyCnsWdSts;
    uint8_t Chks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t Cntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct VehMtnStSafe1
 */
struct VehMtnStSafe1 {
    VehMtnSt2 VehMtnSt;
    uint8_t VehMtnStChks; /*!<Unit: Unitless,  Range:0->255*/
    uint8_t VehMtnStCntr; /*!<Unit: Unitless,  Range:0->15*/
};

/*!
 * \struct VehSpdIndcd1
 * Data type for indicated vehicle speed, containing unitless number for the value of the speed, and a separate defined unit.
 */
struct VehSpdIndcd1 {
    uint16_t VehSpdIndcd; /*!<Unit: Unitless,  Range:0->511*/
    VehSpdIndcdUnit VeSpdIndcdUnit;
};

/*!
 * \struct VehSpdLgtSafe1
 */
struct VehSpdLgtSafe1 {
    double VehSpdLgt; /*!<Unit: m/s,  Range:0->125, Resolution: (0.00391*x+0.0, raw is unsigned, 15 bits )*/
    GenQf1 VehSpdLgtQf;
    uint8_t VehSpdLgtCntr; /*!<Unit: Unitless,  Range:0->15*/
    uint8_t VehSpdLgtChks; /*!<Unit: Unitless,  Range:0->255*/
};

/*!
 * \struct Vin1
 */
struct Vin1 {
    uint8_t BlockNr; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t VINSignalPos1; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t VINSignalPos2; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t VINSignalPos3; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t VINSignalPos4; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t VINSignalPos5; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t VINSignalPos6; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t VINSignalPos7; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
};

/*!
 * \struct VisnAgWideCfg1
 */
struct VisnAgWideCfg1 {
    OnOff1 SteerPahRe;
    OnOff1 SteerPahFrnt;
    OnOff1 SteerPahSide;
    OnOff1 DstInfoRe;
    OnOff1 DstInfoFrnt;
    OnOff1 DstInfoSide;
    OnOff1 CtraInfo;
    OnOff1 TwbrPah;
    OnOff1 ObjDetn;
};

/*!
 * \struct WarnAndIntvPen1
 */
struct WarnAndIntvPen1 {
    WarnAndIntv1 Sts;
    IdPen Pen;
};

/*!
 * \struct WarnTypForLaneChgWarnPen1
 */
struct WarnTypForLaneChgWarnPen1 {
    SoundHptc1 Sts;
    IdPen Pen;
};

/*!
 * \struct WhlRotToothCntrRec
 */
struct WhlRotToothCntrRec {
    uint8_t WhlRotToothCntrFrntLe; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t WhlRotToothCntrFrntRi; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t WhlRotToothCntrReLe; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
    uint8_t WhlRotToothCntrReRi; /*!<Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )*/
};

/*!
 * \struct WipgInfo
 * General information regarding wiping and the position of the wiper arm on the windscreen.
 */
struct WipgInfo {
    WipgSpdInfo WipgSpdInfo_;
    OnOff1 WiprActv;
    OnOff1 WiprInWipgAr;
};

} // end of namespace
#endif