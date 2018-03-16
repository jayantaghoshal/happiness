#coding: utf-8

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#AUTO GENERATED FILE, do not modify by hand.
# Generated with args: generate_signal_scaling.py --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml --fdxdescriptionfile=../CANoe/SPA2610/FDXDescriptionFile.xml --out=generated/pyDataElements.py
SWC_INPUT_FILE="SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml"
COM_INPUT_FILE="SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml"
from enum import Enum
import typing

#
# \struct ADataRawSafe1
#
class ADataRawSafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688fecf8>
        self.ALat = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5630>
        self.ALat1Qf = None  # type: Qf1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688fefd0>
        self.ALgt = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5630>
        self.ALgt1Qf = None  # type: Qf1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6890f0b8>
        self.AVert = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5630>
        self.AVertQf = None  # type: Qf1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6890f048>
        self.Chks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6890f080>
        self.Cntr = None  # type: int


#
# \enum AbsWarnIndcnReq# Controls the ABS warning indication to the driver.

#
class AbsWarnIndcnReq(Enum):
    AbsWarnIndcnOnReq=0
    AbsWarnIndcnFlsgReq=1
    Resd2=2
    AbsWarnIndcnOffReq=3

#
# \enum AccSts1
#
class AccSts1(Enum):
    Off=1
    Stb=2
    Stop=3
    StopTmp=4
    Actv=5

#
# \enum ActrDefrstSts
#
class ActrDefrstSts(Enum):
    Off=0
    On=1
    Limited=2
    NotAvailable=3
    TmrOff=4
    AutoCdn=5

#
# \enum ActrReq
#
class ActrReq(Enum):
    Off=0
    On=1
    AutOn=2

#
# \enum ActvInActv
#
class ActvInActv(Enum):
    Active=0
    Inactive=1

#
# \enum ActvnAvl1
#
class ActvnAvl1(Enum):
    NotAvl=0
    Avl=1
    Deactvd=2
    Actvd=3

#
# \struct ActvnOfCrsEco
#
class ActvnOfCrsEco:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.Sts = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum ActvnOfPrkgAut
#
class ActvnOfPrkgAut(Enum):
    Off=0
    PrkgTypAutSeln=1
    PrkgTypPara=2
    PrkgTypPerp=3
    PrkgManvCncl=4
    ParkOutManv=5
    Resd1=6
    Resd2=7

#
# \enum ActvtAutnmsPrkgCtrl
#
class ActvtAutnmsPrkgCtrl(Enum):
    Idle=0
    Activate=1
    Resume=2
    Pause=3

#
# \enum AdjSpdLimnSts2
#
class AdjSpdLimnSts2(Enum):
    Off=1
    Stb=2
    Actv=3
    Ovrd=4

#
# \enum AdjSpdLimnWarnCoding
#
class AdjSpdLimnWarnCoding(Enum):
    NoWarn=0
    SoundWarn=1
    VisWarn=2
    SoundAndVisWarn=3

#
# \enum AdprTurnSpdMod2# Identifier     Description
# CmftMod     CSA Comfort Mode
# NormMod    CSA Normal Mode
# DynMod      CSA Dynamic Mode

#
class AdprTurnSpdMod2(Enum):
    CmftMod=0
    NormMod=1
    DynMod=2

#
# \struct AdprTurnSpdModPen1
#
class AdprTurnSpdModPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689124e0>
        self.Sts = None  # type: AdprTurnSpdMod2
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \struct AgDataRawSafe1
#
class AgDataRawSafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f689126d8>
        self.RollRate = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5630>
        self.RollRateQf = None  # type: Qf1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f689128d0>
        self.YawRate = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5630>
        self.YawRateQf = None  # type: Qf1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68912898>
        self.Chks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68912940>
        self.Cntr = None  # type: int


#
# \enum AmbLiTintg# Amibiance LIgt color setting:
# 0 = Temperature
# 1 = Off
# 2 - 8 = Colour 1 - 7

#
class AmbLiTintg(Enum):
    T=0
    Off=1
    Tintg1=2
    Tintg2=3
    Tintg3=4
    Tintg4=5
    Tintg5=6
    Tintg6=7
    Tintg7=8

#
# \enum AmbTIndcdUnit
#
class AmbTIndcdUnit(Enum):
    Celsius=0
    Fahrenheit=1
    UkwnUnit=2

#
# \struct AmbTIndcdWithUnit# Indicated ambient temp with indicated unit and QF

#
class AmbTIndcdWithUnit:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68912860>
        self.AmbTIndcd = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68912780>
        self.AmbTIndcdUnit = None  # type: AmbTIndcdUnit
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.QF = None  # type: GenQf1


#
# \struct AmbTWithQly# The door module with a connected ambient temp sensor will send a temp value and QF, the door module without a connected ambient temp sensor will transmitt a temp value of 0x00 and QF = 0x00.

#
class AmbTWithQly:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68912f28>
        self.AmbTVal = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.Qly = None  # type: GenQf1


#
# \enum AprvdSts# Pairing status information for Out of band NFC pairing

#
class AprvdSts(Enum):
    NoData=0
    Setup=1
    Request=2
    Trusted=3
    NotKnown=4
    Disable=5


#
# \brief array Array8ByteU8
#
Array8ByteU8 = typing.List[int]  # Max 8 elements.

#
# \enum AsscSts
#
class AsscSts(Enum):
    Idle=0
    AsscDataTrfInin=1
    AsscDataTrf=2
    AsscReqActv=3
    AsscReqNotAprvd=4

#
# \struct AsyLaneChg1
#
class AsyLaneChg1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68914f28>
        self.Typ = None  # type: AsyLaneChgTyp1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68914a58>
        self.Sts = None  # type: AsyLaneChgSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68914780>
        self.Psbl = None  # type: AsyLaneChgPsbl1


#
# \enum AsyLaneChgPsbl1
#
class AsyLaneChgPsbl1(Enum):
    NotPsbl=0
    Le=1
    Ri=2
    Both=3

#
# \enum AsyLaneChgSts1
#
class AsyLaneChgSts1(Enum):
    Stb=0
    Planned=1
    ShowInt=2
    Resd3=3
    Executing=4
    Resd5=5
    Finshd=6
    Aborted=7
    Resd8=8
    Resd9=9
    Resd10=10

#
# \enum AsyLaneChgTyp1
#
class AsyLaneChgTyp1(Enum):
    NoIntent=0
    LaneChgLe=1
    LaneChgRi=2
    SafeStopInLane=3
    SafeStopToLe=4
    SafeStopToRi=5
    NotAvl=6
    Resd7=7
    Resd8=8
    Resd9=9
    Resd10=10

#
# \enum AudWarnReq1
#
class AudWarnReq1(Enum):
    Off=0
    PrepareToLeave=1
    LeaveNow=2

#
# \enum Aut1
#
class Aut1(Enum):
    Aut=0
    HalfAut=1

#
# \enum AutnmsPrkgActvDirectPrkg
#
class AutnmsPrkgActvDirectPrkg(Enum):
    DirectParkingNotActive=0
    DirectParkingSlot=1
    DirectParkingGarage=2

#
# \enum AutnmsPrkgActvPrkgDir
#
class AutnmsPrkgActvPrkgDir(Enum):
    Inactive=0
    FrontFirst=1
    RearFirst=2

#
# \enum AutnmsPrkgActvPrkgScenario
#
class AutnmsPrkgActvPrkgScenario(Enum):
    Inactive=0
    ParkIn=1
    ParkOut=2

#
# \enum AutnmsPrkgActvPrkgSide
#
class AutnmsPrkgActvPrkgSide(Enum):
    Inactive=0
    Left=1
    Right=2
    Straight=3

#
# \enum AutnmsPrkgActvRem
#
class AutnmsPrkgActvRem(Enum):
    Inactive=0
    NonRemote=1
    Remote=2

#
# \struct AutnmsPrkgActvScenario
#
class AutnmsPrkgActvScenario:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891a048>
        self.APActiveState = None  # type: AutnmsPrkgActvSt
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68918860>
        self.APActiveParkingSide = None  # type: AutnmsPrkgActvPrkgSide
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68918b38>
        self.APActiveSlotType = None  # type: AutnmsPrkgActvSlotTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68918710>
        self.APActiveParkingScenario = None  # type: AutnmsPrkgActvPrkgScenario
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68918630>
        self.APActiveParkingDirection = None  # type: AutnmsPrkgActvPrkgDir
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689189e8>
        self.APActiveRemote = None  # type: AutnmsPrkgActvRem
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891e9e8>
        self.APPauseStatus = None  # type: AutnmsPrkgPauseSts
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891e588>
        self.APCancelStatus = None  # type: AutnmsPrkgCnclSts
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68918550>
        self.APActiveDirectParking = None  # type: AutnmsPrkgActvDirectPrkg


#
# \enum AutnmsPrkgActvSlotTyp
#
class AutnmsPrkgActvSlotTyp(Enum):
    Inactive=0
    Parallel=1
    Perpendicular=2
    Garage=3
    Fishbone=4
    Resd1=5
    Resd2=6
    Resd3=7

#
# \enum AutnmsPrkgActvSt# Indicates which AP state is active

#
class AutnmsPrkgActvSt(Enum):
    Off=0
    PassiveScan=1
    PassiveScanSlotFound=2
    ParkInHighSpeed=3
    ParkInScan=4
    ParkInSlotFoundContinueForward=5
    ParkInStopToPark=6
    SideAndDirectionSelection=7
    DirectParkInInstruction=8
    ParkOutScanAndChooseScenario=9
    ParkOutConfirmDirection=10
    ParkOutStandInZone2=11
    ParkOutStartRemoteParkOut=12
    LookAround=13
    RemoteManeuvre=14
    EnterVehicle=15
    ExitVehicle=16
    WrongGear=17
    StandStillAndConfirm=18
    ReleaseBrakePedal=19
    RearwardMove=20
    ForwardMove=21
    ChangeGearForward=22
    ChangeGearRearward=23
    AutoBrakeInfo=24
    MoveAwayFromObject=25
    RemoteFinished=26
    Finished=27
    Pause=28
    Cancel=29
    PassiveSlotFoundStopToPark=30
    StopAndChangeGearForward=31
    StopAndChangeGearRearward=32
    Resd4=33
    Resd5=34

#
# \enum AutnmsPrkgAvlDirectPrkg
#
class AutnmsPrkgAvlDirectPrkg(Enum):
    Unavailable=0
    Available=1

#
# \enum AutnmsPrkgAvlPrkgDir
#
class AutnmsPrkgAvlPrkgDir(Enum):
    NoSlotDetected=0
    RearFirst=1
    FrontFirst=2
    RearAndFrontFirst=3

#
# \enum AutnmsPrkgAvlPrkgScenario
#
class AutnmsPrkgAvlPrkgScenario(Enum):
    NotAvailable=0
    ParkIn=1
    ParkOut=2
    ParkInAndParkOut=3

#
# \enum AutnmsPrkgAvlPrkgSide
#
class AutnmsPrkgAvlPrkgSide(Enum):
    NoSlotDetected=0
    Left=1
    Right=2
    Straight=3
    LeftAndRight=4
    LeftAndStraight=5
    RightAndStraight=6
    LeftAndRightAndStraight=7

#
# \enum AutnmsPrkgAvlRem
#
class AutnmsPrkgAvlRem(Enum):
    NoSlotDetected=0
    RemoteUnavailable=1
    RemoteAvailable=2

#
# \struct AutnmsPrkgAvlScenario
#
class AutnmsPrkgAvlScenario:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891ac88>
        self.APAvailableParkingSide = None  # type: AutnmsPrkgAvlPrkgSide
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891ab00>
        self.APAvailableParkingScenario = None  # type: AutnmsPrkgAvlPrkgScenario
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891e048>
        self.APAvailableSlotType = None  # type: AutnmsPrkgAvlSlotTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891a9e8>
        self.APAvailableParkingDirection = None  # type: AutnmsPrkgAvlPrkgDir
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891aef0>
        self.APAvailableRemote = None  # type: AutnmsPrkgAvlRem
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689210b8>
        self.APRecommendedParkingSide = None  # type: AutnmsPrkgRecmndPrkgSide
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68921358>
        self.APRecommendedSlotType = None  # type: AutnmsPrkgRecmndSlotTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891ee48>
        self.APRecommendedParkingDirection = None  # type: AutnmsPrkgRecmndPrkgDir
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68921240>
        self.APRecommendedRemote = None  # type: AutnmsPrkgRecmndRem
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891ef28>
        self.APRecommendedParkingScenario = None  # type: AutnmsPrkgRecmndPrkgScenario
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6891a940>
        self.APAvailableDirectParking = None  # type: AutnmsPrkgAvlDirectPrkg


#
# \enum AutnmsPrkgAvlSlotTyp
#
class AutnmsPrkgAvlSlotTyp(Enum):
    NoSlotDetected=0
    Parallel=1
    Perpendicular=2
    ParallelOrPerpendicular=3
    PerpendicularOrFishbone=4
    Fishbone=5
    ParallelOrFishbone=6
    ParallelOrPerpendicularOrFishbone=7
    Resd1=8
    Resd2=9

#
# \enum AutnmsPrkgCnclSts
#
class AutnmsPrkgCnclSts(Enum):
    Inactive=0
    Active=1
    DriverDeactivation=2
    ABSInterference=3
    ESCInterference=4
    TrailerAttached=5
    EngineStall=6
    Malfunction=7
    PathNotFound=8
    MaxNumberOfMoves=9
    MaxLongSpeedExceeded=10
    MaxRemoteLongSpeedExceeded=11
    PauseTimeout=12
    UndefinedCancel=13
    Resd1=14
    Resd2=15

#
# \enum AutnmsPrkgPauseSts
#
class AutnmsPrkgPauseSts(Enum):
    Inactive=0
    Active=1
    VehicleImpactWithObstacle=2
    VehicleEmergencyIntervention=3
    OpenDoor=4
    SteeringOverride=5
    StandstillDueToBrake=6
    GearOverride=7
    DriverInZone1=8
    SafetySwitchReleased=9
    DriverOutOfRange=10
    UndefinedPause=11
    AutobrakeUnavailable=12
    DriverDeactivation=13
    Resd3=14
    Resd4=15

#
# \enum AutnmsPrkgRecmndPrkgDir
#
class AutnmsPrkgRecmndPrkgDir(Enum):
    NoSlotDetected=0
    RearFirst=1
    FrontFirst=2

#
# \enum AutnmsPrkgRecmndPrkgScenario
#
class AutnmsPrkgRecmndPrkgScenario(Enum):
    NoSlotDetected=0
    ParkIn=1
    ParkOut=2

#
# \enum AutnmsPrkgRecmndPrkgSide
#
class AutnmsPrkgRecmndPrkgSide(Enum):
    NoSlotDetected=0
    Left=1
    Right=2
    Straight=3

#
# \enum AutnmsPrkgRecmndRem
#
class AutnmsPrkgRecmndRem(Enum):
    NoSlotDetected=0
    NonRemote=1
    Remote=2

#
# \enum AutnmsPrkgRecmndSlotTyp
#
class AutnmsPrkgRecmndSlotTyp(Enum):
    NoSlotDetected=0
    Parallel=1
    Perpendicular=2
    Garage=3
    Fishbone=4
    Resd1=5
    Resd2=6

#
# \enum AutnmsPrkgSeldDirectPrkg
#
class AutnmsPrkgSeldDirectPrkg(Enum):
    DirectParkingNotSelected=0
    DirectParkingSelected=1

#
# \enum AutnmsPrkgSeldPrkgDir
#
class AutnmsPrkgSeldPrkgDir(Enum):
    Inactive=0
    FrontFirst=1
    RearFirst=2

#
# \enum AutnmsPrkgSeldPrkgScenario
#
class AutnmsPrkgSeldPrkgScenario(Enum):
    Inactive=0
    ParkIn=1
    ParkOut=2

#
# \enum AutnmsPrkgSeldPrkgSide
#
class AutnmsPrkgSeldPrkgSide(Enum):
    Inactive=0
    Left=1
    Right=2
    StraightForward=3

#
# \enum AutnmsPrkgSeldRem
#
class AutnmsPrkgSeldRem(Enum):
    Inactive=0
    NonRemote=1
    Remote=2

#
# \struct AutnmsPrkgSeldScenario
#
class AutnmsPrkgSeldScenario:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68921b70>
        self.AutnmsPrkgSeldSlotTyp = None  # type: AutnmsPrkgSeldSlotTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68921748>
        self.AutnmsPrkgSeldPrkgScenario = None  # type: AutnmsPrkgSeldPrkgScenario
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68921668>
        self.AutnmsPrkgSeldPrkgDir = None  # type: AutnmsPrkgSeldPrkgDir
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68921898>
        self.AutnmsPrkgSeldPrkgSide = None  # type: AutnmsPrkgSeldPrkgSide
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68921a20>
        self.AutnmsPrkgSeldRem = None  # type: AutnmsPrkgSeldRem
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689215c0>
        self.AutnmsPrkgSeldDirectPrkg = None  # type: AutnmsPrkgSeldDirectPrkg


#
# \enum AutnmsPrkgSeldSlotTyp
#
class AutnmsPrkgSeldSlotTyp(Enum):
    Inactive=0
    Parallel=1
    Perpendicular=2
    Fishbone=3
    Garage=4
    HomeZone=5
    ParkLaunch=6
    Resd=7

#
# \enum AvlSts2# Availability status
# 0= not available
# 1= available

#
class AvlSts2(Enum):
    NotAvl=0
    Avl=1

