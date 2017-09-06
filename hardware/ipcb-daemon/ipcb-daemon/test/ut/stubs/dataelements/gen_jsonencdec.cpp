/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
// C++ code generator for AUTOSAR v1.0
// Generated at: 2016-04-28T11:06:19.388754
// Source: SPA1610_IHUVOLVO8_151214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml

#include "gen_jsonencdec.h"

namespace autosar {

template <> QJsonValue toJson<uint32_t>(const uint32_t& e) {
    return QJsonValue((qint64)e);
}

// ==========================================
// ADataRawSafe1
template <>
QJsonValue toJson<ADataRawSafe1>(const ADataRawSafe1& v) {
    QJsonObject j;
    j["ALat"] = toJson<double>(v.ALat);
    j["ALat1Qf"] = toJson<Qf1>(v.ALat1Qf);
    j["ALgt"] = toJson<double>(v.ALgt);
    j["ALgt1Qf"] = toJson<Qf1>(v.ALgt1Qf);
    j["AVert"] = toJson<double>(v.AVert);
    j["AVertQf"] = toJson<Qf1>(v.AVertQf);
    j["Chks"] = toJson<uint8_t>(v.Chks);
    j["Cntr"] = toJson<uint8_t>(v.Cntr);
    return QJsonValue(j);
}
template <>
ADataRawSafe1 fromJson<ADataRawSafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return ADataRawSafe1();
    }
    QJsonObject object = j.toObject();
    ADataRawSafe1 v;
    v.ALat = fromJson<double>(object["ALat"]);
    v.ALat1Qf = fromJson<Qf1>(object["ALat1Qf"]);
    v.ALgt = fromJson<double>(object["ALgt"]);
    v.ALgt1Qf = fromJson<Qf1>(object["ALgt1Qf"]);
    v.AVert = fromJson<double>(object["AVert"]);
    v.AVertQf = fromJson<Qf1>(object["AVertQf"]);
    v.Chks = fromJson<uint8_t>(object["Chks"]);
    v.Cntr = fromJson<uint8_t>(object["Cntr"]);
    return v;
}


// ==========================================
// ActvnOfCrsEco
template <>
QJsonValue toJson<ActvnOfCrsEco>(const ActvnOfCrsEco& v) {
    QJsonObject j;
    j["Sts"] = toJson<OnOff1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
ActvnOfCrsEco fromJson<ActvnOfCrsEco>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return ActvnOfCrsEco();
    }
    QJsonObject object = j.toObject();
    ActvnOfCrsEco v;
    v.Sts = fromJson<OnOff1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// AdprTurnSpdModPen1
template <>
QJsonValue toJson<AdprTurnSpdModPen1>(const AdprTurnSpdModPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<AdprTurnSpdMod2>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
AdprTurnSpdModPen1 fromJson<AdprTurnSpdModPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return AdprTurnSpdModPen1();
    }
    QJsonObject object = j.toObject();
    AdprTurnSpdModPen1 v;
    v.Sts = fromJson<AdprTurnSpdMod2>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// AgDataRawSafe1
template <>
QJsonValue toJson<AgDataRawSafe1>(const AgDataRawSafe1& v) {
    QJsonObject j;
    j["RollRate"] = toJson<double>(v.RollRate);
    j["RollRateQf"] = toJson<Qf1>(v.RollRateQf);
    j["YawRate"] = toJson<double>(v.YawRate);
    j["YawRateQf"] = toJson<Qf1>(v.YawRateQf);
    j["Chks"] = toJson<uint8_t>(v.Chks);
    j["Cntr"] = toJson<uint8_t>(v.Cntr);
    return QJsonValue(j);
}
template <>
AgDataRawSafe1 fromJson<AgDataRawSafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return AgDataRawSafe1();
    }
    QJsonObject object = j.toObject();
    AgDataRawSafe1 v;
    v.RollRate = fromJson<double>(object["RollRate"]);
    v.RollRateQf = fromJson<Qf1>(object["RollRateQf"]);
    v.YawRate = fromJson<double>(object["YawRate"]);
    v.YawRateQf = fromJson<Qf1>(object["YawRateQf"]);
    v.Chks = fromJson<uint8_t>(object["Chks"]);
    v.Cntr = fromJson<uint8_t>(object["Cntr"]);
    return v;
}


// ==========================================
// AmbTIndcdWithUnit
template <>
QJsonValue toJson<AmbTIndcdWithUnit>(const AmbTIndcdWithUnit& v) {
    QJsonObject j;
    j["AmbTIndcd"] = toJson<double>(v.AmbTIndcd);
    j["AmbTIndcdUnit_"] = toJson<AmbTIndcdUnit>(v.AmbTIndcdUnit_);
    j["QF"] = toJson<GenQf1>(v.QF);
    return QJsonValue(j);
}
template <>
AmbTIndcdWithUnit fromJson<AmbTIndcdWithUnit>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return AmbTIndcdWithUnit();
    }
    QJsonObject object = j.toObject();
    AmbTIndcdWithUnit v;
    v.AmbTIndcd = fromJson<double>(object["AmbTIndcd"]);
    v.AmbTIndcdUnit_ = fromJson<AmbTIndcdUnit>(object["AmbTIndcdUnit_"]);
    v.QF = fromJson<GenQf1>(object["QF"]);
    return v;
}


// ==========================================
// AmbTWithQly
template <>
QJsonValue toJson<AmbTWithQly>(const AmbTWithQly& v) {
    QJsonObject j;
    j["AmbTVal"] = toJson<double>(v.AmbTVal);
    j["Qly"] = toJson<GenQf1>(v.Qly);
    return QJsonValue(j);
}
template <>
AmbTWithQly fromJson<AmbTWithQly>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return AmbTWithQly();
    }
    QJsonObject object = j.toObject();
    AmbTWithQly v;
    v.AmbTVal = fromJson<double>(object["AmbTVal"]);
    v.Qly = fromJson<GenQf1>(object["Qly"]);
    return v;
}


// ==========================================
// std::array<uint8_t,8>
template <>
QJsonValue toJson<std::array<uint8_t,8>>(const std::array<uint8_t,8>& v) {
    QJsonArray j;
    for (int i=0 ; i<8; ++i) {
        j.push_back(v[i]);
    }
    return QJsonValue(j);
}
template <>
std::array<uint8_t,8> fromJson<std::array<uint8_t,8>>(const QJsonValue& j) {
    if (!j.isArray()) {
        // TODO exception
        return std::array<uint8_t,8>();
    }
    QJsonArray a = j.toArray();
    if (a.size()!=8) {
        // TODO exception
        return std::array<uint8_t,8>();
    }
    std::array<uint8_t,8> result;
    for (int i=0 ; i<8 ; ++i) {
        const QJsonValue& v = a[i];
        result[i] = v.toVariant().value<uint8_t>(); // Qt5.4 cannot handle a[i].toVariant() directly
    }
    return result;
}


// ==========================================
// BltLockStFrnt
template <>
QJsonValue toJson<BltLockStFrnt>(const BltLockStFrnt& v) {
    QJsonObject j;
    j["BltLockSt1_"] = toJson<BltLockSt1>(v.BltLockSt1_);
    j["BltLockSts"] = toJson<DevErrSts2>(v.BltLockSts);
    return QJsonValue(j);
}
template <>
BltLockStFrnt fromJson<BltLockStFrnt>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return BltLockStFrnt();
    }
    QJsonObject object = j.toObject();
    BltLockStFrnt v;
    v.BltLockSt1_ = fromJson<BltLockSt1>(object["BltLockSt1_"]);
    v.BltLockSts = fromJson<DevErrSts2>(object["BltLockSts"]);
    return v;
}


// ==========================================
// BltLockStRe
template <>
QJsonValue toJson<BltLockStRe>(const BltLockStRe& v) {
    QJsonObject j;
    j["BltLockSt1_"] = toJson<BltLockSt1>(v.BltLockSt1_);
    j["BltLockSts"] = toJson<DevErrSts2>(v.BltLockSts);
    j["BltLockEquid"] = toJson<CptEquid>(v.BltLockEquid);
    return QJsonValue(j);
}
template <>
BltLockStRe fromJson<BltLockStRe>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return BltLockStRe();
    }
    QJsonObject object = j.toObject();
    BltLockStRe v;
    v.BltLockSt1_ = fromJson<BltLockSt1>(object["BltLockSt1_"]);
    v.BltLockSts = fromJson<DevErrSts2>(object["BltLockSts"]);
    v.BltLockEquid = fromJson<CptEquid>(object["BltLockEquid"]);
    return v;
}


// ==========================================
// BrkAndAbsWarnIndcnReqRec1
template <>
QJsonValue toJson<BrkAndAbsWarnIndcnReqRec1>(const BrkAndAbsWarnIndcnReqRec1& v) {
    QJsonObject j;
    j["BrkWarnIndcnReq"] = toJson<OnOff2>(v.BrkWarnIndcnReq);
    j["AbsWarnIndcnReq_"] = toJson<AbsWarnIndcnReq>(v.AbsWarnIndcnReq_);
    j["BrkAndAbsWarnIndcnReqChks"] = toJson<uint8_t>(v.BrkAndAbsWarnIndcnReqChks);
    j["BrkAndAbsWarnIndcnReqCntr"] = toJson<uint8_t>(v.BrkAndAbsWarnIndcnReqCntr);
    return QJsonValue(j);
}
template <>
BrkAndAbsWarnIndcnReqRec1 fromJson<BrkAndAbsWarnIndcnReqRec1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return BrkAndAbsWarnIndcnReqRec1();
    }
    QJsonObject object = j.toObject();
    BrkAndAbsWarnIndcnReqRec1 v;
    v.BrkWarnIndcnReq = fromJson<OnOff2>(object["BrkWarnIndcnReq"]);
    v.AbsWarnIndcnReq_ = fromJson<AbsWarnIndcnReq>(object["AbsWarnIndcnReq_"]);
    v.BrkAndAbsWarnIndcnReqChks = fromJson<uint8_t>(object["BrkAndAbsWarnIndcnReqChks"]);
    v.BrkAndAbsWarnIndcnReqCntr = fromJson<uint8_t>(object["BrkAndAbsWarnIndcnReqCntr"]);
    return v;
}


// ==========================================
// BrkFricTqAtWhlActRec
template <>
QJsonValue toJson<BrkFricTqAtWhlActRec>(const BrkFricTqAtWhlActRec& v) {
    QJsonObject j;
    j["BrkFricTqAtWhlFrntLeAct"] = toJson<uint16_t>(v.BrkFricTqAtWhlFrntLeAct);
    j["BrkFricTqAtWhlFrntRiAct"] = toJson<uint16_t>(v.BrkFricTqAtWhlFrntRiAct);
    j["BrkFricTqAtWhlReLeAct"] = toJson<uint16_t>(v.BrkFricTqAtWhlReLeAct);
    j["BrkFricTqAtWhlReRiAct"] = toJson<uint16_t>(v.BrkFricTqAtWhlReRiAct);
    return QJsonValue(j);
}
template <>
BrkFricTqAtWhlActRec fromJson<BrkFricTqAtWhlActRec>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return BrkFricTqAtWhlActRec();
    }
    QJsonObject object = j.toObject();
    BrkFricTqAtWhlActRec v;
    v.BrkFricTqAtWhlFrntLeAct = fromJson<uint16_t>(object["BrkFricTqAtWhlFrntLeAct"]);
    v.BrkFricTqAtWhlFrntRiAct = fromJson<uint16_t>(object["BrkFricTqAtWhlFrntRiAct"]);
    v.BrkFricTqAtWhlReLeAct = fromJson<uint16_t>(object["BrkFricTqAtWhlReLeAct"]);
    v.BrkFricTqAtWhlReRiAct = fromJson<uint16_t>(object["BrkFricTqAtWhlReRiAct"]);
    return v;
}


// ==========================================
// CamIndReq
template <>
QJsonValue toJson<CamIndReq>(const CamIndReq& v) {
    QJsonObject j;
    j["CamFrnt"] = toJson<OnOff1>(v.CamFrnt);
    j["CamRe"] = toJson<OnOff1>(v.CamRe);
    j["CamLe"] = toJson<OnOff1>(v.CamLe);
    j["CamRi"] = toJson<OnOff1>(v.CamRi);
    return QJsonValue(j);
}
template <>
CamIndReq fromJson<CamIndReq>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return CamIndReq();
    }
    QJsonObject object = j.toObject();
    CamIndReq v;
    v.CamFrnt = fromJson<OnOff1>(object["CamFrnt"]);
    v.CamRe = fromJson<OnOff1>(object["CamRe"]);
    v.CamLe = fromJson<OnOff1>(object["CamLe"]);
    v.CamRi = fromJson<OnOff1>(object["CamRi"]);
    return v;
}


// ==========================================
// CchForFuEco1
template <>
QJsonValue toJson<CchForFuEco1>(const CchForFuEco1& v) {
    QJsonObject j;
    j["BarForFuEco"] = toJson<double>(v.BarForFuEco);
    j["GrdForFuEco"] = toJson<double>(v.GrdForFuEco);
    return QJsonValue(j);
}
template <>
CchForFuEco1 fromJson<CchForFuEco1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return CchForFuEco1();
    }
    QJsonObject object = j.toObject();
    CchForFuEco1 v;
    v.BarForFuEco = fromJson<double>(object["BarForFuEco"]);
    v.GrdForFuEco = fromJson<double>(object["GrdForFuEco"]);
    return v;
}


// ==========================================
// CmptmtAirTEstimdExtd
template <>
QJsonValue toJson<CmptmtAirTEstimdExtd>(const CmptmtAirTEstimdExtd& v) {
    QJsonObject j;
    j["ComptmtT"] = toJson<double>(v.ComptmtT);
    j["QlyFlg"] = toJson<GenQf1>(v.QlyFlg);
    return QJsonValue(j);
}
template <>
CmptmtAirTEstimdExtd fromJson<CmptmtAirTEstimdExtd>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return CmptmtAirTEstimdExtd();
    }
    QJsonObject object = j.toObject();
    CmptmtAirTEstimdExtd v;
    v.ComptmtT = fromJson<double>(object["ComptmtT"]);
    v.QlyFlg = fromJson<GenQf1>(object["QlyFlg"]);
    return v;
}


// ==========================================
// CmptmtTFrnt
template <>
QJsonValue toJson<CmptmtTFrnt>(const CmptmtTFrnt& v) {
    QJsonObject j;
    j["CmptmtTFrnt"] = toJson<double>(v.CmptmtTFrnt);
    j["CmptmtTFrntQf_"] = toJson<CmptmtTFrntQf>(v.CmptmtTFrntQf_);
    j["FanForCmptmtTRunng"] = toJson<Flg1>(v.FanForCmptmtTRunng);
    return QJsonValue(j);
}
template <>
CmptmtTFrnt fromJson<CmptmtTFrnt>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return CmptmtTFrnt();
    }
    QJsonObject object = j.toObject();
    CmptmtTFrnt v;
    v.CmptmtTFrnt = fromJson<double>(object["CmptmtTFrnt"]);
    v.CmptmtTFrntQf_ = fromJson<CmptmtTFrntQf>(object["CmptmtTFrntQf_"]);
    v.FanForCmptmtTRunng = fromJson<Flg1>(object["FanForCmptmtTRunng"]);
    return v;
}


// ==========================================
// ConSftyWarn1
template <>
QJsonValue toJson<ConSftyWarn1>(const ConSftyWarn1& v) {
    QJsonObject j;
    j["ConSftyWarnId"] = toJson<uint8_t>(v.ConSftyWarnId);
    j["ConSftyWarnSnd"] = toJson<OnOff1>(v.ConSftyWarnSnd);
    j["ConSftyWarnTyp"] = toJson<uint8_t>(v.ConSftyWarnTyp);
    j["ConSftyWarnLvl"] = toJson<ConSftyWarnLvl1>(v.ConSftyWarnLvl);
    j["DistanceToWarning"] = toJson<uint16_t>(v.DistanceToWarning);
    return QJsonValue(j);
}
template <>
ConSftyWarn1 fromJson<ConSftyWarn1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return ConSftyWarn1();
    }
    QJsonObject object = j.toObject();
    ConSftyWarn1 v;
    v.ConSftyWarnId = fromJson<uint8_t>(object["ConSftyWarnId"]);
    v.ConSftyWarnSnd = fromJson<OnOff1>(object["ConSftyWarnSnd"]);
    v.ConSftyWarnTyp = fromJson<uint8_t>(object["ConSftyWarnTyp"]);
    v.ConSftyWarnLvl = fromJson<ConSftyWarnLvl1>(object["ConSftyWarnLvl"]);
    v.DistanceToWarning = fromJson<uint16_t>(object["DistanceToWarning"]);
    return v;
}


// ==========================================
// DataSpclTyp
template <>
QJsonValue toJson<DataSpclTyp>(const DataSpclTyp& v) {
    QJsonObject j;
    j["DataIdn"] = toJson<uint8_t>(v.DataIdn);
    j["DataNrSpcl1"] = toJson<uint8_t>(v.DataNrSpcl1);
    j["DataNrSpcl2"] = toJson<uint8_t>(v.DataNrSpcl2);
    j["DataNrSpcl3"] = toJson<uint8_t>(v.DataNrSpcl3);
    return QJsonValue(j);
}
template <>
DataSpclTyp fromJson<DataSpclTyp>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DataSpclTyp();
    }
    QJsonObject object = j.toObject();
    DataSpclTyp v;
    v.DataIdn = fromJson<uint8_t>(object["DataIdn"]);
    v.DataNrSpcl1 = fromJson<uint8_t>(object["DataNrSpcl1"]);
    v.DataNrSpcl2 = fromJson<uint8_t>(object["DataNrSpcl2"]);
    v.DataNrSpcl3 = fromJson<uint8_t>(object["DataNrSpcl3"]);
    return v;
}


// ==========================================
// DateTi1ForSet
template <>
QJsonValue toJson<DateTi1ForSet>(const DateTi1ForSet& v) {
    QJsonObject j;
    j["DateOrTi_"] = toJson<DateOrTi>(v.DateOrTi_);
    j["Year"] = toJson<uint8_t>(v.Year);
    j["Month"] = toJson<uint8_t>(v.Month);
    j["Day"] = toJson<uint8_t>(v.Day);
    j["Hour"] = toJson<uint8_t>(v.Hour);
    j["Minute"] = toJson<uint8_t>(v.Minute);
    j["Second"] = toJson<uint8_t>(v.Second);
    return QJsonValue(j);
}
template <>
DateTi1ForSet fromJson<DateTi1ForSet>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DateTi1ForSet();
    }
    QJsonObject object = j.toObject();
    DateTi1ForSet v;
    v.DateOrTi_ = fromJson<DateOrTi>(object["DateOrTi_"]);
    v.Year = fromJson<uint8_t>(object["Year"]);
    v.Month = fromJson<uint8_t>(object["Month"]);
    v.Day = fromJson<uint8_t>(object["Day"]);
    v.Hour = fromJson<uint8_t>(object["Hour"]);
    v.Minute = fromJson<uint8_t>(object["Minute"]);
    v.Second = fromJson<uint8_t>(object["Second"]);
    return v;
}


// ==========================================
// DateTi30
template <>
QJsonValue toJson<DateTi30>(const DateTi30& v) {
    QJsonObject j;
    j["Yr1"] = toJson<uint8_t>(v.Yr1);
    j["Mth1"] = toJson<uint8_t>(v.Mth1);
    j["Day"] = toJson<uint8_t>(v.Day);
    j["Hr1"] = toJson<uint8_t>(v.Hr1);
    j["Mins1"] = toJson<uint8_t>(v.Mins1);
    j["Sec1"] = toJson<uint8_t>(v.Sec1);
    j["DataValid"] = toJson<NoYes1>(v.DataValid);
    return QJsonValue(j);
}
template <>
DateTi30 fromJson<DateTi30>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DateTi30();
    }
    QJsonObject object = j.toObject();
    DateTi30 v;
    v.Yr1 = fromJson<uint8_t>(object["Yr1"]);
    v.Mth1 = fromJson<uint8_t>(object["Mth1"]);
    v.Day = fromJson<uint8_t>(object["Day"]);
    v.Hr1 = fromJson<uint8_t>(object["Hr1"]);
    v.Mins1 = fromJson<uint8_t>(object["Mins1"]);
    v.Sec1 = fromJson<uint8_t>(object["Sec1"]);
    v.DataValid = fromJson<NoYes1>(object["DataValid"]);
    return v;
}


