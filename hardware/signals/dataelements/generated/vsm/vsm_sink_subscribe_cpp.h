// clang-format off
/*!
 * \file
 * C++ code generator for AUTOSAR v1.0
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved. Delphi Confidential
 * Source: databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml
 */

AccAdprSpdLimActv_de = new DESink<autosar::AccAdprSpdLimActv_info>();
AccAdprSpdLimActv_de->subscribe([]() {
    auto deValue = AccAdprSpdLimActv_de->get().value();
    AccAdprSpdLimActv rteValue;
    rteValue.AccAdprSpdLimActvPen = static_cast<decltype(rteValue.AccAdprSpdLimActvPen)>(deValue.Pen);
    rteValue.AccAdprSpdLimActvSts = static_cast<decltype(rteValue.AccAdprSpdLimActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igAccAdprSpdLimActv_mtx, &rteValue, sizeof(rteValue) ); 
});

AccAdprTurnSpdActv_de = new DESink<autosar::AccAdprTurnSpdActv_info>();
AccAdprTurnSpdActv_de->subscribe([]() {
    auto deValue = AccAdprTurnSpdActv_de->get().value();
    AccAdprTurnSpdActv rteValue;
    rteValue.AccAdprTurnSpdActvPen = static_cast<decltype(rteValue.AccAdprTurnSpdActvPen)>(deValue.Pen);
    rteValue.AccAdprTurnSpdActvSts = static_cast<decltype(rteValue.AccAdprTurnSpdActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igAccAdprTurnSpdActv_mtx, &rteValue, sizeof(rteValue) ); 
});

ActvnOfCrsEco_de = new DESink<autosar::ActvnOfCrsEco_info>();
ActvnOfCrsEco_de->subscribe([]() {
    auto deValue = ActvnOfCrsEco_de->get().value();
    ActvnOfCrsEco rteValue;
    rteValue.ActvnOfCrsEcoPen = static_cast<decltype(rteValue.ActvnOfCrsEcoPen)>(deValue.Pen);
    rteValue.ActvnOfCrsEcoSts = static_cast<decltype(rteValue.ActvnOfCrsEcoSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igActvnOfCrsEco_mtx, &rteValue, sizeof(rteValue) ); 
});

ActvnOfPrkgAssi_de = new DESink<autosar::ActvnOfPrkgAssi_info>();
ActvnOfPrkgAssi_de->subscribe([]() {
    auto deValue = ActvnOfPrkgAssi_de->get().value();
    ActvnOfPrkgAssi rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isActvnOfPrkgAssi_mtx, &rteValue, sizeof(rteValue) ); 
});

ActvnOfPrkgAut_de = new DESink<autosar::ActvnOfPrkgAut_info>();
ActvnOfPrkgAut_de->subscribe([]() {
    auto deValue = ActvnOfPrkgAut_de->get().value();
    ActvnOfPrkgAut rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isActvnOfPrkgAut_mtx, &rteValue, sizeof(rteValue) ); 
});

ActvnOfSwtIllmnCen_de = new DESink<autosar::ActvnOfSwtIllmnCen_info>();
ActvnOfSwtIllmnCen_de->subscribe([]() {
    auto deValue = ActvnOfSwtIllmnCen_de->get().value();
    ActvnOfSwtIllmnCen rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isActvnOfSwtIllmnCen_mtx, &rteValue, sizeof(rteValue) ); 
});

ActvnOfSwtIllmnClima_de = new DESink<autosar::ActvnOfSwtIllmnClima_info>();
ActvnOfSwtIllmnClima_de->subscribe([]() {
    auto deValue = ActvnOfSwtIllmnClima_de->get().value();
    ActvnOfSwtIllmnClima rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isActvnOfSwtIllmnClima_mtx, &rteValue, sizeof(rteValue) ); 
});

ActvnOfSwtIllmnDrvMod_de = new DESink<autosar::ActvnOfSwtIllmnDrvMod_info>();
ActvnOfSwtIllmnDrvMod_de->subscribe([]() {
    auto deValue = ActvnOfSwtIllmnDrvMod_de->get().value();
    ActvnOfSwtIllmnDrvMod rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isActvnOfSwtIllmnDrvMod_mtx, &rteValue, sizeof(rteValue) ); 
});

ActvnOfSwtIllmnForSeatHeatrRe_de = new DESink<autosar::ActvnOfSwtIllmnForSeatHeatrRe_info>();
ActvnOfSwtIllmnForSeatHeatrRe_de->subscribe([]() {
    auto deValue = ActvnOfSwtIllmnForSeatHeatrRe_de->get().value();
    ActvnOfSwtIllmnForSeatHeatrRe rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isActvnOfSwtIllmnForSeatHeatrRe_mtx, &rteValue, sizeof(rteValue) ); 
});

AdprTurnSpdMod_de = new DESink<autosar::AdprTurnSpdMod_info>();
AdprTurnSpdMod_de->subscribe([]() {
    auto deValue = AdprTurnSpdMod_de->get().value();
    AdprTurnSpdMod rteValue;
    rteValue.AdprTurnSpdModPen = static_cast<decltype(rteValue.AdprTurnSpdModPen)>(deValue.Pen);
    rteValue.AdprTurnSpdModSts = static_cast<decltype(rteValue.AdprTurnSpdModSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igAdprTurnSpdMod_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtReq1WdReq5_de = new DESink<autosar::LiExtReq1WdReq5_info>();
LiExtReq1WdReq5_de->subscribe([]() {
    auto deValue = LiExtReq1WdReq5_de->get().value();
    LiExtReq1WdReq5 rteValue;
    rteValue.LiExtReq1WdReq5IdPen = static_cast<decltype(rteValue.LiExtReq1WdReq5IdPen)>(deValue.IdPen_);
    rteValue.LiExtReq1WdReq5SlowNormFast = static_cast<decltype(rteValue.LiExtReq1WdReq5SlowNormFast)>(deValue.SlowNormFast_);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiExtReq1WdReq5_mtx, &rteValue, sizeof(rteValue) ); 
});

AirClngReq_de = new DESink<autosar::AirClngReq_info>();
AirClngReq_de->subscribe([]() {
    auto deValue = AirClngReq_de->get().value();
    AirClngReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isAirClngReq_mtx, &rteValue, sizeof(rteValue) ); 
});

AntithftRednReq_de = new DESink<autosar::AntithftRednReq_info>();
AntithftRednReq_de->subscribe([]() {
    auto deValue = AntithftRednReq_de->get().value();
    AntithftRednReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isAntithftRednReq_mtx, &rteValue, sizeof(rteValue) ); 
});

AudWarnActv_de = new DESink<autosar::AudWarnActv_info>();
AudWarnActv_de->subscribe([]() {
    auto deValue = AudWarnActv_de->get().value();
    AudWarnActv rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isAudWarnActv_mtx, &rteValue, sizeof(rteValue) ); 
});

BlisSwOnOff_de = new DESink<autosar::BlisSwOnOff_info>();
BlisSwOnOff_de->subscribe([]() {
    auto deValue = BlisSwOnOff_de->get().value();
    BlisSwOnOff rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isBlisSwOnOff_mtx, &rteValue, sizeof(rteValue) ); 
});

BtnIllmnForWinDefrstFrnt_de = new DESink<autosar::BtnIllmnForWinDefrstFrnt_info>();
BtnIllmnForWinDefrstFrnt_de->subscribe([]() {
    auto deValue = BtnIllmnForWinDefrstFrnt_de->get().value();
    BtnIllmnForWinDefrstFrnt rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isBtnIllmnForWinDefrstFrnt_mtx, &rteValue, sizeof(rteValue) ); 
});

CallSts_de = new DESink<autosar::CallSts_info>();
CallSts_de->subscribe([]() {
    auto deValue = CallSts_de->get().value();
    CallStsIndcn rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isCallStsIndcn_mtx, &rteValue, sizeof(rteValue) ); 
});

ClimaRqrdFromHmi_de = new DESink<autosar::ClimaRqrdFromHmi_info>();
ClimaRqrdFromHmi_de->subscribe([]() {
    auto deValue = ClimaRqrdFromHmi_de->get().value();
    ClimaRqrd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isClimaRqrd_mtx, &rteValue, sizeof(rteValue) ); 
});

ClimaTmr_de = new DESink<autosar::ClimaTmr_info>();
ClimaTmr_de->subscribe([]() {
    auto deValue = ClimaTmr_de->get().value();
    ClimaTmr rteValue;
    rteValue = round((deValue - 0.0) / 0.1);
    sendAvmpMessageToVip( ComConf_ComSignal_isClimaTmr_mtx, &rteValue, sizeof(rteValue) ); 
});

ClimaTmrStsRqrd_de = new DESink<autosar::ClimaTmrStsRqrd_info>();
ClimaTmrStsRqrd_de->subscribe([]() {
    auto deValue = ClimaTmrStsRqrd_de->get().value();
    ClimaTmrStsRqrd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isClimaTmrStsRqrd_mtx, &rteValue, sizeof(rteValue) ); 
});

CllsnAidSnvtySeld_de = new DESink<autosar::CllsnAidSnvtySeld_info>();
CllsnAidSnvtySeld_de->subscribe([]() {
    auto deValue = CllsnAidSnvtySeld_de->get().value();
    CllsnAidSnvtySeld rteValue;
    rteValue.CllsnAidSnvtySeldPen = static_cast<decltype(rteValue.CllsnAidSnvtySeldPen)>(deValue.Pen);
    rteValue.CllsnAidSnvtySeldSts = static_cast<decltype(rteValue.CllsnAidSnvtySeldSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igCllsnAidSnvtySeld_mtx, &rteValue, sizeof(rteValue) ); 
});

CllsnFwdWarnActv_de = new DESink<autosar::CllsnFwdWarnActv_info>();
CllsnFwdWarnActv_de->subscribe([]() {
    auto deValue = CllsnFwdWarnActv_de->get().value();
    CllsnFwdWarnActv rteValue;
    rteValue.CllsnFwdWarnActvPen = static_cast<decltype(rteValue.CllsnFwdWarnActvPen)>(deValue.Pen);
    rteValue.CllsnFwdWarnActvSts = static_cast<decltype(rteValue.CllsnFwdWarnActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igCllsnFwdWarnActv_mtx, &rteValue, sizeof(rteValue) ); 
});

ClngRqrdFromHmi_de = new DESink<autosar::ClngRqrdFromHmi_info>();
ClngRqrdFromHmi_de->subscribe([]() {
    auto deValue = ClngRqrdFromHmi_de->get().value();
    ClngRqrdFromHmi rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isClngRqrdFromHmi_mtx, &rteValue, sizeof(rteValue) ); 
});

ClsAutEna_de = new DESink<autosar::ClsAutEna_info>();
ClsAutEna_de->subscribe([]() {
    auto deValue = ClsAutEna_de->get().value();
    ClsAutEna rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isClsAutEna_mtx, &rteValue, sizeof(rteValue) ); 
});

CmftFctActv_de = new DESink<autosar::CmftFctActv_info>();
CmftFctActv_de->subscribe([]() {
    auto deValue = CmftFctActv_de->get().value();
    CmftFctActv rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isCmftFctActv_mtx, &rteValue, sizeof(rteValue) ); 
});

CoolgReqForDispCen_de = new DESink<autosar::CoolgReqForDispCen_info>();
CoolgReqForDispCen_de->subscribe([]() {
    auto deValue = CoolgReqForDispCen_de->get().value();
    CoolgReqForDispCen rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isCoolgReqForDispCen_mtx, &rteValue, sizeof(rteValue) ); 
});

CrvtSpdEstimrSnvtySeld_de = new DESink<autosar::CrvtSpdEstimrSnvtySeld_info>();
CrvtSpdEstimrSnvtySeld_de->subscribe([]() {
    auto deValue = CrvtSpdEstimrSnvtySeld_de->get().value();
    CrvtSpdEstimrSnvtySeld rteValue;
    rteValue.TurnSpdWarnSnvtyActPen = static_cast<decltype(rteValue.TurnSpdWarnSnvtyActPen)>(deValue.Pen);
    rteValue.TurnSpdWarnSnvtyActsSts = static_cast<decltype(rteValue.TurnSpdWarnSnvtyActsSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igCrvtSpdEstimrSnvtySeld_mtx, &rteValue, sizeof(rteValue) ); 
});

CrvtSpdWarnActv_de = new DESink<autosar::CrvtSpdWarnActv_info>();
CrvtSpdWarnActv_de->subscribe([]() {
    auto deValue = CrvtSpdWarnActv_de->get().value();
    CrvtSpdWarnActv rteValue;
    rteValue.TurnSpdWarnActvPen = static_cast<decltype(rteValue.TurnSpdWarnActvPen)>(deValue.Pen);
    rteValue.TurnSpdWarnActvSts = static_cast<decltype(rteValue.TurnSpdWarnActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igCrvtSpdWarnActv_mtx, &rteValue, sizeof(rteValue) ); 
});

CtraSwOnOff_de = new DESink<autosar::CtraSwOnOff_info>();
CtraSwOnOff_de->subscribe([]() {
    auto deValue = CtraSwOnOff_de->get().value();
    CtraSwOnOff rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isCtraSwOnOff_mtx, &rteValue, sizeof(rteValue) ); 
});

CurtActvnReReq_de = new DESink<autosar::CurtActvnReReq_info>();
CurtActvnReReq_de->subscribe([]() {
    auto deValue = CurtActvnReReq_de->get().value();
    CurtActvnReReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isCurtActvnReReq_mtx, &rteValue, sizeof(rteValue) ); 
});

DeactvtLvlgCtrl_de = new DESink<autosar::DeactvtLvlgCtrl_info>();
DeactvtLvlgCtrl_de->subscribe([]() {
    auto deValue = DeactvtLvlgCtrl_de->get().value();
    DeactvtLvlgCtrl rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDeactvtLvlgCtrl_mtx, &rteValue, sizeof(rteValue) ); 
});

DispAndHomeBtnSts_de = new DESink<autosar::DispAndHomeBtnSts_info>();
DispAndHomeBtnSts_de->subscribe([]() {
    auto deValue = DispAndHomeBtnSts_de->get().value();
    DispAndHomeBtnSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDispAndHomeBtnSts_mtx, &rteValue, sizeof(rteValue) ); 
});

DispStsCen_de = new DESink<autosar::DispStsCen_info>();
DispStsCen_de->subscribe([]() {
    auto deValue = DispStsCen_de->get().value();
    DispStsCen rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDispStsCen_mtx, &rteValue, sizeof(rteValue) ); 
});

DriveAwayInfoActvReq_de = new DESink<autosar::DriveAwayInfoActvReq_info>();
DriveAwayInfoActvReq_de->subscribe([]() {
    auto deValue = DriveAwayInfoActvReq_de->get().value();
    DriveAwayInfoActvReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDriveAwayInfoActvReq_mtx, &rteValue, sizeof(rteValue) ); 
});