#
# \enum AvlStsForLatAutDrv5
#
class AvlStsForLatAutDrv5(Enum):
    NoMsg=0
    LatCtrlNotAvl=1
    HiSpd=2
    LaneLimrMiss=3
    VehToFolwMiss=4
    OvrdTiMaxExcdd=5
    DrvrNotInLoopDetd=6
    DrvrBucdRqrd=7
    DrvrDoorNotClsd=8
    GearNotInDrv=9
    SnsrBlkd=10
    HldTiMaxExcdd=11
    DrvModSeldNotOk=12
    EpbAppld=13
    SpdLowLimExcdd=14
    NotInUse2=15
    NotInUse3=16
    NotInUse4=17
    NotInUse5=18
    NotInUse6=19
    NotInUse7=20

#
# \enum AxleWarn
#
class AxleWarn(Enum):
    NoWarn=0
    SoftWarn=1
    HardWarn=2
    Resd=3

#
# \enum BltLockSt1
#
class BltLockSt1(Enum):
    Unlock=0
    Lock=1

#
# \struct BltLockStFrnt# Belt lock state for front seats.

#
class BltLockStFrnt:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68923748>
        self.BltLockSt1 = None  # type: BltLockSt1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689299b0>
        self.BltLockSts = None  # type: DevErrSts2


#
# \struct BltLockStRe# Belt lock state for rear seats (both 2nd and 3rd row).

#
class BltLockStRe:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68923748>
        self.BltLockSt1 = None  # type: BltLockSt1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689299b0>
        self.BltLockSts = None  # type: DevErrSts2
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6892ac18>
        self.BltLockEquid = None  # type: CptEquid


#
# \struct BrkAndAbsWarnIndcnReqRec1
#
class BrkAndAbsWarnIndcnReqRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e42b0>
        self.BrkWarnIndcnReq = None  # type: OnOff2
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688feb38>
        self.AbsWarnIndcnReq = None  # type: AbsWarnIndcnReq
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68923940>
        self.BrkAndAbsWarnIndcnReqChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68923b38>
        self.BrkAndAbsWarnIndcnReqCntr = None  # type: int


#
# \struct BrkFricTqAtWhlActRec
#
class BrkFricTqAtWhlActRec:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f689239e8>
        self.BrkFricTqAtWhlFrntLeAct = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68923cc0>
        self.BrkFricTqAtWhlFrntRiAct = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68923d30>
        self.BrkFricTqAtWhlReLeAct = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68923da0>
        self.BrkFricTqAtWhlReRiAct = None  # type: int


#
# \enum BtnAudVolSts# SImple representation of  a virtual (multistep) button for adjusting audio volume.
# 0 = button not pressed. No request to adjust volume
# 1 = button pressed for "volume up"
# 2 = buttono pressed for "volume down"
#
# Receiver side controls how the button status is used.

#
class BtnAudVolSts(Enum):
    NoBtnPsd=0
    BtnVolUpPsd=1
    BtnVolDwnPsd=2

#
# \enum BtnMmedLeRiSts# Represents status of a (multilevel) button "Left/Right.
#
# Semantic defined by receiver side. (Previou/Next, FFBW/FFWD, etc)

#
class BtnMmedLeRiSts(Enum):
    NoBtnPsd=0
    BtnLePsd=1
    BtnRiPsd=2

#
# \enum BtnMmedSetSts# Represents status of a button "Set".
#
# Semantic defined by receiver side. (Activate/Deactivate, Play/Pause, Mute/UnMute etc)

#
class BtnMmedSetSts(Enum):
    BtnSetNotPsd=0
    BtnSetPsd=1

#
# \enum CalPsbl# Warning status
# == 0	Calibration status OK (Normal IHU behavior)
# == 1	Calibration not possible (All 4 rolling wheels shall be grey, menu item for recalibration of iTPMS system shall be unavailable and gray).

#
class CalPsbl(Enum):
    CalPsbl=0
    CalNoPsbl=1

#
# \enum CallSts# Carries call presence information. Defined for CallSts range [0-1]
#
# 0 = Inactive. There is no active call present with any telephony solution in the system.
# 1 = Active. Atleast one active call is present in the system.

#
class CallSts(Enum):
    Inactive=0
    Active=1

#
# \struct CamIndReq
#
class CamIndReq:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.CamFrnt = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.CamRe = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.CamLe = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.CamRi = None  # type: OnOff1


#
# \enum CarModSts1
#
class CarModSts1(Enum):
    CarModNorm=0
    CarModTrnsp=1
    CarModFcy=2
    CarModCrash=3
    CarModDyno=5

#
# \struct CchForFuEco1# Eco signals for DIM.

#
class CchForFuEco1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68927240>
        self.BarForFuEco = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f689274a8>
        self.GrdForFuEco = None  # type: float


#
# \enum ChrgrWirelsSts# Signal for the current status of the Wireless Phone charger

#
class ChrgrWirelsSts(Enum):
    Stopped=0
    Charging=1
    Missaligned=2
    UnknownObj=3
    Completed=4
    SwError=5
    HwError=6
    Reserved=7

#
# \enum ClimaWarn# 0=No Warning
# 1=Fuel Low
# 2=Battery Low
# 3=Fuel and Battery Low
# 4=Temperature Low
# 5=Temperature High

#
class ClimaWarn(Enum):
    NoWarn=0
    FuLo=1
    BattLo=2
    FuAndBattLo=3
    TLo=4
    THi=5
    Error=6
    HVError=7
    ActvnLimd=8

#
# \enum ClkFmt# 24h or 12h indication of clock

#
class ClkFmt(Enum):
    Hr24=0
    Hr12=1
    UkwnClkFmt=2

#
# \enum CllsnWarn1
#
class CllsnWarn1(Enum):
    NoWarn=0
    WarnLvl1=1
    WarnLvl2=2
    Resd3=3

#
# \struct CllsnWarnSide1
#
class CllsnWarnSide1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68927c88>
        self.Le = None  # type: CllsnWarn1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68927c88>
        self.Ri = None  # type: CllsnWarn1


#
# \struct CmptmtAirTEstimdExtd# Estimated compartment temperature with quality flag

#
class CmptmtAirTEstimdExtd:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68927e80>
        self.ComptmtT = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.QlyFlg = None  # type: GenQf1


#
# \struct CmptmtTFrnt
#
class CmptmtTFrnt:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68927eb8>
        self.CmptmtTFrnt = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68927ef0>
        self.CmptmtTFrntQf = None  # type: CmptmtTFrntQf
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc710>
        self.FanForCmptmtTRunng = None  # type: Flg1


#
# \enum CmptmtTFrntQf# Interpretation:
# SnsrDataUndefd - Accuracy of data undefinable
# FanNotRunning - Aspiration fan not running
# SnsrDataNotOk - Data accuracy not within specification
# SnsrDataOk - Data is calculated within specified accuracy

#
class CmptmtTFrntQf(Enum):
    SnsrDataUndefd=0
    FanNotRunning=1
    SnsrDataNotOk=2
    SnsrDataOk=3

#
# \struct CnsPrmRec1# Consumption parameters for Navigation

#
class CnsPrmRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6892a080>
        self.SpdVal = None  # type: float
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6892a400>
        self.SpdIdx = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6892a470>
        self.AuxPwrPrsnt = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6892a4e0>
        self.AuxPwrLvlInct = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6892a550>
        self.AuxTiPrsnt = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6892a5c0>
        self.AuxTiTranPha = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6892a630>
        self.AVal = None  # type: float


#
# \struct ConSftyWarn1
#
class ConSftyWarn1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6892a390>
        self.ConSftyWarnId = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.ConSftyWarnSnd = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6892a3c8>
        self.ConSftyWarnTyp = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6892a278>
        self.ConSftyWarnLvl = None  # type: ConSftyWarnLvl1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6892a6a0>
        self.DistanceToWarning = None  # type: int


#
# \enum ConSftyWarnLvl1
#
class ConSftyWarnLvl1(Enum):
    NoWarn=0
    LoLvl=1
    MedLvl=2
    HiLvl=3

#
# \enum CoolgReqForDisp# Interpretation:
# NoReq - No active cooling requested
# LoReq - Low cooling need
# HiReq - High cooling need

#
class CoolgReqForDisp(Enum):
    NoReq=0
    LoReq=1
    HiReq=2

#
# \enum CoolgStsForDisp# Interpretation:
# NoCoolgActv - No cooling request received, no actions taken
# CoolgLvlStrtd1 - Activated cooling in level 1 started
# CoolgLvlStrtd2 - Activated cooling in level 2 started

#
class CoolgStsForDisp(Enum):
    NoCoolgActv=0
    CoolgLvlStrtd1=1
    CoolgLvlStrtd2=2
    Resd=3

#
# \enum CptEquid# Description if a component is equipped or not.

#
class CptEquid(Enum):
    Equid=0
    NotEquid=1

#
# \enum CrsCtrlrSts1
#
class CrsCtrlrSts1(Enum):
    Off=1
    Stb=2
    Actv=3

#
# \enum CtraIndcn1
#
class CtraIndcn1(Enum):
    NoCtraWarn=0
    CtraWarn=1

#
# \enum CtrlDirOfTrfcLi1
#
class CtrlDirOfTrfcLi1(Enum):
    NoDirection=0
    Straight=1
    Left=2
    LeftStraight=3
    Right=4
    StraightRight=5
    LeftRight=6
    All=7

#
# \enum CurtActvnTyp1
#
class CurtActvnTyp1(Enum):
    BtnReld=0
    BtnPsd=1
    Spare1=2
    Spare2=3

#
# \struct DataSpclTyp
#
class DataSpclTyp:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6892a358>
        self.DataIdn = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929550>
        self.DataNrSpcl1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f689295c0>
        self.DataNrSpcl2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929630>
        self.DataNrSpcl3 = None  # type: int


#
# \enum DateFmt# DateFmt1 = YMD
# DateFmt2 = DMY
# DateFmt3 = MDY

#
class DateFmt(Enum):
    DateFmt1=0
    DateFmt2=1
    DateFmt3=2
    UkwnFmt=3

#
# \enum DateOrTi
#
class DateOrTi(Enum):
    None_=0
    Date=1
    Ti=2
    DateTi=3

#
# \struct DateTi1ForSet
#
class DateTi1ForSet:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68929748>
        self.DateOrTi = None  # type: DateOrTi
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f689294e0>
        self.Year = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929b00>
        self.Month = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929be0>
        self.Day = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929c50>
        self.Hour = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929cc0>
        self.Minute = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929d30>
        self.Second = None  # type: int


#
# \struct DateTi30
#
class DateTi30:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929ac8>
        self.Yr1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929e48>
        self.Mth1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929eb8>
        self.Day = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929f28>
        self.Hr1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68929e10>
        self.Mins1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6892f080>
        self.Sec1 = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.DataValid = None  # type: NoYes1


#
# \enum DevErrSts2
#
class DevErrSts2(Enum):
    NoFlt=0
    Flt=1

#
# \enum DiagcForPanCenCtrl2
#
class DiagcForPanCenCtrl2(Enum):
    NoFlt=0
    FanErr=1
    OutdURng=2
    TmrErr=3
    MemErr=4
    Spare6=5
    Spare7=6
    SnrFltT=7

#
# \enum DiagcForRCSM2
#
class DiagcForRCSM2(Enum):
    NoFlt=0
    SpiErr=1
    OutdURng=2
    TmrErr=3
    MemErr=4
    ADErr=5
    Spare1=6
    Spare2=7

#
# \enum DiagcStsTypDMSM
#
class DiagcStsTypDMSM(Enum):
    OK=0
    CmnFailr=1
    Spare1=2
    Spare2=3

#
# \enum DispOfPrpsnMod3
#
class DispOfPrpsnMod3(Enum):
    NotRdy=0
    ChrgnWithOnBdChrgr=1
    PrpsnMotElecAndPrpsnEng=2
    PrpsnMotElecAndPrpsnEngAndChrgn=3
    OnlyEngPrpsn=4
    OnlyEngPrpsnAndChrgn=5
    OnlyPrpsnMotElec=6
    NoPrpsnMotElecAndEngOff=7
    NoPrpsnMotElecAndOnEng=8
    NoPrpsnMotElecAndOnEngAndChrgn=9
    BrkgRgnAndEngOff=10
    BrkgRgnAndOnEng=11
    BrkgRgnAndEngOnAndChrgn=12
    NotOfUse2=13
    NotOfUse3=14
    NotOfUse4=15

#
# \enum DoorSts2# Status of the door.

#
class DoorSts2(Enum):
    Ukwn=0
    Opend=1
    Clsd=2

#
# \struct DriveAwayInfoWarnReq1
#
class DriveAwayInfoWarnReq1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5da0>
        self.ReqSrc = None  # type: ReqSrc1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68930128>
        self.TiToDrvCntDwnTi = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6892af28>
        self.CtrlDirOfTrfcLi = None  # type: CtrlDirOfTrfcLi1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688962e8>
        self.VisWarnReq = None  # type: VisWarnReq1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68918320>
        self.AudWarnReq = None  # type: AudWarnReq1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d55c0>
        self.InterruptionSrc = None  # type: InterruptionSrc1


#
# \enum DrvModReqType1
#
class DrvModReqType1(Enum):
    Undefd=0
    DrvMod1=1
    DrvMod2=2
    DrvMod3=3
    DrvMod4=4
    DrvMod5=5
    DrvMod6=6
    DrvMod7=7
    DrvMod8=8
    DrvMod9=9
    DrvMod10=10
    DrvMod11=11
    DrvMod12=12
    DrvMod13=13
    DrvMod14=14
    Err=15

#
# \enum DrvgSide1
#
class DrvgSide1(Enum):
    DrvgSideLe=0
    DrvgSideRi=1

#
# \enum DrvrDesDir1# Describes the desired vehicle movement direction from the driver.
# Contains only the drivers longitudinal direction intent. In which direction shall the car move.

#
class DrvrDesDir1(Enum):
    Undefd=0
    Fwd=1
    Rvs=2
    Neut=3
    Resd0=4
    Resd1=5
    Resd2=6
    Resd3=7

#
# \enum DrvrDispSetg
#
class DrvrDispSetg(Enum):
    Cmft=0
    Eco=1
    Dyn=2

#
# \struct DrvrDispSetgReq
#
class DrvrDispSetgReq:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68930b70>
        self.Sts = None  # type: DrvrDispSetg


#
# \enum DrvrHmiBackGndInfo
#
class DrvrHmiBackGndInfo(Enum):
    DrvrHmiBackGndInfoSetg0=0
    DrvrHmiBackGndInfoSetg1=1
    DrvrHmiBackGndInfoSetg2=2
    DrvrHmiBackGndInfoSetg3=3
    DrvrHmiBackGndInfoSetg4=4
    DrvrHmiBackGndInfoSetg5=5

#
# \struct DrvrHmiBackGndInfoSetg
#
class DrvrHmiBackGndInfoSetg:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68930d30>
        self.Setg = None  # type: DrvrHmiBackGndInfo


#
# \enum DrvrHmiDispdMod1
#
class DrvrHmiDispdMod1(Enum):
    DispDftMod=0
    DispAudMod=1
    DispNavMod=2

#
# \struct DrvrHmiDispdModPen
#
class DrvrHmiDispdModPen:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68930f60>
        self.Sts = None  # type: DrvrHmiDispdMod1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen



#
# \brief array DrvrHmiForHmiCen
#
DrvrHmiForHmiCen = typing.List[int]  # Max 254 elements.

#
# \struct DrvrHmiUserIfSetgReq
#
class DrvrHmiUserIfSetgReq:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689321d0>
        self.Setg = None  # type: DrvrHmiUsrIfSetg


#
# \enum DrvrHmiUsrIfSetg
#
class DrvrHmiUsrIfSetg(Enum):
    UsrIfDft=0
    UsrIfVrnt1=1
    UsrIfVrnt2=2
    UsrIfVrnt3=3
    UsrIfVrnt4=4
    UsrIfVrnt5=5
    UsrIfVrnt6=6
    UsrIfVrnt7=7
    UsrIfVrnt8=8
    UsrIfVrnt9=9

#
# \enum DrvrPfmncLvl2
#
class DrvrPfmncLvl2(Enum):
    Ukwn=0
    DrvrLvl1=1
    DrvrLvl2=2
    DrvrLvl3=3
    DrvrLvl4=4
    DrvrLvl5=5

#
# \enum DrvrPfmncWarnReq1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No Warning
# 1			1			WarnForDrvrNotActv	Warning For Driver Not Active	Warning for Inattentive Driver
# 2			2			WarnForDrvrTire		Warning For Driver Tire		Warning for Sleepy Driver
# 3			3			NotInUse			Not In Use			Not used

#
class DrvrPfmncWarnReq1(Enum):
    NoWarn=0
    WarnForDrvrNotActv=1
    WarnForDrvrTire=2
    NotInUse=3

#
# \struct DrvrSeatSwtSts1
#
class DrvrSeatSwtSts1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68886d68>
        self.DrvrSeatSwtSldSts = None  # type: SwtHozlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688880f0>
        self.DrvrSeatSwtHeiSts = None  # type: SwtVertSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688880f0>
        self.DrvrSeatSwtHeiFrntSts = None  # type: SwtVertSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68886d68>
        self.DrvrSeatSwtInclSts = None  # type: SwtHozlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68886d68>
        self.DrvrSeatSwtAdjmtOfSpplFctHozlSts = None  # type: SwtHozlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688880f0>
        self.DrvrSeatSwtAdjmtOfSpplFctVertSts = None  # type: SwtVertSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688880f0>
        self.DrvrSeatSwtSelnOfSpplFctSts = None  # type: SwtVertSts1