// ==========================================
// DrvrDispSetgReq
template <>
QJsonValue toJson<DrvrDispSetgReq>(const DrvrDispSetgReq& v) {
    QJsonObject j;
    j["Pen"] = toJson<IdPen>(v.Pen);
    j["Sts"] = toJson<DrvrDispSetg>(v.Sts);
    return QJsonValue(j);
}
template <>
DrvrDispSetgReq fromJson<DrvrDispSetgReq>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DrvrDispSetgReq();
    }
    QJsonObject object = j.toObject();
    DrvrDispSetgReq v;
    v.Pen = fromJson<IdPen>(object["Pen"]);
    v.Sts = fromJson<DrvrDispSetg>(object["Sts"]);
    return v;
}


// ==========================================
// DrvrHmiBackGndInfoSetg
template <>
QJsonValue toJson<DrvrHmiBackGndInfoSetg>(const DrvrHmiBackGndInfoSetg& v) {
    QJsonObject j;
    j["Pen"] = toJson<IdPen>(v.Pen);
    j["Setg"] = toJson<DrvrHmiBackGndInfo>(v.Setg);
    return QJsonValue(j);
}
template <>
DrvrHmiBackGndInfoSetg fromJson<DrvrHmiBackGndInfoSetg>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DrvrHmiBackGndInfoSetg();
    }
    QJsonObject object = j.toObject();
    DrvrHmiBackGndInfoSetg v;
    v.Pen = fromJson<IdPen>(object["Pen"]);
    v.Setg = fromJson<DrvrHmiBackGndInfo>(object["Setg"]);
    return v;
}


// ==========================================
// DrvrHmiDispdModPen
template <>
QJsonValue toJson<DrvrHmiDispdModPen>(const DrvrHmiDispdModPen& v) {
    QJsonObject j;
    j["Sts"] = toJson<DrvrHmiDispdMod1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
DrvrHmiDispdModPen fromJson<DrvrHmiDispdModPen>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DrvrHmiDispdModPen();
    }
    QJsonObject object = j.toObject();
    DrvrHmiDispdModPen v;
    v.Sts = fromJson<DrvrHmiDispdMod1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// std::array<uint8_t,254>
template <>
QJsonValue toJson<std::array<uint8_t,254>>(const std::array<uint8_t,254>& v) {
    QJsonArray j;
    for (int i=0 ; i<254; ++i) {
        j.push_back(v[i]);
    }
    return QJsonValue(j);
}
template <>
std::array<uint8_t,254> fromJson<std::array<uint8_t,254>>(const QJsonValue& j) {
    if (!j.isArray()) {
        // TODO exception
        return std::array<uint8_t,254>();
    }
    QJsonArray a = j.toArray();
    if (a.size()!=254) {
        // TODO exception
        return std::array<uint8_t,254>();
    }
    std::array<uint8_t,254> result;
    for (int i=0 ; i<254 ; ++i) {
        const QJsonValue& v = a[i];
        result[i] = v.toVariant().value<uint8_t>(); // Qt5.4 cannot handle a[i].toVariant() directly
    }
    return result;
}


// ==========================================
// DrvrHmiUserIfSetgReq
template <>
QJsonValue toJson<DrvrHmiUserIfSetgReq>(const DrvrHmiUserIfSetgReq& v) {
    QJsonObject j;
    j["Pen"] = toJson<IdPen>(v.Pen);
    j["Setg"] = toJson<DrvrHmiUsrIfSetg>(v.Setg);
    return QJsonValue(j);
}
template <>
DrvrHmiUserIfSetgReq fromJson<DrvrHmiUserIfSetgReq>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DrvrHmiUserIfSetgReq();
    }
    QJsonObject object = j.toObject();
    DrvrHmiUserIfSetgReq v;
    v.Pen = fromJson<IdPen>(object["Pen"]);
    v.Setg = fromJson<DrvrHmiUsrIfSetg>(object["Setg"]);
    return v;
}


// ==========================================
// DrvrSeatSwtSts1
template <>
QJsonValue toJson<DrvrSeatSwtSts1>(const DrvrSeatSwtSts1& v) {
    QJsonObject j;
    j["DrvrSeatSwtSldSts"] = toJson<SwtHozlSts1>(v.DrvrSeatSwtSldSts);
    j["DrvrSeatSwtHeiSts"] = toJson<SwtVertSts1>(v.DrvrSeatSwtHeiSts);
    j["DrvrSeatSwtHeiFrntSts"] = toJson<SwtVertSts1>(v.DrvrSeatSwtHeiFrntSts);
    j["DrvrSeatSwtInclSts"] = toJson<SwtHozlSts1>(v.DrvrSeatSwtInclSts);
    j["DrvrSeatSwtAdjmtOfSpplFctHozlSts"] = toJson<SwtHozlSts1>(v.DrvrSeatSwtAdjmtOfSpplFctHozlSts);
    j["DrvrSeatSwtAdjmtOfSpplFctVertSts"] = toJson<SwtVertSts1>(v.DrvrSeatSwtAdjmtOfSpplFctVertSts);
    j["DrvrSeatSwtSelnOfSpplFctSts"] = toJson<SwtVertSts1>(v.DrvrSeatSwtSelnOfSpplFctSts);
    return QJsonValue(j);
}
template <>
DrvrSeatSwtSts1 fromJson<DrvrSeatSwtSts1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DrvrSeatSwtSts1();
    }
    QJsonObject object = j.toObject();
    DrvrSeatSwtSts1 v;
    v.DrvrSeatSwtSldSts = fromJson<SwtHozlSts1>(object["DrvrSeatSwtSldSts"]);
    v.DrvrSeatSwtHeiSts = fromJson<SwtVertSts1>(object["DrvrSeatSwtHeiSts"]);
    v.DrvrSeatSwtHeiFrntSts = fromJson<SwtVertSts1>(object["DrvrSeatSwtHeiFrntSts"]);
    v.DrvrSeatSwtInclSts = fromJson<SwtHozlSts1>(object["DrvrSeatSwtInclSts"]);
    v.DrvrSeatSwtAdjmtOfSpplFctHozlSts = fromJson<SwtHozlSts1>(object["DrvrSeatSwtAdjmtOfSpplFctHozlSts"]);
    v.DrvrSeatSwtAdjmtOfSpplFctVertSts = fromJson<SwtVertSts1>(object["DrvrSeatSwtAdjmtOfSpplFctVertSts"]);
    v.DrvrSeatSwtSelnOfSpplFctSts = fromJson<SwtVertSts1>(object["DrvrSeatSwtSelnOfSpplFctSts"]);
    return v;
}


// ==========================================
// DstToEmptyWithUnit
template <>
QJsonValue toJson<DstToEmptyWithUnit>(const DstToEmptyWithUnit& v) {
    QJsonObject j;
    j["DstToEmpty"] = toJson<uint16_t>(v.DstToEmpty);
    j["DstUnit_"] = toJson<DstUnit>(v.DstUnit_);
    return QJsonValue(j);
}
template <>
DstToEmptyWithUnit fromJson<DstToEmptyWithUnit>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DstToEmptyWithUnit();
    }
    QJsonObject object = j.toObject();
    DstToEmptyWithUnit v;
    v.DstToEmpty = fromJson<uint16_t>(object["DstToEmpty"]);
    v.DstUnit_ = fromJson<DstUnit>(object["DstUnit_"]);
    return v;
}


// ==========================================
// DstToManvLocnByNav
template <>
QJsonValue toJson<DstToManvLocnByNav>(const DstToManvLocnByNav& v) {
    QJsonObject j;
    j["DstToManv"] = toJson<uint16_t>(v.DstToManv);
    j["CntDwnToManvStrt"] = toJson<NoYes1>(v.CntDwnToManvStrt);
    j["PosnQly"] = toJson<PosnFromNavQly>(v.PosnQly);
    j["SpprtForFct"] = toJson<NoYes1>(v.SpprtForFct);
    return QJsonValue(j);
}
template <>
DstToManvLocnByNav fromJson<DstToManvLocnByNav>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return DstToManvLocnByNav();
    }
    QJsonObject object = j.toObject();
    DstToManvLocnByNav v;
    v.DstToManv = fromJson<uint16_t>(object["DstToManv"]);
    v.CntDwnToManvStrt = fromJson<NoYes1>(object["CntDwnToManvStrt"]);
    v.PosnQly = fromJson<PosnFromNavQly>(object["PosnQly"]);
    v.SpprtForFct = fromJson<NoYes1>(object["SpprtForFct"]);
    return v;
}


// ==========================================
// EngIdleEco1
template <>
QJsonValue toJson<EngIdleEco1>(const EngIdleEco1& v) {
    QJsonObject j;
    j["Sts"] = toJson<OnOff1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
EngIdleEco1 fromJson<EngIdleEco1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return EngIdleEco1();
    }
    QJsonObject object = j.toObject();
    EngIdleEco1 v;
    v.Sts = fromJson<OnOff1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// EngNSafe
template <>
QJsonValue toJson<EngNSafe>(const EngNSafe& v) {
    QJsonObject j;
    j["EngN"] = toJson<double>(v.EngN);
    j["EngNGrdt"] = toJson<int16_t>(v.EngNGrdt);
    j["EngNChks"] = toJson<uint8_t>(v.EngNChks);
    j["EngNCntr"] = toJson<uint8_t>(v.EngNCntr);
    return QJsonValue(j);
}
template <>
EngNSafe fromJson<EngNSafe>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return EngNSafe();
    }
    QJsonObject object = j.toObject();
    EngNSafe v;
    v.EngN = fromJson<double>(object["EngN"]);
    v.EngNGrdt = fromJson<int16_t>(object["EngNGrdt"]);
    v.EngNChks = fromJson<uint8_t>(object["EngNChks"]);
    v.EngNCntr = fromJson<uint8_t>(object["EngNCntr"]);
    return v;
}


// ==========================================
// EpbLampReqRec
template <>
QJsonValue toJson<EpbLampReqRec>(const EpbLampReqRec& v) {
    QJsonObject j;
    j["EpbLampReq"] = toJson<EpbLampReqType1>(v.EpbLampReq);
    j["EpbLampReqChks"] = toJson<uint8_t>(v.EpbLampReqChks);
    j["EpbLampReqCntr"] = toJson<uint8_t>(v.EpbLampReqCntr);
    return QJsonValue(j);
}
template <>
EpbLampReqRec fromJson<EpbLampReqRec>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return EpbLampReqRec();
    }
    QJsonObject object = j.toObject();
    EpbLampReqRec v;
    v.EpbLampReq = fromJson<EpbLampReqType1>(object["EpbLampReq"]);
    v.EpbLampReqChks = fromJson<uint8_t>(object["EpbLampReqChks"]);
    v.EpbLampReqCntr = fromJson<uint8_t>(object["EpbLampReqCntr"]);
    return v;
}


// ==========================================
// EscSptModReqdByDrvrRec1
template <>
QJsonValue toJson<EscSptModReqdByDrvrRec1>(const EscSptModReqdByDrvrRec1& v) {
    QJsonObject j;
    j["EscSptModReqdByDrvr"] = toJson<NoYes1>(v.EscSptModReqdByDrvr);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
EscSptModReqdByDrvrRec1 fromJson<EscSptModReqdByDrvrRec1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return EscSptModReqdByDrvrRec1();
    }
    QJsonObject object = j.toObject();
    EscSptModReqdByDrvrRec1 v;
    v.EscSptModReqdByDrvr = fromJson<NoYes1>(object["EscSptModReqdByDrvr"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// EscStSafe1
template <>
QJsonValue toJson<EscStSafe1>(const EscStSafe1& v) {
    QJsonObject j;
    j["EscSt"] = toJson<EscSt1>(v.EscSt);
    j["EscStChks"] = toJson<uint8_t>(v.EscStChks);
    j["EscStCntr"] = toJson<uint8_t>(v.EscStCntr);
    return QJsonValue(j);
}
template <>
EscStSafe1 fromJson<EscStSafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return EscStSafe1();
    }
    QJsonObject object = j.toObject();
    EscStSafe1 v;
    v.EscSt = fromJson<EscSt1>(object["EscSt"]);
    v.EscStChks = fromJson<uint8_t>(object["EscStChks"]);
    v.EscStCntr = fromJson<uint8_t>(object["EscStCntr"]);
    return v;
}


// ==========================================
// EscWarnIndcnReqRec1
template <>
QJsonValue toJson<EscWarnIndcnReqRec1>(const EscWarnIndcnReqRec1& v) {
    QJsonObject j;
    j["EscWarnIndcnReq_"] = toJson<EscWarnIndcnReq>(v.EscWarnIndcnReq_);
    j["EscWarnIndcnReqChks"] = toJson<uint8_t>(v.EscWarnIndcnReqChks);
    j["EscWarnIndcnReqCntr"] = toJson<uint8_t>(v.EscWarnIndcnReqCntr);
    return QJsonValue(j);
}
template <>
EscWarnIndcnReqRec1 fromJson<EscWarnIndcnReqRec1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return EscWarnIndcnReqRec1();
    }
    QJsonObject object = j.toObject();
    EscWarnIndcnReqRec1 v;
    v.EscWarnIndcnReq_ = fromJson<EscWarnIndcnReq>(object["EscWarnIndcnReq_"]);
    v.EscWarnIndcnReqChks = fromJson<uint8_t>(object["EscWarnIndcnReqChks"]);
    v.EscWarnIndcnReqCntr = fromJson<uint8_t>(object["EscWarnIndcnReqCntr"]);
    return v;
}


// ==========================================
// ExtrMirrTiltSetg
template <>
QJsonValue toJson<ExtrMirrTiltSetg>(const ExtrMirrTiltSetg& v) {
    QJsonObject j;
    j["MirrDrvr"] = toJson<OnOff2>(v.MirrDrvr);
    j["MirrPass"] = toJson<OnOff2>(v.MirrPass);
    j["IdPen_"] = toJson<IdPen>(v.IdPen_);
    return QJsonValue(j);
}
template <>
ExtrMirrTiltSetg fromJson<ExtrMirrTiltSetg>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return ExtrMirrTiltSetg();
    }
    QJsonObject object = j.toObject();
    ExtrMirrTiltSetg v;
    v.MirrDrvr = fromJson<OnOff2>(object["MirrDrvr"]);
    v.MirrPass = fromJson<OnOff2>(object["MirrPass"]);
    v.IdPen_ = fromJson<IdPen>(object["IdPen_"]);
    return v;
}


// ==========================================
// FrntWiprLvrReq2
template <>
QJsonValue toJson<FrntWiprLvrReq2>(const FrntWiprLvrReq2& v) {
    QJsonObject j;
    j["FrntWiprLvrCmd1_"] = toJson<FrntWiprLvrCmd1>(v.FrntWiprLvrCmd1_);
    j["FrntWiprLvrQf"] = toJson<Qf1>(v.FrntWiprLvrQf);
    j["FrntWiprLvrCrc"] = toJson<uint8_t>(v.FrntWiprLvrCrc);
    j["FrntWiprLvrCntr"] = toJson<uint8_t>(v.FrntWiprLvrCntr);
    return QJsonValue(j);
}
template <>
FrntWiprLvrReq2 fromJson<FrntWiprLvrReq2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return FrntWiprLvrReq2();
    }
    QJsonObject object = j.toObject();
    FrntWiprLvrReq2 v;
    v.FrntWiprLvrCmd1_ = fromJson<FrntWiprLvrCmd1>(object["FrntWiprLvrCmd1_"]);
    v.FrntWiprLvrQf = fromJson<Qf1>(object["FrntWiprLvrQf"]);
    v.FrntWiprLvrCrc = fromJson<uint8_t>(object["FrntWiprLvrCrc"]);
    v.FrntWiprLvrCntr = fromJson<uint8_t>(object["FrntWiprLvrCntr"]);
    return v;
}


// ==========================================
// FuLvlValWithQly
template <>
QJsonValue toJson<FuLvlValWithQly>(const FuLvlValWithQly& v) {
    QJsonObject j;
    j["FuLvlValFromFuTbl"] = toJson<double>(v.FuLvlValFromFuTbl);
    j["GenQF"] = toJson<GenQf1>(v.GenQF);
    return QJsonValue(j);
}
template <>
FuLvlValWithQly fromJson<FuLvlValWithQly>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return FuLvlValWithQly();
    }
    QJsonObject object = j.toObject();
    FuLvlValWithQly v;
    v.FuLvlValFromFuTbl = fromJson<double>(object["FuLvlValFromFuTbl"]);
    v.GenQF = fromJson<GenQf1>(object["GenQF"]);
    return v;
}


// ==========================================
// GearIndcnRec2
template <>
QJsonValue toJson<GearIndcnRec2>(const GearIndcnRec2& v) {
    QJsonObject j;
    j["GearIndcn"] = toJson<GearIndcnTyp2>(v.GearIndcn);
    j["GearShiftIndcn_"] = toJson<GearShiftIndcn>(v.GearShiftIndcn_);
    j["GearTarIndcn"] = toJson<GearIndcnTyp2>(v.GearTarIndcn);
    return QJsonValue(j);
}
template <>
GearIndcnRec2 fromJson<GearIndcnRec2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return GearIndcnRec2();
    }
    QJsonObject object = j.toObject();
    GearIndcnRec2 v;
    v.GearIndcn = fromJson<GearIndcnTyp2>(object["GearIndcn"]);
    v.GearShiftIndcn_ = fromJson<GearShiftIndcn>(object["GearShiftIndcn_"]);
    v.GearTarIndcn = fromJson<GearIndcnTyp2>(object["GearTarIndcn"]);
    return v;
}


// ==========================================
// GlbRstForSetgAndData
template <>
QJsonValue toJson<GlbRstForSetgAndData>(const GlbRstForSetgAndData& v) {
    QJsonObject j;
    j["GlbRst_"] = toJson<GlbRst>(v.GlbRst_);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
GlbRstForSetgAndData fromJson<GlbRstForSetgAndData>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return GlbRstForSetgAndData();
    }
    QJsonObject object = j.toObject();
    GlbRstForSetgAndData v;
    v.GlbRst_ = fromJson<GlbRst>(object["GlbRst_"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// HmiCmptmtTSp
template <>
QJsonValue toJson<HmiCmptmtTSp>(const HmiCmptmtTSp& v) {
    QJsonObject j;
    j["HmiCmptmtTSpForRowFirstLe"] = toJson<double>(v.HmiCmptmtTSpForRowFirstLe);
    j["HmiCmptmtTSpSpclForRowFirstLe"] = toJson<HmiCmptmtTSpSpcl>(v.HmiCmptmtTSpSpclForRowFirstLe);
    j["HmiCmptmtTSpForRowFirstRi"] = toJson<double>(v.HmiCmptmtTSpForRowFirstRi);
    j["HmiCmptmtTSpSpclForRowFirstRi"] = toJson<HmiCmptmtTSpSpcl>(v.HmiCmptmtTSpSpclForRowFirstRi);
    j["HmiCmptmtTSpForRowSecLe"] = toJson<double>(v.HmiCmptmtTSpForRowSecLe);
    j["HmiCmptmtTSpSpclForRowSecLe"] = toJson<HmiCmptmtTSpSpcl>(v.HmiCmptmtTSpSpclForRowSecLe);
    j["HmiCmptmtTSpForRowSecRi"] = toJson<double>(v.HmiCmptmtTSpForRowSecRi);
    j["HmiCmptmtTSpSpclForRowSecRi"] = toJson<HmiCmptmtTSpSpcl>(v.HmiCmptmtTSpSpclForRowSecRi);
    return QJsonValue(j);
}
template <>
HmiCmptmtTSp fromJson<HmiCmptmtTSp>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HmiCmptmtTSp();
    }
    QJsonObject object = j.toObject();
    HmiCmptmtTSp v;
    v.HmiCmptmtTSpForRowFirstLe = fromJson<double>(object["HmiCmptmtTSpForRowFirstLe"]);
    v.HmiCmptmtTSpSpclForRowFirstLe = fromJson<HmiCmptmtTSpSpcl>(object["HmiCmptmtTSpSpclForRowFirstLe"]);
    v.HmiCmptmtTSpForRowFirstRi = fromJson<double>(object["HmiCmptmtTSpForRowFirstRi"]);
    v.HmiCmptmtTSpSpclForRowFirstRi = fromJson<HmiCmptmtTSpSpcl>(object["HmiCmptmtTSpSpclForRowFirstRi"]);
    v.HmiCmptmtTSpForRowSecLe = fromJson<double>(object["HmiCmptmtTSpForRowSecLe"]);
    v.HmiCmptmtTSpSpclForRowSecLe = fromJson<HmiCmptmtTSpSpcl>(object["HmiCmptmtTSpSpclForRowSecLe"]);
    v.HmiCmptmtTSpForRowSecRi = fromJson<double>(object["HmiCmptmtTSpForRowSecRi"]);
    v.HmiCmptmtTSpSpclForRowSecRi = fromJson<HmiCmptmtTSpSpcl>(object["HmiCmptmtTSpSpclForRowSecRi"]);
    return v;
}


