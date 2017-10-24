// clang-format off
/*!
 * \file
 * C++ code generator for AUTOSAR v1.0
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved. Delphi Confidential
 * Source: databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml
 */
#ifndef _JSONENCDEC_H
#define _JSONENCDEC_H

#include <json.hpp>
#include <type_traits>

#include "gen_datatypes.h"

using json = nlohmann::json;

namespace autosar {

// handles all enum types
template<typename T> using AREnum = typename std::enable_if<std::is_enum<T>::value, T>::type;

// handles everything EXCEPT enum (and the explicits below)
template<typename T> using AllButAREnum = typename std::enable_if<!std::is_enum<T>::value, T>::type;

template <typename T>
json toJson(AREnum<T> e) {
    return json((int)e);
}

template <typename T>
AREnum<T> fromJson(const json& j) {
    if (!j.is_number()) {
        return T();
    }
    return static_cast<T>( j.get<int>() );
}

// handles everything EXCEPT enum (and the explicits below)
template <typename T>
json toJson(const AllButAREnum<T>& e) {
    return json(e);
}

template <typename T>
AllButAREnum<T> fromJson(const json& j) {
    if ( j.is_array() || j.is_null() || j.is_object() ) {
        return T();
    }
    return j.get<T>();
}


// ==========================================
// ADataRawSafe1
template <> json toJson<ADataRawSafe1>(const ADataRawSafe1& v);
template <> ADataRawSafe1 fromJson<ADataRawSafe1>(const json& j);

// ==========================================
// ActvnOfCrsEco
template <> json toJson<ActvnOfCrsEco>(const ActvnOfCrsEco& v);
template <> ActvnOfCrsEco fromJson<ActvnOfCrsEco>(const json& j);

// ==========================================
// AdprTurnSpdModPen1
template <> json toJson<AdprTurnSpdModPen1>(const AdprTurnSpdModPen1& v);
template <> AdprTurnSpdModPen1 fromJson<AdprTurnSpdModPen1>(const json& j);

// ==========================================
// AgDataRawSafe1
template <> json toJson<AgDataRawSafe1>(const AgDataRawSafe1& v);
template <> AgDataRawSafe1 fromJson<AgDataRawSafe1>(const json& j);

// ==========================================
// AmbTIndcdWithUnit
template <> json toJson<AmbTIndcdWithUnit>(const AmbTIndcdWithUnit& v);
template <> AmbTIndcdWithUnit fromJson<AmbTIndcdWithUnit>(const json& j);

// ==========================================
// AmbTWithQly
template <> json toJson<AmbTWithQly>(const AmbTWithQly& v);
template <> AmbTWithQly fromJson<AmbTWithQly>(const json& j);

// ==========================================
// std::array<uint8_t,8>
template <> json toJson<std::array<uint8_t,8>>(const std::array<uint8_t,8>& v);
template <> std::array<uint8_t,8> fromJson<std::array<uint8_t,8>>(const json& j);

// ==========================================
// AsyLaneChg1
template <> json toJson<AsyLaneChg1>(const AsyLaneChg1& v);
template <> AsyLaneChg1 fromJson<AsyLaneChg1>(const json& j);

// ==========================================
// AutnmsPrkgActvScenario
template <> json toJson<AutnmsPrkgActvScenario>(const AutnmsPrkgActvScenario& v);
template <> AutnmsPrkgActvScenario fromJson<AutnmsPrkgActvScenario>(const json& j);

// ==========================================
// AutnmsPrkgAvlScenario
template <> json toJson<AutnmsPrkgAvlScenario>(const AutnmsPrkgAvlScenario& v);
template <> AutnmsPrkgAvlScenario fromJson<AutnmsPrkgAvlScenario>(const json& j);

// ==========================================
// AutnmsPrkgSeldScenario
template <> json toJson<AutnmsPrkgSeldScenario>(const AutnmsPrkgSeldScenario& v);
template <> AutnmsPrkgSeldScenario fromJson<AutnmsPrkgSeldScenario>(const json& j);

// ==========================================
// BltLockStFrnt
template <> json toJson<BltLockStFrnt>(const BltLockStFrnt& v);
template <> BltLockStFrnt fromJson<BltLockStFrnt>(const json& j);

// ==========================================
// BltLockStRe
template <> json toJson<BltLockStRe>(const BltLockStRe& v);
template <> BltLockStRe fromJson<BltLockStRe>(const json& j);

// ==========================================
// BrkAndAbsWarnIndcnReqRec1
template <> json toJson<BrkAndAbsWarnIndcnReqRec1>(const BrkAndAbsWarnIndcnReqRec1& v);
template <> BrkAndAbsWarnIndcnReqRec1 fromJson<BrkAndAbsWarnIndcnReqRec1>(const json& j);

// ==========================================
// BrkFricTqAtWhlActRec
template <> json toJson<BrkFricTqAtWhlActRec>(const BrkFricTqAtWhlActRec& v);
template <> BrkFricTqAtWhlActRec fromJson<BrkFricTqAtWhlActRec>(const json& j);

// ==========================================
// CamIndReq
template <> json toJson<CamIndReq>(const CamIndReq& v);
template <> CamIndReq fromJson<CamIndReq>(const json& j);

// ==========================================
// CchForFuEco1
template <> json toJson<CchForFuEco1>(const CchForFuEco1& v);
template <> CchForFuEco1 fromJson<CchForFuEco1>(const json& j);

// ==========================================
// CllsnWarnSide1
template <> json toJson<CllsnWarnSide1>(const CllsnWarnSide1& v);
template <> CllsnWarnSide1 fromJson<CllsnWarnSide1>(const json& j);

// ==========================================
// CmptmtAirTEstimdExtd
template <> json toJson<CmptmtAirTEstimdExtd>(const CmptmtAirTEstimdExtd& v);
template <> CmptmtAirTEstimdExtd fromJson<CmptmtAirTEstimdExtd>(const json& j);

// ==========================================
// CmptmtTFrnt
template <> json toJson<CmptmtTFrnt>(const CmptmtTFrnt& v);
template <> CmptmtTFrnt fromJson<CmptmtTFrnt>(const json& j);

// ==========================================
// ConSftyWarn1
template <> json toJson<ConSftyWarn1>(const ConSftyWarn1& v);
template <> ConSftyWarn1 fromJson<ConSftyWarn1>(const json& j);

// ==========================================
// DataSpclTyp
template <> json toJson<DataSpclTyp>(const DataSpclTyp& v);
template <> DataSpclTyp fromJson<DataSpclTyp>(const json& j);

// ==========================================
// DateTi1ForSet
template <> json toJson<DateTi1ForSet>(const DateTi1ForSet& v);
template <> DateTi1ForSet fromJson<DateTi1ForSet>(const json& j);

// ==========================================
// DateTi30
template <> json toJson<DateTi30>(const DateTi30& v);
template <> DateTi30 fromJson<DateTi30>(const json& j);

// ==========================================
// DriveAwayInfoWarnReq1
template <> json toJson<DriveAwayInfoWarnReq1>(const DriveAwayInfoWarnReq1& v);
template <> DriveAwayInfoWarnReq1 fromJson<DriveAwayInfoWarnReq1>(const json& j);

// ==========================================
// DrvrDispSetgReq
template <> json toJson<DrvrDispSetgReq>(const DrvrDispSetgReq& v);
template <> DrvrDispSetgReq fromJson<DrvrDispSetgReq>(const json& j);

// ==========================================
// DrvrHmiBackGndInfoSetg
template <> json toJson<DrvrHmiBackGndInfoSetg>(const DrvrHmiBackGndInfoSetg& v);
template <> DrvrHmiBackGndInfoSetg fromJson<DrvrHmiBackGndInfoSetg>(const json& j);

// ==========================================
// DrvrHmiDispdModPen
template <> json toJson<DrvrHmiDispdModPen>(const DrvrHmiDispdModPen& v);
template <> DrvrHmiDispdModPen fromJson<DrvrHmiDispdModPen>(const json& j);

// ==========================================
// std::array<uint8_t,254>
template <> json toJson<std::array<uint8_t,254>>(const std::array<uint8_t,254>& v);
template <> std::array<uint8_t,254> fromJson<std::array<uint8_t,254>>(const json& j);

// ==========================================
// DrvrHmiUserIfSetgReq
template <> json toJson<DrvrHmiUserIfSetgReq>(const DrvrHmiUserIfSetgReq& v);
template <> DrvrHmiUserIfSetgReq fromJson<DrvrHmiUserIfSetgReq>(const json& j);

// ==========================================
// DrvrSeatSwtSts1
template <> json toJson<DrvrSeatSwtSts1>(const DrvrSeatSwtSts1& v);
template <> DrvrSeatSwtSts1 fromJson<DrvrSeatSwtSts1>(const json& j);

// ==========================================
// DstToEmptyWithUnit
template <> json toJson<DstToEmptyWithUnit>(const DstToEmptyWithUnit& v);
template <> DstToEmptyWithUnit fromJson<DstToEmptyWithUnit>(const json& j);

// ==========================================
// DstToManvLocnByNav
template <> json toJson<DstToManvLocnByNav>(const DstToManvLocnByNav& v);
template <> DstToManvLocnByNav fromJson<DstToManvLocnByNav>(const json& j);

// ==========================================
// EngIdleEco1
template <> json toJson<EngIdleEco1>(const EngIdleEco1& v);
template <> EngIdleEco1 fromJson<EngIdleEco1>(const json& j);

// ==========================================
// EngNSafe
template <> json toJson<EngNSafe>(const EngNSafe& v);
template <> EngNSafe fromJson<EngNSafe>(const json& j);

// ==========================================
// EpbLampReqRec
template <> json toJson<EpbLampReqRec>(const EpbLampReqRec& v);
template <> EpbLampReqRec fromJson<EpbLampReqRec>(const json& j);

// ==========================================
// EscSptModReqdByDrvrRec1
template <> json toJson<EscSptModReqdByDrvrRec1>(const EscSptModReqdByDrvrRec1& v);
template <> EscSptModReqdByDrvrRec1 fromJson<EscSptModReqdByDrvrRec1>(const json& j);

// ==========================================
// EscStSafe1
template <> json toJson<EscStSafe1>(const EscStSafe1& v);
template <> EscStSafe1 fromJson<EscStSafe1>(const json& j);

// ==========================================
// EscWarnIndcnReqRec1
template <> json toJson<EscWarnIndcnReqRec1>(const EscWarnIndcnReqRec1& v);
template <> EscWarnIndcnReqRec1 fromJson<EscWarnIndcnReqRec1>(const json& j);

// ==========================================
// ExtrMirrTiltSetg
template <> json toJson<ExtrMirrTiltSetg>(const ExtrMirrTiltSetg& v);
template <> ExtrMirrTiltSetg fromJson<ExtrMirrTiltSetg>(const json& j);

// ==========================================
// FrntWiprLvrReq2
template <> json toJson<FrntWiprLvrReq2>(const FrntWiprLvrReq2& v);
template <> FrntWiprLvrReq2 fromJson<FrntWiprLvrReq2>(const json& j);

// ==========================================
// FuLvlValWithQly
template <> json toJson<FuLvlValWithQly>(const FuLvlValWithQly& v);
template <> FuLvlValWithQly fromJson<FuLvlValWithQly>(const json& j);

// ==========================================
// GearIndcnRec2
template <> json toJson<GearIndcnRec2>(const GearIndcnRec2& v);
template <> GearIndcnRec2 fromJson<GearIndcnRec2>(const json& j);

// ==========================================
// GlbRstForSetgAndData
template <> json toJson<GlbRstForSetgAndData>(const GlbRstForSetgAndData& v);
template <> GlbRstForSetgAndData fromJson<GlbRstForSetgAndData>(const json& j);

// ==========================================
// HmiCmptmtTSp
template <> json toJson<HmiCmptmtTSp>(const HmiCmptmtTSp& v);
template <> HmiCmptmtTSp fromJson<HmiCmptmtTSp>(const json& j);

// ==========================================
// HmiDefrstElecReq
template <> json toJson<HmiDefrstElecReq>(const HmiDefrstElecReq& v);
template <> HmiDefrstElecReq fromJson<HmiDefrstElecReq>(const json& j);

// ==========================================
// HmiDefrstElecSts
template <> json toJson<HmiDefrstElecSts>(const HmiDefrstElecSts& v);
template <> HmiDefrstElecSts fromJson<HmiDefrstElecSts>(const json& j);

// ==========================================
// HmiSeatClima
template <> json toJson<HmiSeatClima>(const HmiSeatClima& v);
template <> HmiSeatClima fromJson<HmiSeatClima>(const json& j);

// ==========================================
// HmiSeatClimaExtd
template <> json toJson<HmiSeatClimaExtd>(const HmiSeatClimaExtd& v);
template <> HmiSeatClimaExtd fromJson<HmiSeatClimaExtd>(const json& j);

// ==========================================
// HudDiagc
template <> json toJson<HudDiagc>(const HudDiagc& v);
template <> HudDiagc fromJson<HudDiagc>(const json& j);

// ==========================================
// HudVisFctSetgReq
template <> json toJson<HudVisFctSetgReq>(const HudVisFctSetgReq& v);
template <> HudVisFctSetgReq fromJson<HudVisFctSetgReq>(const json& j);

// ==========================================
// HwAprvdWirelsAdr1
template <> json toJson<HwAprvdWirelsAdr1>(const HwAprvdWirelsAdr1& v);
template <> HwAprvdWirelsAdr1 fromJson<HwAprvdWirelsAdr1>(const json& j);

// ==========================================
// HznDataGroup2
template <> json toJson<HznDataGroup2>(const HznDataGroup2& v);
template <> HznDataGroup2 fromJson<HznDataGroup2>(const json& j);

// ==========================================
// HznEdgeGroup2
template <> json toJson<HznEdgeGroup2>(const HznEdgeGroup2& v);
template <> HznEdgeGroup2 fromJson<HznEdgeGroup2>(const json& j);

// ==========================================
// HznPosnExtdGroup1
template <> json toJson<HznPosnExtdGroup1>(const HznPosnExtdGroup1& v);
template <> HznPosnExtdGroup1 fromJson<HznPosnExtdGroup1>(const json& j);

// ==========================================
// HznPosnExtdOffs
template <> json toJson<HznPosnExtdOffs>(const HznPosnExtdOffs& v);
template <> HznPosnExtdOffs fromJson<HznPosnExtdOffs>(const json& j);

// ==========================================
// HznPosnGroup3
template <> json toJson<HznPosnGroup3>(const HznPosnGroup3& v);
template <> HznPosnGroup3 fromJson<HznPosnGroup3>(const json& j);

// ==========================================
// HznProfLongExtdGroup1
template <> json toJson<HznProfLongExtdGroup1>(const HznProfLongExtdGroup1& v);
template <> HznProfLongExtdGroup1 fromJson<HznProfLongExtdGroup1>(const json& j);

// ==========================================
// HznProfLongGroup3
template <> json toJson<HznProfLongGroup3>(const HznProfLongGroup3& v);
template <> HznProfLongGroup3 fromJson<HznProfLongGroup3>(const json& j);

// ==========================================
// HznProfSho2
template <> json toJson<HznProfSho2>(const HznProfSho2& v);
template <> HznProfSho2 fromJson<HznProfSho2>(const json& j);

// ==========================================
// HznSegGroup2
template <> json toJson<HznSegGroup2>(const HznSegGroup2& v);
template <> HznSegGroup2 fromJson<HznSegGroup2>(const json& j);

// ==========================================
// IndcnUnit
template <> json toJson<IndcnUnit>(const IndcnUnit& v);
template <> IndcnUnit fromJson<IndcnUnit>(const json& j);

// ==========================================
// IntrLiAmbLiSetg
template <> json toJson<IntrLiAmbLiSetg>(const IntrLiAmbLiSetg& v);
template <> IntrLiAmbLiSetg fromJson<IntrLiAmbLiSetg>(const json& j);

// ==========================================
// IntrLiSurrndgsLiSetg
template <> json toJson<IntrLiSurrndgsLiSetg>(const IntrLiSurrndgsLiSetg& v);
template <> IntrLiSurrndgsLiSetg fromJson<IntrLiSurrndgsLiSetg>(const json& j);

// ==========================================
// KeyProfMpgUpd1
template <> json toJson<KeyProfMpgUpd1>(const KeyProfMpgUpd1& v);
template <> KeyProfMpgUpd1 fromJson<KeyProfMpgUpd1>(const json& j);

// ==========================================
// KeyReadStsToProfCtrl
template <> json toJson<KeyReadStsToProfCtrl>(const KeyReadStsToProfCtrl& v);
template <> KeyReadStsToProfCtrl fromJson<KeyReadStsToProfCtrl>(const json& j);

// ==========================================
// LiAutTranPen1
template <> json toJson<LiAutTranPen1>(const LiAutTranPen1& v);
template <> LiAutTranPen1 fromJson<LiAutTranPen1>(const json& j);

// ==========================================
// LiTiPen2
template <> json toJson<LiTiPen2>(const LiTiPen2& v);
template <> LiTiPen2 fromJson<LiTiPen2>(const json& j);

// ==========================================
// LockgCenSts3
template <> json toJson<LockgCenSts3>(const LockgCenSts3& v);
template <> LockgCenSts3 fromJson<LockgCenSts3>(const json& j);

// ==========================================
// LvlSeldForSpdLimAdpvPen1
template <> json toJson<LvlSeldForSpdLimAdpvPen1>(const LvlSeldForSpdLimAdpvPen1& v);
template <> LvlSeldForSpdLimAdpvPen1 fromJson<LvlSeldForSpdLimAdpvPen1>(const json& j);

// ==========================================
// MassgFctActv
template <> json toJson<MassgFctActv>(const MassgFctActv& v);
template <> MassgFctActv fromJson<MassgFctActv>(const json& j);

// ==========================================
// MirrDimPen
template <> json toJson<MirrDimPen>(const MirrDimPen& v);
template <> MirrDimPen fromJson<MirrDimPen>(const json& j);

// ==========================================
// MtrlSnsrT
template <> json toJson<MtrlSnsrT>(const MtrlSnsrT& v);
template <> MtrlSnsrT fromJson<MtrlSnsrT>(const json& j);

// ==========================================
// NetActvtRec1
template <> json toJson<NetActvtRec1>(const NetActvtRec1& v);
template <> NetActvtRec1 fromJson<NetActvtRec1>(const json& j);

// ==========================================
// NormSptPen
template <> json toJson<NormSptPen>(const NormSptPen& v);
template <> NormSptPen fromJson<NormSptPen>(const json& j);

// ==========================================
// NrSerlNodLIN
template <> json toJson<NrSerlNodLIN>(const NrSerlNodLIN& v);
template <> NrSerlNodLIN fromJson<NrSerlNodLIN>(const json& j);

// ==========================================
// OffsForSpdWarnSetgPen
template <> json toJson<OffsForSpdWarnSetgPen>(const OffsForSpdWarnSetgPen& v);
template <> OffsForSpdWarnSetgPen fromJson<OffsForSpdWarnSetgPen>(const json& j);

// ==========================================
// OnOffPen
template <> json toJson<OnOffPen>(const OnOffPen& v);
template <> OnOffPen fromJson<OnOffPen>(const json& j);

// ==========================================
// PartNrNodLIN
template <> json toJson<PartNrNodLIN>(const PartNrNodLIN& v);
template <> PartNrNodLIN fromJson<PartNrNodLIN>(const json& j);

// ==========================================
// PassSeatSwtSts2
template <> json toJson<PassSeatSwtSts2>(const PassSeatSwtSts2& v);
template <> PassSeatSwtSts2 fromJson<PassSeatSwtSts2>(const json& j);

// ==========================================
// PinionSteerAg1Rec
template <> json toJson<PinionSteerAg1Rec>(const PinionSteerAg1Rec& v);
template <> PinionSteerAg1Rec fromJson<PinionSteerAg1Rec>(const json& j);

// ==========================================
// PosnFromNav
template <> json toJson<PosnFromNav>(const PosnFromNav& v);
template <> PosnFromNav fromJson<PosnFromNav>(const json& j);

// ==========================================
// PosnFromSatlt
template <> json toJson<PosnFromSatlt>(const PosnFromSatlt& v);
template <> PosnFromSatlt fromJson<PosnFromSatlt>(const json& j);

// ==========================================
// PrkgAssiAudWarn
template <> json toJson<PrkgAssiAudWarn>(const PrkgAssiAudWarn& v);
template <> PrkgAssiAudWarn fromJson<PrkgAssiAudWarn>(const json& j);

// ==========================================
// ProfAct1
template <> json toJson<ProfAct1>(const ProfAct1& v);
template <> ProfAct1 fromJson<ProfAct1>(const json& j);

// ==========================================
// ProfLimd1
template <> json toJson<ProfLimd1>(const ProfLimd1& v);
template <> ProfLimd1 fromJson<ProfLimd1>(const json& j);

// ==========================================
// PtCluTq1
template <> json toJson<PtCluTq1>(const PtCluTq1& v);
template <> PtCluTq1 fromJson<PtCluTq1>(const json& j);

// ==========================================
// PtDrvrSetg2
template <> json toJson<PtDrvrSetg2>(const PtDrvrSetg2& v);
template <> PtDrvrSetg2 fromJson<PtDrvrSetg2>(const json& j);

// ==========================================
// PtTqAtWhlFrntActRec1
template <> json toJson<PtTqAtWhlFrntActRec1>(const PtTqAtWhlFrntActRec1& v);
template <> PtTqAtWhlFrntActRec1 fromJson<PtTqAtWhlFrntActRec1>(const json& j);

// ==========================================
// RngBdIllmnCmdPen1
template <> json toJson<RngBdIllmnCmdPen1>(const RngBdIllmnCmdPen1& v);
template <> RngBdIllmnCmdPen1 fromJson<RngBdIllmnCmdPen1>(const json& j);

// ==========================================
// RsdsSysSts
template <> json toJson<RsdsSysSts>(const RsdsSysSts& v);
template <> RsdsSysSts fromJson<RsdsSysSts>(const json& j);

// ==========================================
// SeatBackUnlckd
template <> json toJson<SeatBackUnlckd>(const SeatBackUnlckd& v);
template <> SeatBackUnlckd fromJson<SeatBackUnlckd>(const json& j);

// ==========================================
// SeatDispBtnPsd
template <> json toJson<SeatDispBtnPsd>(const SeatDispBtnPsd& v);
template <> SeatDispBtnPsd fromJson<SeatDispBtnPsd>(const json& j);

// ==========================================
// SeatMassgFct
template <> json toJson<SeatMassgFct>(const SeatMassgFct& v);
template <> SeatMassgFct fromJson<SeatMassgFct>(const json& j);

// ==========================================
// SetOfLang
template <> json toJson<SetOfLang>(const SetOfLang& v);
template <> SetOfLang fromJson<SetOfLang>(const json& j);

// ==========================================
// SftyCchActvnSts1
template <> json toJson<SftyCchActvnSts1>(const SftyCchActvnSts1& v);
template <> SftyCchActvnSts1 fromJson<SftyCchActvnSts1>(const json& j);

// ==========================================
// SftyCchIdx1
template <> json toJson<SftyCchIdx1>(const SftyCchIdx1& v);
template <> SftyCchIdx1 fromJson<SftyCchIdx1>(const json& j);

// ==========================================
// SftyCchIdx2
template <> json toJson<SftyCchIdx2>(const SftyCchIdx2& v);
template <> SftyCchIdx2 fromJson<SftyCchIdx2>(const json& j);

// ==========================================
// SftySigGroupFromAudSafe1
template <> json toJson<SftySigGroupFromAudSafe1>(const SftySigGroupFromAudSafe1& v);
template <> SftySigGroupFromAudSafe1 fromJson<SftySigGroupFromAudSafe1>(const json& j);

// ==========================================
// SftyWarnGroupFromAsySafe1
template <> json toJson<SftyWarnGroupFromAsySafe1>(const SftyWarnGroupFromAsySafe1& v);
template <> SftyWarnGroupFromAsySafe1 fromJson<SftyWarnGroupFromAsySafe1>(const json& j);

// ==========================================
// SlowNormFastPen
template <> json toJson<SlowNormFastPen>(const SlowNormFastPen& v);
template <> SlowNormFastPen fromJson<SlowNormFastPen>(const json& j);

// ==========================================
// SnsrParkAssi1
template <> json toJson<SnsrParkAssi1>(const SnsrParkAssi1& v);
template <> SnsrParkAssi1 fromJson<SnsrParkAssi1>(const json& j);

// ==========================================
// SnsrPrkgAssi2
template <> json toJson<SnsrPrkgAssi2>(const SnsrPrkgAssi2& v);
template <> SnsrPrkgAssi2 fromJson<SnsrPrkgAssi2>(const json& j);

// ==========================================
// SnsrPrkgAssi3
template <> json toJson<SnsrPrkgAssi3>(const SnsrPrkgAssi3& v);
template <> SnsrPrkgAssi3 fromJson<SnsrPrkgAssi3>(const json& j);

// ==========================================
// SnvtyPen1
template <> json toJson<SnvtyPen1>(const SnvtyPen1& v);
template <> SnvtyPen1 fromJson<SnvtyPen1>(const json& j);

// ==========================================
// SnvtyPen3
template <> json toJson<SnvtyPen3>(const SnvtyPen3& v);
template <> SnvtyPen3 fromJson<SnvtyPen3>(const json& j);

// ==========================================
// SteerAssiLvlForAutDrvPen1
template <> json toJson<SteerAssiLvlForAutDrvPen1>(const SteerAssiLvlForAutDrvPen1& v);
template <> SteerAssiLvlForAutDrvPen1 fromJson<SteerAssiLvlForAutDrvPen1>(const json& j);

// ==========================================
// SteerSetg
template <> json toJson<SteerSetg>(const SteerSetg& v);
template <> SteerSetg fromJson<SteerSetg>(const json& j);

// ==========================================
// SteerWhlSnsrSafe1
template <> json toJson<SteerWhlSnsrSafe1>(const SteerWhlSnsrSafe1& v);
template <> SteerWhlSnsrSafe1 fromJson<SteerWhlSnsrSafe1>(const json& j);

// ==========================================
// SuspSetgRec
template <> json toJson<SuspSetgRec>(const SuspSetgRec& v);
template <> SuspSetgRec fromJson<SuspSetgRec>(const json& j);

// ==========================================
// SysU
template <> json toJson<SysU>(const SysU& v);
template <> SysU fromJson<SysU>(const json& j);

// ==========================================
// TiCorrn
template <> json toJson<TiCorrn>(const TiCorrn& v);
template <> TiCorrn fromJson<TiCorrn>(const json& j);

// ==========================================
// TiGapAdpvSeldPen1
template <> json toJson<TiGapAdpvSeldPen1>(const TiGapAdpvSeldPen1& v);
template <> TiGapAdpvSeldPen1 fromJson<TiGapAdpvSeldPen1>(const json& j);

// ==========================================
// TiGapPen1
template <> json toJson<TiGapPen1>(const TiGapPen1& v);
template <> TiGapPen1 fromJson<TiGapPen1>(const json& j);

// ==========================================
// TireCircumCalByNav
template <> json toJson<TireCircumCalByNav>(const TireCircumCalByNav& v);
template <> TireCircumCalByNav fromJson<TireCircumCalByNav>(const json& j);

// ==========================================
// TirePAbsltValFrntLe3
template <> json toJson<TirePAbsltValFrntLe3>(const TirePAbsltValFrntLe3& v);
template <> TirePAbsltValFrntLe3 fromJson<TirePAbsltValFrntLe3>(const json& j);

// ==========================================
// TirePAbsltValFrntRi1
template <> json toJson<TirePAbsltValFrntRi1>(const TirePAbsltValFrntRi1& v);
template <> TirePAbsltValFrntRi1 fromJson<TirePAbsltValFrntRi1>(const json& j);

// ==========================================
// TirePAbsltValReLe1
template <> json toJson<TirePAbsltValReLe1>(const TirePAbsltValReLe1& v);
template <> TirePAbsltValReLe1 fromJson<TirePAbsltValReLe1>(const json& j);

// ==========================================
// TirePAbsltValReRi1
template <> json toJson<TirePAbsltValReRi1>(const TirePAbsltValReRi1& v);
template <> TirePAbsltValReRi1 fromJson<TirePAbsltValReRi1>(const json& j);

// ==========================================
// TirePMonData1
template <> json toJson<TirePMonData1>(const TirePMonData1& v);
template <> TirePMonData1 fromJson<TirePMonData1>(const json& j);

// ==========================================
// TqSafe2
template <> json toJson<TqSafe2>(const TqSafe2& v);
template <> TqSafe2 fromJson<TqSafe2>(const json& j);

// ==========================================
// TwliBriRaw
template <> json toJson<TwliBriRaw>(const TwliBriRaw& v);
template <> TwliBriRaw fromJson<TwliBriRaw>(const json& j);

// ==========================================
// UInt64Rec
template <> json toJson<UInt64Rec>(const UInt64Rec& v);
template <> UInt64Rec fromJson<UInt64Rec>(const json& j);

// ==========================================
// UnlckKeylsCfgPen2
template <> json toJson<UnlckKeylsCfgPen2>(const UnlckKeylsCfgPen2& v);
template <> UnlckKeylsCfgPen2 fromJson<UnlckKeylsCfgPen2>(const json& j);

// ==========================================
// UnlckRemCfgPen1
template <> json toJson<UnlckRemCfgPen1>(const UnlckRemCfgPen1& v);
template <> UnlckRemCfgPen1 fromJson<UnlckRemCfgPen1>(const json& j);

// ==========================================
// UsrSetSpdForKeySpdLimn
template <> json toJson<UsrSetSpdForKeySpdLimn>(const UsrSetSpdForKeySpdLimn& v);
template <> UsrSetSpdForKeySpdLimn fromJson<UsrSetSpdForKeySpdLimn>(const json& j);

// ==========================================
// UsrSetSpdForKeySpdWarn
template <> json toJson<UsrSetSpdForKeySpdWarn>(const UsrSetSpdForKeySpdWarn& v);
template <> UsrSetSpdForKeySpdWarn fromJson<UsrSetSpdForKeySpdWarn>(const json& j);

// ==========================================
// UsrSetVolMaxForKeyVolLimn
template <> json toJson<UsrSetVolMaxForKeyVolLimn>(const UsrSetVolMaxForKeyVolLimn& v);
template <> UsrSetVolMaxForKeyVolLimn fromJson<UsrSetVolMaxForKeyVolLimn>(const json& j);

// ==========================================
// UsrSwtDispClimaReqForRowSec
template <> json toJson<UsrSwtDispClimaReqForRowSec>(const UsrSwtDispClimaReqForRowSec& v);
template <> UsrSwtDispClimaReqForRowSec fromJson<UsrSwtDispClimaReqForRowSec>(const json& j);

// ==========================================
// UsrSwtDispClimaSts
template <> json toJson<UsrSwtDispClimaSts>(const UsrSwtDispClimaSts& v);
template <> UsrSwtDispClimaSts fromJson<UsrSwtDispClimaSts>(const json& j);

// ==========================================
// UsrSwtDispForSecRowSeatVentn
template <> json toJson<UsrSwtDispForSecRowSeatVentn>(const UsrSwtDispForSecRowSeatVentn& v);
template <> UsrSwtDispForSecRowSeatVentn fromJson<UsrSwtDispForSecRowSeatVentn>(const json& j);

// ==========================================
// UsrSwtDispReqForSecRowSeatVentn
template <> json toJson<UsrSwtDispReqForSecRowSeatVentn>(const UsrSwtDispReqForSecRowSeatVentn& v);
template <> UsrSwtDispReqForSecRowSeatVentn fromJson<UsrSwtDispReqForSecRowSeatVentn>(const json& j);

// ==========================================
// UsrSwtDispReqVrnt
template <> json toJson<UsrSwtDispReqVrnt>(const UsrSwtDispReqVrnt& v);
template <> UsrSwtDispReqVrnt fromJson<UsrSwtDispReqVrnt>(const json& j);

// ==========================================
// VFCGrp
template <> json toJson<VFCGrp>(const VFCGrp& v);
template <> VFCGrp fromJson<VFCGrp>(const json& j);

// ==========================================
// VehCfgPrm
template <> json toJson<VehCfgPrm>(const VehCfgPrm& v);
template <> VehCfgPrm fromJson<VehCfgPrm>(const json& j);

// ==========================================
// VehMNomRec1
template <> json toJson<VehMNomRec1>(const VehMNomRec1& v);
template <> VehMNomRec1 fromJson<VehMNomRec1>(const json& j);

// ==========================================
// VehModMngtGlbSafe1
template <> json toJson<VehModMngtGlbSafe1>(const VehModMngtGlbSafe1& v);
template <> VehModMngtGlbSafe1 fromJson<VehModMngtGlbSafe1>(const json& j);

// ==========================================
// VehMtnStSafe1
template <> json toJson<VehMtnStSafe1>(const VehMtnStSafe1& v);
template <> VehMtnStSafe1 fromJson<VehMtnStSafe1>(const json& j);

// ==========================================
// VehSpdIndcd1
template <> json toJson<VehSpdIndcd1>(const VehSpdIndcd1& v);
template <> VehSpdIndcd1 fromJson<VehSpdIndcd1>(const json& j);

// ==========================================
// VehSpdLgtSafe1
template <> json toJson<VehSpdLgtSafe1>(const VehSpdLgtSafe1& v);
template <> VehSpdLgtSafe1 fromJson<VehSpdLgtSafe1>(const json& j);

// ==========================================
// Vin1
template <> json toJson<Vin1>(const Vin1& v);
template <> Vin1 fromJson<Vin1>(const json& j);

// ==========================================
// VisnAgWideCfg1
template <> json toJson<VisnAgWideCfg1>(const VisnAgWideCfg1& v);
template <> VisnAgWideCfg1 fromJson<VisnAgWideCfg1>(const json& j);

// ==========================================
// WarnAndIntvPen1
template <> json toJson<WarnAndIntvPen1>(const WarnAndIntvPen1& v);
template <> WarnAndIntvPen1 fromJson<WarnAndIntvPen1>(const json& j);

// ==========================================
// WarnTypForLaneChgWarnPen1
template <> json toJson<WarnTypForLaneChgWarnPen1>(const WarnTypForLaneChgWarnPen1& v);
template <> WarnTypForLaneChgWarnPen1 fromJson<WarnTypForLaneChgWarnPen1>(const json& j);

// ==========================================
// WhlRotToothCntrRec
template <> json toJson<WhlRotToothCntrRec>(const WhlRotToothCntrRec& v);
template <> WhlRotToothCntrRec fromJson<WhlRotToothCntrRec>(const json& j);

// ==========================================
// WipgInfo
template <> json toJson<WipgInfo>(const WipgInfo& v);
template <> WipgInfo fromJson<WipgInfo>(const json& j);

} // end of namespace
#endif
// clang-format on