#
# \enum DrvrSpprtFct
#
class DrvrSpprtFct(Enum):
    NoReq=0
    ACC=1
    CC=2
    SL=3

#
# \enum DstLong
#
class DstLong(Enum):
    km=0
    miles=1
    UkwnUnit=2

#
# \enum DstOfSnsr1
#
class DstOfSnsr1(Enum):
    DistanceInfinityOrNotUsed=0
    Distance150cm=1
    Distance146cm=2
    Distance142cm=3
    Distance138cm=4
    Distance134cm=5
    Distance130cm=6
    Distance126cm=7
    Distance122cm=8
    Distance118cm=9
    Distance114cm=10
    Distance110cm=11
    Distance106cm=12
    Distance102m=13
    Distance98cm=14
    Distance94cm=15
    Distance90cm=16
    Distance86cm=17
    Distance82cm=18
    Distance78cm=19
    Distance74cm=20
    Distance70cm=21
    Distance66cm=22
    Distance62cm=23
    Distance58cm=24
    Distance54cm=25
    Distance50cm=26
    Distance46cm=27
    Distance42cm=28
    Distance38cm=29
    Distance34cm=30
    Distance30cm=31

#
# \enum DstSho
#
class DstSho(Enum):
    Mtr=0
    Ft=1
    Yards=2
    UkwnUnit=3

#
# \struct DstToEmptyWithUnit
#
class DstToEmptyWithUnit:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f689329b0>
        self.DstToEmpty = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68935748>
        self.DstUnit = None  # type: DstUnit


#
# \struct DstToManvLocnByNav
#
class DstToManvLocnByNav:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f689356d8>
        self.DstToManv = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.CntDwnToManvStrt = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e64e0>
        self.PosnQly = None  # type: PosnFromNavQly
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SpprtForFct = None  # type: NoYes1


#
# \enum DstUnit
#
class DstUnit(Enum):
    km=0
    miles=1

#
# \struct EgyCostForRouteRec1# Remaining energy cost for route

#
class EgyCostForRouteRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68935828>
        self.Egy = None  # type: float
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68935a90>
        self.Idx = None  # type: int


#
# \enum EnaResu
#
class EnaResu(Enum):
    Disable=0
    Enable=1

#
# \enum EnableDisableCoding
#
class EnableDisableCoding(Enum):
    Disabled=0
    Enabled=1

#
# \enum EngCooltIndcn
#
class EngCooltIndcn(Enum):
    MsgNoWarn=0
    EngTHiToSpdRedn=1
    EngTHiToStopSafe=2
    EngTHiToEngStop=3
    EngTHiToMan=4
    CooltLvlLoToEngStop=5
    CooltLvlLoToStopSafe=6
    CooltLvlLoToMan=7
    CooltLvlLoToLvlChk=8
    MsgSrvRqrd=9

#
# \struct EngIdleEco1
#
class EngIdleEco1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.Sts = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \struct EngNSafe
#
class EngNSafe:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68935f98>
        self.EngN = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68937160>
        self.EngNGrdt = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f689371d0>
        self.EngNChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68937240>
        self.EngNCntr = None  # type: int


#
# \enum EngOilLvlSts1# Engine oil level warning information

#
class EngOilLvlSts1(Enum):
    OilLvlOk=0
    OilLvlLo1=1
    OilLvlLo2=2
    OilLvlHi=3
    OilLvlSrvRqrd=4
    Resd=5

#
# \enum EngOilPWarn1# Engine oil pressure warning information

#
class EngOilPWarn1(Enum):
    EngOilPOk=0
    EngOilPNotOk=1

#
# \struct EpbLampReqRec
#
class EpbLampReqRec:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689374a8>
        self.EpbLampReq = None  # type: EpbLampReqType1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f689370f0>
        self.EpbLampReqChks = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68937550>
        self.EpbLampReqCntr = None  # type: int


#
# \enum EpbLampReqType1
#
class EpbLampReqType1(Enum):
    On=0
    Off=1
    Flash2=2
    Flash3=3

#
# \enum Err1
#
class Err1(Enum):
    NoErr=0
    Err=1

#
# \struct EscSptModReqdByDrvrRec1
#
class EscSptModReqdByDrvrRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.EscSptModReqdByDrvr = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum EscSt1# Information on activation/ deactivation of Electronic Stability Control (ESC)

#
class EscSt1(Enum):
    Inin=0
    Ok=1
    TmpErr=2
    PrmntErr=3
    UsrOff=4

#
# \struct EscStSafe1
#
class EscStSafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68937908>
        self.EscSt = None  # type: EscSt1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68937940>
        self.EscStChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68937c88>
        self.EscStCntr = None  # type: int


#
# \enum EscWarnIndcnReq
#
class EscWarnIndcnReq(Enum):
    EscWarnIndcnOnReq=0
    EscWarnIndcnFlsgReq=1
    Resd2=2
    EscWarnIndcnOffReq=3

#
# \struct EscWarnIndcnReqRec1
#
class EscWarnIndcnReqRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68937be0>
        self.EscWarnIndcnReq = None  # type: EscWarnIndcnReq
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68937c50>
        self.EscWarnIndcnReqChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68937f60>
        self.EscWarnIndcnReqCntr = None  # type: int


#
# \struct ExtrMirrTiltSetg
#
class ExtrMirrTiltSetg:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e42b0>
        self.MirrDrvr = None  # type: OnOff2
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e42b0>
        self.MirrPass = None  # type: OnOff2
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.IdPen = None  # type: IdPen


#
# \enum FailrNoFailr1
#
class FailrNoFailr1(Enum):
    NoFailr=0
    Failr=1

#
# \enum FcnAvlSts1
#
class FcnAvlSts1(Enum):
    NoData=0
    NotAvl=1
    Avl=2

#
# \enum FctSts2# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used

#
class FctSts2(Enum):
    Ukwn=0
    Off=1
    SpdLoStb=2
    SpdHiStb=3
    NotAvl=4
    SrvRqrd=5
    On=6
    NotInUse=7

#
# \enum FldLvl# 0= High fluid level
# 1= Low fluid level

#
class FldLvl(Enum):
    FldLvlHi=0
    FldLvlLo=1

#
# \enum Flg1# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)

#
class Flg1(Enum):
    Rst=0
    Set=1

#
# \enum FltCfmd1
#
class FltCfmd1(Enum):
    NoFltCfmd=0
    FltCfmd=1

#
# \enum FltEgyCns1
#
class FltEgyCns1(Enum):
    NoFlt=0
    Flt=1

#
# \enum FrntWiprLvrCmd1
#
class FrntWiprLvrCmd1(Enum):
    FrntWiprOff=0
    FrntWiprSngStk=1
    FrntWiprIntm=2
    FrntWiprContnsLoSpd=3
    FrntWiprContnsHiSpd=4

#
# \struct FrntWiprLvrReq2
#
class FrntWiprLvrReq2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bca20>
        self.FrntWiprLvrCmd1 = None  # type: FrntWiprLvrCmd1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5630>
        self.FrntWiprLvrQf = None  # type: Qf1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688bc080>
        self.FrntWiprLvrCrc = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688bccc0>
        self.FrntWiprLvrCntr = None  # type: int


#
# \enum FuCnsIndcdUnit# LperHectoKiloM = L/100Km
# KiloMPerL = Km/L
# MilePerGallonEngland = MPG (UK)
# MilePerGallonUsa = MPG (US)

#
class FuCnsIndcdUnit(Enum):
    LperHectoKiloM=0
    KiloMPerL=1
    MilePerGallonEngland=2
    MilePerGallonUsa=3
    UkwnUnit=4

#
# \enum FuLvlLoIndcnToNav1
#
class FuLvlLoIndcnToNav1(Enum):
    Off=0
    FuLvlLoIndcn=1
    FuLvlLoWarn=2

#
# \struct FuLvlValWithQly
#
class FuLvlValWithQly:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688bcc50>
        self.FuLvlValFromFuTbl = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.GenQF = None  # type: GenQf1


#
# \struct GearIndcnRec2
#
class GearIndcnRec2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd080>
        self.GearIndcn = None  # type: GearIndcnTyp2
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd828>
        self.GearShiftIndcn = None  # type: GearShiftIndcn
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd080>
        self.GearTarIndcn = None  # type: GearIndcnTyp2


#
# \enum GearIndcnTyp2
#
class GearIndcnTyp2(Enum):
    NoIndcn=0
    Gear1=1
    Gear2=2
    Gear3=3
    Gear4=4
    Gear5=5
    Gear6=6
    Gear7=7
    Gear8=8
    Gear9=9
    Gear10=10
    Gear11=11
    Gear2468=12
    Gear13579=13
    Neut=14
    Rvs=15

#
# \enum GearShiftIndcn
#
class GearShiftIndcn(Enum):
    NoShiftIndcn=0
    ShiftUpIndcn=1
    CoolShiftIndcn=2
    ShiftDwnIndcn=3

#
# \enum GenQf1
#
class GenQf1(Enum):
    UndefindDataAccur=0
    TmpUndefdData=1
    DataAccurNotWithinSpcn=2
    AccurData=3

#
# \enum GlbRst# Specifies global reset

#
class GlbRst(Enum):
    NoRst=0
    CarSetgRst=1
    PrsnlSetgActvRst=2
    PrsnlSetgAllRst=3
    DataAndSetgAllRst=4
    Resd1=5
    Resd2=6
    Resd3=7

#
# \struct GlbRstForSetgAndData# Global reset activation critera together with profile information

#
class GlbRstForSetgAndData:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bda58>
        self.GlbRst = None  # type: GlbRst
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum HeatrPreCdngTyp
#
class HeatrPreCdngTyp(Enum):
    NoReq=0
    Aut=1
    Fu=2
    Elec=3


#
# \brief array HmiCenForDrvrHmi
#
HmiCenForDrvrHmi = typing.List[int]  # Max 254 elements.

#
# \enum HmiCmptmtAirDistbnFrnt# Interpretation:
# 0 - Flr - Floor
# 1 - Vent - Vent
# 2 - Defrst - Defrost
# 3 - FlrDefrst - Floor / defrost
# 4 - FlrVent - Floor / vent
# 5 - VentDefrst - Vent / defrost
# 6 - FlrVentDefrst - Floor / vent / defrost
# 7 - Aut - Auto

#
class HmiCmptmtAirDistbnFrnt(Enum):
    Flr=0
    Vent=1
    Defrst=2
    FlrDefrst=3
    FlrVent=4
    VentDefrst=5
    FlrVentDefrst=6
    Aut=7

#
# \enum HmiCmptmtCoolgReq
#
class HmiCmptmtCoolgReq(Enum):
    Off=0
    Auto=1

#
# \struct HmiCmptmtTSp
#
class HmiCmptmtTSp:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688bdda0>
        self.HmiCmptmtTSpForRowFirstLe = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c04e0>
        self.HmiCmptmtTSpSpclForRowFirstLe = None  # type: HmiCmptmtTSpSpcl
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688c06d8>
        self.HmiCmptmtTSpForRowFirstRi = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c04e0>
        self.HmiCmptmtTSpSpclForRowFirstRi = None  # type: HmiCmptmtTSpSpcl
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688c06a0>
        self.HmiCmptmtTSpForRowSecLe = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c04e0>
        self.HmiCmptmtTSpSpclForRowSecLe = None  # type: HmiCmptmtTSpSpcl
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688c0748>
        self.HmiCmptmtTSpForRowSecRi = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c04e0>
        self.HmiCmptmtTSpSpclForRowSecRi = None  # type: HmiCmptmtTSpSpcl


#
# \enum HmiCmptmtTSpSpcl
#
class HmiCmptmtTSpSpcl(Enum):
    Norm=0
    Lo=1
    Hi=2

#
# \struct HmiDefrstElecReq
#
class HmiDefrstElecReq:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6890f780>
        self.FrntElecReq = None  # type: ActrReq
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6890f780>
        self.ReElecReq = None  # type: ActrReq
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6890f780>
        self.MirrElecReq = None  # type: ActrReq


#
# \struct HmiDefrstElecSts
#
class HmiDefrstElecSts:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6890f4e0>
        self.Frnt = None  # type: ActrDefrstSts
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6890f4e0>
        self.Re = None  # type: ActrDefrstSts
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6890f4e0>
        self.Mirrr = None  # type: ActrDefrstSts


#
# \enum HmiHvacFanLvl
#
class HmiHvacFanLvl(Enum):
    Off=0
    Min=1
    LvlMan1=2
    LvlMan2=3
    LvlMan3=4
    LvlMan4=5
    LvlMan5=6
    LvlMan6=7
    LvlMan7=8
    LvlAutMinusMinus=9
    LvlAutMinus=10
    LvlAutoNorm=11
    LvlAutPlus=12
    LvlAutPlusPlus=13
    Max=14

#
# \enum HmiHvacReCtrl
#
class HmiHvacReCtrl(Enum):
    Off=0
    OffWithNoOccpt=1
    On=2

#
# \enum HmiHvacRecircCmd# Interpretation:
# 0 - OsaFull - Full OSA
# 1 - RecircFull - Full recirculation
# 2 - RecircFullWithTiOut - Full REC with timeout
# 3 - AutWithAirQly - AUTO with AQS
# 4 - Aut - Auto
# 5 - Resd - Reserved

#
class HmiHvacRecircCmd(Enum):
    Aut=0
    AutWithAirQly=1
    RecircFull=2

#
# \struct HmiSeatClima
#
class HmiSeatClima:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.HmiSeatHeatgForRowFirstLe = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.HmiSeatHeatgForRowFirstRi = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.HmiSeatHeatgForRowSecLe = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.HmiSeatHeatgForRowSecRi = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.HmiSeatVentnForRowFirstLe = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.HmiSeatVentnForRowFirstRi = None  # type: SeatClimaLvl


#
# \struct HmiSeatClimaExtd# Seat ventilation request, second row.

#
class HmiSeatClimaExtd:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.HmiSeatVentnForRowSecLe = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.HmiSeatVentnForRowSecRi = None  # type: SeatClimaLvl


#
# \struct HudDiagc
#
class HudDiagc:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689377b8>
        self.HudTSts = None  # type: Err1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689377b8>
        self.HudCircShoSts = None  # type: Err1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689377b8>
        self.HudCricOpenSts = None  # type: Err1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e29e8>
        self.ImgHudTmpNotAvlSts = None  # type: NotAvlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689377b8>
        self.ImgHudErrSts = None  # type: Err1


#
# \enum HudStsForHmi
#
class HudStsForHmi(Enum):
    HudStsAvl=0
    HudStsCalMod=1
    HudStsTmpNotAvl=2
    HudStsErr=3
    Resd1=4
    Resd2=5
    Resd3=6

#
# \struct HudVisFctSetgReq
#
class HudVisFctSetgReq:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct00 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct01 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct02 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct03 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct04 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct05 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct06 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct07 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct08 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct09 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct10 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct11 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct12 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct13 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct14 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct15 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct16 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct17 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct18 = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.HudFct19 = None  # type: OnOff1


#
# \enum HvBattUsgBlkdTyp# HV battery usage function blocked types

#
class HvBattUsgBlkdTyp(Enum):
    NotBlkd=0
    BlkdByBattLvl=1
    BlkdBySysFlt=2
    NotOfUse=3

#
# \enum HvBattUsgType# Propulsion HV Battery usage mode type

#
class HvBattUsgType(Enum):
    NoReq=0
    Norm=1
    Hld=2
    HldSmt=3
    Chrg=4
    NotOfUse1=5
    NotOfUse2=6
    NotOfUse3=7

#
# \struct HwAprvdWirelsAdr1# Status for Bluetooth pairing

#
class HwAprvdWirelsAdr1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c2da0>
        self.HwOffsAdr1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5470>
        self.HwOffsAdr2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c54e0>
        self.HwOffsAdr3 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5550>
        self.HwOffsAdr4 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c55c0>
        self.HwOffsAdr5 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5630>
        self.HwOffsAdr6 = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68912ef0>
        self.AprvdSts = None  # type: AprvdSts


#
# \enum HznBrdg1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			SegNotPartOfBrdg	Segment Not Part Of Bridge	Segment is not part of a bridge.
# 1			1			SegPartOfBrdg	Segment Part Of Bridge	Segment is part of a bridge.
# 2			2			Ukwn		Unknown			Unknown

#
class HznBrdg1(Enum):
    SegNotPartOfBrdg=0
    SegPartOfBrdg=1
    Ukwn=2

#
# \struct HznDataGroup2
#
class HznDataGroup2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8978>
        self.MsgTyp = None  # type: HznMsgTyp1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5400>
        self.CycCntr = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c59b0>
        self.CtryCod = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5a90>
        self.RegnCod = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68930710>
        self.DrvgSide = None  # type: DrvgSide1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68884470>
        self.SpdUnit = None  # type: SpdUnit1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5b00>
        self.HdTxVers = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5b70>
        self.TxVers = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5c50>
        self.ChdTxVers = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5cc0>
        self.HwVers = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c85c0>
        self.MapSrc = None  # type: HznMapSrc2
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5d30>
        self.YrVersOfMap = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c59e8>
        self.PartOfYrVersOfMap = None  # type: int


