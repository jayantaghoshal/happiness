/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
// C++ code generator for AUTOSAR v1.0
// Generated at: 2016-04-28T11:06:19.388754
// Source: SPA1610_IHUVOLVO8_151214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml
#ifndef _JSONENCDEC_H
#define _JSONENCDEC_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QVariant>
#include <QDebug>
#include <type_traits>

#include "gen_datatypes.h"

namespace autosar {

// handles all enum types
template<typename T> using AREnum = typename std::enable_if<std::is_enum<T>::value, T>::type;

// handles everything EXCEPT enum (and the explicits below)
template<typename T> using AllButAREnum = typename std::enable_if<!std::is_enum<T>::value, T>::type;

template <typename T>
QJsonValue toJson(AREnum<T> e) {
    return QJsonValue((int)e);
}

template <typename T>
AREnum<T> fromJson(const QJsonValue& j) {
    if (!j.isDouble()) {
        // TODO exception
        return T();
    }
    return static_cast<T>( j.toInt() );
}

// handles everything EXCEPT enum (and the explicits below)
template <typename T>
QJsonValue toJson(const AllButAREnum<T>& e) {
    return QJsonValue(e);
}

// Needed specialization since QJsonValue does not handle uint32 (unsigned int)
template <> QJsonValue toJson<uint32_t>(const uint32_t& e);

template <typename T>
AllButAREnum<T> fromJson(const QJsonValue& j) {
    if ( j.isArray() || j.isNull() || j.isObject() || j.isUndefined() ) {
        // TODO exception
        return T();
    }
    QVariant v = j.toVariant();
    return v.value<T>();
}


// ==========================================
// ADataRawSafe1
template <> QJsonValue toJson<ADataRawSafe1>(const ADataRawSafe1& v);
template <> ADataRawSafe1 fromJson<ADataRawSafe1>(const QJsonValue& j);

// ==========================================
// ActvnOfCrsEco
template <> QJsonValue toJson<ActvnOfCrsEco>(const ActvnOfCrsEco& v);
template <> ActvnOfCrsEco fromJson<ActvnOfCrsEco>(const QJsonValue& j);

// ==========================================
// AdprTurnSpdModPen1
template <> QJsonValue toJson<AdprTurnSpdModPen1>(const AdprTurnSpdModPen1& v);
template <> AdprTurnSpdModPen1 fromJson<AdprTurnSpdModPen1>(const QJsonValue& j);

// ==========================================
// AgDataRawSafe1
template <> QJsonValue toJson<AgDataRawSafe1>(const AgDataRawSafe1& v);
template <> AgDataRawSafe1 fromJson<AgDataRawSafe1>(const QJsonValue& j);

// ==========================================
// AmbTIndcdWithUnit
template <> QJsonValue toJson<AmbTIndcdWithUnit>(const AmbTIndcdWithUnit& v);
template <> AmbTIndcdWithUnit fromJson<AmbTIndcdWithUnit>(const QJsonValue& j);

// ==========================================
// AmbTWithQly
template <> QJsonValue toJson<AmbTWithQly>(const AmbTWithQly& v);
template <> AmbTWithQly fromJson<AmbTWithQly>(const QJsonValue& j);

// ==========================================
// std::array<uint8_t,8>
template <> QJsonValue toJson<std::array<uint8_t,8>>(const std::array<uint8_t,8>& v);
template <> std::array<uint8_t,8> fromJson<std::array<uint8_t,8>>(const QJsonValue& j);

// ==========================================
// BltLockStFrnt
template <> QJsonValue toJson<BltLockStFrnt>(const BltLockStFrnt& v);
template <> BltLockStFrnt fromJson<BltLockStFrnt>(const QJsonValue& j);

// ==========================================
// BltLockStRe
template <> QJsonValue toJson<BltLockStRe>(const BltLockStRe& v);
template <> BltLockStRe fromJson<BltLockStRe>(const QJsonValue& j);

// ==========================================
// BrkAndAbsWarnIndcnReqRec1
template <> QJsonValue toJson<BrkAndAbsWarnIndcnReqRec1>(const BrkAndAbsWarnIndcnReqRec1& v);
template <> BrkAndAbsWarnIndcnReqRec1 fromJson<BrkAndAbsWarnIndcnReqRec1>(const QJsonValue& j);

// ==========================================
// BrkFricTqAtWhlActRec
template <> QJsonValue toJson<BrkFricTqAtWhlActRec>(const BrkFricTqAtWhlActRec& v);
template <> BrkFricTqAtWhlActRec fromJson<BrkFricTqAtWhlActRec>(const QJsonValue& j);

// ==========================================
// CamIndReq
template <> QJsonValue toJson<CamIndReq>(const CamIndReq& v);
template <> CamIndReq fromJson<CamIndReq>(const QJsonValue& j);

// ==========================================
// CchForFuEco1
template <> QJsonValue toJson<CchForFuEco1>(const CchForFuEco1& v);
template <> CchForFuEco1 fromJson<CchForFuEco1>(const QJsonValue& j);

// ==========================================
// CmptmtAirTEstimdExtd
template <> QJsonValue toJson<CmptmtAirTEstimdExtd>(const CmptmtAirTEstimdExtd& v);
template <> CmptmtAirTEstimdExtd fromJson<CmptmtAirTEstimdExtd>(const QJsonValue& j);

// ==========================================
// CmptmtTFrnt
template <> QJsonValue toJson<CmptmtTFrnt>(const CmptmtTFrnt& v);
template <> CmptmtTFrnt fromJson<CmptmtTFrnt>(const QJsonValue& j);

// ==========================================
// ConSftyWarn1
template <> QJsonValue toJson<ConSftyWarn1>(const ConSftyWarn1& v);
template <> ConSftyWarn1 fromJson<ConSftyWarn1>(const QJsonValue& j);

// ==========================================
// DataSpclTyp
template <> QJsonValue toJson<DataSpclTyp>(const DataSpclTyp& v);
template <> DataSpclTyp fromJson<DataSpclTyp>(const QJsonValue& j);

// ==========================================
// DateTi1ForSet
template <> QJsonValue toJson<DateTi1ForSet>(const DateTi1ForSet& v);
template <> DateTi1ForSet fromJson<DateTi1ForSet>(const QJsonValue& j);

// ==========================================
// DateTi30
template <> QJsonValue toJson<DateTi30>(const DateTi30& v);
template <> DateTi30 fromJson<DateTi30>(const QJsonValue& j);

// ==========================================
// DrvrDispSetgReq
template <> QJsonValue toJson<DrvrDispSetgReq>(const DrvrDispSetgReq& v);
template <> DrvrDispSetgReq fromJson<DrvrDispSetgReq>(const QJsonValue& j);

// ==========================================
// DrvrHmiBackGndInfoSetg
template <> QJsonValue toJson<DrvrHmiBackGndInfoSetg>(const DrvrHmiBackGndInfoSetg& v);
template <> DrvrHmiBackGndInfoSetg fromJson<DrvrHmiBackGndInfoSetg>(const QJsonValue& j);

// ==========================================
// DrvrHmiDispdModPen
template <> QJsonValue toJson<DrvrHmiDispdModPen>(const DrvrHmiDispdModPen& v);
template <> DrvrHmiDispdModPen fromJson<DrvrHmiDispdModPen>(const QJsonValue& j);

// ==========================================
// std::array<uint8_t,254>
template <> QJsonValue toJson<std::array<uint8_t,254>>(const std::array<uint8_t,254>& v);
template <> std::array<uint8_t,254> fromJson<std::array<uint8_t,254>>(const QJsonValue& j);

// ==========================================
// DrvrHmiUserIfSetgReq
template <> QJsonValue toJson<DrvrHmiUserIfSetgReq>(const DrvrHmiUserIfSetgReq& v);
template <> DrvrHmiUserIfSetgReq fromJson<DrvrHmiUserIfSetgReq>(const QJsonValue& j);

// ==========================================
// DrvrSeatSwtSts1
template <> QJsonValue toJson<DrvrSeatSwtSts1>(const DrvrSeatSwtSts1& v);
template <> DrvrSeatSwtSts1 fromJson<DrvrSeatSwtSts1>(const QJsonValue& j);

// ==========================================
// DstToEmptyWithUnit
template <> QJsonValue toJson<DstToEmptyWithUnit>(const DstToEmptyWithUnit& v);
template <> DstToEmptyWithUnit fromJson<DstToEmptyWithUnit>(const QJsonValue& j);

// ==========================================
// DstToManvLocnByNav
template <> QJsonValue toJson<DstToManvLocnByNav>(const DstToManvLocnByNav& v);
template <> DstToManvLocnByNav fromJson<DstToManvLocnByNav>(const QJsonValue& j);

// ==========================================
// EngIdleEco1
template <> QJsonValue toJson<EngIdleEco1>(const EngIdleEco1& v);
template <> EngIdleEco1 fromJson<EngIdleEco1>(const QJsonValue& j);

// ==========================================
// EngNSafe
template <> QJsonValue toJson<EngNSafe>(const EngNSafe& v);
template <> EngNSafe fromJson<EngNSafe>(const QJsonValue& j);

// ==========================================
// EpbLampReqRec
template <> QJsonValue toJson<EpbLampReqRec>(const EpbLampReqRec& v);
template <> EpbLampReqRec fromJson<EpbLampReqRec>(const QJsonValue& j);

// ==========================================
// EscSptModReqdByDrvrRec1
template <> QJsonValue toJson<EscSptModReqdByDrvrRec1>(const EscSptModReqdByDrvrRec1& v);
template <> EscSptModReqdByDrvrRec1 fromJson<EscSptModReqdByDrvrRec1>(const QJsonValue& j);

// ==========================================
// EscStSafe1
template <> QJsonValue toJson<EscStSafe1>(const EscStSafe1& v);
template <> EscStSafe1 fromJson<EscStSafe1>(const QJsonValue& j);

// ==========================================
// EscWarnIndcnReqRec1
template <> QJsonValue toJson<EscWarnIndcnReqRec1>(const EscWarnIndcnReqRec1& v);
template <> EscWarnIndcnReqRec1 fromJson<EscWarnIndcnReqRec1>(const QJsonValue& j);

// ==========================================
// ExtrMirrTiltSetg
template <> QJsonValue toJson<ExtrMirrTiltSetg>(const ExtrMirrTiltSetg& v);
template <> ExtrMirrTiltSetg fromJson<ExtrMirrTiltSetg>(const QJsonValue& j);

// ==========================================
// FrntWiprLvrReq2
template <> QJsonValue toJson<FrntWiprLvrReq2>(const FrntWiprLvrReq2& v);
template <> FrntWiprLvrReq2 fromJson<FrntWiprLvrReq2>(const QJsonValue& j);

// ==========================================
// FuLvlValWithQly
template <> QJsonValue toJson<FuLvlValWithQly>(const FuLvlValWithQly& v);
template <> FuLvlValWithQly fromJson<FuLvlValWithQly>(const QJsonValue& j);

// ==========================================
// GearIndcnRec2
template <> QJsonValue toJson<GearIndcnRec2>(const GearIndcnRec2& v);
template <> GearIndcnRec2 fromJson<GearIndcnRec2>(const QJsonValue& j);

// ==========================================
// GlbRstForSetgAndData
template <> QJsonValue toJson<GlbRstForSetgAndData>(const GlbRstForSetgAndData& v);
template <> GlbRstForSetgAndData fromJson<GlbRstForSetgAndData>(const QJsonValue& j);

// ==========================================
// HmiCmptmtTSp
template <> QJsonValue toJson<HmiCmptmtTSp>(const HmiCmptmtTSp& v);
template <> HmiCmptmtTSp fromJson<HmiCmptmtTSp>(const QJsonValue& j);

// ==========================================
// HmiDefrstElecReq
template <> QJsonValue toJson<HmiDefrstElecReq>(const HmiDefrstElecReq& v);
template <> HmiDefrstElecReq fromJson<HmiDefrstElecReq>(const QJsonValue& j);

// ==========================================
// HmiDefrstElecSts
template <> QJsonValue toJson<HmiDefrstElecSts>(const HmiDefrstElecSts& v);
template <> HmiDefrstElecSts fromJson<HmiDefrstElecSts>(const QJsonValue& j);

// ==========================================
// HmiSeatClima
template <> QJsonValue toJson<HmiSeatClima>(const HmiSeatClima& v);
template <> HmiSeatClima fromJson<HmiSeatClima>(const QJsonValue& j);

// ==========================================
// HmiSeatClimaExtd
template <> QJsonValue toJson<HmiSeatClimaExtd>(const HmiSeatClimaExtd& v);
template <> HmiSeatClimaExtd fromJson<HmiSeatClimaExtd>(const QJsonValue& j);

// ==========================================
// HudDiagc
template <> QJsonValue toJson<HudDiagc>(const HudDiagc& v);
template <> HudDiagc fromJson<HudDiagc>(const QJsonValue& j);

// ==========================================
// HudVisFctSetgReq
template <> QJsonValue toJson<HudVisFctSetgReq>(const HudVisFctSetgReq& v);
template <> HudVisFctSetgReq fromJson<HudVisFctSetgReq>(const QJsonValue& j);

// ==========================================
// HwAprvdWirelsAdr1
template <> QJsonValue toJson<HwAprvdWirelsAdr1>(const HwAprvdWirelsAdr1& v);
template <> HwAprvdWirelsAdr1 fromJson<HwAprvdWirelsAdr1>(const QJsonValue& j);

// ==========================================
// HznDataGroup2
template <> QJsonValue toJson<HznDataGroup2>(const HznDataGroup2& v);
template <> HznDataGroup2 fromJson<HznDataGroup2>(const QJsonValue& j);

// ==========================================
// HznEdgeGroup2
template <> QJsonValue toJson<HznEdgeGroup2>(const HznEdgeGroup2& v);
template <> HznEdgeGroup2 fromJson<HznEdgeGroup2>(const QJsonValue& j);

// ==========================================
// HznPosnExtdGroup1
template <> QJsonValue toJson<HznPosnExtdGroup1>(const HznPosnExtdGroup1& v);
template <> HznPosnExtdGroup1 fromJson<HznPosnExtdGroup1>(const QJsonValue& j);

// ==========================================
// HznPosnGroup3
template <> QJsonValue toJson<HznPosnGroup3>(const HznPosnGroup3& v);
template <> HznPosnGroup3 fromJson<HznPosnGroup3>(const QJsonValue& j);

// ==========================================
// HznProfLongExtdGroup1
template <> QJsonValue toJson<HznProfLongExtdGroup1>(const HznProfLongExtdGroup1& v);
template <> HznProfLongExtdGroup1 fromJson<HznProfLongExtdGroup1>(const QJsonValue& j);

// ==========================================
// HznProfLongGroup3
template <> QJsonValue toJson<HznProfLongGroup3>(const HznProfLongGroup3& v);
template <> HznProfLongGroup3 fromJson<HznProfLongGroup3>(const QJsonValue& j);

// ==========================================
// HznProfSho2
template <> QJsonValue toJson<HznProfSho2>(const HznProfSho2& v);
template <> HznProfSho2 fromJson<HznProfSho2>(const QJsonValue& j);

// ==========================================
// HznSegGroup2
template <> QJsonValue toJson<HznSegGroup2>(const HznSegGroup2& v);
template <> HznSegGroup2 fromJson<HznSegGroup2>(const QJsonValue& j);

// ==========================================
// IndcnUnit
template <> QJsonValue toJson<IndcnUnit>(const IndcnUnit& v);
template <> IndcnUnit fromJson<IndcnUnit>(const QJsonValue& j);

// ==========================================
// IntrLiAmbLiSetg
template <> QJsonValue toJson<IntrLiAmbLiSetg>(const IntrLiAmbLiSetg& v);
template <> IntrLiAmbLiSetg fromJson<IntrLiAmbLiSetg>(const QJsonValue& j);

// ==========================================
// IntrLiSurrndgsLiSetg
template <> QJsonValue toJson<IntrLiSurrndgsLiSetg>(const IntrLiSurrndgsLiSetg& v);
template <> IntrLiSurrndgsLiSetg fromJson<IntrLiSurrndgsLiSetg>(const QJsonValue& j);

// ==========================================
// KeyProfMpgUpd1
template <> QJsonValue toJson<KeyProfMpgUpd1>(const KeyProfMpgUpd1& v);
template <> KeyProfMpgUpd1 fromJson<KeyProfMpgUpd1>(const QJsonValue& j);

// ==========================================
// KeyReadStsToProfCtrl
template <> QJsonValue toJson<KeyReadStsToProfCtrl>(const KeyReadStsToProfCtrl& v);
template <> KeyReadStsToProfCtrl fromJson<KeyReadStsToProfCtrl>(const QJsonValue& j);

// ==========================================
// LiAutTranPen1
template <> QJsonValue toJson<LiAutTranPen1>(const LiAutTranPen1& v);
template <> LiAutTranPen1 fromJson<LiAutTranPen1>(const QJsonValue& j);

// ==========================================
// LiTiPen2
template <> QJsonValue toJson<LiTiPen2>(const LiTiPen2& v);
template <> LiTiPen2 fromJson<LiTiPen2>(const QJsonValue& j);

// ==========================================
// LockgCenSts3
template <> QJsonValue toJson<LockgCenSts3>(const LockgCenSts3& v);
template <> LockgCenSts3 fromJson<LockgCenSts3>(const QJsonValue& j);

// ==========================================
// LvlSeldForSpdLimAdpvPen1
template <> QJsonValue toJson<LvlSeldForSpdLimAdpvPen1>(const LvlSeldForSpdLimAdpvPen1& v);
template <> LvlSeldForSpdLimAdpvPen1 fromJson<LvlSeldForSpdLimAdpvPen1>(const QJsonValue& j);

// ==========================================
// MassgFctActv
template <> QJsonValue toJson<MassgFctActv>(const MassgFctActv& v);
template <> MassgFctActv fromJson<MassgFctActv>(const QJsonValue& j);

// ==========================================
// MirrDimPen
template <> QJsonValue toJson<MirrDimPen>(const MirrDimPen& v);
template <> MirrDimPen fromJson<MirrDimPen>(const QJsonValue& j);

// ==========================================
// MtrlSnsrT
template <> QJsonValue toJson<MtrlSnsrT>(const MtrlSnsrT& v);
template <> MtrlSnsrT fromJson<MtrlSnsrT>(const QJsonValue& j);

// ==========================================
// NetActvtRec1
template <> QJsonValue toJson<NetActvtRec1>(const NetActvtRec1& v);
template <> NetActvtRec1 fromJson<NetActvtRec1>(const QJsonValue& j);

// ==========================================
// NormSptPen
template <> QJsonValue toJson<NormSptPen>(const NormSptPen& v);
template <> NormSptPen fromJson<NormSptPen>(const QJsonValue& j);

// ==========================================
// NrSerlNodLIN
template <> QJsonValue toJson<NrSerlNodLIN>(const NrSerlNodLIN& v);
template <> NrSerlNodLIN fromJson<NrSerlNodLIN>(const QJsonValue& j);

// ==========================================
// OffsForSpdWarnSetgPen
template <> QJsonValue toJson<OffsForSpdWarnSetgPen>(const OffsForSpdWarnSetgPen& v);
template <> OffsForSpdWarnSetgPen fromJson<OffsForSpdWarnSetgPen>(const QJsonValue& j);

// ==========================================
// OnOffPen
template <> QJsonValue toJson<OnOffPen>(const OnOffPen& v);
template <> OnOffPen fromJson<OnOffPen>(const QJsonValue& j);

// ==========================================
// PartNrNodLIN
template <> QJsonValue toJson<PartNrNodLIN>(const PartNrNodLIN& v);
template <> PartNrNodLIN fromJson<PartNrNodLIN>(const QJsonValue& j);

// ==========================================
// PassSeatSwtSts2
template <> QJsonValue toJson<PassSeatSwtSts2>(const PassSeatSwtSts2& v);
template <> PassSeatSwtSts2 fromJson<PassSeatSwtSts2>(const QJsonValue& j);

// ==========================================
// PinionSteerAg1Rec
template <> QJsonValue toJson<PinionSteerAg1Rec>(const PinionSteerAg1Rec& v);
template <> PinionSteerAg1Rec fromJson<PinionSteerAg1Rec>(const QJsonValue& j);

// ==========================================
// PosnFromNav
template <> QJsonValue toJson<PosnFromNav>(const PosnFromNav& v);
template <> PosnFromNav fromJson<PosnFromNav>(const QJsonValue& j);

// ==========================================
// PosnFromSatlt
template <> QJsonValue toJson<PosnFromSatlt>(const PosnFromSatlt& v);
template <> PosnFromSatlt fromJson<PosnFromSatlt>(const QJsonValue& j);

// ==========================================
// ProfAct1
template <> QJsonValue toJson<ProfAct1>(const ProfAct1& v);
template <> ProfAct1 fromJson<ProfAct1>(const QJsonValue& j);

// ==========================================
// ProfLimd1
template <> QJsonValue toJson<ProfLimd1>(const ProfLimd1& v);
template <> ProfLimd1 fromJson<ProfLimd1>(const QJsonValue& j);

// ==========================================
// PtCluTq1
template <> QJsonValue toJson<PtCluTq1>(const PtCluTq1& v);
template <> PtCluTq1 fromJson<PtCluTq1>(const QJsonValue& j);

// ==========================================
// PtDrvrSetg2
template <> QJsonValue toJson<PtDrvrSetg2>(const PtDrvrSetg2& v);
template <> PtDrvrSetg2 fromJson<PtDrvrSetg2>(const QJsonValue& j);

// ==========================================
// PtTqAtWhlFrntActRec1
template <> QJsonValue toJson<PtTqAtWhlFrntActRec1>(const PtTqAtWhlFrntActRec1& v);
template <> PtTqAtWhlFrntActRec1 fromJson<PtTqAtWhlFrntActRec1>(const QJsonValue& j);

// ==========================================
// RngBdIllmnCmdPen1
template <> QJsonValue toJson<RngBdIllmnCmdPen1>(const RngBdIllmnCmdPen1& v);
template <> RngBdIllmnCmdPen1 fromJson<RngBdIllmnCmdPen1>(const QJsonValue& j);

// ==========================================
// RsdsSysSts
template <> QJsonValue toJson<RsdsSysSts>(const RsdsSysSts& v);
template <> RsdsSysSts fromJson<RsdsSysSts>(const QJsonValue& j);

// ==========================================
// SeatBackUnlckd
template <> QJsonValue toJson<SeatBackUnlckd>(const SeatBackUnlckd& v);
template <> SeatBackUnlckd fromJson<SeatBackUnlckd>(const QJsonValue& j);

// ==========================================
// SeatDispBtnPsd
template <> QJsonValue toJson<SeatDispBtnPsd>(const SeatDispBtnPsd& v);
template <> SeatDispBtnPsd fromJson<SeatDispBtnPsd>(const QJsonValue& j);

// ==========================================
// SeatMassgFct
template <> QJsonValue toJson<SeatMassgFct>(const SeatMassgFct& v);
template <> SeatMassgFct fromJson<SeatMassgFct>(const QJsonValue& j);

// ==========================================
// SetOfLang
template <> QJsonValue toJson<SetOfLang>(const SetOfLang& v);
template <> SetOfLang fromJson<SetOfLang>(const QJsonValue& j);

// ==========================================
// SftySigGroupFromAudSafe1
template <> QJsonValue toJson<SftySigGroupFromAudSafe1>(const SftySigGroupFromAudSafe1& v);
template <> SftySigGroupFromAudSafe1 fromJson<SftySigGroupFromAudSafe1>(const QJsonValue& j);

// ==========================================
// SftyWarnGroupFromAsySafe1
template <> QJsonValue toJson<SftyWarnGroupFromAsySafe1>(const SftyWarnGroupFromAsySafe1& v);
template <> SftyWarnGroupFromAsySafe1 fromJson<SftyWarnGroupFromAsySafe1>(const QJsonValue& j);

// ==========================================
// SlowNormFastPen
template <> QJsonValue toJson<SlowNormFastPen>(const SlowNormFastPen& v);
template <> SlowNormFastPen fromJson<SlowNormFastPen>(const QJsonValue& j);

// ==========================================
// SnsrParkAssi1
template <> QJsonValue toJson<SnsrParkAssi1>(const SnsrParkAssi1& v);
template <> SnsrParkAssi1 fromJson<SnsrParkAssi1>(const QJsonValue& j);

// ==========================================
// SnvtyPen1
template <> QJsonValue toJson<SnvtyPen1>(const SnvtyPen1& v);
template <> SnvtyPen1 fromJson<SnvtyPen1>(const QJsonValue& j);

// ==========================================
// SnvtyPen3
template <> QJsonValue toJson<SnvtyPen3>(const SnvtyPen3& v);
template <> SnvtyPen3 fromJson<SnvtyPen3>(const QJsonValue& j);

// ==========================================
// SteerAssiLvlForAutDrvPen1
template <> QJsonValue toJson<SteerAssiLvlForAutDrvPen1>(const SteerAssiLvlForAutDrvPen1& v);
template <> SteerAssiLvlForAutDrvPen1 fromJson<SteerAssiLvlForAutDrvPen1>(const QJsonValue& j);

// ==========================================
// SteerSetg
template <> QJsonValue toJson<SteerSetg>(const SteerSetg& v);
template <> SteerSetg fromJson<SteerSetg>(const QJsonValue& j);

// ==========================================
// SteerWhlSnsrSafe1
template <> QJsonValue toJson<SteerWhlSnsrSafe1>(const SteerWhlSnsrSafe1& v);
template <> SteerWhlSnsrSafe1 fromJson<SteerWhlSnsrSafe1>(const QJsonValue& j);

// ==========================================
// SuspSetgRec
template <> QJsonValue toJson<SuspSetgRec>(const SuspSetgRec& v);
template <> SuspSetgRec fromJson<SuspSetgRec>(const QJsonValue& j);

// ==========================================
// SysU
template <> QJsonValue toJson<SysU>(const SysU& v);
template <> SysU fromJson<SysU>(const QJsonValue& j);

// ==========================================
// TiCorrn
template <> QJsonValue toJson<TiCorrn>(const TiCorrn& v);
template <> TiCorrn fromJson<TiCorrn>(const QJsonValue& j);

// ==========================================
// TiGapAdpvSeldPen1
template <> QJsonValue toJson<TiGapAdpvSeldPen1>(const TiGapAdpvSeldPen1& v);
template <> TiGapAdpvSeldPen1 fromJson<TiGapAdpvSeldPen1>(const QJsonValue& j);

// ==========================================
// TiGapPen1
template <> QJsonValue toJson<TiGapPen1>(const TiGapPen1& v);
template <> TiGapPen1 fromJson<TiGapPen1>(const QJsonValue& j);

// ==========================================
// TireCircumCalByNav
template <> QJsonValue toJson<TireCircumCalByNav>(const TireCircumCalByNav& v);
template <> TireCircumCalByNav fromJson<TireCircumCalByNav>(const QJsonValue& j);

// ==========================================
// TirePAbsltValFrntLe3
template <> QJsonValue toJson<TirePAbsltValFrntLe3>(const TirePAbsltValFrntLe3& v);
template <> TirePAbsltValFrntLe3 fromJson<TirePAbsltValFrntLe3>(const QJsonValue& j);

// ==========================================
// TirePAbsltValFrntRi1
template <> QJsonValue toJson<TirePAbsltValFrntRi1>(const TirePAbsltValFrntRi1& v);
template <> TirePAbsltValFrntRi1 fromJson<TirePAbsltValFrntRi1>(const QJsonValue& j);

// ==========================================
// TirePAbsltValReLe1
template <> QJsonValue toJson<TirePAbsltValReLe1>(const TirePAbsltValReLe1& v);
template <> TirePAbsltValReLe1 fromJson<TirePAbsltValReLe1>(const QJsonValue& j);

// ==========================================
// TirePAbsltValReRi1
template <> QJsonValue toJson<TirePAbsltValReRi1>(const TirePAbsltValReRi1& v);
template <> TirePAbsltValReRi1 fromJson<TirePAbsltValReRi1>(const QJsonValue& j);

// ==========================================
// TirePMonData1
template <> QJsonValue toJson<TirePMonData1>(const TirePMonData1& v);
template <> TirePMonData1 fromJson<TirePMonData1>(const QJsonValue& j);

// ==========================================
// TqSafe2
template <> QJsonValue toJson<TqSafe2>(const TqSafe2& v);
template <> TqSafe2 fromJson<TqSafe2>(const QJsonValue& j);

// ==========================================
// TwliBriRaw
template <> QJsonValue toJson<TwliBriRaw>(const TwliBriRaw& v);
template <> TwliBriRaw fromJson<TwliBriRaw>(const QJsonValue& j);

// ==========================================
// UInt64Rec
template <> QJsonValue toJson<UInt64Rec>(const UInt64Rec& v);
template <> UInt64Rec fromJson<UInt64Rec>(const QJsonValue& j);

// ==========================================
// UnlckKeylsCfgPen2
template <> QJsonValue toJson<UnlckKeylsCfgPen2>(const UnlckKeylsCfgPen2& v);
template <> UnlckKeylsCfgPen2 fromJson<UnlckKeylsCfgPen2>(const QJsonValue& j);

// ==========================================
// UnlckRemCfgPen1
template <> QJsonValue toJson<UnlckRemCfgPen1>(const UnlckRemCfgPen1& v);
template <> UnlckRemCfgPen1 fromJson<UnlckRemCfgPen1>(const QJsonValue& j);

// ==========================================
// UsrSetSpdForKeySpdLimn
template <> QJsonValue toJson<UsrSetSpdForKeySpdLimn>(const UsrSetSpdForKeySpdLimn& v);
template <> UsrSetSpdForKeySpdLimn fromJson<UsrSetSpdForKeySpdLimn>(const QJsonValue& j);

// ==========================================
// UsrSetSpdForKeySpdWarn
template <> QJsonValue toJson<UsrSetSpdForKeySpdWarn>(const UsrSetSpdForKeySpdWarn& v);
template <> UsrSetSpdForKeySpdWarn fromJson<UsrSetSpdForKeySpdWarn>(const QJsonValue& j);

// ==========================================
// UsrSetVolMaxForKeyVolLimn
template <> QJsonValue toJson<UsrSetVolMaxForKeyVolLimn>(const UsrSetVolMaxForKeyVolLimn& v);
template <> UsrSetVolMaxForKeyVolLimn fromJson<UsrSetVolMaxForKeyVolLimn>(const QJsonValue& j);

// ==========================================
// UsrSwtDispClimaReqForRowSec
template <> QJsonValue toJson<UsrSwtDispClimaReqForRowSec>(const UsrSwtDispClimaReqForRowSec& v);
template <> UsrSwtDispClimaReqForRowSec fromJson<UsrSwtDispClimaReqForRowSec>(const QJsonValue& j);

// ==========================================
// UsrSwtDispClimaSts
template <> QJsonValue toJson<UsrSwtDispClimaSts>(const UsrSwtDispClimaSts& v);
template <> UsrSwtDispClimaSts fromJson<UsrSwtDispClimaSts>(const QJsonValue& j);

// ==========================================
// UsrSwtDispForSecRowSeatVentn
template <> QJsonValue toJson<UsrSwtDispForSecRowSeatVentn>(const UsrSwtDispForSecRowSeatVentn& v);
template <> UsrSwtDispForSecRowSeatVentn fromJson<UsrSwtDispForSecRowSeatVentn>(const QJsonValue& j);

// ==========================================
// UsrSwtDispReqForSecRowSeatVentn
template <> QJsonValue toJson<UsrSwtDispReqForSecRowSeatVentn>(const UsrSwtDispReqForSecRowSeatVentn& v);
template <> UsrSwtDispReqForSecRowSeatVentn fromJson<UsrSwtDispReqForSecRowSeatVentn>(const QJsonValue& j);

// ==========================================
// UsrSwtDispReqVrnt
template <> QJsonValue toJson<UsrSwtDispReqVrnt>(const UsrSwtDispReqVrnt& v);
template <> UsrSwtDispReqVrnt fromJson<UsrSwtDispReqVrnt>(const QJsonValue& j);

// ==========================================
// VFCGrp
template <> QJsonValue toJson<VFCGrp>(const VFCGrp& v);
template <> VFCGrp fromJson<VFCGrp>(const QJsonValue& j);

// ==========================================
// VehCfgPrm
template <> QJsonValue toJson<VehCfgPrm>(const VehCfgPrm& v);
template <> VehCfgPrm fromJson<VehCfgPrm>(const QJsonValue& j);

// ==========================================
// VehMNomRec1
template <> QJsonValue toJson<VehMNomRec1>(const VehMNomRec1& v);
template <> VehMNomRec1 fromJson<VehMNomRec1>(const QJsonValue& j);

// ==========================================
// VehModMngtGlbSafe1
template <> QJsonValue toJson<VehModMngtGlbSafe1>(const VehModMngtGlbSafe1& v);
template <> VehModMngtGlbSafe1 fromJson<VehModMngtGlbSafe1>(const QJsonValue& j);

// ==========================================
// VehMtnStSafe1
template <> QJsonValue toJson<VehMtnStSafe1>(const VehMtnStSafe1& v);
template <> VehMtnStSafe1 fromJson<VehMtnStSafe1>(const QJsonValue& j);

// ==========================================
// VehSpdIndcd1
template <> QJsonValue toJson<VehSpdIndcd1>(const VehSpdIndcd1& v);
template <> VehSpdIndcd1 fromJson<VehSpdIndcd1>(const QJsonValue& j);

// ==========================================
// VehSpdLgtSafe1
template <> QJsonValue toJson<VehSpdLgtSafe1>(const VehSpdLgtSafe1& v);
template <> VehSpdLgtSafe1 fromJson<VehSpdLgtSafe1>(const QJsonValue& j);

// ==========================================
// Vin1
template <> QJsonValue toJson<Vin1>(const Vin1& v);
template <> Vin1 fromJson<Vin1>(const QJsonValue& j);

// ==========================================
// VisnAgWideCfg1
template <> QJsonValue toJson<VisnAgWideCfg1>(const VisnAgWideCfg1& v);
template <> VisnAgWideCfg1 fromJson<VisnAgWideCfg1>(const QJsonValue& j);

// ==========================================
// WarnAndIntvPen1
template <> QJsonValue toJson<WarnAndIntvPen1>(const WarnAndIntvPen1& v);
template <> WarnAndIntvPen1 fromJson<WarnAndIntvPen1>(const QJsonValue& j);

// ==========================================
// WarnTypForLaneChgWarnPen1
template <> QJsonValue toJson<WarnTypForLaneChgWarnPen1>(const WarnTypForLaneChgWarnPen1& v);
template <> WarnTypForLaneChgWarnPen1 fromJson<WarnTypForLaneChgWarnPen1>(const QJsonValue& j);

// ==========================================
// WhlRotToothCntrRec
template <> QJsonValue toJson<WhlRotToothCntrRec>(const WhlRotToothCntrRec& v);
template <> WhlRotToothCntrRec fromJson<WhlRotToothCntrRec>(const QJsonValue& j);

// ==========================================
// WipgInfo
template <> QJsonValue toJson<WipgInfo>(const WipgInfo& v);
template <> WipgInfo fromJson<WipgInfo>(const QJsonValue& j);

} // end of namespace
#endif