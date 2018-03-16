#coding: utf-8

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#AUTO GENERATED FILE, do not modify by hand.
# Generated with args: generate_signal_scaling.py --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml --fdxdescriptionfile=../CANoe/SPA2610/FDXDescriptionFile.xml --out=generated/pyDataElements.py
SWC_INPUT_FILE="SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml"
COM_INPUT_FILE="SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml"
from . import datatypes
from Sym import carsimconnection
import json
import time
import threading
import typing
import abc

class DataElements():
    __metaclass__=abc.ABCMeta

    @abc.abstractmethod
    def close(self):
        pass

    @abc.abstractmethod
    def send_ADataRawSafe(self, data):
        # type: (datatypes.ADataRawSafe1) -> None
        pass

    @abc.abstractmethod
    def stop_ADataRawSafe(self):
        pass

    @abc.abstractmethod
    def get_AccAdprSpdLimActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_AccAdprTurnSpdActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_AccAutResuWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_AccAutResuWarnReq(self):
        pass

    @abc.abstractmethod
    def send_AccSts(self, data):
        # type: (datatypes.AccSts1) -> None
        pass

    @abc.abstractmethod
    def stop_AccSts(self):
        pass

    @abc.abstractmethod
    def send_ActrPosnFrnReqForOsaAndRecirc(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_ActrPosnFrnReqForOsaAndRecirc(self):
        pass

    @abc.abstractmethod
    def get_ActvNoiseCtrlReq(self):
        # type: () -> (datatypes.NormSptPen)
        pass

    @abc.abstractmethod
    def send_ActvOfHorn(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_ActvOfHorn(self):
        pass

    @abc.abstractmethod
    def get_ActvnOfCrsEco(self):
        # type: () -> (datatypes.ActvnOfCrsEco)
        pass

    @abc.abstractmethod
    def get_ActvnOfPrkgAssi(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def get_ActvnOfPrkgAut(self):
        # type: () -> datatypes.ActvnOfPrkgAut
        pass

    @abc.abstractmethod
    def get_ActvnOfSwtIllmnCen(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def get_ActvnOfSwtIllmnClima(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def get_ActvnOfSwtIllmnDrvMod(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def get_ActvnOfSwtIllmnForSeatHeatrRe(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def get_ActvtAutnmsPrkgCtrl(self):
        # type: () -> datatypes.ActvtAutnmsPrkgCtrl
        pass

    @abc.abstractmethod
    def send_AdjSpdLimnSts(self, data):
        # type: (datatypes.AdjSpdLimnSts2) -> None
        pass

    @abc.abstractmethod
    def stop_AdjSpdLimnSts(self):
        pass

    @abc.abstractmethod
    def send_AdjSpdLimnWarn(self, data):
        # type: (datatypes.AdjSpdLimnWarnCoding) -> None
        pass

    @abc.abstractmethod
    def stop_AdjSpdLimnWarn(self):
        pass

    @abc.abstractmethod
    def get_AdprTurnSpdMod(self):
        # type: () -> (datatypes.AdprTurnSpdModPen1)
        pass

    @abc.abstractmethod
    def send_AgDataRawSafe(self, data):
        # type: (datatypes.AgDataRawSafe1) -> None
        pass

    @abc.abstractmethod
    def stop_AgDataRawSafe(self):
        pass

    @abc.abstractmethod
    def get_LiExtReq1WdReq5(self):
        # type: () -> (datatypes.SlowNormFastPen)
        pass

    @abc.abstractmethod
    def get_AirClngReq(self):
        # type: () -> datatypes.OnOffNoReq
        pass

    @abc.abstractmethod
    def send_AmbTIndcdWithUnit(self, data):
        # type: (datatypes.AmbTIndcdWithUnit) -> None
        pass

    @abc.abstractmethod
    def stop_AmbTIndcdWithUnit(self):
        pass

    @abc.abstractmethod
    def send_AmbTRaw(self, data):
        # type: (datatypes.AmbTWithQly) -> None
        pass

    @abc.abstractmethod
    def stop_AmbTRaw(self):
        pass

    @abc.abstractmethod
    def get_AntithftRednReq(self):
        # type: () -> (int)
        pass

    @abc.abstractmethod
    def send_AsyFctDeactvnWarn(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_AsyFctDeactvnWarn(self):
        pass

    @abc.abstractmethod
    def send_AsyLaneChg(self, data):
        # type: (datatypes.AsyLaneChg1) -> None
        pass

    @abc.abstractmethod
    def stop_AsyLaneChg(self):
        pass

    @abc.abstractmethod
    def send_AsySteerApplyRqrd(self, data):
        # type: (datatypes.LvlWarn1) -> None
        pass

    @abc.abstractmethod
    def stop_AsySteerApplyRqrd(self):
        pass

    @abc.abstractmethod
    def send_AsySteerFctDeactvdWarn(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_AsySteerFctDeactvdWarn(self):
        pass

    @abc.abstractmethod
    def send_AudMsgReq(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_AudMsgReq(self):
        pass

    @abc.abstractmethod
    def get_AudWarnActv(self):
        # type: () -> datatypes.NoYesCrit1
        pass

    @abc.abstractmethod
    def send_AutnmsPrkgActvScenario(self, data):
        # type: (datatypes.AutnmsPrkgActvScenario) -> None
        pass

    @abc.abstractmethod
    def stop_AutnmsPrkgActvScenario(self):
        pass

    @abc.abstractmethod
    def get_AutnmsPrkgActvn(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_AutnmsPrkgAvlScenario(self, data):
        # type: (datatypes.AutnmsPrkgAvlScenario) -> None
        pass

    @abc.abstractmethod
    def stop_AutnmsPrkgAvlScenario(self):
        pass

    @abc.abstractmethod
    def send_AutnmsPrkgEnaResu(self, data):
        # type: (datatypes.EnaResu) -> None
        pass

    @abc.abstractmethod
    def stop_AutnmsPrkgEnaResu(self):
        pass

    @abc.abstractmethod
    def send_AutnmsPrkgProgs(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_AutnmsPrkgProgs(self):
        pass

    @abc.abstractmethod
    def get_AutnmsPrkgSeldScenario(self):
        # type: () -> (datatypes.AutnmsPrkgSeldScenario)
        pass

    @abc.abstractmethod
    def send_AvlStsForLatAutDrv(self, data):
        # type: (datatypes.AvlStsForLatAutDrv5) -> None
        pass

    @abc.abstractmethod
    def stop_AvlStsForLatAutDrv(self):
        pass

    @abc.abstractmethod
    def send_BackCntrForMissCom(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_BackCntrForMissCom(self):
        pass

    @abc.abstractmethod
    def get_BlisSwOnOff(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_BltLockStAtDrvr(self, data):
        # type: (datatypes.BltLockStFrnt) -> None
        pass

    @abc.abstractmethod
    def stop_BltLockStAtDrvr(self):
        pass

    @abc.abstractmethod
    def send_BltLockStAtPass(self, data):
        # type: (datatypes.BltLockStFrnt) -> None
        pass

    @abc.abstractmethod
    def stop_BltLockStAtPass(self):
        pass

    @abc.abstractmethod
    def send_BltLockStAtRowSecLe(self, data):
        # type: (datatypes.BltLockStRe) -> None
        pass

    @abc.abstractmethod
    def stop_BltLockStAtRowSecLe(self):
        pass

    @abc.abstractmethod
    def send_BltLockStAtRowSecMid(self, data):
        # type: (datatypes.BltLockStRe) -> None
        pass

    @abc.abstractmethod
    def stop_BltLockStAtRowSecMid(self):
        pass

    @abc.abstractmethod
    def send_BltLockStAtRowSecRi(self, data):
        # type: (datatypes.BltLockStRe) -> None
        pass

    @abc.abstractmethod
    def stop_BltLockStAtRowSecRi(self):
        pass

    @abc.abstractmethod
    def send_BltLockStAtRowThrdLe(self, data):
        # type: (datatypes.BltLockStRe) -> None
        pass

    @abc.abstractmethod
    def stop_BltLockStAtRowThrdLe(self):
        pass

    @abc.abstractmethod
    def send_BltLockStAtRowThrdRi(self, data):
        # type: (datatypes.BltLockStRe) -> None
        pass

    @abc.abstractmethod
    def stop_BltLockStAtRowThrdRi(self):
        pass

    @abc.abstractmethod
    def send_BltRmnSound1(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_BltRmnSound1(self):
        pass

    @abc.abstractmethod
    def send_BltRmnSound2(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_BltRmnSound2(self):
        pass

    @abc.abstractmethod
    def send_BltRmnSound3(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_BltRmnSound3(self):
        pass

    @abc.abstractmethod
    def send_BltRmnSound4(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_BltRmnSound4(self):
        pass

    @abc.abstractmethod
    def send_BrkAndAbsWarnIndcnReq(self, data):
        # type: (datatypes.BrkAndAbsWarnIndcnReqRec1) -> None
        pass

    @abc.abstractmethod
    def stop_BrkAndAbsWarnIndcnReq(self):
        pass

    @abc.abstractmethod
    def send_BrkFldLvl(self, data):
        # type: (datatypes.FldLvl) -> None
        pass

    @abc.abstractmethod
    def stop_BrkFldLvl(self):
        pass

    @abc.abstractmethod
    def send_BrkFricTqAtWhlAct(self, data):
        # type: (datatypes.BrkFricTqAtWhlActRec) -> None
        pass

    @abc.abstractmethod
    def stop_BrkFricTqAtWhlAct(self):
        pass

    @abc.abstractmethod
    def send_BrkRelsWarnReq(self, data):
        # type: (datatypes.NoYes1) -> None
        pass

    @abc.abstractmethod
    def stop_BrkRelsWarnReq(self):
        pass

    @abc.abstractmethod
    def send_Btn1ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_Btn1ForUsrSwtPanFrntReq(self):
        pass

    @abc.abstractmethod
    def send_Btn2ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_Btn2ForUsrSwtPanFrntReq(self):
        pass

    @abc.abstractmethod
    def send_Btn3ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_Btn3ForUsrSwtPanFrntReq(self):
        pass

    @abc.abstractmethod
    def send_Btn4ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_Btn4ForUsrSwtPanFrntReq(self):
        pass

    @abc.abstractmethod
    def send_Btn5ForUsrSwtPanFrntReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_Btn5ForUsrSwtPanFrntReq(self):
        pass

    @abc.abstractmethod
    def send_BtnAudFbSts(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_BtnAudFbSts(self):
        pass

    @abc.abstractmethod
    def send_BtnAudVolSts(self, data):
        # type: (datatypes.BtnAudVolSts) -> None
        pass

    @abc.abstractmethod
    def stop_BtnAudVolSts(self):
        pass

    @abc.abstractmethod
    def get_BtnIllmnForWinDefrstFrnt(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_BtnMmedLeRiSts(self, data):
        # type: (datatypes.BtnMmedLeRiSts) -> None
        pass

    @abc.abstractmethod
    def stop_BtnMmedLeRiSts(self):
        pass

    @abc.abstractmethod
    def send_BtnMmedModSts(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_BtnMmedModSts(self):
        pass

    @abc.abstractmethod
    def send_BtnMmedSetSts(self, data):
        # type: (datatypes.BtnMmedSetSts) -> None
        pass

    @abc.abstractmethod
    def stop_BtnMmedSetSts(self):
        pass

    @abc.abstractmethod
    def send_CCSMBtn6(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_CCSMBtn6(self):
        pass

    @abc.abstractmethod
    def get_CallSts(self):
        # type: () -> datatypes.CallSts
        pass

    @abc.abstractmethod
    def send_CarTiGlb(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_CarTiGlb(self):
        pass

    @abc.abstractmethod
    def send_CchForFuEco(self, data):
        # type: (datatypes.CchForFuEco1) -> None
        pass

    @abc.abstractmethod
    def stop_CchForFuEco(self):
        pass

    @abc.abstractmethod
    def send_ChdLockgProtnStsToHmi(self, data):
        # type: (datatypes.OnOffSafe1) -> None
        pass

    @abc.abstractmethod
    def stop_ChdLockgProtnStsToHmi(self):
        pass

    @abc.abstractmethod
    def send_ChdWinProtnStsAtDrvrRe(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_ChdWinProtnStsAtDrvrRe(self):
        pass

    @abc.abstractmethod
    def send_ChdWinProtnStsAtPassRe(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_ChdWinProtnStsAtPassRe(self):
        pass

    @abc.abstractmethod
    def send_ChrgrHwAprvdWirelsAdr(self, data):
        # type: (datatypes.HwAprvdWirelsAdr1) -> None
        pass

    @abc.abstractmethod
    def stop_ChrgrHwAprvdWirelsAdr(self):
        pass

    @abc.abstractmethod
    def send_ChrgrWirelsSts(self, data):
        # type: (datatypes.ChrgrWirelsSts) -> None
        pass

    @abc.abstractmethod
    def stop_ChrgrWirelsSts(self):
        pass

    @abc.abstractmethod
    def send_ClimaActv(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_ClimaActv(self):
        pass

    @abc.abstractmethod
    def send_ClimaPwrCns(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_ClimaPwrCns(self):
        pass

    @abc.abstractmethod
    def get_ClimaRqrdFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        pass

    @abc.abstractmethod
    def get_ClimaTmr(self):
        # type: () -> (float)
        pass

    @abc.abstractmethod
    def send_ClimaTmrSts(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_ClimaTmrSts(self):
        pass

    @abc.abstractmethod
    def get_ClimaTmrStsRqrd(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_ClimaWarn(self, data):
        # type: (datatypes.ClimaWarn) -> None
        pass

    @abc.abstractmethod
    def stop_ClimaWarn(self):
        pass

    @abc.abstractmethod
    def get_CllsnAidSnvtySeld(self):
        # type: () -> (datatypes.SnvtyPen1)
        pass

    @abc.abstractmethod
    def get_CllsnFwdWarnActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_CllsnFwdWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_CllsnFwdWarnReq(self):
        pass

    @abc.abstractmethod
    def send_CllsnWarnReq(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_CllsnWarnReq(self):
        pass

    @abc.abstractmethod
    def send_CllsnWarnSide(self, data):
        # type: (datatypes.CllsnWarnSide1) -> None
        pass

    @abc.abstractmethod
    def stop_CllsnWarnSide(self):
        pass

    @abc.abstractmethod
    def send_ClngActv(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        pass

    @abc.abstractmethod
    def stop_ClngActv(self):
        pass

    @abc.abstractmethod
    def get_ClngRqrdFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        pass

    @abc.abstractmethod
    def get_ClsAutEna(self):
        # type: () -> datatypes.EnableDisableCoding
        pass

    @abc.abstractmethod
    def get_CmftFctActv(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_CmftFctSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_CmftFctSts(self):
        pass

    @abc.abstractmethod
    def send_CmptmtAirTEstimdExtd(self, data):
        # type: (datatypes.CmptmtAirTEstimdExtd) -> None
        pass

    @abc.abstractmethod
    def stop_CmptmtAirTEstimdExtd(self):
        pass

    @abc.abstractmethod
    def send_CmptmtTFrnt(self, data):
        # type: (datatypes.CmptmtTFrnt) -> None
        pass

    @abc.abstractmethod
    def stop_CmptmtTFrnt(self):
        pass

    @abc.abstractmethod
    def send_CnclWarnForCrsCtrl(self, data):
        # type: (datatypes.NoYesCrit1) -> None
        pass

    @abc.abstractmethod
    def stop_CnclWarnForCrsCtrl(self):
        pass

    @abc.abstractmethod
    def send_CnclWarnLgtForAutDrv(self, data):
        # type: (datatypes.NoYesCrit1) -> None
        pass

    @abc.abstractmethod
    def stop_CnclWarnLgtForAutDrv(self):
        pass

    @abc.abstractmethod
    def send_CnsPrm(self, data):
        # type: (datatypes.CnsPrmRec1) -> None
        pass

    @abc.abstractmethod
    def stop_CnsPrm(self):
        pass

    @abc.abstractmethod
    def send_ComLostExtrSound(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_ComLostExtrSound(self):
        pass

    @abc.abstractmethod
    def get_ConSftyDataSharingAllwd(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_ConSftyDataSharingSts(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_ConSftyDataSharingSts(self):
        pass

    @abc.abstractmethod
    def send_ConSftyWarn(self, data):
        # type: (datatypes.ConSftyWarn1) -> None
        pass

    @abc.abstractmethod
    def stop_ConSftyWarn(self):
        pass

    @abc.abstractmethod
    def get_CoolgReqForDispCen(self):
        # type: () -> datatypes.CoolgReqForDisp
        pass

    @abc.abstractmethod
    def send_CoolgStsForDisp(self, data):
        # type: (datatypes.CoolgStsForDisp) -> None
        pass

    @abc.abstractmethod
    def stop_CoolgStsForDisp(self):
        pass

    @abc.abstractmethod
    def send_CrsCtrlrSts(self, data):
        # type: (datatypes.CrsCtrlrSts1) -> None
        pass

    @abc.abstractmethod
    def stop_CrsCtrlrSts(self):
        pass

    @abc.abstractmethod
    def send_CrvtSpdAdpvSts(self, data):
        # type: (datatypes.OffOnNotAvlSrvRqrd) -> None
        pass

    @abc.abstractmethod
    def stop_CrvtSpdAdpvSts(self):
        pass

    @abc.abstractmethod
    def get_CrvtSpdEstimrSnvtySeld(self):
        # type: () -> (datatypes.SnvtyPen3)
        pass

    @abc.abstractmethod
    def get_CrvtSpdWarnActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_CrvtSpdWarnReq(self, data):
        # type: (datatypes.TurnSpdWarnReq1) -> None
        pass

    @abc.abstractmethod
    def stop_CrvtSpdWarnReq(self):
        pass

    @abc.abstractmethod
    def send_CrvtSpdWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_CrvtSpdWarnSts(self):
        pass

    @abc.abstractmethod
    def send_CtraIndcnLe(self, data):
        # type: (datatypes.CtraIndcn1) -> None
        pass

    @abc.abstractmethod
    def stop_CtraIndcnLe(self):
        pass

    @abc.abstractmethod
    def send_CtraIndcnRi(self, data):
        # type: (datatypes.CtraIndcn1) -> None
        pass

    @abc.abstractmethod
    def stop_CtraIndcnRi(self):
        pass

    @abc.abstractmethod
    def send_CtraOn1(self, data):
        # type: (datatypes.LcmaCtraOn) -> None
        pass

    @abc.abstractmethod
    def stop_CtraOn1(self):
        pass

    @abc.abstractmethod
    def get_CtraSwOnOff(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def get_CurtActvnReReq(self):
        # type: () -> datatypes.CurtActvnTyp1
        pass

    @abc.abstractmethod
    def get_DataDistbnAllwdForEgyPred(self):
        # type: () -> datatypes.OnOffNoReq
        pass

    @abc.abstractmethod
    def send_DataDistbnReqForEgyPred(self, data):
        # type: (datatypes.OnOffNoReq) -> None
        pass

    @abc.abstractmethod
    def stop_DataDistbnReqForEgyPred(self):
        pass

    @abc.abstractmethod
    def send_DataSpclDMSM(self, data):
        # type: (datatypes.DataSpclTyp) -> None
        pass

    @abc.abstractmethod
    def stop_DataSpclDMSM(self):
        pass

    @abc.abstractmethod
    def send_DayToSrv(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_DayToSrv(self):
        pass

    @abc.abstractmethod
    def get_DeactvtLvlgCtrl(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def get_DefrstDurgClimaEnad(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_DestSrcAndCfmRqrd(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_DestSrcAndCfmRqrd(self):
        pass

    @abc.abstractmethod
    def send_DiagcCCSM(self, data):
        # type: (datatypes.DiagcForPanCenCtrl2) -> None
        pass

    @abc.abstractmethod
    def stop_DiagcCCSM(self):
        pass

    @abc.abstractmethod
    def send_DiagcRCSM(self, data):
        # type: (datatypes.DiagcForRCSM2) -> None
        pass

    @abc.abstractmethod
    def stop_DiagcRCSM(self):
        pass

    @abc.abstractmethod
    def send_DiagcRSHC(self, data):
        # type: (datatypes.DiagcForRCSM2) -> None
        pass

    @abc.abstractmethod
    def stop_DiagcRSHC(self):
        pass

    @abc.abstractmethod
    def send_DiagcStsDMSM(self, data):
        # type: (datatypes.DiagcStsTypDMSM) -> None
        pass

    @abc.abstractmethod
    def stop_DiagcStsDMSM(self):
        pass

    @abc.abstractmethod
    def get_DispAndHomeBtnSts(self):
        # type: () -> datatypes.Trig1
        pass

    @abc.abstractmethod
    def send_DispBattEgyIn(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_DispBattEgyIn(self):
        pass

    @abc.abstractmethod
    def send_DispBattEgyOut(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_DispBattEgyOut(self):
        pass

    @abc.abstractmethod
    def send_DispOfPrpsnMod(self, data):
        # type: (datatypes.DispOfPrpsnMod3) -> None
        pass

    @abc.abstractmethod
    def stop_DispOfPrpsnMod(self):
        pass

    @abc.abstractmethod
    def get_DispStsCen(self):
        # type: () -> datatypes.FltCfmd1
        pass

    @abc.abstractmethod
    def send_DoorDrvrLockReSts(self, data):
        # type: (datatypes.LockSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DoorDrvrLockReSts(self):
        pass

    @abc.abstractmethod
    def send_DoorDrvrLockSts(self, data):
        # type: (datatypes.LockSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DoorDrvrLockSts(self):
        pass

    @abc.abstractmethod
    def send_DoorDrvrReSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DoorDrvrReSts(self):
        pass

    @abc.abstractmethod
    def send_DoorDrvrSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DoorDrvrSts(self):
        pass

    @abc.abstractmethod
    def send_DoorPassLockReSts(self, data):
        # type: (datatypes.LockSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DoorPassLockReSts(self):
        pass

    @abc.abstractmethod
    def send_DoorPassLockSts(self, data):
        # type: (datatypes.LockSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DoorPassLockSts(self):
        pass

    @abc.abstractmethod
    def send_DoorPassReSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DoorPassReSts(self):
        pass

    @abc.abstractmethod
    def send_DoorPassSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DoorPassSts(self):
        pass

    @abc.abstractmethod
    def get_DriveAwayInfoActvReq(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_DriveAwayInfoActvSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DriveAwayInfoActvSts(self):
        pass

    @abc.abstractmethod
    def get_DriveAwayInfoSoundWarnActvReq(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_DriveAwayInfoWarnReq(self, data):
        # type: (datatypes.DriveAwayInfoWarnReq1) -> None
        pass

    @abc.abstractmethod
    def stop_DriveAwayInfoWarnReq(self):
        pass

    @abc.abstractmethod
    def get_DrvModDispd(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def get_DrvModReq(self):
        # type: () -> datatypes.DrvModReqType1
        pass

    @abc.abstractmethod
    def get_DrvgInWrgDirOfTrfc(self):
        # type: () -> datatypes.NoYes1
        pass

    @abc.abstractmethod
    def get_DrvrCtrlOfPassSeatFrntReqd(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_DrvrCtrlOfPassSeatFrntSts(self, data):
        # type: (datatypes.ActvnAvl1) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrCtrlOfPassSeatFrntSts(self):
        pass

    @abc.abstractmethod
    def send_DrvrDesDir(self, data):
        # type: (datatypes.DrvrDesDir1) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrDesDir(self):
        pass

    @abc.abstractmethod
    def get_DrvrDispSetg(self):
        # type: () -> (datatypes.DrvrDispSetgReq)
        pass

    @abc.abstractmethod
    def get_DrvrEntryLoReq(self):
        # type: () -> datatypes.NoYes1
        pass

    @abc.abstractmethod
    def get_DrvrHmiBackGndForHmiCen(self):
        # type: () -> datatypes.DrvrHmiForHmiCen
        pass

    @abc.abstractmethod
    def get_DrvrHmiBackGndInfoSetg(self):
        # type: () -> (datatypes.DrvrHmiBackGndInfoSetg)
        pass

    @abc.abstractmethod
    def get_DrvrHmiDispdMod(self):
        # type: () -> (datatypes.DrvrHmiDispdModPen)
        pass

    @abc.abstractmethod
    def get_DrvrHmiForHmiCen(self):
        # type: () -> datatypes.DrvrHmiForHmiCen
        pass

    @abc.abstractmethod
    def send_DrvrHmiSpdLimAdpnSts(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrHmiSpdLimAdpnSts(self):
        pass

    @abc.abstractmethod
    def get_DrvrHmiTmrForHmiCen(self):
        # type: () -> datatypes.Array8ByteU8
        pass

    @abc.abstractmethod
    def get_DrvrHmiUsrIfSetg(self):
        # type: () -> (datatypes.DrvrHmiUserIfSetgReq)
        pass

    @abc.abstractmethod
    def send_DrvrMassgRunng(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrMassgRunng(self):
        pass

    @abc.abstractmethod
    def send_DrvrPfmncLvl(self, data):
        # type: (datatypes.DrvrPfmncLvl2) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrPfmncLvl(self):
        pass

    @abc.abstractmethod
    def get_DrvrPfmncMonActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_DrvrPfmncSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrPfmncSts(self):
        pass

    @abc.abstractmethod
    def send_DrvrPfmncWarnReq(self, data):
        # type: (datatypes.DrvrPfmncWarnReq1) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrPfmncWarnReq(self):
        pass

    @abc.abstractmethod
    def get_DrvrPfmncWarnReqToNav(self):
        # type: () -> datatypes.DrvrPfmncWarnReq1
        pass

    @abc.abstractmethod
    def send_DrvrSeatActvSpplFct(self, data):
        # type: (datatypes.SeatActvSpplFct1) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrSeatActvSpplFct(self):
        pass

    @abc.abstractmethod
    def get_DrvrSeatDispMassgFct(self):
        # type: () -> (datatypes.SeatMassgFct)
        pass

    @abc.abstractmethod
    def get_DrvrSeatDispSpplFct(self):
        # type: () -> datatypes.SeatActvSpplFct1
        pass

    @abc.abstractmethod
    def send_DrvrSeatMassgFct(self, data):
        # type: (datatypes.SeatMassgFct) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrSeatMassgFct(self):
        pass

    @abc.abstractmethod
    def send_DrvrSeatSwtSts(self, data):
        # type: (datatypes.DrvrSeatSwtSts1) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrSeatSwtSts(self):
        pass

    @abc.abstractmethod
    def send_DrvrWLoadLvl(self, data):
        # type: (datatypes.Lvl30) -> None
        pass

    @abc.abstractmethod
    def stop_DrvrWLoadLvl(self):
        pass

    @abc.abstractmethod
    def send_DstEstimdToEmptyForDrvgElec(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_DstEstimdToEmptyForDrvgElec(self):
        pass

    @abc.abstractmethod
    def send_DstEstimdToEmptyForDrvgElecEco(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_DstEstimdToEmptyForDrvgElecEco(self):
        pass

    @abc.abstractmethod
    def get_DstNotifActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_DstNotifSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_DstNotifSts(self):
        pass

    @abc.abstractmethod
    def send_DstToEmptyIndcd(self, data):
        # type: (datatypes.DstToEmptyWithUnit) -> None
        pass

    @abc.abstractmethod
    def stop_DstToEmptyIndcd(self):
        pass

    @abc.abstractmethod
    def get_DstToManvLocn(self):
        # type: () -> (datatypes.DstToManvLocnByNav)
        pass

    @abc.abstractmethod
    def send_DstToSrv(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_DstToSrv(self):
        pass

    @abc.abstractmethod
    def send_DstTrvld1(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_DstTrvld1(self):
        pass

    @abc.abstractmethod
    def send_DstTrvld2(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_DstTrvld2(self):
        pass

    @abc.abstractmethod
    def send_DstTrvldHiResl(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_DstTrvldHiResl(self):
        pass

    @abc.abstractmethod
    def send_BkpOfDstTrvld(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_BkpOfDstTrvld(self):
        pass

    @abc.abstractmethod
    def send_DstTrvldMst2(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_DstTrvldMst2(self):
        pass

    @abc.abstractmethod
    def get_EasyInOutDrvrSeatAdjmt(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_EgyCostForRoute(self, data):
        # type: (datatypes.EgyCostForRouteRec1) -> None
        pass

    @abc.abstractmethod
    def stop_EgyCostForRoute(self):
        pass

    @abc.abstractmethod
    def get_EgySave(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_EmgyAsscSts(self, data):
        # type: (datatypes.AsscSts) -> None
        pass

    @abc.abstractmethod
    def stop_EmgyAsscSts(self):
        pass

    @abc.abstractmethod
    def send_EmgyAsscStsConnGen2(self, data):
        # type: (datatypes.AsscSts) -> None
        pass

    @abc.abstractmethod
    def stop_EmgyAsscStsConnGen2(self):
        pass

    @abc.abstractmethod
    def get_EmgyVehWarnActv(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_EmgyVehWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_EmgyVehWarnSts(self):
        pass

    @abc.abstractmethod
    def send_EngCooltIndcnReq(self, data):
        # type: (datatypes.EngCooltIndcn) -> None
        pass

    @abc.abstractmethod
    def stop_EngCooltIndcnReq(self):
        pass

    @abc.abstractmethod
    def send_EngCooltLvl(self, data):
        # type: (datatypes.FldLvl) -> None
        pass

    @abc.abstractmethod
    def stop_EngCooltLvl(self):
        pass

    @abc.abstractmethod
    def send_EngFuCns(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_EngFuCns(self):
        pass

    @abc.abstractmethod
    def send_EngFuCnsFild(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_EngFuCnsFild(self):
        pass

    @abc.abstractmethod
    def send_EngHrToSrv(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_EngHrToSrv(self):
        pass

    @abc.abstractmethod
    def get_EngIdleEco1(self):
        # type: () -> (datatypes.EngIdleEco1)
        pass

    @abc.abstractmethod
    def send_EngNSafe(self, data):
        # type: (datatypes.EngNSafe) -> None
        pass

    @abc.abstractmethod
    def stop_EngNSafe(self):
        pass

    @abc.abstractmethod
    def send_EngOilLvl(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_EngOilLvl(self):
        pass

    @abc.abstractmethod
    def send_EngOilLvlSts(self, data):
        # type: (datatypes.EngOilLvlSts1) -> None
        pass

    @abc.abstractmethod
    def stop_EngOilLvlSts(self):
        pass

    @abc.abstractmethod
    def send_EngOilPWarn(self, data):
        # type: (datatypes.EngOilPWarn1) -> None
        pass

    @abc.abstractmethod
    def stop_EngOilPWarn(self):
        pass

    @abc.abstractmethod
    def send_EngSpdDispd(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_EngSpdDispd(self):
        pass

    @abc.abstractmethod
    def get_EngStrtStopDrvModIndSetg(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_EngStrtStopSetg(self):
        # type: () -> datatypes.OnOff2
        pass

    @abc.abstractmethod
    def get_EpbApplyAutSetg(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_EpbEngStallApplySetg(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_EpbLampReq(self, data):
        # type: (datatypes.EpbLampReqRec) -> None
        pass

    @abc.abstractmethod
    def stop_EpbLampReq(self):
        pass

    @abc.abstractmethod
    def get_EscSptModReqdByDrvr(self):
        # type: () -> (datatypes.EscSptModReqdByDrvrRec1)
        pass

    @abc.abstractmethod
    def send_EscSt(self, data):
        # type: (datatypes.EscStSafe1) -> None
        pass

    @abc.abstractmethod
    def stop_EscSt(self):
        pass

    @abc.abstractmethod
    def send_EscWarnIndcnReq(self, data):
        # type: (datatypes.EscWarnIndcnReqRec1) -> None
        pass

    @abc.abstractmethod
    def stop_EscWarnIndcnReq(self):
        pass

    @abc.abstractmethod
    def get_ExtrMirrFoldSetg(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_ExtrMirrTiltSetg2(self):
        # type: () -> (datatypes.ExtrMirrTiltSetg)
        pass

    @abc.abstractmethod
    def get_ExtrMirrTintgSetg(self):
        # type: () -> (datatypes.MirrDimPen)
        pass

    @abc.abstractmethod
    def send_FRNetworkStatus(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_FRNetworkStatus(self):
        pass

    @abc.abstractmethod
    def send_FltIndcrTurnLeFrnt(self, data):
        # type: (datatypes.DevErrSts2) -> None
        pass

    @abc.abstractmethod
    def stop_FltIndcrTurnLeFrnt(self):
        pass

    @abc.abstractmethod
    def send_FltIndcrTurnLeRe(self, data):
        # type: (datatypes.DevErrSts2) -> None
        pass

    @abc.abstractmethod
    def stop_FltIndcrTurnLeRe(self):
        pass

    @abc.abstractmethod
    def send_FltIndcrTurnRiFrnt(self, data):
        # type: (datatypes.DevErrSts2) -> None
        pass

    @abc.abstractmethod
    def stop_FltIndcrTurnRiFrnt(self):
        pass

    @abc.abstractmethod
    def send_FltIndcrTurnRiRe(self, data):
        # type: (datatypes.DevErrSts2) -> None
        pass

    @abc.abstractmethod
    def stop_FltIndcrTurnRiRe(self):
        pass

    @abc.abstractmethod
    def send_FltOfLiDaytiRunngRi(self, data):
        # type: (datatypes.DevErrSts2) -> None
        pass

    @abc.abstractmethod
    def stop_FltOfLiDaytiRunngRi(self):
        pass

    @abc.abstractmethod
    def send_FrntAxleWarn(self, data):
        # type: (datatypes.AxleWarn) -> None
        pass

    @abc.abstractmethod
    def stop_FrntAxleWarn(self):
        pass

    @abc.abstractmethod
    def send_FrntWiprLvrReq2(self, data):
        # type: (datatypes.FrntWiprLvrReq2) -> None
        pass

    @abc.abstractmethod
    def stop_FrntWiprLvrReq2(self):
        pass

    @abc.abstractmethod
    def send_FuHeatrActv(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_FuHeatrActv(self):
        pass

    @abc.abstractmethod
    def send_FuHeatrFuCns1(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_FuHeatrFuCns1(self):
        pass

    @abc.abstractmethod
    def send_FuHeatrFuCnsDurgCyc1(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_FuHeatrFuCnsDurgCyc1(self):
        pass

    @abc.abstractmethod
    def send_FuLvlIndcd(self, data):
        # type: (datatypes.FuLvlValWithQly) -> None
        pass

    @abc.abstractmethod
    def stop_FuLvlIndcd(self):
        pass

    @abc.abstractmethod
    def send_FuLvlLoIndcn(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_FuLvlLoIndcn(self):
        pass

    @abc.abstractmethod
    def get_FuLvlLoIndcnToNav(self):
        # type: () -> datatypes.FuLvlLoIndcnToNav1
        pass

    @abc.abstractmethod
    def send_FuLvlLoWarn(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_FuLvlLoWarn(self):
        pass

    @abc.abstractmethod
    def send_GearIndcnRec(self, data):
        # type: (datatypes.GearIndcnRec2) -> None
        pass

    @abc.abstractmethod
    def stop_GearIndcnRec(self):
        pass

    @abc.abstractmethod
    def get_GlbRstForSetgAndData(self):
        # type: () -> (datatypes.GlbRstForSetgAndData)
        pass

    @abc.abstractmethod
    def get_HdHwAprvdWirelsAdr(self):
        # type: () -> (datatypes.HwAprvdWirelsAdr1)
        pass

    @abc.abstractmethod
    def get_HdrestFoldReq2(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def get_HeatrDurgDrvgReqd(self):
        # type: () -> datatypes.OffOnAut1
        pass

    @abc.abstractmethod
    def get_HeatrPreCdngTyp(self):
        # type: () -> datatypes.HeatrPreCdngTyp
        pass

    @abc.abstractmethod
    def get_HmiAudSts(self):
        # type: () -> datatypes.FltCfmd1
        pass

    @abc.abstractmethod
    def send_HmiCenForDrvrHmi(self, data):
        # type: (datatypes.HmiCenForDrvrHmi) -> None
        pass

    @abc.abstractmethod
    def stop_HmiCenForDrvrHmi(self):
        pass

    @abc.abstractmethod
    def get_HmiCmptmtAirDistbnFrnt(self):
        # type: () -> datatypes.HmiCmptmtAirDistbnFrnt
        pass

    @abc.abstractmethod
    def get_HmiCmptmtCoolgReq(self):
        # type: () -> datatypes.HmiCmptmtCoolgReq
        pass

    @abc.abstractmethod
    def get_HmiCmptmtTSp(self):
        # type: () -> (datatypes.HmiCmptmtTSp)
        pass

    @abc.abstractmethod
    def get_HmiDefrstElecReq(self):
        # type: () -> (datatypes.HmiDefrstElecReq)
        pass

    @abc.abstractmethod
    def send_HmiDefrstElecSts(self, data):
        # type: (datatypes.HmiDefrstElecSts) -> None
        pass

    @abc.abstractmethod
    def stop_HmiDefrstElecSts(self):
        pass

    @abc.abstractmethod
    def get_HmiDefrstMaxReq(self):
        # type: () -> datatypes.ActrReq
        pass

    @abc.abstractmethod
    def get_HmiHvacFanLvlFrnt(self):
        # type: () -> datatypes.HmiHvacFanLvl
        pass

    @abc.abstractmethod
    def get_HmiHvacFanLvlRe(self):
        # type: () -> datatypes.HmiHvacFanLvl
        pass

    @abc.abstractmethod
    def get_HmiHvacReCtrl(self):
        # type: () -> datatypes.HmiHvacReCtrl
        pass

    @abc.abstractmethod
    def get_HmiHvacRecircCmd(self):
        # type: () -> datatypes.HmiHvacRecircCmd
        pass

    @abc.abstractmethod
    def get_HmiSeatClima(self):
        # type: () -> (datatypes.HmiSeatClima)
        pass

    @abc.abstractmethod
    def get_HmiSeatClimaExtd(self):
        # type: () -> (datatypes.HmiSeatClimaExtd)
        pass

    @abc.abstractmethod
    def send_HoodSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        pass

    @abc.abstractmethod
    def stop_HoodSts(self):
        pass

    @abc.abstractmethod
    def get_HptcWarnSeldForCllsnFwdWarn(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_HudActvReq(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_HudActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_HudActvSts(self):
        pass

    @abc.abstractmethod
    def get_HudAdjmtReq(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_HudDiagc(self, data):
        # type: (datatypes.HudDiagc) -> None
        pass

    @abc.abstractmethod
    def stop_HudDiagc(self):
        pass

    @abc.abstractmethod
    def get_HudErgoSetgReq(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_HudSts(self, data):
        # type: (datatypes.HudStsForHmi) -> None
        pass

    @abc.abstractmethod
    def stop_HudSts(self):
        pass

    @abc.abstractmethod
    def get_HudVisFctSetg(self):
        # type: () -> (datatypes.HudVisFctSetgReq)
        pass

    @abc.abstractmethod
    def send_HvBattEgyAvlDcha3(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_HvBattEgyAvlDcha3(self):
        pass

    @abc.abstractmethod
    def send_HvBattSmtActv(self, data):
        # type: (datatypes.Flg1) -> None
        pass

    @abc.abstractmethod
    def stop_HvBattSmtActv(self):
        pass

    @abc.abstractmethod
    def get_HvBattSmtSeld(self):
        # type: () -> datatypes.OnOffNoReq
        pass

    @abc.abstractmethod
    def send_HvacAirMFlowEstimd(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_HvacAirMFlowEstimd(self):
        pass

    @abc.abstractmethod
    def get_HznData(self):
        # type: () -> (datatypes.HznDataGroup2)
        pass

    @abc.abstractmethod
    def get_HznEdge(self):
        # type: () -> (datatypes.HznEdgeGroup2)
        pass

    @abc.abstractmethod
    def get_HznPosn(self):
        # type: () -> (datatypes.HznPosnGroup3)
        pass

    @abc.abstractmethod
    def get_HznPosnExtd(self):
        # type: () -> (datatypes.HznPosnExtdGroup1)
        pass

    @abc.abstractmethod
    def get_HznPosnExtdOffs(self):
        # type: () -> (datatypes.HznPosnExtdOffs)
        pass

    @abc.abstractmethod
    def get_HznProfLong(self):
        # type: () -> (datatypes.HznProfLongGroup3)
        pass

    @abc.abstractmethod
    def get_HznProfLongExtd(self):
        # type: () -> (datatypes.HznProfLongExtdGroup1)
        pass

    @abc.abstractmethod
    def get_HznProfSho(self):
        # type: () -> (datatypes.HznProfSho2)
        pass

    @abc.abstractmethod
    def send_HznRstExtd(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_HznRstExtd(self):
        pass

    @abc.abstractmethod
    def get_HznSeg(self):
        # type: () -> (datatypes.HznSegGroup2)
        pass

    @abc.abstractmethod
    def get_HznSplyElectcSts(self):
        # type: () -> datatypes.HznSplyElectcSts1
        pass

    @abc.abstractmethod
    def get_HzrdLiWarnActv(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_HzrdLiWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_HzrdLiWarnSts(self):
        pass

    @abc.abstractmethod
    def send_IndcnOfParkAssiSts(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_IndcnOfParkAssiSts(self):
        pass

    @abc.abstractmethod
    def send_IndcnOfPrkgAutSts(self, data):
        # type: (datatypes.IndOfPrkgAutSts) -> None
        pass

    @abc.abstractmethod
    def stop_IndcnOfPrkgAutSts(self):
        pass

    @abc.abstractmethod
    def get_IndcnUnit(self):
        # type: () -> (datatypes.IndcnUnit)
        pass

    @abc.abstractmethod
    def send_IndcrDisp1WdSts(self, data):
        # type: (datatypes.IndcrSts1) -> None
        pass

    @abc.abstractmethod
    def stop_IndcrDisp1WdSts(self):
        pass

    @abc.abstractmethod
    def send_IndcrTurnSts1WdSts(self, data):
        # type: (datatypes.IndcrSts1) -> None
        pass

    @abc.abstractmethod
    def stop_IndcrTurnSts1WdSts(self):
        pass

    @abc.abstractmethod
    def send_IniValSigCfgIDBackboneFR(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_IniValSigCfgIDBackboneFR(self):
        pass

    @abc.abstractmethod
    def get_InsdCarNoiseMeasd(self):
        # type: () -> (float)
        pass

    @abc.abstractmethod
    def get_IntAudCnclWarn(self):
        # type: () -> datatypes.YesNo2
        pass

    @abc.abstractmethod
    def send_IntrBriSts(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_IntrBriSts(self):
        pass

    @abc.abstractmethod
    def get_IntrBriStsForSeatHeatrRe(self):
        # type: () -> (int)
        pass

    @abc.abstractmethod
    def get_IntrLiAmbLiSetg(self):
        # type: () -> (datatypes.IntrLiAmbLiSetg)
        pass

    @abc.abstractmethod
    def get_IntrLiSurrndgsLiSetgLi(self):
        # type: () -> (datatypes.IntrLiSurrndgsLiSetg)
        pass

    @abc.abstractmethod
    def get_IntrMirrTintgSetg(self):
        # type: () -> (datatypes.MirrDimPen)
        pass

    @abc.abstractmethod
    def get_IntvAndWarnModForLaneKeepAid(self):
        # type: () -> (datatypes.WarnAndIntvPen1)
        pass

    @abc.abstractmethod
    def send_KeyLostWarnIndcn(self, data):
        # type: (datatypes.KeyLostWarnIndcn) -> None
        pass

    @abc.abstractmethod
    def stop_KeyLostWarnIndcn(self):
        pass

    @abc.abstractmethod
    def get_KeyProfMpgUpd(self):
        # type: () -> (datatypes.KeyProfMpgUpd1)
        pass

    @abc.abstractmethod
    def get_KeyReadReqFromSetgMgr(self):
        # type: () -> datatypes.KeyLocn1
        pass

    @abc.abstractmethod
    def send_KeyReadStsToProfCtrl(self, data):
        # type: (datatypes.KeyReadStsToProfCtrl) -> None
        pass

    @abc.abstractmethod
    def stop_KeyReadStsToProfCtrl(self):
        pass

    @abc.abstractmethod
    def send_KeyRmnIndcn(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_KeyRmnIndcn(self):
        pass

    @abc.abstractmethod
    def send_KeySpdWarn(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_KeySpdWarn(self):
        pass

    @abc.abstractmethod
    def get_LampSuppSrv(self):
        # type: () -> datatypes.SrvSts
        pass

    @abc.abstractmethod
    def get_LaneChgWarnActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_LaneChgWarnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_LaneChgWarnSts(self):
        pass

    @abc.abstractmethod
    def send_LaneDetnStsForAutDrv(self, data):
        # type: (datatypes.LaneDetnSts1) -> None
        pass

    @abc.abstractmethod
    def stop_LaneDetnStsForAutDrv(self):
        pass

    @abc.abstractmethod
    def send_LaneDetnStsForLaneKeepAid(self, data):
        # type: (datatypes.LaneDetnSts1) -> None
        pass

    @abc.abstractmethod
    def stop_LaneDetnStsForLaneKeepAid(self):
        pass

    @abc.abstractmethod
    def get_LaneKeepAidActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_LaneKeepAidRoadEdgeActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_LaneKeepAidSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_LaneKeepAidSts(self):
        pass

    @abc.abstractmethod
    def send_LatMovmtWarn(self, data):
        # type: (datatypes.WarnLeRi1) -> None
        pass

    @abc.abstractmethod
    def stop_LatMovmtWarn(self):
        pass

    @abc.abstractmethod
    def send_LcmaOn1(self, data):
        # type: (datatypes.LcmaCtraOn) -> None
        pass

    @abc.abstractmethod
    def stop_LcmaOn1(self):
        pass

    @abc.abstractmethod
    def get_LiExtReq1WdReq1(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_LiExtReq2WdReq1(self):
        # type: () -> (datatypes.LiAutTranPen1)
        pass

    @abc.abstractmethod
    def get_LiExtReq1WdReq4(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_LiExtReq1WdReq6(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_LiExtReq1WdReq2(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_LiDrvrFltIndcrTurn(self, data):
        # type: (datatypes.DevErrSts2) -> None
        pass

    @abc.abstractmethod
    def stop_LiDrvrFltIndcrTurn(self):
        pass

    @abc.abstractmethod
    def get_LiForBtn4ForUsrSwtPanFrntCmd(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def get_LiForBtn5ForUsrSwtPanFrntCmd(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def get_LiExtSafe1WdReq2(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_LiExtSafe1WdReq1(self):
        # type: () -> (datatypes.LiTiPen2)
        pass

    @abc.abstractmethod
    def send_LiLvrSwt1Req(self, data):
        # type: (datatypes.SwtPush) -> None
        pass

    @abc.abstractmethod
    def stop_LiLvrSwt1Req(self):
        pass

    @abc.abstractmethod
    def send_LiPassFltIndcrTurn(self, data):
        # type: (datatypes.DevErrSts2) -> None
        pass

    @abc.abstractmethod
    def stop_LiPassFltIndcrTurn(self):
        pass

    @abc.abstractmethod
    def get_LiSeldForDrvrPfmncMon(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_LiTrfcSide1WdReq1(self):
        # type: () -> datatypes.LiTrfcSide1
        pass

    @abc.abstractmethod
    def send_ListOfNodAv(self, data):
        # type: (datatypes.ListOfNodAv) -> None
        pass

    @abc.abstractmethod
    def stop_ListOfNodAv(self):
        pass

    @abc.abstractmethod
    def get_LockSpdReq(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_LockgCenSts(self, data):
        # type: (datatypes.LockgCenSts3) -> None
        pass

    @abc.abstractmethod
    def stop_LockgCenSts(self):
        pass

    @abc.abstractmethod
    def send_LockgCenStsForUsrFb(self, data):
        # type: (datatypes.LockSt2) -> None
        pass

    @abc.abstractmethod
    def stop_LockgCenStsForUsrFb(self):
        pass

    @abc.abstractmethod
    def get_LockgFbSoundReq(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_LockgFbVisReq(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_LockgPrsnlReqFromHmi(self):
        # type: () -> datatypes.OnOffNoReq
        pass

    @abc.abstractmethod
    def send_LockgPrsnlSts(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_LockgPrsnlSts(self):
        pass

    @abc.abstractmethod
    def send_LvlOfClimaCmft(self, data):
        # type: (datatypes.LvlOfClimaCmft) -> None
        pass

    @abc.abstractmethod
    def stop_LvlOfClimaCmft(self):
        pass

    @abc.abstractmethod
    def get_LvlSeldForSpdLimAdpv(self):
        # type: () -> (datatypes.LvlSeldForSpdLimAdpvPen1)
        pass

    @abc.abstractmethod
    def get_MassgFctActv(self):
        # type: () -> (datatypes.MassgFctActv)
        pass

    @abc.abstractmethod
    def send_MemBtnSound(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_MemBtnSound(self):
        pass

    @abc.abstractmethod
    def send_MirrDwnStsAtDrvr(self, data):
        # type: (datatypes.MirrDwnStsTyp) -> None
        pass

    @abc.abstractmethod
    def stop_MirrDwnStsAtDrvr(self):
        pass

    @abc.abstractmethod
    def send_MirrDwnStsAtPass(self, data):
        # type: (datatypes.MirrDwnStsTyp) -> None
        pass

    @abc.abstractmethod
    def stop_MirrDwnStsAtPass(self):
        pass

    @abc.abstractmethod
    def send_MirrFoldStsAtDrvr(self, data):
        # type: (datatypes.MirrFoldStsTyp) -> None
        pass

    @abc.abstractmethod
    def stop_MirrFoldStsAtDrvr(self):
        pass

    @abc.abstractmethod
    def send_MirrFoldStsAtPass(self, data):
        # type: (datatypes.MirrFoldStsTyp) -> None
        pass

    @abc.abstractmethod
    def stop_MirrFoldStsAtPass(self):
        pass

    @abc.abstractmethod
    def get_MmedHdPwrMod(self):
        # type: () -> datatypes.MmedMaiPwrMod
        pass

    @abc.abstractmethod
    def get_MmedHmiModStd(self):
        # type: () -> datatypes.MmedHmiModStd2
        pass

    @abc.abstractmethod
    def get_MmedTvmPwerMod(self):
        # type: () -> datatypes.MmedTvmPwerMod
        pass

    @abc.abstractmethod
    def send_MstCfgIDBackboneFR(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_MstCfgIDBackboneFR(self):
        pass

    @abc.abstractmethod
    def send_MtrlSnsrT(self, data):
        # type: (datatypes.MtrlSnsrT) -> None
        pass

    @abc.abstractmethod
    def stop_MtrlSnsrT(self):
        pass

    @abc.abstractmethod
    def send_NFSDataFront(self, data):
        # type: (datatypes.SnsrPrkgAssi3) -> None
        pass

    @abc.abstractmethod
    def stop_NFSDataFront(self):
        pass

    @abc.abstractmethod
    def send_NFSDataRear(self, data):
        # type: (datatypes.SnsrPrkgAssi3) -> None
        pass

    @abc.abstractmethod
    def stop_NFSDataRear(self):
        pass

    @abc.abstractmethod
    def send_NetCtrlrActvt(self, data):
        # type: (datatypes.NetActvtRec1) -> None
        pass

    @abc.abstractmethod
    def stop_NetCtrlrActvt(self):
        pass

    @abc.abstractmethod
    def get_NetHdActvt(self):
        # type: () -> (datatypes.NetActvtRec1)
        pass

    @abc.abstractmethod
    def send_NetTelmActvt(self, data):
        # type: (datatypes.NetActvtRec1) -> None
        pass

    @abc.abstractmethod
    def stop_NetTelmActvt(self):
        pass

    @abc.abstractmethod
    def send_NewTripCdn(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_NewTripCdn(self):
        pass

    @abc.abstractmethod
    def send_NoEntryWarnReq(self, data):
        # type: (datatypes.Warn2) -> None
        pass

    @abc.abstractmethod
    def stop_NoEntryWarnReq(self):
        pass

    @abc.abstractmethod
    def get_NoSoundSys(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def send_NotifChkDistbn(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_NotifChkDistbn(self):
        pass

    @abc.abstractmethod
    def send_NrOfBltAppld(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_NrOfBltAppld(self):
        pass

    @abc.abstractmethod
    def send_NrOfKeyAvl(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_NrOfKeyAvl(self):
        pass

    @abc.abstractmethod
    def send_NrSerlDMSM(self, data):
        # type: (datatypes.NrSerlNodLIN) -> None
        pass

    @abc.abstractmethod
    def stop_NrSerlDMSM(self):
        pass

    @abc.abstractmethod
    def get_OffsForDrvrSpprtFctActvSts(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def get_OffsForSpdWarnSetg(self):
        # type: () -> (datatypes.OffsForSpdWarnSetgPen)
        pass

    @abc.abstractmethod
    def send_PartNrDMSM(self, data):
        # type: (datatypes.PartNrNodLIN) -> None
        pass

    @abc.abstractmethod
    def stop_PartNrDMSM(self):
        pass

    @abc.abstractmethod
    def get_PasAlrmDeactvnReq(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_PasAlrmSts(self, data):
        # type: (datatypes.NoReqOffReq) -> None
        pass

    @abc.abstractmethod
    def stop_PasAlrmSts(self):
        pass

    @abc.abstractmethod
    def send_PassMassgRunng(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_PassMassgRunng(self):
        pass

    @abc.abstractmethod
    def send_PassSeatActvSpplFct(self, data):
        # type: (datatypes.SeatActvSpplFct1) -> None
        pass

    @abc.abstractmethod
    def stop_PassSeatActvSpplFct(self):
        pass

    @abc.abstractmethod
    def get_PassSeatDispMassgFct(self):
        # type: () -> (datatypes.SeatMassgFct)
        pass

    @abc.abstractmethod
    def get_PassSeatDispSpplFct(self):
        # type: () -> datatypes.SeatActvSpplFct1
        pass

    @abc.abstractmethod
    def send_PassSeatMassgFct(self, data):
        # type: (datatypes.SeatMassgFct) -> None
        pass

    @abc.abstractmethod
    def stop_PassSeatMassgFct(self):
        pass

    @abc.abstractmethod
    def send_PassSeatSts(self, data):
        # type: (datatypes.PassSeatSts1) -> None
        pass

    @abc.abstractmethod
    def stop_PassSeatSts(self):
        pass

    @abc.abstractmethod
    def send_PassSeatSwtSts2(self, data):
        # type: (datatypes.PassSeatSwtSts2) -> None
        pass

    @abc.abstractmethod
    def stop_PassSeatSwtSts2(self):
        pass

    @abc.abstractmethod
    def send_PinionSteerAg1(self, data):
        # type: (datatypes.PinionSteerAg1Rec) -> None
        pass

    @abc.abstractmethod
    def stop_PinionSteerAg1(self):
        pass

    @abc.abstractmethod
    def get_PosnFromNav(self):
        # type: () -> (datatypes.PosnFromNav)
        pass

    @abc.abstractmethod
    def send_PosnFromSatlt(self, data):
        # type: (datatypes.PosnFromSatlt) -> None
        pass

    @abc.abstractmethod
    def stop_PosnFromSatlt(self):
        pass

    @abc.abstractmethod
    def send_PostDrvgClimaAvl(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_PostDrvgClimaAvl(self):
        pass

    @abc.abstractmethod
    def get_PostDrvgClimaReq(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_PreClngNotif(self, data):
        # type: (datatypes.PreClngNotif) -> None
        pass

    @abc.abstractmethod
    def stop_PreClngNotif(self):
        pass

    @abc.abstractmethod
    def send_PrkgAssiActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgAssiActvSts(self):
        pass

    @abc.abstractmethod
    def send_PrkgAssiAudWarnFront(self, data):
        # type: (datatypes.PrkgAssiAudWarn) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgAssiAudWarnFront(self):
        pass

    @abc.abstractmethod
    def send_PrkgAssiAudWarnRear(self, data):
        # type: (datatypes.PrkgAssiAudWarn) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgAssiAudWarnRear(self):
        pass

    @abc.abstractmethod
    def send_PrkgAssiFailr(self, data):
        # type: (datatypes.PrkgAssiFailr2) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgAssiFailr(self):
        pass

    @abc.abstractmethod
    def send_PrkgAssiManvActvSts(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgAssiManvActvSts(self):
        pass

    @abc.abstractmethod
    def send_PrkgAssiManvProgs(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgAssiManvProgs(self):
        pass

    @abc.abstractmethod
    def send_PrkgAssiSts(self, data):
        # type: (datatypes.PrkgAssiSts3) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgAssiSts(self):
        pass

    @abc.abstractmethod
    def send_PrkgAutSts(self, data):
        # type: (datatypes.PrkgAutSts3) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgAutSts(self):
        pass

    @abc.abstractmethod
    def send_PrkgCamSysAvlSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgCamSysAvlSts(self):
        pass

    @abc.abstractmethod
    def get_PrkgOutCfm(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_PrkgTypVld(self, data):
        # type: (datatypes.PrkgTypVld1) -> None
        pass

    @abc.abstractmethod
    def stop_PrkgTypVld(self):
        pass

    @abc.abstractmethod
    def send_ProfAct(self, data):
        # type: (datatypes.ProfAct1) -> None
        pass

    @abc.abstractmethod
    def stop_ProfAct(self):
        pass

    @abc.abstractmethod
    def get_ProfChg(self):
        # type: () -> datatypes.IdPen
        pass

    @abc.abstractmethod
    def send_ProfLimd(self, data):
        # type: (datatypes.ProfLimd1) -> None
        pass

    @abc.abstractmethod
    def stop_ProfLimd(self):
        pass

    @abc.abstractmethod
    def send_ProfPenSts1(self, data):
        # type: (datatypes.IdPen) -> None
        pass

    @abc.abstractmethod
    def stop_ProfPenSts1(self):
        pass

    @abc.abstractmethod
    def send_PrpsnDrvMod(self, data):
        # type: (datatypes.PrpsnDrvMod) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnDrvMod(self):
        pass

    @abc.abstractmethod
    def send_PrpsnHvBattUsgModAct(self, data):
        # type: (datatypes.HvBattUsgType) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnHvBattUsgModAct(self):
        pass

    @abc.abstractmethod
    def get_PrpsnHvBattUsgModReq(self):
        # type: () -> datatypes.HvBattUsgType
        pass

    @abc.abstractmethod
    def send_PrpsnHvBattUsgOfChrgBlkd(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnHvBattUsgOfChrgBlkd(self):
        pass

    @abc.abstractmethod
    def send_PrpsnHvBattUsgOfChrgBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnHvBattUsgOfChrgBlkd2(self):
        pass

    @abc.abstractmethod
    def send_PrpsnHvBattUsgOfHldBlkd(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnHvBattUsgOfHldBlkd(self):
        pass

    @abc.abstractmethod
    def send_PrpsnHvBattUsgOfHldBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnHvBattUsgOfHldBlkd2(self):
        pass

    @abc.abstractmethod
    def send_PrpsnHvBattUsgOfHldSmtBlkd(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnHvBattUsgOfHldSmtBlkd(self):
        pass

    @abc.abstractmethod
    def send_PrpsnHvBattUsgOfHldSmtBlkd2(self, data):
        # type: (datatypes.HvBattUsgBlkdTyp) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnHvBattUsgOfHldSmtBlkd2(self):
        pass

    @abc.abstractmethod
    def get_PrpsnHvBattUsgOfHldSpd(self):
        # type: () -> (int)
        pass

    @abc.abstractmethod
    def get_PrpsnHvBattUsgStsDispd(self):
        # type: () -> (bool)
        pass

    @abc.abstractmethod
    def send_PrpsnModElecDrvBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnModElecDrvBlkd(self):
        pass

    @abc.abstractmethod
    def send_PrpsnModOfSaveBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnModOfSaveBlkd(self):
        pass

    @abc.abstractmethod
    def send_PrpsnModOfTracBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnModOfTracBlkd(self):
        pass

    @abc.abstractmethod
    def send_PrpsnModOffroadBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnModOffroadBlkd(self):
        pass

    @abc.abstractmethod
    def send_PrpsnModSptBlkd(self, data):
        # type: (datatypes.Typ1) -> None
        pass

    @abc.abstractmethod
    def stop_PrpsnModSptBlkd(self):
        pass

    @abc.abstractmethod
    def send_PtCluTq(self, data):
        # type: (datatypes.PtCluTq1) -> None
        pass

    @abc.abstractmethod
    def stop_PtCluTq(self):
        pass

    @abc.abstractmethod
    def get_PtDrvrSetg(self):
        # type: () -> (datatypes.PtDrvrSetg2)
        pass

    @abc.abstractmethod
    def send_PtGearTar(self, data):
        # type: (datatypes.PtGearAct1) -> None
        pass

    @abc.abstractmethod
    def stop_PtGearTar(self):
        pass

    @abc.abstractmethod
    def send_PtTqAtWhlFrntAct(self, data):
        # type: (datatypes.PtTqAtWhlFrntActRec1) -> None
        pass

    @abc.abstractmethod
    def stop_PtTqAtWhlFrntAct(self):
        pass

    @abc.abstractmethod
    def send_PwrChrgDetdForPrkgHeatrElec(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_PwrChrgDetdForPrkgHeatrElec(self):
        pass

    @abc.abstractmethod
    def send_PwrSplyErrSts(self, data):
        # type: (datatypes.PwrSplyErrSts1) -> None
        pass

    @abc.abstractmethod
    def stop_PwrSplyErrSts(self):
        pass

    @abc.abstractmethod
    def get_RadioFrqAn(self):
        # type: () -> (int)
        pass

    @abc.abstractmethod
    def get_RainSenMemdReq(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_ReAxleWarn(self, data):
        # type: (datatypes.AxleWarn) -> None
        pass

    @abc.abstractmethod
    def stop_ReAxleWarn(self):
        pass

    @abc.abstractmethod
    def send_RlyPwrDistbnCmd1WdBattSaveCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_RlyPwrDistbnCmd1WdBattSaveCmd(self):
        pass

    @abc.abstractmethod
    def send_RlyPwrDistbnCmd1WdIgnRlyCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_RlyPwrDistbnCmd1WdIgnRlyCmd(self):
        pass

    @abc.abstractmethod
    def send_RlyPwrDistbnCmd1WdIgnRlyExtCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_RlyPwrDistbnCmd1WdIgnRlyExtCmd(self):
        pass

    @abc.abstractmethod
    def send_RlyPwrDistbnCmd1WdPreBattSaveCmd(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_RlyPwrDistbnCmd1WdPreBattSaveCmd(self):
        pass

    @abc.abstractmethod
    def send_RmnLockgPrsnlReq(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_RmnLockgPrsnlReq(self):
        pass

    @abc.abstractmethod
    def get_RngbdIllmnCmd(self):
        # type: () -> (datatypes.RngBdIllmnCmdPen1)
        pass

    @abc.abstractmethod
    def get_RoadFricIndcnActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_RoadFricIndcnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_RoadFricIndcnSts(self):
        pass

    @abc.abstractmethod
    def send_RoadFricWarnReq(self, data):
        # type: (datatypes.RoadFricWarnReq1) -> None
        pass

    @abc.abstractmethod
    def stop_RoadFricWarnReq(self):
        pass

    @abc.abstractmethod
    def get_RoadSgnInfoActv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_RoadSgnInfoSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_RoadSgnInfoSts(self):
        pass

    @abc.abstractmethod
    def get_RoadSpdLimActvSts(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def get_RoadUsrProtnActv(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_RoadUsrProtnSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_RoadUsrProtnSts(self):
        pass

    @abc.abstractmethod
    def send_RotyDirReq2(self, data):
        # type: (datatypes.RotyDirUI) -> None
        pass

    @abc.abstractmethod
    def stop_RotyDirReq2(self):
        pass

    @abc.abstractmethod
    def send_RotyDirReq1(self, data):
        # type: (datatypes.RotyDirUI1) -> None
        pass

    @abc.abstractmethod
    def stop_RotyDirReq1(self):
        pass

    @abc.abstractmethod
    def send_RotyPosReq2(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_RotyPosReq2(self):
        pass

    @abc.abstractmethod
    def send_RotyPosReq1(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_RotyPosReq1(self):
        pass

    @abc.abstractmethod
    def get_RouteInfo(self):
        # type: () -> (datatypes.RouteInfoRec1)
        pass

    @abc.abstractmethod
    def send_RsdsSysStsLe(self, data):
        # type: (datatypes.RsdsSysSts) -> None
        pass

    @abc.abstractmethod
    def stop_RsdsSysStsLe(self):
        pass

    @abc.abstractmethod
    def send_RsdsSysStsRi(self, data):
        # type: (datatypes.RsdsSysSts) -> None
        pass

    @abc.abstractmethod
    def stop_RsdsSysStsRi(self):
        pass

    @abc.abstractmethod
    def get_SaveSetgToMemPrmnt(self):
        # type: () -> datatypes.OffOnAut1
        pass

    @abc.abstractmethod
    def send_ScrBarVolIndcn(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_ScrBarVolIndcn(self):
        pass

    @abc.abstractmethod
    def send_ScrMaxFillgVol(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_ScrMaxFillgVol(self):
        pass

    @abc.abstractmethod
    def send_ScrReagentTankVol(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_ScrReagentTankVol(self):
        pass

    @abc.abstractmethod
    def send_SeatBackUnlckdThrd(self, data):
        # type: (datatypes.SeatBackUnlckd) -> None
        pass

    @abc.abstractmethod
    def stop_SeatBackUnlckdThrd(self):
        pass

    @abc.abstractmethod
    def get_SeatDispBtnPsd(self):
        # type: () -> (datatypes.SeatDispBtnPsd)
        pass

    @abc.abstractmethod
    def get_SeatHeatDurgClimaEnad(self):
        # type: () -> datatypes.SeatHeatDurgClimaEnad
        pass

    @abc.abstractmethod
    def send_SeatHeatgAutCdn(self, data):
        # type: (datatypes.Flg1) -> None
        pass

    @abc.abstractmethod
    def stop_SeatHeatgAutCdn(self):
        pass

    @abc.abstractmethod
    def get_SeatSwtLeSigThrd(self):
        # type: () -> datatypes.SwtVertSts1
        pass

    @abc.abstractmethod
    def get_SeatSwtRiSigThrd(self):
        # type: () -> datatypes.SwtVertSts1
        pass

    @abc.abstractmethod
    def send_SeatVentnAutCdn(self, data):
        # type: (datatypes.Flg1) -> None
        pass

    @abc.abstractmethod
    def stop_SeatVentnAutCdn(self):
        pass

    @abc.abstractmethod
    def get_SetOfLang(self):
        # type: () -> (datatypes.SetOfLang)
        pass

    @abc.abstractmethod
    def send_SetOfLangAck(self, data):
        # type: (datatypes.FailrNoFailr1) -> None
        pass

    @abc.abstractmethod
    def stop_SetOfLangAck(self):
        pass

    @abc.abstractmethod
    def get_SetTiAndDate(self):
        # type: () -> (datatypes.DateTi1ForSet)
        pass

    @abc.abstractmethod
    def get_SetgAndRstOfTripForDrvr(self):
        # type: () -> (datatypes.SetgAndRstOfTripForDrvr)
        pass

    @abc.abstractmethod
    def send_SftyCchAccSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchAccSts(self):
        pass

    @abc.abstractmethod
    def send_SftyCchDrvgCycIdx(self, data):
        # type: (datatypes.SftyCchIdx2) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchDrvgCycIdx(self):
        pass

    @abc.abstractmethod
    def send_SftyCchLaneKeepAidSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchLaneKeepAidSts(self):
        pass

    @abc.abstractmethod
    def send_SftyCchLongTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchLongTermIdx(self):
        pass

    @abc.abstractmethod
    def get_SftyCchLongTermIdxRst(self):
        # type: () -> datatypes.NoYes1
        pass

    @abc.abstractmethod
    def send_SftyCchMidTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchMidTermIdx(self):
        pass

    @abc.abstractmethod
    def send_SftyCchPilotAssiSts(self, data):
        # type: (datatypes.SftyCchActvnSts2) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchPilotAssiSts(self):
        pass

    @abc.abstractmethod
    def get_SftyCchPostTripSts(self):
        # type: () -> datatypes.SftyCchPostTripSts1
        pass

    @abc.abstractmethod
    def send_SftyCchPrimFbAftDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchPrimFbAftDrvg(self):
        pass

    @abc.abstractmethod
    def send_SftyCchPrimFbDurgDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchPrimFbDurgDrvg(self):
        pass

    @abc.abstractmethod
    def send_SftyCchSecFbAftDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchSecFbAftDrvg(self):
        pass

    @abc.abstractmethod
    def send_SftyCchSecFbDurgDrvg(self, data):
        # type: (datatypes.SftyCchFb1) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchSecFbDurgDrvg(self):
        pass

    @abc.abstractmethod
    def send_SftyCchShortTermIdx(self, data):
        # type: (datatypes.SftyCchIdx1) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchShortTermIdx(self):
        pass

    @abc.abstractmethod
    def send_SftyCchSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_SftyCchSts(self):
        pass

    @abc.abstractmethod
    def get_SftySigGroupFromAudSafe(self):
        # type: () -> (datatypes.SftySigGroupFromAudSafe1)
        pass

    @abc.abstractmethod
    def send_SftyWarnGroupFromAsySafe(self, data):
        # type: (datatypes.SftyWarnGroupFromAsySafe1) -> None
        pass

    @abc.abstractmethod
    def stop_SftyWarnGroupFromAsySafe(self):
        pass

    @abc.abstractmethod
    def send_SnsrParkAssiFrnt(self, data):
        # type: (datatypes.SnsrParkAssi1) -> None
        pass

    @abc.abstractmethod
    def stop_SnsrParkAssiFrnt(self):
        pass

    @abc.abstractmethod
    def send_SnsrParkAssiRe(self, data):
        # type: (datatypes.SnsrParkAssi1) -> None
        pass

    @abc.abstractmethod
    def stop_SnsrParkAssiRe(self):
        pass

    @abc.abstractmethod
    def send_SnsrPrkgAssiFrnt(self, data):
        # type: (datatypes.SnsrPrkgAssi2) -> None
        pass

    @abc.abstractmethod
    def stop_SnsrPrkgAssiFrnt(self):
        pass

    @abc.abstractmethod
    def send_SnsrPrkgAssiRe(self, data):
        # type: (datatypes.SnsrPrkgAssi2) -> None
        pass

    @abc.abstractmethod
    def stop_SnsrPrkgAssiRe(self):
        pass

    @abc.abstractmethod
    def get_SoundExtActvSts(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_WhlMotSysSpdAct(self, data):
        # type: (float) -> None
        pass

    @abc.abstractmethod
    def stop_WhlMotSysSpdAct(self):
        pass

    @abc.abstractmethod
    def get_SpdAlrmActvForRoadSgnInfo(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_SpdCameraInfoSeldForRoadSgnInfo(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_SpdLimAdpvSts(self, data):
        # type: (datatypes.OffOnNotAvlSrvRqrd) -> None
        pass

    @abc.abstractmethod
    def stop_SpdLimAdpvSts(self):
        pass

    @abc.abstractmethod
    def send_SpdLimWarnReq(self, data):
        # type: (datatypes.SpdLimWarnReq1) -> None
        pass

    @abc.abstractmethod
    def stop_SpdLimWarnReq(self):
        pass

    @abc.abstractmethod
    def send_SpdWarn(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_SpdWarn(self):
        pass

    @abc.abstractmethod
    def send_SrvRst(self, data):
        # type: (datatypes.Trig1) -> None
        pass

    @abc.abstractmethod
    def stop_SrvRst(self):
        pass

    @abc.abstractmethod
    def send_SrvTrig(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_SrvTrig(self):
        pass

    @abc.abstractmethod
    def get_SteerAssiActvForAutDrv(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_SteerAssiLvlForAutDrv(self):
        # type: () -> (datatypes.SteerAssiLvlForAutDrvPen1)
        pass

    @abc.abstractmethod
    def get_SteerSetg(self):
        # type: () -> (datatypes.SteerSetg)
        pass

    @abc.abstractmethod
    def send_SteerWarnReqForAutDrv(self, data):
        # type: (datatypes.SteerWarnReqForAutDrv1) -> None
        pass

    @abc.abstractmethod
    def stop_SteerWarnReqForAutDrv(self):
        pass

    @abc.abstractmethod
    def send_SteerWarnReqForLaneKeepAid(self, data):
        # type: (datatypes.WarnLeRi) -> None
        pass

    @abc.abstractmethod
    def stop_SteerWarnReqForLaneKeepAid(self):
        pass

    @abc.abstractmethod
    def get_SteerWhlHeatgDurgClimaEnad(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def get_SteerWhlHeatgOnReq(self):
        # type: () -> datatypes.SteerWhlHeatgOnCmdTyp
        pass

    @abc.abstractmethod
    def send_SteerWhlHeatgStrtAutCdnOk(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_SteerWhlHeatgStrtAutCdnOk(self):
        pass

    @abc.abstractmethod
    def send_SteerWhlSnsrSafe(self, data):
        # type: (datatypes.SteerWhlSnsrSafe1) -> None
        pass

    @abc.abstractmethod
    def stop_SteerWhlSnsrSafe(self):
        pass

    @abc.abstractmethod
    def send_SteerWhlSwtPwr(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_SteerWhlSwtPwr(self):
        pass

    @abc.abstractmethod
    def send_StopStrtInhb(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_StopStrtInhb(self):
        pass

    @abc.abstractmethod
    def send_StrtInProgs(self, data):
        # type: (datatypes.StrtInProgs1) -> None
        pass

    @abc.abstractmethod
    def stop_StrtInProgs(self):
        pass

    @abc.abstractmethod
    def send_SunRoofPosnSts(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        pass

    @abc.abstractmethod
    def stop_SunRoofPosnSts(self):
        pass

    @abc.abstractmethod
    def get_SuspSetg(self):
        # type: () -> (datatypes.SuspSetgRec)
        pass

    @abc.abstractmethod
    def send_SwtAcptReq(self, data):
        # type: (datatypes.PsdNotPsd) -> None
        pass

    @abc.abstractmethod
    def stop_SwtAcptReq(self):
        pass

    @abc.abstractmethod
    def get_SwtAtCenSts(self):
        # type: () -> datatypes.Trig1
        pass

    @abc.abstractmethod
    def send_SwtForPassAirbCutOffSt(self, data):
        # type: (datatypes.OnOff2) -> None
        pass

    @abc.abstractmethod
    def stop_SwtForPassAirbCutOffSt(self):
        pass

    @abc.abstractmethod
    def send_TankFlapSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        pass

    @abc.abstractmethod
    def stop_TankFlapSts(self):
        pass

    @abc.abstractmethod
    def send_TelmSts(self, data):
        # type: (datatypes.TelmSts) -> None
        pass

    @abc.abstractmethod
    def stop_TelmSts(self):
        pass

    @abc.abstractmethod
    def send_TelmStsConnGen2(self, data):
        # type: (datatypes.TelmSts) -> None
        pass

    @abc.abstractmethod
    def stop_TelmStsConnGen2(self):
        pass

    @abc.abstractmethod
    def send_TiAndDateIndcn(self, data):
        # type: (datatypes.DateTi30) -> None
        pass

    @abc.abstractmethod
    def stop_TiAndDateIndcn(self):
        pass

    @abc.abstractmethod
    def get_TiGapAdpvSeld(self):
        # type: () -> (datatypes.TiGapAdpvSeldPen1)
        pass

    @abc.abstractmethod
    def get_TiGapLimd(self):
        # type: () -> (datatypes.TiGapPen1)
        pass

    @abc.abstractmethod
    def get_TireCircumCalByNav(self):
        # type: () -> (datatypes.TireCircumCalByNav)
        pass

    @abc.abstractmethod
    def get_TireMonCalReq(self):
        # type: () -> datatypes.StrtStopReq1
        pass

    @abc.abstractmethod
    def get_TireMonrDispReq(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_TirePAbsltValFrntRi(self, data):
        # type: (datatypes.TirePAbsltValFrntRi1) -> None
        pass

    @abc.abstractmethod
    def stop_TirePAbsltValFrntRi(self):
        pass

    @abc.abstractmethod
    def send_TirePAbsltValFrntLe(self, data):
        # type: (datatypes.TirePAbsltValFrntLe3) -> None
        pass

    @abc.abstractmethod
    def stop_TirePAbsltValFrntLe(self):
        pass

    @abc.abstractmethod
    def send_TirePAbsltValReLe(self, data):
        # type: (datatypes.TirePAbsltValReLe1) -> None
        pass

    @abc.abstractmethod
    def stop_TirePAbsltValReLe(self):
        pass

    @abc.abstractmethod
    def send_TirePAbsltValReRi(self, data):
        # type: (datatypes.TirePAbsltValReRi1) -> None
        pass

    @abc.abstractmethod
    def stop_TirePAbsltValReRi(self):
        pass

    @abc.abstractmethod
    def send_TirePCalSts(self, data):
        # type: (datatypes.TirePCalSts2) -> None
        pass

    @abc.abstractmethod
    def stop_TirePCalSts(self):
        pass

    @abc.abstractmethod
    def send_TirePMonData(self, data):
        # type: (datatypes.TirePMonData1) -> None
        pass

    @abc.abstractmethod
    def stop_TirePMonData(self):
        pass

    @abc.abstractmethod
    def send_TnlEntryOrEnd(self, data):
        # type: (datatypes.NoYes1) -> None
        pass

    @abc.abstractmethod
    def stop_TnlEntryOrEnd(self):
        pass

    @abc.abstractmethod
    def send_TrSts(self, data):
        # type: (datatypes.DoorSts2) -> None
        pass

    @abc.abstractmethod
    def stop_TrSts(self):
        pass

    @abc.abstractmethod
    def get_TrfcLiSpprtActv(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_TrfcLiSpprtSts(self, data):
        # type: (datatypes.FctSts2) -> None
        pass

    @abc.abstractmethod
    def stop_TrfcLiSpprtSts(self):
        pass

    @abc.abstractmethod
    def get_TrfcRglnSeldForRoadSgnInfo(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_TripModSeln(self):
        # type: () -> datatypes.TripModSeln
        pass

    @abc.abstractmethod
    def send_TrlrLampChkSts1WdSts1(self, data):
        # type: (datatypes.TrlrLampChkSts1) -> None
        pass

    @abc.abstractmethod
    def stop_TrlrLampChkSts1WdSts1(self):
        pass

    @abc.abstractmethod
    def get_TrlrLampChkAutReq(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_TrlrLampChkRemReq(self):
        # type: () -> datatypes.StrtStopReq1
        pass

    @abc.abstractmethod
    def send_TrlrLampChkSts1WdSts2(self, data):
        # type: (datatypes.TrlrLampChkSts2) -> None
        pass

    @abc.abstractmethod
    def stop_TrlrLampChkSts1WdSts2(self):
        pass

    @abc.abstractmethod
    def send_TrlrPrsntSts1WdSts(self, data):
        # type: (datatypes.TrlrPrsntSts) -> None
        pass

    @abc.abstractmethod
    def stop_TrlrPrsntSts1WdSts(self):
        pass

    @abc.abstractmethod
    def get_LiExtReq1WdReq3(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_TwliBriRaw(self, data):
        # type: (datatypes.TwliBriRaw) -> None
        pass

    @abc.abstractmethod
    def stop_TwliBriRaw(self):
        pass

    @abc.abstractmethod
    def send_TwliBriSts(self, data):
        # type: (datatypes.TwliBriSts1) -> None
        pass

    @abc.abstractmethod
    def stop_TwliBriSts(self):
        pass

    @abc.abstractmethod
    def get_TwliBriStsForSeatHeatrRe(self):
        # type: () -> datatypes.TwliBriSts1
        pass

    @abc.abstractmethod
    def send_UkwnCptReqToInfoMgr(self, data):
        # type: (datatypes.UInt64Rec) -> None
        pass

    @abc.abstractmethod
    def stop_UkwnCptReqToInfoMgr(self):
        pass

    @abc.abstractmethod
    def send_UkwnCptReqToSoundMgr(self, data):
        # type: (datatypes.UInt64Rec) -> None
        pass

    @abc.abstractmethod
    def stop_UkwnCptReqToSoundMgr(self):
        pass

    @abc.abstractmethod
    def get_UkwnCptRespFromInfoMgr(self):
        # type: () -> (datatypes.UInt64Rec)
        pass

    @abc.abstractmethod
    def get_UkwnCptRespFromSoundMgr(self):
        # type: () -> (datatypes.UInt64Rec)
        pass

    @abc.abstractmethod
    def get_UnlckFbVisReq(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def get_UnlckKeylsReq(self):
        # type: () -> (datatypes.UnlckKeylsCfgPen2)
        pass

    @abc.abstractmethod
    def get_UnlckRemReq(self):
        # type: () -> (datatypes.UnlckRemCfgPen1)
        pass

    @abc.abstractmethod
    def get_UsrSeldDrvrSpprtFct(self):
        # type: () -> datatypes.DrvrSpprtFct
        pass

    @abc.abstractmethod
    def get_UsrSetSpdForKeySpdLimn(self):
        # type: () -> (datatypes.UsrSetSpdForKeySpdLimn)
        pass

    @abc.abstractmethod
    def get_UsrSetSpdForKeySpdWarn(self):
        # type: () -> (datatypes.UsrSetSpdForKeySpdWarn)
        pass

    @abc.abstractmethod
    def get_UsrSetVolMaxForKeyVolLimn(self):
        # type: () -> (datatypes.UsrSetVolMaxForKeyVolLimn)
        pass

    @abc.abstractmethod
    def get_UsrSwtDispClimaReq(self):
        # type: () -> datatypes.OnOffIdle1
        pass

    @abc.abstractmethod
    def send_UsrSwtDispClimaReqForRowSec(self, data):
        # type: (datatypes.UsrSwtDispClimaReqForRowSec) -> None
        pass

    @abc.abstractmethod
    def stop_UsrSwtDispClimaReqForRowSec(self):
        pass

    @abc.abstractmethod
    def get_SeatHeatReLeSts(self):
        # type: () -> datatypes.SeatClimaLvl
        pass

    @abc.abstractmethod
    def get_SeatHeatReRiSts(self):
        # type: () -> datatypes.SeatClimaLvl
        pass

    @abc.abstractmethod
    def get_UsrSwtDispClimaSts(self):
        # type: () -> (datatypes.UsrSwtDispClimaSts)
        pass

    @abc.abstractmethod
    def get_UsrSwtDispForSecRowSeatVentn(self):
        # type: () -> (datatypes.UsrSwtDispForSecRowSeatVentn)
        pass

    @abc.abstractmethod
    def send_UsrSwtDispReqForSecRowSeatVentn(self, data):
        # type: (datatypes.UsrSwtDispReqForSecRowSeatVentn) -> None
        pass

    @abc.abstractmethod
    def stop_UsrSwtDispReqForSecRowSeatVentn(self):
        pass

    @abc.abstractmethod
    def get_UsrSwtDispReqVrnt(self):
        # type: () -> (datatypes.UsrSwtDispReqVrnt)
        pass

    @abc.abstractmethod
    def send_SeatHeatLvlReqLe(self, data):
        # type: (datatypes.SeatClimaLvl) -> None
        pass

    @abc.abstractmethod
    def stop_SeatHeatLvlReqLe(self):
        pass

    @abc.abstractmethod
    def send_SeatHeatLvlReqRi(self, data):
        # type: (datatypes.SeatClimaLvl) -> None
        pass

    @abc.abstractmethod
    def stop_SeatHeatLvlReqRi(self):
        pass

    @abc.abstractmethod
    def get_VFCVectorIHU(self):
        # type: () -> (datatypes.VFCGrp)
        pass

    @abc.abstractmethod
    def send_VehActvMsgToDrvr(self, data):
        # type: (datatypes.VehActv1) -> None
        pass

    @abc.abstractmethod
    def stop_VehActvMsgToDrvr(self):
        pass

    @abc.abstractmethod
    def send_VehBattU(self, data):
        # type: (datatypes.SysU) -> None
        pass

    @abc.abstractmethod
    def stop_VehBattU(self):
        pass

    @abc.abstractmethod
    def send_VehCfgPrm(self, data):
        # type: (datatypes.VehCfgPrm) -> None
        pass

    @abc.abstractmethod
    def stop_VehCfgPrm(self):
        pass

    @abc.abstractmethod
    def get_VehDecelCtrlSetg(self):
        # type: () -> (datatypes.NormSptPen)
        pass

    @abc.abstractmethod
    def send_VehMNom(self, data):
        # type: (datatypes.VehMNomRec1) -> None
        pass

    @abc.abstractmethod
    def stop_VehMNom(self):
        pass

    @abc.abstractmethod
    def send_VehModMngtGlbSafe1(self, data):
        # type: (datatypes.VehModMngtGlbSafe1) -> None
        pass

    @abc.abstractmethod
    def stop_VehModMngtGlbSafe1(self):
        pass

    @abc.abstractmethod
    def send_VehMtnStSafe(self, data):
        # type: (datatypes.VehMtnStSafe1) -> None
        pass

    @abc.abstractmethod
    def stop_VehMtnStSafe(self):
        pass

    @abc.abstractmethod
    def send_AudWarn(self, data):
        # type: (bool) -> None
        pass

    @abc.abstractmethod
    def stop_AudWarn(self):
        pass

    @abc.abstractmethod
    def send_VehSpdAvgIndcd(self, data):
        # type: (datatypes.VehSpdIndcd1) -> None
        pass

    @abc.abstractmethod
    def stop_VehSpdAvgIndcd(self):
        pass

    @abc.abstractmethod
    def get_VehSpdCtrlActvSts(self):
        # type: () -> datatypes.OnOff1
        pass

    @abc.abstractmethod
    def send_VehSpdIndcd(self, data):
        # type: (datatypes.VehSpdIndcd1) -> None
        pass

    @abc.abstractmethod
    def stop_VehSpdIndcd(self):
        pass

    @abc.abstractmethod
    def send_VehSpdLgtSafe(self, data):
        # type: (datatypes.VehSpdLgtSafe1) -> None
        pass

    @abc.abstractmethod
    def stop_VehSpdLgtSafe(self):
        pass

    @abc.abstractmethod
    def send_Vin(self, data):
        # type: (datatypes.Vin1) -> None
        pass

    @abc.abstractmethod
    def stop_Vin(self):
        pass

    @abc.abstractmethod
    def get_VisnAgWideCfg(self):
        # type: () -> (datatypes.VisnAgWideCfg1)
        pass

    @abc.abstractmethod
    def send_VisnAgWideSts(self, data):
        # type: (datatypes.AvlSts2) -> None
        pass

    @abc.abstractmethod
    def stop_VisnAgWideSts(self):
        pass

    @abc.abstractmethod
    def send_VisnImgAgWideInUse(self, data):
        # type: (datatypes.VisnImgAgWideInUse) -> None
        pass

    @abc.abstractmethod
    def stop_VisnImgAgWideInUse(self):
        pass

    @abc.abstractmethod
    def get_VisnImgAgWideReq(self):
        # type: () -> datatypes.VisnImgAgWideInUse
        pass

    @abc.abstractmethod
    def get_VisnImgCamReq(self):
        # type: () -> (datatypes.CamIndReq)
        pass

    @abc.abstractmethod
    def get_WarnTypForLaneChgWarn(self):
        # type: () -> (datatypes.WarnTypForLaneChgWarnPen1)
        pass

    @abc.abstractmethod
    def get_WarnTypForLaneKeepAid(self):
        # type: () -> (datatypes.WarnTypForLaneChgWarnPen1)
        pass

    @abc.abstractmethod
    def send_WhlCircum(self, data):
        # type: (int) -> None
        pass

    @abc.abstractmethod
    def stop_WhlCircum(self):
        pass

    @abc.abstractmethod
    def send_WhlMotSysTqEst(self, data):
        # type: (datatypes.TqSafe2) -> None
        pass

    @abc.abstractmethod
    def stop_WhlMotSysTqEst(self):
        pass

    @abc.abstractmethod
    def send_WhlRotToothCntr(self, data):
        # type: (datatypes.WhlRotToothCntrRec) -> None
        pass

    @abc.abstractmethod
    def stop_WhlRotToothCntr(self):
        pass

    @abc.abstractmethod
    def send_WinPosnStsAtDrv(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        pass

    @abc.abstractmethod
    def stop_WinPosnStsAtDrv(self):
        pass

    @abc.abstractmethod
    def send_WinPosnStsAtDrvrRe(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        pass

    @abc.abstractmethod
    def stop_WinPosnStsAtDrvrRe(self):
        pass

    @abc.abstractmethod
    def send_WinPosnStsAtPass(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        pass

    @abc.abstractmethod
    def stop_WinPosnStsAtPass(self):
        pass

    @abc.abstractmethod
    def send_WinPosnStsAtPassRe(self, data):
        # type: (datatypes.WinAndRoofAndCurtPosnTyp) -> None
        pass

    @abc.abstractmethod
    def stop_WinPosnStsAtPassRe(self):
        pass

    @abc.abstractmethod
    def send_WipgInfo(self, data):
        # type: (datatypes.WipgInfo) -> None
        pass

    @abc.abstractmethod
    def stop_WipgInfo(self):
        pass

    @abc.abstractmethod
    def get_WiprFrntSrvModReq(self):
        # type: () -> datatypes.WiprFrntSrvModReq
        pass

    @abc.abstractmethod
    def send_WiprInPosnForSrv(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_WiprInPosnForSrv(self):
        pass

    @abc.abstractmethod
    def get_WiprReAutReq(self):
        # type: () -> (datatypes.OnOffPen)
        pass

    @abc.abstractmethod
    def send_WshrFldSts1WdElmHMI(self, data):
        # type: (datatypes.OnOff1) -> None
        pass

    @abc.abstractmethod
    def stop_WshrFldSts1WdElmHMI(self):
        pass

    @abc.abstractmethod
    def send_iTPMSCalPsbl(self, data):
        # type: (datatypes.CalPsbl) -> None
        pass

    @abc.abstractmethod
    def stop_iTPMSCalPsbl(self):
        pass

    @abc.abstractmethod
    def get_iTPMSCalReq(self):
        # type: () -> datatypes.StrtStopReq1
        pass

    @abc.abstractmethod
    def send_iTPMSCalSts(self, data):
        # type: (datatypes.iTPMSTirePCalSts) -> None
        pass

    @abc.abstractmethod
    def stop_iTPMSCalSts(self):
        pass

    @abc.abstractmethod
    def send_iTPMSTirePMSts(self, data):
        # type: (datatypes.iTPMSTirePMSts) -> None
        pass

    @abc.abstractmethod
    def stop_iTPMSTirePMSts(self):
        pass

