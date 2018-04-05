# coding=utf-8

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# Signal scaling database
# --- AUTO GENERATED ---
# Inputs: generate_signal_scaling.py
#    --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Swc.arxml
#    --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2610_IHUVOLVO43_170920_UnFlattened_Splitted__WithSparePNC_Com.arxml
#    --fdxdescriptionfile=../CANoe/SPA2610/FDXDescriptionFile.xml
#    --out=generated/pyDataElements.py

import os
import logging
import time
import threading
from fdx import fdx_client
from fdx import fdx_description_file_parser

# Dummy class used when no real FDX connection is used (debugging on host without any hardware)
class FDXDummyConnection:
    def send_data_exchange(self, a, b, c):
        pass
    def close(self):
        pass


ns_per_ms = 1000000
class FrSignalInterface:

    def __init__(self):

        self.connected = False
        self.logger = logging.getLogger(__name__)

        (self.groups, self.sysvar_list, self.signal_list) = fdx_description_file_parser.parse(
            os.path.join(os.path.dirname(__file__), "../../CANoe/SPA2610/FDXDescriptionFile.xml"))

        self.group_id_map = {g.group_id: g for g in self.groups}

        received_group_ids = set()

        def data_exchange(group_id, data):
            received_group_ids.add(group_id)
            group = self.group_id_map[group_id]
            group.receive_data(data)

        #TODO: Remove this, we don't want implicit dummy mode, it should be explicit
        if "VECTOR_FDX_IP" in os.environ:
            try:
                self.connection = fdx_client.FDXConnection(
                    data_exchange,
                    os.environ['VECTOR_FDX_IP'],
                    int(os.environ.get('VECTOR_FDX_PORT', '2809')))
                self.connection.confirmed_stop()    # Stop in case previous test failed to stop
                self.connection.confirmed_start()
                self.verify_simulation_version()
                groups_to_subscribe = [g for g in self.groups if "ihubackbone" in g.name.lower() or "ihulin19" in g.name.lower()]
                expected_group_ids = set([g.group_id for g in groups_to_subscribe])
                for g in groups_to_subscribe:
                    # Sleep is Super ugly, super important. Seems like the Vector some times miss some free_running_request, this fixes it.
                    time.sleep(0.3)
                    self.connection.send_free_running_request(g.group_id, fdx_client.kFreeRunningFlag.transmitCyclic, 50 * ns_per_ms, 0)

                # Extra check that free_running_request for all group ids is working, it has shown some reliability issues (see sleep above)
                deadline = time.time() + 3
                while True:
                    missing_group_ids = expected_group_ids - received_group_ids
                    if len(missing_group_ids) == 0:
                        break
                    time.sleep(0.05)
                    if time.time() < deadline:
                        continue
                    else:
                        raise Exception("Failed to subscribe to all groupids, missing: %r" % missing_group_ids)

                self.connected = True
            except:
                self.connection.close()
                raise
        else:
            self.connection = FDXDummyConnection()
            self.logger.error("Environment variables VECTOR_FDX_PORT and/or VECTOR_FDX_IP not found, no connection to target")

        name_to_item_map = { i.msg_or_namespace + '::' + i.name : i for i in self.signal_list }

        self.AbsWarnIndcnReq = AbsWarnIndcnReq(self, name_to_item_map['VDDMBackBoneSignalIpdu06::' +AbsWarnIndcnReq.fdx_name])
        self.AccAdprSpdLimActvPen = AccAdprSpdLimActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AccAdprSpdLimActvPen.fdx_name])
        self.AccAdprSpdLimActvSts = AccAdprSpdLimActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AccAdprSpdLimActvSts.fdx_name])
        self.AccAdprTurnSpdActvPen = AccAdprTurnSpdActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +AccAdprTurnSpdActvPen.fdx_name])
        self.AccAdprTurnSpdActvSts = AccAdprTurnSpdActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +AccAdprTurnSpdActvSts.fdx_name])
        self.AccAutResuWarnReq = AccAutResuWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +AccAutResuWarnReq.fdx_name])
        self.AccSts = AccSts(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +AccSts.fdx_name])
        self.ActvnOfCrsEcoPen = ActvnOfCrsEcoPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ActvnOfCrsEcoPen.fdx_name])
        self.ActvnOfCrsEcoSts = ActvnOfCrsEcoSts(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ActvnOfCrsEcoSts.fdx_name])
        self.ActvnOfPrkgAssi = ActvnOfPrkgAssi(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +ActvnOfPrkgAssi.fdx_name])
        self.ActvnOfPrkgAut = ActvnOfPrkgAut(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +ActvnOfPrkgAut.fdx_name])
        self.ActvOfHorn = ActvOfHorn(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +ActvOfHorn.fdx_name])
        self.ActvtAutnmsPrkgCtrl = ActvtAutnmsPrkgCtrl(self, name_to_item_map['IHUBackboneSignalIPdu01::' +ActvtAutnmsPrkgCtrl.fdx_name])
        self.AdjSpdLimnSts = AdjSpdLimnSts(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +AdjSpdLimnSts.fdx_name])
        self.AdjSpdLimnWarn = AdjSpdLimnWarn(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +AdjSpdLimnWarn.fdx_name])
        self.AdprTurnSpdModPen = AdprTurnSpdModPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AdprTurnSpdModPen.fdx_name])
        self.AdprTurnSpdModSts = AdprTurnSpdModSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AdprTurnSpdModSts.fdx_name])
        self.AirClngReq = AirClngReq(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +AirClngReq.fdx_name])
        self.ALat1 = ALat1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +ALat1.fdx_name])
        self.ALat1Qf1 = ALat1Qf1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +ALat1Qf1.fdx_name])
        self.ALgt1 = ALgt1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +ALgt1.fdx_name])
        self.ALgt1Qf1 = ALgt1Qf1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +ALgt1Qf1.fdx_name])
        self.AmbTIndcd = AmbTIndcd(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +AmbTIndcd.fdx_name])
        self.AmbTIndcdQf = AmbTIndcdQf(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +AmbTIndcdQf.fdx_name])
        self.AmbTIndcdUnit = AmbTIndcdUnit(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +AmbTIndcdUnit.fdx_name])
        self.AmbTRawQly = AmbTRawQly(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +AmbTRawQly.fdx_name])
        self.AmbTRawVal = AmbTRawVal(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +AmbTRawVal.fdx_name])
        self.AntithftRednReq = AntithftRednReq(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +AntithftRednReq.fdx_name])
        self.AsyFctDeactvnWarn = AsyFctDeactvnWarn(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +AsyFctDeactvnWarn.fdx_name])
        self.AsyLaneChgPsbl = AsyLaneChgPsbl(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +AsyLaneChgPsbl.fdx_name])
        self.AsyLaneChgSts = AsyLaneChgSts(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +AsyLaneChgSts.fdx_name])
        self.AsyLaneChgTyp = AsyLaneChgTyp(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +AsyLaneChgTyp.fdx_name])
        self.AsySteerApplyRqrd = AsySteerApplyRqrd(self, name_to_item_map['ASDMBackBoneSignalIPdu02::' +AsySteerApplyRqrd.fdx_name])
        self.AsySteerFctDeactvdWarn = AsySteerFctDeactvdWarn(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +AsySteerFctDeactvdWarn.fdx_name])
        self.AudMsgReq = AudMsgReq(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +AudMsgReq.fdx_name])
        self.AudWarn = AudWarn(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +AudWarn.fdx_name])
        self.AudWarnActv = AudWarnActv(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +AudWarnActv.fdx_name])
        self.AutnmsPrkgActvn = AutnmsPrkgActvn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AutnmsPrkgActvn.fdx_name])
        self.AutnmsPrkgSeldScenarioAutnmsPrkgSeldDirectPrkg = AutnmsPrkgSeldScenarioAutnmsPrkgSeldDirectPrkg(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AutnmsPrkgSeldScenarioAutnmsPrkgSeldDirectPrkg.fdx_name])
        self.AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgDir = AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgDir(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgDir.fdx_name])
        self.AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgScenario = AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgScenario(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgScenario.fdx_name])
        self.AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgSide = AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgSide(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgSide.fdx_name])
        self.AutnmsPrkgSeldScenarioAutnmsPrkgSeldRem = AutnmsPrkgSeldScenarioAutnmsPrkgSeldRem(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AutnmsPrkgSeldScenarioAutnmsPrkgSeldRem.fdx_name])
        self.AutnmsPrkgSeldScenarioAutnmsPrkgSeldSlotTyp = AutnmsPrkgSeldScenarioAutnmsPrkgSeldSlotTyp(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +AutnmsPrkgSeldScenarioAutnmsPrkgSeldSlotTyp.fdx_name])
        self.AVert2 = AVert2(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +AVert2.fdx_name])
        self.AVert2Qf1 = AVert2Qf1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +AVert2Qf1.fdx_name])
        self.AvlStsForLatAutDrv = AvlStsForLatAutDrv(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +AvlStsForLatAutDrv.fdx_name])
        self.BackCntrForMissCom = BackCntrForMissCom(self, name_to_item_map['VCMBackboneSignalIPdu00::' +BackCntrForMissCom.fdx_name])
        self.BarForFuEco = BarForFuEco(self, name_to_item_map['VDDMBackBoneSignalIPdu04::' +BarForFuEco.fdx_name])
        self.BkpOfDstTrvld = BkpOfDstTrvld(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +BkpOfDstTrvld.fdx_name])
        self.BlisSwOnOff = BlisSwOnOff(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +BlisSwOnOff.fdx_name])
        self.BltLockStAtDrvrForBltLockSt1 = BltLockStAtDrvrForBltLockSt1(self, name_to_item_map['SRSBackboneSignalIPdu01::' +BltLockStAtDrvrForBltLockSt1.fdx_name])
        self.BltLockStAtDrvrForDevErrSts2 = BltLockStAtDrvrForDevErrSts2(self, name_to_item_map['SRSBackboneSignalIPdu01::' +BltLockStAtDrvrForDevErrSts2.fdx_name])
        self.BltLockStAtPassForBltLockSt1 = BltLockStAtPassForBltLockSt1(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtPassForBltLockSt1.fdx_name])
        self.BltLockStAtPassForBltLockSts = BltLockStAtPassForBltLockSts(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtPassForBltLockSts.fdx_name])
        self.BltLockStAtRowSecLeForBltLockEquid = BltLockStAtRowSecLeForBltLockEquid(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecLeForBltLockEquid.fdx_name])
        self.BltLockStAtRowSecLeForBltLockSt1 = BltLockStAtRowSecLeForBltLockSt1(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecLeForBltLockSt1.fdx_name])
        self.BltLockStAtRowSecLeForBltLockSts = BltLockStAtRowSecLeForBltLockSts(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecLeForBltLockSts.fdx_name])
        self.BltLockStAtRowSecMidForBltLockEquid = BltLockStAtRowSecMidForBltLockEquid(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecMidForBltLockEquid.fdx_name])
        self.BltLockStAtRowSecMidForBltLockSt1 = BltLockStAtRowSecMidForBltLockSt1(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecMidForBltLockSt1.fdx_name])
        self.BltLockStAtRowSecMidForBltLockSts = BltLockStAtRowSecMidForBltLockSts(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecMidForBltLockSts.fdx_name])
        self.BltLockStAtRowSecRiForBltLockEquid = BltLockStAtRowSecRiForBltLockEquid(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecRiForBltLockEquid.fdx_name])
        self.BltLockStAtRowSecRiForBltLockSt1 = BltLockStAtRowSecRiForBltLockSt1(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecRiForBltLockSt1.fdx_name])
        self.BltLockStAtRowSecRiForBltLockSts = BltLockStAtRowSecRiForBltLockSts(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowSecRiForBltLockSts.fdx_name])
        self.BltLockStAtRowThrdLeForBltLockEquid = BltLockStAtRowThrdLeForBltLockEquid(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowThrdLeForBltLockEquid.fdx_name])
        self.BltLockStAtRowThrdLeForBltLockSt1 = BltLockStAtRowThrdLeForBltLockSt1(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowThrdLeForBltLockSt1.fdx_name])
        self.BltLockStAtRowThrdLeForBltLockSts = BltLockStAtRowThrdLeForBltLockSts(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowThrdLeForBltLockSts.fdx_name])
        self.BltLockStAtRowThrdRiForBltLockEquid = BltLockStAtRowThrdRiForBltLockEquid(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowThrdRiForBltLockEquid.fdx_name])
        self.BltLockStAtRowThrdRiForBltLockSt1 = BltLockStAtRowThrdRiForBltLockSt1(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowThrdRiForBltLockSt1.fdx_name])
        self.BltLockStAtRowThrdRiForBltLockSts = BltLockStAtRowThrdRiForBltLockSts(self, name_to_item_map['SRSBackboneSignalIPdu02::' +BltLockStAtRowThrdRiForBltLockSts.fdx_name])
        self.BltRmnSound1 = BltRmnSound1(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +BltRmnSound1.fdx_name])
        self.BltRmnSound2 = BltRmnSound2(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +BltRmnSound2.fdx_name])
        self.BltRmnSound3 = BltRmnSound3(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +BltRmnSound3.fdx_name])
        self.BltRmnSound4 = BltRmnSound4(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +BltRmnSound4.fdx_name])
        self.BrdgInHznSeg = BrdgInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +BrdgInHznSeg.fdx_name])
        self.BrkFldLvl = BrkFldLvl(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +BrkFldLvl.fdx_name])
        self.BrkFricTqAtWhlFrntLeAct = BrkFricTqAtWhlFrntLeAct(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +BrkFricTqAtWhlFrntLeAct.fdx_name])
        self.BrkFricTqAtWhlFrntRiAct = BrkFricTqAtWhlFrntRiAct(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +BrkFricTqAtWhlFrntRiAct.fdx_name])
        self.BrkFricTqAtWhlReLeAct = BrkFricTqAtWhlReLeAct(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +BrkFricTqAtWhlReLeAct.fdx_name])
        self.BrkFricTqAtWhlReRiAct = BrkFricTqAtWhlReRiAct(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +BrkFricTqAtWhlReRiAct.fdx_name])
        self.BrkRelsWarnReq = BrkRelsWarnReq(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +BrkRelsWarnReq.fdx_name])
        self.BrkWarnIndcnReq = BrkWarnIndcnReq(self, name_to_item_map['VDDMBackBoneSignalIpdu06::' +BrkWarnIndcnReq.fdx_name])
        self.BtnAudFbSts = BtnAudFbSts(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +BtnAudFbSts.fdx_name])
        self.BtnAudVolSts = BtnAudVolSts(self, name_to_item_map['DIMBackBoneSignalIPdu02::' +BtnAudVolSts.fdx_name])
        self.BtnMmedLeRiSts = BtnMmedLeRiSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +BtnMmedLeRiSts.fdx_name])
        self.BtnMmedModSts = BtnMmedModSts(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +BtnMmedModSts.fdx_name])
        self.BtnMmedSetSts = BtnMmedSetSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +BtnMmedSetSts.fdx_name])
        self.CallStsIndcn = CallStsIndcn(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +CallStsIndcn.fdx_name])
        self.CarModSts1 = CarModSts1(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +CarModSts1.fdx_name])
        self.CarModSubtypWdCarModSubtyp = CarModSubtypWdCarModSubtyp(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +CarModSubtypWdCarModSubtyp.fdx_name])
        self.CarTiGlb = CarTiGlb(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +CarTiGlb.fdx_name])
        self.ChdLockgProtnStsToHmi = ChdLockgProtnStsToHmi(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ChdLockgProtnStsToHmi.fdx_name])
        self.ChdTxVersInHznData = ChdTxVersInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +ChdTxVersInHznData.fdx_name])
        self.ChdWinProtnStsAtDrvrRe = ChdWinProtnStsAtDrvrRe(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +ChdWinProtnStsAtDrvrRe.fdx_name])
        self.ChdWinProtnStsAtPassRe = ChdWinProtnStsAtPassRe(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +ChdWinProtnStsAtPassRe.fdx_name])
        self.ClimaActv = ClimaActv(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ClimaActv.fdx_name])
        self.ClimaEcoModRqrd = ClimaEcoModRqrd(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +ClimaEcoModRqrd.fdx_name])
        self.ClimaPwrCns = ClimaPwrCns(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +ClimaPwrCns.fdx_name])
        self.ClimaRqrd = ClimaRqrd(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ClimaRqrd.fdx_name])
        self.ClimaTmr = ClimaTmr(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +ClimaTmr.fdx_name])
        self.ClimaTmrSts = ClimaTmrSts(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +ClimaTmrSts.fdx_name])
        self.ClimaTmrStsRqrd = ClimaTmrStsRqrd(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +ClimaTmrStsRqrd.fdx_name])
        self.ClimaWarn = ClimaWarn(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +ClimaWarn.fdx_name])
        self.ClkFmt = ClkFmt(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ClkFmt.fdx_name])
        self.CllsnAidSnvtySeldPen = CllsnAidSnvtySeldPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +CllsnAidSnvtySeldPen.fdx_name])
        self.CllsnAidSnvtySeldSts = CllsnAidSnvtySeldSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +CllsnAidSnvtySeldSts.fdx_name])
        self.CllsnFwdWarnActvPen = CllsnFwdWarnActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +CllsnFwdWarnActvPen.fdx_name])
        self.CllsnFwdWarnActvSts = CllsnFwdWarnActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +CllsnFwdWarnActvSts.fdx_name])
        self.CllsnFwdWarnReq = CllsnFwdWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu01::' +CllsnFwdWarnReq.fdx_name])
        self.CllsnWarnReq = CllsnWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu01::' +CllsnWarnReq.fdx_name])
        self.CllsnWarnSideLe = CllsnWarnSideLe(self, name_to_item_map['ASDMBackBoneSignalIPdu01::' +CllsnWarnSideLe.fdx_name])
        self.CllsnWarnSideRi = CllsnWarnSideRi(self, name_to_item_map['ASDMBackBoneSignalIPdu01::' +CllsnWarnSideRi.fdx_name])
        self.ClngActv = ClngActv(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +ClngActv.fdx_name])
        self.ClngRqrdFromHmi = ClngRqrdFromHmi(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +ClngRqrdFromHmi.fdx_name])
        self.ClsAutEna = ClsAutEna(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +ClsAutEna.fdx_name])
        self.CmftFctActv = CmftFctActv(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +CmftFctActv.fdx_name])
        self.CmftFctSts = CmftFctSts(self, name_to_item_map['VCMBackboneSignalIPdu00::' +CmftFctSts.fdx_name])
        self.CmptmtAirTEstimdExtdComptmtT = CmptmtAirTEstimdExtdComptmtT(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +CmptmtAirTEstimdExtdComptmtT.fdx_name])
        self.CmptmtAirTEstimdExtdQlyFlg = CmptmtAirTEstimdExtdQlyFlg(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +CmptmtAirTEstimdExtdQlyFlg.fdx_name])
        self.CmptmtTFrnt = CmptmtTFrnt(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +CmptmtTFrnt.fdx_name])
        self.CmptmtTFrntQf = CmptmtTFrntQf(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +CmptmtTFrntQf.fdx_name])
        self.CnclWarnForCrsCtrl = CnclWarnForCrsCtrl(self, name_to_item_map['VDDMBackBoneSignalIPdu13::' +CnclWarnForCrsCtrl.fdx_name])
        self.CnclWarnLatForAutDrv = CnclWarnLatForAutDrv(self, name_to_item_map['ASDMBackBoneSignalIPdu01::' +CnclWarnLatForAutDrv.fdx_name])
        self.CnclWarnLgtForAutDrv = CnclWarnLgtForAutDrv(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +CnclWarnLgtForAutDrv.fdx_name])
        self.CntDwnToManvStrtInDstToManvLocn = CntDwnToManvStrtInDstToManvLocn(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +CntDwnToManvStrtInDstToManvLocn.fdx_name])
        self.ComLostExtrSound = ComLostExtrSound(self, name_to_item_map['VDDMBackBoneSignalIpdu08::' +ComLostExtrSound.fdx_name])
        self.ConSftyDataSharingAllwd = ConSftyDataSharingAllwd(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +ConSftyDataSharingAllwd.fdx_name])
        self.ConSftyDataSharingSts = ConSftyDataSharingSts(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +ConSftyDataSharingSts.fdx_name])
        self.ConSftyWarnConSftyWarnId = ConSftyWarnConSftyWarnId(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +ConSftyWarnConSftyWarnId.fdx_name])
        self.ConSftyWarnConSftyWarnLvl = ConSftyWarnConSftyWarnLvl(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +ConSftyWarnConSftyWarnLvl.fdx_name])
        self.ConSftyWarnConSftyWarnSnd = ConSftyWarnConSftyWarnSnd(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +ConSftyWarnConSftyWarnSnd.fdx_name])
        self.ConSftyWarnConSftyWarnTyp = ConSftyWarnConSftyWarnTyp(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +ConSftyWarnConSftyWarnTyp.fdx_name])
        self.ConSftyWarnDistanceToWarning = ConSftyWarnDistanceToWarning(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +ConSftyWarnDistanceToWarning.fdx_name])
        self.CoolgReqForDispCen = CoolgReqForDispCen(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +CoolgReqForDispCen.fdx_name])
        self.CoolgStsForDisp = CoolgStsForDisp(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +CoolgStsForDisp.fdx_name])
        self.CrsCtrlrSts = CrsCtrlrSts(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +CrsCtrlrSts.fdx_name])
        self.CrvtSpdAdpvSts = CrvtSpdAdpvSts(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +CrvtSpdAdpvSts.fdx_name])
        self.CrvtSpdWarnReq = CrvtSpdWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +CrvtSpdWarnReq.fdx_name])
        self.CrvtSpdWarnSts = CrvtSpdWarnSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +CrvtSpdWarnSts.fdx_name])
        self.CtraIndcnLe = CtraIndcnLe(self, name_to_item_map['ASDMBackBoneSignalIPdu02::' +CtraIndcnLe.fdx_name])
        self.CtraIndcnRi = CtraIndcnRi(self, name_to_item_map['ASDMBackBoneSignalIPdu02::' +CtraIndcnRi.fdx_name])
        self.CtraOn1 = CtraOn1(self, name_to_item_map['ASDMBackBoneSignalIPdu01::' +CtraOn1.fdx_name])
        self.CtraSwOnOff = CtraSwOnOff(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +CtraSwOnOff.fdx_name])
        self.CtryCodInHznData = CtryCodInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +CtryCodInHznData.fdx_name])
        self.CurtActvnReReq = CurtActvnReReq(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +CurtActvnReReq.fdx_name])
        self.CycCntrInHznData = CycCntrInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +CycCntrInHznData.fdx_name])
        self.CycCntrInHznEdge = CycCntrInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +CycCntrInHznEdge.fdx_name])
        self.CycCntrInHznPosn = CycCntrInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +CycCntrInHznPosn.fdx_name])
        self.CycCntrInHznProfLong = CycCntrInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +CycCntrInHznProfLong.fdx_name])
        self.CycCntrInHznProfSho = CycCntrInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +CycCntrInHznProfSho.fdx_name])
        self.CycCntrInHznSeg = CycCntrInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +CycCntrInHznSeg.fdx_name])
        self.DataDistbnAllwdForEgyPred = DataDistbnAllwdForEgyPred(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +DataDistbnAllwdForEgyPred.fdx_name])
        self.DataDistbnReqForEgyPred = DataDistbnReqForEgyPred(self, name_to_item_map['VDDMBackBoneSignalIPdu16::' +DataDistbnReqForEgyPred.fdx_name])
        self.DateOrTi = DateOrTi(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +DateOrTi.fdx_name])
        self.Day = Day(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +Day.fdx_name])
        self.Day1 = Day1(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +Day1.fdx_name])
        self.DayToSrv = DayToSrv(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +DayToSrv.fdx_name])
        self.DeactvtLvlgCtrl = DeactvtLvlgCtrl(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +DeactvtLvlgCtrl.fdx_name])
        self.DestSrcAndCfmRqrd = DestSrcAndCfmRqrd(self, name_to_item_map['VDDMBackBoneSignalIPdu16::' +DestSrcAndCfmRqrd.fdx_name])
        self.DispAndHomeBtnSts = DispAndHomeBtnSts(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +DispAndHomeBtnSts.fdx_name])
        self.DispBattEgyIn = DispBattEgyIn(self, name_to_item_map['VDDMBackBoneSignalIpdu08::' +DispBattEgyIn.fdx_name])
        self.DispBattEgyOut = DispBattEgyOut(self, name_to_item_map['VDDMBackBoneSignalIpdu08::' +DispBattEgyOut.fdx_name])
        self.DispOfPrpsnMod = DispOfPrpsnMod(self, name_to_item_map['VDDMBackBoneSignalIpdu05::' +DispOfPrpsnMod.fdx_name])
        self.DispStsCen = DispStsCen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +DispStsCen.fdx_name])
        self.DoorDrvrLockReSts = DoorDrvrLockReSts(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +DoorDrvrLockReSts.fdx_name])
        self.DoorDrvrLockSts = DoorDrvrLockSts(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +DoorDrvrLockSts.fdx_name])
        self.DoorDrvrReSts = DoorDrvrReSts(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +DoorDrvrReSts.fdx_name])
        self.DoorDrvrSts = DoorDrvrSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +DoorDrvrSts.fdx_name])
        self.DoorPassLockReSts = DoorPassLockReSts(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +DoorPassLockReSts.fdx_name])
        self.DoorPassLockSts = DoorPassLockSts(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +DoorPassLockSts.fdx_name])
        self.DoorPassReSts = DoorPassReSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +DoorPassReSts.fdx_name])
        self.DoorPassSts = DoorPassSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +DoorPassSts.fdx_name])
        self.DriveAwayInfoActvReq = DriveAwayInfoActvReq(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +DriveAwayInfoActvReq.fdx_name])
        self.DriveAwayInfoSoundWarnActvReq = DriveAwayInfoSoundWarnActvReq(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +DriveAwayInfoSoundWarnActvReq.fdx_name])
        self.DriveAwayInfoWarnReqAudWarnReq = DriveAwayInfoWarnReqAudWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +DriveAwayInfoWarnReqAudWarnReq.fdx_name])
        self.DriveAwayInfoWarnReqCtrlDirOfTrfcLi = DriveAwayInfoWarnReqCtrlDirOfTrfcLi(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +DriveAwayInfoWarnReqCtrlDirOfTrfcLi.fdx_name])
        self.DriveAwayInfoWarnReqInterruptionSrc = DriveAwayInfoWarnReqInterruptionSrc(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +DriveAwayInfoWarnReqInterruptionSrc.fdx_name])
        self.DriveAwayInfoWarnReqReqSrc = DriveAwayInfoWarnReqReqSrc(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +DriveAwayInfoWarnReqReqSrc.fdx_name])
        self.DriveAwayInfoWarnReqTiToDrvCntDwnTi = DriveAwayInfoWarnReqTiToDrvCntDwnTi(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +DriveAwayInfoWarnReqTiToDrvCntDwnTi.fdx_name])
        self.DriveAwayInfoWarnReqVisWarnReq = DriveAwayInfoWarnReqVisWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +DriveAwayInfoWarnReqVisWarnReq.fdx_name])
        self.DrvgInWrgDirOfTrfc = DrvgInWrgDirOfTrfc(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +DrvgInWrgDirOfTrfc.fdx_name])
        self.DrvgSideInHznData = DrvgSideInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +DrvgSideInHznData.fdx_name])
        self.DrvModDispd = DrvModDispd(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +DrvModDispd.fdx_name])
        self.DrvModReq = DrvModReq(self, name_to_item_map['IHUBackboneSignalIPdu01::' +DrvModReq.fdx_name])
        self.DrvrCtrlOfPassSeatFrntReqd = DrvrCtrlOfPassSeatFrntReqd(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +DrvrCtrlOfPassSeatFrntReqd.fdx_name])
        self.DrvrCtrlOfPassSeatFrntSts = DrvrCtrlOfPassSeatFrntSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +DrvrCtrlOfPassSeatFrntSts.fdx_name])
        self.DrvrDesDir = DrvrDesDir(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +DrvrDesDir.fdx_name])
        self.DrvrDispSetgPen = DrvrDispSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +DrvrDispSetgPen.fdx_name])
        self.DrvrDispSetgSts = DrvrDispSetgSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +DrvrDispSetgSts.fdx_name])
        self.DrvrEntryLoReq = DrvrEntryLoReq(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +DrvrEntryLoReq.fdx_name])
        self.DrvrHmiBackGndForHmiCen = DrvrHmiBackGndForHmiCen(self, name_to_item_map['IHUBackBoneSignalIPdu11::' +DrvrHmiBackGndForHmiCen.fdx_name])
        self.DrvrHmiBackGndInfoSetgPen = DrvrHmiBackGndInfoSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +DrvrHmiBackGndInfoSetgPen.fdx_name])
        self.DrvrHmiBackGndInfoSetgSetg = DrvrHmiBackGndInfoSetgSetg(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +DrvrHmiBackGndInfoSetgSetg.fdx_name])
        self.DrvrHmiDispdModPen = DrvrHmiDispdModPen(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +DrvrHmiDispdModPen.fdx_name])
        self.DrvrHmiDispdModSts = DrvrHmiDispdModSts(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +DrvrHmiDispdModSts.fdx_name])
        self.DrvrHmiForHmiCen = DrvrHmiForHmiCen(self, name_to_item_map['IHUBackBoneSignalIPdu09::' +DrvrHmiForHmiCen.fdx_name])
        self.DrvrHmiSpdLimAdpnSts = DrvrHmiSpdLimAdpnSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +DrvrHmiSpdLimAdpnSts.fdx_name])
        self.DrvrHmiUsrIfSetgPen = DrvrHmiUsrIfSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +DrvrHmiUsrIfSetgPen.fdx_name])
        self.DrvrHmiUsrIfSetgSetg = DrvrHmiUsrIfSetgSetg(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +DrvrHmiUsrIfSetgSetg.fdx_name])
        self.DrvrMassgRunng = DrvrMassgRunng(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +DrvrMassgRunng.fdx_name])
        self.DrvrPfmncLvl = DrvrPfmncLvl(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +DrvrPfmncLvl.fdx_name])
        self.DrvrPfmncMonActvPen = DrvrPfmncMonActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +DrvrPfmncMonActvPen.fdx_name])
        self.DrvrPfmncMonActvSts = DrvrPfmncMonActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +DrvrPfmncMonActvSts.fdx_name])
        self.DrvrPfmncSts = DrvrPfmncSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +DrvrPfmncSts.fdx_name])
        self.DrvrPfmncWarnReq = DrvrPfmncWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +DrvrPfmncWarnReq.fdx_name])
        self.DrvrSeatActvSpplFct = DrvrSeatActvSpplFct(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +DrvrSeatActvSpplFct.fdx_name])
        self.DrvrSeatDispMassgFctMassgInten = DrvrSeatDispMassgFctMassgInten(self, name_to_item_map['IHUBackboneSignalIPdu01::' +DrvrSeatDispMassgFctMassgInten.fdx_name])
        self.DrvrSeatDispMassgFctMassgProg = DrvrSeatDispMassgFctMassgProg(self, name_to_item_map['IHUBackboneSignalIPdu01::' +DrvrSeatDispMassgFctMassgProg.fdx_name])
        self.DrvrSeatDispMassgFctMassgSpdLvl = DrvrSeatDispMassgFctMassgSpdLvl(self, name_to_item_map['IHUBackboneSignalIPdu01::' +DrvrSeatDispMassgFctMassgSpdLvl.fdx_name])
        self.DrvrSeatDispMassgFctOnOff = DrvrSeatDispMassgFctOnOff(self, name_to_item_map['IHUBackboneSignalIPdu01::' +DrvrSeatDispMassgFctOnOff.fdx_name])
        self.DrvrSeatDispSpplFct = DrvrSeatDispSpplFct(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +DrvrSeatDispSpplFct.fdx_name])
        self.DrvrSeatMassgFctMassgInten = DrvrSeatMassgFctMassgInten(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +DrvrSeatMassgFctMassgInten.fdx_name])
        self.DrvrSeatMassgFctMassgProg = DrvrSeatMassgFctMassgProg(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +DrvrSeatMassgFctMassgProg.fdx_name])
        self.DrvrSeatMassgFctMassgSpdLvl = DrvrSeatMassgFctMassgSpdLvl(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +DrvrSeatMassgFctMassgSpdLvl.fdx_name])
        self.DrvrSeatMassgFctOnOff = DrvrSeatMassgFctOnOff(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +DrvrSeatMassgFctOnOff.fdx_name])
        self.DrvrSeatSwtAdjmtOfSpplFctHozlSts = DrvrSeatSwtAdjmtOfSpplFctHozlSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +DrvrSeatSwtAdjmtOfSpplFctHozlSts.fdx_name])
        self.DrvrSeatSwtAdjmtOfSpplFctVertSts = DrvrSeatSwtAdjmtOfSpplFctVertSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +DrvrSeatSwtAdjmtOfSpplFctVertSts.fdx_name])
        self.DrvrSeatSwtHeiFrntSts = DrvrSeatSwtHeiFrntSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +DrvrSeatSwtHeiFrntSts.fdx_name])
        self.DrvrSeatSwtHeiSts = DrvrSeatSwtHeiSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +DrvrSeatSwtHeiSts.fdx_name])
        self.DrvrSeatSwtInclSts = DrvrSeatSwtInclSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +DrvrSeatSwtInclSts.fdx_name])
        self.DrvrSeatSwtSelnOfSpplFctSts = DrvrSeatSwtSelnOfSpplFctSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +DrvrSeatSwtSelnOfSpplFctSts.fdx_name])
        self.DrvrSeatSwtSldSts = DrvrSeatSwtSldSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +DrvrSeatSwtSldSts.fdx_name])
        self.DrvrWLoadLvl = DrvrWLoadLvl(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +DrvrWLoadLvl.fdx_name])
        self.Dst1InHznProfSho = Dst1InHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +Dst1InHznProfSho.fdx_name])
        self.DstEstimdToEmptyForDrvgElec = DstEstimdToEmptyForDrvgElec(self, name_to_item_map['VDDMBackBoneSignalIpdu08::' +DstEstimdToEmptyForDrvgElec.fdx_name])
        self.DstEstimdToEmptyForDrvgElecEco = DstEstimdToEmptyForDrvgElecEco(self, name_to_item_map['VDDMBackBoneSignalIPdu16::' +DstEstimdToEmptyForDrvgElecEco.fdx_name])
        self.DstLong = DstLong(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +DstLong.fdx_name])
        self.DstNotifActvPen = DstNotifActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +DstNotifActvPen.fdx_name])
        self.DstNotifActvSts = DstNotifActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +DstNotifActvSts.fdx_name])
        self.DstNotifSts = DstNotifSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +DstNotifSts.fdx_name])
        self.DstSho = DstSho(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +DstSho.fdx_name])
        self.DstToEmpty = DstToEmpty(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +DstToEmpty.fdx_name])
        self.DstToManvInDstToManvLocn = DstToManvInDstToManvLocn(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +DstToManvInDstToManvLocn.fdx_name])
        self.DstToSrv = DstToSrv(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +DstToSrv.fdx_name])
        self.DstTrvld1 = DstTrvld1(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +DstTrvld1.fdx_name])
        self.DstTrvld2 = DstTrvld2(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +DstTrvld2.fdx_name])
        self.DstTrvldHiResl = DstTrvldHiResl(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +DstTrvldHiResl.fdx_name])
        self.DstTrvldMst2 = DstTrvldMst2(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +DstTrvldMst2.fdx_name])
        self.DstUnit = DstUnit(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +DstUnit.fdx_name])
        self.EasyInOutDrvrSeatAdjmtPen = EasyInOutDrvrSeatAdjmtPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +EasyInOutDrvrSeatAdjmtPen.fdx_name])
        self.EasyInOutDrvrSeatAdjmtSts = EasyInOutDrvrSeatAdjmtSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +EasyInOutDrvrSeatAdjmtSts.fdx_name])
        self.EgyLvlElecMai = EgyLvlElecMai(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +EgyLvlElecMai.fdx_name])
        self.EgyLvlElecSubtyp = EgyLvlElecSubtyp(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +EgyLvlElecSubtyp.fdx_name])
        self.EgySave = EgySave(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +EgySave.fdx_name])
        self.EmgyAsscSts = EmgyAsscSts(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +EmgyAsscSts.fdx_name])
        self.EmgyVehWarnActv = EmgyVehWarnActv(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +EmgyVehWarnActv.fdx_name])
        self.EmgyVehWarnSts = EmgyVehWarnSts(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +EmgyVehWarnSts.fdx_name])
        self.EngCooltIndcnReq = EngCooltIndcnReq(self, name_to_item_map['VDDMBackBoneSignalIpdu06::' +EngCooltIndcnReq.fdx_name])
        self.EngCooltLvl = EngCooltLvl(self, name_to_item_map['VDDMBackBoneSignalIpdu06::' +EngCooltLvl.fdx_name])
        self.EngFuCns = EngFuCns(self, name_to_item_map['VDDMBackBoneSignalIpdu06::' +EngFuCns.fdx_name])
        self.EngFuCnsFild = EngFuCnsFild(self, name_to_item_map['VDDMBackBoneSignalIpdu09::' +EngFuCnsFild.fdx_name])
        self.EngHrToSrv = EngHrToSrv(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +EngHrToSrv.fdx_name])
        self.EngIdleEco1Pen = EngIdleEco1Pen(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +EngIdleEco1Pen.fdx_name])
        self.EngIdleEco1Sts = EngIdleEco1Sts(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +EngIdleEco1Sts.fdx_name])
        self.EngN = EngN(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +EngN.fdx_name])
        self.EngNSafeEngNGrdt = EngNSafeEngNGrdt(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +EngNSafeEngNGrdt.fdx_name])
        self.EngOilLvl = EngOilLvl(self, name_to_item_map['VDDMBackBoneSignalIpdu08::' +EngOilLvl.fdx_name])
        self.EngOilLvlSts = EngOilLvlSts(self, name_to_item_map['VDDMBackBoneSignalIpdu08::' +EngOilLvlSts.fdx_name])
        self.EngOilPWarn = EngOilPWarn(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +EngOilPWarn.fdx_name])
        self.EngSpdDispd = EngSpdDispd(self, name_to_item_map['VDDMBackBoneSignalIPdu04::' +EngSpdDispd.fdx_name])
        self.EngStrtStopDrvModIndSetgPen = EngStrtStopDrvModIndSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +EngStrtStopDrvModIndSetgPen.fdx_name])
        self.EngStrtStopDrvModIndSetgSts = EngStrtStopDrvModIndSetgSts(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +EngStrtStopDrvModIndSetgSts.fdx_name])
        self.EngStrtStopSetg = EngStrtStopSetg(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +EngStrtStopSetg.fdx_name])
        self.EpbApplyAutSetgPen = EpbApplyAutSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +EpbApplyAutSetgPen.fdx_name])
        self.EpbApplyAutSetgSts = EpbApplyAutSetgSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +EpbApplyAutSetgSts.fdx_name])
        self.EpbEngStallApplySetgPen = EpbEngStallApplySetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +EpbEngStallApplySetgPen.fdx_name])
        self.EpbEngStallApplySetgSts = EpbEngStallApplySetgSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +EpbEngStallApplySetgSts.fdx_name])
        self.EpbLampReq = EpbLampReq(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +EpbLampReq.fdx_name])
        self.EscSptModReqdByDrvr = EscSptModReqdByDrvr(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +EscSptModReqdByDrvr.fdx_name])
        self.EscSptModReqdByDrvrPen = EscSptModReqdByDrvrPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +EscSptModReqdByDrvrPen.fdx_name])
        self.EscSt = EscSt(self, name_to_item_map['VDDMBackBoneSignalIpdu05::' +EscSt.fdx_name])
        self.EscWarnIndcnReq = EscWarnIndcnReq(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +EscWarnIndcnReq.fdx_name])
        self.ExtrMirrFoldSetgPen = ExtrMirrFoldSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ExtrMirrFoldSetgPen.fdx_name])
        self.ExtrMirrFoldSetgSts = ExtrMirrFoldSetgSts(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ExtrMirrFoldSetgSts.fdx_name])
        self.ExtrMirrTiltSetg2IdPen = ExtrMirrTiltSetg2IdPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ExtrMirrTiltSetg2IdPen.fdx_name])
        self.ExtrMirrTiltSetg2MirrDrvr = ExtrMirrTiltSetg2MirrDrvr(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ExtrMirrTiltSetg2MirrDrvr.fdx_name])
        self.ExtrMirrTiltSetg2MirrPass = ExtrMirrTiltSetg2MirrPass(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +ExtrMirrTiltSetg2MirrPass.fdx_name])
        self.FanForCmptmtTRunng = FanForCmptmtTRunng(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +FanForCmptmtTRunng.fdx_name])
        self.FltEgyCnsWdSts = FltEgyCnsWdSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +FltEgyCnsWdSts.fdx_name])
        self.FltIndcrTurnLeFrnt = FltIndcrTurnLeFrnt(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +FltIndcrTurnLeFrnt.fdx_name])
        self.FltIndcrTurnLeRe = FltIndcrTurnLeRe(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +FltIndcrTurnLeRe.fdx_name])
        self.FltIndcrTurnRiFrnt = FltIndcrTurnRiFrnt(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +FltIndcrTurnRiFrnt.fdx_name])
        self.FltIndcrTurnRiRe = FltIndcrTurnRiRe(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +FltIndcrTurnRiRe.fdx_name])
        self.FltOfLiDaytiRunngRi = FltOfLiDaytiRunngRi(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +FltOfLiDaytiRunngRi.fdx_name])
        self.FRNetworkStatus = FRNetworkStatus(self, name_to_item_map['VCMBackboneSignalIPdu00::' +FRNetworkStatus.fdx_name])
        self.FrntAxleWarn = FrntAxleWarn(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +FrntAxleWarn.fdx_name])
        self.FrntWiprLvrReq2FrntWiprLvrCmd1 = FrntWiprLvrReq2FrntWiprLvrCmd1(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +FrntWiprLvrReq2FrntWiprLvrCmd1.fdx_name])
        self.FrntWiprLvrReq2FrntWiprLvrCrc = FrntWiprLvrReq2FrntWiprLvrCrc(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +FrntWiprLvrReq2FrntWiprLvrCrc.fdx_name])
        self.FrntWiprLvrReq2FrntWiprLvrQf = FrntWiprLvrReq2FrntWiprLvrQf(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +FrntWiprLvrReq2FrntWiprLvrQf.fdx_name])
        self.FuCnsUnit = FuCnsUnit(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +FuCnsUnit.fdx_name])
        self.FuHeatrActv = FuHeatrActv(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +FuHeatrActv.fdx_name])
        self.FuHeatrFuCns1 = FuHeatrFuCns1(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +FuHeatrFuCns1.fdx_name])
        self.FuHeatrFuCnsDurgCyc1 = FuHeatrFuCnsDurgCyc1(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +FuHeatrFuCnsDurgCyc1.fdx_name])
        self.FuLvlIndcdQly = FuLvlIndcdQly(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +FuLvlIndcdQly.fdx_name])
        self.FuLvlIndcdVal = FuLvlIndcdVal(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +FuLvlIndcdVal.fdx_name])
        self.FuLvlLoIndcn = FuLvlLoIndcn(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +FuLvlLoIndcn.fdx_name])
        self.FuLvlLoWarn = FuLvlLoWarn(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +FuLvlLoWarn.fdx_name])
        self.GearIndcn = GearIndcn(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +GearIndcn.fdx_name])
        self.GearShiftIndcn = GearShiftIndcn(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +GearShiftIndcn.fdx_name])
        self.GearTarIndcn = GearTarIndcn(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +GearTarIndcn.fdx_name])
        self.GrdForFuEco = GrdForFuEco(self, name_to_item_map['VDDMBackBoneSignalIPdu04::' +GrdForFuEco.fdx_name])
        self.HdHwAprvdWirelsAdrAprvdSts = HdHwAprvdWirelsAdrAprvdSts(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HdHwAprvdWirelsAdrAprvdSts.fdx_name])
        self.HdHwAprvdWirelsAdrHwOffsAdr1 = HdHwAprvdWirelsAdrHwOffsAdr1(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HdHwAprvdWirelsAdrHwOffsAdr1.fdx_name])
        self.HdHwAprvdWirelsAdrHwOffsAdr2 = HdHwAprvdWirelsAdrHwOffsAdr2(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HdHwAprvdWirelsAdrHwOffsAdr2.fdx_name])
        self.HdHwAprvdWirelsAdrHwOffsAdr3 = HdHwAprvdWirelsAdrHwOffsAdr3(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HdHwAprvdWirelsAdrHwOffsAdr3.fdx_name])
        self.HdHwAprvdWirelsAdrHwOffsAdr4 = HdHwAprvdWirelsAdrHwOffsAdr4(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HdHwAprvdWirelsAdrHwOffsAdr4.fdx_name])
        self.HdHwAprvdWirelsAdrHwOffsAdr5 = HdHwAprvdWirelsAdrHwOffsAdr5(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HdHwAprvdWirelsAdrHwOffsAdr5.fdx_name])
        self.HdHwAprvdWirelsAdrHwOffsAdr6 = HdHwAprvdWirelsAdrHwOffsAdr6(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HdHwAprvdWirelsAdrHwOffsAdr6.fdx_name])
        self.HdrestFoldReq2 = HdrestFoldReq2(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +HdrestFoldReq2.fdx_name])
        self.HdTxVersInHznData = HdTxVersInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +HdTxVersInHznData.fdx_name])
        self.HeatrDurgDrvgReqd = HeatrDurgDrvgReqd(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +HeatrDurgDrvgReqd.fdx_name])
        self.HeatrPreCdngTyp = HeatrPreCdngTyp(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HeatrPreCdngTyp.fdx_name])
        self.HiQlyInTireCircumCalByNav = HiQlyInTireCircumCalByNav(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +HiQlyInTireCircumCalByNav.fdx_name])
        self.HmiAudSts = HmiAudSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +HmiAudSts.fdx_name])
        self.HmiCenForDrvrHmi = HmiCenForDrvrHmi(self, name_to_item_map['DIMBackBoneSignalIPdu06::' +HmiCenForDrvrHmi.fdx_name])
        self.HmiCmptmtAirDistbnFrnt = HmiCmptmtAirDistbnFrnt(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +HmiCmptmtAirDistbnFrnt.fdx_name])
        self.HmiCmptmtCoolgReq = HmiCmptmtCoolgReq(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtCoolgReq.fdx_name])
        self.HmiCmptmtTSpForRowFirstLe = HmiCmptmtTSpForRowFirstLe(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtTSpForRowFirstLe.fdx_name])
        self.HmiCmptmtTSpForRowFirstRi = HmiCmptmtTSpForRowFirstRi(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtTSpForRowFirstRi.fdx_name])
        self.HmiCmptmtTSpForRowSecLe = HmiCmptmtTSpForRowSecLe(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtTSpForRowSecLe.fdx_name])
        self.HmiCmptmtTSpForRowSecRi = HmiCmptmtTSpForRowSecRi(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtTSpForRowSecRi.fdx_name])
        self.HmiCmptmtTSpSpclForRowFirstLe = HmiCmptmtTSpSpclForRowFirstLe(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtTSpSpclForRowFirstLe.fdx_name])
        self.HmiCmptmtTSpSpclForRowFirstRi = HmiCmptmtTSpSpclForRowFirstRi(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtTSpSpclForRowFirstRi.fdx_name])
        self.HmiCmptmtTSpSpclForRowSecLe = HmiCmptmtTSpSpclForRowSecLe(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtTSpSpclForRowSecLe.fdx_name])
        self.HmiCmptmtTSpSpclForRowSecRi = HmiCmptmtTSpSpclForRowSecRi(self, name_to_item_map['IHUBackboneSignalIPdu01::' +HmiCmptmtTSpSpclForRowSecRi.fdx_name])
        self.HmiDefrstElecForMirrReq = HmiDefrstElecForMirrReq(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiDefrstElecForMirrReq.fdx_name])
        self.HmiDefrstElecFrntReq = HmiDefrstElecFrntReq(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiDefrstElecFrntReq.fdx_name])
        self.HmiDefrstElecReReq = HmiDefrstElecReReq(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiDefrstElecReReq.fdx_name])
        self.HmiDefrstFrntSts = HmiDefrstFrntSts(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +HmiDefrstFrntSts.fdx_name])
        self.HmiDefrstMaxReq = HmiDefrstMaxReq(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HmiDefrstMaxReq.fdx_name])
        self.HmiDfrstReSts = HmiDfrstReSts(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +HmiDfrstReSts.fdx_name])
        self.HmiHvacFanLvlFrnt = HmiHvacFanLvlFrnt(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +HmiHvacFanLvlFrnt.fdx_name])
        self.HmiHvacFanLvlRe = HmiHvacFanLvlRe(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +HmiHvacFanLvlRe.fdx_name])
        self.HmiHvacRecircCmd = HmiHvacRecircCmd(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +HmiHvacRecircCmd.fdx_name])
        self.HmiHvacReCtrl = HmiHvacReCtrl(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +HmiHvacReCtrl.fdx_name])
        self.HmiMirrDefrstSts = HmiMirrDefrstSts(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +HmiMirrDefrstSts.fdx_name])
        self.HmiSeatClimaExtdHmiSeatVentnForRowSecLe = HmiSeatClimaExtdHmiSeatVentnForRowSecLe(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiSeatClimaExtdHmiSeatVentnForRowSecLe.fdx_name])
        self.HmiSeatClimaExtdHmiSeatVentnForRowSecRi = HmiSeatClimaExtdHmiSeatVentnForRowSecRi(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiSeatClimaExtdHmiSeatVentnForRowSecRi.fdx_name])
        self.HmiSeatHeatgForRowFirstLe = HmiSeatHeatgForRowFirstLe(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiSeatHeatgForRowFirstLe.fdx_name])
        self.HmiSeatHeatgForRowFirstRi = HmiSeatHeatgForRowFirstRi(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiSeatHeatgForRowFirstRi.fdx_name])
        self.HmiSeatHeatgForRowSecLe = HmiSeatHeatgForRowSecLe(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiSeatHeatgForRowSecLe.fdx_name])
        self.HmiSeatHeatgForRowSecRi = HmiSeatHeatgForRowSecRi(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiSeatHeatgForRowSecRi.fdx_name])
        self.HmiSeatVentnForRowFirstLe = HmiSeatVentnForRowFirstLe(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiSeatVentnForRowFirstLe.fdx_name])
        self.HmiSeatVentnForRowFirstRi = HmiSeatVentnForRowFirstRi(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +HmiSeatVentnForRowFirstRi.fdx_name])
        self.HoodSts = HoodSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +HoodSts.fdx_name])
        self.Hr = Hr(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +Hr.fdx_name])
        self.Hr1 = Hr1(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +Hr1.fdx_name])
        self.HudActvReqPen = HudActvReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +HudActvReqPen.fdx_name])
        self.HudActvReqSts = HudActvReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +HudActvReqSts.fdx_name])
        self.HudActvSts = HudActvSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +HudActvSts.fdx_name])
        self.HudAdjmtReq = HudAdjmtReq(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +HudAdjmtReq.fdx_name])
        self.HudDiagcHudCircShoSts = HudDiagcHudCircShoSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +HudDiagcHudCircShoSts.fdx_name])
        self.HudDiagcHudCricOpenSts = HudDiagcHudCricOpenSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +HudDiagcHudCricOpenSts.fdx_name])
        self.HudDiagcHudTSts = HudDiagcHudTSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +HudDiagcHudTSts.fdx_name])
        self.HudDiagcImgHudErrSts = HudDiagcImgHudErrSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +HudDiagcImgHudErrSts.fdx_name])
        self.HudDiagcImgHudTmpNotAvlSts = HudDiagcImgHudTmpNotAvlSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +HudDiagcImgHudTmpNotAvlSts.fdx_name])
        self.HudErgoSetgReq = HudErgoSetgReq(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +HudErgoSetgReq.fdx_name])
        self.HudSts = HudSts(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +HudSts.fdx_name])
        self.HudVisFctSetgHudFct00 = HudVisFctSetgHudFct00(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct00.fdx_name])
        self.HudVisFctSetgHudFct01 = HudVisFctSetgHudFct01(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct01.fdx_name])
        self.HudVisFctSetgHudFct02 = HudVisFctSetgHudFct02(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct02.fdx_name])
        self.HudVisFctSetgHudFct03 = HudVisFctSetgHudFct03(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct03.fdx_name])
        self.HudVisFctSetgHudFct04 = HudVisFctSetgHudFct04(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct04.fdx_name])
        self.HudVisFctSetgHudFct05 = HudVisFctSetgHudFct05(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct05.fdx_name])
        self.HudVisFctSetgHudFct06 = HudVisFctSetgHudFct06(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct06.fdx_name])
        self.HudVisFctSetgHudFct07 = HudVisFctSetgHudFct07(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct07.fdx_name])
        self.HudVisFctSetgHudFct08 = HudVisFctSetgHudFct08(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct08.fdx_name])
        self.HudVisFctSetgHudFct09 = HudVisFctSetgHudFct09(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct09.fdx_name])
        self.HudVisFctSetgHudFct10 = HudVisFctSetgHudFct10(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct10.fdx_name])
        self.HudVisFctSetgHudFct11 = HudVisFctSetgHudFct11(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct11.fdx_name])
        self.HudVisFctSetgHudFct12 = HudVisFctSetgHudFct12(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct12.fdx_name])
        self.HudVisFctSetgHudFct13 = HudVisFctSetgHudFct13(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct13.fdx_name])
        self.HudVisFctSetgHudFct14 = HudVisFctSetgHudFct14(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct14.fdx_name])
        self.HudVisFctSetgHudFct15 = HudVisFctSetgHudFct15(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct15.fdx_name])
        self.HudVisFctSetgHudFct16 = HudVisFctSetgHudFct16(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct16.fdx_name])
        self.HudVisFctSetgHudFct17 = HudVisFctSetgHudFct17(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct17.fdx_name])
        self.HudVisFctSetgHudFct18 = HudVisFctSetgHudFct18(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct18.fdx_name])
        self.HudVisFctSetgHudFct19 = HudVisFctSetgHudFct19(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgHudFct19.fdx_name])
        self.HudVisFctSetgPen = HudVisFctSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +HudVisFctSetgPen.fdx_name])
        self.HvacAirMFlowEstimd = HvacAirMFlowEstimd(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +HvacAirMFlowEstimd.fdx_name])
        self.HvBattSmtActv = HvBattSmtActv(self, name_to_item_map['VDDMBackBoneSignalIpdu05::' +HvBattSmtActv.fdx_name])
        self.HvBattSmtSeld = HvBattSmtSeld(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +HvBattSmtSeld.fdx_name])
        self.HwVersInHznData = HwVersInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +HwVersInHznData.fdx_name])
        self.HznPosnExtdLanePrsnt = HznPosnExtdLanePrsnt(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdLanePrsnt.fdx_name])
        self.HznPosnExtdMsgTyp = HznPosnExtdMsgTyp(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdMsgTyp.fdx_name])
        self.HznPosnExtdOffsOffs = HznPosnExtdOffsOffs(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdOffsOffs.fdx_name])
        self.HznPosnExtdPahIdx = HznPosnExtdPahIdx(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdPahIdx.fdx_name])
        self.HznPosnExtdPosnIdx = HznPosnExtdPosnIdx(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdPosnIdx.fdx_name])
        self.HznPosnExtdPosnProblty = HznPosnExtdPosnProblty(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdPosnProblty.fdx_name])
        self.HznPosnExtdPosnQly = HznPosnExtdPosnQly(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdPosnQly.fdx_name])
        self.HznPosnExtdPosnTiDif = HznPosnExtdPosnTiDif(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdPosnTiDif.fdx_name])
        self.HznPosnExtdRelDir = HznPosnExtdRelDir(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdRelDir.fdx_name])
        self.HznPosnExtdSpd = HznPosnExtdSpd(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznPosnExtdSpd.fdx_name])
        self.HznProfLongExtdMsgTyp = HznProfLongExtdMsgTyp(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznProfLongExtdMsgTyp.fdx_name])
        self.HznProfLongExtdNodCtrl = HznProfLongExtdNodCtrl(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznProfLongExtdNodCtrl.fdx_name])
        self.HznProfLongExtdOffs = HznProfLongExtdOffs(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznProfLongExtdOffs.fdx_name])
        self.HznProfLongExtdPahIdx = HznProfLongExtdPahIdx(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznProfLongExtdPahIdx.fdx_name])
        self.HznProfLongExtdProfTyp = HznProfLongExtdProfTyp(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznProfLongExtdProfTyp.fdx_name])
        self.HznProfLongExtdTxPrev = HznProfLongExtdTxPrev(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznProfLongExtdTxPrev.fdx_name])
        self.HznProfLongExtdUpd = HznProfLongExtdUpd(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznProfLongExtdUpd.fdx_name])
        self.HznProfLongExtdVal = HznProfLongExtdVal(self, name_to_item_map['IHUBackBoneSignalIPdu12::' +HznProfLongExtdVal.fdx_name])
        self.HznRstExtd = HznRstExtd(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +HznRstExtd.fdx_name])
        self.HznSplyElectcSts = HznSplyElectcSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +HznSplyElectcSts.fdx_name])
        self.HzrdLiWarnActv = HzrdLiWarnActv(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +HzrdLiWarnActv.fdx_name])
        self.HzrdLiWarnSts = HzrdLiWarnSts(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +HzrdLiWarnSts.fdx_name])
        self.IdPenForUnits = IdPenForUnits(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +IdPenForUnits.fdx_name])
        self.IndcnOfParkAssiSts = IndcnOfParkAssiSts(self, name_to_item_map['PAMBackboneSignalIpdu01::' +IndcnOfParkAssiSts.fdx_name])
        self.IndcnOfPrkgAutSts = IndcnOfPrkgAutSts(self, name_to_item_map['PAMBackboneSignalIpdu01::' +IndcnOfPrkgAutSts.fdx_name])
        self.IndcnUnitDateFmt = IndcnUnitDateFmt(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +IndcnUnitDateFmt.fdx_name])
        self.IndcrDisp1WdSts = IndcrDisp1WdSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +IndcrDisp1WdSts.fdx_name])
        self.IndcrTurnSts1WdSts = IndcrTurnSts1WdSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +IndcrTurnSts1WdSts.fdx_name])
        self.IniValSigCfgIDBackboneFR = IniValSigCfgIDBackboneFR(self, name_to_item_map['ETCBackBoneSignalIPdu01::' +IniValSigCfgIDBackboneFR.fdx_name])
        self.InsdCarNoiseMeasd = InsdCarNoiseMeasd(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +InsdCarNoiseMeasd.fdx_name])
        self.IntrBriSts = IntrBriSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +IntrBriSts.fdx_name])
        self.IntrLiAmbLiSetgForLiInten = IntrLiAmbLiSetgForLiInten(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +IntrLiAmbLiSetgForLiInten.fdx_name])
        self.IntrLiAmbLiSetgForLiTintg = IntrLiAmbLiSetgForLiTintg(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +IntrLiAmbLiSetgForLiTintg.fdx_name])
        self.IntrLiAmbLiSetgPen = IntrLiAmbLiSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +IntrLiAmbLiSetgPen.fdx_name])
        self.IntrLiSurrndgsLiSetgForLiInten = IntrLiSurrndgsLiSetgForLiInten(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +IntrLiSurrndgsLiSetgForLiInten.fdx_name])
        self.IntrLiSurrndgsLiSetgForLiLvl = IntrLiSurrndgsLiSetgForLiLvl(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +IntrLiSurrndgsLiSetgForLiLvl.fdx_name])
        self.IntrLiSurrndgsLiSetgPen = IntrLiSurrndgsLiSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +IntrLiSurrndgsLiSetgPen.fdx_name])
        self.IntrMirrTintgSetgPen = IntrMirrTintgSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +IntrMirrTintgSetgPen.fdx_name])
        self.IntrMirrTintgSetgSts = IntrMirrTintgSetgSts(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +IntrMirrTintgSetgSts.fdx_name])
        self.IntscnCmplxInHznEdge = IntscnCmplxInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +IntscnCmplxInHznEdge.fdx_name])
        self.IntscnCmplxInHznSeg = IntscnCmplxInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +IntscnCmplxInHznSeg.fdx_name])
        self.IntvAndWarnModForLaneKeepAidPen = IntvAndWarnModForLaneKeepAidPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +IntvAndWarnModForLaneKeepAidPen.fdx_name])
        self.IntvAndWarnModForLaneKeepAidSts = IntvAndWarnModForLaneKeepAidSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +IntvAndWarnModForLaneKeepAidSts.fdx_name])
        self.iTPMSCalPsbl = iTPMSCalPsbl(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +iTPMSCalPsbl.fdx_name])
        self.iTPMSCalReq = iTPMSCalReq(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +iTPMSCalReq.fdx_name])
        self.iTPMSCalSts = iTPMSCalSts(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +iTPMSCalSts.fdx_name])
        self.iTPMSTirePMSts = iTPMSTirePMSts(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +iTPMSTirePMSts.fdx_name])
        self.KeyLostWarnIndcn = KeyLostWarnIndcn(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +KeyLostWarnIndcn.fdx_name])
        self.KeyProfMpgUpdForIdPen = KeyProfMpgUpdForIdPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +KeyProfMpgUpdForIdPen.fdx_name])
        self.KeyProfMpgUpdKeyProfMpgUpdOff = KeyProfMpgUpdKeyProfMpgUpdOff(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +KeyProfMpgUpdKeyProfMpgUpdOff.fdx_name])
        self.KeyReadReqFromSetgMgr = KeyReadReqFromSetgMgr(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +KeyReadReqFromSetgMgr.fdx_name])
        self.KeyReadStsToProfCtrlBoolean = KeyReadStsToProfCtrlBoolean(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +KeyReadStsToProfCtrlBoolean.fdx_name])
        self.KeyReadStsToProfCtrlKeyId = KeyReadStsToProfCtrlKeyId(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +KeyReadStsToProfCtrlKeyId.fdx_name])
        self.KeyRmnIndcn = KeyRmnIndcn(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +KeyRmnIndcn.fdx_name])
        self.KeySpdWarn = KeySpdWarn(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +KeySpdWarn.fdx_name])
        self.LampSuppSrv = LampSuppSrv(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +LampSuppSrv.fdx_name])
        self.LaneChgWarnActvPen = LaneChgWarnActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +LaneChgWarnActvPen.fdx_name])
        self.LaneChgWarnActvSts = LaneChgWarnActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +LaneChgWarnActvSts.fdx_name])
        self.LaneChgWarnSts = LaneChgWarnSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +LaneChgWarnSts.fdx_name])
        self.LaneDetnStsForAutDrv = LaneDetnStsForAutDrv(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +LaneDetnStsForAutDrv.fdx_name])
        self.LaneDetnStsForLaneKeepAid = LaneDetnStsForLaneKeepAid(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +LaneDetnStsForLaneKeepAid.fdx_name])
        self.LaneKeepAidActvPen = LaneKeepAidActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +LaneKeepAidActvPen.fdx_name])
        self.LaneKeepAidActvSts = LaneKeepAidActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +LaneKeepAidActvSts.fdx_name])
        self.LaneKeepAidRoadEdgeActvPen = LaneKeepAidRoadEdgeActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +LaneKeepAidRoadEdgeActvPen.fdx_name])
        self.LaneKeepAidRoadEdgeActvSts = LaneKeepAidRoadEdgeActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +LaneKeepAidRoadEdgeActvSts.fdx_name])
        self.LaneKeepAidSts = LaneKeepAidSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +LaneKeepAidSts.fdx_name])
        self.LanePrsntInHznPosn = LanePrsntInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +LanePrsntInHznPosn.fdx_name])
        self.LatMovmtWarn = LatMovmtWarn(self, name_to_item_map['ASDMBackBoneSignalIPdu02::' +LatMovmtWarn.fdx_name])
        self.LcmaOn1 = LcmaOn1(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +LcmaOn1.fdx_name])
        self.LiAdpvReqPen = LiAdpvReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiAdpvReqPen.fdx_name])
        self.LiAdpvReqSts = LiAdpvReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiAdpvReqSts.fdx_name])
        self.LiAutTranReqPen = LiAutTranReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +LiAutTranReqPen.fdx_name])
        self.LiAutTranReqSts = LiAutTranReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +LiAutTranReqSts.fdx_name])
        self.LiBeamHiAuxReqPen = LiBeamHiAuxReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiBeamHiAuxReqPen.fdx_name])
        self.LiBeamHiAuxReqSts = LiBeamHiAuxReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiBeamHiAuxReqSts.fdx_name])
        self.LiCornrgReqPen = LiCornrgReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiCornrgReqPen.fdx_name])
        self.LiCornrgReqSts = LiCornrgReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiCornrgReqSts.fdx_name])
        self.LiDaytiRunngReqPen = LiDaytiRunngReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiDaytiRunngReqPen.fdx_name])
        self.LiDaytiRunngReqSts = LiDaytiRunngReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiDaytiRunngReqSts.fdx_name])
        self.LiDrvrFltIndcrTurn = LiDrvrFltIndcrTurn(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +LiDrvrFltIndcrTurn.fdx_name])
        self.LiExtReq1WdReq5IdPen = LiExtReq1WdReq5IdPen(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +LiExtReq1WdReq5IdPen.fdx_name])
        self.LiExtReq1WdReq5SlowNormFast = LiExtReq1WdReq5SlowNormFast(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +LiExtReq1WdReq5SlowNormFast.fdx_name])
        self.LiHomeLvngReqPen = LiHomeLvngReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiHomeLvngReqPen.fdx_name])
        self.LiHomeLvngReqSts = LiHomeLvngReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiHomeLvngReqSts.fdx_name])
        self.LiHomeSafeReqPen = LiHomeSafeReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiHomeSafeReqPen.fdx_name])
        self.LiHomeSafeReqSts = LiHomeSafeReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiHomeSafeReqSts.fdx_name])
        self.LiLvrSwt1 = LiLvrSwt1(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +LiLvrSwt1.fdx_name])
        self.LiPassFltIndcrTurn = LiPassFltIndcrTurn(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +LiPassFltIndcrTurn.fdx_name])
        self.LiSeldForDrvrPfmncMonPen = LiSeldForDrvrPfmncMonPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +LiSeldForDrvrPfmncMonPen.fdx_name])
        self.LiSeldForDrvrPfmncMonSts = LiSeldForDrvrPfmncMonSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +LiSeldForDrvrPfmncMonSts.fdx_name])
        self.ListOfNodAvListOfNodAv1 = ListOfNodAvListOfNodAv1(self, name_to_item_map['CEMBackBoneSignalIpdu14::' +ListOfNodAvListOfNodAv1.fdx_name])
        self.ListOfNodAvListOfNodAv2 = ListOfNodAvListOfNodAv2(self, name_to_item_map['CEMBackBoneSignalIpdu14::' +ListOfNodAvListOfNodAv2.fdx_name])
        self.ListOfNodAvListOfNodAv3 = ListOfNodAvListOfNodAv3(self, name_to_item_map['CEMBackBoneSignalIpdu14::' +ListOfNodAvListOfNodAv3.fdx_name])
        self.ListOfNodAvListOfNodAv4 = ListOfNodAvListOfNodAv4(self, name_to_item_map['CEMBackBoneSignalIpdu14::' +ListOfNodAvListOfNodAv4.fdx_name])
        self.ListOfNodAvListOfNodAv5 = ListOfNodAvListOfNodAv5(self, name_to_item_map['CEMBackBoneSignalIpdu14::' +ListOfNodAvListOfNodAv5.fdx_name])
        self.ListOfNodAvListOfNodAv6 = ListOfNodAvListOfNodAv6(self, name_to_item_map['CEMBackBoneSignalIpdu14::' +ListOfNodAvListOfNodAv6.fdx_name])
        self.ListOfNodAvListOfNodAv7 = ListOfNodAvListOfNodAv7(self, name_to_item_map['CEMBackBoneSignalIpdu14::' +ListOfNodAvListOfNodAv7.fdx_name])
        self.ListOfNodAvListOfNodAv8 = ListOfNodAvListOfNodAv8(self, name_to_item_map['CEMBackBoneSignalIpdu14::' +ListOfNodAvListOfNodAv8.fdx_name])
        self.LiTrfcSideReq = LiTrfcSideReq(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LiTrfcSideReq.fdx_name])
        self.LockgCenStsForUsrFb = LockgCenStsForUsrFb(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +LockgCenStsForUsrFb.fdx_name])
        self.LockgCenStsLockSt = LockgCenStsLockSt(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +LockgCenStsLockSt.fdx_name])
        self.LockgCenStsUpdEve = LockgCenStsUpdEve(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +LockgCenStsUpdEve.fdx_name])
        self.LockgCenTrigSrc = LockgCenTrigSrc(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +LockgCenTrigSrc.fdx_name])
        self.LockgFbSoundReqPen = LockgFbSoundReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +LockgFbSoundReqPen.fdx_name])
        self.LockgFbSoundReqSts = LockgFbSoundReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +LockgFbSoundReqSts.fdx_name])
        self.LockgFbVisReqPen = LockgFbVisReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +LockgFbVisReqPen.fdx_name])
        self.LockgFbVisReqSts = LockgFbVisReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +LockgFbVisReqSts.fdx_name])
        self.LockgPrsnlReqFromHmi = LockgPrsnlReqFromHmi(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LockgPrsnlReqFromHmi.fdx_name])
        self.LockgPrsnlSts = LockgPrsnlSts(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +LockgPrsnlSts.fdx_name])
        self.LockSpdReqPen = LockSpdReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +LockSpdReqPen.fdx_name])
        self.LockSpdReqSts = LockSpdReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +LockSpdReqSts.fdx_name])
        self.LstEdgeAtOffsInHznEdge = LstEdgeAtOffsInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LstEdgeAtOffsInHznEdge.fdx_name])
        self.LvlCtrlSetg = LvlCtrlSetg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +LvlCtrlSetg.fdx_name])
        self.LvlOfClimaCmft = LvlOfClimaCmft(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +LvlOfClimaCmft.fdx_name])
        self.LvlSeldForSpdLimAdpvPen = LvlSeldForSpdLimAdpvPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +LvlSeldForSpdLimAdpvPen.fdx_name])
        self.LvlSeldForSpdLimAdpvSts = LvlSeldForSpdLimAdpvSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +LvlSeldForSpdLimAdpvSts.fdx_name])
        self.MapSrcInHznData = MapSrcInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +MapSrcInHznData.fdx_name])
        self.MassgFctActvDrvrMassgFctActv = MassgFctActvDrvrMassgFctActv(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +MassgFctActvDrvrMassgFctActv.fdx_name])
        self.MassgFctActvPassMassgFctActv = MassgFctActvPassMassgFctActv(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +MassgFctActvPassMassgFctActv.fdx_name])
        self.MemBtnSound = MemBtnSound(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +MemBtnSound.fdx_name])
        self.Mins = Mins(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +Mins.fdx_name])
        self.Mins1 = Mins1(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +Mins1.fdx_name])
        self.MirrDwnStsAtDrvr = MirrDwnStsAtDrvr(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +MirrDwnStsAtDrvr.fdx_name])
        self.MirrDwnStsAtPass = MirrDwnStsAtPass(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +MirrDwnStsAtPass.fdx_name])
        self.MirrFoldStsAtDrvr = MirrFoldStsAtDrvr(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +MirrFoldStsAtDrvr.fdx_name])
        self.MirrFoldStsAtPass = MirrFoldStsAtPass(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +MirrFoldStsAtPass.fdx_name])
        self.MmedHmiModStd = MmedHmiModStd(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +MmedHmiModStd.fdx_name])
        self.MmedMaiPwrMod = MmedMaiPwrMod(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +MmedMaiPwrMod.fdx_name])
        self.MmedTvmPwerMod = MmedTvmPwerMod(self, name_to_item_map['IHUBackBoneSignalIPdu13::' +MmedTvmPwerMod.fdx_name])
        self.MsgTypInHznData = MsgTypInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +MsgTypInHznData.fdx_name])
        self.MsgTypInHznEdge = MsgTypInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +MsgTypInHznEdge.fdx_name])
        self.MsgTypInHznPosn = MsgTypInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +MsgTypInHznPosn.fdx_name])
        self.MsgTypInHznProfLong = MsgTypInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +MsgTypInHznProfLong.fdx_name])
        self.MsgTypInHznProfSho = MsgTypInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +MsgTypInHznProfSho.fdx_name])
        self.MsgTypInHznSeg = MsgTypInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +MsgTypInHznSeg.fdx_name])
        self.MstCfgIDBackboneFR = MstCfgIDBackboneFR(self, name_to_item_map['VCMBackboneSignalIPdu00::' +MstCfgIDBackboneFR.fdx_name])
        self.Mth = Mth(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +Mth.fdx_name])
        self.Mth1 = Mth1(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +Mth1.fdx_name])
        self.NetCtrlrActvtPrio = NetCtrlrActvtPrio(self, name_to_item_map['VCMBackboneSignalIPdu00::' +NetCtrlrActvtPrio.fdx_name])
        self.NetCtrlrActvtResourceGroup = NetCtrlrActvtResourceGroup(self, name_to_item_map['VCMBackboneSignalIPdu00::' +NetCtrlrActvtResourceGroup.fdx_name])
        self.NetHdActvtPrio = NetHdActvtPrio(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +NetHdActvtPrio.fdx_name])
        self.NetHdActvtResourceGroup = NetHdActvtResourceGroup(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +NetHdActvtResourceGroup.fdx_name])
        self.NetTelmActvtPrio = NetTelmActvtPrio(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +NetTelmActvtPrio.fdx_name])
        self.NetTelmActvtResourceGroup = NetTelmActvtResourceGroup(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +NetTelmActvtResourceGroup.fdx_name])
        self.NewTripCdn = NewTripCdn(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +NewTripCdn.fdx_name])
        self.NodCtrlInHznProfLong = NodCtrlInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +NodCtrlInHznProfLong.fdx_name])
        self.NodCtrlInHznProfSho = NodCtrlInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +NodCtrlInHznProfSho.fdx_name])
        self.NoEntryWarnReq = NoEntryWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +NoEntryWarnReq.fdx_name])
        self.NoOfSatltForSysNo1InPosnFromSatlt = NoOfSatltForSysNo1InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +NoOfSatltForSysNo1InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo2InPosnFromSatlt = NoOfSatltForSysNo2InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +NoOfSatltForSysNo2InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo3InPosnFromSatlt = NoOfSatltForSysNo3InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +NoOfSatltForSysNo3InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo4InPosnFromSatlt = NoOfSatltForSysNo4InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +NoOfSatltForSysNo4InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo5InPosnFromSatlt = NoOfSatltForSysNo5InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +NoOfSatltForSysNo5InPosnFromSatlt.fdx_name])
        self.NoOfSatltForSysNo6InPosnFromSatlt = NoOfSatltForSysNo6InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +NoOfSatltForSysNo6InPosnFromSatlt.fdx_name])
        self.NoSoundSys = NoSoundSys(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +NoSoundSys.fdx_name])
        self.NotifChkDistbn = NotifChkDistbn(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +NotifChkDistbn.fdx_name])
        self.NrOfBltAppld = NrOfBltAppld(self, name_to_item_map['SRSBackboneSignalIPdu03::' +NrOfBltAppld.fdx_name])
        self.NrOfKeyAvl = NrOfKeyAvl(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +NrOfKeyAvl.fdx_name])
        self.NrOfLaneInDrvgDirInHznEdge = NrOfLaneInDrvgDirInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +NrOfLaneInDrvgDirInHznEdge.fdx_name])
        self.NrOfLaneInDrvgDirInHznSeg = NrOfLaneInDrvgDirInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +NrOfLaneInDrvgDirInHznSeg.fdx_name])
        self.NrOfLaneInSecDirInHznEdge = NrOfLaneInSecDirInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +NrOfLaneInSecDirInHznEdge.fdx_name])
        self.NrOfLaneInSecDirInHznSeg = NrOfLaneInSecDirInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +NrOfLaneInSecDirInHznSeg.fdx_name])
        self.OffsForDrvrSpprtFctActvSts = OffsForDrvrSpprtFctActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +OffsForDrvrSpprtFctActvSts.fdx_name])
        self.OffsForSpdWarnSetgPen = OffsForSpdWarnSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +OffsForSpdWarnSetgPen.fdx_name])
        self.OffsForSpdWarnSetgSts = OffsForSpdWarnSetgSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +OffsForSpdWarnSetgSts.fdx_name])
        self.OffsInHznEdge = OffsInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +OffsInHznEdge.fdx_name])
        self.OffsInHznPosn = OffsInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +OffsInHznPosn.fdx_name])
        self.OffsInHznProfLong = OffsInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +OffsInHznProfLong.fdx_name])
        self.OffsInHznProfSho = OffsInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +OffsInHznProfSho.fdx_name])
        self.OffsInHznSeg = OffsInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +OffsInHznSeg.fdx_name])
        self.PahIdxInHznEdge = PahIdxInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +PahIdxInHznEdge.fdx_name])
        self.PahIdxInHznPosn = PahIdxInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +PahIdxInHznPosn.fdx_name])
        self.PahIdxInHznProfLong = PahIdxInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +PahIdxInHznProfLong.fdx_name])
        self.PahIdxInHznProfSho = PahIdxInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +PahIdxInHznProfSho.fdx_name])
        self.PahIdxInHznSeg = PahIdxInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +PahIdxInHznSeg.fdx_name])
        self.PahIdxNewInHznEdge = PahIdxNewInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +PahIdxNewInHznEdge.fdx_name])
        self.ParkAssiDstFrntOfAudSideWarn = ParkAssiDstFrntOfAudSideWarn(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstFrntOfAudSideWarn.fdx_name])
        self.ParkAssiDstFrntOfAudWarnDir = ParkAssiDstFrntOfAudWarnDir(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstFrntOfAudWarnDir.fdx_name])
        self.ParkAssiDstFrntOfDstOfSnsrInsdRi = ParkAssiDstFrntOfDstOfSnsrInsdRi(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstFrntOfDstOfSnsrInsdRi.fdx_name])
        self.ParkAssiDstFrntOfDstOfSnsrLeSide = ParkAssiDstFrntOfDstOfSnsrLeSide(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstFrntOfDstOfSnsrLeSide.fdx_name])
        self.ParkAssiDstFrntOfDstOfSnsrOutdRi = ParkAssiDstFrntOfDstOfSnsrOutdRi(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstFrntOfDstOfSnsrOutdRi.fdx_name])
        self.ParkAssiDstFrntOfDstOfSnsrRiSide = ParkAssiDstFrntOfDstOfSnsrRiSide(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstFrntOfDstOfSnsrRiSide.fdx_name])
        self.ParkAssiDstFrntOfSnsrInsdLe = ParkAssiDstFrntOfSnsrInsdLe(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstFrntOfSnsrInsdLe.fdx_name])
        self.ParkAssiDstFrntOfSnsrOutdLe = ParkAssiDstFrntOfSnsrOutdLe(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstFrntOfSnsrOutdLe.fdx_name])
        self.ParkAssiDstReOfAudSideWarn = ParkAssiDstReOfAudSideWarn(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstReOfAudSideWarn.fdx_name])
        self.ParkAssiDstReOfAudWarnDir = ParkAssiDstReOfAudWarnDir(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstReOfAudWarnDir.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrInsdLe = ParkAssiDstReOfDstOfSnsrInsdLe(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstReOfDstOfSnsrInsdLe.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrInsdRi = ParkAssiDstReOfDstOfSnsrInsdRi(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstReOfDstOfSnsrInsdRi.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrLeSide = ParkAssiDstReOfDstOfSnsrLeSide(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstReOfDstOfSnsrLeSide.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrOutdLe = ParkAssiDstReOfDstOfSnsrOutdLe(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstReOfDstOfSnsrOutdLe.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrOutdRi = ParkAssiDstReOfDstOfSnsrOutdRi(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstReOfDstOfSnsrOutdRi.fdx_name])
        self.ParkAssiDstReOfDstOfSnsrRiSide = ParkAssiDstReOfDstOfSnsrRiSide(self, name_to_item_map['PAMBackboneSignalIpdu01::' +ParkAssiDstReOfDstOfSnsrRiSide.fdx_name])
        self.PartOfCourseCalcdInHznEdge = PartOfCourseCalcdInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +PartOfCourseCalcdInHznEdge.fdx_name])
        self.PartOfCourseCalcdInHznSeg = PartOfCourseCalcdInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +PartOfCourseCalcdInHznSeg.fdx_name])
        self.PartOfYrVersOfMapInHznData = PartOfYrVersOfMapInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +PartOfYrVersOfMapInHznData.fdx_name])
        self.PasAlrmDeactvnReq = PasAlrmDeactvnReq(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +PasAlrmDeactvnReq.fdx_name])
        self.PasAlrmSts = PasAlrmSts(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +PasAlrmSts.fdx_name])
        self.PassMassgRunng = PassMassgRunng(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +PassMassgRunng.fdx_name])
        self.PassSeatActvSpplFct = PassSeatActvSpplFct(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +PassSeatActvSpplFct.fdx_name])
        self.PassSeatDispMassgFctMassgInten = PassSeatDispMassgFctMassgInten(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +PassSeatDispMassgFctMassgInten.fdx_name])
        self.PassSeatDispMassgFctMassgProg = PassSeatDispMassgFctMassgProg(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +PassSeatDispMassgFctMassgProg.fdx_name])
        self.PassSeatDispMassgFctMassgSpdLvl = PassSeatDispMassgFctMassgSpdLvl(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +PassSeatDispMassgFctMassgSpdLvl.fdx_name])
        self.PassSeatDispMassgFctOnOff = PassSeatDispMassgFctOnOff(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +PassSeatDispMassgFctOnOff.fdx_name])
        self.PassSeatDispSpplFct = PassSeatDispSpplFct(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +PassSeatDispSpplFct.fdx_name])
        self.PassSeatMassgFctMassgInten = PassSeatMassgFctMassgInten(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +PassSeatMassgFctMassgInten.fdx_name])
        self.PassSeatMassgFctMassgProg = PassSeatMassgFctMassgProg(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +PassSeatMassgFctMassgProg.fdx_name])
        self.PassSeatMassgFctMassgSpdLvl = PassSeatMassgFctMassgSpdLvl(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +PassSeatMassgFctMassgSpdLvl.fdx_name])
        self.PassSeatMassgFctOnOff = PassSeatMassgFctOnOff(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +PassSeatMassgFctOnOff.fdx_name])
        self.PassSeatSts = PassSeatSts(self, name_to_item_map['SRSBackboneSignalIPdu03::' +PassSeatSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts = PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts = PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtHeiFrntSts = PassSeatSwtSts2PassSeatSwtHeiFrntSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +PassSeatSwtSts2PassSeatSwtHeiFrntSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtHeiSts = PassSeatSwtSts2PassSeatSwtHeiSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +PassSeatSwtSts2PassSeatSwtHeiSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtInclSts = PassSeatSwtSts2PassSeatSwtInclSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +PassSeatSwtSts2PassSeatSwtInclSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts = PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts.fdx_name])
        self.PassSeatSwtSts2PassSeatSwtSldSts = PassSeatSwtSts2PassSeatSwtSldSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +PassSeatSwtSts2PassSeatSwtSldSts.fdx_name])
        self.PinionSteerAg1 = PinionSteerAg1(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +PinionSteerAg1.fdx_name])
        self.PinionSteerAg1Qf = PinionSteerAg1Qf(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +PinionSteerAg1Qf.fdx_name])
        self.PosnAltiInPosnFromSatlt = PosnAltiInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PosnAltiInPosnFromSatlt.fdx_name])
        self.PosnDirInPosnFromSatlt = PosnDirInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PosnDirInPosnFromSatlt.fdx_name])
        self.PosnIdxInHznPosn = PosnIdxInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +PosnIdxInHznPosn.fdx_name])
        self.PosnLatInPosnFromSatlt = PosnLatInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PosnLatInPosnFromSatlt.fdx_name])
        self.PosnLgtInPosnFromSatlt = PosnLgtInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PosnLgtInPosnFromSatlt.fdx_name])
        self.PosnProbltyInHznPosn = PosnProbltyInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +PosnProbltyInHznPosn.fdx_name])
        self.PosnQlyInDstToManvLocn = PosnQlyInDstToManvLocn(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +PosnQlyInDstToManvLocn.fdx_name])
        self.PosnQlyInHznPosn = PosnQlyInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +PosnQlyInHznPosn.fdx_name])
        self.PosnSpdInPosnFromSatlt = PosnSpdInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PosnSpdInPosnFromSatlt.fdx_name])
        self.PosnTiDifInHznPosn = PosnTiDifInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +PosnTiDifInHznPosn.fdx_name])
        self.PosnVHozlInPosnFromSatlt = PosnVHozlInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PosnVHozlInPosnFromSatlt.fdx_name])
        self.PosnVVertInPosnFromSatlt = PosnVVertInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PosnVVertInPosnFromSatlt.fdx_name])
        self.PostDrvgClimaAvl = PostDrvgClimaAvl(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +PostDrvgClimaAvl.fdx_name])
        self.PreClngNotif = PreClngNotif(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +PreClngNotif.fdx_name])
        self.PreHozlDilInPosnFromSatlt = PreHozlDilInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PreHozlDilInPosnFromSatlt.fdx_name])
        self.PrePosnDilInPosnFromSatlt = PrePosnDilInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PrePosnDilInPosnFromSatlt.fdx_name])
        self.PreTiDilInPosnFromSatlt = PreTiDilInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PreTiDilInPosnFromSatlt.fdx_name])
        self.PreVertDilInPosnFromSatlt = PreVertDilInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +PreVertDilInPosnFromSatlt.fdx_name])
        self.PrkgAssiFailr = PrkgAssiFailr(self, name_to_item_map['PAMBackboneSignalIpdu01::' +PrkgAssiFailr.fdx_name])
        self.PrkgAssiManvActvSts = PrkgAssiManvActvSts(self, name_to_item_map['PAMBackboneSignalIpdu01::' +PrkgAssiManvActvSts.fdx_name])
        self.PrkgAssiManvProgs = PrkgAssiManvProgs(self, name_to_item_map['PAMBackboneSignalIpdu01::' +PrkgAssiManvProgs.fdx_name])
        self.PrkgAssiSts = PrkgAssiSts(self, name_to_item_map['PAMBackboneSignalIpdu01::' +PrkgAssiSts.fdx_name])
        self.PrkgAutSts = PrkgAutSts(self, name_to_item_map['PAMBackboneSignalIpdu01::' +PrkgAutSts.fdx_name])
        self.PrkgCamSysAvlSts = PrkgCamSysAvlSts(self, name_to_item_map['ASDMBackBoneSignalIPdu03::' +PrkgCamSysAvlSts.fdx_name])
        self.PrkgOutCfm = PrkgOutCfm(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +PrkgOutCfm.fdx_name])
        self.PrkgTypVld = PrkgTypVld(self, name_to_item_map['PAMBackboneSignalIpdu01::' +PrkgTypVld.fdx_name])
        self.ProfActProf1 = ProfActProf1(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf1.fdx_name])
        self.ProfActProf10 = ProfActProf10(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf10.fdx_name])
        self.ProfActProf11 = ProfActProf11(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf11.fdx_name])
        self.ProfActProf12 = ProfActProf12(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf12.fdx_name])
        self.ProfActProf2 = ProfActProf2(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf2.fdx_name])
        self.ProfActProf3 = ProfActProf3(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf3.fdx_name])
        self.ProfActProf4 = ProfActProf4(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf4.fdx_name])
        self.ProfActProf5 = ProfActProf5(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf5.fdx_name])
        self.ProfActProf6 = ProfActProf6(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf6.fdx_name])
        self.ProfActProf7 = ProfActProf7(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf7.fdx_name])
        self.ProfActProf8 = ProfActProf8(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf8.fdx_name])
        self.ProfActProf9 = ProfActProf9(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +ProfActProf9.fdx_name])
        self.ProfChg = ProfChg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +ProfChg.fdx_name])
        self.ProfLimdProf1 = ProfLimdProf1(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf1.fdx_name])
        self.ProfLimdProf10 = ProfLimdProf10(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf10.fdx_name])
        self.ProfLimdProf11 = ProfLimdProf11(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf11.fdx_name])
        self.ProfLimdProf12 = ProfLimdProf12(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf12.fdx_name])
        self.ProfLimdProf2 = ProfLimdProf2(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf2.fdx_name])
        self.ProfLimdProf3 = ProfLimdProf3(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf3.fdx_name])
        self.ProfLimdProf4 = ProfLimdProf4(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf4.fdx_name])
        self.ProfLimdProf5 = ProfLimdProf5(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf5.fdx_name])
        self.ProfLimdProf6 = ProfLimdProf6(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf6.fdx_name])
        self.ProfLimdProf7 = ProfLimdProf7(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf7.fdx_name])
        self.ProfLimdProf8 = ProfLimdProf8(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf8.fdx_name])
        self.ProfLimdProf9 = ProfLimdProf9(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfLimdProf9.fdx_name])
        self.ProfPenSts1 = ProfPenSts1(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +ProfPenSts1.fdx_name])
        self.ProfTypInHznProfLong = ProfTypInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +ProfTypInHznProfLong.fdx_name])
        self.ProfTypInHznProfSho = ProfTypInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +ProfTypInHznProfSho.fdx_name])
        self.ProfTypQlyInHznProfSho = ProfTypQlyInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +ProfTypQlyInHznProfSho.fdx_name])
        self.PrpsnDrvMod = PrpsnDrvMod(self, name_to_item_map['VDDMBackBoneSignalIpdu11::' +PrpsnDrvMod.fdx_name])
        self.PrpsnHvBattUsgModAct = PrpsnHvBattUsgModAct(self, name_to_item_map['VDDMBackBoneSignalIpdu05::' +PrpsnHvBattUsgModAct.fdx_name])
        self.PrpsnHvBattUsgModReq = PrpsnHvBattUsgModReq(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +PrpsnHvBattUsgModReq.fdx_name])
        self.PrpsnHvBattUsgOfChrgBlkd = PrpsnHvBattUsgOfChrgBlkd(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +PrpsnHvBattUsgOfChrgBlkd.fdx_name])
        self.PrpsnHvBattUsgOfChrgBlkd2 = PrpsnHvBattUsgOfChrgBlkd2(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +PrpsnHvBattUsgOfChrgBlkd2.fdx_name])
        self.PrpsnHvBattUsgOfHldBlkd = PrpsnHvBattUsgOfHldBlkd(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +PrpsnHvBattUsgOfHldBlkd.fdx_name])
        self.PrpsnHvBattUsgOfHldBlkd2 = PrpsnHvBattUsgOfHldBlkd2(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +PrpsnHvBattUsgOfHldBlkd2.fdx_name])
        self.PrpsnHvBattUsgOfHldSmtBlkd = PrpsnHvBattUsgOfHldSmtBlkd(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +PrpsnHvBattUsgOfHldSmtBlkd.fdx_name])
        self.PrpsnHvBattUsgOfHldSmtBlkd2 = PrpsnHvBattUsgOfHldSmtBlkd2(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +PrpsnHvBattUsgOfHldSmtBlkd2.fdx_name])
        self.PrpsnHvBattUsgOfHldSpd = PrpsnHvBattUsgOfHldSpd(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +PrpsnHvBattUsgOfHldSpd.fdx_name])
        self.PrpsnHvBattUsgStsDispd = PrpsnHvBattUsgStsDispd(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +PrpsnHvBattUsgStsDispd.fdx_name])
        self.PrpsnModElecDrvBlkd = PrpsnModElecDrvBlkd(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +PrpsnModElecDrvBlkd.fdx_name])
        self.PrpsnModOffroadBlkd = PrpsnModOffroadBlkd(self, name_to_item_map['VDDMBackBoneSignalIpdu08::' +PrpsnModOffroadBlkd.fdx_name])
        self.PrpsnModOfSaveBlkd = PrpsnModOfSaveBlkd(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +PrpsnModOfSaveBlkd.fdx_name])
        self.PrpsnModOfTracBlkd = PrpsnModOfTracBlkd(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +PrpsnModOfTracBlkd.fdx_name])
        self.PrpsnModSptBlkd = PrpsnModSptBlkd(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +PrpsnModSptBlkd.fdx_name])
        self.PtCluTqPtCluTq = PtCluTqPtCluTq(self, name_to_item_map['VDDMBackBoneSignalIPdu04::' +PtCluTqPtCluTq.fdx_name])
        self.PtCluTqPtCluTqDyn = PtCluTqPtCluTqDyn(self, name_to_item_map['VDDMBackBoneSignalIPdu04::' +PtCluTqPtCluTqDyn.fdx_name])
        self.PtCluTqPtCluTqQly = PtCluTqPtCluTqQly(self, name_to_item_map['VDDMBackBoneSignalIPdu04::' +PtCluTqPtCluTqQly.fdx_name])
        self.PtDrvrSetgPen = PtDrvrSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +PtDrvrSetgPen.fdx_name])
        self.PtDrvrSetgSts = PtDrvrSetgSts(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +PtDrvrSetgSts.fdx_name])
        self.PtGearTar = PtGearTar(self, name_to_item_map['VDDMBackBoneSignalIpdu07::' +PtGearTar.fdx_name])
        self.PtTqAtAxleFrntAct = PtTqAtAxleFrntAct(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +PtTqAtAxleFrntAct.fdx_name])
        self.PtTqAtWhlFrntLeAct = PtTqAtWhlFrntLeAct(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +PtTqAtWhlFrntLeAct.fdx_name])
        self.PtTqAtWhlFrntRiAct = PtTqAtWhlFrntRiAct(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +PtTqAtWhlFrntRiAct.fdx_name])
        self.PtTqAtWhlsFrntQly = PtTqAtWhlsFrntQly(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +PtTqAtWhlsFrntQly.fdx_name])
        self.PUnit = PUnit(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +PUnit.fdx_name])
        self.PwrChrgDetdForPrkgHeatrElec = PwrChrgDetdForPrkgHeatrElec(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +PwrChrgDetdForPrkgHeatrElec.fdx_name])
        self.PwrLvlElecMai = PwrLvlElecMai(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +PwrLvlElecMai.fdx_name])
        self.PwrLvlElecSubtyp = PwrLvlElecSubtyp(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +PwrLvlElecSubtyp.fdx_name])
        self.PwrSplyErrSts = PwrSplyErrSts(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +PwrSplyErrSts.fdx_name])
        self.RadioFrqAn = RadioFrqAn(self, name_to_item_map['IHUBackboneSignalIPdu01::' +RadioFrqAn.fdx_name])
        self.RainSenMemdReqPen = RainSenMemdReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RainSenMemdReqPen.fdx_name])
        self.RainSenMemdReqSts = RainSenMemdReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RainSenMemdReqSts.fdx_name])
        self.ReAxleWarn = ReAxleWarn(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +ReAxleWarn.fdx_name])
        self.RegnCodInHznData = RegnCodInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +RegnCodInHznData.fdx_name])
        self.RelDirInHznPosn = RelDirInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +RelDirInHznPosn.fdx_name])
        self.RelProbltyInHznEdge = RelProbltyInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RelProbltyInHznEdge.fdx_name])
        self.RelProbltyInHznSeg = RelProbltyInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RelProbltyInHznSeg.fdx_name])
        self.RiOfWayInHznEdge = RiOfWayInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RiOfWayInHznEdge.fdx_name])
        self.RlyPwrDistbnCmd1WdBattSaveCmd = RlyPwrDistbnCmd1WdBattSaveCmd(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +RlyPwrDistbnCmd1WdBattSaveCmd.fdx_name])
        self.RlyPwrDistbnCmd1WdIgnRlyCmd = RlyPwrDistbnCmd1WdIgnRlyCmd(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +RlyPwrDistbnCmd1WdIgnRlyCmd.fdx_name])
        self.RlyPwrDistbnCmd1WdIgnRlyExtCmd = RlyPwrDistbnCmd1WdIgnRlyExtCmd(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +RlyPwrDistbnCmd1WdIgnRlyExtCmd.fdx_name])
        self.RlyPwrDistbnCmd1WdPreBattSaveCmd = RlyPwrDistbnCmd1WdPreBattSaveCmd(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +RlyPwrDistbnCmd1WdPreBattSaveCmd.fdx_name])
        self.RmnLockgPrsnlReq = RmnLockgPrsnlReq(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +RmnLockgPrsnlReq.fdx_name])
        self.RngbdIllmnCmdPen = RngbdIllmnCmdPen(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +RngbdIllmnCmdPen.fdx_name])
        self.RngbdIllmnCmdSts = RngbdIllmnCmdSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +RngbdIllmnCmdSts.fdx_name])
        self.RoadClassInHznEdge = RoadClassInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RoadClassInHznEdge.fdx_name])
        self.RoadClassInHznSeg = RoadClassInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RoadClassInHznSeg.fdx_name])
        self.RoadFricIndcnActvPen = RoadFricIndcnActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +RoadFricIndcnActvPen.fdx_name])
        self.RoadFricIndcnActvSts = RoadFricIndcnActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +RoadFricIndcnActvSts.fdx_name])
        self.RoadFricIndcnSts = RoadFricIndcnSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RoadFricIndcnSts.fdx_name])
        self.RoadFricWarnReq = RoadFricWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RoadFricWarnReq.fdx_name])
        self.RoadLiArInHznSeg = RoadLiArInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RoadLiArInHznSeg.fdx_name])
        self.RoadMplInHznSeg = RoadMplInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +RoadMplInHznSeg.fdx_name])
        self.RoadSgnInfoActvPen = RoadSgnInfoActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +RoadSgnInfoActvPen.fdx_name])
        self.RoadSgnInfoActvSts = RoadSgnInfoActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +RoadSgnInfoActvSts.fdx_name])
        self.RoadSgnInfoSts = RoadSgnInfoSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RoadSgnInfoSts.fdx_name])
        self.RoadSpdLimActvSts = RoadSpdLimActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +RoadSpdLimActvSts.fdx_name])
        self.RoadUsrProtnActv = RoadUsrProtnActv(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +RoadUsrProtnActv.fdx_name])
        self.RoadUsrProtnSts = RoadUsrProtnSts(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +RoadUsrProtnSts.fdx_name])
        self.RollRate1 = RollRate1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +RollRate1.fdx_name])
        self.RollRate1Qf1 = RollRate1Qf1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +RollRate1Qf1.fdx_name])
        self.RouteInfoDestInfo = RouteInfoDestInfo(self, name_to_item_map['IHUBackBoneSignalIPdu13::' +RouteInfoDestInfo.fdx_name])
        self.RouteInfoDestProblty = RouteInfoDestProblty(self, name_to_item_map['IHUBackBoneSignalIPdu13::' +RouteInfoDestProblty.fdx_name])
        self.RouteInfoDestSrc = RouteInfoDestSrc(self, name_to_item_map['IHUBackBoneSignalIPdu13::' +RouteInfoDestSrc.fdx_name])
        self.RouteInfoIdOfDest = RouteInfoIdOfDest(self, name_to_item_map['IHUBackBoneSignalIPdu13::' +RouteInfoIdOfDest.fdx_name])
        self.RouteInfoNrOfDestCmpl = RouteInfoNrOfDestCmpl(self, name_to_item_map['IHUBackBoneSignalIPdu13::' +RouteInfoNrOfDestCmpl.fdx_name])
        self.RsdsSysStsLeCtraSts = RsdsSysStsLeCtraSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsLeCtraSts.fdx_name])
        self.RsdsSysStsLeCtraSts1 = RsdsSysStsLeCtraSts1(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsLeCtraSts1.fdx_name])
        self.RsdsSysStsLeLcmaSts = RsdsSysStsLeLcmaSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsLeLcmaSts.fdx_name])
        self.RsdsSysStsLeLcmaSts1 = RsdsSysStsLeLcmaSts1(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsLeLcmaSts1.fdx_name])
        self.RsdsSysStsLeRcmSts = RsdsSysStsLeRcmSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsLeRcmSts.fdx_name])
        self.RsdsSysStsLeRcwSts = RsdsSysStsLeRcwSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsLeRcwSts.fdx_name])
        self.RsdsSysStsLeRsdsSts = RsdsSysStsLeRsdsSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsLeRsdsSts.fdx_name])
        self.RsdsSysStsRiCtraSts = RsdsSysStsRiCtraSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsRiCtraSts.fdx_name])
        self.RsdsSysStsRiCtraSts1 = RsdsSysStsRiCtraSts1(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsRiCtraSts1.fdx_name])
        self.RsdsSysStsRiLcmaSts = RsdsSysStsRiLcmaSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsRiLcmaSts.fdx_name])
        self.RsdsSysStsRiLcmaSts1 = RsdsSysStsRiLcmaSts1(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsRiLcmaSts1.fdx_name])
        self.RsdsSysStsRiRcmSts = RsdsSysStsRiRcmSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsRiRcmSts.fdx_name])
        self.RsdsSysStsRiRcwSts = RsdsSysStsRiRcwSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsRiRcwSts.fdx_name])
        self.RsdsSysStsRiRsdsSts = RsdsSysStsRiRsdsSts(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +RsdsSysStsRiRsdsSts.fdx_name])
        self.SatltPosnStsPrm1InPosnFromSatlt = SatltPosnStsPrm1InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltPosnStsPrm1InPosnFromSatlt.fdx_name])
        self.SatltPosnStsPrm2InPosnFromSatlt = SatltPosnStsPrm2InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltPosnStsPrm2InPosnFromSatlt.fdx_name])
        self.SatltPosnStsPrm3InPosnFromSatlt = SatltPosnStsPrm3InPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltPosnStsPrm3InPosnFromSatlt.fdx_name])
        self.SatltSysNo1InUseInPosnFromSatlt = SatltSysNo1InUseInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltSysNo1InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo2InUseInPosnFromSatlt = SatltSysNo2InUseInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltSysNo2InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo3InUseInPosnFromSatlt = SatltSysNo3InUseInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltSysNo3InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo4InUseInPosnFromSatlt = SatltSysNo4InUseInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltSysNo4InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo5InUseInPosnFromSatlt = SatltSysNo5InUseInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltSysNo5InUseInPosnFromSatlt.fdx_name])
        self.SatltSysNo6InUseInPosnFromSatlt = SatltSysNo6InUseInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +SatltSysNo6InUseInPosnFromSatlt.fdx_name])
        self.SaveSetgToMemPrmnt = SaveSetgToMemPrmnt(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +SaveSetgToMemPrmnt.fdx_name])
        self.ScrBarVolIndcn = ScrBarVolIndcn(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +ScrBarVolIndcn.fdx_name])
        self.ScrMaxFillgVol = ScrMaxFillgVol(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +ScrMaxFillgVol.fdx_name])
        self.ScrReagentTankVol = ScrReagentTankVol(self, name_to_item_map['VDDMBackBoneSignalIpdu10::' +ScrReagentTankVol.fdx_name])
        self.SeatBackUnlckdThrdLe = SeatBackUnlckdThrdLe(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +SeatBackUnlckdThrdLe.fdx_name])
        self.SeatBackUnlckdThrdRi = SeatBackUnlckdThrdRi(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +SeatBackUnlckdThrdRi.fdx_name])
        self.SeatDispBtnPsdDrvrSeatDispBtnPsd = SeatDispBtnPsdDrvrSeatDispBtnPsd(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +SeatDispBtnPsdDrvrSeatDispBtnPsd.fdx_name])
        self.SeatDispBtnPsdPassSeatDispBtnPsd = SeatDispBtnPsdPassSeatDispBtnPsd(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +SeatDispBtnPsdPassSeatDispBtnPsd.fdx_name])
        self.SeatHeatDurgClimaEnad = SeatHeatDurgClimaEnad(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SeatHeatDurgClimaEnad.fdx_name])
        self.SeatHeatgAutCdn = SeatHeatgAutCdn(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +SeatHeatgAutCdn.fdx_name])
        self.SeatSwtLeSigThrd = SeatSwtLeSigThrd(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +SeatSwtLeSigThrd.fdx_name])
        self.SeatSwtRiSigThrd = SeatSwtRiSigThrd(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +SeatSwtRiSigThrd.fdx_name])
        self.SeatVentnAutCdn = SeatVentnAutCdn(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +SeatVentnAutCdn.fdx_name])
        self.Sec = Sec(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +Sec.fdx_name])
        self.Sec1 = Sec1(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +Sec1.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForDrvrMtr = SetgAndRstOfTripForDrvrSetgTripForDrvrMtr(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForDrvrMtr.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForDstLong = SetgAndRstOfTripForDrvrSetgTripForDstLong(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForDstLong.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForDstSho1 = SetgAndRstOfTripForDrvrSetgTripForDstSho1(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForDstSho1.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForDstSho2 = SetgAndRstOfTripForDrvrSetgTripForDstSho2(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForDstSho2.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg1 = SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg1(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg1.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg2 = SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg2(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg2.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForEgyDst = SetgAndRstOfTripForDrvrSetgTripForEgyDst(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForEgyDst.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForFuCns = SetgAndRstOfTripForDrvrSetgTripForFuCns(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForFuCns.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg1 = SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg1(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg1.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg2 = SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg2(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg2.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForFuDst = SetgAndRstOfTripForDrvrSetgTripForFuDst(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForFuDst.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForRstOfAllTripFct1 = SetgAndRstOfTripForDrvrSetgTripForRstOfAllTripFct1(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForRstOfAllTripFct1.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForSpdAvg1 = SetgAndRstOfTripForDrvrSetgTripForSpdAvg1(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForSpdAvg1.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForSpdAvg2 = SetgAndRstOfTripForDrvrSetgTripForSpdAvg2(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForSpdAvg2.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForSpdDig = SetgAndRstOfTripForDrvrSetgTripForSpdDig(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForSpdDig.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForSpdPtr = SetgAndRstOfTripForDrvrSetgTripForSpdPtr(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForSpdPtr.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForSpdTrvl = SetgAndRstOfTripForDrvrSetgTripForSpdTrvl(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForSpdTrvl.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForTiDrv1 = SetgAndRstOfTripForDrvrSetgTripForTiDrv1(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForTiDrv1.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForTiDrv2 = SetgAndRstOfTripForDrvrSetgTripForTiDrv2(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForTiDrv2.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripForTiRstCdn = SetgAndRstOfTripForDrvrSetgTripForTiRstCdn(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripForTiRstCdn.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripOffOn = SetgAndRstOfTripForDrvrSetgTripOffOn(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripOffOn.fdx_name])
        self.SetgAndRstOfTripForDrvrSetgTripWithProfID = SetgAndRstOfTripForDrvrSetgTripWithProfID(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SetgAndRstOfTripForDrvrSetgTripWithProfID.fdx_name])
        self.SetOfLangIdPen = SetOfLangIdPen(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +SetOfLangIdPen.fdx_name])
        self.SetOfLangLangTyp = SetOfLangLangTyp(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +SetOfLangLangTyp.fdx_name])
        self.SftyAudDend = SftyAudDend(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +SftyAudDend.fdx_name])
        self.SftyCchAccStsActv = SftyCchAccStsActv(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchAccStsActv.fdx_name])
        self.SftyCchAccStsDay = SftyCchAccStsDay(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchAccStsDay.fdx_name])
        self.SftyCchAccStsMth = SftyCchAccStsMth(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchAccStsMth.fdx_name])
        self.SftyCchAccStsYr = SftyCchAccStsYr(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchAccStsYr.fdx_name])
        self.SftyCchLaneKeepAidStsActv = SftyCchLaneKeepAidStsActv(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLaneKeepAidStsActv.fdx_name])
        self.SftyCchLaneKeepAidStsDay = SftyCchLaneKeepAidStsDay(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLaneKeepAidStsDay.fdx_name])
        self.SftyCchLaneKeepAidStsMth = SftyCchLaneKeepAidStsMth(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLaneKeepAidStsMth.fdx_name])
        self.SftyCchLaneKeepAidStsYr = SftyCchLaneKeepAidStsYr(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLaneKeepAidStsYr.fdx_name])
        self.SftyCchLongTermIdxAttention = SftyCchLongTermIdxAttention(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxAttention.fdx_name])
        self.SftyCchLongTermIdxAttentionAvl = SftyCchLongTermIdxAttentionAvl(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxAttentionAvl.fdx_name])
        self.SftyCchLongTermIdxDstToVeh = SftyCchLongTermIdxDstToVeh(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxDstToVeh.fdx_name])
        self.SftyCchLongTermIdxDstToVehAvl = SftyCchLongTermIdxDstToVehAvl(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxDstToVehAvl.fdx_name])
        self.SftyCchLongTermIdxKeepSpdLim = SftyCchLongTermIdxKeepSpdLim(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxKeepSpdLim.fdx_name])
        self.SftyCchLongTermIdxKeepSpdLimAvl = SftyCchLongTermIdxKeepSpdLimAvl(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxKeepSpdLimAvl.fdx_name])
        self.SftyCchLongTermIdxLaneKeep = SftyCchLongTermIdxLaneKeep(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxLaneKeep.fdx_name])
        self.SftyCchLongTermIdxLaneKeepAvl = SftyCchLongTermIdxLaneKeepAvl(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxLaneKeepAvl.fdx_name])
        self.SftyCchLongTermIdxRst = SftyCchLongTermIdxRst(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SftyCchLongTermIdxRst.fdx_name])
        self.SftyCchLongTermIdxTot = SftyCchLongTermIdxTot(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxTot.fdx_name])
        self.SftyCchLongTermIdxTotAvl = SftyCchLongTermIdxTotAvl(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchLongTermIdxTotAvl.fdx_name])
        self.SftyCchPilotAssiStsActv = SftyCchPilotAssiStsActv(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchPilotAssiStsActv.fdx_name])
        self.SftyCchPilotAssiStsDay = SftyCchPilotAssiStsDay(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchPilotAssiStsDay.fdx_name])
        self.SftyCchPilotAssiStsMth = SftyCchPilotAssiStsMth(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchPilotAssiStsMth.fdx_name])
        self.SftyCchPilotAssiStsYr = SftyCchPilotAssiStsYr(self, name_to_item_map['ASDMBackBoneSignalIPdu09::' +SftyCchPilotAssiStsYr.fdx_name])
        self.SftyCchPostTripSts = SftyCchPostTripSts(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SftyCchPostTripSts.fdx_name])
        self.SftySigFaildDetdByAud = SftySigFaildDetdByAud(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +SftySigFaildDetdByAud.fdx_name])
        self.SftySigGroupFromAudSafeSftyAudEna = SftySigGroupFromAudSafeSftyAudEna(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +SftySigGroupFromAudSafeSftyAudEna.fdx_name])
        self.SoundExtActvSts = SoundExtActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SoundExtActvSts.fdx_name])
        self.SpdAlrmActvForRoadSgnInfoPen = SpdAlrmActvForRoadSgnInfoPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +SpdAlrmActvForRoadSgnInfoPen.fdx_name])
        self.SpdAlrmActvForRoadSgnInfoSts = SpdAlrmActvForRoadSgnInfoSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +SpdAlrmActvForRoadSgnInfoSts.fdx_name])
        self.SpdCameraInfoSeldForRoadSgnInfoPen = SpdCameraInfoSeldForRoadSgnInfoPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +SpdCameraInfoSeldForRoadSgnInfoPen.fdx_name])
        self.SpdCameraInfoSeldForRoadSgnInfoSts = SpdCameraInfoSeldForRoadSgnInfoSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +SpdCameraInfoSeldForRoadSgnInfoSts.fdx_name])
        self.SpdInHznPosn = SpdInHznPosn(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +SpdInHznPosn.fdx_name])
        self.SpdLimAdpvSts = SpdLimAdpvSts(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +SpdLimAdpvSts.fdx_name])
        self.SpdLimEfcInHznSeg = SpdLimEfcInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +SpdLimEfcInHznSeg.fdx_name])
        self.SpdLimTypEfcInHznSeg = SpdLimTypEfcInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +SpdLimTypEfcInHznSeg.fdx_name])
        self.SpdLimWarnReq = SpdLimWarnReq(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +SpdLimWarnReq.fdx_name])
        self.SpdUnit = SpdUnit(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +SpdUnit.fdx_name])
        self.SpdUnitInHznData = SpdUnitInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +SpdUnitInHznData.fdx_name])
        self.SpdWarn = SpdWarn(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +SpdWarn.fdx_name])
        self.SpprtForFctInDstToManvLocn = SpprtForFctInDstToManvLocn(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +SpprtForFctInDstToManvLocn.fdx_name])
        self.SpprtForFctInTireCircumCalByNav = SpprtForFctInTireCircumCalByNav(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +SpprtForFctInTireCircumCalByNav.fdx_name])
        self.SrvRst = SrvRst(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +SrvRst.fdx_name])
        self.SrvTrig = SrvTrig(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +SrvTrig.fdx_name])
        self.SteerAsscLvl = SteerAsscLvl(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SteerAsscLvl.fdx_name])
        self.SteerAssiActvForAutDrvPen = SteerAssiActvForAutDrvPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +SteerAssiActvForAutDrvPen.fdx_name])
        self.SteerAssiActvForAutDrvSts = SteerAssiActvForAutDrvSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +SteerAssiActvForAutDrvSts.fdx_name])
        self.SteerMod = SteerMod(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SteerMod.fdx_name])
        self.SteerOvrdWarnReqForAutDrv = SteerOvrdWarnReqForAutDrv(self, name_to_item_map['ASDMBackBoneSignalIPdu01::' +SteerOvrdWarnReqForAutDrv.fdx_name])
        self.SteerSetgPen = SteerSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +SteerSetgPen.fdx_name])
        self.SteerStsForAutDrv = SteerStsForAutDrv(self, name_to_item_map['ASDMBackBoneSignalIPdu01::' +SteerStsForAutDrv.fdx_name])
        self.SteerWarnReqForAutDrv = SteerWarnReqForAutDrv(self, name_to_item_map['ASDMBackBoneSignalIPdu02::' +SteerWarnReqForAutDrv.fdx_name])
        self.SteerWarnReqForLaneKeepAid = SteerWarnReqForLaneKeepAid(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +SteerWarnReqForLaneKeepAid.fdx_name])
        self.SteerWhlAgSafe = SteerWhlAgSafe(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +SteerWhlAgSafe.fdx_name])
        self.SteerWhlAgSpdSafe = SteerWhlAgSpdSafe(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +SteerWhlAgSpdSafe.fdx_name])
        self.SteerWhlHeatgDurgClimaEnad = SteerWhlHeatgDurgClimaEnad(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +SteerWhlHeatgDurgClimaEnad.fdx_name])
        self.SteerWhlHeatgOnReq = SteerWhlHeatgOnReq(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +SteerWhlHeatgOnReq.fdx_name])
        self.SteerWhlHeatgStrtAutCdnOk = SteerWhlHeatgStrtAutCdnOk(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +SteerWhlHeatgStrtAutCdnOk.fdx_name])
        self.SteerWhlSnsrQf = SteerWhlSnsrQf(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +SteerWhlSnsrQf.fdx_name])
        self.SteerWhlSwtPwr = SteerWhlSwtPwr(self, name_to_item_map['DIMBackBoneSignalIPdu04::' +SteerWhlSwtPwr.fdx_name])
        self.StopStrtInhb = StopStrtInhb(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +StopStrtInhb.fdx_name])
        self.StrtInProgs = StrtInProgs(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +StrtInProgs.fdx_name])
        self.SunRoofPosnSts = SunRoofPosnSts(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +SunRoofPosnSts.fdx_name])
        self.SuspCtrlSetg = SuspCtrlSetg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +SuspCtrlSetg.fdx_name])
        self.SuspIdPen = SuspIdPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +SuspIdPen.fdx_name])
        self.SwtAtCenSts = SwtAtCenSts(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +SwtAtCenSts.fdx_name])
        self.SwtForPassAirbCutOffSt = SwtForPassAirbCutOffSt(self, name_to_item_map['SRSBackboneSignalIPdu03::' +SwtForPassAirbCutOffSt.fdx_name])
        self.TankFlapSts = TankFlapSts(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +TankFlapSts.fdx_name])
        self.TelmSts = TelmSts(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +TelmSts.fdx_name])
        self.TiAndDateVld = TiAndDateVld(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TiAndDateVld.fdx_name])
        self.TiForDayInPosnFromSatlt = TiForDayInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +TiForDayInPosnFromSatlt.fdx_name])
        self.TiForHrInPosnFromSatlt = TiForHrInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +TiForHrInPosnFromSatlt.fdx_name])
        self.TiForMinsInPosnFromSatlt = TiForMinsInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +TiForMinsInPosnFromSatlt.fdx_name])
        self.TiForMthInPosnFromSatlt = TiForMthInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +TiForMthInPosnFromSatlt.fdx_name])
        self.TiForSecInPosnFromSatlt = TiForSecInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +TiForSecInPosnFromSatlt.fdx_name])
        self.TiForYrInPosnFromSatlt = TiForYrInPosnFromSatlt(self, name_to_item_map['VCMBackboneSignalIPdu01::' +TiForYrInPosnFromSatlt.fdx_name])
        self.TiGapAdpvSeldPen = TiGapAdpvSeldPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TiGapAdpvSeldPen.fdx_name])
        self.TiGapAdpvSeldSts = TiGapAdpvSeldSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TiGapAdpvSeldSts.fdx_name])
        self.TiGapLimdPen = TiGapLimdPen(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +TiGapLimdPen.fdx_name])
        self.TiGapLimdSts = TiGapLimdSts(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +TiGapLimdSts.fdx_name])
        self.TireCircumInTireCircumCalByNav = TireCircumInTireCircumCalByNav(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +TireCircumInTireCircumCalByNav.fdx_name])
        self.TireMonCalReq = TireMonCalReq(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +TireMonCalReq.fdx_name])
        self.TireMonrDispReq = TireMonrDispReq(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +TireMonrDispReq.fdx_name])
        self.TirePAbsltValForFrntLe = TirePAbsltValForFrntLe(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltValForFrntLe.fdx_name])
        self.TirePAbsltValForReLe = TirePAbsltValForReLe(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltValForReLe.fdx_name])
        self.TirePAbsltValForReRi = TirePAbsltValForReRi(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltValForReRi.fdx_name])
        self.TirePAbsltValFrntRiTirePAbsltVal1 = TirePAbsltValFrntRiTirePAbsltVal1(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltValFrntRiTirePAbsltVal1.fdx_name])
        self.TirePAbsltValFrntRiTirePPosn = TirePAbsltValFrntRiTirePPosn(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltValFrntRiTirePPosn.fdx_name])
        self.TirePAbsltValQFForFrnRi = TirePAbsltValQFForFrnRi(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltValQFForFrnRi.fdx_name])
        self.TirePAbsltValQfForReLe = TirePAbsltValQfForReLe(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltValQfForReLe.fdx_name])
        self.TirePAbsltValQfForReRi = TirePAbsltValQfForReRi(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltValQfForReRi.fdx_name])
        self.TirePAbsltVaQflForFrntLe = TirePAbsltVaQflForFrntLe(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePAbsltVaQflForFrntLe.fdx_name])
        self.TirePCalSts = TirePCalSts(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePCalSts.fdx_name])
        self.TirePFrntLe = TirePFrntLe(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePFrntLe.fdx_name])
        self.TirePFrntRi = TirePFrntRi(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePFrntRi.fdx_name])
        self.TirePMonSts = TirePMonSts(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePMonSts.fdx_name])
        self.TirePPosnForFrntLe = TirePPosnForFrntLe(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePPosnForFrntLe.fdx_name])
        self.TirePPosnForReLe = TirePPosnForReLe(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePPosnForReLe.fdx_name])
        self.TirePPosnForReRi = TirePPosnForReRi(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePPosnForReRi.fdx_name])
        self.TirePReLe = TirePReLe(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePReLe.fdx_name])
        self.TirePReRi = TirePReRi(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +TirePReRi.fdx_name])
        self.TnlEntryOrEnd = TnlEntryOrEnd(self, name_to_item_map['ASDMBackBoneSignalIPdu02::' +TnlEntryOrEnd.fdx_name])
        self.TnlInHznSeg = TnlInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +TnlInHznSeg.fdx_name])
        self.TrfcLiSpprtActv = TrfcLiSpprtActv(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +TrfcLiSpprtActv.fdx_name])
        self.TrfcLiSpprtSts = TrfcLiSpprtSts(self, name_to_item_map['ASDMBackBoneSignalIPdu08::' +TrfcLiSpprtSts.fdx_name])
        self.TrfcRglnSeldForRoadSgnInfoPen = TrfcRglnSeldForRoadSgnInfoPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TrfcRglnSeldForRoadSgnInfoPen.fdx_name])
        self.TrfcRglnSeldForRoadSgnInfoSts = TrfcRglnSeldForRoadSgnInfoSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TrfcRglnSeldForRoadSgnInfoSts.fdx_name])
        self.TripModSeln = TripModSeln(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TripModSeln.fdx_name])
        self.TrlrLampActvtChk = TrlrLampActvtChk(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +TrlrLampActvtChk.fdx_name])
        self.TrlrLampChkAutReqPen = TrlrLampChkAutReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +TrlrLampChkAutReqPen.fdx_name])
        self.TrlrLampChkAutReqSts = TrlrLampChkAutReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +TrlrLampChkAutReqSts.fdx_name])
        self.TrlrLampChkRemReq = TrlrLampChkRemReq(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +TrlrLampChkRemReq.fdx_name])
        self.TrlrLampChkSts = TrlrLampChkSts(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +TrlrLampChkSts.fdx_name])
        self.TrlrPrsnt = TrlrPrsnt(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +TrlrPrsnt.fdx_name])
        self.TrSts = TrSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +TrSts.fdx_name])
        self.TUnit = TUnit(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +TUnit.fdx_name])
        self.TurnAgInHznEdge = TurnAgInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +TurnAgInHznEdge.fdx_name])
        self.TurnAutFlsgReqPen = TurnAutFlsgReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +TurnAutFlsgReqPen.fdx_name])
        self.TurnAutFlsgReqSts = TurnAutFlsgReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +TurnAutFlsgReqSts.fdx_name])
        self.TurnSpdWarnActvPen = TurnSpdWarnActvPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TurnSpdWarnActvPen.fdx_name])
        self.TurnSpdWarnActvSts = TurnSpdWarnActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TurnSpdWarnActvSts.fdx_name])
        self.TurnSpdWarnSnvtyActPen = TurnSpdWarnSnvtyActPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +TurnSpdWarnSnvtyActPen.fdx_name])
        self.TurnSpdWarnSnvtyActsSts = TurnSpdWarnSnvtyActsSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +TurnSpdWarnSnvtyActsSts.fdx_name])
        self.TwliBriRaw = TwliBriRaw(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +TwliBriRaw.fdx_name])
        self.TwliBriRawQf = TwliBriRawQf(self, name_to_item_map['CEMBackBoneSignalIpdu02::' +TwliBriRawQf.fdx_name])
        self.TwliBriSts = TwliBriSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +TwliBriSts.fdx_name])
        self.TxPrevInHznEdge = TxPrevInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +TxPrevInHznEdge.fdx_name])
        self.TxPrevInHznProfLong = TxPrevInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TxPrevInHznProfLong.fdx_name])
        self.TxPrevInHznProfSho = TxPrevInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +TxPrevInHznProfSho.fdx_name])
        self.TxPrevInHznSeg = TxPrevInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +TxPrevInHznSeg.fdx_name])
        self.TxVersInHznData = TxVersInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +TxVersInHznData.fdx_name])
        self.TypOfWayInHznEdge = TypOfWayInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +TypOfWayInHznEdge.fdx_name])
        self.TypOfWayInHznSeg = TypOfWayInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +TypOfWayInHznSeg.fdx_name])
        self.UkwnCptReqToInfoMgrByte0 = UkwnCptReqToInfoMgrByte0(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToInfoMgrByte0.fdx_name])
        self.UkwnCptReqToInfoMgrByte1 = UkwnCptReqToInfoMgrByte1(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToInfoMgrByte1.fdx_name])
        self.UkwnCptReqToInfoMgrByte2 = UkwnCptReqToInfoMgrByte2(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToInfoMgrByte2.fdx_name])
        self.UkwnCptReqToInfoMgrByte3 = UkwnCptReqToInfoMgrByte3(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToInfoMgrByte3.fdx_name])
        self.UkwnCptReqToInfoMgrByte4 = UkwnCptReqToInfoMgrByte4(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToInfoMgrByte4.fdx_name])
        self.UkwnCptReqToInfoMgrByte5 = UkwnCptReqToInfoMgrByte5(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToInfoMgrByte5.fdx_name])
        self.UkwnCptReqToInfoMgrByte6 = UkwnCptReqToInfoMgrByte6(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToInfoMgrByte6.fdx_name])
        self.UkwnCptReqToInfoMgrByte7 = UkwnCptReqToInfoMgrByte7(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToInfoMgrByte7.fdx_name])
        self.UkwnCptReqToSoundMgrByte0 = UkwnCptReqToSoundMgrByte0(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToSoundMgrByte0.fdx_name])
        self.UkwnCptReqToSoundMgrByte1 = UkwnCptReqToSoundMgrByte1(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToSoundMgrByte1.fdx_name])
        self.UkwnCptReqToSoundMgrByte2 = UkwnCptReqToSoundMgrByte2(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToSoundMgrByte2.fdx_name])
        self.UkwnCptReqToSoundMgrByte3 = UkwnCptReqToSoundMgrByte3(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToSoundMgrByte3.fdx_name])
        self.UkwnCptReqToSoundMgrByte4 = UkwnCptReqToSoundMgrByte4(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToSoundMgrByte4.fdx_name])
        self.UkwnCptReqToSoundMgrByte5 = UkwnCptReqToSoundMgrByte5(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToSoundMgrByte5.fdx_name])
        self.UkwnCptReqToSoundMgrByte6 = UkwnCptReqToSoundMgrByte6(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToSoundMgrByte6.fdx_name])
        self.UkwnCptReqToSoundMgrByte7 = UkwnCptReqToSoundMgrByte7(self, name_to_item_map['CEMBackBoneSignalIpdu13::' +UkwnCptReqToSoundMgrByte7.fdx_name])
        self.UkwnCptRespFromInfoMgrByte0 = UkwnCptRespFromInfoMgrByte0(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromInfoMgrByte0.fdx_name])
        self.UkwnCptRespFromInfoMgrByte1 = UkwnCptRespFromInfoMgrByte1(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromInfoMgrByte1.fdx_name])
        self.UkwnCptRespFromInfoMgrByte2 = UkwnCptRespFromInfoMgrByte2(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromInfoMgrByte2.fdx_name])
        self.UkwnCptRespFromInfoMgrByte3 = UkwnCptRespFromInfoMgrByte3(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromInfoMgrByte3.fdx_name])
        self.UkwnCptRespFromInfoMgrByte4 = UkwnCptRespFromInfoMgrByte4(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromInfoMgrByte4.fdx_name])
        self.UkwnCptRespFromInfoMgrByte5 = UkwnCptRespFromInfoMgrByte5(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromInfoMgrByte5.fdx_name])
        self.UkwnCptRespFromInfoMgrByte6 = UkwnCptRespFromInfoMgrByte6(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromInfoMgrByte6.fdx_name])
        self.UkwnCptRespFromInfoMgrByte7 = UkwnCptRespFromInfoMgrByte7(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromInfoMgrByte7.fdx_name])
        self.UkwnCptRespFromSoundMgrByte0 = UkwnCptRespFromSoundMgrByte0(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromSoundMgrByte0.fdx_name])
        self.UkwnCptRespFromSoundMgrByte1 = UkwnCptRespFromSoundMgrByte1(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromSoundMgrByte1.fdx_name])
        self.UkwnCptRespFromSoundMgrByte2 = UkwnCptRespFromSoundMgrByte2(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromSoundMgrByte2.fdx_name])
        self.UkwnCptRespFromSoundMgrByte3 = UkwnCptRespFromSoundMgrByte3(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromSoundMgrByte3.fdx_name])
        self.UkwnCptRespFromSoundMgrByte4 = UkwnCptRespFromSoundMgrByte4(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromSoundMgrByte4.fdx_name])
        self.UkwnCptRespFromSoundMgrByte5 = UkwnCptRespFromSoundMgrByte5(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromSoundMgrByte5.fdx_name])
        self.UkwnCptRespFromSoundMgrByte6 = UkwnCptRespFromSoundMgrByte6(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromSoundMgrByte6.fdx_name])
        self.UkwnCptRespFromSoundMgrByte7 = UkwnCptRespFromSoundMgrByte7(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UkwnCptRespFromSoundMgrByte7.fdx_name])
        self.UnlckFbVisReqPen = UnlckFbVisReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UnlckFbVisReqPen.fdx_name])
        self.UnlckFbVisReqSts = UnlckFbVisReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UnlckFbVisReqSts.fdx_name])
        self.UnlckKeylsReqPen = UnlckKeylsReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UnlckKeylsReqPen.fdx_name])
        self.UnlckKeylsReqSts = UnlckKeylsReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UnlckKeylsReqSts.fdx_name])
        self.UnlckRemReqPen = UnlckRemReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +UnlckRemReqPen.fdx_name])
        self.UnlckRemReqSts = UnlckRemReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +UnlckRemReqSts.fdx_name])
        self.UpdInHznEdge = UpdInHznEdge(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +UpdInHznEdge.fdx_name])
        self.UpdInHznProfLong = UpdInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +UpdInHznProfLong.fdx_name])
        self.UpdInHznProfSho = UpdInHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +UpdInHznProfSho.fdx_name])
        self.UpdInHznSeg = UpdInHznSeg(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +UpdInHznSeg.fdx_name])
        self.UsgModSts = UsgModSts(self, name_to_item_map['CEMBackBoneSignalIpdu01::' +UsgModSts.fdx_name])
        self.UsrSeldDrvrSpprtFct = UsrSeldDrvrSpprtFct(self, name_to_item_map['IHUBackBoneSignalIPdu07::' +UsrSeldDrvrSpprtFct.fdx_name])
        self.UsrSetSpdForKeySpdLimnPen = UsrSetSpdForKeySpdLimnPen(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UsrSetSpdForKeySpdLimnPen.fdx_name])
        self.UsrSetSpdForKeySpdLimnSts = UsrSetSpdForKeySpdLimnSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +UsrSetSpdForKeySpdLimnSts.fdx_name])
        self.UsrSetSpdForKeySpdWarn1 = UsrSetSpdForKeySpdWarn1(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UsrSetSpdForKeySpdWarn1.fdx_name])
        self.UsrSetSpdForKeySpdWarn2 = UsrSetSpdForKeySpdWarn2(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UsrSetSpdForKeySpdWarn2.fdx_name])
        self.UsrSetSpdForKeySpdWarn3 = UsrSetSpdForKeySpdWarn3(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UsrSetSpdForKeySpdWarn3.fdx_name])
        self.UsrSetSpdForKeySpdWarn4 = UsrSetSpdForKeySpdWarn4(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UsrSetSpdForKeySpdWarn4.fdx_name])
        self.UsrSetSpdForKeySpdWarn5 = UsrSetSpdForKeySpdWarn5(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UsrSetSpdForKeySpdWarn5.fdx_name])
        self.UsrSetSpdForKeySpdWarn6 = UsrSetSpdForKeySpdWarn6(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UsrSetSpdForKeySpdWarn6.fdx_name])
        self.UsrSetSpdForKeySpdWarnPen = UsrSetSpdForKeySpdWarnPen(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +UsrSetSpdForKeySpdWarnPen.fdx_name])
        self.Val0InHznProfSho = Val0InHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +Val0InHznProfSho.fdx_name])
        self.Val1InHznProfSho = Val1InHznProfSho(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +Val1InHznProfSho.fdx_name])
        self.ValInHznProfLong = ValInHznProfLong(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +ValInHznProfLong.fdx_name])
        self.VehActvMsgToDrvr = VehActvMsgToDrvr(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +VehActvMsgToDrvr.fdx_name])
        self.VehBattUSysU = VehBattUSysU(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VehBattUSysU.fdx_name])
        self.VehBattUSysUQf = VehBattUSysUQf(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VehBattUSysUQf.fdx_name])
        self.VehCfgPrmBlk = VehCfgPrmBlk(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +VehCfgPrmBlk.fdx_name])
        self.VehCfgPrmVal1 = VehCfgPrmVal1(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +VehCfgPrmVal1.fdx_name])
        self.VehCfgPrmVal2 = VehCfgPrmVal2(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +VehCfgPrmVal2.fdx_name])
        self.VehCfgPrmVal3 = VehCfgPrmVal3(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +VehCfgPrmVal3.fdx_name])
        self.VehCfgPrmVal4 = VehCfgPrmVal4(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +VehCfgPrmVal4.fdx_name])
        self.VehCfgPrmVal5 = VehCfgPrmVal5(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +VehCfgPrmVal5.fdx_name])
        self.VehCfgPrmVal6 = VehCfgPrmVal6(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +VehCfgPrmVal6.fdx_name])
        self.VehCfgPrmVal7 = VehCfgPrmVal7(self, name_to_item_map['CEMBackBoneSignalIpdu04::' +VehCfgPrmVal7.fdx_name])
        self.VehDecelCtrlSetgNormSpt = VehDecelCtrlSetgNormSpt(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +VehDecelCtrlSetgNormSpt.fdx_name])
        self.VehDecelCtrlSetgPen = VehDecelCtrlSetgPen(self, name_to_item_map['IHUBackBoneSignalIPdu06::' +VehDecelCtrlSetgPen.fdx_name])
        self.VehM = VehM(self, name_to_item_map['VDDMBackBoneSignalIpdu06::' +VehM.fdx_name])
        self.VehMNomTrlrM = VehMNomTrlrM(self, name_to_item_map['VDDMBackBoneSignalIpdu06::' +VehMNomTrlrM.fdx_name])
        self.VehMQly = VehMQly(self, name_to_item_map['VDDMBackBoneSignalIpdu06::' +VehMQly.fdx_name])
        self.VehMtnStSafe = VehMtnStSafe(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +VehMtnStSafe.fdx_name])
        self.VehSpdAvg = VehSpdAvg(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +VehSpdAvg.fdx_name])
        self.VehSpdAvgUnit = VehSpdAvgUnit(self, name_to_item_map['DIMBackBoneSignalIPdu05::' +VehSpdAvgUnit.fdx_name])
        self.VehSpdCtrlActvSts = VehSpdCtrlActvSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +VehSpdCtrlActvSts.fdx_name])
        self.VehSpdIndcdUnit = VehSpdIndcdUnit(self, name_to_item_map['DIMBackBoneSignalIPdu01::' +VehSpdIndcdUnit.fdx_name])
        self.VehSpdIndcdVal = VehSpdIndcdVal(self, name_to_item_map['DIMBackBoneSignalIPdu01::' +VehSpdIndcdVal.fdx_name])
        self.VehSpdLgtSafe = VehSpdLgtSafe(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +VehSpdLgtSafe.fdx_name])
        self.VehSpdLgtSafeQf = VehSpdLgtSafeQf(self, name_to_item_map['VDDMBackBoneSignalIPdu01::' +VehSpdLgtSafeQf.fdx_name])
        self.VFCVectorIHUGrp1 = VFCVectorIHUGrp1(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +VFCVectorIHUGrp1.fdx_name])
        self.VFCVectorIHUGrp2 = VFCVectorIHUGrp2(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +VFCVectorIHUGrp2.fdx_name])
        self.VFCVectorIHUGrp3 = VFCVectorIHUGrp3(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +VFCVectorIHUGrp3.fdx_name])
        self.VinBlk = VinBlk(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VinBlk.fdx_name])
        self.VinPosn1 = VinPosn1(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VinPosn1.fdx_name])
        self.VinPosn2 = VinPosn2(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VinPosn2.fdx_name])
        self.VinPosn3 = VinPosn3(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VinPosn3.fdx_name])
        self.VinPosn4 = VinPosn4(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VinPosn4.fdx_name])
        self.VinPosn5 = VinPosn5(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VinPosn5.fdx_name])
        self.VinPosn6 = VinPosn6(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VinPosn6.fdx_name])
        self.VinPosn7 = VinPosn7(self, name_to_item_map['CEMBackBoneSignalIpdu07::' +VinPosn7.fdx_name])
        self.VisnAgWideCfgCtraInfo = VisnAgWideCfgCtraInfo(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgCtraInfo.fdx_name])
        self.VisnAgWideCfgDstInfoFrnt = VisnAgWideCfgDstInfoFrnt(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgDstInfoFrnt.fdx_name])
        self.VisnAgWideCfgDstInfoRe = VisnAgWideCfgDstInfoRe(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgDstInfoRe.fdx_name])
        self.VisnAgWideCfgDstInfoSide = VisnAgWideCfgDstInfoSide(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgDstInfoSide.fdx_name])
        self.VisnAgWideCfgObjDetn = VisnAgWideCfgObjDetn(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgObjDetn.fdx_name])
        self.VisnAgWideCfgSteerPahFrnt = VisnAgWideCfgSteerPahFrnt(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgSteerPahFrnt.fdx_name])
        self.VisnAgWideCfgSteerPahRe = VisnAgWideCfgSteerPahRe(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgSteerPahRe.fdx_name])
        self.VisnAgWideCfgSteerPahSide = VisnAgWideCfgSteerPahSide(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgSteerPahSide.fdx_name])
        self.VisnAgWideCfgTwbrPah = VisnAgWideCfgTwbrPah(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnAgWideCfgTwbrPah.fdx_name])
        self.VisnAgWideSts = VisnAgWideSts(self, name_to_item_map['ASDMBackBoneSignalIPdu06::' +VisnAgWideSts.fdx_name])
        self.VisnImgAgWideInUse = VisnImgAgWideInUse(self, name_to_item_map['ASDMBackBoneSignalIPdu07::' +VisnImgAgWideInUse.fdx_name])
        self.VisnImgAgWideReq = VisnImgAgWideReq(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnImgAgWideReq.fdx_name])
        self.VisnImgCamReqCamFrnt = VisnImgCamReqCamFrnt(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnImgCamReqCamFrnt.fdx_name])
        self.VisnImgCamReqCamLe = VisnImgCamReqCamLe(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnImgCamReqCamLe.fdx_name])
        self.VisnImgCamReqCamRe = VisnImgCamReqCamRe(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnImgCamReqCamRe.fdx_name])
        self.VisnImgCamReqCamRi = VisnImgCamReqCamRi(self, name_to_item_map['IHUBackboneSignalIPdu01::' +VisnImgCamReqCamRi.fdx_name])
        self.VolUnit = VolUnit(self, name_to_item_map['IHUBackBoneSignalIPdu02::' +VolUnit.fdx_name])
        self.WarnTypForLaneChgWarnPen = WarnTypForLaneChgWarnPen(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +WarnTypForLaneChgWarnPen.fdx_name])
        self.WarnTypForLaneChgWarnSts = WarnTypForLaneChgWarnSts(self, name_to_item_map['IHUBackBoneSignalIPdu04::' +WarnTypForLaneChgWarnSts.fdx_name])
        self.WarnTypForLaneKeepAidPen = WarnTypForLaneKeepAidPen(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +WarnTypForLaneKeepAidPen.fdx_name])
        self.WarnTypForLaneKeepAidSts = WarnTypForLaneKeepAidSts(self, name_to_item_map['IHUBackBoneSignalIPdu03::' +WarnTypForLaneKeepAidSts.fdx_name])
        self.WhlCircum = WhlCircum(self, name_to_item_map['CEMBackBoneSignalIpdu03::' +WhlCircum.fdx_name])
        self.WhlMotSysSpdAct = WhlMotSysSpdAct(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +WhlMotSysSpdAct.fdx_name])
        self.WhlMotSysTqEst = WhlMotSysTqEst(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +WhlMotSysTqEst.fdx_name])
        self.WhlMotSysTqEstQlyFac = WhlMotSysTqEstQlyFac(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +WhlMotSysTqEstQlyFac.fdx_name])
        self.WhlMotSysTqEstSpdLimn = WhlMotSysTqEstSpdLimn(self, name_to_item_map['VDDMBackBoneSignalIPdu03::' +WhlMotSysTqEstSpdLimn.fdx_name])
        self.WhlRotToothCntrFrntLe = WhlRotToothCntrFrntLe(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +WhlRotToothCntrFrntLe.fdx_name])
        self.WhlRotToothCntrFrntRi = WhlRotToothCntrFrntRi(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +WhlRotToothCntrFrntRi.fdx_name])
        self.WhlRotToothCntrReLe = WhlRotToothCntrReLe(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +WhlRotToothCntrReLe.fdx_name])
        self.WhlRotToothCntrReRi = WhlRotToothCntrReRi(self, name_to_item_map['VDDMBackBoneSignalIPdu02::' +WhlRotToothCntrReRi.fdx_name])
        self.WinPosnStsAtDrvrRe = WinPosnStsAtDrvrRe(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +WinPosnStsAtDrvrRe.fdx_name])
        self.WinPosnStsDrv = WinPosnStsDrv(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +WinPosnStsDrv.fdx_name])
        self.WinPosnStsPass = WinPosnStsPass(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +WinPosnStsPass.fdx_name])
        self.WinPosnStsRePass = WinPosnStsRePass(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +WinPosnStsRePass.fdx_name])
        self.WipgInfoWipgSpdInfo = WipgInfoWipgSpdInfo(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +WipgInfoWipgSpdInfo.fdx_name])
        self.WipgInfoWiprActv = WipgInfoWiprActv(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +WipgInfoWiprActv.fdx_name])
        self.WipgInfoWiprInWipgAr = WipgInfoWiprInWipgAr(self, name_to_item_map['CEMBackBoneSignalIpdu05::' +WipgInfoWiprInWipgAr.fdx_name])
        self.WiprFrntSrvModReq = WiprFrntSrvModReq(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +WiprFrntSrvModReq.fdx_name])
        self.WiprInPosnForSrv = WiprInPosnForSrv(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +WiprInPosnForSrv.fdx_name])
        self.WiprReAutReqPen = WiprReAutReqPen(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +WiprReAutReqPen.fdx_name])
        self.WiprReAutReqSts = WiprReAutReqSts(self, name_to_item_map['IHUBackBoneSignalIPdu08::' +WiprReAutReqSts.fdx_name])
        self.WshrFldSts = WshrFldSts(self, name_to_item_map['CEMBackBoneSignalIpdu11::' +WshrFldSts.fdx_name])
        self.YawRate1 = YawRate1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +YawRate1.fdx_name])
        self.YawRate1Qf1 = YawRate1Qf1(self, name_to_item_map['SRSBackBoneSignalIPdu04::' +YawRate1Qf1.fdx_name])
        self.Yr = Yr(self, name_to_item_map['CEMBackBoneSignalIpdu06::' +Yr.fdx_name])
        self.Yr1 = Yr1(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +Yr1.fdx_name])
        self.YrVersOfMapInHznData = YrVersOfMapInHznData(self, name_to_item_map['IHUBackBoneSignalIPdu05::' +YrVersOfMapInHznData.fdx_name])


    def verify_simulation_version(self):
        # SPA2210/FR_Body_LIN/SimulationDB/Simulation.vsysvar
        EXPECTED_VERSION = 10
        simulation_version = next((s for s in self.sysvar_list if s.msg_or_namespace == "Configuration::VersionInfo" and s.name == "SimulationVersion"))
        deadline = time.time() + 20
        while simulation_version.value_raw != EXPECTED_VERSION and time.time() < deadline:
            self.connection.send_data_request(simulation_version.parent_group.group_id)
            time.sleep(0.5)
        if simulation_version.value_raw != EXPECTED_VERSION:
            raise Exception("Simulation version mismatch! CANoe simulation version=%r, expected version=%d" % (simulation_version.value_raw, EXPECTED_VERSION))

    def close(self):
        if self.connected:
            self.connection.confirmed_stop()
            self.connection.close()

class BaseIntegerSender(object):
    fdx_name = ""

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
        self.is_sending = False

    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min <= physical <= cls.max)
        return physical

    def set(self, value_physical):
        self.item.value_raw = self.p2r(value_physical)

    def send(self, value_physical):
        self.item.value_raw = self.p2r(value_physical)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group.group_id, self.item.parent_group.size, self.item.parent_group.build_data())
        self.signal_interface.logger.debug('send %s=%d',self.fdx_name, value_physical)

    def get(self):
        value = self.r2p(self.item.value_raw)
        self.signal_interface.logger.debug('get %s=%d',self.fdx_name, value)
        return value

    def add_timer(self, time_ms, func):
        if self.is_sending:
            self.t = threading.Timer(float(time_ms)/1000, func)
            self.t.start()

    def send_repetitive(self, value_physical):
        self.is_sending = True
        def bind_send():
            self.send(value_physical)
            self.add_timer(100.0, bind_send)
        bind_send()

    def stop_send(self):
        self.is_sending = False
        self.t.cancel()

class BaseFloatSender(object):
    fdx_name = ""

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
        self.is_sending = False

    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min <= physical <= cls.max)
        return (physical - cls.offset) / cls.scale

    def set(self, value_physical):
        self.item.value_raw = self.p2r(value_physical)

    def send(self, value_physical):
        self.item.value_raw = self.p2r(value_physical)
        self.signal_interface.connection.send_data_exchange(self.item.parent_group.group_id, self.item.parent_group.size, self.item.parent_group.build_data())
        self.signal_interface.logger.debug('send %s=%d',self.fdx_name, value_physical)

    def get(self):
        value = self.r2p(self.item.value_raw)
        self.signal_interface.logger.debug('get %s=%d',self.fdx_name, value)
        return value

    def add_timer(self, time_ms, func):
        if self.is_sending:
            self.t = threading.Timer(float(time_ms)/1000, func)
            self.t.start()

    def send_repetitive(self, value_physical):
        self.is_sending = True
        def bind_send():
            self.send(value_physical)
            self.add_timer(100.0, bind_send)
        bind_send()

    def stop_send(self):
        self.is_sending = False
        self.t.cancel()

class BaseBoolSender(object):
    fdx_name = ""

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
        self.is_sending = False

    def set(self, value_physical):
        self.item.value_raw = value_physical

    def send(self, value_physical):
        self.item.value_raw = value_physical
        self.signal_interface.connection.send_data_exchange(self.item.parent_group.group_id, self.item.parent_group.size, self.item.parent_group.build_data())
        self.signal_interface.logger.debug('send %s=%d',self.fdx_name, value_physical)

    def get(self):
        value = self.item.value_raw
        self.signal_interface.logger.debug('get %s=%d',self.fdx_name, value)
        return value

    def add_timer(self, time_ms, func):
        if self.is_sending:
            self.t = threading.Timer(float(time_ms)/1000, func)
            self.t.start()

    def send_repetitive(self, value_physical):
        self.is_sending = True
        def bind_send():
            self.send(value_physical)
            self.add_timer(100.0, bind_send)
        bind_send()

    def stop_send(self):
        self.is_sending = False
        self.t.cancel()

class BaseEnumSender(object):
    fdx_name = ""

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
        self.is_sending = False

    def set(self, value_physical):
        self.item.value_raw = value_physical

    def send(self, value_physical):
        self.item.value_raw = value_physical
        self.signal_interface.connection.send_data_exchange(self.item.parent_group.group_id, self.item.parent_group.size, self.item.parent_group.build_data())
        self.signal_interface.logger.debug('send %s=%d',self.fdx_name, value_physical)

    def get(self):
        self.signal_interface.logger.debug('get %s=%d',self.fdx_name, self.item.value_raw)
        return self.item.value_raw

    def add_timer(self, time_ms, func):
        if self.is_sending:
            self.t = threading.Timer(float(time_ms)/1000, func)
            self.t.start()

    def send_repetitive(self, value_physical):
        self.is_sending = True
        def bind_send():
            self.send(value_physical)
            self.add_timer(100.0, bind_send)
        bind_send()

    def stop_send(self):
        self.is_sending = False
        self.t.cancel()


class BaseArraySender(object):
    fdx_name = ""

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        self.signal_interface = signal_interface
        self.item = item
        self.is_sending = False

    def set(self, value_physical):
        assert len(value_physical) == self.array_length
        self.item.value_raw = value_physical

    def send(self, value_physical):
        assert len(value_physical) == self.array_length
        self.item.value_raw = value_physical
        self.signal_interface.connection.send_data_exchange(self.item.parent_group.group_id, self.item.parent_group.size, self.item.parent_group.build_data())
        self.signal_interface.logger.debug('send %%s=%%d',self.fdx_name, value_physical)

    def get(self):
        self.signal_interface.logger.debug('get %%s=%%d',self.fdx_name, self.item.value_raw)
        return self.item.value_raw

    def add_timer(self, time_ms, func):
        if self.is_sending:
            self.t = threading.Timer(float(time_ms)/1000, func)
            self.t.start()

    def send_repetitive(self, value_physical):
        self.is_sending = True
        def bind_send():
            self.send(value_physical)
            self.add_timer(100.0, bind_send)
        bind_send()

    def stop_send(self):
        self.is_sending = False
        self.t.close()

####################################################################################################


# Controls the ABS warning indication to the driver.
class AbsWarnIndcnReq(BaseEnumSender):
    de_name     = "BrkAndAbsWarnIndcnReq.AbsWarnIndcnReq"
    fdx_name    = "AbsWarnIndcnReq"
    fdx_groupid = 3240
    class map:
       AbsWarnIndcnOnReq = 0
       AbsWarnIndcnFlsgReq = 1
       Resd2 = 2
       AbsWarnIndcnOffReq = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AbsWarnIndcnReq, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class AccAdprSpdLimActvPen(BaseEnumSender):
    de_name     = "AccAdprSpdLimActv.Pen"
    fdx_name    = "AccAdprSpdLimActvPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AccAdprSpdLimActvPen, self).__init__(signal_interface, item)


class AccAdprSpdLimActvSts(BaseEnumSender):
    de_name     = "AccAdprSpdLimActv.Sts"
    fdx_name    = "AccAdprSpdLimActvSts"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AccAdprSpdLimActvSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class AccAdprTurnSpdActvPen(BaseEnumSender):
    de_name     = "AccAdprTurnSpdActv.Pen"
    fdx_name    = "AccAdprTurnSpdActvPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AccAdprTurnSpdActvPen, self).__init__(signal_interface, item)


class AccAdprTurnSpdActvSts(BaseEnumSender):
    de_name     = "AccAdprTurnSpdActv.Sts"
    fdx_name    = "AccAdprTurnSpdActvSts"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AccAdprTurnSpdActvSts, self).__init__(signal_interface, item)


class AccAutResuWarnReq(BaseEnumSender):
    de_name     = "AccAutResuWarnReq"
    fdx_name    = "AccAutResuWarnReq"
    fdx_groupid = 3190
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AccAutResuWarnReq, self).__init__(signal_interface, item)


class AccSts(BaseEnumSender):
    de_name     = "AccSts"
    fdx_name    = "AccSts"
    fdx_groupid = 3230
    class map:
       Off = 1
       Stb = 2
       Stop = 3
       StopTmp = 4
       Actv = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AccSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ActvnOfCrsEcoPen(BaseEnumSender):
    de_name     = "ActvnOfCrsEco.Pen"
    fdx_name    = "ActvnOfCrsEcoPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ActvnOfCrsEcoPen, self).__init__(signal_interface, item)


class ActvnOfCrsEcoSts(BaseEnumSender):
    de_name     = "ActvnOfCrsEco.Sts"
    fdx_name    = "ActvnOfCrsEcoSts"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ActvnOfCrsEcoSts, self).__init__(signal_interface, item)


class ActvnOfPrkgAssi(BaseEnumSender):
    de_name     = "ActvnOfPrkgAssi"
    fdx_name    = "ActvnOfPrkgAssi"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ActvnOfPrkgAssi, self).__init__(signal_interface, item)


class ActvnOfPrkgAut(BaseEnumSender):
    de_name     = "ActvnOfPrkgAut"
    fdx_name    = "ActvnOfPrkgAut"
    fdx_groupid = 3217
    class map:
       Off = 0
       PrkgTypAutSeln = 1
       PrkgTypPara = 2
       PrkgTypPerp = 3
       PrkgManvCncl = 4
       ParkOutManv = 5
       Resd1 = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ActvnOfPrkgAut, self).__init__(signal_interface, item)


class ActvOfHorn(BaseEnumSender):
    de_name     = "ActvOfHorn"
    fdx_name    = "ActvOfHorn"
    fdx_groupid = 3200
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ActvOfHorn, self).__init__(signal_interface, item)


class ActvtAutnmsPrkgCtrl(BaseEnumSender):
    de_name     = "ActvtAutnmsPrkgCtrl"
    fdx_name    = "ActvtAutnmsPrkgCtrl"
    fdx_groupid = 3222
    class map:
       Idle = 0
       Activate = 1
       Resume = 2
       Pause = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ActvtAutnmsPrkgCtrl, self).__init__(signal_interface, item)


class AdjSpdLimnSts(BaseEnumSender):
    de_name     = "AdjSpdLimnSts"
    fdx_name    = "AdjSpdLimnSts"
    fdx_groupid = 3230
    class map:
       Off = 1
       Stb = 2
       Actv = 3
       Ovrd = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AdjSpdLimnSts, self).__init__(signal_interface, item)


class AdjSpdLimnWarn(BaseEnumSender):
    de_name     = "AdjSpdLimnWarn"
    fdx_name    = "AdjSpdLimnWarn"
    fdx_groupid = 3230
    class map:
       NoWarn = 0
       SoundWarn = 1
       VisWarn = 2
       SoundAndVisWarn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AdjSpdLimnWarn, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class AdprTurnSpdModPen(BaseEnumSender):
    de_name     = "AdprTurnSpdMod.Pen"
    fdx_name    = "AdprTurnSpdModPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AdprTurnSpdModPen, self).__init__(signal_interface, item)


# Identifier     Description
# CmftMod     CSA Comfort Mode
# NormMod    CSA Normal Mode
# DynMod      CSA Dynamic Mode
class AdprTurnSpdModSts(BaseEnumSender):
    de_name     = "AdprTurnSpdMod.Sts"
    fdx_name    = "AdprTurnSpdModSts"
    fdx_groupid = 3212
    class map:
       CmftMod = 0
       NormMod = 1
       DynMod = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AdprTurnSpdModSts, self).__init__(signal_interface, item)


class AirClngReq(BaseEnumSender):
    de_name     = "AirClngReq"
    fdx_name    = "AirClngReq"
    fdx_groupid = 3216
    class map:
       NoReq = 0
       On = 1
       Off = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AirClngReq, self).__init__(signal_interface, item)


# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class ALat1(BaseFloatSender):
    de_name     = "ADataRawSafe.ALat"
    fdx_name    = "ALat1"
    fdx_groupid = 3224
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ALat1, self).__init__(signal_interface, item)


class ALat1Qf1(BaseEnumSender):
    de_name     = "ADataRawSafe.ALat1Qf"
    fdx_name    = "ALat1Qf1"
    fdx_groupid = 3224
    class map:
       DevOfDataUndefd = 0
       DataTmpUndefdAndEvlnInProgs = 1
       DevOfDataNotWithinRngAllwd = 2
       DataCalcdWithDevDefd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ALat1Qf1, self).__init__(signal_interface, item)


# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class ALgt1(BaseFloatSender):
    de_name     = "ADataRawSafe.ALgt"
    fdx_name    = "ALgt1"
    fdx_groupid = 3224
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ALgt1, self).__init__(signal_interface, item)


class ALgt1Qf1(BaseEnumSender):
    de_name     = "ADataRawSafe.ALgt1Qf"
    fdx_name    = "ALgt1Qf1"
    fdx_groupid = 3224
    class map:
       DevOfDataUndefd = 0
       DataTmpUndefdAndEvlnInProgs = 1
       DevOfDataNotWithinRngAllwd = 2
       DataCalcdWithDevDefd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ALgt1Qf1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:-100->309.5, Resolution: (0.1*x+-100.0, raw is unsigned, 12 bits )
class AmbTIndcd(BaseFloatSender):
    de_name     = "AmbTIndcdWithUnit.AmbTIndcd"
    fdx_name    = "AmbTIndcd"
    fdx_groupid = 3205
    min    = -100
    max    = 309.5
    scale  = 0.1
    offset = -100.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AmbTIndcd, self).__init__(signal_interface, item)


class AmbTIndcdQf(BaseEnumSender):
    de_name     = "AmbTIndcdWithUnit.QF"
    fdx_name    = "AmbTIndcdQf"
    fdx_groupid = 3205
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AmbTIndcdQf, self).__init__(signal_interface, item)


class AmbTIndcdUnit(BaseEnumSender):
    de_name     = "AmbTIndcdWithUnit.AmbTIndcdUnit"
    fdx_name    = "AmbTIndcdUnit"
    fdx_groupid = 3205
    class map:
       Celsius = 0
       Fahrenheit = 1
       UkwnUnit = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AmbTIndcdUnit, self).__init__(signal_interface, item)


class AmbTRawQly(BaseEnumSender):
    de_name     = "AmbTRaw.Qly"
    fdx_name    = "AmbTRawQly"
    fdx_groupid = 3199
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AmbTRawQly, self).__init__(signal_interface, item)


# Unit: degC,  Range:-70->134.7, Resolution: (0.1*x+-70.0, raw is unsigned, 11 bits )
class AmbTRawVal(BaseFloatSender):
    de_name     = "AmbTRaw.AmbTVal"
    fdx_name    = "AmbTRawVal"
    fdx_groupid = 3199
    min    = -70
    max    = 134.7
    scale  = 0.1
    offset = -70.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AmbTRawVal, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->7
class AntithftRednReq(BaseIntegerSender):
    de_name     = "AntithftRednReq"
    fdx_name    = "AntithftRednReq"
    fdx_groupid = 3215
    min = 0
    max = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AntithftRednReq, self).__init__(signal_interface, item)


# Unit: %,  Range:0->100
class AsyFctDeactvnWarn(BaseIntegerSender):
    de_name     = "AsyFctDeactvnWarn"
    fdx_name    = "AsyFctDeactvnWarn"
    fdx_groupid = 3190
    min = 0
    max = 100


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AsyFctDeactvnWarn, self).__init__(signal_interface, item)


class AsyLaneChgPsbl(BaseEnumSender):
    de_name     = "AsyLaneChg.Psbl"
    fdx_name    = "AsyLaneChgPsbl"
    fdx_groupid = 3189
    class map:
       NotPsbl = 0
       Le = 1
       Ri = 2
       Both = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AsyLaneChgPsbl, self).__init__(signal_interface, item)


class AsyLaneChgSts(BaseEnumSender):
    de_name     = "AsyLaneChg.Sts"
    fdx_name    = "AsyLaneChgSts"
    fdx_groupid = 3189
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AsyLaneChgSts, self).__init__(signal_interface, item)


class AsyLaneChgTyp(BaseEnumSender):
    de_name     = "AsyLaneChg.Typ"
    fdx_name    = "AsyLaneChgTyp"
    fdx_groupid = 3189
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AsyLaneChgTyp, self).__init__(signal_interface, item)


class AsySteerApplyRqrd(BaseEnumSender):
    de_name     = "AsySteerApplyRqrd"
    fdx_name    = "AsySteerApplyRqrd"
    fdx_groupid = 3183
    class map:
       NoWarn = 0
       Lvl1 = 1
       Lvl2 = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AsySteerApplyRqrd, self).__init__(signal_interface, item)


class AsySteerFctDeactvdWarn(BaseEnumSender):
    de_name     = "AsySteerFctDeactvdWarn"
    fdx_name    = "AsySteerFctDeactvdWarn"
    fdx_groupid = 3190
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AsySteerFctDeactvdWarn, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class AudMsgReq(BaseIntegerSender):
    de_name     = "AudMsgReq"
    fdx_name    = "AudMsgReq"
    fdx_groupid = 3206
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AudMsgReq, self).__init__(signal_interface, item)


class AudWarn(BaseBoolSender):
    de_name     = "AudWarn"
    fdx_name    = "AudWarn"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AudWarn, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class AudWarnActv(BaseEnumSender):
    de_name     = "AudWarnActv"
    fdx_name    = "AudWarnActv"
    fdx_groupid = 3217
    class map:
       NotVld1 = 0
       No = 1
       Yes = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AudWarnActv, self).__init__(signal_interface, item)


class AutnmsPrkgActvn(BaseEnumSender):
    de_name     = "AutnmsPrkgActvn"
    fdx_name    = "AutnmsPrkgActvn"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AutnmsPrkgActvn, self).__init__(signal_interface, item)


class AutnmsPrkgSeldScenarioAutnmsPrkgSeldDirectPrkg(BaseEnumSender):
    de_name     = "AutnmsPrkgSeldScenario.AutnmsPrkgSeldDirectPrkg"
    fdx_name    = "AutnmsPrkgSeldScenarioAutnmsPrkgSeldDirectPrkg"
    fdx_groupid = 3212
    class map:
       DirectParkingNotSelected = 0
       DirectParkingSelected = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AutnmsPrkgSeldScenarioAutnmsPrkgSeldDirectPrkg, self).__init__(signal_interface, item)


class AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgDir(BaseEnumSender):
    de_name     = "AutnmsPrkgSeldScenario.AutnmsPrkgSeldPrkgDir"
    fdx_name    = "AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgDir"
    fdx_groupid = 3212
    class map:
       Inactive = 0
       FrontFirst = 1
       RearFirst = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgDir, self).__init__(signal_interface, item)


class AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgScenario(BaseEnumSender):
    de_name     = "AutnmsPrkgSeldScenario.AutnmsPrkgSeldPrkgScenario"
    fdx_name    = "AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgScenario"
    fdx_groupid = 3212
    class map:
       Inactive = 0
       ParkIn = 1
       ParkOut = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgScenario, self).__init__(signal_interface, item)


class AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgSide(BaseEnumSender):
    de_name     = "AutnmsPrkgSeldScenario.AutnmsPrkgSeldPrkgSide"
    fdx_name    = "AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgSide"
    fdx_groupid = 3212
    class map:
       Inactive = 0
       Left = 1
       Right = 2
       StraightForward = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AutnmsPrkgSeldScenarioAutnmsPrkgSeldPrkgSide, self).__init__(signal_interface, item)


class AutnmsPrkgSeldScenarioAutnmsPrkgSeldRem(BaseEnumSender):
    de_name     = "AutnmsPrkgSeldScenario.AutnmsPrkgSeldRem"
    fdx_name    = "AutnmsPrkgSeldScenarioAutnmsPrkgSeldRem"
    fdx_groupid = 3212
    class map:
       Inactive = 0
       NonRemote = 1
       Remote = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AutnmsPrkgSeldScenarioAutnmsPrkgSeldRem, self).__init__(signal_interface, item)


class AutnmsPrkgSeldScenarioAutnmsPrkgSeldSlotTyp(BaseEnumSender):
    de_name     = "AutnmsPrkgSeldScenario.AutnmsPrkgSeldSlotTyp"
    fdx_name    = "AutnmsPrkgSeldScenarioAutnmsPrkgSeldSlotTyp"
    fdx_groupid = 3212
    class map:
       Inactive = 0
       Parallel = 1
       Perpendicular = 2
       Fishbone = 3
       Garage = 4
       HomeZone = 5
       ParkLaunch = 6
       Resd = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AutnmsPrkgSeldScenarioAutnmsPrkgSeldSlotTyp, self).__init__(signal_interface, item)


# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class AVert2(BaseFloatSender):
    de_name     = "ADataRawSafe.AVert"
    fdx_name    = "AVert2"
    fdx_groupid = 3224
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AVert2, self).__init__(signal_interface, item)


class AVert2Qf1(BaseEnumSender):
    de_name     = "ADataRawSafe.AVertQf"
    fdx_name    = "AVert2Qf1"
    fdx_groupid = 3224
    class map:
       DevOfDataUndefd = 0
       DataTmpUndefdAndEvlnInProgs = 1
       DevOfDataNotWithinRngAllwd = 2
       DataCalcdWithDevDefd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AVert2Qf1, self).__init__(signal_interface, item)


class AvlStsForLatAutDrv(BaseEnumSender):
    de_name     = "AvlStsForLatAutDrv"
    fdx_name    = "AvlStsForLatAutDrv"
    fdx_groupid = 3188
    class map:
       NoMsg = 0
       LatCtrlNotAvl = 1
       HiSpd = 2
       LaneLimrMiss = 3
       VehToFolwMiss = 4
       OvrdTiMaxExcdd = 5
       DrvrNotInLoopDetd = 6
       DrvrBucdRqrd = 7
       DrvrDoorNotClsd = 8
       GearNotInDrv = 9
       SnsrBlkd = 10
       HldTiMaxExcdd = 11
       DrvModSeldNotOk = 12
       EpbAppld = 13
       SpdLowLimExcdd = 14
       NotInUse2 = 15
       NotInUse3 = 16
       NotInUse4 = 17
       NotInUse5 = 18
       NotInUse6 = 19
       NotInUse7 = 20


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(AvlStsForLatAutDrv, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class BackCntrForMissCom(BaseIntegerSender):
    de_name     = "BackCntrForMissCom"
    fdx_name    = "BackCntrForMissCom"
    fdx_groupid = 3228
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BackCntrForMissCom, self).__init__(signal_interface, item)


# Unit: %,  Range:0->102.3, Resolution: (0.1*x+0.0, raw is unsigned, 10 bits )
class BarForFuEco(BaseFloatSender):
    de_name     = "CchForFuEco.BarForFuEco"
    fdx_name    = "BarForFuEco"
    fdx_groupid = 3233
    min    = 0
    max    = 102.3
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BarForFuEco, self).__init__(signal_interface, item)


# Unit: km,  Range:0->2000000, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class BkpOfDstTrvld(BaseFloatSender):
    de_name     = "BkpOfDstTrvld"
    fdx_name    = "BkpOfDstTrvld"
    fdx_groupid = 3205
    min    = 0
    max    = 2000000
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BkpOfDstTrvld, self).__init__(signal_interface, item)


class BlisSwOnOff(BaseEnumSender):
    de_name     = "BlisSwOnOff"
    fdx_name    = "BlisSwOnOff"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BlisSwOnOff, self).__init__(signal_interface, item)


class BltLockStAtDrvrForBltLockSt1(BaseEnumSender):
    de_name     = "BltLockStAtDrvr.BltLockSt1"
    fdx_name    = "BltLockStAtDrvrForBltLockSt1"
    fdx_groupid = 3225
    class map:
       Unlock = 0
       Lock = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtDrvrForBltLockSt1, self).__init__(signal_interface, item)


class BltLockStAtDrvrForDevErrSts2(BaseEnumSender):
    de_name     = "BltLockStAtDrvr.BltLockSts"
    fdx_name    = "BltLockStAtDrvrForDevErrSts2"
    fdx_groupid = 3225
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtDrvrForDevErrSts2, self).__init__(signal_interface, item)


class BltLockStAtPassForBltLockSt1(BaseEnumSender):
    de_name     = "BltLockStAtPass.BltLockSt1"
    fdx_name    = "BltLockStAtPassForBltLockSt1"
    fdx_groupid = 3226
    class map:
       Unlock = 0
       Lock = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtPassForBltLockSt1, self).__init__(signal_interface, item)


class BltLockStAtPassForBltLockSts(BaseEnumSender):
    de_name     = "BltLockStAtPass.BltLockSts"
    fdx_name    = "BltLockStAtPassForBltLockSts"
    fdx_groupid = 3226
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtPassForBltLockSts, self).__init__(signal_interface, item)


# Description if a component is equipped or not.
class BltLockStAtRowSecLeForBltLockEquid(BaseEnumSender):
    de_name     = "BltLockStAtRowSecLe.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecLeForBltLockEquid"
    fdx_groupid = 3226
    class map:
       Equid = 0
       NotEquid = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecLeForBltLockEquid, self).__init__(signal_interface, item)


class BltLockStAtRowSecLeForBltLockSt1(BaseEnumSender):
    de_name     = "BltLockStAtRowSecLe.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecLeForBltLockSt1"
    fdx_groupid = 3226
    class map:
       Unlock = 0
       Lock = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecLeForBltLockSt1, self).__init__(signal_interface, item)


class BltLockStAtRowSecLeForBltLockSts(BaseEnumSender):
    de_name     = "BltLockStAtRowSecLe.BltLockSts"
    fdx_name    = "BltLockStAtRowSecLeForBltLockSts"
    fdx_groupid = 3226
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecLeForBltLockSts, self).__init__(signal_interface, item)


# Description if a component is equipped or not.
class BltLockStAtRowSecMidForBltLockEquid(BaseEnumSender):
    de_name     = "BltLockStAtRowSecMid.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecMidForBltLockEquid"
    fdx_groupid = 3226
    class map:
       Equid = 0
       NotEquid = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecMidForBltLockEquid, self).__init__(signal_interface, item)


class BltLockStAtRowSecMidForBltLockSt1(BaseEnumSender):
    de_name     = "BltLockStAtRowSecMid.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecMidForBltLockSt1"
    fdx_groupid = 3226
    class map:
       Unlock = 0
       Lock = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecMidForBltLockSt1, self).__init__(signal_interface, item)


class BltLockStAtRowSecMidForBltLockSts(BaseEnumSender):
    de_name     = "BltLockStAtRowSecMid.BltLockSts"
    fdx_name    = "BltLockStAtRowSecMidForBltLockSts"
    fdx_groupid = 3226
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecMidForBltLockSts, self).__init__(signal_interface, item)


# Description if a component is equipped or not.
class BltLockStAtRowSecRiForBltLockEquid(BaseEnumSender):
    de_name     = "BltLockStAtRowSecRi.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecRiForBltLockEquid"
    fdx_groupid = 3226
    class map:
       Equid = 0
       NotEquid = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecRiForBltLockEquid, self).__init__(signal_interface, item)


class BltLockStAtRowSecRiForBltLockSt1(BaseEnumSender):
    de_name     = "BltLockStAtRowSecRi.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecRiForBltLockSt1"
    fdx_groupid = 3226
    class map:
       Unlock = 0
       Lock = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecRiForBltLockSt1, self).__init__(signal_interface, item)


class BltLockStAtRowSecRiForBltLockSts(BaseEnumSender):
    de_name     = "BltLockStAtRowSecRi.BltLockSts"
    fdx_name    = "BltLockStAtRowSecRiForBltLockSts"
    fdx_groupid = 3226
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowSecRiForBltLockSts, self).__init__(signal_interface, item)


# Description if a component is equipped or not.
class BltLockStAtRowThrdLeForBltLockEquid(BaseEnumSender):
    de_name     = "BltLockStAtRowThrdLe.BltLockEquid"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockEquid"
    fdx_groupid = 3226
    class map:
       Equid = 0
       NotEquid = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowThrdLeForBltLockEquid, self).__init__(signal_interface, item)


class BltLockStAtRowThrdLeForBltLockSt1(BaseEnumSender):
    de_name     = "BltLockStAtRowThrdLe.BltLockSt1"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockSt1"
    fdx_groupid = 3226
    class map:
       Unlock = 0
       Lock = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowThrdLeForBltLockSt1, self).__init__(signal_interface, item)


class BltLockStAtRowThrdLeForBltLockSts(BaseEnumSender):
    de_name     = "BltLockStAtRowThrdLe.BltLockSts"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockSts"
    fdx_groupid = 3226
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowThrdLeForBltLockSts, self).__init__(signal_interface, item)


# Description if a component is equipped or not.
class BltLockStAtRowThrdRiForBltLockEquid(BaseEnumSender):
    de_name     = "BltLockStAtRowThrdRi.BltLockEquid"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockEquid"
    fdx_groupid = 3226
    class map:
       Equid = 0
       NotEquid = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowThrdRiForBltLockEquid, self).__init__(signal_interface, item)


class BltLockStAtRowThrdRiForBltLockSt1(BaseEnumSender):
    de_name     = "BltLockStAtRowThrdRi.BltLockSt1"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockSt1"
    fdx_groupid = 3226
    class map:
       Unlock = 0
       Lock = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowThrdRiForBltLockSt1, self).__init__(signal_interface, item)


class BltLockStAtRowThrdRiForBltLockSts(BaseEnumSender):
    de_name     = "BltLockStAtRowThrdRi.BltLockSts"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockSts"
    fdx_groupid = 3226
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltLockStAtRowThrdRiForBltLockSts, self).__init__(signal_interface, item)


class BltRmnSound1(BaseEnumSender):
    de_name     = "BltRmnSound1"
    fdx_name    = "BltRmnSound1"
    fdx_groupid = 3205
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltRmnSound1, self).__init__(signal_interface, item)


class BltRmnSound2(BaseEnumSender):
    de_name     = "BltRmnSound2"
    fdx_name    = "BltRmnSound2"
    fdx_groupid = 3205
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltRmnSound2, self).__init__(signal_interface, item)


class BltRmnSound3(BaseEnumSender):
    de_name     = "BltRmnSound3"
    fdx_name    = "BltRmnSound3"
    fdx_groupid = 3205
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltRmnSound3, self).__init__(signal_interface, item)


class BltRmnSound4(BaseEnumSender):
    de_name     = "BltRmnSound4"
    fdx_name    = "BltRmnSound4"
    fdx_groupid = 3205
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BltRmnSound4, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			SegNotPartOfBrdg	Segment Not Part Of Bridge	Segment is not part of a bridge.
# 1			1			SegPartOfBrdg	Segment Part Of Bridge	Segment is part of a bridge.
# 2			2			Ukwn		Unknown			Unknown
class BrdgInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.Brdg"
    fdx_name    = "BrdgInHznSeg"
    fdx_groupid = 3215
    class map:
       SegNotPartOfBrdg = 0
       SegPartOfBrdg = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BrdgInHznSeg, self).__init__(signal_interface, item)


# 0= High fluid level
# 1= Low fluid level
class BrkFldLvl(BaseEnumSender):
    de_name     = "BrkFldLvl"
    fdx_name    = "BrkFldLvl"
    fdx_groupid = 3241
    class map:
       FldLvlHi = 0
       FldLvlLo = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BrkFldLvl, self).__init__(signal_interface, item)


# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlFrntLeAct(BaseFloatSender):
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlFrntLeAct"
    fdx_name    = "BrkFricTqAtWhlFrntLeAct"
    fdx_groupid = 3244
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BrkFricTqAtWhlFrntLeAct, self).__init__(signal_interface, item)


# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlFrntRiAct(BaseFloatSender):
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlFrntRiAct"
    fdx_name    = "BrkFricTqAtWhlFrntRiAct"
    fdx_groupid = 3244
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BrkFricTqAtWhlFrntRiAct, self).__init__(signal_interface, item)


# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlReLeAct(BaseFloatSender):
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlReLeAct"
    fdx_name    = "BrkFricTqAtWhlReLeAct"
    fdx_groupid = 3244
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BrkFricTqAtWhlReLeAct, self).__init__(signal_interface, item)


# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlReRiAct(BaseFloatSender):
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlReRiAct"
    fdx_name    = "BrkFricTqAtWhlReRiAct"
    fdx_groupid = 3244
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BrkFricTqAtWhlReRiAct, self).__init__(signal_interface, item)


class BrkRelsWarnReq(BaseEnumSender):
    de_name     = "BrkRelsWarnReq"
    fdx_name    = "BrkRelsWarnReq"
    fdx_groupid = 3232
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BrkRelsWarnReq, self).__init__(signal_interface, item)


# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class BrkWarnIndcnReq(BaseEnumSender):
    de_name     = "BrkAndAbsWarnIndcnReq.BrkWarnIndcnReq"
    fdx_name    = "BrkWarnIndcnReq"
    fdx_groupid = 3240
    class map:
       On = 0
       Off = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BrkWarnIndcnReq, self).__init__(signal_interface, item)


class BtnAudFbSts(BaseEnumSender):
    de_name     = "BtnAudFbSts"
    fdx_name    = "BtnAudFbSts"
    fdx_groupid = 3205
    class map:
       NotPsd = 0
       Psd = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BtnAudFbSts, self).__init__(signal_interface, item)


# SImple representation of  a virtual (multistep) button for adjusting audio volume.
# 0 = button not pressed. No request to adjust volume
# 1 = button pressed for "volume up"
# 2 = buttono pressed for "volume down"
#
# Receiver side controls how the button status is used.
class BtnAudVolSts(BaseEnumSender):
    de_name     = "BtnAudVolSts"
    fdx_name    = "BtnAudVolSts"
    fdx_groupid = 3205
    class map:
       NoBtnPsd = 0
       BtnVolUpPsd = 1
       BtnVolDwnPsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BtnAudVolSts, self).__init__(signal_interface, item)


# Represents status of a (multilevel) button "Left/Right.
#
# Semantic defined by receiver side. (Previou/Next, FFBW/FFWD, etc)
class BtnMmedLeRiSts(BaseEnumSender):
    de_name     = "BtnMmedLeRiSts"
    fdx_name    = "BtnMmedLeRiSts"
    fdx_groupid = 3206
    class map:
       NoBtnPsd = 0
       BtnLePsd = 1
       BtnRiPsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BtnMmedLeRiSts, self).__init__(signal_interface, item)


class BtnMmedModSts(BaseEnumSender):
    de_name     = "BtnMmedModSts"
    fdx_name    = "BtnMmedModSts"
    fdx_groupid = 3207
    class map:
       NotPsd = 0
       Psd = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BtnMmedModSts, self).__init__(signal_interface, item)


# Represents status of a button "Set".
#
# Semantic defined by receiver side. (Activate/Deactivate, Play/Pause, Mute/UnMute etc)
class BtnMmedSetSts(BaseEnumSender):
    de_name     = "BtnMmedSetSts"
    fdx_name    = "BtnMmedSetSts"
    fdx_groupid = 3206
    class map:
       BtnSetNotPsd = 0
       BtnSetPsd = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(BtnMmedSetSts, self).__init__(signal_interface, item)


# Carries call presence information. Defined for CallSts range [0-1]
#
# 0 = Inactive. There is no active call present with any telephony solution in the system.
# 1 = Active. Atleast one active call is present in the system.
class CallStsIndcn(BaseEnumSender):
    de_name     = "CallSts"
    fdx_name    = "CallStsIndcn"
    fdx_groupid = 3214
    class map:
       Inactive = 0
       Active = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CallStsIndcn, self).__init__(signal_interface, item)


class CarModSts1(BaseEnumSender):
    de_name     = "VehModMngtGlbSafe1.CarModSts1"
    fdx_name    = "CarModSts1"
    fdx_groupid = 3192
    class map:
       CarModNorm = 0
       CarModTrnsp = 1
       CarModFcy = 2
       CarModCrash = 3
       CarModDyno = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CarModSts1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->7
class CarModSubtypWdCarModSubtyp(BaseIntegerSender):
    de_name     = "VehModMngtGlbSafe1.CarModSubtypWdCarModSubtyp"
    fdx_name    = "CarModSubtypWdCarModSubtyp"
    fdx_groupid = 3192
    min = 0
    max = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CarModSubtypWdCarModSubtyp, self).__init__(signal_interface, item)


# Unit: s,  Range:0->4.294967294E8, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class CarTiGlb(BaseFloatSender):
    de_name     = "CarTiGlb"
    fdx_name    = "CarTiGlb"
    fdx_groupid = 3196
    min    = 0
    max    = 4.294967294E8
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CarTiGlb, self).__init__(signal_interface, item)


class ChdLockgProtnStsToHmi(BaseEnumSender):
    de_name     = "ChdLockgProtnStsToHmi"
    fdx_name    = "ChdLockgProtnStsToHmi"
    fdx_groupid = 3194
    class map:
       OnOffSafeInvld1 = 0
       OnOffSafeOn = 1
       OnOffSafeOff = 2
       OnOffSafeInvld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ChdLockgProtnStsToHmi, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->7
class ChdTxVersInHznData(BaseIntegerSender):
    de_name     = "HznData.ChdTxVers"
    fdx_name    = "ChdTxVersInHznData"
    fdx_groupid = 3214
    min = 0
    max = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ChdTxVersInHznData, self).__init__(signal_interface, item)


class ChdWinProtnStsAtDrvrRe(BaseEnumSender):
    de_name     = "ChdWinProtnStsAtDrvrRe"
    fdx_name    = "ChdWinProtnStsAtDrvrRe"
    fdx_groupid = 3199
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ChdWinProtnStsAtDrvrRe, self).__init__(signal_interface, item)


class ChdWinProtnStsAtPassRe(BaseEnumSender):
    de_name     = "ChdWinProtnStsAtPassRe"
    fdx_name    = "ChdWinProtnStsAtPassRe"
    fdx_groupid = 3199
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ChdWinProtnStsAtPassRe, self).__init__(signal_interface, item)


class ClimaActv(BaseEnumSender):
    de_name     = "ClimaActv"
    fdx_name    = "ClimaActv"
    fdx_groupid = 3194
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClimaActv, self).__init__(signal_interface, item)


class ClimaEcoModRqrd(BaseEnumSender):
    de_name     = "PostDrvgClimaReq"
    fdx_name    = "ClimaEcoModRqrd"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClimaEcoModRqrd, self).__init__(signal_interface, item)


# Unit: W,  Range:0->2047, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class ClimaPwrCns(BaseFloatSender):
    de_name     = "ClimaPwrCns"
    fdx_name    = "ClimaPwrCns"
    fdx_groupid = 3197
    min    = 0
    max    = 2047
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClimaPwrCns, self).__init__(signal_interface, item)


class ClimaRqrd(BaseEnumSender):
    de_name     = "ClimaRqrdFromHmi"
    fdx_name    = "ClimaRqrd"
    fdx_groupid = 3211
    class map:
       NoReq = 0
       On = 1
       Off = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClimaRqrd, self).__init__(signal_interface, item)


# Unit: s,  Range:0->4.294967294E8, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class ClimaTmr(BaseFloatSender):
    de_name     = "ClimaTmr"
    fdx_name    = "ClimaTmr"
    fdx_groupid = 3216
    min    = 0
    max    = 4.294967294E8
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClimaTmr, self).__init__(signal_interface, item)


class ClimaTmrSts(BaseEnumSender):
    de_name     = "ClimaTmrSts"
    fdx_name    = "ClimaTmrSts"
    fdx_groupid = 3198
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClimaTmrSts, self).__init__(signal_interface, item)


class ClimaTmrStsRqrd(BaseEnumSender):
    de_name     = "ClimaTmrStsRqrd"
    fdx_name    = "ClimaTmrStsRqrd"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClimaTmrStsRqrd, self).__init__(signal_interface, item)


# 0=No Warning
# 1=Fuel Low
# 2=Battery Low
# 3=Fuel and Battery Low
# 4=Temperature Low
# 5=Temperature High
class ClimaWarn(BaseEnumSender):
    de_name     = "ClimaWarn"
    fdx_name    = "ClimaWarn"
    fdx_groupid = 3195
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClimaWarn, self).__init__(signal_interface, item)


# 24h or 12h indication of clock
class ClkFmt(BaseEnumSender):
    de_name     = "IndcnUnit.ClkFmt"
    fdx_name    = "ClkFmt"
    fdx_groupid = 3211
    class map:
       Hr24 = 0
       Hr12 = 1
       UkwnClkFmt = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClkFmt, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class CllsnAidSnvtySeldPen(BaseEnumSender):
    de_name     = "CllsnAidSnvtySeld.Pen"
    fdx_name    = "CllsnAidSnvtySeldPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CllsnAidSnvtySeldPen, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			NotInUse		Not In Use	Not used
# 1			1			LoSnvty		Low Sensitivity	Low Sensitivity
# 2			2			NormSnvty	Normal Sensitivity	Normal Sensitivity
# 3			3			HiSnvty		High Sensitivity	High Sensitivity
class CllsnAidSnvtySeldSts(BaseEnumSender):
    de_name     = "CllsnAidSnvtySeld.Sts"
    fdx_name    = "CllsnAidSnvtySeldSts"
    fdx_groupid = 3212
    class map:
       NotInUse = 0
       LoSnvty = 1
       NormSnvty = 2
       HiSnvty = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CllsnAidSnvtySeldSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class CllsnFwdWarnActvPen(BaseEnumSender):
    de_name     = "CllsnFwdWarnActv.Pen"
    fdx_name    = "CllsnFwdWarnActvPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CllsnFwdWarnActvPen, self).__init__(signal_interface, item)


class CllsnFwdWarnActvSts(BaseEnumSender):
    de_name     = "CllsnFwdWarnActv.Sts"
    fdx_name    = "CllsnFwdWarnActvSts"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CllsnFwdWarnActvSts, self).__init__(signal_interface, item)


class CllsnFwdWarnReq(BaseEnumSender):
    de_name     = "CllsnFwdWarnReq"
    fdx_name    = "CllsnFwdWarnReq"
    fdx_groupid = 3182
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CllsnFwdWarnReq, self).__init__(signal_interface, item)


class CllsnWarnReq(BaseEnumSender):
    de_name     = "CllsnWarnReq"
    fdx_name    = "CllsnWarnReq"
    fdx_groupid = 3182
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CllsnWarnReq, self).__init__(signal_interface, item)


class CllsnWarnSideLe(BaseEnumSender):
    de_name     = "CllsnWarnSide.Le"
    fdx_name    = "CllsnWarnSideLe"
    fdx_groupid = 3182
    class map:
       NoWarn = 0
       WarnLvl1 = 1
       WarnLvl2 = 2
       Resd3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CllsnWarnSideLe, self).__init__(signal_interface, item)


class CllsnWarnSideRi(BaseEnumSender):
    de_name     = "CllsnWarnSide.Ri"
    fdx_name    = "CllsnWarnSideRi"
    fdx_groupid = 3182
    class map:
       NoWarn = 0
       WarnLvl1 = 1
       WarnLvl2 = 2
       Resd3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CllsnWarnSideRi, self).__init__(signal_interface, item)


class ClngActv(BaseEnumSender):
    de_name     = "ClngActv"
    fdx_name    = "ClngActv"
    fdx_groupid = 3199
    class map:
       NoReq = 0
       On = 1
       Off = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClngActv, self).__init__(signal_interface, item)


class ClngRqrdFromHmi(BaseEnumSender):
    de_name     = "ClngRqrdFromHmi"
    fdx_name    = "ClngRqrdFromHmi"
    fdx_groupid = 3217
    class map:
       NoReq = 0
       On = 1
       Off = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClngRqrdFromHmi, self).__init__(signal_interface, item)


class ClsAutEna(BaseEnumSender):
    de_name     = "ClsAutEna"
    fdx_name    = "ClsAutEna"
    fdx_groupid = 3217
    class map:
       Disabled = 0
       Enabled = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ClsAutEna, self).__init__(signal_interface, item)


class CmftFctActv(BaseEnumSender):
    de_name     = "CmftFctActv"
    fdx_name    = "CmftFctActv"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CmftFctActv, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class CmftFctSts(BaseEnumSender):
    de_name     = "CmftFctSts"
    fdx_name    = "CmftFctSts"
    fdx_groupid = 3228
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CmftFctSts, self).__init__(signal_interface, item)


# Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )
class CmptmtAirTEstimdExtdComptmtT(BaseFloatSender):
    de_name     = "CmptmtAirTEstimdExtd.ComptmtT"
    fdx_name    = "CmptmtAirTEstimdExtdComptmtT"
    fdx_groupid = 3199
    min    = -60
    max    = 125
    scale  = 0.1
    offset = -60.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CmptmtAirTEstimdExtdComptmtT, self).__init__(signal_interface, item)


class CmptmtAirTEstimdExtdQlyFlg(BaseEnumSender):
    de_name     = "CmptmtAirTEstimdExtd.QlyFlg"
    fdx_name    = "CmptmtAirTEstimdExtdQlyFlg"
    fdx_groupid = 3199
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CmptmtAirTEstimdExtdQlyFlg, self).__init__(signal_interface, item)


# Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )
class CmptmtTFrnt(BaseFloatSender):
    de_name     = "CmptmtTFrnt.CmptmtTFrnt"
    fdx_name    = "CmptmtTFrnt"
    fdx_groupid = 3216
    min    = -60
    max    = 125
    scale  = 0.1
    offset = -60.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CmptmtTFrnt, self).__init__(signal_interface, item)


# Interpretation:
# SnsrDataUndefd - Accuracy of data undefinable
# FanNotRunning - Aspiration fan not running
# SnsrDataNotOk - Data accuracy not within specification
# SnsrDataOk - Data is calculated within specified accuracy
class CmptmtTFrntQf(BaseEnumSender):
    de_name     = "CmptmtTFrnt.CmptmtTFrntQf"
    fdx_name    = "CmptmtTFrntQf"
    fdx_groupid = 3216
    class map:
       SnsrDataUndefd = 0
       FanNotRunning = 1
       SnsrDataNotOk = 2
       SnsrDataOk = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CmptmtTFrntQf, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnForCrsCtrl(BaseEnumSender):
    de_name     = "CnclWarnForCrsCtrl"
    fdx_name    = "CnclWarnForCrsCtrl"
    fdx_groupid = 3234
    class map:
       NotVld1 = 0
       No = 1
       Yes = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CnclWarnForCrsCtrl, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnLatForAutDrv(BaseEnumSender):
    de_name     = "SftyWarnGroupFromAsySafe.CnclWarnLatForAutDrv"
    fdx_name    = "CnclWarnLatForAutDrv"
    fdx_groupid = 3182
    class map:
       NotVld1 = 0
       No = 1
       Yes = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CnclWarnLatForAutDrv, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnLgtForAutDrv(BaseEnumSender):
    de_name     = "CnclWarnLgtForAutDrv"
    fdx_name    = "CnclWarnLgtForAutDrv"
    fdx_groupid = 3187
    class map:
       NotVld1 = 0
       No = 1
       Yes = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CnclWarnLgtForAutDrv, self).__init__(signal_interface, item)


class CntDwnToManvStrtInDstToManvLocn(BaseEnumSender):
    de_name     = "DstToManvLocn.CntDwnToManvStrt"
    fdx_name    = "CntDwnToManvStrtInDstToManvLocn"
    fdx_groupid = 3217
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CntDwnToManvStrtInDstToManvLocn, self).__init__(signal_interface, item)


class ComLostExtrSound(BaseBoolSender):
    de_name     = "ComLostExtrSound"
    fdx_name    = "ComLostExtrSound"
    fdx_groupid = 3242


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ComLostExtrSound, self).__init__(signal_interface, item)


class ConSftyDataSharingAllwd(BaseEnumSender):
    de_name     = "ConSftyDataSharingAllwd"
    fdx_name    = "ConSftyDataSharingAllwd"
    fdx_groupid = 3214
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ConSftyDataSharingAllwd, self).__init__(signal_interface, item)


class ConSftyDataSharingSts(BaseEnumSender):
    de_name     = "ConSftyDataSharingSts"
    fdx_name    = "ConSftyDataSharingSts"
    fdx_groupid = 3189
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ConSftyDataSharingSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class ConSftyWarnConSftyWarnId(BaseIntegerSender):
    de_name     = "ConSftyWarn.ConSftyWarnId"
    fdx_name    = "ConSftyWarnConSftyWarnId"
    fdx_groupid = 3189
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ConSftyWarnConSftyWarnId, self).__init__(signal_interface, item)


class ConSftyWarnConSftyWarnLvl(BaseEnumSender):
    de_name     = "ConSftyWarn.ConSftyWarnLvl"
    fdx_name    = "ConSftyWarnConSftyWarnLvl"
    fdx_groupid = 3189
    class map:
       NoWarn = 0
       LoLvl = 1
       MedLvl = 2
       HiLvl = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ConSftyWarnConSftyWarnLvl, self).__init__(signal_interface, item)


class ConSftyWarnConSftyWarnSnd(BaseEnumSender):
    de_name     = "ConSftyWarn.ConSftyWarnSnd"
    fdx_name    = "ConSftyWarnConSftyWarnSnd"
    fdx_groupid = 3189
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ConSftyWarnConSftyWarnSnd, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class ConSftyWarnConSftyWarnTyp(BaseIntegerSender):
    de_name     = "ConSftyWarn.ConSftyWarnTyp"
    fdx_name    = "ConSftyWarnConSftyWarnTyp"
    fdx_groupid = 3189
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ConSftyWarnConSftyWarnTyp, self).__init__(signal_interface, item)


# Unit: m,  Range:0->65535
class ConSftyWarnDistanceToWarning(BaseIntegerSender):
    de_name     = "ConSftyWarn.DistanceToWarning"
    fdx_name    = "ConSftyWarnDistanceToWarning"
    fdx_groupid = 3189
    min = 0
    max = 65535


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ConSftyWarnDistanceToWarning, self).__init__(signal_interface, item)


# Interpretation:
# NoReq - No active cooling requested
# LoReq - Low cooling need
# HiReq - High cooling need
class CoolgReqForDispCen(BaseEnumSender):
    de_name     = "CoolgReqForDispCen"
    fdx_name    = "CoolgReqForDispCen"
    fdx_groupid = 3216
    class map:
       NoReq = 0
       LoReq = 1
       HiReq = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CoolgReqForDispCen, self).__init__(signal_interface, item)


# Interpretation:
# NoCoolgActv - No cooling request received, no actions taken
# CoolgLvlStrtd1 - Activated cooling in level 1 started
# CoolgLvlStrtd2 - Activated cooling in level 2 started
class CoolgStsForDisp(BaseEnumSender):
    de_name     = "CoolgStsForDisp"
    fdx_name    = "CoolgStsForDisp"
    fdx_groupid = 3198
    class map:
       NoCoolgActv = 0
       CoolgLvlStrtd1 = 1
       CoolgLvlStrtd2 = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CoolgStsForDisp, self).__init__(signal_interface, item)


class CrsCtrlrSts(BaseEnumSender):
    de_name     = "CrsCtrlrSts"
    fdx_name    = "CrsCtrlrSts"
    fdx_groupid = 3230
    class map:
       Off = 1
       Stb = 2
       Actv = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CrsCtrlrSts, self).__init__(signal_interface, item)


class CrvtSpdAdpvSts(BaseEnumSender):
    de_name     = "CrvtSpdAdpvSts"
    fdx_name    = "CrvtSpdAdpvSts"
    fdx_groupid = 3187
    class map:
       Off = 0
       On = 1
       NotAvl = 2
       SrvRqrd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CrvtSpdAdpvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No warning
# 1			1			WarnForCmngCrvtLe	Warning For Coming Curvature Left	Warning for upcoming left curve
# 2			2			WarnForCmngCrvtRi	Warning For Coming Curvature Right	Warning for upcoming right curve
class CrvtSpdWarnReq(BaseEnumSender):
    de_name     = "CrvtSpdWarnReq"
    fdx_name    = "CrvtSpdWarnReq"
    fdx_groupid = 3187
    class map:
       NoWarn = 0
       WarnForCmngCrvtLe = 1
       WarnForCmngCrvtRi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CrvtSpdWarnReq, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class CrvtSpdWarnSts(BaseEnumSender):
    de_name     = "CrvtSpdWarnSts"
    fdx_name    = "CrvtSpdWarnSts"
    fdx_groupid = 3188
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CrvtSpdWarnSts, self).__init__(signal_interface, item)


class CtraIndcnLe(BaseEnumSender):
    de_name     = "CtraIndcnLe"
    fdx_name    = "CtraIndcnLe"
    fdx_groupid = 3183
    class map:
       NoCtraWarn = 0
       CtraWarn = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CtraIndcnLe, self).__init__(signal_interface, item)


class CtraIndcnRi(BaseEnumSender):
    de_name     = "CtraIndcnRi"
    fdx_name    = "CtraIndcnRi"
    fdx_groupid = 3183
    class map:
       NoCtraWarn = 0
       CtraWarn = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CtraIndcnRi, self).__init__(signal_interface, item)


class CtraOn1(BaseEnumSender):
    de_name     = "CtraOn1"
    fdx_name    = "CtraOn1"
    fdx_groupid = 3182
    class map:
       StrtUpOn = 0
       On = 1
       Off = 2
       TrlrOff = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CtraOn1, self).__init__(signal_interface, item)


class CtraSwOnOff(BaseEnumSender):
    de_name     = "CtraSwOnOff"
    fdx_name    = "CtraSwOnOff"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CtraSwOnOff, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->1023
class CtryCodInHznData(BaseIntegerSender):
    de_name     = "HznData.CtryCod"
    fdx_name    = "CtryCodInHznData"
    fdx_groupid = 3214
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CtryCodInHznData, self).__init__(signal_interface, item)


class CurtActvnReReq(BaseEnumSender):
    de_name     = "CurtActvnReReq"
    fdx_name    = "CurtActvnReReq"
    fdx_groupid = 3213
    class map:
       BtnReld = 0
       BtnPsd = 1
       Spare1 = 2
       Spare2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CurtActvnReReq, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class CycCntrInHznData(BaseIntegerSender):
    de_name     = "HznData.CycCntr"
    fdx_name    = "CycCntrInHznData"
    fdx_groupid = 3214
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CycCntrInHznData, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class CycCntrInHznEdge(BaseIntegerSender):
    de_name     = "HznEdge.CycCntr"
    fdx_name    = "CycCntrInHznEdge"
    fdx_groupid = 3215
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CycCntrInHznEdge, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class CycCntrInHznPosn(BaseIntegerSender):
    de_name     = "HznPosn.CycCntr"
    fdx_name    = "CycCntrInHznPosn"
    fdx_groupid = 3212
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CycCntrInHznPosn, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class CycCntrInHznProfLong(BaseIntegerSender):
    de_name     = "HznProfLong.CycCntr"
    fdx_name    = "CycCntrInHznProfLong"
    fdx_groupid = 3213
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CycCntrInHznProfLong, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class CycCntrInHznProfSho(BaseIntegerSender):
    de_name     = "HznProfSho.CycCntr"
    fdx_name    = "CycCntrInHznProfSho"
    fdx_groupid = 3213
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CycCntrInHznProfSho, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class CycCntrInHznSeg(BaseIntegerSender):
    de_name     = "HznSeg.CycCntr"
    fdx_name    = "CycCntrInHznSeg"
    fdx_groupid = 3215
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(CycCntrInHznSeg, self).__init__(signal_interface, item)


class DataDistbnAllwdForEgyPred(BaseEnumSender):
    de_name     = "DataDistbnAllwdForEgyPred"
    fdx_name    = "DataDistbnAllwdForEgyPred"
    fdx_groupid = 3212
    class map:
       NoReq = 0
       On = 1
       Off = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DataDistbnAllwdForEgyPred, self).__init__(signal_interface, item)


class DataDistbnReqForEgyPred(BaseEnumSender):
    de_name     = "DataDistbnReqForEgyPred"
    fdx_name    = "DataDistbnReqForEgyPred"
    fdx_groupid = 3237
    class map:
       NoReq = 0
       On = 1
       Off = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DataDistbnReqForEgyPred, self).__init__(signal_interface, item)


class DateOrTi(BaseEnumSender):
    de_name     = "SetTiAndDate.DateOrTi"
    fdx_name    = "DateOrTi"
    fdx_groupid = 3214
    class map:
       None_ = 0
       Date = 1
       Ti = 2
       DateTi = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DateOrTi, self).__init__(signal_interface, item)


# Unit: Days,  Range:1->31
class Day(BaseIntegerSender):
    de_name     = "TiAndDateIndcn.Day"
    fdx_name    = "Day"
    fdx_groupid = 3197
    min = 1
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Day, self).__init__(signal_interface, item)


# Unit: Days,  Range:1->31
class Day1(BaseIntegerSender):
    de_name     = "SetTiAndDate.Day"
    fdx_name    = "Day1"
    fdx_groupid = 3214
    min = 1
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Day1, self).__init__(signal_interface, item)


# Unit: Days,  Range:0->750
class DayToSrv(BaseIntegerSender):
    de_name     = "DayToSrv"
    fdx_name    = "DayToSrv"
    fdx_groupid = 3207
    min = 0
    max = 750


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DayToSrv, self).__init__(signal_interface, item)


class DeactvtLvlgCtrl(BaseBoolSender):
    de_name     = "DeactvtLvlgCtrl"
    fdx_name    = "DeactvtLvlgCtrl"
    fdx_groupid = 3215


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DeactvtLvlgCtrl, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->1023
class DestSrcAndCfmRqrd(BaseIntegerSender):
    de_name     = "DestSrcAndCfmRqrd"
    fdx_name    = "DestSrcAndCfmRqrd"
    fdx_groupid = 3237
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DestSrcAndCfmRqrd, self).__init__(signal_interface, item)


class DispAndHomeBtnSts(BaseEnumSender):
    de_name     = "DispAndHomeBtnSts"
    fdx_name    = "DispAndHomeBtnSts"
    fdx_groupid = 3214
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DispAndHomeBtnSts, self).__init__(signal_interface, item)


# Unit: Wh,  Range:0->127.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class DispBattEgyIn(BaseFloatSender):
    de_name     = "DispBattEgyIn"
    fdx_name    = "DispBattEgyIn"
    fdx_groupid = 3242
    min    = 0
    max    = 127.5
    scale  = 0.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DispBattEgyIn, self).__init__(signal_interface, item)


# Unit: Wh,  Range:0->127.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class DispBattEgyOut(BaseFloatSender):
    de_name     = "DispBattEgyOut"
    fdx_name    = "DispBattEgyOut"
    fdx_groupid = 3242
    min    = 0
    max    = 127.5
    scale  = 0.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DispBattEgyOut, self).__init__(signal_interface, item)


class DispOfPrpsnMod(BaseEnumSender):
    de_name     = "DispOfPrpsnMod"
    fdx_name    = "DispOfPrpsnMod"
    fdx_groupid = 3239
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DispOfPrpsnMod, self).__init__(signal_interface, item)


class DispStsCen(BaseEnumSender):
    de_name     = "DispStsCen"
    fdx_name    = "DispStsCen"
    fdx_groupid = 3215
    class map:
       NoFltCfmd = 0
       FltCfmd = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DispStsCen, self).__init__(signal_interface, item)


class DoorDrvrLockReSts(BaseEnumSender):
    de_name     = "DoorDrvrLockReSts"
    fdx_name    = "DoorDrvrLockReSts"
    fdx_groupid = 3199
    class map:
       LockStsUkwn = 0
       Unlckd = 1
       Lockd = 2
       SafeLockd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DoorDrvrLockReSts, self).__init__(signal_interface, item)


class DoorDrvrLockSts(BaseEnumSender):
    de_name     = "DoorDrvrLockSts"
    fdx_name    = "DoorDrvrLockSts"
    fdx_groupid = 3199
    class map:
       LockStsUkwn = 0
       Unlckd = 1
       Lockd = 2
       SafeLockd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DoorDrvrLockSts, self).__init__(signal_interface, item)


# Status of the door.
class DoorDrvrReSts(BaseEnumSender):
    de_name     = "DoorDrvrReSts"
    fdx_name    = "DoorDrvrReSts"
    fdx_groupid = 3194
    class map:
       Ukwn = 0
       Opend = 1
       Clsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DoorDrvrReSts, self).__init__(signal_interface, item)


# Status of the door.
class DoorDrvrSts(BaseEnumSender):
    de_name     = "DoorDrvrSts"
    fdx_name    = "DoorDrvrSts"
    fdx_groupid = 3192
    class map:
       Ukwn = 0
       Opend = 1
       Clsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DoorDrvrSts, self).__init__(signal_interface, item)


class DoorPassLockReSts(BaseEnumSender):
    de_name     = "DoorPassLockReSts"
    fdx_name    = "DoorPassLockReSts"
    fdx_groupid = 3199
    class map:
       LockStsUkwn = 0
       Unlckd = 1
       Lockd = 2
       SafeLockd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DoorPassLockReSts, self).__init__(signal_interface, item)


class DoorPassLockSts(BaseEnumSender):
    de_name     = "DoorPassLockSts"
    fdx_name    = "DoorPassLockSts"
    fdx_groupid = 3199
    class map:
       LockStsUkwn = 0
       Unlckd = 1
       Lockd = 2
       SafeLockd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DoorPassLockSts, self).__init__(signal_interface, item)


# Status of the door.
class DoorPassReSts(BaseEnumSender):
    de_name     = "DoorPassReSts"
    fdx_name    = "DoorPassReSts"
    fdx_groupid = 3192
    class map:
       Ukwn = 0
       Opend = 1
       Clsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DoorPassReSts, self).__init__(signal_interface, item)


# Status of the door.
class DoorPassSts(BaseEnumSender):
    de_name     = "DoorPassSts"
    fdx_name    = "DoorPassSts"
    fdx_groupid = 3192
    class map:
       Ukwn = 0
       Opend = 1
       Clsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DoorPassSts, self).__init__(signal_interface, item)


class DriveAwayInfoActvReq(BaseEnumSender):
    de_name     = "DriveAwayInfoActvReq"
    fdx_name    = "DriveAwayInfoActvReq"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DriveAwayInfoActvReq, self).__init__(signal_interface, item)


class DriveAwayInfoSoundWarnActvReq(BaseEnumSender):
    de_name     = "DriveAwayInfoSoundWarnActvReq"
    fdx_name    = "DriveAwayInfoSoundWarnActvReq"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DriveAwayInfoSoundWarnActvReq, self).__init__(signal_interface, item)


class DriveAwayInfoWarnReqAudWarnReq(BaseEnumSender):
    de_name     = "DriveAwayInfoWarnReq.AudWarnReq"
    fdx_name    = "DriveAwayInfoWarnReqAudWarnReq"
    fdx_groupid = 3190
    class map:
       Off = 0
       PrepareToLeave = 1
       LeaveNow = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DriveAwayInfoWarnReqAudWarnReq, self).__init__(signal_interface, item)


class DriveAwayInfoWarnReqCtrlDirOfTrfcLi(BaseEnumSender):
    de_name     = "DriveAwayInfoWarnReq.CtrlDirOfTrfcLi"
    fdx_name    = "DriveAwayInfoWarnReqCtrlDirOfTrfcLi"
    fdx_groupid = 3190
    class map:
       NoDirection = 0
       Straight = 1
       Left = 2
       LeftStraight = 3
       Right = 4
       StraightRight = 5
       LeftRight = 6
       All = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DriveAwayInfoWarnReqCtrlDirOfTrfcLi, self).__init__(signal_interface, item)


class DriveAwayInfoWarnReqInterruptionSrc(BaseEnumSender):
    de_name     = "DriveAwayInfoWarnReq.InterruptionSrc"
    fdx_name    = "DriveAwayInfoWarnReqInterruptionSrc"
    fdx_groupid = 3190
    class map:
       Off = 0
       TiOut = 1
       ResuPsd = 2
       BrkPedl = 3
       AccrPedlPsd = 4
       Resd5 = 5
       Resd6 = 6
       Resd7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DriveAwayInfoWarnReqInterruptionSrc, self).__init__(signal_interface, item)


class DriveAwayInfoWarnReqReqSrc(BaseEnumSender):
    de_name     = "DriveAwayInfoWarnReq.ReqSrc"
    fdx_name    = "DriveAwayInfoWarnReqReqSrc"
    fdx_groupid = 3190
    class map:
       NoReq = 0
       TtgReq = 1
       Resd2 = 2
       Resd3 = 3
       Resd4 = 4
       Resd5 = 5
       Resd6 = 6
       Resd7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DriveAwayInfoWarnReqReqSrc, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class DriveAwayInfoWarnReqTiToDrvCntDwnTi(BaseIntegerSender):
    de_name     = "DriveAwayInfoWarnReq.TiToDrvCntDwnTi"
    fdx_name    = "DriveAwayInfoWarnReqTiToDrvCntDwnTi"
    fdx_groupid = 3190
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DriveAwayInfoWarnReqTiToDrvCntDwnTi, self).__init__(signal_interface, item)


class DriveAwayInfoWarnReqVisWarnReq(BaseEnumSender):
    de_name     = "DriveAwayInfoWarnReq.VisWarnReq"
    fdx_name    = "DriveAwayInfoWarnReqVisWarnReq"
    fdx_groupid = 3190
    class map:
       Off = 0
       Countdown = 1
       LeaveNow = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DriveAwayInfoWarnReqVisWarnReq, self).__init__(signal_interface, item)


class DrvgInWrgDirOfTrfc(BaseEnumSender):
    de_name     = "DrvgInWrgDirOfTrfc"
    fdx_name    = "DrvgInWrgDirOfTrfc"
    fdx_groupid = 3217
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvgInWrgDirOfTrfc, self).__init__(signal_interface, item)


class DrvgSideInHznData(BaseEnumSender):
    de_name     = "HznData.DrvgSide"
    fdx_name    = "DrvgSideInHznData"
    fdx_groupid = 3214
    class map:
       DrvgSideLe = 0
       DrvgSideRi = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvgSideInHznData, self).__init__(signal_interface, item)


class DrvModDispd(BaseBoolSender):
    de_name     = "DrvModDispd"
    fdx_name    = "DrvModDispd"
    fdx_groupid = 3216


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvModDispd, self).__init__(signal_interface, item)


class DrvModReq(BaseEnumSender):
    de_name     = "DrvModReq"
    fdx_name    = "DrvModReq"
    fdx_groupid = 3222
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvModReq, self).__init__(signal_interface, item)


class DrvrCtrlOfPassSeatFrntReqd(BaseEnumSender):
    de_name     = "DrvrCtrlOfPassSeatFrntReqd"
    fdx_name    = "DrvrCtrlOfPassSeatFrntReqd"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrCtrlOfPassSeatFrntReqd, self).__init__(signal_interface, item)


class DrvrCtrlOfPassSeatFrntSts(BaseEnumSender):
    de_name     = "DrvrCtrlOfPassSeatFrntSts"
    fdx_name    = "DrvrCtrlOfPassSeatFrntSts"
    fdx_groupid = 3192
    class map:
       NotAvl = 0
       Avl = 1
       Deactvd = 2
       Actvd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrCtrlOfPassSeatFrntSts, self).__init__(signal_interface, item)


# Describes the desired vehicle movement direction from the driver.
# Contains only the drivers longitudinal direction intent. In which direction shall the car move.
class DrvrDesDir(BaseEnumSender):
    de_name     = "DrvrDesDir"
    fdx_name    = "DrvrDesDir"
    fdx_groupid = 3231
    class map:
       Undefd = 0
       Fwd = 1
       Rvs = 2
       Neut = 3
       Resd0 = 4
       Resd1 = 5
       Resd2 = 6
       Resd3 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrDesDir, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrDispSetgPen(BaseEnumSender):
    de_name     = "DrvrDispSetg.Pen"
    fdx_name    = "DrvrDispSetgPen"
    fdx_groupid = 3217
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrDispSetgPen, self).__init__(signal_interface, item)


class DrvrDispSetgSts(BaseEnumSender):
    de_name     = "DrvrDispSetg.Sts"
    fdx_name    = "DrvrDispSetgSts"
    fdx_groupid = 3217
    class map:
       Cmft = 0
       Eco = 1
       Dyn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrDispSetgSts, self).__init__(signal_interface, item)


class DrvrEntryLoReq(BaseEnumSender):
    de_name     = "DrvrEntryLoReq"
    fdx_name    = "DrvrEntryLoReq"
    fdx_groupid = 3211
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrEntryLoReq, self).__init__(signal_interface, item)


class DrvrHmiBackGndForHmiCen(BaseArraySender):
    de_name     = "DrvrHmiBackGndForHmiCen"
    fdx_name    = "DrvrHmiBackGndForHmiCen"
    fdx_groupid = 3219
    array_length = 254

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiBackGndForHmiCen, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiBackGndInfoSetgPen(BaseEnumSender):
    de_name     = "DrvrHmiBackGndInfoSetg.Pen"
    fdx_name    = "DrvrHmiBackGndInfoSetgPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiBackGndInfoSetgPen, self).__init__(signal_interface, item)


class DrvrHmiBackGndInfoSetgSetg(BaseEnumSender):
    de_name     = "DrvrHmiBackGndInfoSetg.Setg"
    fdx_name    = "DrvrHmiBackGndInfoSetgSetg"
    fdx_groupid = 3211
    class map:
       DrvrHmiBackGndInfoSetg0 = 0
       DrvrHmiBackGndInfoSetg1 = 1
       DrvrHmiBackGndInfoSetg2 = 2
       DrvrHmiBackGndInfoSetg3 = 3
       DrvrHmiBackGndInfoSetg4 = 4
       DrvrHmiBackGndInfoSetg5 = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiBackGndInfoSetgSetg, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiDispdModPen(BaseEnumSender):
    de_name     = "DrvrHmiDispdMod.Pen"
    fdx_name    = "DrvrHmiDispdModPen"
    fdx_groupid = 3216
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiDispdModPen, self).__init__(signal_interface, item)


class DrvrHmiDispdModSts(BaseEnumSender):
    de_name     = "DrvrHmiDispdMod.Sts"
    fdx_name    = "DrvrHmiDispdModSts"
    fdx_groupid = 3216
    class map:
       DispDftMod = 0
       DispAudMod = 1
       DispNavMod = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiDispdModSts, self).__init__(signal_interface, item)


class DrvrHmiForHmiCen(BaseArraySender):
    de_name     = "DrvrHmiForHmiCen"
    fdx_name    = "DrvrHmiForHmiCen"
    fdx_groupid = 3218
    array_length = 254

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiForHmiCen, self).__init__(signal_interface, item)


class DrvrHmiSpdLimAdpnSts(BaseEnumSender):
    de_name     = "DrvrHmiSpdLimAdpnSts"
    fdx_name    = "DrvrHmiSpdLimAdpnSts"
    fdx_groupid = 3206
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiSpdLimAdpnSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiUsrIfSetgPen(BaseEnumSender):
    de_name     = "DrvrHmiUsrIfSetg.Pen"
    fdx_name    = "DrvrHmiUsrIfSetgPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiUsrIfSetgPen, self).__init__(signal_interface, item)


class DrvrHmiUsrIfSetgSetg(BaseEnumSender):
    de_name     = "DrvrHmiUsrIfSetg.Setg"
    fdx_name    = "DrvrHmiUsrIfSetgSetg"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrHmiUsrIfSetgSetg, self).__init__(signal_interface, item)


class DrvrMassgRunng(BaseEnumSender):
    de_name     = "DrvrMassgRunng"
    fdx_name    = "DrvrMassgRunng"
    fdx_groupid = 3198
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrMassgRunng, self).__init__(signal_interface, item)


class DrvrPfmncLvl(BaseEnumSender):
    de_name     = "DrvrPfmncLvl"
    fdx_name    = "DrvrPfmncLvl"
    fdx_groupid = 3188
    class map:
       Ukwn = 0
       DrvrLvl1 = 1
       DrvrLvl2 = 2
       DrvrLvl3 = 3
       DrvrLvl4 = 4
       DrvrLvl5 = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrPfmncLvl, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrPfmncMonActvPen(BaseEnumSender):
    de_name     = "DrvrPfmncMonActv.Pen"
    fdx_name    = "DrvrPfmncMonActvPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrPfmncMonActvPen, self).__init__(signal_interface, item)


class DrvrPfmncMonActvSts(BaseEnumSender):
    de_name     = "DrvrPfmncMonActv.Sts"
    fdx_name    = "DrvrPfmncMonActvSts"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrPfmncMonActvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class DrvrPfmncSts(BaseEnumSender):
    de_name     = "DrvrPfmncSts"
    fdx_name    = "DrvrPfmncSts"
    fdx_groupid = 3188
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrPfmncSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No Warning
# 1			1			WarnForDrvrNotActv	Warning For Driver Not Active	Warning for Inattentive Driver
# 2			2			WarnForDrvrTire		Warning For Driver Tire		Warning for Sleepy Driver
# 3			3			NotInUse			Not In Use			Not used
class DrvrPfmncWarnReq(BaseEnumSender):
    de_name     = "DrvrPfmncWarnReq"
    fdx_name    = "DrvrPfmncWarnReq"
    fdx_groupid = 3188
    class map:
       NoWarn = 0
       WarnForDrvrNotActv = 1
       WarnForDrvrTire = 2
       NotInUse = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrPfmncWarnReq, self).__init__(signal_interface, item)


class DrvrSeatActvSpplFct(BaseEnumSender):
    de_name     = "DrvrSeatActvSpplFct"
    fdx_name    = "DrvrSeatActvSpplFct"
    fdx_groupid = 3198
    class map:
       NotAvl = 0
       LumExtnAndLumHei = 1
       BackBlster = 2
       CushExtn = 3
       HdrestHeiAndHdrestTilt = 4
       MassgFct = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatActvSpplFct, self).__init__(signal_interface, item)


class DrvrSeatDispMassgFctMassgInten(BaseEnumSender):
    de_name     = "DrvrSeatDispMassgFct.MassgInten"
    fdx_name    = "DrvrSeatDispMassgFctMassgInten"
    fdx_groupid = 3222
    class map:
       IntenLo = 0
       IntenNorm = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatDispMassgFctMassgInten, self).__init__(signal_interface, item)


class DrvrSeatDispMassgFctMassgProg(BaseEnumSender):
    de_name     = "DrvrSeatDispMassgFct.MassgProg"
    fdx_name    = "DrvrSeatDispMassgFctMassgProg"
    fdx_groupid = 3222
    class map:
       Prog1 = 0
       Prog2 = 1
       Prog3 = 2
       Prog4 = 3
       Prog5 = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatDispMassgFctMassgProg, self).__init__(signal_interface, item)


class DrvrSeatDispMassgFctMassgSpdLvl(BaseEnumSender):
    de_name     = "DrvrSeatDispMassgFct.MassgSpdLvl"
    fdx_name    = "DrvrSeatDispMassgFctMassgSpdLvl"
    fdx_groupid = 3222
    class map:
       IntenLo = 0
       IntenNorm = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatDispMassgFctMassgSpdLvl, self).__init__(signal_interface, item)


class DrvrSeatDispMassgFctOnOff(BaseEnumSender):
    de_name     = "DrvrSeatDispMassgFct.OnOff"
    fdx_name    = "DrvrSeatDispMassgFctOnOff"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatDispMassgFctOnOff, self).__init__(signal_interface, item)


class DrvrSeatDispSpplFct(BaseEnumSender):
    de_name     = "DrvrSeatDispSpplFct"
    fdx_name    = "DrvrSeatDispSpplFct"
    fdx_groupid = 3216
    class map:
       NotAvl = 0
       LumExtnAndLumHei = 1
       BackBlster = 2
       CushExtn = 3
       HdrestHeiAndHdrestTilt = 4
       MassgFct = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatDispSpplFct, self).__init__(signal_interface, item)


class DrvrSeatMassgFctMassgInten(BaseEnumSender):
    de_name     = "DrvrSeatMassgFct.MassgInten"
    fdx_name    = "DrvrSeatMassgFctMassgInten"
    fdx_groupid = 3197
    class map:
       IntenLo = 0
       IntenNorm = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatMassgFctMassgInten, self).__init__(signal_interface, item)


class DrvrSeatMassgFctMassgProg(BaseEnumSender):
    de_name     = "DrvrSeatMassgFct.MassgProg"
    fdx_name    = "DrvrSeatMassgFctMassgProg"
    fdx_groupid = 3197
    class map:
       Prog1 = 0
       Prog2 = 1
       Prog3 = 2
       Prog4 = 3
       Prog5 = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatMassgFctMassgProg, self).__init__(signal_interface, item)


class DrvrSeatMassgFctMassgSpdLvl(BaseEnumSender):
    de_name     = "DrvrSeatMassgFct.MassgSpdLvl"
    fdx_name    = "DrvrSeatMassgFctMassgSpdLvl"
    fdx_groupid = 3197
    class map:
       IntenLo = 0
       IntenNorm = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatMassgFctMassgSpdLvl, self).__init__(signal_interface, item)


class DrvrSeatMassgFctOnOff(BaseEnumSender):
    de_name     = "DrvrSeatMassgFct.OnOff"
    fdx_name    = "DrvrSeatMassgFctOnOff"
    fdx_groupid = 3197
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatMassgFctOnOff, self).__init__(signal_interface, item)


class DrvrSeatSwtAdjmtOfSpplFctHozlSts(BaseEnumSender):
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_name    = "DrvrSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Fwd = 1
       Backw = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatSwtAdjmtOfSpplFctHozlSts, self).__init__(signal_interface, item)


class DrvrSeatSwtAdjmtOfSpplFctVertSts(BaseEnumSender):
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtAdjmtOfSpplFctVertSts"
    fdx_name    = "DrvrSeatSwtAdjmtOfSpplFctVertSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatSwtAdjmtOfSpplFctVertSts, self).__init__(signal_interface, item)


class DrvrSeatSwtHeiFrntSts(BaseEnumSender):
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtHeiFrntSts"
    fdx_name    = "DrvrSeatSwtHeiFrntSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatSwtHeiFrntSts, self).__init__(signal_interface, item)


class DrvrSeatSwtHeiSts(BaseEnumSender):
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtHeiSts"
    fdx_name    = "DrvrSeatSwtHeiSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatSwtHeiSts, self).__init__(signal_interface, item)


class DrvrSeatSwtInclSts(BaseEnumSender):
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtInclSts"
    fdx_name    = "DrvrSeatSwtInclSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Fwd = 1
       Backw = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatSwtInclSts, self).__init__(signal_interface, item)


class DrvrSeatSwtSelnOfSpplFctSts(BaseEnumSender):
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtSelnOfSpplFctSts"
    fdx_name    = "DrvrSeatSwtSelnOfSpplFctSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatSwtSelnOfSpplFctSts, self).__init__(signal_interface, item)


class DrvrSeatSwtSldSts(BaseEnumSender):
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtSldSts"
    fdx_name    = "DrvrSeatSwtSldSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Fwd = 1
       Backw = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrSeatSwtSldSts, self).__init__(signal_interface, item)


class DrvrWLoadLvl(BaseEnumSender):
    de_name     = "DrvrWLoadLvl"
    fdx_name    = "DrvrWLoadLvl"
    fdx_groupid = 3207
    class map:
       NoLvl = 0
       LoLvl = 1
       MidLvl = 2
       HiLvl = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DrvrWLoadLvl, self).__init__(signal_interface, item)


# Unit: m,  Range:0->1023
class Dst1InHznProfSho(BaseIntegerSender):
    de_name     = "HznProfSho.Dst1"
    fdx_name    = "Dst1InHznProfSho"
    fdx_groupid = 3213
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Dst1InHznProfSho, self).__init__(signal_interface, item)


# Unit: KiloMtr,  Range:0->1023
class DstEstimdToEmptyForDrvgElec(BaseIntegerSender):
    de_name     = "DstEstimdToEmptyForDrvgElec"
    fdx_name    = "DstEstimdToEmptyForDrvgElec"
    fdx_groupid = 3242
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstEstimdToEmptyForDrvgElec, self).__init__(signal_interface, item)


# Unit: KiloMtr,  Range:0->1023
class DstEstimdToEmptyForDrvgElecEco(BaseIntegerSender):
    de_name     = "DstEstimdToEmptyForDrvgElecEco"
    fdx_name    = "DstEstimdToEmptyForDrvgElecEco"
    fdx_groupid = 3237
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstEstimdToEmptyForDrvgElecEco, self).__init__(signal_interface, item)


class DstLong(BaseEnumSender):
    de_name     = "IndcnUnit.DstLong"
    fdx_name    = "DstLong"
    fdx_groupid = 3211
    class map:
       km = 0
       miles = 1
       UkwnUnit = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstLong, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DstNotifActvPen(BaseEnumSender):
    de_name     = "DstNotifActv.Pen"
    fdx_name    = "DstNotifActvPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstNotifActvPen, self).__init__(signal_interface, item)


class DstNotifActvSts(BaseEnumSender):
    de_name     = "DstNotifActv.Sts"
    fdx_name    = "DstNotifActvSts"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstNotifActvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class DstNotifSts(BaseEnumSender):
    de_name     = "DstNotifSts"
    fdx_name    = "DstNotifSts"
    fdx_groupid = 3188
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstNotifSts, self).__init__(signal_interface, item)


class DstSho(BaseEnumSender):
    de_name     = "IndcnUnit.DstSho"
    fdx_name    = "DstSho"
    fdx_groupid = 3211
    class map:
       Mtr = 0
       Ft = 1
       Yards = 2
       UkwnUnit = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstSho, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->2000
class DstToEmpty(BaseIntegerSender):
    de_name     = "DstToEmptyIndcd.DstToEmpty"
    fdx_name    = "DstToEmpty"
    fdx_groupid = 3207
    min = 0
    max = 2000


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstToEmpty, self).__init__(signal_interface, item)


# Unit: m,  Range:0->4000
class DstToManvInDstToManvLocn(BaseIntegerSender):
    de_name     = "DstToManvLocn.DstToManv"
    fdx_name    = "DstToManvInDstToManvLocn"
    fdx_groupid = 3217
    min = 0
    max = 4000


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstToManvInDstToManvLocn, self).__init__(signal_interface, item)


# Unit: km,  Range:0->32000
class DstToSrv(BaseIntegerSender):
    de_name     = "DstToSrv"
    fdx_name    = "DstToSrv"
    fdx_groupid = 3207
    min = 0
    max = 32000


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstToSrv, self).__init__(signal_interface, item)


# Unit: km,  Range:0->9999.9, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class DstTrvld1(BaseFloatSender):
    de_name     = "DstTrvld1"
    fdx_name    = "DstTrvld1"
    fdx_groupid = 3207
    min    = 0
    max    = 9999.9
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstTrvld1, self).__init__(signal_interface, item)


# Unit: km,  Range:0->9999.9, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class DstTrvld2(BaseFloatSender):
    de_name     = "DstTrvld2"
    fdx_name    = "DstTrvld2"
    fdx_groupid = 3206
    min    = 0
    max    = 9999.9
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstTrvld2, self).__init__(signal_interface, item)


# Unit: m,  Range:0->255
class DstTrvldHiResl(BaseIntegerSender):
    de_name     = "DstTrvldHiResl"
    fdx_name    = "DstTrvldHiResl"
    fdx_groupid = 3206
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstTrvldHiResl, self).__init__(signal_interface, item)


# Unit: Milg,  Range:0->1000000
class DstTrvldMst2(BaseIntegerSender):
    de_name     = "DstTrvldMst2"
    fdx_name    = "DstTrvldMst2"
    fdx_groupid = 3207
    min = 0
    max = 1000000


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstTrvldMst2, self).__init__(signal_interface, item)


class DstUnit(BaseEnumSender):
    de_name     = "DstToEmptyIndcd.DstUnit"
    fdx_name    = "DstUnit"
    fdx_groupid = 3207
    class map:
       km = 0
       miles = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(DstUnit, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class EasyInOutDrvrSeatAdjmtPen(BaseEnumSender):
    de_name     = "EasyInOutDrvrSeatAdjmt.Pen"
    fdx_name    = "EasyInOutDrvrSeatAdjmtPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EasyInOutDrvrSeatAdjmtPen, self).__init__(signal_interface, item)


class EasyInOutDrvrSeatAdjmtSts(BaseEnumSender):
    de_name     = "EasyInOutDrvrSeatAdjmt.Sts"
    fdx_name    = "EasyInOutDrvrSeatAdjmtSts"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EasyInOutDrvrSeatAdjmtSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class EgyLvlElecMai(BaseIntegerSender):
    de_name     = "VehModMngtGlbSafe1.EgyLvlElecMai"
    fdx_name    = "EgyLvlElecMai"
    fdx_groupid = 3192
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EgyLvlElecMai, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class EgyLvlElecSubtyp(BaseIntegerSender):
    de_name     = "VehModMngtGlbSafe1.EgyLvlElecSubtyp"
    fdx_name    = "EgyLvlElecSubtyp"
    fdx_groupid = 3192
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EgyLvlElecSubtyp, self).__init__(signal_interface, item)


class EgySave(BaseEnumSender):
    de_name     = "EgySave"
    fdx_name    = "EgySave"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EgySave, self).__init__(signal_interface, item)


class EmgyAsscSts(BaseEnumSender):
    de_name     = "EmgyAsscSts"
    fdx_name    = "EmgyAsscSts"
    fdx_groupid = 3195
    class map:
       Idle = 0
       AsscDataTrfInin = 1
       AsscDataTrf = 2
       AsscReqActv = 3
       AsscReqNotAprvd = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EmgyAsscSts, self).__init__(signal_interface, item)


class EmgyVehWarnActv(BaseEnumSender):
    de_name     = "EmgyVehWarnActv"
    fdx_name    = "EmgyVehWarnActv"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EmgyVehWarnActv, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class EmgyVehWarnSts(BaseEnumSender):
    de_name     = "EmgyVehWarnSts"
    fdx_name    = "EmgyVehWarnSts"
    fdx_groupid = 3189
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EmgyVehWarnSts, self).__init__(signal_interface, item)


class EngCooltIndcnReq(BaseEnumSender):
    de_name     = "EngCooltIndcnReq"
    fdx_name    = "EngCooltIndcnReq"
    fdx_groupid = 3240
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngCooltIndcnReq, self).__init__(signal_interface, item)


# 0= High fluid level
# 1= Low fluid level
class EngCooltLvl(BaseEnumSender):
    de_name     = "EngCooltLvl"
    fdx_name    = "EngCooltLvl"
    fdx_groupid = 3240
    class map:
       FldLvlHi = 0
       FldLvlLo = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngCooltLvl, self).__init__(signal_interface, item)


# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class EngFuCns(BaseFloatSender):
    de_name     = "EngFuCns"
    fdx_name    = "EngFuCns"
    fdx_groupid = 3240
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngFuCns, self).__init__(signal_interface, item)


# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class EngFuCnsFild(BaseFloatSender):
    de_name     = "EngFuCnsFild"
    fdx_name    = "EngFuCnsFild"
    fdx_groupid = 3243
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngFuCnsFild, self).__init__(signal_interface, item)


# Unit: hours,  Range:0->2000
class EngHrToSrv(BaseIntegerSender):
    de_name     = "EngHrToSrv"
    fdx_name    = "EngHrToSrv"
    fdx_groupid = 3207
    min = 0
    max = 2000


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngHrToSrv, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class EngIdleEco1Pen(BaseEnumSender):
    de_name     = "EngIdleEco1.Pen"
    fdx_name    = "EngIdleEco1Pen"
    fdx_groupid = 3216
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngIdleEco1Pen, self).__init__(signal_interface, item)


class EngIdleEco1Sts(BaseEnumSender):
    de_name     = "EngIdleEco1.Sts"
    fdx_name    = "EngIdleEco1Sts"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngIdleEco1Sts, self).__init__(signal_interface, item)


# Unit: rpm,  Range:0->16383, Resolution: (0.5*x+0.0, raw is unsigned, 15 bits )
class EngN(BaseFloatSender):
    de_name     = "EngNSafe.EngN"
    fdx_name    = "EngN"
    fdx_groupid = 3232
    min    = 0
    max    = 16383
    scale  = 0.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngN, self).__init__(signal_interface, item)


# Unit: RpmPerSec,  Range:-30000->30000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class EngNSafeEngNGrdt(BaseFloatSender):
    de_name     = "EngNSafe.EngNGrdt"
    fdx_name    = "EngNSafeEngNGrdt"
    fdx_groupid = 3232
    min    = -30000
    max    = 30000
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngNSafeEngNGrdt, self).__init__(signal_interface, item)


# Unit: %,  Range:0->120, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class EngOilLvl(BaseFloatSender):
    de_name     = "EngOilLvl"
    fdx_name    = "EngOilLvl"
    fdx_groupid = 3242
    min    = 0
    max    = 120
    scale  = 0.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngOilLvl, self).__init__(signal_interface, item)


# Engine oil level warning information
class EngOilLvlSts(BaseEnumSender):
    de_name     = "EngOilLvlSts"
    fdx_name    = "EngOilLvlSts"
    fdx_groupid = 3242
    class map:
       OilLvlOk = 0
       OilLvlLo1 = 1
       OilLvlLo2 = 2
       OilLvlHi = 3
       OilLvlSrvRqrd = 4
       Resd = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngOilLvlSts, self).__init__(signal_interface, item)


# Engine oil pressure warning information
class EngOilPWarn(BaseEnumSender):
    de_name     = "EngOilPWarn"
    fdx_name    = "EngOilPWarn"
    fdx_groupid = 3241
    class map:
       EngOilPOk = 0
       EngOilPNotOk = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngOilPWarn, self).__init__(signal_interface, item)


# Unit: rpm,  Range:0->16383.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class EngSpdDispd(BaseFloatSender):
    de_name     = "EngSpdDispd"
    fdx_name    = "EngSpdDispd"
    fdx_groupid = 3233
    min    = 0
    max    = 16383.5
    scale  = 0.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngSpdDispd, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class EngStrtStopDrvModIndSetgPen(BaseEnumSender):
    de_name     = "EngStrtStopDrvModIndSetg.Pen"
    fdx_name    = "EngStrtStopDrvModIndSetgPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngStrtStopDrvModIndSetgPen, self).__init__(signal_interface, item)


class EngStrtStopDrvModIndSetgSts(BaseEnumSender):
    de_name     = "EngStrtStopDrvModIndSetg.Sts"
    fdx_name    = "EngStrtStopDrvModIndSetgSts"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngStrtStopDrvModIndSetgSts, self).__init__(signal_interface, item)


# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class EngStrtStopSetg(BaseEnumSender):
    de_name     = "EngStrtStopSetg"
    fdx_name    = "EngStrtStopSetg"
    fdx_groupid = 3211
    class map:
       On = 0
       Off = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EngStrtStopSetg, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class EpbApplyAutSetgPen(BaseEnumSender):
    de_name     = "EpbApplyAutSetg.Pen"
    fdx_name    = "EpbApplyAutSetgPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EpbApplyAutSetgPen, self).__init__(signal_interface, item)


class EpbApplyAutSetgSts(BaseEnumSender):
    de_name     = "EpbApplyAutSetg.Sts"
    fdx_name    = "EpbApplyAutSetgSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EpbApplyAutSetgSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class EpbEngStallApplySetgPen(BaseEnumSender):
    de_name     = "EpbEngStallApplySetg.Pen"
    fdx_name    = "EpbEngStallApplySetgPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EpbEngStallApplySetgPen, self).__init__(signal_interface, item)


class EpbEngStallApplySetgSts(BaseEnumSender):
    de_name     = "EpbEngStallApplySetg.Sts"
    fdx_name    = "EpbEngStallApplySetgSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EpbEngStallApplySetgSts, self).__init__(signal_interface, item)


class EpbLampReq(BaseEnumSender):
    de_name     = "EpbLampReq.EpbLampReq"
    fdx_name    = "EpbLampReq"
    fdx_groupid = 3241
    class map:
       On = 0
       Off = 1
       Flash2 = 2
       Flash3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EpbLampReq, self).__init__(signal_interface, item)


class EscSptModReqdByDrvr(BaseEnumSender):
    de_name     = "EscSptModReqdByDrvr.EscSptModReqdByDrvr"
    fdx_name    = "EscSptModReqdByDrvr"
    fdx_groupid = 3215
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EscSptModReqdByDrvr, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class EscSptModReqdByDrvrPen(BaseEnumSender):
    de_name     = "EscSptModReqdByDrvr.Pen"
    fdx_name    = "EscSptModReqdByDrvrPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EscSptModReqdByDrvrPen, self).__init__(signal_interface, item)


# Information on activation/ deactivation of Electronic Stability Control (ESC)
class EscSt(BaseEnumSender):
    de_name     = "EscSt.EscSt"
    fdx_name    = "EscSt"
    fdx_groupid = 3239
    class map:
       Inin = 0
       Ok = 1
       TmpErr = 2
       PrmntErr = 3
       UsrOff = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EscSt, self).__init__(signal_interface, item)


class EscWarnIndcnReq(BaseEnumSender):
    de_name     = "EscWarnIndcnReq.EscWarnIndcnReq"
    fdx_name    = "EscWarnIndcnReq"
    fdx_groupid = 3241
    class map:
       EscWarnIndcnOnReq = 0
       EscWarnIndcnFlsgReq = 1
       Resd2 = 2
       EscWarnIndcnOffReq = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(EscWarnIndcnReq, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ExtrMirrFoldSetgPen(BaseEnumSender):
    de_name     = "ExtrMirrFoldSetg.Pen"
    fdx_name    = "ExtrMirrFoldSetgPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ExtrMirrFoldSetgPen, self).__init__(signal_interface, item)


class ExtrMirrFoldSetgSts(BaseEnumSender):
    de_name     = "ExtrMirrFoldSetg.Sts"
    fdx_name    = "ExtrMirrFoldSetgSts"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ExtrMirrFoldSetgSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ExtrMirrTiltSetg2IdPen(BaseEnumSender):
    de_name     = "ExtrMirrTiltSetg2.IdPen"
    fdx_name    = "ExtrMirrTiltSetg2IdPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ExtrMirrTiltSetg2IdPen, self).__init__(signal_interface, item)


# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class ExtrMirrTiltSetg2MirrDrvr(BaseEnumSender):
    de_name     = "ExtrMirrTiltSetg2.MirrDrvr"
    fdx_name    = "ExtrMirrTiltSetg2MirrDrvr"
    fdx_groupid = 3211
    class map:
       On = 0
       Off = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ExtrMirrTiltSetg2MirrDrvr, self).__init__(signal_interface, item)


# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class ExtrMirrTiltSetg2MirrPass(BaseEnumSender):
    de_name     = "ExtrMirrTiltSetg2.MirrPass"
    fdx_name    = "ExtrMirrTiltSetg2MirrPass"
    fdx_groupid = 3211
    class map:
       On = 0
       Off = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ExtrMirrTiltSetg2MirrPass, self).__init__(signal_interface, item)


# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class FanForCmptmtTRunng(BaseEnumSender):
    de_name     = "CmptmtTFrnt.FanForCmptmtTRunng"
    fdx_name    = "FanForCmptmtTRunng"
    fdx_groupid = 3216
    class map:
       Rst = 0
       Set = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FanForCmptmtTRunng, self).__init__(signal_interface, item)


class FltEgyCnsWdSts(BaseEnumSender):
    de_name     = "VehModMngtGlbSafe1.FltEgyCnsWdSts"
    fdx_name    = "FltEgyCnsWdSts"
    fdx_groupid = 3192
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FltEgyCnsWdSts, self).__init__(signal_interface, item)


class FltIndcrTurnLeFrnt(BaseEnumSender):
    de_name     = "FltIndcrTurnLeFrnt"
    fdx_name    = "FltIndcrTurnLeFrnt"
    fdx_groupid = 3200
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FltIndcrTurnLeFrnt, self).__init__(signal_interface, item)


class FltIndcrTurnLeRe(BaseEnumSender):
    de_name     = "FltIndcrTurnLeRe"
    fdx_name    = "FltIndcrTurnLeRe"
    fdx_groupid = 3200
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FltIndcrTurnLeRe, self).__init__(signal_interface, item)


class FltIndcrTurnRiFrnt(BaseEnumSender):
    de_name     = "FltIndcrTurnRiFrnt"
    fdx_name    = "FltIndcrTurnRiFrnt"
    fdx_groupid = 3200
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FltIndcrTurnRiFrnt, self).__init__(signal_interface, item)


class FltIndcrTurnRiRe(BaseEnumSender):
    de_name     = "FltIndcrTurnRiRe"
    fdx_name    = "FltIndcrTurnRiRe"
    fdx_groupid = 3200
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FltIndcrTurnRiRe, self).__init__(signal_interface, item)


class FltOfLiDaytiRunngRi(BaseEnumSender):
    de_name     = "FltOfLiDaytiRunngRi"
    fdx_name    = "FltOfLiDaytiRunngRi"
    fdx_groupid = 3200
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FltOfLiDaytiRunngRi, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->65535
class FRNetworkStatus(BaseIntegerSender):
    de_name     = "FRNetworkStatus"
    fdx_name    = "FRNetworkStatus"
    fdx_groupid = 3228
    min = 0
    max = 65535


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FRNetworkStatus, self).__init__(signal_interface, item)


class FrntAxleWarn(BaseEnumSender):
    de_name     = "FrntAxleWarn"
    fdx_name    = "FrntAxleWarn"
    fdx_groupid = 3197
    class map:
       NoWarn = 0
       SoftWarn = 1
       HardWarn = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FrntAxleWarn, self).__init__(signal_interface, item)


class FrntWiprLvrReq2FrntWiprLvrCmd1(BaseEnumSender):
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrCmd1"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrCmd1"
    fdx_groupid = 3197
    class map:
       FrntWiprOff = 0
       FrntWiprSngStk = 1
       FrntWiprIntm = 2
       FrntWiprContnsLoSpd = 3
       FrntWiprContnsHiSpd = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FrntWiprLvrReq2FrntWiprLvrCmd1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class FrntWiprLvrReq2FrntWiprLvrCrc(BaseIntegerSender):
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrCrc"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrCrc"
    fdx_groupid = 3197
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FrntWiprLvrReq2FrntWiprLvrCrc, self).__init__(signal_interface, item)


class FrntWiprLvrReq2FrntWiprLvrQf(BaseEnumSender):
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrQf"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrQf"
    fdx_groupid = 3197
    class map:
       DevOfDataUndefd = 0
       DataTmpUndefdAndEvlnInProgs = 1
       DevOfDataNotWithinRngAllwd = 2
       DataCalcdWithDevDefd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FrntWiprLvrReq2FrntWiprLvrQf, self).__init__(signal_interface, item)


# LperHectoKiloM = L/100Km
# KiloMPerL = Km/L
# MilePerGallonEngland = MPG (UK)
# MilePerGallonUsa = MPG (US)
class FuCnsUnit(BaseEnumSender):
    de_name     = "IndcnUnit.FuCnsUnit"
    fdx_name    = "FuCnsUnit"
    fdx_groupid = 3211
    class map:
       LperHectoKiloM = 0
       KiloMPerL = 1
       MilePerGallonEngland = 2
       MilePerGallonUsa = 3
       UkwnUnit = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FuCnsUnit, self).__init__(signal_interface, item)


class FuHeatrActv(BaseEnumSender):
    de_name     = "FuHeatrActv"
    fdx_name    = "FuHeatrActv"
    fdx_groupid = 3198
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FuHeatrActv, self).__init__(signal_interface, item)


# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class FuHeatrFuCns1(BaseFloatSender):
    de_name     = "FuHeatrFuCns1"
    fdx_name    = "FuHeatrFuCns1"
    fdx_groupid = 3197
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FuHeatrFuCns1, self).__init__(signal_interface, item)


# Unit: Litre,  Range:0->1.27, Resolution: (0.01*x+0.0, raw is unsigned, 0 bits )
class FuHeatrFuCnsDurgCyc1(BaseFloatSender):
    de_name     = "FuHeatrFuCnsDurgCyc1"
    fdx_name    = "FuHeatrFuCnsDurgCyc1"
    fdx_groupid = 3198
    min    = 0
    max    = 1.27
    scale  = 0.01
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FuHeatrFuCnsDurgCyc1, self).__init__(signal_interface, item)


class FuLvlIndcdQly(BaseEnumSender):
    de_name     = "FuLvlIndcd.GenQF"
    fdx_name    = "FuLvlIndcdQly"
    fdx_groupid = 3206
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FuLvlIndcdQly, self).__init__(signal_interface, item)


# Unit: Litre,  Range:0->204.6, Resolution: (0.2*x+0.0, raw is unsigned, 10 bits )
class FuLvlIndcdVal(BaseFloatSender):
    de_name     = "FuLvlIndcd.FuLvlValFromFuTbl"
    fdx_name    = "FuLvlIndcdVal"
    fdx_groupid = 3206
    min    = 0
    max    = 204.6
    scale  = 0.2
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FuLvlIndcdVal, self).__init__(signal_interface, item)


class FuLvlLoIndcn(BaseEnumSender):
    de_name     = "FuLvlLoIndcn"
    fdx_name    = "FuLvlLoIndcn"
    fdx_groupid = 3207
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FuLvlLoIndcn, self).__init__(signal_interface, item)


class FuLvlLoWarn(BaseEnumSender):
    de_name     = "FuLvlLoWarn"
    fdx_name    = "FuLvlLoWarn"
    fdx_groupid = 3207
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(FuLvlLoWarn, self).__init__(signal_interface, item)


class GearIndcn(BaseEnumSender):
    de_name     = "GearIndcnRec.GearIndcn"
    fdx_name    = "GearIndcn"
    fdx_groupid = 3231
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(GearIndcn, self).__init__(signal_interface, item)


class GearShiftIndcn(BaseEnumSender):
    de_name     = "GearIndcnRec.GearShiftIndcn"
    fdx_name    = "GearShiftIndcn"
    fdx_groupid = 3231
    class map:
       NoShiftIndcn = 0
       ShiftUpIndcn = 1
       CoolShiftIndcn = 2
       ShiftDwnIndcn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(GearShiftIndcn, self).__init__(signal_interface, item)


class GearTarIndcn(BaseEnumSender):
    de_name     = "GearIndcnRec.GearTarIndcn"
    fdx_name    = "GearTarIndcn"
    fdx_groupid = 3231
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(GearTarIndcn, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->102.3, Resolution: (0.1*x+0.0, raw is unsigned, 10 bits )
class GrdForFuEco(BaseFloatSender):
    de_name     = "CchForFuEco.GrdForFuEco"
    fdx_name    = "GrdForFuEco"
    fdx_groupid = 3233
    min    = 0
    max    = 102.3
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(GrdForFuEco, self).__init__(signal_interface, item)


# Pairing status information for Out of band NFC pairing
class HdHwAprvdWirelsAdrAprvdSts(BaseEnumSender):
    de_name     = "HdHwAprvdWirelsAdr.AprvdSts"
    fdx_name    = "HdHwAprvdWirelsAdrAprvdSts"
    fdx_groupid = 3222
    class map:
       NoData = 0
       Setup = 1
       Request = 2
       Trusted = 3
       NotKnown = 4
       Disable = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdHwAprvdWirelsAdrAprvdSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class HdHwAprvdWirelsAdrHwOffsAdr1(BaseIntegerSender):
    de_name     = "HdHwAprvdWirelsAdr.HwOffsAdr1"
    fdx_name    = "HdHwAprvdWirelsAdrHwOffsAdr1"
    fdx_groupid = 3222
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdHwAprvdWirelsAdrHwOffsAdr1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class HdHwAprvdWirelsAdrHwOffsAdr2(BaseIntegerSender):
    de_name     = "HdHwAprvdWirelsAdr.HwOffsAdr2"
    fdx_name    = "HdHwAprvdWirelsAdrHwOffsAdr2"
    fdx_groupid = 3222
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdHwAprvdWirelsAdrHwOffsAdr2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class HdHwAprvdWirelsAdrHwOffsAdr3(BaseIntegerSender):
    de_name     = "HdHwAprvdWirelsAdr.HwOffsAdr3"
    fdx_name    = "HdHwAprvdWirelsAdrHwOffsAdr3"
    fdx_groupid = 3222
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdHwAprvdWirelsAdrHwOffsAdr3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class HdHwAprvdWirelsAdrHwOffsAdr4(BaseIntegerSender):
    de_name     = "HdHwAprvdWirelsAdr.HwOffsAdr4"
    fdx_name    = "HdHwAprvdWirelsAdrHwOffsAdr4"
    fdx_groupid = 3222
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdHwAprvdWirelsAdrHwOffsAdr4, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class HdHwAprvdWirelsAdrHwOffsAdr5(BaseIntegerSender):
    de_name     = "HdHwAprvdWirelsAdr.HwOffsAdr5"
    fdx_name    = "HdHwAprvdWirelsAdrHwOffsAdr5"
    fdx_groupid = 3222
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdHwAprvdWirelsAdrHwOffsAdr5, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class HdHwAprvdWirelsAdrHwOffsAdr6(BaseIntegerSender):
    de_name     = "HdHwAprvdWirelsAdr.HwOffsAdr6"
    fdx_name    = "HdHwAprvdWirelsAdrHwOffsAdr6"
    fdx_groupid = 3222
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdHwAprvdWirelsAdrHwOffsAdr6, self).__init__(signal_interface, item)


class HdrestFoldReq2(BaseBoolSender):
    de_name     = "HdrestFoldReq2"
    fdx_name    = "HdrestFoldReq2"
    fdx_groupid = 3217


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdrestFoldReq2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class HdTxVersInHznData(BaseIntegerSender):
    de_name     = "HznData.HdTxVers"
    fdx_name    = "HdTxVersInHznData"
    fdx_groupid = 3214
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HdTxVersInHznData, self).__init__(signal_interface, item)


class HeatrDurgDrvgReqd(BaseEnumSender):
    de_name     = "HeatrDurgDrvgReqd"
    fdx_name    = "HeatrDurgDrvgReqd"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1
       Aut = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HeatrDurgDrvgReqd, self).__init__(signal_interface, item)


class HeatrPreCdngTyp(BaseEnumSender):
    de_name     = "HeatrPreCdngTyp"
    fdx_name    = "HeatrPreCdngTyp"
    fdx_groupid = 3216
    class map:
       NoReq = 0
       Aut = 1
       Fu = 2
       Elec = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HeatrPreCdngTyp, self).__init__(signal_interface, item)


class HiQlyInTireCircumCalByNav(BaseEnumSender):
    de_name     = "TireCircumCalByNav.HiQly"
    fdx_name    = "HiQlyInTireCircumCalByNav"
    fdx_groupid = 3217
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HiQlyInTireCircumCalByNav, self).__init__(signal_interface, item)


class HmiAudSts(BaseEnumSender):
    de_name     = "HmiAudSts"
    fdx_name    = "HmiAudSts"
    fdx_groupid = 3215
    class map:
       NoFltCfmd = 0
       FltCfmd = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiAudSts, self).__init__(signal_interface, item)


class HmiCenForDrvrHmi(BaseArraySender):
    de_name     = "HmiCenForDrvrHmi"
    fdx_name    = "HmiCenForDrvrHmi"
    fdx_groupid = 3208
    array_length = 254

    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCenForDrvrHmi, self).__init__(signal_interface, item)


# Interpretation:
# 0 - Flr - Floor
# 1 - Vent - Vent
# 2 - Defrst - Defrost
# 3 - FlrDefrst - Floor / defrost
# 4 - FlrVent - Floor / vent
# 5 - VentDefrst - Vent / defrost
# 6 - FlrVentDefrst - Floor / vent / defrost
# 7 - Aut - Auto
class HmiCmptmtAirDistbnFrnt(BaseEnumSender):
    de_name     = "HmiCmptmtAirDistbnFrnt"
    fdx_name    = "HmiCmptmtAirDistbnFrnt"
    fdx_groupid = 3213
    class map:
       Flr = 0
       Vent = 1
       Defrst = 2
       FlrDefrst = 3
       FlrVent = 4
       VentDefrst = 5
       FlrVentDefrst = 6
       Aut = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtAirDistbnFrnt, self).__init__(signal_interface, item)


class HmiCmptmtCoolgReq(BaseEnumSender):
    de_name     = "HmiCmptmtCoolgReq"
    fdx_name    = "HmiCmptmtCoolgReq"
    fdx_groupid = 3222
    class map:
       Off = 0
       Auto = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtCoolgReq, self).__init__(signal_interface, item)


# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowFirstLe(BaseFloatSender):
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe"
    fdx_name    = "HmiCmptmtTSpForRowFirstLe"
    fdx_groupid = 3222
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtTSpForRowFirstLe, self).__init__(signal_interface, item)


# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowFirstRi(BaseFloatSender):
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi"
    fdx_name    = "HmiCmptmtTSpForRowFirstRi"
    fdx_groupid = 3222
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtTSpForRowFirstRi, self).__init__(signal_interface, item)


# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowSecLe(BaseFloatSender):
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowSecLe"
    fdx_name    = "HmiCmptmtTSpForRowSecLe"
    fdx_groupid = 3222
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtTSpForRowSecLe, self).__init__(signal_interface, item)


# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowSecRi(BaseFloatSender):
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowSecRi"
    fdx_name    = "HmiCmptmtTSpForRowSecRi"
    fdx_groupid = 3222
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtTSpForRowSecRi, self).__init__(signal_interface, item)


class HmiCmptmtTSpSpclForRowFirstLe(BaseEnumSender):
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe"
    fdx_name    = "HmiCmptmtTSpSpclForRowFirstLe"
    fdx_groupid = 3222
    class map:
       Norm = 0
       Lo = 1
       Hi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtTSpSpclForRowFirstLe, self).__init__(signal_interface, item)


class HmiCmptmtTSpSpclForRowFirstRi(BaseEnumSender):
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi"
    fdx_name    = "HmiCmptmtTSpSpclForRowFirstRi"
    fdx_groupid = 3222
    class map:
       Norm = 0
       Lo = 1
       Hi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtTSpSpclForRowFirstRi, self).__init__(signal_interface, item)


class HmiCmptmtTSpSpclForRowSecLe(BaseEnumSender):
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowSecLe"
    fdx_name    = "HmiCmptmtTSpSpclForRowSecLe"
    fdx_groupid = 3222
    class map:
       Norm = 0
       Lo = 1
       Hi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtTSpSpclForRowSecLe, self).__init__(signal_interface, item)


class HmiCmptmtTSpSpclForRowSecRi(BaseEnumSender):
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowSecRi"
    fdx_name    = "HmiCmptmtTSpSpclForRowSecRi"
    fdx_groupid = 3222
    class map:
       Norm = 0
       Lo = 1
       Hi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiCmptmtTSpSpclForRowSecRi, self).__init__(signal_interface, item)


class HmiDefrstElecForMirrReq(BaseEnumSender):
    de_name     = "HmiDefrstElecReq.MirrElecReq"
    fdx_name    = "HmiDefrstElecForMirrReq"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1
       AutOn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiDefrstElecForMirrReq, self).__init__(signal_interface, item)


class HmiDefrstElecFrntReq(BaseEnumSender):
    de_name     = "HmiDefrstElecReq.FrntElecReq"
    fdx_name    = "HmiDefrstElecFrntReq"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1
       AutOn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiDefrstElecFrntReq, self).__init__(signal_interface, item)


class HmiDefrstElecReReq(BaseEnumSender):
    de_name     = "HmiDefrstElecReq.ReElecReq"
    fdx_name    = "HmiDefrstElecReReq"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1
       AutOn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiDefrstElecReReq, self).__init__(signal_interface, item)


class HmiDefrstFrntSts(BaseEnumSender):
    de_name     = "HmiDefrstElecSts.Frnt"
    fdx_name    = "HmiDefrstFrntSts"
    fdx_groupid = 3200
    class map:
       Off = 0
       On = 1
       Limited = 2
       NotAvailable = 3
       TmrOff = 4
       AutoCdn = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiDefrstFrntSts, self).__init__(signal_interface, item)


class HmiDefrstMaxReq(BaseEnumSender):
    de_name     = "HmiDefrstMaxReq"
    fdx_name    = "HmiDefrstMaxReq"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1
       AutOn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiDefrstMaxReq, self).__init__(signal_interface, item)


class HmiDfrstReSts(BaseEnumSender):
    de_name     = "HmiDefrstElecSts.Re"
    fdx_name    = "HmiDfrstReSts"
    fdx_groupid = 3200
    class map:
       Off = 0
       On = 1
       Limited = 2
       NotAvailable = 3
       TmrOff = 4
       AutoCdn = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiDfrstReSts, self).__init__(signal_interface, item)


class HmiHvacFanLvlFrnt(BaseEnumSender):
    de_name     = "HmiHvacFanLvlFrnt"
    fdx_name    = "HmiHvacFanLvlFrnt"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiHvacFanLvlFrnt, self).__init__(signal_interface, item)


class HmiHvacFanLvlRe(BaseEnumSender):
    de_name     = "HmiHvacFanLvlRe"
    fdx_name    = "HmiHvacFanLvlRe"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiHvacFanLvlRe, self).__init__(signal_interface, item)


# Interpretation:
# 0 - OsaFull - Full OSA
# 1 - RecircFull - Full recirculation
# 2 - RecircFullWithTiOut - Full REC with timeout
# 3 - AutWithAirQly - AUTO with AQS
# 4 - Aut - Auto
# 5 - Resd - Reserved
class HmiHvacRecircCmd(BaseEnumSender):
    de_name     = "HmiHvacRecircCmd"
    fdx_name    = "HmiHvacRecircCmd"
    fdx_groupid = 3213
    class map:
       Aut = 0
       AutWithAirQly = 1
       RecircFull = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiHvacRecircCmd, self).__init__(signal_interface, item)


class HmiHvacReCtrl(BaseEnumSender):
    de_name     = "HmiHvacReCtrl"
    fdx_name    = "HmiHvacReCtrl"
    fdx_groupid = 3213
    class map:
       Off = 0
       OffWithNoOccpt = 1
       On = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiHvacReCtrl, self).__init__(signal_interface, item)


class HmiMirrDefrstSts(BaseEnumSender):
    de_name     = "HmiDefrstElecSts.Mirrr"
    fdx_name    = "HmiMirrDefrstSts"
    fdx_groupid = 3200
    class map:
       Off = 0
       On = 1
       Limited = 2
       NotAvailable = 3
       TmrOff = 4
       AutoCdn = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiMirrDefrstSts, self).__init__(signal_interface, item)


class HmiSeatClimaExtdHmiSeatVentnForRowSecLe(BaseEnumSender):
    de_name     = "HmiSeatClimaExtd.HmiSeatVentnForRowSecLe"
    fdx_name    = "HmiSeatClimaExtdHmiSeatVentnForRowSecLe"
    fdx_groupid = 3216
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiSeatClimaExtdHmiSeatVentnForRowSecLe, self).__init__(signal_interface, item)


class HmiSeatClimaExtdHmiSeatVentnForRowSecRi(BaseEnumSender):
    de_name     = "HmiSeatClimaExtd.HmiSeatVentnForRowSecRi"
    fdx_name    = "HmiSeatClimaExtdHmiSeatVentnForRowSecRi"
    fdx_groupid = 3216
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiSeatClimaExtdHmiSeatVentnForRowSecRi, self).__init__(signal_interface, item)


class HmiSeatHeatgForRowFirstLe(BaseEnumSender):
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowFirstLe"
    fdx_name    = "HmiSeatHeatgForRowFirstLe"
    fdx_groupid = 3216
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiSeatHeatgForRowFirstLe, self).__init__(signal_interface, item)


class HmiSeatHeatgForRowFirstRi(BaseEnumSender):
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowFirstRi"
    fdx_name    = "HmiSeatHeatgForRowFirstRi"
    fdx_groupid = 3216
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiSeatHeatgForRowFirstRi, self).__init__(signal_interface, item)


class HmiSeatHeatgForRowSecLe(BaseEnumSender):
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowSecLe"
    fdx_name    = "HmiSeatHeatgForRowSecLe"
    fdx_groupid = 3216
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiSeatHeatgForRowSecLe, self).__init__(signal_interface, item)


class HmiSeatHeatgForRowSecRi(BaseEnumSender):
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowSecRi"
    fdx_name    = "HmiSeatHeatgForRowSecRi"
    fdx_groupid = 3216
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiSeatHeatgForRowSecRi, self).__init__(signal_interface, item)


class HmiSeatVentnForRowFirstLe(BaseEnumSender):
    de_name     = "HmiSeatClima.HmiSeatVentnForRowFirstLe"
    fdx_name    = "HmiSeatVentnForRowFirstLe"
    fdx_groupid = 3216
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiSeatVentnForRowFirstLe, self).__init__(signal_interface, item)


class HmiSeatVentnForRowFirstRi(BaseEnumSender):
    de_name     = "HmiSeatClima.HmiSeatVentnForRowFirstRi"
    fdx_name    = "HmiSeatVentnForRowFirstRi"
    fdx_groupid = 3216
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HmiSeatVentnForRowFirstRi, self).__init__(signal_interface, item)


# Status of the door.
class HoodSts(BaseEnumSender):
    de_name     = "HoodSts"
    fdx_name    = "HoodSts"
    fdx_groupid = 3192
    class map:
       Ukwn = 0
       Opend = 1
       Clsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HoodSts, self).__init__(signal_interface, item)


# Unit: hours,  Range:0->23
class Hr(BaseIntegerSender):
    de_name     = "TiAndDateIndcn.Hr1"
    fdx_name    = "Hr"
    fdx_groupid = 3197
    min = 0
    max = 23


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Hr, self).__init__(signal_interface, item)


# Unit: hours,  Range:0->23
class Hr1(BaseIntegerSender):
    de_name     = "SetTiAndDate.Hour"
    fdx_name    = "Hr1"
    fdx_groupid = 3214
    min = 0
    max = 23


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Hr1, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class HudActvReqPen(BaseEnumSender):
    de_name     = "HudActvReq.Pen"
    fdx_name    = "HudActvReqPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudActvReqPen, self).__init__(signal_interface, item)


class HudActvReqSts(BaseEnumSender):
    de_name     = "HudActvReq.Sts"
    fdx_name    = "HudActvReqSts"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudActvReqSts, self).__init__(signal_interface, item)


class HudActvSts(BaseEnumSender):
    de_name     = "HudActvSts"
    fdx_name    = "HudActvSts"
    fdx_groupid = 3206
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudActvSts, self).__init__(signal_interface, item)


class HudAdjmtReq(BaseEnumSender):
    de_name     = "HudAdjmtReq"
    fdx_name    = "HudAdjmtReq"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudAdjmtReq, self).__init__(signal_interface, item)


class HudDiagcHudCircShoSts(BaseEnumSender):
    de_name     = "HudDiagc.HudCircShoSts"
    fdx_name    = "HudDiagcHudCircShoSts"
    fdx_groupid = 3206
    class map:
       NoErr = 0
       Err = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudDiagcHudCircShoSts, self).__init__(signal_interface, item)


class HudDiagcHudCricOpenSts(BaseEnumSender):
    de_name     = "HudDiagc.HudCricOpenSts"
    fdx_name    = "HudDiagcHudCricOpenSts"
    fdx_groupid = 3206
    class map:
       NoErr = 0
       Err = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudDiagcHudCricOpenSts, self).__init__(signal_interface, item)


class HudDiagcHudTSts(BaseEnumSender):
    de_name     = "HudDiagc.HudTSts"
    fdx_name    = "HudDiagcHudTSts"
    fdx_groupid = 3206
    class map:
       NoErr = 0
       Err = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudDiagcHudTSts, self).__init__(signal_interface, item)


class HudDiagcImgHudErrSts(BaseEnumSender):
    de_name     = "HudDiagc.ImgHudErrSts"
    fdx_name    = "HudDiagcImgHudErrSts"
    fdx_groupid = 3206
    class map:
       NoErr = 0
       Err = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudDiagcImgHudErrSts, self).__init__(signal_interface, item)


class HudDiagcImgHudTmpNotAvlSts(BaseEnumSender):
    de_name     = "HudDiagc.ImgHudTmpNotAvlSts"
    fdx_name    = "HudDiagcImgHudTmpNotAvlSts"
    fdx_groupid = 3206
    class map:
       Avl = 0
       NotAvl = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudDiagcImgHudTmpNotAvlSts, self).__init__(signal_interface, item)


class HudErgoSetgReq(BaseEnumSender):
    de_name     = "HudErgoSetgReq"
    fdx_name    = "HudErgoSetgReq"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudErgoSetgReq, self).__init__(signal_interface, item)


class HudSts(BaseEnumSender):
    de_name     = "HudSts"
    fdx_name    = "HudSts"
    fdx_groupid = 3206
    class map:
       HudStsAvl = 0
       HudStsCalMod = 1
       HudStsTmpNotAvl = 2
       HudStsErr = 3
       Resd1 = 4
       Resd2 = 5
       Resd3 = 6


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudSts, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct00(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct00"
    fdx_name    = "HudVisFctSetgHudFct00"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct00, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct01(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct01"
    fdx_name    = "HudVisFctSetgHudFct01"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct01, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct02(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct02"
    fdx_name    = "HudVisFctSetgHudFct02"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct02, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct03(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct03"
    fdx_name    = "HudVisFctSetgHudFct03"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct03, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct04(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct04"
    fdx_name    = "HudVisFctSetgHudFct04"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct04, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct05(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct05"
    fdx_name    = "HudVisFctSetgHudFct05"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct05, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct06(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct06"
    fdx_name    = "HudVisFctSetgHudFct06"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct06, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct07(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct07"
    fdx_name    = "HudVisFctSetgHudFct07"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct07, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct08(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct08"
    fdx_name    = "HudVisFctSetgHudFct08"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct08, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct09(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct09"
    fdx_name    = "HudVisFctSetgHudFct09"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct09, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct10(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct10"
    fdx_name    = "HudVisFctSetgHudFct10"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct10, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct11(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct11"
    fdx_name    = "HudVisFctSetgHudFct11"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct11, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct12(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct12"
    fdx_name    = "HudVisFctSetgHudFct12"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct12, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct13(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct13"
    fdx_name    = "HudVisFctSetgHudFct13"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct13, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct14(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct14"
    fdx_name    = "HudVisFctSetgHudFct14"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct14, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct15(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct15"
    fdx_name    = "HudVisFctSetgHudFct15"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct15, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct16(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct16"
    fdx_name    = "HudVisFctSetgHudFct16"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct16, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct17(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct17"
    fdx_name    = "HudVisFctSetgHudFct17"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct17, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct18(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct18"
    fdx_name    = "HudVisFctSetgHudFct18"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct18, self).__init__(signal_interface, item)


class HudVisFctSetgHudFct19(BaseEnumSender):
    de_name     = "HudVisFctSetg.HudFct19"
    fdx_name    = "HudVisFctSetgHudFct19"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgHudFct19, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class HudVisFctSetgPen(BaseEnumSender):
    de_name     = "HudVisFctSetg.Pen"
    fdx_name    = "HudVisFctSetgPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HudVisFctSetgPen, self).__init__(signal_interface, item)


# Unit: Kg / h,  Range:0->1000, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class HvacAirMFlowEstimd(BaseFloatSender):
    de_name     = "HvacAirMFlowEstimd"
    fdx_name    = "HvacAirMFlowEstimd"
    fdx_groupid = 3194
    min    = 0
    max    = 1000
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HvacAirMFlowEstimd, self).__init__(signal_interface, item)


# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class HvBattSmtActv(BaseEnumSender):
    de_name     = "HvBattSmtActv"
    fdx_name    = "HvBattSmtActv"
    fdx_groupid = 3239
    class map:
       Rst = 0
       Set = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HvBattSmtActv, self).__init__(signal_interface, item)


class HvBattSmtSeld(BaseEnumSender):
    de_name     = "HvBattSmtSeld"
    fdx_name    = "HvBattSmtSeld"
    fdx_groupid = 3212
    class map:
       NoReq = 0
       On = 1
       Off = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HvBattSmtSeld, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->511
class HwVersInHznData(BaseIntegerSender):
    de_name     = "HznData.HwVers"
    fdx_name    = "HwVersInHznData"
    fdx_groupid = 3214
    min = 0
    max = 511


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HwVersInHznData, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Ukwn		Unknown		Unknown
# 1			1			EmgyLane	Emergency Lane	Emergency lane
# 2			2			RoadLaneSng	Road Lane Single	Single-lane road
# 3			3			LaneLe		Lane Left		Left-most lane
# 4			4			LaneRi		Lane Right	Right-most lane
# 5			5			LaneMid		Lane Middle	One of middle lanes on road with three or more lanes
# 6			6			Resd		Reserved		Reserved
# 7			7			Ukwn2		Unknown 2	Unknown 2
class HznPosnExtdLanePrsnt(BaseEnumSender):
    de_name     = "HznPosnExtd.LanePrsnt"
    fdx_name    = "HznPosnExtdLanePrsnt"
    fdx_groupid = 3220
    class map:
       Ukwn = 0
       EmgyLane = 1
       RoadLaneSng = 2
       LaneLe = 3
       LaneRi = 4
       LaneMid = 5
       Resd = 6
       Ukwn2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdLanePrsnt, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved
class HznPosnExtdMsgTyp(BaseEnumSender):
    de_name     = "HznPosnExtd.MsgTyp"
    fdx_name    = "HznPosnExtdMsgTyp"
    fdx_groupid = 3220
    class map:
       Resd1 = 0
       Posn = 1
       Seg = 2
       Edge = 3
       ProfSho = 4
       ProfLong = 5
       Data = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdMsgTyp, self).__init__(signal_interface, item)


# Unit: m,  Range:0->1048448, Resolution: (128.0*x+0.0, raw is unsigned, 13 bits )
class HznPosnExtdOffsOffs(BaseFloatSender):
    de_name     = "HznPosnExtdOffs.Offs"
    fdx_name    = "HznPosnExtdOffsOffs"
    fdx_groupid = 3220
    min    = 0
    max    = 1048448
    scale  = 128.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdOffsOffs, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class HznPosnExtdPahIdx(BaseIntegerSender):
    de_name     = "HznPosnExtd.PahIdx"
    fdx_name    = "HznPosnExtdPahIdx"
    fdx_groupid = 3220
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdPahIdx, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class HznPosnExtdPosnIdx(BaseIntegerSender):
    de_name     = "HznPosnExtd.PosnIdx"
    fdx_name    = "HznPosnExtdPosnIdx"
    fdx_groupid = 3220
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdPosnIdx, self).__init__(signal_interface, item)


# Unit: %,  Range:0->103.33333323, Resolution: (3.33333333*x+0.0, raw is unsigned, 5 bits )
class HznPosnExtdPosnProblty(BaseFloatSender):
    de_name     = "HznPosnExtd.PosnProblty"
    fdx_name    = "HznPosnExtdPosnProblty"
    fdx_groupid = 3220
    min    = 0
    max    = 103.33333323
    scale  = 3.33333333
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdPosnProblty, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->7
class HznPosnExtdPosnQly(BaseIntegerSender):
    de_name     = "HznPosnExtd.PosnQly"
    fdx_name    = "HznPosnExtdPosnQly"
    fdx_groupid = 3220
    min = 0
    max = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdPosnQly, self).__init__(signal_interface, item)


# Unit: ms,  Range:0->2555, Resolution: (5.0*x+0.0, raw is unsigned, 9 bits )
class HznPosnExtdPosnTiDif(BaseFloatSender):
    de_name     = "HznPosnExtd.PosnTiDif"
    fdx_name    = "HznPosnExtdPosnTiDif"
    fdx_groupid = 3220
    min    = 0
    max    = 2555
    scale  = 5.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdPosnTiDif, self).__init__(signal_interface, item)


# Unit: Deg,  Range:0->360, Resolution: (1.4173228346456692*x+0.0, raw is unsigned, 8 bits )
class HznPosnExtdRelDir(BaseFloatSender):
    de_name     = "HznPosnExtd.RelDir"
    fdx_name    = "HznPosnExtdRelDir"
    fdx_groupid = 3220
    min    = 0
    max    = 360
    scale  = 1.4173228346456692
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdRelDir, self).__init__(signal_interface, item)


# Unit: m/s,  Range:-12.8->89.4, Resolution: (0.2*x+-12.8, raw is unsigned, 9 bits )
class HznPosnExtdSpd(BaseFloatSender):
    de_name     = "HznPosnExtd.Spd"
    fdx_name    = "HznPosnExtdSpd"
    fdx_groupid = 3220
    min    = -12.8
    max    = 89.4
    scale  = 0.2
    offset = -12.8


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznPosnExtdSpd, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved
class HznProfLongExtdMsgTyp(BaseEnumSender):
    de_name     = "HznProfLongExtd.MsgTyp"
    fdx_name    = "HznProfLongExtdMsgTyp"
    fdx_groupid = 3220
    class map:
       Resd1 = 0
       Posn = 1
       Seg = 2
       Edge = 3
       ProfSho = 4
       ProfLong = 5
       Data = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznProfLongExtdMsgTyp, self).__init__(signal_interface, item)


class HznProfLongExtdNodCtrl(BaseEnumSender):
    de_name     = "HznProfLongExtd.NodCtrl"
    fdx_name    = "HznProfLongExtdNodCtrl"
    fdx_groupid = 3220
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznProfLongExtdNodCtrl, self).__init__(signal_interface, item)


# Unit: m,  Range:0->1048448, Resolution: (128.0*x+0.0, raw is unsigned, 13 bits )
class HznProfLongExtdOffs(BaseFloatSender):
    de_name     = "HznProfLongExtd.Offs"
    fdx_name    = "HznProfLongExtdOffs"
    fdx_groupid = 3220
    min    = 0
    max    = 1048448
    scale  = 128.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznProfLongExtdOffs, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class HznProfLongExtdPahIdx(BaseIntegerSender):
    de_name     = "HznProfLongExtd.PahIdx"
    fdx_name    = "HznProfLongExtdPahIdx"
    fdx_groupid = 3220
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznProfLongExtdPahIdx, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
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
class HznProfLongExtdProfTyp(BaseEnumSender):
    de_name     = "HznProfLongExtd.ProfTyp"
    fdx_name    = "HznProfLongExtdProfTyp"
    fdx_groupid = 3220
    class map:
       NotInUse = 0
       Lgt = 1
       Lat = 2
       Alti = 3
       NodCtrlForLgt = 4
       NodCtrlForLat = 5
       NodCtrlForAlti = 6
       Id = 7
       SpdRng = 16
       AltiAvg = 17
       DestOrInterDest = 18
       SpdFromTrfcInfo = 20
       LvlOfSrvFromTrfcInfo = 21
       SpdFromMap = 22
       EvChrgnLocn = 23
       VehStopLocn = 24
       Slop = 25
       Ukw = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznProfLongExtdProfTyp, self).__init__(signal_interface, item)


class HznProfLongExtdTxPrev(BaseEnumSender):
    de_name     = "HznProfLongExtd.TxPrev"
    fdx_name    = "HznProfLongExtdTxPrev"
    fdx_groupid = 3220
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznProfLongExtdTxPrev, self).__init__(signal_interface, item)


class HznProfLongExtdUpd(BaseEnumSender):
    de_name     = "HznProfLongExtd.Upd"
    fdx_name    = "HznProfLongExtdUpd"
    fdx_groupid = 3220
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznProfLongExtdUpd, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class HznProfLongExtdVal(BaseIntegerSender):
    de_name     = "HznProfLongExtd.Val"
    fdx_name    = "HznProfLongExtdVal"
    fdx_groupid = 3220
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznProfLongExtdVal, self).__init__(signal_interface, item)


class HznRstExtd(BaseBoolSender):
    de_name     = "HznRstExtd"
    fdx_name    = "HznRstExtd"
    fdx_groupid = 3244


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznRstExtd, self).__init__(signal_interface, item)


# EH (Electronic Horizon) provider status and support.
class HznSplyElectcSts(BaseEnumSender):
    de_name     = "HznSplyElectcSts"
    fdx_name    = "HznSplyElectcSts"
    fdx_groupid = 3217
    class map:
       Ukwn = 0
       NoSpprt = 1
       Failr = 2
       Spprt = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HznSplyElectcSts, self).__init__(signal_interface, item)


class HzrdLiWarnActv(BaseEnumSender):
    de_name     = "HzrdLiWarnActv"
    fdx_name    = "HzrdLiWarnActv"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HzrdLiWarnActv, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class HzrdLiWarnSts(BaseEnumSender):
    de_name     = "HzrdLiWarnSts"
    fdx_name    = "HzrdLiWarnSts"
    fdx_groupid = 3189
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(HzrdLiWarnSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IdPenForUnits(BaseEnumSender):
    de_name     = "IndcnUnit.IdPenForUnit"
    fdx_name    = "IdPenForUnits"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IdPenForUnits, self).__init__(signal_interface, item)


class IndcnOfParkAssiSts(BaseEnumSender):
    de_name     = "IndcnOfParkAssiSts"
    fdx_name    = "IndcnOfParkAssiSts"
    fdx_groupid = 3223
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IndcnOfParkAssiSts, self).__init__(signal_interface, item)


class IndcnOfPrkgAutSts(BaseEnumSender):
    de_name     = "IndcnOfPrkgAutSts"
    fdx_name    = "IndcnOfPrkgAutSts"
    fdx_groupid = 3223
    class map:
       Off = 0
       PrkgTypAutSeln = 1
       PrkgTypPara = 2
       PrkgTypPerp = 3
       PrkgOutManv = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IndcnOfPrkgAutSts, self).__init__(signal_interface, item)


# DateFmt1 = YMD
# DateFmt2 = DMY
# DateFmt3 = MDY
class IndcnUnitDateFmt(BaseEnumSender):
    de_name     = "IndcnUnit.DateFmt"
    fdx_name    = "IndcnUnitDateFmt"
    fdx_groupid = 3211
    class map:
       DateFmt1 = 0
       DateFmt2 = 1
       DateFmt3 = 2
       UkwnFmt = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IndcnUnitDateFmt, self).__init__(signal_interface, item)


class IndcrDisp1WdSts(BaseEnumSender):
    de_name     = "IndcrDisp1WdSts"
    fdx_name    = "IndcrDisp1WdSts"
    fdx_groupid = 3193
    class map:
       Off = 0
       LeOn = 1
       RiOn = 2
       LeAndRiOn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IndcrDisp1WdSts, self).__init__(signal_interface, item)


class IndcrTurnSts1WdSts(BaseEnumSender):
    de_name     = "IndcrTurnSts1WdSts"
    fdx_name    = "IndcrTurnSts1WdSts"
    fdx_groupid = 3192
    class map:
       Off = 0
       LeOn = 1
       RiOn = 2
       LeAndRiOn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IndcrTurnSts1WdSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->65535
class IniValSigCfgIDBackboneFR(BaseIntegerSender):
    de_name     = "IniValSigCfgIDBackboneFR"
    fdx_name    = "IniValSigCfgIDBackboneFR"
    fdx_groupid = 3209
    min = 0
    max = 65535


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IniValSigCfgIDBackboneFR, self).__init__(signal_interface, item)


# Unit: dB,  Range:0->127.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class InsdCarNoiseMeasd(BaseFloatSender):
    de_name     = "InsdCarNoiseMeasd"
    fdx_name    = "InsdCarNoiseMeasd"
    fdx_groupid = 3212
    min    = 0
    max    = 127.5
    scale  = 0.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(InsdCarNoiseMeasd, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class IntrBriSts(BaseIntegerSender):
    de_name     = "IntrBriSts"
    fdx_name    = "IntrBriSts"
    fdx_groupid = 3192
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrBriSts, self).__init__(signal_interface, item)


class IntrLiAmbLiSetgForLiInten(BaseEnumSender):
    de_name     = "IntrLiAmbLiSetg.LiInten"
    fdx_name    = "IntrLiAmbLiSetgForLiInten"
    fdx_groupid = 3211
    class map:
       Off = 0
       IntenLo = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrLiAmbLiSetgForLiInten, self).__init__(signal_interface, item)


# Amibiance LIgt color setting:
# 0 = Temperature
# 1 = Off
# 2 - 8 = Colour 1 - 7
class IntrLiAmbLiSetgForLiTintg(BaseEnumSender):
    de_name     = "IntrLiAmbLiSetg.LiTintg"
    fdx_name    = "IntrLiAmbLiSetgForLiTintg"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrLiAmbLiSetgForLiTintg, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrLiAmbLiSetgPen(BaseEnumSender):
    de_name     = "IntrLiAmbLiSetg.Pen"
    fdx_name    = "IntrLiAmbLiSetgPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrLiAmbLiSetgPen, self).__init__(signal_interface, item)


class IntrLiSurrndgsLiSetgForLiInten(BaseEnumSender):
    de_name     = "IntrLiSurrndgsLiSetgLi.LiInten"
    fdx_name    = "IntrLiSurrndgsLiSetgForLiInten"
    fdx_groupid = 3211
    class map:
       Off = 0
       IntenLo = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrLiSurrndgsLiSetgForLiInten, self).__init__(signal_interface, item)


# 0 = Full
# 1 = Reduced
class IntrLiSurrndgsLiSetgForLiLvl(BaseEnumSender):
    de_name     = "IntrLiSurrndgsLiSetgLi.LiLvl"
    fdx_name    = "IntrLiSurrndgsLiSetgForLiLvl"
    fdx_groupid = 3211
    class map:
       LvlFull = 0
       LvlReduced = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrLiSurrndgsLiSetgForLiLvl, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrLiSurrndgsLiSetgPen(BaseEnumSender):
    de_name     = "IntrLiSurrndgsLiSetgLi.Pen"
    fdx_name    = "IntrLiSurrndgsLiSetgPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrLiSurrndgsLiSetgPen, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrMirrTintgSetgPen(BaseEnumSender):
    de_name     = "IntrMirrTintgSetg.Pen"
    fdx_name    = "IntrMirrTintgSetgPen"
    fdx_groupid = 3216
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrMirrTintgSetgPen, self).__init__(signal_interface, item)


class IntrMirrTintgSetgSts(BaseEnumSender):
    de_name     = "IntrMirrTintgSetg.MirrDimLvl"
    fdx_name    = "IntrMirrTintgSetgSts"
    fdx_groupid = 3216
    class map:
       Normal = 0
       Dark = 1
       Light = 2
       Inhibit = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntrMirrTintgSetgSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			SegNotPartOfIntscnCmplx	Segment Not Part Of Intersection Complex	Segment is not part of complex intersection
# 1			1			SegPartOfIntscnCmplx	Segment Part Of Intersection Complex		Segment is part of a complex intersection
# 2			2			Ukwn			Unknown					Unknown
class IntscnCmplxInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.IntscnCmplx"
    fdx_name    = "IntscnCmplxInHznEdge"
    fdx_groupid = 3215
    class map:
       SegNotPartOfIntscnCmplx = 0
       SegPartOfIntscnCmplx = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntscnCmplxInHznEdge, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			SegNotPartOfIntscnCmplx	Segment Not Part Of Intersection Complex	Segment is not part of complex intersection
# 1			1			SegPartOfIntscnCmplx	Segment Part Of Intersection Complex		Segment is part of a complex intersection
# 2			2			Ukwn			Unknown					Unknown
class IntscnCmplxInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.IntscnCmplx"
    fdx_name    = "IntscnCmplxInHznSeg"
    fdx_groupid = 3215
    class map:
       SegNotPartOfIntscnCmplx = 0
       SegPartOfIntscnCmplx = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntscnCmplxInHznSeg, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntvAndWarnModForLaneKeepAidPen(BaseEnumSender):
    de_name     = "IntvAndWarnModForLaneKeepAid.Pen"
    fdx_name    = "IntvAndWarnModForLaneKeepAidPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntvAndWarnModForLaneKeepAidPen, self).__init__(signal_interface, item)


class IntvAndWarnModForLaneKeepAidSts(BaseEnumSender):
    de_name     = "IntvAndWarnModForLaneKeepAid.Sts"
    fdx_name    = "IntvAndWarnModForLaneKeepAidSts"
    fdx_groupid = 3213
    class map:
       WarnAndIntv = 0
       Intv = 1
       Warn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(IntvAndWarnModForLaneKeepAidSts, self).__init__(signal_interface, item)


# Warning status
# == 0	Calibration status OK (Normal IHU behavior)
# == 1	Calibration not possible (All 4 rolling wheels shall be grey, menu item for recalibration of iTPMS system shall be unavailable and gray).
class iTPMSCalPsbl(BaseEnumSender):
    de_name     = "iTPMSCalPsbl"
    fdx_name    = "iTPMSCalPsbl"
    fdx_groupid = 3244
    class map:
       CalPsbl = 0
       CalNoPsbl = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(iTPMSCalPsbl, self).__init__(signal_interface, item)


# Used for sending a Start or a Stop/Cancel request from HMI to a core function
class iTPMSCalReq(BaseEnumSender):
    de_name     = "iTPMSCalReq"
    fdx_name    = "iTPMSCalReq"
    fdx_groupid = 3217
    class map:
       Idle = 0
       StrtReq = 1
       StopReq = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(iTPMSCalReq, self).__init__(signal_interface, item)


# Calibration status
class iTPMSCalSts(BaseEnumSender):
    de_name     = "iTPMSCalSts"
    fdx_name    = "iTPMSCalSts"
    fdx_groupid = 3244
    class map:
       NoCalReq = 0
       CalOn = 1
       CalCmpl = 2
       CalFaild = 3
       Resd1 = 4
       Resd2 = 5
       Resd3 = 6


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(iTPMSCalSts, self).__init__(signal_interface, item)


# iTPMS system status and warnings
class iTPMSTirePMSts(BaseEnumSender):
    de_name     = "iTPMSTirePMSts"
    fdx_name    = "iTPMSTirePMSts"
    fdx_groupid = 3244
    class map:
       NoWarn = 0
       CmnWarn = 1
       WarnFL = 2
       WarnFR = 3
       WarnRL = 4
       WarnRR = 5
       SysUnAvi = 6
       SysFailr = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(iTPMSTirePMSts, self).__init__(signal_interface, item)


class KeyLostWarnIndcn(BaseEnumSender):
    de_name     = "KeyLostWarnIndcn"
    fdx_name    = "KeyLostWarnIndcn"
    fdx_groupid = 3193
    class map:
       KeyLostNoMsg = 0
       KeyMsgAndSoundLost = 1
       KeyMsgLost = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(KeyLostWarnIndcn, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class KeyProfMpgUpdForIdPen(BaseEnumSender):
    de_name     = "KeyProfMpgUpd.KeyProfMpgUpdForIdPen"
    fdx_name    = "KeyProfMpgUpdForIdPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(KeyProfMpgUpdForIdPen, self).__init__(signal_interface, item)


class KeyProfMpgUpdKeyProfMpgUpdOff(BaseBoolSender):
    de_name     = "KeyProfMpgUpd.KeyProfMpgUpdOff"
    fdx_name    = "KeyProfMpgUpdKeyProfMpgUpdOff"
    fdx_groupid = 3215


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(KeyProfMpgUpdKeyProfMpgUpdOff, self).__init__(signal_interface, item)


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
class KeyReadReqFromSetgMgr(BaseEnumSender):
    de_name     = "KeyReadReqFromSetgMgr"
    fdx_name    = "KeyReadReqFromSetgMgr"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(KeyReadReqFromSetgMgr, self).__init__(signal_interface, item)


class KeyReadStsToProfCtrlBoolean(BaseBoolSender):
    de_name     = "KeyReadStsToProfCtrl.Boolean"
    fdx_name    = "KeyReadStsToProfCtrlBoolean"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(KeyReadStsToProfCtrlBoolean, self).__init__(signal_interface, item)


class KeyReadStsToProfCtrlKeyId(BaseEnumSender):
    de_name     = "KeyReadStsToProfCtrl.KeyId"
    fdx_name    = "KeyReadStsToProfCtrlKeyId"
    fdx_groupid = 3194
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(KeyReadStsToProfCtrlKeyId, self).__init__(signal_interface, item)


class KeyRmnIndcn(BaseEnumSender):
    de_name     = "KeyRmnIndcn"
    fdx_name    = "KeyRmnIndcn"
    fdx_groupid = 3193
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(KeyRmnIndcn, self).__init__(signal_interface, item)


class KeySpdWarn(BaseEnumSender):
    de_name     = "KeySpdWarn"
    fdx_name    = "KeySpdWarn"
    fdx_groupid = 3206
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(KeySpdWarn, self).__init__(signal_interface, item)


# Used be Connected service booking to inform Driver information about the current status of a service booking.
class LampSuppSrv(BaseEnumSender):
    de_name     = "LampSuppSrv"
    fdx_name    = "LampSuppSrv"
    fdx_groupid = 3217
    class map:
       NoSrv = 0
       Srv = 1
       CfmdSrv = 2
       SrvPassd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LampSuppSrv, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LaneChgWarnActvPen(BaseEnumSender):
    de_name     = "LaneChgWarnActv.Pen"
    fdx_name    = "LaneChgWarnActvPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneChgWarnActvPen, self).__init__(signal_interface, item)


class LaneChgWarnActvSts(BaseEnumSender):
    de_name     = "LaneChgWarnActv.Sts"
    fdx_name    = "LaneChgWarnActvSts"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneChgWarnActvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class LaneChgWarnSts(BaseEnumSender):
    de_name     = "LaneChgWarnSts"
    fdx_name    = "LaneChgWarnSts"
    fdx_groupid = 3188
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneChgWarnSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoLaneDetd	No Lane Detedcted		No lane tracked
# 1			1			LaneLeDetd	Lane Left Detected		Left lane marking tracked
# 2			2			LaneRiDetd	Lane Right Detected	Right lane marking tracked
# 3			3			LaneLeAndRiDetd	Lane Left And Right Detected	Left and right lane marking tracked
class LaneDetnStsForAutDrv(BaseEnumSender):
    de_name     = "LaneDetnStsForAutDrv"
    fdx_name    = "LaneDetnStsForAutDrv"
    fdx_groupid = 3187
    class map:
       NoLaneDetd = 0
       LaneLeDetd = 1
       LaneRiDetd = 2
       LaneLeAndRiDetd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneDetnStsForAutDrv, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoLaneDetd	No Lane Detedcted		No lane tracked
# 1			1			LaneLeDetd	Lane Left Detected		Left lane marking tracked
# 2			2			LaneRiDetd	Lane Right Detected	Right lane marking tracked
# 3			3			LaneLeAndRiDetd	Lane Left And Right Detected	Left and right lane marking tracked
class LaneDetnStsForLaneKeepAid(BaseEnumSender):
    de_name     = "LaneDetnStsForLaneKeepAid"
    fdx_name    = "LaneDetnStsForLaneKeepAid"
    fdx_groupid = 3187
    class map:
       NoLaneDetd = 0
       LaneLeDetd = 1
       LaneRiDetd = 2
       LaneLeAndRiDetd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneDetnStsForLaneKeepAid, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LaneKeepAidActvPen(BaseEnumSender):
    de_name     = "LaneKeepAidActv.Pen"
    fdx_name    = "LaneKeepAidActvPen"
    fdx_groupid = 3211
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneKeepAidActvPen, self).__init__(signal_interface, item)


class LaneKeepAidActvSts(BaseEnumSender):
    de_name     = "LaneKeepAidActv.Sts"
    fdx_name    = "LaneKeepAidActvSts"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneKeepAidActvSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LaneKeepAidRoadEdgeActvPen(BaseEnumSender):
    de_name     = "LaneKeepAidRoadEdgeActv.Pen"
    fdx_name    = "LaneKeepAidRoadEdgeActvPen"
    fdx_groupid = 3216
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneKeepAidRoadEdgeActvPen, self).__init__(signal_interface, item)


class LaneKeepAidRoadEdgeActvSts(BaseEnumSender):
    de_name     = "LaneKeepAidRoadEdgeActv.Sts"
    fdx_name    = "LaneKeepAidRoadEdgeActvSts"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneKeepAidRoadEdgeActvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class LaneKeepAidSts(BaseEnumSender):
    de_name     = "LaneKeepAidSts"
    fdx_name    = "LaneKeepAidSts"
    fdx_groupid = 3188
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LaneKeepAidSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Ukwn		Unknown		Unknown
# 1			1			EmgyLane	Emergency Lane	Emergency lane
# 2			2			RoadLaneSng	Road Lane Single	Single-lane road
# 3			3			LaneLe		Lane Left		Left-most lane
# 4			4			LaneRi		Lane Right	Right-most lane
# 5			5			LaneMid		Lane Middle	One of middle lanes on road with three or more lanes
# 6			6			Resd		Reserved		Reserved
# 7			7			Ukwn2		Unknown 2	Unknown 2
class LanePrsntInHznPosn(BaseEnumSender):
    de_name     = "HznPosn.LanePrsnt"
    fdx_name    = "LanePrsntInHznPosn"
    fdx_groupid = 3212
    class map:
       Ukwn = 0
       EmgyLane = 1
       RoadLaneSng = 2
       LaneLe = 3
       LaneRi = 4
       LaneMid = 5
       Resd = 6
       Ukwn2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LanePrsntInHznPosn, self).__init__(signal_interface, item)


class LatMovmtWarn(BaseEnumSender):
    de_name     = "LatMovmtWarn"
    fdx_name    = "LatMovmtWarn"
    fdx_groupid = 3183
    class map:
       NoWarn = 0
       WarnLe = 1
       WarnRi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LatMovmtWarn, self).__init__(signal_interface, item)


class LcmaOn1(BaseEnumSender):
    de_name     = "LcmaOn1"
    fdx_name    = "LcmaOn1"
    fdx_groupid = 3188
    class map:
       StrtUpOn = 0
       On = 1
       Off = 2
       TrlrOff = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LcmaOn1, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiAdpvReqPen(BaseEnumSender):
    de_name     = "LiExtReq1WdReq1.Pen"
    fdx_name    = "LiAdpvReqPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiAdpvReqPen, self).__init__(signal_interface, item)


class LiAdpvReqSts(BaseEnumSender):
    de_name     = "LiExtReq1WdReq1.Sts"
    fdx_name    = "LiAdpvReqSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiAdpvReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiAutTranReqPen(BaseEnumSender):
    de_name     = "LiExtReq2WdReq1.Pen"
    fdx_name    = "LiAutTranReqPen"
    fdx_groupid = 3217
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiAutTranReqPen, self).__init__(signal_interface, item)


# SntvyLo = Sensitivity Low, light transition happens later
# SntvyLo = Sensitivity Norma
# SntvyLo = Sensitivity High, light transition happens earlier
class LiAutTranReqSts(BaseEnumSender):
    de_name     = "LiExtReq2WdReq1.Sts"
    fdx_name    = "LiAutTranReqSts"
    fdx_groupid = 3217
    class map:
       SnvtyLo = 0
       SnvtyNorm = 1
       SnvtyHi = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiAutTranReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiBeamHiAuxReqPen(BaseEnumSender):
    de_name     = "LiExtReq1WdReq4.Pen"
    fdx_name    = "LiBeamHiAuxReqPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiBeamHiAuxReqPen, self).__init__(signal_interface, item)


class LiBeamHiAuxReqSts(BaseEnumSender):
    de_name     = "LiExtReq1WdReq4.Sts"
    fdx_name    = "LiBeamHiAuxReqSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiBeamHiAuxReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiCornrgReqPen(BaseEnumSender):
    de_name     = "LiExtReq1WdReq6.Pen"
    fdx_name    = "LiCornrgReqPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiCornrgReqPen, self).__init__(signal_interface, item)


class LiCornrgReqSts(BaseEnumSender):
    de_name     = "LiExtReq1WdReq6.Sts"
    fdx_name    = "LiCornrgReqSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiCornrgReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiDaytiRunngReqPen(BaseEnumSender):
    de_name     = "LiExtReq1WdReq2.Pen"
    fdx_name    = "LiDaytiRunngReqPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiDaytiRunngReqPen, self).__init__(signal_interface, item)


class LiDaytiRunngReqSts(BaseEnumSender):
    de_name     = "LiExtReq1WdReq2.Sts"
    fdx_name    = "LiDaytiRunngReqSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiDaytiRunngReqSts, self).__init__(signal_interface, item)


class LiDrvrFltIndcrTurn(BaseEnumSender):
    de_name     = "LiDrvrFltIndcrTurn"
    fdx_name    = "LiDrvrFltIndcrTurn"
    fdx_groupid = 3198
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiDrvrFltIndcrTurn, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiExtReq1WdReq5IdPen(BaseEnumSender):
    de_name     = "LiExtReq1WdReq5.IdPen"
    fdx_name    = "LiExtReq1WdReq5IdPen"
    fdx_groupid = 3216
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiExtReq1WdReq5IdPen, self).__init__(signal_interface, item)


class LiExtReq1WdReq5SlowNormFast(BaseEnumSender):
    de_name     = "LiExtReq1WdReq5.SlowNormFast"
    fdx_name    = "LiExtReq1WdReq5SlowNormFast"
    fdx_groupid = 3216
    class map:
       Ukwn = 0
       Slow = 1
       Norm = 2
       Fast = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiExtReq1WdReq5SlowNormFast, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiHomeLvngReqPen(BaseEnumSender):
    de_name     = "LiExtSafe1WdReq2.Pen"
    fdx_name    = "LiHomeLvngReqPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiHomeLvngReqPen, self).__init__(signal_interface, item)


class LiHomeLvngReqSts(BaseEnumSender):
    de_name     = "LiExtSafe1WdReq2.Sts"
    fdx_name    = "LiHomeLvngReqSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiHomeLvngReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiHomeSafeReqPen(BaseEnumSender):
    de_name     = "LiExtSafe1WdReq1.Pen"
    fdx_name    = "LiHomeSafeReqPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiHomeSafeReqPen, self).__init__(signal_interface, item)


# Enumeration for intervals in tenths of seconds.
# E.g
# 3 = 30 seconds
# 4 = 40 seconds
class LiHomeSafeReqSts(BaseEnumSender):
    de_name     = "LiExtSafe1WdReq1.Sts"
    fdx_name    = "LiHomeSafeReqSts"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiHomeSafeReqSts, self).__init__(signal_interface, item)


class LiLvrSwt1(BaseEnumSender):
    de_name     = "LiLvrSwt1Req"
    fdx_name    = "LiLvrSwt1"
    fdx_groupid = 3192
    class map:
       NotPsd = 0
       Psd = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiLvrSwt1, self).__init__(signal_interface, item)


class LiPassFltIndcrTurn(BaseEnumSender):
    de_name     = "LiPassFltIndcrTurn"
    fdx_name    = "LiPassFltIndcrTurn"
    fdx_groupid = 3198
    class map:
       NoFlt = 0
       Flt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiPassFltIndcrTurn, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiSeldForDrvrPfmncMonPen(BaseEnumSender):
    de_name     = "LiSeldForDrvrPfmncMon.Pen"
    fdx_name    = "LiSeldForDrvrPfmncMonPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiSeldForDrvrPfmncMonPen, self).__init__(signal_interface, item)


class LiSeldForDrvrPfmncMonSts(BaseEnumSender):
    de_name     = "LiSeldForDrvrPfmncMon.Sts"
    fdx_name    = "LiSeldForDrvrPfmncMonSts"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiSeldForDrvrPfmncMonSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ListOfNodAvListOfNodAv1(BaseIntegerSender):
    de_name     = "ListOfNodAv.ListOfNodAv1"
    fdx_name    = "ListOfNodAvListOfNodAv1"
    fdx_groupid = 3201
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ListOfNodAvListOfNodAv1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ListOfNodAvListOfNodAv2(BaseIntegerSender):
    de_name     = "ListOfNodAv.ListOfNodAv2"
    fdx_name    = "ListOfNodAvListOfNodAv2"
    fdx_groupid = 3201
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ListOfNodAvListOfNodAv2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ListOfNodAvListOfNodAv3(BaseIntegerSender):
    de_name     = "ListOfNodAv.ListOfNodAv3"
    fdx_name    = "ListOfNodAvListOfNodAv3"
    fdx_groupid = 3201
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ListOfNodAvListOfNodAv3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ListOfNodAvListOfNodAv4(BaseIntegerSender):
    de_name     = "ListOfNodAv.ListOfNodAv4"
    fdx_name    = "ListOfNodAvListOfNodAv4"
    fdx_groupid = 3201
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ListOfNodAvListOfNodAv4, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ListOfNodAvListOfNodAv5(BaseIntegerSender):
    de_name     = "ListOfNodAv.ListOfNodAv5"
    fdx_name    = "ListOfNodAvListOfNodAv5"
    fdx_groupid = 3201
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ListOfNodAvListOfNodAv5, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ListOfNodAvListOfNodAv6(BaseIntegerSender):
    de_name     = "ListOfNodAv.ListOfNodAv6"
    fdx_name    = "ListOfNodAvListOfNodAv6"
    fdx_groupid = 3201
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ListOfNodAvListOfNodAv6, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ListOfNodAvListOfNodAv7(BaseIntegerSender):
    de_name     = "ListOfNodAv.ListOfNodAv7"
    fdx_name    = "ListOfNodAvListOfNodAv7"
    fdx_groupid = 3201
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ListOfNodAvListOfNodAv7, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ListOfNodAvListOfNodAv8(BaseIntegerSender):
    de_name     = "ListOfNodAv.ListOfNodAv8"
    fdx_name    = "ListOfNodAvListOfNodAv8"
    fdx_groupid = 3201
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ListOfNodAvListOfNodAv8, self).__init__(signal_interface, item)


# TrfcSideTmpLe = Traffic Side Temporary Left. Vehicle drives on left side of road
# TrfcSideTmpRi =  Traffic Side Temporary Right. Vehicle drives on right side of road
class LiTrfcSideReq(BaseEnumSender):
    de_name     = "LiTrfcSide1WdReq1"
    fdx_name    = "LiTrfcSideReq"
    fdx_groupid = 3215
    class map:
       Off = 0
       TrfcSideTmpLe = 1
       TrfcSideTmpRi = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LiTrfcSideReq, self).__init__(signal_interface, item)


class LockgCenStsForUsrFb(BaseEnumSender):
    de_name     = "LockgCenStsForUsrFb"
    fdx_name    = "LockgCenStsForUsrFb"
    fdx_groupid = 3200
    class map:
       Undefd = 0
       Opend = 1
       Clsd = 2
       Lockd = 3
       Safe = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgCenStsForUsrFb, self).__init__(signal_interface, item)


class LockgCenStsLockSt(BaseEnumSender):
    de_name     = "LockgCenSts.LockSt"
    fdx_name    = "LockgCenStsLockSt"
    fdx_groupid = 3193
    class map:
       LockUndefd = 0
       LockUnlckd = 1
       LockTrUnlckd = 2
       LockLockd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgCenStsLockSt, self).__init__(signal_interface, item)


class LockgCenStsUpdEve(BaseBoolSender):
    de_name     = "LockgCenSts.UpdEve"
    fdx_name    = "LockgCenStsUpdEve"
    fdx_groupid = 3193


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgCenStsUpdEve, self).__init__(signal_interface, item)


class LockgCenTrigSrc(BaseEnumSender):
    de_name     = "LockgCenSts.TrigSrc"
    fdx_name    = "LockgCenTrigSrc"
    fdx_groupid = 3193
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgCenTrigSrc, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockgFbSoundReqPen(BaseEnumSender):
    de_name     = "LockgFbSoundReq.Pen"
    fdx_name    = "LockgFbSoundReqPen"
    fdx_groupid = 3214
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgFbSoundReqPen, self).__init__(signal_interface, item)


class LockgFbSoundReqSts(BaseEnumSender):
    de_name     = "LockgFbSoundReq.Sts"
    fdx_name    = "LockgFbSoundReqSts"
    fdx_groupid = 3214
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgFbSoundReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockgFbVisReqPen(BaseEnumSender):
    de_name     = "LockgFbVisReq.Pen"
    fdx_name    = "LockgFbVisReqPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgFbVisReqPen, self).__init__(signal_interface, item)


class LockgFbVisReqSts(BaseEnumSender):
    de_name     = "LockgFbVisReq.Sts"
    fdx_name    = "LockgFbVisReqSts"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgFbVisReqSts, self).__init__(signal_interface, item)


class LockgPrsnlReqFromHmi(BaseEnumSender):
    de_name     = "LockgPrsnlReqFromHmi"
    fdx_name    = "LockgPrsnlReqFromHmi"
    fdx_groupid = 3215
    class map:
       NoReq = 0
       On = 1
       Off = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgPrsnlReqFromHmi, self).__init__(signal_interface, item)


class LockgPrsnlSts(BaseEnumSender):
    de_name     = "LockgPrsnlSts"
    fdx_name    = "LockgPrsnlSts"
    fdx_groupid = 3196
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockgPrsnlSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockSpdReqPen(BaseEnumSender):
    de_name     = "LockSpdReq.Pen"
    fdx_name    = "LockSpdReqPen"
    fdx_groupid = 3214
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockSpdReqPen, self).__init__(signal_interface, item)


class LockSpdReqSts(BaseEnumSender):
    de_name     = "LockSpdReq.Sts"
    fdx_name    = "LockSpdReqSts"
    fdx_groupid = 3214
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LockSpdReqSts, self).__init__(signal_interface, item)


class LstEdgeAtOffsInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.LstEdgeAtOffs"
    fdx_name    = "LstEdgeAtOffsInHznEdge"
    fdx_groupid = 3215
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LstEdgeAtOffsInHznEdge, self).__init__(signal_interface, item)


class LvlCtrlSetg(BaseEnumSender):
    de_name     = "SuspSetg.LvlCtrlSetg"
    fdx_name    = "LvlCtrlSetg"
    fdx_groupid = 3215
    class map:
       Cmft = 0
       Norm = 1
       Dyn = 2
       XC = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LvlCtrlSetg, self).__init__(signal_interface, item)


class LvlOfClimaCmft(BaseEnumSender):
    de_name     = "LvlOfClimaCmft"
    fdx_name    = "LvlOfClimaCmft"
    fdx_groupid = 3199
    class map:
       Off = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3
       Lvl4 = 4
       Lvl5 = 5
       Lvl6 = 6
       Lvl7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LvlOfClimaCmft, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LvlSeldForSpdLimAdpvPen(BaseEnumSender):
    de_name     = "LvlSeldForSpdLimAdpv.Pen"
    fdx_name    = "LvlSeldForSpdLimAdpvPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LvlSeldForSpdLimAdpvPen, self).__init__(signal_interface, item)


class LvlSeldForSpdLimAdpvSts(BaseEnumSender):
    de_name     = "LvlSeldForSpdLimAdpv.Sts"
    fdx_name    = "LvlSeldForSpdLimAdpvSts"
    fdx_groupid = 3212
    class map:
       Aut = 0
       HalfAut = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(LvlSeldForSpdLimAdpvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Ukwn		Unknown		Unknown
# 1			1			Src1		Source 1		NAVTEQ
# 2			2			Src2		Source 2		TeleAtlas
# 3			3			Src3		Source 3		Zenrin
# 4			4			Resd1		Reserved 1	reserved
# 5			5			Resd2		Reserved 2	reserved
# 6			6			Resd3		Reserved 3	reserved
# 7			7			Ukwn2		Unknown 2	Unknown 2
class MapSrcInHznData(BaseEnumSender):
    de_name     = "HznData.MapSrc"
    fdx_name    = "MapSrcInHznData"
    fdx_groupid = 3214
    class map:
       Ukwn = 0
       Src1 = 1
       Src2 = 2
       Src3 = 3
       Resd1 = 4
       Resd2 = 5
       Resd3 = 6
       Ukwn2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MapSrcInHznData, self).__init__(signal_interface, item)


class MassgFctActvDrvrMassgFctActv(BaseEnumSender):
    de_name     = "MassgFctActv.DrvrMassgFctActv"
    fdx_name    = "MassgFctActvDrvrMassgFctActv"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MassgFctActvDrvrMassgFctActv, self).__init__(signal_interface, item)


class MassgFctActvPassMassgFctActv(BaseEnumSender):
    de_name     = "MassgFctActv.PassMassgFctActv"
    fdx_name    = "MassgFctActvPassMassgFctActv"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MassgFctActvPassMassgFctActv, self).__init__(signal_interface, item)


class MemBtnSound(BaseBoolSender):
    de_name     = "MemBtnSound"
    fdx_name    = "MemBtnSound"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MemBtnSound, self).__init__(signal_interface, item)


# Unit: Mins,  Range:0->59
class Mins(BaseIntegerSender):
    de_name     = "TiAndDateIndcn.Mins1"
    fdx_name    = "Mins"
    fdx_groupid = 3197
    min = 0
    max = 59


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Mins, self).__init__(signal_interface, item)


# Unit: Mins,  Range:0->59
class Mins1(BaseIntegerSender):
    de_name     = "SetTiAndDate.Minute"
    fdx_name    = "Mins1"
    fdx_groupid = 3214
    min = 0
    max = 59


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Mins1, self).__init__(signal_interface, item)


class MirrDwnStsAtDrvr(BaseEnumSender):
    de_name     = "MirrDwnStsAtDrvr"
    fdx_name    = "MirrDwnStsAtDrvr"
    fdx_groupid = 3198
    class map:
       MirrTiltUndefd = 0
       MirrUpPosn = 1
       MirrTiltPosn = 2
       MirrMovgToUpPosn = 3
       MirrMovgToTiltPosn = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MirrDwnStsAtDrvr, self).__init__(signal_interface, item)


class MirrDwnStsAtPass(BaseEnumSender):
    de_name     = "MirrDwnStsAtPass"
    fdx_name    = "MirrDwnStsAtPass"
    fdx_groupid = 3200
    class map:
       MirrTiltUndefd = 0
       MirrUpPosn = 1
       MirrTiltPosn = 2
       MirrMovgToUpPosn = 3
       MirrMovgToTiltPosn = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MirrDwnStsAtPass, self).__init__(signal_interface, item)


class MirrFoldStsAtDrvr(BaseEnumSender):
    de_name     = "MirrFoldStsAtDrvr"
    fdx_name    = "MirrFoldStsAtDrvr"
    fdx_groupid = 3192
    class map:
       MirrFoldPosnUndefd = 0
       MirrNotFoldPosn = 1
       MirrFoldPosn = 2
       MirrMovgToNotFold = 3
       MirrMovgToFold = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MirrFoldStsAtDrvr, self).__init__(signal_interface, item)


class MirrFoldStsAtPass(BaseEnumSender):
    de_name     = "MirrFoldStsAtPass"
    fdx_name    = "MirrFoldStsAtPass"
    fdx_groupid = 3193
    class map:
       MirrFoldPosnUndefd = 0
       MirrNotFoldPosn = 1
       MirrFoldPosn = 2
       MirrMovgToNotFold = 3
       MirrMovgToFold = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MirrFoldStsAtPass, self).__init__(signal_interface, item)


class MmedHmiModStd(BaseEnumSender):
    de_name     = "MmedHmiModStd"
    fdx_name    = "MmedHmiModStd"
    fdx_groupid = 3216
    class map:
       InfModeOff = 0
       InfModeOn = 1
       InfModePartial = 2
       InfModeWelcome = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MmedHmiModStd, self).__init__(signal_interface, item)


class MmedMaiPwrMod(BaseEnumSender):
    de_name     = "MmedHdPwrMod"
    fdx_name    = "MmedMaiPwrMod"
    fdx_groupid = 3213
    class map:
       IHUStateSleep = 0
       IHUStateStandby = 1
       IHUStatePartial = 2
       IHUStateOn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MmedMaiPwrMod, self).__init__(signal_interface, item)


class MmedTvmPwerMod(BaseEnumSender):
    de_name     = "MmedTvmPwerMod"
    fdx_name    = "MmedTvmPwerMod"
    fdx_groupid = 3221
    class map:
       Sleep = 0
       Switch = 1
       Standby = 2
       TPEG = 3
       On = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MmedTvmPwerMod, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved
class MsgTypInHznData(BaseEnumSender):
    de_name     = "HznData.MsgTyp"
    fdx_name    = "MsgTypInHznData"
    fdx_groupid = 3214
    class map:
       Resd1 = 0
       Posn = 1
       Seg = 2
       Edge = 3
       ProfSho = 4
       ProfLong = 5
       Data = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MsgTypInHznData, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved
class MsgTypInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.MsgTyp"
    fdx_name    = "MsgTypInHznEdge"
    fdx_groupid = 3215
    class map:
       Resd1 = 0
       Posn = 1
       Seg = 2
       Edge = 3
       ProfSho = 4
       ProfLong = 5
       Data = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MsgTypInHznEdge, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved
class MsgTypInHznPosn(BaseEnumSender):
    de_name     = "HznPosn.MsgTyp"
    fdx_name    = "MsgTypInHznPosn"
    fdx_groupid = 3212
    class map:
       Resd1 = 0
       Posn = 1
       Seg = 2
       Edge = 3
       ProfSho = 4
       ProfLong = 5
       Data = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MsgTypInHznPosn, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved
class MsgTypInHznProfLong(BaseEnumSender):
    de_name     = "HznProfLong.MsgTyp"
    fdx_name    = "MsgTypInHznProfLong"
    fdx_groupid = 3213
    class map:
       Resd1 = 0
       Posn = 1
       Seg = 2
       Edge = 3
       ProfSho = 4
       ProfLong = 5
       Data = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MsgTypInHznProfLong, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved
class MsgTypInHznProfSho(BaseEnumSender):
    de_name     = "HznProfSho.MsgTyp"
    fdx_name    = "MsgTypInHznProfSho"
    fdx_groupid = 3213
    class map:
       Resd1 = 0
       Posn = 1
       Seg = 2
       Edge = 3
       ProfSho = 4
       ProfLong = 5
       Data = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MsgTypInHznProfSho, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			Resd1		Reserved 1	Reserved
# 1			1			Posn		Position		POSITION
# 2			2			Seg		Segment		SEGMENT
# 3			3			Edge		Edge		STUB
# 4			4			ProfSho		Profile Short	PROFILE SHORT
# 5			5			ProfLong		Profile Long	PROFILE LONG
# 6			6			Data		Data		META-DATA
# 7			7			Resd2		Reserved 2	Reserved
class MsgTypInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.MsgTyp"
    fdx_name    = "MsgTypInHznSeg"
    fdx_groupid = 3215
    class map:
       Resd1 = 0
       Posn = 1
       Seg = 2
       Edge = 3
       ProfSho = 4
       ProfLong = 5
       Data = 6
       Resd2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MsgTypInHznSeg, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->65535
class MstCfgIDBackboneFR(BaseIntegerSender):
    de_name     = "MstCfgIDBackboneFR"
    fdx_name    = "MstCfgIDBackboneFR"
    fdx_groupid = 3228
    min = 0
    max = 65535


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(MstCfgIDBackboneFR, self).__init__(signal_interface, item)


# Unit: Month,  Range:1->12
class Mth(BaseIntegerSender):
    de_name     = "TiAndDateIndcn.Mth1"
    fdx_name    = "Mth"
    fdx_groupid = 3197
    min = 1
    max = 12


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Mth, self).__init__(signal_interface, item)


# Unit: Month,  Range:1->12
class Mth1(BaseIntegerSender):
    de_name     = "SetTiAndDate.Month"
    fdx_name    = "Mth1"
    fdx_groupid = 3214
    min = 1
    max = 12


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Mth1, self).__init__(signal_interface, item)


class NetCtrlrActvtPrio(BaseEnumSender):
    de_name     = "NetCtrlrActvt.Prio"
    fdx_name    = "NetCtrlrActvtPrio"
    fdx_groupid = 3228
    class map:
       PrioNormal = 0
       PrioHigh = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NetCtrlrActvtPrio, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class NetCtrlrActvtResourceGroup(BaseIntegerSender):
    de_name     = "NetCtrlrActvt.ResourceGroup"
    fdx_name    = "NetCtrlrActvtResourceGroup"
    fdx_groupid = 3228
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NetCtrlrActvtResourceGroup, self).__init__(signal_interface, item)


class NetHdActvtPrio(BaseEnumSender):
    de_name     = "NetHdActvt.Prio"
    fdx_name    = "NetHdActvtPrio"
    fdx_groupid = 3216
    class map:
       PrioNormal = 0
       PrioHigh = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NetHdActvtPrio, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class NetHdActvtResourceGroup(BaseIntegerSender):
    de_name     = "NetHdActvt.ResourceGroup"
    fdx_name    = "NetHdActvtResourceGroup"
    fdx_groupid = 3216
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NetHdActvtResourceGroup, self).__init__(signal_interface, item)


class NetTelmActvtPrio(BaseEnumSender):
    de_name     = "NetTelmActvt.Prio"
    fdx_name    = "NetTelmActvtPrio"
    fdx_groupid = 3194
    class map:
       PrioNormal = 0
       PrioHigh = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NetTelmActvtPrio, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class NetTelmActvtResourceGroup(BaseIntegerSender):
    de_name     = "NetTelmActvt.ResourceGroup"
    fdx_name    = "NetTelmActvtResourceGroup"
    fdx_groupid = 3194
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NetTelmActvtResourceGroup, self).__init__(signal_interface, item)


class NewTripCdn(BaseEnumSender):
    de_name     = "NewTripCdn"
    fdx_name    = "NewTripCdn"
    fdx_groupid = 3207
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NewTripCdn, self).__init__(signal_interface, item)


class NodCtrlInHznProfLong(BaseEnumSender):
    de_name     = "HznProfLong.NodCtrl"
    fdx_name    = "NodCtrlInHznProfLong"
    fdx_groupid = 3213
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NodCtrlInHznProfLong, self).__init__(signal_interface, item)


class NodCtrlInHznProfSho(BaseEnumSender):
    de_name     = "HznProfSho.NodCtrl"
    fdx_name    = "NodCtrlInHznProfSho"
    fdx_groupid = 3213
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NodCtrlInHznProfSho, self).__init__(signal_interface, item)


class NoEntryWarnReq(BaseEnumSender):
    de_name     = "NoEntryWarnReq"
    fdx_name    = "NoEntryWarnReq"
    fdx_groupid = 3187
    class map:
       NoWarn = 0
       Warn = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NoEntryWarnReq, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo1InPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo1"
    fdx_name    = "NoOfSatltForSysNo1InPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NoOfSatltForSysNo1InPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo2InPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo2"
    fdx_name    = "NoOfSatltForSysNo2InPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NoOfSatltForSysNo2InPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo3InPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo3"
    fdx_name    = "NoOfSatltForSysNo3InPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NoOfSatltForSysNo3InPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo4InPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo4"
    fdx_name    = "NoOfSatltForSysNo4InPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NoOfSatltForSysNo4InPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo5InPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo5"
    fdx_name    = "NoOfSatltForSysNo5InPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NoOfSatltForSysNo5InPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo6InPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo6"
    fdx_name    = "NoOfSatltForSysNo6InPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NoOfSatltForSysNo6InPosnFromSatlt, self).__init__(signal_interface, item)


class NoSoundSys(BaseBoolSender):
    de_name     = "NoSoundSys"
    fdx_name    = "NoSoundSys"
    fdx_groupid = 3217


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NoSoundSys, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->65535
class NotifChkDistbn(BaseIntegerSender):
    de_name     = "NotifChkDistbn"
    fdx_name    = "NotifChkDistbn"
    fdx_groupid = 3207
    min = 0
    max = 65535


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NotifChkDistbn, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->7
class NrOfBltAppld(BaseIntegerSender):
    de_name     = "NrOfBltAppld"
    fdx_name    = "NrOfBltAppld"
    fdx_groupid = 3227
    min = 0
    max = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NrOfBltAppld, self).__init__(signal_interface, item)


class NrOfKeyAvl(BaseBoolSender):
    de_name     = "NrOfKeyAvl"
    fdx_name    = "NrOfKeyAvl"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NrOfKeyAvl, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			WrgDirInRoadSngDir	Wrong Direction In Road Single Direction	One-direction road driven in wrong direction
# 1			1			Lane1			Lane 1					1 lane
# 2			2			Lane2			Lane 2					2 lanes
# 3			3			Lane3			Lane 3					3 lanes
# 4			4			Lane4			Lane 4					4 lanes
# 5			5			Lane5			Lane 5					5 lanes
# 6			6			LaneEquOrExcd6		Lane Equal Or Exceed 6			6 or more lanes
class NrOfLaneInDrvgDirInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.NrOfLaneInDrvgDir"
    fdx_name    = "NrOfLaneInDrvgDirInHznEdge"
    fdx_groupid = 3215
    class map:
       WrgDirInRoadSngDir = 0
       Lane1 = 1
       Lane2 = 2
       Lane3 = 3
       Lane4 = 4
       Lane5 = 5
       LaneEquOrExcd6 = 6
       Ukwn = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NrOfLaneInDrvgDirInHznEdge, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			WrgDirInRoadSngDir	Wrong Direction In Road Single Direction	One-direction road driven in wrong direction
# 1			1			Lane1			Lane 1					1 lane
# 2			2			Lane2			Lane 2					2 lanes
# 3			3			Lane3			Lane 3					3 lanes
# 4			4			Lane4			Lane 4					4 lanes
# 5			5			Lane5			Lane 5					5 lanes
# 6			6			LaneEquOrExcd6		Lane Equal Or Exceed 6			6 or more lanes
class NrOfLaneInDrvgDirInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.NrOfLaneInDrvgDir"
    fdx_name    = "NrOfLaneInDrvgDirInHznSeg"
    fdx_groupid = 3215
    class map:
       WrgDirInRoadSngDir = 0
       Lane1 = 1
       Lane2 = 2
       Lane3 = 3
       Lane4 = 4
       Lane5 = 5
       LaneEquOrExcd6 = 6
       Ukwn = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NrOfLaneInDrvgDirInHznSeg, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			RoadSngDir	Road Single Direction	One-direction road
# 1			1			LaneSng		Lane Single		One lane
# 2			2			LaneEquOrExcd2	Lane Equal Or Exceed 2	2 or more lanes
class NrOfLaneInSecDirInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.NrOfLaneInSecDir"
    fdx_name    = "NrOfLaneInSecDirInHznEdge"
    fdx_groupid = 3215
    class map:
       RoadSngDir = 0
       LaneSng = 1
       LaneEquOrExcd2 = 2
       Ukwn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NrOfLaneInSecDirInHznEdge, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			RoadSngDir	Road Single Direction	One-direction road
# 1			1			LaneSng		Lane Single		One lane
# 2			2			LaneEquOrExcd2	Lane Equal Or Exceed 2	2 or more lanes
class NrOfLaneInSecDirInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.NrOfLaneInSecDir"
    fdx_name    = "NrOfLaneInSecDirInHznSeg"
    fdx_groupid = 3215
    class map:
       RoadSngDir = 0
       LaneSng = 1
       LaneEquOrExcd2 = 2
       Ukwn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(NrOfLaneInSecDirInHznSeg, self).__init__(signal_interface, item)


class OffsForDrvrSpprtFctActvSts(BaseEnumSender):
    de_name     = "OffsForDrvrSpprtFctActvSts"
    fdx_name    = "OffsForDrvrSpprtFctActvSts"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(OffsForDrvrSpprtFctActvSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class OffsForSpdWarnSetgPen(BaseEnumSender):
    de_name     = "OffsForSpdWarnSetg.Pen"
    fdx_name    = "OffsForSpdWarnSetgPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(OffsForSpdWarnSetgPen, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class OffsForSpdWarnSetgSts(BaseFloatSender):
    de_name     = "OffsForSpdWarnSetg.Sts"
    fdx_name    = "OffsForSpdWarnSetgSts"
    fdx_groupid = 3212
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(OffsForSpdWarnSetgSts, self).__init__(signal_interface, item)


# Unit: m,  Range:0->8191
class OffsInHznEdge(BaseIntegerSender):
    de_name     = "HznEdge.Offs"
    fdx_name    = "OffsInHznEdge"
    fdx_groupid = 3215
    min = 0
    max = 8191


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(OffsInHznEdge, self).__init__(signal_interface, item)


# Unit: m,  Range:0->8191
class OffsInHznPosn(BaseIntegerSender):
    de_name     = "HznPosn.Offs"
    fdx_name    = "OffsInHznPosn"
    fdx_groupid = 3212
    min = 0
    max = 8191


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(OffsInHznPosn, self).__init__(signal_interface, item)


# Unit: m,  Range:0->8191
class OffsInHznProfLong(BaseIntegerSender):
    de_name     = "HznProfLong.Offs"
    fdx_name    = "OffsInHznProfLong"
    fdx_groupid = 3213
    min = 0
    max = 8191


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(OffsInHznProfLong, self).__init__(signal_interface, item)


# Unit: m,  Range:0->8191
class OffsInHznProfSho(BaseIntegerSender):
    de_name     = "HznProfSho.Offs"
    fdx_name    = "OffsInHznProfSho"
    fdx_groupid = 3213
    min = 0
    max = 8191


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(OffsInHznProfSho, self).__init__(signal_interface, item)


# Unit: m,  Range:0->8191
class OffsInHznSeg(BaseIntegerSender):
    de_name     = "HznSeg.Offs"
    fdx_name    = "OffsInHznSeg"
    fdx_groupid = 3215
    min = 0
    max = 8191


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(OffsInHznSeg, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class PahIdxInHznEdge(BaseIntegerSender):
    de_name     = "HznEdge.PahIdx"
    fdx_name    = "PahIdxInHznEdge"
    fdx_groupid = 3215
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PahIdxInHznEdge, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class PahIdxInHznPosn(BaseIntegerSender):
    de_name     = "HznPosn.PahIdx"
    fdx_name    = "PahIdxInHznPosn"
    fdx_groupid = 3212
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PahIdxInHznPosn, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class PahIdxInHznProfLong(BaseIntegerSender):
    de_name     = "HznProfLong.PahIdx"
    fdx_name    = "PahIdxInHznProfLong"
    fdx_groupid = 3213
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PahIdxInHznProfLong, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class PahIdxInHznProfSho(BaseIntegerSender):
    de_name     = "HznProfSho.PahIdx"
    fdx_name    = "PahIdxInHznProfSho"
    fdx_groupid = 3213
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PahIdxInHznProfSho, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class PahIdxInHznSeg(BaseIntegerSender):
    de_name     = "HznSeg.PahIdx"
    fdx_name    = "PahIdxInHznSeg"
    fdx_groupid = 3215
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PahIdxInHznSeg, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class PahIdxNewInHznEdge(BaseIntegerSender):
    de_name     = "HznEdge.PahIdxNew"
    fdx_name    = "PahIdxNewInHznEdge"
    fdx_groupid = 3215
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PahIdxNewInHznEdge, self).__init__(signal_interface, item)


class ParkAssiDstFrntOfAudSideWarn(BaseEnumSender):
    de_name     = "SnsrParkAssiFrnt.AudSideWarn"
    fdx_name    = "ParkAssiDstFrntOfAudSideWarn"
    fdx_groupid = 3223
    class map:
       None_ = 0
       Left = 1
       Right = 2
       LeftAndRight = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstFrntOfAudSideWarn, self).__init__(signal_interface, item)


class ParkAssiDstFrntOfAudWarnDir(BaseEnumSender):
    de_name     = "SnsrParkAssiFrnt.AudWarnDir"
    fdx_name    = "ParkAssiDstFrntOfAudWarnDir"
    fdx_groupid = 3223
    class map:
       NoWarn = 0
       LeftSide = 1
       OuterLeft = 2
       InnerLeft = 3
       InnerRight = 4
       OuterRight = 5
       RightSide = 6


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstFrntOfAudWarnDir, self).__init__(signal_interface, item)


class ParkAssiDstFrntOfDstOfSnsrInsdRi(BaseEnumSender):
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrInsdRi"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrInsdRi"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstFrntOfDstOfSnsrInsdRi, self).__init__(signal_interface, item)


class ParkAssiDstFrntOfDstOfSnsrLeSide(BaseEnumSender):
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrLeSide"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrLeSide"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstFrntOfDstOfSnsrLeSide, self).__init__(signal_interface, item)


class ParkAssiDstFrntOfDstOfSnsrOutdRi(BaseEnumSender):
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrOutdRi"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrOutdRi"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstFrntOfDstOfSnsrOutdRi, self).__init__(signal_interface, item)


class ParkAssiDstFrntOfDstOfSnsrRiSide(BaseEnumSender):
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrRiSide"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrRiSide"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstFrntOfDstOfSnsrRiSide, self).__init__(signal_interface, item)


class ParkAssiDstFrntOfSnsrInsdLe(BaseEnumSender):
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrInsdLe"
    fdx_name    = "ParkAssiDstFrntOfSnsrInsdLe"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstFrntOfSnsrInsdLe, self).__init__(signal_interface, item)


class ParkAssiDstFrntOfSnsrOutdLe(BaseEnumSender):
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrOutdLe"
    fdx_name    = "ParkAssiDstFrntOfSnsrOutdLe"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstFrntOfSnsrOutdLe, self).__init__(signal_interface, item)


class ParkAssiDstReOfAudSideWarn(BaseEnumSender):
    de_name     = "SnsrParkAssiRe.AudSideWarn"
    fdx_name    = "ParkAssiDstReOfAudSideWarn"
    fdx_groupid = 3223
    class map:
       None_ = 0
       Left = 1
       Right = 2
       LeftAndRight = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstReOfAudSideWarn, self).__init__(signal_interface, item)


class ParkAssiDstReOfAudWarnDir(BaseEnumSender):
    de_name     = "SnsrParkAssiRe.AudWarnDir"
    fdx_name    = "ParkAssiDstReOfAudWarnDir"
    fdx_groupid = 3223
    class map:
       NoWarn = 0
       LeftSide = 1
       OuterLeft = 2
       InnerLeft = 3
       InnerRight = 4
       OuterRight = 5
       RightSide = 6


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstReOfAudWarnDir, self).__init__(signal_interface, item)


class ParkAssiDstReOfDstOfSnsrInsdLe(BaseEnumSender):
    de_name     = "SnsrParkAssiRe.DstOfSnsrInsdLe"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrInsdLe"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstReOfDstOfSnsrInsdLe, self).__init__(signal_interface, item)


class ParkAssiDstReOfDstOfSnsrInsdRi(BaseEnumSender):
    de_name     = "SnsrParkAssiRe.DstOfSnsrInsdRi"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrInsdRi"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstReOfDstOfSnsrInsdRi, self).__init__(signal_interface, item)


class ParkAssiDstReOfDstOfSnsrLeSide(BaseEnumSender):
    de_name     = "SnsrParkAssiRe.DstOfSnsrLeSide"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrLeSide"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstReOfDstOfSnsrLeSide, self).__init__(signal_interface, item)


class ParkAssiDstReOfDstOfSnsrOutdLe(BaseEnumSender):
    de_name     = "SnsrParkAssiRe.DstOfSnsrOutdLe"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrOutdLe"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstReOfDstOfSnsrOutdLe, self).__init__(signal_interface, item)


class ParkAssiDstReOfDstOfSnsrOutdRi(BaseEnumSender):
    de_name     = "SnsrParkAssiRe.DstOfSnsrOutdRi"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrOutdRi"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstReOfDstOfSnsrOutdRi, self).__init__(signal_interface, item)


class ParkAssiDstReOfDstOfSnsrRiSide(BaseEnumSender):
    de_name     = "SnsrParkAssiRe.DstOfSnsrRiSide"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrRiSide"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ParkAssiDstReOfDstOfSnsrRiSide, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			SegNotPartOfPahCalcd	Segment Not Part Of Path Calculated	Segment is not part of Calculated Route
# 1			1			SegPartOfPahCalcd	Segment 	Part Of Path Calculated		Segment is part of Calculated Route
# 2			2			Ukwn			Unknown				Unknown
class PartOfCourseCalcdInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.PartOfCourseCalcd"
    fdx_name    = "PartOfCourseCalcdInHznEdge"
    fdx_groupid = 3215
    class map:
       SegNotPartOfPahCalcd = 0
       SegPartOfPahCalcd = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PartOfCourseCalcdInHznEdge, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			SegNotPartOfPahCalcd	Segment Not Part Of Path Calculated	Segment is not part of Calculated Route
# 1			1			SegPartOfPahCalcd	Segment 	Part Of Path Calculated		Segment is part of Calculated Route
# 2			2			Ukwn			Unknown				Unknown
class PartOfCourseCalcdInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.PartOfCourseCalcd"
    fdx_name    = "PartOfCourseCalcdInHznSeg"
    fdx_groupid = 3215
    class map:
       SegNotPartOfPahCalcd = 0
       SegPartOfPahCalcd = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PartOfCourseCalcdInHznSeg, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class PartOfYrVersOfMapInHznData(BaseIntegerSender):
    de_name     = "HznData.PartOfYrVersOfMap"
    fdx_name    = "PartOfYrVersOfMapInHznData"
    fdx_groupid = 3214
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PartOfYrVersOfMapInHznData, self).__init__(signal_interface, item)


class PasAlrmDeactvnReq(BaseEnumSender):
    de_name     = "PasAlrmDeactvnReq"
    fdx_name    = "PasAlrmDeactvnReq"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PasAlrmDeactvnReq, self).__init__(signal_interface, item)


class PasAlrmSts(BaseEnumSender):
    de_name     = "PasAlrmSts"
    fdx_name    = "PasAlrmSts"
    fdx_groupid = 3199
    class map:
       NoReq = 0
       OffReq = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PasAlrmSts, self).__init__(signal_interface, item)


class PassMassgRunng(BaseEnumSender):
    de_name     = "PassMassgRunng"
    fdx_name    = "PassMassgRunng"
    fdx_groupid = 3198
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassMassgRunng, self).__init__(signal_interface, item)


class PassSeatActvSpplFct(BaseEnumSender):
    de_name     = "PassSeatActvSpplFct"
    fdx_name    = "PassSeatActvSpplFct"
    fdx_groupid = 3198
    class map:
       NotAvl = 0
       LumExtnAndLumHei = 1
       BackBlster = 2
       CushExtn = 3
       HdrestHeiAndHdrestTilt = 4
       MassgFct = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatActvSpplFct, self).__init__(signal_interface, item)


class PassSeatDispMassgFctMassgInten(BaseEnumSender):
    de_name     = "PassSeatDispMassgFct.MassgInten"
    fdx_name    = "PassSeatDispMassgFctMassgInten"
    fdx_groupid = 3211
    class map:
       IntenLo = 0
       IntenNorm = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatDispMassgFctMassgInten, self).__init__(signal_interface, item)


class PassSeatDispMassgFctMassgProg(BaseEnumSender):
    de_name     = "PassSeatDispMassgFct.MassgProg"
    fdx_name    = "PassSeatDispMassgFctMassgProg"
    fdx_groupid = 3211
    class map:
       Prog1 = 0
       Prog2 = 1
       Prog3 = 2
       Prog4 = 3
       Prog5 = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatDispMassgFctMassgProg, self).__init__(signal_interface, item)


class PassSeatDispMassgFctMassgSpdLvl(BaseEnumSender):
    de_name     = "PassSeatDispMassgFct.MassgSpdLvl"
    fdx_name    = "PassSeatDispMassgFctMassgSpdLvl"
    fdx_groupid = 3211
    class map:
       IntenLo = 0
       IntenNorm = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatDispMassgFctMassgSpdLvl, self).__init__(signal_interface, item)


class PassSeatDispMassgFctOnOff(BaseEnumSender):
    de_name     = "PassSeatDispMassgFct.OnOff"
    fdx_name    = "PassSeatDispMassgFctOnOff"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatDispMassgFctOnOff, self).__init__(signal_interface, item)


class PassSeatDispSpplFct(BaseEnumSender):
    de_name     = "PassSeatDispSpplFct"
    fdx_name    = "PassSeatDispSpplFct"
    fdx_groupid = 3216
    class map:
       NotAvl = 0
       LumExtnAndLumHei = 1
       BackBlster = 2
       CushExtn = 3
       HdrestHeiAndHdrestTilt = 4
       MassgFct = 5


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatDispSpplFct, self).__init__(signal_interface, item)


class PassSeatMassgFctMassgInten(BaseEnumSender):
    de_name     = "PassSeatMassgFct.MassgInten"
    fdx_name    = "PassSeatMassgFctMassgInten"
    fdx_groupid = 3197
    class map:
       IntenLo = 0
       IntenNorm = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatMassgFctMassgInten, self).__init__(signal_interface, item)


class PassSeatMassgFctMassgProg(BaseEnumSender):
    de_name     = "PassSeatMassgFct.MassgProg"
    fdx_name    = "PassSeatMassgFctMassgProg"
    fdx_groupid = 3197
    class map:
       Prog1 = 0
       Prog2 = 1
       Prog3 = 2
       Prog4 = 3
       Prog5 = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatMassgFctMassgProg, self).__init__(signal_interface, item)


class PassSeatMassgFctMassgSpdLvl(BaseEnumSender):
    de_name     = "PassSeatMassgFct.MassgSpdLvl"
    fdx_name    = "PassSeatMassgFctMassgSpdLvl"
    fdx_groupid = 3197
    class map:
       IntenLo = 0
       IntenNorm = 1
       IntenHi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatMassgFctMassgSpdLvl, self).__init__(signal_interface, item)


class PassSeatMassgFctOnOff(BaseEnumSender):
    de_name     = "PassSeatMassgFct.OnOff"
    fdx_name    = "PassSeatMassgFctOnOff"
    fdx_groupid = 3197
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatMassgFctOnOff, self).__init__(signal_interface, item)


class PassSeatSts(BaseEnumSender):
    de_name     = "PassSeatSts"
    fdx_name    = "PassSeatSts"
    fdx_groupid = 3227
    class map:
       Empty = 0
       Fmale = 1
       OccptLrg = 2
       Ukwn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatSts, self).__init__(signal_interface, item)


class PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts(BaseEnumSender):
    de_name     = "PassSeatSwtSts2.PassSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Fwd = 1
       Backw = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts, self).__init__(signal_interface, item)


class PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts(BaseEnumSender):
    de_name     = "PassSeatSwtSts2.PassSeatSwtAdjmtOfSpplFctVerSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts, self).__init__(signal_interface, item)


class PassSeatSwtSts2PassSeatSwtHeiFrntSts(BaseEnumSender):
    de_name     = "PassSeatSwtSts2.PassSeatSwtHeiFrntSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtHeiFrntSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatSwtSts2PassSeatSwtHeiFrntSts, self).__init__(signal_interface, item)


class PassSeatSwtSts2PassSeatSwtHeiSts(BaseEnumSender):
    de_name     = "PassSeatSwtSts2.PassSeatSwtHeiSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtHeiSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatSwtSts2PassSeatSwtHeiSts, self).__init__(signal_interface, item)


class PassSeatSwtSts2PassSeatSwtInclSts(BaseEnumSender):
    de_name     = "PassSeatSwtSts2.PassSeatSwtInclSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtInclSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Fwd = 1
       Backw = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatSwtSts2PassSeatSwtInclSts, self).__init__(signal_interface, item)


class PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts(BaseEnumSender):
    de_name     = "PassSeatSwtSts2.PassSeatSwtSelnOfSpplFctStsSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts, self).__init__(signal_interface, item)


class PassSeatSwtSts2PassSeatSwtSldSts(BaseEnumSender):
    de_name     = "PassSeatSwtSts2.PassSeatSwtSldSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtSldSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       Fwd = 1
       Backw = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PassSeatSwtSts2PassSeatSwtSldSts, self).__init__(signal_interface, item)


# Unit: rad,  Range:-14.5->14.5, Resolution: (0.0009765625*x+0.0, raw is signed, 15 bits )
class PinionSteerAg1(BaseFloatSender):
    de_name     = "PinionSteerAg1.PinionSteerAg1"
    fdx_name    = "PinionSteerAg1"
    fdx_groupid = 3231
    min    = -14.5
    max    = 14.5
    scale  = 0.0009765625
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PinionSteerAg1, self).__init__(signal_interface, item)


class PinionSteerAg1Qf(BaseEnumSender):
    de_name     = "PinionSteerAg1.PinionSteerAg1Qf"
    fdx_name    = "PinionSteerAg1Qf"
    fdx_groupid = 3231
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PinionSteerAg1Qf, self).__init__(signal_interface, item)


# Unit: m,  Range:-100->6000, Resolution: (0.1*x+-100.0, raw is unsigned, 16 bits )
class PosnAltiInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PosnAlti"
    fdx_name    = "PosnAltiInPosnFromSatlt"
    fdx_groupid = 3229
    min    = -100
    max    = 6000
    scale  = 0.1
    offset = -100.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnAltiInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Deg,  Range:0->359.99, Resolution: (0.01*x+0.0, raw is unsigned, 16 bits )
class PosnDirInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PosnDir"
    fdx_name    = "PosnDirInPosnFromSatlt"
    fdx_groupid = 3229
    min    = 0
    max    = 359.99
    scale  = 0.01
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnDirInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->3
class PosnIdxInHznPosn(BaseIntegerSender):
    de_name     = "HznPosn.PosnIdx"
    fdx_name    = "PosnIdxInHznPosn"
    fdx_groupid = 3212
    min = 0
    max = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnIdxInHznPosn, self).__init__(signal_interface, item)


# Unit: Deg,  Range:-90->90, Resolution: (2.7777777777777776e-07*x+0.0, raw is signed, 30 bits )
class PosnLatInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PosnLat"
    fdx_name    = "PosnLatInPosnFromSatlt"
    fdx_groupid = 3229
    min    = -90
    max    = 90
    scale  = 2.7777777777777776e-07
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnLatInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Deg,  Range:-180->180, Resolution: (2.7777777777777776e-07*x+0.0, raw is signed, 31 bits )
class PosnLgtInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PosnLgt"
    fdx_name    = "PosnLgtInPosnFromSatlt"
    fdx_groupid = 3229
    min    = -180
    max    = 180
    scale  = 2.7777777777777776e-07
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnLgtInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: %,  Range:0->103.33333323, Resolution: (3.33333333*x+0.0, raw is unsigned, 5 bits )
class PosnProbltyInHznPosn(BaseFloatSender):
    de_name     = "HznPosn.PosnProblty"
    fdx_name    = "PosnProbltyInHznPosn"
    fdx_groupid = 3212
    min    = 0
    max    = 103.33333323
    scale  = 3.33333333
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnProbltyInHznPosn, self).__init__(signal_interface, item)


class PosnQlyInDstToManvLocn(BaseEnumSender):
    de_name     = "DstToManvLocn.PosnQly"
    fdx_name    = "PosnQlyInDstToManvLocn"
    fdx_groupid = 3217
    class map:
       QlyOfLvl0 = 0
       QlyOfLvl1 = 1
       QlyOfLvl2 = 2
       QlyOfLvl3 = 3
       QlyOfLvl4 = 4
       QlyOfLvl5 = 5
       QlyOfLvl6 = 6
       QlyOfLvl7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnQlyInDstToManvLocn, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->7
class PosnQlyInHznPosn(BaseIntegerSender):
    de_name     = "HznPosn.PosnQly"
    fdx_name    = "PosnQlyInHznPosn"
    fdx_groupid = 3212
    min = 0
    max = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnQlyInHznPosn, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 17 bits )
class PosnSpdInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PosnSpd"
    fdx_name    = "PosnSpdInPosnFromSatlt"
    fdx_groupid = 3229
    min    = 0
    max    = 100
    scale  = 0.001
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnSpdInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: ms,  Range:0->2555, Resolution: (5.0*x+0.0, raw is unsigned, 9 bits )
class PosnTiDifInHznPosn(BaseFloatSender):
    de_name     = "HznPosn.PosnTiDif"
    fdx_name    = "PosnTiDifInHznPosn"
    fdx_groupid = 3212
    min    = 0
    max    = 2555
    scale  = 5.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnTiDifInHznPosn, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 17 bits )
class PosnVHozlInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PosnVHozl"
    fdx_name    = "PosnVHozlInPosnFromSatlt"
    fdx_groupid = 3229
    min    = 0
    max    = 100
    scale  = 0.001
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnVHozlInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: m/s,  Range:-100->100, Resolution: (0.001*x+0.0, raw is signed, 18 bits )
class PosnVVertInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PosnVVert"
    fdx_name    = "PosnVVertInPosnFromSatlt"
    fdx_groupid = 3229
    min    = -100
    max    = 100
    scale  = 0.001
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PosnVVertInPosnFromSatlt, self).__init__(signal_interface, item)


class PostDrvgClimaAvl(BaseEnumSender):
    de_name     = "PostDrvgClimaAvl"
    fdx_name    = "PostDrvgClimaAvl"
    fdx_groupid = 3198
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PostDrvgClimaAvl, self).__init__(signal_interface, item)


class PreClngNotif(BaseEnumSender):
    de_name     = "PreClngNotif"
    fdx_name    = "PreClngNotif"
    fdx_groupid = 3199
    class map:
       NoReq = 0
       NoWarn = 1
       Done = 2
       Err = 3
       Intrpt = 4
       Spare1 = 5
       Spare2 = 6
       Spare3 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PreClngNotif, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreHozlDilInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PreHozlDil"
    fdx_name    = "PreHozlDilInPosnFromSatlt"
    fdx_groupid = 3229
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PreHozlDilInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PrePosnDilInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PrePosnDil"
    fdx_name    = "PrePosnDilInPosnFromSatlt"
    fdx_groupid = 3229
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrePosnDilInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreTiDilInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PreTiDil"
    fdx_name    = "PreTiDilInPosnFromSatlt"
    fdx_groupid = 3229
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PreTiDilInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreVertDilInPosnFromSatlt(BaseFloatSender):
    de_name     = "PosnFromSatlt.PreVertDil"
    fdx_name    = "PreVertDilInPosnFromSatlt"
    fdx_groupid = 3229
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PreVertDilInPosnFromSatlt, self).__init__(signal_interface, item)


class PrkgAssiFailr(BaseEnumSender):
    de_name     = "PrkgAssiFailr"
    fdx_name    = "PrkgAssiFailr"
    fdx_groupid = 3223
    class map:
       NoFailr = 0
       SgnlFailr = 1
       SnsrFailr = 2
       DgrSnsr = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrkgAssiFailr, self).__init__(signal_interface, item)


class PrkgAssiManvActvSts(BaseEnumSender):
    de_name     = "PrkgAssiManvActvSts"
    fdx_name    = "PrkgAssiManvActvSts"
    fdx_groupid = 3223
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrkgAssiManvActvSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class PrkgAssiManvProgs(BaseIntegerSender):
    de_name     = "PrkgAssiManvProgs"
    fdx_name    = "PrkgAssiManvProgs"
    fdx_groupid = 3223
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrkgAssiManvProgs, self).__init__(signal_interface, item)


class PrkgAssiSts(BaseEnumSender):
    de_name     = "PrkgAssiSts"
    fdx_name    = "PrkgAssiSts"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrkgAssiSts, self).__init__(signal_interface, item)


class PrkgAutSts(BaseEnumSender):
    de_name     = "PrkgAutSts"
    fdx_name    = "PrkgAutSts"
    fdx_groupid = 3223
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrkgAutSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class PrkgCamSysAvlSts(BaseEnumSender):
    de_name     = "PrkgCamSysAvlSts"
    fdx_name    = "PrkgCamSysAvlSts"
    fdx_groupid = 3184
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrkgCamSysAvlSts, self).__init__(signal_interface, item)


class PrkgOutCfm(BaseEnumSender):
    de_name     = "PrkgOutCfm"
    fdx_name    = "PrkgOutCfm"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrkgOutCfm, self).__init__(signal_interface, item)


class PrkgTypVld(BaseEnumSender):
    de_name     = "PrkgTypVld"
    fdx_name    = "PrkgTypVld"
    fdx_groupid = 3223
    class map:
       NoPrkgDetd = 0
       ParaPrkgDetd = 1
       PerpPrkgDetd = 2
       ParaAndPerpPrkgDetd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrkgTypVld, self).__init__(signal_interface, item)


class ProfActProf1(BaseBoolSender):
    de_name     = "ProfAct.Prof1"
    fdx_name    = "ProfActProf1"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf1, self).__init__(signal_interface, item)


class ProfActProf10(BaseBoolSender):
    de_name     = "ProfAct.Prof10"
    fdx_name    = "ProfActProf10"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf10, self).__init__(signal_interface, item)


class ProfActProf11(BaseBoolSender):
    de_name     = "ProfAct.Prof11"
    fdx_name    = "ProfActProf11"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf11, self).__init__(signal_interface, item)


class ProfActProf12(BaseBoolSender):
    de_name     = "ProfAct.Prof12"
    fdx_name    = "ProfActProf12"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf12, self).__init__(signal_interface, item)


class ProfActProf2(BaseBoolSender):
    de_name     = "ProfAct.Prof2"
    fdx_name    = "ProfActProf2"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf2, self).__init__(signal_interface, item)


class ProfActProf3(BaseBoolSender):
    de_name     = "ProfAct.Prof3"
    fdx_name    = "ProfActProf3"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf3, self).__init__(signal_interface, item)


class ProfActProf4(BaseBoolSender):
    de_name     = "ProfAct.Prof4"
    fdx_name    = "ProfActProf4"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf4, self).__init__(signal_interface, item)


class ProfActProf5(BaseBoolSender):
    de_name     = "ProfAct.Prof5"
    fdx_name    = "ProfActProf5"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf5, self).__init__(signal_interface, item)


class ProfActProf6(BaseBoolSender):
    de_name     = "ProfAct.Prof6"
    fdx_name    = "ProfActProf6"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf6, self).__init__(signal_interface, item)


class ProfActProf7(BaseBoolSender):
    de_name     = "ProfAct.Prof7"
    fdx_name    = "ProfActProf7"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf7, self).__init__(signal_interface, item)


class ProfActProf8(BaseBoolSender):
    de_name     = "ProfAct.Prof8"
    fdx_name    = "ProfActProf8"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf8, self).__init__(signal_interface, item)


class ProfActProf9(BaseBoolSender):
    de_name     = "ProfAct.Prof9"
    fdx_name    = "ProfActProf9"
    fdx_groupid = 3196


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfActProf9, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ProfChg(BaseEnumSender):
    de_name     = "ProfChg"
    fdx_name    = "ProfChg"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfChg, self).__init__(signal_interface, item)


class ProfLimdProf1(BaseBoolSender):
    de_name     = "ProfLimd.Prof1"
    fdx_name    = "ProfLimdProf1"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf1, self).__init__(signal_interface, item)


class ProfLimdProf10(BaseBoolSender):
    de_name     = "ProfLimd.Prof10"
    fdx_name    = "ProfLimdProf10"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf10, self).__init__(signal_interface, item)


class ProfLimdProf11(BaseBoolSender):
    de_name     = "ProfLimd.Prof11"
    fdx_name    = "ProfLimdProf11"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf11, self).__init__(signal_interface, item)


class ProfLimdProf12(BaseBoolSender):
    de_name     = "ProfLimd.Prof12"
    fdx_name    = "ProfLimdProf12"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf12, self).__init__(signal_interface, item)


class ProfLimdProf2(BaseBoolSender):
    de_name     = "ProfLimd.Prof2"
    fdx_name    = "ProfLimdProf2"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf2, self).__init__(signal_interface, item)


class ProfLimdProf3(BaseBoolSender):
    de_name     = "ProfLimd.Prof3"
    fdx_name    = "ProfLimdProf3"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf3, self).__init__(signal_interface, item)


class ProfLimdProf4(BaseBoolSender):
    de_name     = "ProfLimd.Prof4"
    fdx_name    = "ProfLimdProf4"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf4, self).__init__(signal_interface, item)


class ProfLimdProf5(BaseBoolSender):
    de_name     = "ProfLimd.Prof5"
    fdx_name    = "ProfLimdProf5"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf5, self).__init__(signal_interface, item)


class ProfLimdProf6(BaseBoolSender):
    de_name     = "ProfLimd.Prof6"
    fdx_name    = "ProfLimdProf6"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf6, self).__init__(signal_interface, item)


class ProfLimdProf7(BaseBoolSender):
    de_name     = "ProfLimd.Prof7"
    fdx_name    = "ProfLimdProf7"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf7, self).__init__(signal_interface, item)


class ProfLimdProf8(BaseBoolSender):
    de_name     = "ProfLimd.Prof8"
    fdx_name    = "ProfLimdProf8"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf8, self).__init__(signal_interface, item)


class ProfLimdProf9(BaseBoolSender):
    de_name     = "ProfLimd.Prof9"
    fdx_name    = "ProfLimdProf9"
    fdx_groupid = 3194


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfLimdProf9, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ProfPenSts1(BaseEnumSender):
    de_name     = "ProfPenSts1"
    fdx_name    = "ProfPenSts1"
    fdx_groupid = 3194
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfPenSts1, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
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
class ProfTypInHznProfLong(BaseEnumSender):
    de_name     = "HznProfLong.ProfTyp"
    fdx_name    = "ProfTypInHznProfLong"
    fdx_groupid = 3213
    class map:
       NotInUse = 0
       Lgt = 1
       Lat = 2
       Alti = 3
       NodCtrlForLgt = 4
       NodCtrlForLat = 5
       NodCtrlForAlti = 6
       Id = 7
       SpdLim1 = 16
       SpdLim2 = 17
       SpdLim3 = 18
       CamSpd = 19
       RoadWInfo = 20
       TrfcLi = 21
       Ukwn = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfTypInHznProfLong, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
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
class ProfTypInHznProfSho(BaseEnumSender):
    de_name     = "HznProfSho.ProfTyp"
    fdx_name    = "ProfTypInHznProfSho"
    fdx_groupid = 3213
    class map:
       NotInUse = 0
       Crvt = 1
       PahNrTyp = 2
       Slop1 = 3
       Slop2 = 4
       RoadAcs = 5
       RoadCdn = 6
       PosnOfSpdSgnNotStat = 7
       ChgOfDir = 8
       TrfcSgn = 18
       CrashAr = 20
       Ukwn = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfTypInHznProfSho, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			HiQly		High Quality	means highest accuracy.
# 1			1			NotInUse		Not In Use	Not used
# 2			2			LoQly		Low Quality	mean lowest accuracy.
# 3			3			UkwnQly		Unknown Quality	indicate that accuracy is unknown.
class ProfTypQlyInHznProfSho(BaseEnumSender):
    de_name     = "HznProfSho.ProfTypQly"
    fdx_name    = "ProfTypQlyInHznProfSho"
    fdx_groupid = 3213
    class map:
       HiQly = 0
       NotInUse = 1
       LoQly = 2
       UkwnQly = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ProfTypQlyInHznProfSho, self).__init__(signal_interface, item)


class PrpsnDrvMod(BaseEnumSender):
    de_name     = "PrpsnDrvMod"
    fdx_name    = "PrpsnDrvMod"
    fdx_groupid = 3245
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnDrvMod, self).__init__(signal_interface, item)


# Propulsion HV Battery usage mode type
class PrpsnHvBattUsgModAct(BaseEnumSender):
    de_name     = "PrpsnHvBattUsgModAct"
    fdx_name    = "PrpsnHvBattUsgModAct"
    fdx_groupid = 3239
    class map:
       NoReq = 0
       Norm = 1
       Hld = 2
       HldSmt = 3
       Chrg = 4
       NotOfUse1 = 5
       NotOfUse2 = 6
       NotOfUse3 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgModAct, self).__init__(signal_interface, item)


# Propulsion HV Battery usage mode type
class PrpsnHvBattUsgModReq(BaseEnumSender):
    de_name     = "PrpsnHvBattUsgModReq"
    fdx_name    = "PrpsnHvBattUsgModReq"
    fdx_groupid = 3213
    class map:
       NoReq = 0
       Norm = 1
       Hld = 2
       HldSmt = 3
       Chrg = 4
       NotOfUse1 = 5
       NotOfUse2 = 6
       NotOfUse3 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgModReq, self).__init__(signal_interface, item)


class PrpsnHvBattUsgOfChrgBlkd(BaseBoolSender):
    de_name     = "PrpsnHvBattUsgOfChrgBlkd"
    fdx_name    = "PrpsnHvBattUsgOfChrgBlkd"
    fdx_groupid = 3244


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgOfChrgBlkd, self).__init__(signal_interface, item)


# HV battery usage function blocked types
class PrpsnHvBattUsgOfChrgBlkd2(BaseEnumSender):
    de_name     = "PrpsnHvBattUsgOfChrgBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfChrgBlkd2"
    fdx_groupid = 3244
    class map:
       NotBlkd = 0
       BlkdByBattLvl = 1
       BlkdBySysFlt = 2
       NotOfUse = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgOfChrgBlkd2, self).__init__(signal_interface, item)


class PrpsnHvBattUsgOfHldBlkd(BaseBoolSender):
    de_name     = "PrpsnHvBattUsgOfHldBlkd"
    fdx_name    = "PrpsnHvBattUsgOfHldBlkd"
    fdx_groupid = 3244


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgOfHldBlkd, self).__init__(signal_interface, item)


# HV battery usage function blocked types
class PrpsnHvBattUsgOfHldBlkd2(BaseEnumSender):
    de_name     = "PrpsnHvBattUsgOfHldBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfHldBlkd2"
    fdx_groupid = 3244
    class map:
       NotBlkd = 0
       BlkdByBattLvl = 1
       BlkdBySysFlt = 2
       NotOfUse = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgOfHldBlkd2, self).__init__(signal_interface, item)


class PrpsnHvBattUsgOfHldSmtBlkd(BaseBoolSender):
    de_name     = "PrpsnHvBattUsgOfHldSmtBlkd"
    fdx_name    = "PrpsnHvBattUsgOfHldSmtBlkd"
    fdx_groupid = 3244


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgOfHldSmtBlkd, self).__init__(signal_interface, item)


# HV battery usage function blocked types
class PrpsnHvBattUsgOfHldSmtBlkd2(BaseEnumSender):
    de_name     = "PrpsnHvBattUsgOfHldSmtBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfHldSmtBlkd2"
    fdx_groupid = 3244
    class map:
       NotBlkd = 0
       BlkdByBattLvl = 1
       BlkdBySysFlt = 2
       NotOfUse = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgOfHldSmtBlkd2, self).__init__(signal_interface, item)


# Unit: Km/h,  Range:0->150
class PrpsnHvBattUsgOfHldSpd(BaseIntegerSender):
    de_name     = "PrpsnHvBattUsgOfHldSpd"
    fdx_name    = "PrpsnHvBattUsgOfHldSpd"
    fdx_groupid = 3216
    min = 0
    max = 150


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgOfHldSpd, self).__init__(signal_interface, item)


class PrpsnHvBattUsgStsDispd(BaseBoolSender):
    de_name     = "PrpsnHvBattUsgStsDispd"
    fdx_name    = "PrpsnHvBattUsgStsDispd"
    fdx_groupid = 3216


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnHvBattUsgStsDispd, self).__init__(signal_interface, item)


# Basic type list
# 8 types
class PrpsnModElecDrvBlkd(BaseEnumSender):
    de_name     = "PrpsnModElecDrvBlkd"
    fdx_name    = "PrpsnModElecDrvBlkd"
    fdx_groupid = 3241
    class map:
       Typ0 = 0
       Typ1 = 1
       Typ2 = 2
       Typ3 = 3
       Typ4 = 4
       Typ5 = 5
       Typ6 = 6
       Typ7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnModElecDrvBlkd, self).__init__(signal_interface, item)


# Basic type list
# 8 types
class PrpsnModOffroadBlkd(BaseEnumSender):
    de_name     = "PrpsnModOffroadBlkd"
    fdx_name    = "PrpsnModOffroadBlkd"
    fdx_groupid = 3242
    class map:
       Typ0 = 0
       Typ1 = 1
       Typ2 = 2
       Typ3 = 3
       Typ4 = 4
       Typ5 = 5
       Typ6 = 6
       Typ7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnModOffroadBlkd, self).__init__(signal_interface, item)


# Basic type list
# 8 types
class PrpsnModOfSaveBlkd(BaseEnumSender):
    de_name     = "PrpsnModOfSaveBlkd"
    fdx_name    = "PrpsnModOfSaveBlkd"
    fdx_groupid = 3241
    class map:
       Typ0 = 0
       Typ1 = 1
       Typ2 = 2
       Typ3 = 3
       Typ4 = 4
       Typ5 = 5
       Typ6 = 6
       Typ7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnModOfSaveBlkd, self).__init__(signal_interface, item)


# Basic type list
# 8 types
class PrpsnModOfTracBlkd(BaseEnumSender):
    de_name     = "PrpsnModOfTracBlkd"
    fdx_name    = "PrpsnModOfTracBlkd"
    fdx_groupid = 3241
    class map:
       Typ0 = 0
       Typ1 = 1
       Typ2 = 2
       Typ3 = 3
       Typ4 = 4
       Typ5 = 5
       Typ6 = 6
       Typ7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnModOfTracBlkd, self).__init__(signal_interface, item)


# Basic type list
# 8 types
class PrpsnModSptBlkd(BaseEnumSender):
    de_name     = "PrpsnModSptBlkd"
    fdx_name    = "PrpsnModSptBlkd"
    fdx_groupid = 3241
    class map:
       Typ0 = 0
       Typ1 = 1
       Typ2 = 2
       Typ3 = 3
       Typ4 = 4
       Typ5 = 5
       Typ6 = 6
       Typ7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PrpsnModSptBlkd, self).__init__(signal_interface, item)


# Unit: NewtonMeter,  Range:-1024->1023, Resolution: (1.0*x+0.0, raw is unsigned, 11 bits )
class PtCluTqPtCluTq(BaseFloatSender):
    de_name     = "PtCluTq.PtCluTq"
    fdx_name    = "PtCluTqPtCluTq"
    fdx_groupid = 3233
    min    = -1024
    max    = 1023
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtCluTqPtCluTq, self).__init__(signal_interface, item)


# Unit: NewtonMeter,  Range:-1024->1023, Resolution: (1.0*x+0.0, raw is unsigned, 11 bits )
class PtCluTqPtCluTqDyn(BaseFloatSender):
    de_name     = "PtCluTq.PtCluTqDyn"
    fdx_name    = "PtCluTqPtCluTqDyn"
    fdx_groupid = 3233
    min    = -1024
    max    = 1023
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtCluTqPtCluTqDyn, self).__init__(signal_interface, item)


# General DATA-TYPE to indicate quality. Each quality deviation value, De0-De7, shall be specified on the DATA-ELEMENT using this DATA-TYPE.
class PtCluTqPtCluTqQly(BaseEnumSender):
    de_name     = "PtCluTq.PtCluTqQly"
    fdx_name    = "PtCluTqPtCluTqQly"
    fdx_groupid = 3233
    class map:
       De0 = 0
       De1 = 1
       De2 = 2
       De3 = 3
       De4 = 4
       De5 = 5
       De6 = 6
       De7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtCluTqPtCluTqQly, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class PtDrvrSetgPen(BaseEnumSender):
    de_name     = "PtDrvrSetg.Pen"
    fdx_name    = "PtDrvrSetgPen"
    fdx_groupid = 3216
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtDrvrSetgPen, self).__init__(signal_interface, item)


class PtDrvrSetgSts(BaseEnumSender):
    de_name     = "PtDrvrSetg.Sts"
    fdx_name    = "PtDrvrSetgSts"
    fdx_groupid = 3216
    class map:
       Undefd = 0
       Cmft = 1
       Eco = 2
       Dyn = 3
       Pure = 4
       Hyb = 5
       Pwr = 6
       AWD = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtDrvrSetgSts, self).__init__(signal_interface, item)


class PtGearTar(BaseEnumSender):
    de_name     = "PtGearTar"
    fdx_name    = "PtGearTar"
    fdx_groupid = 3241
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtGearTar, self).__init__(signal_interface, item)


# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtAxleFrntAct(BaseFloatSender):
    de_name     = "PtTqAtWhlFrntAct.PtTqAtAxleFrntAct"
    fdx_name    = "PtTqAtAxleFrntAct"
    fdx_groupid = 3232
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtTqAtAxleFrntAct, self).__init__(signal_interface, item)


# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtWhlFrntLeAct(BaseFloatSender):
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlFrntLeAct"
    fdx_name    = "PtTqAtWhlFrntLeAct"
    fdx_groupid = 3232
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtTqAtWhlFrntLeAct, self).__init__(signal_interface, item)


# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtWhlFrntRiAct(BaseFloatSender):
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlFrntRiAct"
    fdx_name    = "PtTqAtWhlFrntRiAct"
    fdx_groupid = 3232
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtTqAtWhlFrntRiAct, self).__init__(signal_interface, item)


# General DATA-TYPE to indicate quality. Each quality deviation value, De0-De7, shall be specified on the DATA-ELEMENT using this DATA-TYPE.
class PtTqAtWhlsFrntQly(BaseEnumSender):
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlsFrntQly"
    fdx_name    = "PtTqAtWhlsFrntQly"
    fdx_groupid = 3232
    class map:
       De0 = 0
       De1 = 1
       De2 = 2
       De3 = 3
       De4 = 4
       De5 = 5
       De6 = 6
       De7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PtTqAtWhlsFrntQly, self).__init__(signal_interface, item)


class PUnit(BaseEnumSender):
    de_name     = "IndcnUnit.PUnit"
    fdx_name    = "PUnit"
    fdx_groupid = 3211
    class map:
       Pa = 0
       Psi = 1
       Bar = 2
       Atm = 3
       UkwnUnit = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PUnit, self).__init__(signal_interface, item)


class PwrChrgDetdForPrkgHeatrElec(BaseBoolSender):
    de_name     = "PwrChrgDetdForPrkgHeatrElec"
    fdx_name    = "PwrChrgDetdForPrkgHeatrElec"
    fdx_groupid = 3199


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PwrChrgDetdForPrkgHeatrElec, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class PwrLvlElecMai(BaseIntegerSender):
    de_name     = "VehModMngtGlbSafe1.PwrLvlElecMai"
    fdx_name    = "PwrLvlElecMai"
    fdx_groupid = 3192
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PwrLvlElecMai, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class PwrLvlElecSubtyp(BaseIntegerSender):
    de_name     = "VehModMngtGlbSafe1.PwrLvlElecSubtyp"
    fdx_name    = "PwrLvlElecSubtyp"
    fdx_groupid = 3192
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PwrLvlElecSubtyp, self).__init__(signal_interface, item)


# Status of power supply components
class PwrSplyErrSts(BaseEnumSender):
    de_name     = "PwrSplyErrSts"
    fdx_name    = "PwrSplyErrSts"
    fdx_groupid = 3195
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(PwrSplyErrSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:522->1710
class RadioFrqAn(BaseIntegerSender):
    de_name     = "RadioFrqAn"
    fdx_name    = "RadioFrqAn"
    fdx_groupid = 3222
    min = 522
    max = 1710


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RadioFrqAn, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RainSenMemdReqPen(BaseEnumSender):
    de_name     = "RainSenMemdReq.Pen"
    fdx_name    = "RainSenMemdReqPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RainSenMemdReqPen, self).__init__(signal_interface, item)


class RainSenMemdReqSts(BaseEnumSender):
    de_name     = "RainSenMemdReq.Sts"
    fdx_name    = "RainSenMemdReqSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RainSenMemdReqSts, self).__init__(signal_interface, item)


class ReAxleWarn(BaseEnumSender):
    de_name     = "ReAxleWarn"
    fdx_name    = "ReAxleWarn"
    fdx_groupid = 3197
    class map:
       NoWarn = 0
       SoftWarn = 1
       HardWarn = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ReAxleWarn, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->32767
class RegnCodInHznData(BaseIntegerSender):
    de_name     = "HznData.RegnCod"
    fdx_name    = "RegnCodInHznData"
    fdx_groupid = 3214
    min = 0
    max = 32767


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RegnCodInHznData, self).__init__(signal_interface, item)


# Unit: Deg,  Range:0->360, Resolution: (1.4173228346456692*x+0.0, raw is unsigned, 8 bits )
class RelDirInHznPosn(BaseFloatSender):
    de_name     = "HznPosn.RelDir"
    fdx_name    = "RelDirInHznPosn"
    fdx_groupid = 3212
    min    = 0
    max    = 360
    scale  = 1.4173228346456692
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RelDirInHznPosn, self).__init__(signal_interface, item)


# Unit: %,  Range:0->103.33333323, Resolution: (3.33333333*x+0.0, raw is unsigned, 5 bits )
class RelProbltyInHznEdge(BaseFloatSender):
    de_name     = "HznEdge.RelProblty"
    fdx_name    = "RelProbltyInHznEdge"
    fdx_groupid = 3215
    min    = 0
    max    = 103.33333323
    scale  = 3.33333333
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RelProbltyInHznEdge, self).__init__(signal_interface, item)


# Unit: %,  Range:0->103.33333323, Resolution: (3.33333333*x+0.0, raw is unsigned, 5 bits )
class RelProbltyInHznSeg(BaseFloatSender):
    de_name     = "HznSeg.RelProblty"
    fdx_name    = "RelProbltyInHznSeg"
    fdx_groupid = 3215
    min    = 0
    max    = 103.33333323
    scale  = 3.33333333
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RelProbltyInHznSeg, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			RiOfWayOfHdPah	Right Of Way Of Head Path	Parent path has right-of-way over sub-path
# 1			1			RiOfWayOfChdPah	Right Of Way Of Child Path	Sub-path has right-of-way over parent path
# 2			2			Ukwn		Unknown			Unknown
class RiOfWayInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.RiOfWay"
    fdx_name    = "RiOfWayInHznEdge"
    fdx_groupid = 3215
    class map:
       RiOfWayOfHdPah = 0
       RiOfWayOfChdPah = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RiOfWayInHznEdge, self).__init__(signal_interface, item)


class RlyPwrDistbnCmd1WdBattSaveCmd(BaseEnumSender):
    de_name     = "RlyPwrDistbnCmd1WdBattSaveCmd"
    fdx_name    = "RlyPwrDistbnCmd1WdBattSaveCmd"
    fdx_groupid = 3199
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RlyPwrDistbnCmd1WdBattSaveCmd, self).__init__(signal_interface, item)


class RlyPwrDistbnCmd1WdIgnRlyCmd(BaseEnumSender):
    de_name     = "RlyPwrDistbnCmd1WdIgnRlyCmd"
    fdx_name    = "RlyPwrDistbnCmd1WdIgnRlyCmd"
    fdx_groupid = 3200
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RlyPwrDistbnCmd1WdIgnRlyCmd, self).__init__(signal_interface, item)


class RlyPwrDistbnCmd1WdIgnRlyExtCmd(BaseEnumSender):
    de_name     = "RlyPwrDistbnCmd1WdIgnRlyExtCmd"
    fdx_name    = "RlyPwrDistbnCmd1WdIgnRlyExtCmd"
    fdx_groupid = 3200
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RlyPwrDistbnCmd1WdIgnRlyExtCmd, self).__init__(signal_interface, item)


class RlyPwrDistbnCmd1WdPreBattSaveCmd(BaseEnumSender):
    de_name     = "RlyPwrDistbnCmd1WdPreBattSaveCmd"
    fdx_name    = "RlyPwrDistbnCmd1WdPreBattSaveCmd"
    fdx_groupid = 3199
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RlyPwrDistbnCmd1WdPreBattSaveCmd, self).__init__(signal_interface, item)


class RmnLockgPrsnlReq(BaseEnumSender):
    de_name     = "RmnLockgPrsnlReq"
    fdx_name    = "RmnLockgPrsnlReq"
    fdx_groupid = 3196
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RmnLockgPrsnlReq, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RngbdIllmnCmdPen(BaseEnumSender):
    de_name     = "RngbdIllmnCmd.Pen"
    fdx_name    = "RngbdIllmnCmdPen"
    fdx_groupid = 3217
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RngbdIllmnCmdPen, self).__init__(signal_interface, item)


# For running board illumination setting value
class RngbdIllmnCmdSts(BaseEnumSender):
    de_name     = "RngbdIllmnCmd.Cmd"
    fdx_name    = "RngbdIllmnCmdSts"
    fdx_groupid = 3217
    class map:
       Di = 0
       Aut = 1
       Resd2 = 2
       Resd3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RngbdIllmnCmdSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->7
class RoadClassInHznEdge(BaseIntegerSender):
    de_name     = "HznEdge.RoadClass"
    fdx_name    = "RoadClassInHznEdge"
    fdx_groupid = 3215
    min = 0
    max = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadClassInHznEdge, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->7
class RoadClassInHznSeg(BaseIntegerSender):
    de_name     = "HznSeg.RoadClass"
    fdx_name    = "RoadClassInHznSeg"
    fdx_groupid = 3215
    min = 0
    max = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadClassInHznSeg, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RoadFricIndcnActvPen(BaseEnumSender):
    de_name     = "RoadFricIndcnActv.Pen"
    fdx_name    = "RoadFricIndcnActvPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadFricIndcnActvPen, self).__init__(signal_interface, item)


class RoadFricIndcnActvSts(BaseEnumSender):
    de_name     = "RoadFricIndcnActv.Sts"
    fdx_name    = "RoadFricIndcnActvSts"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadFricIndcnActvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class RoadFricIndcnSts(BaseEnumSender):
    de_name     = "RoadFricIndcnSts"
    fdx_name    = "RoadFricIndcnSts"
    fdx_groupid = 3188
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadFricIndcnSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoWarn		No Warning		No warning
# 1			1			FricLoPsbl	Friction 	Low Possible		Potentially low friction
# 2			2			FricLoCfmd	Friction Low Confirmed	Confirmed low friction
class RoadFricWarnReq(BaseEnumSender):
    de_name     = "RoadFricWarnReq"
    fdx_name    = "RoadFricWarnReq"
    fdx_groupid = 3188
    class map:
       NoWarn = 0
       FricLoPsbl = 1
       FricLoCfmd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadFricWarnReq, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			SegNotPartOfTownOrCty	Segment Not Part Of Town Or City	Segment is not part of a built-up area.
# 1			1			SegPartOfTownOrCty	Segment Part Of Town Or City	Segment is part of a built-up area.
# 2			2			Ukwn			Unknown				Unknown
class RoadLiArInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.RoadLiAr"
    fdx_name    = "RoadLiArInHznSeg"
    fdx_groupid = 3215
    class map:
       SegNotPartOfTownOrCty = 0
       SegPartOfTownOrCty = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadLiArInHznSeg, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			SegNotPartOfRoadMpl	Segment Not Part Of Road Multiple	Segment is not part of a divided road/dual carriageway.
# 1			1			SegPartOfRoadMpl		Segment Part Of Road Multiple	Segment is part of a divided road/dual carriageway.
# 2			2			Ukwn			Unknown				Unknown
class RoadMplInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.RoadMpl"
    fdx_name    = "RoadMplInHznSeg"
    fdx_groupid = 3215
    class map:
       SegNotPartOfRoadMpl = 0
       SegPartOfRoadMpl = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadMplInHznSeg, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RoadSgnInfoActvPen(BaseEnumSender):
    de_name     = "RoadSgnInfoActv.Pen"
    fdx_name    = "RoadSgnInfoActvPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadSgnInfoActvPen, self).__init__(signal_interface, item)


class RoadSgnInfoActvSts(BaseEnumSender):
    de_name     = "RoadSgnInfoActv.Sts"
    fdx_name    = "RoadSgnInfoActvSts"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadSgnInfoActvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class RoadSgnInfoSts(BaseEnumSender):
    de_name     = "RoadSgnInfoSts"
    fdx_name    = "RoadSgnInfoSts"
    fdx_groupid = 3188
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadSgnInfoSts, self).__init__(signal_interface, item)


class RoadSpdLimActvSts(BaseEnumSender):
    de_name     = "RoadSpdLimActvSts"
    fdx_name    = "RoadSpdLimActvSts"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadSpdLimActvSts, self).__init__(signal_interface, item)


class RoadUsrProtnActv(BaseEnumSender):
    de_name     = "RoadUsrProtnActv"
    fdx_name    = "RoadUsrProtnActv"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadUsrProtnActv, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class RoadUsrProtnSts(BaseEnumSender):
    de_name     = "RoadUsrProtnSts"
    fdx_name    = "RoadUsrProtnSts"
    fdx_groupid = 3189
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RoadUsrProtnSts, self).__init__(signal_interface, item)


# Unit: rad/s,  Range:-6->6, Resolution: (0.000244140625*x+0.0, raw is signed, 16 bits )
class RollRate1(BaseFloatSender):
    de_name     = "AgDataRawSafe.RollRate"
    fdx_name    = "RollRate1"
    fdx_groupid = 3224
    min    = -6
    max    = 6
    scale  = 0.000244140625
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RollRate1, self).__init__(signal_interface, item)


class RollRate1Qf1(BaseEnumSender):
    de_name     = "AgDataRawSafe.RollRateQf"
    fdx_name    = "RollRate1Qf1"
    fdx_groupid = 3224
    class map:
       DevOfDataUndefd = 0
       DataTmpUndefdAndEvlnInProgs = 1
       DevOfDataNotWithinRngAllwd = 2
       DataCalcdWithDevDefd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RollRate1Qf1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->1023
class RouteInfoDestInfo(BaseIntegerSender):
    de_name     = "RouteInfo.DestInfo"
    fdx_name    = "RouteInfoDestInfo"
    fdx_groupid = 3221
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RouteInfoDestInfo, self).__init__(signal_interface, item)


# Unit: %,  Range:0->100
class RouteInfoDestProblty(BaseIntegerSender):
    de_name     = "RouteInfo.DestProblty"
    fdx_name    = "RouteInfoDestProblty"
    fdx_groupid = 3221
    min = 0
    max = 100


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RouteInfoDestProblty, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class RouteInfoDestSrc(BaseIntegerSender):
    de_name     = "RouteInfo.DestSrc"
    fdx_name    = "RouteInfoDestSrc"
    fdx_groupid = 3221
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RouteInfoDestSrc, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->127
class RouteInfoIdOfDest(BaseIntegerSender):
    de_name     = "RouteInfo.IdOfDest"
    fdx_name    = "RouteInfoIdOfDest"
    fdx_groupid = 3221
    min = 0
    max = 127


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RouteInfoIdOfDest, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4095
class RouteInfoNrOfDestCmpl(BaseIntegerSender):
    de_name     = "RouteInfo.NrOfDestCmpl"
    fdx_name    = "RouteInfoNrOfDestCmpl"
    fdx_groupid = 3221
    min = 0
    max = 4095


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RouteInfoNrOfDestCmpl, self).__init__(signal_interface, item)


class RsdsSysStsLeCtraSts(BaseEnumSender):
    de_name     = "RsdsSysStsLe.CtraSts"
    fdx_name    = "RsdsSysStsLeCtraSts"
    fdx_groupid = 3188
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsLeCtraSts, self).__init__(signal_interface, item)


class RsdsSysStsLeCtraSts1(BaseEnumSender):
    de_name     = "RsdsSysStsLe.CtraSts1"
    fdx_name    = "RsdsSysStsLeCtraSts1"
    fdx_groupid = 3188
    class map:
       Active = 0
       Inactive = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsLeCtraSts1, self).__init__(signal_interface, item)


class RsdsSysStsLeLcmaSts(BaseEnumSender):
    de_name     = "RsdsSysStsLe.LcmaSts"
    fdx_name    = "RsdsSysStsLeLcmaSts"
    fdx_groupid = 3188
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsLeLcmaSts, self).__init__(signal_interface, item)


class RsdsSysStsLeLcmaSts1(BaseEnumSender):
    de_name     = "RsdsSysStsLe.LcmaSts1"
    fdx_name    = "RsdsSysStsLeLcmaSts1"
    fdx_groupid = 3188
    class map:
       Active = 0
       Inactive = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsLeLcmaSts1, self).__init__(signal_interface, item)


class RsdsSysStsLeRcmSts(BaseEnumSender):
    de_name     = "RsdsSysStsLe.RcmSts"
    fdx_name    = "RsdsSysStsLeRcmSts"
    fdx_groupid = 3188
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsLeRcmSts, self).__init__(signal_interface, item)


class RsdsSysStsLeRcwSts(BaseEnumSender):
    de_name     = "RsdsSysStsLe.RcwSts"
    fdx_name    = "RsdsSysStsLeRcwSts"
    fdx_groupid = 3188
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsLeRcwSts, self).__init__(signal_interface, item)


class RsdsSysStsLeRsdsSts(BaseEnumSender):
    de_name     = "RsdsSysStsLe.RsdsSts"
    fdx_name    = "RsdsSysStsLeRsdsSts"
    fdx_groupid = 3188
    class map:
       Cfg = 0
       StrtUp = 1
       Runng = 2
       Blkd = 3
       Faulty = 4
       Shutdown = 5
       Hot = 6
       Cal = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsLeRsdsSts, self).__init__(signal_interface, item)


class RsdsSysStsRiCtraSts(BaseEnumSender):
    de_name     = "RsdsSysStsRi.CtraSts"
    fdx_name    = "RsdsSysStsRiCtraSts"
    fdx_groupid = 3188
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsRiCtraSts, self).__init__(signal_interface, item)


class RsdsSysStsRiCtraSts1(BaseEnumSender):
    de_name     = "RsdsSysStsRi.CtraSts1"
    fdx_name    = "RsdsSysStsRiCtraSts1"
    fdx_groupid = 3188
    class map:
       Active = 0
       Inactive = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsRiCtraSts1, self).__init__(signal_interface, item)


class RsdsSysStsRiLcmaSts(BaseEnumSender):
    de_name     = "RsdsSysStsRi.LcmaSts"
    fdx_name    = "RsdsSysStsRiLcmaSts"
    fdx_groupid = 3188
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsRiLcmaSts, self).__init__(signal_interface, item)


class RsdsSysStsRiLcmaSts1(BaseEnumSender):
    de_name     = "RsdsSysStsRi.LcmaSts1"
    fdx_name    = "RsdsSysStsRiLcmaSts1"
    fdx_groupid = 3188
    class map:
       Active = 0
       Inactive = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsRiLcmaSts1, self).__init__(signal_interface, item)


class RsdsSysStsRiRcmSts(BaseEnumSender):
    de_name     = "RsdsSysStsRi.RcmSts"
    fdx_name    = "RsdsSysStsRiRcmSts"
    fdx_groupid = 3188
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsRiRcmSts, self).__init__(signal_interface, item)


class RsdsSysStsRiRcwSts(BaseEnumSender):
    de_name     = "RsdsSysStsRi.RcwSts"
    fdx_name    = "RsdsSysStsRiRcwSts"
    fdx_groupid = 3188
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsRiRcwSts, self).__init__(signal_interface, item)


class RsdsSysStsRiRsdsSts(BaseEnumSender):
    de_name     = "RsdsSysStsRi.RsdsSts"
    fdx_name    = "RsdsSysStsRiRsdsSts"
    fdx_groupid = 3188
    class map:
       Cfg = 0
       StrtUp = 1
       Runng = 2
       Blkd = 3
       Faulty = 4
       Shutdown = 5
       Hot = 6
       Cal = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(RsdsSysStsRiRsdsSts, self).__init__(signal_interface, item)


# 0: No navigation solution
# 1: 3 satelites solution (2D)
# 2: more than 3 satellites solution (3D)
# 3: CSF (Cold Start Fix)
class SatltPosnStsPrm1InPosnFromSatlt(BaseEnumSender):
    de_name     = "PosnFromSatlt.SatltPosnStsPrm1"
    fdx_name    = "SatltPosnStsPrm1InPosnFromSatlt"
    fdx_groupid = 3229
    class map:
       PosnStsOfLvl1 = 0
       PosnStsOfLvl2 = 1
       PosnStsOfLvl3 = 2
       PosnStsOfLvl4 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltPosnStsPrm1InPosnFromSatlt, self).__init__(signal_interface, item)


class SatltPosnStsPrm2InPosnFromSatlt(BaseBoolSender):
    de_name     = "PosnFromSatlt.SatltPosnStsPrm2"
    fdx_name    = "SatltPosnStsPrm2InPosnFromSatlt"
    fdx_groupid = 3229


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltPosnStsPrm2InPosnFromSatlt, self).__init__(signal_interface, item)


class SatltPosnStsPrm3InPosnFromSatlt(BaseBoolSender):
    de_name     = "PosnFromSatlt.SatltPosnStsPrm3"
    fdx_name    = "SatltPosnStsPrm3InPosnFromSatlt"
    fdx_groupid = 3229


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltPosnStsPrm3InPosnFromSatlt, self).__init__(signal_interface, item)


class SatltSysNo1InUseInPosnFromSatlt(BaseEnumSender):
    de_name     = "PosnFromSatlt.SatltSysNo1InUse"
    fdx_name    = "SatltSysNo1InUseInPosnFromSatlt"
    fdx_groupid = 3229
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltSysNo1InUseInPosnFromSatlt, self).__init__(signal_interface, item)


class SatltSysNo2InUseInPosnFromSatlt(BaseEnumSender):
    de_name     = "PosnFromSatlt.SatltSysNo2InUse"
    fdx_name    = "SatltSysNo2InUseInPosnFromSatlt"
    fdx_groupid = 3229
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltSysNo2InUseInPosnFromSatlt, self).__init__(signal_interface, item)


class SatltSysNo3InUseInPosnFromSatlt(BaseEnumSender):
    de_name     = "PosnFromSatlt.SatltSysNo3InUse"
    fdx_name    = "SatltSysNo3InUseInPosnFromSatlt"
    fdx_groupid = 3229
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltSysNo3InUseInPosnFromSatlt, self).__init__(signal_interface, item)


class SatltSysNo4InUseInPosnFromSatlt(BaseEnumSender):
    de_name     = "PosnFromSatlt.SatltSysNo4InUse"
    fdx_name    = "SatltSysNo4InUseInPosnFromSatlt"
    fdx_groupid = 3229
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltSysNo4InUseInPosnFromSatlt, self).__init__(signal_interface, item)


class SatltSysNo5InUseInPosnFromSatlt(BaseEnumSender):
    de_name     = "PosnFromSatlt.SatltSysNo5InUse"
    fdx_name    = "SatltSysNo5InUseInPosnFromSatlt"
    fdx_groupid = 3229
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltSysNo5InUseInPosnFromSatlt, self).__init__(signal_interface, item)


class SatltSysNo6InUseInPosnFromSatlt(BaseEnumSender):
    de_name     = "PosnFromSatlt.SatltSysNo6InUse"
    fdx_name    = "SatltSysNo6InUseInPosnFromSatlt"
    fdx_groupid = 3229
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SatltSysNo6InUseInPosnFromSatlt, self).__init__(signal_interface, item)


class SaveSetgToMemPrmnt(BaseEnumSender):
    de_name     = "SaveSetgToMemPrmnt"
    fdx_name    = "SaveSetgToMemPrmnt"
    fdx_groupid = 3211
    class map:
       Off = 0
       On = 1
       Aut = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SaveSetgToMemPrmnt, self).__init__(signal_interface, item)


# Unit: %,  Range:0->100, Resolution: (5.0*x+0.0, raw is unsigned, 0 bits )
class ScrBarVolIndcn(BaseFloatSender):
    de_name     = "ScrBarVolIndcn"
    fdx_name    = "ScrBarVolIndcn"
    fdx_groupid = 3244
    min    = 0
    max    = 100
    scale  = 5.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ScrBarVolIndcn, self).__init__(signal_interface, item)


# Unit: Litre,  Range:0->31
class ScrMaxFillgVol(BaseIntegerSender):
    de_name     = "ScrMaxFillgVol"
    fdx_name    = "ScrMaxFillgVol"
    fdx_groupid = 3244
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ScrMaxFillgVol, self).__init__(signal_interface, item)


# Unit: Litre,  Range:0->31
class ScrReagentTankVol(BaseIntegerSender):
    de_name     = "ScrReagentTankVol"
    fdx_name    = "ScrReagentTankVol"
    fdx_groupid = 3244
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ScrReagentTankVol, self).__init__(signal_interface, item)


class SeatBackUnlckdThrdLe(BaseBoolSender):
    de_name     = "SeatBackUnlckdThrd.SeatBackUnlckdLe"
    fdx_name    = "SeatBackUnlckdThrdLe"
    fdx_groupid = 3198


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatBackUnlckdThrdLe, self).__init__(signal_interface, item)


class SeatBackUnlckdThrdRi(BaseBoolSender):
    de_name     = "SeatBackUnlckdThrd.SeatBackUnlckdRi"
    fdx_name    = "SeatBackUnlckdThrdRi"
    fdx_groupid = 3198


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatBackUnlckdThrdRi, self).__init__(signal_interface, item)


class SeatDispBtnPsdDrvrSeatDispBtnPsd(BaseBoolSender):
    de_name     = "SeatDispBtnPsd.DrvrSeatDispBtnPsd"
    fdx_name    = "SeatDispBtnPsdDrvrSeatDispBtnPsd"
    fdx_groupid = 3215


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatDispBtnPsdDrvrSeatDispBtnPsd, self).__init__(signal_interface, item)


class SeatDispBtnPsdPassSeatDispBtnPsd(BaseBoolSender):
    de_name     = "SeatDispBtnPsd.PassSeatDispBtnPsd"
    fdx_name    = "SeatDispBtnPsdPassSeatDispBtnPsd"
    fdx_groupid = 3215


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatDispBtnPsdPassSeatDispBtnPsd, self).__init__(signal_interface, item)


class SeatHeatDurgClimaEnad(BaseEnumSender):
    de_name     = "SeatHeatDurgClimaEnad"
    fdx_name    = "SeatHeatDurgClimaEnad"
    fdx_groupid = 3216
    class map:
       SeatHeatOff = 0
       SeatDrvOn = 1
       SeatPassOn = 2
       SeatDrvrAndPass = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatHeatDurgClimaEnad, self).__init__(signal_interface, item)


# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class SeatHeatgAutCdn(BaseEnumSender):
    de_name     = "SeatHeatgAutCdn"
    fdx_name    = "SeatHeatgAutCdn"
    fdx_groupid = 3196
    class map:
       Rst = 0
       Set = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatHeatgAutCdn, self).__init__(signal_interface, item)


class SeatSwtLeSigThrd(BaseEnumSender):
    de_name     = "SeatSwtLeSigThrd"
    fdx_name    = "SeatSwtLeSigThrd"
    fdx_groupid = 3211
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatSwtLeSigThrd, self).__init__(signal_interface, item)


class SeatSwtRiSigThrd(BaseEnumSender):
    de_name     = "SeatSwtRiSigThrd"
    fdx_name    = "SeatSwtRiSigThrd"
    fdx_groupid = 3211
    class map:
       Idle = 0
       Up = 1
       Dwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatSwtRiSigThrd, self).__init__(signal_interface, item)


# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class SeatVentnAutCdn(BaseEnumSender):
    de_name     = "SeatVentnAutCdn"
    fdx_name    = "SeatVentnAutCdn"
    fdx_groupid = 3200
    class map:
       Rst = 0
       Set = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SeatVentnAutCdn, self).__init__(signal_interface, item)


# Unit: s,  Range:0->59
class Sec(BaseIntegerSender):
    de_name     = "TiAndDateIndcn.Sec1"
    fdx_name    = "Sec"
    fdx_groupid = 3197
    min = 0
    max = 59


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Sec, self).__init__(signal_interface, item)


# Unit: s,  Range:0->59
class Sec1(BaseIntegerSender):
    de_name     = "SetTiAndDate.Second"
    fdx_name    = "Sec1"
    fdx_groupid = 3214
    min = 0
    max = 59


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Sec1, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForDrvrMtr(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForDrvrMtr"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForDrvrMtr"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForDrvrMtr, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForDstLong(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForDstLong"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForDstLong"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForDstLong, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForDstSho1(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForDstSho1"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForDstSho1"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForDstSho1, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForDstSho2(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForDstSho2"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForDstSho2"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForDstSho2, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg1(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForEgyCnsAvg1"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg1"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg1, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg2(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForEgyCnsAvg2"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg2"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForEgyCnsAvg2, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForEgyDst(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForEgyDst"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForEgyDst"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForEgyDst, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForFuCns(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForFuCns"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForFuCns"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForFuCns, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg1(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForFuCnsAvg1"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg1"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg1, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg2(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForFuCnsAvg2"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg2"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForFuCnsAvg2, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForFuDst(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForFuDst"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForFuDst"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForFuDst, self).__init__(signal_interface, item)


class SetgAndRstOfTripForDrvrSetgTripForRstOfAllTripFct1(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForRstOfAllTripFct1"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForRstOfAllTripFct1"
    fdx_groupid = 3216
    class map:
       NoRstOfTrip1 = 0
       RstOfDstSho1 = 1
       RstOfFuCnsAvg1 = 2
       RstOfEgyCnsAvg1 = 3
       RstOfForSpdAvg1 = 4
       RstOfTiDrv1 = 5
       RstOfAllTrip1 = 6
       Resv = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForRstOfAllTripFct1, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForSpdAvg1(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForSpdAvg1"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForSpdAvg1"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForSpdAvg1, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForSpdAvg2(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForSpdAvg2"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForSpdAvg2"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForSpdAvg2, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForSpdDig(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForSpdDig"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForSpdDig"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForSpdDig, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForSpdPtr(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForSpdPtr"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForSpdPtr"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForSpdPtr, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForSpdTrvl(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForSpdTrvl"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForSpdTrvl"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForSpdTrvl, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForTiDrv1(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForTiDrv1"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForTiDrv1"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForTiDrv1, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripForTiDrv2(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForTiDrv2"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForTiDrv2"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForTiDrv2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->1023
class SetgAndRstOfTripForDrvrSetgTripForTiRstCdn(BaseIntegerSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripForTiRstCdn"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripForTiRstCdn"
    fdx_groupid = 3216
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripForTiRstCdn, self).__init__(signal_interface, item)


# To protect critical signaling from one bit flipping.
#
# Min	Max	Physical Value	"Long name"
# 0	0	NotVld		Not Valid
# 1	1	Off		Off
# 2	2	On		On
# 3	3	NotVld		Not Valid
class SetgAndRstOfTripForDrvrSetgTripOffOn(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripOffOn"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripOffOn"
    fdx_groupid = 3216
    class map:
       NotVld1 = 0
       Off = 1
       On = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripOffOn, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SetgAndRstOfTripForDrvrSetgTripWithProfID(BaseEnumSender):
    de_name     = "SetgAndRstOfTripForDrvr.SetgTripWithProfID"
    fdx_name    = "SetgAndRstOfTripForDrvrSetgTripWithProfID"
    fdx_groupid = 3216
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetgAndRstOfTripForDrvrSetgTripWithProfID, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SetOfLangIdPen(BaseEnumSender):
    de_name     = "SetOfLang.IdPen"
    fdx_name    = "SetOfLangIdPen"
    fdx_groupid = 3214
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetOfLangIdPen, self).__init__(signal_interface, item)


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
class SetOfLangLangTyp(BaseEnumSender):
    de_name     = "SetOfLang.LangTyp"
    fdx_name    = "SetOfLangLangTyp"
    fdx_groupid = 3214
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SetOfLangLangTyp, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class SftyAudDend(BaseEnumSender):
    de_name     = "SftySigGroupFromAudSafe.SftyAudDend"
    fdx_name    = "SftyAudDend"
    fdx_groupid = 3211
    class map:
       NotVld1 = 0
       No = 1
       Yes = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyAudDend, self).__init__(signal_interface, item)


class SftyCchAccStsActv(BaseEnumSender):
    de_name     = "SftyCchAccSts.Actv"
    fdx_name    = "SftyCchAccStsActv"
    fdx_groupid = 3190
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchAccStsActv, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class SftyCchAccStsDay(BaseIntegerSender):
    de_name     = "SftyCchAccSts.Day"
    fdx_name    = "SftyCchAccStsDay"
    fdx_groupid = 3190
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchAccStsDay, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class SftyCchAccStsMth(BaseIntegerSender):
    de_name     = "SftyCchAccSts.Mth"
    fdx_name    = "SftyCchAccStsMth"
    fdx_groupid = 3190
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchAccStsMth, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class SftyCchAccStsYr(BaseIntegerSender):
    de_name     = "SftyCchAccSts.Yr"
    fdx_name    = "SftyCchAccStsYr"
    fdx_groupid = 3190
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchAccStsYr, self).__init__(signal_interface, item)


class SftyCchLaneKeepAidStsActv(BaseEnumSender):
    de_name     = "SftyCchLaneKeepAidSts.Actv"
    fdx_name    = "SftyCchLaneKeepAidStsActv"
    fdx_groupid = 3190
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLaneKeepAidStsActv, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class SftyCchLaneKeepAidStsDay(BaseIntegerSender):
    de_name     = "SftyCchLaneKeepAidSts.Day"
    fdx_name    = "SftyCchLaneKeepAidStsDay"
    fdx_groupid = 3190
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLaneKeepAidStsDay, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class SftyCchLaneKeepAidStsMth(BaseIntegerSender):
    de_name     = "SftyCchLaneKeepAidSts.Mth"
    fdx_name    = "SftyCchLaneKeepAidStsMth"
    fdx_groupid = 3190
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLaneKeepAidStsMth, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class SftyCchLaneKeepAidStsYr(BaseIntegerSender):
    de_name     = "SftyCchLaneKeepAidSts.Yr"
    fdx_name    = "SftyCchLaneKeepAidStsYr"
    fdx_groupid = 3190
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLaneKeepAidStsYr, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxAttention(BaseFloatSender):
    de_name     = "SftyCchLongTermIdx.Attention"
    fdx_name    = "SftyCchLongTermIdxAttention"
    fdx_groupid = 3190
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxAttention, self).__init__(signal_interface, item)


class SftyCchLongTermIdxAttentionAvl(BaseEnumSender):
    de_name     = "SftyCchLongTermIdx.AttentionAvl"
    fdx_name    = "SftyCchLongTermIdxAttentionAvl"
    fdx_groupid = 3190
    class map:
       NoData = 0
       NotAvl = 1
       Avl = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxAttentionAvl, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxDstToVeh(BaseFloatSender):
    de_name     = "SftyCchLongTermIdx.DstToVeh"
    fdx_name    = "SftyCchLongTermIdxDstToVeh"
    fdx_groupid = 3190
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxDstToVeh, self).__init__(signal_interface, item)


class SftyCchLongTermIdxDstToVehAvl(BaseEnumSender):
    de_name     = "SftyCchLongTermIdx.DstToVehAvl"
    fdx_name    = "SftyCchLongTermIdxDstToVehAvl"
    fdx_groupid = 3190
    class map:
       NoData = 0
       NotAvl = 1
       Avl = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxDstToVehAvl, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxKeepSpdLim(BaseFloatSender):
    de_name     = "SftyCchLongTermIdx.KeepSpdLim"
    fdx_name    = "SftyCchLongTermIdxKeepSpdLim"
    fdx_groupid = 3190
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxKeepSpdLim, self).__init__(signal_interface, item)


class SftyCchLongTermIdxKeepSpdLimAvl(BaseEnumSender):
    de_name     = "SftyCchLongTermIdx.KeepSpdLimAvl"
    fdx_name    = "SftyCchLongTermIdxKeepSpdLimAvl"
    fdx_groupid = 3190
    class map:
       NoData = 0
       NotAvl = 1
       Avl = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxKeepSpdLimAvl, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxLaneKeep(BaseFloatSender):
    de_name     = "SftyCchLongTermIdx.LaneKeep"
    fdx_name    = "SftyCchLongTermIdxLaneKeep"
    fdx_groupid = 3190
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxLaneKeep, self).__init__(signal_interface, item)


class SftyCchLongTermIdxLaneKeepAvl(BaseEnumSender):
    de_name     = "SftyCchLongTermIdx.LaneKeepAvl"
    fdx_name    = "SftyCchLongTermIdxLaneKeepAvl"
    fdx_groupid = 3190
    class map:
       NoData = 0
       NotAvl = 1
       Avl = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxLaneKeepAvl, self).__init__(signal_interface, item)


class SftyCchLongTermIdxRst(BaseEnumSender):
    de_name     = "SftyCchLongTermIdxRst"
    fdx_name    = "SftyCchLongTermIdxRst"
    fdx_groupid = 3216
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxRst, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxTot(BaseFloatSender):
    de_name     = "SftyCchLongTermIdx.Tot"
    fdx_name    = "SftyCchLongTermIdxTot"
    fdx_groupid = 3190
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxTot, self).__init__(signal_interface, item)


class SftyCchLongTermIdxTotAvl(BaseEnumSender):
    de_name     = "SftyCchLongTermIdx.TotAvl"
    fdx_name    = "SftyCchLongTermIdxTotAvl"
    fdx_groupid = 3190
    class map:
       NoData = 0
       NotAvl = 1
       Avl = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchLongTermIdxTotAvl, self).__init__(signal_interface, item)


class SftyCchPilotAssiStsActv(BaseEnumSender):
    de_name     = "SftyCchPilotAssiSts.Actv"
    fdx_name    = "SftyCchPilotAssiStsActv"
    fdx_groupid = 3190
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchPilotAssiStsActv, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->31
class SftyCchPilotAssiStsDay(BaseIntegerSender):
    de_name     = "SftyCchPilotAssiSts.Day"
    fdx_name    = "SftyCchPilotAssiStsDay"
    fdx_groupid = 3190
    min = 0
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchPilotAssiStsDay, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class SftyCchPilotAssiStsMth(BaseIntegerSender):
    de_name     = "SftyCchPilotAssiSts.Mth"
    fdx_name    = "SftyCchPilotAssiStsMth"
    fdx_groupid = 3190
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchPilotAssiStsMth, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class SftyCchPilotAssiStsYr(BaseIntegerSender):
    de_name     = "SftyCchPilotAssiSts.Yr"
    fdx_name    = "SftyCchPilotAssiStsYr"
    fdx_groupid = 3190
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchPilotAssiStsYr, self).__init__(signal_interface, item)


class SftyCchPostTripSts(BaseEnumSender):
    de_name     = "SftyCchPostTripSts"
    fdx_name    = "SftyCchPostTripSts"
    fdx_groupid = 3216
    class map:
       NoData = 0
       Drvg = 1
       PostTrip = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftyCchPostTripSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class SftySigFaildDetdByAud(BaseEnumSender):
    de_name     = "SftySigGroupFromAudSafe.SftySigFaildDetdByAud"
    fdx_name    = "SftySigFaildDetdByAud"
    fdx_groupid = 3211
    class map:
       NotVld1 = 0
       No = 1
       Yes = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftySigFaildDetdByAud, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class SftySigGroupFromAudSafeSftyAudEna(BaseEnumSender):
    de_name     = "SftySigGroupFromAudSafe.SftyAudEna"
    fdx_name    = "SftySigGroupFromAudSafeSftyAudEna"
    fdx_groupid = 3211
    class map:
       NotVld1 = 0
       No = 1
       Yes = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SftySigGroupFromAudSafeSftyAudEna, self).__init__(signal_interface, item)


class SoundExtActvSts(BaseEnumSender):
    de_name     = "SoundExtActvSts"
    fdx_name    = "SoundExtActvSts"
    fdx_groupid = 3216
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SoundExtActvSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SpdAlrmActvForRoadSgnInfoPen(BaseEnumSender):
    de_name     = "SpdAlrmActvForRoadSgnInfo.Pen"
    fdx_name    = "SpdAlrmActvForRoadSgnInfoPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdAlrmActvForRoadSgnInfoPen, self).__init__(signal_interface, item)


class SpdAlrmActvForRoadSgnInfoSts(BaseEnumSender):
    de_name     = "SpdAlrmActvForRoadSgnInfo.Sts"
    fdx_name    = "SpdAlrmActvForRoadSgnInfoSts"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdAlrmActvForRoadSgnInfoSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SpdCameraInfoSeldForRoadSgnInfoPen(BaseEnumSender):
    de_name     = "SpdCameraInfoSeldForRoadSgnInfo.Pen"
    fdx_name    = "SpdCameraInfoSeldForRoadSgnInfoPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdCameraInfoSeldForRoadSgnInfoPen, self).__init__(signal_interface, item)


class SpdCameraInfoSeldForRoadSgnInfoSts(BaseEnumSender):
    de_name     = "SpdCameraInfoSeldForRoadSgnInfo.Sts"
    fdx_name    = "SpdCameraInfoSeldForRoadSgnInfoSts"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdCameraInfoSeldForRoadSgnInfoSts, self).__init__(signal_interface, item)


# Unit: m/s,  Range:-12.8->89.4, Resolution: (0.2*x+-12.8, raw is unsigned, 9 bits )
class SpdInHznPosn(BaseFloatSender):
    de_name     = "HznPosn.Spd"
    fdx_name    = "SpdInHznPosn"
    fdx_groupid = 3212
    min    = -12.8
    max    = 89.4
    scale  = 0.2
    offset = -12.8


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdInHznPosn, self).__init__(signal_interface, item)


class SpdLimAdpvSts(BaseEnumSender):
    de_name     = "SpdLimAdpvSts"
    fdx_name    = "SpdLimAdpvSts"
    fdx_groupid = 3187
    class map:
       Off = 0
       On = 1
       NotAvl = 2
       SrvRqrd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdLimAdpvSts, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
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
class SpdLimEfcInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.SpdLimEfc"
    fdx_name    = "SpdLimEfcInHznSeg"
    fdx_groupid = 3215
    class map:
       Ukwn = 0
       Val5OrLowr = 1
       Val7 = 2
       Val10 = 3
       Val15 = 4
       Val20 = 5
       Val25 = 6
       Val30 = 7
       Val35 = 8
       Val40 = 9
       Val45 = 10
       Val50 = 11
       Val55 = 12
       Val60 = 13
       Val65 = 14
       Val70 = 15
       Val75 = 16
       Val80 = 17
       Val85 = 18
       Val90 = 19
       Val95 = 20
       Val100 = 21
       Val105 = 22
       Val110 = 23
       Val115 = 24
       Val120 = 25
       Val130 = 26
       Val140 = 27
       Val150 = 28
       Val160 = 29
       NoLim = 30


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdLimEfcInHznSeg, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"			Description
# 0			0			SecInfo		Second Information			Implicit (for instance, default speed limit in the cities)
# 1			1			PrimInfoOnTrfcSgn	Primary Information On Traffic Sign	Explicit - on traffic sign
# 2			2			PrimInfoByNight	Primary Information By Night		Explicit - by night
# 3			3			PrimInfoByDay	Primary Information By Day		Explicit - by day
# 4			4			PrimInfoByTiOfDay	Primary Information By Time Of Day	Explicit - time of day
# 5			5			PrimInfoAtRain	Primary Information At Rain		Explicit - rain
# 6			6			PrimInfoAtSnow	Primary Information At Snow		Explicit - snow
# 7			7			Ukwn		Unknown				Unknown
class SpdLimTypEfcInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.SpdLimTypEfc"
    fdx_name    = "SpdLimTypEfcInHznSeg"
    fdx_groupid = 3215
    class map:
       SecInfo = 0
       PrimInfoOnTrfcSgn = 1
       PrimInfoByNight = 2
       PrimInfoByDay = 3
       PrimInfoByTiOfDay = 4
       PrimInfoAtRain = 5
       PrimInfoAtSnow = 6
       Ukwn = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdLimTypEfcInHznSeg, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No warning
# 1			1			SpdHi			Speed High			Too high speed compared to speed limit
# 2			2			SpdHiAndSpdCtrlPassd	Speed High And Speed Control Passed	Too high speed and passing speed camera
class SpdLimWarnReq(BaseEnumSender):
    de_name     = "SpdLimWarnReq"
    fdx_name    = "SpdLimWarnReq"
    fdx_groupid = 3187
    class map:
       NoWarn = 0
       SpdHi = 1
       SpdHiAndSpdCtrlPassd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdLimWarnReq, self).__init__(signal_interface, item)


class SpdUnit(BaseEnumSender):
    de_name     = "IndcnUnit.SpdUnit"
    fdx_name    = "SpdUnit"
    fdx_groupid = 3211
    class map:
       Kmph = 0
       Mph = 1
       UkwnUnit = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdUnit, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"	Description
# 0			0			KiloMtrPerHr	Kilo Meter Per Hour	km/h
# 1			1			MilePerHr		Mile Per Hour	mph
class SpdUnitInHznData(BaseEnumSender):
    de_name     = "HznData.SpdUnit"
    fdx_name    = "SpdUnitInHznData"
    fdx_groupid = 3214
    class map:
       KiloMtrPerHr = 0
       MilePerHr = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdUnitInHznData, self).__init__(signal_interface, item)


class SpdWarn(BaseEnumSender):
    de_name     = "SpdWarn"
    fdx_name    = "SpdWarn"
    fdx_groupid = 3206
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpdWarn, self).__init__(signal_interface, item)


class SpprtForFctInDstToManvLocn(BaseEnumSender):
    de_name     = "DstToManvLocn.SpprtForFct"
    fdx_name    = "SpprtForFctInDstToManvLocn"
    fdx_groupid = 3217
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpprtForFctInDstToManvLocn, self).__init__(signal_interface, item)


class SpprtForFctInTireCircumCalByNav(BaseEnumSender):
    de_name     = "TireCircumCalByNav.SpprtForFct"
    fdx_name    = "SpprtForFctInTireCircumCalByNav"
    fdx_groupid = 3217
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SpprtForFctInTireCircumCalByNav, self).__init__(signal_interface, item)


class SrvRst(BaseEnumSender):
    de_name     = "SrvRst"
    fdx_name    = "SrvRst"
    fdx_groupid = 3207
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SrvRst, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class SrvTrig(BaseFloatSender):
    de_name     = "SrvTrig"
    fdx_name    = "SrvTrig"
    fdx_groupid = 3207
    min    = 0
    max    = 15
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SrvTrig, self).__init__(signal_interface, item)


class SteerAsscLvl(BaseEnumSender):
    de_name     = "SteerSetg.SteerAsscLvl"
    fdx_name    = "SteerAsscLvl"
    fdx_groupid = 3216
    class map:
       Ukwn = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3
       Lvl4 = 4
       Resd5 = 5
       Resd6 = 6
       Resd7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerAsscLvl, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SteerAssiActvForAutDrvPen(BaseEnumSender):
    de_name     = "SteerAssiActvForAutDrv.Pen"
    fdx_name    = "SteerAssiActvForAutDrvPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerAssiActvForAutDrvPen, self).__init__(signal_interface, item)


class SteerAssiActvForAutDrvSts(BaseEnumSender):
    de_name     = "SteerAssiActvForAutDrv.Sts"
    fdx_name    = "SteerAssiActvForAutDrvSts"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerAssiActvForAutDrvSts, self).__init__(signal_interface, item)


# Mod1 = Comfort
# Mod2 = Dynamic/Sport
# Mod3 = Reserved
# Mod4 = Reserved
class SteerMod(BaseEnumSender):
    de_name     = "SteerSetg.SteerMod"
    fdx_name    = "SteerMod"
    fdx_groupid = 3216
    class map:
       Ukwn = 0
       Mod1 = 1
       Mod2 = 2
       Mod3 = 3
       Mod4 = 4
       Resd5 = 5
       Resd6 = 6
       Resd7 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerMod, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class SteerOvrdWarnReqForAutDrv(BaseEnumSender):
    de_name     = "SftyWarnGroupFromAsySafe.SteerOvrdWarnReqForAutDrv"
    fdx_name    = "SteerOvrdWarnReqForAutDrv"
    fdx_groupid = 3182
    class map:
       NotVld1 = 0
       No = 1
       Yes = 2
       NotVld2 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerOvrdWarnReqForAutDrv, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SteerSetgPen(BaseEnumSender):
    de_name     = "SteerSetg.Pen"
    fdx_name    = "SteerSetgPen"
    fdx_groupid = 3216
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerSetgPen, self).__init__(signal_interface, item)


class SteerStsForAutDrv(BaseEnumSender):
    de_name     = "SftyWarnGroupFromAsySafe.SteerStsForAutDrv"
    fdx_name    = "SteerStsForAutDrv"
    fdx_groupid = 3182
    class map:
       Off = 0
       Stb = 1
       Actv = 2
       OvrdByDrvr = 3
       PndStb = 4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerStsForAutDrv, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			NoWarn			No Warning				No warning
# 1			1			DispWarnForDrvrSteerLe	Display Warning For Driver Steering Left	Show warning that driver shall steer left
# 2			2			DispWarnForDrvrSteerRi	Display Warning For Driver Steering Right	Show warning that driver shall steer right
# 3			3			DispGenWarn		Display Generic Warning			Show general warning
class SteerWarnReqForAutDrv(BaseEnumSender):
    de_name     = "SteerWarnReqForAutDrv"
    fdx_name    = "SteerWarnReqForAutDrv"
    fdx_groupid = 3183
    class map:
       NoWarn = 0
       DispWarnForDrvrSteerLe = 1
       DispWarnForDrvrSteerRi = 2
       DispGenWarn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWarnReqForAutDrv, self).__init__(signal_interface, item)


class SteerWarnReqForLaneKeepAid(BaseEnumSender):
    de_name     = "SteerWarnReqForLaneKeepAid"
    fdx_name    = "SteerWarnReqForLaneKeepAid"
    fdx_groupid = 3187
    class map:
       NoWarn = 0
       WarnLe = 1
       WarnRi = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWarnReqForLaneKeepAid, self).__init__(signal_interface, item)


# Unit: rad,  Range:-14.5->14.5, Resolution: (0.0009765625*x+0.0, raw is signed, 15 bits )
class SteerWhlAgSafe(BaseFloatSender):
    de_name     = "SteerWhlSnsrSafe.SteerWhlAg"
    fdx_name    = "SteerWhlAgSafe"
    fdx_groupid = 3230
    min    = -14.5
    max    = 14.5
    scale  = 0.0009765625
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWhlAgSafe, self).__init__(signal_interface, item)


# Unit: rad/s,  Range:-50->50, Resolution: (0.0078125*x+0.0, raw is signed, 14 bits )
class SteerWhlAgSpdSafe(BaseFloatSender):
    de_name     = "SteerWhlSnsrSafe.SteerWhlAgSpd"
    fdx_name    = "SteerWhlAgSpdSafe"
    fdx_groupid = 3230
    min    = -50
    max    = 50
    scale  = 0.0078125
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWhlAgSpdSafe, self).__init__(signal_interface, item)


class SteerWhlHeatgDurgClimaEnad(BaseEnumSender):
    de_name     = "SteerWhlHeatgDurgClimaEnad"
    fdx_name    = "SteerWhlHeatgDurgClimaEnad"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWhlHeatgDurgClimaEnad, self).__init__(signal_interface, item)


class SteerWhlHeatgOnReq(BaseEnumSender):
    de_name     = "SteerWhlHeatgOnReq"
    fdx_name    = "SteerWhlHeatgOnReq"
    fdx_groupid = 3215
    class map:
       Off = 0
       Lo = 1
       Med = 2
       Hi = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWhlHeatgOnReq, self).__init__(signal_interface, item)


class SteerWhlHeatgStrtAutCdnOk(BaseEnumSender):
    de_name     = "SteerWhlHeatgStrtAutCdnOk"
    fdx_name    = "SteerWhlHeatgStrtAutCdnOk"
    fdx_groupid = 3199
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWhlHeatgStrtAutCdnOk, self).__init__(signal_interface, item)


class SteerWhlSnsrQf(BaseEnumSender):
    de_name     = "SteerWhlSnsrSafe.SteerWhlSnsrQf"
    fdx_name    = "SteerWhlSnsrQf"
    fdx_groupid = 3230
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWhlSnsrQf, self).__init__(signal_interface, item)


class SteerWhlSwtPwr(BaseEnumSender):
    de_name     = "SteerWhlSwtPwr"
    fdx_name    = "SteerWhlSwtPwr"
    fdx_groupid = 3206
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SteerWhlSwtPwr, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->16777215
class StopStrtInhb(BaseIntegerSender):
    de_name     = "StopStrtInhb"
    fdx_name    = "StopStrtInhb"
    fdx_groupid = 3230
    min = 0
    max = 16777215


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(StopStrtInhb, self).__init__(signal_interface, item)


class StrtInProgs(BaseEnumSender):
    de_name     = "StrtInProgs"
    fdx_name    = "StrtInProgs"
    fdx_groupid = 3200
    class map:
       StrtStsOff = 0
       StrtStsImminent = 1
       StrtStsStrtng = 2
       StrtStsRunng = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(StrtInProgs, self).__init__(signal_interface, item)


class SunRoofPosnSts(BaseEnumSender):
    de_name     = "SunRoofPosnSts"
    fdx_name    = "SunRoofPosnSts"
    fdx_groupid = 3194
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SunRoofPosnSts, self).__init__(signal_interface, item)


class SuspCtrlSetg(BaseEnumSender):
    de_name     = "SuspSetg.SuspCtrlSetg"
    fdx_name    = "SuspCtrlSetg"
    fdx_groupid = 3215
    class map:
       Cmft = 0
       Norm = 1
       Dyn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SuspCtrlSetg, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SuspIdPen(BaseEnumSender):
    de_name     = "SuspSetg.SuspIdPen"
    fdx_name    = "SuspIdPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SuspIdPen, self).__init__(signal_interface, item)


class SwtAtCenSts(BaseEnumSender):
    de_name     = "SwtAtCenSts"
    fdx_name    = "SwtAtCenSts"
    fdx_groupid = 3214
    class map:
       NoTrig = 0
       Trig = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SwtAtCenSts, self).__init__(signal_interface, item)


# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class SwtForPassAirbCutOffSt(BaseEnumSender):
    de_name     = "SwtForPassAirbCutOffSt"
    fdx_name    = "SwtForPassAirbCutOffSt"
    fdx_groupid = 3227
    class map:
       On = 0
       Off = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(SwtForPassAirbCutOffSt, self).__init__(signal_interface, item)


# Status of the door.
class TankFlapSts(BaseEnumSender):
    de_name     = "TankFlapSts"
    fdx_name    = "TankFlapSts"
    fdx_groupid = 3196
    class map:
       Ukwn = 0
       Opend = 1
       Clsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TankFlapSts, self).__init__(signal_interface, item)


class TelmSts(BaseEnumSender):
    de_name     = "TelmSts"
    fdx_name    = "TelmSts"
    fdx_groupid = 3198
    class map:
       NoErr = 0
       TelmErr = 1
       TelmDi = 2
       TelmEnd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TelmSts, self).__init__(signal_interface, item)


class TiAndDateVld(BaseEnumSender):
    de_name     = "TiAndDateIndcn.DataValid"
    fdx_name    = "TiAndDateVld"
    fdx_groupid = 3197
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiAndDateVld, self).__init__(signal_interface, item)


# Unit: Days,  Range:1->31
class TiForDayInPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.TiForDay"
    fdx_name    = "TiForDayInPosnFromSatlt"
    fdx_groupid = 3229
    min = 1
    max = 31


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiForDayInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: hours,  Range:0->23
class TiForHrInPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.TiForHr"
    fdx_name    = "TiForHrInPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 23


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiForHrInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Mins,  Range:0->59
class TiForMinsInPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.TiForMins"
    fdx_name    = "TiForMinsInPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 59


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiForMinsInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Month,  Range:1->12
class TiForMthInPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.TiForMth"
    fdx_name    = "TiForMthInPosnFromSatlt"
    fdx_groupid = 3229
    min = 1
    max = 12


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiForMthInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: s,  Range:0->59
class TiForSecInPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.TiForSec"
    fdx_name    = "TiForSecInPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 59


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiForSecInPosnFromSatlt, self).__init__(signal_interface, item)


# Unit: Year,  Range:0->99
class TiForYrInPosnFromSatlt(BaseIntegerSender):
    de_name     = "PosnFromSatlt.TiForYr"
    fdx_name    = "TiForYrInPosnFromSatlt"
    fdx_groupid = 3229
    min = 0
    max = 99


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiForYrInPosnFromSatlt, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TiGapAdpvSeldPen(BaseEnumSender):
    de_name     = "TiGapAdpvSeld.Pen"
    fdx_name    = "TiGapAdpvSeldPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiGapAdpvSeldPen, self).__init__(signal_interface, item)


class TiGapAdpvSeldSts(BaseEnumSender):
    de_name     = "TiGapAdpvSeld.Sts"
    fdx_name    = "TiGapAdpvSeldSts"
    fdx_groupid = 3213
    class map:
       Man = 0
       Adpv = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiGapAdpvSeldSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TiGapLimdPen(BaseEnumSender):
    de_name     = "TiGapLimd.Pen"
    fdx_name    = "TiGapLimdPen"
    fdx_groupid = 3214
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiGapLimdPen, self).__init__(signal_interface, item)


# Unit: s,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class TiGapLimdSts(BaseFloatSender):
    de_name     = "TiGapLimd.Sts"
    fdx_name    = "TiGapLimdSts"
    fdx_groupid = 3214
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TiGapLimdSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4095
class TireCircumInTireCircumCalByNav(BaseIntegerSender):
    de_name     = "TireCircumCalByNav.TireCircum"
    fdx_name    = "TireCircumInTireCircumCalByNav"
    fdx_groupid = 3217
    min = 0
    max = 4095


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TireCircumInTireCircumCalByNav, self).__init__(signal_interface, item)


# Used for sending a Start or a Stop/Cancel request from HMI to a core function
class TireMonCalReq(BaseEnumSender):
    de_name     = "TireMonCalReq"
    fdx_name    = "TireMonCalReq"
    fdx_groupid = 3214
    class map:
       Idle = 0
       StrtReq = 1
       StopReq = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TireMonCalReq, self).__init__(signal_interface, item)


class TireMonrDispReq(BaseEnumSender):
    de_name     = "TireMonrDispReq"
    fdx_name    = "TireMonrDispReq"
    fdx_groupid = 3214
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TireMonrDispReq, self).__init__(signal_interface, item)


# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForFrntLe(BaseFloatSender):
    de_name     = "TirePAbsltValFrntLe.TirepabsltVal1"
    fdx_name    = "TirePAbsltValForFrntLe"
    fdx_groupid = 3197
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltValForFrntLe, self).__init__(signal_interface, item)


# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForReLe(BaseFloatSender):
    de_name     = "TirePAbsltValReLe.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValForReLe"
    fdx_groupid = 3197
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltValForReLe, self).__init__(signal_interface, item)


# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForReRi(BaseFloatSender):
    de_name     = "TirePAbsltValReRi.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValForReRi"
    fdx_groupid = 3197
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltValForReRi, self).__init__(signal_interface, item)


# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValFrntRiTirePAbsltVal1(BaseFloatSender):
    de_name     = "TirePAbsltValFrntRi.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValFrntRiTirePAbsltVal1"
    fdx_groupid = 3197
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltValFrntRiTirePAbsltVal1, self).__init__(signal_interface, item)


class TirePAbsltValFrntRiTirePPosn(BaseEnumSender):
    de_name     = "TirePAbsltValFrntRi.TirePPosn"
    fdx_name    = "TirePAbsltValFrntRiTirePPosn"
    fdx_groupid = 3197
    class map:
       NoPReadingOrPReadingOkWithOutPosn = 0
       PReadingWithPosn = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltValFrntRiTirePPosn, self).__init__(signal_interface, item)


class TirePAbsltValQFForFrnRi(BaseEnumSender):
    de_name     = "TirePAbsltValFrntRi.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQFForFrnRi"
    fdx_groupid = 3197
    class map:
       Norm = 0
       OldVal = 1
       UpdVal = 2
       SnsrFlt = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltValQFForFrnRi, self).__init__(signal_interface, item)


class TirePAbsltValQfForReLe(BaseEnumSender):
    de_name     = "TirePAbsltValReLe.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQfForReLe"
    fdx_groupid = 3197
    class map:
       Norm = 0
       OldVal = 1
       UpdVal = 2
       SnsrFlt = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltValQfForReLe, self).__init__(signal_interface, item)


class TirePAbsltValQfForReRi(BaseEnumSender):
    de_name     = "TirePAbsltValReRi.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQfForReRi"
    fdx_groupid = 3197
    class map:
       Norm = 0
       OldVal = 1
       UpdVal = 2
       SnsrFlt = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltValQfForReRi, self).__init__(signal_interface, item)


class TirePAbsltVaQflForFrntLe(BaseEnumSender):
    de_name     = "TirePAbsltValFrntLe.TirePabsltValQF"
    fdx_name    = "TirePAbsltVaQflForFrntLe"
    fdx_groupid = 3197
    class map:
       Norm = 0
       OldVal = 1
       UpdVal = 2
       SnsrFlt = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePAbsltVaQflForFrntLe, self).__init__(signal_interface, item)


class TirePCalSts(BaseEnumSender):
    de_name     = "TirePCalSts"
    fdx_name    = "TirePCalSts"
    fdx_groupid = 3197
    class map:
       Norm = 0
       Cal = 1
       CalNotCmpl = 2
       CalCmpl = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePCalSts, self).__init__(signal_interface, item)


class TirePFrntLe(BaseEnumSender):
    de_name     = "TirePMonData.TirePFrntLe"
    fdx_name    = "TirePFrntLe"
    fdx_groupid = 3197
    class map:
       NoWarn = 0
       SoftWarn = 1
       HardWarn = 2
       ResdWarn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePFrntLe, self).__init__(signal_interface, item)


class TirePFrntRi(BaseEnumSender):
    de_name     = "TirePMonData.TirePFrntRe"
    fdx_name    = "TirePFrntRi"
    fdx_groupid = 3197
    class map:
       NoWarn = 0
       SoftWarn = 1
       HardWarn = 2
       ResdWarn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePFrntRi, self).__init__(signal_interface, item)


class TirePMonSts(BaseEnumSender):
    de_name     = "TirePMonData.TirePMonSts1"
    fdx_name    = "TirePMonSts"
    fdx_groupid = 3197
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePMonSts, self).__init__(signal_interface, item)


class TirePPosnForFrntLe(BaseEnumSender):
    de_name     = "TirePAbsltValFrntLe.TirePPosn"
    fdx_name    = "TirePPosnForFrntLe"
    fdx_groupid = 3197
    class map:
       NoPReadingOrPReadingOkWithOutPosn = 0
       PReadingWithPosn = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePPosnForFrntLe, self).__init__(signal_interface, item)


class TirePPosnForReLe(BaseEnumSender):
    de_name     = "TirePAbsltValReLe.TirePPosn"
    fdx_name    = "TirePPosnForReLe"
    fdx_groupid = 3197
    class map:
       NoPReadingOrPReadingOkWithOutPosn = 0
       PReadingWithPosn = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePPosnForReLe, self).__init__(signal_interface, item)


class TirePPosnForReRi(BaseEnumSender):
    de_name     = "TirePAbsltValReRi.TirePPosn"
    fdx_name    = "TirePPosnForReRi"
    fdx_groupid = 3197
    class map:
       NoPReadingOrPReadingOkWithOutPosn = 0
       PReadingWithPosn = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePPosnForReRi, self).__init__(signal_interface, item)


class TirePReLe(BaseEnumSender):
    de_name     = "TirePMonData.TirePReLe"
    fdx_name    = "TirePReLe"
    fdx_groupid = 3197
    class map:
       NoWarn = 0
       SoftWarn = 1
       HardWarn = 2
       ResdWarn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePReLe, self).__init__(signal_interface, item)


class TirePReRi(BaseEnumSender):
    de_name     = "TirePMonData.TirePReRi"
    fdx_name    = "TirePReRi"
    fdx_groupid = 3197
    class map:
       NoWarn = 0
       SoftWarn = 1
       HardWarn = 2
       ResdWarn = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TirePReRi, self).__init__(signal_interface, item)


class TnlEntryOrEnd(BaseEnumSender):
    de_name     = "TnlEntryOrEnd"
    fdx_name    = "TnlEntryOrEnd"
    fdx_groupid = 3183
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TnlEntryOrEnd, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			SegNotPartOfTnl	Segment Not Part Of Tunnel	Segment is not part of a tunnel
# 1			1			SegPartOfTnl	Segment Part Of Tunnel	Segment is part of a tunnel
# 2			2			Ukwn		Unknown			Unknown
class TnlInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.Tnl"
    fdx_name    = "TnlInHznSeg"
    fdx_groupid = 3215
    class map:
       SegNotPartOfTnl = 0
       SegPartOfTnl = 1
       Ukwn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TnlInHznSeg, self).__init__(signal_interface, item)


class TrfcLiSpprtActv(BaseEnumSender):
    de_name     = "TrfcLiSpprtActv"
    fdx_name    = "TrfcLiSpprtActv"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrfcLiSpprtActv, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			Ukwn		Unknown			Unknown
# 1			1			Off		Off			Off
# 2			2			SpdLoStb		Speed Low Standby		Standby due to low speed
# 3			3			SpdHiStb		Speed High Standby	Standby due to high speed
# 4			4			NotAvl		Not Available		Unavailable
# 5			5			SrvRqrd		Service Required		Service required
# 6			6			On		On			On
# 7			7			NotInUse		Not In Use		Not used
class TrfcLiSpprtSts(BaseEnumSender):
    de_name     = "TrfcLiSpprtSts"
    fdx_name    = "TrfcLiSpprtSts"
    fdx_groupid = 3189
    class map:
       Ukwn = 0
       Off = 1
       SpdLoStb = 2
       SpdHiStb = 3
       NotAvl = 4
       SrvRqrd = 5
       On = 6
       NotInUse = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrfcLiSpprtSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TrfcRglnSeldForRoadSgnInfoPen(BaseEnumSender):
    de_name     = "TrfcRglnSeldForRoadSgnInfo.Pen"
    fdx_name    = "TrfcRglnSeldForRoadSgnInfoPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrfcRglnSeldForRoadSgnInfoPen, self).__init__(signal_interface, item)


class TrfcRglnSeldForRoadSgnInfoSts(BaseEnumSender):
    de_name     = "TrfcRglnSeldForRoadSgnInfo.Sts"
    fdx_name    = "TrfcRglnSeldForRoadSgnInfoSts"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrfcRglnSeldForRoadSgnInfoSts, self).__init__(signal_interface, item)


# Selects trip trip meter type
# Trip1 - Reset by driver
# Trip2 - Reset by timer (new trip condition)
# Resd - Reserved for future Trip indication modes
class TripModSeln(BaseEnumSender):
    de_name     = "TripModSeln"
    fdx_name    = "TripModSeln"
    fdx_groupid = 3213
    class map:
       Ukwn = 0
       Trip1 = 1
       Trip2 = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TripModSeln, self).__init__(signal_interface, item)


# 0 = Trailer Lamp Check not possible to execute
# 1 = Trailer Lamp Check possible to execute
class TrlrLampActvtChk(BaseEnumSender):
    de_name     = "TrlrLampChkSts1WdSts1"
    fdx_name    = "TrlrLampActvtChk"
    fdx_groupid = 3194
    class map:
       TrlrLampChkNotAvl = 0
       TrlrLampChkAvl = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrlrLampActvtChk, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TrlrLampChkAutReqPen(BaseEnumSender):
    de_name     = "TrlrLampChkAutReq.Pen"
    fdx_name    = "TrlrLampChkAutReqPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrlrLampChkAutReqPen, self).__init__(signal_interface, item)


class TrlrLampChkAutReqSts(BaseEnumSender):
    de_name     = "TrlrLampChkAutReq.Sts"
    fdx_name    = "TrlrLampChkAutReqSts"
    fdx_groupid = 3212
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrlrLampChkAutReqSts, self).__init__(signal_interface, item)


# Used for sending a Start or a Stop/Cancel request from HMI to a core function
class TrlrLampChkRemReq(BaseEnumSender):
    de_name     = "TrlrLampChkRemReq"
    fdx_name    = "TrlrLampChkRemReq"
    fdx_groupid = 3212
    class map:
       Idle = 0
       StrtReq = 1
       StopReq = 2
       Resd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrlrLampChkRemReq, self).__init__(signal_interface, item)


# 0 = No status
# 1 = Trailer Lamp Check started
# 2 = Trailer Lamp Check completed
# 3 = Operation canceled
# 4 = Trailer Lamp Check requesting engine off to driver
# 5 = Reserved
# 6 = Reserved
# 7 = Reserved
class TrlrLampChkSts(BaseEnumSender):
    de_name     = "TrlrLampChkSts1WdSts2"
    fdx_name    = "TrlrLampChkSts"
    fdx_groupid = 3193
    class map:
       Idle = 0
       TrlrLampChkStrtd = 1
       TrlrLampChkCmpl = 2
       CnclOper = 3
       TrlrLampEngOffReqd = 4
       Resd1 = 5
       Resd2 = 6
       Resd3 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrlrLampChkSts, self).__init__(signal_interface, item)


# 0 = Trailer unconnected
# 1 = Trailer connected to the vehicle
class TrlrPrsnt(BaseEnumSender):
    de_name     = "TrlrPrsntSts1WdSts"
    fdx_name    = "TrlrPrsnt"
    fdx_groupid = 3196
    class map:
       TrlrNotPrsnt = 0
       TrlrPrsnt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrlrPrsnt, self).__init__(signal_interface, item)


# Status of the door.
class TrSts(BaseEnumSender):
    de_name     = "TrSts"
    fdx_name    = "TrSts"
    fdx_groupid = 3192
    class map:
       Ukwn = 0
       Opend = 1
       Clsd = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TrSts, self).__init__(signal_interface, item)


class TUnit(BaseEnumSender):
    de_name     = "IndcnUnit.TUnit"
    fdx_name    = "TUnit"
    fdx_groupid = 3211
    class map:
       Celsius = 0
       Fahrenheit = 1
       UkwnUnit = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TUnit, self).__init__(signal_interface, item)


# Unit: Deg,  Range:0->360, Resolution: (1.4173228346456692*x+0.0, raw is unsigned, 8 bits )
class TurnAgInHznEdge(BaseFloatSender):
    de_name     = "HznEdge.TurnAg"
    fdx_name    = "TurnAgInHznEdge"
    fdx_groupid = 3215
    min    = 0
    max    = 360
    scale  = 1.4173228346456692
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TurnAgInHznEdge, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TurnAutFlsgReqPen(BaseEnumSender):
    de_name     = "LiExtReq1WdReq3.Pen"
    fdx_name    = "TurnAutFlsgReqPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TurnAutFlsgReqPen, self).__init__(signal_interface, item)


class TurnAutFlsgReqSts(BaseEnumSender):
    de_name     = "LiExtReq1WdReq3.Sts"
    fdx_name    = "TurnAutFlsgReqSts"
    fdx_groupid = 3215
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TurnAutFlsgReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TurnSpdWarnActvPen(BaseEnumSender):
    de_name     = "CrvtSpdWarnActv.Pen"
    fdx_name    = "TurnSpdWarnActvPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TurnSpdWarnActvPen, self).__init__(signal_interface, item)


class TurnSpdWarnActvSts(BaseEnumSender):
    de_name     = "CrvtSpdWarnActv.Sts"
    fdx_name    = "TurnSpdWarnActvSts"
    fdx_groupid = 3213
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TurnSpdWarnActvSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TurnSpdWarnSnvtyActPen(BaseEnumSender):
    de_name     = "CrvtSpdEstimrSnvtySeld.Pen"
    fdx_name    = "TurnSpdWarnSnvtyActPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TurnSpdWarnSnvtyActPen, self).__init__(signal_interface, item)


class TurnSpdWarnSnvtyActsSts(BaseEnumSender):
    de_name     = "CrvtSpdEstimrSnvtySeld.Sts"
    fdx_name    = "TurnSpdWarnSnvtyActsSts"
    fdx_groupid = 3212
    class map:
       CmftMod = 0
       NormMod = 1
       SptMod = 2
       CrvtMod = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TurnSpdWarnSnvtyActsSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->10000
class TwliBriRaw(BaseIntegerSender):
    de_name     = "TwliBriRaw.TwliBriRaw1"
    fdx_name    = "TwliBriRaw"
    fdx_groupid = 3193
    min = 0
    max = 10000


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TwliBriRaw, self).__init__(signal_interface, item)


class TwliBriRawQf(BaseEnumSender):
    de_name     = "TwliBriRaw.TwliBriRawQf"
    fdx_name    = "TwliBriRawQf"
    fdx_groupid = 3193
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TwliBriRawQf, self).__init__(signal_interface, item)


class TwliBriSts(BaseEnumSender):
    de_name     = "TwliBriSts"
    fdx_name    = "TwliBriSts"
    fdx_groupid = 3192
    class map:
       Night = 0
       Day = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TwliBriSts, self).__init__(signal_interface, item)


class TxPrevInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.TxPrev"
    fdx_name    = "TxPrevInHznEdge"
    fdx_groupid = 3215
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TxPrevInHznEdge, self).__init__(signal_interface, item)


class TxPrevInHznProfLong(BaseEnumSender):
    de_name     = "HznProfLong.TxPrev"
    fdx_name    = "TxPrevInHznProfLong"
    fdx_groupid = 3213
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TxPrevInHznProfLong, self).__init__(signal_interface, item)


class TxPrevInHznProfSho(BaseEnumSender):
    de_name     = "HznProfSho.TxPrev"
    fdx_name    = "TxPrevInHznProfSho"
    fdx_groupid = 3213
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TxPrevInHznProfSho, self).__init__(signal_interface, item)


class TxPrevInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.TxPrev"
    fdx_name    = "TxPrevInHznSeg"
    fdx_groupid = 3215
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TxPrevInHznSeg, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->15
class TxVersInHznData(BaseIntegerSender):
    de_name     = "HznData.TxVers"
    fdx_name    = "TxVersInHznData"
    fdx_groupid = 3214
    min = 0
    max = 15


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TxVersInHznData, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"		Description
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
class TypOfWayInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.TypOfWay"
    fdx_name    = "TypOfWayInHznEdge"
    fdx_groupid = 3215
    class map:
       Ukwn = 0
       FreeWay = 1
       RoadMpl = 2
       RoadSng = 3
       Roty = 4
       TrfcSq = 5
       Resd1 = 6
       Resd2 = 7
       RampInEquDir = 8
       FreeWayRamp = 9
       RampNotOnFreeWay = 10
       RoadForSrvAndAcs = 11
       EntryToCarPark = 12
       EntryToSrv = 13
       PedAr = 14


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TypOfWayInHznEdge, self).__init__(signal_interface, item)


# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"		Description
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
class TypOfWayInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.TypOfWay"
    fdx_name    = "TypOfWayInHznSeg"
    fdx_groupid = 3215
    class map:
       Ukwn = 0
       FreeWay = 1
       RoadMpl = 2
       RoadSng = 3
       Roty = 4
       TrfcSq = 5
       Resd1 = 6
       Resd2 = 7
       RampInEquDir = 8
       FreeWayRamp = 9
       RampNotOnFreeWay = 10
       RoadForSrvAndAcs = 11
       EntryToCarPark = 12
       EntryToSrv = 13
       PedAr = 14


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(TypOfWayInHznSeg, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte0(BaseIntegerSender):
    de_name     = "UkwnCptReqToInfoMgr.Byte0"
    fdx_name    = "UkwnCptReqToInfoMgrByte0"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToInfoMgrByte0, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte1(BaseIntegerSender):
    de_name     = "UkwnCptReqToInfoMgr.Byte1"
    fdx_name    = "UkwnCptReqToInfoMgrByte1"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToInfoMgrByte1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte2(BaseIntegerSender):
    de_name     = "UkwnCptReqToInfoMgr.Byte2"
    fdx_name    = "UkwnCptReqToInfoMgrByte2"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToInfoMgrByte2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte3(BaseIntegerSender):
    de_name     = "UkwnCptReqToInfoMgr.Byte3"
    fdx_name    = "UkwnCptReqToInfoMgrByte3"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToInfoMgrByte3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte4(BaseIntegerSender):
    de_name     = "UkwnCptReqToInfoMgr.Byte4"
    fdx_name    = "UkwnCptReqToInfoMgrByte4"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToInfoMgrByte4, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte5(BaseIntegerSender):
    de_name     = "UkwnCptReqToInfoMgr.Byte5"
    fdx_name    = "UkwnCptReqToInfoMgrByte5"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToInfoMgrByte5, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte6(BaseIntegerSender):
    de_name     = "UkwnCptReqToInfoMgr.Byte6"
    fdx_name    = "UkwnCptReqToInfoMgrByte6"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToInfoMgrByte6, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte7(BaseIntegerSender):
    de_name     = "UkwnCptReqToInfoMgr.Byte7"
    fdx_name    = "UkwnCptReqToInfoMgrByte7"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToInfoMgrByte7, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte0(BaseIntegerSender):
    de_name     = "UkwnCptReqToSoundMgr.Byte0"
    fdx_name    = "UkwnCptReqToSoundMgrByte0"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToSoundMgrByte0, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte1(BaseIntegerSender):
    de_name     = "UkwnCptReqToSoundMgr.Byte1"
    fdx_name    = "UkwnCptReqToSoundMgrByte1"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToSoundMgrByte1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte2(BaseIntegerSender):
    de_name     = "UkwnCptReqToSoundMgr.Byte2"
    fdx_name    = "UkwnCptReqToSoundMgrByte2"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToSoundMgrByte2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte3(BaseIntegerSender):
    de_name     = "UkwnCptReqToSoundMgr.Byte3"
    fdx_name    = "UkwnCptReqToSoundMgrByte3"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToSoundMgrByte3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte4(BaseIntegerSender):
    de_name     = "UkwnCptReqToSoundMgr.Byte4"
    fdx_name    = "UkwnCptReqToSoundMgrByte4"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToSoundMgrByte4, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte5(BaseIntegerSender):
    de_name     = "UkwnCptReqToSoundMgr.Byte5"
    fdx_name    = "UkwnCptReqToSoundMgrByte5"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToSoundMgrByte5, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte6(BaseIntegerSender):
    de_name     = "UkwnCptReqToSoundMgr.Byte6"
    fdx_name    = "UkwnCptReqToSoundMgrByte6"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToSoundMgrByte6, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte7(BaseIntegerSender):
    de_name     = "UkwnCptReqToSoundMgr.Byte7"
    fdx_name    = "UkwnCptReqToSoundMgrByte7"
    fdx_groupid = 3200
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptReqToSoundMgrByte7, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte0(BaseIntegerSender):
    de_name     = "UkwnCptRespFromInfoMgr.Byte0"
    fdx_name    = "UkwnCptRespFromInfoMgrByte0"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromInfoMgrByte0, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte1(BaseIntegerSender):
    de_name     = "UkwnCptRespFromInfoMgr.Byte1"
    fdx_name    = "UkwnCptRespFromInfoMgrByte1"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromInfoMgrByte1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte2(BaseIntegerSender):
    de_name     = "UkwnCptRespFromInfoMgr.Byte2"
    fdx_name    = "UkwnCptRespFromInfoMgrByte2"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromInfoMgrByte2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte3(BaseIntegerSender):
    de_name     = "UkwnCptRespFromInfoMgr.Byte3"
    fdx_name    = "UkwnCptRespFromInfoMgrByte3"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromInfoMgrByte3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte4(BaseIntegerSender):
    de_name     = "UkwnCptRespFromInfoMgr.Byte4"
    fdx_name    = "UkwnCptRespFromInfoMgrByte4"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromInfoMgrByte4, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte5(BaseIntegerSender):
    de_name     = "UkwnCptRespFromInfoMgr.Byte5"
    fdx_name    = "UkwnCptRespFromInfoMgrByte5"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromInfoMgrByte5, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte6(BaseIntegerSender):
    de_name     = "UkwnCptRespFromInfoMgr.Byte6"
    fdx_name    = "UkwnCptRespFromInfoMgrByte6"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromInfoMgrByte6, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte7(BaseIntegerSender):
    de_name     = "UkwnCptRespFromInfoMgr.Byte7"
    fdx_name    = "UkwnCptRespFromInfoMgrByte7"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromInfoMgrByte7, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte0(BaseIntegerSender):
    de_name     = "UkwnCptRespFromSoundMgr.Byte0"
    fdx_name    = "UkwnCptRespFromSoundMgrByte0"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromSoundMgrByte0, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte1(BaseIntegerSender):
    de_name     = "UkwnCptRespFromSoundMgr.Byte1"
    fdx_name    = "UkwnCptRespFromSoundMgrByte1"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromSoundMgrByte1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte2(BaseIntegerSender):
    de_name     = "UkwnCptRespFromSoundMgr.Byte2"
    fdx_name    = "UkwnCptRespFromSoundMgrByte2"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromSoundMgrByte2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte3(BaseIntegerSender):
    de_name     = "UkwnCptRespFromSoundMgr.Byte3"
    fdx_name    = "UkwnCptRespFromSoundMgrByte3"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromSoundMgrByte3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte4(BaseIntegerSender):
    de_name     = "UkwnCptRespFromSoundMgr.Byte4"
    fdx_name    = "UkwnCptRespFromSoundMgrByte4"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromSoundMgrByte4, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte5(BaseIntegerSender):
    de_name     = "UkwnCptRespFromSoundMgr.Byte5"
    fdx_name    = "UkwnCptRespFromSoundMgrByte5"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromSoundMgrByte5, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte6(BaseIntegerSender):
    de_name     = "UkwnCptRespFromSoundMgr.Byte6"
    fdx_name    = "UkwnCptRespFromSoundMgrByte6"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromSoundMgrByte6, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte7(BaseIntegerSender):
    de_name     = "UkwnCptRespFromSoundMgr.Byte7"
    fdx_name    = "UkwnCptRespFromSoundMgrByte7"
    fdx_groupid = 3217
    min = 0
    max = 255


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UkwnCptRespFromSoundMgrByte7, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckFbVisReqPen(BaseEnumSender):
    de_name     = "UnlckFbVisReq.Pen"
    fdx_name    = "UnlckFbVisReqPen"
    fdx_groupid = 3214
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UnlckFbVisReqPen, self).__init__(signal_interface, item)


class UnlckFbVisReqSts(BaseEnumSender):
    de_name     = "UnlckFbVisReq.Sts"
    fdx_name    = "UnlckFbVisReqSts"
    fdx_groupid = 3214
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UnlckFbVisReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckKeylsReqPen(BaseEnumSender):
    de_name     = "UnlckKeylsReq.IdPen"
    fdx_name    = "UnlckKeylsReqPen"
    fdx_groupid = 3214
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UnlckKeylsReqPen, self).__init__(signal_interface, item)


class UnlckKeylsReqSts(BaseEnumSender):
    de_name     = "UnlckKeylsReq.KeylsCfg"
    fdx_name    = "UnlckKeylsReqSts"
    fdx_groupid = 3214
    class map:
       UnlckCen = 0
       UnlckInd = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UnlckKeylsReqSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckRemReqPen(BaseEnumSender):
    de_name     = "UnlckRemReq.Pen"
    fdx_name    = "UnlckRemReqPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UnlckRemReqPen, self).__init__(signal_interface, item)


class UnlckRemReqSts(BaseEnumSender):
    de_name     = "UnlckRemReq.Sts"
    fdx_name    = "UnlckRemReqSts"
    fdx_groupid = 3213
    class map:
       UnlckCen = 0
       UnlckStep2 = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UnlckRemReqSts, self).__init__(signal_interface, item)


class UpdInHznEdge(BaseEnumSender):
    de_name     = "HznEdge.Upd"
    fdx_name    = "UpdInHznEdge"
    fdx_groupid = 3215
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UpdInHznEdge, self).__init__(signal_interface, item)


class UpdInHznProfLong(BaseEnumSender):
    de_name     = "HznProfLong.Upd"
    fdx_name    = "UpdInHznProfLong"
    fdx_groupid = 3213
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UpdInHznProfLong, self).__init__(signal_interface, item)


class UpdInHznProfSho(BaseEnumSender):
    de_name     = "HznProfSho.Upd"
    fdx_name    = "UpdInHznProfSho"
    fdx_groupid = 3213
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UpdInHznProfSho, self).__init__(signal_interface, item)


class UpdInHznSeg(BaseEnumSender):
    de_name     = "HznSeg.Upd"
    fdx_name    = "UpdInHznSeg"
    fdx_groupid = 3215
    class map:
       No = 0
       Yes = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UpdInHznSeg, self).__init__(signal_interface, item)


# UsageMode Data
class UsgModSts(BaseEnumSender):
    de_name     = "VehModMngtGlbSafe1.UsgModSts"
    fdx_name    = "UsgModSts"
    fdx_groupid = 3192
    class map:
       UsgModAbdnd = 0
       UsgModInActv = 1
       UsgModCnvinc = 2
       UsgModActv = 11
       UsgModDrvg = 13


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsgModSts, self).__init__(signal_interface, item)


class UsrSeldDrvrSpprtFct(BaseEnumSender):
    de_name     = "UsrSeldDrvrSpprtFct"
    fdx_name    = "UsrSeldDrvrSpprtFct"
    fdx_groupid = 3216
    class map:
       NoReq = 0
       ACC = 1
       CC = 2
       SL = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSeldDrvrSpprtFct, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UsrSetSpdForKeySpdLimnPen(BaseEnumSender):
    de_name     = "UsrSetSpdForKeySpdLimn.Pen"
    fdx_name    = "UsrSetSpdForKeySpdLimnPen"
    fdx_groupid = 3217
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdLimnPen, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdLimnSts(BaseFloatSender):
    de_name     = "UsrSetSpdForKeySpdLimn.Sts"
    fdx_name    = "UsrSetSpdForKeySpdLimnSts"
    fdx_groupid = 3217
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdLimnSts, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn1(BaseFloatSender):
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn1"
    fdx_name    = "UsrSetSpdForKeySpdWarn1"
    fdx_groupid = 3214
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdWarn1, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn2(BaseFloatSender):
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn2"
    fdx_name    = "UsrSetSpdForKeySpdWarn2"
    fdx_groupid = 3214
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdWarn2, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn3(BaseFloatSender):
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn3"
    fdx_name    = "UsrSetSpdForKeySpdWarn3"
    fdx_groupid = 3214
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdWarn3, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn4(BaseFloatSender):
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn4"
    fdx_name    = "UsrSetSpdForKeySpdWarn4"
    fdx_groupid = 3214
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdWarn4, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn5(BaseFloatSender):
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn5"
    fdx_name    = "UsrSetSpdForKeySpdWarn5"
    fdx_groupid = 3214
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdWarn5, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn6(BaseFloatSender):
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn6"
    fdx_name    = "UsrSetSpdForKeySpdWarn6"
    fdx_groupid = 3214
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdWarn6, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UsrSetSpdForKeySpdWarnPen(BaseEnumSender):
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarnPen"
    fdx_name    = "UsrSetSpdForKeySpdWarnPen"
    fdx_groupid = 3214
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(UsrSetSpdForKeySpdWarnPen, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->1023
class Val0InHznProfSho(BaseIntegerSender):
    de_name     = "HznProfSho.Val0"
    fdx_name    = "Val0InHznProfSho"
    fdx_groupid = 3213
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Val0InHznProfSho, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->1023
class Val1InHznProfSho(BaseIntegerSender):
    de_name     = "HznProfSho.Val1"
    fdx_name    = "Val1InHznProfSho"
    fdx_groupid = 3213
    min = 0
    max = 1023


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Val1InHznProfSho, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4294967295
class ValInHznProfLong(BaseIntegerSender):
    de_name     = "HznProfLong.Val"
    fdx_name    = "ValInHznProfLong"
    fdx_groupid = 3213
    min = 0
    max = 4294967295


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(ValInHznProfLong, self).__init__(signal_interface, item)


# 0  No text
# 1  "Engine is running"
# 2  "Ignition on"
# 3  TBD
class VehActvMsgToDrvr(BaseEnumSender):
    de_name     = "VehActvMsgToDrvr"
    fdx_name    = "VehActvMsgToDrvr"
    fdx_groupid = 3197
    class map:
       NoMsg = 0
       Msg1 = 1
       Msg2 = 2
       Msg3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehActvMsgToDrvr, self).__init__(signal_interface, item)


# Unit: Volt,  Range:0->25, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class VehBattUSysU(BaseFloatSender):
    de_name     = "VehBattU.SysU"
    fdx_name    = "VehBattUSysU"
    fdx_groupid = 3198
    min    = 0
    max    = 25
    scale  = 0.1
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehBattUSysU, self).__init__(signal_interface, item)


class VehBattUSysUQf(BaseEnumSender):
    de_name     = "VehBattU.SysUQf"
    fdx_name    = "VehBattUSysUQf"
    fdx_groupid = 3198
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehBattUSysUQf, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmBlk(BaseFloatSender):
    de_name     = "VehCfgPrm.BlkIDBytePosn1"
    fdx_name    = "VehCfgPrmBlk"
    fdx_groupid = 3195
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehCfgPrmBlk, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal1(BaseFloatSender):
    de_name     = "VehCfgPrm.CCPBytePosn2"
    fdx_name    = "VehCfgPrmVal1"
    fdx_groupid = 3195
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehCfgPrmVal1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal2(BaseFloatSender):
    de_name     = "VehCfgPrm.CCPBytePosn3"
    fdx_name    = "VehCfgPrmVal2"
    fdx_groupid = 3195
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehCfgPrmVal2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal3(BaseFloatSender):
    de_name     = "VehCfgPrm.CCPBytePosn4"
    fdx_name    = "VehCfgPrmVal3"
    fdx_groupid = 3195
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehCfgPrmVal3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal4(BaseFloatSender):
    de_name     = "VehCfgPrm.CCPBytePosn5"
    fdx_name    = "VehCfgPrmVal4"
    fdx_groupid = 3195
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehCfgPrmVal4, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal5(BaseFloatSender):
    de_name     = "VehCfgPrm.CCPBytePosn6"
    fdx_name    = "VehCfgPrmVal5"
    fdx_groupid = 3195
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehCfgPrmVal5, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal6(BaseFloatSender):
    de_name     = "VehCfgPrm.CCPBytePosn7"
    fdx_name    = "VehCfgPrmVal6"
    fdx_groupid = 3195
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehCfgPrmVal6, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal7(BaseFloatSender):
    de_name     = "VehCfgPrm.CCPBytePosn8"
    fdx_name    = "VehCfgPrmVal7"
    fdx_groupid = 3195
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehCfgPrmVal7, self).__init__(signal_interface, item)


class VehDecelCtrlSetgNormSpt(BaseEnumSender):
    de_name     = "VehDecelCtrlSetg.NormSpt"
    fdx_name    = "VehDecelCtrlSetgNormSpt"
    fdx_groupid = 3215
    class map:
       Norm = 0
       Spt = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehDecelCtrlSetgNormSpt, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class VehDecelCtrlSetgPen(BaseEnumSender):
    de_name     = "VehDecelCtrlSetg.Pen"
    fdx_name    = "VehDecelCtrlSetgPen"
    fdx_groupid = 3215
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehDecelCtrlSetgPen, self).__init__(signal_interface, item)


# Unit: kg,  Range:0->10000, Resolution: (1.0*x+0.0, raw is unsigned, 14 bits )
class VehM(BaseFloatSender):
    de_name     = "VehMNom.VehM"
    fdx_name    = "VehM"
    fdx_groupid = 3240
    min    = 0
    max    = 10000
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehM, self).__init__(signal_interface, item)


class VehMNomTrlrM(BaseEnumSender):
    de_name     = "VehMNom.TrlrM"
    fdx_name    = "VehMNomTrlrM"
    fdx_groupid = 3240
    class map:
       Lvl0 = 0
       Lvl1 = 1
       Lvl2 = 2
       Lvl3 = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehMNomTrlrM, self).__init__(signal_interface, item)


# Main usage shall be to indicate quality for vehicle state estimations, not to be used for failure code setting and function degradation unless absolutely necassary.
class VehMQly(BaseEnumSender):
    de_name     = "VehMNom.VehMQly"
    fdx_name    = "VehMQly"
    fdx_groupid = 3240
    class map:
       Flt = 0
       NoInfo = 1
       Vld = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehMQly, self).__init__(signal_interface, item)


class VehMtnStSafe(BaseEnumSender):
    de_name     = "VehMtnStSafe.VehMtnSt"
    fdx_name    = "VehMtnStSafe"
    fdx_groupid = 3230
    class map:
       Ukwn = 0
       StandStillVal1 = 1
       StandStillVal2 = 2
       StandStillVal3 = 3
       RollgFwdVal1 = 4
       RollgFwdVal2 = 5
       RollgBackwVal1 = 6
       RollgBackwVal2 = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehMtnStSafe, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->511
class VehSpdAvg(BaseIntegerSender):
    de_name     = "VehSpdAvgIndcd.VehSpdIndcd"
    fdx_name    = "VehSpdAvg"
    fdx_groupid = 3207
    min = 0
    max = 511


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehSpdAvg, self).__init__(signal_interface, item)


class VehSpdAvgUnit(BaseEnumSender):
    de_name     = "VehSpdAvgIndcd.VeSpdIndcdUnit"
    fdx_name    = "VehSpdAvgUnit"
    fdx_groupid = 3207
    class map:
       Kmph = 0
       Mph = 1
       UkwnUnit = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehSpdAvgUnit, self).__init__(signal_interface, item)


class VehSpdCtrlActvSts(BaseEnumSender):
    de_name     = "VehSpdCtrlActvSts"
    fdx_name    = "VehSpdCtrlActvSts"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehSpdCtrlActvSts, self).__init__(signal_interface, item)


class VehSpdIndcdUnit(BaseEnumSender):
    de_name     = "VehSpdIndcd.VeSpdIndcdUnit"
    fdx_name    = "VehSpdIndcdUnit"
    fdx_groupid = 3204
    class map:
       Kmph = 0
       Mph = 1
       UkwnUnit = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehSpdIndcdUnit, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->511
class VehSpdIndcdVal(BaseIntegerSender):
    de_name     = "VehSpdIndcd.VehSpdIndcd"
    fdx_name    = "VehSpdIndcdVal"
    fdx_groupid = 3204
    min = 0
    max = 511


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehSpdIndcdVal, self).__init__(signal_interface, item)


# Unit: m/s,  Range:0->125, Resolution: (0.00391*x+0.0, raw is unsigned, 15 bits )
class VehSpdLgtSafe(BaseFloatSender):
    de_name     = "VehSpdLgtSafe.VehSpdLgt"
    fdx_name    = "VehSpdLgtSafe"
    fdx_groupid = 3230
    min    = 0
    max    = 125
    scale  = 0.00391
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehSpdLgtSafe, self).__init__(signal_interface, item)


class VehSpdLgtSafeQf(BaseEnumSender):
    de_name     = "VehSpdLgtSafe.VehSpdLgtQf"
    fdx_name    = "VehSpdLgtSafeQf"
    fdx_groupid = 3230
    class map:
       UndefindDataAccur = 0
       TmpUndefdData = 1
       DataAccurNotWithinSpcn = 2
       AccurData = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VehSpdLgtSafeQf, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp1(BaseIntegerSender):
    de_name     = "VFCVectorIHU.Grp1"
    fdx_name    = "VFCVectorIHUGrp1"
    fdx_groupid = 3211
    min = 0
    max = 65535


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VFCVectorIHUGrp1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp2(BaseIntegerSender):
    de_name     = "VFCVectorIHU.Grp2"
    fdx_name    = "VFCVectorIHUGrp2"
    fdx_groupid = 3211
    min = 0
    max = 65535


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VFCVectorIHUGrp2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp3(BaseIntegerSender):
    de_name     = "VFCVectorIHU.Grp3"
    fdx_name    = "VFCVectorIHUGrp3"
    fdx_groupid = 3211
    min = 0
    max = 65535


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VFCVectorIHUGrp3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinBlk(BaseFloatSender):
    de_name     = "Vin.BlockNr"
    fdx_name    = "VinBlk"
    fdx_groupid = 3198
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VinBlk, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn1(BaseFloatSender):
    de_name     = "Vin.VINSignalPos1"
    fdx_name    = "VinPosn1"
    fdx_groupid = 3198
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VinPosn1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn2(BaseFloatSender):
    de_name     = "Vin.VINSignalPos2"
    fdx_name    = "VinPosn2"
    fdx_groupid = 3198
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VinPosn2, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn3(BaseFloatSender):
    de_name     = "Vin.VINSignalPos3"
    fdx_name    = "VinPosn3"
    fdx_groupid = 3198
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VinPosn3, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn4(BaseFloatSender):
    de_name     = "Vin.VINSignalPos4"
    fdx_name    = "VinPosn4"
    fdx_groupid = 3198
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VinPosn4, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn5(BaseFloatSender):
    de_name     = "Vin.VINSignalPos5"
    fdx_name    = "VinPosn5"
    fdx_groupid = 3198
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VinPosn5, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn6(BaseFloatSender):
    de_name     = "Vin.VINSignalPos6"
    fdx_name    = "VinPosn6"
    fdx_groupid = 3198
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VinPosn6, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn7(BaseFloatSender):
    de_name     = "Vin.VINSignalPos7"
    fdx_name    = "VinPosn7"
    fdx_groupid = 3198
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VinPosn7, self).__init__(signal_interface, item)


class VisnAgWideCfgCtraInfo(BaseEnumSender):
    de_name     = "VisnAgWideCfg.CtraInfo"
    fdx_name    = "VisnAgWideCfgCtraInfo"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgCtraInfo, self).__init__(signal_interface, item)


class VisnAgWideCfgDstInfoFrnt(BaseEnumSender):
    de_name     = "VisnAgWideCfg.DstInfoFrnt"
    fdx_name    = "VisnAgWideCfgDstInfoFrnt"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgDstInfoFrnt, self).__init__(signal_interface, item)


class VisnAgWideCfgDstInfoRe(BaseEnumSender):
    de_name     = "VisnAgWideCfg.DstInfoRe"
    fdx_name    = "VisnAgWideCfgDstInfoRe"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgDstInfoRe, self).__init__(signal_interface, item)


class VisnAgWideCfgDstInfoSide(BaseEnumSender):
    de_name     = "VisnAgWideCfg.DstInfoSide"
    fdx_name    = "VisnAgWideCfgDstInfoSide"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgDstInfoSide, self).__init__(signal_interface, item)


class VisnAgWideCfgObjDetn(BaseEnumSender):
    de_name     = "VisnAgWideCfg.ObjDetn"
    fdx_name    = "VisnAgWideCfgObjDetn"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgObjDetn, self).__init__(signal_interface, item)


class VisnAgWideCfgSteerPahFrnt(BaseEnumSender):
    de_name     = "VisnAgWideCfg.SteerPahFrnt"
    fdx_name    = "VisnAgWideCfgSteerPahFrnt"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgSteerPahFrnt, self).__init__(signal_interface, item)


class VisnAgWideCfgSteerPahRe(BaseEnumSender):
    de_name     = "VisnAgWideCfg.SteerPahRe"
    fdx_name    = "VisnAgWideCfgSteerPahRe"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgSteerPahRe, self).__init__(signal_interface, item)


class VisnAgWideCfgSteerPahSide(BaseEnumSender):
    de_name     = "VisnAgWideCfg.SteerPahSide"
    fdx_name    = "VisnAgWideCfgSteerPahSide"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgSteerPahSide, self).__init__(signal_interface, item)


class VisnAgWideCfgTwbrPah(BaseEnumSender):
    de_name     = "VisnAgWideCfg.TwbrPah"
    fdx_name    = "VisnAgWideCfgTwbrPah"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideCfgTwbrPah, self).__init__(signal_interface, item)


# Availability status
# 0= not available
# 1= available
class VisnAgWideSts(BaseEnumSender):
    de_name     = "VisnAgWideSts"
    fdx_name    = "VisnAgWideSts"
    fdx_groupid = 3187
    class map:
       NotAvl = 0
       Avl = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnAgWideSts, self).__init__(signal_interface, item)


class VisnImgAgWideInUse(BaseEnumSender):
    de_name     = "VisnImgAgWideInUse"
    fdx_name    = "VisnImgAgWideInUse"
    fdx_groupid = 3188
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnImgAgWideInUse, self).__init__(signal_interface, item)


class VisnImgAgWideReq(BaseEnumSender):
    de_name     = "VisnImgAgWideReq"
    fdx_name    = "VisnImgAgWideReq"
    fdx_groupid = 3222
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnImgAgWideReq, self).__init__(signal_interface, item)


class VisnImgCamReqCamFrnt(BaseEnumSender):
    de_name     = "VisnImgCamReq.CamFrnt"
    fdx_name    = "VisnImgCamReqCamFrnt"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnImgCamReqCamFrnt, self).__init__(signal_interface, item)


class VisnImgCamReqCamLe(BaseEnumSender):
    de_name     = "VisnImgCamReq.CamLe"
    fdx_name    = "VisnImgCamReqCamLe"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnImgCamReqCamLe, self).__init__(signal_interface, item)


class VisnImgCamReqCamRe(BaseEnumSender):
    de_name     = "VisnImgCamReq.CamRe"
    fdx_name    = "VisnImgCamReqCamRe"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnImgCamReqCamRe, self).__init__(signal_interface, item)


class VisnImgCamReqCamRi(BaseEnumSender):
    de_name     = "VisnImgCamReq.CamRi"
    fdx_name    = "VisnImgCamReqCamRi"
    fdx_groupid = 3222
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VisnImgCamReqCamRi, self).__init__(signal_interface, item)


# Gallon 1 = UK gallons
# Gallon 2 = US gallons
class VolUnit(BaseEnumSender):
    de_name     = "IndcnUnit.VolUnit"
    fdx_name    = "VolUnit"
    fdx_groupid = 3211
    class map:
       litre = 0
       gallon1 = 1
       gallon2 = 2
       UkwnUnit = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(VolUnit, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class WarnTypForLaneChgWarnPen(BaseEnumSender):
    de_name     = "WarnTypForLaneChgWarn.Pen"
    fdx_name    = "WarnTypForLaneChgWarnPen"
    fdx_groupid = 3213
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WarnTypForLaneChgWarnPen, self).__init__(signal_interface, item)


class WarnTypForLaneChgWarnSts(BaseEnumSender):
    de_name     = "WarnTypForLaneChgWarn.Sts"
    fdx_name    = "WarnTypForLaneChgWarnSts"
    fdx_groupid = 3213
    class map:
       Sound = 0
       Hptc = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WarnTypForLaneChgWarnSts, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class WarnTypForLaneKeepAidPen(BaseEnumSender):
    de_name     = "WarnTypForLaneKeepAid.Pen"
    fdx_name    = "WarnTypForLaneKeepAidPen"
    fdx_groupid = 3212
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WarnTypForLaneKeepAidPen, self).__init__(signal_interface, item)


class WarnTypForLaneKeepAidSts(BaseEnumSender):
    de_name     = "WarnTypForLaneKeepAid.Sts"
    fdx_name    = "WarnTypForLaneKeepAidSts"
    fdx_groupid = 3212
    class map:
       Sound = 0
       Hptc = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WarnTypForLaneKeepAidSts, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->4095
class WhlCircum(BaseIntegerSender):
    de_name     = "WhlCircum"
    fdx_name    = "WhlCircum"
    fdx_groupid = 3194
    min = 0
    max = 4095


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlCircum, self).__init__(signal_interface, item)


# Unit: rpm,  Range:-1638.4->1638.3, Resolution: (0.1*x+-1638.4, raw is unsigned, 0 bits )
class WhlMotSysSpdAct(BaseFloatSender):
    de_name     = "WhlMotSysSpdAct"
    fdx_name    = "WhlMotSysSpdAct"
    fdx_groupid = 3231
    min    = -1638.4
    max    = 1638.3
    scale  = 0.1
    offset = -1638.4


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlMotSysSpdAct, self).__init__(signal_interface, item)


# Unit: NewtonMeter,  Range:-8188->8192, Resolution: (4.0*x+-8188.0, raw is unsigned, 12 bits )
class WhlMotSysTqEst(BaseFloatSender):
    de_name     = "WhlMotSysTqEst.TqAct"
    fdx_name    = "WhlMotSysTqEst"
    fdx_groupid = 3232
    min    = -8188
    max    = 8192
    scale  = 4.0
    offset = -8188.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlMotSysTqEst, self).__init__(signal_interface, item)


class WhlMotSysTqEstQlyFac(BaseEnumSender):
    de_name     = "WhlMotSysTqEst.TqActQlyFac"
    fdx_name    = "WhlMotSysTqEstQlyFac"
    fdx_groupid = 3232
    class map:
       QfUndefd = 0
       QfInProgs = 1
       QfNotSpc = 2
       QfSnsrDataOk = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlMotSysTqEstQlyFac, self).__init__(signal_interface, item)


# Unit: rpm,  Range:0->2550, Resolution: (10.0*x+0.0, raw is unsigned, 8 bits )
class WhlMotSysTqEstSpdLimn(BaseFloatSender):
    de_name     = "WhlMotSysTqEst.SpdLimn"
    fdx_name    = "WhlMotSysTqEstSpdLimn"
    fdx_groupid = 3232
    min    = 0
    max    = 2550
    scale  = 10.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlMotSysTqEstSpdLimn, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrFrntLe(BaseFloatSender):
    de_name     = "WhlRotToothCntr.WhlRotToothCntrFrntLe"
    fdx_name    = "WhlRotToothCntrFrntLe"
    fdx_groupid = 3231
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlRotToothCntrFrntLe, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrFrntRi(BaseFloatSender):
    de_name     = "WhlRotToothCntr.WhlRotToothCntrFrntRi"
    fdx_name    = "WhlRotToothCntrFrntRi"
    fdx_groupid = 3231
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlRotToothCntrFrntRi, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrReLe(BaseFloatSender):
    de_name     = "WhlRotToothCntr.WhlRotToothCntrReLe"
    fdx_name    = "WhlRotToothCntrReLe"
    fdx_groupid = 3231
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlRotToothCntrReLe, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrReRi(BaseFloatSender):
    de_name     = "WhlRotToothCntr.WhlRotToothCntrReRi"
    fdx_name    = "WhlRotToothCntrReRi"
    fdx_groupid = 3231
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WhlRotToothCntrReRi, self).__init__(signal_interface, item)


class WinPosnStsAtDrvrRe(BaseEnumSender):
    de_name     = "WinPosnStsAtDrvrRe"
    fdx_name    = "WinPosnStsAtDrvrRe"
    fdx_groupid = 3196
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WinPosnStsAtDrvrRe, self).__init__(signal_interface, item)


class WinPosnStsDrv(BaseEnumSender):
    de_name     = "WinPosnStsAtDrv"
    fdx_name    = "WinPosnStsDrv"
    fdx_groupid = 3199
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WinPosnStsDrv, self).__init__(signal_interface, item)


class WinPosnStsPass(BaseEnumSender):
    de_name     = "WinPosnStsAtPass"
    fdx_name    = "WinPosnStsPass"
    fdx_groupid = 3199
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WinPosnStsPass, self).__init__(signal_interface, item)


class WinPosnStsRePass(BaseEnumSender):
    de_name     = "WinPosnStsAtPassRe"
    fdx_name    = "WinPosnStsRePass"
    fdx_groupid = 3197
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WinPosnStsRePass, self).__init__(signal_interface, item)


# The signal reflects the actual wiping speed or mode regardless of activation source. Both rain sensor and wiper stalk info are used to create the signal. When in interval mode the wipes are carried out in low speed.
class WipgInfoWipgSpdInfo(BaseEnumSender):
    de_name     = "WipgInfo.WipgSpdInfo"
    fdx_name    = "WipgInfoWipgSpdInfo"
    fdx_groupid = 3196
    class map:
       Off = 0
       IntlLo = 1
       IntlHi = 2
       WipgSpd4045 = 3
       WipgSpd4650 = 4
       WipgSpd5155 = 5
       WipgSpd5660 = 6
       WiprErr = 7


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WipgInfoWipgSpdInfo, self).__init__(signal_interface, item)


class WipgInfoWiprActv(BaseEnumSender):
    de_name     = "WipgInfo.WiprActv"
    fdx_name    = "WipgInfoWiprActv"
    fdx_groupid = 3196
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WipgInfoWiprActv, self).__init__(signal_interface, item)


class WipgInfoWiprInWipgAr(BaseEnumSender):
    de_name     = "WipgInfo.WiprInWipgAr"
    fdx_name    = "WipgInfoWiprInWipgAr"
    fdx_groupid = 3196
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WipgInfoWiprInWipgAr, self).__init__(signal_interface, item)


class WiprFrntSrvModReq(BaseEnumSender):
    de_name     = "WiprFrntSrvModReq"
    fdx_name    = "WiprFrntSrvModReq"
    fdx_groupid = 3214
    class map:
       NoActn = 0
       ActvtSrvPosn = 1
       DeActvtSrvPosn = 2


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WiprFrntSrvModReq, self).__init__(signal_interface, item)


class WiprInPosnForSrv(BaseEnumSender):
    de_name     = "WiprInPosnForSrv"
    fdx_name    = "WiprInPosnForSrv"
    fdx_groupid = 3199
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WiprInPosnForSrv, self).__init__(signal_interface, item)


# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
#
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class WiprReAutReqPen(BaseEnumSender):
    de_name     = "WiprReAutReq.Pen"
    fdx_name    = "WiprReAutReqPen"
    fdx_groupid = 3217
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


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WiprReAutReqPen, self).__init__(signal_interface, item)


class WiprReAutReqSts(BaseEnumSender):
    de_name     = "WiprReAutReq.Sts"
    fdx_name    = "WiprReAutReqSts"
    fdx_groupid = 3217
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WiprReAutReqSts, self).__init__(signal_interface, item)


class WshrFldSts(BaseEnumSender):
    de_name     = "WshrFldSts1WdElmHMI"
    fdx_name    = "WshrFldSts"
    fdx_groupid = 3199
    class map:
       Off = 0
       On = 1


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(WshrFldSts, self).__init__(signal_interface, item)


# Unit: rad/s,  Range:-6->6, Resolution: (0.000244140625*x+0.0, raw is signed, 16 bits )
class YawRate1(BaseFloatSender):
    de_name     = "AgDataRawSafe.YawRate"
    fdx_name    = "YawRate1"
    fdx_groupid = 3224
    min    = -6
    max    = 6
    scale  = 0.000244140625
    offset = 0.0


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(YawRate1, self).__init__(signal_interface, item)


class YawRate1Qf1(BaseEnumSender):
    de_name     = "AgDataRawSafe.YawRateQf"
    fdx_name    = "YawRate1Qf1"
    fdx_groupid = 3224
    class map:
       DevOfDataUndefd = 0
       DataTmpUndefdAndEvlnInProgs = 1
       DevOfDataNotWithinRngAllwd = 2
       DataCalcdWithDevDefd = 3


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(YawRate1Qf1, self).__init__(signal_interface, item)


# Unit: Year,  Range:0->99
class Yr(BaseIntegerSender):
    de_name     = "TiAndDateIndcn.Yr1"
    fdx_name    = "Yr"
    fdx_groupid = 3197
    min = 0
    max = 99


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Yr, self).__init__(signal_interface, item)


# Unit: Year,  Range:0->99
class Yr1(BaseIntegerSender):
    de_name     = "SetTiAndDate.Year"
    fdx_name    = "Yr1"
    fdx_groupid = 3214
    min = 0
    max = 99


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(Yr1, self).__init__(signal_interface, item)


# Unit: Unitless,  Range:0->63
class YrVersOfMapInHznData(BaseIntegerSender):
    de_name     = "HznData.YrVersOfMap"
    fdx_name    = "YrVersOfMapInHznData"
    fdx_groupid = 3214
    min = 0
    max = 63


    def __init__(self, signal_interface, item):
        # type: (FrSignalInterface, fdx_description_file_parser.Item) -> None
        super(YrVersOfMapInHznData, self).__init__(signal_interface, item)

