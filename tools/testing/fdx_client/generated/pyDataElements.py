# Signal scaling database
# --- AUTO GENERATED ---
# Inputs: generate_signal_scaling.py 
#    --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml 
#    --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Com.arxml 
#    --fdxdescriptionfile=FDXDescriptionFile.xml 
#    --out=generated/pyDataElements.py
from fdx import fdx_client
from fdx import fdx_description_file_parser
from . import config

class FrSignalInterface:
    def __init__(self):
        (self.groups, self.sysvar_list, self.signal_list) = fdx_description_file_parser.parse(
                config.fdx_description_file_path)
        self.group_id_map = {g.group_id: g for g in self.groups}

        def data_exchange(self, group_id, data):
            group = self.group_id_map[group_id]
            group.receive_data(data)

        try:
            self.connection = fdx_client.FDXConnection(data_exchange, config.vector_fdx_ip, config.vector_fdx_port)
            self.connection.send_start()
            self.connection.confirmed_start()
        except:
            self.connection.close()
            raise


        name_to_item_map = { i.name : i for i in self.signal_list }

        self.AbsWarnIndcnReq = AbsWarnIndcnReq(self, name_to_item_map[AbsWarnIndcnReq.fdx_name])
        self.AccAdprSpdLimActvPen = AccAdprSpdLimActvPen(self, name_to_item_map[AccAdprSpdLimActvPen.fdx_name])
        self.AccAdprSpdLimActvSts = AccAdprSpdLimActvSts(self, name_to_item_map[AccAdprSpdLimActvSts.fdx_name])
        self.AccAdprTurnSpdActvPen = AccAdprTurnSpdActvPen(self, name_to_item_map[AccAdprTurnSpdActvPen.fdx_name])
        self.AccAdprTurnSpdActvSts = AccAdprTurnSpdActvSts(self, name_to_item_map[AccAdprTurnSpdActvSts.fdx_name])
        self.AccAutResuWarnReq = AccAutResuWarnReq(self, name_to_item_map[AccAutResuWarnReq.fdx_name])
        self.AccSts = AccSts(self, name_to_item_map[AccSts.fdx_name])
        self.ActvOfHorn = ActvOfHorn(self, name_to_item_map[ActvOfHorn.fdx_name])
        self.ADataRawSafeChks = ADataRawSafeChks(self, name_to_item_map[ADataRawSafeChks.fdx_name])
        self.ADataRawSafeCntr = ADataRawSafeCntr(self, name_to_item_map[ADataRawSafeCntr.fdx_name])
        self.AdjSpdLimnSts = AdjSpdLimnSts(self, name_to_item_map[AdjSpdLimnSts.fdx_name])
        self.AdjSpdLimnWarn = AdjSpdLimnWarn(self, name_to_item_map[AdjSpdLimnWarn.fdx_name])
        self.AgDataRawSafeChks = AgDataRawSafeChks(self, name_to_item_map[AgDataRawSafeChks.fdx_name])
        self.AgDataRawSafeCntr = AgDataRawSafeCntr(self, name_to_item_map[AgDataRawSafeCntr.fdx_name])
        self.AirClngReq = AirClngReq(self, name_to_item_map[AirClngReq.fdx_name])
        self.ALat1 = ALat1(self, name_to_item_map[ALat1.fdx_name])
        self.ALat1Qf1 = ALat1Qf1(self, name_to_item_map[ALat1Qf1.fdx_name])
        self.ALgt1 = ALgt1(self, name_to_item_map[ALgt1.fdx_name])
        self.ALgt1Qf1 = ALgt1Qf1(self, name_to_item_map[ALgt1Qf1.fdx_name])
        self.AmbTIndcd = AmbTIndcd(self, name_to_item_map[AmbTIndcd.fdx_name])
        self.AmbTIndcdQf = AmbTIndcdQf(self, name_to_item_map[AmbTIndcdQf.fdx_name])
        self.AmbTIndcdUnit = AmbTIndcdUnit(self, name_to_item_map[AmbTIndcdUnit.fdx_name])
        self.AmbTRawQly = AmbTRawQly(self, name_to_item_map[AmbTRawQly.fdx_name])
        self.AmbTRawVal = AmbTRawVal(self, name_to_item_map[AmbTRawVal.fdx_name])
        self.AntithftRednReq = AntithftRednReq(self, name_to_item_map[AntithftRednReq.fdx_name])
        self.AsyLaneChgPsbl = AsyLaneChgPsbl(self, name_to_item_map[AsyLaneChgPsbl.fdx_name])
        self.AsyLaneChgSts = AsyLaneChgSts(self, name_to_item_map[AsyLaneChgSts.fdx_name])
        self.AsyLaneChgTyp = AsyLaneChgTyp(self, name_to_item_map[AsyLaneChgTyp.fdx_name])
        self.AsySteerApplyRqrd = AsySteerApplyRqrd(self, name_to_item_map[AsySteerApplyRqrd.fdx_name])
        self.AudMsgReq = AudMsgReq(self, name_to_item_map[AudMsgReq.fdx_name])
        self.AudWarn = AudWarn(self, name_to_item_map[AudWarn.fdx_name])
        self.AudWarnActv = AudWarnActv(self, name_to_item_map[AudWarnActv.fdx_name])
        self.AVert2 = AVert2(self, name_to_item_map[AVert2.fdx_name])
        self.AVert2Qf1 = AVert2Qf1(self, name_to_item_map[AVert2Qf1.fdx_name])
        self.BackCntrForMissCom = BackCntrForMissCom(self, name_to_item_map[BackCntrForMissCom.fdx_name])
        self.BarForFuEco = BarForFuEco(self, name_to_item_map[BarForFuEco.fdx_name])
        self.BkpOfDstTrvld = BkpOfDstTrvld(self, name_to_item_map[BkpOfDstTrvld.fdx_name])
        self.BltLockStAtDrvrForBltLockSt1 = BltLockStAtDrvrForBltLockSt1(self, name_to_item_map[BltLockStAtDrvrForBltLockSt1.fdx_name])
        self.BltLockStAtDrvrForDevErrSts2 = BltLockStAtDrvrForDevErrSts2(self, name_to_item_map[BltLockStAtDrvrForDevErrSts2.fdx_name])
        self.BltLockStAtPassForBltLockSt1 = BltLockStAtPassForBltLockSt1(self, name_to_item_map[BltLockStAtPassForBltLockSt1.fdx_name])
        self.BltLockStAtPassForBltLockSts = BltLockStAtPassForBltLockSts(self, name_to_item_map[BltLockStAtPassForBltLockSts.fdx_name])
        self.BltLockStAtRowSecLeForBltLockEquid = BltLockStAtRowSecLeForBltLockEquid(self, name_to_item_map[BltLockStAtRowSecLeForBltLockEquid.fdx_name])
        self.BltLockStAtRowSecLeForBltLockSt1 = BltLockStAtRowSecLeForBltLockSt1(self, name_to_item_map[BltLockStAtRowSecLeForBltLockSt1.fdx_name])
        self.BltLockStAtRowSecLeForBltLockSts = BltLockStAtRowSecLeForBltLockSts(self, name_to_item_map[BltLockStAtRowSecLeForBltLockSts.fdx_name])
        self.BltLockStAtRowSecMidForBltLockEquid = BltLockStAtRowSecMidForBltLockEquid(self, name_to_item_map[BltLockStAtRowSecMidForBltLockEquid.fdx_name])
        self.BltLockStAtRowSecMidForBltLockSt1 = BltLockStAtRowSecMidForBltLockSt1(self, name_to_item_map[BltLockStAtRowSecMidForBltLockSt1.fdx_name])
        self.BltLockStAtRowSecMidForBltLockSts = BltLockStAtRowSecMidForBltLockSts(self, name_to_item_map[BltLockStAtRowSecMidForBltLockSts.fdx_name])
        self.BltLockStAtRowSecRiForBltLockEquid = BltLockStAtRowSecRiForBltLockEquid(self, name_to_item_map[BltLockStAtRowSecRiForBltLockEquid.fdx_name])
        self.BltLockStAtRowSecRiForBltLockSt1 = BltLockStAtRowSecRiForBltLockSt1(self, name_to_item_map[BltLockStAtRowSecRiForBltLockSt1.fdx_name])
        self.BltLockStAtRowSecRiForBltLockSts = BltLockStAtRowSecRiForBltLockSts(self, name_to_item_map[BltLockStAtRowSecRiForBltLockSts.fdx_name])
        self.BltLockStAtRowThrdLeForBltLockEquid = BltLockStAtRowThrdLeForBltLockEquid(self, name_to_item_map[BltLockStAtRowThrdLeForBltLockEquid.fdx_name])
        self.BltLockStAtRowThrdLeForBltLockSt1 = BltLockStAtRowThrdLeForBltLockSt1(self, name_to_item_map[BltLockStAtRowThrdLeForBltLockSt1.fdx_name])
        self.BltLockStAtRowThrdLeForBltLockSts = BltLockStAtRowThrdLeForBltLockSts(self, name_to_item_map[BltLockStAtRowThrdLeForBltLockSts.fdx_name])
        self.BltLockStAtRowThrdRiForBltLockEquid = BltLockStAtRowThrdRiForBltLockEquid(self, name_to_item_map[BltLockStAtRowThrdRiForBltLockEquid.fdx_name])
        self.BltLockStAtRowThrdRiForBltLockSt1 = BltLockStAtRowThrdRiForBltLockSt1(self, name_to_item_map[BltLockStAtRowThrdRiForBltLockSt1.fdx_name])
        self.BltLockStAtRowThrdRiForBltLockSts = BltLockStAtRowThrdRiForBltLockSts(self, name_to_item_map[BltLockStAtRowThrdRiForBltLockSts.fdx_name])
        self.BltRmnSound1 = BltRmnSound1(self, name_to_item_map[BltRmnSound1.fdx_name])
        self.BltRmnSound2 = BltRmnSound2(self, name_to_item_map[BltRmnSound2.fdx_name])
        self.BltRmnSound3 = BltRmnSound3(self, name_to_item_map[BltRmnSound3.fdx_name])
        self.BltRmnSound4 = BltRmnSound4(self, name_to_item_map[BltRmnSound4.fdx_name])
        self.BrkAndAbsWarnIndcnReqChks = BrkAndAbsWarnIndcnReqChks(self, name_to_item_map[BrkAndAbsWarnIndcnReqChks.fdx_name])
        self.BrkAndAbsWarnIndcnReqCntr = BrkAndAbsWarnIndcnReqCntr(self, name_to_item_map[BrkAndAbsWarnIndcnReqCntr.fdx_name])
        self.BrkFldLvl = BrkFldLvl(self, name_to_item_map[BrkFldLvl.fdx_name])
        self.BrkFricTqAtWhlFrntLeAct = BrkFricTqAtWhlFrntLeAct(self, name_to_item_map[BrkFricTqAtWhlFrntLeAct.fdx_name])
        self.BrkFricTqAtWhlFrntRiAct = BrkFricTqAtWhlFrntRiAct(self, name_to_item_map[BrkFricTqAtWhlFrntRiAct.fdx_name])
        self.BrkFricTqAtWhlReLeAct = BrkFricTqAtWhlReLeAct(self, name_to_item_map[BrkFricTqAtWhlReLeAct.fdx_name])
        self.BrkFricTqAtWhlReRiAct = BrkFricTqAtWhlReRiAct(self, name_to_item_map[BrkFricTqAtWhlReRiAct.fdx_name])
        self.BrkRelsWarnReq = BrkRelsWarnReq(self, name_to_item_map[BrkRelsWarnReq.fdx_name])
        self.BrkWarnIndcnReq = BrkWarnIndcnReq(self, name_to_item_map[BrkWarnIndcnReq.fdx_name])
        self.Btn1ForUsrSwtPanFrntReq = Btn1ForUsrSwtPanFrntReq(self, name_to_item_map[Btn1ForUsrSwtPanFrntReq.fdx_name])
        self.Btn2ForUsrSwtPanFrntReq = Btn2ForUsrSwtPanFrntReq(self, name_to_item_map[Btn2ForUsrSwtPanFrntReq.fdx_name])
        self.Btn3ForUsrSwtPanFrntReq = Btn3ForUsrSwtPanFrntReq(self, name_to_item_map[Btn3ForUsrSwtPanFrntReq.fdx_name])
        self.Btn4ForUsrSwtPanFrntReq = Btn4ForUsrSwtPanFrntReq(self, name_to_item_map[Btn4ForUsrSwtPanFrntReq.fdx_name])
        self.Btn5ForUsrSwtPanFrntReq = Btn5ForUsrSwtPanFrntReq(self, name_to_item_map[Btn5ForUsrSwtPanFrntReq.fdx_name])
        self.BtnAudFbSts = BtnAudFbSts(self, name_to_item_map[BtnAudFbSts.fdx_name])
        self.BtnAudVolSts = BtnAudVolSts(self, name_to_item_map[BtnAudVolSts.fdx_name])
        self.BtnMmedLeRiSts = BtnMmedLeRiSts(self, name_to_item_map[BtnMmedLeRiSts.fdx_name])
        self.BtnMmedModSts = BtnMmedModSts(self, name_to_item_map[BtnMmedModSts.fdx_name])
        self.BtnMmedSetSts = BtnMmedSetSts(self, name_to_item_map[BtnMmedSetSts.fdx_name])
        self.CallStsIndcn = CallStsIndcn(self, name_to_item_map[CallStsIndcn.fdx_name])
        self.CarModSts1 = CarModSts1(self, name_to_item_map[CarModSts1.fdx_name])
        self.CarModSubtypWdCarModSubtyp = CarModSubtypWdCarModSubtyp(self, name_to_item_map[CarModSubtypWdCarModSubtyp.fdx_name])
        self.CarTiGlb = CarTiGlb(self, name_to_item_map[CarTiGlb.fdx_name])
        self.CCSMBtn6 = CCSMBtn6(self, name_to_item_map[CCSMBtn6.fdx_name])
        self.ChdLockgProtnStsToHmi = ChdLockgProtnStsToHmi(self, name_to_item_map[ChdLockgProtnStsToHmi.fdx_name])
        self.ChdWinProtnStsAtDrvrRe = ChdWinProtnStsAtDrvrRe(self, name_to_item_map[ChdWinProtnStsAtDrvrRe.fdx_name])
        self.ChdWinProtnStsAtPassRe = ChdWinProtnStsAtPassRe(self, name_to_item_map[ChdWinProtnStsAtPassRe.fdx_name])
        self.ClimaActv = ClimaActv(self, name_to_item_map[ClimaActv.fdx_name])
        self.ClimaEcoModRqrd = ClimaEcoModRqrd(self, name_to_item_map[ClimaEcoModRqrd.fdx_name])
        self.ClimaPwrCns = ClimaPwrCns(self, name_to_item_map[ClimaPwrCns.fdx_name])
        self.ClimaRqrd = ClimaRqrd(self, name_to_item_map[ClimaRqrd.fdx_name])
        self.ClimaTmr = ClimaTmr(self, name_to_item_map[ClimaTmr.fdx_name])
        self.ClimaTmrSts = ClimaTmrSts(self, name_to_item_map[ClimaTmrSts.fdx_name])
        self.ClimaTmrStsRqrd = ClimaTmrStsRqrd(self, name_to_item_map[ClimaTmrStsRqrd.fdx_name])
        self.ClimaWarn = ClimaWarn(self, name_to_item_map[ClimaWarn.fdx_name])
        self.ClkFmt = ClkFmt(self, name_to_item_map[ClkFmt.fdx_name])
        self.CllsnFwdWarnActvPen = CllsnFwdWarnActvPen(self, name_to_item_map[CllsnFwdWarnActvPen.fdx_name])
        self.CllsnFwdWarnActvSts = CllsnFwdWarnActvSts(self, name_to_item_map[CllsnFwdWarnActvSts.fdx_name])
        self.CllsnFwdWarnReq = CllsnFwdWarnReq(self, name_to_item_map[CllsnFwdWarnReq.fdx_name])
        self.CllsnWarnReq = CllsnWarnReq(self, name_to_item_map[CllsnWarnReq.fdx_name])
        self.CllsnWarnSideLe = CllsnWarnSideLe(self, name_to_item_map[CllsnWarnSideLe.fdx_name])
        self.CllsnWarnSideRi = CllsnWarnSideRi(self, name_to_item_map[CllsnWarnSideRi.fdx_name])
        self.ClngActv = ClngActv(self, name_to_item_map[ClngActv.fdx_name])
        self.ClngRqrdFromHmi = ClngRqrdFromHmi(self, name_to_item_map[ClngRqrdFromHmi.fdx_name])
        self.ClsAutEna = ClsAutEna(self, name_to_item_map[ClsAutEna.fdx_name])
        self.CmftFctActv = CmftFctActv(self, name_to_item_map[CmftFctActv.fdx_name])
        self.CmftFctSts = CmftFctSts(self, name_to_item_map[CmftFctSts.fdx_name])
        self.CmptmtAirTEstimdExtdComptmtT = CmptmtAirTEstimdExtdComptmtT(self, name_to_item_map[CmptmtAirTEstimdExtdComptmtT.fdx_name])
        self.CmptmtAirTEstimdExtdQlyFlg = CmptmtAirTEstimdExtdQlyFlg(self, name_to_item_map[CmptmtAirTEstimdExtdQlyFlg.fdx_name])
        self.CmptmtTFrnt = CmptmtTFrnt(self, name_to_item_map[CmptmtTFrnt.fdx_name])
        self.CmptmtTFrnt = CmptmtTFrnt(self, name_to_item_map[CmptmtTFrnt.fdx_name])
        self.CmptmtTFrntQf = CmptmtTFrntQf(self, name_to_item_map[CmptmtTFrntQf.fdx_name])
        self.CmptmtTFrntQf = CmptmtTFrntQf(self, name_to_item_map[CmptmtTFrntQf.fdx_name])
        self.CnclWarnForCrsCtrl = CnclWarnForCrsCtrl(self, name_to_item_map[CnclWarnForCrsCtrl.fdx_name])
        self.CnclWarnLatForAutDrv = CnclWarnLatForAutDrv(self, name_to_item_map[CnclWarnLatForAutDrv.fdx_name])
        self.CnclWarnLgtForAutDrv = CnclWarnLgtForAutDrv(self, name_to_item_map[CnclWarnLgtForAutDrv.fdx_name])
        self.CntDwnToManvStrtInDstToManvLocn = CntDwnToManvStrtInDstToManvLocn(self, name_to_item_map[CntDwnToManvStrtInDstToManvLocn.fdx_name])
        self.ComLostExtrSound = ComLostExtrSound(self, name_to_item_map[ComLostExtrSound.fdx_name])
        self.ConSftyWarnConSftyWarnId = ConSftyWarnConSftyWarnId(self, name_to_item_map[ConSftyWarnConSftyWarnId.fdx_name])
        self.ConSftyWarnConSftyWarnLvl = ConSftyWarnConSftyWarnLvl(self, name_to_item_map[ConSftyWarnConSftyWarnLvl.fdx_name])
        self.ConSftyWarnConSftyWarnSnd = ConSftyWarnConSftyWarnSnd(self, name_to_item_map[ConSftyWarnConSftyWarnSnd.fdx_name])
        self.ConSftyWarnConSftyWarnTyp = ConSftyWarnConSftyWarnTyp(self, name_to_item_map[ConSftyWarnConSftyWarnTyp.fdx_name])
        self.ConSftyWarnDistanceToWarning = ConSftyWarnDistanceToWarning(self, name_to_item_map[ConSftyWarnDistanceToWarning.fdx_name])
        self.CoolgReqForDispCen = CoolgReqForDispCen(self, name_to_item_map[CoolgReqForDispCen.fdx_name])
        self.CoolgStsForDisp = CoolgStsForDisp(self, name_to_item_map[CoolgStsForDisp.fdx_name])
        self.CrsCtrlrSts = CrsCtrlrSts(self, name_to_item_map[CrsCtrlrSts.fdx_name])
        self.CrvtSpdAdpvSts = CrvtSpdAdpvSts(self, name_to_item_map[CrvtSpdAdpvSts.fdx_name])
        self.CrvtSpdWarnReq = CrvtSpdWarnReq(self, name_to_item_map[CrvtSpdWarnReq.fdx_name])
        self.CrvtSpdWarnSts = CrvtSpdWarnSts(self, name_to_item_map[CrvtSpdWarnSts.fdx_name])
        self.CtraIndcnLe = CtraIndcnLe(self, name_to_item_map[CtraIndcnLe.fdx_name])
        self.CtraIndcnRi = CtraIndcnRi(self, name_to_item_map[CtraIndcnRi.fdx_name])
        self.CtraOn1 = CtraOn1(self, name_to_item_map[CtraOn1.fdx_name])
        self.CtraSwOnOff = CtraSwOnOff(self, name_to_item_map[CtraSwOnOff.fdx_name])
        self.CurtActvnReReq = CurtActvnReReq(self, name_to_item_map[CurtActvnReReq.fdx_name])
        self.DataSpclDMSMDataIdn = DataSpclDMSMDataIdn(self, name_to_item_map[DataSpclDMSMDataIdn.fdx_name])
        self.DataSpclDMSMDataNrSpcl1 = DataSpclDMSMDataNrSpcl1(self, name_to_item_map[DataSpclDMSMDataNrSpcl1.fdx_name])
        self.DataSpclDMSMDataNrSpcl2 = DataSpclDMSMDataNrSpcl2(self, name_to_item_map[DataSpclDMSMDataNrSpcl2.fdx_name])
        self.DataSpclDMSMDataNrSpcl3 = DataSpclDMSMDataNrSpcl3(self, name_to_item_map[DataSpclDMSMDataNrSpcl3.fdx_name])
        self.DateOrTi = DateOrTi(self, name_to_item_map[DateOrTi.fdx_name])
        self.Day = Day(self, name_to_item_map[Day.fdx_name])
        self.Day1 = Day1(self, name_to_item_map[Day1.fdx_name])
        self.DayToSrv = DayToSrv(self, name_to_item_map[DayToSrv.fdx_name])
        self.DiagcCCSM = DiagcCCSM(self, name_to_item_map[DiagcCCSM.fdx_name])
        self.DiagcRCSM = DiagcRCSM(self, name_to_item_map[DiagcRCSM.fdx_name])
        self.DiagcRSHC = DiagcRSHC(self, name_to_item_map[DiagcRSHC.fdx_name])
        self.DiagcStsDMSM = DiagcStsDMSM(self, name_to_item_map[DiagcStsDMSM.fdx_name])
        self.DispAndHomeBtnSts = DispAndHomeBtnSts(self, name_to_item_map[DispAndHomeBtnSts.fdx_name])
        self.DispBattEgyIn = DispBattEgyIn(self, name_to_item_map[DispBattEgyIn.fdx_name])
        self.DispBattEgyOut = DispBattEgyOut(self, name_to_item_map[DispBattEgyOut.fdx_name])
        self.DispOfPrpsnMod = DispOfPrpsnMod(self, name_to_item_map[DispOfPrpsnMod.fdx_name])
        self.DoorDrvrLockReSts = DoorDrvrLockReSts(self, name_to_item_map[DoorDrvrLockReSts.fdx_name])
        self.DoorDrvrLockSts = DoorDrvrLockSts(self, name_to_item_map[DoorDrvrLockSts.fdx_name])
        self.DoorDrvrReSts = DoorDrvrReSts(self, name_to_item_map[DoorDrvrReSts.fdx_name])
        self.DoorDrvrSts = DoorDrvrSts(self, name_to_item_map[DoorDrvrSts.fdx_name])
        self.DoorPassLockReSts = DoorPassLockReSts(self, name_to_item_map[DoorPassLockReSts.fdx_name])
        self.DoorPassLockSts = DoorPassLockSts(self, name_to_item_map[DoorPassLockSts.fdx_name])
        self.DoorPassReSts = DoorPassReSts(self, name_to_item_map[DoorPassReSts.fdx_name])
        self.DoorPassSts = DoorPassSts(self, name_to_item_map[DoorPassSts.fdx_name])
        self.DriveAwayInfoWarnReqAudWarnReq = DriveAwayInfoWarnReqAudWarnReq(self, name_to_item_map[DriveAwayInfoWarnReqAudWarnReq.fdx_name])
        self.DriveAwayInfoWarnReqCtrlDirOfTrfcLi = DriveAwayInfoWarnReqCtrlDirOfTrfcLi(self, name_to_item_map[DriveAwayInfoWarnReqCtrlDirOfTrfcLi.fdx_name])
        self.DriveAwayInfoWarnReqInterruptionSrc = DriveAwayInfoWarnReqInterruptionSrc(self, name_to_item_map[DriveAwayInfoWarnReqInterruptionSrc.fdx_name])
        self.DriveAwayInfoWarnReqReqSrc = DriveAwayInfoWarnReqReqSrc(self, name_to_item_map[DriveAwayInfoWarnReqReqSrc.fdx_name])
        self.DriveAwayInfoWarnReqTiToDrvCntDwnTi = DriveAwayInfoWarnReqTiToDrvCntDwnTi(self, name_to_item_map[DriveAwayInfoWarnReqTiToDrvCntDwnTi.fdx_name])
        self.DriveAwayInfoWarnReqVisWarnReq = DriveAwayInfoWarnReqVisWarnReq(self, name_to_item_map[DriveAwayInfoWarnReqVisWarnReq.fdx_name])
        self.DrvgInWrgDirOfTrfc = DrvgInWrgDirOfTrfc(self, name_to_item_map[DrvgInWrgDirOfTrfc.fdx_name])
        self.DrvModReq = DrvModReq(self, name_to_item_map[DrvModReq.fdx_name])
        self.DrvrCtrlOfPassSeatFrntReqd = DrvrCtrlOfPassSeatFrntReqd(self, name_to_item_map[DrvrCtrlOfPassSeatFrntReqd.fdx_name])
        self.DrvrCtrlOfPassSeatFrntSts = DrvrCtrlOfPassSeatFrntSts(self, name_to_item_map[DrvrCtrlOfPassSeatFrntSts.fdx_name])
        self.DrvrDesDir = DrvrDesDir(self, name_to_item_map[DrvrDesDir.fdx_name])
        self.DrvrDispSetgPen = DrvrDispSetgPen(self, name_to_item_map[DrvrDispSetgPen.fdx_name])
        self.DrvrDispSetgSts = DrvrDispSetgSts(self, name_to_item_map[DrvrDispSetgSts.fdx_name])
        self.DrvrHmiBackGndInfoSetgPen = DrvrHmiBackGndInfoSetgPen(self, name_to_item_map[DrvrHmiBackGndInfoSetgPen.fdx_name])
        self.DrvrHmiBackGndInfoSetgSetg = DrvrHmiBackGndInfoSetgSetg(self, name_to_item_map[DrvrHmiBackGndInfoSetgSetg.fdx_name])
        self.DrvrHmiDispdModPen = DrvrHmiDispdModPen(self, name_to_item_map[DrvrHmiDispdModPen.fdx_name])
        self.DrvrHmiDispdModSts = DrvrHmiDispdModSts(self, name_to_item_map[DrvrHmiDispdModSts.fdx_name])
        self.DrvrHmiUsrIfSetgPen = DrvrHmiUsrIfSetgPen(self, name_to_item_map[DrvrHmiUsrIfSetgPen.fdx_name])
        self.DrvrHmiUsrIfSetgSetg = DrvrHmiUsrIfSetgSetg(self, name_to_item_map[DrvrHmiUsrIfSetgSetg.fdx_name])
        self.DrvrMassgRunng = DrvrMassgRunng(self, name_to_item_map[DrvrMassgRunng.fdx_name])
        self.DrvrPfmncLvl = DrvrPfmncLvl(self, name_to_item_map[DrvrPfmncLvl.fdx_name])
        self.DrvrPfmncMonActvPen = DrvrPfmncMonActvPen(self, name_to_item_map[DrvrPfmncMonActvPen.fdx_name])
        self.DrvrPfmncMonActvSts = DrvrPfmncMonActvSts(self, name_to_item_map[DrvrPfmncMonActvSts.fdx_name])
        self.DrvrPfmncSts = DrvrPfmncSts(self, name_to_item_map[DrvrPfmncSts.fdx_name])
        self.DrvrPfmncWarnReq = DrvrPfmncWarnReq(self, name_to_item_map[DrvrPfmncWarnReq.fdx_name])
        self.DrvrSeatActvSpplFct = DrvrSeatActvSpplFct(self, name_to_item_map[DrvrSeatActvSpplFct.fdx_name])
        self.DrvrSeatDispMassgFctMassgInten = DrvrSeatDispMassgFctMassgInten(self, name_to_item_map[DrvrSeatDispMassgFctMassgInten.fdx_name])
        self.DrvrSeatDispMassgFctMassgProg = DrvrSeatDispMassgFctMassgProg(self, name_to_item_map[DrvrSeatDispMassgFctMassgProg.fdx_name])
        self.DrvrSeatDispMassgFctMassgSpdLvl = DrvrSeatDispMassgFctMassgSpdLvl(self, name_to_item_map[DrvrSeatDispMassgFctMassgSpdLvl.fdx_name])
        self.DrvrSeatDispMassgFctOnOff = DrvrSeatDispMassgFctOnOff(self, name_to_item_map[DrvrSeatDispMassgFctOnOff.fdx_name])
        self.DrvrSeatDispSpplFct = DrvrSeatDispSpplFct(self, name_to_item_map[DrvrSeatDispSpplFct.fdx_name])
        self.DrvrSeatMassgFctMassgInten = DrvrSeatMassgFctMassgInten(self, name_to_item_map[DrvrSeatMassgFctMassgInten.fdx_name])
        self.DrvrSeatMassgFctMassgProg = DrvrSeatMassgFctMassgProg(self, name_to_item_map[DrvrSeatMassgFctMassgProg.fdx_name])
        self.DrvrSeatMassgFctMassgSpdLvl = DrvrSeatMassgFctMassgSpdLvl(self, name_to_item_map[DrvrSeatMassgFctMassgSpdLvl.fdx_name])
        self.DrvrSeatMassgFctOnOff = DrvrSeatMassgFctOnOff(self, name_to_item_map[DrvrSeatMassgFctOnOff.fdx_name])
        self.DrvrSeatSwtAdjmtOfSpplFctHozlSts = DrvrSeatSwtAdjmtOfSpplFctHozlSts(self, name_to_item_map[DrvrSeatSwtAdjmtOfSpplFctHozlSts.fdx_name])
        self.DrvrSeatSwtAdjmtOfSpplFctVertSts = DrvrSeatSwtAdjmtOfSpplFctVertSts(self, name_to_item_map[DrvrSeatSwtAdjmtOfSpplFctVertSts.fdx_name])
        self.DrvrSeatSwtHeiFrntSts = DrvrSeatSwtHeiFrntSts(self, name_to_item_map[DrvrSeatSwtHeiFrntSts.fdx_name])
        self.DrvrSeatSwtHeiSts = DrvrSeatSwtHeiSts(self, name_to_item_map[DrvrSeatSwtHeiSts.fdx_name])
        self.DrvrSeatSwtInclSts = DrvrSeatSwtInclSts(self, name_to_item_map[DrvrSeatSwtInclSts.fdx_name])
        self.DrvrSeatSwtSelnOfSpplFctSts = DrvrSeatSwtSelnOfSpplFctSts(self, name_to_item_map[DrvrSeatSwtSelnOfSpplFctSts.fdx_name])
        self.DrvrSeatSwtSldSts = DrvrSeatSwtSldSts(self, name_to_item_map[DrvrSeatSwtSldSts.fdx_name])
        self.DrvrWLoadLvl = DrvrWLoadLvl(self, name_to_item_map[DrvrWLoadLvl.fdx_name])
        self.DstLong = DstLong(self, name_to_item_map[DstLong.fdx_name])
        self.DstNotifSts = DstNotifSts(self, name_to_item_map[DstNotifSts.fdx_name])
        self.DstSho = DstSho(self, name_to_item_map[DstSho.fdx_name])
        self.DstToEmpty = DstToEmpty(self, name_to_item_map[DstToEmpty.fdx_name])
        self.DstToManvInDstToManvLocn = DstToManvInDstToManvLocn(self, name_to_item_map[DstToManvInDstToManvLocn.fdx_name])
        self.DstToSrv = DstToSrv(self, name_to_item_map[DstToSrv.fdx_name])
        self.DstTrvld1 = DstTrvld1(self, name_to_item_map[DstTrvld1.fdx_name])
        self.DstTrvld2 = DstTrvld2(self, name_to_item_map[DstTrvld2.fdx_name])
        self.DstTrvldHiResl = DstTrvldHiResl(self, name_to_item_map[DstTrvldHiResl.fdx_name])
        self.DstTrvldMst2 = DstTrvldMst2(self, name_to_item_map[DstTrvldMst2.fdx_name])
        self.DstUnit = DstUnit(self, name_to_item_map[DstUnit.fdx_name])
        self.EasyInOutDrvrSeatAdjmtPen = EasyInOutDrvrSeatAdjmtPen(self, name_to_item_map[EasyInOutDrvrSeatAdjmtPen.fdx_name])
        self.EasyInOutDrvrSeatAdjmtSts = EasyInOutDrvrSeatAdjmtSts(self, name_to_item_map[EasyInOutDrvrSeatAdjmtSts.fdx_name])
        self.EgyLvlElecMai = EgyLvlElecMai(self, name_to_item_map[EgyLvlElecMai.fdx_name])
        self.EgyLvlElecSubtyp = EgyLvlElecSubtyp(self, name_to_item_map[EgyLvlElecSubtyp.fdx_name])
        self.EgySave = EgySave(self, name_to_item_map[EgySave.fdx_name])
        self.EmgyAsscSts = EmgyAsscSts(self, name_to_item_map[EmgyAsscSts.fdx_name])
        self.EmgyVehWarnSts = EmgyVehWarnSts(self, name_to_item_map[EmgyVehWarnSts.fdx_name])
        self.EngCooltIndcnReq = EngCooltIndcnReq(self, name_to_item_map[EngCooltIndcnReq.fdx_name])
        self.EngCooltLvl = EngCooltLvl(self, name_to_item_map[EngCooltLvl.fdx_name])
        self.EngFuCns = EngFuCns(self, name_to_item_map[EngFuCns.fdx_name])
        self.EngFuCnsFild = EngFuCnsFild(self, name_to_item_map[EngFuCnsFild.fdx_name])
        self.EngHrToSrv = EngHrToSrv(self, name_to_item_map[EngHrToSrv.fdx_name])
        self.EngN = EngN(self, name_to_item_map[EngN.fdx_name])
        self.EngNChks = EngNChks(self, name_to_item_map[EngNChks.fdx_name])
        self.EngNCntr = EngNCntr(self, name_to_item_map[EngNCntr.fdx_name])
        self.EngNSafeEngNGrdt = EngNSafeEngNGrdt(self, name_to_item_map[EngNSafeEngNGrdt.fdx_name])
        self.EngOilLvl = EngOilLvl(self, name_to_item_map[EngOilLvl.fdx_name])
        self.EngOilLvlSts = EngOilLvlSts(self, name_to_item_map[EngOilLvlSts.fdx_name])
        self.EngOilPWarn = EngOilPWarn(self, name_to_item_map[EngOilPWarn.fdx_name])
        self.EngSpdDispd = EngSpdDispd(self, name_to_item_map[EngSpdDispd.fdx_name])
        self.EngStrtStopSetg = EngStrtStopSetg(self, name_to_item_map[EngStrtStopSetg.fdx_name])
        self.EpbLampReq = EpbLampReq(self, name_to_item_map[EpbLampReq.fdx_name])
        self.EpbLampReqChks = EpbLampReqChks(self, name_to_item_map[EpbLampReqChks.fdx_name])
        self.EpbLampReqCntr = EpbLampReqCntr(self, name_to_item_map[EpbLampReqCntr.fdx_name])
        self.EscSt = EscSt(self, name_to_item_map[EscSt.fdx_name])
        self.EscStChks = EscStChks(self, name_to_item_map[EscStChks.fdx_name])
        self.EscStCntr = EscStCntr(self, name_to_item_map[EscStCntr.fdx_name])
        self.EscWarnIndcnReq = EscWarnIndcnReq(self, name_to_item_map[EscWarnIndcnReq.fdx_name])
        self.EscWarnIndcnReqChks = EscWarnIndcnReqChks(self, name_to_item_map[EscWarnIndcnReqChks.fdx_name])
        self.EscWarnIndcnReqCntr = EscWarnIndcnReqCntr(self, name_to_item_map[EscWarnIndcnReqCntr.fdx_name])
        self.ExtrMirrFoldSetgPen = ExtrMirrFoldSetgPen(self, name_to_item_map[ExtrMirrFoldSetgPen.fdx_name])
        self.ExtrMirrFoldSetgSts = ExtrMirrFoldSetgSts(self, name_to_item_map[ExtrMirrFoldSetgSts.fdx_name])
        self.ExtrMirrTiltSetg2IdPen = ExtrMirrTiltSetg2IdPen(self, name_to_item_map[ExtrMirrTiltSetg2IdPen.fdx_name])
        self.ExtrMirrTiltSetg2MirrDrvr = ExtrMirrTiltSetg2MirrDrvr(self, name_to_item_map[ExtrMirrTiltSetg2MirrDrvr.fdx_name])
        self.ExtrMirrTiltSetg2MirrPass = ExtrMirrTiltSetg2MirrPass(self, name_to_item_map[ExtrMirrTiltSetg2MirrPass.fdx_name])
        self.FanForCmptmtTRunng = FanForCmptmtTRunng(self, name_to_item_map[FanForCmptmtTRunng.fdx_name])
        self.FanForCmptmtTRunng = FanForCmptmtTRunng(self, name_to_item_map[FanForCmptmtTRunng.fdx_name])
        self.FltEgyCnsWdSts = FltEgyCnsWdSts(self, name_to_item_map[FltEgyCnsWdSts.fdx_name])
        self.FltIndcrTurnLeFrnt = FltIndcrTurnLeFrnt(self, name_to_item_map[FltIndcrTurnLeFrnt.fdx_name])
        self.FltIndcrTurnLeRe = FltIndcrTurnLeRe(self, name_to_item_map[FltIndcrTurnLeRe.fdx_name])
        self.FltIndcrTurnRiFrnt = FltIndcrTurnRiFrnt(self, name_to_item_map[FltIndcrTurnRiFrnt.fdx_name])
        self.FltIndcrTurnRiRe = FltIndcrTurnRiRe(self, name_to_item_map[FltIndcrTurnRiRe.fdx_name])
        self.FltOfLiDaytiRunngRi = FltOfLiDaytiRunngRi(self, name_to_item_map[FltOfLiDaytiRunngRi.fdx_name])
        self.FRNetworkStatus = FRNetworkStatus(self, name_to_item_map[FRNetworkStatus.fdx_name])
        self.FrntAxleWarn = FrntAxleWarn(self, name_to_item_map[FrntAxleWarn.fdx_name])
        self.FrntWiprLvrReq2FrntWiprLvrCmd1 = FrntWiprLvrReq2FrntWiprLvrCmd1(self, name_to_item_map[FrntWiprLvrReq2FrntWiprLvrCmd1.fdx_name])
        self.FrntWiprLvrReq2FrntWiprLvrCntr = FrntWiprLvrReq2FrntWiprLvrCntr(self, name_to_item_map[FrntWiprLvrReq2FrntWiprLvrCntr.fdx_name])
        self.FrntWiprLvrReq2FrntWiprLvrCrc = FrntWiprLvrReq2FrntWiprLvrCrc(self, name_to_item_map[FrntWiprLvrReq2FrntWiprLvrCrc.fdx_name])
        self.FrntWiprLvrReq2FrntWiprLvrQf = FrntWiprLvrReq2FrntWiprLvrQf(self, name_to_item_map[FrntWiprLvrReq2FrntWiprLvrQf.fdx_name])
        self.FuCnsUnit = FuCnsUnit(self, name_to_item_map[FuCnsUnit.fdx_name])
        self.FuHeatrActv = FuHeatrActv(self, name_to_item_map[FuHeatrActv.fdx_name])
        self.FuHeatrFuCns1 = FuHeatrFuCns1(self, name_to_item_map[FuHeatrFuCns1.fdx_name])
        self.FuHeatrFuCnsDurgCyc1 = FuHeatrFuCnsDurgCyc1(self, name_to_item_map[FuHeatrFuCnsDurgCyc1.fdx_name])
        self.FuLvlIndcdQly = FuLvlIndcdQly(self, name_to_item_map[FuLvlIndcdQly.fdx_name])
        self.FuLvlIndcdVal = FuLvlIndcdVal(self, name_to_item_map[FuLvlIndcdVal.fdx_name])
        self.FuLvlLoIndcn = FuLvlLoIndcn(self, name_to_item_map[FuLvlLoIndcn.fdx_name])
        self.FuLvlLoWarn = FuLvlLoWarn(self, name_to_item_map[FuLvlLoWarn.fdx_name])
        self.GearIndcn = GearIndcn(self, name_to_item_map[GearIndcn.fdx_name])
        self.GearShiftIndcn = GearShiftIndcn(self, name_to_item_map[GearShiftIndcn.fdx_name])
        self.GearTarIndcn = GearTarIndcn(self, name_to_item_map[GearTarIndcn.fdx_name])
        self.GrdForFuEco = GrdForFuEco(self, name_to_item_map[GrdForFuEco.fdx_name])
        self.HdrestFoldReq2 = HdrestFoldReq2(self, name_to_item_map[HdrestFoldReq2.fdx_name])
        self.HeatrDurgDrvgReqd = HeatrDurgDrvgReqd(self, name_to_item_map[HeatrDurgDrvgReqd.fdx_name])
        self.HiQlyInTireCircumCalByNav = HiQlyInTireCircumCalByNav(self, name_to_item_map[HiQlyInTireCircumCalByNav.fdx_name])
        self.HmiCmptmtAirDistbnFrnt = HmiCmptmtAirDistbnFrnt(self, name_to_item_map[HmiCmptmtAirDistbnFrnt.fdx_name])
        self.HmiCmptmtCoolgReq = HmiCmptmtCoolgReq(self, name_to_item_map[HmiCmptmtCoolgReq.fdx_name])
        self.HmiCmptmtTSpForRowFirstLe = HmiCmptmtTSpForRowFirstLe(self, name_to_item_map[HmiCmptmtTSpForRowFirstLe.fdx_name])
        self.HmiCmptmtTSpForRowFirstRi = HmiCmptmtTSpForRowFirstRi(self, name_to_item_map[HmiCmptmtTSpForRowFirstRi.fdx_name])
        self.HmiCmptmtTSpForRowSecLe = HmiCmptmtTSpForRowSecLe(self, name_to_item_map[HmiCmptmtTSpForRowSecLe.fdx_name])
        self.HmiCmptmtTSpForRowSecRi = HmiCmptmtTSpForRowSecRi(self, name_to_item_map[HmiCmptmtTSpForRowSecRi.fdx_name])
        self.HmiCmptmtTSpSpclForRowFirstLe = HmiCmptmtTSpSpclForRowFirstLe(self, name_to_item_map[HmiCmptmtTSpSpclForRowFirstLe.fdx_name])
        self.HmiCmptmtTSpSpclForRowFirstRi = HmiCmptmtTSpSpclForRowFirstRi(self, name_to_item_map[HmiCmptmtTSpSpclForRowFirstRi.fdx_name])
        self.HmiCmptmtTSpSpclForRowSecLe = HmiCmptmtTSpSpclForRowSecLe(self, name_to_item_map[HmiCmptmtTSpSpclForRowSecLe.fdx_name])
        self.HmiCmptmtTSpSpclForRowSecRi = HmiCmptmtTSpSpclForRowSecRi(self, name_to_item_map[HmiCmptmtTSpSpclForRowSecRi.fdx_name])
        self.HmiDefrstElecForMirrReq = HmiDefrstElecForMirrReq(self, name_to_item_map[HmiDefrstElecForMirrReq.fdx_name])
        self.HmiDefrstElecFrntReq = HmiDefrstElecFrntReq(self, name_to_item_map[HmiDefrstElecFrntReq.fdx_name])
        self.HmiDefrstElecReReq = HmiDefrstElecReReq(self, name_to_item_map[HmiDefrstElecReReq.fdx_name])
        self.HmiDefrstFrntSts = HmiDefrstFrntSts(self, name_to_item_map[HmiDefrstFrntSts.fdx_name])
        self.HmiDefrstMaxReq = HmiDefrstMaxReq(self, name_to_item_map[HmiDefrstMaxReq.fdx_name])
        self.HmiDfrstReSts = HmiDfrstReSts(self, name_to_item_map[HmiDfrstReSts.fdx_name])
        self.HmiHvacFanLvlFrnt = HmiHvacFanLvlFrnt(self, name_to_item_map[HmiHvacFanLvlFrnt.fdx_name])
        self.HmiHvacFanLvlRe = HmiHvacFanLvlRe(self, name_to_item_map[HmiHvacFanLvlRe.fdx_name])
        self.HmiHvacRecircCmd = HmiHvacRecircCmd(self, name_to_item_map[HmiHvacRecircCmd.fdx_name])
        self.HmiHvacReCtrl = HmiHvacReCtrl(self, name_to_item_map[HmiHvacReCtrl.fdx_name])
        self.HmiMirrDefrstSts = HmiMirrDefrstSts(self, name_to_item_map[HmiMirrDefrstSts.fdx_name])
        self.HmiSeatClimaExtdHmiSeatVentnForRowSecLe = HmiSeatClimaExtdHmiSeatVentnForRowSecLe(self, name_to_item_map[HmiSeatClimaExtdHmiSeatVentnForRowSecLe.fdx_name])
        self.HmiSeatClimaExtdHmiSeatVentnForRowSecRi = HmiSeatClimaExtdHmiSeatVentnForRowSecRi(self, name_to_item_map[HmiSeatClimaExtdHmiSeatVentnForRowSecRi.fdx_name])
        self.HmiSeatHeatgForRowFirstLe = HmiSeatHeatgForRowFirstLe(self, name_to_item_map[HmiSeatHeatgForRowFirstLe.fdx_name])
        self.HmiSeatHeatgForRowFirstRi = HmiSeatHeatgForRowFirstRi(self, name_to_item_map[HmiSeatHeatgForRowFirstRi.fdx_name])
        self.HmiSeatHeatgForRowSecLe = HmiSeatHeatgForRowSecLe(self, name_to_item_map[HmiSeatHeatgForRowSecLe.fdx_name])
        self.HmiSeatHeatgForRowSecRi = HmiSeatHeatgForRowSecRi(self, name_to_item_map[HmiSeatHeatgForRowSecRi.fdx_name])
        self.HmiSeatVentnForRowFirstLe = HmiSeatVentnForRowFirstLe(self, name_to_item_map[HmiSeatVentnForRowFirstLe.fdx_name])
        self.HmiSeatVentnForRowFirstRi = HmiSeatVentnForRowFirstRi(self, name_to_item_map[HmiSeatVentnForRowFirstRi.fdx_name])
        self.HoodSts = HoodSts(self, name_to_item_map[HoodSts.fdx_name])
        self.Hr = Hr(self, name_to_item_map[Hr.fdx_name])
        self.Hr1 = Hr1(self, name_to_item_map[Hr1.fdx_name])
        self.HudActvReqPen = HudActvReqPen(self, name_to_item_map[HudActvReqPen.fdx_name])
        self.HudActvReqSts = HudActvReqSts(self, name_to_item_map[HudActvReqSts.fdx_name])
        self.HudActvSts = HudActvSts(self, name_to_item_map[HudActvSts.fdx_name])
        self.HudAdjmtReq = HudAdjmtReq(self, name_to_item_map[HudAdjmtReq.fdx_name])
        self.HudDiagcHudCircShoSts = HudDiagcHudCircShoSts(self, name_to_item_map[HudDiagcHudCircShoSts.fdx_name])
        self.HudDiagcHudCricOpenSts = HudDiagcHudCricOpenSts(self, name_to_item_map[HudDiagcHudCricOpenSts.fdx_name])
        self.HudDiagcHudTSts = HudDiagcHudTSts(self, name_to_item_map[HudDiagcHudTSts.fdx_name])
        self.HudDiagcImgHudErrSts = HudDiagcImgHudErrSts(self, name_to_item_map[HudDiagcImgHudErrSts.fdx_name])
        self.HudDiagcImgHudTmpNotAvlSts = HudDiagcImgHudTmpNotAvlSts(self, name_to_item_map[HudDiagcImgHudTmpNotAvlSts.fdx_name])
        self.HudErgoSetgReq = HudErgoSetgReq(self, name_to_item_map[HudErgoSetgReq.fdx_name])
        self.HudSts = HudSts(self, name_to_item_map[HudSts.fdx_name])
        self.HudVisFctSetgHudFct00 = HudVisFctSetgHudFct00(self, name_to_item_map[HudVisFctSetgHudFct00.fdx_name])
        self.HudVisFctSetgHudFct01 = HudVisFctSetgHudFct01(self, name_to_item_map[HudVisFctSetgHudFct01.fdx_name])
        self.HudVisFctSetgHudFct02 = HudVisFctSetgHudFct02(self, name_to_item_map[HudVisFctSetgHudFct02.fdx_name])
        self.HudVisFctSetgHudFct03 = HudVisFctSetgHudFct03(self, name_to_item_map[HudVisFctSetgHudFct03.fdx_name])
        self.HudVisFctSetgHudFct04 = HudVisFctSetgHudFct04(self, name_to_item_map[HudVisFctSetgHudFct04.fdx_name])
        self.HudVisFctSetgHudFct05 = HudVisFctSetgHudFct05(self, name_to_item_map[HudVisFctSetgHudFct05.fdx_name])
        self.HudVisFctSetgHudFct06 = HudVisFctSetgHudFct06(self, name_to_item_map[HudVisFctSetgHudFct06.fdx_name])
        self.HudVisFctSetgHudFct07 = HudVisFctSetgHudFct07(self, name_to_item_map[HudVisFctSetgHudFct07.fdx_name])
        self.HudVisFctSetgHudFct08 = HudVisFctSetgHudFct08(self, name_to_item_map[HudVisFctSetgHudFct08.fdx_name])
        self.HudVisFctSetgHudFct09 = HudVisFctSetgHudFct09(self, name_to_item_map[HudVisFctSetgHudFct09.fdx_name])
        self.HudVisFctSetgHudFct10 = HudVisFctSetgHudFct10(self, name_to_item_map[HudVisFctSetgHudFct10.fdx_name])
        self.HudVisFctSetgHudFct11 = HudVisFctSetgHudFct11(self, name_to_item_map[HudVisFctSetgHudFct11.fdx_name])
        self.HudVisFctSetgHudFct12 = HudVisFctSetgHudFct12(self, name_to_item_map[HudVisFctSetgHudFct12.fdx_name])
        self.HudVisFctSetgHudFct13 = HudVisFctSetgHudFct13(self, name_to_item_map[HudVisFctSetgHudFct13.fdx_name])
        self.HudVisFctSetgHudFct14 = HudVisFctSetgHudFct14(self, name_to_item_map[HudVisFctSetgHudFct14.fdx_name])
        self.HudVisFctSetgHudFct15 = HudVisFctSetgHudFct15(self, name_to_item_map[HudVisFctSetgHudFct15.fdx_name])
        self.HudVisFctSetgHudFct16 = HudVisFctSetgHudFct16(self, name_to_item_map[HudVisFctSetgHudFct16.fdx_name])
        self.HudVisFctSetgHudFct17 = HudVisFctSetgHudFct17(self, name_to_item_map[HudVisFctSetgHudFct17.fdx_name])
        self.HudVisFctSetgHudFct18 = HudVisFctSetgHudFct18(self, name_to_item_map[HudVisFctSetgHudFct18.fdx_name])
        self.HudVisFctSetgHudFct19 = HudVisFctSetgHudFct19(self, name_to_item_map[HudVisFctSetgHudFct19.fdx_name])
        self.HudVisFctSetgPen = HudVisFctSetgPen(self, name_to_item_map[HudVisFctSetgPen.fdx_name])
        self.HvacAirMFlowEstimd = HvacAirMFlowEstimd(self, name_to_item_map[HvacAirMFlowEstimd.fdx_name])
        self.HznRstExtd = HznRstExtd(self, name_to_item_map[HznRstExtd.fdx_name])
        self.HzrdLiWarnSts = HzrdLiWarnSts(self, name_to_item_map[HzrdLiWarnSts.fdx_name])
        self.IdPenForUnits = IdPenForUnits(self, name_to_item_map[IdPenForUnits.fdx_name])
        self.IndcnOfParkAssiSts = IndcnOfParkAssiSts(self, name_to_item_map[IndcnOfParkAssiSts.fdx_name])
        self.IndcnOfPrkgAutSts = IndcnOfPrkgAutSts(self, name_to_item_map[IndcnOfPrkgAutSts.fdx_name])
        self.IndcnUnitDateFmt = IndcnUnitDateFmt(self, name_to_item_map[IndcnUnitDateFmt.fdx_name])
        self.IndcrDisp1WdSts = IndcrDisp1WdSts(self, name_to_item_map[IndcrDisp1WdSts.fdx_name])
        self.IndcrTurnSts1WdSts = IndcrTurnSts1WdSts(self, name_to_item_map[IndcrTurnSts1WdSts.fdx_name])
        self.IniValSigCfgIDBackboneFR = IniValSigCfgIDBackboneFR(self, name_to_item_map[IniValSigCfgIDBackboneFR.fdx_name])
        self.IntrBriSts = IntrBriSts(self, name_to_item_map[IntrBriSts.fdx_name])
        self.IntrLiAmbLiSetgForLiInten = IntrLiAmbLiSetgForLiInten(self, name_to_item_map[IntrLiAmbLiSetgForLiInten.fdx_name])
        self.IntrLiAmbLiSetgForLiTintg = IntrLiAmbLiSetgForLiTintg(self, name_to_item_map[IntrLiAmbLiSetgForLiTintg.fdx_name])
        self.IntrLiAmbLiSetgPen = IntrLiAmbLiSetgPen(self, name_to_item_map[IntrLiAmbLiSetgPen.fdx_name])
        self.IntrLiSurrndgsLiSetgForLiInten = IntrLiSurrndgsLiSetgForLiInten(self, name_to_item_map[IntrLiSurrndgsLiSetgForLiInten.fdx_name])
        self.IntrLiSurrndgsLiSetgForLiLvl = IntrLiSurrndgsLiSetgForLiLvl(self, name_to_item_map[IntrLiSurrndgsLiSetgForLiLvl.fdx_name])
        self.IntrLiSurrndgsLiSetgPen = IntrLiSurrndgsLiSetgPen(self, name_to_item_map[IntrLiSurrndgsLiSetgPen.fdx_name])
        self.IntrMirrTintgSetgPen = IntrMirrTintgSetgPen(self, name_to_item_map[IntrMirrTintgSetgPen.fdx_name])
        self.IntrMirrTintgSetgSts = IntrMirrTintgSetgSts(self, name_to_item_map[IntrMirrTintgSetgSts.fdx_name])
        self.iTPMSCalPsbl = iTPMSCalPsbl(self, name_to_item_map[iTPMSCalPsbl.fdx_name])
        self.iTPMSCalSts = iTPMSCalSts(self, name_to_item_map[iTPMSCalSts.fdx_name])
        self.iTPMSTirePMSts = iTPMSTirePMSts(self, name_to_item_map[iTPMSTirePMSts.fdx_name])
        self.KeyLostWarnIndcn = KeyLostWarnIndcn(self, name_to_item_map[KeyLostWarnIndcn.fdx_name])
        self.KeyProfMpgUpdForIdPen = KeyProfMpgUpdForIdPen(self, name_to_item_map[KeyProfMpgUpdForIdPen.fdx_name])
        self.KeyProfMpgUpdKeyProfMpgUpdOff = KeyProfMpgUpdKeyProfMpgUpdOff(self, name_to_item_map[KeyProfMpgUpdKeyProfMpgUpdOff.fdx_name])
        self.KeyReadReqFromSetgMgr = KeyReadReqFromSetgMgr(self, name_to_item_map[KeyReadReqFromSetgMgr.fdx_name])
        self.KeyReadStsToProfCtrlBoolean = KeyReadStsToProfCtrlBoolean(self, name_to_item_map[KeyReadStsToProfCtrlBoolean.fdx_name])
        self.KeyReadStsToProfCtrlKeyId = KeyReadStsToProfCtrlKeyId(self, name_to_item_map[KeyReadStsToProfCtrlKeyId.fdx_name])
        self.KeyRmnIndcn = KeyRmnIndcn(self, name_to_item_map[KeyRmnIndcn.fdx_name])
        self.KeySpdWarn = KeySpdWarn(self, name_to_item_map[KeySpdWarn.fdx_name])
        self.LampSuppSrv = LampSuppSrv(self, name_to_item_map[LampSuppSrv.fdx_name])
        self.LaneChgWarnActvPen = LaneChgWarnActvPen(self, name_to_item_map[LaneChgWarnActvPen.fdx_name])
        self.LaneChgWarnActvSts = LaneChgWarnActvSts(self, name_to_item_map[LaneChgWarnActvSts.fdx_name])
        self.LaneChgWarnSts = LaneChgWarnSts(self, name_to_item_map[LaneChgWarnSts.fdx_name])
        self.LaneDetnStsForAutDrv = LaneDetnStsForAutDrv(self, name_to_item_map[LaneDetnStsForAutDrv.fdx_name])
        self.LaneDetnStsForLaneKeepAid = LaneDetnStsForLaneKeepAid(self, name_to_item_map[LaneDetnStsForLaneKeepAid.fdx_name])
        self.LaneKeepAidActvPen = LaneKeepAidActvPen(self, name_to_item_map[LaneKeepAidActvPen.fdx_name])
        self.LaneKeepAidActvSts = LaneKeepAidActvSts(self, name_to_item_map[LaneKeepAidActvSts.fdx_name])
        self.LaneKeepAidSts = LaneKeepAidSts(self, name_to_item_map[LaneKeepAidSts.fdx_name])
        self.LatMovmtWarn = LatMovmtWarn(self, name_to_item_map[LatMovmtWarn.fdx_name])
        self.LcmaOn1 = LcmaOn1(self, name_to_item_map[LcmaOn1.fdx_name])
        self.LiAdpvReqPen = LiAdpvReqPen(self, name_to_item_map[LiAdpvReqPen.fdx_name])
        self.LiAdpvReqSts = LiAdpvReqSts(self, name_to_item_map[LiAdpvReqSts.fdx_name])
        self.LiAutTranReqPen = LiAutTranReqPen(self, name_to_item_map[LiAutTranReqPen.fdx_name])
        self.LiAutTranReqSts = LiAutTranReqSts(self, name_to_item_map[LiAutTranReqSts.fdx_name])
        self.LiBeamHiAuxReqPen = LiBeamHiAuxReqPen(self, name_to_item_map[LiBeamHiAuxReqPen.fdx_name])
        self.LiBeamHiAuxReqSts = LiBeamHiAuxReqSts(self, name_to_item_map[LiBeamHiAuxReqSts.fdx_name])
        self.LiCornrgReqPen = LiCornrgReqPen(self, name_to_item_map[LiCornrgReqPen.fdx_name])
        self.LiCornrgReqSts = LiCornrgReqSts(self, name_to_item_map[LiCornrgReqSts.fdx_name])
        self.LiDaytiRunngReqPen = LiDaytiRunngReqPen(self, name_to_item_map[LiDaytiRunngReqPen.fdx_name])
        self.LiDaytiRunngReqSts = LiDaytiRunngReqSts(self, name_to_item_map[LiDaytiRunngReqSts.fdx_name])
        self.LiDrvrFltIndcrTurn = LiDrvrFltIndcrTurn(self, name_to_item_map[LiDrvrFltIndcrTurn.fdx_name])
        self.LiExtReq1WdReq5IdPen = LiExtReq1WdReq5IdPen(self, name_to_item_map[LiExtReq1WdReq5IdPen.fdx_name])
        self.LiExtReq1WdReq5SlowNormFast = LiExtReq1WdReq5SlowNormFast(self, name_to_item_map[LiExtReq1WdReq5SlowNormFast.fdx_name])
        self.LiHomeLvngReqPen = LiHomeLvngReqPen(self, name_to_item_map[LiHomeLvngReqPen.fdx_name])
        self.LiHomeLvngReqSts = LiHomeLvngReqSts(self, name_to_item_map[LiHomeLvngReqSts.fdx_name])
        self.LiHomeSafeReqPen = LiHomeSafeReqPen(self, name_to_item_map[LiHomeSafeReqPen.fdx_name])
        self.LiHomeSafeReqSts = LiHomeSafeReqSts(self, name_to_item_map[LiHomeSafeReqSts.fdx_name])
        self.LiLvrSwt1 = LiLvrSwt1(self, name_to_item_map[LiLvrSwt1.fdx_name])
        self.LiPassFltIndcrTurn = LiPassFltIndcrTurn(self, name_to_item_map[LiPassFltIndcrTurn.fdx_name])
        self.LiSeldForDrvrPfmncMonPen = LiSeldForDrvrPfmncMonPen(self, name_to_item_map[LiSeldForDrvrPfmncMonPen.fdx_name])
        self.LiSeldForDrvrPfmncMonSts = LiSeldForDrvrPfmncMonSts(self, name_to_item_map[LiSeldForDrvrPfmncMonSts.fdx_name])
        self.LiTrfcSideReq = LiTrfcSideReq(self, name_to_item_map[LiTrfcSideReq.fdx_name])
        self.LockgCenStsForUsrFb = LockgCenStsForUsrFb(self, name_to_item_map[LockgCenStsForUsrFb.fdx_name])
        self.LockgCenStsLockSt = LockgCenStsLockSt(self, name_to_item_map[LockgCenStsLockSt.fdx_name])
        self.LockgCenStsUpdEve = LockgCenStsUpdEve(self, name_to_item_map[LockgCenStsUpdEve.fdx_name])
        self.LockgCenTrigSrc = LockgCenTrigSrc(self, name_to_item_map[LockgCenTrigSrc.fdx_name])
        self.LockgFbSoundReqPen = LockgFbSoundReqPen(self, name_to_item_map[LockgFbSoundReqPen.fdx_name])
        self.LockgFbSoundReqSts = LockgFbSoundReqSts(self, name_to_item_map[LockgFbSoundReqSts.fdx_name])
        self.LockgFbVisReqPen = LockgFbVisReqPen(self, name_to_item_map[LockgFbVisReqPen.fdx_name])
        self.LockgFbVisReqSts = LockgFbVisReqSts(self, name_to_item_map[LockgFbVisReqSts.fdx_name])
        self.LockgPrsnlReqFromHmi = LockgPrsnlReqFromHmi(self, name_to_item_map[LockgPrsnlReqFromHmi.fdx_name])
        self.LockgPrsnlSts = LockgPrsnlSts(self, name_to_item_map[LockgPrsnlSts.fdx_name])
        self.LockSpdReqPen = LockSpdReqPen(self, name_to_item_map[LockSpdReqPen.fdx_name])
        self.LockSpdReqSts = LockSpdReqSts(self, name_to_item_map[LockSpdReqSts.fdx_name])
        self.LvlOfClimaCmft = LvlOfClimaCmft(self, name_to_item_map[LvlOfClimaCmft.fdx_name])
        self.MassgFctActvDrvrMassgFctActv = MassgFctActvDrvrMassgFctActv(self, name_to_item_map[MassgFctActvDrvrMassgFctActv.fdx_name])
        self.MassgFctActvPassMassgFctActv = MassgFctActvPassMassgFctActv(self, name_to_item_map[MassgFctActvPassMassgFctActv.fdx_name])
        self.MemBtnSound = MemBtnSound(self, name_to_item_map[MemBtnSound.fdx_name])
        self.Mins = Mins(self, name_to_item_map[Mins.fdx_name])
        self.Mins1 = Mins1(self, name_to_item_map[Mins1.fdx_name])
        self.MirrDwnStsAtDrvr = MirrDwnStsAtDrvr(self, name_to_item_map[MirrDwnStsAtDrvr.fdx_name])
        self.MirrDwnStsAtPass = MirrDwnStsAtPass(self, name_to_item_map[MirrDwnStsAtPass.fdx_name])
        self.MirrFoldStsAtDrvr = MirrFoldStsAtDrvr(self, name_to_item_map[MirrFoldStsAtDrvr.fdx_name])
        self.MirrFoldStsAtPass = MirrFoldStsAtPass(self, name_to_item_map[MirrFoldStsAtPass.fdx_name])
        self.MmedHmiModStd = MmedHmiModStd(self, name_to_item_map[MmedHmiModStd.fdx_name])
        self.MmedMaiPwrMod = MmedMaiPwrMod(self, name_to_item_map[MmedMaiPwrMod.fdx_name])
        self.MstCfgIDBackboneFR = MstCfgIDBackboneFR(self, name_to_item_map[MstCfgIDBackboneFR.fdx_name])
        self.Mth = Mth(self, name_to_item_map[Mth.fdx_name])
        self.Mth1 = Mth1(self, name_to_item_map[Mth1.fdx_name])
        self.NetCtrlrActvtPrio = NetCtrlrActvtPrio(self, name_to_item_map[NetCtrlrActvtPrio.fdx_name])
        self.NetCtrlrActvtResourceGroup = NetCtrlrActvtResourceGroup(self, name_to_item_map[NetCtrlrActvtResourceGroup.fdx_name])
        self.NetHdActvtPrio = NetHdActvtPrio(self, name_to_item_map[NetHdActvtPrio.fdx_name])
        self.NetHdActvtResourceGroup = NetHdActvtResourceGroup(self, name_to_item_map[NetHdActvtResourceGroup.fdx_name])
        self.NetTelmActvtPrio = NetTelmActvtPrio(self, name_to_item_map[NetTelmActvtPrio.fdx_name])
        self.NetTelmActvtResourceGroup = NetTelmActvtResourceGroup(self, name_to_item_map[NetTelmActvtResourceGroup.fdx_name])
        self.NewTripCdn = NewTripCdn(self, name_to_item_map[NewTripCdn.fdx_name])
        self.NoEntryWarnReq = NoEntryWarnReq(self, name_to_item_map[NoEntryWarnReq.fdx_name])
        self.NoOfSatltForSysNo1InPosnFromSatlt = NoOfSatltForSysNo1InPosnFromSatlt(self, name_to_item_map[NoOfSatltForSysNo1InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo2InPosnFromSatlt = NoOfSatltForSysNo2InPosnFromSatlt(self, name_to_item_map[NoOfSatltForSysNo2InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo3InPosnFromSatlt = NoOfSatltForSysNo3InPosnFromSatlt(self, name_to_item_map[NoOfSatltForSysNo3InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo4InPosnFromSatlt = NoOfSatltForSysNo4InPosnFromSatlt(self, name_to_item_map[NoOfSatltForSysNo4InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo5InPosnFromSatlt = NoOfSatltForSysNo5InPosnFromSatlt(self, name_to_item_map[NoOfSatltForSysNo5InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo6InPosnFromSatlt = NoOfSatltForSysNo6InPosnFromSatlt(self, name_to_item_map[NoOfSatltForSysNo6InPosnFromSatlt.fdx_name])
        self.NoSoundSys = NoSoundSys(self, name_to_item_map[NoSoundSys.fdx_name])
        self.NotifChkDistbn = NotifChkDistbn(self, name_to_item_map[NotifChkDistbn.fdx_name])
        self.NrOfBltAppld = NrOfBltAppld(self, name_to_item_map[NrOfBltAppld.fdx_name])
        self.NrOfKeyAvl = NrOfKeyAvl(self, name_to_item_map[NrOfKeyAvl.fdx_name])
        self.NrSerlDMSMNr1 = NrSerlDMSMNr1(self, name_to_item_map[NrSerlDMSMNr1.fdx_name])
        self.NrSerlDMSMNr2 = NrSerlDMSMNr2(self, name_to_item_map[NrSerlDMSMNr2.fdx_name])
        self.NrSerlDMSMNr3 = NrSerlDMSMNr3(self, name_to_item_map[NrSerlDMSMNr3.fdx_name])
        self.NrSerlDMSMNr4 = NrSerlDMSMNr4(self, name_to_item_map[NrSerlDMSMNr4.fdx_name])
        self.OffsForDrvrSpprtFctActvSts = OffsForDrvrSpprtFctActvSts(self, name_to_item_map[OffsForDrvrSpprtFctActvSts.fdx_name])
        self.ParkAssiDstFrntOfAudSideWarn = ParkAssiDstFrntOfAudSideWarn(self, name_to_item_map[ParkAssiDstFrntOfAudSideWarn.fdx_name])
        self.ParkAssiDstFrntOfAudWarnDir = ParkAssiDstFrntOfAudWarnDir(self, name_to_item_map[ParkAssiDstFrntOfAudWarnDir.fdx_name])
        self.ParkAssiDstFrntOfDstOfSnsrInsdRi = ParkAssiDstFrntOfDstOfSnsrInsdRi(self, name_to_item_map[ParkAssiDstFrntOfDstOfSnsrInsdRi.fdx_name])
        self.ParkAssiDstFrntOfDstOfSnsrLeSide = ParkAssiDstFrntOfDstOfSnsrLeSide(self, name_to_item_map[ParkAssiDstFrntOfDstOfSnsrLeSide.fdx_name])
        self.ParkAssiDstFrntOfDstOfSnsrOutdRi = ParkAssiDstFrntOfDstOfSnsrOutdRi(self, name_to_item_map[ParkAssiDstFrntOfDstOfSnsrOutdRi.fdx_name])
        self.ParkAssiDstFrntOfDstOfSnsrRiSide = ParkAssiDstFrntOfDstOfSnsrRiSide(self, name_to_item_map[ParkAssiDstFrntOfDstOfSnsrRiSide.fdx_name])
        self.ParkAssiDstFrntOfSnsrInsdLe = ParkAssiDstFrntOfSnsrInsdLe(self, name_to_item_map[ParkAssiDstFrntOfSnsrInsdLe.fdx_name])
        self.ParkAssiDstFrntOfSnsrOutdLe = ParkAssiDstFrntOfSnsrOutdLe(self, name_to_item_map[ParkAssiDstFrntOfSnsrOutdLe.fdx_name])
        self.ParkAssiDstReOfAudSideWarn = ParkAssiDstReOfAudSideWarn(self, name_to_item_map[ParkAssiDstReOfAudSideWarn.fdx_name])
        self.ParkAssiDstReOfAudWarnDir = ParkAssiDstReOfAudWarnDir(self, name_to_item_map[ParkAssiDstReOfAudWarnDir.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrInsdLe = ParkAssiDstReOfDstOfSnsrInsdLe(self, name_to_item_map[ParkAssiDstReOfDstOfSnsrInsdLe.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrInsdRi = ParkAssiDstReOfDstOfSnsrInsdRi(self, name_to_item_map[ParkAssiDstReOfDstOfSnsrInsdRi.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrLeSide = ParkAssiDstReOfDstOfSnsrLeSide(self, name_to_item_map[ParkAssiDstReOfDstOfSnsrLeSide.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrOutdLe = ParkAssiDstReOfDstOfSnsrOutdLe(self, name_to_item_map[ParkAssiDstReOfDstOfSnsrOutdLe.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrOutdRi = ParkAssiDstReOfDstOfSnsrOutdRi(self, name_to_item_map[ParkAssiDstReOfDstOfSnsrOutdRi.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrRiSide = ParkAssiDstReOfDstOfSnsrRiSide(self, name_to_item_map[ParkAssiDstReOfDstOfSnsrRiSide.fdx_name])
        self.PartNrDMSMEndSgn1 = PartNrDMSMEndSgn1(self, name_to_item_map[PartNrDMSMEndSgn1.fdx_name])
        self.PartNrDMSMEndSgn2 = PartNrDMSMEndSgn2(self, name_to_item_map[PartNrDMSMEndSgn2.fdx_name])
        self.PartNrDMSMEndSgn3 = PartNrDMSMEndSgn3(self, name_to_item_map[PartNrDMSMEndSgn3.fdx_name])
        self.PartNrDMSMNr1 = PartNrDMSMNr1(self, name_to_item_map[PartNrDMSMNr1.fdx_name])
        self.PartNrDMSMNr2 = PartNrDMSMNr2(self, name_to_item_map[PartNrDMSMNr2.fdx_name])
        self.PartNrDMSMNr3 = PartNrDMSMNr3(self, name_to_item_map[PartNrDMSMNr3.fdx_name])
        self.PartNrDMSMNr4 = PartNrDMSMNr4(self, name_to_item_map[PartNrDMSMNr4.fdx_name])
        self.PasAlrmDeactvnReq = PasAlrmDeactvnReq(self, name_to_item_map[PasAlrmDeactvnReq.fdx_name])
        self.PasAlrmSts = PasAlrmSts(self, name_to_item_map[PasAlrmSts.fdx_name])
        self.PassMassgRunng = PassMassgRunng(self, name_to_item_map[PassMassgRunng.fdx_name])
        self.PassSeatActvSpplFct = PassSeatActvSpplFct(self, name_to_item_map[PassSeatActvSpplFct.fdx_name])
        self.PassSeatDispMassgFctMassgInten = PassSeatDispMassgFctMassgInten(self, name_to_item_map[PassSeatDispMassgFctMassgInten.fdx_name])
        self.PassSeatDispMassgFctMassgProg = PassSeatDispMassgFctMassgProg(self, name_to_item_map[PassSeatDispMassgFctMassgProg.fdx_name])
        self.PassSeatDispMassgFctMassgSpdLvl = PassSeatDispMassgFctMassgSpdLvl(self, name_to_item_map[PassSeatDispMassgFctMassgSpdLvl.fdx_name])
        self.PassSeatDispMassgFctOnOff = PassSeatDispMassgFctOnOff(self, name_to_item_map[PassSeatDispMassgFctOnOff.fdx_name])
        self.PassSeatDispSpplFct = PassSeatDispSpplFct(self, name_to_item_map[PassSeatDispSpplFct.fdx_name])
        self.PassSeatMassgFctMassgInten = PassSeatMassgFctMassgInten(self, name_to_item_map[PassSeatMassgFctMassgInten.fdx_name])
        self.PassSeatMassgFctMassgProg = PassSeatMassgFctMassgProg(self, name_to_item_map[PassSeatMassgFctMassgProg.fdx_name])
        self.PassSeatMassgFctMassgSpdLvl = PassSeatMassgFctMassgSpdLvl(self, name_to_item_map[PassSeatMassgFctMassgSpdLvl.fdx_name])
        self.PassSeatMassgFctOnOff = PassSeatMassgFctOnOff(self, name_to_item_map[PassSeatMassgFctOnOff.fdx_name])
        self.PassSeatSts = PassSeatSts(self, name_to_item_map[PassSeatSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts = PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts(self, name_to_item_map[PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts = PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts(self, name_to_item_map[PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtHeiFrntSts = PassSeatSwtSts2PassSeatSwtHeiFrntSts(self, name_to_item_map[PassSeatSwtSts2PassSeatSwtHeiFrntSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtHeiSts = PassSeatSwtSts2PassSeatSwtHeiSts(self, name_to_item_map[PassSeatSwtSts2PassSeatSwtHeiSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtInclSts = PassSeatSwtSts2PassSeatSwtInclSts(self, name_to_item_map[PassSeatSwtSts2PassSeatSwtInclSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts = PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts(self, name_to_item_map[PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtSldSts = PassSeatSwtSts2PassSeatSwtSldSts(self, name_to_item_map[PassSeatSwtSts2PassSeatSwtSldSts.fdx_name])
        self.PinionSteerAg1 = PinionSteerAg1(self, name_to_item_map[PinionSteerAg1.fdx_name])
        self.PinionSteerAg1Qf = PinionSteerAg1Qf(self, name_to_item_map[PinionSteerAg1Qf.fdx_name])
        self.PosnAltiInPosnFromSatlt = PosnAltiInPosnFromSatlt(self, name_to_item_map[PosnAltiInPosnFromSatlt.fdx_name])
        self.PosnDirInPosnFromSatlt = PosnDirInPosnFromSatlt(self, name_to_item_map[PosnDirInPosnFromSatlt.fdx_name])
        self.PosnLatInPosnFromSatlt = PosnLatInPosnFromSatlt(self, name_to_item_map[PosnLatInPosnFromSatlt.fdx_name])
        self.PosnLgtInPosnFromSatlt = PosnLgtInPosnFromSatlt(self, name_to_item_map[PosnLgtInPosnFromSatlt.fdx_name])
        self.PosnQlyInDstToManvLocn = PosnQlyInDstToManvLocn(self, name_to_item_map[PosnQlyInDstToManvLocn.fdx_name])
        self.PosnSpdInPosnFromSatlt = PosnSpdInPosnFromSatlt(self, name_to_item_map[PosnSpdInPosnFromSatlt.fdx_name])
        self.PosnVHozlInPosnFromSatlt = PosnVHozlInPosnFromSatlt(self, name_to_item_map[PosnVHozlInPosnFromSatlt.fdx_name])
        self.PosnVVertInPosnFromSatlt = PosnVVertInPosnFromSatlt(self, name_to_item_map[PosnVVertInPosnFromSatlt.fdx_name])
        self.PostDrvgClimaAvl = PostDrvgClimaAvl(self, name_to_item_map[PostDrvgClimaAvl.fdx_name])
        self.PreClngNotif = PreClngNotif(self, name_to_item_map[PreClngNotif.fdx_name])
        self.PreHozlDilInPosnFromSatlt = PreHozlDilInPosnFromSatlt(self, name_to_item_map[PreHozlDilInPosnFromSatlt.fdx_name])
        self.PrePosnDilInPosnFromSatlt = PrePosnDilInPosnFromSatlt(self, name_to_item_map[PrePosnDilInPosnFromSatlt.fdx_name])
        self.PreTiDilInPosnFromSatlt = PreTiDilInPosnFromSatlt(self, name_to_item_map[PreTiDilInPosnFromSatlt.fdx_name])
        self.PreVertDilInPosnFromSatlt = PreVertDilInPosnFromSatlt(self, name_to_item_map[PreVertDilInPosnFromSatlt.fdx_name])
        self.PrkgAssiFailr = PrkgAssiFailr(self, name_to_item_map[PrkgAssiFailr.fdx_name])
        self.PrkgAssiManvActvSts = PrkgAssiManvActvSts(self, name_to_item_map[PrkgAssiManvActvSts.fdx_name])
        self.PrkgAssiManvProgs = PrkgAssiManvProgs(self, name_to_item_map[PrkgAssiManvProgs.fdx_name])
        self.PrkgAssiSts = PrkgAssiSts(self, name_to_item_map[PrkgAssiSts.fdx_name])
        self.PrkgAutSts = PrkgAutSts(self, name_to_item_map[PrkgAutSts.fdx_name])
        self.PrkgCamSysAvlSts = PrkgCamSysAvlSts(self, name_to_item_map[PrkgCamSysAvlSts.fdx_name])
        self.PrkgTypVld = PrkgTypVld(self, name_to_item_map[PrkgTypVld.fdx_name])
        self.ProfActProf1 = ProfActProf1(self, name_to_item_map[ProfActProf1.fdx_name])
        self.ProfActProf10 = ProfActProf10(self, name_to_item_map[ProfActProf10.fdx_name])
        self.ProfActProf11 = ProfActProf11(self, name_to_item_map[ProfActProf11.fdx_name])
        self.ProfActProf12 = ProfActProf12(self, name_to_item_map[ProfActProf12.fdx_name])
        self.ProfActProf2 = ProfActProf2(self, name_to_item_map[ProfActProf2.fdx_name])
        self.ProfActProf3 = ProfActProf3(self, name_to_item_map[ProfActProf3.fdx_name])
        self.ProfActProf4 = ProfActProf4(self, name_to_item_map[ProfActProf4.fdx_name])
        self.ProfActProf5 = ProfActProf5(self, name_to_item_map[ProfActProf5.fdx_name])
        self.ProfActProf6 = ProfActProf6(self, name_to_item_map[ProfActProf6.fdx_name])
        self.ProfActProf7 = ProfActProf7(self, name_to_item_map[ProfActProf7.fdx_name])
        self.ProfActProf8 = ProfActProf8(self, name_to_item_map[ProfActProf8.fdx_name])
        self.ProfActProf9 = ProfActProf9(self, name_to_item_map[ProfActProf9.fdx_name])
        self.ProfChg = ProfChg(self, name_to_item_map[ProfChg.fdx_name])
        self.ProfLimdProf1 = ProfLimdProf1(self, name_to_item_map[ProfLimdProf1.fdx_name])
        self.ProfLimdProf10 = ProfLimdProf10(self, name_to_item_map[ProfLimdProf10.fdx_name])
        self.ProfLimdProf11 = ProfLimdProf11(self, name_to_item_map[ProfLimdProf11.fdx_name])
        self.ProfLimdProf12 = ProfLimdProf12(self, name_to_item_map[ProfLimdProf12.fdx_name])
        self.ProfLimdProf2 = ProfLimdProf2(self, name_to_item_map[ProfLimdProf2.fdx_name])
        self.ProfLimdProf3 = ProfLimdProf3(self, name_to_item_map[ProfLimdProf3.fdx_name])
        self.ProfLimdProf4 = ProfLimdProf4(self, name_to_item_map[ProfLimdProf4.fdx_name])
        self.ProfLimdProf5 = ProfLimdProf5(self, name_to_item_map[ProfLimdProf5.fdx_name])
        self.ProfLimdProf6 = ProfLimdProf6(self, name_to_item_map[ProfLimdProf6.fdx_name])
        self.ProfLimdProf7 = ProfLimdProf7(self, name_to_item_map[ProfLimdProf7.fdx_name])
        self.ProfLimdProf8 = ProfLimdProf8(self, name_to_item_map[ProfLimdProf8.fdx_name])
        self.ProfLimdProf9 = ProfLimdProf9(self, name_to_item_map[ProfLimdProf9.fdx_name])
        self.ProfPenSts1 = ProfPenSts1(self, name_to_item_map[ProfPenSts1.fdx_name])
        self.PrpsnHvBattUsgModAct = PrpsnHvBattUsgModAct(self, name_to_item_map[PrpsnHvBattUsgModAct.fdx_name])
        self.PrpsnHvBattUsgModReq = PrpsnHvBattUsgModReq(self, name_to_item_map[PrpsnHvBattUsgModReq.fdx_name])
        self.PrpsnHvBattUsgOfChrgBlkd = PrpsnHvBattUsgOfChrgBlkd(self, name_to_item_map[PrpsnHvBattUsgOfChrgBlkd.fdx_name])
        self.PrpsnHvBattUsgOfChrgBlkd2 = PrpsnHvBattUsgOfChrgBlkd2(self, name_to_item_map[PrpsnHvBattUsgOfChrgBlkd2.fdx_name])
        self.PrpsnHvBattUsgOfHldBlkd = PrpsnHvBattUsgOfHldBlkd(self, name_to_item_map[PrpsnHvBattUsgOfHldBlkd.fdx_name])
        self.PrpsnHvBattUsgOfHldBlkd2 = PrpsnHvBattUsgOfHldBlkd2(self, name_to_item_map[PrpsnHvBattUsgOfHldBlkd2.fdx_name])
        self.PrpsnHvBattUsgOfHldSmtBlkd = PrpsnHvBattUsgOfHldSmtBlkd(self, name_to_item_map[PrpsnHvBattUsgOfHldSmtBlkd.fdx_name])
        self.PrpsnHvBattUsgOfHldSmtBlkd2 = PrpsnHvBattUsgOfHldSmtBlkd2(self, name_to_item_map[PrpsnHvBattUsgOfHldSmtBlkd2.fdx_name])
        self.PrpsnHvBattUsgOfHldSpd = PrpsnHvBattUsgOfHldSpd(self, name_to_item_map[PrpsnHvBattUsgOfHldSpd.fdx_name])
        self.PrpsnModElecDrvBlkd = PrpsnModElecDrvBlkd(self, name_to_item_map[PrpsnModElecDrvBlkd.fdx_name])
        self.PrpsnModOffroadBlkd = PrpsnModOffroadBlkd(self, name_to_item_map[PrpsnModOffroadBlkd.fdx_name])
        self.PrpsnModOfSaveBlkd = PrpsnModOfSaveBlkd(self, name_to_item_map[PrpsnModOfSaveBlkd.fdx_name])
        self.PrpsnModOfTracBlkd = PrpsnModOfTracBlkd(self, name_to_item_map[PrpsnModOfTracBlkd.fdx_name])
        self.PrpsnModSptBlkd = PrpsnModSptBlkd(self, name_to_item_map[PrpsnModSptBlkd.fdx_name])
        self.PtCluTqPtCluTq = PtCluTqPtCluTq(self, name_to_item_map[PtCluTqPtCluTq.fdx_name])
        self.PtCluTqPtCluTqDyn = PtCluTqPtCluTqDyn(self, name_to_item_map[PtCluTqPtCluTqDyn.fdx_name])
        self.PtCluTqPtCluTqQly = PtCluTqPtCluTqQly(self, name_to_item_map[PtCluTqPtCluTqQly.fdx_name])
        self.PtGearTar = PtGearTar(self, name_to_item_map[PtGearTar.fdx_name])
        self.PtTqAtAxleFrntAct = PtTqAtAxleFrntAct(self, name_to_item_map[PtTqAtAxleFrntAct.fdx_name])
        self.PtTqAtWhlFrntLeAct = PtTqAtWhlFrntLeAct(self, name_to_item_map[PtTqAtWhlFrntLeAct.fdx_name])
        self.PtTqAtWhlFrntRiAct = PtTqAtWhlFrntRiAct(self, name_to_item_map[PtTqAtWhlFrntRiAct.fdx_name])
        self.PtTqAtWhlsFrntQly = PtTqAtWhlsFrntQly(self, name_to_item_map[PtTqAtWhlsFrntQly.fdx_name])
        self.PUnit = PUnit(self, name_to_item_map[PUnit.fdx_name])
        self.PwrChrgDetdForPrkgHeatrElec = PwrChrgDetdForPrkgHeatrElec(self, name_to_item_map[PwrChrgDetdForPrkgHeatrElec.fdx_name])
        self.PwrLvlElecMai = PwrLvlElecMai(self, name_to_item_map[PwrLvlElecMai.fdx_name])
        self.PwrLvlElecSubtyp = PwrLvlElecSubtyp(self, name_to_item_map[PwrLvlElecSubtyp.fdx_name])
        self.PwrSplyErrSts = PwrSplyErrSts(self, name_to_item_map[PwrSplyErrSts.fdx_name])
        self.RainSenMemdReqPen = RainSenMemdReqPen(self, name_to_item_map[RainSenMemdReqPen.fdx_name])
        self.RainSenMemdReqSts = RainSenMemdReqSts(self, name_to_item_map[RainSenMemdReqSts.fdx_name])
        self.ReAxleWarn = ReAxleWarn(self, name_to_item_map[ReAxleWarn.fdx_name])
        self.RmnLockgPrsnlReq = RmnLockgPrsnlReq(self, name_to_item_map[RmnLockgPrsnlReq.fdx_name])
        self.RngbdIllmnCmdPen = RngbdIllmnCmdPen(self, name_to_item_map[RngbdIllmnCmdPen.fdx_name])
        self.RngbdIllmnCmdSts = RngbdIllmnCmdSts(self, name_to_item_map[RngbdIllmnCmdSts.fdx_name])
        self.RoadFricIndcnActvPen = RoadFricIndcnActvPen(self, name_to_item_map[RoadFricIndcnActvPen.fdx_name])
        self.RoadFricIndcnActvSts = RoadFricIndcnActvSts(self, name_to_item_map[RoadFricIndcnActvSts.fdx_name])
        self.RoadFricIndcnSts = RoadFricIndcnSts(self, name_to_item_map[RoadFricIndcnSts.fdx_name])
        self.RoadFricWarnReq = RoadFricWarnReq(self, name_to_item_map[RoadFricWarnReq.fdx_name])
        self.RoadSgnInfoActvPen = RoadSgnInfoActvPen(self, name_to_item_map[RoadSgnInfoActvPen.fdx_name])
        self.RoadSgnInfoActvSts = RoadSgnInfoActvSts(self, name_to_item_map[RoadSgnInfoActvSts.fdx_name])
        self.RoadSgnInfoSts = RoadSgnInfoSts(self, name_to_item_map[RoadSgnInfoSts.fdx_name])
        self.RoadSpdLimActvSts = RoadSpdLimActvSts(self, name_to_item_map[RoadSpdLimActvSts.fdx_name])
        self.RoadUsrProtnSts = RoadUsrProtnSts(self, name_to_item_map[RoadUsrProtnSts.fdx_name])
        self.RollRate1 = RollRate1(self, name_to_item_map[RollRate1.fdx_name])
        self.RollRate1Qf1 = RollRate1Qf1(self, name_to_item_map[RollRate1Qf1.fdx_name])
        self.RotyDirReq1 = RotyDirReq1(self, name_to_item_map[RotyDirReq1.fdx_name])
        self.RotyDirReq2 = RotyDirReq2(self, name_to_item_map[RotyDirReq2.fdx_name])
        self.RotyPosReq1 = RotyPosReq1(self, name_to_item_map[RotyPosReq1.fdx_name])
        self.RotyPosReq2 = RotyPosReq2(self, name_to_item_map[RotyPosReq2.fdx_name])
        self.RsdsSysStsLeCtraSts = RsdsSysStsLeCtraSts(self, name_to_item_map[RsdsSysStsLeCtraSts.fdx_name])
        self.RsdsSysStsLeCtraSts1 = RsdsSysStsLeCtraSts1(self, name_to_item_map[RsdsSysStsLeCtraSts1.fdx_name])
        self.RsdsSysStsLeLcmaSts = RsdsSysStsLeLcmaSts(self, name_to_item_map[RsdsSysStsLeLcmaSts.fdx_name])
        self.RsdsSysStsLeLcmaSts1 = RsdsSysStsLeLcmaSts1(self, name_to_item_map[RsdsSysStsLeLcmaSts1.fdx_name])
        self.RsdsSysStsLeRcmSts = RsdsSysStsLeRcmSts(self, name_to_item_map[RsdsSysStsLeRcmSts.fdx_name])
        self.RsdsSysStsLeRcwSts = RsdsSysStsLeRcwSts(self, name_to_item_map[RsdsSysStsLeRcwSts.fdx_name])
        self.RsdsSysStsLeRsdsSts = RsdsSysStsLeRsdsSts(self, name_to_item_map[RsdsSysStsLeRsdsSts.fdx_name])
        self.RsdsSysStsRiCtraSts = RsdsSysStsRiCtraSts(self, name_to_item_map[RsdsSysStsRiCtraSts.fdx_name])
        self.RsdsSysStsRiCtraSts1 = RsdsSysStsRiCtraSts1(self, name_to_item_map[RsdsSysStsRiCtraSts1.fdx_name])
        self.RsdsSysStsRiLcmaSts = RsdsSysStsRiLcmaSts(self, name_to_item_map[RsdsSysStsRiLcmaSts.fdx_name])
        self.RsdsSysStsRiLcmaSts1 = RsdsSysStsRiLcmaSts1(self, name_to_item_map[RsdsSysStsRiLcmaSts1.fdx_name])
        self.RsdsSysStsRiRcmSts = RsdsSysStsRiRcmSts(self, name_to_item_map[RsdsSysStsRiRcmSts.fdx_name])
        self.RsdsSysStsRiRcwSts = RsdsSysStsRiRcwSts(self, name_to_item_map[RsdsSysStsRiRcwSts.fdx_name])
        self.RsdsSysStsRiRsdsSts = RsdsSysStsRiRsdsSts(self, name_to_item_map[RsdsSysStsRiRsdsSts.fdx_name])
        self.SatltPosnStsPrm1InPosnFromSatlt = SatltPosnStsPrm1InPosnFromSatlt(self, name_to_item_map[SatltPosnStsPrm1InPosnFromSatlt.fdx_name])
        self.SatltPosnStsPrm2InPosnFromSatlt = SatltPosnStsPrm2InPosnFromSatlt(self, name_to_item_map[SatltPosnStsPrm2InPosnFromSatlt.fdx_name])
        self.SatltPosnStsPrm3InPosnFromSatlt = SatltPosnStsPrm3InPosnFromSatlt(self, name_to_item_map[SatltPosnStsPrm3InPosnFromSatlt.fdx_name])
        self.SatltSysNo1InUseInPosnFromSatlt = SatltSysNo1InUseInPosnFromSatlt(self, name_to_item_map[SatltSysNo1InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo2InUseInPosnFromSatlt = SatltSysNo2InUseInPosnFromSatlt(self, name_to_item_map[SatltSysNo2InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo3InUseInPosnFromSatlt = SatltSysNo3InUseInPosnFromSatlt(self, name_to_item_map[SatltSysNo3InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo4InUseInPosnFromSatlt = SatltSysNo4InUseInPosnFromSatlt(self, name_to_item_map[SatltSysNo4InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo5InUseInPosnFromSatlt = SatltSysNo5InUseInPosnFromSatlt(self, name_to_item_map[SatltSysNo5InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo6InUseInPosnFromSatlt = SatltSysNo6InUseInPosnFromSatlt(self, name_to_item_map[SatltSysNo6InUseInPosnFromSatlt.fdx_name])
        self.SaveSetgToMemPrmnt = SaveSetgToMemPrmnt(self, name_to_item_map[SaveSetgToMemPrmnt.fdx_name])
        self.ScrBarVolIndcn = ScrBarVolIndcn(self, name_to_item_map[ScrBarVolIndcn.fdx_name])
        self.ScrMaxFillgVol = ScrMaxFillgVol(self, name_to_item_map[ScrMaxFillgVol.fdx_name])
        self.ScrReagentTankVol = ScrReagentTankVol(self, name_to_item_map[ScrReagentTankVol.fdx_name])
        self.SeatBackUnlckdThrdLe = SeatBackUnlckdThrdLe(self, name_to_item_map[SeatBackUnlckdThrdLe.fdx_name])
        self.SeatBackUnlckdThrdRi = SeatBackUnlckdThrdRi(self, name_to_item_map[SeatBackUnlckdThrdRi.fdx_name])
        self.SeatDispBtnPsdDrvrSeatDispBtnPsd = SeatDispBtnPsdDrvrSeatDispBtnPsd(self, name_to_item_map[SeatDispBtnPsdDrvrSeatDispBtnPsd.fdx_name])
        self.SeatDispBtnPsdPassSeatDispBtnPsd = SeatDispBtnPsdPassSeatDispBtnPsd(self, name_to_item_map[SeatDispBtnPsdPassSeatDispBtnPsd.fdx_name])
        self.SeatHeatDurgClimaEnad = SeatHeatDurgClimaEnad(self, name_to_item_map[SeatHeatDurgClimaEnad.fdx_name])
        self.SeatHeatgAutCdn = SeatHeatgAutCdn(self, name_to_item_map[SeatHeatgAutCdn.fdx_name])
        self.SeatHeatLvlReqLe = SeatHeatLvlReqLe(self, name_to_item_map[SeatHeatLvlReqLe.fdx_name])
        self.SeatHeatLvlReqRi = SeatHeatLvlReqRi(self, name_to_item_map[SeatHeatLvlReqRi.fdx_name])
        self.SeatSwtLeSigThrd = SeatSwtLeSigThrd(self, name_to_item_map[SeatSwtLeSigThrd.fdx_name])
        self.SeatSwtRiSigThrd = SeatSwtRiSigThrd(self, name_to_item_map[SeatSwtRiSigThrd.fdx_name])
        self.SeatVentnAutCdn = SeatVentnAutCdn(self, name_to_item_map[SeatVentnAutCdn.fdx_name])
        self.Sec = Sec(self, name_to_item_map[Sec.fdx_name])
        self.Sec1 = Sec1(self, name_to_item_map[Sec1.fdx_name])
        self.SetOfLangIdPen = SetOfLangIdPen(self, name_to_item_map[SetOfLangIdPen.fdx_name])
        self.SetOfLangLangTyp = SetOfLangLangTyp(self, name_to_item_map[SetOfLangLangTyp.fdx_name])
        self.SftyCchAccActvnStsActv = SftyCchAccActvnStsActv(self, name_to_item_map[SftyCchAccActvnStsActv.fdx_name])
        self.SftyCchAccActvnStsDay = SftyCchAccActvnStsDay(self, name_to_item_map[SftyCchAccActvnStsDay.fdx_name])
        self.SftyCchAccActvnStsMth = SftyCchAccActvnStsMth(self, name_to_item_map[SftyCchAccActvnStsMth.fdx_name])
        self.SftyCchAccActvnStsYr = SftyCchAccActvnStsYr(self, name_to_item_map[SftyCchAccActvnStsYr.fdx_name])
        self.SftyCchLaneKeepAidActvnStsActv = SftyCchLaneKeepAidActvnStsActv(self, name_to_item_map[SftyCchLaneKeepAidActvnStsActv.fdx_name])
        self.SftyCchLaneKeepAidActvnStsDay = SftyCchLaneKeepAidActvnStsDay(self, name_to_item_map[SftyCchLaneKeepAidActvnStsDay.fdx_name])
        self.SftyCchLaneKeepAidActvnStsMth = SftyCchLaneKeepAidActvnStsMth(self, name_to_item_map[SftyCchLaneKeepAidActvnStsMth.fdx_name])
        self.SftyCchLaneKeepAidActvnStsYr = SftyCchLaneKeepAidActvnStsYr(self, name_to_item_map[SftyCchLaneKeepAidActvnStsYr.fdx_name])
        self.SftyCchLongTermIdxAttention = SftyCchLongTermIdxAttention(self, name_to_item_map[SftyCchLongTermIdxAttention.fdx_name])
        self.SftyCchLongTermIdxAttentionAvl = SftyCchLongTermIdxAttentionAvl(self, name_to_item_map[SftyCchLongTermIdxAttentionAvl.fdx_name])
        self.SftyCchLongTermIdxDstToVeh = SftyCchLongTermIdxDstToVeh(self, name_to_item_map[SftyCchLongTermIdxDstToVeh.fdx_name])
        self.SftyCchLongTermIdxDstToVehAvl = SftyCchLongTermIdxDstToVehAvl(self, name_to_item_map[SftyCchLongTermIdxDstToVehAvl.fdx_name])
        self.SftyCchLongTermIdxKeepSpdLim = SftyCchLongTermIdxKeepSpdLim(self, name_to_item_map[SftyCchLongTermIdxKeepSpdLim.fdx_name])
        self.SftyCchLongTermIdxKeepSpdLimAvl = SftyCchLongTermIdxKeepSpdLimAvl(self, name_to_item_map[SftyCchLongTermIdxKeepSpdLimAvl.fdx_name])
        self.SftyCchLongTermIdxLaneKeep = SftyCchLongTermIdxLaneKeep(self, name_to_item_map[SftyCchLongTermIdxLaneKeep.fdx_name])
        self.SftyCchLongTermIdxLaneKeepAvl = SftyCchLongTermIdxLaneKeepAvl(self, name_to_item_map[SftyCchLongTermIdxLaneKeepAvl.fdx_name])
        self.SftyCchLongTermIdxTot = SftyCchLongTermIdxTot(self, name_to_item_map[SftyCchLongTermIdxTot.fdx_name])
        self.SftyCchLongTermIdxTotAvl = SftyCchLongTermIdxTotAvl(self, name_to_item_map[SftyCchLongTermIdxTotAvl.fdx_name])
        self.SftyWarnGroupFromAsySafeChks = SftyWarnGroupFromAsySafeChks(self, name_to_item_map[SftyWarnGroupFromAsySafeChks.fdx_name])
        self.SftyWarnGroupFromAsySafeCntr = SftyWarnGroupFromAsySafeCntr(self, name_to_item_map[SftyWarnGroupFromAsySafeCntr.fdx_name])
        self.SpdAlrmActvForRoadSgnInfoPen = SpdAlrmActvForRoadSgnInfoPen(self, name_to_item_map[SpdAlrmActvForRoadSgnInfoPen.fdx_name])
        self.SpdAlrmActvForRoadSgnInfoSts = SpdAlrmActvForRoadSgnInfoSts(self, name_to_item_map[SpdAlrmActvForRoadSgnInfoSts.fdx_name])
        self.SpdCameraInfoSeldForRoadSgnInfoPen = SpdCameraInfoSeldForRoadSgnInfoPen(self, name_to_item_map[SpdCameraInfoSeldForRoadSgnInfoPen.fdx_name])
        self.SpdCameraInfoSeldForRoadSgnInfoSts = SpdCameraInfoSeldForRoadSgnInfoSts(self, name_to_item_map[SpdCameraInfoSeldForRoadSgnInfoSts.fdx_name])
        self.SpdLimAdpvSts = SpdLimAdpvSts(self, name_to_item_map[SpdLimAdpvSts.fdx_name])
        self.SpdLimWarnReq = SpdLimWarnReq(self, name_to_item_map[SpdLimWarnReq.fdx_name])
        self.SpdUnit = SpdUnit(self, name_to_item_map[SpdUnit.fdx_name])
        self.SpdWarn = SpdWarn(self, name_to_item_map[SpdWarn.fdx_name])
        self.SpprtForFctInDstToManvLocn = SpprtForFctInDstToManvLocn(self, name_to_item_map[SpprtForFctInDstToManvLocn.fdx_name])
        self.SpprtForFctInTireCircumCalByNav = SpprtForFctInTireCircumCalByNav(self, name_to_item_map[SpprtForFctInTireCircumCalByNav.fdx_name])
        self.SrvRst = SrvRst(self, name_to_item_map[SrvRst.fdx_name])
        self.SrvTrig = SrvTrig(self, name_to_item_map[SrvTrig.fdx_name])
        self.SteerOvrdWarnReqForAutDrv = SteerOvrdWarnReqForAutDrv(self, name_to_item_map[SteerOvrdWarnReqForAutDrv.fdx_name])
        self.SteerStsForAutDrv = SteerStsForAutDrv(self, name_to_item_map[SteerStsForAutDrv.fdx_name])
        self.SteerWarnReqForAutDrv = SteerWarnReqForAutDrv(self, name_to_item_map[SteerWarnReqForAutDrv.fdx_name])
        self.SteerWarnReqForLaneKeepAid = SteerWarnReqForLaneKeepAid(self, name_to_item_map[SteerWarnReqForLaneKeepAid.fdx_name])
        self.SteerWhlAgSafe = SteerWhlAgSafe(self, name_to_item_map[SteerWhlAgSafe.fdx_name])
        self.SteerWhlAgSpdSafe = SteerWhlAgSpdSafe(self, name_to_item_map[SteerWhlAgSpdSafe.fdx_name])
        self.SteerWhlHeatgDurgClimaEnad = SteerWhlHeatgDurgClimaEnad(self, name_to_item_map[SteerWhlHeatgDurgClimaEnad.fdx_name])
        self.SteerWhlHeatgOnReq = SteerWhlHeatgOnReq(self, name_to_item_map[SteerWhlHeatgOnReq.fdx_name])
        self.SteerWhlHeatgStrtAutCdnOk = SteerWhlHeatgStrtAutCdnOk(self, name_to_item_map[SteerWhlHeatgStrtAutCdnOk.fdx_name])
        self.SteerWhlSnsrQf = SteerWhlSnsrQf(self, name_to_item_map[SteerWhlSnsrQf.fdx_name])
        self.SteerWhlSnsrSafeChks = SteerWhlSnsrSafeChks(self, name_to_item_map[SteerWhlSnsrSafeChks.fdx_name])
        self.SteerWhlSnsrSafeCntr = SteerWhlSnsrSafeCntr(self, name_to_item_map[SteerWhlSnsrSafeCntr.fdx_name])
        self.SteerWhlSwtPwr = SteerWhlSwtPwr(self, name_to_item_map[SteerWhlSwtPwr.fdx_name])
        self.StopStrtInhb = StopStrtInhb(self, name_to_item_map[StopStrtInhb.fdx_name])
        self.StrtInProgs = StrtInProgs(self, name_to_item_map[StrtInProgs.fdx_name])
        self.SunRoofPosnSts = SunRoofPosnSts(self, name_to_item_map[SunRoofPosnSts.fdx_name])
        self.SwtAcptReq = SwtAcptReq(self, name_to_item_map[SwtAcptReq.fdx_name])
        self.SwtAtCenSts = SwtAtCenSts(self, name_to_item_map[SwtAtCenSts.fdx_name])
        self.SwtForPassAirbCutOffSt = SwtForPassAirbCutOffSt(self, name_to_item_map[SwtForPassAirbCutOffSt.fdx_name])
        self.TankFlapSts = TankFlapSts(self, name_to_item_map[TankFlapSts.fdx_name])
        self.TelmSts = TelmSts(self, name_to_item_map[TelmSts.fdx_name])
        self.TiAndDateVld = TiAndDateVld(self, name_to_item_map[TiAndDateVld.fdx_name])
        self.TiForDayInPosnFromSatlt = TiForDayInPosnFromSatlt(self, name_to_item_map[TiForDayInPosnFromSatlt.fdx_name])
        self.TiForHrInPosnFromSatlt = TiForHrInPosnFromSatlt(self, name_to_item_map[TiForHrInPosnFromSatlt.fdx_name])
        self.TiForMinsInPosnFromSatlt = TiForMinsInPosnFromSatlt(self, name_to_item_map[TiForMinsInPosnFromSatlt.fdx_name])
        self.TiForMthInPosnFromSatlt = TiForMthInPosnFromSatlt(self, name_to_item_map[TiForMthInPosnFromSatlt.fdx_name])
        self.TiForSecInPosnFromSatlt = TiForSecInPosnFromSatlt(self, name_to_item_map[TiForSecInPosnFromSatlt.fdx_name])
        self.TiForYrInPosnFromSatlt = TiForYrInPosnFromSatlt(self, name_to_item_map[TiForYrInPosnFromSatlt.fdx_name])
        self.TiGapLimdPen = TiGapLimdPen(self, name_to_item_map[TiGapLimdPen.fdx_name])
        self.TiGapLimdSts = TiGapLimdSts(self, name_to_item_map[TiGapLimdSts.fdx_name])
        self.TireCircumInTireCircumCalByNav = TireCircumInTireCircumCalByNav(self, name_to_item_map[TireCircumInTireCircumCalByNav.fdx_name])
        self.TireMonCalReq = TireMonCalReq(self, name_to_item_map[TireMonCalReq.fdx_name])
        self.TireMonrDispReq = TireMonrDispReq(self, name_to_item_map[TireMonrDispReq.fdx_name])
        self.TirePAbsltValForFrntLe = TirePAbsltValForFrntLe(self, name_to_item_map[TirePAbsltValForFrntLe.fdx_name])
        self.TirePAbsltValForReLe = TirePAbsltValForReLe(self, name_to_item_map[TirePAbsltValForReLe.fdx_name])
        self.TirePAbsltValForReRi = TirePAbsltValForReRi(self, name_to_item_map[TirePAbsltValForReRi.fdx_name])
        self.TirePAbsltValFrntRiTirePAbsltVal1 = TirePAbsltValFrntRiTirePAbsltVal1(self, name_to_item_map[TirePAbsltValFrntRiTirePAbsltVal1.fdx_name])
        self.TirePAbsltValFrntRiTirePPosn = TirePAbsltValFrntRiTirePPosn(self, name_to_item_map[TirePAbsltValFrntRiTirePPosn.fdx_name])
        self.TirePAbsltValQFForFrnRi = TirePAbsltValQFForFrnRi(self, name_to_item_map[TirePAbsltValQFForFrnRi.fdx_name])
        self.TirePAbsltValQfForReLe = TirePAbsltValQfForReLe(self, name_to_item_map[TirePAbsltValQfForReLe.fdx_name])
        self.TirePAbsltValQfForReRi = TirePAbsltValQfForReRi(self, name_to_item_map[TirePAbsltValQfForReRi.fdx_name])
        self.TirePAbsltVaQflForFrntLe = TirePAbsltVaQflForFrntLe(self, name_to_item_map[TirePAbsltVaQflForFrntLe.fdx_name])
        self.TirePCalSts = TirePCalSts(self, name_to_item_map[TirePCalSts.fdx_name])
        self.TirePFrntLe = TirePFrntLe(self, name_to_item_map[TirePFrntLe.fdx_name])
        self.TirePFrntRi = TirePFrntRi(self, name_to_item_map[TirePFrntRi.fdx_name])
        self.TirePMonSts = TirePMonSts(self, name_to_item_map[TirePMonSts.fdx_name])
        self.TirePPosnForFrntLe = TirePPosnForFrntLe(self, name_to_item_map[TirePPosnForFrntLe.fdx_name])
        self.TirePPosnForReLe = TirePPosnForReLe(self, name_to_item_map[TirePPosnForReLe.fdx_name])
        self.TirePPosnForReRi = TirePPosnForReRi(self, name_to_item_map[TirePPosnForReRi.fdx_name])
        self.TirePReLe = TirePReLe(self, name_to_item_map[TirePReLe.fdx_name])
        self.TirePReRi = TirePReRi(self, name_to_item_map[TirePReRi.fdx_name])
        self.TnlEntryOrEnd = TnlEntryOrEnd(self, name_to_item_map[TnlEntryOrEnd.fdx_name])
        self.TrfcLiSpprtSts = TrfcLiSpprtSts(self, name_to_item_map[TrfcLiSpprtSts.fdx_name])
        self.TripModSeln = TripModSeln(self, name_to_item_map[TripModSeln.fdx_name])
        self.TrlrLampActvtChk = TrlrLampActvtChk(self, name_to_item_map[TrlrLampActvtChk.fdx_name])
        self.TrlrLampChkAutReqPen = TrlrLampChkAutReqPen(self, name_to_item_map[TrlrLampChkAutReqPen.fdx_name])
        self.TrlrLampChkAutReqSts = TrlrLampChkAutReqSts(self, name_to_item_map[TrlrLampChkAutReqSts.fdx_name])
        self.TrlrLampChkRemReq = TrlrLampChkRemReq(self, name_to_item_map[TrlrLampChkRemReq.fdx_name])
        self.TrlrLampChkSts = TrlrLampChkSts(self, name_to_item_map[TrlrLampChkSts.fdx_name])
        self.TrlrPrsnt = TrlrPrsnt(self, name_to_item_map[TrlrPrsnt.fdx_name])
        self.TrSts = TrSts(self, name_to_item_map[TrSts.fdx_name])
        self.TUnit = TUnit(self, name_to_item_map[TUnit.fdx_name])
        self.TurnAutFlsgReqPen = TurnAutFlsgReqPen(self, name_to_item_map[TurnAutFlsgReqPen.fdx_name])
        self.TurnAutFlsgReqSts = TurnAutFlsgReqSts(self, name_to_item_map[TurnAutFlsgReqSts.fdx_name])
        self.TwliBriRaw = TwliBriRaw(self, name_to_item_map[TwliBriRaw.fdx_name])
        self.TwliBriRawQf = TwliBriRawQf(self, name_to_item_map[TwliBriRawQf.fdx_name])
        self.TwliBriSts = TwliBriSts(self, name_to_item_map[TwliBriSts.fdx_name])
        self.UkwnCptReqToInfoMgrByte0 = UkwnCptReqToInfoMgrByte0(self, name_to_item_map[UkwnCptReqToInfoMgrByte0.fdx_name])
        self.UkwnCptReqToInfoMgrByte1 = UkwnCptReqToInfoMgrByte1(self, name_to_item_map[UkwnCptReqToInfoMgrByte1.fdx_name])
        self.UkwnCptReqToInfoMgrByte2 = UkwnCptReqToInfoMgrByte2(self, name_to_item_map[UkwnCptReqToInfoMgrByte2.fdx_name])
        self.UkwnCptReqToInfoMgrByte3 = UkwnCptReqToInfoMgrByte3(self, name_to_item_map[UkwnCptReqToInfoMgrByte3.fdx_name])
        self.UkwnCptReqToInfoMgrByte4 = UkwnCptReqToInfoMgrByte4(self, name_to_item_map[UkwnCptReqToInfoMgrByte4.fdx_name])
        self.UkwnCptReqToInfoMgrByte5 = UkwnCptReqToInfoMgrByte5(self, name_to_item_map[UkwnCptReqToInfoMgrByte5.fdx_name])
        self.UkwnCptReqToInfoMgrByte6 = UkwnCptReqToInfoMgrByte6(self, name_to_item_map[UkwnCptReqToInfoMgrByte6.fdx_name])
        self.UkwnCptReqToInfoMgrByte7 = UkwnCptReqToInfoMgrByte7(self, name_to_item_map[UkwnCptReqToInfoMgrByte7.fdx_name])
        self.UkwnCptReqToSoundMgrByte0 = UkwnCptReqToSoundMgrByte0(self, name_to_item_map[UkwnCptReqToSoundMgrByte0.fdx_name])
        self.UkwnCptReqToSoundMgrByte1 = UkwnCptReqToSoundMgrByte1(self, name_to_item_map[UkwnCptReqToSoundMgrByte1.fdx_name])
        self.UkwnCptReqToSoundMgrByte2 = UkwnCptReqToSoundMgrByte2(self, name_to_item_map[UkwnCptReqToSoundMgrByte2.fdx_name])
        self.UkwnCptReqToSoundMgrByte3 = UkwnCptReqToSoundMgrByte3(self, name_to_item_map[UkwnCptReqToSoundMgrByte3.fdx_name])
        self.UkwnCptReqToSoundMgrByte4 = UkwnCptReqToSoundMgrByte4(self, name_to_item_map[UkwnCptReqToSoundMgrByte4.fdx_name])
        self.UkwnCptReqToSoundMgrByte5 = UkwnCptReqToSoundMgrByte5(self, name_to_item_map[UkwnCptReqToSoundMgrByte5.fdx_name])
        self.UkwnCptReqToSoundMgrByte6 = UkwnCptReqToSoundMgrByte6(self, name_to_item_map[UkwnCptReqToSoundMgrByte6.fdx_name])
        self.UkwnCptReqToSoundMgrByte7 = UkwnCptReqToSoundMgrByte7(self, name_to_item_map[UkwnCptReqToSoundMgrByte7.fdx_name])
        self.UkwnCptRespFromInfoMgrByte0 = UkwnCptRespFromInfoMgrByte0(self, name_to_item_map[UkwnCptRespFromInfoMgrByte0.fdx_name])
        self.UkwnCptRespFromInfoMgrByte1 = UkwnCptRespFromInfoMgrByte1(self, name_to_item_map[UkwnCptRespFromInfoMgrByte1.fdx_name])
        self.UkwnCptRespFromInfoMgrByte2 = UkwnCptRespFromInfoMgrByte2(self, name_to_item_map[UkwnCptRespFromInfoMgrByte2.fdx_name])
        self.UkwnCptRespFromInfoMgrByte3 = UkwnCptRespFromInfoMgrByte3(self, name_to_item_map[UkwnCptRespFromInfoMgrByte3.fdx_name])
        self.UkwnCptRespFromInfoMgrByte4 = UkwnCptRespFromInfoMgrByte4(self, name_to_item_map[UkwnCptRespFromInfoMgrByte4.fdx_name])
        self.UkwnCptRespFromInfoMgrByte5 = UkwnCptRespFromInfoMgrByte5(self, name_to_item_map[UkwnCptRespFromInfoMgrByte5.fdx_name])
        self.UkwnCptRespFromInfoMgrByte6 = UkwnCptRespFromInfoMgrByte6(self, name_to_item_map[UkwnCptRespFromInfoMgrByte6.fdx_name])
        self.UkwnCptRespFromInfoMgrByte7 = UkwnCptRespFromInfoMgrByte7(self, name_to_item_map[UkwnCptRespFromInfoMgrByte7.fdx_name])
        self.UkwnCptRespFromSoundMgrByte0 = UkwnCptRespFromSoundMgrByte0(self, name_to_item_map[UkwnCptRespFromSoundMgrByte0.fdx_name])
        self.UkwnCptRespFromSoundMgrByte1 = UkwnCptRespFromSoundMgrByte1(self, name_to_item_map[UkwnCptRespFromSoundMgrByte1.fdx_name])
        self.UkwnCptRespFromSoundMgrByte2 = UkwnCptRespFromSoundMgrByte2(self, name_to_item_map[UkwnCptRespFromSoundMgrByte2.fdx_name])
        self.UkwnCptRespFromSoundMgrByte3 = UkwnCptRespFromSoundMgrByte3(self, name_to_item_map[UkwnCptRespFromSoundMgrByte3.fdx_name])
        self.UkwnCptRespFromSoundMgrByte4 = UkwnCptRespFromSoundMgrByte4(self, name_to_item_map[UkwnCptRespFromSoundMgrByte4.fdx_name])
        self.UkwnCptRespFromSoundMgrByte5 = UkwnCptRespFromSoundMgrByte5(self, name_to_item_map[UkwnCptRespFromSoundMgrByte5.fdx_name])
        self.UkwnCptRespFromSoundMgrByte6 = UkwnCptRespFromSoundMgrByte6(self, name_to_item_map[UkwnCptRespFromSoundMgrByte6.fdx_name])
        self.UkwnCptRespFromSoundMgrByte7 = UkwnCptRespFromSoundMgrByte7(self, name_to_item_map[UkwnCptRespFromSoundMgrByte7.fdx_name])
        self.UnlckFbVisReqPen = UnlckFbVisReqPen(self, name_to_item_map[UnlckFbVisReqPen.fdx_name])
        self.UnlckFbVisReqSts = UnlckFbVisReqSts(self, name_to_item_map[UnlckFbVisReqSts.fdx_name])
        self.UnlckKeylsReqPen = UnlckKeylsReqPen(self, name_to_item_map[UnlckKeylsReqPen.fdx_name])
        self.UnlckKeylsReqSts = UnlckKeylsReqSts(self, name_to_item_map[UnlckKeylsReqSts.fdx_name])
        self.UnlckRemReqPen = UnlckRemReqPen(self, name_to_item_map[UnlckRemReqPen.fdx_name])
        self.UnlckRemReqSts = UnlckRemReqSts(self, name_to_item_map[UnlckRemReqSts.fdx_name])
        self.UsgModSts = UsgModSts(self, name_to_item_map[UsgModSts.fdx_name])
        self.UsrSetSpdForKeySpdLimnPen = UsrSetSpdForKeySpdLimnPen(self, name_to_item_map[UsrSetSpdForKeySpdLimnPen.fdx_name])
        self.UsrSetSpdForKeySpdLimnSts = UsrSetSpdForKeySpdLimnSts(self, name_to_item_map[UsrSetSpdForKeySpdLimnSts.fdx_name])
        self.UsrSetSpdForKeySpdWarn1 = UsrSetSpdForKeySpdWarn1(self, name_to_item_map[UsrSetSpdForKeySpdWarn1.fdx_name])
        self.UsrSetSpdForKeySpdWarn2 = UsrSetSpdForKeySpdWarn2(self, name_to_item_map[UsrSetSpdForKeySpdWarn2.fdx_name])
        self.UsrSetSpdForKeySpdWarn3 = UsrSetSpdForKeySpdWarn3(self, name_to_item_map[UsrSetSpdForKeySpdWarn3.fdx_name])
        self.UsrSetSpdForKeySpdWarn4 = UsrSetSpdForKeySpdWarn4(self, name_to_item_map[UsrSetSpdForKeySpdWarn4.fdx_name])
        self.UsrSetSpdForKeySpdWarn5 = UsrSetSpdForKeySpdWarn5(self, name_to_item_map[UsrSetSpdForKeySpdWarn5.fdx_name])
        self.UsrSetSpdForKeySpdWarn6 = UsrSetSpdForKeySpdWarn6(self, name_to_item_map[UsrSetSpdForKeySpdWarn6.fdx_name])
        self.UsrSetSpdForKeySpdWarnPen = UsrSetSpdForKeySpdWarnPen(self, name_to_item_map[UsrSetSpdForKeySpdWarnPen.fdx_name])
        self.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe = UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe(self, name_to_item_map[UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe.fdx_name])
        self.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi = UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi(self, name_to_item_map[UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi.fdx_name])
        self.UsrSwtDispClimaReqForTSpForRowSecLe = UsrSwtDispClimaReqForTSpForRowSecLe(self, name_to_item_map[UsrSwtDispClimaReqForTSpForRowSecLe.fdx_name])
        self.UsrSwtDispClimaReqForTSpForRowSecRi = UsrSwtDispClimaReqForTSpForRowSecRi(self, name_to_item_map[UsrSwtDispClimaReqForTSpForRowSecRi.fdx_name])
        self.UsrSwtDispClimaReqForTSpSpclForRowSecLe = UsrSwtDispClimaReqForTSpSpclForRowSecLe(self, name_to_item_map[UsrSwtDispClimaReqForTSpSpclForRowSecLe.fdx_name])
        self.UsrSwtDispClimaReqForTSpSpclForRowSecRi = UsrSwtDispClimaReqForTSpSpclForRowSecRi(self, name_to_item_map[UsrSwtDispClimaReqForTSpSpclForRowSecRi.fdx_name])
        self.UsrSwtDispReqForFanLvlForRowSec = UsrSwtDispReqForFanLvlForRowSec(self, name_to_item_map[UsrSwtDispReqForFanLvlForRowSec.fdx_name])
        self.UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe = UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe(self, name_to_item_map[UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe.fdx_name])
        self.UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnRi = UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnRi(self, name_to_item_map[UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnRi.fdx_name])
        self.UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnLe = UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnLe(self, name_to_item_map[UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnLe.fdx_name])
        self.UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnRi = UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnRi(self, name_to_item_map[UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnRi.fdx_name])
        self.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe = UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe(self, name_to_item_map[UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe.fdx_name])
        self.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi = UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi(self, name_to_item_map[UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi.fdx_name])
        self.UsrSwtDispUpdClimaReqForTSpForRowSecLe = UsrSwtDispUpdClimaReqForTSpForRowSecLe(self, name_to_item_map[UsrSwtDispUpdClimaReqForTSpForRowSecLe.fdx_name])
        self.UsrSwtDispUpdClimaReqForTSpForRowSecRi = UsrSwtDispUpdClimaReqForTSpForRowSecRi(self, name_to_item_map[UsrSwtDispUpdClimaReqForTSpForRowSecRi.fdx_name])
        self.UsrSwtDispUpdReqForFanLvlForRowSec = UsrSwtDispUpdReqForFanLvlForRowSec(self, name_to_item_map[UsrSwtDispUpdReqForFanLvlForRowSec.fdx_name])
        self.VehActvMsgToDrvr = VehActvMsgToDrvr(self, name_to_item_map[VehActvMsgToDrvr.fdx_name])
        self.VehBattUSysU = VehBattUSysU(self, name_to_item_map[VehBattUSysU.fdx_name])
        self.VehBattUSysUQf = VehBattUSysUQf(self, name_to_item_map[VehBattUSysUQf.fdx_name])
        self.VehCfgPrmBlk = VehCfgPrmBlk(self, name_to_item_map[VehCfgPrmBlk.fdx_name])
        self.VehCfgPrmVal1 = VehCfgPrmVal1(self, name_to_item_map[VehCfgPrmVal1.fdx_name])
        self.VehCfgPrmVal2 = VehCfgPrmVal2(self, name_to_item_map[VehCfgPrmVal2.fdx_name])
        self.VehCfgPrmVal3 = VehCfgPrmVal3(self, name_to_item_map[VehCfgPrmVal3.fdx_name])
        self.VehCfgPrmVal4 = VehCfgPrmVal4(self, name_to_item_map[VehCfgPrmVal4.fdx_name])
        self.VehCfgPrmVal5 = VehCfgPrmVal5(self, name_to_item_map[VehCfgPrmVal5.fdx_name])
        self.VehCfgPrmVal6 = VehCfgPrmVal6(self, name_to_item_map[VehCfgPrmVal6.fdx_name])
        self.VehCfgPrmVal7 = VehCfgPrmVal7(self, name_to_item_map[VehCfgPrmVal7.fdx_name])
        self.VehM = VehM(self, name_to_item_map[VehM.fdx_name])
        self.VehMNomTrlrM = VehMNomTrlrM(self, name_to_item_map[VehMNomTrlrM.fdx_name])
        self.VehModMngtGlbSafe1Chks = VehModMngtGlbSafe1Chks(self, name_to_item_map[VehModMngtGlbSafe1Chks.fdx_name])
        self.VehModMngtGlbSafe1Cntr = VehModMngtGlbSafe1Cntr(self, name_to_item_map[VehModMngtGlbSafe1Cntr.fdx_name])
        self.VehMQly = VehMQly(self, name_to_item_map[VehMQly.fdx_name])
        self.VehMtnStChks = VehMtnStChks(self, name_to_item_map[VehMtnStChks.fdx_name])
        self.VehMtnStCntr = VehMtnStCntr(self, name_to_item_map[VehMtnStCntr.fdx_name])
        self.VehMtnStSafe = VehMtnStSafe(self, name_to_item_map[VehMtnStSafe.fdx_name])
        self.VehSpdAvg = VehSpdAvg(self, name_to_item_map[VehSpdAvg.fdx_name])
        self.VehSpdAvgUnit = VehSpdAvgUnit(self, name_to_item_map[VehSpdAvgUnit.fdx_name])
        self.VehSpdCtrlActvSts = VehSpdCtrlActvSts(self, name_to_item_map[VehSpdCtrlActvSts.fdx_name])
        self.VehSpdIndcdUnit = VehSpdIndcdUnit(self, name_to_item_map[VehSpdIndcdUnit.fdx_name])
        self.VehSpdIndcdVal = VehSpdIndcdVal(self, name_to_item_map[VehSpdIndcdVal.fdx_name])
        self.VehSpdLgtSafe = VehSpdLgtSafe(self, name_to_item_map[VehSpdLgtSafe.fdx_name])
        self.VehSpdLgtSafeChks = VehSpdLgtSafeChks(self, name_to_item_map[VehSpdLgtSafeChks.fdx_name])
        self.VehSpdLgtSafeCntr = VehSpdLgtSafeCntr(self, name_to_item_map[VehSpdLgtSafeCntr.fdx_name])
        self.VehSpdLgtSafeQf = VehSpdLgtSafeQf(self, name_to_item_map[VehSpdLgtSafeQf.fdx_name])
        self.VFCVectorIHUGrp1 = VFCVectorIHUGrp1(self, name_to_item_map[VFCVectorIHUGrp1.fdx_name])
        self.VFCVectorIHUGrp2 = VFCVectorIHUGrp2(self, name_to_item_map[VFCVectorIHUGrp2.fdx_name])
        self.VFCVectorIHUGrp3 = VFCVectorIHUGrp3(self, name_to_item_map[VFCVectorIHUGrp3.fdx_name])
        self.VinBlk = VinBlk(self, name_to_item_map[VinBlk.fdx_name])
        self.VinPosn1 = VinPosn1(self, name_to_item_map[VinPosn1.fdx_name])
        self.VinPosn2 = VinPosn2(self, name_to_item_map[VinPosn2.fdx_name])
        self.VinPosn3 = VinPosn3(self, name_to_item_map[VinPosn3.fdx_name])
        self.VinPosn4 = VinPosn4(self, name_to_item_map[VinPosn4.fdx_name])
        self.VinPosn5 = VinPosn5(self, name_to_item_map[VinPosn5.fdx_name])
        self.VinPosn6 = VinPosn6(self, name_to_item_map[VinPosn6.fdx_name])
        self.VinPosn7 = VinPosn7(self, name_to_item_map[VinPosn7.fdx_name])
        self.VisnAgWideSts = VisnAgWideSts(self, name_to_item_map[VisnAgWideSts.fdx_name])
        self.VisnImgAgWideInUse = VisnImgAgWideInUse(self, name_to_item_map[VisnImgAgWideInUse.fdx_name])
        self.VolUnit = VolUnit(self, name_to_item_map[VolUnit.fdx_name])
        self.WhlCircum = WhlCircum(self, name_to_item_map[WhlCircum.fdx_name])
        self.WhlMotSysSpdAct = WhlMotSysSpdAct(self, name_to_item_map[WhlMotSysSpdAct.fdx_name])
        self.WhlMotSysTqEst = WhlMotSysTqEst(self, name_to_item_map[WhlMotSysTqEst.fdx_name])
        self.WhlMotSysTqEstChks = WhlMotSysTqEstChks(self, name_to_item_map[WhlMotSysTqEstChks.fdx_name])
        self.WhlMotSysTqEstCntr = WhlMotSysTqEstCntr(self, name_to_item_map[WhlMotSysTqEstCntr.fdx_name])
        self.WhlMotSysTqEstQlyFac = WhlMotSysTqEstQlyFac(self, name_to_item_map[WhlMotSysTqEstQlyFac.fdx_name])
        self.WhlMotSysTqEstSpdLimn = WhlMotSysTqEstSpdLimn(self, name_to_item_map[WhlMotSysTqEstSpdLimn.fdx_name])
        self.WhlRotToothCntrFrntLe = WhlRotToothCntrFrntLe(self, name_to_item_map[WhlRotToothCntrFrntLe.fdx_name])
        self.WhlRotToothCntrFrntRi = WhlRotToothCntrFrntRi(self, name_to_item_map[WhlRotToothCntrFrntRi.fdx_name])
        self.WhlRotToothCntrReLe = WhlRotToothCntrReLe(self, name_to_item_map[WhlRotToothCntrReLe.fdx_name])
        self.WhlRotToothCntrReRi = WhlRotToothCntrReRi(self, name_to_item_map[WhlRotToothCntrReRi.fdx_name])
        self.WinPosnStsAtDrvrRe = WinPosnStsAtDrvrRe(self, name_to_item_map[WinPosnStsAtDrvrRe.fdx_name])
        self.WinPosnStsDrv = WinPosnStsDrv(self, name_to_item_map[WinPosnStsDrv.fdx_name])
        self.WinPosnStsPass = WinPosnStsPass(self, name_to_item_map[WinPosnStsPass.fdx_name])
        self.WinPosnStsRePass = WinPosnStsRePass(self, name_to_item_map[WinPosnStsRePass.fdx_name])
        self.WipgInfoWipgSpdInfo = WipgInfoWipgSpdInfo(self, name_to_item_map[WipgInfoWipgSpdInfo.fdx_name])
        self.WipgInfoWiprActv = WipgInfoWiprActv(self, name_to_item_map[WipgInfoWiprActv.fdx_name])
        self.WipgInfoWiprInWipgAr = WipgInfoWiprInWipgAr(self, name_to_item_map[WipgInfoWiprInWipgAr.fdx_name])
        self.WiprFrntSrvModReq = WiprFrntSrvModReq(self, name_to_item_map[WiprFrntSrvModReq.fdx_name])
        self.WiprInPosnForSrv = WiprInPosnForSrv(self, name_to_item_map[WiprInPosnForSrv.fdx_name])
        self.WiprReAutReqPen = WiprReAutReqPen(self, name_to_item_map[WiprReAutReqPen.fdx_name])
        self.WiprReAutReqSts = WiprReAutReqSts(self, name_to_item_map[WiprReAutReqSts.fdx_name])
        self.WshrFldSts = WshrFldSts(self, name_to_item_map[WshrFldSts.fdx_name])
        self.YawRate1 = YawRate1(self, name_to_item_map[YawRate1.fdx_name])
        self.YawRate1Qf1 = YawRate1Qf1(self, name_to_item_map[YawRate1Qf1.fdx_name])
        self.Yr = Yr(self, name_to_item_map[Yr.fdx_name])
        self.Yr1 = Yr1(self, name_to_item_map[Yr1.fdx_name])

# Controls the ABS warning indication to the driver.
class AbsWarnIndcnReq:
    de_name     = "BrkAndAbsWarnIndcnReq.AbsWarnIndcnReq"
    fdx_name    = "AbsWarnIndcnReq"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        AbsWarnIndcnOnReq = 0
        AbsWarnIndcnFlsgReq = 1
        Resd2 = 2
        AbsWarnIndcnOffReq = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class AccAdprSpdLimActvPen:
    de_name     = "AccAdprSpdLimActv.Pen"
    fdx_name    = "AccAdprSpdLimActvPen"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AccAdprSpdLimActvSts:
    de_name     = "AccAdprSpdLimActv.Sts"
    fdx_name    = "AccAdprSpdLimActvSts"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class AccAdprTurnSpdActvPen:
    de_name     = "AccAdprTurnSpdActv.Pen"
    fdx_name    = "AccAdprTurnSpdActvPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AccAdprTurnSpdActvSts:
    de_name     = "AccAdprTurnSpdActv.Sts"
    fdx_name    = "AccAdprTurnSpdActvSts"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AccAutResuWarnReq:
    de_name     = "AccAutResuWarnReq"
    fdx_name    = "AccAutResuWarnReq"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AccSts:
    de_name     = "AccSts"
    fdx_name    = "AccSts"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 1
        Stb = 2
        Stop = 3
        StopTmp = 4
        Actv = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ActvOfHorn:
    de_name     = "ActvOfHorn"
    fdx_name    = "ActvOfHorn"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class ADataRawSafeChks:
    de_name     = "ADataRawSafe.Chks"
    fdx_name    = "ADataRawSafeChks"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class ADataRawSafeCntr:
    de_name     = "ADataRawSafe.Cntr"
    fdx_name    = "ADataRawSafeCntr"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class AdjSpdLimnSts:
    de_name     = "AdjSpdLimnSts"
    fdx_name    = "AdjSpdLimnSts"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 1
        Stb = 2
        Actv = 3
        Ovrd = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AdjSpdLimnWarn:
    de_name     = "AdjSpdLimnWarn"
    fdx_name    = "AdjSpdLimnWarn"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SoundWarn = 1
        VisWarn = 2
        SoundAndVisWarn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class AgDataRawSafeChks:
    de_name     = "AgDataRawSafe.Chks"
    fdx_name    = "AgDataRawSafeChks"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class AgDataRawSafeCntr:
    de_name     = "AgDataRawSafe.Cntr"
    fdx_name    = "AgDataRawSafeCntr"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class AirClngReq:
    de_name     = "AirClngReq"
    fdx_name    = "AirClngReq"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        On = 1
        Off = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class ALat1:
    de_name     = "ADataRawSafe.ALat"
    fdx_name    = "ALat1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ALat1Qf1:
    de_name     = "ADataRawSafe.ALat1Qf"
    fdx_name    = "ALat1Qf1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class ALgt1:
    de_name     = "ADataRawSafe.ALgt"
    fdx_name    = "ALgt1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ALgt1Qf1:
    de_name     = "ADataRawSafe.ALgt1Qf"
    fdx_name    = "ALgt1Qf1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:-100->309.5, Resolution: (0.1*x+-100.0, raw is unsigned, 12 bits )
class AmbTIndcd:
    de_name     = "AmbTIndcdWithUnit.AmbTIndcd"
    fdx_name    = "AmbTIndcd"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -100
    max    = 309.5
    scale  = 0.1
    offset = -100.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class AmbTIndcdQf:
    de_name     = "AmbTIndcdWithUnit.QF"
    fdx_name    = "AmbTIndcdQf"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AmbTIndcdUnit:
    de_name     = "AmbTIndcdWithUnit.AmbTIndcdUnit"
    fdx_name    = "AmbTIndcdUnit"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Celsius = 0
        Fahrenheit = 1
        UkwnUnit = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AmbTRawQly:
    de_name     = "AmbTRaw.Qly"
    fdx_name    = "AmbTRawQly"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: degC,  Range:-70->134.7, Resolution: (0.1*x+-70.0, raw is unsigned, 11 bits )
class AmbTRawVal:
    de_name     = "AmbTRaw.AmbTVal"
    fdx_name    = "AmbTRawVal"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -70
    max    = 134.7
    scale  = 0.1
    offset = -70.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->7
class AntithftRednReq:
    de_name     = "AntithftRednReq"
    fdx_name    = "AntithftRednReq"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 7
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class AsyLaneChgPsbl:
    de_name     = "AsyLaneChg.Psbl"
    fdx_name    = "AsyLaneChgPsbl"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsbl = 0
        Le = 1
        Ri = 2
        Both = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AsyLaneChgSts:
    de_name     = "AsyLaneChg.Sts"
    fdx_name    = "AsyLaneChgSts"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Stb = 0
        Planned = 1
        ShowInt = 2
        Resd3 = 3
        Executing = 4
        Resd5 = 5
        Finshd = 6
        Aborted = 7
        Resd8 = 8
        Resd9 = 9
        Resd10 = 10

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AsyLaneChgTyp:
    de_name     = "AsyLaneChg.Typ"
    fdx_name    = "AsyLaneChgTyp"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoIntent = 0
        LaneChgLe = 1
        LaneChgRi = 2
        SafeStopInLane = 3
        SafeStopToLe = 4
        SafeStopToRi = 5
        NotAvl = 6
        Resd7 = 7
        Resd8 = 8
        Resd9 = 9
        Resd10 = 10

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class AsySteerApplyRqrd:
    de_name     = "AsySteerApplyRqrd"
    fdx_name    = "AsySteerApplyRqrd"
    fdx_groupid = 1139

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        Lvl1 = 1
        Lvl2 = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class AudMsgReq:
    de_name     = "AudMsgReq"
    fdx_name    = "AudMsgReq"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class AudWarn:
    de_name     = "AudWarn"
    fdx_name    = "AudWarn"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class AudWarnActv:
    de_name     = "AudWarnActv"
    fdx_name    = "AudWarnActv"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class AVert2:
    de_name     = "ADataRawSafe.AVert"
    fdx_name    = "AVert2"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class AVert2Qf1:
    de_name     = "ADataRawSafe.AVertQf"
    fdx_name    = "AVert2Qf1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class BackCntrForMissCom:
    de_name     = "BackCntrForMissCom"
    fdx_name    = "BackCntrForMissCom"
    fdx_groupid = 1366

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: %,  Range:0->102.3, Resolution: (0.1*x+0.0, raw is unsigned, 10 bits )
class BarForFuEco:
    de_name     = "CchForFuEco.BarForFuEco"
    fdx_name    = "BarForFuEco"
    fdx_groupid = 1371

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 102.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: km,  Range:0->2000000, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class BkpOfDstTrvld:
    de_name     = "BkpOfDstTrvld"
    fdx_name    = "BkpOfDstTrvld"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 2000000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class BltLockStAtDrvrForBltLockSt1:
    de_name     = "BltLockStAtDrvr.BltLockSt1"
    fdx_name    = "BltLockStAtDrvrForBltLockSt1"
    fdx_groupid = 1351

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Unlock = 0
        Lock = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtDrvrForDevErrSts2:
    de_name     = "BltLockStAtDrvr.BltLockSts"
    fdx_name    = "BltLockStAtDrvrForDevErrSts2"
    fdx_groupid = 1351

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtPassForBltLockSt1:
    de_name     = "BltLockStAtPass.BltLockSt1"
    fdx_name    = "BltLockStAtPassForBltLockSt1"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Unlock = 0
        Lock = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtPassForBltLockSts:
    de_name     = "BltLockStAtPass.BltLockSts"
    fdx_name    = "BltLockStAtPassForBltLockSts"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Description if a component is equipped or not.
class BltLockStAtRowSecLeForBltLockEquid:
    de_name     = "BltLockStAtRowSecLe.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecLeForBltLockEquid"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Equid = 0
        NotEquid = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowSecLeForBltLockSt1:
    de_name     = "BltLockStAtRowSecLe.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecLeForBltLockSt1"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Unlock = 0
        Lock = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowSecLeForBltLockSts:
    de_name     = "BltLockStAtRowSecLe.BltLockSts"
    fdx_name    = "BltLockStAtRowSecLeForBltLockSts"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Description if a component is equipped or not.
class BltLockStAtRowSecMidForBltLockEquid:
    de_name     = "BltLockStAtRowSecMid.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecMidForBltLockEquid"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Equid = 0
        NotEquid = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowSecMidForBltLockSt1:
    de_name     = "BltLockStAtRowSecMid.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecMidForBltLockSt1"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Unlock = 0
        Lock = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowSecMidForBltLockSts:
    de_name     = "BltLockStAtRowSecMid.BltLockSts"
    fdx_name    = "BltLockStAtRowSecMidForBltLockSts"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Description if a component is equipped or not.
class BltLockStAtRowSecRiForBltLockEquid:
    de_name     = "BltLockStAtRowSecRi.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecRiForBltLockEquid"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Equid = 0
        NotEquid = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowSecRiForBltLockSt1:
    de_name     = "BltLockStAtRowSecRi.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecRiForBltLockSt1"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Unlock = 0
        Lock = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowSecRiForBltLockSts:
    de_name     = "BltLockStAtRowSecRi.BltLockSts"
    fdx_name    = "BltLockStAtRowSecRiForBltLockSts"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Description if a component is equipped or not.
class BltLockStAtRowThrdLeForBltLockEquid:
    de_name     = "BltLockStAtRowThrdLe.BltLockEquid"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockEquid"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Equid = 0
        NotEquid = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowThrdLeForBltLockSt1:
    de_name     = "BltLockStAtRowThrdLe.BltLockSt1"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockSt1"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Unlock = 0
        Lock = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowThrdLeForBltLockSts:
    de_name     = "BltLockStAtRowThrdLe.BltLockSts"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockSts"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Description if a component is equipped or not.
class BltLockStAtRowThrdRiForBltLockEquid:
    de_name     = "BltLockStAtRowThrdRi.BltLockEquid"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockEquid"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Equid = 0
        NotEquid = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowThrdRiForBltLockSt1:
    de_name     = "BltLockStAtRowThrdRi.BltLockSt1"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockSt1"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Unlock = 0
        Lock = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltLockStAtRowThrdRiForBltLockSts:
    de_name     = "BltLockStAtRowThrdRi.BltLockSts"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockSts"
    fdx_groupid = 1352

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltRmnSound1:
    de_name     = "BltRmnSound1"
    fdx_name    = "BltRmnSound1"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltRmnSound2:
    de_name     = "BltRmnSound2"
    fdx_name    = "BltRmnSound2"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltRmnSound3:
    de_name     = "BltRmnSound3"
    fdx_name    = "BltRmnSound3"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BltRmnSound4:
    de_name     = "BltRmnSound4"
    fdx_name    = "BltRmnSound4"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class BrkAndAbsWarnIndcnReqChks:
    de_name     = "BrkAndAbsWarnIndcnReq.BrkAndAbsWarnIndcnReqChks"
    fdx_name    = "BrkAndAbsWarnIndcnReqChks"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class BrkAndAbsWarnIndcnReqCntr:
    de_name     = "BrkAndAbsWarnIndcnReq.BrkAndAbsWarnIndcnReqCntr"
    fdx_name    = "BrkAndAbsWarnIndcnReqCntr"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# 0= High fluid level
# 1= Low fluid level
class BrkFldLvl:
    de_name     = "BrkFldLvl"
    fdx_name    = "BrkFldLvl"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        FldLvlHi = 0
        FldLvlLo = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlFrntLeAct:
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlFrntLeAct"
    fdx_name    = "BrkFricTqAtWhlFrntLeAct"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlFrntRiAct:
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlFrntRiAct"
    fdx_name    = "BrkFricTqAtWhlFrntRiAct"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlReLeAct:
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlReLeAct"
    fdx_name    = "BrkFricTqAtWhlReLeAct"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlReRiAct:
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlReRiAct"
    fdx_name    = "BrkFricTqAtWhlReRiAct"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class BrkRelsWarnReq:
    de_name     = "BrkRelsWarnReq"
    fdx_name    = "BrkRelsWarnReq"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class BrkWarnIndcnReq:
    de_name     = "BrkAndAbsWarnIndcnReq.BrkWarnIndcnReq"
    fdx_name    = "BrkWarnIndcnReq"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        On = 0
        Off = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class Btn1ForUsrSwtPanFrntReq:
    de_name     = "Btn1ForUsrSwtPanFrntReq"
    fdx_name    = "Btn1ForUsrSwtPanFrntReq"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class Btn2ForUsrSwtPanFrntReq:
    de_name     = "Btn2ForUsrSwtPanFrntReq"
    fdx_name    = "Btn2ForUsrSwtPanFrntReq"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class Btn3ForUsrSwtPanFrntReq:
    de_name     = "Btn3ForUsrSwtPanFrntReq"
    fdx_name    = "Btn3ForUsrSwtPanFrntReq"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class Btn4ForUsrSwtPanFrntReq:
    de_name     = "Btn4ForUsrSwtPanFrntReq"
    fdx_name    = "Btn4ForUsrSwtPanFrntReq"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class Btn5ForUsrSwtPanFrntReq:
    de_name     = "Btn5ForUsrSwtPanFrntReq"
    fdx_name    = "Btn5ForUsrSwtPanFrntReq"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BtnAudFbSts:
    de_name     = "BtnAudFbSts"
    fdx_name    = "BtnAudFbSts"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# SImple representation of  a virtual (multistep) button for adjusting audio volume.
# 0 = button not pressed. No request to adjust volume
# 1 = button pressed for "volume up"
# 2 = buttono pressed for "volume down"
# 
# Receiver side controls how the button status is used.
class BtnAudVolSts:
    de_name     = "BtnAudVolSts"
    fdx_name    = "BtnAudVolSts"
    fdx_groupid = 1231

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoBtnPsd = 0
        BtnVolUpPsd = 1
        BtnVolDwnPsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Represents status of a (multilevel) button "Left/Right. 
# 
# Semantic defined by receiver side. (Previou/Next, FFBW/FFWD, etc)
class BtnMmedLeRiSts:
    de_name     = "BtnMmedLeRiSts"
    fdx_name    = "BtnMmedLeRiSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoBtnPsd = 0
        BtnLePsd = 1
        BtnRiPsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class BtnMmedModSts:
    de_name     = "BtnMmedModSts"
    fdx_name    = "BtnMmedModSts"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Represents status of a button "Set". 
# 
# Semantic defined by receiver side. (Activate/Deactivate, Play/Pause, Mute/UnMute etc)
class BtnMmedSetSts:
    de_name     = "BtnMmedSetSts"
    fdx_name    = "BtnMmedSetSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        BtnSetNotPsd = 0
        BtnSetPsd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries call presence information. Defined for CallSts range [0-1]
# 
# 0 = Inactive. There is no active call present with any telephony solution in the system.
# 1 = Active. Atleast one active call is present in the system.
class CallStsIndcn:
    de_name     = "CallSts"
    fdx_name    = "CallStsIndcn"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Inactive = 0
        Active = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CarModSts1:
    de_name     = "VehModMngtGlbSafe1.CarModSts1"
    fdx_name    = "CarModSts1"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        CarModNorm = 0
        CarModTrnsp = 1
        CarModFcy = 2
        CarModCrash = 3
        CarModDyno = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->7
class CarModSubtypWdCarModSubtyp:
    de_name     = "VehModMngtGlbSafe1.CarModSubtypWdCarModSubtyp"
    fdx_name    = "CarModSubtypWdCarModSubtyp"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 7
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: s,  Range:0->4.294967294E8, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class CarTiGlb:
    de_name     = "CarTiGlb"
    fdx_name    = "CarTiGlb"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 4.294967294E8
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class CCSMBtn6:
    de_name     = "CCSMBtn6"
    fdx_name    = "CCSMBtn6"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ChdLockgProtnStsToHmi:
    de_name     = "ChdLockgProtnStsToHmi"
    fdx_name    = "ChdLockgProtnStsToHmi"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        OnOffSafeInvld1 = 0
        OnOffSafeOn = 1
        OnOffSafeOff = 2
        OnOffSafeInvld2 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ChdWinProtnStsAtDrvrRe:
    de_name     = "ChdWinProtnStsAtDrvrRe"
    fdx_name    = "ChdWinProtnStsAtDrvrRe"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ChdWinProtnStsAtPassRe:
    de_name     = "ChdWinProtnStsAtPassRe"
    fdx_name    = "ChdWinProtnStsAtPassRe"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ClimaActv:
    de_name     = "ClimaActv"
    fdx_name    = "ClimaActv"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ClimaEcoModRqrd:
    de_name     = "PostDrvgClimaReq"
    fdx_name    = "ClimaEcoModRqrd"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: W,  Range:0->2047, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class ClimaPwrCns:
    de_name     = "ClimaPwrCns"
    fdx_name    = "ClimaPwrCns"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 2047
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ClimaRqrd:
    de_name     = "ClimaRqrdFromHmi"
    fdx_name    = "ClimaRqrd"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        On = 1
        Off = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: s,  Range:0->4.294967294E8, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class ClimaTmr:
    de_name     = "ClimaTmr"
    fdx_name    = "ClimaTmr"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 4.294967294E8
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ClimaTmrSts:
    de_name     = "ClimaTmrSts"
    fdx_name    = "ClimaTmrSts"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ClimaTmrStsRqrd:
    de_name     = "ClimaTmrStsRqrd"
    fdx_name    = "ClimaTmrStsRqrd"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# 0=No Warning
# 1=Fuel Low
# 2=Battery Low
# 3=Fuel and Battery Low
# 4=Temperature Low
# 5=Temperature High
class ClimaWarn:
    de_name     = "ClimaWarn"
    fdx_name    = "ClimaWarn"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        FuLo = 1
        BattLo = 2
        FuAndBattLo = 3
        TLo = 4
        THi = 5
        Error = 6
        HVError = 7
        ActvnLimd = 8

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# 24h or 12h indication of clock
class ClkFmt:
    de_name     = "IndcnUnit.ClkFmt"
    fdx_name    = "ClkFmt"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Hr24 = 0
        Hr12 = 1
        UkwnClkFmt = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class CllsnFwdWarnActvPen:
    de_name     = "CllsnFwdWarnActv.Pen"
    fdx_name    = "CllsnFwdWarnActvPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CllsnFwdWarnActvSts:
    de_name     = "CllsnFwdWarnActv.Sts"
    fdx_name    = "CllsnFwdWarnActvSts"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CllsnFwdWarnReq:
    de_name     = "CllsnFwdWarnReq"
    fdx_name    = "CllsnFwdWarnReq"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CllsnWarnReq:
    de_name     = "CllsnWarnReq"
    fdx_name    = "CllsnWarnReq"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CllsnWarnSideLe:
    de_name     = "CllsnWarnSide.Le"
    fdx_name    = "CllsnWarnSideLe"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        WarnLvl1 = 1
        WarnLvl2 = 2
        Resd3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CllsnWarnSideRi:
    de_name     = "CllsnWarnSide.Ri"
    fdx_name    = "CllsnWarnSideRi"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        WarnLvl1 = 1
        WarnLvl2 = 2
        Resd3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ClngActv:
    de_name     = "ClngActv"
    fdx_name    = "ClngActv"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        On = 1
        Off = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ClngRqrdFromHmi:
    de_name     = "ClngRqrdFromHmi"
    fdx_name    = "ClngRqrdFromHmi"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        On = 1
        Off = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ClsAutEna:
    de_name     = "ClsAutEna"
    fdx_name    = "ClsAutEna"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Disabled = 0
        Enabled = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CmftFctActv:
    de_name     = "CmftFctActv"
    fdx_name    = "CmftFctActv"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class CmftFctSts:
    de_name     = "CmftFctSts"
    fdx_name    = "CmftFctSts"
    fdx_groupid = 1366

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )
class CmptmtAirTEstimdExtdComptmtT:
    de_name     = "CmptmtAirTEstimdExtd.ComptmtT"
    fdx_name    = "CmptmtAirTEstimdExtdComptmtT"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -60
    max    = 125
    scale  = 0.1
    offset = -60.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class CmptmtAirTEstimdExtdQlyFlg:
    de_name     = "CmptmtAirTEstimdExtd.QlyFlg"
    fdx_name    = "CmptmtAirTEstimdExtdQlyFlg"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )
class CmptmtTFrnt:
    de_name     = "CmptmtTFrnt.CmptmtTFrnt"
    fdx_name    = "CmptmtTFrnt"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -60
    max    = 125
    scale  = 0.1
    offset = -60.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )
class CmptmtTFrnt:
    de_name     = "CmptmtTFrnt.CmptmtTFrnt"
    fdx_name    = "CmptmtTFrnt"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -60
    max    = 125
    scale  = 0.1
    offset = -60.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Interpretation:
# SnsrDataUndefd - Accuracy of data undefinable
# FanNotRunning - Aspiration fan not running
# SnsrDataNotOk - Data accuracy not within specification
# SnsrDataOk - Data is calculated within specified accuracy
class CmptmtTFrntQf:
    de_name     = "CmptmtTFrnt.CmptmtTFrntQf"
    fdx_name    = "CmptmtTFrntQf"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        SnsrDataUndefd = 0
        FanNotRunning = 1
        SnsrDataNotOk = 2
        SnsrDataOk = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Interpretation:
# SnsrDataUndefd - Accuracy of data undefinable
# FanNotRunning - Aspiration fan not running
# SnsrDataNotOk - Data accuracy not within specification
# SnsrDataOk - Data is calculated within specified accuracy
class CmptmtTFrntQf:
    de_name     = "CmptmtTFrnt.CmptmtTFrntQf"
    fdx_name    = "CmptmtTFrntQf"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        SnsrDataUndefd = 0
        FanNotRunning = 1
        SnsrDataNotOk = 2
        SnsrDataOk = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnForCrsCtrl:
    de_name     = "CnclWarnForCrsCtrl"
    fdx_name    = "CnclWarnForCrsCtrl"
    fdx_groupid = 1378

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnLatForAutDrv:
    de_name     = "SftyWarnGroupFromAsySafe.CnclWarnLatForAutDrv"
    fdx_name    = "CnclWarnLatForAutDrv"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnLgtForAutDrv:
    de_name     = "CnclWarnLgtForAutDrv"
    fdx_name    = "CnclWarnLgtForAutDrv"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CntDwnToManvStrtInDstToManvLocn:
    de_name     = "DstToManvLocn.CntDwnToManvStrt"
    fdx_name    = "CntDwnToManvStrtInDstToManvLocn"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ComLostExtrSound:
    de_name     = "ComLostExtrSound"
    fdx_name    = "ComLostExtrSound"
    fdx_groupid = 1375

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class ConSftyWarnConSftyWarnId:
    de_name     = "ConSftyWarn.ConSftyWarnId"
    fdx_name    = "ConSftyWarnConSftyWarnId"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ConSftyWarnConSftyWarnLvl:
    de_name     = "ConSftyWarn.ConSftyWarnLvl"
    fdx_name    = "ConSftyWarnConSftyWarnLvl"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        LoLvl = 1
        MedLvl = 2
        HiLvl = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ConSftyWarnConSftyWarnSnd:
    de_name     = "ConSftyWarn.ConSftyWarnSnd"
    fdx_name    = "ConSftyWarnConSftyWarnSnd"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->15
class ConSftyWarnConSftyWarnTyp:
    de_name     = "ConSftyWarn.ConSftyWarnTyp"
    fdx_name    = "ConSftyWarnConSftyWarnTyp"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m,  Range:0->65535
class ConSftyWarnDistanceToWarning:
    de_name     = "ConSftyWarn.DistanceToWarning"
    fdx_name    = "ConSftyWarnDistanceToWarning"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Interpretation:
# NoReq - No active cooling requested
# LoReq - Low cooling need
# HiReq - High cooling need
class CoolgReqForDispCen:
    de_name     = "CoolgReqForDispCen"
    fdx_name    = "CoolgReqForDispCen"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        LoReq = 1
        HiReq = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Interpretation:
# NoCoolgActv - No cooling request received, no actions taken
# CoolgLvlStrtd1 - Activated cooling in level 1 started
# CoolgLvlStrtd2 - Activated cooling in level 2 started
class CoolgStsForDisp:
    de_name     = "CoolgStsForDisp"
    fdx_name    = "CoolgStsForDisp"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoCoolgActv = 0
        CoolgLvlStrtd1 = 1
        CoolgLvlStrtd2 = 2
        Resd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CrsCtrlrSts:
    de_name     = "CrsCtrlrSts"
    fdx_name    = "CrsCtrlrSts"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 1
        Stb = 2
        Actv = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CrvtSpdAdpvSts:
    de_name     = "CrvtSpdAdpvSts"
    fdx_name    = "CrvtSpdAdpvSts"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        NotAvl = 2
        SrvRqrd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No warning
# 1			1			WarnForCmngCrvtLe	Warning For Coming Curvature Left	Warning for upcoming left curve
# 2			2			WarnForCmngCrvtRi	Warning For Coming Curvature Right	Warning for upcoming right curve
class CrvtSpdWarnReq:
    de_name     = "CrvtSpdWarnReq"
    fdx_name    = "CrvtSpdWarnReq"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        WarnForCmngCrvtLe = 1
        WarnForCmngCrvtRi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class CrvtSpdWarnSts:
    de_name     = "CrvtSpdWarnSts"
    fdx_name    = "CrvtSpdWarnSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CtraIndcnLe:
    de_name     = "CtraIndcnLe"
    fdx_name    = "CtraIndcnLe"
    fdx_groupid = 1139

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoCtraWarn = 0
        CtraWarn = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CtraIndcnRi:
    de_name     = "CtraIndcnRi"
    fdx_name    = "CtraIndcnRi"
    fdx_groupid = 1139

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoCtraWarn = 0
        CtraWarn = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CtraOn1:
    de_name     = "CtraOn1"
    fdx_name    = "CtraOn1"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        StrtUpOn = 0
        On = 1
        Off = 2
        TrlrOff = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CtraSwOnOff:
    de_name     = "CtraSwOnOff"
    fdx_name    = "CtraSwOnOff"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class CurtActvnReReq:
    de_name     = "CurtActvnReReq"
    fdx_name    = "CurtActvnReReq"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        BtnReld = 0
        BtnPsd = 1
        Spare1 = 2
        Spare2 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class DataSpclDMSMDataIdn:
    de_name     = "DataSpclDMSM.DataIdn"
    fdx_name    = "DataSpclDMSMDataIdn"
    fdx_groupid = 1226

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class DataSpclDMSMDataNrSpcl1:
    de_name     = "DataSpclDMSM.DataNrSpcl1"
    fdx_name    = "DataSpclDMSMDataNrSpcl1"
    fdx_groupid = 1226

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class DataSpclDMSMDataNrSpcl2:
    de_name     = "DataSpclDMSM.DataNrSpcl2"
    fdx_name    = "DataSpclDMSMDataNrSpcl2"
    fdx_groupid = 1226

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class DataSpclDMSMDataNrSpcl3:
    de_name     = "DataSpclDMSM.DataNrSpcl3"
    fdx_name    = "DataSpclDMSMDataNrSpcl3"
    fdx_groupid = 1226

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class DateOrTi:
    de_name     = "SetTiAndDate.DateOrTi"
    fdx_name    = "DateOrTi"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        None_ = 0
        Date = 1
        Ti = 2
        DateTi = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Days,  Range:1->31
class Day:
    de_name     = "TiAndDateIndcn.Day"
    fdx_name    = "Day"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 1
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Days,  Range:1->31
class Day1:
    de_name     = "SetTiAndDate.Day"
    fdx_name    = "Day1"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 1
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Days,  Range:0->750
class DayToSrv:
    de_name     = "DayToSrv"
    fdx_name    = "DayToSrv"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 750
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class DiagcCCSM:
    de_name     = "DiagcCCSM"
    fdx_name    = "DiagcCCSM"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        FanErr = 1
        OutdURng = 2
        TmrErr = 3
        MemErr = 4
        Spare6 = 5
        Spare7 = 6
        SnrFltT = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DiagcRCSM:
    de_name     = "DiagcRCSM"
    fdx_name    = "DiagcRCSM"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        SpiErr = 1
        OutdURng = 2
        TmrErr = 3
        MemErr = 4
        ADErr = 5
        Spare1 = 6
        Spare2 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DiagcRSHC:
    de_name     = "DiagcRSHC"
    fdx_name    = "DiagcRSHC"
    fdx_groupid = 1335

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        SpiErr = 1
        OutdURng = 2
        TmrErr = 3
        MemErr = 4
        ADErr = 5
        Spare1 = 6
        Spare2 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DiagcStsDMSM:
    de_name     = "DiagcStsDMSM"
    fdx_name    = "DiagcStsDMSM"
    fdx_groupid = 1226

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        OK = 0
        CmnFailr = 1
        Spare1 = 2
        Spare2 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DispAndHomeBtnSts:
    de_name     = "DispAndHomeBtnSts"
    fdx_name    = "DispAndHomeBtnSts"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Wh,  Range:0->127.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class DispBattEgyIn:
    de_name     = "DispBattEgyIn"
    fdx_name    = "DispBattEgyIn"
    fdx_groupid = 1375

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 127.5
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Wh,  Range:0->127.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class DispBattEgyOut:
    de_name     = "DispBattEgyOut"
    fdx_name    = "DispBattEgyOut"
    fdx_groupid = 1375

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 127.5
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class DispOfPrpsnMod:
    de_name     = "DispOfPrpsnMod"
    fdx_name    = "DispOfPrpsnMod"
    fdx_groupid = 1372

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotRdy = 0
        ChrgnWithOnBdChrgr = 1
        PrpsnMotElecAndPrpsnEng = 2
        PrpsnMotElecAndPrpsnEngAndChrgn = 3
        OnlyEngPrpsn = 4
        OnlyEngPrpsnAndChrgn = 5
        OnlyPrpsnMotElec = 6
        NoPrpsnMotElecAndEngOff = 7
        NoPrpsnMotElecAndOnEng = 8
        NoPrpsnMotElecAndOnEngAndChrgn = 9
        BrkgRgnAndEngOff = 10
        BrkgRgnAndOnEng = 11
        BrkgRgnAndEngOnAndChrgn = 12
        NotOfUse2 = 13
        NotOfUse3 = 14
        NotOfUse4 = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DoorDrvrLockReSts:
    de_name     = "DoorDrvrLockReSts"
    fdx_name    = "DoorDrvrLockReSts"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        LockStsUkwn = 0
        Unlckd = 1
        Lockd = 2
        SafeLockd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DoorDrvrLockSts:
    de_name     = "DoorDrvrLockSts"
    fdx_name    = "DoorDrvrLockSts"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        LockStsUkwn = 0
        Unlckd = 1
        Lockd = 2
        SafeLockd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Status of the door.
class DoorDrvrReSts:
    de_name     = "DoorDrvrReSts"
    fdx_name    = "DoorDrvrReSts"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Status of the door.
class DoorDrvrSts:
    de_name     = "DoorDrvrSts"
    fdx_name    = "DoorDrvrSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DoorPassLockReSts:
    de_name     = "DoorPassLockReSts"
    fdx_name    = "DoorPassLockReSts"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        LockStsUkwn = 0
        Unlckd = 1
        Lockd = 2
        SafeLockd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DoorPassLockSts:
    de_name     = "DoorPassLockSts"
    fdx_name    = "DoorPassLockSts"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        LockStsUkwn = 0
        Unlckd = 1
        Lockd = 2
        SafeLockd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Status of the door.
class DoorPassReSts:
    de_name     = "DoorPassReSts"
    fdx_name    = "DoorPassReSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Status of the door.
class DoorPassSts:
    de_name     = "DoorPassSts"
    fdx_name    = "DoorPassSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DriveAwayInfoWarnReqAudWarnReq:
    de_name     = "DriveAwayInfoWarnReq.AudWarnReq"
    fdx_name    = "DriveAwayInfoWarnReqAudWarnReq"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        PrepareToLeave = 1
        LeaveNow = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DriveAwayInfoWarnReqCtrlDirOfTrfcLi:
    de_name     = "DriveAwayInfoWarnReq.CtrlDirOfTrfcLi"
    fdx_name    = "DriveAwayInfoWarnReqCtrlDirOfTrfcLi"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoDirection = 0
        Straight = 1
        Left = 2
        LeftStraight = 3
        Right = 4
        StraightRight = 5
        LeftRight = 6
        All = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DriveAwayInfoWarnReqInterruptionSrc:
    de_name     = "DriveAwayInfoWarnReq.InterruptionSrc"
    fdx_name    = "DriveAwayInfoWarnReqInterruptionSrc"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        TiOut = 1
        ResuPsd = 2
        BrkPedl = 3
        AccrPedlPsd = 4
        Resd5 = 5
        Resd6 = 6
        Resd7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DriveAwayInfoWarnReqReqSrc:
    de_name     = "DriveAwayInfoWarnReq.ReqSrc"
    fdx_name    = "DriveAwayInfoWarnReqReqSrc"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        TtgReq = 1
        Resd2 = 2
        Resd3 = 3
        Resd4 = 4
        Resd5 = 5
        Resd6 = 6
        Resd7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class DriveAwayInfoWarnReqTiToDrvCntDwnTi:
    de_name     = "DriveAwayInfoWarnReq.TiToDrvCntDwnTi"
    fdx_name    = "DriveAwayInfoWarnReqTiToDrvCntDwnTi"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class DriveAwayInfoWarnReqVisWarnReq:
    de_name     = "DriveAwayInfoWarnReq.VisWarnReq"
    fdx_name    = "DriveAwayInfoWarnReqVisWarnReq"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Countdown = 1
        LeaveNow = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvgInWrgDirOfTrfc:
    de_name     = "DrvgInWrgDirOfTrfc"
    fdx_name    = "DrvgInWrgDirOfTrfc"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvModReq:
    de_name     = "DrvModReq"
    fdx_name    = "DrvModReq"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Undefd = 0
        DrvMod1 = 1
        DrvMod2 = 2
        DrvMod3 = 3
        DrvMod4 = 4
        DrvMod5 = 5
        DrvMod6 = 6
        DrvMod7 = 7
        DrvMod8 = 8
        DrvMod9 = 9
        DrvMod10 = 10
        DrvMod11 = 11
        DrvMod12 = 12
        DrvMod13 = 13
        DrvMod14 = 14
        Err = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrCtrlOfPassSeatFrntReqd:
    de_name     = "DrvrCtrlOfPassSeatFrntReqd"
    fdx_name    = "DrvrCtrlOfPassSeatFrntReqd"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrCtrlOfPassSeatFrntSts:
    de_name     = "DrvrCtrlOfPassSeatFrntSts"
    fdx_name    = "DrvrCtrlOfPassSeatFrntSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotAvl = 0
        Avl = 1
        Deactvd = 2
        Actvd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Describes the desired vehicle movement direction from the driver.
# Contains only the drivers longitudinal direction intent. In which direction shall the car move.
class DrvrDesDir:
    de_name     = "DrvrDesDir"
    fdx_name    = "DrvrDesDir"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Undefd = 0
        Fwd = 1
        Rvs = 2
        Neut = 3
        Resd0 = 4
        Resd1 = 5
        Resd2 = 6
        Resd3 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrDispSetgPen:
    de_name     = "DrvrDispSetg.Pen"
    fdx_name    = "DrvrDispSetgPen"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrDispSetgSts:
    de_name     = "DrvrDispSetg.Sts"
    fdx_name    = "DrvrDispSetgSts"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Cmft = 0
        Eco = 1
        Dyn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiBackGndInfoSetgPen:
    de_name     = "DrvrHmiBackGndInfoSetg.Pen"
    fdx_name    = "DrvrHmiBackGndInfoSetgPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrHmiBackGndInfoSetgSetg:
    de_name     = "DrvrHmiBackGndInfoSetg.Setg"
    fdx_name    = "DrvrHmiBackGndInfoSetgSetg"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DrvrHmiBackGndInfoSetg0 = 0
        DrvrHmiBackGndInfoSetg1 = 1
        DrvrHmiBackGndInfoSetg2 = 2
        DrvrHmiBackGndInfoSetg3 = 3
        DrvrHmiBackGndInfoSetg4 = 4
        DrvrHmiBackGndInfoSetg5 = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiDispdModPen:
    de_name     = "DrvrHmiDispdMod.Pen"
    fdx_name    = "DrvrHmiDispdModPen"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrHmiDispdModSts:
    de_name     = "DrvrHmiDispdMod.Sts"
    fdx_name    = "DrvrHmiDispdModSts"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DispDftMod = 0
        DispAudMod = 1
        DispNavMod = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiUsrIfSetgPen:
    de_name     = "DrvrHmiUsrIfSetg.Pen"
    fdx_name    = "DrvrHmiUsrIfSetgPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrHmiUsrIfSetgSetg:
    de_name     = "DrvrHmiUsrIfSetg.Setg"
    fdx_name    = "DrvrHmiUsrIfSetgSetg"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UsrIfDft = 0
        UsrIfVrnt1 = 1
        UsrIfVrnt2 = 2
        UsrIfVrnt3 = 3
        UsrIfVrnt4 = 4
        UsrIfVrnt5 = 5
        UsrIfVrnt6 = 6
        UsrIfVrnt7 = 7
        UsrIfVrnt8 = 8
        UsrIfVrnt9 = 9

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrMassgRunng:
    de_name     = "DrvrMassgRunng"
    fdx_name    = "DrvrMassgRunng"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrPfmncLvl:
    de_name     = "DrvrPfmncLvl"
    fdx_name    = "DrvrPfmncLvl"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        DrvrLvl1 = 1
        DrvrLvl2 = 2
        DrvrLvl3 = 3
        DrvrLvl4 = 4
        DrvrLvl5 = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrPfmncMonActvPen:
    de_name     = "DrvrPfmncMonActv.Pen"
    fdx_name    = "DrvrPfmncMonActvPen"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrPfmncMonActvSts:
    de_name     = "DrvrPfmncMonActv.Sts"
    fdx_name    = "DrvrPfmncMonActvSts"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class DrvrPfmncSts:
    de_name     = "DrvrPfmncSts"
    fdx_name    = "DrvrPfmncSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No Warning
# 1			1			WarnForDrvrNotActv	Warning For Driver Not Active	Warning for Inattentive Driver
# 2			2			WarnForDrvrTire		Warning For Driver Tire		Warning for Sleepy Driver
# 3			3			NotInUse			Not In Use			Not used
class DrvrPfmncWarnReq:
    de_name     = "DrvrPfmncWarnReq"
    fdx_name    = "DrvrPfmncWarnReq"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        WarnForDrvrNotActv = 1
        WarnForDrvrTire = 2
        NotInUse = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatActvSpplFct:
    de_name     = "DrvrSeatActvSpplFct"
    fdx_name    = "DrvrSeatActvSpplFct"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotAvl = 0
        LumExtnAndLumHei = 1
        BackBlster = 2
        CushExtn = 3
        HdrestHeiAndHdrestTilt = 4
        MassgFct = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatDispMassgFctMassgInten:
    de_name     = "DrvrSeatDispMassgFct.MassgInten"
    fdx_name    = "DrvrSeatDispMassgFctMassgInten"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatDispMassgFctMassgProg:
    de_name     = "DrvrSeatDispMassgFct.MassgProg"
    fdx_name    = "DrvrSeatDispMassgFctMassgProg"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Prog1 = 0
        Prog2 = 1
        Prog3 = 2
        Prog4 = 3
        Prog5 = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatDispMassgFctMassgSpdLvl:
    de_name     = "DrvrSeatDispMassgFct.MassgSpdLvl"
    fdx_name    = "DrvrSeatDispMassgFctMassgSpdLvl"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatDispMassgFctOnOff:
    de_name     = "DrvrSeatDispMassgFct.OnOff"
    fdx_name    = "DrvrSeatDispMassgFctOnOff"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatDispSpplFct:
    de_name     = "DrvrSeatDispSpplFct"
    fdx_name    = "DrvrSeatDispSpplFct"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotAvl = 0
        LumExtnAndLumHei = 1
        BackBlster = 2
        CushExtn = 3
        HdrestHeiAndHdrestTilt = 4
        MassgFct = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatMassgFctMassgInten:
    de_name     = "DrvrSeatMassgFct.MassgInten"
    fdx_name    = "DrvrSeatMassgFctMassgInten"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatMassgFctMassgProg:
    de_name     = "DrvrSeatMassgFct.MassgProg"
    fdx_name    = "DrvrSeatMassgFctMassgProg"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Prog1 = 0
        Prog2 = 1
        Prog3 = 2
        Prog4 = 3
        Prog5 = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatMassgFctMassgSpdLvl:
    de_name     = "DrvrSeatMassgFct.MassgSpdLvl"
    fdx_name    = "DrvrSeatMassgFctMassgSpdLvl"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatMassgFctOnOff:
    de_name     = "DrvrSeatMassgFct.OnOff"
    fdx_name    = "DrvrSeatMassgFctOnOff"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatSwtAdjmtOfSpplFctHozlSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_name    = "DrvrSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatSwtAdjmtOfSpplFctVertSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtAdjmtOfSpplFctVertSts"
    fdx_name    = "DrvrSeatSwtAdjmtOfSpplFctVertSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatSwtHeiFrntSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtHeiFrntSts"
    fdx_name    = "DrvrSeatSwtHeiFrntSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatSwtHeiSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtHeiSts"
    fdx_name    = "DrvrSeatSwtHeiSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatSwtInclSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtInclSts"
    fdx_name    = "DrvrSeatSwtInclSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatSwtSelnOfSpplFctSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtSelnOfSpplFctSts"
    fdx_name    = "DrvrSeatSwtSelnOfSpplFctSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrSeatSwtSldSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtSldSts"
    fdx_name    = "DrvrSeatSwtSldSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DrvrWLoadLvl:
    de_name     = "DrvrWLoadLvl"
    fdx_name    = "DrvrWLoadLvl"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoLvl = 0
        LoLvl = 1
        MidLvl = 2
        HiLvl = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DstLong:
    de_name     = "IndcnUnit.DstLong"
    fdx_name    = "DstLong"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        km = 0
        miles = 1
        UkwnUnit = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class DstNotifSts:
    de_name     = "DstNotifSts"
    fdx_name    = "DstNotifSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class DstSho:
    de_name     = "IndcnUnit.DstSho"
    fdx_name    = "DstSho"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Mtr = 0
        Ft = 1
        Yards = 2
        UkwnUnit = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->2000
class DstToEmpty:
    de_name     = "DstToEmptyIndcd.DstToEmpty"
    fdx_name    = "DstToEmpty"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 2000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m,  Range:0->4000
class DstToManvInDstToManvLocn:
    de_name     = "DstToManvLocn.DstToManv"
    fdx_name    = "DstToManvInDstToManvLocn"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 4000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: km,  Range:0->32000
class DstToSrv:
    de_name     = "DstToSrv"
    fdx_name    = "DstToSrv"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 32000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: km,  Range:0->9999.9, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class DstTrvld1:
    de_name     = "DstTrvld1"
    fdx_name    = "DstTrvld1"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 9999.9
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: km,  Range:0->9999.9, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class DstTrvld2:
    de_name     = "DstTrvld2"
    fdx_name    = "DstTrvld2"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 9999.9
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m,  Range:0->255
class DstTrvldHiResl:
    de_name     = "DstTrvldHiResl"
    fdx_name    = "DstTrvldHiResl"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Milg,  Range:0->1000000
class DstTrvldMst2:
    de_name     = "DstTrvldMst2"
    fdx_name    = "DstTrvldMst2"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 1000000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class DstUnit:
    de_name     = "DstToEmptyIndcd.DstUnit"
    fdx_name    = "DstUnit"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        km = 0
        miles = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class EasyInOutDrvrSeatAdjmtPen:
    de_name     = "EasyInOutDrvrSeatAdjmt.Pen"
    fdx_name    = "EasyInOutDrvrSeatAdjmtPen"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class EasyInOutDrvrSeatAdjmtSts:
    de_name     = "EasyInOutDrvrSeatAdjmt.Sts"
    fdx_name    = "EasyInOutDrvrSeatAdjmtSts"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->15
class EgyLvlElecMai:
    de_name     = "VehModMngtGlbSafe1.EgyLvlElecMai"
    fdx_name    = "EgyLvlElecMai"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class EgyLvlElecSubtyp:
    de_name     = "VehModMngtGlbSafe1.EgyLvlElecSubtyp"
    fdx_name    = "EgyLvlElecSubtyp"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class EgySave:
    de_name     = "EgySave"
    fdx_name    = "EgySave"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class EmgyAsscSts:
    de_name     = "EmgyAsscSts"
    fdx_name    = "EmgyAsscSts"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        AsscDataTrfInin = 1
        AsscDataTrf = 2
        AsscReqActv = 3
        AsscReqNotAprvd = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class EmgyVehWarnSts:
    de_name     = "EmgyVehWarnSts"
    fdx_name    = "EmgyVehWarnSts"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class EngCooltIndcnReq:
    de_name     = "EngCooltIndcnReq"
    fdx_name    = "EngCooltIndcnReq"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        MsgNoWarn = 0
        EngTHiToSpdRedn = 1
        EngTHiToStopSafe = 2
        EngTHiToEngStop = 3
        EngTHiToMan = 4
        CooltLvlLoToEngStop = 5
        CooltLvlLoToStopSafe = 6
        CooltLvlLoToMan = 7
        CooltLvlLoToLvlChk = 8
        MsgSrvRqrd = 9

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# 0= High fluid level
# 1= Low fluid level
class EngCooltLvl:
    de_name     = "EngCooltLvl"
    fdx_name    = "EngCooltLvl"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        FldLvlHi = 0
        FldLvlLo = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class EngFuCns:
    de_name     = "EngFuCns"
    fdx_name    = "EngFuCns"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class EngFuCnsFild:
    de_name     = "EngFuCnsFild"
    fdx_name    = "EngFuCnsFild"
    fdx_groupid = 1376

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: hours,  Range:0->2000
class EngHrToSrv:
    de_name     = "EngHrToSrv"
    fdx_name    = "EngHrToSrv"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 2000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: rpm,  Range:0->16383, Resolution: (0.5*x+0.0, raw is unsigned, 15 bits )
class EngN:
    de_name     = "EngNSafe.EngN"
    fdx_name    = "EngN"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 16383
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class EngNChks:
    de_name     = "EngNSafe.EngNChks"
    fdx_name    = "EngNChks"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class EngNCntr:
    de_name     = "EngNSafe.EngNCntr"
    fdx_name    = "EngNCntr"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: RpmPerSec,  Range:-30000->30000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class EngNSafeEngNGrdt:
    de_name     = "EngNSafe.EngNGrdt"
    fdx_name    = "EngNSafeEngNGrdt"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -30000
    max    = 30000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: %,  Range:0->120, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class EngOilLvl:
    de_name     = "EngOilLvl"
    fdx_name    = "EngOilLvl"
    fdx_groupid = 1375

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 120
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Engine oil level warning information
class EngOilLvlSts:
    de_name     = "EngOilLvlSts"
    fdx_name    = "EngOilLvlSts"
    fdx_groupid = 1375

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        OilLvlOk = 0
        OilLvlLo1 = 1
        OilLvlLo2 = 2
        OilLvlHi = 3
        OilLvlSrvRqrd = 4
        Resd = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Engine oil pressure warning information
class EngOilPWarn:
    de_name     = "EngOilPWarn"
    fdx_name    = "EngOilPWarn"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        EngOilPOk = 0
        EngOilPNotOk = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: rpm,  Range:0->16383.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class EngSpdDispd:
    de_name     = "EngSpdDispd"
    fdx_name    = "EngSpdDispd"
    fdx_groupid = 1371

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 16383.5
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class EngStrtStopSetg:
    de_name     = "EngStrtStopSetg"
    fdx_name    = "EngStrtStopSetg"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        On = 0
        Off = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class EpbLampReq:
    de_name     = "EpbLampReq.EpbLampReq"
    fdx_name    = "EpbLampReq"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        On = 0
        Off = 1
        Flash2 = 2
        Flash3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class EpbLampReqChks:
    de_name     = "EpbLampReq.EpbLampReqChks"
    fdx_name    = "EpbLampReqChks"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15, Resolution: (1.0*x+0.0, raw is unsigned, 4 bits )
class EpbLampReqCntr:
    de_name     = "EpbLampReq.EpbLampReqCntr"
    fdx_name    = "EpbLampReqCntr"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 15
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Information on activation/ deactivation of Electronic Stability Control (ESC)
class EscSt:
    de_name     = "EscSt.EscSt"
    fdx_name    = "EscSt"
    fdx_groupid = 1372

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Inin = 0
        Ok = 1
        TmpErr = 2
        PrmntErr = 3
        UsrOff = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class EscStChks:
    de_name     = "EscSt.EscStChks"
    fdx_name    = "EscStChks"
    fdx_groupid = 1372

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class EscStCntr:
    de_name     = "EscSt.EscStCntr"
    fdx_name    = "EscStCntr"
    fdx_groupid = 1372

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class EscWarnIndcnReq:
    de_name     = "EscWarnIndcnReq.EscWarnIndcnReq"
    fdx_name    = "EscWarnIndcnReq"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        EscWarnIndcnOnReq = 0
        EscWarnIndcnFlsgReq = 1
        Resd2 = 2
        EscWarnIndcnOffReq = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class EscWarnIndcnReqChks:
    de_name     = "EscWarnIndcnReq.EscWarnIndcnReqChks"
    fdx_name    = "EscWarnIndcnReqChks"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class EscWarnIndcnReqCntr:
    de_name     = "EscWarnIndcnReq.EscWarnIndcnReqCntr"
    fdx_name    = "EscWarnIndcnReqCntr"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ExtrMirrFoldSetgPen:
    de_name     = "ExtrMirrFoldSetg.Pen"
    fdx_name    = "ExtrMirrFoldSetgPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ExtrMirrFoldSetgSts:
    de_name     = "ExtrMirrFoldSetg.Sts"
    fdx_name    = "ExtrMirrFoldSetgSts"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ExtrMirrTiltSetg2IdPen:
    de_name     = "ExtrMirrTiltSetg2.IdPen"
    fdx_name    = "ExtrMirrTiltSetg2IdPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class ExtrMirrTiltSetg2MirrDrvr:
    de_name     = "ExtrMirrTiltSetg2.MirrDrvr"
    fdx_name    = "ExtrMirrTiltSetg2MirrDrvr"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        On = 0
        Off = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class ExtrMirrTiltSetg2MirrPass:
    de_name     = "ExtrMirrTiltSetg2.MirrPass"
    fdx_name    = "ExtrMirrTiltSetg2MirrPass"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        On = 0
        Off = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class FanForCmptmtTRunng:
    de_name     = "CmptmtTFrnt.FanForCmptmtTRunng"
    fdx_name    = "FanForCmptmtTRunng"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Rst = 0
        Set = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class FanForCmptmtTRunng:
    de_name     = "CmptmtTFrnt.FanForCmptmtTRunng"
    fdx_name    = "FanForCmptmtTRunng"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Rst = 0
        Set = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FltEgyCnsWdSts:
    de_name     = "VehModMngtGlbSafe1.FltEgyCnsWdSts"
    fdx_name    = "FltEgyCnsWdSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FltIndcrTurnLeFrnt:
    de_name     = "FltIndcrTurnLeFrnt"
    fdx_name    = "FltIndcrTurnLeFrnt"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FltIndcrTurnLeRe:
    de_name     = "FltIndcrTurnLeRe"
    fdx_name    = "FltIndcrTurnLeRe"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FltIndcrTurnRiFrnt:
    de_name     = "FltIndcrTurnRiFrnt"
    fdx_name    = "FltIndcrTurnRiFrnt"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FltIndcrTurnRiRe:
    de_name     = "FltIndcrTurnRiRe"
    fdx_name    = "FltIndcrTurnRiRe"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FltOfLiDaytiRunngRi:
    de_name     = "FltOfLiDaytiRunngRi"
    fdx_name    = "FltOfLiDaytiRunngRi"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->65535
class FRNetworkStatus:
    de_name     = "FRNetworkStatus"
    fdx_name    = "FRNetworkStatus"
    fdx_groupid = 1366

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class FrntAxleWarn:
    de_name     = "FrntAxleWarn"
    fdx_name    = "FrntAxleWarn"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        Resd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FrntWiprLvrReq2FrntWiprLvrCmd1:
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrCmd1"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrCmd1"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        FrntWiprOff = 0
        FrntWiprSngStk = 1
        FrntWiprIntm = 2
        FrntWiprContnsLoSpd = 3
        FrntWiprContnsHiSpd = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->3
class FrntWiprLvrReq2FrntWiprLvrCntr:
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrCntr"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrCntr"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 3
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class FrntWiprLvrReq2FrntWiprLvrCrc:
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrCrc"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrCrc"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class FrntWiprLvrReq2FrntWiprLvrQf:
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrQf"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrQf"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# LperHectoKiloM = L/100Km
# KiloMPerL = Km/L
# MilePerGallonEngland = MPG (UK)
# MilePerGallonUsa = MPG (US)
class FuCnsUnit:
    de_name     = "IndcnUnit.FuCnsUnit"
    fdx_name    = "FuCnsUnit"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        LperHectoKiloM = 0
        KiloMPerL = 1
        MilePerGallonEngland = 2
        MilePerGallonUsa = 3
        UkwnUnit = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FuHeatrActv:
    de_name     = "FuHeatrActv"
    fdx_name    = "FuHeatrActv"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class FuHeatrFuCns1:
    de_name     = "FuHeatrFuCns1"
    fdx_name    = "FuHeatrFuCns1"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Litre,  Range:0->1.27, Resolution: (0.01*x+0.0, raw is unsigned, 0 bits )
class FuHeatrFuCnsDurgCyc1:
    de_name     = "FuHeatrFuCnsDurgCyc1"
    fdx_name    = "FuHeatrFuCnsDurgCyc1"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 1.27
    scale  = 0.01
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class FuLvlIndcdQly:
    de_name     = "FuLvlIndcd.GenQF"
    fdx_name    = "FuLvlIndcdQly"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Litre,  Range:0->204.6, Resolution: (0.2*x+0.0, raw is unsigned, 10 bits )
class FuLvlIndcdVal:
    de_name     = "FuLvlIndcd.FuLvlValFromFuTbl"
    fdx_name    = "FuLvlIndcdVal"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 204.6
    scale  = 0.2
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class FuLvlLoIndcn:
    de_name     = "FuLvlLoIndcn"
    fdx_name    = "FuLvlLoIndcn"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class FuLvlLoWarn:
    de_name     = "FuLvlLoWarn"
    fdx_name    = "FuLvlLoWarn"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class GearIndcn:
    de_name     = "GearIndcnRec.GearIndcn"
    fdx_name    = "GearIndcn"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoIndcn = 0
        Gear1 = 1
        Gear2 = 2
        Gear3 = 3
        Gear4 = 4
        Gear5 = 5
        Gear6 = 6
        Gear7 = 7
        Gear8 = 8
        Gear9 = 9
        Gear10 = 10
        Gear11 = 11
        Gear2468 = 12
        Gear13579 = 13
        Neut = 14
        Rvs = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class GearShiftIndcn:
    de_name     = "GearIndcnRec.GearShiftIndcn"
    fdx_name    = "GearShiftIndcn"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoShiftIndcn = 0
        ShiftUpIndcn = 1
        CoolShiftIndcn = 2
        ShiftDwnIndcn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class GearTarIndcn:
    de_name     = "GearIndcnRec.GearTarIndcn"
    fdx_name    = "GearTarIndcn"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoIndcn = 0
        Gear1 = 1
        Gear2 = 2
        Gear3 = 3
        Gear4 = 4
        Gear5 = 5
        Gear6 = 6
        Gear7 = 7
        Gear8 = 8
        Gear9 = 9
        Gear10 = 10
        Gear11 = 11
        Gear2468 = 12
        Gear13579 = 13
        Neut = 14
        Rvs = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->102.3, Resolution: (0.1*x+0.0, raw is unsigned, 10 bits )
class GrdForFuEco:
    de_name     = "CchForFuEco.GrdForFuEco"
    fdx_name    = "GrdForFuEco"
    fdx_groupid = 1371

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 102.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class HdrestFoldReq2:
    de_name     = "HdrestFoldReq2"
    fdx_name    = "HdrestFoldReq2"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class HeatrDurgDrvgReqd:
    de_name     = "HeatrDurgDrvgReqd"
    fdx_name    = "HeatrDurgDrvgReqd"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        Aut = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HiQlyInTireCircumCalByNav:
    de_name     = "TireCircumCalByNav.HiQly"
    fdx_name    = "HiQlyInTireCircumCalByNav"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Interpretation:
# 0 - Flr - Floor
# 1 - Vent - Vent
# 2 - Defrst - Defrost
# 3 - FlrDefrst - Floor / defrost
# 4 - FlrVent - Floor / vent
# 5 - VentDefrst - Vent / defrost
# 6 - FlrVentDefrst - Floor / vent / defrost
# 7 - Aut - Auto
class HmiCmptmtAirDistbnFrnt:
    de_name     = "HmiCmptmtAirDistbnFrnt"
    fdx_name    = "HmiCmptmtAirDistbnFrnt"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Flr = 0
        Vent = 1
        Defrst = 2
        FlrDefrst = 3
        FlrVent = 4
        VentDefrst = 5
        FlrVentDefrst = 6
        Aut = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiCmptmtCoolgReq:
    de_name     = "HmiCmptmtCoolgReq"
    fdx_name    = "HmiCmptmtCoolgReq"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Auto = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowFirstLe:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe"
    fdx_name    = "HmiCmptmtTSpForRowFirstLe"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowFirstRi:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi"
    fdx_name    = "HmiCmptmtTSpForRowFirstRi"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowSecLe:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowSecLe"
    fdx_name    = "HmiCmptmtTSpForRowSecLe"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowSecRi:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowSecRi"
    fdx_name    = "HmiCmptmtTSpForRowSecRi"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class HmiCmptmtTSpSpclForRowFirstLe:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe"
    fdx_name    = "HmiCmptmtTSpSpclForRowFirstLe"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        Lo = 1
        Hi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiCmptmtTSpSpclForRowFirstRi:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi"
    fdx_name    = "HmiCmptmtTSpSpclForRowFirstRi"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        Lo = 1
        Hi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiCmptmtTSpSpclForRowSecLe:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowSecLe"
    fdx_name    = "HmiCmptmtTSpSpclForRowSecLe"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        Lo = 1
        Hi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiCmptmtTSpSpclForRowSecRi:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowSecRi"
    fdx_name    = "HmiCmptmtTSpSpclForRowSecRi"
    fdx_groupid = 1274

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        Lo = 1
        Hi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiDefrstElecForMirrReq:
    de_name     = "HmiDefrstElecReq.MirrElecReq"
    fdx_name    = "HmiDefrstElecForMirrReq"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        AutOn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiDefrstElecFrntReq:
    de_name     = "HmiDefrstElecReq.FrntElecReq"
    fdx_name    = "HmiDefrstElecFrntReq"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        AutOn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiDefrstElecReReq:
    de_name     = "HmiDefrstElecReq.ReElecReq"
    fdx_name    = "HmiDefrstElecReReq"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        AutOn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiDefrstFrntSts:
    de_name     = "HmiDefrstElecSts.Frnt"
    fdx_name    = "HmiDefrstFrntSts"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        Limited = 2
        NotAvailable = 3
        TmrOff = 4
        AutoCdn = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiDefrstMaxReq:
    de_name     = "HmiDefrstMaxReq"
    fdx_name    = "HmiDefrstMaxReq"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        AutOn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiDfrstReSts:
    de_name     = "HmiDefrstElecSts.Re"
    fdx_name    = "HmiDfrstReSts"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        Limited = 2
        NotAvailable = 3
        TmrOff = 4
        AutoCdn = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiHvacFanLvlFrnt:
    de_name     = "HmiHvacFanLvlFrnt"
    fdx_name    = "HmiHvacFanLvlFrnt"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Min = 1
        LvlMan1 = 2
        LvlMan2 = 3
        LvlMan3 = 4
        LvlMan4 = 5
        LvlMan5 = 6
        LvlMan6 = 7
        LvlMan7 = 8
        LvlAutMinusMinus = 9
        LvlAutMinus = 10
        LvlAutoNorm = 11
        LvlAutPlus = 12
        LvlAutPlusPlus = 13
        Max = 14

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiHvacFanLvlRe:
    de_name     = "HmiHvacFanLvlRe"
    fdx_name    = "HmiHvacFanLvlRe"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Min = 1
        LvlMan1 = 2
        LvlMan2 = 3
        LvlMan3 = 4
        LvlMan4 = 5
        LvlMan5 = 6
        LvlMan6 = 7
        LvlMan7 = 8
        LvlAutMinusMinus = 9
        LvlAutMinus = 10
        LvlAutoNorm = 11
        LvlAutPlus = 12
        LvlAutPlusPlus = 13
        Max = 14

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Interpretation:
# 0 - OsaFull - Full OSA
# 1 - RecircFull - Full recirculation
# 2 - RecircFullWithTiOut - Full REC with timeout
# 3 - AutWithAirQly - AUTO with AQS
# 4 - Aut - Auto
# 5 - Resd - Reserved
class HmiHvacRecircCmd:
    de_name     = "HmiHvacRecircCmd"
    fdx_name    = "HmiHvacRecircCmd"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Aut = 0
        AutWithAirQly = 1
        RecircFull = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiHvacReCtrl:
    de_name     = "HmiHvacReCtrl"
    fdx_name    = "HmiHvacReCtrl"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        OffWithNoOccpt = 1
        On = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiMirrDefrstSts:
    de_name     = "HmiDefrstElecSts.Mirrr"
    fdx_name    = "HmiMirrDefrstSts"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        Limited = 2
        NotAvailable = 3
        TmrOff = 4
        AutoCdn = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiSeatClimaExtdHmiSeatVentnForRowSecLe:
    de_name     = "HmiSeatClimaExtd.HmiSeatVentnForRowSecLe"
    fdx_name    = "HmiSeatClimaExtdHmiSeatVentnForRowSecLe"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiSeatClimaExtdHmiSeatVentnForRowSecRi:
    de_name     = "HmiSeatClimaExtd.HmiSeatVentnForRowSecRi"
    fdx_name    = "HmiSeatClimaExtdHmiSeatVentnForRowSecRi"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiSeatHeatgForRowFirstLe:
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowFirstLe"
    fdx_name    = "HmiSeatHeatgForRowFirstLe"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiSeatHeatgForRowFirstRi:
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowFirstRi"
    fdx_name    = "HmiSeatHeatgForRowFirstRi"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiSeatHeatgForRowSecLe:
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowSecLe"
    fdx_name    = "HmiSeatHeatgForRowSecLe"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiSeatHeatgForRowSecRi:
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowSecRi"
    fdx_name    = "HmiSeatHeatgForRowSecRi"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiSeatVentnForRowFirstLe:
    de_name     = "HmiSeatClima.HmiSeatVentnForRowFirstLe"
    fdx_name    = "HmiSeatVentnForRowFirstLe"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HmiSeatVentnForRowFirstRi:
    de_name     = "HmiSeatClima.HmiSeatVentnForRowFirstRi"
    fdx_name    = "HmiSeatVentnForRowFirstRi"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Status of the door.
class HoodSts:
    de_name     = "HoodSts"
    fdx_name    = "HoodSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: hours,  Range:0->23
class Hr:
    de_name     = "TiAndDateIndcn.Hr1"
    fdx_name    = "Hr"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 23
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: hours,  Range:0->23
class Hr1:
    de_name     = "SetTiAndDate.Hour"
    fdx_name    = "Hr1"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 23
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class HudActvReqPen:
    de_name     = "HudActvReq.Pen"
    fdx_name    = "HudActvReqPen"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudActvReqSts:
    de_name     = "HudActvReq.Sts"
    fdx_name    = "HudActvReqSts"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudActvSts:
    de_name     = "HudActvSts"
    fdx_name    = "HudActvSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudAdjmtReq:
    de_name     = "HudAdjmtReq"
    fdx_name    = "HudAdjmtReq"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudDiagcHudCircShoSts:
    de_name     = "HudDiagc.HudCircShoSts"
    fdx_name    = "HudDiagcHudCircShoSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoErr = 0
        Err = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudDiagcHudCricOpenSts:
    de_name     = "HudDiagc.HudCricOpenSts"
    fdx_name    = "HudDiagcHudCricOpenSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoErr = 0
        Err = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudDiagcHudTSts:
    de_name     = "HudDiagc.HudTSts"
    fdx_name    = "HudDiagcHudTSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoErr = 0
        Err = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudDiagcImgHudErrSts:
    de_name     = "HudDiagc.ImgHudErrSts"
    fdx_name    = "HudDiagcImgHudErrSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoErr = 0
        Err = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudDiagcImgHudTmpNotAvlSts:
    de_name     = "HudDiagc.ImgHudTmpNotAvlSts"
    fdx_name    = "HudDiagcImgHudTmpNotAvlSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Avl = 0
        NotAvl = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudErgoSetgReq:
    de_name     = "HudErgoSetgReq"
    fdx_name    = "HudErgoSetgReq"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudSts:
    de_name     = "HudSts"
    fdx_name    = "HudSts"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        HudStsAvl = 0
        HudStsCalMod = 1
        HudStsTmpNotAvl = 2
        HudStsErr = 3
        Resd1 = 4
        Resd2 = 5
        Resd3 = 6

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct00:
    de_name     = "HudVisFctSetg.HudFct00"
    fdx_name    = "HudVisFctSetgHudFct00"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct01:
    de_name     = "HudVisFctSetg.HudFct01"
    fdx_name    = "HudVisFctSetgHudFct01"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct02:
    de_name     = "HudVisFctSetg.HudFct02"
    fdx_name    = "HudVisFctSetgHudFct02"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct03:
    de_name     = "HudVisFctSetg.HudFct03"
    fdx_name    = "HudVisFctSetgHudFct03"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct04:
    de_name     = "HudVisFctSetg.HudFct04"
    fdx_name    = "HudVisFctSetgHudFct04"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct05:
    de_name     = "HudVisFctSetg.HudFct05"
    fdx_name    = "HudVisFctSetgHudFct05"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct06:
    de_name     = "HudVisFctSetg.HudFct06"
    fdx_name    = "HudVisFctSetgHudFct06"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct07:
    de_name     = "HudVisFctSetg.HudFct07"
    fdx_name    = "HudVisFctSetgHudFct07"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct08:
    de_name     = "HudVisFctSetg.HudFct08"
    fdx_name    = "HudVisFctSetgHudFct08"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct09:
    de_name     = "HudVisFctSetg.HudFct09"
    fdx_name    = "HudVisFctSetgHudFct09"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct10:
    de_name     = "HudVisFctSetg.HudFct10"
    fdx_name    = "HudVisFctSetgHudFct10"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct11:
    de_name     = "HudVisFctSetg.HudFct11"
    fdx_name    = "HudVisFctSetgHudFct11"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct12:
    de_name     = "HudVisFctSetg.HudFct12"
    fdx_name    = "HudVisFctSetgHudFct12"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct13:
    de_name     = "HudVisFctSetg.HudFct13"
    fdx_name    = "HudVisFctSetgHudFct13"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct14:
    de_name     = "HudVisFctSetg.HudFct14"
    fdx_name    = "HudVisFctSetgHudFct14"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct15:
    de_name     = "HudVisFctSetg.HudFct15"
    fdx_name    = "HudVisFctSetgHudFct15"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct16:
    de_name     = "HudVisFctSetg.HudFct16"
    fdx_name    = "HudVisFctSetgHudFct16"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct17:
    de_name     = "HudVisFctSetg.HudFct17"
    fdx_name    = "HudVisFctSetgHudFct17"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct18:
    de_name     = "HudVisFctSetg.HudFct18"
    fdx_name    = "HudVisFctSetgHudFct18"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class HudVisFctSetgHudFct19:
    de_name     = "HudVisFctSetg.HudFct19"
    fdx_name    = "HudVisFctSetgHudFct19"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class HudVisFctSetgPen:
    de_name     = "HudVisFctSetg.Pen"
    fdx_name    = "HudVisFctSetgPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Kg / h,  Range:0->1000, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class HvacAirMFlowEstimd:
    de_name     = "HvacAirMFlowEstimd"
    fdx_name    = "HvacAirMFlowEstimd"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 1000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class HznRstExtd:
    de_name     = "HznRstExtd"
    fdx_name    = "HznRstExtd"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class HzrdLiWarnSts:
    de_name     = "HzrdLiWarnSts"
    fdx_name    = "HzrdLiWarnSts"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IdPenForUnits:
    de_name     = "IndcnUnit.IdPenForUnit"
    fdx_name    = "IdPenForUnits"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class IndcnOfParkAssiSts:
    de_name     = "IndcnOfParkAssiSts"
    fdx_name    = "IndcnOfParkAssiSts"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class IndcnOfPrkgAutSts:
    de_name     = "IndcnOfPrkgAutSts"
    fdx_name    = "IndcnOfPrkgAutSts"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        PrkgTypAutSeln = 1
        PrkgTypPara = 2
        PrkgTypPerp = 3
        PrkgOutManv = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# DateFmt1 = YMD
# DateFmt2 = DMY
# DateFmt3 = MDY
class IndcnUnitDateFmt:
    de_name     = "IndcnUnit.DateFmt"
    fdx_name    = "IndcnUnitDateFmt"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DateFmt1 = 0
        DateFmt2 = 1
        DateFmt3 = 2
        UkwnFmt = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class IndcrDisp1WdSts:
    de_name     = "IndcrDisp1WdSts"
    fdx_name    = "IndcrDisp1WdSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        LeOn = 1
        RiOn = 2
        LeAndRiOn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class IndcrTurnSts1WdSts:
    de_name     = "IndcrTurnSts1WdSts"
    fdx_name    = "IndcrTurnSts1WdSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        LeOn = 1
        RiOn = 2
        LeAndRiOn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->65535
class IniValSigCfgIDBackboneFR:
    de_name     = "IniValSigCfgIDBackboneFR"
    fdx_name    = "IniValSigCfgIDBackboneFR"
    fdx_groupid = 1244

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class IntrBriSts:
    de_name     = "IntrBriSts"
    fdx_name    = "IntrBriSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class IntrLiAmbLiSetgForLiInten:
    de_name     = "IntrLiAmbLiSetg.LiInten"
    fdx_name    = "IntrLiAmbLiSetgForLiInten"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        IntenLo = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Amibiance LIgt color setting:
# 0 = Temperature
# 1 = Off
# 2 - 8 = Colour 1 - 7
class IntrLiAmbLiSetgForLiTintg:
    de_name     = "IntrLiAmbLiSetg.LiTintg"
    fdx_name    = "IntrLiAmbLiSetgForLiTintg"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        T = 0
        Off = 1
        Tintg1 = 2
        Tintg2 = 3
        Tintg3 = 4
        Tintg4 = 5
        Tintg5 = 6
        Tintg6 = 7
        Tintg7 = 8

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrLiAmbLiSetgPen:
    de_name     = "IntrLiAmbLiSetg.Pen"
    fdx_name    = "IntrLiAmbLiSetgPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class IntrLiSurrndgsLiSetgForLiInten:
    de_name     = "IntrLiSurrndgsLiSetgLi.LiInten"
    fdx_name    = "IntrLiSurrndgsLiSetgForLiInten"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        IntenLo = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# 0 = Full
# 1 = Reduced
class IntrLiSurrndgsLiSetgForLiLvl:
    de_name     = "IntrLiSurrndgsLiSetgLi.LiLvl"
    fdx_name    = "IntrLiSurrndgsLiSetgForLiLvl"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        LvlFull = 0
        LvlReduced = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrLiSurrndgsLiSetgPen:
    de_name     = "IntrLiSurrndgsLiSetgLi.Pen"
    fdx_name    = "IntrLiSurrndgsLiSetgPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrMirrTintgSetgPen:
    de_name     = "IntrMirrTintgSetg.Pen"
    fdx_name    = "IntrMirrTintgSetgPen"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class IntrMirrTintgSetgSts:
    de_name     = "IntrMirrTintgSetg.MirrDimLvl"
    fdx_name    = "IntrMirrTintgSetgSts"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Normal = 0
        Dark = 1
        Light = 2
        Inhibit = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Warning status	                                
# == 0	Calibration status OK (Normal IHU behavior) 
# == 1	Calibration not possible (All 4 rolling wheels shall be grey, menu item for recalibration of iTPMS system shall be unavailable and gray).
class iTPMSCalPsbl:
    de_name     = "iTPMSCalPsbl"
    fdx_name    = "iTPMSCalPsbl"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        CalPsbl = 0
        CalNoPsbl = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Calibration status
class iTPMSCalSts:
    de_name     = "iTPMSCalSts"
    fdx_name    = "iTPMSCalSts"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoCalReq = 0
        CalOn = 1
        CalCmpl = 2
        CalFaild = 3
        Resd1 = 4
        Resd2 = 5
        Resd3 = 6

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# iTPMS system status and warnings
class iTPMSTirePMSts:
    de_name     = "iTPMSTirePMSts"
    fdx_name    = "iTPMSTirePMSts"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        CmnWarn = 1
        WarnFL = 2
        WarnFR = 3
        WarnRL = 4
        WarnRR = 5
        SysUnAvi = 6
        SysFailr = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class KeyLostWarnIndcn:
    de_name     = "KeyLostWarnIndcn"
    fdx_name    = "KeyLostWarnIndcn"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        KeyLostNoMsg = 0
        KeyMsgAndSoundLost = 1
        KeyMsgLost = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class KeyProfMpgUpdForIdPen:
    de_name     = "KeyProfMpgUpd.KeyProfMpgUpdForIdPen"
    fdx_name    = "KeyProfMpgUpdForIdPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class KeyProfMpgUpdKeyProfMpgUpdOff:
    de_name     = "KeyProfMpgUpd.KeyProfMpgUpdOff"
    fdx_name    = "KeyProfMpgUpdKeyProfMpgUpdOff"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Data-Type for Key Search Location
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
class KeyReadReqFromSetgMgr:
    de_name     = "KeyReadReqFromSetgMgr"
    fdx_name    = "KeyReadReqFromSetgMgr"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        KeyLocnIdle = 0
        KeyLocnAll = 1
        KeyLocnAllExt = 2
        KeyLocnDrvrExt = 3
        KeyLocnPassExt = 4
        KeyLocnTrExt = 5
        KeyLocnAllInt = 6
        KeyLocnDrvrInt = 7
        KeyLocnPassInt = 8
        KeyLocnResvInt = 9
        KeyLocnResvIntSimple = 10

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class KeyReadStsToProfCtrlBoolean:
    de_name     = "KeyReadStsToProfCtrl.Boolean"
    fdx_name    = "KeyReadStsToProfCtrlBoolean"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class KeyReadStsToProfCtrlKeyId:
    de_name     = "KeyReadStsToProfCtrl.KeyId"
    fdx_name    = "KeyReadStsToProfCtrlKeyId"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Key0 = 0
        Key1 = 1
        Key2 = 2
        Key3 = 3
        Key4 = 4
        Key5 = 5
        Key6 = 6
        Key7 = 7
        Key8 = 8
        Key9 = 9
        Key10 = 10
        Key11 = 11

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class KeyRmnIndcn:
    de_name     = "KeyRmnIndcn"
    fdx_name    = "KeyRmnIndcn"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class KeySpdWarn:
    de_name     = "KeySpdWarn"
    fdx_name    = "KeySpdWarn"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Used be Connected service booking to inform Driver information about the current status of a service booking.
class LampSuppSrv:
    de_name     = "LampSuppSrv"
    fdx_name    = "LampSuppSrv"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoSrv = 0
        Srv = 1
        CfmdSrv = 2
        SrvPassd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LaneChgWarnActvPen:
    de_name     = "LaneChgWarnActv.Pen"
    fdx_name    = "LaneChgWarnActvPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LaneChgWarnActvSts:
    de_name     = "LaneChgWarnActv.Sts"
    fdx_name    = "LaneChgWarnActvSts"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class LaneChgWarnSts:
    de_name     = "LaneChgWarnSts"
    fdx_name    = "LaneChgWarnSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoLaneDetd	No Lane Detedcted		No lane tracked
# 1			1			LaneLeDetd	Lane Left Detected		Left lane marking tracked
# 2			2			LaneRiDetd	Lane Right Detected	Right lane marking tracked
# 3			3			LaneLeAndRiDetd	Lane Left And Right Detected	Left and right lane marking tracked
class LaneDetnStsForAutDrv:
    de_name     = "LaneDetnStsForAutDrv"
    fdx_name    = "LaneDetnStsForAutDrv"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoLaneDetd = 0
        LaneLeDetd = 1
        LaneRiDetd = 2
        LaneLeAndRiDetd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoLaneDetd	No Lane Detedcted		No lane tracked
# 1			1			LaneLeDetd	Lane Left Detected		Left lane marking tracked
# 2			2			LaneRiDetd	Lane Right Detected	Right lane marking tracked
# 3			3			LaneLeAndRiDetd	Lane Left And Right Detected	Left and right lane marking tracked
class LaneDetnStsForLaneKeepAid:
    de_name     = "LaneDetnStsForLaneKeepAid"
    fdx_name    = "LaneDetnStsForLaneKeepAid"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoLaneDetd = 0
        LaneLeDetd = 1
        LaneRiDetd = 2
        LaneLeAndRiDetd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LaneKeepAidActvPen:
    de_name     = "LaneKeepAidActv.Pen"
    fdx_name    = "LaneKeepAidActvPen"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LaneKeepAidActvSts:
    de_name     = "LaneKeepAidActv.Sts"
    fdx_name    = "LaneKeepAidActvSts"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class LaneKeepAidSts:
    de_name     = "LaneKeepAidSts"
    fdx_name    = "LaneKeepAidSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LatMovmtWarn:
    de_name     = "LatMovmtWarn"
    fdx_name    = "LatMovmtWarn"
    fdx_groupid = 1139

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        WarnLe = 1
        WarnRi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LcmaOn1:
    de_name     = "LcmaOn1"
    fdx_name    = "LcmaOn1"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        StrtUpOn = 0
        On = 1
        Off = 2
        TrlrOff = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiAdpvReqPen:
    de_name     = "LiExtReq1WdReq1.Pen"
    fdx_name    = "LiAdpvReqPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiAdpvReqSts:
    de_name     = "LiExtReq1WdReq1.Sts"
    fdx_name    = "LiAdpvReqSts"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiAutTranReqPen:
    de_name     = "LiExtReq2WdReq1.Pen"
    fdx_name    = "LiAutTranReqPen"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# SntvyLo = Sensitivity Low, light transition happens later
# SntvyLo = Sensitivity Norma 
# SntvyLo = Sensitivity High, light transition happens earlier
class LiAutTranReqSts:
    de_name     = "LiExtReq2WdReq1.Sts"
    fdx_name    = "LiAutTranReqSts"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        SnvtyLo = 0
        SnvtyNorm = 1
        SnvtyHi = 2
        Resd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiBeamHiAuxReqPen:
    de_name     = "LiExtReq1WdReq4.Pen"
    fdx_name    = "LiBeamHiAuxReqPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiBeamHiAuxReqSts:
    de_name     = "LiExtReq1WdReq4.Sts"
    fdx_name    = "LiBeamHiAuxReqSts"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiCornrgReqPen:
    de_name     = "LiExtReq1WdReq6.Pen"
    fdx_name    = "LiCornrgReqPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiCornrgReqSts:
    de_name     = "LiExtReq1WdReq6.Sts"
    fdx_name    = "LiCornrgReqSts"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiDaytiRunngReqPen:
    de_name     = "LiExtReq1WdReq2.Pen"
    fdx_name    = "LiDaytiRunngReqPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiDaytiRunngReqSts:
    de_name     = "LiExtReq1WdReq2.Sts"
    fdx_name    = "LiDaytiRunngReqSts"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiDrvrFltIndcrTurn:
    de_name     = "LiDrvrFltIndcrTurn"
    fdx_name    = "LiDrvrFltIndcrTurn"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiExtReq1WdReq5IdPen:
    de_name     = "LiExtReq1WdReq5.IdPen"
    fdx_name    = "LiExtReq1WdReq5IdPen"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiExtReq1WdReq5SlowNormFast:
    de_name     = "LiExtReq1WdReq5.SlowNormFast"
    fdx_name    = "LiExtReq1WdReq5SlowNormFast"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Slow = 1
        Norm = 2
        Fast = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiHomeLvngReqPen:
    de_name     = "LiExtSafe1WdReq2.Pen"
    fdx_name    = "LiHomeLvngReqPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiHomeLvngReqSts:
    de_name     = "LiExtSafe1WdReq2.Sts"
    fdx_name    = "LiHomeLvngReqSts"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiHomeSafeReqPen:
    de_name     = "LiExtSafe1WdReq1.Pen"
    fdx_name    = "LiHomeSafeReqPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Enumeration for intervals in tenths of seconds.
# E.g 
# 3 = 30 seconds
# 4 = 40 seconds
class LiHomeSafeReqSts:
    de_name     = "LiExtSafe1WdReq1.Sts"
    fdx_name    = "LiHomeSafeReqSts"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Sec0 = 0
        Sec10 = 1
        Sec20 = 2
        Sec30 = 3
        Sec40 = 4
        Sec50 = 5
        Sec60 = 6
        Sec70 = 7
        Sec80 = 8
        Sec90 = 9
        Sec100 = 10
        Sec110 = 11
        Sec120 = 12
        Resd1 = 13
        Resd2 = 14
        Resd3 = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiLvrSwt1:
    de_name     = "LiLvrSwt1Req"
    fdx_name    = "LiLvrSwt1"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiPassFltIndcrTurn:
    de_name     = "LiPassFltIndcrTurn"
    fdx_name    = "LiPassFltIndcrTurn"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFlt = 0
        Flt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiSeldForDrvrPfmncMonPen:
    de_name     = "LiSeldForDrvrPfmncMon.Pen"
    fdx_name    = "LiSeldForDrvrPfmncMonPen"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LiSeldForDrvrPfmncMonSts:
    de_name     = "LiSeldForDrvrPfmncMon.Sts"
    fdx_name    = "LiSeldForDrvrPfmncMonSts"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# TrfcSideTmpLe = Traffic Side Temporary Left. Vehicle drives on left side of road
# TrfcSideTmpRi =  Traffic Side Temporary Right. Vehicle drives on right side of road
class LiTrfcSideReq:
    de_name     = "LiTrfcSide1WdReq1"
    fdx_name    = "LiTrfcSideReq"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        TrfcSideTmpLe = 1
        TrfcSideTmpRi = 2
        Resd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LockgCenStsForUsrFb:
    de_name     = "LockgCenStsForUsrFb"
    fdx_name    = "LockgCenStsForUsrFb"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Undefd = 0
        Opend = 1
        Clsd = 2
        Lockd = 3
        Safe = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LockgCenStsLockSt:
    de_name     = "LockgCenSts.LockSt"
    fdx_name    = "LockgCenStsLockSt"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        LockUndefd = 0
        LockUnlckd = 1
        LockTrUnlckd = 2
        LockLockd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LockgCenStsUpdEve:
    de_name     = "LockgCenSts.UpdEve"
    fdx_name    = "LockgCenStsUpdEve"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class LockgCenTrigSrc:
    de_name     = "LockgCenSts.TrigSrc"
    fdx_name    = "LockgCenTrigSrc"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrigSrc = 0
        KeyRem = 1
        Keyls = 2
        IntrSwt = 3
        SpdAut = 4
        TmrAut = 5
        Slam = 6
        Telm = 7
        Crash = 8

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockgFbSoundReqPen:
    de_name     = "LockgFbSoundReq.Pen"
    fdx_name    = "LockgFbSoundReqPen"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LockgFbSoundReqSts:
    de_name     = "LockgFbSoundReq.Sts"
    fdx_name    = "LockgFbSoundReqSts"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockgFbVisReqPen:
    de_name     = "LockgFbVisReq.Pen"
    fdx_name    = "LockgFbVisReqPen"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LockgFbVisReqSts:
    de_name     = "LockgFbVisReq.Sts"
    fdx_name    = "LockgFbVisReqSts"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LockgPrsnlReqFromHmi:
    de_name     = "LockgPrsnlReqFromHmi"
    fdx_name    = "LockgPrsnlReqFromHmi"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        On = 1
        Off = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LockgPrsnlSts:
    de_name     = "LockgPrsnlSts"
    fdx_name    = "LockgPrsnlSts"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockSpdReqPen:
    de_name     = "LockSpdReq.Pen"
    fdx_name    = "LockSpdReqPen"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LockSpdReqSts:
    de_name     = "LockSpdReq.Sts"
    fdx_name    = "LockSpdReqSts"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class LvlOfClimaCmft:
    de_name     = "LvlOfClimaCmft"
    fdx_name    = "LvlOfClimaCmft"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
        Lvl4 = 4
        Lvl5 = 5
        Lvl6 = 6
        Lvl7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class MassgFctActvDrvrMassgFctActv:
    de_name     = "MassgFctActv.DrvrMassgFctActv"
    fdx_name    = "MassgFctActvDrvrMassgFctActv"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class MassgFctActvPassMassgFctActv:
    de_name     = "MassgFctActv.PassMassgFctActv"
    fdx_name    = "MassgFctActvPassMassgFctActv"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class MemBtnSound:
    de_name     = "MemBtnSound"
    fdx_name    = "MemBtnSound"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Mins,  Range:0->59
class Mins:
    de_name     = "TiAndDateIndcn.Mins1"
    fdx_name    = "Mins"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Mins,  Range:0->59
class Mins1:
    de_name     = "SetTiAndDate.Minute"
    fdx_name    = "Mins1"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class MirrDwnStsAtDrvr:
    de_name     = "MirrDwnStsAtDrvr"
    fdx_name    = "MirrDwnStsAtDrvr"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        MirrTiltUndefd = 0
        MirrUpPosn = 1
        MirrTiltPosn = 2
        MirrMovgToUpPosn = 3
        MirrMovgToTiltPosn = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class MirrDwnStsAtPass:
    de_name     = "MirrDwnStsAtPass"
    fdx_name    = "MirrDwnStsAtPass"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        MirrTiltUndefd = 0
        MirrUpPosn = 1
        MirrTiltPosn = 2
        MirrMovgToUpPosn = 3
        MirrMovgToTiltPosn = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class MirrFoldStsAtDrvr:
    de_name     = "MirrFoldStsAtDrvr"
    fdx_name    = "MirrFoldStsAtDrvr"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        MirrFoldPosnUndefd = 0
        MirrNotFoldPosn = 1
        MirrFoldPosn = 2
        MirrMovgToNotFold = 3
        MirrMovgToFold = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class MirrFoldStsAtPass:
    de_name     = "MirrFoldStsAtPass"
    fdx_name    = "MirrFoldStsAtPass"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        MirrFoldPosnUndefd = 0
        MirrNotFoldPosn = 1
        MirrFoldPosn = 2
        MirrMovgToNotFold = 3
        MirrMovgToFold = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class MmedHmiModStd:
    de_name     = "MmedHmiModStd"
    fdx_name    = "MmedHmiModStd"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        InfModeOff = 0
        InfModeOn = 1
        InfModePartial = 2
        InfModeWelcome = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class MmedMaiPwrMod:
    de_name     = "MmedHdPwrMod"
    fdx_name    = "MmedMaiPwrMod"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IHUStateSleep = 0
        IHUStateStandby = 1
        IHUStatePartial = 2
        IHUStateOn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->65535
class MstCfgIDBackboneFR:
    de_name     = "MstCfgIDBackboneFR"
    fdx_name    = "MstCfgIDBackboneFR"
    fdx_groupid = 1366

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Month,  Range:1->12
class Mth:
    de_name     = "TiAndDateIndcn.Mth1"
    fdx_name    = "Mth"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 1
    max = 12
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Month,  Range:1->12
class Mth1:
    de_name     = "SetTiAndDate.Month"
    fdx_name    = "Mth1"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 1
    max = 12
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class NetCtrlrActvtPrio:
    de_name     = "NetCtrlrActvt.Prio"
    fdx_name    = "NetCtrlrActvtPrio"
    fdx_groupid = 1366

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PrioNormal = 0
        PrioHigh = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class NetCtrlrActvtResourceGroup:
    de_name     = "NetCtrlrActvt.ResourceGroup"
    fdx_name    = "NetCtrlrActvtResourceGroup"
    fdx_groupid = 1366

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class NetHdActvtPrio:
    de_name     = "NetHdActvt.Prio"
    fdx_name    = "NetHdActvtPrio"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PrioNormal = 0
        PrioHigh = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class NetHdActvtResourceGroup:
    de_name     = "NetHdActvt.ResourceGroup"
    fdx_name    = "NetHdActvtResourceGroup"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class NetTelmActvtPrio:
    de_name     = "NetTelmActvt.Prio"
    fdx_name    = "NetTelmActvtPrio"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PrioNormal = 0
        PrioHigh = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class NetTelmActvtResourceGroup:
    de_name     = "NetTelmActvt.ResourceGroup"
    fdx_name    = "NetTelmActvtResourceGroup"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class NewTripCdn:
    de_name     = "NewTripCdn"
    fdx_name    = "NewTripCdn"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class NoEntryWarnReq:
    de_name     = "NoEntryWarnReq"
    fdx_name    = "NoEntryWarnReq"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        Warn = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo1InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo1"
    fdx_name    = "NoOfSatltForSysNo1InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo2InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo2"
    fdx_name    = "NoOfSatltForSysNo2InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo3InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo3"
    fdx_name    = "NoOfSatltForSysNo3InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo4InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo4"
    fdx_name    = "NoOfSatltForSysNo4InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo5InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo5"
    fdx_name    = "NoOfSatltForSysNo5InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo6InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo6"
    fdx_name    = "NoOfSatltForSysNo6InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class NoSoundSys:
    de_name     = "NoSoundSys"
    fdx_name    = "NoSoundSys"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->65535
class NotifChkDistbn:
    de_name     = "NotifChkDistbn"
    fdx_name    = "NotifChkDistbn"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->7
class NrOfBltAppld:
    de_name     = "NrOfBltAppld"
    fdx_name    = "NrOfBltAppld"
    fdx_groupid = 1350

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 7
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class NrOfKeyAvl:
    de_name     = "NrOfKeyAvl"
    fdx_name    = "NrOfKeyAvl"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class NrSerlDMSMNr1:
    de_name     = "NrSerlDMSM.Nr1"
    fdx_name    = "NrSerlDMSMNr1"
    fdx_groupid = 1228

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class NrSerlDMSMNr2:
    de_name     = "NrSerlDMSM.Nr2"
    fdx_name    = "NrSerlDMSMNr2"
    fdx_groupid = 1228

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class NrSerlDMSMNr3:
    de_name     = "NrSerlDMSM.Nr3"
    fdx_name    = "NrSerlDMSMNr3"
    fdx_groupid = 1228

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class NrSerlDMSMNr4:
    de_name     = "NrSerlDMSM.Nr4"
    fdx_name    = "NrSerlDMSMNr4"
    fdx_groupid = 1228

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class OffsForDrvrSpprtFctActvSts:
    de_name     = "OffsForDrvrSpprtFctActvSts"
    fdx_name    = "OffsForDrvrSpprtFctActvSts"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstFrntOfAudSideWarn:
    de_name     = "SnsrParkAssiFrnt.AudSideWarn"
    fdx_name    = "ParkAssiDstFrntOfAudSideWarn"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        None_ = 0
        Left = 1
        Right = 2
        LeftAndRight = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstFrntOfAudWarnDir:
    de_name     = "SnsrParkAssiFrnt.AudWarnDir"
    fdx_name    = "ParkAssiDstFrntOfAudWarnDir"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        LeftSide = 1
        OuterLeft = 2
        InnerLeft = 3
        InnerRight = 4
        OuterRight = 5
        RightSide = 6

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstFrntOfDstOfSnsrInsdRi:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrInsdRi"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrInsdRi"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstFrntOfDstOfSnsrLeSide:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrLeSide"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrLeSide"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstFrntOfDstOfSnsrOutdRi:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrOutdRi"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrOutdRi"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstFrntOfDstOfSnsrRiSide:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrRiSide"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrRiSide"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstFrntOfSnsrInsdLe:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrInsdLe"
    fdx_name    = "ParkAssiDstFrntOfSnsrInsdLe"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstFrntOfSnsrOutdLe:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrOutdLe"
    fdx_name    = "ParkAssiDstFrntOfSnsrOutdLe"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstReOfAudSideWarn:
    de_name     = "SnsrParkAssiRe.AudSideWarn"
    fdx_name    = "ParkAssiDstReOfAudSideWarn"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        None_ = 0
        Left = 1
        Right = 2
        LeftAndRight = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstReOfAudWarnDir:
    de_name     = "SnsrParkAssiRe.AudWarnDir"
    fdx_name    = "ParkAssiDstReOfAudWarnDir"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        LeftSide = 1
        OuterLeft = 2
        InnerLeft = 3
        InnerRight = 4
        OuterRight = 5
        RightSide = 6

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstReOfDstOfSnsrInsdLe:
    de_name     = "SnsrParkAssiRe.DstOfSnsrInsdLe"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrInsdLe"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstReOfDstOfSnsrInsdRi:
    de_name     = "SnsrParkAssiRe.DstOfSnsrInsdRi"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrInsdRi"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstReOfDstOfSnsrLeSide:
    de_name     = "SnsrParkAssiRe.DstOfSnsrLeSide"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrLeSide"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstReOfDstOfSnsrOutdLe:
    de_name     = "SnsrParkAssiRe.DstOfSnsrOutdLe"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrOutdLe"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstReOfDstOfSnsrOutdRi:
    de_name     = "SnsrParkAssiRe.DstOfSnsrOutdRi"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrOutdRi"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ParkAssiDstReOfDstOfSnsrRiSide:
    de_name     = "SnsrParkAssiRe.DstOfSnsrRiSide"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrRiSide"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DistanceInfinityOrNotUsed = 0
        Distance150cm = 1
        Distance146cm = 2
        Distance142cm = 3
        Distance138cm = 4
        Distance134cm = 5
        Distance130cm = 6
        Distance126cm = 7
        Distance122cm = 8
        Distance118cm = 9
        Distance114cm = 10
        Distance110cm = 11
        Distance106cm = 12
        Distance102m = 13
        Distance98cm = 14
        Distance94cm = 15
        Distance90cm = 16
        Distance86cm = 17
        Distance82cm = 18
        Distance78cm = 19
        Distance74cm = 20
        Distance70cm = 21
        Distance66cm = 22
        Distance62cm = 23
        Distance58cm = 24
        Distance54cm = 25
        Distance50cm = 26
        Distance46cm = 27
        Distance42cm = 28
        Distance38cm = 29
        Distance34cm = 30
        Distance30cm = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class PartNrDMSMEndSgn1:
    de_name     = "PartNrDMSM.EndSgn1"
    fdx_name    = "PartNrDMSMEndSgn1"
    fdx_groupid = 1227

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class PartNrDMSMEndSgn2:
    de_name     = "PartNrDMSM.EndSgn2"
    fdx_name    = "PartNrDMSMEndSgn2"
    fdx_groupid = 1227

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class PartNrDMSMEndSgn3:
    de_name     = "PartNrDMSM.EndSgn3"
    fdx_name    = "PartNrDMSMEndSgn3"
    fdx_groupid = 1227

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class PartNrDMSMNr1:
    de_name     = "PartNrDMSM.Nr1"
    fdx_name    = "PartNrDMSMNr1"
    fdx_groupid = 1227

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class PartNrDMSMNr2:
    de_name     = "PartNrDMSM.Nr2"
    fdx_name    = "PartNrDMSMNr2"
    fdx_groupid = 1227

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class PartNrDMSMNr3:
    de_name     = "PartNrDMSM.Nr3"
    fdx_name    = "PartNrDMSMNr3"
    fdx_groupid = 1227

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class PartNrDMSMNr4:
    de_name     = "PartNrDMSM.Nr4"
    fdx_name    = "PartNrDMSMNr4"
    fdx_groupid = 1227

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class PasAlrmDeactvnReq:
    de_name     = "PasAlrmDeactvnReq"
    fdx_name    = "PasAlrmDeactvnReq"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PasAlrmSts:
    de_name     = "PasAlrmSts"
    fdx_name    = "PasAlrmSts"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        OffReq = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassMassgRunng:
    de_name     = "PassMassgRunng"
    fdx_name    = "PassMassgRunng"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatActvSpplFct:
    de_name     = "PassSeatActvSpplFct"
    fdx_name    = "PassSeatActvSpplFct"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotAvl = 0
        LumExtnAndLumHei = 1
        BackBlster = 2
        CushExtn = 3
        HdrestHeiAndHdrestTilt = 4
        MassgFct = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatDispMassgFctMassgInten:
    de_name     = "PassSeatDispMassgFct.MassgInten"
    fdx_name    = "PassSeatDispMassgFctMassgInten"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatDispMassgFctMassgProg:
    de_name     = "PassSeatDispMassgFct.MassgProg"
    fdx_name    = "PassSeatDispMassgFctMassgProg"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Prog1 = 0
        Prog2 = 1
        Prog3 = 2
        Prog4 = 3
        Prog5 = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatDispMassgFctMassgSpdLvl:
    de_name     = "PassSeatDispMassgFct.MassgSpdLvl"
    fdx_name    = "PassSeatDispMassgFctMassgSpdLvl"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatDispMassgFctOnOff:
    de_name     = "PassSeatDispMassgFct.OnOff"
    fdx_name    = "PassSeatDispMassgFctOnOff"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatDispSpplFct:
    de_name     = "PassSeatDispSpplFct"
    fdx_name    = "PassSeatDispSpplFct"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotAvl = 0
        LumExtnAndLumHei = 1
        BackBlster = 2
        CushExtn = 3
        HdrestHeiAndHdrestTilt = 4
        MassgFct = 5

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatMassgFctMassgInten:
    de_name     = "PassSeatMassgFct.MassgInten"
    fdx_name    = "PassSeatMassgFctMassgInten"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatMassgFctMassgProg:
    de_name     = "PassSeatMassgFct.MassgProg"
    fdx_name    = "PassSeatMassgFctMassgProg"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Prog1 = 0
        Prog2 = 1
        Prog3 = 2
        Prog4 = 3
        Prog5 = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatMassgFctMassgSpdLvl:
    de_name     = "PassSeatMassgFct.MassgSpdLvl"
    fdx_name    = "PassSeatMassgFctMassgSpdLvl"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatMassgFctOnOff:
    de_name     = "PassSeatMassgFct.OnOff"
    fdx_name    = "PassSeatMassgFctOnOff"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatSts:
    de_name     = "PassSeatSts"
    fdx_name    = "PassSeatSts"
    fdx_groupid = 1350

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Empty = 0
        Fmale = 1
        OccptLrg = 2
        Ukwn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtAdjmtOfSpplFctVerSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatSwtSts2PassSeatSwtHeiFrntSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtHeiFrntSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtHeiFrntSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatSwtSts2PassSeatSwtHeiSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtHeiSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtHeiSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatSwtSts2PassSeatSwtInclSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtInclSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtInclSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtSelnOfSpplFctStsSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PassSeatSwtSts2PassSeatSwtSldSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtSldSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtSldSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: rad,  Range:-14.5->14.5, Resolution: (0.0009765625*x+0.0, raw is signed, 15 bits )
class PinionSteerAg1:
    de_name     = "PinionSteerAg1.PinionSteerAg1"
    fdx_name    = "PinionSteerAg1"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -14.5
    max    = 14.5
    scale  = 0.0009765625
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class PinionSteerAg1Qf:
    de_name     = "PinionSteerAg1.PinionSteerAg1Qf"
    fdx_name    = "PinionSteerAg1Qf"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: m,  Range:-100->6000, Resolution: (0.1*x+-100.0, raw is unsigned, 16 bits )
class PosnAltiInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnAlti"
    fdx_name    = "PosnAltiInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -100
    max    = 6000
    scale  = 0.1
    offset = -100.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Deg,  Range:0->359.99, Resolution: (0.01*x+0.0, raw is unsigned, 16 bits )
class PosnDirInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnDir"
    fdx_name    = "PosnDirInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 359.99
    scale  = 0.01
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Deg,  Range:-90->90, Resolution: (2.7777777777777776e-07*x+0.0, raw is signed, 30 bits )
class PosnLatInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnLat"
    fdx_name    = "PosnLatInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -90
    max    = 90
    scale  = 2.7777777777777776e-07
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Deg,  Range:-180->180, Resolution: (2.7777777777777776e-07*x+0.0, raw is signed, 31 bits )
class PosnLgtInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnLgt"
    fdx_name    = "PosnLgtInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -180
    max    = 180
    scale  = 2.7777777777777776e-07
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class PosnQlyInDstToManvLocn:
    de_name     = "DstToManvLocn.PosnQly"
    fdx_name    = "PosnQlyInDstToManvLocn"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        QlyOfLvl0 = 0
        QlyOfLvl1 = 1
        QlyOfLvl2 = 2
        QlyOfLvl3 = 3
        QlyOfLvl4 = 4
        QlyOfLvl5 = 5
        QlyOfLvl6 = 6
        QlyOfLvl7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 17 bits )
class PosnSpdInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnSpd"
    fdx_name    = "PosnSpdInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 100
    scale  = 0.001
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 17 bits )
class PosnVHozlInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnVHozl"
    fdx_name    = "PosnVHozlInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 100
    scale  = 0.001
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:-100->100, Resolution: (0.001*x+0.0, raw is signed, 18 bits )
class PosnVVertInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnVVert"
    fdx_name    = "PosnVVertInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -100
    max    = 100
    scale  = 0.001
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class PostDrvgClimaAvl:
    de_name     = "PostDrvgClimaAvl"
    fdx_name    = "PostDrvgClimaAvl"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PreClngNotif:
    de_name     = "PreClngNotif"
    fdx_name    = "PreClngNotif"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        NoWarn = 1
        Done = 2
        Err = 3
        Intrpt = 4
        Spare1 = 5
        Spare2 = 6
        Spare3 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreHozlDilInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PreHozlDil"
    fdx_name    = "PreHozlDilInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PrePosnDilInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PrePosnDil"
    fdx_name    = "PrePosnDilInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreTiDilInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PreTiDil"
    fdx_name    = "PreTiDilInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreVertDilInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PreVertDil"
    fdx_name    = "PreVertDilInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class PrkgAssiFailr:
    de_name     = "PrkgAssiFailr"
    fdx_name    = "PrkgAssiFailr"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoFailr = 0
        SgnlFailr = 1
        SnsrFailr = 2
        DgrSnsr = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PrkgAssiManvActvSts:
    de_name     = "PrkgAssiManvActvSts"
    fdx_name    = "PrkgAssiManvActvSts"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->63
class PrkgAssiManvProgs:
    de_name     = "PrkgAssiManvProgs"
    fdx_name    = "PrkgAssiManvProgs"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 63
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class PrkgAssiSts:
    de_name     = "PrkgAssiSts"
    fdx_name    = "PrkgAssiSts"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        SnsrAllOff = 0
        FourReSnsrActv = 1
        FourFrntFourReSnsrAct = 2
        SixFrntSnsrTwoReActv = 3
        Resd7 = 4
        SnsrAllStb = 5
        Resd3 = 6
        Resd4 = 7
        Resd5 = 8
        SnsrFailrAllOff = 9
        SnsrAllDeactvd = 10
        SnsrTrlrOff4 = 11
        SnsrTrlrOff12 = 12
        SixFrntSnsrAndSixReActv = 13
        Resd1 = 14
        Resd2 = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PrkgAutSts:
    de_name     = "PrkgAutSts"
    fdx_name    = "PrkgAutSts"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        SAPNotActive = 0
        SAPPassiveScan = 1
        OverspeedSlowDown = 2
        ScanLeft = 3
        ScanRight = 4
        SAPPSlotFoundLeftGoForward = 5
        SAPPSlotFoundRightGoForward = 6
        PPASlotFoundLeftGoForward = 7
        PPASlotFoundRightGoForward = 8
        SAPStopToParkLeft = 9
        SAPStopToParkRight = 10
        PPAStopToParkLeft = 11
        PPAStopToParkRight = 12
        SAPPStopAndReverseGearLeft = 13
        SAPPStopAndReverseGearRight = 14
        PPAStopAndReverseGearLeft = 15
        PPAStopAndReverseGearRight = 16
        SAPPLookAroundLeft = 17
        SAPPLookAroundRight = 18
        PPALookAroundLeft = 19
        PPALookAroundRight = 20
        SAPPFirstMoveLeft = 21
        SAPPFirstMoveRight = 22
        PPAFirstMoveLeft = 23
        PPAFirstMoveRight = 24
        SAPPChangeToForwardLeft = 25
        SAPPChangeToForwardRight = 26
        PPAChangeToForwardLeft = 27
        PPAChangeToForwardRight = 28
        SAPPMoveForwardLeft = 29
        SAPPMoveForwardRight = 30
        PPAMoveForwardLeft = 31
        PPAMoveForwardRight = 32
        SAPPChangeToReverseLeft = 33
        SAPPChangeToReverseRight = 34
        PPAChangeToReverseLeft = 35
        PPAChangeToReverseRight = 36
        SAPPMoveRearwardLeft = 37
        SAPPMoveRearwardRight = 38
        PPAMoveRearwardLeft = 39
        PPAMoveRearwardRight = 40
        Finished = 41
        ParkingFail = 42
        Off = 43
        TemporaryNotAvailable = 44
        CancelHighSpeed = 45
        CancelSlipperyRoad = 46
        CancelTrailerConnect = 47
        CancelEngineStall = 48
        CancelSteringIntervention = 49
        CancelDriverDeactivation = 50
        CancelManyAttempts = 51
        CancelWrongGear = 52
        Resume = 53
        SelectParkOutSide = 54
        POAScannig = 55
        POAPathNotFound = 56
        POAChanageToForwardGearLeft = 57
        POAChanageToForwardGearRight = 58
        POAChanageToReverseGearLeft = 59
        POAChanageToReverseGearRight = 60
        POAStopAndChangeToForwardGearLeft = 61
        POAStopAndChangeToForwardGearRight = 62
        POAStopAndChangeToReverseGearLeft = 63
        POAStopAndChangeToReverseGearRight = 64
        POARearwardMoveLeft = 65
        POARearwardMoveRight = 66
        POAForwardMoveLeft = 67
        POAForwardMoveRight = 68
        POAFinishedGoLeft = 69
        POAFinishedGoRight = 70
        POALookAroundLeft = 71
        POALookAroundRight = 72
        POAFailr = 73
        EmgyBrkDsbld = 74
        EmgyBrkActv = 75
        EmgyBrkUsrActn = 76

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class PrkgCamSysAvlSts:
    de_name     = "PrkgCamSysAvlSts"
    fdx_name    = "PrkgCamSysAvlSts"
    fdx_groupid = 1140

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PrkgTypVld:
    de_name     = "PrkgTypVld"
    fdx_name    = "PrkgTypVld"
    fdx_groupid = 1327

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoPrkgDetd = 0
        ParaPrkgDetd = 1
        PerpPrkgDetd = 2
        ParaAndPerpPrkgDetd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ProfActProf1:
    de_name     = "ProfAct.Prof1"
    fdx_name    = "ProfActProf1"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf10:
    de_name     = "ProfAct.Prof10"
    fdx_name    = "ProfActProf10"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf11:
    de_name     = "ProfAct.Prof11"
    fdx_name    = "ProfActProf11"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf12:
    de_name     = "ProfAct.Prof12"
    fdx_name    = "ProfActProf12"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf2:
    de_name     = "ProfAct.Prof2"
    fdx_name    = "ProfActProf2"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf3:
    de_name     = "ProfAct.Prof3"
    fdx_name    = "ProfActProf3"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf4:
    de_name     = "ProfAct.Prof4"
    fdx_name    = "ProfActProf4"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf5:
    de_name     = "ProfAct.Prof5"
    fdx_name    = "ProfActProf5"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf6:
    de_name     = "ProfAct.Prof6"
    fdx_name    = "ProfActProf6"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf7:
    de_name     = "ProfAct.Prof7"
    fdx_name    = "ProfActProf7"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf8:
    de_name     = "ProfAct.Prof8"
    fdx_name    = "ProfActProf8"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfActProf9:
    de_name     = "ProfAct.Prof9"
    fdx_name    = "ProfActProf9"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ProfChg:
    de_name     = "ProfChg"
    fdx_name    = "ProfChg"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ProfLimdProf1:
    de_name     = "ProfLimd.Prof1"
    fdx_name    = "ProfLimdProf1"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf10:
    de_name     = "ProfLimd.Prof10"
    fdx_name    = "ProfLimdProf10"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf11:
    de_name     = "ProfLimd.Prof11"
    fdx_name    = "ProfLimdProf11"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf12:
    de_name     = "ProfLimd.Prof12"
    fdx_name    = "ProfLimdProf12"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf2:
    de_name     = "ProfLimd.Prof2"
    fdx_name    = "ProfLimdProf2"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf3:
    de_name     = "ProfLimd.Prof3"
    fdx_name    = "ProfLimdProf3"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf4:
    de_name     = "ProfLimd.Prof4"
    fdx_name    = "ProfLimdProf4"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf5:
    de_name     = "ProfLimd.Prof5"
    fdx_name    = "ProfLimdProf5"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf6:
    de_name     = "ProfLimd.Prof6"
    fdx_name    = "ProfLimdProf6"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf7:
    de_name     = "ProfLimd.Prof7"
    fdx_name    = "ProfLimdProf7"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf8:
    de_name     = "ProfLimd.Prof8"
    fdx_name    = "ProfLimdProf8"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class ProfLimdProf9:
    de_name     = "ProfLimd.Prof9"
    fdx_name    = "ProfLimdProf9"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ProfPenSts1:
    de_name     = "ProfPenSts1"
    fdx_name    = "ProfPenSts1"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Propulsion HV Battery usage mode type
class PrpsnHvBattUsgModAct:
    de_name     = "PrpsnHvBattUsgModAct"
    fdx_name    = "PrpsnHvBattUsgModAct"
    fdx_groupid = 1372

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        Norm = 1
        Hld = 2
        HldSmt = 3
        Chrg = 4
        NotOfUse1 = 5
        NotOfUse2 = 6
        NotOfUse3 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Propulsion HV Battery usage mode type
class PrpsnHvBattUsgModReq:
    de_name     = "PrpsnHvBattUsgModReq"
    fdx_name    = "PrpsnHvBattUsgModReq"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoReq = 0
        Norm = 1
        Hld = 2
        HldSmt = 3
        Chrg = 4
        NotOfUse1 = 5
        NotOfUse2 = 6
        NotOfUse3 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PrpsnHvBattUsgOfChrgBlkd:
    de_name     = "PrpsnHvBattUsgOfChrgBlkd"
    fdx_name    = "PrpsnHvBattUsgOfChrgBlkd"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# HV battery usage function blocked types
class PrpsnHvBattUsgOfChrgBlkd2:
    de_name     = "PrpsnHvBattUsgOfChrgBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfChrgBlkd2"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotBlkd = 0
        BlkdByBattLvl = 1
        BlkdBySysFlt = 2
        NotOfUse = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PrpsnHvBattUsgOfHldBlkd:
    de_name     = "PrpsnHvBattUsgOfHldBlkd"
    fdx_name    = "PrpsnHvBattUsgOfHldBlkd"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# HV battery usage function blocked types
class PrpsnHvBattUsgOfHldBlkd2:
    de_name     = "PrpsnHvBattUsgOfHldBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfHldBlkd2"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotBlkd = 0
        BlkdByBattLvl = 1
        BlkdBySysFlt = 2
        NotOfUse = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PrpsnHvBattUsgOfHldSmtBlkd:
    de_name     = "PrpsnHvBattUsgOfHldSmtBlkd"
    fdx_name    = "PrpsnHvBattUsgOfHldSmtBlkd"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# HV battery usage function blocked types
class PrpsnHvBattUsgOfHldSmtBlkd2:
    de_name     = "PrpsnHvBattUsgOfHldSmtBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfHldSmtBlkd2"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotBlkd = 0
        BlkdByBattLvl = 1
        BlkdBySysFlt = 2
        NotOfUse = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Km/h,  Range:0->150
class PrpsnHvBattUsgOfHldSpd:
    de_name     = "PrpsnHvBattUsgOfHldSpd"
    fdx_name    = "PrpsnHvBattUsgOfHldSpd"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 150
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Basic type list
# 8 types
class PrpsnModElecDrvBlkd:
    de_name     = "PrpsnModElecDrvBlkd"
    fdx_name    = "PrpsnModElecDrvBlkd"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Basic type list
# 8 types
class PrpsnModOffroadBlkd:
    de_name     = "PrpsnModOffroadBlkd"
    fdx_name    = "PrpsnModOffroadBlkd"
    fdx_groupid = 1375

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Basic type list
# 8 types
class PrpsnModOfSaveBlkd:
    de_name     = "PrpsnModOfSaveBlkd"
    fdx_name    = "PrpsnModOfSaveBlkd"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Basic type list
# 8 types
class PrpsnModOfTracBlkd:
    de_name     = "PrpsnModOfTracBlkd"
    fdx_name    = "PrpsnModOfTracBlkd"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Basic type list
# 8 types
class PrpsnModSptBlkd:
    de_name     = "PrpsnModSptBlkd"
    fdx_name    = "PrpsnModSptBlkd"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: NewtonMeter,  Range:-1024->1023, Resolution: (1.0*x+0.0, raw is unsigned, 11 bits )
class PtCluTqPtCluTq:
    de_name     = "PtCluTq.PtCluTq"
    fdx_name    = "PtCluTqPtCluTq"
    fdx_groupid = 1371

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -1024
    max    = 1023
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: NewtonMeter,  Range:-1024->1023, Resolution: (1.0*x+0.0, raw is unsigned, 11 bits )
class PtCluTqPtCluTqDyn:
    de_name     = "PtCluTq.PtCluTqDyn"
    fdx_name    = "PtCluTqPtCluTqDyn"
    fdx_groupid = 1371

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -1024
    max    = 1023
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# General DATA-TYPE to indicate quality. Each quality deviation value, De0-De7, shall be specified on the DATA-ELEMENT using this DATA-TYPE.
class PtCluTqPtCluTqQly:
    de_name     = "PtCluTq.PtCluTqQly"
    fdx_name    = "PtCluTqPtCluTqQly"
    fdx_groupid = 1371

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        De0 = 0
        De1 = 1
        De2 = 2
        De3 = 3
        De4 = 4
        De5 = 5
        De6 = 6
        De7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PtGearTar:
    de_name     = "PtGearTar"
    fdx_name    = "PtGearTar"
    fdx_groupid = 1374

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Neut = 0
        Gear1 = 1
        Gear2 = 2
        Gear3 = 3
        Gear4 = 4
        Gear5 = 5
        Gear6 = 6
        Gear7 = 7
        Gear8 = 8
        Gear9 = 9
        Gear10 = 10
        Gear11 = 11
        Gear12 = 12
        Gear13 = 13
        Ukwn = 14
        Rvs = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtAxleFrntAct:
    de_name     = "PtTqAtWhlFrntAct.PtTqAtAxleFrntAct"
    fdx_name    = "PtTqAtAxleFrntAct"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtWhlFrntLeAct:
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlFrntLeAct"
    fdx_name    = "PtTqAtWhlFrntLeAct"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtWhlFrntRiAct:
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlFrntRiAct"
    fdx_name    = "PtTqAtWhlFrntRiAct"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# General DATA-TYPE to indicate quality. Each quality deviation value, De0-De7, shall be specified on the DATA-ELEMENT using this DATA-TYPE.
class PtTqAtWhlsFrntQly:
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlsFrntQly"
    fdx_name    = "PtTqAtWhlsFrntQly"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        De0 = 0
        De1 = 1
        De2 = 2
        De3 = 3
        De4 = 4
        De5 = 5
        De6 = 6
        De7 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PUnit:
    de_name     = "IndcnUnit.PUnit"
    fdx_name    = "PUnit"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Pa = 0
        Psi = 1
        Bar = 2
        Atm = 3
        UkwnUnit = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class PwrChrgDetdForPrkgHeatrElec:
    de_name     = "PwrChrgDetdForPrkgHeatrElec"
    fdx_name    = "PwrChrgDetdForPrkgHeatrElec"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class PwrLvlElecMai:
    de_name     = "VehModMngtGlbSafe1.PwrLvlElecMai"
    fdx_name    = "PwrLvlElecMai"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class PwrLvlElecSubtyp:
    de_name     = "VehModMngtGlbSafe1.PwrLvlElecSubtyp"
    fdx_name    = "PwrLvlElecSubtyp"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Status of power supply components
class PwrSplyErrSts:
    de_name     = "PwrSplyErrSts"
    fdx_name    = "PwrSplyErrSts"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        SysOk = 0
        AltFltMecl = 1
        AltFltElec = 2
        AltFltT = 3
        AltFltCom = 4
        UHiDurgDrvg = 5
        BattSwtFltCom = 6
        BattSwtHwFlt = 7
        ULoDurgDrvg = 8
        BattSnsrFltCom = 9
        BattSnsrHwFlt = 10
        SpprtBattFltChrgn = 11
        FltComDcDc = 12
        FltElecDcDc = 13
        FltTDcDc = 14

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RainSenMemdReqPen:
    de_name     = "RainSenMemdReq.Pen"
    fdx_name    = "RainSenMemdReqPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RainSenMemdReqSts:
    de_name     = "RainSenMemdReq.Sts"
    fdx_name    = "RainSenMemdReqSts"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class ReAxleWarn:
    de_name     = "ReAxleWarn"
    fdx_name    = "ReAxleWarn"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        Resd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RmnLockgPrsnlReq:
    de_name     = "RmnLockgPrsnlReq"
    fdx_name    = "RmnLockgPrsnlReq"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RngbdIllmnCmdPen:
    de_name     = "RngbdIllmnCmd.Pen"
    fdx_name    = "RngbdIllmnCmdPen"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# For running board illumination setting value
class RngbdIllmnCmdSts:
    de_name     = "RngbdIllmnCmd.Cmd"
    fdx_name    = "RngbdIllmnCmdSts"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Di = 0
        Aut = 1
        Resd2 = 2
        Resd3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RoadFricIndcnActvPen:
    de_name     = "RoadFricIndcnActv.Pen"
    fdx_name    = "RoadFricIndcnActvPen"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RoadFricIndcnActvSts:
    de_name     = "RoadFricIndcnActv.Sts"
    fdx_name    = "RoadFricIndcnActvSts"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class RoadFricIndcnSts:
    de_name     = "RoadFricIndcnSts"
    fdx_name    = "RoadFricIndcnSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoWarn		No Warning		No warning
# 1			1			FricLoPsbl	Friction 	Low Possible		Potentially low friction
# 2			2			FricLoCfmd	Friction Low Confirmed	Confirmed low friction
class RoadFricWarnReq:
    de_name     = "RoadFricWarnReq"
    fdx_name    = "RoadFricWarnReq"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        FricLoPsbl = 1
        FricLoCfmd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RoadSgnInfoActvPen:
    de_name     = "RoadSgnInfoActv.Pen"
    fdx_name    = "RoadSgnInfoActvPen"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RoadSgnInfoActvSts:
    de_name     = "RoadSgnInfoActv.Sts"
    fdx_name    = "RoadSgnInfoActvSts"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class RoadSgnInfoSts:
    de_name     = "RoadSgnInfoSts"
    fdx_name    = "RoadSgnInfoSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RoadSpdLimActvSts:
    de_name     = "RoadSpdLimActvSts"
    fdx_name    = "RoadSpdLimActvSts"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class RoadUsrProtnSts:
    de_name     = "RoadUsrProtnSts"
    fdx_name    = "RoadUsrProtnSts"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: rad/s,  Range:-6->6, Resolution: (0.000244140625*x+0.0, raw is signed, 16 bits )
class RollRate1:
    de_name     = "AgDataRawSafe.RollRate"
    fdx_name    = "RollRate1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -6
    max    = 6
    scale  = 0.000244140625
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class RollRate1Qf1:
    de_name     = "AgDataRawSafe.RollRateQf"
    fdx_name    = "RollRate1Qf1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Signal for CCSM rotary knob
class RotyDirReq1:
    de_name     = "RotyDirReq1"
    fdx_name    = "RotyDirReq1"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        CW = 1
        CCW = 2
        Spare = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RotyDirReq2:
    de_name     = "RotyDirReq2"
    fdx_name    = "RotyDirReq2"
    fdx_groupid = 1226

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        CW = 1
        CCW = 2
        Failr = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->31
class RotyPosReq1:
    de_name     = "RotyPosReq1"
    fdx_name    = "RotyPosReq1"
    fdx_groupid = 1182

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->31
class RotyPosReq2:
    de_name     = "RotyPosReq2"
    fdx_name    = "RotyPosReq2"
    fdx_groupid = 1226

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class RsdsSysStsLeCtraSts:
    de_name     = "RsdsSysStsLe.CtraSts"
    fdx_name    = "RsdsSysStsLeCtraSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsLeCtraSts1:
    de_name     = "RsdsSysStsLe.CtraSts1"
    fdx_name    = "RsdsSysStsLeCtraSts1"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Active = 0
        Inactive = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsLeLcmaSts:
    de_name     = "RsdsSysStsLe.LcmaSts"
    fdx_name    = "RsdsSysStsLeLcmaSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsLeLcmaSts1:
    de_name     = "RsdsSysStsLe.LcmaSts1"
    fdx_name    = "RsdsSysStsLeLcmaSts1"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Active = 0
        Inactive = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsLeRcmSts:
    de_name     = "RsdsSysStsLe.RcmSts"
    fdx_name    = "RsdsSysStsLeRcmSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsLeRcwSts:
    de_name     = "RsdsSysStsLe.RcwSts"
    fdx_name    = "RsdsSysStsLeRcwSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsLeRsdsSts:
    de_name     = "RsdsSysStsLe.RsdsSts"
    fdx_name    = "RsdsSysStsLeRsdsSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Cfg = 0
        StrtUp = 1
        Runng = 2
        Blkd = 3
        Faulty = 4
        Shutdown = 5
        Hot = 6
        Cal = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsRiCtraSts:
    de_name     = "RsdsSysStsRi.CtraSts"
    fdx_name    = "RsdsSysStsRiCtraSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsRiCtraSts1:
    de_name     = "RsdsSysStsRi.CtraSts1"
    fdx_name    = "RsdsSysStsRiCtraSts1"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Active = 0
        Inactive = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsRiLcmaSts:
    de_name     = "RsdsSysStsRi.LcmaSts"
    fdx_name    = "RsdsSysStsRiLcmaSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsRiLcmaSts1:
    de_name     = "RsdsSysStsRi.LcmaSts1"
    fdx_name    = "RsdsSysStsRiLcmaSts1"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Active = 0
        Inactive = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsRiRcmSts:
    de_name     = "RsdsSysStsRi.RcmSts"
    fdx_name    = "RsdsSysStsRiRcmSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsRiRcwSts:
    de_name     = "RsdsSysStsRi.RcwSts"
    fdx_name    = "RsdsSysStsRiRcwSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class RsdsSysStsRiRsdsSts:
    de_name     = "RsdsSysStsRi.RsdsSts"
    fdx_name    = "RsdsSysStsRiRsdsSts"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Cfg = 0
        StrtUp = 1
        Runng = 2
        Blkd = 3
        Faulty = 4
        Shutdown = 5
        Hot = 6
        Cal = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# 0: No navigation solution
# 1: 3 satelites solution (2D)
# 2: more than 3 satellites solution (3D)
# 3: CSF (Cold Start Fix)
class SatltPosnStsPrm1InPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltPosnStsPrm1"
    fdx_name    = "SatltPosnStsPrm1InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PosnStsOfLvl1 = 0
        PosnStsOfLvl2 = 1
        PosnStsOfLvl3 = 2
        PosnStsOfLvl4 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SatltPosnStsPrm2InPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltPosnStsPrm2"
    fdx_name    = "SatltPosnStsPrm2InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SatltPosnStsPrm3InPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltPosnStsPrm3"
    fdx_name    = "SatltPosnStsPrm3InPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SatltSysNo1InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo1InUse"
    fdx_name    = "SatltSysNo1InUseInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SatltSysNo2InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo2InUse"
    fdx_name    = "SatltSysNo2InUseInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SatltSysNo3InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo3InUse"
    fdx_name    = "SatltSysNo3InUseInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SatltSysNo4InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo4InUse"
    fdx_name    = "SatltSysNo4InUseInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SatltSysNo5InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo5InUse"
    fdx_name    = "SatltSysNo5InUseInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SatltSysNo6InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo6InUse"
    fdx_name    = "SatltSysNo6InUseInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SaveSetgToMemPrmnt:
    de_name     = "SaveSetgToMemPrmnt"
    fdx_name    = "SaveSetgToMemPrmnt"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        Aut = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: %,  Range:0->100, Resolution: (5.0*x+0.0, raw is unsigned, 0 bits )
class ScrBarVolIndcn:
    de_name     = "ScrBarVolIndcn"
    fdx_name    = "ScrBarVolIndcn"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 100
    scale  = 5.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Litre,  Range:0->31
class ScrMaxFillgVol:
    de_name     = "ScrMaxFillgVol"
    fdx_name    = "ScrMaxFillgVol"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Litre,  Range:0->31
class ScrReagentTankVol:
    de_name     = "ScrReagentTankVol"
    fdx_name    = "ScrReagentTankVol"
    fdx_groupid = 1377

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SeatBackUnlckdThrdLe:
    de_name     = "SeatBackUnlckdThrd.SeatBackUnlckdLe"
    fdx_name    = "SeatBackUnlckdThrdLe"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SeatBackUnlckdThrdRi:
    de_name     = "SeatBackUnlckdThrd.SeatBackUnlckdRi"
    fdx_name    = "SeatBackUnlckdThrdRi"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SeatDispBtnPsdDrvrSeatDispBtnPsd:
    de_name     = "SeatDispBtnPsd.DrvrSeatDispBtnPsd"
    fdx_name    = "SeatDispBtnPsdDrvrSeatDispBtnPsd"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SeatDispBtnPsdPassSeatDispBtnPsd:
    de_name     = "SeatDispBtnPsd.PassSeatDispBtnPsd"
    fdx_name    = "SeatDispBtnPsdPassSeatDispBtnPsd"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SeatHeatDurgClimaEnad:
    de_name     = "SeatHeatDurgClimaEnad"
    fdx_name    = "SeatHeatDurgClimaEnad"
    fdx_groupid = 1279

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        SeatHeatOff = 0
        SeatDrvOn = 1
        SeatPassOn = 2
        SeatDrvrAndPass = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class SeatHeatgAutCdn:
    de_name     = "SeatHeatgAutCdn"
    fdx_name    = "SeatHeatgAutCdn"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Rst = 0
        Set = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SeatHeatLvlReqLe:
    de_name     = "SeatHeatLvlReqLe"
    fdx_name    = "SeatHeatLvlReqLe"
    fdx_groupid = 1335

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SeatHeatLvlReqRi:
    de_name     = "SeatHeatLvlReqRi"
    fdx_name    = "SeatHeatLvlReqRi"
    fdx_groupid = 1335

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SeatSwtLeSigThrd:
    de_name     = "SeatSwtLeSigThrd"
    fdx_name    = "SeatSwtLeSigThrd"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SeatSwtRiSigThrd:
    de_name     = "SeatSwtRiSigThrd"
    fdx_name    = "SeatSwtRiSigThrd"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        Up = 1
        Dwn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class SeatVentnAutCdn:
    de_name     = "SeatVentnAutCdn"
    fdx_name    = "SeatVentnAutCdn"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Rst = 0
        Set = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: s,  Range:0->59
class Sec:
    de_name     = "TiAndDateIndcn.Sec1"
    fdx_name    = "Sec"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: s,  Range:0->59
class Sec1:
    de_name     = "SetTiAndDate.Second"
    fdx_name    = "Sec1"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SetOfLangIdPen:
    de_name     = "SetOfLang.IdPen"
    fdx_name    = "SetOfLangIdPen"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Uknw = Unknown
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
class SetOfLangLangTyp:
    de_name     = "SetOfLang.LangTyp"
    fdx_name    = "SetOfLangLangTyp"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Lang1 = 1
        Lang2 = 2
        Lang3 = 3
        Lang4 = 4
        Lang5 = 5
        Lang6 = 6
        Lang7 = 7
        Lang8 = 8
        Lang9 = 9
        Lang10 = 10
        Lang11 = 11
        Lang12 = 12
        Lang13 = 13
        Lang14 = 14
        Lang15 = 15
        Lang16 = 16
        Lang17 = 17
        Lang18 = 18
        Lang19 = 19
        Lang20 = 20
        Lang21 = 21
        Lang22 = 22
        Lang23 = 23
        Lang24 = 24
        Lang25 = 25
        Lang26 = 26
        Lang27 = 27
        Lang28 = 28
        Lang29 = 29
        Lang30 = 30
        Lang31 = 31
        Lang32 = 32
        Lang33 = 33
        Lang34 = 34
        Lang35 = 35
        Lang36 = 36
        Lang37 = 37

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SftyCchAccActvnStsActv:
    de_name     = "SftyCchAccActvnSts.Actv"
    fdx_name    = "SftyCchAccActvnStsActv"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->15
class SftyCchAccActvnStsDay:
    de_name     = "SftyCchAccActvnSts.Day"
    fdx_name    = "SftyCchAccActvnStsDay"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class SftyCchAccActvnStsMth:
    de_name     = "SftyCchAccActvnSts.Mth"
    fdx_name    = "SftyCchAccActvnStsMth"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class SftyCchAccActvnStsYr:
    de_name     = "SftyCchAccActvnSts.Yr"
    fdx_name    = "SftyCchAccActvnStsYr"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SftyCchLaneKeepAidActvnStsActv:
    de_name     = "SftyCchLaneKeepAidActvnSts.Actv"
    fdx_name    = "SftyCchLaneKeepAidActvnStsActv"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->15
class SftyCchLaneKeepAidActvnStsDay:
    de_name     = "SftyCchLaneKeepAidActvnSts.Day"
    fdx_name    = "SftyCchLaneKeepAidActvnStsDay"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class SftyCchLaneKeepAidActvnStsMth:
    de_name     = "SftyCchLaneKeepAidActvnSts.Mth"
    fdx_name    = "SftyCchLaneKeepAidActvnStsMth"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class SftyCchLaneKeepAidActvnStsYr:
    de_name     = "SftyCchLaneKeepAidActvnSts.Yr"
    fdx_name    = "SftyCchLaneKeepAidActvnStsYr"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxAttention:
    de_name     = "SftyCchLongTermIdx.Attention"
    fdx_name    = "SftyCchLongTermIdxAttention"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SftyCchLongTermIdxAttentionAvl:
    de_name     = "SftyCchLongTermIdx.AttentionAvl"
    fdx_name    = "SftyCchLongTermIdxAttentionAvl"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxDstToVeh:
    de_name     = "SftyCchLongTermIdx.DstToVeh"
    fdx_name    = "SftyCchLongTermIdxDstToVeh"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SftyCchLongTermIdxDstToVehAvl:
    de_name     = "SftyCchLongTermIdx.DstToVehAvl"
    fdx_name    = "SftyCchLongTermIdxDstToVehAvl"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxKeepSpdLim:
    de_name     = "SftyCchLongTermIdx.KeepSpdLim"
    fdx_name    = "SftyCchLongTermIdxKeepSpdLim"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SftyCchLongTermIdxKeepSpdLimAvl:
    de_name     = "SftyCchLongTermIdx.KeepSpdLimAvl"
    fdx_name    = "SftyCchLongTermIdxKeepSpdLimAvl"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxLaneKeep:
    de_name     = "SftyCchLongTermIdx.LaneKeep"
    fdx_name    = "SftyCchLongTermIdxLaneKeep"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SftyCchLongTermIdxLaneKeepAvl:
    de_name     = "SftyCchLongTermIdx.LaneKeepAvl"
    fdx_name    = "SftyCchLongTermIdxLaneKeepAvl"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxTot:
    de_name     = "SftyCchLongTermIdx.Tot"
    fdx_name    = "SftyCchLongTermIdxTot"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SftyCchLongTermIdxTotAvl:
    de_name     = "SftyCchLongTermIdx.TotAvl"
    fdx_name    = "SftyCchLongTermIdxTotAvl"
    fdx_groupid = 1145

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class SftyWarnGroupFromAsySafeChks:
    de_name     = "SftyWarnGroupFromAsySafe.SftyWarnGroupFromAsySafeChks"
    fdx_name    = "SftyWarnGroupFromAsySafeChks"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class SftyWarnGroupFromAsySafeCntr:
    de_name     = "SftyWarnGroupFromAsySafe.SftyWarnGroupFromAsySafeCntr"
    fdx_name    = "SftyWarnGroupFromAsySafeCntr"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SpdAlrmActvForRoadSgnInfoPen:
    de_name     = "SpdAlrmActvForRoadSgnInfo.Pen"
    fdx_name    = "SpdAlrmActvForRoadSgnInfoPen"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SpdAlrmActvForRoadSgnInfoSts:
    de_name     = "SpdAlrmActvForRoadSgnInfo.Sts"
    fdx_name    = "SpdAlrmActvForRoadSgnInfoSts"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SpdCameraInfoSeldForRoadSgnInfoPen:
    de_name     = "SpdCameraInfoSeldForRoadSgnInfo.Pen"
    fdx_name    = "SpdCameraInfoSeldForRoadSgnInfoPen"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SpdCameraInfoSeldForRoadSgnInfoSts:
    de_name     = "SpdCameraInfoSeldForRoadSgnInfo.Sts"
    fdx_name    = "SpdCameraInfoSeldForRoadSgnInfoSts"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SpdLimAdpvSts:
    de_name     = "SpdLimAdpvSts"
    fdx_name    = "SpdLimAdpvSts"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1
        NotAvl = 2
        SrvRqrd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No warning
# 1			1			SpdHi			Speed High			Too high speed compared to speed limit
# 2			2			SpdHiAndSpdCtrlPassd	Speed High And Speed Control Passed	Too high speed and passing speed camera
class SpdLimWarnReq:
    de_name     = "SpdLimWarnReq"
    fdx_name    = "SpdLimWarnReq"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SpdHi = 1
        SpdHiAndSpdCtrlPassd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SpdUnit:
    de_name     = "IndcnUnit.SpdUnit"
    fdx_name    = "SpdUnit"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Kmph = 0
        Mph = 1
        UkwnUnit = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SpdWarn:
    de_name     = "SpdWarn"
    fdx_name    = "SpdWarn"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SpprtForFctInDstToManvLocn:
    de_name     = "DstToManvLocn.SpprtForFct"
    fdx_name    = "SpprtForFctInDstToManvLocn"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SpprtForFctInTireCircumCalByNav:
    de_name     = "TireCircumCalByNav.SpprtForFct"
    fdx_name    = "SpprtForFctInTireCircumCalByNav"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SrvRst:
    de_name     = "SrvRst"
    fdx_name    = "SrvRst"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->15, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class SrvTrig:
    de_name     = "SrvTrig"
    fdx_name    = "SrvTrig"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 15
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class SteerOvrdWarnReqForAutDrv:
    de_name     = "SftyWarnGroupFromAsySafe.SteerOvrdWarnReqForAutDrv"
    fdx_name    = "SteerOvrdWarnReqForAutDrv"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SteerStsForAutDrv:
    de_name     = "SftyWarnGroupFromAsySafe.SteerStsForAutDrv"
    fdx_name    = "SteerStsForAutDrv"
    fdx_groupid = 1138

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Stb = 1
        Actv = 2
        OvrdByDrvr = 3
        PndStb = 4

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			NoWarn			No Warning				No warning
# 1			1			DispWarnForDrvrSteerLe	Display Warning For Driver Steering Left	Show warning that driver shall steer left
# 2			2			DispWarnForDrvrSteerRi	Display Warning For Driver Steering Right	Show warning that driver shall steer right
# 3			3			DispGenWarn		Display Generic Warning			Show general warning
class SteerWarnReqForAutDrv:
    de_name     = "SteerWarnReqForAutDrv"
    fdx_name    = "SteerWarnReqForAutDrv"
    fdx_groupid = 1139

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        DispWarnForDrvrSteerLe = 1
        DispWarnForDrvrSteerRi = 2
        DispGenWarn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SteerWarnReqForLaneKeepAid:
    de_name     = "SteerWarnReqForLaneKeepAid"
    fdx_name    = "SteerWarnReqForLaneKeepAid"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        WarnLe = 1
        WarnRi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: rad,  Range:-14.5->14.5, Resolution: (0.0009765625*x+0.0, raw is signed, 15 bits )
class SteerWhlAgSafe:
    de_name     = "SteerWhlSnsrSafe.SteerWhlAg"
    fdx_name    = "SteerWhlAgSafe"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -14.5
    max    = 14.5
    scale  = 0.0009765625
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: rad/s,  Range:-50->50, Resolution: (0.0078125*x+0.0, raw is signed, 14 bits )
class SteerWhlAgSpdSafe:
    de_name     = "SteerWhlSnsrSafe.SteerWhlAgSpd"
    fdx_name    = "SteerWhlAgSpdSafe"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -50
    max    = 50
    scale  = 0.0078125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SteerWhlHeatgDurgClimaEnad:
    de_name     = "SteerWhlHeatgDurgClimaEnad"
    fdx_name    = "SteerWhlHeatgDurgClimaEnad"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SteerWhlHeatgOnReq:
    de_name     = "SteerWhlHeatgOnReq"
    fdx_name    = "SteerWhlHeatgOnReq"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lo = 1
        Med = 2
        Hi = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SteerWhlHeatgStrtAutCdnOk:
    de_name     = "SteerWhlHeatgStrtAutCdnOk"
    fdx_name    = "SteerWhlHeatgStrtAutCdnOk"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SteerWhlSnsrQf:
    de_name     = "SteerWhlSnsrSafe.SteerWhlSnsrQf"
    fdx_name    = "SteerWhlSnsrQf"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class SteerWhlSnsrSafeChks:
    de_name     = "SteerWhlSnsrSafe.SteerWhlSnsrChks"
    fdx_name    = "SteerWhlSnsrSafeChks"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class SteerWhlSnsrSafeCntr:
    de_name     = "SteerWhlSnsrSafe.SteerWhlSnsrCntr"
    fdx_name    = "SteerWhlSnsrSafeCntr"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class SteerWhlSwtPwr:
    de_name     = "SteerWhlSwtPwr"
    fdx_name    = "SteerWhlSwtPwr"
    fdx_groupid = 1230

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->16777215
class StopStrtInhb:
    de_name     = "StopStrtInhb"
    fdx_name    = "StopStrtInhb"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 16777215
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class StrtInProgs:
    de_name     = "StrtInProgs"
    fdx_name    = "StrtInProgs"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        StrtStsOff = 0
        StrtStsImminent = 1
        StrtStsStrtng = 2
        StrtStsRunng = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SunRoofPosnSts:
    de_name     = "SunRoofPosnSts"
    fdx_name    = "SunRoofPosnSts"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PosnUkwn = 0
        ClsFull = 1
        PercOpen4 = 2
        PercOpen8 = 3
        PercOpen12 = 4
        PercOpen16 = 5
        PercOpen20 = 6
        PercOpen24 = 7
        PercOpen28 = 8
        PercOpen32 = 9
        PercOpen36 = 10
        PercOpen40 = 11
        PercOpen44 = 12
        PercOpen48 = 13
        PercOpen52 = 14
        PercOpen56 = 15
        PercOpen60 = 16
        PercOpen64 = 17
        PercOpen68 = 18
        PercOpen72 = 19
        PercOpen76 = 20
        PercOpen80 = 21
        PercOpen84 = 22
        PercOpen88 = 23
        PercOpen92 = 24
        PercOpen96 = 25
        OpenFull = 26
        Resd1 = 27
        Resd2 = 28
        Resd3 = 29
        Resd4 = 30
        Movg = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SwtAcptReq:
    de_name     = "SwtAcptReq"
    fdx_name    = "SwtAcptReq"
    fdx_groupid = 1226

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotPsd = 0
        Psd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class SwtAtCenSts:
    de_name     = "SwtAtCenSts"
    fdx_name    = "SwtAtCenSts"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoTrig = 0
        Trig = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class SwtForPassAirbCutOffSt:
    de_name     = "SwtForPassAirbCutOffSt"
    fdx_name    = "SwtForPassAirbCutOffSt"
    fdx_groupid = 1350

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        On = 0
        Off = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Status of the door.
class TankFlapSts:
    de_name     = "TankFlapSts"
    fdx_name    = "TankFlapSts"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TelmSts:
    de_name     = "TelmSts"
    fdx_name    = "TelmSts"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoErr = 0
        TelmErr = 1
        TelmDi = 2
        TelmEnd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TiAndDateVld:
    de_name     = "TiAndDateIndcn.DataValid"
    fdx_name    = "TiAndDateVld"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Days,  Range:1->31
class TiForDayInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForDay"
    fdx_name    = "TiForDayInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 1
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: hours,  Range:0->23
class TiForHrInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForHr"
    fdx_name    = "TiForHrInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 23
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Mins,  Range:0->59
class TiForMinsInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForMins"
    fdx_name    = "TiForMinsInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Month,  Range:1->12
class TiForMthInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForMth"
    fdx_name    = "TiForMthInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 1
    max = 12
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: s,  Range:0->59
class TiForSecInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForSec"
    fdx_name    = "TiForSecInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Year,  Range:0->99
class TiForYrInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForYr"
    fdx_name    = "TiForYrInPosnFromSatlt"
    fdx_groupid = 1367

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 99
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TiGapLimdPen:
    de_name     = "TiGapLimd.Pen"
    fdx_name    = "TiGapLimdPen"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: s,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class TiGapLimdSts:
    de_name     = "TiGapLimd.Sts"
    fdx_name    = "TiGapLimdSts"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->4095
class TireCircumInTireCircumCalByNav:
    de_name     = "TireCircumCalByNav.TireCircum"
    fdx_name    = "TireCircumInTireCircumCalByNav"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 4095
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Used for sending a Start or a Stop/Cancel request from HMI to a core function
class TireMonCalReq:
    de_name     = "TireMonCalReq"
    fdx_name    = "TireMonCalReq"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        StrtReq = 1
        StopReq = 2
        Resd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TireMonrDispReq:
    de_name     = "TireMonrDispReq"
    fdx_name    = "TireMonrDispReq"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForFrntLe:
    de_name     = "TirePAbsltValFrntLe.TirepabsltVal1"
    fdx_name    = "TirePAbsltValForFrntLe"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForReLe:
    de_name     = "TirePAbsltValReLe.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValForReLe"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForReRi:
    de_name     = "TirePAbsltValReRi.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValForReRi"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValFrntRiTirePAbsltVal1:
    de_name     = "TirePAbsltValFrntRi.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValFrntRiTirePAbsltVal1"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class TirePAbsltValFrntRiTirePPosn:
    de_name     = "TirePAbsltValFrntRi.TirePPosn"
    fdx_name    = "TirePAbsltValFrntRiTirePPosn"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoPReadingOrPReadingOkWithOutPosn = 0
        PReadingWithPosn = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePAbsltValQFForFrnRi:
    de_name     = "TirePAbsltValFrntRi.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQFForFrnRi"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        OldVal = 1
        UpdVal = 2
        SnsrFlt = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePAbsltValQfForReLe:
    de_name     = "TirePAbsltValReLe.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQfForReLe"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        OldVal = 1
        UpdVal = 2
        SnsrFlt = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePAbsltValQfForReRi:
    de_name     = "TirePAbsltValReRi.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQfForReRi"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        OldVal = 1
        UpdVal = 2
        SnsrFlt = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePAbsltVaQflForFrntLe:
    de_name     = "TirePAbsltValFrntLe.TirePabsltValQF"
    fdx_name    = "TirePAbsltVaQflForFrntLe"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        OldVal = 1
        UpdVal = 2
        SnsrFlt = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePCalSts:
    de_name     = "TirePCalSts"
    fdx_name    = "TirePCalSts"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        Cal = 1
        CalNotCmpl = 2
        CalCmpl = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePFrntLe:
    de_name     = "TirePMonData.TirePFrntLe"
    fdx_name    = "TirePFrntLe"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        ResdWarn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePFrntRi:
    de_name     = "TirePMonData.TirePFrntRe"
    fdx_name    = "TirePFrntRi"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        ResdWarn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePMonSts:
    de_name     = "TirePMonData.TirePMonSts1"
    fdx_name    = "TirePMonSts"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SysFlt = 1
        FourMiss = 2
        Resd1 = 3
        GmnSoftWarn = 4
        GmnHardWarn = 5
        Resd2 = 6
        Resd3 = 7
        Resd4 = 8
        Resd5 = 9

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePPosnForFrntLe:
    de_name     = "TirePAbsltValFrntLe.TirePPosn"
    fdx_name    = "TirePPosnForFrntLe"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoPReadingOrPReadingOkWithOutPosn = 0
        PReadingWithPosn = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePPosnForReLe:
    de_name     = "TirePAbsltValReLe.TirePPosn"
    fdx_name    = "TirePPosnForReLe"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoPReadingOrPReadingOkWithOutPosn = 0
        PReadingWithPosn = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePPosnForReRi:
    de_name     = "TirePAbsltValReRi.TirePPosn"
    fdx_name    = "TirePPosnForReRi"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoPReadingOrPReadingOkWithOutPosn = 0
        PReadingWithPosn = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePReLe:
    de_name     = "TirePMonData.TirePReLe"
    fdx_name    = "TirePReLe"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        ResdWarn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TirePReRi:
    de_name     = "TirePMonData.TirePReRi"
    fdx_name    = "TirePReRi"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        ResdWarn = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TnlEntryOrEnd:
    de_name     = "TnlEntryOrEnd"
    fdx_name    = "TnlEntryOrEnd"
    fdx_groupid = 1139

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        No = 0
        Yes = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class TrfcLiSpprtSts:
    de_name     = "TrfcLiSpprtSts"
    fdx_name    = "TrfcLiSpprtSts"
    fdx_groupid = 1144

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Selects trip trip meter type
# Trip1 - Reset by driver
# Trip2 - Reset by timer (new trip condition)
# Resd - Reserved for future Trip indication modes
class TripModSeln:
    de_name     = "TripModSeln"
    fdx_name    = "TripModSeln"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Trip1 = 1
        Trip2 = 2
        Resd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# 0 = Trailer Lamp Check not possible to execute
# 1 = Trailer Lamp Check possible to execute
class TrlrLampActvtChk:
    de_name     = "TrlrLampChkSts1WdSts1"
    fdx_name    = "TrlrLampActvtChk"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        TrlrLampChkNotAvl = 0
        TrlrLampChkAvl = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TrlrLampChkAutReqPen:
    de_name     = "TrlrLampChkAutReq.Pen"
    fdx_name    = "TrlrLampChkAutReqPen"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TrlrLampChkAutReqSts:
    de_name     = "TrlrLampChkAutReq.Sts"
    fdx_name    = "TrlrLampChkAutReqSts"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Used for sending a Start or a Stop/Cancel request from HMI to a core function
class TrlrLampChkRemReq:
    de_name     = "TrlrLampChkRemReq"
    fdx_name    = "TrlrLampChkRemReq"
    fdx_groupid = 1275

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        StrtReq = 1
        StopReq = 2
        Resd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# 0 = No status
# 1 = Trailer Lamp Check started
# 2 = Trailer Lamp Check completed
# 3 = Operation canceled
# 4 = Trailer Lamp Check requesting engine off to driver
# 5 = Reserved
# 6 = Reserved
# 7 = Reserved
class TrlrLampChkSts:
    de_name     = "TrlrLampChkSts1WdSts2"
    fdx_name    = "TrlrLampChkSts"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Idle = 0
        TrlrLampChkStrtd = 1
        TrlrLampChkCmpl = 2
        CnclOper = 3
        TrlrLampEngOffReqd = 4
        Resd1 = 5
        Resd2 = 6
        Resd3 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# 0 = Trailer unconnected
# 1 = Trailer connected to the vehicle
class TrlrPrsnt:
    de_name     = "TrlrPrsntSts1WdSts"
    fdx_name    = "TrlrPrsnt"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        TrlrNotPrsnt = 0
        TrlrPrsnt = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Status of the door.
class TrSts:
    de_name     = "TrSts"
    fdx_name    = "TrSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TUnit:
    de_name     = "IndcnUnit.TUnit"
    fdx_name    = "TUnit"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Celsius = 0
        Fahrenheit = 1
        UkwnUnit = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TurnAutFlsgReqPen:
    de_name     = "LiExtReq1WdReq3.Pen"
    fdx_name    = "TurnAutFlsgReqPen"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TurnAutFlsgReqSts:
    de_name     = "LiExtReq1WdReq3.Sts"
    fdx_name    = "TurnAutFlsgReqSts"
    fdx_groupid = 1278

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->10000
class TwliBriRaw:
    de_name     = "TwliBriRaw.TwliBriRaw1"
    fdx_name    = "TwliBriRaw"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 10000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class TwliBriRawQf:
    de_name     = "TwliBriRaw.TwliBriRawQf"
    fdx_name    = "TwliBriRawQf"
    fdx_groupid = 1213

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class TwliBriSts:
    de_name     = "TwliBriSts"
    fdx_name    = "TwliBriSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Night = 0
        Day = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte0:
    de_name     = "UkwnCptReqToInfoMgr.Byte0"
    fdx_name    = "UkwnCptReqToInfoMgrByte0"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte1:
    de_name     = "UkwnCptReqToInfoMgr.Byte1"
    fdx_name    = "UkwnCptReqToInfoMgrByte1"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte2:
    de_name     = "UkwnCptReqToInfoMgr.Byte2"
    fdx_name    = "UkwnCptReqToInfoMgrByte2"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte3:
    de_name     = "UkwnCptReqToInfoMgr.Byte3"
    fdx_name    = "UkwnCptReqToInfoMgrByte3"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte4:
    de_name     = "UkwnCptReqToInfoMgr.Byte4"
    fdx_name    = "UkwnCptReqToInfoMgrByte4"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte5:
    de_name     = "UkwnCptReqToInfoMgr.Byte5"
    fdx_name    = "UkwnCptReqToInfoMgrByte5"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte6:
    de_name     = "UkwnCptReqToInfoMgr.Byte6"
    fdx_name    = "UkwnCptReqToInfoMgrByte6"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte7:
    de_name     = "UkwnCptReqToInfoMgr.Byte7"
    fdx_name    = "UkwnCptReqToInfoMgrByte7"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte0:
    de_name     = "UkwnCptReqToSoundMgr.Byte0"
    fdx_name    = "UkwnCptReqToSoundMgrByte0"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte1:
    de_name     = "UkwnCptReqToSoundMgr.Byte1"
    fdx_name    = "UkwnCptReqToSoundMgrByte1"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte2:
    de_name     = "UkwnCptReqToSoundMgr.Byte2"
    fdx_name    = "UkwnCptReqToSoundMgrByte2"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte3:
    de_name     = "UkwnCptReqToSoundMgr.Byte3"
    fdx_name    = "UkwnCptReqToSoundMgrByte3"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte4:
    de_name     = "UkwnCptReqToSoundMgr.Byte4"
    fdx_name    = "UkwnCptReqToSoundMgrByte4"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte5:
    de_name     = "UkwnCptReqToSoundMgr.Byte5"
    fdx_name    = "UkwnCptReqToSoundMgrByte5"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte6:
    de_name     = "UkwnCptReqToSoundMgr.Byte6"
    fdx_name    = "UkwnCptReqToSoundMgrByte6"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte7:
    de_name     = "UkwnCptReqToSoundMgr.Byte7"
    fdx_name    = "UkwnCptReqToSoundMgrByte7"
    fdx_groupid = 1220

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte0:
    de_name     = "UkwnCptRespFromInfoMgr.Byte0"
    fdx_name    = "UkwnCptRespFromInfoMgrByte0"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte1:
    de_name     = "UkwnCptRespFromInfoMgr.Byte1"
    fdx_name    = "UkwnCptRespFromInfoMgrByte1"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte2:
    de_name     = "UkwnCptRespFromInfoMgr.Byte2"
    fdx_name    = "UkwnCptRespFromInfoMgrByte2"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte3:
    de_name     = "UkwnCptRespFromInfoMgr.Byte3"
    fdx_name    = "UkwnCptRespFromInfoMgrByte3"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte4:
    de_name     = "UkwnCptRespFromInfoMgr.Byte4"
    fdx_name    = "UkwnCptRespFromInfoMgrByte4"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte5:
    de_name     = "UkwnCptRespFromInfoMgr.Byte5"
    fdx_name    = "UkwnCptRespFromInfoMgrByte5"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte6:
    de_name     = "UkwnCptRespFromInfoMgr.Byte6"
    fdx_name    = "UkwnCptRespFromInfoMgrByte6"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte7:
    de_name     = "UkwnCptRespFromInfoMgr.Byte7"
    fdx_name    = "UkwnCptRespFromInfoMgrByte7"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte0:
    de_name     = "UkwnCptRespFromSoundMgr.Byte0"
    fdx_name    = "UkwnCptRespFromSoundMgrByte0"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte1:
    de_name     = "UkwnCptRespFromSoundMgr.Byte1"
    fdx_name    = "UkwnCptRespFromSoundMgrByte1"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte2:
    de_name     = "UkwnCptRespFromSoundMgr.Byte2"
    fdx_name    = "UkwnCptRespFromSoundMgrByte2"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte3:
    de_name     = "UkwnCptRespFromSoundMgr.Byte3"
    fdx_name    = "UkwnCptRespFromSoundMgrByte3"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte4:
    de_name     = "UkwnCptRespFromSoundMgr.Byte4"
    fdx_name    = "UkwnCptRespFromSoundMgrByte4"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte5:
    de_name     = "UkwnCptRespFromSoundMgr.Byte5"
    fdx_name    = "UkwnCptRespFromSoundMgrByte5"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte6:
    de_name     = "UkwnCptRespFromSoundMgr.Byte6"
    fdx_name    = "UkwnCptRespFromSoundMgrByte6"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte7:
    de_name     = "UkwnCptRespFromSoundMgr.Byte7"
    fdx_name    = "UkwnCptRespFromSoundMgrByte7"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckFbVisReqPen:
    de_name     = "UnlckFbVisReq.Pen"
    fdx_name    = "UnlckFbVisReqPen"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UnlckFbVisReqSts:
    de_name     = "UnlckFbVisReq.Sts"
    fdx_name    = "UnlckFbVisReqSts"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckKeylsReqPen:
    de_name     = "UnlckKeylsReq.IdPen"
    fdx_name    = "UnlckKeylsReqPen"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UnlckKeylsReqSts:
    de_name     = "UnlckKeylsReq.KeylsCfg"
    fdx_name    = "UnlckKeylsReqSts"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UnlckCen = 0
        UnlckInd = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckRemReqPen:
    de_name     = "UnlckRemReq.Pen"
    fdx_name    = "UnlckRemReqPen"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UnlckRemReqSts:
    de_name     = "UnlckRemReq.Sts"
    fdx_name    = "UnlckRemReqSts"
    fdx_groupid = 1276

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UnlckCen = 0
        UnlckStep2 = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# UsageMode Data
class UsgModSts:
    de_name     = "VehModMngtGlbSafe1.UsgModSts"
    fdx_name    = "UsgModSts"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UsgModAbdnd = 0
        UsgModInActv = 1
        UsgModCnvinc = 2
        UsgModActv = 11
        UsgModDrvg = 13

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UsrSetSpdForKeySpdLimnPen:
    de_name     = "UsrSetSpdForKeySpdLimn.Pen"
    fdx_name    = "UsrSetSpdForKeySpdLimnPen"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdLimnSts:
    de_name     = "UsrSetSpdForKeySpdLimn.Sts"
    fdx_name    = "UsrSetSpdForKeySpdLimnSts"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn1:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn1"
    fdx_name    = "UsrSetSpdForKeySpdWarn1"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn2:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn2"
    fdx_name    = "UsrSetSpdForKeySpdWarn2"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn3:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn3"
    fdx_name    = "UsrSetSpdForKeySpdWarn3"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn4:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn4"
    fdx_name    = "UsrSetSpdForKeySpdWarn4"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn5:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn5"
    fdx_name    = "UsrSetSpdForKeySpdWarn5"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn6:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn6"
    fdx_name    = "UsrSetSpdForKeySpdWarn6"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UsrSetSpdForKeySpdWarnPen:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarnPen"
    fdx_name    = "UsrSetSpdForKeySpdWarnPen"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe"
    fdx_name    = "UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi"
    fdx_name    = "UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class UsrSwtDispClimaReqForTSpForRowSecLe:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispClimaReqForTSpForRowSecLe"
    fdx_name    = "UsrSwtDispClimaReqForTSpForRowSecLe"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class UsrSwtDispClimaReqForTSpForRowSecRi:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispClimaReqForTSpForRowSecRi"
    fdx_name    = "UsrSwtDispClimaReqForTSpForRowSecRi"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class UsrSwtDispClimaReqForTSpSpclForRowSecLe:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispClimaReqForTSpSpclForRowSecLe"
    fdx_name    = "UsrSwtDispClimaReqForTSpSpclForRowSecLe"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        Lo = 1
        Hi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UsrSwtDispClimaReqForTSpSpclForRowSecRi:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispClimaReqForTSpSpclForRowSecRi"
    fdx_name    = "UsrSwtDispClimaReqForTSpSpclForRowSecRi"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Norm = 0
        Lo = 1
        Hi = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UsrSwtDispReqForFanLvlForRowSec:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispReqForFanLvlForRowSec"
    fdx_name    = "UsrSwtDispReqForFanLvlForRowSec"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Min = 1
        LvlMan1 = 2
        LvlMan2 = 3
        LvlMan3 = 4
        LvlMan4 = 5
        LvlMan5 = 6
        LvlMan6 = 7
        LvlMan7 = 8
        LvlAutMinusMinus = 9
        LvlAutMinus = 10
        LvlAutoNorm = 11
        LvlAutPlus = 12
        LvlAutPlusPlus = 13
        Max = 14

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe:
    de_name     = "UsrSwtDispReqForSecRowSeatVentn.UsrSwtDispReqForSecRowSeatVentnLe"
    fdx_name    = "UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe"
    fdx_groupid = 1331

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnRi:
    de_name     = "UsrSwtDispReqForSecRowSeatVentn.UsrSwtDispReqForSecRowSeatVentnRi"
    fdx_name    = "UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnRi"
    fdx_groupid = 1331

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnLe:
    de_name     = "UsrSwtDispReqForSecRowSeatVentn.usrSwtDispUpdReqForSecRowSeatVentnLe"
    fdx_name    = "UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnLe"
    fdx_groupid = 1331

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnRi:
    de_name     = "UsrSwtDispReqForSecRowSeatVentn.usrSwtDispUpdReqForSecRowSeatVentnRi"
    fdx_name    = "UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnRi"
    fdx_groupid = 1331

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe"
    fdx_name    = "UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi"
    fdx_name    = "UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class UsrSwtDispUpdClimaReqForTSpForRowSecLe:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispUpdClimaReqForTSpForRowSecLe"
    fdx_name    = "UsrSwtDispUpdClimaReqForTSpForRowSecLe"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class UsrSwtDispUpdClimaReqForTSpForRowSecRi:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispUpdClimaReqForTSpForRowSecRi"
    fdx_name    = "UsrSwtDispUpdClimaReqForTSpForRowSecRi"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class UsrSwtDispUpdReqForFanLvlForRowSec:
    de_name     = "UsrSwtDispClimaReqForRowSec.UsrSwtDispUpdReqForFanLvlForRowSec"
    fdx_name    = "UsrSwtDispUpdReqForFanLvlForRowSec"
    fdx_groupid = 1330

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    def r2p(raw):
        return raw
    def p2r(physical):
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# 0  No text
# 1  "Engine is running"
# 2  "Ignition on"
# 3  TBD
class VehActvMsgToDrvr:
    de_name     = "VehActvMsgToDrvr"
    fdx_name    = "VehActvMsgToDrvr"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoMsg = 0
        Msg1 = 1
        Msg2 = 2
        Msg3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Volt,  Range:0->25, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class VehBattUSysU:
    de_name     = "VehBattU.SysU"
    fdx_name    = "VehBattUSysU"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 25
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class VehBattUSysUQf:
    de_name     = "VehBattU.SysUQf"
    fdx_name    = "VehBattUSysUQf"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmBlk:
    de_name     = "VehCfgPrm.BlkIDBytePosn1"
    fdx_name    = "VehCfgPrmBlk"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal1:
    de_name     = "VehCfgPrm.CCPBytePosn2"
    fdx_name    = "VehCfgPrmVal1"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal2:
    de_name     = "VehCfgPrm.CCPBytePosn3"
    fdx_name    = "VehCfgPrmVal2"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal3:
    de_name     = "VehCfgPrm.CCPBytePosn4"
    fdx_name    = "VehCfgPrmVal3"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal4:
    de_name     = "VehCfgPrm.CCPBytePosn5"
    fdx_name    = "VehCfgPrmVal4"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal5:
    de_name     = "VehCfgPrm.CCPBytePosn6"
    fdx_name    = "VehCfgPrmVal5"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal6:
    de_name     = "VehCfgPrm.CCPBytePosn7"
    fdx_name    = "VehCfgPrmVal6"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal7:
    de_name     = "VehCfgPrm.CCPBytePosn8"
    fdx_name    = "VehCfgPrmVal7"
    fdx_groupid = 1215

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: kg,  Range:0->10000, Resolution: (1.0*x+0.0, raw is unsigned, 14 bits )
class VehM:
    de_name     = "VehMNom.VehM"
    fdx_name    = "VehM"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 10000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class VehMNomTrlrM:
    de_name     = "VehMNom.TrlrM"
    fdx_name    = "VehMNomTrlrM"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Lvl0 = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class VehModMngtGlbSafe1Chks:
    de_name     = "VehModMngtGlbSafe1.Chks"
    fdx_name    = "VehModMngtGlbSafe1Chks"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class VehModMngtGlbSafe1Cntr:
    de_name     = "VehModMngtGlbSafe1.Cntr"
    fdx_name    = "VehModMngtGlbSafe1Cntr"
    fdx_groupid = 1212

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Main usage shall be to indicate quality for vehicle state estimations, not to be used for failure code setting and function degradation unless absolutely necassary.
class VehMQly:
    de_name     = "VehMNom.VehMQly"
    fdx_name    = "VehMQly"
    fdx_groupid = 1373

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Flt = 0
        NoInfo = 1
        Vld = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->255
class VehMtnStChks:
    de_name     = "VehMtnStSafe.VehMtnStChks"
    fdx_name    = "VehMtnStChks"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class VehMtnStCntr:
    de_name     = "VehMtnStSafe.VehMtnStCntr"
    fdx_name    = "VehMtnStCntr"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class VehMtnStSafe:
    de_name     = "VehMtnStSafe.VehMtnSt"
    fdx_name    = "VehMtnStSafe"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Ukwn = 0
        StandStillVal1 = 1
        StandStillVal2 = 2
        StandStillVal3 = 3
        RollgFwdVal1 = 4
        RollgFwdVal2 = 5
        RollgBackwVal1 = 6
        RollgBackwVal2 = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->511
class VehSpdAvg:
    de_name     = "VehSpdAvgIndcd.VehSpdIndcd"
    fdx_name    = "VehSpdAvg"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 511
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class VehSpdAvgUnit:
    de_name     = "VehSpdAvgIndcd.VeSpdIndcdUnit"
    fdx_name    = "VehSpdAvgUnit"
    fdx_groupid = 1232

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Kmph = 0
        Mph = 1
        UkwnUnit = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class VehSpdCtrlActvSts:
    de_name     = "VehSpdCtrlActvSts"
    fdx_name    = "VehSpdCtrlActvSts"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class VehSpdIndcdUnit:
    de_name     = "VehSpdIndcd.VeSpdIndcdUnit"
    fdx_name    = "VehSpdIndcdUnit"
    fdx_groupid = 1229

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Kmph = 0
        Mph = 1
        UkwnUnit = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->511
class VehSpdIndcdVal:
    de_name     = "VehSpdIndcd.VehSpdIndcd"
    fdx_name    = "VehSpdIndcdVal"
    fdx_groupid = 1229

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 511
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: m/s,  Range:0->125, Resolution: (0.00391*x+0.0, raw is unsigned, 15 bits )
class VehSpdLgtSafe:
    de_name     = "VehSpdLgtSafe.VehSpdLgt"
    fdx_name    = "VehSpdLgtSafe"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 125
    scale  = 0.00391
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class VehSpdLgtSafeChks:
    de_name     = "VehSpdLgtSafe.VehSpdLgtChks"
    fdx_name    = "VehSpdLgtSafeChks"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class VehSpdLgtSafeCntr:
    de_name     = "VehSpdLgtSafe.VehSpdLgtCntr"
    fdx_name    = "VehSpdLgtSafeCntr"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class VehSpdLgtSafeQf:
    de_name     = "VehSpdLgtSafe.VehSpdLgtQf"
    fdx_name    = "VehSpdLgtSafeQf"
    fdx_groupid = 1368

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp1:
    de_name     = "VFCVectorIHU.Grp1"
    fdx_name    = "VFCVectorIHUGrp1"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp2:
    de_name     = "VFCVectorIHU.Grp2"
    fdx_name    = "VFCVectorIHUGrp2"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp3:
    de_name     = "VFCVectorIHU.Grp3"
    fdx_name    = "VFCVectorIHUGrp3"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinBlk:
    de_name     = "Vin.BlockNr"
    fdx_name    = "VinBlk"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn1:
    de_name     = "Vin.VINSignalPos1"
    fdx_name    = "VinPosn1"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn2:
    de_name     = "Vin.VINSignalPos2"
    fdx_name    = "VinPosn2"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn3:
    de_name     = "Vin.VINSignalPos3"
    fdx_name    = "VinPosn3"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn4:
    de_name     = "Vin.VINSignalPos4"
    fdx_name    = "VinPosn4"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn5:
    de_name     = "Vin.VINSignalPos5"
    fdx_name    = "VinPosn5"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn6:
    de_name     = "Vin.VINSignalPos6"
    fdx_name    = "VinPosn6"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn7:
    de_name     = "Vin.VINSignalPos7"
    fdx_name    = "VinPosn7"
    fdx_groupid = 1218

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Availability status
# 0= not available
# 1= available
class VisnAgWideSts:
    de_name     = "VisnAgWideSts"
    fdx_name    = "VisnAgWideSts"
    fdx_groupid = 1142

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NotAvl = 0
        Avl = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class VisnImgAgWideInUse:
    de_name     = "VisnImgAgWideInUse"
    fdx_name    = "VisnImgAgWideInUse"
    fdx_groupid = 1143

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoImg = 0
        Img1 = 1
        Img2 = 2
        Img3 = 3
        Img4 = 4
        Img5 = 5
        Img6 = 6
        Img7 = 7
        Img8 = 8
        Img9 = 9
        Img10 = 10
        Img11 = 11
        Img12 = 12
        Img13 = 13
        Img14 = 14
        Img15 = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Gallon 1 = UK gallons
# Gallon 2 = US gallons
class VolUnit:
    de_name     = "IndcnUnit.VolUnit"
    fdx_name    = "VolUnit"
    fdx_groupid = 1282

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        litre = 0
        gallon1 = 1
        gallon2 = 2
        UkwnUnit = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Unitless,  Range:0->4095
class WhlCircum:
    de_name     = "WhlCircum"
    fdx_name    = "WhlCircum"
    fdx_groupid = 1214

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 4095
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: rpm,  Range:-1638.4->1638.3, Resolution: (0.1*x+-1638.4, raw is unsigned, 0 bits )
class WhlMotSysSpdAct:
    de_name     = "WhlMotSysSpdAct"
    fdx_name    = "WhlMotSysSpdAct"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -1638.4
    max    = 1638.3
    scale  = 0.1
    offset = -1638.4
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: NewtonMeter,  Range:-8188->8192, Resolution: (4.0*x+-8188.0, raw is unsigned, 12 bits )
class WhlMotSysTqEst:
    de_name     = "WhlMotSysTqEst.TqAct"
    fdx_name    = "WhlMotSysTqEst"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -8188
    max    = 8192
    scale  = 4.0
    offset = -8188.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255
class WhlMotSysTqEstChks:
    de_name     = "WhlMotSysTqEst.TqActChks"
    fdx_name    = "WhlMotSysTqEstChks"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->15
class WhlMotSysTqEstCntr:
    de_name     = "WhlMotSysTqEst.TqActCntr"
    fdx_name    = "WhlMotSysTqEstCntr"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class WhlMotSysTqEstQlyFac:
    de_name     = "WhlMotSysTqEst.TqActQlyFac"
    fdx_name    = "WhlMotSysTqEstQlyFac"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        QfUndefd = 0
        QfInProgs = 1
        QfNotSpc = 2
        QfSnsrDataOk = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: rpm,  Range:0->2550, Resolution: (10.0*x+0.0, raw is unsigned, 8 bits )
class WhlMotSysTqEstSpdLimn:
    de_name     = "WhlMotSysTqEst.SpdLimn"
    fdx_name    = "WhlMotSysTqEstSpdLimn"
    fdx_groupid = 1370

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 2550
    scale  = 10.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrFrntLe:
    de_name     = "WhlRotToothCntr.WhlRotToothCntrFrntLe"
    fdx_name    = "WhlRotToothCntrFrntLe"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrFrntRi:
    de_name     = "WhlRotToothCntr.WhlRotToothCntrFrntRi"
    fdx_name    = "WhlRotToothCntrFrntRi"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrReLe:
    de_name     = "WhlRotToothCntr.WhlRotToothCntrReLe"
    fdx_name    = "WhlRotToothCntrReLe"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrReRi:
    de_name     = "WhlRotToothCntr.WhlRotToothCntrReRi"
    fdx_name    = "WhlRotToothCntrReRi"
    fdx_groupid = 1369

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class WinPosnStsAtDrvrRe:
    de_name     = "WinPosnStsAtDrvrRe"
    fdx_name    = "WinPosnStsAtDrvrRe"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PosnUkwn = 0
        ClsFull = 1
        PercOpen4 = 2
        PercOpen8 = 3
        PercOpen12 = 4
        PercOpen16 = 5
        PercOpen20 = 6
        PercOpen24 = 7
        PercOpen28 = 8
        PercOpen32 = 9
        PercOpen36 = 10
        PercOpen40 = 11
        PercOpen44 = 12
        PercOpen48 = 13
        PercOpen52 = 14
        PercOpen56 = 15
        PercOpen60 = 16
        PercOpen64 = 17
        PercOpen68 = 18
        PercOpen72 = 19
        PercOpen76 = 20
        PercOpen80 = 21
        PercOpen84 = 22
        PercOpen88 = 23
        PercOpen92 = 24
        PercOpen96 = 25
        OpenFull = 26
        Resd1 = 27
        Resd2 = 28
        Resd3 = 29
        Resd4 = 30
        Movg = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WinPosnStsDrv:
    de_name     = "WinPosnStsAtDrv"
    fdx_name    = "WinPosnStsDrv"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PosnUkwn = 0
        ClsFull = 1
        PercOpen4 = 2
        PercOpen8 = 3
        PercOpen12 = 4
        PercOpen16 = 5
        PercOpen20 = 6
        PercOpen24 = 7
        PercOpen28 = 8
        PercOpen32 = 9
        PercOpen36 = 10
        PercOpen40 = 11
        PercOpen44 = 12
        PercOpen48 = 13
        PercOpen52 = 14
        PercOpen56 = 15
        PercOpen60 = 16
        PercOpen64 = 17
        PercOpen68 = 18
        PercOpen72 = 19
        PercOpen76 = 20
        PercOpen80 = 21
        PercOpen84 = 22
        PercOpen88 = 23
        PercOpen92 = 24
        PercOpen96 = 25
        OpenFull = 26
        Resd1 = 27
        Resd2 = 28
        Resd3 = 29
        Resd4 = 30
        Movg = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WinPosnStsPass:
    de_name     = "WinPosnStsAtPass"
    fdx_name    = "WinPosnStsPass"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PosnUkwn = 0
        ClsFull = 1
        PercOpen4 = 2
        PercOpen8 = 3
        PercOpen12 = 4
        PercOpen16 = 5
        PercOpen20 = 6
        PercOpen24 = 7
        PercOpen28 = 8
        PercOpen32 = 9
        PercOpen36 = 10
        PercOpen40 = 11
        PercOpen44 = 12
        PercOpen48 = 13
        PercOpen52 = 14
        PercOpen56 = 15
        PercOpen60 = 16
        PercOpen64 = 17
        PercOpen68 = 18
        PercOpen72 = 19
        PercOpen76 = 20
        PercOpen80 = 21
        PercOpen84 = 22
        PercOpen88 = 23
        PercOpen92 = 24
        PercOpen96 = 25
        OpenFull = 26
        Resd1 = 27
        Resd2 = 28
        Resd3 = 29
        Resd4 = 30
        Movg = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WinPosnStsRePass:
    de_name     = "WinPosnStsAtPassRe"
    fdx_name    = "WinPosnStsRePass"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        PosnUkwn = 0
        ClsFull = 1
        PercOpen4 = 2
        PercOpen8 = 3
        PercOpen12 = 4
        PercOpen16 = 5
        PercOpen20 = 6
        PercOpen24 = 7
        PercOpen28 = 8
        PercOpen32 = 9
        PercOpen36 = 10
        PercOpen40 = 11
        PercOpen44 = 12
        PercOpen48 = 13
        PercOpen52 = 14
        PercOpen56 = 15
        PercOpen60 = 16
        PercOpen64 = 17
        PercOpen68 = 18
        PercOpen72 = 19
        PercOpen76 = 20
        PercOpen80 = 21
        PercOpen84 = 22
        PercOpen88 = 23
        PercOpen92 = 24
        PercOpen96 = 25
        OpenFull = 26
        Resd1 = 27
        Resd2 = 28
        Resd3 = 29
        Resd4 = 30
        Movg = 31

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# The signal reflects the actual wiping speed or mode regardless of activation source. Both rain sensor and wiper stalk info are used to create the signal. When in interval mode the wipes are carried out in low speed.
class WipgInfoWipgSpdInfo:
    de_name     = "WipgInfo.WipgSpdInfo"
    fdx_name    = "WipgInfoWipgSpdInfo"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        IntlLo = 1
        IntlHi = 2
        WipgSpd4045 = 3
        WipgSpd4650 = 4
        WipgSpd5155 = 5
        WipgSpd5660 = 6
        WiprErr = 7

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WipgInfoWiprActv:
    de_name     = "WipgInfo.WiprActv"
    fdx_name    = "WipgInfoWiprActv"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WipgInfoWiprInWipgAr:
    de_name     = "WipgInfo.WiprInWipgAr"
    fdx_name    = "WipgInfoWiprInWipgAr"
    fdx_groupid = 1216

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WiprFrntSrvModReq:
    de_name     = "WiprFrntSrvModReq"
    fdx_name    = "WiprFrntSrvModReq"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        NoActn = 0
        ActvtSrvPosn = 1
        DeActvtSrvPosn = 2

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WiprInPosnForSrv:
    de_name     = "WiprInPosnForSrv"
    fdx_name    = "WiprInPosnForSrv"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class WiprReAutReqPen:
    de_name     = "WiprReAutReq.Pen"
    fdx_name    = "WiprReAutReqPen"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        ProfUkwn = 0
        Prof1 = 1
        Prof2 = 2
        Prof3 = 3
        Prof4 = 4
        Prof5 = 5
        Prof6 = 6
        Prof7 = 7
        Prof8 = 8
        Prof9 = 9
        Prof10 = 10
        Prof11 = 11
        Prof12 = 12
        Prof13 = 13
        Resd14 = 14
        ProfAll = 15

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WiprReAutReqSts:
    de_name     = "WiprReAutReq.Sts"
    fdx_name    = "WiprReAutReqSts"
    fdx_groupid = 1280

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
class WshrFldSts:
    de_name     = "WshrFldSts1WdElmHMI"
    fdx_name    = "WshrFldSts"
    fdx_groupid = 1219

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        Off = 0
        On = 1

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: rad/s,  Range:-6->6, Resolution: (0.000244140625*x+0.0, raw is signed, 16 bits )
class YawRate1:
    de_name     = "AgDataRawSafe.YawRate"
    fdx_name    = "YawRate1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min    = -6
    max    = 6
    scale  = 0.000244140625
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

class YawRate1Qf1:
    de_name     = "AgDataRawSafe.YawRateQf"
    fdx_name    = "YawRate1Qf1"
    fdx_groupid = 1353

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3

    def send(self, value):
        self.item.value_raw(value)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        return self.item.value_raw()
        
# Unit: Year,  Range:0->99
class Yr:
    de_name     = "TiAndDateIndcn.Yr1"
    fdx_name    = "Yr"
    fdx_groupid = 1217

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 99
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

# Unit: Year,  Range:0->99
class Yr1:
    de_name     = "SetTiAndDate.Year"
    fdx_name    = "Yr1"
    fdx_groupid = 1277

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
    min = 0
    max = 99
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical
    
    def send(self, value):
        self.item.value_raw(self.p2r(value))
        self.signal_interface.connection.send_data_exchange(self.item.parent_group, self.item.size, self.item.value_raw())

    def receive(self):
        value = self.r2p(self.item.value_raw())
        return value