// ==========================================
// HmiDefrstElecReq
template <>
QJsonValue toJson<HmiDefrstElecReq>(const HmiDefrstElecReq& v) {
    QJsonObject j;
    j["FrntElecReq"] = toJson<ActrReq>(v.FrntElecReq);
    j["ReElecReq"] = toJson<ActrReq>(v.ReElecReq);
    j["MirrElecReq"] = toJson<ActrReq>(v.MirrElecReq);
    return QJsonValue(j);
}
template <>
HmiDefrstElecReq fromJson<HmiDefrstElecReq>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HmiDefrstElecReq();
    }
    QJsonObject object = j.toObject();
    HmiDefrstElecReq v;
    v.FrntElecReq = fromJson<ActrReq>(object["FrntElecReq"]);
    v.ReElecReq = fromJson<ActrReq>(object["ReElecReq"]);
    v.MirrElecReq = fromJson<ActrReq>(object["MirrElecReq"]);
    return v;
}


// ==========================================
// HmiDefrstElecSts
template <>
QJsonValue toJson<HmiDefrstElecSts>(const HmiDefrstElecSts& v) {
    QJsonObject j;
    j["Frnt"] = toJson<ActrDefrstSts>(v.Frnt);
    j["Re"] = toJson<ActrDefrstSts>(v.Re);
    j["Mirrr"] = toJson<ActrDefrstSts>(v.Mirrr);
    return QJsonValue(j);
}
template <>
HmiDefrstElecSts fromJson<HmiDefrstElecSts>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HmiDefrstElecSts();
    }
    QJsonObject object = j.toObject();
    HmiDefrstElecSts v;
    v.Frnt = fromJson<ActrDefrstSts>(object["Frnt"]);
    v.Re = fromJson<ActrDefrstSts>(object["Re"]);
    v.Mirrr = fromJson<ActrDefrstSts>(object["Mirrr"]);
    return v;
}


// ==========================================
// HmiSeatClima
template <>
QJsonValue toJson<HmiSeatClima>(const HmiSeatClima& v) {
    QJsonObject j;
    j["HmiSeatHeatgForRowFirstLe"] = toJson<SeatClimaLvl>(v.HmiSeatHeatgForRowFirstLe);
    j["HmiSeatHeatgForRowFirstRi"] = toJson<SeatClimaLvl>(v.HmiSeatHeatgForRowFirstRi);
    j["HmiSeatHeatgForRowSecLe"] = toJson<SeatClimaLvl>(v.HmiSeatHeatgForRowSecLe);
    j["HmiSeatHeatgForRowSecRi"] = toJson<SeatClimaLvl>(v.HmiSeatHeatgForRowSecRi);
    j["HmiSeatVentnForRowFirstLe"] = toJson<SeatClimaLvl>(v.HmiSeatVentnForRowFirstLe);
    j["HmiSeatVentnForRowFirstRi"] = toJson<SeatClimaLvl>(v.HmiSeatVentnForRowFirstRi);
    return QJsonValue(j);
}
template <>
HmiSeatClima fromJson<HmiSeatClima>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HmiSeatClima();
    }
    QJsonObject object = j.toObject();
    HmiSeatClima v;
    v.HmiSeatHeatgForRowFirstLe = fromJson<SeatClimaLvl>(object["HmiSeatHeatgForRowFirstLe"]);
    v.HmiSeatHeatgForRowFirstRi = fromJson<SeatClimaLvl>(object["HmiSeatHeatgForRowFirstRi"]);
    v.HmiSeatHeatgForRowSecLe = fromJson<SeatClimaLvl>(object["HmiSeatHeatgForRowSecLe"]);
    v.HmiSeatHeatgForRowSecRi = fromJson<SeatClimaLvl>(object["HmiSeatHeatgForRowSecRi"]);
    v.HmiSeatVentnForRowFirstLe = fromJson<SeatClimaLvl>(object["HmiSeatVentnForRowFirstLe"]);
    v.HmiSeatVentnForRowFirstRi = fromJson<SeatClimaLvl>(object["HmiSeatVentnForRowFirstRi"]);
    return v;
}


// ==========================================
// HmiSeatClimaExtd
template <>
QJsonValue toJson<HmiSeatClimaExtd>(const HmiSeatClimaExtd& v) {
    QJsonObject j;
    j["HmiSeatVentnForRowSecLe"] = toJson<SeatClimaLvl>(v.HmiSeatVentnForRowSecLe);
    j["HmiSeatVentnForRowSecRi"] = toJson<SeatClimaLvl>(v.HmiSeatVentnForRowSecRi);
    return QJsonValue(j);
}
template <>
HmiSeatClimaExtd fromJson<HmiSeatClimaExtd>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HmiSeatClimaExtd();
    }
    QJsonObject object = j.toObject();
    HmiSeatClimaExtd v;
    v.HmiSeatVentnForRowSecLe = fromJson<SeatClimaLvl>(object["HmiSeatVentnForRowSecLe"]);
    v.HmiSeatVentnForRowSecRi = fromJson<SeatClimaLvl>(object["HmiSeatVentnForRowSecRi"]);
    return v;
}


// ==========================================
// HudDiagc
template <>
QJsonValue toJson<HudDiagc>(const HudDiagc& v) {
    QJsonObject j;
    j["HudTSts"] = toJson<Err1>(v.HudTSts);
    j["HudCircShoSts"] = toJson<Err1>(v.HudCircShoSts);
    j["HudCricOpenSts"] = toJson<Err1>(v.HudCricOpenSts);
    j["ImgHudTmpNotAvlSts"] = toJson<NotAvlSts1>(v.ImgHudTmpNotAvlSts);
    j["ImgHudErrSts"] = toJson<Err1>(v.ImgHudErrSts);
    return QJsonValue(j);
}
template <>
HudDiagc fromJson<HudDiagc>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HudDiagc();
    }
    QJsonObject object = j.toObject();
    HudDiagc v;
    v.HudTSts = fromJson<Err1>(object["HudTSts"]);
    v.HudCircShoSts = fromJson<Err1>(object["HudCircShoSts"]);
    v.HudCricOpenSts = fromJson<Err1>(object["HudCricOpenSts"]);
    v.ImgHudTmpNotAvlSts = fromJson<NotAvlSts1>(object["ImgHudTmpNotAvlSts"]);
    v.ImgHudErrSts = fromJson<Err1>(object["ImgHudErrSts"]);
    return v;
}


// ==========================================
// HudVisFctSetgReq
template <>
QJsonValue toJson<HudVisFctSetgReq>(const HudVisFctSetgReq& v) {
    QJsonObject j;
    j["Pen"] = toJson<IdPen>(v.Pen);
    j["HudFct00"] = toJson<OnOff1>(v.HudFct00);
    j["HudFct01"] = toJson<OnOff1>(v.HudFct01);
    j["HudFct02"] = toJson<OnOff1>(v.HudFct02);
    j["HudFct03"] = toJson<OnOff1>(v.HudFct03);
    j["HudFct04"] = toJson<OnOff1>(v.HudFct04);
    j["HudFct05"] = toJson<OnOff1>(v.HudFct05);
    j["HudFct06"] = toJson<OnOff1>(v.HudFct06);
    j["HudFct07"] = toJson<OnOff1>(v.HudFct07);
    j["HudFct08"] = toJson<OnOff1>(v.HudFct08);
    j["HudFct09"] = toJson<OnOff1>(v.HudFct09);
    j["HudFct10"] = toJson<OnOff1>(v.HudFct10);
    j["HudFct11"] = toJson<OnOff1>(v.HudFct11);
    j["HudFct12"] = toJson<OnOff1>(v.HudFct12);
    j["HudFct13"] = toJson<OnOff1>(v.HudFct13);
    j["HudFct14"] = toJson<OnOff1>(v.HudFct14);
    j["HudFct15"] = toJson<OnOff1>(v.HudFct15);
    j["HudFct16"] = toJson<OnOff1>(v.HudFct16);
    j["HudFct17"] = toJson<OnOff1>(v.HudFct17);
    j["HudFct18"] = toJson<OnOff1>(v.HudFct18);
    j["HudFct19"] = toJson<OnOff1>(v.HudFct19);
    return QJsonValue(j);
}
template <>
HudVisFctSetgReq fromJson<HudVisFctSetgReq>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HudVisFctSetgReq();
    }
    QJsonObject object = j.toObject();
    HudVisFctSetgReq v;
    v.Pen = fromJson<IdPen>(object["Pen"]);
    v.HudFct00 = fromJson<OnOff1>(object["HudFct00"]);
    v.HudFct01 = fromJson<OnOff1>(object["HudFct01"]);
    v.HudFct02 = fromJson<OnOff1>(object["HudFct02"]);
    v.HudFct03 = fromJson<OnOff1>(object["HudFct03"]);
    v.HudFct04 = fromJson<OnOff1>(object["HudFct04"]);
    v.HudFct05 = fromJson<OnOff1>(object["HudFct05"]);
    v.HudFct06 = fromJson<OnOff1>(object["HudFct06"]);
    v.HudFct07 = fromJson<OnOff1>(object["HudFct07"]);
    v.HudFct08 = fromJson<OnOff1>(object["HudFct08"]);
    v.HudFct09 = fromJson<OnOff1>(object["HudFct09"]);
    v.HudFct10 = fromJson<OnOff1>(object["HudFct10"]);
    v.HudFct11 = fromJson<OnOff1>(object["HudFct11"]);
    v.HudFct12 = fromJson<OnOff1>(object["HudFct12"]);
    v.HudFct13 = fromJson<OnOff1>(object["HudFct13"]);
    v.HudFct14 = fromJson<OnOff1>(object["HudFct14"]);
    v.HudFct15 = fromJson<OnOff1>(object["HudFct15"]);
    v.HudFct16 = fromJson<OnOff1>(object["HudFct16"]);
    v.HudFct17 = fromJson<OnOff1>(object["HudFct17"]);
    v.HudFct18 = fromJson<OnOff1>(object["HudFct18"]);
    v.HudFct19 = fromJson<OnOff1>(object["HudFct19"]);
    return v;
}


// ==========================================
// HwAprvdWirelsAdr1
template <>
QJsonValue toJson<HwAprvdWirelsAdr1>(const HwAprvdWirelsAdr1& v) {
    QJsonObject j;
    j["HwOffsAdr1"] = toJson<uint8_t>(v.HwOffsAdr1);
    j["HwOffsAdr2"] = toJson<uint8_t>(v.HwOffsAdr2);
    j["HwOffsAdr3"] = toJson<uint8_t>(v.HwOffsAdr3);
    j["HwOffsAdr4"] = toJson<uint8_t>(v.HwOffsAdr4);
    j["HwOffsAdr5"] = toJson<uint8_t>(v.HwOffsAdr5);
    j["HwOffsAdr6"] = toJson<uint8_t>(v.HwOffsAdr6);
    j["AprvdSts_"] = toJson<AprvdSts>(v.AprvdSts_);
    return QJsonValue(j);
}
template <>
HwAprvdWirelsAdr1 fromJson<HwAprvdWirelsAdr1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HwAprvdWirelsAdr1();
    }
    QJsonObject object = j.toObject();
    HwAprvdWirelsAdr1 v;
    v.HwOffsAdr1 = fromJson<uint8_t>(object["HwOffsAdr1"]);
    v.HwOffsAdr2 = fromJson<uint8_t>(object["HwOffsAdr2"]);
    v.HwOffsAdr3 = fromJson<uint8_t>(object["HwOffsAdr3"]);
    v.HwOffsAdr4 = fromJson<uint8_t>(object["HwOffsAdr4"]);
    v.HwOffsAdr5 = fromJson<uint8_t>(object["HwOffsAdr5"]);
    v.HwOffsAdr6 = fromJson<uint8_t>(object["HwOffsAdr6"]);
    v.AprvdSts_ = fromJson<AprvdSts>(object["AprvdSts_"]);
    return v;
}


// ==========================================
// HznDataGroup2
template <>
QJsonValue toJson<HznDataGroup2>(const HznDataGroup2& v) {
    QJsonObject j;
    j["MsgTyp"] = toJson<HznMsgTyp1>(v.MsgTyp);
    j["CycCntr"] = toJson<uint8_t>(v.CycCntr);
    j["CtryCod"] = toJson<uint16_t>(v.CtryCod);
    j["RegnCod"] = toJson<uint16_t>(v.RegnCod);
    j["DrvgSide"] = toJson<DrvgSide1>(v.DrvgSide);
    j["SpdUnit"] = toJson<SpdUnit1>(v.SpdUnit);
    j["HdTxVers"] = toJson<uint8_t>(v.HdTxVers);
    j["TxVers"] = toJson<uint8_t>(v.TxVers);
    j["ChdTxVers"] = toJson<uint8_t>(v.ChdTxVers);
    j["HwVers"] = toJson<uint16_t>(v.HwVers);
    j["MapSrc"] = toJson<HznMapSrc2>(v.MapSrc);
    j["YrVersOfMap"] = toJson<uint8_t>(v.YrVersOfMap);
    j["PartOfYrVersOfMap"] = toJson<uint8_t>(v.PartOfYrVersOfMap);
    return QJsonValue(j);
}
template <>
HznDataGroup2 fromJson<HznDataGroup2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HznDataGroup2();
    }
    QJsonObject object = j.toObject();
    HznDataGroup2 v;
    v.MsgTyp = fromJson<HznMsgTyp1>(object["MsgTyp"]);
    v.CycCntr = fromJson<uint8_t>(object["CycCntr"]);
    v.CtryCod = fromJson<uint16_t>(object["CtryCod"]);
    v.RegnCod = fromJson<uint16_t>(object["RegnCod"]);
    v.DrvgSide = fromJson<DrvgSide1>(object["DrvgSide"]);
    v.SpdUnit = fromJson<SpdUnit1>(object["SpdUnit"]);
    v.HdTxVers = fromJson<uint8_t>(object["HdTxVers"]);
    v.TxVers = fromJson<uint8_t>(object["TxVers"]);
    v.ChdTxVers = fromJson<uint8_t>(object["ChdTxVers"]);
    v.HwVers = fromJson<uint16_t>(object["HwVers"]);
    v.MapSrc = fromJson<HznMapSrc2>(object["MapSrc"]);
    v.YrVersOfMap = fromJson<uint8_t>(object["YrVersOfMap"]);
    v.PartOfYrVersOfMap = fromJson<uint8_t>(object["PartOfYrVersOfMap"]);
    return v;
}


// ==========================================
// HznEdgeGroup2
template <>
QJsonValue toJson<HznEdgeGroup2>(const HznEdgeGroup2& v) {
    QJsonObject j;
    j["MsgTyp"] = toJson<HznMsgTyp1>(v.MsgTyp);
    j["CycCntr"] = toJson<uint8_t>(v.CycCntr);
    j["TxPrev"] = toJson<NoYes1>(v.TxPrev);
    j["PahIdx"] = toJson<uint8_t>(v.PahIdx);
    j["Offs"] = toJson<uint16_t>(v.Offs);
    j["Upd"] = toJson<NoYes1>(v.Upd);
    j["PahIdxNew"] = toJson<uint8_t>(v.PahIdxNew);
    j["TurnAg"] = toJson<double>(v.TurnAg);
    j["RelProblty"] = toJson<double>(v.RelProblty);
    j["TypOfWay"] = toJson<HznTypOfWay1>(v.TypOfWay);
    j["NrOfLaneInDrvgDir"] = toJson<HznNrOfLaneInDrvgDir1>(v.NrOfLaneInDrvgDir);
    j["NrOfLaneInSecDir"] = toJson<HznNrOfLaneInSecDir1>(v.NrOfLaneInSecDir);
    j["IntscnCmplx"] = toJson<HznIntscnCmplx1>(v.IntscnCmplx);
    j["RiOfWay"] = toJson<HznRiOfWay1>(v.RiOfWay);
    j["RoadClass"] = toJson<uint8_t>(v.RoadClass);
    j["PartOfCourseCalcd"] = toJson<HznPartOfCourseCalcd1>(v.PartOfCourseCalcd);
    j["LstEdgeAtOffs"] = toJson<NoYes1>(v.LstEdgeAtOffs);
    return QJsonValue(j);
}
template <>
HznEdgeGroup2 fromJson<HznEdgeGroup2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HznEdgeGroup2();
    }
    QJsonObject object = j.toObject();
    HznEdgeGroup2 v;
    v.MsgTyp = fromJson<HznMsgTyp1>(object["MsgTyp"]);
    v.CycCntr = fromJson<uint8_t>(object["CycCntr"]);
    v.TxPrev = fromJson<NoYes1>(object["TxPrev"]);
    v.PahIdx = fromJson<uint8_t>(object["PahIdx"]);
    v.Offs = fromJson<uint16_t>(object["Offs"]);
    v.Upd = fromJson<NoYes1>(object["Upd"]);
    v.PahIdxNew = fromJson<uint8_t>(object["PahIdxNew"]);
    v.TurnAg = fromJson<double>(object["TurnAg"]);
    v.RelProblty = fromJson<double>(object["RelProblty"]);
    v.TypOfWay = fromJson<HznTypOfWay1>(object["TypOfWay"]);
    v.NrOfLaneInDrvgDir = fromJson<HznNrOfLaneInDrvgDir1>(object["NrOfLaneInDrvgDir"]);
    v.NrOfLaneInSecDir = fromJson<HznNrOfLaneInSecDir1>(object["NrOfLaneInSecDir"]);
    v.IntscnCmplx = fromJson<HznIntscnCmplx1>(object["IntscnCmplx"]);
    v.RiOfWay = fromJson<HznRiOfWay1>(object["RiOfWay"]);
    v.RoadClass = fromJson<uint8_t>(object["RoadClass"]);
    v.PartOfCourseCalcd = fromJson<HznPartOfCourseCalcd1>(object["PartOfCourseCalcd"]);
    v.LstEdgeAtOffs = fromJson<NoYes1>(object["LstEdgeAtOffs"]);
    return v;
}


// ==========================================
// HznPosnExtdGroup1
template <>
QJsonValue toJson<HznPosnExtdGroup1>(const HznPosnExtdGroup1& v) {
    QJsonObject j;
    j["MsgTyp"] = toJson<HznMsgTyp1>(v.MsgTyp);
    j["CycCntr"] = toJson<uint8_t>(v.CycCntr);
    j["PahIdx"] = toJson<uint8_t>(v.PahIdx);
    j["Offs"] = toJson<double>(v.Offs);
    j["PosnIdx"] = toJson<uint8_t>(v.PosnIdx);
    j["PosnTiDif"] = toJson<double>(v.PosnTiDif);
    j["Spd"] = toJson<double>(v.Spd);
    j["RelDir"] = toJson<double>(v.RelDir);
    j["PosnProblty"] = toJson<double>(v.PosnProblty);
    j["PosnQly"] = toJson<uint8_t>(v.PosnQly);
    j["LanePrsnt"] = toJson<HznLanePrsnt2>(v.LanePrsnt);
    return QJsonValue(j);
}
template <>
HznPosnExtdGroup1 fromJson<HznPosnExtdGroup1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HznPosnExtdGroup1();
    }
    QJsonObject object = j.toObject();
    HznPosnExtdGroup1 v;
    v.MsgTyp = fromJson<HznMsgTyp1>(object["MsgTyp"]);
    v.CycCntr = fromJson<uint8_t>(object["CycCntr"]);
    v.PahIdx = fromJson<uint8_t>(object["PahIdx"]);
    v.Offs = fromJson<double>(object["Offs"]);
    v.PosnIdx = fromJson<uint8_t>(object["PosnIdx"]);
    v.PosnTiDif = fromJson<double>(object["PosnTiDif"]);
    v.Spd = fromJson<double>(object["Spd"]);
    v.RelDir = fromJson<double>(object["RelDir"]);
    v.PosnProblty = fromJson<double>(object["PosnProblty"]);
    v.PosnQly = fromJson<uint8_t>(object["PosnQly"]);
    v.LanePrsnt = fromJson<HznLanePrsnt2>(object["LanePrsnt"]);
    return v;
}