#
# \struct HznEdgeGroup2
#
class HznEdgeGroup2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8978>
        self.MsgTyp = None  # type: HznMsgTyp1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c5f98>
        self.CycCntr = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.TxPrev = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c8080>
        self.PahIdx = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c8048>
        self.Offs = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.Upd = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c8160>
        self.PahIdxNew = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688c80b8>
        self.TurnAg = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688c8278>
        self.RelProblty = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3588>
        self.TypOfWay = None  # type: HznTypOfWay1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8cf8>
        self.NrOfLaneInDrvgDir = None  # type: HznNrOfLaneInDrvgDir1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cb080>
        self.NrOfLaneInSecDir = None  # type: HznNrOfLaneInSecDir1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c5710>
        self.IntscnCmplx = None  # type: HznIntscnCmplx1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688ce940>
        self.RiOfWay = None  # type: HznRiOfWay1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688c82e8>
        self.RoadClass = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cb208>
        self.PartOfCourseCalcd = None  # type: HznPartOfCourseCalcd1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.LstEdgeAtOffs = None  # type: NoYes1


#
# \enum HznIntscnCmplx1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			SegNotPartOfIntscnCmplx	Segment Not Part Of Intersection Complex	Segment is not part of complex intersection
# 1			1			SegPartOfIntscnCmplx	Segment Part Of Intersection Complex		Segment is part of a complex intersection
# 2			2			Ukwn			Unknown					Unknown

#
class HznIntscnCmplx1(Enum):
    SegNotPartOfIntscnCmplx=0
    SegPartOfIntscnCmplx=1
    Ukwn=2

#
# \enum HznLanePrsnt2# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Ukwn		Unknown		Unknown
# 1			1			EmgyLane	Emergency Lane	Emergency lane
# 2			2			RoadLaneSng	Road Lane Single	Single-lane road
# 3			3			LaneLe		Lane Left		Left-most lane
# 4			4			LaneRi		Lane Right	Right-most lane
# 5			5			LaneMid		Lane Middle	One of middle lanes on road with three or more lanes
# 6			6			Resd		Reserved		Reserved
# 7			7			Ukwn2		Unknown 2	Unknown 2

#
class HznLanePrsnt2(Enum):
    Ukwn=0
    EmgyLane=1
    RoadLaneSng=2
    LaneLe=3
    LaneRi=4
    LaneMid=5
    Resd=6
    Ukwn2=7

#
# \enum HznMapSrc2# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Ukwn		Unknown		Unknown
# 1			1			Src1		Source 1		NAVTEQ
# 2			2			Src2		Source 2		TeleAtlas
# 3			3			Src3		Source 3		Zenrin
# 4			4			Resd1		Reserved 1	reserved
# 5			5			Resd2		Reserved 2	reserved
# 6			6			Resd3		Reserved 3	reserved
# 7			7			Ukwn2		Unknown 2	Unknown 2

#
class HznMapSrc2(Enum):
    Ukwn=0
    Src1=1
    Src2=2
    Src3=3
    Resd1=4
    Resd2=5
    Resd3=6
    Ukwn2=7

#
# \enum HznMsgTyp1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved

#
class HznMsgTyp1(Enum):
    Resd1=0
    Posn=1
    Seg=2
    Edge=3
    ProfSho=4
    ProfLong=5
    Data=6
    Resd2=7

#
# \enum HznNrOfLaneInDrvgDir1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			WrgDirInRoadSngDir	Wrong Direction In Road Single Direction	One-direction road driven in wrong direction
# 1			1			Lane1			Lane 1					1 lane
# 2			2			Lane2			Lane 2					2 lanes
# 3			3			Lane3			Lane 3					3 lanes
# 4			4			Lane4			Lane 4					4 lanes
# 5			5			Lane5			Lane 5					5 lanes
# 6			6			LaneEquOrExcd6		Lane Equal Or Exceed 6			6 or more lanes

#
class HznNrOfLaneInDrvgDir1(Enum):
    WrgDirInRoadSngDir=0
    Lane1=1
    Lane2=2
    Lane3=3
    Lane4=4
    Lane5=5
    LaneEquOrExcd6=6
    Ukwn=7

#
# \enum HznNrOfLaneInSecDir1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			RoadSngDir	Road Single Direction	One-direction road
# 1			1			LaneSng		Lane Single		One lane
# 2			2			LaneEquOrExcd2	Lane Equal Or Exceed 2	2 or more lanes

#
class HznNrOfLaneInSecDir1(Enum):
    RoadSngDir=0
    LaneSng=1
    LaneEquOrExcd2=2
    Ukwn=3

#
# \enum HznPartOfCourseCalcd1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			SegNotPartOfPahCalcd	Segment Not Part Of Path Calculated	Segment is not part of Calculated Route
# 1			1			SegPartOfPahCalcd	Segment 	Part Of Path Calculated		Segment is part of Calculated Route
# 2			2			Ukwn			Unknown				Unknown

#
class HznPartOfCourseCalcd1(Enum):
    SegNotPartOfPahCalcd=0
    SegPartOfPahCalcd=1
    Ukwn=2

#
# \struct HznPosnExtdGroup1
#
class HznPosnExtdGroup1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8978>
        self.MsgTyp = None  # type: HznMsgTyp1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cb5c0>
        self.PahIdx = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cb550>
        self.PosnIdx = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cb630>
        self.PosnTiDif = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cb6a0>
        self.Spd = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cb710>
        self.RelDir = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cb780>
        self.PosnProblty = None  # type: float
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cb7f0>
        self.PosnQly = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c5828>
        self.LanePrsnt = None  # type: HznLanePrsnt2


#
# \struct HznPosnExtdOffs
#
class HznPosnExtdOffs:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cb518>
        self.Offs = None  # type: float
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cb438>
        self.CycCntr = None  # type: int


#
# \struct HznPosnGroup3
#
class HznPosnGroup3:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8978>
        self.MsgTyp = None  # type: HznMsgTyp1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cb3c8>
        self.CycCntr = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cba90>
        self.PahIdx = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cbb70>
        self.Offs = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cbbe0>
        self.PosnIdx = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cbc50>
        self.PosnTiDif = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cbcc0>
        self.Spd = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cbd30>
        self.RelDir = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cbda0>
        self.PosnProblty = None  # type: float
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cbe10>
        self.PosnQly = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c5828>
        self.LanePrsnt = None  # type: HznLanePrsnt2


#
# \struct HznProfLongExtdGroup1
#
class HznProfLongExtdGroup1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8978>
        self.MsgTyp = None  # type: HznMsgTyp1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cba58>
        self.CycCntr = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.TxPrev = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cbef0>
        self.PahIdx = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688cbf98>
        self.Offs = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.Upd = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cc898>
        self.ProfTyp = None  # type: HznProfLongTypExtd1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.NodCtrl = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cc080>
        self.Val = None  # type: int


#
# \struct HznProfLongGroup3
#
class HznProfLongGroup3:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8978>
        self.MsgTyp = None  # type: HznMsgTyp1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cc278>
        self.CycCntr = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.TxPrev = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cc208>
        self.PahIdx = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cc240>
        self.Offs = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.Upd = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cb860>
        self.ProfTyp = None  # type: HznProfLongTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.NodCtrl = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688cc390>
        self.Val = None  # type: int


#
# \enum HznProfLongTyp# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NotInUse		Not In Use		N/A
# 1			1			Lgt		Longitudinal		Longitude
# 2			2			Lat		Lateral			Latitude
# 3			3			Alti		Altitude			Altitude
# 4			4			NodCtrlForLgt	Node Control For Longitudinal	(Bézier) Control Point Longitude
# 5			5			NodCtrlForLat	Node Control For Lateral	(Bézier) Control Point Latitude
# 6			6			NodCtrlForAlti	Node Control For Altitude	(Bézier) Control Point Altitude
# 7			7			Id		Identifier			Link Identifier
# 8			15								Reserved for standard types
# 16			16			SpdLim1		Speed limits 1
# 17			17			SpdLim2		Speed limits 2
# 18			18			SpdLim3		Speed limits 3
# 19			19			CamSpd		Speed camera
# 20			20			RoadWInfo	Road Work Information
# 21			21			TrfcLi		Traffic lights
# 22			30								Reserved for system specific types
# 31			31			Ukwn		Unknown

#
class HznProfLongTyp(Enum):
    NotInUse=0
    Lgt=1
    Lat=2
    Alti=3
    NodCtrlForLgt=4
    NodCtrlForLat=5
    NodCtrlForAlti=6
    Id=7
    SpdLim1=16
    SpdLim2=17
    SpdLim3=18
    CamSpd=19
    RoadWInfo=20
    TrfcLi=21
    Ukwn=31

#
# \enum HznProfLongTypExtd1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NotInUse		Not In Use		N/A
# 1			1			Lgt		Longitudinal		Longitude
# 2			2			Lat		Lateral			Latitude
# 3			3			Alti		Altitude			Altitude
# 4			4			NodCtrlForLgt	Node Control For Longitudinal	(Bézier) Control Point Longitude
# 5			5			NodCtrlForLat	Node Control For Lateral	(Bézier) Control Point Latitude
# 6			6			NodCtrlForAlti	Node Control For Altitude	(Bézier) Control Point Altitude
# 7			7			Id		Identifier			Link Identifier
#
# VCC specific profile types:
# 16			16
# 17			17
# 18			18
# 20			20
# 21			21
# 22			22
# 23                                            23
# 24                                            24
# 25                                            25
#
# 31			31			Ukwn		Unknown

#
class HznProfLongTypExtd1(Enum):
    NotInUse=0
    Lgt=1
    Lat=2
    Alti=3
    NodCtrlForLgt=4
    NodCtrlForLat=5
    NodCtrlForAlti=6
    Id=7
    SpdRng=16
    AltiAvg=17
    DestOrInterDest=18
    SpdFromTrfcInfo=20
    LvlOfSrvFromTrfcInfo=21
    SpdFromMap=22
    EvChrgnLocn=23
    VehStopLocn=24
    Slop=25
    Ukw=31

#
# \struct HznProfSho2
#
class HznProfSho2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8978>
        self.MsgTyp = None  # type: HznMsgTyp1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688ce208>
        self.CycCntr = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.TxPrev = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688ce198>
        self.PahIdx = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688ce1d0>
        self.Offs = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.Upd = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cceb8>
        self.ProfTyp = None  # type: HznProfShoTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.NodCtrl = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688ce320>
        self.Val0 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688ce278>
        self.Dst1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688ce4a8>
        self.Val1 = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688ce780>
        self.ProfTypQly = None  # type: HznProfTypQly1


#
# \enum HznProfShoTyp# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NotInUse			Not In Use			N/A
# 1			1			Crvt			Curvature			Curvature
# 2			2			PahNrTyp			Path Number Type			Route Number Types
# 3			3			Slop1			Slop 1				Slope
# 4			4			Slop2			Slop 2				Slope
# 5			5			RoadAcs			Road Access			Road accessibility
# 6			6			RoadCdn			Road Condition			Road condition
# 7			7			PosnOfSpdSgnNotStat	Position Of Speed Sign Not Static	Variable Speed Sign Position
# 8			8			ChgOfDir			Change Of Direction		Heading Change
# 9			15										Reserved for standard types
# 16			17										Reserved for system specific types
# 18			18			TrfcSgn			Traffic Signs
# 19			19										Reserved for system specific types
# 20			20			CrashAr			Black Spot
# 21			30										Reserved for system specific types
# 31			31			Ukwn			Unknown

#
class HznProfShoTyp(Enum):
    NotInUse=0
    Crvt=1
    PahNrTyp=2
    Slop1=3
    Slop2=4
    RoadAcs=5
    RoadCdn=6
    PosnOfSpdSgnNotStat=7
    ChgOfDir=8
    TrfcSgn=18
    CrashAr=20
    Ukwn=31

#
# \enum HznProfTypQly1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			HiQly		High Quality	means highest accuracy.
# 1			1			NotInUse		Not In Use	Not used
# 2			2			LoQly		Low Quality	mean lowest accuracy.
# 3			3			UkwnQly		Unknown Quality	indicate that accuracy is unknown.

#
class HznProfTypQly1(Enum):
    HiQly=0
    NotInUse=1
    LoQly=2
    UkwnQly=3

#
# \enum HznRiOfWay1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			RiOfWayOfHdPah	Right Of Way Of Head Path	Parent path has right-of-way over sub-path
# 1			1			RiOfWayOfChdPah	Right Of Way Of Child Path	Sub-path has right-of-way over parent path
# 2			2			Ukwn		Unknown			Unknown

#
class HznRiOfWay1(Enum):
    RiOfWayOfHdPah=0
    RiOfWayOfChdPah=1
    Ukwn=2

#
# \enum HznRoadLiAr1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			SegNotPartOfTownOrCty	Segment Not Part Of Town Or City	Segment is not part of a built-up area.
# 1			1			SegPartOfTownOrCty	Segment Part Of Town Or City	Segment is part of a built-up area.
# 2			2			Ukwn			Unknown				Unknown

#
class HznRoadLiAr1(Enum):
    SegNotPartOfTownOrCty=0
    SegPartOfTownOrCty=1
    Ukwn=2

#
# \enum HznRoadMpl1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			SegNotPartOfRoadMpl	Segment Not Part Of Road Multiple	Segment is not part of a divided road/dual carriageway.
# 1			1			SegPartOfRoadMpl		Segment Part Of Road Multiple	Segment is part of a divided road/dual carriageway.
# 2			2			Ukwn			Unknown				Unknown

#
class HznRoadMpl1(Enum):
    SegNotPartOfRoadMpl=0
    SegPartOfRoadMpl=1
    Ukwn=2

#
# \struct HznSegGroup2
#
class HznSegGroup2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8978>
        self.MsgTyp = None  # type: HznMsgTyp1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688d0198>
        self.CycCntr = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.TxPrev = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688d0128>
        self.PahIdx = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688d0160>
        self.Offs = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.Upd = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688d02b0>
        self.RoadClass = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3588>
        self.TypOfWay = None  # type: HznTypOfWay1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cecc0>
        self.SpdLimEfc = None  # type: HznSpdLimEfc1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d0fd0>
        self.SpdLimTypEfc = None  # type: HznSpdLimTypEfc1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c8cf8>
        self.NrOfLaneInDrvgDir = None  # type: HznNrOfLaneInDrvgDir1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cb080>
        self.NrOfLaneInSecDir = None  # type: HznNrOfLaneInSecDir1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3470>
        self.Tnl = None  # type: HznTnl1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c5048>
        self.Brdg = None  # type: HznBrdg1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688ceb70>
        self.RoadMpl = None  # type: HznRoadMpl1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cea58>
        self.RoadLiAr = None  # type: HznRoadLiAr1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c5710>
        self.IntscnCmplx = None  # type: HznIntscnCmplx1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688d0208>
        self.RelProblty = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688cb208>
        self.PartOfCourseCalcd = None  # type: HznPartOfCourseCalcd1


#
# \enum HznSpdLimEfc1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Ukwn		Unknown		Unknown
# 1			1			Val5OrLowr	Value 5 Or Lower	<= 5 km/h [mph]
# 2			2			Val7		Value 7		7 km/h [mph]
# 3			3			Val10		Value 10		10 km/h [mph]
# 4			4			Val15		Value 15		15 km/h [mph]
# 5			5			Val20		Value 20		20 km/h [mph]
# 6			6			Val25		Value 25		25 km/h [mph]
# 7			7			Val30		Value 30		30 km/h [mph]
# 8			8			Val35		Value 35		35 km/h [mph]
# 9			9			Val40		Value 40		40 km/h [mph]
# 10			10			Val45		Value 45		45 km/h [mph]
# 11			11			Val50		Value 50		50 km/h [mph]
# 12			12			Val55		Value 55		55 km/h [mph]
# 13			13			Val60		Value 60		60 km/h [mph]
# 14			14			Val65		Value 65		65 km/h [mph]
# 15			15			Val70		Value 70		70 km/h [mph]
# 16			16			Val75		Value 75		75 km/h [mph]
# 17			17			Val80		Value 80		80 km/h [mph]
# 18			18			Val85		Value 85		85 km/h [mph]
# 19			19			Val90		Value 90		90 km/h [mph]
# 20			20			Val95		Value 95		95 km/h [mph]
# 21			21			Val100		Value 100		100 km/h [mph]
# 22			22			Val105		Value 105		105 km/h [mph]
# 23			23			Val110		Value 110		110 km/h [mph]
# 24			24			Val115		Value 115		115 km/h [mph]
# 25			25			Val120		Value 120		120 km/h [mph]
# 26			26			Val130		Value 130		130 km/h [mph]
# 27			27			Val140		Value 140		140 km/h [mph]
# 28			28			Val150		Value 150		150 km/h [mph]
# 29			29			Val160		Value 160		160 km/h [mph]
# 30			30			NoLim		No Limit		Unlimited

#
class HznSpdLimEfc1(Enum):
    Ukwn=0
    Val5OrLowr=1
    Val7=2
    Val10=3
    Val15=4
    Val20=5
    Val25=6
    Val30=7
    Val35=8
    Val40=9
    Val45=10
    Val50=11
    Val55=12
    Val60=13
    Val65=14
    Val70=15
    Val75=16
    Val80=17
    Val85=18
    Val90=19
    Val95=20
    Val100=21
    Val105=22
    Val110=23
    Val115=24
    Val120=25
    Val130=26
    Val140=27
    Val150=28
    Val160=29
    NoLim=30

#
# \enum HznSpdLimTypEfc1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"			Description
# 0			0			SecInfo		Second Information			Implicit (for instance, default speed limit in the cities)
# 1			1			PrimInfoOnTrfcSgn	Primary Information On Traffic Sign	Explicit - on traffic sign
# 2			2			PrimInfoByNight	Primary Information By Night		Explicit - by night
# 3			3			PrimInfoByDay	Primary Information By Day		Explicit - by day
# 4			4			PrimInfoByTiOfDay	Primary Information By Time Of Day	Explicit - time of day
# 5			5			PrimInfoAtRain	Primary Information At Rain		Explicit - rain
# 6			6			PrimInfoAtSnow	Primary Information At Snow		Explicit - snow
# 7			7			Ukwn		Unknown				Unknown