DriveAwayInfoSoundWarnActvReq_de = new DESink<autosar::DriveAwayInfoSoundWarnActvReq_info>();
DriveAwayInfoSoundWarnActvReq_de->subscribe([]() {
    auto deValue = DriveAwayInfoSoundWarnActvReq_de->get().value();
    DriveAwayInfoSoundWarnActvReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDriveAwayInfoSoundWarnActvReq_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvModDispd_de = new DESink<autosar::DrvModDispd_info>();
DrvModDispd_de->subscribe([]() {
    auto deValue = DrvModDispd_de->get().value();
    DrvModDispd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDrvModDispd_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvModReq_de = new DESink<autosar::DrvModReq_info>();
DrvModReq_de->subscribe([]() {
    auto deValue = DrvModReq_de->get().value();
    DrvModReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDrvModReq_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvgInWrgDirOfTrfc_de = new DESink<autosar::DrvgInWrgDirOfTrfc_info>();
DrvgInWrgDirOfTrfc_de->subscribe([]() {
    auto deValue = DrvgInWrgDirOfTrfc_de->get().value();
    DrvgInWrgDirOfTrfc rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDrvgInWrgDirOfTrfc_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrCtrlOfPassSeatFrntReqd_de = new DESink<autosar::DrvrCtrlOfPassSeatFrntReqd_info>();
DrvrCtrlOfPassSeatFrntReqd_de->subscribe([]() {
    auto deValue = DrvrCtrlOfPassSeatFrntReqd_de->get().value();
    DrvrCtrlOfPassSeatFrntReqd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDrvrCtrlOfPassSeatFrntReqd_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrDispSetg_de = new DESink<autosar::DrvrDispSetg_info>();
DrvrDispSetg_de->subscribe([]() {
    auto deValue = DrvrDispSetg_de->get().value();
    DrvrDispSetg rteValue;
    rteValue.DrvrDispSetgPen = static_cast<decltype(rteValue.DrvrDispSetgPen)>(deValue.Pen);
    rteValue.DrvrDispSetgSts = static_cast<decltype(rteValue.DrvrDispSetgSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDrvrDispSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrEntryLoReq_de = new DESink<autosar::DrvrEntryLoReq_info>();
DrvrEntryLoReq_de->subscribe([]() {
    auto deValue = DrvrEntryLoReq_de->get().value();
    DrvrEntryLoReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDrvrEntryLoReq_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrHmiBackGndForHmiCen_de = new DESink<autosar::DrvrHmiBackGndForHmiCen_info>();
DrvrHmiBackGndForHmiCen_de->subscribe([]() {
    auto deValue = DrvrHmiBackGndForHmiCen_de->get().value();
    DrvrHmiBackGndForHmiCen rteValue;
    for (unsigned int i=0; i<deValue.size(); ++i) rteValue[i] = static_cast<std::remove_reference<decltype( *rteValue )>::type>( deValue[i] );
    sendAvmpMessageToVip( ComConf_ComSignal_isDrvrHmiBackGndForHmiCen_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrHmiBackGndInfoSetg_de = new DESink<autosar::DrvrHmiBackGndInfoSetg_info>();
DrvrHmiBackGndInfoSetg_de->subscribe([]() {
    auto deValue = DrvrHmiBackGndInfoSetg_de->get().value();
    DrvrHmiBackGndInfoSetg rteValue;
    rteValue.DrvrHmiBackGndInfoSetgPen = static_cast<decltype(rteValue.DrvrHmiBackGndInfoSetgPen)>(deValue.Pen);
    rteValue.DrvrHmiBackGndInfoSetgSetg = static_cast<decltype(rteValue.DrvrHmiBackGndInfoSetgSetg)>(deValue.Setg);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDrvrHmiBackGndInfoSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrHmiDispdMod_de = new DESink<autosar::DrvrHmiDispdMod_info>();
DrvrHmiDispdMod_de->subscribe([]() {
    auto deValue = DrvrHmiDispdMod_de->get().value();
    DrvrHmiDispdMod rteValue;
    rteValue.DrvrHmiDispdModPen = static_cast<decltype(rteValue.DrvrHmiDispdModPen)>(deValue.Pen);
    rteValue.DrvrHmiDispdModSts = static_cast<decltype(rteValue.DrvrHmiDispdModSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDrvrHmiDispdMod_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrHmiForHmiCen_de = new DESink<autosar::DrvrHmiForHmiCen_info>();
DrvrHmiForHmiCen_de->subscribe([]() {
    auto deValue = DrvrHmiForHmiCen_de->get().value();
    DrvrHmiForHmiCen rteValue;
    for (unsigned int i=0; i<deValue.size(); ++i) rteValue[i] = static_cast<std::remove_reference<decltype( *rteValue )>::type>( deValue[i] );
    sendAvmpMessageToVip( ComConf_ComSignal_isDrvrHmiForHmiCen_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrHmiTmrForHmiCen_de = new DESink<autosar::DrvrHmiTmrForHmiCen_info>();
DrvrHmiTmrForHmiCen_de->subscribe([]() {
    auto deValue = DrvrHmiTmrForHmiCen_de->get().value();
    DrvrHmiTmrForHmiCen rteValue;
    for (unsigned int i=0; i<deValue.size(); ++i) rteValue[i] = static_cast<std::remove_reference<decltype( *rteValue )>::type>( deValue[i] );
    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDrvrHmiTmrForHmiCen_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrHmiUsrIfSetg_de = new DESink<autosar::DrvrHmiUsrIfSetg_info>();
DrvrHmiUsrIfSetg_de->subscribe([]() {
    auto deValue = DrvrHmiUsrIfSetg_de->get().value();
    DrvrHmiUsrIfSetg rteValue;
    rteValue.DrvrHmiUsrIfSetgPen = static_cast<decltype(rteValue.DrvrHmiUsrIfSetgPen)>(deValue.Pen);
    rteValue.DrvrHmiUsrIfSetgSetg = static_cast<decltype(rteValue.DrvrHmiUsrIfSetgSetg)>(deValue.Setg);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDrvrHmiUsrIfSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrPfmncMonActv_de = new DESink<autosar::DrvrPfmncMonActv_info>();
DrvrPfmncMonActv_de->subscribe([]() {
    auto deValue = DrvrPfmncMonActv_de->get().value();
    DrvrPfmncMonActv rteValue;
    rteValue.DrvrPfmncMonActvPen = static_cast<decltype(rteValue.DrvrPfmncMonActvPen)>(deValue.Pen);
    rteValue.DrvrPfmncMonActvSts = static_cast<decltype(rteValue.DrvrPfmncMonActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDrvrPfmncMonActv_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrSeatDispMassgFct_de = new DESink<autosar::DrvrSeatDispMassgFct_info>();
DrvrSeatDispMassgFct_de->subscribe([]() {
    auto deValue = DrvrSeatDispMassgFct_de->get().value();
    DrvrSeatDispMassgFct rteValue;
    rteValue.DrvrSeatDispMassgFctMassgInten = static_cast<decltype(rteValue.DrvrSeatDispMassgFctMassgInten)>(deValue.MassgInten);
    rteValue.DrvrSeatDispMassgFctMassgProg = static_cast<decltype(rteValue.DrvrSeatDispMassgFctMassgProg)>(deValue.MassgProg);
    rteValue.DrvrSeatDispMassgFctMassgSpdLvl = static_cast<decltype(rteValue.DrvrSeatDispMassgFctMassgSpdLvl)>(deValue.MassgSpdLvl);
    rteValue.DrvrSeatDispMassgFctOnOff = static_cast<decltype(rteValue.DrvrSeatDispMassgFctOnOff)>(deValue.OnOff);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDrvrSeatDispMassgFct_mtx, &rteValue, sizeof(rteValue) ); 
});

DrvrSeatDispSpplFct_de = new DESink<autosar::DrvrSeatDispSpplFct_info>();
DrvrSeatDispSpplFct_de->subscribe([]() {
    auto deValue = DrvrSeatDispSpplFct_de->get().value();
    DrvrSeatDispSpplFct rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isDrvrSeatDispSpplFct_mtx, &rteValue, sizeof(rteValue) ); 
});

DstNotifActv_de = new DESink<autosar::DstNotifActv_info>();
DstNotifActv_de->subscribe([]() {
    auto deValue = DstNotifActv_de->get().value();
    DstNotifActv rteValue;
    rteValue.DstNotifActvPen = static_cast<decltype(rteValue.DstNotifActvPen)>(deValue.Pen);
    rteValue.DstNotifActvSts = static_cast<decltype(rteValue.DstNotifActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDstNotifActv_mtx, &rteValue, sizeof(rteValue) ); 
});

DstToManvLocn_de = new DESink<autosar::DstToManvLocn_info>();
DstToManvLocn_de->subscribe([]() {
    auto deValue = DstToManvLocn_de->get().value();
    DstToManvLocn rteValue;
    rteValue.CntDwnToManvStrtInDstToManvLocn = static_cast<decltype(rteValue.CntDwnToManvStrtInDstToManvLocn)>(deValue.CntDwnToManvStrt);
    rteValue.DstToManvInDstToManvLocn = static_cast<decltype(rteValue.DstToManvInDstToManvLocn)>(deValue.DstToManv);
    rteValue.PosnQlyInDstToManvLocn = static_cast<decltype(rteValue.PosnQlyInDstToManvLocn)>(deValue.PosnQly);
    rteValue.SpprtForFctInDstToManvLocn = static_cast<decltype(rteValue.SpprtForFctInDstToManvLocn)>(deValue.SpprtForFct);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igDstToManvLocn_mtx, &rteValue, sizeof(rteValue) ); 
});

EasyInOutDrvrSeatAdjmt_de = new DESink<autosar::EasyInOutDrvrSeatAdjmt_info>();
EasyInOutDrvrSeatAdjmt_de->subscribe([]() {
    auto deValue = EasyInOutDrvrSeatAdjmt_de->get().value();
    EasyInOutDrvrSeatAdjmt rteValue;
    rteValue.EasyInOutDrvrSeatAdjmtPen = static_cast<decltype(rteValue.EasyInOutDrvrSeatAdjmtPen)>(deValue.Pen);
    rteValue.EasyInOutDrvrSeatAdjmtSts = static_cast<decltype(rteValue.EasyInOutDrvrSeatAdjmtSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igEasyInOutDrvrSeatAdjmt_mtx, &rteValue, sizeof(rteValue) ); 
});

EgySave_de = new DESink<autosar::EgySave_info>();
EgySave_de->subscribe([]() {
    auto deValue = EgySave_de->get().value();
    EgySave rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isEgySave_mtx, &rteValue, sizeof(rteValue) ); 
});

EmgyVehWarnActv_de = new DESink<autosar::EmgyVehWarnActv_info>();
EmgyVehWarnActv_de->subscribe([]() {
    auto deValue = EmgyVehWarnActv_de->get().value();
    EmgyVehWarnActv rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isEmgyVehWarnActv_mtx, &rteValue, sizeof(rteValue) ); 
});

EngIdleEco1_de = new DESink<autosar::EngIdleEco1_info>();
EngIdleEco1_de->subscribe([]() {
    auto deValue = EngIdleEco1_de->get().value();
    EngIdleEco1 rteValue;
    rteValue.EngIdleEco1Pen = static_cast<decltype(rteValue.EngIdleEco1Pen)>(deValue.Pen);
    rteValue.EngIdleEco1Sts = static_cast<decltype(rteValue.EngIdleEco1Sts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igEngIdleEco1_mtx, &rteValue, sizeof(rteValue) ); 
});

EngStrtStopDrvModIndSetg_de = new DESink<autosar::EngStrtStopDrvModIndSetg_info>();
EngStrtStopDrvModIndSetg_de->subscribe([]() {
    auto deValue = EngStrtStopDrvModIndSetg_de->get().value();
    EngStrtStopDrvModIndSetg rteValue;
    rteValue.EngStrtStopDrvModIndSetgPen = static_cast<decltype(rteValue.EngStrtStopDrvModIndSetgPen)>(deValue.Pen);
    rteValue.EngStrtStopDrvModIndSetgSts = static_cast<decltype(rteValue.EngStrtStopDrvModIndSetgSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igEngStrtStopDrvModIndSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

EngStrtStopSetg_de = new DESink<autosar::EngStrtStopSetg_info>();
EngStrtStopSetg_de->subscribe([]() {
    auto deValue = EngStrtStopSetg_de->get().value();
    EngStrtStopSetg rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isEngStrtStopSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

EpbApplyAutSetg_de = new DESink<autosar::EpbApplyAutSetg_info>();
EpbApplyAutSetg_de->subscribe([]() {
    auto deValue = EpbApplyAutSetg_de->get().value();
    EpbApplyAutSetg rteValue;
    rteValue.EpbApplyAutSetgPen = static_cast<decltype(rteValue.EpbApplyAutSetgPen)>(deValue.Pen);
    rteValue.EpbApplyAutSetgSts = static_cast<decltype(rteValue.EpbApplyAutSetgSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igEpbApplyAutSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

EpbEngStallApplySetg_de = new DESink<autosar::EpbEngStallApplySetg_info>();
EpbEngStallApplySetg_de->subscribe([]() {
    auto deValue = EpbEngStallApplySetg_de->get().value();
    EpbEngStallApplySetg rteValue;
    rteValue.EpbEngStallApplySetgPen = static_cast<decltype(rteValue.EpbEngStallApplySetgPen)>(deValue.Pen);
    rteValue.EpbEngStallApplySetgSts = static_cast<decltype(rteValue.EpbEngStallApplySetgSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igEpbEngStallApplySetg_mtx, &rteValue, sizeof(rteValue) ); 
});

EscSptModReqdByDrvr_de = new DESink<autosar::EscSptModReqdByDrvr_info>();
EscSptModReqdByDrvr_de->subscribe([]() {
    auto deValue = EscSptModReqdByDrvr_de->get().value();
    EscSptModReqdByDrvrGroup rteValue;
    rteValue.EscSptModReqdByDrvr = static_cast<decltype(rteValue.EscSptModReqdByDrvr)>(deValue.EscSptModReqdByDrvr);
    rteValue.EscSptModReqdByDrvrPen = static_cast<decltype(rteValue.EscSptModReqdByDrvrPen)>(deValue.Pen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igEscSptModReqdByDrvrGroup_mtx, &rteValue, sizeof(rteValue) ); 
});

ExtrMirrFoldSetg_de = new DESink<autosar::ExtrMirrFoldSetg_info>();
ExtrMirrFoldSetg_de->subscribe([]() {
    auto deValue = ExtrMirrFoldSetg_de->get().value();
    ExtrMirrFoldSetg rteValue;
    rteValue.ExtrMirrFoldSetgPen = static_cast<decltype(rteValue.ExtrMirrFoldSetgPen)>(deValue.Pen);
    rteValue.ExtrMirrFoldSetgSts = static_cast<decltype(rteValue.ExtrMirrFoldSetgSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igExtrMirrFoldSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

ExtrMirrTiltSetg2_de = new DESink<autosar::ExtrMirrTiltSetg2_info>();
ExtrMirrTiltSetg2_de->subscribe([]() {
    auto deValue = ExtrMirrTiltSetg2_de->get().value();
    ExtrMirrTiltSetg2 rteValue;
    rteValue.ExtrMirrTiltSetg2IdPen = static_cast<decltype(rteValue.ExtrMirrTiltSetg2IdPen)>(deValue.IdPen_);
    rteValue.ExtrMirrTiltSetg2MirrDrvr = static_cast<decltype(rteValue.ExtrMirrTiltSetg2MirrDrvr)>(deValue.MirrDrvr);
    rteValue.ExtrMirrTiltSetg2MirrPass = static_cast<decltype(rteValue.ExtrMirrTiltSetg2MirrPass)>(deValue.MirrPass);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igExtrMirrTiltSetg2_mtx, &rteValue, sizeof(rteValue) ); 
});

HdHwAprvdWirelsAdr_de = new DESink<autosar::HdHwAprvdWirelsAdr_info>();
HdHwAprvdWirelsAdr_de->subscribe([]() {
    auto deValue = HdHwAprvdWirelsAdr_de->get().value();
    HdHwAprvdWirelsAdr rteValue;
    rteValue.HdHwAprvdWirelsAdrAprvdSts = static_cast<decltype(rteValue.HdHwAprvdWirelsAdrAprvdSts)>(deValue.AprvdSts_);
    rteValue.HdHwAprvdWirelsAdrHwOffsAdr1 = static_cast<decltype(rteValue.HdHwAprvdWirelsAdrHwOffsAdr1)>(deValue.HwOffsAdr1);
    rteValue.HdHwAprvdWirelsAdrHwOffsAdr2 = static_cast<decltype(rteValue.HdHwAprvdWirelsAdrHwOffsAdr2)>(deValue.HwOffsAdr2);
    rteValue.HdHwAprvdWirelsAdrHwOffsAdr3 = static_cast<decltype(rteValue.HdHwAprvdWirelsAdrHwOffsAdr3)>(deValue.HwOffsAdr3);
    rteValue.HdHwAprvdWirelsAdrHwOffsAdr4 = static_cast<decltype(rteValue.HdHwAprvdWirelsAdrHwOffsAdr4)>(deValue.HwOffsAdr4);
    rteValue.HdHwAprvdWirelsAdrHwOffsAdr5 = static_cast<decltype(rteValue.HdHwAprvdWirelsAdrHwOffsAdr5)>(deValue.HwOffsAdr5);
    rteValue.HdHwAprvdWirelsAdrHwOffsAdr6 = static_cast<decltype(rteValue.HdHwAprvdWirelsAdrHwOffsAdr6)>(deValue.HwOffsAdr6);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHdHwAprvdWirelsAdr_mtx, &rteValue, sizeof(rteValue) ); 
});

HdrestFoldReq2_de = new DESink<autosar::HdrestFoldReq2_info>();
HdrestFoldReq2_de->subscribe([]() {
    auto deValue = HdrestFoldReq2_de->get().value();
    HdrestFoldReq2 rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHdrestFoldReq2_mtx, &rteValue, sizeof(rteValue) ); 
});

HeatrDurgDrvgReqd_de = new DESink<autosar::HeatrDurgDrvgReqd_info>();
HeatrDurgDrvgReqd_de->subscribe([]() {
    auto deValue = HeatrDurgDrvgReqd_de->get().value();
    HeatrDurgDrvgReqd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHeatrDurgDrvgReqd_mtx, &rteValue, sizeof(rteValue) ); 
});

HeatrPreCdngTyp_de = new DESink<autosar::HeatrPreCdngTyp_info>();
HeatrPreCdngTyp_de->subscribe([]() {
    auto deValue = HeatrPreCdngTyp_de->get().value();
    HeatrPreCdngTyp rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHeatrPreCdngTyp_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiAudSts_de = new DESink<autosar::HmiAudSts_info>();
HmiAudSts_de->subscribe([]() {
    auto deValue = HmiAudSts_de->get().value();
    HmiAudSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHmiAudSts_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiCmptmtAirDistbnFrnt_de = new DESink<autosar::HmiCmptmtAirDistbnFrnt_info>();
HmiCmptmtAirDistbnFrnt_de->subscribe([]() {
    auto deValue = HmiCmptmtAirDistbnFrnt_de->get().value();
    HmiCmptmtAirDistbnFrnt rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHmiCmptmtAirDistbnFrnt_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiCmptmtCoolgReq_de = new DESink<autosar::HmiCmptmtCoolgReq_info>();
HmiCmptmtCoolgReq_de->subscribe([]() {
    auto deValue = HmiCmptmtCoolgReq_de->get().value();
    HmiCmptmtCoolgReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHmiCmptmtCoolgReq_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiCmptmtTSp_de = new DESink<autosar::HmiCmptmtTSp_info>();
HmiCmptmtTSp_de->subscribe([]() {
    auto deValue = HmiCmptmtTSp_de->get().value();
    HmiCmptmtTSp rteValue;
    rteValue.HmiCmptmtTSpForRowFirstLe = round((deValue.HmiCmptmtTSpForRowFirstLe - 15.0) / 0.5);
    rteValue.HmiCmptmtTSpForRowFirstRi = round((deValue.HmiCmptmtTSpForRowFirstRi - 15.0) / 0.5);
    rteValue.HmiCmptmtTSpForRowSecLe = round((deValue.HmiCmptmtTSpForRowSecLe - 15.0) / 0.5);
    rteValue.HmiCmptmtTSpForRowSecRi = round((deValue.HmiCmptmtTSpForRowSecRi - 15.0) / 0.5);
    rteValue.HmiCmptmtTSpSpclForRowFirstLe = static_cast<decltype(rteValue.HmiCmptmtTSpSpclForRowFirstLe)>(deValue.HmiCmptmtTSpSpclForRowFirstLe);
    rteValue.HmiCmptmtTSpSpclForRowFirstRi = static_cast<decltype(rteValue.HmiCmptmtTSpSpclForRowFirstRi)>(deValue.HmiCmptmtTSpSpclForRowFirstRi);
    rteValue.HmiCmptmtTSpSpclForRowSecLe = static_cast<decltype(rteValue.HmiCmptmtTSpSpclForRowSecLe)>(deValue.HmiCmptmtTSpSpclForRowSecLe);
    rteValue.HmiCmptmtTSpSpclForRowSecRi = static_cast<decltype(rteValue.HmiCmptmtTSpSpclForRowSecRi)>(deValue.HmiCmptmtTSpSpclForRowSecRi);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHmiCmptmtTSp_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiDefrstElecReq_de = new DESink<autosar::HmiDefrstElecReq_info>();
HmiDefrstElecReq_de->subscribe([]() {
    auto deValue = HmiDefrstElecReq_de->get().value();
    HmiDefrstElecReq rteValue;
    rteValue.HmiDefrstElecFrntReq = static_cast<decltype(rteValue.HmiDefrstElecFrntReq)>(deValue.FrntElecReq);
    rteValue.HmiDefrstElecForMirrReq = static_cast<decltype(rteValue.HmiDefrstElecForMirrReq)>(deValue.MirrElecReq);
    rteValue.HmiDefrstElecReReq = static_cast<decltype(rteValue.HmiDefrstElecReReq)>(deValue.ReElecReq);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHmiDefrstElecReq_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiDefrstMaxReq_de = new DESink<autosar::HmiDefrstMaxReq_info>();
HmiDefrstMaxReq_de->subscribe([]() {
    auto deValue = HmiDefrstMaxReq_de->get().value();
    HmiDefrstMaxReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHmiDefrstMaxReq_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiHvacFanLvlFrnt_de = new DESink<autosar::HmiHvacFanLvlFrnt_info>();
HmiHvacFanLvlFrnt_de->subscribe([]() {
    auto deValue = HmiHvacFanLvlFrnt_de->get().value();
    HmiHvacFanLvlFrnt rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHmiHvacFanLvlFrnt_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiHvacFanLvlRe_de = new DESink<autosar::HmiHvacFanLvlRe_info>();
HmiHvacFanLvlRe_de->subscribe([]() {
    auto deValue = HmiHvacFanLvlRe_de->get().value();
    HmiHvacFanLvlRe rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHmiHvacFanLvlRe_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiHvacReCtrl_de = new DESink<autosar::HmiHvacReCtrl_info>();
HmiHvacReCtrl_de->subscribe([]() {
    auto deValue = HmiHvacReCtrl_de->get().value();
    HmiHvacReCtrl rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHmiHvacReCtrl_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiHvacRecircCmd_de = new DESink<autosar::HmiHvacRecircCmd_info>();
HmiHvacRecircCmd_de->subscribe([]() {
    auto deValue = HmiHvacRecircCmd_de->get().value();
    HmiHvacRecircCmd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHmiHvacRecircCmd_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiSeatClima_de = new DESink<autosar::HmiSeatClima_info>();
HmiSeatClima_de->subscribe([]() {
    auto deValue = HmiSeatClima_de->get().value();
    HmiSeatClima rteValue;
    rteValue.HmiSeatHeatgForRowFirstLe = static_cast<decltype(rteValue.HmiSeatHeatgForRowFirstLe)>(deValue.HmiSeatHeatgForRowFirstLe);
    rteValue.HmiSeatHeatgForRowFirstRi = static_cast<decltype(rteValue.HmiSeatHeatgForRowFirstRi)>(deValue.HmiSeatHeatgForRowFirstRi);
    rteValue.HmiSeatHeatgForRowSecLe = static_cast<decltype(rteValue.HmiSeatHeatgForRowSecLe)>(deValue.HmiSeatHeatgForRowSecLe);
    rteValue.HmiSeatHeatgForRowSecRi = static_cast<decltype(rteValue.HmiSeatHeatgForRowSecRi)>(deValue.HmiSeatHeatgForRowSecRi);
    rteValue.HmiSeatVentnForRowFirstLe = static_cast<decltype(rteValue.HmiSeatVentnForRowFirstLe)>(deValue.HmiSeatVentnForRowFirstLe);
    rteValue.HmiSeatVentnForRowFirstRi = static_cast<decltype(rteValue.HmiSeatVentnForRowFirstRi)>(deValue.HmiSeatVentnForRowFirstRi);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHmiSeatClima_mtx, &rteValue, sizeof(rteValue) ); 
});

HmiSeatClimaExtd_de = new DESink<autosar::HmiSeatClimaExtd_info>();
HmiSeatClimaExtd_de->subscribe([]() {
    auto deValue = HmiSeatClimaExtd_de->get().value();
    HmiSeatClimaExtd rteValue;
    rteValue.HmiSeatClimaExtdHmiSeatVentnForRowSecLe = static_cast<decltype(rteValue.HmiSeatClimaExtdHmiSeatVentnForRowSecLe)>(deValue.HmiSeatVentnForRowSecLe);
    rteValue.HmiSeatClimaExtdHmiSeatVentnForRowSecRi = static_cast<decltype(rteValue.HmiSeatClimaExtdHmiSeatVentnForRowSecRi)>(deValue.HmiSeatVentnForRowSecRi);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHmiSeatClimaExtd_mtx, &rteValue, sizeof(rteValue) ); 
});

HudActvReq_de = new DESink<autosar::HudActvReq_info>();
HudActvReq_de->subscribe([]() {
    auto deValue = HudActvReq_de->get().value();
    HudActvReq rteValue;
    rteValue.HudActvReqPen = static_cast<decltype(rteValue.HudActvReqPen)>(deValue.Pen);
    rteValue.HudActvReqSts = static_cast<decltype(rteValue.HudActvReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHudActvReq_mtx, &rteValue, sizeof(rteValue) ); 
});

HudAdjmtReq_de = new DESink<autosar::HudAdjmtReq_info>();
HudAdjmtReq_de->subscribe([]() {
    auto deValue = HudAdjmtReq_de->get().value();
    HudAdjmtReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHudAdjmtReq_mtx, &rteValue, sizeof(rteValue) ); 
});

HudErgoSetgReq_de = new DESink<autosar::HudErgoSetgReq_info>();
HudErgoSetgReq_de->subscribe([]() {
    auto deValue = HudErgoSetgReq_de->get().value();
    HudErgoSetgReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHudErgoSetgReq_mtx, &rteValue, sizeof(rteValue) ); 
});

HudVisFctSetg_de = new DESink<autosar::HudVisFctSetg_info>();
HudVisFctSetg_de->subscribe([]() {
    auto deValue = HudVisFctSetg_de->get().value();
    HudVisFctSetg rteValue;
    rteValue.HudVisFctSetgHudFct00 = static_cast<decltype(rteValue.HudVisFctSetgHudFct00)>(deValue.HudFct00);
    rteValue.HudVisFctSetgHudFct01 = static_cast<decltype(rteValue.HudVisFctSetgHudFct01)>(deValue.HudFct01);
    rteValue.HudVisFctSetgHudFct02 = static_cast<decltype(rteValue.HudVisFctSetgHudFct02)>(deValue.HudFct02);
    rteValue.HudVisFctSetgHudFct03 = static_cast<decltype(rteValue.HudVisFctSetgHudFct03)>(deValue.HudFct03);
    rteValue.HudVisFctSetgHudFct04 = static_cast<decltype(rteValue.HudVisFctSetgHudFct04)>(deValue.HudFct04);
    rteValue.HudVisFctSetgHudFct05 = static_cast<decltype(rteValue.HudVisFctSetgHudFct05)>(deValue.HudFct05);
    rteValue.HudVisFctSetgHudFct06 = static_cast<decltype(rteValue.HudVisFctSetgHudFct06)>(deValue.HudFct06);
    rteValue.HudVisFctSetgHudFct07 = static_cast<decltype(rteValue.HudVisFctSetgHudFct07)>(deValue.HudFct07);
    rteValue.HudVisFctSetgHudFct08 = static_cast<decltype(rteValue.HudVisFctSetgHudFct08)>(deValue.HudFct08);
    rteValue.HudVisFctSetgHudFct09 = static_cast<decltype(rteValue.HudVisFctSetgHudFct09)>(deValue.HudFct09);
    rteValue.HudVisFctSetgHudFct10 = static_cast<decltype(rteValue.HudVisFctSetgHudFct10)>(deValue.HudFct10);
    rteValue.HudVisFctSetgHudFct11 = static_cast<decltype(rteValue.HudVisFctSetgHudFct11)>(deValue.HudFct11);
    rteValue.HudVisFctSetgHudFct12 = static_cast<decltype(rteValue.HudVisFctSetgHudFct12)>(deValue.HudFct12);
    rteValue.HudVisFctSetgHudFct13 = static_cast<decltype(rteValue.HudVisFctSetgHudFct13)>(deValue.HudFct13);
    rteValue.HudVisFctSetgHudFct14 = static_cast<decltype(rteValue.HudVisFctSetgHudFct14)>(deValue.HudFct14);
    rteValue.HudVisFctSetgHudFct15 = static_cast<decltype(rteValue.HudVisFctSetgHudFct15)>(deValue.HudFct15);
    rteValue.HudVisFctSetgHudFct16 = static_cast<decltype(rteValue.HudVisFctSetgHudFct16)>(deValue.HudFct16);
    rteValue.HudVisFctSetgHudFct17 = static_cast<decltype(rteValue.HudVisFctSetgHudFct17)>(deValue.HudFct17);
    rteValue.HudVisFctSetgHudFct18 = static_cast<decltype(rteValue.HudVisFctSetgHudFct18)>(deValue.HudFct18);
    rteValue.HudVisFctSetgHudFct19 = static_cast<decltype(rteValue.HudVisFctSetgHudFct19)>(deValue.HudFct19);
    rteValue.HudVisFctSetgPen = static_cast<decltype(rteValue.HudVisFctSetgPen)>(deValue.Pen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHudVisFctSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

HznData_de = new DESink<autosar::HznData_info>();
HznData_de->subscribe([]() {
    auto deValue = HznData_de->get().value();
    HznData rteValue;
    rteValue.ChdTxVersInHznData = static_cast<decltype(rteValue.ChdTxVersInHznData)>(deValue.ChdTxVers);
    rteValue.CtryCodInHznData = static_cast<decltype(rteValue.CtryCodInHznData)>(deValue.CtryCod);
    rteValue.CycCntrInHznData = static_cast<decltype(rteValue.CycCntrInHznData)>(deValue.CycCntr);
    rteValue.DrvgSideInHznData = static_cast<decltype(rteValue.DrvgSideInHznData)>(deValue.DrvgSide);
    rteValue.HdTxVersInHznData = static_cast<decltype(rteValue.HdTxVersInHznData)>(deValue.HdTxVers);
    rteValue.HwVersInHznData = static_cast<decltype(rteValue.HwVersInHznData)>(deValue.HwVers);
    rteValue.MapSrcInHznData = static_cast<decltype(rteValue.MapSrcInHznData)>(deValue.MapSrc);
    rteValue.MsgTypInHznData = static_cast<decltype(rteValue.MsgTypInHznData)>(deValue.MsgTyp);
    rteValue.PartOfYrVersOfMapInHznData = static_cast<decltype(rteValue.PartOfYrVersOfMapInHznData)>(deValue.PartOfYrVersOfMap);
    rteValue.RegnCodInHznData = static_cast<decltype(rteValue.RegnCodInHznData)>(deValue.RegnCod);
    rteValue.SpdUnitInHznData = static_cast<decltype(rteValue.SpdUnitInHznData)>(deValue.SpdUnit);
    rteValue.TxVersInHznData = static_cast<decltype(rteValue.TxVersInHznData)>(deValue.TxVers);
    rteValue.YrVersOfMapInHznData = static_cast<decltype(rteValue.YrVersOfMapInHznData)>(deValue.YrVersOfMap);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznData_mtx, &rteValue, sizeof(rteValue) ); 
});

HznEdge_de = new DESink<autosar::HznEdge_info>();
HznEdge_de->subscribe([]() {
    auto deValue = HznEdge_de->get().value();
    HznEdge rteValue;
    rteValue.CycCntrInHznEdge = static_cast<decltype(rteValue.CycCntrInHznEdge)>(deValue.CycCntr);
    rteValue.IntscnCmplxInHznEdge = static_cast<decltype(rteValue.IntscnCmplxInHznEdge)>(deValue.IntscnCmplx);
    rteValue.LstEdgeAtOffsInHznEdge = static_cast<decltype(rteValue.LstEdgeAtOffsInHznEdge)>(deValue.LstEdgeAtOffs);
    rteValue.MsgTypInHznEdge = static_cast<decltype(rteValue.MsgTypInHznEdge)>(deValue.MsgTyp);
    rteValue.NrOfLaneInDrvgDirInHznEdge = static_cast<decltype(rteValue.NrOfLaneInDrvgDirInHznEdge)>(deValue.NrOfLaneInDrvgDir);
    rteValue.NrOfLaneInSecDirInHznEdge = static_cast<decltype(rteValue.NrOfLaneInSecDirInHznEdge)>(deValue.NrOfLaneInSecDir);
    rteValue.OffsInHznEdge = static_cast<decltype(rteValue.OffsInHznEdge)>(deValue.Offs);
    rteValue.PahIdxInHznEdge = static_cast<decltype(rteValue.PahIdxInHznEdge)>(deValue.PahIdx);
    rteValue.PahIdxNewInHznEdge = static_cast<decltype(rteValue.PahIdxNewInHznEdge)>(deValue.PahIdxNew);
    rteValue.PartOfCourseCalcdInHznEdge = static_cast<decltype(rteValue.PartOfCourseCalcdInHznEdge)>(deValue.PartOfCourseCalcd);
    rteValue.RelProbltyInHznEdge = round((deValue.RelProblty - 0.0) / 3.33333333);
    rteValue.RiOfWayInHznEdge = static_cast<decltype(rteValue.RiOfWayInHznEdge)>(deValue.RiOfWay);
    rteValue.RoadClassInHznEdge = static_cast<decltype(rteValue.RoadClassInHznEdge)>(deValue.RoadClass);
    rteValue.TurnAgInHznEdge = round((deValue.TurnAg - 0.0) / 1.4173228346456692);
    rteValue.TxPrevInHznEdge = static_cast<decltype(rteValue.TxPrevInHznEdge)>(deValue.TxPrev);
    rteValue.TypOfWayInHznEdge = static_cast<decltype(rteValue.TypOfWayInHznEdge)>(deValue.TypOfWay);
    rteValue.UpdInHznEdge = static_cast<decltype(rteValue.UpdInHznEdge)>(deValue.Upd);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznEdge_mtx, &rteValue, sizeof(rteValue) ); 
});

HznPosn_de = new DESink<autosar::HznPosn_info>();
HznPosn_de->subscribe([]() {
    auto deValue = HznPosn_de->get().value();
    HznPosn rteValue;
    rteValue.CycCntrInHznPosn = static_cast<decltype(rteValue.CycCntrInHznPosn)>(deValue.CycCntr);
    rteValue.LanePrsntInHznPosn = static_cast<decltype(rteValue.LanePrsntInHznPosn)>(deValue.LanePrsnt);
    rteValue.MsgTypInHznPosn = static_cast<decltype(rteValue.MsgTypInHznPosn)>(deValue.MsgTyp);
    rteValue.OffsInHznPosn = static_cast<decltype(rteValue.OffsInHznPosn)>(deValue.Offs);
    rteValue.PahIdxInHznPosn = static_cast<decltype(rteValue.PahIdxInHznPosn)>(deValue.PahIdx);
    rteValue.PosnIdxInHznPosn = static_cast<decltype(rteValue.PosnIdxInHznPosn)>(deValue.PosnIdx);
    rteValue.PosnProbltyInHznPosn = round((deValue.PosnProblty - 0.0) / 3.33333333);
    rteValue.PosnQlyInHznPosn = static_cast<decltype(rteValue.PosnQlyInHznPosn)>(deValue.PosnQly);
    rteValue.PosnTiDifInHznPosn = round((deValue.PosnTiDif - 0.0) / 5.0);
    rteValue.RelDirInHznPosn = round((deValue.RelDir - 0.0) / 1.4173228346456692);
    rteValue.SpdInHznPosn = round((deValue.Spd - -12.8) / 0.2);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznPosn_mtx, &rteValue, sizeof(rteValue) ); 
});

HznPosnExtd_de = new DESink<autosar::HznPosnExtd_info>();
HznPosnExtd_de->subscribe([]() {
    auto deValue = HznPosnExtd_de->get().value();
    HznPosnExtd rteValue;
    rteValue.HznPosnExtdLanePrsnt = static_cast<decltype(rteValue.HznPosnExtdLanePrsnt)>(deValue.LanePrsnt);
    rteValue.HznPosnExtdMsgTyp = static_cast<decltype(rteValue.HznPosnExtdMsgTyp)>(deValue.MsgTyp);
    rteValue.HznPosnExtdPahIdx = static_cast<decltype(rteValue.HznPosnExtdPahIdx)>(deValue.PahIdx);
    rteValue.HznPosnExtdPosnIdx = static_cast<decltype(rteValue.HznPosnExtdPosnIdx)>(deValue.PosnIdx);
    rteValue.HznPosnExtdPosnProblty = round((deValue.PosnProblty - 0.0) / 3.33333333);
    rteValue.HznPosnExtdPosnQly = static_cast<decltype(rteValue.HznPosnExtdPosnQly)>(deValue.PosnQly);
    rteValue.HznPosnExtdPosnTiDif = round((deValue.PosnTiDif - 0.0) / 5.0);
    rteValue.HznPosnExtdRelDir = round((deValue.RelDir - 0.0) / 1.4173228346456692);
    rteValue.HznPosnExtdSpd = round((deValue.Spd - -12.8) / 0.2);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznPosnExtd_mtx, &rteValue, sizeof(rteValue) ); 
});

HznPosnExtdOffs_de = new DESink<autosar::HznPosnExtdOffs_info>();
HznPosnExtdOffs_de->subscribe([]() {
    auto deValue = HznPosnExtdOffs_de->get().value();
    HznPosnExtdOffs rteValue;
    rteValue.HznPosnExtdOffsCycCntr = static_cast<decltype(rteValue.HznPosnExtdOffsCycCntr)>(deValue.CycCntr);
    rteValue.HznPosnExtdOffsOffs = round((deValue.Offs - 0.0) / 128.0);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznPosnExtdOffs_mtx, &rteValue, sizeof(rteValue) ); 
});

HznProfLong_de = new DESink<autosar::HznProfLong_info>();
HznProfLong_de->subscribe([]() {
    auto deValue = HznProfLong_de->get().value();
    HznProfLong rteValue;
    rteValue.CycCntrInHznProfLong = static_cast<decltype(rteValue.CycCntrInHznProfLong)>(deValue.CycCntr);
    rteValue.MsgTypInHznProfLong = static_cast<decltype(rteValue.MsgTypInHznProfLong)>(deValue.MsgTyp);
    rteValue.NodCtrlInHznProfLong = static_cast<decltype(rteValue.NodCtrlInHznProfLong)>(deValue.NodCtrl);
    rteValue.OffsInHznProfLong = static_cast<decltype(rteValue.OffsInHznProfLong)>(deValue.Offs);
    rteValue.PahIdxInHznProfLong = static_cast<decltype(rteValue.PahIdxInHznProfLong)>(deValue.PahIdx);
    rteValue.ProfTypInHznProfLong = static_cast<decltype(rteValue.ProfTypInHznProfLong)>(deValue.ProfTyp);
    rteValue.TxPrevInHznProfLong = static_cast<decltype(rteValue.TxPrevInHznProfLong)>(deValue.TxPrev);
    rteValue.UpdInHznProfLong = static_cast<decltype(rteValue.UpdInHznProfLong)>(deValue.Upd);
    rteValue.ValInHznProfLong = static_cast<decltype(rteValue.ValInHznProfLong)>(deValue.Val);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznProfLong_mtx, &rteValue, sizeof(rteValue) ); 
});

HznProfLongExtd_de = new DESink<autosar::HznProfLongExtd_info>();
HznProfLongExtd_de->subscribe([]() {
    auto deValue = HznProfLongExtd_de->get().value();
    HznProfLongExtd rteValue;
    rteValue.HznProfLongExtdCycCntr = static_cast<decltype(rteValue.HznProfLongExtdCycCntr)>(deValue.CycCntr);
    rteValue.HznProfLongExtdMsgTyp = static_cast<decltype(rteValue.HznProfLongExtdMsgTyp)>(deValue.MsgTyp);
    rteValue.HznProfLongExtdNodCtrl = static_cast<decltype(rteValue.HznProfLongExtdNodCtrl)>(deValue.NodCtrl);
    rteValue.HznProfLongExtdOffs = round((deValue.Offs - 0.0) / 128.0);
    rteValue.HznProfLongExtdPahIdx = static_cast<decltype(rteValue.HznProfLongExtdPahIdx)>(deValue.PahIdx);
    rteValue.HznProfLongExtdProfTyp = static_cast<decltype(rteValue.HznProfLongExtdProfTyp)>(deValue.ProfTyp);
    rteValue.HznProfLongExtdTxPrev = static_cast<decltype(rteValue.HznProfLongExtdTxPrev)>(deValue.TxPrev);
    rteValue.HznProfLongExtdUpd = static_cast<decltype(rteValue.HznProfLongExtdUpd)>(deValue.Upd);
    rteValue.HznProfLongExtdVal = static_cast<decltype(rteValue.HznProfLongExtdVal)>(deValue.Val);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznProfLongExtd_mtx, &rteValue, sizeof(rteValue) ); 
});

HznProfSho_de = new DESink<autosar::HznProfSho_info>();
HznProfSho_de->subscribe([]() {
    auto deValue = HznProfSho_de->get().value();
    HznProfSho rteValue;
    rteValue.CycCntrInHznProfSho = static_cast<decltype(rteValue.CycCntrInHznProfSho)>(deValue.CycCntr);
    rteValue.Dst1InHznProfSho = static_cast<decltype(rteValue.Dst1InHznProfSho)>(deValue.Dst1);
    rteValue.MsgTypInHznProfSho = static_cast<decltype(rteValue.MsgTypInHznProfSho)>(deValue.MsgTyp);
    rteValue.NodCtrlInHznProfSho = static_cast<decltype(rteValue.NodCtrlInHznProfSho)>(deValue.NodCtrl);
    rteValue.OffsInHznProfSho = static_cast<decltype(rteValue.OffsInHznProfSho)>(deValue.Offs);
    rteValue.PahIdxInHznProfSho = static_cast<decltype(rteValue.PahIdxInHznProfSho)>(deValue.PahIdx);
    rteValue.ProfTypInHznProfSho = static_cast<decltype(rteValue.ProfTypInHznProfSho)>(deValue.ProfTyp);
    rteValue.ProfTypQlyInHznProfSho = static_cast<decltype(rteValue.ProfTypQlyInHznProfSho)>(deValue.ProfTypQly);
    rteValue.TxPrevInHznProfSho = static_cast<decltype(rteValue.TxPrevInHznProfSho)>(deValue.TxPrev);
    rteValue.UpdInHznProfSho = static_cast<decltype(rteValue.UpdInHznProfSho)>(deValue.Upd);
    rteValue.Val0InHznProfSho = static_cast<decltype(rteValue.Val0InHznProfSho)>(deValue.Val0);
    rteValue.Val1InHznProfSho = static_cast<decltype(rteValue.Val1InHznProfSho)>(deValue.Val1);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznProfSho_mtx, &rteValue, sizeof(rteValue) ); 
});

HznSeg_de = new DESink<autosar::HznSeg_info>();
HznSeg_de->subscribe([]() {
    auto deValue = HznSeg_de->get().value();
    HznSeg rteValue;
    rteValue.BrdgInHznSeg = static_cast<decltype(rteValue.BrdgInHznSeg)>(deValue.Brdg);
    rteValue.CycCntrInHznSeg = static_cast<decltype(rteValue.CycCntrInHznSeg)>(deValue.CycCntr);
    rteValue.IntscnCmplxInHznSeg = static_cast<decltype(rteValue.IntscnCmplxInHznSeg)>(deValue.IntscnCmplx);
    rteValue.MsgTypInHznSeg = static_cast<decltype(rteValue.MsgTypInHznSeg)>(deValue.MsgTyp);
    rteValue.NrOfLaneInDrvgDirInHznSeg = static_cast<decltype(rteValue.NrOfLaneInDrvgDirInHznSeg)>(deValue.NrOfLaneInDrvgDir);
    rteValue.NrOfLaneInSecDirInHznSeg = static_cast<decltype(rteValue.NrOfLaneInSecDirInHznSeg)>(deValue.NrOfLaneInSecDir);
    rteValue.OffsInHznSeg = static_cast<decltype(rteValue.OffsInHznSeg)>(deValue.Offs);
    rteValue.PahIdxInHznSeg = static_cast<decltype(rteValue.PahIdxInHznSeg)>(deValue.PahIdx);
    rteValue.PartOfCourseCalcdInHznSeg = static_cast<decltype(rteValue.PartOfCourseCalcdInHznSeg)>(deValue.PartOfCourseCalcd);
    rteValue.RelProbltyInHznSeg = round((deValue.RelProblty - 0.0) / 3.33333333);
    rteValue.RoadClassInHznSeg = static_cast<decltype(rteValue.RoadClassInHznSeg)>(deValue.RoadClass);
    rteValue.RoadLiArInHznSeg = static_cast<decltype(rteValue.RoadLiArInHznSeg)>(deValue.RoadLiAr);
    rteValue.RoadMplInHznSeg = static_cast<decltype(rteValue.RoadMplInHznSeg)>(deValue.RoadMpl);
    rteValue.SpdLimEfcInHznSeg = static_cast<decltype(rteValue.SpdLimEfcInHznSeg)>(deValue.SpdLimEfc);
    rteValue.SpdLimTypEfcInHznSeg = static_cast<decltype(rteValue.SpdLimTypEfcInHznSeg)>(deValue.SpdLimTypEfc);
    rteValue.TnlInHznSeg = static_cast<decltype(rteValue.TnlInHznSeg)>(deValue.Tnl);
    rteValue.TxPrevInHznSeg = static_cast<decltype(rteValue.TxPrevInHznSeg)>(deValue.TxPrev);
    rteValue.TypOfWayInHznSeg = static_cast<decltype(rteValue.TypOfWayInHznSeg)>(deValue.TypOfWay);
    rteValue.UpdInHznSeg = static_cast<decltype(rteValue.UpdInHznSeg)>(deValue.Upd);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igHznSeg_mtx, &rteValue, sizeof(rteValue) ); 
});

HznSplyElectcSts_de = new DESink<autosar::HznSplyElectcSts_info>();
HznSplyElectcSts_de->subscribe([]() {
    auto deValue = HznSplyElectcSts_de->get().value();
    HznSplyElectcSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHznSplyElectcSts_mtx, &rteValue, sizeof(rteValue) ); 
});

HzrdLiWarnActv_de = new DESink<autosar::HzrdLiWarnActv_info>();
HzrdLiWarnActv_de->subscribe([]() {
    auto deValue = HzrdLiWarnActv_de->get().value();
    HzrdLiWarnActv rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isHzrdLiWarnActv_mtx, &rteValue, sizeof(rteValue) ); 
});

IndcnUnit_de = new DESink<autosar::IndcnUnit_info>();
IndcnUnit_de->subscribe([]() {
    auto deValue = IndcnUnit_de->get().value();
    IndcnUnit rteValue;
    rteValue.ClkFmt = static_cast<decltype(rteValue.ClkFmt)>(deValue.ClkFmt_);
    rteValue.IndcnUnitDateFmt = static_cast<decltype(rteValue.IndcnUnitDateFmt)>(deValue.DateFmt_);
    rteValue.DstLong = static_cast<decltype(rteValue.DstLong)>(deValue.DstLong_);
    rteValue.DstSho = static_cast<decltype(rteValue.DstSho)>(deValue.DstSho_);
    rteValue.FuCnsUnit = static_cast<decltype(rteValue.FuCnsUnit)>(deValue.FuCnsUnit);
    rteValue.IdPenForUnits = static_cast<decltype(rteValue.IdPenForUnits)>(deValue.IdPenForUnit);
    rteValue.PUnit = static_cast<decltype(rteValue.PUnit)>(deValue.PUnit_);
    rteValue.SpdUnit = static_cast<decltype(rteValue.SpdUnit)>(deValue.SpdUnit);
    rteValue.TUnit = static_cast<decltype(rteValue.TUnit)>(deValue.TUnit);
    rteValue.VolUnit = static_cast<decltype(rteValue.VolUnit)>(deValue.VolUnit_);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igIndcnUnit_mtx, &rteValue, sizeof(rteValue) ); 
});

InsdCarNoiseMeasd_de = new DESink<autosar::InsdCarNoiseMeasd_info>();
InsdCarNoiseMeasd_de->subscribe([]() {
    auto deValue = InsdCarNoiseMeasd_de->get().value();
    InsdCarNoiseMeasd rteValue;
    rteValue = round((deValue - 0.0) / 0.5);
    sendAvmpMessageToVip( ComConf_ComSignal_isInsdCarNoiseMeasd_mtx, &rteValue, sizeof(rteValue) ); 
});

IntrBriStsForSeatHeatrRe_de = new DESink<autosar::IntrBriStsForSeatHeatrRe_info>();
IntrBriStsForSeatHeatrRe_de->subscribe([]() {
    auto deValue = IntrBriStsForSeatHeatrRe_de->get().value();
    IntrBriStsForSeatHeatrRe rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isIntrBriStsForSeatHeatrRe_mtx, &rteValue, sizeof(rteValue) ); 
});

IntrLiAmbLiSetg_de = new DESink<autosar::IntrLiAmbLiSetg_info>();
IntrLiAmbLiSetg_de->subscribe([]() {
    auto deValue = IntrLiAmbLiSetg_de->get().value();
    IntrLiAmbLiSetg rteValue;
    rteValue.IntrLiAmbLiSetgForLiInten = static_cast<decltype(rteValue.IntrLiAmbLiSetgForLiInten)>(deValue.LiInten);
    rteValue.IntrLiAmbLiSetgForLiTintg = static_cast<decltype(rteValue.IntrLiAmbLiSetgForLiTintg)>(deValue.LiTintg);
    rteValue.IntrLiAmbLiSetgPen = static_cast<decltype(rteValue.IntrLiAmbLiSetgPen)>(deValue.Pen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igIntrLiAmbLiSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

IntrLiSurrndgsLiSetgLi_de = new DESink<autosar::IntrLiSurrndgsLiSetgLi_info>();
IntrLiSurrndgsLiSetgLi_de->subscribe([]() {
    auto deValue = IntrLiSurrndgsLiSetgLi_de->get().value();
    IntrLiSurrndgsLiSetgLi rteValue;
    rteValue.IntrLiSurrndgsLiSetgForLiInten = static_cast<decltype(rteValue.IntrLiSurrndgsLiSetgForLiInten)>(deValue.LiInten);
    rteValue.IntrLiSurrndgsLiSetgForLiLvl = static_cast<decltype(rteValue.IntrLiSurrndgsLiSetgForLiLvl)>(deValue.LiLvl);
    rteValue.IntrLiSurrndgsLiSetgPen = static_cast<decltype(rteValue.IntrLiSurrndgsLiSetgPen)>(deValue.Pen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igIntrLiSurrndgsLiSetgLi_mtx, &rteValue, sizeof(rteValue) ); 
});

IntrMirrTintgSetg_de = new DESink<autosar::IntrMirrTintgSetg_info>();
IntrMirrTintgSetg_de->subscribe([]() {
    auto deValue = IntrMirrTintgSetg_de->get().value();
    IntrMirrTintgSetg rteValue;
    rteValue.IntrMirrTintgSetgSts = static_cast<decltype(rteValue.IntrMirrTintgSetgSts)>(deValue.MirrDimLvl);
    rteValue.IntrMirrTintgSetgPen = static_cast<decltype(rteValue.IntrMirrTintgSetgPen)>(deValue.Pen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igIntrMirrTintgSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

IntvAndWarnModForLaneKeepAid_de = new DESink<autosar::IntvAndWarnModForLaneKeepAid_info>();
IntvAndWarnModForLaneKeepAid_de->subscribe([]() {
    auto deValue = IntvAndWarnModForLaneKeepAid_de->get().value();
    IntvAndWarnModForLaneKeepAid rteValue;
    rteValue.IntvAndWarnModForLaneKeepAidPen = static_cast<decltype(rteValue.IntvAndWarnModForLaneKeepAidPen)>(deValue.Pen);
    rteValue.IntvAndWarnModForLaneKeepAidSts = static_cast<decltype(rteValue.IntvAndWarnModForLaneKeepAidSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igIntvAndWarnModForLaneKeepAid_mtx, &rteValue, sizeof(rteValue) ); 
});

KeyProfMpgUpd_de = new DESink<autosar::KeyProfMpgUpd_info>();
KeyProfMpgUpd_de->subscribe([]() {
    auto deValue = KeyProfMpgUpd_de->get().value();
    KeyProfMpgUpd rteValue;
    rteValue.KeyProfMpgUpdForIdPen = static_cast<decltype(rteValue.KeyProfMpgUpdForIdPen)>(deValue.KeyProfMpgUpdForIdPen);
    rteValue.KeyProfMpgUpdKeyProfMpgUpdOff = static_cast<decltype(rteValue.KeyProfMpgUpdKeyProfMpgUpdOff)>(deValue.KeyProfMpgUpdOff);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igKeyProfMpgUpd_mtx, &rteValue, sizeof(rteValue) ); 
});

KeyReadReqFromSetgMgr_de = new DESink<autosar::KeyReadReqFromSetgMgr_info>();
KeyReadReqFromSetgMgr_de->subscribe([]() {
    auto deValue = KeyReadReqFromSetgMgr_de->get().value();
    KeyReadReqFromSetgMgr rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isKeyReadReqFromSetgMgr_mtx, &rteValue, sizeof(rteValue) ); 
});

LampSuppSrv_de = new DESink<autosar::LampSuppSrv_info>();
LampSuppSrv_de->subscribe([]() {
    auto deValue = LampSuppSrv_de->get().value();
    LampSuppSrv rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isLampSuppSrv_mtx, &rteValue, sizeof(rteValue) ); 
});

LaneChgWarnActv_de = new DESink<autosar::LaneChgWarnActv_info>();
LaneChgWarnActv_de->subscribe([]() {
    auto deValue = LaneChgWarnActv_de->get().value();
    LaneChgWarnActv rteValue;
    rteValue.LaneChgWarnActvPen = static_cast<decltype(rteValue.LaneChgWarnActvPen)>(deValue.Pen);
    rteValue.LaneChgWarnActvSts = static_cast<decltype(rteValue.LaneChgWarnActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLaneChgWarnActv_mtx, &rteValue, sizeof(rteValue) ); 
});

LaneKeepAidActv_de = new DESink<autosar::LaneKeepAidActv_info>();
LaneKeepAidActv_de->subscribe([]() {
    auto deValue = LaneKeepAidActv_de->get().value();
    LaneKeepAidActv rteValue;
    rteValue.LaneKeepAidActvPen = static_cast<decltype(rteValue.LaneKeepAidActvPen)>(deValue.Pen);
    rteValue.LaneKeepAidActvSts = static_cast<decltype(rteValue.LaneKeepAidActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLaneKeepAidActv_mtx, &rteValue, sizeof(rteValue) ); 
});

LaneKeepAidRoadEdgeActv_de = new DESink<autosar::LaneKeepAidRoadEdgeActv_info>();
LaneKeepAidRoadEdgeActv_de->subscribe([]() {
    auto deValue = LaneKeepAidRoadEdgeActv_de->get().value();
    LaneKeepAidRoadEdgeActv rteValue;
    rteValue.LaneKeepAidRoadEdgeActvPen = static_cast<decltype(rteValue.LaneKeepAidRoadEdgeActvPen)>(deValue.Pen);
    rteValue.LaneKeepAidRoadEdgeActvSts = static_cast<decltype(rteValue.LaneKeepAidRoadEdgeActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLaneKeepAidRoadEdgeActv_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtReq1WdReq1_de = new DESink<autosar::LiExtReq1WdReq1_info>();
LiExtReq1WdReq1_de->subscribe([]() {
    auto deValue = LiExtReq1WdReq1_de->get().value();
    LiAdpvReq rteValue;
    rteValue.LiAdpvReqPen = static_cast<decltype(rteValue.LiAdpvReqPen)>(deValue.Pen);
    rteValue.LiAdpvReqSts = static_cast<decltype(rteValue.LiAdpvReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiAdpvReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtReq2WdReq1_de = new DESink<autosar::LiExtReq2WdReq1_info>();
LiExtReq2WdReq1_de->subscribe([]() {
    auto deValue = LiExtReq2WdReq1_de->get().value();
    LiAutTranReq rteValue;
    rteValue.LiAutTranReqPen = static_cast<decltype(rteValue.LiAutTranReqPen)>(deValue.Pen);
    rteValue.LiAutTranReqSts = static_cast<decltype(rteValue.LiAutTranReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiAutTranReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtReq1WdReq4_de = new DESink<autosar::LiExtReq1WdReq4_info>();
LiExtReq1WdReq4_de->subscribe([]() {
    auto deValue = LiExtReq1WdReq4_de->get().value();
    LiBeamHiAuxReq rteValue;
    rteValue.LiBeamHiAuxReqPen = static_cast<decltype(rteValue.LiBeamHiAuxReqPen)>(deValue.Pen);
    rteValue.LiBeamHiAuxReqSts = static_cast<decltype(rteValue.LiBeamHiAuxReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiBeamHiAuxReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtReq1WdReq6_de = new DESink<autosar::LiExtReq1WdReq6_info>();
LiExtReq1WdReq6_de->subscribe([]() {
    auto deValue = LiExtReq1WdReq6_de->get().value();
    LiCornrgReq rteValue;
    rteValue.LiCornrgReqPen = static_cast<decltype(rteValue.LiCornrgReqPen)>(deValue.Pen);
    rteValue.LiCornrgReqSts = static_cast<decltype(rteValue.LiCornrgReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiCornrgReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtReq1WdReq2_de = new DESink<autosar::LiExtReq1WdReq2_info>();
LiExtReq1WdReq2_de->subscribe([]() {
    auto deValue = LiExtReq1WdReq2_de->get().value();
    LiDaytiRunngReq rteValue;
    rteValue.LiDaytiRunngReqPen = static_cast<decltype(rteValue.LiDaytiRunngReqPen)>(deValue.Pen);
    rteValue.LiDaytiRunngReqSts = static_cast<decltype(rteValue.LiDaytiRunngReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiDaytiRunngReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LiForBtn4ForUsrSwtPanFrntCmd_de = new DESink<autosar::LiForBtn4ForUsrSwtPanFrntCmd_info>();
LiForBtn4ForUsrSwtPanFrntCmd_de->subscribe([]() {
    auto deValue = LiForBtn4ForUsrSwtPanFrntCmd_de->get().value();
    LiForBtn4ForUsrSwtPanFrntCmd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isLiForBtn4ForUsrSwtPanFrntCmd_mtx, &rteValue, sizeof(rteValue) ); 
});

LiForBtn5ForUsrSwtPanFrntCmd_de = new DESink<autosar::LiForBtn5ForUsrSwtPanFrntCmd_info>();
LiForBtn5ForUsrSwtPanFrntCmd_de->subscribe([]() {
    auto deValue = LiForBtn5ForUsrSwtPanFrntCmd_de->get().value();
    LiForBtn5ForUsrSwtPanFrntCmd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isLiForBtn5ForUsrSwtPanFrntCmd_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtSafe1WdReq2_de = new DESink<autosar::LiExtSafe1WdReq2_info>();
LiExtSafe1WdReq2_de->subscribe([]() {
    auto deValue = LiExtSafe1WdReq2_de->get().value();
    LiHomeLvngReq rteValue;
    rteValue.LiHomeLvngReqPen = static_cast<decltype(rteValue.LiHomeLvngReqPen)>(deValue.Pen);
    rteValue.LiHomeLvngReqSts = static_cast<decltype(rteValue.LiHomeLvngReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiHomeLvngReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtSafe1WdReq1_de = new DESink<autosar::LiExtSafe1WdReq1_info>();
LiExtSafe1WdReq1_de->subscribe([]() {
    auto deValue = LiExtSafe1WdReq1_de->get().value();
    LiHomeSafeReq rteValue;
    rteValue.LiHomeSafeReqPen = static_cast<decltype(rteValue.LiHomeSafeReqPen)>(deValue.Pen);
    rteValue.LiHomeSafeReqSts = static_cast<decltype(rteValue.LiHomeSafeReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiHomeSafeReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LiSeldForDrvrPfmncMon_de = new DESink<autosar::LiSeldForDrvrPfmncMon_info>();
LiSeldForDrvrPfmncMon_de->subscribe([]() {
    auto deValue = LiSeldForDrvrPfmncMon_de->get().value();
    LiSeldForDrvrPfmncMon rteValue;
    rteValue.LiSeldForDrvrPfmncMonPen = static_cast<decltype(rteValue.LiSeldForDrvrPfmncMonPen)>(deValue.Pen);
    rteValue.LiSeldForDrvrPfmncMonSts = static_cast<decltype(rteValue.LiSeldForDrvrPfmncMonSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLiSeldForDrvrPfmncMon_mtx, &rteValue, sizeof(rteValue) ); 
});

LiTrfcSide1WdReq1_de = new DESink<autosar::LiTrfcSide1WdReq1_info>();
LiTrfcSide1WdReq1_de->subscribe([]() {
    auto deValue = LiTrfcSide1WdReq1_de->get().value();
    LiTrfcSideReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isLiTrfcSideReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LockSpdReq_de = new DESink<autosar::LockSpdReq_info>();
LockSpdReq_de->subscribe([]() {
    auto deValue = LockSpdReq_de->get().value();
    LockSpdReq rteValue;
    rteValue.LockSpdReqPen = static_cast<decltype(rteValue.LockSpdReqPen)>(deValue.Pen);
    rteValue.LockSpdReqSts = static_cast<decltype(rteValue.LockSpdReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLockSpdReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LockgFbSoundReq_de = new DESink<autosar::LockgFbSoundReq_info>();
LockgFbSoundReq_de->subscribe([]() {
    auto deValue = LockgFbSoundReq_de->get().value();
    LockgFbSoundReq rteValue;
    rteValue.LockgFbSoundReqPen = static_cast<decltype(rteValue.LockgFbSoundReqPen)>(deValue.Pen);
    rteValue.LockgFbSoundReqSts = static_cast<decltype(rteValue.LockgFbSoundReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLockgFbSoundReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LockgFbVisReq_de = new DESink<autosar::LockgFbVisReq_info>();
LockgFbVisReq_de->subscribe([]() {
    auto deValue = LockgFbVisReq_de->get().value();
    LockgFbVisReq rteValue;
    rteValue.LockgFbVisReqPen = static_cast<decltype(rteValue.LockgFbVisReqPen)>(deValue.Pen);
    rteValue.LockgFbVisReqSts = static_cast<decltype(rteValue.LockgFbVisReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLockgFbVisReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LockgPrsnlReqFromHmi_de = new DESink<autosar::LockgPrsnlReqFromHmi_info>();
LockgPrsnlReqFromHmi_de->subscribe([]() {
    auto deValue = LockgPrsnlReqFromHmi_de->get().value();
    LockgPrsnlReqFromHmi rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isLockgPrsnlReqFromHmi_mtx, &rteValue, sizeof(rteValue) ); 
});

LvlSeldForSpdLimAdpv_de = new DESink<autosar::LvlSeldForSpdLimAdpv_info>();
LvlSeldForSpdLimAdpv_de->subscribe([]() {
    auto deValue = LvlSeldForSpdLimAdpv_de->get().value();
    LvlSeldForSpdLimAdpv rteValue;
    rteValue.LvlSeldForSpdLimAdpvPen = static_cast<decltype(rteValue.LvlSeldForSpdLimAdpvPen)>(deValue.Pen);
    rteValue.LvlSeldForSpdLimAdpvSts = static_cast<decltype(rteValue.LvlSeldForSpdLimAdpvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igLvlSeldForSpdLimAdpv_mtx, &rteValue, sizeof(rteValue) ); 
});

MassgFctActv_de = new DESink<autosar::MassgFctActv_info>();
MassgFctActv_de->subscribe([]() {
    auto deValue = MassgFctActv_de->get().value();
    MassgFctActv rteValue;
    rteValue.MassgFctActvDrvrMassgFctActv = static_cast<decltype(rteValue.MassgFctActvDrvrMassgFctActv)>(deValue.DrvrMassgFctActv);
    rteValue.MassgFctActvPassMassgFctActv = static_cast<decltype(rteValue.MassgFctActvPassMassgFctActv)>(deValue.PassMassgFctActv);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igMassgFctActv_mtx, &rteValue, sizeof(rteValue) ); 
});

MmedHdPwrMod_de = new DESink<autosar::MmedHdPwrMod_info>();
MmedHdPwrMod_de->subscribe([]() {
    auto deValue = MmedHdPwrMod_de->get().value();
    MmedMaiPwrMod rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isMmedMaiPwrMod_mtx, &rteValue, sizeof(rteValue) ); 
});

MmedHmiModStd_de = new DESink<autosar::MmedHmiModStd_info>();
MmedHmiModStd_de->subscribe([]() {
    auto deValue = MmedHmiModStd_de->get().value();
    MmedHmiModStd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isMmedHmiModStd_mtx, &rteValue, sizeof(rteValue) ); 
});

MmedTvmPwerMod_de = new DESink<autosar::MmedTvmPwerMod_info>();
MmedTvmPwerMod_de->subscribe([]() {
    auto deValue = MmedTvmPwerMod_de->get().value();
    MmedTvmPwerMod rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isMmedTvmPwerMod_mtx, &rteValue, sizeof(rteValue) ); 
});

NetHdActvt_de = new DESink<autosar::NetHdActvt_info>();
NetHdActvt_de->subscribe([]() {
    auto deValue = NetHdActvt_de->get().value();
    NetHdActvt rteValue;
    rteValue.NetHdActvtPrio = static_cast<decltype(rteValue.NetHdActvtPrio)>(deValue.Prio);
    rteValue.NetHdActvtResourceGroup = static_cast<decltype(rteValue.NetHdActvtResourceGroup)>(deValue.ResourceGroup);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igNetHdActvt_mtx, &rteValue, sizeof(rteValue) ); 
});

NoSoundSys_de = new DESink<autosar::NoSoundSys_info>();
NoSoundSys_de->subscribe([]() {
    auto deValue = NoSoundSys_de->get().value();
    NoSoundSys rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isNoSoundSys_mtx, &rteValue, sizeof(rteValue) ); 
});

OffsForDrvrSpprtFctActvSts_de = new DESink<autosar::OffsForDrvrSpprtFctActvSts_info>();
OffsForDrvrSpprtFctActvSts_de->subscribe([]() {
    auto deValue = OffsForDrvrSpprtFctActvSts_de->get().value();
    OffsForDrvrSpprtFctActvSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isOffsForDrvrSpprtFctActvSts_mtx, &rteValue, sizeof(rteValue) ); 
});

OffsForSpdWarnSetg_de = new DESink<autosar::OffsForSpdWarnSetg_info>();
OffsForSpdWarnSetg_de->subscribe([]() {
    auto deValue = OffsForSpdWarnSetg_de->get().value();
    OffsForSpdWarnSetg rteValue;
    rteValue.OffsForSpdWarnSetgPen = static_cast<decltype(rteValue.OffsForSpdWarnSetgPen)>(deValue.Pen);
    rteValue.OffsForSpdWarnSetgSts = round((deValue.Sts - 0.0) / 1.0);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igOffsForSpdWarnSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

PasAlrmDeactvnReq_de = new DESink<autosar::PasAlrmDeactvnReq_info>();
PasAlrmDeactvnReq_de->subscribe([]() {
    auto deValue = PasAlrmDeactvnReq_de->get().value();
    PasAlrmDeactvnReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isPasAlrmDeactvnReq_mtx, &rteValue, sizeof(rteValue) ); 
});

PassSeatDispMassgFct_de = new DESink<autosar::PassSeatDispMassgFct_info>();
PassSeatDispMassgFct_de->subscribe([]() {
    auto deValue = PassSeatDispMassgFct_de->get().value();
    PassSeatDispMassgFct rteValue;
    rteValue.PassSeatDispMassgFctMassgInten = static_cast<decltype(rteValue.PassSeatDispMassgFctMassgInten)>(deValue.MassgInten);
    rteValue.PassSeatDispMassgFctMassgProg = static_cast<decltype(rteValue.PassSeatDispMassgFctMassgProg)>(deValue.MassgProg);
    rteValue.PassSeatDispMassgFctMassgSpdLvl = static_cast<decltype(rteValue.PassSeatDispMassgFctMassgSpdLvl)>(deValue.MassgSpdLvl);
    rteValue.PassSeatDispMassgFctOnOff = static_cast<decltype(rteValue.PassSeatDispMassgFctOnOff)>(deValue.OnOff);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igPassSeatDispMassgFct_mtx, &rteValue, sizeof(rteValue) ); 
});

PassSeatDispSpplFct_de = new DESink<autosar::PassSeatDispSpplFct_info>();
PassSeatDispSpplFct_de->subscribe([]() {
    auto deValue = PassSeatDispSpplFct_de->get().value();
    PassSeatDispSpplFct rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isPassSeatDispSpplFct_mtx, &rteValue, sizeof(rteValue) ); 
});

PostDrvgClimaReq_de = new DESink<autosar::PostDrvgClimaReq_info>();
PostDrvgClimaReq_de->subscribe([]() {
    auto deValue = PostDrvgClimaReq_de->get().value();
    ClimaEcoModRqrd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isClimaEcoModRqrd_mtx, &rteValue, sizeof(rteValue) ); 
});

PrkgOutCfm_de = new DESink<autosar::PrkgOutCfm_info>();
PrkgOutCfm_de->subscribe([]() {
    auto deValue = PrkgOutCfm_de->get().value();
    PrkgOutCfm rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isPrkgOutCfm_mtx, &rteValue, sizeof(rteValue) ); 
});

ProfChg_de = new DESink<autosar::ProfChg_info>();
ProfChg_de->subscribe([]() {
    auto deValue = ProfChg_de->get().value();
    ProfChg rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isProfChg_mtx, &rteValue, sizeof(rteValue) ); 
});

PrpsnHvBattUsgModReq_de = new DESink<autosar::PrpsnHvBattUsgModReq_info>();
PrpsnHvBattUsgModReq_de->subscribe([]() {
    auto deValue = PrpsnHvBattUsgModReq_de->get().value();
    PrpsnHvBattUsgModReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isPrpsnHvBattUsgModReq_mtx, &rteValue, sizeof(rteValue) ); 
});

PrpsnHvBattUsgOfHldSpd_de = new DESink<autosar::PrpsnHvBattUsgOfHldSpd_info>();
PrpsnHvBattUsgOfHldSpd_de->subscribe([]() {
    auto deValue = PrpsnHvBattUsgOfHldSpd_de->get().value();
    PrpsnHvBattUsgOfHldSpd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isPrpsnHvBattUsgOfHldSpd_mtx, &rteValue, sizeof(rteValue) ); 
});

PrpsnHvBattUsgStsDispd_de = new DESink<autosar::PrpsnHvBattUsgStsDispd_info>();
PrpsnHvBattUsgStsDispd_de->subscribe([]() {
    auto deValue = PrpsnHvBattUsgStsDispd_de->get().value();
    PrpsnHvBattUsgStsDispd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isPrpsnHvBattUsgStsDispd_mtx, &rteValue, sizeof(rteValue) ); 
});

PtDrvrSetg_de = new DESink<autosar::PtDrvrSetg_info>();
PtDrvrSetg_de->subscribe([]() {
    auto deValue = PtDrvrSetg_de->get().value();
    PtDrvrSetg rteValue;
    rteValue.PtDrvrSetgPen = static_cast<decltype(rteValue.PtDrvrSetgPen)>(deValue.Pen);
    rteValue.PtDrvrSetgSts = static_cast<decltype(rteValue.PtDrvrSetgSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igPtDrvrSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

RadioFrqAn_de = new DESink<autosar::RadioFrqAn_info>();
RadioFrqAn_de->subscribe([]() {
    auto deValue = RadioFrqAn_de->get().value();
    RadioFrqAn rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isRadioFrqAn_mtx, &rteValue, sizeof(rteValue) ); 
});

RainSenMemdReq_de = new DESink<autosar::RainSenMemdReq_info>();
RainSenMemdReq_de->subscribe([]() {
    auto deValue = RainSenMemdReq_de->get().value();
    RainSenMemdReq rteValue;
    rteValue.RainSenMemdReqPen = static_cast<decltype(rteValue.RainSenMemdReqPen)>(deValue.Pen);
    rteValue.RainSenMemdReqSts = static_cast<decltype(rteValue.RainSenMemdReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igRainSenMemdReq_mtx, &rteValue, sizeof(rteValue) ); 
});

RngbdIllmnCmd_de = new DESink<autosar::RngbdIllmnCmd_info>();
RngbdIllmnCmd_de->subscribe([]() {
    auto deValue = RngbdIllmnCmd_de->get().value();
    RngbdIllmnCmd rteValue;
    rteValue.RngbdIllmnCmdSts = static_cast<decltype(rteValue.RngbdIllmnCmdSts)>(deValue.Cmd);
    rteValue.RngbdIllmnCmdPen = static_cast<decltype(rteValue.RngbdIllmnCmdPen)>(deValue.Pen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igRngbdIllmnCmd_mtx, &rteValue, sizeof(rteValue) ); 
});

RoadFricIndcnActv_de = new DESink<autosar::RoadFricIndcnActv_info>();
RoadFricIndcnActv_de->subscribe([]() {
    auto deValue = RoadFricIndcnActv_de->get().value();
    RoadFricIndcnActv rteValue;
    rteValue.RoadFricIndcnActvPen = static_cast<decltype(rteValue.RoadFricIndcnActvPen)>(deValue.Pen);
    rteValue.RoadFricIndcnActvSts = static_cast<decltype(rteValue.RoadFricIndcnActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igRoadFricIndcnActv_mtx, &rteValue, sizeof(rteValue) ); 
});

RoadSgnInfoActv_de = new DESink<autosar::RoadSgnInfoActv_info>();
RoadSgnInfoActv_de->subscribe([]() {
    auto deValue = RoadSgnInfoActv_de->get().value();
    RoadSgnInfoActv rteValue;
    rteValue.RoadSgnInfoActvPen = static_cast<decltype(rteValue.RoadSgnInfoActvPen)>(deValue.Pen);
    rteValue.RoadSgnInfoActvSts = static_cast<decltype(rteValue.RoadSgnInfoActvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igRoadSgnInfoActv_mtx, &rteValue, sizeof(rteValue) ); 
});

RoadSpdLimActvSts_de = new DESink<autosar::RoadSpdLimActvSts_info>();
RoadSpdLimActvSts_de->subscribe([]() {
    auto deValue = RoadSpdLimActvSts_de->get().value();
    RoadSpdLimActvSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isRoadSpdLimActvSts_mtx, &rteValue, sizeof(rteValue) ); 
});

RoadUsrProtnActv_de = new DESink<autosar::RoadUsrProtnActv_info>();
RoadUsrProtnActv_de->subscribe([]() {
    auto deValue = RoadUsrProtnActv_de->get().value();
    RoadUsrProtnActv rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isRoadUsrProtnActv_mtx, &rteValue, sizeof(rteValue) ); 
});

SaveSetgToMemPrmnt_de = new DESink<autosar::SaveSetgToMemPrmnt_info>();
SaveSetgToMemPrmnt_de->subscribe([]() {
    auto deValue = SaveSetgToMemPrmnt_de->get().value();
    SaveSetgToMemPrmnt rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSaveSetgToMemPrmnt_mtx, &rteValue, sizeof(rteValue) ); 
});

SeatDispBtnPsd_de = new DESink<autosar::SeatDispBtnPsd_info>();
SeatDispBtnPsd_de->subscribe([]() {
    auto deValue = SeatDispBtnPsd_de->get().value();
    SeatDispBtnPsd rteValue;
    rteValue.SeatDispBtnPsdDrvrSeatDispBtnPsd = static_cast<decltype(rteValue.SeatDispBtnPsdDrvrSeatDispBtnPsd)>(deValue.DrvrSeatDispBtnPsd);
    rteValue.SeatDispBtnPsdPassSeatDispBtnPsd = static_cast<decltype(rteValue.SeatDispBtnPsdPassSeatDispBtnPsd)>(deValue.PassSeatDispBtnPsd);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSeatDispBtnPsd_mtx, &rteValue, sizeof(rteValue) ); 
});

SeatHeatDurgClimaEnad_de = new DESink<autosar::SeatHeatDurgClimaEnad_info>();
SeatHeatDurgClimaEnad_de->subscribe([]() {
    auto deValue = SeatHeatDurgClimaEnad_de->get().value();
    SeatHeatDurgClimaEnad rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSeatHeatDurgClimaEnad_mtx, &rteValue, sizeof(rteValue) ); 
});

SeatSwtLeSigThrd_de = new DESink<autosar::SeatSwtLeSigThrd_info>();
SeatSwtLeSigThrd_de->subscribe([]() {
    auto deValue = SeatSwtLeSigThrd_de->get().value();
    SeatSwtLeSigThrd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSeatSwtLeSigThrd_mtx, &rteValue, sizeof(rteValue) ); 
});

SeatSwtRiSigThrd_de = new DESink<autosar::SeatSwtRiSigThrd_info>();
SeatSwtRiSigThrd_de->subscribe([]() {
    auto deValue = SeatSwtRiSigThrd_de->get().value();
    SeatSwtRiSigThrd rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSeatSwtRiSigThrd_mtx, &rteValue, sizeof(rteValue) ); 
});

SetOfLang_de = new DESink<autosar::SetOfLang_info>();
SetOfLang_de->subscribe([]() {
    auto deValue = SetOfLang_de->get().value();
    SetOfLang rteValue;
    rteValue.SetOfLangIdPen = static_cast<decltype(rteValue.SetOfLangIdPen)>(deValue.IdPen_);
    rteValue.SetOfLangLangTyp = static_cast<decltype(rteValue.SetOfLangLangTyp)>(deValue.LangTyp_);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSetOfLang_mtx, &rteValue, sizeof(rteValue) ); 
});

SetTiAndDate_de = new DESink<autosar::SetTiAndDate_info>();
SetTiAndDate_de->subscribe([]() {
    auto deValue = SetTiAndDate_de->get().value();
    SetTiAndDate rteValue;
    rteValue.DateOrTi = static_cast<decltype(rteValue.DateOrTi)>(deValue.DateOrTi_);
    rteValue.Day1 = static_cast<decltype(rteValue.Day1)>(deValue.Day);
    rteValue.Hr1 = static_cast<decltype(rteValue.Hr1)>(deValue.Hour);
    rteValue.Mins1 = static_cast<decltype(rteValue.Mins1)>(deValue.Minute);
    rteValue.Mth1 = static_cast<decltype(rteValue.Mth1)>(deValue.Month);
    rteValue.Sec1 = static_cast<decltype(rteValue.Sec1)>(deValue.Second);
    rteValue.Yr1 = static_cast<decltype(rteValue.Yr1)>(deValue.Year);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSetTiAndDate_mtx, &rteValue, sizeof(rteValue) ); 
});

SftyCchLongTermIdxRst_de = new DESink<autosar::SftyCchLongTermIdxRst_info>();
SftyCchLongTermIdxRst_de->subscribe([]() {
    auto deValue = SftyCchLongTermIdxRst_de->get().value();
    SftyCchLongTermIdxRst rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSftyCchLongTermIdxRst_mtx, &rteValue, sizeof(rteValue) ); 
});

SftyCchPostTripSts_de = new DESink<autosar::SftyCchPostTripSts_info>();
SftyCchPostTripSts_de->subscribe([]() {
    auto deValue = SftyCchPostTripSts_de->get().value();
    SftyCchPostTripSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSftyCchPostTripSts_mtx, &rteValue, sizeof(rteValue) ); 
});

SftySigGroupFromAudSafe_de = new DESink<autosar::SftySigGroupFromAudSafe_info>();
SftySigGroupFromAudSafe_de->subscribe([]() {
    auto deValue = SftySigGroupFromAudSafe_de->get().value();
    SftySigGroupFromAudSafe rteValue;
    rteValue.SftyAudDend = static_cast<decltype(rteValue.SftyAudDend)>(deValue.SftyAudDend);
    rteValue.SftySigGroupFromAudSafeSftyAudEna = static_cast<decltype(rteValue.SftySigGroupFromAudSafeSftyAudEna)>(deValue.SftyAudEna);
    rteValue.SftySigFaildDetdByAud = static_cast<decltype(rteValue.SftySigFaildDetdByAud)>(deValue.SftySigFaildDetdByAud);
    rteValue.SftySigGroupFromAudSafeChks = static_cast<decltype(rteValue.SftySigGroupFromAudSafeChks)>(deValue.SftySigGroupFromAudSafeChks);
    rteValue.SftySigGroupFromAudSafeCntr = static_cast<decltype(rteValue.SftySigGroupFromAudSafeCntr)>(deValue.SftySigGroupFromAudSafeCntr);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSftySigGroupFromAudSafe_mtx, &rteValue, sizeof(rteValue) ); 
});

SoundExtActvSts_de = new DESink<autosar::SoundExtActvSts_info>();
SoundExtActvSts_de->subscribe([]() {
    auto deValue = SoundExtActvSts_de->get().value();
    SoundExtActvSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSoundExtActvSts_mtx, &rteValue, sizeof(rteValue) ); 
});

SpdAlrmActvForRoadSgnInfo_de = new DESink<autosar::SpdAlrmActvForRoadSgnInfo_info>();
SpdAlrmActvForRoadSgnInfo_de->subscribe([]() {
    auto deValue = SpdAlrmActvForRoadSgnInfo_de->get().value();
    SpdAlrmActvForRoadSgnInfo rteValue;
    rteValue.SpdAlrmActvForRoadSgnInfoPen = static_cast<decltype(rteValue.SpdAlrmActvForRoadSgnInfoPen)>(deValue.Pen);
    rteValue.SpdAlrmActvForRoadSgnInfoSts = static_cast<decltype(rteValue.SpdAlrmActvForRoadSgnInfoSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSpdAlrmActvForRoadSgnInfo_mtx, &rteValue, sizeof(rteValue) ); 
});

SpdCameraInfoSeldForRoadSgnInfo_de = new DESink<autosar::SpdCameraInfoSeldForRoadSgnInfo_info>();
SpdCameraInfoSeldForRoadSgnInfo_de->subscribe([]() {
    auto deValue = SpdCameraInfoSeldForRoadSgnInfo_de->get().value();
    SpdCameraInfoSeldForRoadSgnInfo rteValue;
    rteValue.SpdCameraInfoSeldForRoadSgnInfoPen = static_cast<decltype(rteValue.SpdCameraInfoSeldForRoadSgnInfoPen)>(deValue.Pen);
    rteValue.SpdCameraInfoSeldForRoadSgnInfoSts = static_cast<decltype(rteValue.SpdCameraInfoSeldForRoadSgnInfoSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSpdCameraInfoSeldForRoadSgnInfo_mtx, &rteValue, sizeof(rteValue) ); 
});

SteerAssiActvForAutDrv_de = new DESink<autosar::SteerAssiActvForAutDrv_info>();
SteerAssiActvForAutDrv_de->subscribe([]() {
    auto deValue = SteerAssiActvForAutDrv_de->get().value();
    SteerAssiActvForAutDrv rteValue;
    rteValue.SteerAssiActvForAutDrvPen = static_cast<decltype(rteValue.SteerAssiActvForAutDrvPen)>(deValue.Pen);
    rteValue.SteerAssiActvForAutDrvSts = static_cast<decltype(rteValue.SteerAssiActvForAutDrvSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSteerAssiActvForAutDrv_mtx, &rteValue, sizeof(rteValue) ); 
});

SteerSetg_de = new DESink<autosar::SteerSetg_info>();
SteerSetg_de->subscribe([]() {
    auto deValue = SteerSetg_de->get().value();
    SteerSetg rteValue;
    rteValue.SteerSetgPen = static_cast<decltype(rteValue.SteerSetgPen)>(deValue.Pen);
    rteValue.SteerAsscLvl = static_cast<decltype(rteValue.SteerAsscLvl)>(deValue.SteerAsscLvl_);
    rteValue.SteerMod = static_cast<decltype(rteValue.SteerMod)>(deValue.SteerMod_);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSteerSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

SteerWhlHeatgDurgClimaEnad_de = new DESink<autosar::SteerWhlHeatgDurgClimaEnad_info>();
SteerWhlHeatgDurgClimaEnad_de->subscribe([]() {
    auto deValue = SteerWhlHeatgDurgClimaEnad_de->get().value();
    SteerWhlHeatgDurgClimaEnad rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSteerWhlHeatgDurgClimaEnad_mtx, &rteValue, sizeof(rteValue) ); 
});

SteerWhlHeatgOnReq_de = new DESink<autosar::SteerWhlHeatgOnReq_info>();
SteerWhlHeatgOnReq_de->subscribe([]() {
    auto deValue = SteerWhlHeatgOnReq_de->get().value();
    SteerWhlHeatgOnReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSteerWhlHeatgOnReq_mtx, &rteValue, sizeof(rteValue) ); 
});

SuspSetg_de = new DESink<autosar::SuspSetg_info>();
SuspSetg_de->subscribe([]() {
    auto deValue = SuspSetg_de->get().value();
    SuspSetg rteValue;
    rteValue.LvlCtrlSetg = static_cast<decltype(rteValue.LvlCtrlSetg)>(deValue.LvlCtrlSetg);
    rteValue.SuspCtrlSetg = static_cast<decltype(rteValue.SuspCtrlSetg)>(deValue.SuspCtrlSetg);
    rteValue.SuspIdPen = static_cast<decltype(rteValue.SuspIdPen)>(deValue.SuspIdPen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igSuspSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

SwtAtCenSts_de = new DESink<autosar::SwtAtCenSts_info>();
SwtAtCenSts_de->subscribe([]() {
    auto deValue = SwtAtCenSts_de->get().value();
    SwtAtCenSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSwtAtCenSts_mtx, &rteValue, sizeof(rteValue) ); 
});

TiGapAdpvSeld_de = new DESink<autosar::TiGapAdpvSeld_info>();
TiGapAdpvSeld_de->subscribe([]() {
    auto deValue = TiGapAdpvSeld_de->get().value();
    TiGapAdpvSeld rteValue;
    rteValue.TiGapAdpvSeldPen = static_cast<decltype(rteValue.TiGapAdpvSeldPen)>(deValue.Pen);
    rteValue.TiGapAdpvSeldSts = static_cast<decltype(rteValue.TiGapAdpvSeldSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igTiGapAdpvSeld_mtx, &rteValue, sizeof(rteValue) ); 
});

TiGapLimd_de = new DESink<autosar::TiGapLimd_info>();
TiGapLimd_de->subscribe([]() {
    auto deValue = TiGapLimd_de->get().value();
    TiGapLimd rteValue;
    rteValue.TiGapLimdPen = static_cast<decltype(rteValue.TiGapLimdPen)>(deValue.Pen);
    rteValue.TiGapLimdSts = round((deValue.Sts - 0.0) / 0.1);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igTiGapLimd_mtx, &rteValue, sizeof(rteValue) ); 
});

TireCircumCalByNav_de = new DESink<autosar::TireCircumCalByNav_info>();
TireCircumCalByNav_de->subscribe([]() {
    auto deValue = TireCircumCalByNav_de->get().value();
    TireCircumCalByNav rteValue;
    rteValue.HiQlyInTireCircumCalByNav = static_cast<decltype(rteValue.HiQlyInTireCircumCalByNav)>(deValue.HiQly);
    rteValue.SpprtForFctInTireCircumCalByNav = static_cast<decltype(rteValue.SpprtForFctInTireCircumCalByNav)>(deValue.SpprtForFct);
    rteValue.TireCircumInTireCircumCalByNav = static_cast<decltype(rteValue.TireCircumInTireCircumCalByNav)>(deValue.TireCircum);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igTireCircumCalByNav_mtx, &rteValue, sizeof(rteValue) ); 
});

TireMonCalReq_de = new DESink<autosar::TireMonCalReq_info>();
TireMonCalReq_de->subscribe([]() {
    auto deValue = TireMonCalReq_de->get().value();
    TireMonCalReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isTireMonCalReq_mtx, &rteValue, sizeof(rteValue) ); 
});

TireMonrDispReq_de = new DESink<autosar::TireMonrDispReq_info>();
TireMonrDispReq_de->subscribe([]() {
    auto deValue = TireMonrDispReq_de->get().value();
    TireMonrDispReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isTireMonrDispReq_mtx, &rteValue, sizeof(rteValue) ); 
});

TrfcLiSpprtActv_de = new DESink<autosar::TrfcLiSpprtActv_info>();
TrfcLiSpprtActv_de->subscribe([]() {
    auto deValue = TrfcLiSpprtActv_de->get().value();
    TrfcLiSpprtActv rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isTrfcLiSpprtActv_mtx, &rteValue, sizeof(rteValue) ); 
});

TrfcRglnSeldForRoadSgnInfo_de = new DESink<autosar::TrfcRglnSeldForRoadSgnInfo_info>();
TrfcRglnSeldForRoadSgnInfo_de->subscribe([]() {
    auto deValue = TrfcRglnSeldForRoadSgnInfo_de->get().value();
    TrfcRglnSeldForRoadSgnInfo rteValue;
    rteValue.TrfcRglnSeldForRoadSgnInfoPen = static_cast<decltype(rteValue.TrfcRglnSeldForRoadSgnInfoPen)>(deValue.Pen);
    rteValue.TrfcRglnSeldForRoadSgnInfoSts = static_cast<decltype(rteValue.TrfcRglnSeldForRoadSgnInfoSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igTrfcRglnSeldForRoadSgnInfo_mtx, &rteValue, sizeof(rteValue) ); 
});

TripModSeln_de = new DESink<autosar::TripModSeln_info>();
TripModSeln_de->subscribe([]() {
    auto deValue = TripModSeln_de->get().value();
    TripModSeln rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isTripModSeln_mtx, &rteValue, sizeof(rteValue) ); 
});

TrlrLampChkAutReq_de = new DESink<autosar::TrlrLampChkAutReq_info>();
TrlrLampChkAutReq_de->subscribe([]() {
    auto deValue = TrlrLampChkAutReq_de->get().value();
    TrlrLampChkAutReq rteValue;
    rteValue.TrlrLampChkAutReqPen = static_cast<decltype(rteValue.TrlrLampChkAutReqPen)>(deValue.Pen);
    rteValue.TrlrLampChkAutReqSts = static_cast<decltype(rteValue.TrlrLampChkAutReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igTrlrLampChkAutReq_mtx, &rteValue, sizeof(rteValue) ); 
});

TrlrLampChkRemReq_de = new DESink<autosar::TrlrLampChkRemReq_info>();
TrlrLampChkRemReq_de->subscribe([]() {
    auto deValue = TrlrLampChkRemReq_de->get().value();
    TrlrLampChkRemReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isTrlrLampChkRemReq_mtx, &rteValue, sizeof(rteValue) ); 
});

LiExtReq1WdReq3_de = new DESink<autosar::LiExtReq1WdReq3_info>();
LiExtReq1WdReq3_de->subscribe([]() {
    auto deValue = LiExtReq1WdReq3_de->get().value();
    TurnAutFlsgReq rteValue;
    rteValue.TurnAutFlsgReqPen = static_cast<decltype(rteValue.TurnAutFlsgReqPen)>(deValue.Pen);
    rteValue.TurnAutFlsgReqSts = static_cast<decltype(rteValue.TurnAutFlsgReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igTurnAutFlsgReq_mtx, &rteValue, sizeof(rteValue) ); 
});

TwliBriStsForSeatHeatrRe_de = new DESink<autosar::TwliBriStsForSeatHeatrRe_info>();
TwliBriStsForSeatHeatrRe_de->subscribe([]() {
    auto deValue = TwliBriStsForSeatHeatrRe_de->get().value();
    TwliBriStsForSeatHeatrRe rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isTwliBriStsForSeatHeatrRe_mtx, &rteValue, sizeof(rteValue) ); 
});

UkwnCptRespFromInfoMgr_de = new DESink<autosar::UkwnCptRespFromInfoMgr_info>();
UkwnCptRespFromInfoMgr_de->subscribe([]() {
    auto deValue = UkwnCptRespFromInfoMgr_de->get().value();
    UkwnCptRespFromInfoMgr rteValue;
    rteValue.UkwnCptRespFromInfoMgrByte0 = static_cast<decltype(rteValue.UkwnCptRespFromInfoMgrByte0)>(deValue.Byte0);
    rteValue.UkwnCptRespFromInfoMgrByte1 = static_cast<decltype(rteValue.UkwnCptRespFromInfoMgrByte1)>(deValue.Byte1);
    rteValue.UkwnCptRespFromInfoMgrByte2 = static_cast<decltype(rteValue.UkwnCptRespFromInfoMgrByte2)>(deValue.Byte2);
    rteValue.UkwnCptRespFromInfoMgrByte3 = static_cast<decltype(rteValue.UkwnCptRespFromInfoMgrByte3)>(deValue.Byte3);
    rteValue.UkwnCptRespFromInfoMgrByte4 = static_cast<decltype(rteValue.UkwnCptRespFromInfoMgrByte4)>(deValue.Byte4);
    rteValue.UkwnCptRespFromInfoMgrByte5 = static_cast<decltype(rteValue.UkwnCptRespFromInfoMgrByte5)>(deValue.Byte5);
    rteValue.UkwnCptRespFromInfoMgrByte6 = static_cast<decltype(rteValue.UkwnCptRespFromInfoMgrByte6)>(deValue.Byte6);
    rteValue.UkwnCptRespFromInfoMgrByte7 = static_cast<decltype(rteValue.UkwnCptRespFromInfoMgrByte7)>(deValue.Byte7);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUkwnCptRespFromInfoMgr_mtx, &rteValue, sizeof(rteValue) ); 
});

UkwnCptRespFromSoundMgr_de = new DESink<autosar::UkwnCptRespFromSoundMgr_info>();
UkwnCptRespFromSoundMgr_de->subscribe([]() {
    auto deValue = UkwnCptRespFromSoundMgr_de->get().value();
    UkwnCptRespFromSoundMgr rteValue;
    rteValue.UkwnCptRespFromSoundMgrByte0 = static_cast<decltype(rteValue.UkwnCptRespFromSoundMgrByte0)>(deValue.Byte0);
    rteValue.UkwnCptRespFromSoundMgrByte1 = static_cast<decltype(rteValue.UkwnCptRespFromSoundMgrByte1)>(deValue.Byte1);
    rteValue.UkwnCptRespFromSoundMgrByte2 = static_cast<decltype(rteValue.UkwnCptRespFromSoundMgrByte2)>(deValue.Byte2);
    rteValue.UkwnCptRespFromSoundMgrByte3 = static_cast<decltype(rteValue.UkwnCptRespFromSoundMgrByte3)>(deValue.Byte3);
    rteValue.UkwnCptRespFromSoundMgrByte4 = static_cast<decltype(rteValue.UkwnCptRespFromSoundMgrByte4)>(deValue.Byte4);
    rteValue.UkwnCptRespFromSoundMgrByte5 = static_cast<decltype(rteValue.UkwnCptRespFromSoundMgrByte5)>(deValue.Byte5);
    rteValue.UkwnCptRespFromSoundMgrByte6 = static_cast<decltype(rteValue.UkwnCptRespFromSoundMgrByte6)>(deValue.Byte6);
    rteValue.UkwnCptRespFromSoundMgrByte7 = static_cast<decltype(rteValue.UkwnCptRespFromSoundMgrByte7)>(deValue.Byte7);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUkwnCptRespFromSoundMgr_mtx, &rteValue, sizeof(rteValue) ); 
});

UnlckFbVisReq_de = new DESink<autosar::UnlckFbVisReq_info>();
UnlckFbVisReq_de->subscribe([]() {
    auto deValue = UnlckFbVisReq_de->get().value();
    UnlckFbVisReq rteValue;
    rteValue.UnlckFbVisReqPen = static_cast<decltype(rteValue.UnlckFbVisReqPen)>(deValue.Pen);
    rteValue.UnlckFbVisReqSts = static_cast<decltype(rteValue.UnlckFbVisReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUnlckFbVisReq_mtx, &rteValue, sizeof(rteValue) ); 
});

UnlckKeylsReq_de = new DESink<autosar::UnlckKeylsReq_info>();
UnlckKeylsReq_de->subscribe([]() {
    auto deValue = UnlckKeylsReq_de->get().value();
    UnlckKeylsReq rteValue;
    rteValue.UnlckKeylsReqPen = static_cast<decltype(rteValue.UnlckKeylsReqPen)>(deValue.IdPen_);
    rteValue.UnlckKeylsReqSts = static_cast<decltype(rteValue.UnlckKeylsReqSts)>(deValue.KeylsCfg);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUnlckKeylsReq_mtx, &rteValue, sizeof(rteValue) ); 
});

UnlckRemReq_de = new DESink<autosar::UnlckRemReq_info>();
UnlckRemReq_de->subscribe([]() {
    auto deValue = UnlckRemReq_de->get().value();
    UnlckRemReq rteValue;
    rteValue.UnlckRemReqPen = static_cast<decltype(rteValue.UnlckRemReqPen)>(deValue.Pen);
    rteValue.UnlckRemReqSts = static_cast<decltype(rteValue.UnlckRemReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUnlckRemReq_mtx, &rteValue, sizeof(rteValue) ); 
});

UsrSetSpdForKeySpdLimn_de = new DESink<autosar::UsrSetSpdForKeySpdLimn_info>();
UsrSetSpdForKeySpdLimn_de->subscribe([]() {
    auto deValue = UsrSetSpdForKeySpdLimn_de->get().value();
    UsrSetSpdForKeySpdLimn rteValue;
    rteValue.UsrSetSpdForKeySpdLimnPen = static_cast<decltype(rteValue.UsrSetSpdForKeySpdLimnPen)>(deValue.Pen);
    rteValue.UsrSetSpdForKeySpdLimnSts = round((deValue.Sts - 0.0) / 0.03125);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUsrSetSpdForKeySpdLimn_mtx, &rteValue, sizeof(rteValue) ); 
});

UsrSetSpdForKeySpdWarn_de = new DESink<autosar::UsrSetSpdForKeySpdWarn_info>();
UsrSetSpdForKeySpdWarn_de->subscribe([]() {
    auto deValue = UsrSetSpdForKeySpdWarn_de->get().value();
    UsrSetSpdForKeySpdWarn rteValue;
    rteValue.UsrSetSpdForKeySpdWarn1 = round((deValue.UsrSetSpdForKeySpdWarn1 - 0.0) / 0.03125);
    rteValue.UsrSetSpdForKeySpdWarn2 = round((deValue.UsrSetSpdForKeySpdWarn2 - 0.0) / 0.03125);
    rteValue.UsrSetSpdForKeySpdWarn3 = round((deValue.UsrSetSpdForKeySpdWarn3 - 0.0) / 0.03125);
    rteValue.UsrSetSpdForKeySpdWarn4 = round((deValue.UsrSetSpdForKeySpdWarn4 - 0.0) / 0.03125);
    rteValue.UsrSetSpdForKeySpdWarn5 = round((deValue.UsrSetSpdForKeySpdWarn5 - 0.0) / 0.03125);
    rteValue.UsrSetSpdForKeySpdWarn6 = round((deValue.UsrSetSpdForKeySpdWarn6 - 0.0) / 0.03125);
    rteValue.UsrSetSpdForKeySpdWarnPen = static_cast<decltype(rteValue.UsrSetSpdForKeySpdWarnPen)>(deValue.UsrSetSpdForKeySpdWarnPen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUsrSetSpdForKeySpdWarn_mtx, &rteValue, sizeof(rteValue) ); 
});

UsrSwtDispClimaReq_de = new DESink<autosar::UsrSwtDispClimaReq_info>();
UsrSwtDispClimaReq_de->subscribe([]() {
    auto deValue = UsrSwtDispClimaReq_de->get().value();
    UsrSwtDispClimaReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isUsrSwtDispClimaReq_mtx, &rteValue, sizeof(rteValue) ); 
});

SeatHeatReLeSts_de = new DESink<autosar::SeatHeatReLeSts_info>();
SeatHeatReLeSts_de->subscribe([]() {
    auto deValue = SeatHeatReLeSts_de->get().value();
    SeatHeatReLeSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSeatHeatReLeSts_mtx, &rteValue, sizeof(rteValue) ); 
});

SeatHeatReRiSts_de = new DESink<autosar::SeatHeatReRiSts_info>();
SeatHeatReRiSts_de->subscribe([]() {
    auto deValue = SeatHeatReRiSts_de->get().value();
    SeatHeatReRiSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isSeatHeatReRiSts_mtx, &rteValue, sizeof(rteValue) ); 
});

UsrSwtDispClimaSts_de = new DESink<autosar::UsrSwtDispClimaSts_info>();
UsrSwtDispClimaSts_de->subscribe([]() {
    auto deValue = UsrSwtDispClimaSts_de->get().value();
    UsrSwtDispClimaSts rteValue;
    rteValue.UsrSwtDispClimaTSpForRowSecLe = round((deValue.UsrSwtDispClimaTSpForRowSecLe - 15.0) / 0.5);
    rteValue.UsrSwtDispClimaTSpForRowSecRi = round((deValue.UsrSwtDispClimaTSpForRowSecRi - 15.0) / 0.5);
    rteValue.UsrSwtDispClimaTSpSpclForRowSecLe = static_cast<decltype(rteValue.UsrSwtDispClimaTSpSpclForRowSecLe)>(deValue.UsrSwtDispClimaTSpSpclForRowSecLe);
    rteValue.UsrSwtDispClimaTSpSpclForRowSecRi = static_cast<decltype(rteValue.UsrSwtDispClimaTSpSpclForRowSecRi)>(deValue.UsrSwtDispClimaTSpSpclForRowSecRi);
    rteValue.UsrSwtDispFanLvlForRowSec = static_cast<decltype(rteValue.UsrSwtDispFanLvlForRowSec)>(deValue.UsrSwtDispFanLvlForRowSec);
    rteValue.UsrSwtDispSeatHeatLvlForRowSecLe = static_cast<decltype(rteValue.UsrSwtDispSeatHeatLvlForRowSecLe)>(deValue.UsrSwtDispSeatHeatLvlForRowSecLe);
    rteValue.UsrSwtDispSeatHeatLvlForRowSecRi = static_cast<decltype(rteValue.UsrSwtDispSeatHeatLvlForRowSecRi)>(deValue.UsrSwtDispSeatHeatLvlForRowSecRi);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUsrSwtDispClimaSts_mtx, &rteValue, sizeof(rteValue) ); 
});

UsrSwtDispForSecRowSeatVentn_de = new DESink<autosar::UsrSwtDispForSecRowSeatVentn_info>();
UsrSwtDispForSecRowSeatVentn_de->subscribe([]() {
    auto deValue = UsrSwtDispForSecRowSeatVentn_de->get().value();
    UsrSwtDispForSecRowSeatVentn rteValue;
    rteValue.UsrSwtDispForSecRowSeatVentnUsrSwtDispForSecRowSeatVentnLe = static_cast<decltype(rteValue.UsrSwtDispForSecRowSeatVentnUsrSwtDispForSecRowSeatVentnLe)>(deValue.UsrSwtDispForSecRowSeatVentnLe);
    rteValue.UsrSwtDispForSecRowSeatVentnUsrSwtDispForSecRowSeatVentnRi = static_cast<decltype(rteValue.UsrSwtDispForSecRowSeatVentnUsrSwtDispForSecRowSeatVentnRi)>(deValue.UsrSwtDispForSecRowSeatVentnRi);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUsrSwtDispForSecRowSeatVentn_mtx, &rteValue, sizeof(rteValue) ); 
});

UsrSwtDispReqVrnt_de = new DESink<autosar::UsrSwtDispReqVrnt_info>();
UsrSwtDispReqVrnt_de->subscribe([]() {
    auto deValue = UsrSwtDispReqVrnt_de->get().value();
    UsrSwtDispReqVrnt rteValue;
    rteValue.UsrSwtDispSeatHeatFct = static_cast<decltype(rteValue.UsrSwtDispSeatHeatFct)>(deValue.UsrSwtDispSeatHeatFct);
    rteValue.UsrSwtDispTUnit = static_cast<decltype(rteValue.UsrSwtDispTUnit)>(deValue.UsrSwtDispTUnit_);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igUsrSwtDispReqVrnt_mtx, &rteValue, sizeof(rteValue) ); 
});

VFCVectorIHU_de = new DESink<autosar::VFCVectorIHU_info>();
VFCVectorIHU_de->subscribe([]() {
    auto deValue = VFCVectorIHU_de->get().value();
    VFCVectorIHU rteValue;
    rteValue.VFCVectorIHUGrp1 = static_cast<decltype(rteValue.VFCVectorIHUGrp1)>(deValue.Grp1);
    rteValue.VFCVectorIHUGrp2 = static_cast<decltype(rteValue.VFCVectorIHUGrp2)>(deValue.Grp2);
    rteValue.VFCVectorIHUGrp3 = static_cast<decltype(rteValue.VFCVectorIHUGrp3)>(deValue.Grp3);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igVFCVectorIHU_mtx, &rteValue, sizeof(rteValue) ); 
});

VehDecelCtrlSetg_de = new DESink<autosar::VehDecelCtrlSetg_info>();
VehDecelCtrlSetg_de->subscribe([]() {
    auto deValue = VehDecelCtrlSetg_de->get().value();
    VehDecelCtrlSetg rteValue;
    rteValue.VehDecelCtrlSetgNormSpt = static_cast<decltype(rteValue.VehDecelCtrlSetgNormSpt)>(deValue.NormSpt);
    rteValue.VehDecelCtrlSetgPen = static_cast<decltype(rteValue.VehDecelCtrlSetgPen)>(deValue.Pen);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igVehDecelCtrlSetg_mtx, &rteValue, sizeof(rteValue) ); 
});

VehSpdCtrlActvSts_de = new DESink<autosar::VehSpdCtrlActvSts_info>();
VehSpdCtrlActvSts_de->subscribe([]() {
    auto deValue = VehSpdCtrlActvSts_de->get().value();
    VehSpdCtrlActvSts rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isVehSpdCtrlActvSts_mtx, &rteValue, sizeof(rteValue) ); 
});

VisnAgWideCfg_de = new DESink<autosar::VisnAgWideCfg_info>();
VisnAgWideCfg_de->subscribe([]() {
    auto deValue = VisnAgWideCfg_de->get().value();
    VisnAgWideCfg rteValue;
    rteValue.VisnAgWideCfgCtraInfo = static_cast<decltype(rteValue.VisnAgWideCfgCtraInfo)>(deValue.CtraInfo);
    rteValue.VisnAgWideCfgDstInfoFrnt = static_cast<decltype(rteValue.VisnAgWideCfgDstInfoFrnt)>(deValue.DstInfoFrnt);
    rteValue.VisnAgWideCfgDstInfoRe = static_cast<decltype(rteValue.VisnAgWideCfgDstInfoRe)>(deValue.DstInfoRe);
    rteValue.VisnAgWideCfgDstInfoSide = static_cast<decltype(rteValue.VisnAgWideCfgDstInfoSide)>(deValue.DstInfoSide);
    rteValue.VisnAgWideCfgObjDetn = static_cast<decltype(rteValue.VisnAgWideCfgObjDetn)>(deValue.ObjDetn);
    rteValue.VisnAgWideCfgSteerPahFrnt = static_cast<decltype(rteValue.VisnAgWideCfgSteerPahFrnt)>(deValue.SteerPahFrnt);
    rteValue.VisnAgWideCfgSteerPahRe = static_cast<decltype(rteValue.VisnAgWideCfgSteerPahRe)>(deValue.SteerPahRe);
    rteValue.VisnAgWideCfgSteerPahSide = static_cast<decltype(rteValue.VisnAgWideCfgSteerPahSide)>(deValue.SteerPahSide);
    rteValue.VisnAgWideCfgTwbrPah = static_cast<decltype(rteValue.VisnAgWideCfgTwbrPah)>(deValue.TwbrPah);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igVisnAgWideCfg_mtx, &rteValue, sizeof(rteValue) ); 
});

VisnImgAgWideReq_de = new DESink<autosar::VisnImgAgWideReq_info>();
VisnImgAgWideReq_de->subscribe([]() {
    auto deValue = VisnImgAgWideReq_de->get().value();
    VisnImgAgWideReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isVisnImgAgWideReq_mtx, &rteValue, sizeof(rteValue) ); 
});

VisnImgCamReq_de = new DESink<autosar::VisnImgCamReq_info>();
VisnImgCamReq_de->subscribe([]() {
    auto deValue = VisnImgCamReq_de->get().value();
    VisnImgCamReq rteValue;
    rteValue.VisnImgCamReqCamFrnt = static_cast<decltype(rteValue.VisnImgCamReqCamFrnt)>(deValue.CamFrnt);
    rteValue.VisnImgCamReqCamLe = static_cast<decltype(rteValue.VisnImgCamReqCamLe)>(deValue.CamLe);
    rteValue.VisnImgCamReqCamRe = static_cast<decltype(rteValue.VisnImgCamReqCamRe)>(deValue.CamRe);
    rteValue.VisnImgCamReqCamRi = static_cast<decltype(rteValue.VisnImgCamReqCamRi)>(deValue.CamRi);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igVisnImgCamReq_mtx, &rteValue, sizeof(rteValue) ); 
});

WarnTypForLaneChgWarn_de = new DESink<autosar::WarnTypForLaneChgWarn_info>();
WarnTypForLaneChgWarn_de->subscribe([]() {
    auto deValue = WarnTypForLaneChgWarn_de->get().value();
    WarnTypForLaneChgWarn rteValue;
    rteValue.WarnTypForLaneChgWarnPen = static_cast<decltype(rteValue.WarnTypForLaneChgWarnPen)>(deValue.Pen);
    rteValue.WarnTypForLaneChgWarnSts = static_cast<decltype(rteValue.WarnTypForLaneChgWarnSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igWarnTypForLaneChgWarn_mtx, &rteValue, sizeof(rteValue) ); 
});

WarnTypForLaneKeepAid_de = new DESink<autosar::WarnTypForLaneKeepAid_info>();
WarnTypForLaneKeepAid_de->subscribe([]() {
    auto deValue = WarnTypForLaneKeepAid_de->get().value();
    WarnTypForLaneKeepAid rteValue;
    rteValue.WarnTypForLaneKeepAidPen = static_cast<decltype(rteValue.WarnTypForLaneKeepAidPen)>(deValue.Pen);
    rteValue.WarnTypForLaneKeepAidSts = static_cast<decltype(rteValue.WarnTypForLaneKeepAidSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igWarnTypForLaneKeepAid_mtx, &rteValue, sizeof(rteValue) ); 
});

WiprFrntSrvModReq_de = new DESink<autosar::WiprFrntSrvModReq_info>();
WiprFrntSrvModReq_de->subscribe([]() {
    auto deValue = WiprFrntSrvModReq_de->get().value();
    WiprFrntSrvModReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isWiprFrntSrvModReq_mtx, &rteValue, sizeof(rteValue) ); 
});

WiprReAutReq_de = new DESink<autosar::WiprReAutReq_info>();
WiprReAutReq_de->subscribe([]() {
    auto deValue = WiprReAutReq_de->get().value();
    WiprReAutReq rteValue;
    rteValue.WiprReAutReqPen = static_cast<decltype(rteValue.WiprReAutReqPen)>(deValue.Pen);
    rteValue.WiprReAutReqSts = static_cast<decltype(rteValue.WiprReAutReqSts)>(deValue.Sts);

    sendAvmpMessageToVip( SignalGroup|ComConf_ComSignalGroup_igWiprReAutReq_mtx, &rteValue, sizeof(rteValue) ); 
});

iTPMSCalReq_de = new DESink<autosar::iTPMSCalReq_info>();
iTPMSCalReq_de->subscribe([]() {
    auto deValue = iTPMSCalReq_de->get().value();
    iTPMSCalReq rteValue;
    rteValue = static_cast<decltype(rteValue)>(deValue);
    sendAvmpMessageToVip( ComConf_ComSignal_isiTPMSCalReq_mtx, &rteValue, sizeof(rteValue) ); 
});


// clang-format on