// ==========================================
// HznPosnGroup3
template <>
QJsonValue toJson<HznPosnGroup3>(const HznPosnGroup3& v) {
    QJsonObject j;
    j["MsgTyp"] = toJson<HznMsgTyp1>(v.MsgTyp);
    j["CycCntr"] = toJson<uint8_t>(v.CycCntr);
    j["PahIdx"] = toJson<uint8_t>(v.PahIdx);
    j["Offs"] = toJson<uint16_t>(v.Offs);
    j["PosnIdx"] = toJson<uint8_t>(v.PosnIdx);
    j["PosnTiDif"] = toJson<double>(v.PosnTiDif);
    j["Spd"] = toJson<double>(v.Spd);
    j["RelDir"] = toJson<double>(v.RelDir);
    j["PosnProblty"] = toJson<double>(v.PosnProblty);
    j["PosnQly"] = toJson<uint8_t>(v.PosnQly);
    j["LanePrsnt"] = toJson<HznLanePrsnt2>(v.LanePrsnt);
    return QJsonValue(j);
}
template <>
HznPosnGroup3 fromJson<HznPosnGroup3>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HznPosnGroup3();
    }
    QJsonObject object = j.toObject();
    HznPosnGroup3 v;
    v.MsgTyp = fromJson<HznMsgTyp1>(object["MsgTyp"]);
    v.CycCntr = fromJson<uint8_t>(object["CycCntr"]);
    v.PahIdx = fromJson<uint8_t>(object["PahIdx"]);
    v.Offs = fromJson<uint16_t>(object["Offs"]);
    v.PosnIdx = fromJson<uint8_t>(object["PosnIdx"]);
    v.PosnTiDif = fromJson<double>(object["PosnTiDif"]);
    v.Spd = fromJson<double>(object["Spd"]);
    v.RelDir = fromJson<double>(object["RelDir"]);
    v.PosnProblty = fromJson<double>(object["PosnProblty"]);
    v.PosnQly = fromJson<uint8_t>(object["PosnQly"]);
    v.LanePrsnt = fromJson<HznLanePrsnt2>(object["LanePrsnt"]);
    return v;
}


// ==========================================
// HznProfLongExtdGroup1
template <>
QJsonValue toJson<HznProfLongExtdGroup1>(const HznProfLongExtdGroup1& v) {
    QJsonObject j;
    j["MsgTyp"] = toJson<HznMsgTyp1>(v.MsgTyp);
    j["CycCntr"] = toJson<uint8_t>(v.CycCntr);
    j["TxPrev"] = toJson<NoYes1>(v.TxPrev);
    j["PahIdx"] = toJson<uint8_t>(v.PahIdx);
    j["Offs"] = toJson<double>(v.Offs);
    j["Upd"] = toJson<NoYes1>(v.Upd);
    j["ProfTyp"] = toJson<HznProfLongTypExtd1>(v.ProfTyp);
    j["NodCtrl"] = toJson<NoYes1>(v.NodCtrl);
    j["Val"] = toJson<uint32_t>(v.Val);
    return QJsonValue(j);
}
template <>
HznProfLongExtdGroup1 fromJson<HznProfLongExtdGroup1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HznProfLongExtdGroup1();
    }
    QJsonObject object = j.toObject();
    HznProfLongExtdGroup1 v;
    v.MsgTyp = fromJson<HznMsgTyp1>(object["MsgTyp"]);
    v.CycCntr = fromJson<uint8_t>(object["CycCntr"]);
    v.TxPrev = fromJson<NoYes1>(object["TxPrev"]);
    v.PahIdx = fromJson<uint8_t>(object["PahIdx"]);
    v.Offs = fromJson<double>(object["Offs"]);
    v.Upd = fromJson<NoYes1>(object["Upd"]);
    v.ProfTyp = fromJson<HznProfLongTypExtd1>(object["ProfTyp"]);
    v.NodCtrl = fromJson<NoYes1>(object["NodCtrl"]);
    v.Val = fromJson<uint32_t>(object["Val"]);
    return v;
}


// ==========================================
// HznProfLongGroup3
template <>
QJsonValue toJson<HznProfLongGroup3>(const HznProfLongGroup3& v) {
    QJsonObject j;
    j["MsgTyp"] = toJson<HznMsgTyp1>(v.MsgTyp);
    j["CycCntr"] = toJson<uint8_t>(v.CycCntr);
    j["TxPrev"] = toJson<NoYes1>(v.TxPrev);
    j["PahIdx"] = toJson<uint8_t>(v.PahIdx);
    j["Offs"] = toJson<uint16_t>(v.Offs);
    j["Upd"] = toJson<NoYes1>(v.Upd);
    j["ProfTyp"] = toJson<HznProfLongTyp>(v.ProfTyp);
    j["NodCtrl"] = toJson<NoYes1>(v.NodCtrl);
    j["Val"] = toJson<uint32_t>(v.Val);
    return QJsonValue(j);
}
template <>
HznProfLongGroup3 fromJson<HznProfLongGroup3>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HznProfLongGroup3();
    }
    QJsonObject object = j.toObject();
    HznProfLongGroup3 v;
    v.MsgTyp = fromJson<HznMsgTyp1>(object["MsgTyp"]);
    v.CycCntr = fromJson<uint8_t>(object["CycCntr"]);
    v.TxPrev = fromJson<NoYes1>(object["TxPrev"]);
    v.PahIdx = fromJson<uint8_t>(object["PahIdx"]);
    v.Offs = fromJson<uint16_t>(object["Offs"]);
    v.Upd = fromJson<NoYes1>(object["Upd"]);
    v.ProfTyp = fromJson<HznProfLongTyp>(object["ProfTyp"]);
    v.NodCtrl = fromJson<NoYes1>(object["NodCtrl"]);
    v.Val = fromJson<uint32_t>(object["Val"]);
    return v;
}


// ==========================================
// HznProfSho2
template <>
QJsonValue toJson<HznProfSho2>(const HznProfSho2& v) {
    QJsonObject j;
    j["MsgTyp"] = toJson<HznMsgTyp1>(v.MsgTyp);
    j["CycCntr"] = toJson<uint8_t>(v.CycCntr);
    j["TxPrev"] = toJson<NoYes1>(v.TxPrev);
    j["PahIdx"] = toJson<uint8_t>(v.PahIdx);
    j["Offs"] = toJson<uint16_t>(v.Offs);
    j["Upd"] = toJson<NoYes1>(v.Upd);
    j["ProfTyp"] = toJson<HznProfShoTyp>(v.ProfTyp);
    j["NodCtrl"] = toJson<NoYes1>(v.NodCtrl);
    j["Val0"] = toJson<uint16_t>(v.Val0);
    j["Dst1"] = toJson<uint16_t>(v.Dst1);
    j["Val1"] = toJson<uint16_t>(v.Val1);
    j["ProfTypQly"] = toJson<HznProfTypQly1>(v.ProfTypQly);
    return QJsonValue(j);
}
template <>
HznProfSho2 fromJson<HznProfSho2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HznProfSho2();
    }
    QJsonObject object = j.toObject();
    HznProfSho2 v;
    v.MsgTyp = fromJson<HznMsgTyp1>(object["MsgTyp"]);
    v.CycCntr = fromJson<uint8_t>(object["CycCntr"]);
    v.TxPrev = fromJson<NoYes1>(object["TxPrev"]);
    v.PahIdx = fromJson<uint8_t>(object["PahIdx"]);
    v.Offs = fromJson<uint16_t>(object["Offs"]);
    v.Upd = fromJson<NoYes1>(object["Upd"]);
    v.ProfTyp = fromJson<HznProfShoTyp>(object["ProfTyp"]);
    v.NodCtrl = fromJson<NoYes1>(object["NodCtrl"]);
    v.Val0 = fromJson<uint16_t>(object["Val0"]);
    v.Dst1 = fromJson<uint16_t>(object["Dst1"]);
    v.Val1 = fromJson<uint16_t>(object["Val1"]);
    v.ProfTypQly = fromJson<HznProfTypQly1>(object["ProfTypQly"]);
    return v;
}


// ==========================================
// HznSegGroup2
template <>
QJsonValue toJson<HznSegGroup2>(const HznSegGroup2& v) {
    QJsonObject j;
    j["MsgTyp"] = toJson<HznMsgTyp1>(v.MsgTyp);
    j["CycCntr"] = toJson<uint8_t>(v.CycCntr);
    j["TxPrev"] = toJson<NoYes1>(v.TxPrev);
    j["PahIdx"] = toJson<uint8_t>(v.PahIdx);
    j["Offs"] = toJson<uint16_t>(v.Offs);
    j["Upd"] = toJson<NoYes1>(v.Upd);
    j["RoadClass"] = toJson<uint8_t>(v.RoadClass);
    j["TypOfWay"] = toJson<HznTypOfWay1>(v.TypOfWay);
    j["SpdLimEfc"] = toJson<HznSpdLimEfc1>(v.SpdLimEfc);
    j["SpdLimTypEfc"] = toJson<HznSpdLimTypEfc1>(v.SpdLimTypEfc);
    j["NrOfLaneInDrvgDir"] = toJson<HznNrOfLaneInDrvgDir1>(v.NrOfLaneInDrvgDir);
    j["NrOfLaneInSecDir"] = toJson<HznNrOfLaneInSecDir1>(v.NrOfLaneInSecDir);
    j["Tnl"] = toJson<HznTnl1>(v.Tnl);
    j["Brdg"] = toJson<HznBrdg1>(v.Brdg);
    j["RoadMpl"] = toJson<HznRoadMpl1>(v.RoadMpl);
    j["RoadLiAr"] = toJson<HznRoadLiAr1>(v.RoadLiAr);
    j["IntscnCmplx"] = toJson<HznIntscnCmplx1>(v.IntscnCmplx);
    j["RelProblty"] = toJson<double>(v.RelProblty);
    j["PartOfCourseCalcd"] = toJson<HznPartOfCourseCalcd1>(v.PartOfCourseCalcd);
    return QJsonValue(j);
}
template <>
HznSegGroup2 fromJson<HznSegGroup2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return HznSegGroup2();
    }
    QJsonObject object = j.toObject();
    HznSegGroup2 v;
    v.MsgTyp = fromJson<HznMsgTyp1>(object["MsgTyp"]);
    v.CycCntr = fromJson<uint8_t>(object["CycCntr"]);
    v.TxPrev = fromJson<NoYes1>(object["TxPrev"]);
    v.PahIdx = fromJson<uint8_t>(object["PahIdx"]);
    v.Offs = fromJson<uint16_t>(object["Offs"]);
    v.Upd = fromJson<NoYes1>(object["Upd"]);
    v.RoadClass = fromJson<uint8_t>(object["RoadClass"]);
    v.TypOfWay = fromJson<HznTypOfWay1>(object["TypOfWay"]);
    v.SpdLimEfc = fromJson<HznSpdLimEfc1>(object["SpdLimEfc"]);
    v.SpdLimTypEfc = fromJson<HznSpdLimTypEfc1>(object["SpdLimTypEfc"]);
    v.NrOfLaneInDrvgDir = fromJson<HznNrOfLaneInDrvgDir1>(object["NrOfLaneInDrvgDir"]);
    v.NrOfLaneInSecDir = fromJson<HznNrOfLaneInSecDir1>(object["NrOfLaneInSecDir"]);
    v.Tnl = fromJson<HznTnl1>(object["Tnl"]);
    v.Brdg = fromJson<HznBrdg1>(object["Brdg"]);
    v.RoadMpl = fromJson<HznRoadMpl1>(object["RoadMpl"]);
    v.RoadLiAr = fromJson<HznRoadLiAr1>(object["RoadLiAr"]);
    v.IntscnCmplx = fromJson<HznIntscnCmplx1>(object["IntscnCmplx"]);
    v.RelProblty = fromJson<double>(object["RelProblty"]);
    v.PartOfCourseCalcd = fromJson<HznPartOfCourseCalcd1>(object["PartOfCourseCalcd"]);
    return v;
}


// ==========================================
// IndcnUnit
template <>
QJsonValue toJson<IndcnUnit>(const IndcnUnit& v) {
    QJsonObject j;
    j["TUnit"] = toJson<AmbTIndcdUnit>(v.TUnit);
    j["SpdUnit"] = toJson<VehSpdIndcdUnit>(v.SpdUnit);
    j["DstLong_"] = toJson<DstLong>(v.DstLong_);
    j["DstSho_"] = toJson<DstSho>(v.DstSho_);
    j["FuCnsUnit"] = toJson<FuCnsIndcdUnit>(v.FuCnsUnit);
    j["VolUnit_"] = toJson<VolUnit>(v.VolUnit_);
    j["IdPenForUnit"] = toJson<IdPen>(v.IdPenForUnit);
    j["ClkFmt_"] = toJson<ClkFmt>(v.ClkFmt_);
    j["PUnit_"] = toJson<PUnit>(v.PUnit_);
    j["DateFmt_"] = toJson<DateFmt>(v.DateFmt_);
    return QJsonValue(j);
}
template <>
IndcnUnit fromJson<IndcnUnit>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return IndcnUnit();
    }
    QJsonObject object = j.toObject();
    IndcnUnit v;
    v.TUnit = fromJson<AmbTIndcdUnit>(object["TUnit"]);
    v.SpdUnit = fromJson<VehSpdIndcdUnit>(object["SpdUnit"]);
    v.DstLong_ = fromJson<DstLong>(object["DstLong_"]);
    v.DstSho_ = fromJson<DstSho>(object["DstSho_"]);
    v.FuCnsUnit = fromJson<FuCnsIndcdUnit>(object["FuCnsUnit"]);
    v.VolUnit_ = fromJson<VolUnit>(object["VolUnit_"]);
    v.IdPenForUnit = fromJson<IdPen>(object["IdPenForUnit"]);
    v.ClkFmt_ = fromJson<ClkFmt>(object["ClkFmt_"]);
    v.PUnit_ = fromJson<PUnit>(object["PUnit_"]);
    v.DateFmt_ = fromJson<DateFmt>(object["DateFmt_"]);
    return v;
}


// ==========================================
// IntrLiAmbLiSetg
template <>
QJsonValue toJson<IntrLiAmbLiSetg>(const IntrLiAmbLiSetg& v) {
    QJsonObject j;
    j["LiInten"] = toJson<IntrLiInten>(v.LiInten);
    j["LiTintg"] = toJson<AmbLiTintg>(v.LiTintg);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
IntrLiAmbLiSetg fromJson<IntrLiAmbLiSetg>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return IntrLiAmbLiSetg();
    }
    QJsonObject object = j.toObject();
    IntrLiAmbLiSetg v;
    v.LiInten = fromJson<IntrLiInten>(object["LiInten"]);
    v.LiTintg = fromJson<AmbLiTintg>(object["LiTintg"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// IntrLiSurrndgsLiSetg
template <>
QJsonValue toJson<IntrLiSurrndgsLiSetg>(const IntrLiSurrndgsLiSetg& v) {
    QJsonObject j;
    j["LiInten"] = toJson<IntrLiInten>(v.LiInten);
    j["LiLvl"] = toJson<SurrndgsLiLvl>(v.LiLvl);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
IntrLiSurrndgsLiSetg fromJson<IntrLiSurrndgsLiSetg>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return IntrLiSurrndgsLiSetg();
    }
    QJsonObject object = j.toObject();
    IntrLiSurrndgsLiSetg v;
    v.LiInten = fromJson<IntrLiInten>(object["LiInten"]);
    v.LiLvl = fromJson<SurrndgsLiLvl>(object["LiLvl"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// KeyProfMpgUpd1
template <>
QJsonValue toJson<KeyProfMpgUpd1>(const KeyProfMpgUpd1& v) {
    QJsonObject j;
    j["KeyProfMpgUpdForIdPen"] = toJson<IdPen>(v.KeyProfMpgUpdForIdPen);
    j["KeyProfMpgUpdOff"] = toJson<bool>(v.KeyProfMpgUpdOff);
    return QJsonValue(j);
}
template <>
KeyProfMpgUpd1 fromJson<KeyProfMpgUpd1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return KeyProfMpgUpd1();
    }
    QJsonObject object = j.toObject();
    KeyProfMpgUpd1 v;
    v.KeyProfMpgUpdForIdPen = fromJson<IdPen>(object["KeyProfMpgUpdForIdPen"]);
    v.KeyProfMpgUpdOff = fromJson<bool>(object["KeyProfMpgUpdOff"]);
    return v;
}


// ==========================================
// KeyReadStsToProfCtrl
template <>
QJsonValue toJson<KeyReadStsToProfCtrl>(const KeyReadStsToProfCtrl& v) {
    QJsonObject j;
    j["KeyId"] = toJson<KeyId1>(v.KeyId);
    j["Boolean"] = toJson<bool>(v.Boolean);
    return QJsonValue(j);
}
template <>
KeyReadStsToProfCtrl fromJson<KeyReadStsToProfCtrl>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return KeyReadStsToProfCtrl();
    }
    QJsonObject object = j.toObject();
    KeyReadStsToProfCtrl v;
    v.KeyId = fromJson<KeyId1>(object["KeyId"]);
    v.Boolean = fromJson<bool>(object["Boolean"]);
    return v;
}


// ==========================================
// LiAutTranPen1
template <>
QJsonValue toJson<LiAutTranPen1>(const LiAutTranPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<LiAutTran1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
LiAutTranPen1 fromJson<LiAutTranPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return LiAutTranPen1();
    }
    QJsonObject object = j.toObject();
    LiAutTranPen1 v;
    v.Sts = fromJson<LiAutTran1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// LiTiPen2
template <>
QJsonValue toJson<LiTiPen2>(const LiTiPen2& v) {
    QJsonObject j;
    j["Sts"] = toJson<LiTi2>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
LiTiPen2 fromJson<LiTiPen2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return LiTiPen2();
    }
    QJsonObject object = j.toObject();
    LiTiPen2 v;
    v.Sts = fromJson<LiTi2>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// LockgCenSts3
template <>
QJsonValue toJson<LockgCenSts3>(const LockgCenSts3& v) {
    QJsonObject j;
    j["LockSt"] = toJson<LockSt3>(v.LockSt);
    j["TrigSrc"] = toJson<LockTrigSrc2>(v.TrigSrc);
    j["UpdEve"] = toJson<bool>(v.UpdEve);
    return QJsonValue(j);
}
template <>
LockgCenSts3 fromJson<LockgCenSts3>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return LockgCenSts3();
    }
    QJsonObject object = j.toObject();
    LockgCenSts3 v;
    v.LockSt = fromJson<LockSt3>(object["LockSt"]);
    v.TrigSrc = fromJson<LockTrigSrc2>(object["TrigSrc"]);
    v.UpdEve = fromJson<bool>(object["UpdEve"]);
    return v;
}


// ==========================================
// LvlSeldForSpdLimAdpvPen1
template <>
QJsonValue toJson<LvlSeldForSpdLimAdpvPen1>(const LvlSeldForSpdLimAdpvPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<Aut1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
LvlSeldForSpdLimAdpvPen1 fromJson<LvlSeldForSpdLimAdpvPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return LvlSeldForSpdLimAdpvPen1();
    }
    QJsonObject object = j.toObject();
    LvlSeldForSpdLimAdpvPen1 v;
    v.Sts = fromJson<Aut1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// MassgFctActv
template <>
QJsonValue toJson<MassgFctActv>(const MassgFctActv& v) {
    QJsonObject j;
    j["DrvrMassgFctActv"] = toJson<OnOff1>(v.DrvrMassgFctActv);
    j["PassMassgFctActv"] = toJson<OnOff1>(v.PassMassgFctActv);
    return QJsonValue(j);
}
template <>
MassgFctActv fromJson<MassgFctActv>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return MassgFctActv();
    }
    QJsonObject object = j.toObject();
    MassgFctActv v;
    v.DrvrMassgFctActv = fromJson<OnOff1>(object["DrvrMassgFctActv"]);
    v.PassMassgFctActv = fromJson<OnOff1>(object["PassMassgFctActv"]);
    return v;
}


// ==========================================
// MirrDimPen
template <>
QJsonValue toJson<MirrDimPen>(const MirrDimPen& v) {
    QJsonObject j;
    j["MirrDimLvl"] = toJson<MirrDimLvlTyp>(v.MirrDimLvl);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
MirrDimPen fromJson<MirrDimPen>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return MirrDimPen();
    }
    QJsonObject object = j.toObject();
    MirrDimPen v;
    v.MirrDimLvl = fromJson<MirrDimLvlTyp>(object["MirrDimLvl"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// MtrlSnsrT
template <>
QJsonValue toJson<MtrlSnsrT>(const MtrlSnsrT& v) {
    QJsonObject j;
    j["MtrlSnsrT"] = toJson<double>(v.MtrlSnsrT);
    j["MtrlSnsrTFacQly_"] = toJson<MtrlSnsrTFacQly>(v.MtrlSnsrTFacQly_);
    return QJsonValue(j);
}
template <>
MtrlSnsrT fromJson<MtrlSnsrT>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return MtrlSnsrT();
    }
    QJsonObject object = j.toObject();
    MtrlSnsrT v;
    v.MtrlSnsrT = fromJson<double>(object["MtrlSnsrT"]);
    v.MtrlSnsrTFacQly_ = fromJson<MtrlSnsrTFacQly>(object["MtrlSnsrTFacQly_"]);
    return v;
}


// ==========================================
// NetActvtRec1
template <>
QJsonValue toJson<NetActvtRec1>(const NetActvtRec1& v) {
    QJsonObject j;
    j["ResourceGroup"] = toJson<uint8_t>(v.ResourceGroup);
    j["Prio"] = toJson<PrioHighNormal>(v.Prio);
    return QJsonValue(j);
}
template <>
NetActvtRec1 fromJson<NetActvtRec1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return NetActvtRec1();
    }
    QJsonObject object = j.toObject();
    NetActvtRec1 v;
    v.ResourceGroup = fromJson<uint8_t>(object["ResourceGroup"]);
    v.Prio = fromJson<PrioHighNormal>(object["Prio"]);
    return v;
}