#
class HznSpdLimTypEfc1(Enum):
    SecInfo=0
    PrimInfoOnTrfcSgn=1
    PrimInfoByNight=2
    PrimInfoByDay=3
    PrimInfoByTiOfDay=4
    PrimInfoAtRain=5
    PrimInfoAtSnow=6
    Ukwn=7

#
# \enum HznSplyElectcSts1# EH (Electronic Horizon) provider status and support.

#
class HznSplyElectcSts1(Enum):
    Ukwn=0
    NoSpprt=1
    Failr=2
    Spprt=3

#
# \enum HznTnl1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			SegNotPartOfTnl	Segment Not Part Of Tunnel	Segment is not part of a tunnel
# 1			1			SegPartOfTnl	Segment Part Of Tunnel	Segment is part of a tunnel
# 2			2			Ukwn		Unknown			Unknown

#
class HznTnl1(Enum):
    SegNotPartOfTnl=0
    SegPartOfTnl=1
    Ukwn=2

#
# \enum HznTypOfWay1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"		Description
# 0			0			Ukwn			Unknown			Unknown
# 1			1			FreeWay			Free Way			Freeway or Controlled Access road that is not a slip road/ramp
# 2			2			RoadMpl			Raod Multiple		Multiple Carriageway or Multiply Digitized Road
# 3			3			RoadSng			Road Single		Single Carriageway (default)
# 4			4			Roty			Rotary			Roundabout Circle
# 5			5			TrfcSq			Traffic Square		Traffic Square/Special Traffic Figure
# 6			6			Resd1			Reserved 1		Reserved
# 7			7			Resd2			Reserved 2		Reserved
# 8			8			RampInEquDir		Ramp In Equal Direction	Parallel Road (as special type of a slip road/ramp)
# 9			9			FreeWayRamp		Free Way Ramp		Slip Road/Ramp on a Freeway or Controlled Access road
# 10			10			RampNotOnFreeWay	Ramp Not On Free Way	Slip Road/Ramp (not on a Freeway or Controlled Access road)
# 11			11			RoadForSrvAndAcs		Road For Service And Access	Service Road or Frontage Road
# 12			12			EntryToCarPark		Entry To Car Park		Entrance to or exit of a Car Park
# 13			13			EntryToSrv		Entry To Service		Entrance to or exit to Service
# 14			14			PedAr			Pedestrian Area		Pedestrian Zone

#
class HznTypOfWay1(Enum):
    Ukwn=0
    FreeWay=1
    RoadMpl=2
    RoadSng=3
    Roty=4
    TrfcSq=5
    Resd1=6
    Resd2=7
    RampInEquDir=8
    FreeWayRamp=9
    RampNotOnFreeWay=10
    RoadForSrvAndAcs=11
    EntryToCarPark=12
    EntryToSrv=13
    PedAr=14

#
# \enum IdPen# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.

#
class IdPen(Enum):
    ProfUkwn=0
    Prof1=1
    Prof2=2
    Prof3=3
    Prof4=4
    Prof5=5
    Prof6=6
    Prof7=7
    Prof8=8
    Prof9=9
    Prof10=10
    Prof11=11
    Prof12=12
    Prof13=13
    Resd14=14
    ProfAll=15

#
# \enum IndOfPrkgAutSts
#
class IndOfPrkgAutSts(Enum):
    Off=0
    PrkgTypAutSeln=1
    PrkgTypPara=2
    PrkgTypPerp=3
    PrkgOutManv=4

#
# \struct IndcnUnit# Settings for units and formats that shall be used for indication in the vehicle

#
class IndcnUnit:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68912780>
        self.TUnit = None  # type: AmbTIndcdUnit
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68893f28>
        self.SpdUnit = None  # type: VehSpdIndcdUnit
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68932be0>
        self.DstLong = None  # type: DstLong
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f689354a8>
        self.DstSho = None  # type: DstSho
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bcba8>
        self.FuCnsUnit = None  # type: FuCnsIndcdUnit
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688992b0>
        self.VolUnit = None  # type: VolUnit
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.IdPenForUnit = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68927b38>
        self.ClkFmt = None  # type: ClkFmt
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e4a90>
        self.PUnit = None  # type: PUnit
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68929438>
        self.DateFmt = None  # type: DateFmt


#
# \enum IndcrSts1
#
class IndcrSts1(Enum):
    Off=0
    LeOn=1
    RiOn=2
    LeAndRiOn=3

#
# \enum InterruptionSrc1
#
class InterruptionSrc1(Enum):
    Off=0
    TiOut=1
    ResuPsd=2
    BrkPedl=3
    AccrPedlPsd=4
    Resd5=5
    Resd6=6
    Resd7=7

#
# \struct IntrLiAmbLiSetg
#
class IntrLiAmbLiSetg:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d5ba8>
        self.LiInten = None  # type: IntrLiInten
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6f6969b0>
        self.LiTintg = None  # type: AmbLiTintg
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum IntrLiInten
#
class IntrLiInten(Enum):
    Off=0
    IntenLo=1
    IntenHi=2

#
# \struct IntrLiSurrndgsLiSetg
#
class IntrLiSurrndgsLiSetg:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d5ba8>
        self.LiInten = None  # type: IntrLiInten
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68886ac8>
        self.LiLvl = None  # type: SurrndgsLiLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum KeyId1
#
class KeyId1(Enum):
    Key0=0
    Key1=1
    Key2=2
    Key3=3
    Key4=4
    Key5=5
    Key6=6
    Key7=7
    Key8=8
    Key9=9
    Key10=10
    Key11=11

#
# \enum KeyLocn1# Data-Type for Key Search Location
# KeyLocnAll  -  All zones
# KeyLocnAllExt  -  All external zones
# KeyLocnDrvrExt  -  External driver side
# KeyLocnPassExt - External passenger side
# KeyLocnTrExt - External behind trunk
# KeyLocnAllInt - All internal zones
# KeyLocnDrvrInt - Internal driver side
# KeyLocnPassInt - Internal passenger side
# KeyLocnResvInt - Internal backup reader (front central position)
# KeyLocnResvIntSimple - Quick/simple internal backup reader request

#
class KeyLocn1(Enum):
    KeyLocnIdle=0
    KeyLocnAll=1
    KeyLocnAllExt=2
    KeyLocnDrvrExt=3
    KeyLocnPassExt=4
    KeyLocnTrExt=5
    KeyLocnAllInt=6
    KeyLocnDrvrInt=7
    KeyLocnPassInt=8
    KeyLocnResvInt=9
    KeyLocnResvIntSimple=10

#
# \enum KeyLostWarnIndcn
#
class KeyLostWarnIndcn(Enum):
    KeyLostNoMsg=0
    KeyMsgAndSoundLost=1
    KeyMsgLost=2

#
# \struct KeyProfMpgUpd1
#
class KeyProfMpgUpd1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.KeyProfMpgUpdForIdPen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.KeyProfMpgUpdOff = None  # type: bool


#
# \struct KeyReadStsToProfCtrl
#
class KeyReadStsToProfCtrl:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d5e10>
        self.KeyId = None  # type: KeyId1
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Boolean = None  # type: bool


#
# \enum LaneDetnSts1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoLaneDetd	No Lane Detedcted		No lane tracked
# 1			1			LaneLeDetd	Lane Left Detected		Left lane marking tracked
# 2			2			LaneRiDetd	Lane Right Detected	Right lane marking tracked
# 3			3			LaneLeAndRiDetd	Lane Left And Right Detected	Left and right lane marking tracked

#
class LaneDetnSts1(Enum):
    NoLaneDetd=0
    LaneLeDetd=1
    LaneRiDetd=2
    LaneLeAndRiDetd=3

#
# \enum LangTyp# Uknw = Unknown
# Lang1 = Arabic
# Lang2 = Bulgarian
# Lang3 = Chinese Cantonese Traditional
# Lang4 = Chinese Mandarin Simplifed
# Lang5 = Chinese Mandarin Traditional
# Lang6 = Czech
# Lang7 = Danish
# Lang8 = Dutch
# Lang9 = English AU
# Lang10 = English UK
# Lang11 = English US
# Lang12 = Estonian
# Lang13 = Finnish
# Lang14 = Flemmish
# Lang15 = French CAN
# Lang16 = French EU
# Lang17 = German
# Lang18 = Greek
# Lang19 = Hungarian
# Lang20 = Italian
# Lang21 = Japanese
# Lang22 = Korean
# Lang23 = Latvian
# Lang24 = Lithuanian
# Lang25 = Norwegian
# Lang26 = Polish
# Lang27 = Portuguese BRA
# Lang28 = Portuguese EU
# Lang29 = Romanian
# Lang30 = Russian
# Lang31 = Slovak
# Lang32 = Slovene
# Lang33 = Spanish EU
# Lang34 = Spanish US
# Lang35 = Swedish
# Lang36 = Thai
# Lang37 = Turkish

#
class LangTyp(Enum):
    Ukwn=0
    Lang1=1
    Lang2=2
    Lang3=3
    Lang4=4
    Lang5=5
    Lang6=6
    Lang7=7
    Lang8=8
    Lang9=9
    Lang10=10
    Lang11=11
    Lang12=12
    Lang13=13
    Lang14=14
    Lang15=15
    Lang16=16
    Lang17=17
    Lang18=18
    Lang19=19
    Lang20=20
    Lang21=21
    Lang22=22
    Lang23=23
    Lang24=24
    Lang25=25
    Lang26=26
    Lang27=27
    Lang28=28
    Lang29=29
    Lang30=30
    Lang31=31
    Lang32=32
    Lang33=33
    Lang34=34
    Lang35=35
    Lang36=36
    Lang37=37

#
# \enum LcmaCtraOn
#
class LcmaCtraOn(Enum):
    StrtUpOn=0
    On=1
    Off=2
    TrlrOff=3

#
# \enum LiAutTran1# SntvyLo = Sensitivity Low, light transition happens later
# SntvyLo = Sensitivity Norma
# SntvyLo = Sensitivity High, light transition happens earlier

#
class LiAutTran1(Enum):
    SnvtyLo=0
    SnvtyNorm=1
    SnvtyHi=2
    Resd=3

#
# \struct LiAutTranPen1
#
class LiAutTranPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688dbd30>
        self.Sts = None  # type: LiAutTran1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum LiTi2# Enumeration for intervals in tenths of seconds.
# E.g
# 3 = 30 seconds
# 4 = 40 seconds

#
class LiTi2(Enum):
    Sec0=0
    Sec10=1
    Sec20=2
    Sec30=3
    Sec40=4
    Sec50=5
    Sec60=6
    Sec70=7
    Sec80=8
    Sec90=9
    Sec100=10
    Sec110=11
    Sec120=12
    Resd1=13
    Resd2=14
    Resd3=15

#
# \struct LiTiPen2
#
class LiTiPen2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688dbf28>
        self.Sts = None  # type: LiTi2
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum LiTrfcSide1# TrfcSideTmpLe = Traffic Side Temporary Left. Vehicle drives on left side of road
# TrfcSideTmpRi =  Traffic Side Temporary Right. Vehicle drives on right side of road

#
class LiTrfcSide1(Enum):
    Off=0
    TrfcSideTmpLe=1
    TrfcSideTmpRi=2
    Resd=3

#
# \struct ListOfNodAv
#
class ListOfNodAv:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688dc748>
        self.ListOfNodAv1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688dcb38>
        self.ListOfNodAv2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688dcba8>
        self.ListOfNodAv3 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688dcc18>
        self.ListOfNodAv4 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688dcc88>
        self.ListOfNodAv5 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688dccf8>
        self.ListOfNodAv6 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688dcd68>
        self.ListOfNodAv7 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688dcdd8>
        self.ListOfNodAv8 = None  # type: int


#
# \enum LockSt2
#
class LockSt2(Enum):
    Undefd=0
    Opend=1
    Clsd=2
    Lockd=3
    Safe=4

#
# \enum LockSt3
#
class LockSt3(Enum):
    LockUndefd=0
    LockUnlckd=1
    LockTrUnlckd=2
    LockLockd=3

#
# \enum LockSts2
#
class LockSts2(Enum):
    LockStsUkwn=0
    Unlckd=1
    Lockd=2
    SafeLockd=3

#
# \enum LockTrigSrc2
#
class LockTrigSrc2(Enum):
    NoTrigSrc=0
    KeyRem=1
    Keyls=2
    IntrSwt=3
    SpdAut=4
    TmrAut=5
    Slam=6
    Telm=7
    Crash=8

#
# \struct LockgCenSts3
#
class LockgCenSts3:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688dcf28>
        self.LockSt = None  # type: LockSt3
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688de208>
        self.TrigSrc = None  # type: LockTrigSrc2
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.UpdEve = None  # type: bool


#
# \enum Lvl2
#
class Lvl2(Enum):
    Std=0
    Lo=1

#
# \enum Lvl30
#
class Lvl30(Enum):
    NoLvl=0
    LoLvl=1
    MidLvl=2
    HiLvl=3

#
# \enum LvlCtrlSetgTyp
#
class LvlCtrlSetgTyp(Enum):
    Cmft=0
    Norm=1
    Dyn=2
    XC=3

#
# \enum LvlOfClimaCmft
#
class LvlOfClimaCmft(Enum):
    Off=0
    Lvl1=1
    Lvl2=2
    Lvl3=3
    Lvl4=4
    Lvl5=5
    Lvl6=6
    Lvl7=7

#
# \struct LvlSeldForSpdLimAdpvPen1
#
class LvlSeldForSpdLimAdpvPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68918438>
        self.Sts = None  # type: Aut1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum LvlWarn1
#
class LvlWarn1(Enum):
    NoWarn=0
    Lvl1=1
    Lvl2=2

#
# \enum ManAdpv1
#
class ManAdpv1(Enum):
    Man=0
    Adpv=1

#
# \struct MassgFctActv
#
class MassgFctActv:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.DrvrMassgFctActv = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.PassMassgFctActv = None  # type: OnOff1


#
# \enum MassgIntenLvl
#
class MassgIntenLvl(Enum):
    IntenLo=0
    IntenNorm=1
    IntenHi=2

#
# \enum MassgProgTyp
#
class MassgProgTyp(Enum):
    Prog1=0
    Prog2=1
    Prog3=2
    Prog4=3
    Prog5=4

#
# \enum MirrDimLvlTyp
#
class MirrDimLvlTyp(Enum):
    Normal=0
    Dark=1
    Light=2
    Inhibit=3

#
# \struct MirrDimPen
#
class MirrDimPen:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e0780>
        self.MirrDimLvl = None  # type: MirrDimLvlTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum MirrDwnStsTyp
#
class MirrDwnStsTyp(Enum):
    MirrTiltUndefd=0
    MirrUpPosn=1
    MirrTiltPosn=2
    MirrMovgToUpPosn=3
    MirrMovgToTiltPosn=4

#
# \enum MirrFoldStsTyp
#
class MirrFoldStsTyp(Enum):
    MirrFoldPosnUndefd=0
    MirrNotFoldPosn=1
    MirrFoldPosn=2
    MirrMovgToNotFold=3
    MirrMovgToFold=4

#
# \enum MmedHmiModStd2
#
class MmedHmiModStd2(Enum):
    InfModeOff=0
    InfModeOn=1
    InfModePartial=2
    InfModeWelcome=3

#
# \enum MmedMaiPwrMod
#
class MmedMaiPwrMod(Enum):
    IHUStateSleep=0
    IHUStateStandby=1
    IHUStatePartial=2
    IHUStateOn=3

#
# \enum MmedTvmPwerMod
#
class MmedTvmPwerMod(Enum):
    Sleep=0
    Switch=1
    Standby=2
    TPEG=3
    On=4

#
# \struct MtrlSnsrT
#
class MtrlSnsrT:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e09e8>
        self.MtrlSnsrT = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e21d0>
        self.MtrlSnsrTFacQly = None  # type: MtrlSnsrTFacQly


#
# \enum MtrlSnsrTFacQly
#
class MtrlSnsrTFacQly(Enum):
    SnsrDataNotOK=0
    SnsrDataOK=1

#
# \struct NetActvtRec1
#
class NetActvtRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e2208>
        self.ResourceGroup = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e6ef0>
        self.Prio = None  # type: PrioHighNormal


#
# \enum NoReqOffReq
#
class NoReqOffReq(Enum):
    NoReq=0
    OffReq=1

#
# \enum NoYes1
#
class NoYes1(Enum):
    No=0
    Yes=1

#
# \enum NoYesCrit1# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid

#
class NoYesCrit1(Enum):
    NotVld1=0
    No=1
    Yes=2
    NotVld2=3

#
# \struct NormSptPen
#
class NormSptPen:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2860>
        self.NormSpt = None  # type: NormSptType
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum NormSptType
#
class NormSptType(Enum):
    Norm=0
    Spt=1

#
# \enum NotAvlSts1
#
class NotAvlSts1(Enum):
    Avl=0
    NotAvl=1

#
# \struct NrSerlNodLIN
#
class NrSerlNodLIN:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e2898>
        self.Nr1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e2c50>
        self.Nr2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e2cc0>
        self.Nr3 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e2d30>
        self.Nr4 = None  # type: int


#
# \enum OffOnAut1
#
class OffOnAut1(Enum):
    Off=0
    On=1
    Aut=2

#
# \enum OffOnNotAvlSrvRqrd
#
class OffOnNotAvlSrvRqrd(Enum):
    Off=0
    On=1
    NotAvl=2
    SrvRqrd=3

#
# \struct OffsForSpdWarnSetgPen
#
class OffsForSpdWarnSetgPen:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e2be0>
        self.Sts = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum OnOff1
#
class OnOff1(Enum):
    Off=0
    On=1

#
# \enum OnOff2# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.

#
class OnOff2(Enum):
    On=0
    Off=1

#
# \enum OnOffCrit1# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid

#
class OnOffCrit1(Enum):
    NotVld1=0
    Off=1
    On=2
    NotVld2=3

#
# \enum OnOffIdle1
#
class OnOffIdle1(Enum):
    Off=0
    OffOrSpare1=1
    On=2
    OffOrSpare2=3

#
# \enum OnOffNoReq
#
class OnOffNoReq(Enum):
    NoReq=0
    On=1
    Off=2

#
# \struct OnOffPen
#
class OnOffPen:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.Sts = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum OnOffSafe1
#
class OnOffSafe1(Enum):
    OnOffSafeInvld1=0
    OnOffSafeOn=1
    OnOffSafeOff=2
    OnOffSafeInvld2=3

#
# \enum PUnit
#
class PUnit(Enum):
    Pa=0
    Psi=1
    Bar=2
    Atm=3
    UkwnUnit=4

#
# \enum ParkAssiLeRi1
#
class ParkAssiLeRi1(Enum):
    None_=0
    Left=1
    Right=2
    LeftAndRight=3

#
# \struct PartNrNodLIN
#
class PartNrNodLIN:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e4940>
        self.Nr1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e60b8>
        self.Nr2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e6128>
        self.Nr3 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e6198>
        self.Nr4 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e6208>
        self.EndSgn1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e6278>
        self.EndSgn2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e62e8>
        self.EndSgn3 = None  # type: int


#
# \enum PassSeatSts1
#
class PassSeatSts1(Enum):
    Empty=0
    Fmale=1
    OccptLrg=2
    Ukwn=3

#
# \struct PassSeatSwtSts2# Describes PSMP switch pack information.

#
class PassSeatSwtSts2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68886d68>
        self.PassSeatSwtSldSts = None  # type: SwtHozlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688880f0>
        self.PassSeatSwtHeiSts = None  # type: SwtVertSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688880f0>
        self.PassSeatSwtHeiFrntSts = None  # type: SwtVertSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68886d68>
        self.PassSeatSwtInclSts = None  # type: SwtHozlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68886d68>
        self.PassSeatSwtAdjmtOfSpplFctHozlSts = None  # type: SwtHozlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688880f0>
        self.PassSeatSwtAdjmtOfSpplFctVerSts = None  # type: SwtVertSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688880f0>
        self.PassSeatSwtSelnOfSpplFctStsSts = None  # type: SwtVertSts1


#
# \struct PinionSteerAg1Rec
#
class PinionSteerAg1Rec:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e6550>
        self.PinionSteerAg1 = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.PinionSteerAg1Qf = None  # type: GenQf1


#
# \struct PosnFromNav
#
class PosnFromNav:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e6470>
        self.PosnLat = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e6828>
        self.PosnLgt = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e6898>
        self.PosnAlti = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e6908>
        self.PosnSpd = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e6978>
        self.PosnDir = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e64e0>
        self.PosnQly = None  # type: PosnFromNavQly


#
# \enum PosnFromNavQly
#
class PosnFromNavQly(Enum):
    QlyOfLvl0=0
    QlyOfLvl1=1
    QlyOfLvl2=2
    QlyOfLvl3=3
    QlyOfLvl4=4
    QlyOfLvl5=5
    QlyOfLvl6=6
    QlyOfLvl7=7

#
# \struct PosnFromSatlt
#
class PosnFromSatlt:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e67b8>
        self.PosnLat = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e92b0>
        self.PosnLgt = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e9320>
        self.PosnAlti = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e9390>
        self.PosnSpd = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e9400>
        self.PosnVHozl = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e9470>
        self.PosnVVert = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e94e0>
        self.PosnDir = None  # type: float
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9550>
        self.TiForYr = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e95c0>
        self.TiForMth = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9630>
        self.TiForDay = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e96a0>
        self.TiForHr = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9710>
        self.TiForMins = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9780>
        self.TiForSec = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SatltSysNo1InUse = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SatltSysNo2InUse = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SatltSysNo3InUse = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SatltSysNo4InUse = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SatltSysNo5InUse = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SatltSysNo6InUse = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6f281ac8>
        self.SatltPosnStsPrm1 = None  # type: SatltPosnSts
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.SatltPosnStsPrm2 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.SatltPosnStsPrm3 = None  # type: bool
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e97f0>
        self.NoOfSatltForSysNo1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9278>
        self.NoOfSatltForSysNo2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9ac8>
        self.NoOfSatltForSysNo3 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9b38>
        self.NoOfSatltForSysNo4 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9ba8>
        self.NoOfSatltForSysNo5 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688e9c18>
        self.NoOfSatltForSysNo6 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e9c88>
        self.PrePosnDil = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e9cf8>
        self.PreHozlDil = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e9d68>
        self.PreVertDil = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688e9dd8>
        self.PreTiDil = None  # type: float


#
# \enum PreClngNotif
#
class PreClngNotif(Enum):
    NoReq=0
    NoWarn=1
    Done=2
    Err=3
    Intrpt=4
    Spare1=5
    Spare2=6
    Spare3=7

#
# \enum PrioHighNormal
#
class PrioHighNormal(Enum):
    PrioNormal=0
    PrioHigh=1

#
# \struct PrkgAssiAudWarn# New record type to hold audio warning data types for PAS.

#
class PrkgAssiAudWarn:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68899710>
        self.AudWarnDir = None  # type: WarnDir1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e4cc0>
        self.AudSideWarn = None  # type: ParkAssiLeRi1


#
# \enum PrkgAssiFailr2
#
class PrkgAssiFailr2(Enum):
    NoFailr=0
    SgnlFailr=1
    SnsrFailr=2
    DgrSnsr=3

#
# \enum PrkgAssiSts3
#
class PrkgAssiSts3(Enum):
    SnsrAllOff=0
    FourReSnsrActv=1
    FourFrntFourReSnsrAct=2
    SixFrntSnsrTwoReActv=3
    Resd7=4
    SnsrAllStb=5
    Resd3=6
    Resd4=7
    Resd5=8
    SnsrFailrAllOff=9
    SnsrAllDeactvd=10
    SnsrTrlrOff4=11
    SnsrTrlrOff12=12
    SixFrntSnsrAndSixReActv=13
    Resd1=14
    Resd2=15

#
# \enum PrkgAutSts3
#
class PrkgAutSts3(Enum):
    SAPNotActive=0
    SAPPassiveScan=1
    OverspeedSlowDown=2
    ScanLeft=3
    ScanRight=4
    SAPPSlotFoundLeftGoForward=5
    SAPPSlotFoundRightGoForward=6
    PPASlotFoundLeftGoForward=7
    PPASlotFoundRightGoForward=8
    SAPStopToParkLeft=9
    SAPStopToParkRight=10
    PPAStopToParkLeft=11
    PPAStopToParkRight=12
    SAPPStopAndReverseGearLeft=13
    SAPPStopAndReverseGearRight=14
    PPAStopAndReverseGearLeft=15
    PPAStopAndReverseGearRight=16
    SAPPLookAroundLeft=17
    SAPPLookAroundRight=18
    PPALookAroundLeft=19
    PPALookAroundRight=20
    SAPPFirstMoveLeft=21
    SAPPFirstMoveRight=22
    PPAFirstMoveLeft=23
    PPAFirstMoveRight=24
    SAPPChangeToForwardLeft=25
    SAPPChangeToForwardRight=26
    PPAChangeToForwardLeft=27
    PPAChangeToForwardRight=28
    SAPPMoveForwardLeft=29
    SAPPMoveForwardRight=30
    PPAMoveForwardLeft=31
    PPAMoveForwardRight=32
    SAPPChangeToReverseLeft=33
    SAPPChangeToReverseRight=34
    PPAChangeToReverseLeft=35
    PPAChangeToReverseRight=36
    SAPPMoveRearwardLeft=37
    SAPPMoveRearwardRight=38
    PPAMoveRearwardLeft=39
    PPAMoveRearwardRight=40
    Finished=41
    ParkingFail=42
    Off=43
    TemporaryNotAvailable=44
    CancelHighSpeed=45
    CancelSlipperyRoad=46
    CancelTrailerConnect=47
    CancelEngineStall=48
    CancelSteringIntervention=49
    CancelDriverDeactivation=50
    CancelManyAttempts=51
    CancelWrongGear=52
    Resume=53
    SelectParkOutSide=54
    POAScannig=55
    POAPathNotFound=56
    POAChanageToForwardGearLeft=57
    POAChanageToForwardGearRight=58
    POAChanageToReverseGearLeft=59
    POAChanageToReverseGearRight=60
    POAStopAndChangeToForwardGearLeft=61
    POAStopAndChangeToForwardGearRight=62
    POAStopAndChangeToReverseGearLeft=63
    POAStopAndChangeToReverseGearRight=64
    POARearwardMoveLeft=65
    POARearwardMoveRight=66
    POAForwardMoveLeft=67
    POAForwardMoveRight=68
    POAFinishedGoLeft=69
    POAFinishedGoRight=70
    POALookAroundLeft=71
    POALookAroundRight=72
    POAFailr=73
    EmgyBrkDsbld=74
    EmgyBrkActv=75
    EmgyBrkUsrActn=76

#
# \enum PrkgTypVld1
#
class PrkgTypVld1(Enum):
    NoPrkgDetd=0
    ParaPrkgDetd=1
    PerpPrkgDetd=2
    ParaAndPerpPrkgDetd=3

#
# \struct ProfAct1
#
class ProfAct1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof1 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof2 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof3 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof4 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof5 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof6 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof7 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof8 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof9 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof10 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof11 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof12 = None  # type: bool


#
# \struct ProfLimd1
#
class ProfLimd1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof1 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof2 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof3 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof4 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof5 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof6 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof7 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof8 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof9 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof10 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof11 = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.Prof12 = None  # type: bool


#
# \enum PrpsnDrvMod
#
class PrpsnDrvMod(Enum):
    Undefd=0
    DrvMod1=1
    DrvMod2=2
    DrvMod3=3
    DrvMod4=4
    DrvMod5=5
    DrvMod6=6
    DrvMod7=7
    DrvMod8=8
    DrvMod9=9
    DrvMod10=10
    DrvMod11=11
    DrvMod12=12
    DrvMod13=13
    DrvMod14=14
    Err=15

#
# \enum PsdNotPsd
#
class PsdNotPsd(Enum):
    NotPsd=0
    Psd=1

#
# \struct PtCluTq1
#
class PtCluTq1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688f0908>
        self.PtCluTq = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688f3470>
        self.PtCluTqDyn = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f58d0>
        self.PtCluTqQly = None  # type: Qly3


#
# \enum PtDrvrModSetg2
#
class PtDrvrModSetg2(Enum):
    Undefd=0
    Cmft=1
    Eco=2
    Dyn=3
    Pure=4
    Hyb=5
    Pwr=6
    AWD=7

#
# \struct PtDrvrSetg2
#
class PtDrvrSetg2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f3390>
        self.Sts = None  # type: PtDrvrModSetg2


#
# \enum PtGearAct1
#
class PtGearAct1(Enum):
    Neut=0
    Gear1=1
    Gear2=2
    Gear3=3
    Gear4=4
    Gear5=5
    Gear6=6
    Gear7=7
    Gear8=8
    Gear9=9
    Gear10=10
    Gear11=11
    Gear12=12
    Gear13=13
    Ukwn=14
    Rvs=15

#
# \struct PtTqAtWhlFrntActRec1
#
class PtTqAtWhlFrntActRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688f38d0>
        self.PtTqAtWhlFrntLeAct = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688f51d0>
        self.PtTqAtWhlFrntRiAct = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688f5240>
        self.PtTqAtAxleFrntAct = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f58d0>
        self.PtTqAtWhlsFrntQly = None  # type: Qly3


#
# \enum PwrSplyErrSts1# Status of power supply components

#
class PwrSplyErrSts1(Enum):
    SysOk=0
    AltFltMecl=1
    AltFltElec=2
    AltFltT=3
    AltFltCom=4
    UHiDurgDrvg=5
    BattSwtFltCom=6
    BattSwtHwFlt=7
    ULoDurgDrvg=8
    BattSnsrFltCom=9
    BattSnsrHwFlt=10
    SpprtBattFltChrgn=11
    FltComDcDc=12
    FltElecDcDc=13
    FltTDcDc=14

#
# \enum Qf1
#
class Qf1(Enum):
    DevOfDataUndefd=0
    DataTmpUndefdAndEvlnInProgs=1
    DevOfDataNotWithinRngAllwd=2
    DataCalcdWithDevDefd=3

#
# \enum Qly2# Main usage shall be to indicate quality for vehicle state estimations, not to be used for failure code setting and function degradation unless absolutely necassary.

#
class Qly2(Enum):
    Flt=0
    NoInfo=1
    Vld=2

#
# \enum Qly3# General DATA-TYPE to indicate quality. Each quality deviation value, De0-De7, shall be specified on the DATA-ELEMENT using this DATA-TYPE.

#
class Qly3(Enum):
    De0=0
    De1=1
    De2=2
    De3=3
    De4=4
    De5=5
    De6=6
    De7=7

#
# \enum QualityFactor
#
class QualityFactor(Enum):
    QfUndefd=0
    QfInProgs=1
    QfNotSpc=2
    QfSnsrDataOk=3

#
# \enum ReqSrc1
#
class ReqSrc1(Enum):
    NoReq=0
    TtgReq=1
    Resd2=2
    Resd3=3
    Resd4=4
    Resd5=5
    Resd6=6
    Resd7=7

#
# \struct RngBdIllmnCmdPen1
#
class RngBdIllmnCmdPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f71d0>
        self.Cmd = None  # type: RngbdIllmnCmd1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum RngbdIllmnCmd1# For running board illumination setting value

#
class RngbdIllmnCmd1(Enum):
    Di=0
    Aut=1
    Resd2=2
    Resd3=3

#
# \enum RoadFricWarnReq1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoWarn		No Warning		No warning
# 1			1			FricLoPsbl	Friction 	Low Possible		Potentially low friction
# 2			2			FricLoCfmd	Friction Low Confirmed	Confirmed low friction

#
class RoadFricWarnReq1(Enum):
    NoWarn=0
    FricLoPsbl=1
    FricLoCfmd=2

#
# \enum RotyDirUI
#
class RotyDirUI(Enum):
    Idle=0
    CW=1
    CCW=2
    Failr=3

#
# \enum RotyDirUI1# Signal for CCSM rotary knob

#
class RotyDirUI1(Enum):
    Idle=0
    CW=1
    CCW=2
    Spare=3

#
# \struct RouteInfoRec1# Route information for function Vehicle Energy Management

#
class RouteInfoRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688f7208>
        self.DestInfo = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688f7ac8>
        self.IdOfDest = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688f7b38>
        self.DestProblty = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688f7ba8>
        self.NrOfDestCmpl = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688f7c18>
        self.DestSrc = None  # type: int


#
# \enum RsdsMstSt
#
class RsdsMstSt(Enum):
    Cfg=0
    StrtUp=1
    Runng=2
    Blkd=3
    Faulty=4
    Shutdown=5
    Hot=6
    Cal=7

#
# \struct RsdsSysSts# Status of the different internal statemachines.
#
# Master Statemachine
# RsdsMstSt (Enum):
# Cfg
# StrtUp
# Runng
# Blkd
# Faulty
# Shutdown
# Hot
# Cal
#
# Lcma:
# OnOff1
# ActvInActv
#
# Ctra:
# OnOff1
# ActvInActv
#
# Rcw:
# OnOff1
#
# Rcm:
# OnOff1

#
class RsdsSysSts:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f7978>
        self.RsdsSts = None  # type: RsdsMstSt
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.LcmaSts = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6890f908>
        self.LcmaSts1 = None  # type: ActvInActv
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.CtraSts = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6890f908>
        self.CtraSts1 = None  # type: ActvInActv
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.RcwSts = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.RcmSts = None  # type: OnOff1


#
# \enum RstOfTrip1
#
class RstOfTrip1(Enum):
    NoRstOfTrip1=0
    RstOfDstSho1=1
    RstOfFuCnsAvg1=2
    RstOfEgyCnsAvg1=3
    RstOfForSpdAvg1=4
    RstOfTiDrv1=5
    RstOfAllTrip1=6
    Resv=7

#
# \enum SatltPosnSts# 0: No navigation solution
# 1: 3 satelites solution (2D)
# 2: more than 3 satellites solution (3D)
# 3: CSF (Cold Start Fix)

#
class SatltPosnSts(Enum):
    PosnStsOfLvl1=0
    PosnStsOfLvl2=1
    PosnStsOfLvl3=2
    PosnStsOfLvl4=3

#
# \enum SeatActvSpplFct1
#
class SeatActvSpplFct1(Enum):
    NotAvl=0
    LumExtnAndLumHei=1
    BackBlster=2
    CushExtn=3
    HdrestHeiAndHdrestTilt=4
    MassgFct=5

#
# \struct SeatBackUnlckd
#
class SeatBackUnlckd:
    def __init__(self):
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.SeatBackUnlckdLe = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.SeatBackUnlckdRi = None  # type: bool


#
# \enum SeatClimaLvl
#
class SeatClimaLvl(Enum):
    Off=0
    Lvl1=1
    Lvl2=2
    Lvl3=3

#
# \struct SeatDispBtnPsd
#
class SeatDispBtnPsd:
    def __init__(self):
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.DrvrSeatDispBtnPsd = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.PassSeatDispBtnPsd = None  # type: bool