// ==========================================
// NormSptPen
template <>
QJsonValue toJson<NormSptPen>(const NormSptPen& v) {
    QJsonObject j;
    j["NormSpt"] = toJson<NormSptType>(v.NormSpt);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
NormSptPen fromJson<NormSptPen>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return NormSptPen();
    }
    QJsonObject object = j.toObject();
    NormSptPen v;
    v.NormSpt = fromJson<NormSptType>(object["NormSpt"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// NrSerlNodLIN
template <>
QJsonValue toJson<NrSerlNodLIN>(const NrSerlNodLIN& v) {
    QJsonObject j;
    j["Nr1"] = toJson<uint8_t>(v.Nr1);
    j["Nr2"] = toJson<uint8_t>(v.Nr2);
    j["Nr3"] = toJson<uint8_t>(v.Nr3);
    j["Nr4"] = toJson<uint8_t>(v.Nr4);
    return QJsonValue(j);
}
template <>
NrSerlNodLIN fromJson<NrSerlNodLIN>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return NrSerlNodLIN();
    }
    QJsonObject object = j.toObject();
    NrSerlNodLIN v;
    v.Nr1 = fromJson<uint8_t>(object["Nr1"]);
    v.Nr2 = fromJson<uint8_t>(object["Nr2"]);
    v.Nr3 = fromJson<uint8_t>(object["Nr3"]);
    v.Nr4 = fromJson<uint8_t>(object["Nr4"]);
    return v;
}


// ==========================================
// OffsForSpdWarnSetgPen
template <>
QJsonValue toJson<OffsForSpdWarnSetgPen>(const OffsForSpdWarnSetgPen& v) {
    QJsonObject j;
    j["Sts"] = toJson<uint8_t>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
OffsForSpdWarnSetgPen fromJson<OffsForSpdWarnSetgPen>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return OffsForSpdWarnSetgPen();
    }
    QJsonObject object = j.toObject();
    OffsForSpdWarnSetgPen v;
    v.Sts = fromJson<uint8_t>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// OnOffPen
template <>
QJsonValue toJson<OnOffPen>(const OnOffPen& v) {
    QJsonObject j;
    j["Sts"] = toJson<OnOff1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
OnOffPen fromJson<OnOffPen>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return OnOffPen();
    }
    QJsonObject object = j.toObject();
    OnOffPen v;
    v.Sts = fromJson<OnOff1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// PartNrNodLIN
template <>
QJsonValue toJson<PartNrNodLIN>(const PartNrNodLIN& v) {
    QJsonObject j;
    j["Nr1"] = toJson<uint8_t>(v.Nr1);
    j["Nr2"] = toJson<uint8_t>(v.Nr2);
    j["Nr3"] = toJson<uint8_t>(v.Nr3);
    j["Nr4"] = toJson<uint8_t>(v.Nr4);
    j["EndSgn1"] = toJson<uint8_t>(v.EndSgn1);
    j["EndSgn2"] = toJson<uint8_t>(v.EndSgn2);
    j["EndSgn3"] = toJson<uint8_t>(v.EndSgn3);
    return QJsonValue(j);
}
template <>
PartNrNodLIN fromJson<PartNrNodLIN>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return PartNrNodLIN();
    }
    QJsonObject object = j.toObject();
    PartNrNodLIN v;
    v.Nr1 = fromJson<uint8_t>(object["Nr1"]);
    v.Nr2 = fromJson<uint8_t>(object["Nr2"]);
    v.Nr3 = fromJson<uint8_t>(object["Nr3"]);
    v.Nr4 = fromJson<uint8_t>(object["Nr4"]);
    v.EndSgn1 = fromJson<uint8_t>(object["EndSgn1"]);
    v.EndSgn2 = fromJson<uint8_t>(object["EndSgn2"]);
    v.EndSgn3 = fromJson<uint8_t>(object["EndSgn3"]);
    return v;
}


// ==========================================
// PassSeatSwtSts2
template <>
QJsonValue toJson<PassSeatSwtSts2>(const PassSeatSwtSts2& v) {
    QJsonObject j;
    j["PassSeatSwtSldSts"] = toJson<SwtHozlSts1>(v.PassSeatSwtSldSts);
    j["PassSeatSwtHeiSts"] = toJson<SwtVertSts1>(v.PassSeatSwtHeiSts);
    j["PassSeatSwtHeiFrntSts"] = toJson<SwtVertSts1>(v.PassSeatSwtHeiFrntSts);
    j["PassSeatSwtInclSts"] = toJson<SwtHozlSts1>(v.PassSeatSwtInclSts);
    j["PassSeatSwtAdjmtOfSpplFctHozlSts"] = toJson<SwtHozlSts1>(v.PassSeatSwtAdjmtOfSpplFctHozlSts);
    j["PassSeatSwtAdjmtOfSpplFctVerSts"] = toJson<SwtVertSts1>(v.PassSeatSwtAdjmtOfSpplFctVerSts);
    j["PassSeatSwtSelnOfSpplFctStsSts"] = toJson<SwtVertSts1>(v.PassSeatSwtSelnOfSpplFctStsSts);
    return QJsonValue(j);
}
template <>
PassSeatSwtSts2 fromJson<PassSeatSwtSts2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return PassSeatSwtSts2();
    }
    QJsonObject object = j.toObject();
    PassSeatSwtSts2 v;
    v.PassSeatSwtSldSts = fromJson<SwtHozlSts1>(object["PassSeatSwtSldSts"]);
    v.PassSeatSwtHeiSts = fromJson<SwtVertSts1>(object["PassSeatSwtHeiSts"]);
    v.PassSeatSwtHeiFrntSts = fromJson<SwtVertSts1>(object["PassSeatSwtHeiFrntSts"]);
    v.PassSeatSwtInclSts = fromJson<SwtHozlSts1>(object["PassSeatSwtInclSts"]);
    v.PassSeatSwtAdjmtOfSpplFctHozlSts = fromJson<SwtHozlSts1>(object["PassSeatSwtAdjmtOfSpplFctHozlSts"]);
    v.PassSeatSwtAdjmtOfSpplFctVerSts = fromJson<SwtVertSts1>(object["PassSeatSwtAdjmtOfSpplFctVerSts"]);
    v.PassSeatSwtSelnOfSpplFctStsSts = fromJson<SwtVertSts1>(object["PassSeatSwtSelnOfSpplFctStsSts"]);
    return v;
}


// ==========================================
// PinionSteerAg1Rec
template <>
QJsonValue toJson<PinionSteerAg1Rec>(const PinionSteerAg1Rec& v) {
    QJsonObject j;
    j["PinionSteerAg1"] = toJson<double>(v.PinionSteerAg1);
    j["PinionSteerAg1Qf"] = toJson<GenQf1>(v.PinionSteerAg1Qf);
    return QJsonValue(j);
}
template <>
PinionSteerAg1Rec fromJson<PinionSteerAg1Rec>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return PinionSteerAg1Rec();
    }
    QJsonObject object = j.toObject();
    PinionSteerAg1Rec v;
    v.PinionSteerAg1 = fromJson<double>(object["PinionSteerAg1"]);
    v.PinionSteerAg1Qf = fromJson<GenQf1>(object["PinionSteerAg1Qf"]);
    return v;
}


// ==========================================
// PosnFromNav
template <>
QJsonValue toJson<PosnFromNav>(const PosnFromNav& v) {
    QJsonObject j;
    j["PosnLat"] = toJson<double>(v.PosnLat);
    j["PosnLgt"] = toJson<double>(v.PosnLgt);
    j["PosnAlti"] = toJson<double>(v.PosnAlti);
    j["PosnSpd"] = toJson<double>(v.PosnSpd);
    j["PosnDir"] = toJson<double>(v.PosnDir);
    j["PosnQly"] = toJson<PosnFromNavQly>(v.PosnQly);
    return QJsonValue(j);
}
template <>
PosnFromNav fromJson<PosnFromNav>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return PosnFromNav();
    }
    QJsonObject object = j.toObject();
    PosnFromNav v;
    v.PosnLat = fromJson<double>(object["PosnLat"]);
    v.PosnLgt = fromJson<double>(object["PosnLgt"]);
    v.PosnAlti = fromJson<double>(object["PosnAlti"]);
    v.PosnSpd = fromJson<double>(object["PosnSpd"]);
    v.PosnDir = fromJson<double>(object["PosnDir"]);
    v.PosnQly = fromJson<PosnFromNavQly>(object["PosnQly"]);
    return v;
}


// ==========================================
// PosnFromSatlt
template <>
QJsonValue toJson<PosnFromSatlt>(const PosnFromSatlt& v) {
    QJsonObject j;
    j["PosnLat"] = toJson<double>(v.PosnLat);
    j["PosnLgt"] = toJson<double>(v.PosnLgt);
    j["PosnAlti"] = toJson<double>(v.PosnAlti);
    j["PosnSpd"] = toJson<double>(v.PosnSpd);
    j["PosnVHozl"] = toJson<double>(v.PosnVHozl);
    j["PosnVVert"] = toJson<double>(v.PosnVVert);
    j["PosnDir"] = toJson<double>(v.PosnDir);
    j["TiForYr"] = toJson<uint8_t>(v.TiForYr);
    j["TiForMth"] = toJson<uint8_t>(v.TiForMth);
    j["TiForDay"] = toJson<uint8_t>(v.TiForDay);
    j["TiForHr"] = toJson<uint8_t>(v.TiForHr);
    j["TiForMins"] = toJson<uint8_t>(v.TiForMins);
    j["TiForSec"] = toJson<uint8_t>(v.TiForSec);
    j["SatltSysNo1InUse"] = toJson<NoYes1>(v.SatltSysNo1InUse);
    j["SatltSysNo2InUse"] = toJson<NoYes1>(v.SatltSysNo2InUse);
    j["SatltSysNo3InUse"] = toJson<NoYes1>(v.SatltSysNo3InUse);
    j["SatltSysNo4InUse"] = toJson<NoYes1>(v.SatltSysNo4InUse);
    j["SatltSysNo5InUse"] = toJson<NoYes1>(v.SatltSysNo5InUse);
    j["SatltSysNo6InUse"] = toJson<NoYes1>(v.SatltSysNo6InUse);
    j["SatltPosnStsPrm1"] = toJson<SatltPosnSts>(v.SatltPosnStsPrm1);
    j["SatltPosnStsPrm2"] = toJson<bool>(v.SatltPosnStsPrm2);
    j["SatltPosnStsPrm3"] = toJson<bool>(v.SatltPosnStsPrm3);
    j["NoOfSatltForSysNo1"] = toJson<uint8_t>(v.NoOfSatltForSysNo1);
    j["NoOfSatltForSysNo2"] = toJson<uint8_t>(v.NoOfSatltForSysNo2);
    j["NoOfSatltForSysNo3"] = toJson<uint8_t>(v.NoOfSatltForSysNo3);
    j["NoOfSatltForSysNo4"] = toJson<uint8_t>(v.NoOfSatltForSysNo4);
    j["NoOfSatltForSysNo5"] = toJson<uint8_t>(v.NoOfSatltForSysNo5);
    j["NoOfSatltForSysNo6"] = toJson<uint8_t>(v.NoOfSatltForSysNo6);
    j["PrePosnDil"] = toJson<double>(v.PrePosnDil);
    j["PreHozlDil"] = toJson<double>(v.PreHozlDil);
    j["PreVertDil"] = toJson<double>(v.PreVertDil);
    j["PreTiDil"] = toJson<double>(v.PreTiDil);
    return QJsonValue(j);
}
template <>
PosnFromSatlt fromJson<PosnFromSatlt>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return PosnFromSatlt();
    }
    QJsonObject object = j.toObject();
    PosnFromSatlt v;
    v.PosnLat = fromJson<double>(object["PosnLat"]);
    v.PosnLgt = fromJson<double>(object["PosnLgt"]);
    v.PosnAlti = fromJson<double>(object["PosnAlti"]);
    v.PosnSpd = fromJson<double>(object["PosnSpd"]);
    v.PosnVHozl = fromJson<double>(object["PosnVHozl"]);
    v.PosnVVert = fromJson<double>(object["PosnVVert"]);
    v.PosnDir = fromJson<double>(object["PosnDir"]);
    v.TiForYr = fromJson<uint8_t>(object["TiForYr"]);
    v.TiForMth = fromJson<uint8_t>(object["TiForMth"]);
    v.TiForDay = fromJson<uint8_t>(object["TiForDay"]);
    v.TiForHr = fromJson<uint8_t>(object["TiForHr"]);
    v.TiForMins = fromJson<uint8_t>(object["TiForMins"]);
    v.TiForSec = fromJson<uint8_t>(object["TiForSec"]);
    v.SatltSysNo1InUse = fromJson<NoYes1>(object["SatltSysNo1InUse"]);
    v.SatltSysNo2InUse = fromJson<NoYes1>(object["SatltSysNo2InUse"]);
    v.SatltSysNo3InUse = fromJson<NoYes1>(object["SatltSysNo3InUse"]);
    v.SatltSysNo4InUse = fromJson<NoYes1>(object["SatltSysNo4InUse"]);
    v.SatltSysNo5InUse = fromJson<NoYes1>(object["SatltSysNo5InUse"]);
    v.SatltSysNo6InUse = fromJson<NoYes1>(object["SatltSysNo6InUse"]);
    v.SatltPosnStsPrm1 = fromJson<SatltPosnSts>(object["SatltPosnStsPrm1"]);
    v.SatltPosnStsPrm2 = fromJson<bool>(object["SatltPosnStsPrm2"]);
    v.SatltPosnStsPrm3 = fromJson<bool>(object["SatltPosnStsPrm3"]);
    v.NoOfSatltForSysNo1 = fromJson<uint8_t>(object["NoOfSatltForSysNo1"]);
    v.NoOfSatltForSysNo2 = fromJson<uint8_t>(object["NoOfSatltForSysNo2"]);
    v.NoOfSatltForSysNo3 = fromJson<uint8_t>(object["NoOfSatltForSysNo3"]);
    v.NoOfSatltForSysNo4 = fromJson<uint8_t>(object["NoOfSatltForSysNo4"]);
    v.NoOfSatltForSysNo5 = fromJson<uint8_t>(object["NoOfSatltForSysNo5"]);
    v.NoOfSatltForSysNo6 = fromJson<uint8_t>(object["NoOfSatltForSysNo6"]);
    v.PrePosnDil = fromJson<double>(object["PrePosnDil"]);
    v.PreHozlDil = fromJson<double>(object["PreHozlDil"]);
    v.PreVertDil = fromJson<double>(object["PreVertDil"]);
    v.PreTiDil = fromJson<double>(object["PreTiDil"]);
    return v;
}


// ==========================================
// ProfAct1
template <>
QJsonValue toJson<ProfAct1>(const ProfAct1& v) {
    QJsonObject j;
    j["Prof1"] = toJson<bool>(v.Prof1);
    j["Prof2"] = toJson<bool>(v.Prof2);
    j["Prof3"] = toJson<bool>(v.Prof3);
    j["Prof4"] = toJson<bool>(v.Prof4);
    j["Prof5"] = toJson<bool>(v.Prof5);
    j["Prof6"] = toJson<bool>(v.Prof6);
    j["Prof7"] = toJson<bool>(v.Prof7);
    j["Prof8"] = toJson<bool>(v.Prof8);
    j["Prof9"] = toJson<bool>(v.Prof9);
    j["Prof10"] = toJson<bool>(v.Prof10);
    j["Prof11"] = toJson<bool>(v.Prof11);
    j["Prof12"] = toJson<bool>(v.Prof12);
    return QJsonValue(j);
}
template <>
ProfAct1 fromJson<ProfAct1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return ProfAct1();
    }
    QJsonObject object = j.toObject();
    ProfAct1 v;
    v.Prof1 = fromJson<bool>(object["Prof1"]);
    v.Prof2 = fromJson<bool>(object["Prof2"]);
    v.Prof3 = fromJson<bool>(object["Prof3"]);
    v.Prof4 = fromJson<bool>(object["Prof4"]);
    v.Prof5 = fromJson<bool>(object["Prof5"]);
    v.Prof6 = fromJson<bool>(object["Prof6"]);
    v.Prof7 = fromJson<bool>(object["Prof7"]);
    v.Prof8 = fromJson<bool>(object["Prof8"]);
    v.Prof9 = fromJson<bool>(object["Prof9"]);
    v.Prof10 = fromJson<bool>(object["Prof10"]);
    v.Prof11 = fromJson<bool>(object["Prof11"]);
    v.Prof12 = fromJson<bool>(object["Prof12"]);
    return v;
}


// ==========================================
// ProfLimd1
template <>
QJsonValue toJson<ProfLimd1>(const ProfLimd1& v) {
    QJsonObject j;
    j["Prof1"] = toJson<bool>(v.Prof1);
    j["Prof2"] = toJson<bool>(v.Prof2);
    j["Prof3"] = toJson<bool>(v.Prof3);
    j["Prof4"] = toJson<bool>(v.Prof4);
    j["Prof5"] = toJson<bool>(v.Prof5);
    j["Prof6"] = toJson<bool>(v.Prof6);
    j["Prof7"] = toJson<bool>(v.Prof7);
    j["Prof8"] = toJson<bool>(v.Prof8);
    j["Prof9"] = toJson<bool>(v.Prof9);
    j["Prof10"] = toJson<bool>(v.Prof10);
    j["Prof11"] = toJson<bool>(v.Prof11);
    j["Prof12"] = toJson<bool>(v.Prof12);
    return QJsonValue(j);
}
template <>
ProfLimd1 fromJson<ProfLimd1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return ProfLimd1();
    }
    QJsonObject object = j.toObject();
    ProfLimd1 v;
    v.Prof1 = fromJson<bool>(object["Prof1"]);
    v.Prof2 = fromJson<bool>(object["Prof2"]);
    v.Prof3 = fromJson<bool>(object["Prof3"]);
    v.Prof4 = fromJson<bool>(object["Prof4"]);
    v.Prof5 = fromJson<bool>(object["Prof5"]);
    v.Prof6 = fromJson<bool>(object["Prof6"]);
    v.Prof7 = fromJson<bool>(object["Prof7"]);
    v.Prof8 = fromJson<bool>(object["Prof8"]);
    v.Prof9 = fromJson<bool>(object["Prof9"]);
    v.Prof10 = fromJson<bool>(object["Prof10"]);
    v.Prof11 = fromJson<bool>(object["Prof11"]);
    v.Prof12 = fromJson<bool>(object["Prof12"]);
    return v;
}


// ==========================================
// PtCluTq1
template <>
QJsonValue toJson<PtCluTq1>(const PtCluTq1& v) {
    QJsonObject j;
    j["PtCluTq"] = toJson<int16_t>(v.PtCluTq);
    j["PtCluTqDyn"] = toJson<int16_t>(v.PtCluTqDyn);
    j["PtCluTqQly"] = toJson<Qly3>(v.PtCluTqQly);
    return QJsonValue(j);
}
template <>
PtCluTq1 fromJson<PtCluTq1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return PtCluTq1();
    }
    QJsonObject object = j.toObject();
    PtCluTq1 v;
    v.PtCluTq = fromJson<int16_t>(object["PtCluTq"]);
    v.PtCluTqDyn = fromJson<int16_t>(object["PtCluTqDyn"]);
    v.PtCluTqQly = fromJson<Qly3>(object["PtCluTqQly"]);
    return v;
}


// ==========================================
// PtDrvrSetg2
template <>
QJsonValue toJson<PtDrvrSetg2>(const PtDrvrSetg2& v) {
    QJsonObject j;
    j["Pen"] = toJson<IdPen>(v.Pen);
    j["Sts"] = toJson<PtDrvrModSetg2>(v.Sts);
    return QJsonValue(j);
}
template <>
PtDrvrSetg2 fromJson<PtDrvrSetg2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return PtDrvrSetg2();
    }
    QJsonObject object = j.toObject();
    PtDrvrSetg2 v;
    v.Pen = fromJson<IdPen>(object["Pen"]);
    v.Sts = fromJson<PtDrvrModSetg2>(object["Sts"]);
    return v;
}


// ==========================================
// PtTqAtWhlFrntActRec1
template <>
QJsonValue toJson<PtTqAtWhlFrntActRec1>(const PtTqAtWhlFrntActRec1& v) {
    QJsonObject j;
    j["PtTqAtWhlFrntLeAct"] = toJson<int16_t>(v.PtTqAtWhlFrntLeAct);
    j["PtTqAtWhlFrntRiAct"] = toJson<int16_t>(v.PtTqAtWhlFrntRiAct);
    j["PtTqAtAxleFrntAct"] = toJson<int16_t>(v.PtTqAtAxleFrntAct);
    j["PtTqAtWhlsFrntQly"] = toJson<Qly3>(v.PtTqAtWhlsFrntQly);
    return QJsonValue(j);
}
template <>
PtTqAtWhlFrntActRec1 fromJson<PtTqAtWhlFrntActRec1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return PtTqAtWhlFrntActRec1();
    }
    QJsonObject object = j.toObject();
    PtTqAtWhlFrntActRec1 v;
    v.PtTqAtWhlFrntLeAct = fromJson<int16_t>(object["PtTqAtWhlFrntLeAct"]);
    v.PtTqAtWhlFrntRiAct = fromJson<int16_t>(object["PtTqAtWhlFrntRiAct"]);
    v.PtTqAtAxleFrntAct = fromJson<int16_t>(object["PtTqAtAxleFrntAct"]);
    v.PtTqAtWhlsFrntQly = fromJson<Qly3>(object["PtTqAtWhlsFrntQly"]);
    return v;
}


// ==========================================
// RngBdIllmnCmdPen1
template <>
QJsonValue toJson<RngBdIllmnCmdPen1>(const RngBdIllmnCmdPen1& v) {
    QJsonObject j;
    j["Cmd"] = toJson<RngbdIllmnCmd1>(v.Cmd);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
RngBdIllmnCmdPen1 fromJson<RngBdIllmnCmdPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return RngBdIllmnCmdPen1();
    }
    QJsonObject object = j.toObject();
    RngBdIllmnCmdPen1 v;
    v.Cmd = fromJson<RngbdIllmnCmd1>(object["Cmd"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// RsdsSysSts
template <>
QJsonValue toJson<RsdsSysSts>(const RsdsSysSts& v) {
    QJsonObject j;
    j["RsdsSts"] = toJson<RsdsMstSt>(v.RsdsSts);
    j["LcmaSts"] = toJson<OnOff1>(v.LcmaSts);
    j["LcmaSts1"] = toJson<ActvInActv>(v.LcmaSts1);
    j["CtraSts"] = toJson<OnOff1>(v.CtraSts);
    j["CtraSts1"] = toJson<ActvInActv>(v.CtraSts1);
    j["RcwSts"] = toJson<OnOff1>(v.RcwSts);
    j["RcmSts"] = toJson<OnOff1>(v.RcmSts);
    return QJsonValue(j);
}
template <>
RsdsSysSts fromJson<RsdsSysSts>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return RsdsSysSts();
    }
    QJsonObject object = j.toObject();
    RsdsSysSts v;
    v.RsdsSts = fromJson<RsdsMstSt>(object["RsdsSts"]);
    v.LcmaSts = fromJson<OnOff1>(object["LcmaSts"]);
    v.LcmaSts1 = fromJson<ActvInActv>(object["LcmaSts1"]);
    v.CtraSts = fromJson<OnOff1>(object["CtraSts"]);
    v.CtraSts1 = fromJson<ActvInActv>(object["CtraSts1"]);
    v.RcwSts = fromJson<OnOff1>(object["RcwSts"]);
    v.RcmSts = fromJson<OnOff1>(object["RcmSts"]);
    return v;
}


// ==========================================
// SeatBackUnlckd
template <>
QJsonValue toJson<SeatBackUnlckd>(const SeatBackUnlckd& v) {
    QJsonObject j;
    j["SeatBackUnlckdLe"] = toJson<bool>(v.SeatBackUnlckdLe);
    j["SeatBackUnlckdRi"] = toJson<bool>(v.SeatBackUnlckdRi);
    return QJsonValue(j);
}
template <>
SeatBackUnlckd fromJson<SeatBackUnlckd>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SeatBackUnlckd();
    }
    QJsonObject object = j.toObject();
    SeatBackUnlckd v;
    v.SeatBackUnlckdLe = fromJson<bool>(object["SeatBackUnlckdLe"]);
    v.SeatBackUnlckdRi = fromJson<bool>(object["SeatBackUnlckdRi"]);
    return v;
}


// ==========================================
// SeatDispBtnPsd
template <>
QJsonValue toJson<SeatDispBtnPsd>(const SeatDispBtnPsd& v) {
    QJsonObject j;
    j["DrvrSeatDispBtnPsd"] = toJson<bool>(v.DrvrSeatDispBtnPsd);
    j["PassSeatDispBtnPsd"] = toJson<bool>(v.PassSeatDispBtnPsd);
    return QJsonValue(j);
}
template <>
SeatDispBtnPsd fromJson<SeatDispBtnPsd>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SeatDispBtnPsd();
    }
    QJsonObject object = j.toObject();
    SeatDispBtnPsd v;
    v.DrvrSeatDispBtnPsd = fromJson<bool>(object["DrvrSeatDispBtnPsd"]);
    v.PassSeatDispBtnPsd = fromJson<bool>(object["PassSeatDispBtnPsd"]);
    return v;
}


// ==========================================
// SeatMassgFct
template <>
QJsonValue toJson<SeatMassgFct>(const SeatMassgFct& v) {
    QJsonObject j;
    j["OnOff"] = toJson<OnOff1>(v.OnOff);
    j["MassgProg"] = toJson<MassgProgTyp>(v.MassgProg);
    j["MassgInten"] = toJson<MassgIntenLvl>(v.MassgInten);
    j["MassgSpdLvl"] = toJson<MassgIntenLvl>(v.MassgSpdLvl);
    return QJsonValue(j);
}
template <>
SeatMassgFct fromJson<SeatMassgFct>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SeatMassgFct();
    }
    QJsonObject object = j.toObject();
    SeatMassgFct v;
    v.OnOff = fromJson<OnOff1>(object["OnOff"]);
    v.MassgProg = fromJson<MassgProgTyp>(object["MassgProg"]);
    v.MassgInten = fromJson<MassgIntenLvl>(object["MassgInten"]);
    v.MassgSpdLvl = fromJson<MassgIntenLvl>(object["MassgSpdLvl"]);
    return v;
}


// ==========================================
// SetOfLang
template <>
QJsonValue toJson<SetOfLang>(const SetOfLang& v) {
    QJsonObject j;
    j["IdPen_"] = toJson<IdPen>(v.IdPen_);
    j["LangTyp_"] = toJson<LangTyp>(v.LangTyp_);
    return QJsonValue(j);
}
template <>
SetOfLang fromJson<SetOfLang>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SetOfLang();
    }
    QJsonObject object = j.toObject();
    SetOfLang v;
    v.IdPen_ = fromJson<IdPen>(object["IdPen_"]);
    v.LangTyp_ = fromJson<LangTyp>(object["LangTyp_"]);
    return v;
}


// ==========================================
// SftySigGroupFromAudSafe1
template <>
QJsonValue toJson<SftySigGroupFromAudSafe1>(const SftySigGroupFromAudSafe1& v) {
    QJsonObject j;
    j["SftySigFaildDetdByAud"] = toJson<NoYesCrit1>(v.SftySigFaildDetdByAud);
    j["SftyAudDend"] = toJson<NoYesCrit1>(v.SftyAudDend);
    j["SftyAudEna"] = toJson<NoYesCrit1>(v.SftyAudEna);
    j["SftySigGroupFromAudSafeChks"] = toJson<uint8_t>(v.SftySigGroupFromAudSafeChks);
    j["SftySigGroupFromAudSafeCntr"] = toJson<uint8_t>(v.SftySigGroupFromAudSafeCntr);
    return QJsonValue(j);
}
template <>
SftySigGroupFromAudSafe1 fromJson<SftySigGroupFromAudSafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SftySigGroupFromAudSafe1();
    }
    QJsonObject object = j.toObject();
    SftySigGroupFromAudSafe1 v;
    v.SftySigFaildDetdByAud = fromJson<NoYesCrit1>(object["SftySigFaildDetdByAud"]);
    v.SftyAudDend = fromJson<NoYesCrit1>(object["SftyAudDend"]);
    v.SftyAudEna = fromJson<NoYesCrit1>(object["SftyAudEna"]);
    v.SftySigGroupFromAudSafeChks = fromJson<uint8_t>(object["SftySigGroupFromAudSafeChks"]);
    v.SftySigGroupFromAudSafeCntr = fromJson<uint8_t>(object["SftySigGroupFromAudSafeCntr"]);
    return v;
}


// ==========================================
// SftyWarnGroupFromAsySafe1
template <>
QJsonValue toJson<SftyWarnGroupFromAsySafe1>(const SftyWarnGroupFromAsySafe1& v) {
    QJsonObject j;
    j["CnclWarnLatForAutDrv"] = toJson<NoYesCrit1>(v.CnclWarnLatForAutDrv);
    j["SteerOvrdWarnReqForAutDrv"] = toJson<NoYesCrit1>(v.SteerOvrdWarnReqForAutDrv);
    j["SteerStsForAutDrv"] = toJson<SteerStsForAutDrv3>(v.SteerStsForAutDrv);
    j["SftyWarnGroupFromAsySafeChks"] = toJson<uint8_t>(v.SftyWarnGroupFromAsySafeChks);
    j["SftyWarnGroupFromAsySafeCntr"] = toJson<uint8_t>(v.SftyWarnGroupFromAsySafeCntr);
    return QJsonValue(j);
}
template <>
SftyWarnGroupFromAsySafe1 fromJson<SftyWarnGroupFromAsySafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SftyWarnGroupFromAsySafe1();
    }
    QJsonObject object = j.toObject();
    SftyWarnGroupFromAsySafe1 v;
    v.CnclWarnLatForAutDrv = fromJson<NoYesCrit1>(object["CnclWarnLatForAutDrv"]);
    v.SteerOvrdWarnReqForAutDrv = fromJson<NoYesCrit1>(object["SteerOvrdWarnReqForAutDrv"]);
    v.SteerStsForAutDrv = fromJson<SteerStsForAutDrv3>(object["SteerStsForAutDrv"]);
    v.SftyWarnGroupFromAsySafeChks = fromJson<uint8_t>(object["SftyWarnGroupFromAsySafeChks"]);
    v.SftyWarnGroupFromAsySafeCntr = fromJson<uint8_t>(object["SftyWarnGroupFromAsySafeCntr"]);
    return v;
}


// ==========================================
// SlowNormFastPen
template <>
QJsonValue toJson<SlowNormFastPen>(const SlowNormFastPen& v) {
    QJsonObject j;
    j["SlowNormFast_"] = toJson<SlowNormFast>(v.SlowNormFast_);
    j["IdPen_"] = toJson<IdPen>(v.IdPen_);
    return QJsonValue(j);
}
template <>
SlowNormFastPen fromJson<SlowNormFastPen>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SlowNormFastPen();
    }
    QJsonObject object = j.toObject();
    SlowNormFastPen v;
    v.SlowNormFast_ = fromJson<SlowNormFast>(object["SlowNormFast_"]);
    v.IdPen_ = fromJson<IdPen>(object["IdPen_"]);
    return v;
}


// ==========================================
// SnsrParkAssi1
template <>
QJsonValue toJson<SnsrParkAssi1>(const SnsrParkAssi1& v) {
    QJsonObject j;
    j["DstOfSnsrInsdLe"] = toJson<DstOfSnsr1>(v.DstOfSnsrInsdLe);
    j["DstOfSnsrOutdLe"] = toJson<DstOfSnsr1>(v.DstOfSnsrOutdLe);
    j["DstOfSnsrLeSide"] = toJson<DstOfSnsr1>(v.DstOfSnsrLeSide);
    j["DstOfSnsrInsdRi"] = toJson<DstOfSnsr1>(v.DstOfSnsrInsdRi);
    j["DstOfSnsrOutdRi"] = toJson<DstOfSnsr1>(v.DstOfSnsrOutdRi);
    j["DstOfSnsrRiSide"] = toJson<DstOfSnsr1>(v.DstOfSnsrRiSide);
    j["AudWarnDir"] = toJson<WarnDir1>(v.AudWarnDir);
    j["AudSideWarn"] = toJson<ParkAssiLeRi1>(v.AudSideWarn);
    return QJsonValue(j);
}
template <>
SnsrParkAssi1 fromJson<SnsrParkAssi1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SnsrParkAssi1();
    }
    QJsonObject object = j.toObject();
    SnsrParkAssi1 v;
    v.DstOfSnsrInsdLe = fromJson<DstOfSnsr1>(object["DstOfSnsrInsdLe"]);
    v.DstOfSnsrOutdLe = fromJson<DstOfSnsr1>(object["DstOfSnsrOutdLe"]);
    v.DstOfSnsrLeSide = fromJson<DstOfSnsr1>(object["DstOfSnsrLeSide"]);
    v.DstOfSnsrInsdRi = fromJson<DstOfSnsr1>(object["DstOfSnsrInsdRi"]);
    v.DstOfSnsrOutdRi = fromJson<DstOfSnsr1>(object["DstOfSnsrOutdRi"]);
    v.DstOfSnsrRiSide = fromJson<DstOfSnsr1>(object["DstOfSnsrRiSide"]);
    v.AudWarnDir = fromJson<WarnDir1>(object["AudWarnDir"]);
    v.AudSideWarn = fromJson<ParkAssiLeRi1>(object["AudSideWarn"]);
    return v;
}


// ==========================================
// SnvtyPen1
template <>
QJsonValue toJson<SnvtyPen1>(const SnvtyPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<Snvty1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
SnvtyPen1 fromJson<SnvtyPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SnvtyPen1();
    }
    QJsonObject object = j.toObject();
    SnvtyPen1 v;
    v.Sts = fromJson<Snvty1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// SnvtyPen3
template <>
QJsonValue toJson<SnvtyPen3>(const SnvtyPen3& v) {
    QJsonObject j;
    j["Sts"] = toJson<Snvty4>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
SnvtyPen3 fromJson<SnvtyPen3>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SnvtyPen3();
    }
    QJsonObject object = j.toObject();
    SnvtyPen3 v;
    v.Sts = fromJson<Snvty4>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// SteerAssiLvlForAutDrvPen1
template <>
QJsonValue toJson<SteerAssiLvlForAutDrvPen1>(const SteerAssiLvlForAutDrvPen1& v) {
    QJsonObject j;
    j["Pen"] = toJson<IdPen>(v.Pen);
    j["Sts"] = toJson<Lvl2>(v.Sts);
    return QJsonValue(j);
}
template <>
SteerAssiLvlForAutDrvPen1 fromJson<SteerAssiLvlForAutDrvPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SteerAssiLvlForAutDrvPen1();
    }
    QJsonObject object = j.toObject();
    SteerAssiLvlForAutDrvPen1 v;
    v.Pen = fromJson<IdPen>(object["Pen"]);
    v.Sts = fromJson<Lvl2>(object["Sts"]);
    return v;
}


// ==========================================
// SteerSetg
template <>
QJsonValue toJson<SteerSetg>(const SteerSetg& v) {
    QJsonObject j;
    j["SteerAsscLvl_"] = toJson<SteerAsscLvl>(v.SteerAsscLvl_);
    j["SteerMod_"] = toJson<SteerMod>(v.SteerMod_);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
SteerSetg fromJson<SteerSetg>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SteerSetg();
    }
    QJsonObject object = j.toObject();
    SteerSetg v;
    v.SteerAsscLvl_ = fromJson<SteerAsscLvl>(object["SteerAsscLvl_"]);
    v.SteerMod_ = fromJson<SteerMod>(object["SteerMod_"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// SteerWhlSnsrSafe1
template <>
QJsonValue toJson<SteerWhlSnsrSafe1>(const SteerWhlSnsrSafe1& v) {
    QJsonObject j;
    j["SteerWhlAg"] = toJson<double>(v.SteerWhlAg);
    j["SteerWhlAgSpd"] = toJson<double>(v.SteerWhlAgSpd);
    j["SteerWhlSnsrQf"] = toJson<GenQf1>(v.SteerWhlSnsrQf);
    j["SteerWhlSnsrChks"] = toJson<uint8_t>(v.SteerWhlSnsrChks);
    j["SteerWhlSnsrCntr"] = toJson<uint8_t>(v.SteerWhlSnsrCntr);
    return QJsonValue(j);
}
template <>
SteerWhlSnsrSafe1 fromJson<SteerWhlSnsrSafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SteerWhlSnsrSafe1();
    }
    QJsonObject object = j.toObject();
    SteerWhlSnsrSafe1 v;
    v.SteerWhlAg = fromJson<double>(object["SteerWhlAg"]);
    v.SteerWhlAgSpd = fromJson<double>(object["SteerWhlAgSpd"]);
    v.SteerWhlSnsrQf = fromJson<GenQf1>(object["SteerWhlSnsrQf"]);
    v.SteerWhlSnsrChks = fromJson<uint8_t>(object["SteerWhlSnsrChks"]);
    v.SteerWhlSnsrCntr = fromJson<uint8_t>(object["SteerWhlSnsrCntr"]);
    return v;
}


// ==========================================
// SuspSetgRec
template <>
QJsonValue toJson<SuspSetgRec>(const SuspSetgRec& v) {
    QJsonObject j;
    j["SuspIdPen"] = toJson<IdPen>(v.SuspIdPen);
    j["LvlCtrlSetg"] = toJson<LvlCtrlSetgTyp>(v.LvlCtrlSetg);
    j["SuspCtrlSetg"] = toJson<SuspCtrlSetgTyp>(v.SuspCtrlSetg);
    return QJsonValue(j);
}
template <>
SuspSetgRec fromJson<SuspSetgRec>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SuspSetgRec();
    }
    QJsonObject object = j.toObject();
    SuspSetgRec v;
    v.SuspIdPen = fromJson<IdPen>(object["SuspIdPen"]);
    v.LvlCtrlSetg = fromJson<LvlCtrlSetgTyp>(object["LvlCtrlSetg"]);
    v.SuspCtrlSetg = fromJson<SuspCtrlSetgTyp>(object["SuspCtrlSetg"]);
    return v;
}


// ==========================================
// SysU
template <>
QJsonValue toJson<SysU>(const SysU& v) {
    QJsonObject j;
    j["SysU"] = toJson<double>(v.SysU);
    j["SysUQf"] = toJson<GenQf1>(v.SysUQf);
    return QJsonValue(j);
}
template <>
SysU fromJson<SysU>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return SysU();
    }
    QJsonObject object = j.toObject();
    SysU v;
    v.SysU = fromJson<double>(object["SysU"]);
    v.SysUQf = fromJson<GenQf1>(object["SysUQf"]);
    return v;
}


// ==========================================
// TiCorrn
template <>
QJsonValue toJson<TiCorrn>(const TiCorrn& v) {
    QJsonObject j;
    j["HrCorrn"] = toJson<int8_t>(v.HrCorrn);
    j["MinsCorrn"] = toJson<int8_t>(v.MinsCorrn);
    j["DayLiSaveTi"] = toJson<int8_t>(v.DayLiSaveTi);
    j["SpprtForFct"] = toJson<NoYes1>(v.SpprtForFct);
    return QJsonValue(j);
}
template <>
TiCorrn fromJson<TiCorrn>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TiCorrn();
    }
    QJsonObject object = j.toObject();
    TiCorrn v;
    v.HrCorrn = fromJson<int8_t>(object["HrCorrn"]);
    v.MinsCorrn = fromJson<int8_t>(object["MinsCorrn"]);
    v.DayLiSaveTi = fromJson<int8_t>(object["DayLiSaveTi"]);
    v.SpprtForFct = fromJson<NoYes1>(object["SpprtForFct"]);
    return v;
}


// ==========================================
// TiGapAdpvSeldPen1
template <>
QJsonValue toJson<TiGapAdpvSeldPen1>(const TiGapAdpvSeldPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<ManAdpv1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
TiGapAdpvSeldPen1 fromJson<TiGapAdpvSeldPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TiGapAdpvSeldPen1();
    }
    QJsonObject object = j.toObject();
    TiGapAdpvSeldPen1 v;
    v.Sts = fromJson<ManAdpv1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// TiGapPen1
template <>
QJsonValue toJson<TiGapPen1>(const TiGapPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<double>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
TiGapPen1 fromJson<TiGapPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TiGapPen1();
    }
    QJsonObject object = j.toObject();
    TiGapPen1 v;
    v.Sts = fromJson<double>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// TireCircumCalByNav
template <>
QJsonValue toJson<TireCircumCalByNav>(const TireCircumCalByNav& v) {
    QJsonObject j;
    j["TireCircum"] = toJson<uint16_t>(v.TireCircum);
    j["HiQly"] = toJson<NoYes1>(v.HiQly);
    j["SpprtForFct"] = toJson<NoYes1>(v.SpprtForFct);
    return QJsonValue(j);
}
template <>
TireCircumCalByNav fromJson<TireCircumCalByNav>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TireCircumCalByNav();
    }
    QJsonObject object = j.toObject();
    TireCircumCalByNav v;
    v.TireCircum = fromJson<uint16_t>(object["TireCircum"]);
    v.HiQly = fromJson<NoYes1>(object["HiQly"]);
    v.SpprtForFct = fromJson<NoYes1>(object["SpprtForFct"]);
    return v;
}