#
# \enum SeatHeatDurgClimaEnad
#
class SeatHeatDurgClimaEnad(Enum):
    SeatHeatOff=0
    SeatDrvOn=1
    SeatPassOn=2
    SeatDrvrAndPass=3

#
# \struct SeatMassgFct
#
class SeatMassgFct:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.OnOff = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e0518>
        self.MassgProg = None  # type: MassgProgTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e0358>
        self.MassgInten = None  # type: MassgIntenLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e0358>
        self.MassgSpdLvl = None  # type: MassgIntenLvl


#
# \struct SetOfLang
#
class SetOfLang:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.IdPen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d8a58>
        self.LangTyp = None  # type: LangTyp


#
# \enum SetVol# 0 = No max volume set
# 1 = Max volume is set. Sound is muted

#
class SetVol(Enum):
    SoundNotSet=0
    SoundLo=1

#
# \struct SetgAndRstOfTripForDrvr# To set the indicaiton of trip computer and vehicle speed. Also reset.

#
class SetgAndRstOfTripForDrvr:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForDstLong = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForFuCns = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForFuDst = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForEgyDst = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForDstSho1 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForFuCnsAvg1 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForEgyCnsAvg1 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForSpdAvg1 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForTiDrv1 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForDstSho2 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForFuCnsAvg2 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForEgyCnsAvg2 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForSpdAvg2 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForTiDrv2 = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForDrvrMtr = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForSpdPtr = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForSpdDig = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripForSpdTrvl = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6f281898>
        self.SetgTripForRstOfAllTripFct1 = None  # type: RstOfTrip1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6887c0b8>
        self.SetgTripForTiRstCdn = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e43c8>
        self.SetgTripOffOn = None  # type: OnOffCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.SetgTripWithProfID = None  # type: IdPen


#
# \struct SftyCchActvnSts2# Current status of function and also time since function was last active in Years, months and days.

#
class SftyCchActvnSts2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.Actv = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688f9eb8>
        self.Yr = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688f9b38>
        self.Mth = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688f9c18>
        self.Day = None  # type: int


#
# \enum SftyCchCntxt1
#
class SftyCchCntxt1(Enum):
    NoInfo=0
    Fog=1
    Rain=2
    Darkness=3
    DenseTrfc=4
    FricLo=5
    HighwayDrvg=6
    CtyDrvg=7
    RoadW=8
    Misc9=9
    Misc10=10
    Misc11=11
    Misc12=12
    Misc13=13
    Misc14=14
    Misc15=15

#
# \enum SftyCchFb1
#
class SftyCchFb1(Enum):
    NoMsg=0
    Msg1=1
    Msg2=2
    Msg3=3
    Msg4=4
    Msg5=5
    Msg6=6
    Msg7=7
    Msg8=8
    Msg9=9
    Msg10=10
    Msg11=11
    Msg12=12
    Msg13=13
    Msg14=14
    Msg15=15
    Msg16=16
    Msg17=17
    Msg18=18
    Msg19=19
    Msg20=20
    Msg21=21
    Msg22=22
    Msg23=23
    Msg24=24
    Msg25=25
    Msg26=26
    Msg27=27
    Msg28=28
    Msg29=29
    Msg30=30
    Msg31=31
    Msg32=32
    Msg33=33
    Msg34=34
    Msg35=35
    Msg36=36
    Msg37=37
    Msg38=38
    Msg39=39
    Msg40=40
    Msg41=41
    Msg42=42
    Msg43=43
    Msg44=44
    Msg45=45
    Msg46=46
    Msg47=47
    Msg48=48
    Msg49=49
    Msg50=50
    Msg51=51
    Msg52=52
    Msg53=53
    Msg54=54
    Msg55=55
    Msg56=56
    Msg57=57
    Msg58=58
    Msg59=59
    Msg60=60
    Msg61=61
    Msg62=62
    Msg63=63

#
# \struct SftyCchIdx1
#
class SftyCchIdx1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688f9b00>
        self.Tot = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.TotAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887f710>
        self.DstToVeh = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.DstToVehAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887f6d8>
        self.LaneKeep = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.LaneKeepAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887f780>
        self.Attention = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.AttentionAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887f828>
        self.KeepSpdLim = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.KeepSpdLimAvl = None  # type: FcnAvlSts1


#
# \struct SftyCchIdx2
#
class SftyCchIdx2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887f6a0>
        self.Tot = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.TotAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9a58>
        self.TotCntxt = None  # type: SftyCchCntxt1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887fc18>
        self.DstToVeh = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.DstToVehAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9a58>
        self.DstToVehCntxt = None  # type: SftyCchCntxt1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887fc88>
        self.LaneKeep = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.LaneKeepAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9a58>
        self.LaneKeepCntxt = None  # type: SftyCchCntxt1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887fd68>
        self.Attention = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.AttentionAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9a58>
        self.AttentionCntxt = None  # type: SftyCchCntxt1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6887fe48>
        self.KeepSpdLim = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.KeepSpdLimAvl = None  # type: FcnAvlSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9a58>
        self.KeepSpdLimCntxt = None  # type: SftyCchCntxt1
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688810b8>
        self.FldTot = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc1d0>
        self.FldTotAvl = None  # type: FcnAvlSts1


#
# \enum SftyCchPostTripSts1
#
class SftyCchPostTripSts1(Enum):
    NoData=0
    Drvg=1
    PostTrip=2

#
# \struct SftySigGroupFromAudSafe1
#
class SftySigGroupFromAudSafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2630>
        self.SftySigFaildDetdByAud = None  # type: NoYesCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2630>
        self.SftyAudDend = None  # type: NoYesCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2630>
        self.SftyAudEna = None  # type: NoYesCrit1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6887fac8>
        self.SftySigGroupFromAudSafeChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68881128>
        self.SftySigGroupFromAudSafeCntr = None  # type: int


#
# \struct SftyWarnGroupFromAsySafe1
#
class SftyWarnGroupFromAsySafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2630>
        self.CnclWarnLatForAutDrv = None  # type: NoYesCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2630>
        self.SteerOvrdWarnReqForAutDrv = None  # type: NoYesCrit1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68884f28>
        self.SteerStsForAutDrv = None  # type: SteerStsForAutDrv3
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68881240>
        self.SftyWarnGroupFromAsySafeChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688811d0>
        self.SftyWarnGroupFromAsySafeCntr = None  # type: int


#
# \enum SlowNormFast
#
class SlowNormFast(Enum):
    Ukwn=0
    Slow=1
    Norm=2
    Fast=3

#
# \struct SlowNormFastPen
#
class SlowNormFastPen:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6887f8d0>
        self.SlowNormFast = None  # type: SlowNormFast
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.IdPen = None  # type: IdPen


#
# \struct SnsrParkAssi1
#
class SnsrParkAssi1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68932d30>
        self.DstOfSnsrInsdLe = None  # type: DstOfSnsr1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68932d30>
        self.DstOfSnsrOutdLe = None  # type: DstOfSnsr1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68932d30>
        self.DstOfSnsrLeSide = None  # type: DstOfSnsr1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68932d30>
        self.DstOfSnsrInsdRi = None  # type: DstOfSnsr1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68932d30>
        self.DstOfSnsrOutdRi = None  # type: DstOfSnsr1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68932d30>
        self.DstOfSnsrRiSide = None  # type: DstOfSnsr1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68899710>
        self.AudWarnDir = None  # type: WarnDir1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e4cc0>
        self.AudSideWarn = None  # type: ParkAssiLeRi1


#
# \struct SnsrPrkgAssi2# New record type to hold distances using new data type for longer range ultrasound sensors (DstOfSnsr2). Rest of content (audio warning data types) is the same as in (older) SnsrParkAssi1 record type.

#
class SnsrPrkgAssi2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688816a0>
        self.SnsrDstInsdLe = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881908>
        self.SnsrDstOutdLe = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881978>
        self.SnsrDstSideLe = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688819e8>
        self.SnsrDstInsdRi = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881a58>
        self.SnsrDstOutdRi = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881ac8>
        self.SnsrDstSideRi = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68899710>
        self.AudWarnDir = None  # type: WarnDir1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e4cc0>
        self.AudSideWarn = None  # type: ParkAssiLeRi1


#
# \struct SnsrPrkgAssi3# New record type to hold distances using new data type for longer range ultrasound sensors (DstOfSnsr2 and DstOfSnsr3).

#
class SnsrPrkgAssi3:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688816d8>
        self.SnsrDstInsdLe = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881710>
        self.SnsrDstOutdLe = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881c50>
        self.SnsrDstSideLe = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881cc0>
        self.SnsrDstInsdRi = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881d30>
        self.SnsrDstOutdRi = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68881da0>
        self.SnsrDstSideRi = None  # type: float


#
# \enum Snvty1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			NotInUse		Not In Use	Not used
# 1			1			LoSnvty		Low Sensitivity	Low Sensitivity
# 2			2			NormSnvty	Normal Sensitivity	Normal Sensitivity
# 3			3			HiSnvty		High Sensitivity	High Sensitivity

#
class Snvty1(Enum):
    NotInUse=0
    LoSnvty=1
    NormSnvty=2
    HiSnvty=3

#
# \enum Snvty4
#
class Snvty4(Enum):
    CmftMod=0
    NormMod=1
    SptMod=2
    CrvtMod=3

#
# \struct SnvtyPen1
#
class SnvtyPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68881518>
        self.Sts = None  # type: Snvty1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \struct SnvtyPen3
#
class SnvtyPen3:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68881e80>
        self.Sts = None  # type: Snvty4
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum SoundHptc1
#
class SoundHptc1(Enum):
    Sound=0
    Hptc=1

#
# \enum SpdLimWarnReq1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No warning
# 1			1			SpdHi			Speed High			Too high speed compared to speed limit
# 2			2			SpdHiAndSpdCtrlPassd	Speed High And Speed Control Passed	Too high speed and passing speed camera

#
class SpdLimWarnReq1(Enum):
    NoWarn=0
    SpdHi=1
    SpdHiAndSpdCtrlPassd=2

#
# \enum SpdUnit1# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			KiloMtrPerHr	Kilo Meter Per Hour	km/h
# 1			1			MilePerHr		Mile Per Hour	mph

#
class SpdUnit1(Enum):
    KiloMtrPerHr=0
    MilePerHr=1

#
# \enum SrvSts# Used be Connected service booking to inform Driver information about the current status of a service booking.

#
class SrvSts(Enum):
    NoSrv=0
    Srv=1
    CfmdSrv=2
    SrvPassd=3

#
# \enum SteerAsscLvl
#
class SteerAsscLvl(Enum):
    Ukwn=0
    Lvl1=1
    Lvl2=2
    Lvl3=3
    Lvl4=4
    Resd5=5
    Resd6=6
    Resd7=7

#
# \struct SteerAssiLvlForAutDrvPen1
#
class SteerAssiLvlForAutDrvPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688de630>
        self.Sts = None  # type: Lvl2


#
# \enum SteerMod# Mod1 = Comfort
# Mod2 = Dynamic/Sport
# Mod3 = Reserved
# Mod4 = Reserved

#
class SteerMod(Enum):
    Ukwn=0
    Mod1=1
    Mod2=2
    Mod3=3
    Mod4=4
    Resd5=5
    Resd6=6
    Resd7=7

#
# \struct SteerSetg
#
class SteerSetg:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688846d8>
        self.SteerAsscLvl = None  # type: SteerAsscLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68884ac8>
        self.SteerMod = None  # type: SteerMod
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum SteerStsForAutDrv3
#
class SteerStsForAutDrv3(Enum):
    Off=0
    Stb=1
    Actv=2
    OvrdByDrvr=3
    PndStb=4

#
# \enum SteerWarnReqForAutDrv1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			NoWarn			No Warning				No warning
# 1			1			DispWarnForDrvrSteerLe	Display Warning For Driver Steering Left	Show warning that driver shall steer left
# 2			2			DispWarnForDrvrSteerRi	Display Warning For Driver Steering Right	Show warning that driver shall steer right
# 3			3			DispGenWarn		Display Generic Warning			Show general warning

#
class SteerWarnReqForAutDrv1(Enum):
    NoWarn=0
    DispWarnForDrvrSteerLe=1
    DispWarnForDrvrSteerRi=2
    DispGenWarn=3

#
# \enum SteerWhlHeatgOnCmdTyp
#
class SteerWhlHeatgOnCmdTyp(Enum):
    Off=0
    Lo=1
    Med=2
    Hi=3

#
# \struct SteerWhlSnsrSafe1
#
class SteerWhlSnsrSafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68884f98>
        self.SteerWhlAg = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68886710>
        self.SteerWhlAgSpd = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.SteerWhlSnsrQf = None  # type: GenQf1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68886780>
        self.SteerWhlSnsrChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688866d8>
        self.SteerWhlSnsrCntr = None  # type: int


#
# \enum StrtInProgs1
#
class StrtInProgs1(Enum):
    StrtStsOff=0
    StrtStsImminent=1
    StrtStsStrtng=2
    StrtStsRunng=3

#
# \enum StrtStopReq1# Used for sending a Start or a Stop/Cancel request from HMI to a core function

#
class StrtStopReq1(Enum):
    Idle=0
    StrtReq=1
    StopReq=2
    Resd=3

#
# \enum SurrndgsLiLvl# 0 = Full
# 1 = Reduced

#
class SurrndgsLiLvl(Enum):
    LvlFull=0
    LvlReduced=1

#
# \enum SuspCtrlSetgTyp
#
class SuspCtrlSetgTyp(Enum):
    Cmft=0
    Norm=1
    Dyn=2

#
# \struct SuspSetgRec
#
class SuspSetgRec:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.SuspIdPen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688de9e8>
        self.LvlCtrlSetg = None  # type: LvlCtrlSetgTyp
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68886ba8>
        self.SuspCtrlSetg = None  # type: SuspCtrlSetgTyp


#
# \enum SwtHozlSts1
#
class SwtHozlSts1(Enum):
    Idle=0
    Fwd=1
    Backw=2

#
# \enum SwtPush
#
class SwtPush(Enum):
    NotPsd=0
    Psd=1

#
# \enum SwtVertSts1
#
class SwtVertSts1(Enum):
    Idle=0
    Up=1
    Dwn=2

#
# \struct SysU# System voltage represented by the CEM node Voltage

#
class SysU:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68886dd8>
        self.SysU = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.SysUQf = None  # type: GenQf1


#
# \enum TelmSts
#
class TelmSts(Enum):
    NoErr=0
    TelmErr=1
    TelmDi=2
    TelmEnd=3

#
# \struct TiCorrn
#
class TiCorrn:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688882e8>
        self.HrCorrn = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688886a0>
        self.MinsCorrn = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68888710>
        self.DayLiSaveTi = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SpprtForFct = None  # type: NoYes1


#
# \struct TiGapAdpvSeldPen1
#
class TiGapAdpvSeldPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e0208>
        self.Sts = None  # type: ManAdpv1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \struct TiGapPen1
#
class TiGapPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688885f8>
        self.Sts = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \struct TireCircumCalByNav
#
class TireCircumCalByNav:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68888780>
        self.TireCircum = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.HiQly = None  # type: NoYes1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e2518>
        self.SpprtForFct = None  # type: NoYes1


#
# \struct TirePAbsltValFrntLe3
#
class TirePAbsltValFrntLe3:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688887f0>
        self.TirepabsltVal1 = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688889b0>
        self.TirePabsltValQF = None  # type: TirePAbsltValQf
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68889748>
        self.TirePPosn = None  # type: TirePPosn


#
# \struct TirePAbsltValFrntRi1
#
class TirePAbsltValFrntRi1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68888978>
        self.TirePAbsltVal1 = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688889b0>
        self.TirePAbsltValQF = None  # type: TirePAbsltValQf
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68889748>
        self.TirePPosn = None  # type: TirePPosn


#
# \enum TirePAbsltValQf
#
class TirePAbsltValQf(Enum):
    Norm=0
    OldVal=1
    UpdVal=2
    SnsrFlt=3

#
# \struct TirePAbsltValReLe1
#
class TirePAbsltValReLe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68888a20>
        self.TirePAbsltVal1 = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688889b0>
        self.TirePAbsltValQF = None  # type: TirePAbsltValQf
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68889748>
        self.TirePPosn = None  # type: TirePPosn


#
# \struct TirePAbsltValReRi1
#
class TirePAbsltValReRi1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68888f60>
        self.TirePAbsltVal1 = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688889b0>
        self.TirePAbsltValQF = None  # type: TirePAbsltValQf
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68889748>
        self.TirePPosn = None  # type: TirePPosn


#
# \enum TirePCalSts2
#
class TirePCalSts2(Enum):
    Norm=0
    Cal=1
    CalNotCmpl=2
    CalCmpl=3

#
# \struct TirePMonData1
#
class TirePMonData1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68889240>
        self.TirePMonSts1 = None  # type: TirePMonSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688897f0>
        self.TirePReRi = None  # type: TirePWarn
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688897f0>
        self.TirePReLe = None  # type: TirePWarn
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688897f0>
        self.TirePFrntLe = None  # type: TirePWarn
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688897f0>
        self.TirePFrntRe = None  # type: TirePWarn


#
# \enum TirePMonSts1
#
class TirePMonSts1(Enum):
    NoWarn=0
    SysFlt=1
    FourMiss=2
    Resd1=3
    GmnSoftWarn=4
    GmnHardWarn=5
    Resd2=6
    Resd3=7
    Resd4=8
    Resd5=9

#
# \enum TirePPosn
#
class TirePPosn(Enum):
    NoPReadingOrPReadingOkWithOutPosn=0
    PReadingWithPosn=1