// ==========================================
// TirePAbsltValFrntLe3
template <>
QJsonValue toJson<TirePAbsltValFrntLe3>(const TirePAbsltValFrntLe3& v) {
    QJsonObject j;
    j["TirepabsltVal1"] = toJson<double>(v.TirepabsltVal1);
    j["TirePabsltValQF"] = toJson<TirePAbsltValQf>(v.TirePabsltValQF);
    j["TirePPosn_"] = toJson<TirePPosn>(v.TirePPosn_);
    return QJsonValue(j);
}
template <>
TirePAbsltValFrntLe3 fromJson<TirePAbsltValFrntLe3>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TirePAbsltValFrntLe3();
    }
    QJsonObject object = j.toObject();
    TirePAbsltValFrntLe3 v;
    v.TirepabsltVal1 = fromJson<double>(object["TirepabsltVal1"]);
    v.TirePabsltValQF = fromJson<TirePAbsltValQf>(object["TirePabsltValQF"]);
    v.TirePPosn_ = fromJson<TirePPosn>(object["TirePPosn_"]);
    return v;
}


// ==========================================
// TirePAbsltValFrntRi1
template <>
QJsonValue toJson<TirePAbsltValFrntRi1>(const TirePAbsltValFrntRi1& v) {
    QJsonObject j;
    j["TirePAbsltVal1"] = toJson<double>(v.TirePAbsltVal1);
    j["TirePAbsltValQF"] = toJson<TirePAbsltValQf>(v.TirePAbsltValQF);
    j["TirePPosn_"] = toJson<TirePPosn>(v.TirePPosn_);
    return QJsonValue(j);
}
template <>
TirePAbsltValFrntRi1 fromJson<TirePAbsltValFrntRi1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TirePAbsltValFrntRi1();
    }
    QJsonObject object = j.toObject();
    TirePAbsltValFrntRi1 v;
    v.TirePAbsltVal1 = fromJson<double>(object["TirePAbsltVal1"]);
    v.TirePAbsltValQF = fromJson<TirePAbsltValQf>(object["TirePAbsltValQF"]);
    v.TirePPosn_ = fromJson<TirePPosn>(object["TirePPosn_"]);
    return v;
}


// ==========================================
// TirePAbsltValReLe1
template <>
QJsonValue toJson<TirePAbsltValReLe1>(const TirePAbsltValReLe1& v) {
    QJsonObject j;
    j["TirePAbsltVal1"] = toJson<double>(v.TirePAbsltVal1);
    j["TirePAbsltValQF"] = toJson<TirePAbsltValQf>(v.TirePAbsltValQF);
    j["TirePPosn_"] = toJson<TirePPosn>(v.TirePPosn_);
    return QJsonValue(j);
}
template <>
TirePAbsltValReLe1 fromJson<TirePAbsltValReLe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TirePAbsltValReLe1();
    }
    QJsonObject object = j.toObject();
    TirePAbsltValReLe1 v;
    v.TirePAbsltVal1 = fromJson<double>(object["TirePAbsltVal1"]);
    v.TirePAbsltValQF = fromJson<TirePAbsltValQf>(object["TirePAbsltValQF"]);
    v.TirePPosn_ = fromJson<TirePPosn>(object["TirePPosn_"]);
    return v;
}


// ==========================================
// TirePAbsltValReRi1
template <>
QJsonValue toJson<TirePAbsltValReRi1>(const TirePAbsltValReRi1& v) {
    QJsonObject j;
    j["TirePAbsltVal1"] = toJson<double>(v.TirePAbsltVal1);
    j["TirePAbsltValQF"] = toJson<TirePAbsltValQf>(v.TirePAbsltValQF);
    j["TirePPosn_"] = toJson<TirePPosn>(v.TirePPosn_);
    return QJsonValue(j);
}
template <>
TirePAbsltValReRi1 fromJson<TirePAbsltValReRi1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TirePAbsltValReRi1();
    }
    QJsonObject object = j.toObject();
    TirePAbsltValReRi1 v;
    v.TirePAbsltVal1 = fromJson<double>(object["TirePAbsltVal1"]);
    v.TirePAbsltValQF = fromJson<TirePAbsltValQf>(object["TirePAbsltValQF"]);
    v.TirePPosn_ = fromJson<TirePPosn>(object["TirePPosn_"]);
    return v;
}


// ==========================================
// TirePMonData1
template <>
QJsonValue toJson<TirePMonData1>(const TirePMonData1& v) {
    QJsonObject j;
    j["TirePMonSts1_"] = toJson<TirePMonSts1>(v.TirePMonSts1_);
    j["TirePReRi"] = toJson<TirePWarn>(v.TirePReRi);
    j["TirePReLe"] = toJson<TirePWarn>(v.TirePReLe);
    j["TirePFrntLe"] = toJson<TirePWarn>(v.TirePFrntLe);
    j["TirePFrntRe"] = toJson<TirePWarn>(v.TirePFrntRe);
    return QJsonValue(j);
}
template <>
TirePMonData1 fromJson<TirePMonData1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TirePMonData1();
    }
    QJsonObject object = j.toObject();
    TirePMonData1 v;
    v.TirePMonSts1_ = fromJson<TirePMonSts1>(object["TirePMonSts1_"]);
    v.TirePReRi = fromJson<TirePWarn>(object["TirePReRi"]);
    v.TirePReLe = fromJson<TirePWarn>(object["TirePReLe"]);
    v.TirePFrntLe = fromJson<TirePWarn>(object["TirePFrntLe"]);
    v.TirePFrntRe = fromJson<TirePWarn>(object["TirePFrntRe"]);
    return v;
}


// ==========================================
// TqSafe2
template <>
QJsonValue toJson<TqSafe2>(const TqSafe2& v) {
    QJsonObject j;
    j["TqAct"] = toJson<double>(v.TqAct);
    j["TqActChks"] = toJson<uint8_t>(v.TqActChks);
    j["TqActCntr"] = toJson<uint8_t>(v.TqActCntr);
    j["TqActQlyFac"] = toJson<QualityFactor>(v.TqActQlyFac);
    j["SpdLimn"] = toJson<double>(v.SpdLimn);
    return QJsonValue(j);
}
template <>
TqSafe2 fromJson<TqSafe2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TqSafe2();
    }
    QJsonObject object = j.toObject();
    TqSafe2 v;
    v.TqAct = fromJson<double>(object["TqAct"]);
    v.TqActChks = fromJson<uint8_t>(object["TqActChks"]);
    v.TqActCntr = fromJson<uint8_t>(object["TqActCntr"]);
    v.TqActQlyFac = fromJson<QualityFactor>(object["TqActQlyFac"]);
    v.SpdLimn = fromJson<double>(object["SpdLimn"]);
    return v;
}


// ==========================================
// TwliBriRaw
template <>
QJsonValue toJson<TwliBriRaw>(const TwliBriRaw& v) {
    QJsonObject j;
    j["TwliBriRaw1"] = toJson<uint16_t>(v.TwliBriRaw1);
    j["TwliBriRawQf"] = toJson<GenQf1>(v.TwliBriRawQf);
    return QJsonValue(j);
}
template <>
TwliBriRaw fromJson<TwliBriRaw>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return TwliBriRaw();
    }
    QJsonObject object = j.toObject();
    TwliBriRaw v;
    v.TwliBriRaw1 = fromJson<uint16_t>(object["TwliBriRaw1"]);
    v.TwliBriRawQf = fromJson<GenQf1>(object["TwliBriRawQf"]);
    return v;
}


// ==========================================
// UInt64Rec
template <>
QJsonValue toJson<UInt64Rec>(const UInt64Rec& v) {
    QJsonObject j;
    j["Byte0"] = toJson<uint8_t>(v.Byte0);
    j["Byte1"] = toJson<uint8_t>(v.Byte1);
    j["Byte2"] = toJson<uint8_t>(v.Byte2);
    j["Byte3"] = toJson<uint8_t>(v.Byte3);
    j["Byte4"] = toJson<uint8_t>(v.Byte4);
    j["Byte5"] = toJson<uint8_t>(v.Byte5);
    j["Byte6"] = toJson<uint8_t>(v.Byte6);
    j["Byte7"] = toJson<uint8_t>(v.Byte7);
    return QJsonValue(j);
}
template <>
UInt64Rec fromJson<UInt64Rec>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UInt64Rec();
    }
    QJsonObject object = j.toObject();
    UInt64Rec v;
    v.Byte0 = fromJson<uint8_t>(object["Byte0"]);
    v.Byte1 = fromJson<uint8_t>(object["Byte1"]);
    v.Byte2 = fromJson<uint8_t>(object["Byte2"]);
    v.Byte3 = fromJson<uint8_t>(object["Byte3"]);
    v.Byte4 = fromJson<uint8_t>(object["Byte4"]);
    v.Byte5 = fromJson<uint8_t>(object["Byte5"]);
    v.Byte6 = fromJson<uint8_t>(object["Byte6"]);
    v.Byte7 = fromJson<uint8_t>(object["Byte7"]);
    return v;
}


// ==========================================
// UnlckKeylsCfgPen2
template <>
QJsonValue toJson<UnlckKeylsCfgPen2>(const UnlckKeylsCfgPen2& v) {
    QJsonObject j;
    j["IdPen_"] = toJson<IdPen>(v.IdPen_);
    j["KeylsCfg"] = toJson<UnlckKeylsCfg2>(v.KeylsCfg);
    return QJsonValue(j);
}
template <>
UnlckKeylsCfgPen2 fromJson<UnlckKeylsCfgPen2>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UnlckKeylsCfgPen2();
    }
    QJsonObject object = j.toObject();
    UnlckKeylsCfgPen2 v;
    v.IdPen_ = fromJson<IdPen>(object["IdPen_"]);
    v.KeylsCfg = fromJson<UnlckKeylsCfg2>(object["KeylsCfg"]);
    return v;
}


// ==========================================
// UnlckRemCfgPen1
template <>
QJsonValue toJson<UnlckRemCfgPen1>(const UnlckRemCfgPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<UnlckRemCfg1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
UnlckRemCfgPen1 fromJson<UnlckRemCfgPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UnlckRemCfgPen1();
    }
    QJsonObject object = j.toObject();
    UnlckRemCfgPen1 v;
    v.Sts = fromJson<UnlckRemCfg1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// UsrSetSpdForKeySpdLimn
template <>
QJsonValue toJson<UsrSetSpdForKeySpdLimn>(const UsrSetSpdForKeySpdLimn& v) {
    QJsonObject j;
    j["Sts"] = toJson<double>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
UsrSetSpdForKeySpdLimn fromJson<UsrSetSpdForKeySpdLimn>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UsrSetSpdForKeySpdLimn();
    }
    QJsonObject object = j.toObject();
    UsrSetSpdForKeySpdLimn v;
    v.Sts = fromJson<double>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// UsrSetSpdForKeySpdWarn
template <>
QJsonValue toJson<UsrSetSpdForKeySpdWarn>(const UsrSetSpdForKeySpdWarn& v) {
    QJsonObject j;
    j["UsrSetSpdForKeySpdWarn1"] = toJson<double>(v.UsrSetSpdForKeySpdWarn1);
    j["UsrSetSpdForKeySpdWarn2"] = toJson<double>(v.UsrSetSpdForKeySpdWarn2);
    j["UsrSetSpdForKeySpdWarn3"] = toJson<double>(v.UsrSetSpdForKeySpdWarn3);
    j["UsrSetSpdForKeySpdWarn4"] = toJson<double>(v.UsrSetSpdForKeySpdWarn4);
    j["UsrSetSpdForKeySpdWarn5"] = toJson<double>(v.UsrSetSpdForKeySpdWarn5);
    j["UsrSetSpdForKeySpdWarn6"] = toJson<double>(v.UsrSetSpdForKeySpdWarn6);
    j["UsrSetSpdForKeySpdWarnPen"] = toJson<IdPen>(v.UsrSetSpdForKeySpdWarnPen);
    return QJsonValue(j);
}
template <>
UsrSetSpdForKeySpdWarn fromJson<UsrSetSpdForKeySpdWarn>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UsrSetSpdForKeySpdWarn();
    }
    QJsonObject object = j.toObject();
    UsrSetSpdForKeySpdWarn v;
    v.UsrSetSpdForKeySpdWarn1 = fromJson<double>(object["UsrSetSpdForKeySpdWarn1"]);
    v.UsrSetSpdForKeySpdWarn2 = fromJson<double>(object["UsrSetSpdForKeySpdWarn2"]);
    v.UsrSetSpdForKeySpdWarn3 = fromJson<double>(object["UsrSetSpdForKeySpdWarn3"]);
    v.UsrSetSpdForKeySpdWarn4 = fromJson<double>(object["UsrSetSpdForKeySpdWarn4"]);
    v.UsrSetSpdForKeySpdWarn5 = fromJson<double>(object["UsrSetSpdForKeySpdWarn5"]);
    v.UsrSetSpdForKeySpdWarn6 = fromJson<double>(object["UsrSetSpdForKeySpdWarn6"]);
    v.UsrSetSpdForKeySpdWarnPen = fromJson<IdPen>(object["UsrSetSpdForKeySpdWarnPen"]);
    return v;
}


// ==========================================
// UsrSetVolMaxForKeyVolLimn
template <>
QJsonValue toJson<UsrSetVolMaxForKeyVolLimn>(const UsrSetVolMaxForKeyVolLimn& v) {
    QJsonObject j;
    j["Pen"] = toJson<IdPen>(v.Pen);
    j["SetVol_"] = toJson<SetVol>(v.SetVol_);
    return QJsonValue(j);
}
template <>
UsrSetVolMaxForKeyVolLimn fromJson<UsrSetVolMaxForKeyVolLimn>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UsrSetVolMaxForKeyVolLimn();
    }
    QJsonObject object = j.toObject();
    UsrSetVolMaxForKeyVolLimn v;
    v.Pen = fromJson<IdPen>(object["Pen"]);
    v.SetVol_ = fromJson<SetVol>(object["SetVol_"]);
    return v;
}


// ==========================================
// UsrSwtDispClimaReqForRowSec
template <>
QJsonValue toJson<UsrSwtDispClimaReqForRowSec>(const UsrSwtDispClimaReqForRowSec& v) {
    QJsonObject j;
    j["UsrSwtDispClimaReqForTSpForRowSecLe"] = toJson<double>(v.UsrSwtDispClimaReqForTSpForRowSecLe);
    j["UsrSwtDispUpdClimaReqForTSpForRowSecLe"] = toJson<bool>(v.UsrSwtDispUpdClimaReqForTSpForRowSecLe);
    j["UsrSwtDispClimaReqForTSpForRowSecRi"] = toJson<double>(v.UsrSwtDispClimaReqForTSpForRowSecRi);
    j["UsrSwtDispUpdClimaReqForTSpForRowSecRi"] = toJson<bool>(v.UsrSwtDispUpdClimaReqForTSpForRowSecRi);
    j["UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe"] = toJson<SeatClimaLvl>(v.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe);
    j["UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe"] = toJson<bool>(v.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe);
    j["UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi"] = toJson<bool>(v.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi);
    j["UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi"] = toJson<SeatClimaLvl>(v.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi);
    j["UsrSwtDispReqForFanLvlForRowSec"] = toJson<HmiHvacFanLvl>(v.UsrSwtDispReqForFanLvlForRowSec);
    j["UsrSwtDispUpdReqForFanLvlForRowSec"] = toJson<bool>(v.UsrSwtDispUpdReqForFanLvlForRowSec);
    j["UsrSwtDispClimaReqForTSpSpclForRowSecLe"] = toJson<HmiCmptmtTSpSpcl>(v.UsrSwtDispClimaReqForTSpSpclForRowSecLe);
    j["UsrSwtDispClimaReqForTSpSpclForRowSecRi"] = toJson<HmiCmptmtTSpSpcl>(v.UsrSwtDispClimaReqForTSpSpclForRowSecRi);
    return QJsonValue(j);
}
template <>
UsrSwtDispClimaReqForRowSec fromJson<UsrSwtDispClimaReqForRowSec>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UsrSwtDispClimaReqForRowSec();
    }
    QJsonObject object = j.toObject();
    UsrSwtDispClimaReqForRowSec v;
    v.UsrSwtDispClimaReqForTSpForRowSecLe = fromJson<double>(object["UsrSwtDispClimaReqForTSpForRowSecLe"]);
    v.UsrSwtDispUpdClimaReqForTSpForRowSecLe = fromJson<bool>(object["UsrSwtDispUpdClimaReqForTSpForRowSecLe"]);
    v.UsrSwtDispClimaReqForTSpForRowSecRi = fromJson<double>(object["UsrSwtDispClimaReqForTSpForRowSecRi"]);
    v.UsrSwtDispUpdClimaReqForTSpForRowSecRi = fromJson<bool>(object["UsrSwtDispUpdClimaReqForTSpForRowSecRi"]);
    v.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe = fromJson<SeatClimaLvl>(object["UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe"]);
    v.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe = fromJson<bool>(object["UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe"]);
    v.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi = fromJson<bool>(object["UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi"]);
    v.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi = fromJson<SeatClimaLvl>(object["UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi"]);
    v.UsrSwtDispReqForFanLvlForRowSec = fromJson<HmiHvacFanLvl>(object["UsrSwtDispReqForFanLvlForRowSec"]);
    v.UsrSwtDispUpdReqForFanLvlForRowSec = fromJson<bool>(object["UsrSwtDispUpdReqForFanLvlForRowSec"]);
    v.UsrSwtDispClimaReqForTSpSpclForRowSecLe = fromJson<HmiCmptmtTSpSpcl>(object["UsrSwtDispClimaReqForTSpSpclForRowSecLe"]);
    v.UsrSwtDispClimaReqForTSpSpclForRowSecRi = fromJson<HmiCmptmtTSpSpcl>(object["UsrSwtDispClimaReqForTSpSpclForRowSecRi"]);
    return v;
}


// ==========================================
// UsrSwtDispClimaSts
template <>
QJsonValue toJson<UsrSwtDispClimaSts>(const UsrSwtDispClimaSts& v) {
    QJsonObject j;
    j["UsrSwtDispClimaTSpForRowSecLe"] = toJson<double>(v.UsrSwtDispClimaTSpForRowSecLe);
    j["UsrSwtDispClimaTSpForRowSecRi"] = toJson<double>(v.UsrSwtDispClimaTSpForRowSecRi);
    j["UsrSwtDispSeatHeatLvlForRowSecLe"] = toJson<SeatClimaLvl>(v.UsrSwtDispSeatHeatLvlForRowSecLe);
    j["UsrSwtDispSeatHeatLvlForRowSecRi"] = toJson<SeatClimaLvl>(v.UsrSwtDispSeatHeatLvlForRowSecRi);
    j["UsrSwtDispFanLvlForRowSec"] = toJson<HmiHvacFanLvl>(v.UsrSwtDispFanLvlForRowSec);
    j["UsrSwtDispClimaTSpSpclForRowSecLe"] = toJson<HmiCmptmtTSpSpcl>(v.UsrSwtDispClimaTSpSpclForRowSecLe);
    j["UsrSwtDispClimaTSpSpclForRowSecRi"] = toJson<HmiCmptmtTSpSpcl>(v.UsrSwtDispClimaTSpSpclForRowSecRi);
    return QJsonValue(j);
}
template <>
UsrSwtDispClimaSts fromJson<UsrSwtDispClimaSts>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UsrSwtDispClimaSts();
    }
    QJsonObject object = j.toObject();
    UsrSwtDispClimaSts v;
    v.UsrSwtDispClimaTSpForRowSecLe = fromJson<double>(object["UsrSwtDispClimaTSpForRowSecLe"]);
    v.UsrSwtDispClimaTSpForRowSecRi = fromJson<double>(object["UsrSwtDispClimaTSpForRowSecRi"]);
    v.UsrSwtDispSeatHeatLvlForRowSecLe = fromJson<SeatClimaLvl>(object["UsrSwtDispSeatHeatLvlForRowSecLe"]);
    v.UsrSwtDispSeatHeatLvlForRowSecRi = fromJson<SeatClimaLvl>(object["UsrSwtDispSeatHeatLvlForRowSecRi"]);
    v.UsrSwtDispFanLvlForRowSec = fromJson<HmiHvacFanLvl>(object["UsrSwtDispFanLvlForRowSec"]);
    v.UsrSwtDispClimaTSpSpclForRowSecLe = fromJson<HmiCmptmtTSpSpcl>(object["UsrSwtDispClimaTSpSpclForRowSecLe"]);
    v.UsrSwtDispClimaTSpSpclForRowSecRi = fromJson<HmiCmptmtTSpSpcl>(object["UsrSwtDispClimaTSpSpclForRowSecRi"]);
    return v;
}


// ==========================================
// UsrSwtDispForSecRowSeatVentn
template <>
QJsonValue toJson<UsrSwtDispForSecRowSeatVentn>(const UsrSwtDispForSecRowSeatVentn& v) {
    QJsonObject j;
    j["UsrSwtDispForSecRowSeatVentnLe"] = toJson<SeatClimaLvl>(v.UsrSwtDispForSecRowSeatVentnLe);
    j["UsrSwtDispForSecRowSeatVentnRi"] = toJson<SeatClimaLvl>(v.UsrSwtDispForSecRowSeatVentnRi);
    return QJsonValue(j);
}
template <>
UsrSwtDispForSecRowSeatVentn fromJson<UsrSwtDispForSecRowSeatVentn>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UsrSwtDispForSecRowSeatVentn();
    }
    QJsonObject object = j.toObject();
    UsrSwtDispForSecRowSeatVentn v;
    v.UsrSwtDispForSecRowSeatVentnLe = fromJson<SeatClimaLvl>(object["UsrSwtDispForSecRowSeatVentnLe"]);
    v.UsrSwtDispForSecRowSeatVentnRi = fromJson<SeatClimaLvl>(object["UsrSwtDispForSecRowSeatVentnRi"]);
    return v;
}


// ==========================================
// UsrSwtDispReqForSecRowSeatVentn
template <>
QJsonValue toJson<UsrSwtDispReqForSecRowSeatVentn>(const UsrSwtDispReqForSecRowSeatVentn& v) {
    QJsonObject j;
    j["UsrSwtDispReqForSecRowSeatVentnLe"] = toJson<SeatClimaLvl>(v.UsrSwtDispReqForSecRowSeatVentnLe);
    j["UsrSwtDispReqForSecRowSeatVentnRi"] = toJson<SeatClimaLvl>(v.UsrSwtDispReqForSecRowSeatVentnRi);
    j["usrSwtDispUpdReqForSecRowSeatVentnLe"] = toJson<bool>(v.usrSwtDispUpdReqForSecRowSeatVentnLe);
    j["usrSwtDispUpdReqForSecRowSeatVentnRi"] = toJson<bool>(v.usrSwtDispUpdReqForSecRowSeatVentnRi);
    return QJsonValue(j);
}
template <>
UsrSwtDispReqForSecRowSeatVentn fromJson<UsrSwtDispReqForSecRowSeatVentn>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UsrSwtDispReqForSecRowSeatVentn();
    }
    QJsonObject object = j.toObject();
    UsrSwtDispReqForSecRowSeatVentn v;
    v.UsrSwtDispReqForSecRowSeatVentnLe = fromJson<SeatClimaLvl>(object["UsrSwtDispReqForSecRowSeatVentnLe"]);
    v.UsrSwtDispReqForSecRowSeatVentnRi = fromJson<SeatClimaLvl>(object["UsrSwtDispReqForSecRowSeatVentnRi"]);
    v.usrSwtDispUpdReqForSecRowSeatVentnLe = fromJson<bool>(object["usrSwtDispUpdReqForSecRowSeatVentnLe"]);
    v.usrSwtDispUpdReqForSecRowSeatVentnRi = fromJson<bool>(object["usrSwtDispUpdReqForSecRowSeatVentnRi"]);
    return v;
}


// ==========================================
// UsrSwtDispReqVrnt
template <>
QJsonValue toJson<UsrSwtDispReqVrnt>(const UsrSwtDispReqVrnt& v) {
    QJsonObject j;
    j["UsrSwtDispSeatHeatFct"] = toJson<OnOff1>(v.UsrSwtDispSeatHeatFct);
    j["UsrSwtDispTUnit_"] = toJson<UsrSwtDispTUnit>(v.UsrSwtDispTUnit_);
    return QJsonValue(j);
}
template <>
UsrSwtDispReqVrnt fromJson<UsrSwtDispReqVrnt>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return UsrSwtDispReqVrnt();
    }
    QJsonObject object = j.toObject();
    UsrSwtDispReqVrnt v;
    v.UsrSwtDispSeatHeatFct = fromJson<OnOff1>(object["UsrSwtDispSeatHeatFct"]);
    v.UsrSwtDispTUnit_ = fromJson<UsrSwtDispTUnit>(object["UsrSwtDispTUnit_"]);
    return v;
}


// ==========================================
// VFCGrp
template <>
QJsonValue toJson<VFCGrp>(const VFCGrp& v) {
    QJsonObject j;
    j["Grp1"] = toJson<uint16_t>(v.Grp1);
    j["Grp2"] = toJson<uint16_t>(v.Grp2);
    j["Grp3"] = toJson<uint16_t>(v.Grp3);
    return QJsonValue(j);
}
template <>
VFCGrp fromJson<VFCGrp>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return VFCGrp();
    }
    QJsonObject object = j.toObject();
    VFCGrp v;
    v.Grp1 = fromJson<uint16_t>(object["Grp1"]);
    v.Grp2 = fromJson<uint16_t>(object["Grp2"]);
    v.Grp3 = fromJson<uint16_t>(object["Grp3"]);
    return v;
}


// ==========================================
// VehCfgPrm
template <>
QJsonValue toJson<VehCfgPrm>(const VehCfgPrm& v) {
    QJsonObject j;
    j["BlkIDBytePosn1"] = toJson<uint8_t>(v.BlkIDBytePosn1);
    j["CCPBytePosn2"] = toJson<uint8_t>(v.CCPBytePosn2);
    j["CCPBytePosn3"] = toJson<uint8_t>(v.CCPBytePosn3);
    j["CCPBytePosn4"] = toJson<uint8_t>(v.CCPBytePosn4);
    j["CCPBytePosn5"] = toJson<uint8_t>(v.CCPBytePosn5);
    j["CCPBytePosn6"] = toJson<uint8_t>(v.CCPBytePosn6);
    j["CCPBytePosn7"] = toJson<uint8_t>(v.CCPBytePosn7);
    j["CCPBytePosn8"] = toJson<uint8_t>(v.CCPBytePosn8);
    return QJsonValue(j);
}
template <>
VehCfgPrm fromJson<VehCfgPrm>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return VehCfgPrm();
    }
    QJsonObject object = j.toObject();
    VehCfgPrm v;
    v.BlkIDBytePosn1 = fromJson<uint8_t>(object["BlkIDBytePosn1"]);
    v.CCPBytePosn2 = fromJson<uint8_t>(object["CCPBytePosn2"]);
    v.CCPBytePosn3 = fromJson<uint8_t>(object["CCPBytePosn3"]);
    v.CCPBytePosn4 = fromJson<uint8_t>(object["CCPBytePosn4"]);
    v.CCPBytePosn5 = fromJson<uint8_t>(object["CCPBytePosn5"]);
    v.CCPBytePosn6 = fromJson<uint8_t>(object["CCPBytePosn6"]);
    v.CCPBytePosn7 = fromJson<uint8_t>(object["CCPBytePosn7"]);
    v.CCPBytePosn8 = fromJson<uint8_t>(object["CCPBytePosn8"]);
    return v;
}


// ==========================================
// VehMNomRec1
template <>
QJsonValue toJson<VehMNomRec1>(const VehMNomRec1& v) {
    QJsonObject j;
    j["VehM"] = toJson<uint16_t>(v.VehM);
    j["VehMQly"] = toJson<Qly2>(v.VehMQly);
    j["TrlrM_"] = toJson<TrlrM>(v.TrlrM_);
    return QJsonValue(j);
}
template <>
VehMNomRec1 fromJson<VehMNomRec1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return VehMNomRec1();
    }
    QJsonObject object = j.toObject();
    VehMNomRec1 v;
    v.VehM = fromJson<uint16_t>(object["VehM"]);
    v.VehMQly = fromJson<Qly2>(object["VehMQly"]);
    v.TrlrM_ = fromJson<TrlrM>(object["TrlrM_"]);
    return v;
}


// ==========================================
// VehModMngtGlbSafe1
template <>
QJsonValue toJson<VehModMngtGlbSafe1>(const VehModMngtGlbSafe1& v) {
    QJsonObject j;
    j["UsgModSts"] = toJson<UsgModSts1>(v.UsgModSts);
    j["CarModSts1_"] = toJson<CarModSts1>(v.CarModSts1_);
    j["CarModSubtypWdCarModSubtyp"] = toJson<uint8_t>(v.CarModSubtypWdCarModSubtyp);
    j["EgyLvlElecMai"] = toJson<uint8_t>(v.EgyLvlElecMai);
    j["EgyLvlElecSubtyp"] = toJson<uint8_t>(v.EgyLvlElecSubtyp);
    j["PwrLvlElecMai"] = toJson<uint8_t>(v.PwrLvlElecMai);
    j["PwrLvlElecSubtyp"] = toJson<uint8_t>(v.PwrLvlElecSubtyp);
    j["FltEgyCnsWdSts"] = toJson<FltEgyCns1>(v.FltEgyCnsWdSts);
    j["Chks"] = toJson<uint8_t>(v.Chks);
    j["Cntr"] = toJson<uint8_t>(v.Cntr);
    return QJsonValue(j);
}
template <>
VehModMngtGlbSafe1 fromJson<VehModMngtGlbSafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return VehModMngtGlbSafe1();
    }
    QJsonObject object = j.toObject();
    VehModMngtGlbSafe1 v;
    v.UsgModSts = fromJson<UsgModSts1>(object["UsgModSts"]);
    v.CarModSts1_ = fromJson<CarModSts1>(object["CarModSts1_"]);
    v.CarModSubtypWdCarModSubtyp = fromJson<uint8_t>(object["CarModSubtypWdCarModSubtyp"]);
    v.EgyLvlElecMai = fromJson<uint8_t>(object["EgyLvlElecMai"]);
    v.EgyLvlElecSubtyp = fromJson<uint8_t>(object["EgyLvlElecSubtyp"]);
    v.PwrLvlElecMai = fromJson<uint8_t>(object["PwrLvlElecMai"]);
    v.PwrLvlElecSubtyp = fromJson<uint8_t>(object["PwrLvlElecSubtyp"]);
    v.FltEgyCnsWdSts = fromJson<FltEgyCns1>(object["FltEgyCnsWdSts"]);
    v.Chks = fromJson<uint8_t>(object["Chks"]);
    v.Cntr = fromJson<uint8_t>(object["Cntr"]);
    return v;
}


// ==========================================
// VehMtnStSafe1
template <>
QJsonValue toJson<VehMtnStSafe1>(const VehMtnStSafe1& v) {
    QJsonObject j;
    j["VehMtnSt"] = toJson<VehMtnSt2>(v.VehMtnSt);
    j["VehMtnStChks"] = toJson<uint8_t>(v.VehMtnStChks);
    j["VehMtnStCntr"] = toJson<uint8_t>(v.VehMtnStCntr);
    return QJsonValue(j);
}
template <>
VehMtnStSafe1 fromJson<VehMtnStSafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return VehMtnStSafe1();
    }
    QJsonObject object = j.toObject();
    VehMtnStSafe1 v;
    v.VehMtnSt = fromJson<VehMtnSt2>(object["VehMtnSt"]);
    v.VehMtnStChks = fromJson<uint8_t>(object["VehMtnStChks"]);
    v.VehMtnStCntr = fromJson<uint8_t>(object["VehMtnStCntr"]);
    return v;
}


// ==========================================
// VehSpdIndcd1
template <>
QJsonValue toJson<VehSpdIndcd1>(const VehSpdIndcd1& v) {
    QJsonObject j;
    j["VehSpdIndcd"] = toJson<uint16_t>(v.VehSpdIndcd);
    j["VeSpdIndcdUnit"] = toJson<VehSpdIndcdUnit>(v.VeSpdIndcdUnit);
    return QJsonValue(j);
}
template <>
VehSpdIndcd1 fromJson<VehSpdIndcd1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return VehSpdIndcd1();
    }
    QJsonObject object = j.toObject();
    VehSpdIndcd1 v;
    v.VehSpdIndcd = fromJson<uint16_t>(object["VehSpdIndcd"]);
    v.VeSpdIndcdUnit = fromJson<VehSpdIndcdUnit>(object["VeSpdIndcdUnit"]);
    return v;
}


// ==========================================
// VehSpdLgtSafe1
template <>
QJsonValue toJson<VehSpdLgtSafe1>(const VehSpdLgtSafe1& v) {
    QJsonObject j;
    j["VehSpdLgt"] = toJson<double>(v.VehSpdLgt);
    j["VehSpdLgtQf"] = toJson<GenQf1>(v.VehSpdLgtQf);
    j["VehSpdLgtCntr"] = toJson<uint8_t>(v.VehSpdLgtCntr);
    j["VehSpdLgtChks"] = toJson<uint8_t>(v.VehSpdLgtChks);
    return QJsonValue(j);
}
template <>
VehSpdLgtSafe1 fromJson<VehSpdLgtSafe1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return VehSpdLgtSafe1();
    }
    QJsonObject object = j.toObject();
    VehSpdLgtSafe1 v;
    v.VehSpdLgt = fromJson<double>(object["VehSpdLgt"]);
    v.VehSpdLgtQf = fromJson<GenQf1>(object["VehSpdLgtQf"]);
    v.VehSpdLgtCntr = fromJson<uint8_t>(object["VehSpdLgtCntr"]);
    v.VehSpdLgtChks = fromJson<uint8_t>(object["VehSpdLgtChks"]);
    return v;
}


// ==========================================
// Vin1
template <>
QJsonValue toJson<Vin1>(const Vin1& v) {
    QJsonObject j;
    j["BlockNr"] = toJson<uint8_t>(v.BlockNr);
    j["VINSignalPos1"] = toJson<uint8_t>(v.VINSignalPos1);
    j["VINSignalPos2"] = toJson<uint8_t>(v.VINSignalPos2);
    j["VINSignalPos3"] = toJson<uint8_t>(v.VINSignalPos3);
    j["VINSignalPos4"] = toJson<uint8_t>(v.VINSignalPos4);
    j["VINSignalPos5"] = toJson<uint8_t>(v.VINSignalPos5);
    j["VINSignalPos6"] = toJson<uint8_t>(v.VINSignalPos6);
    j["VINSignalPos7"] = toJson<uint8_t>(v.VINSignalPos7);
    return QJsonValue(j);
}
template <>
Vin1 fromJson<Vin1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return Vin1();
    }
    QJsonObject object = j.toObject();
    Vin1 v;
    v.BlockNr = fromJson<uint8_t>(object["BlockNr"]);
    v.VINSignalPos1 = fromJson<uint8_t>(object["VINSignalPos1"]);
    v.VINSignalPos2 = fromJson<uint8_t>(object["VINSignalPos2"]);
    v.VINSignalPos3 = fromJson<uint8_t>(object["VINSignalPos3"]);
    v.VINSignalPos4 = fromJson<uint8_t>(object["VINSignalPos4"]);
    v.VINSignalPos5 = fromJson<uint8_t>(object["VINSignalPos5"]);
    v.VINSignalPos6 = fromJson<uint8_t>(object["VINSignalPos6"]);
    v.VINSignalPos7 = fromJson<uint8_t>(object["VINSignalPos7"]);
    return v;
}


// ==========================================
// VisnAgWideCfg1
template <>
QJsonValue toJson<VisnAgWideCfg1>(const VisnAgWideCfg1& v) {
    QJsonObject j;
    j["SteerPahRe"] = toJson<OnOff1>(v.SteerPahRe);
    j["SteerPahFrnt"] = toJson<OnOff1>(v.SteerPahFrnt);
    j["SteerPahSide"] = toJson<OnOff1>(v.SteerPahSide);
    j["DstInfoRe"] = toJson<OnOff1>(v.DstInfoRe);
    j["DstInfoFrnt"] = toJson<OnOff1>(v.DstInfoFrnt);
    j["DstInfoSide"] = toJson<OnOff1>(v.DstInfoSide);
    j["CtraInfo"] = toJson<OnOff1>(v.CtraInfo);
    j["TwbrPah"] = toJson<OnOff1>(v.TwbrPah);
    j["ObjDetn"] = toJson<OnOff1>(v.ObjDetn);
    return QJsonValue(j);
}
template <>
VisnAgWideCfg1 fromJson<VisnAgWideCfg1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return VisnAgWideCfg1();
    }
    QJsonObject object = j.toObject();
    VisnAgWideCfg1 v;
    v.SteerPahRe = fromJson<OnOff1>(object["SteerPahRe"]);
    v.SteerPahFrnt = fromJson<OnOff1>(object["SteerPahFrnt"]);
    v.SteerPahSide = fromJson<OnOff1>(object["SteerPahSide"]);
    v.DstInfoRe = fromJson<OnOff1>(object["DstInfoRe"]);
    v.DstInfoFrnt = fromJson<OnOff1>(object["DstInfoFrnt"]);
    v.DstInfoSide = fromJson<OnOff1>(object["DstInfoSide"]);
    v.CtraInfo = fromJson<OnOff1>(object["CtraInfo"]);
    v.TwbrPah = fromJson<OnOff1>(object["TwbrPah"]);
    v.ObjDetn = fromJson<OnOff1>(object["ObjDetn"]);
    return v;
}


// ==========================================
// WarnAndIntvPen1
template <>
QJsonValue toJson<WarnAndIntvPen1>(const WarnAndIntvPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<WarnAndIntv1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
WarnAndIntvPen1 fromJson<WarnAndIntvPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return WarnAndIntvPen1();
    }
    QJsonObject object = j.toObject();
    WarnAndIntvPen1 v;
    v.Sts = fromJson<WarnAndIntv1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// WarnTypForLaneChgWarnPen1
template <>
QJsonValue toJson<WarnTypForLaneChgWarnPen1>(const WarnTypForLaneChgWarnPen1& v) {
    QJsonObject j;
    j["Sts"] = toJson<SoundHptc1>(v.Sts);
    j["Pen"] = toJson<IdPen>(v.Pen);
    return QJsonValue(j);
}
template <>
WarnTypForLaneChgWarnPen1 fromJson<WarnTypForLaneChgWarnPen1>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return WarnTypForLaneChgWarnPen1();
    }
    QJsonObject object = j.toObject();
    WarnTypForLaneChgWarnPen1 v;
    v.Sts = fromJson<SoundHptc1>(object["Sts"]);
    v.Pen = fromJson<IdPen>(object["Pen"]);
    return v;
}


// ==========================================
// WhlRotToothCntrRec
template <>
QJsonValue toJson<WhlRotToothCntrRec>(const WhlRotToothCntrRec& v) {
    QJsonObject j;
    j["WhlRotToothCntrFrntLe"] = toJson<uint8_t>(v.WhlRotToothCntrFrntLe);
    j["WhlRotToothCntrFrntRi"] = toJson<uint8_t>(v.WhlRotToothCntrFrntRi);
    j["WhlRotToothCntrReLe"] = toJson<uint8_t>(v.WhlRotToothCntrReLe);
    j["WhlRotToothCntrReRi"] = toJson<uint8_t>(v.WhlRotToothCntrReRi);
    return QJsonValue(j);
}
template <>
WhlRotToothCntrRec fromJson<WhlRotToothCntrRec>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return WhlRotToothCntrRec();
    }
    QJsonObject object = j.toObject();
    WhlRotToothCntrRec v;
    v.WhlRotToothCntrFrntLe = fromJson<uint8_t>(object["WhlRotToothCntrFrntLe"]);
    v.WhlRotToothCntrFrntRi = fromJson<uint8_t>(object["WhlRotToothCntrFrntRi"]);
    v.WhlRotToothCntrReLe = fromJson<uint8_t>(object["WhlRotToothCntrReLe"]);
    v.WhlRotToothCntrReRi = fromJson<uint8_t>(object["WhlRotToothCntrReRi"]);
    return v;
}


// ==========================================
// WipgInfo
template <>
QJsonValue toJson<WipgInfo>(const WipgInfo& v) {
    QJsonObject j;
    j["WipgSpdInfo_"] = toJson<WipgSpdInfo>(v.WipgSpdInfo_);
    j["WiprActv"] = toJson<OnOff1>(v.WiprActv);
    j["WiprInWipgAr"] = toJson<OnOff1>(v.WiprInWipgAr);
    return QJsonValue(j);
}
template <>
WipgInfo fromJson<WipgInfo>(const QJsonValue& j) {
    if (!j.isObject()) {
        // TODO exception
        return WipgInfo();
    }
    QJsonObject object = j.toObject();
    WipgInfo v;
    v.WipgSpdInfo_ = fromJson<WipgSpdInfo>(object["WipgSpdInfo_"]);
    v.WiprActv = fromJson<OnOff1>(object["WiprActv"]);
    v.WiprInWipgAr = fromJson<OnOff1>(object["WiprInWipgAr"]);
    return v;
}


} // end of namespace