#
# \enum TirePWarn
#
class TirePWarn(Enum):
    NoWarn=0
    SoftWarn=1
    HardWarn=2
    ResdWarn=3

#
# \struct TqSafe2
#
class TqSafe2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68889320>
        self.TqAct = None  # type: float
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68889ac8>
        self.TqActChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68889b38>
        self.TqActCntr = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5c88>
        self.TqActQlyFac = None  # type: QualityFactor
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68889ba8>
        self.SpdLimn = None  # type: float


#
# \enum Trig1
#
class Trig1(Enum):
    NoTrig=0
    Trig=1

#
# \enum TripModSeln# Selects trip trip meter type
# Trip1 - Reset by driver
# Trip2 - Reset by timer (new trip condition)
# Resd - Reserved for future Trip indication modes

#
class TripModSeln(Enum):
    Ukwn=0
    Trip1=1
    Trip2=2
    Resd=3

#
# \enum TrlrLampChkSts1# 0 = Trailer Lamp Check not possible to execute
# 1 = Trailer Lamp Check possible to execute

#
class TrlrLampChkSts1(Enum):
    TrlrLampChkNotAvl=0
    TrlrLampChkAvl=1

#
# \enum TrlrLampChkSts2# 0 = No status
# 1 = Trailer Lamp Check started
# 2 = Trailer Lamp Check completed
# 3 = Operation canceled
# 4 = Trailer Lamp Check requesting engine off to driver
# 5 = Reserved
# 6 = Reserved
# 7 = Reserved

#
class TrlrLampChkSts2(Enum):
    Idle=0
    TrlrLampChkStrtd=1
    TrlrLampChkCmpl=2
    CnclOper=3
    TrlrLampEngOffReqd=4
    Resd1=5
    Resd2=6
    Resd3=7

#
# \enum TrlrM
#
class TrlrM(Enum):
    Lvl0=0
    Lvl1=1
    Lvl2=2
    Lvl3=3

#
# \enum TrlrPrsntSts# 0 = Trailer unconnected
# 1 = Trailer connected to the vehicle

#
class TrlrPrsntSts(Enum):
    TrlrNotPrsnt=0
    TrlrPrsnt=1

#
# \enum TurnSpdWarnReq1# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No warning
# 1			1			WarnForCmngCrvtLe	Warning For Coming Curvature Left	Warning for upcoming left curve
# 2			2			WarnForCmngCrvtRi	Warning For Coming Curvature Right	Warning for upcoming right curve

#
class TurnSpdWarnReq1(Enum):
    NoWarn=0
    WarnForCmngCrvtLe=1
    WarnForCmngCrvtRi=2

#
# \struct TwliBriRaw
#
class TwliBriRaw:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68889a58>
        self.TwliBriRaw1 = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.TwliBriRawQf = None  # type: GenQf1


#
# \enum TwliBriSts1
#
class TwliBriSts1(Enum):
    Night=0
    Day=1

#
# \enum Typ1# Basic type list
# 8 types

#
class Typ1(Enum):
    Typ0=0
    Typ1=1
    Typ2=2
    Typ3=3
    Typ4=4
    Typ5=5
    Typ6=6
    Typ7=7

#
# \struct UInt64Rec
#
class UInt64Rec:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888e668>
        self.Byte0 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888eda0>
        self.Byte1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888ee10>
        self.Byte2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888ee80>
        self.Byte3 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888eef0>
        self.Byte4 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888efd0>
        self.Byte5 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888ed68>
        self.Byte6 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888f080>
        self.Byte7 = None  # type: int


#
# \enum UnlckKeylsCfg2
#
class UnlckKeylsCfg2(Enum):
    UnlckCen=0
    UnlckInd=1

#
# \struct UnlckKeylsCfgPen2
#
class UnlckKeylsCfgPen2:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.IdPen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6888eba8>
        self.KeylsCfg = None  # type: UnlckKeylsCfg2


#
# \enum UnlckRemCfg1
#
class UnlckRemCfg1(Enum):
    UnlckCen=0
    UnlckStep2=1

#
# \struct UnlckRemCfgPen1
#
class UnlckRemCfgPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6888ec88>
        self.Sts = None  # type: UnlckRemCfg1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum UsgModSts1# UsageMode Data

#
class UsgModSts1(Enum):
    UsgModAbdnd=0
    UsgModInActv=1
    UsgModCnvinc=2
    UsgModActv=11
    UsgModDrvg=13

#
# \struct UsrSetSpdForKeySpdLimn
#
class UsrSetSpdForKeySpdLimn:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888f1d0>
        self.Sts = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \struct UsrSetSpdForKeySpdWarn# Conveys the 6 personalized speed thresholds for Restricted Key Speed Alert function.
#
# Value 0 = the speed threshold is not used
#
# Personal ID shows to which Profile the settings belongs.

#
class UsrSetSpdForKeySpdWarn:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888f3c8>
        self.UsrSetSpdForKeySpdWarn1 = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888f630>
        self.UsrSetSpdForKeySpdWarn2 = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888f6a0>
        self.UsrSetSpdForKeySpdWarn3 = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888f710>
        self.UsrSetSpdForKeySpdWarn4 = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888f780>
        self.UsrSetSpdForKeySpdWarn5 = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888f7f0>
        self.UsrSetSpdForKeySpdWarn6 = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.UsrSetSpdForKeySpdWarnPen = None  # type: IdPen


#
# \struct UsrSetVolMaxForKeyVolLimn
#
class UsrSetVolMaxForKeyVolLimn:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f97b8>
        self.SetVol = None  # type: SetVol


#
# \struct UsrSwtDispClimaReqForRowSec
#
class UsrSwtDispClimaReqForRowSec:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888f400>
        self.UsrSwtDispClimaReqForTSpForRowSecLe = None  # type: float
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.UsrSwtDispUpdClimaReqForTSpForRowSecLe = None  # type: bool
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888fb00>
        self.UsrSwtDispClimaReqForTSpForRowSecRi = None  # type: float
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.UsrSwtDispUpdClimaReqForTSpForRowSecRi = None  # type: bool
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi = None  # type: bool
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c0898>
        self.UsrSwtDispReqForFanLvlForRowSec = None  # type: HmiHvacFanLvl
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.UsrSwtDispUpdReqForFanLvlForRowSec = None  # type: bool
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c04e0>
        self.UsrSwtDispClimaReqForTSpSpclForRowSecLe = None  # type: HmiCmptmtTSpSpcl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c04e0>
        self.UsrSwtDispClimaReqForTSpSpclForRowSecRi = None  # type: HmiCmptmtTSpSpcl


#
# \struct UsrSwtDispClimaSts
#
class UsrSwtDispClimaSts:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888fa90>
        self.UsrSwtDispClimaTSpForRowSecLe = None  # type: float
        #  <dataelements_generator.model.DE_Value object at 0x7f0f6888fa20>
        self.UsrSwtDispClimaTSpForRowSecRi = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.UsrSwtDispSeatHeatLvlForRowSecLe = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.UsrSwtDispSeatHeatLvlForRowSecRi = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c0898>
        self.UsrSwtDispFanLvlForRowSec = None  # type: HmiHvacFanLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c04e0>
        self.UsrSwtDispClimaTSpSpclForRowSecLe = None  # type: HmiCmptmtTSpSpcl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688c04e0>
        self.UsrSwtDispClimaTSpSpclForRowSecRi = None  # type: HmiCmptmtTSpSpcl


#
# \struct UsrSwtDispForSecRowSeatVentn
#
class UsrSwtDispForSecRowSeatVentn:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.UsrSwtDispForSecRowSeatVentnLe = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.UsrSwtDispForSecRowSeatVentnRi = None  # type: SeatClimaLvl


#
# \struct UsrSwtDispReqForSecRowSeatVentn
#
class UsrSwtDispReqForSecRowSeatVentn:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.UsrSwtDispReqForSecRowSeatVentnLe = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f9320>
        self.UsrSwtDispReqForSecRowSeatVentnRi = None  # type: SeatClimaLvl
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.usrSwtDispUpdReqForSecRowSeatVentnLe = None  # type: bool
        #  <dataelements_generator.model.DE_Boolean object at 0x7f0f6d74c080>
        self.usrSwtDispUpdReqForSecRowSeatVentnRi = None  # type: bool


#
# \struct UsrSwtDispReqVrnt
#
class UsrSwtDispReqVrnt:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.UsrSwtDispSeatHeatFct = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6888f5f8>
        self.UsrSwtDispTUnit = None  # type: UsrSwtDispTUnit


#
# \enum UsrSwtDispTUnit
#
class UsrSwtDispTUnit(Enum):
    Celcius=0
    Farenheit=1

#
# \struct VFCGrp
#
class VFCGrp:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f6888f940>
        self.Grp1 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893208>
        self.Grp2 = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893278>
        self.Grp3 = None  # type: int


#
# \enum VehActv1# 0  No text
# 1  "Engine is running"
# 2  "Ignition on"
# 3  TBD

#
class VehActv1(Enum):
    NoMsg=0
    Msg1=1
    Msg2=2
    Msg3=3

#
# \struct VehCfgPrm
#
class VehCfgPrm:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68893198>
        self.BlkIDBytePosn1 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688935f8>
        self.CCPBytePosn2 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68893668>
        self.CCPBytePosn3 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688936d8>
        self.CCPBytePosn4 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68893748>
        self.CCPBytePosn5 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688937b8>
        self.CCPBytePosn6 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68893828>
        self.CCPBytePosn7 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68893898>
        self.CCPBytePosn8 = None  # type: int


#
# \struct VehMNomRec1
#
class VehMNomRec1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68893588>
        self.VehM = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688f5748>
        self.VehMQly = None  # type: Qly2
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6888e240>
        self.TrlrM = None  # type: TrlrM


#
# \struct VehModMngtGlbSafe1
#
class VehModMngtGlbSafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6888f198>
        self.UsgModSts = None  # type: UsgModSts1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68927198>
        self.CarModSts1 = None  # type: CarModSts1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688934a8>
        self.CarModSubtypWdCarModSubtyp = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893b38>
        self.EgyLvlElecMai = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893c18>
        self.EgyLvlElecSubtyp = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893c88>
        self.PwrLvlElecMai = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893cf8>
        self.PwrLvlElecSubtyp = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bc908>
        self.FltEgyCnsWdSts = None  # type: FltEgyCns1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893d68>
        self.Chks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893b00>
        self.Cntr = None  # type: int


#
# \enum VehMtnSt2
#
class VehMtnSt2(Enum):
    Ukwn=0
    StandStillVal1=1
    StandStillVal2=2
    StandStillVal3=3
    RollgFwdVal1=4
    RollgFwdVal2=5
    RollgBackwVal1=6
    RollgBackwVal2=7

#
# \struct VehMtnStSafe1
#
class VehMtnStSafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68893470>
        self.VehMtnSt = None  # type: VehMtnSt2
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893f98>
        self.VehMtnStChks = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68896080>
        self.VehMtnStCntr = None  # type: int


#
# \struct VehSpdIndcd1# Data type for indicated vehicle speed, containing unitless number for the value of the speed, and a separate defined unit.

#
class VehSpdIndcd1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68893f60>
        self.VehSpdIndcd = None  # type: int
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68893f28>
        self.VeSpdIndcdUnit = None  # type: VehSpdIndcdUnit


#
# \enum VehSpdIndcdUnit
#
class VehSpdIndcdUnit(Enum):
    Kmph=0
    Mph=1
    UkwnUnit=2

#
# \struct VehSpdLgtSafe1
#
class VehSpdLgtSafe1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688960b8>
        self.VehSpdLgt = None  # type: float
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688bd940>
        self.VehSpdLgtQf = None  # type: GenQf1
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f688963c8>
        self.VehSpdLgtCntr = None  # type: int
        #  <dataelements_generator.model.DE_Identical object at 0x7f0f68896390>
        self.VehSpdLgtChks = None  # type: int


#
# \struct Vin1
#
class Vin1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68896358>
        self.BlockNr = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68896668>
        self.VINSignalPos1 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688966d8>
        self.VINSignalPos2 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68896748>
        self.VINSignalPos3 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f688967b8>
        self.VINSignalPos4 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68896828>
        self.VINSignalPos5 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68896898>
        self.VINSignalPos6 = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68896908>
        self.VINSignalPos7 = None  # type: int


#
# \enum VisWarnReq1
#
class VisWarnReq1(Enum):
    Off=0
    Countdown=1
    LeaveNow=2

#
# \struct VisnAgWideCfg1
#
class VisnAgWideCfg1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.SteerPahRe = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.SteerPahFrnt = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.SteerPahSide = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.DstInfoRe = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.DstInfoFrnt = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.DstInfoSide = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.CtraInfo = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.TwbrPah = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.ObjDetn = None  # type: OnOff1


#
# \enum VisnImgAgWideInUse
#
class VisnImgAgWideInUse(Enum):
    NoImg=0
    Img1=1
    Img2=2
    Img3=3
    Img4=4
    Img5=5
    Img6=6
    Img7=7
    Img8=8
    Img9=9
    Img10=10
    Img11=11
    Img12=12
    Img13=13
    Img14=14
    Img15=15

#
# \enum VolUnit# Gallon 1 = UK gallons
# Gallon 2 = US gallons

#
class VolUnit(Enum):
    litre=0
    gallon1=1
    gallon2=2
    UkwnUnit=3

#
# \enum Warn2
#
class Warn2(Enum):
    NoWarn=0
    Warn=1

#
# \enum WarnAndIntv1
#
class WarnAndIntv1(Enum):
    WarnAndIntv=0
    Intv=1
    Warn=2

#
# \struct WarnAndIntvPen1
#
class WarnAndIntvPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f68899588>
        self.Sts = None  # type: WarnAndIntv1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \enum WarnDir1
#
class WarnDir1(Enum):
    NoWarn=0
    LeftSide=1
    OuterLeft=2
    InnerLeft=3
    InnerRight=4
    OuterRight=5
    RightSide=6

#
# \enum WarnLeRi
#
class WarnLeRi(Enum):
    NoWarn=0
    WarnLe=1
    WarnRi=2

#
# \enum WarnLeRi1
#
class WarnLeRi1(Enum):
    NoWarn=0
    WarnLe=1
    WarnRi=2

#
# \struct WarnTypForLaneChgWarnPen1
#
class WarnTypForLaneChgWarnPen1:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688841d0>
        self.Sts = None  # type: SoundHptc1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688d3b00>
        self.Pen = None  # type: IdPen


#
# \struct WhlRotToothCntrRec
#
class WhlRotToothCntrRec:
    def __init__(self):
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68899cf8>
        self.WhlRotToothCntrFrntLe = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68899e80>
        self.WhlRotToothCntrFrntRi = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68899ef0>
        self.WhlRotToothCntrReLe = None  # type: int
        #  <dataelements_generator.model.DE_Value object at 0x7f0f68899fd0>
        self.WhlRotToothCntrReRi = None  # type: int


#
# \enum WinAndRoofAndCurtPosnTyp
#
class WinAndRoofAndCurtPosnTyp(Enum):
    PosnUkwn=0
    ClsFull=1
    PercOpen4=2
    PercOpen8=3
    PercOpen12=4
    PercOpen16=5
    PercOpen20=6
    PercOpen24=7
    PercOpen28=8
    PercOpen32=9
    PercOpen36=10
    PercOpen40=11
    PercOpen44=12
    PercOpen48=13
    PercOpen52=14
    PercOpen56=15
    PercOpen60=16
    PercOpen64=17
    PercOpen68=18
    PercOpen72=19
    PercOpen76=20
    PercOpen80=21
    PercOpen84=22
    PercOpen88=23
    PercOpen92=24
    PercOpen96=25
    OpenFull=26
    Resd1=27
    Resd2=28
    Resd3=29
    Resd4=30
    Movg=31

#
# \struct WipgInfo# General information regarding wiping and the position of the wiper arm on the windscreen.

#
class WipgInfo:
    def __init__(self):
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f6889b828>
        self.WipgSpdInfo = None  # type: WipgSpdInfo
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.WiprActv = None  # type: OnOff1
        #  <dataelements_generator.model.DE_Enum object at 0x7f0f688e40f0>
        self.WiprInWipgAr = None  # type: OnOff1


#
# \enum WipgSpdInfo# The signal reflects the actual wiping speed or mode regardless of activation source. Both rain sensor and wiper stalk info are used to create the signal. When in interval mode the wipes are carried out in low speed.

#
class WipgSpdInfo(Enum):
    Off=0
    IntlLo=1
    IntlHi=2
    WipgSpd4045=3
    WipgSpd4650=4
    WipgSpd5155=5
    WipgSpd5660=6
    WiprErr=7

#
# \enum WiprFrntSrvModReq
#
class WiprFrntSrvModReq(Enum):
    NoActn=0
    ActvtSrvPosn=1
    DeActvtSrvPosn=2

#
# \enum YesNo2
#
class YesNo2(Enum):
    No=0
    Yes=1

#
# \enum iTPMSTirePCalSts# Calibration status

#
class iTPMSTirePCalSts(Enum):
    NoCalReq=0
    CalOn=1
    CalCmpl=2
    CalFaild=3
    Resd1=4
    Resd2=5
    Resd3=6

#
# \enum iTPMSTirePMSts# iTPMS system status and warnings

#
class iTPMSTirePMSts(Enum):
    NoWarn=0
    CmnWarn=1
    WarnFL=2
    WarnFR=3
    WarnRL=4
    WarnRR=5
    SysUnAvi=6
    SysFailr=7

#
# \enum status# ClientConfigurationStatus response

#
class status(Enum):
    OK=0
    UNAVAILABLE=1
    NOK=2

