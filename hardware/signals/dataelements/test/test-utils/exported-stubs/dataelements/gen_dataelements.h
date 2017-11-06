// C++ code generator for AUTOSAR v1.0
// Generated at: 2016-03-10T08:23:29.440172
// Source: SPA1610_IHUVOLVO8_151214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml
#ifndef _DATAELEMENTS_H
#define _DATAELEMENTS_H

#include "gen_datatypes.h"

namespace autosar {

enum class Dir { IN, OUT };

struct DataElemInfo {
    virtual const char* name() const = 0;
    virtual const char* typeName() const = 0;
    virtual Dir direction() const = 0;
};

// Provide compile time info if a data-element is in or out
class InTag {};

class OutTag {};

struct ADataRawSafe_info : InTag, public DataElemInfo {
    using data_elem_type = ADataRawSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AccAdprSpdLimActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AccAdprTurnSpdActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AccSts_info : InTag, public DataElemInfo {
    using data_elem_type = AccSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvNoiseCtrlReq_info : OutTag, public DataElemInfo {
    using data_elem_type = NormSptPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvOfHorn_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvnOfCrsEco_info : OutTag, public DataElemInfo {
    using data_elem_type = ActvnOfCrsEco;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvnOfPrkgAssi_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvnOfPrkgAut_info : OutTag, public DataElemInfo {
    using data_elem_type = ActvnOfPrkgAut;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvnOfSwtIllmnCen_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvnOfSwtIllmnClima_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvnOfSwtIllmnDrvMod_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ActvnOfSwtIllmnForSeatHeatrRe_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AdjSpdLimnSts_info : InTag, public DataElemInfo {
    using data_elem_type = AdjSpdLimnSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AdjSpdLimnWarn_info : InTag, public DataElemInfo {
    using data_elem_type = AdjSpdLimnWarnCoding;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AdprTurnSpdMod_info : OutTag, public DataElemInfo {
    using data_elem_type = AdprTurnSpdModPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AgDataRawSafe_info : InTag, public DataElemInfo {
    using data_elem_type = AgDataRawSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtReq1WdReq5_info : OutTag, public DataElemInfo {
    using data_elem_type = SlowNormFastPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AirClngReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffNoReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AmbTIndcdWithUnit_info : InTag, public DataElemInfo {
    using data_elem_type = AmbTIndcdWithUnit;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AmbTRaw_info : InTag, public DataElemInfo {
    using data_elem_type = AmbTWithQly;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AntithftRednReq_info : OutTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->7

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AsySteerApplyRqrd_info : InTag, public DataElemInfo {
    using data_elem_type = LvlWarn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AudMsgReq_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->255

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AudWarnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = NoYesCrit1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BackCntrForMissCom_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->255

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BlisSwOnOff_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltLockStAtDrvr_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStFrnt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltLockStAtPass_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStFrnt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltLockStAtRowSecLe_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltLockStAtRowSecMid_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltLockStAtRowSecRi_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltLockStAtRowThrdLe_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltLockStAtRowThrdRi_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltRmnSound1_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltRmnSound2_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltRmnSound3_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BltRmnSound4_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BrkAndAbsWarnIndcnReq_info : InTag, public DataElemInfo {
    using data_elem_type = BrkAndAbsWarnIndcnReqRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BrkFldLvl_info : InTag, public DataElemInfo {
    using data_elem_type = FldLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BrkFricTqAtWhlAct_info : InTag, public DataElemInfo {
    using data_elem_type = BrkFricTqAtWhlActRec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BrkRelsWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = NoYes1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct Btn1ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct Btn2ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct Btn3ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct Btn4ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct Btn5ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BtnAudFbSts_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BtnAudVolSts_info : InTag, public DataElemInfo {
    using data_elem_type = BtnAudVolSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BtnIllmnForWinDefrstFrnt_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BtnMmedLeRiSts_info : InTag, public DataElemInfo {
    using data_elem_type = BtnMmedLeRiSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BtnMmedModSts_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BtnMmedSetSts_info : InTag, public DataElemInfo {
    using data_elem_type = BtnMmedSetSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CCSMBtn6_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CallSts_info : OutTag, public DataElemInfo {
    using data_elem_type = CallSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CarTiGlb_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->4.294967294E8

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CchForFuEco_info : InTag, public DataElemInfo {
    using data_elem_type = CchForFuEco1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ChdLockgProtnStsToHmi_info : InTag, public DataElemInfo {
    using data_elem_type = OnOffSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ChdWinProtnStsAtDrvrRe_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ChdWinProtnStsAtPassRe_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ChrgrHwAprvdWirelsAdr_info : InTag, public DataElemInfo {
    using data_elem_type = HwAprvdWirelsAdr1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClimaActv_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClimaPwrCns_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->2047

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClimaRqrdFromHmi_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffNoReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClimaTmr_info : OutTag, public DataElemInfo {
    using data_elem_type = double;  // 0->4.294967294E8

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClimaTmrSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClimaTmrStsRqrd_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClimaWarn_info : InTag, public DataElemInfo {
    using data_elem_type = ClimaWarn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CllsnAidSnvtySeld_info : OutTag, public DataElemInfo {
    using data_elem_type = SnvtyPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CllsnFwdWarnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CllsnFwdWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CllsnWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClngActv_info : InTag, public DataElemInfo {
    using data_elem_type = OnOffNoReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClngRqrdFromHmi_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffNoReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ClsAutEna_info : OutTag, public DataElemInfo {
    using data_elem_type = EnableDisableCoding;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CmptmtAirTEstimdExtd_info : InTag, public DataElemInfo {
    using data_elem_type = CmptmtAirTEstimdExtd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CmptmtTFrnt_info : InTag, public DataElemInfo {
    using data_elem_type = CmptmtTFrnt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CnclWarnForCrsCtrl_info : InTag, public DataElemInfo {
    using data_elem_type = NoYesCrit1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CnclWarnLgtForAutDrv_info : InTag, public DataElemInfo {
    using data_elem_type = NoYesCrit1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ComLostExtrSound_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ConSftyWarn_info : InTag, public DataElemInfo {
    using data_elem_type = ConSftyWarn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CoolgReqForDispCen_info : OutTag, public DataElemInfo {
    using data_elem_type = CoolgReqForDisp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CoolgStsForDisp_info : InTag, public DataElemInfo {
    using data_elem_type = CoolgStsForDisp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CrsCtrlrSts_info : InTag, public DataElemInfo {
    using data_elem_type = CrsCtrlrSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CrvtSpdAdpvSts_info : InTag, public DataElemInfo {
    using data_elem_type = OffOnNotAvlSrvRqrd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CrvtSpdEstimrSnvtySeld_info : OutTag, public DataElemInfo {
    using data_elem_type = SnvtyPen3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CrvtSpdWarnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CrvtSpdWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = TurnSpdWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CrvtSpdWarnSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CtraIndcnLe_info : InTag, public DataElemInfo {
    using data_elem_type = CtraIndcn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CtraIndcnRi_info : InTag, public DataElemInfo {
    using data_elem_type = CtraIndcn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CtraOn1_info : InTag, public DataElemInfo {
    using data_elem_type = LcmaCtraOn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CtraSwOnOff_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct CurtActvnReReq_info : OutTag, public DataElemInfo {
    using data_elem_type = CurtActvnTyp1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DataSpclDMSM_info : InTag, public DataElemInfo {
    using data_elem_type = DataSpclTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DayToSrv_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->750

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DeactvtLvlgCtrl_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DefrstDurgClimaEnad_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DiagcCCSM_info : InTag, public DataElemInfo {
    using data_elem_type = DiagcForPanCenCtrl2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DiagcRCSM_info : InTag, public DataElemInfo {
    using data_elem_type = DiagcForRCSM2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DiagcRSHC_info : InTag, public DataElemInfo {
    using data_elem_type = DiagcForRCSM2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DiagcStsDMSM_info : InTag, public DataElemInfo {
    using data_elem_type = DiagcStsTypDMSM;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DispAndHomeBtnSts_info : OutTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DispBattEgyIn_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->127.5

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DispBattEgyOut_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->127.5

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DispOfPrpsnMod_info : InTag, public DataElemInfo {
    using data_elem_type = DispOfPrpsnMod3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DispStsCen_info : OutTag, public DataElemInfo {
    using data_elem_type = FltCfmd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DoorDrvrLockReSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DoorDrvrLockSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DoorDrvrReSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DoorDrvrSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DoorPassLockReSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DoorPassLockSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DoorPassReSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DoorPassSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvModDispd_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvModReq_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvModReqType1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvgInWrgDirOfTrfc_info : OutTag, public DataElemInfo {
    using data_elem_type = NoYes1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrCtrlOfPassSeatFrntReqd_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrCtrlOfPassSeatFrntSts_info : InTag, public DataElemInfo {
    using data_elem_type = ActvnAvl1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrDesDir_info : InTag, public DataElemInfo {
    using data_elem_type = DrvrDesDir1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrDispSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrDispSetgReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrEntryLoReq_info : OutTag, public DataElemInfo {
    using data_elem_type = NoYes1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrHmiBackGndForHmiCen_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrHmiForHmiCen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrHmiBackGndInfoSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrHmiBackGndInfoSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrHmiDispdMod_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrHmiDispdModPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrHmiForHmiCen_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrHmiForHmiCen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrHmiTmrForHmiCen_info : OutTag, public DataElemInfo {
    using data_elem_type = Array8ByteU8;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrHmiUsrIfSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrHmiUserIfSetgReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrMassgRunng_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrPfmncLvl_info : InTag, public DataElemInfo {
    using data_elem_type = DrvrPfmncLvl2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrPfmncMonActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrPfmncSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrPfmncWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = DrvrPfmncWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrPfmncWarnReqToNav_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrPfmncWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrSeatActvSpplFct_info : InTag, public DataElemInfo {
    using data_elem_type = SeatActvSpplFct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrSeatDispMassgFct_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatMassgFct;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrSeatDispSpplFct_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatActvSpplFct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrSeatMassgFct_info : InTag, public DataElemInfo {
    using data_elem_type = SeatMassgFct;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrSeatSwtSts_info : InTag, public DataElemInfo {
    using data_elem_type = DrvrSeatSwtSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DrvrWLoadLvl_info : InTag, public DataElemInfo {
    using data_elem_type = Lvl30;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstNotifActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstNotifSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstToEmptyIndcd_info : InTag, public DataElemInfo {
    using data_elem_type = DstToEmptyWithUnit;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstToManvLocn_info : OutTag, public DataElemInfo {
    using data_elem_type = DstToManvLocnByNav;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstToSrv_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->32000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstTrvld1_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->9999.9

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstTrvld2_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->9999.9

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstTrvldHiResl_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->255

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct BkpOfDstTrvld_info : InTag, public DataElemInfo {
    using data_elem_type = uint32_t;  // 0->2000000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct DstTrvldMst2_info : InTag, public DataElemInfo {
    using data_elem_type = uint32_t;  // 0->1000000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EasyInOutDrvrSeatAdjmt_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EgySave_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EmgyAsscSts_info : InTag, public DataElemInfo {
    using data_elem_type = AsscSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngCooltIndcnReq_info : InTag, public DataElemInfo {
    using data_elem_type = EngCooltIndcn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngCooltLvl_info : InTag, public DataElemInfo {
    using data_elem_type = FldLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngFuCns_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->51000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngFuCnsFild_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->51000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngHrToSrv_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->2000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngIdleEco1_info : OutTag, public DataElemInfo {
    using data_elem_type = EngIdleEco1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngNSafe_info : InTag, public DataElemInfo {
    using data_elem_type = EngNSafe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngOilLvl_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->120

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngOilLvlSts_info : InTag, public DataElemInfo {
    using data_elem_type = EngOilLvlSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngOilPWarn_info : InTag, public DataElemInfo {
    using data_elem_type = EngOilPWarn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngSpdDispd_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->16383.5

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngStrtStopDrvModIndSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EngStrtStopSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EpbApplyAutSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EpbEngStallApplySetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EpbLampReq_info : InTag, public DataElemInfo {
    using data_elem_type = EpbLampReqRec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EscSptModReqdByDrvr_info : OutTag, public DataElemInfo {
    using data_elem_type = EscSptModReqdByDrvrRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EscSt_info : InTag, public DataElemInfo {
    using data_elem_type = EscStSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct EscWarnIndcnReq_info : InTag, public DataElemInfo {
    using data_elem_type = EscWarnIndcnReqRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ExtrMirrFoldSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ExtrMirrTiltSetg2_info : OutTag, public DataElemInfo {
    using data_elem_type = ExtrMirrTiltSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ExtrMirrTintgSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = MirrDimPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FRNetworkStatus_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->65535

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FltIndcrTurnLeFrnt_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FltIndcrTurnLeRe_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FltIndcrTurnRiFrnt_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FltIndcrTurnRiRe_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FltOfLiDaytiRunngRi_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FrntAxleWarn_info : InTag, public DataElemInfo {
    using data_elem_type = AxleWarn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FrntWiprLvrReq2_info : InTag, public DataElemInfo {
    using data_elem_type = FrntWiprLvrReq2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FuHeatrActv_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FuHeatrFuCns1_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->51000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FuHeatrFuCnsDurgCyc1_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->1.27

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FuLvlIndcd_info : InTag, public DataElemInfo {
    using data_elem_type = FuLvlValWithQly;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FuLvlLoIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FuLvlLoIndcnToNav_info : OutTag, public DataElemInfo {
    using data_elem_type = FuLvlLoIndcnToNav1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct FuLvlLoWarn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct GearIndcnRec_info : InTag, public DataElemInfo {
    using data_elem_type = GearIndcnRec2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct GlbRstForSetgAndData_info : OutTag, public DataElemInfo {
    using data_elem_type = GlbRstForSetgAndData;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HdHwAprvdWirelsAdr_info : OutTag, public DataElemInfo {
    using data_elem_type = HwAprvdWirelsAdr1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HdrestFoldReq2_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HeatrDurgDrvgReqd_info : OutTag, public DataElemInfo {
    using data_elem_type = OffOnAut1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiAudSts_info : OutTag, public DataElemInfo {
    using data_elem_type = FltCfmd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiCenForDrvrHmi_info : InTag, public DataElemInfo {
    using data_elem_type = HmiCenForDrvrHmi;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiCmptmtAirDistbnFrnt_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiCmptmtAirDistbnFrnt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiCmptmtCoolgReq_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiCmptmtCoolgReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiCmptmtTSp_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiCmptmtTSp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiDefrstElecReq_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiDefrstElecReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiDefrstElecSts_info : InTag, public DataElemInfo {
    using data_elem_type = HmiDefrstElecSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiDefrstMaxReq_info : OutTag, public DataElemInfo {
    using data_elem_type = ActrReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiHvacFanLvlFrnt_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiHvacFanLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiHvacFanLvlRe_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiHvacFanLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiHvacReCtrl_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiHvacReCtrl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiHvacRecircCmd_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiHvacRecircCmd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiSeatClima_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiSeatClima;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HmiSeatClimaExtd_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiSeatClimaExtd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HoodSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HptcWarnSeldForCllsnFwdWarn_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HudActvReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HudActvSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HudAdjmtReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HudDiagc_info : InTag, public DataElemInfo {
    using data_elem_type = HudDiagc;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HudErgoSetgReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HudSts_info : InTag, public DataElemInfo {
    using data_elem_type = HudStsForHmi;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HudVisFctSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = HudVisFctSetgReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HvacAirMFlowEstimd_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->1000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznData_info : OutTag, public DataElemInfo {
    using data_elem_type = HznDataGroup2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznEdge_info : OutTag, public DataElemInfo {
    using data_elem_type = HznEdgeGroup2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznPosn_info : OutTag, public DataElemInfo {
    using data_elem_type = HznPosnGroup3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznPosnExtd_info : OutTag, public DataElemInfo {
    using data_elem_type = HznPosnExtdGroup1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznProfLong_info : OutTag, public DataElemInfo {
    using data_elem_type = HznProfLongGroup3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznProfLongExtd_info : OutTag, public DataElemInfo {
    using data_elem_type = HznProfLongExtdGroup1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznProfSho_info : OutTag, public DataElemInfo {
    using data_elem_type = HznProfSho2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznSeg_info : OutTag, public DataElemInfo {
    using data_elem_type = HznSegGroup2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct HznSplyElectcSts_info : OutTag, public DataElemInfo {
    using data_elem_type = HznSplyElectcSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IndcnOfParkAssiSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IndcnOfPrkgAutSts_info : InTag, public DataElemInfo {
    using data_elem_type = IndOfPrkgAutSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IndcnUnit_info : OutTag, public DataElemInfo {
    using data_elem_type = IndcnUnit;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IndcrDisp1WdSts_info : InTag, public DataElemInfo {
    using data_elem_type = IndcrSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IndcrTurnSts1WdSts_info : InTag, public DataElemInfo {
    using data_elem_type = IndcrSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IniValSigCfgIDBackboneFR_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->65535

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct InsdCarNoiseMeasd_info : OutTag, public DataElemInfo {
    using data_elem_type = double;  // 0->127.5

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IntAudCnclWarn_info : OutTag, public DataElemInfo {
    using data_elem_type = YesNo2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IntrBriSts_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->15

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IntrBriStsForSeatHeatrRe_info : OutTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->15

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IntrLiAmbLiSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = IntrLiAmbLiSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IntrLiSurrndgsLiSetgLi_info : OutTag, public DataElemInfo {
    using data_elem_type = IntrLiSurrndgsLiSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IntrMirrTintgSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = MirrDimPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct IntvAndWarnModForLaneKeepAid_info : OutTag, public DataElemInfo {
    using data_elem_type = WarnAndIntvPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct KeyLostWarnIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = KeyLostWarnIndcn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct KeyProfMpgUpd_info : OutTag, public DataElemInfo {
    using data_elem_type = KeyProfMpgUpd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct KeyReadReqFromSetgMgr_info : OutTag, public DataElemInfo {
    using data_elem_type = KeyLocn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct KeyReadStsToProfCtrl_info : InTag, public DataElemInfo {
    using data_elem_type = KeyReadStsToProfCtrl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct KeyRmnIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct KeySpdWarn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LampSuppSrv_info : OutTag, public DataElemInfo {
    using data_elem_type = SrvSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LaneChgWarnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LaneChgWarnSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LaneDetnStsForAutDrv_info : InTag, public DataElemInfo {
    using data_elem_type = LaneDetnSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LaneDetnStsForLaneKeepAid_info : InTag, public DataElemInfo {
    using data_elem_type = LaneDetnSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LaneKeepAidActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LaneKeepAidRoadEdgeActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LaneKeepAidSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LatMovmtWarn_info : InTag, public DataElemInfo {
    using data_elem_type = WarnLeRi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LcmaOn1_info : InTag, public DataElemInfo {
    using data_elem_type = LcmaCtraOn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtReq1WdReq1_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtReq2WdReq1_info : OutTag, public DataElemInfo {
    using data_elem_type = LiAutTranPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtReq1WdReq4_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtReq1WdReq6_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtReq1WdReq2_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiDrvrFltIndcrTurn_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiForBtn4ForUsrSwtPanFrntCmd_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiForBtn5ForUsrSwtPanFrntCmd_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtSafe1WdReq2_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtSafe1WdReq1_info : OutTag, public DataElemInfo {
    using data_elem_type = LiTiPen2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiLvrSwt1Req_info : InTag, public DataElemInfo {
    using data_elem_type = SwtPush;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiPassFltIndcrTurn_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiSeldForDrvrPfmncMon_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiTrfcSide1WdReq1_info : OutTag, public DataElemInfo {
    using data_elem_type = LiTrfcSide1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LockSpdReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LockgCenSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockgCenSts3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LockgCenStsForUsrFb_info : InTag, public DataElemInfo {
    using data_elem_type = LockSt2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LockgFbSoundReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LockgFbVisReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LockgPrsnlReqFromHmi_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffNoReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LockgPrsnlSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LvlOfClimaCmft_info : InTag, public DataElemInfo {
    using data_elem_type = LvlOfClimaCmft;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LvlSeldForSpdLimAdpv_info : OutTag, public DataElemInfo {
    using data_elem_type = LvlSeldForSpdLimAdpvPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MassgFctActv_info : OutTag, public DataElemInfo {
    using data_elem_type = MassgFctActv;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MemBtnSound_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MirrDwnStsAtDrvr_info : InTag, public DataElemInfo {
    using data_elem_type = MirrDwnStsTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MirrDwnStsAtPass_info : InTag, public DataElemInfo {
    using data_elem_type = MirrDwnStsTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MirrFoldStsAtDrvr_info : InTag, public DataElemInfo {
    using data_elem_type = MirrFoldStsTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MirrFoldStsAtPass_info : InTag, public DataElemInfo {
    using data_elem_type = MirrFoldStsTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MmedHdPwrMod_info : OutTag, public DataElemInfo {
    using data_elem_type = MmedMaiPwrMod;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MmedHmiModStd_info : OutTag, public DataElemInfo {
    using data_elem_type = MmedHmiModStd2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MmedTvmPwerMod_info : OutTag, public DataElemInfo {
    using data_elem_type = MmedTvmPwerMod;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MstCfgIDBackboneFR_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->65535

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct MtrlSnsrT_info : InTag, public DataElemInfo {
    using data_elem_type = MtrlSnsrT;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NetCtrlrActvt_info : InTag, public DataElemInfo {
    using data_elem_type = NetActvtRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NetHdActvt_info : OutTag, public DataElemInfo {
    using data_elem_type = NetActvtRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NetTelmActvt_info : InTag, public DataElemInfo {
    using data_elem_type = NetActvtRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NewTripCdn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NoEntryWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = Warn2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NoSoundSys_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NotifChkDistbn_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->65535

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NrOfBltAppld_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->7

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NrOfKeyAvl_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct NrSerlDMSM_info : InTag, public DataElemInfo {
    using data_elem_type = NrSerlNodLIN;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct OffsForDrvrSpprtFctActvSts_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct OffsForSpdWarnSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OffsForSpdWarnSetgPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PartNrDMSM_info : InTag, public DataElemInfo {
    using data_elem_type = PartNrNodLIN;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PasAlrmDeactvnReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PasAlrmSts_info : InTag, public DataElemInfo {
    using data_elem_type = NoReqOffReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PassMassgRunng_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PassSeatActvSpplFct_info : InTag, public DataElemInfo {
    using data_elem_type = SeatActvSpplFct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PassSeatDispMassgFct_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatMassgFct;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PassSeatDispSpplFct_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatActvSpplFct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PassSeatMassgFct_info : InTag, public DataElemInfo {
    using data_elem_type = SeatMassgFct;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PassSeatSts_info : InTag, public DataElemInfo {
    using data_elem_type = PassSeatSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PassSeatSwtSts2_info : InTag, public DataElemInfo {
    using data_elem_type = PassSeatSwtSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PinionSteerAg1_info : InTag, public DataElemInfo {
    using data_elem_type = PinionSteerAg1Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PosnFromNav_info : OutTag, public DataElemInfo {
    using data_elem_type = PosnFromNav;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PosnFromSatlt_info : InTag, public DataElemInfo {
    using data_elem_type = PosnFromSatlt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PostDrvgClimaAvl_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PostDrvgClimaReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PreClngNotif_info : InTag, public DataElemInfo {
    using data_elem_type = PreClngNotif;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrkgAssiFailr_info : InTag, public DataElemInfo {
    using data_elem_type = PrkgAssiFailr2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrkgAssiManvActvSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrkgAssiManvProgs_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->63

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrkgAssiSts_info : InTag, public DataElemInfo {
    using data_elem_type = PrkgAssiSts3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrkgAutSts_info : InTag, public DataElemInfo {
    using data_elem_type = PrkgAutSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrkgCamSysAvlSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrkgOutCfm_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrkgTypVld_info : InTag, public DataElemInfo {
    using data_elem_type = PrkgTypVld1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ProfAct_info : InTag, public DataElemInfo {
    using data_elem_type = ProfAct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ProfChg_info : OutTag, public DataElemInfo {
    using data_elem_type = IdPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ProfLimd_info : InTag, public DataElemInfo {
    using data_elem_type = ProfLimd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ProfPenSts1_info : InTag, public DataElemInfo {
    using data_elem_type = IdPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgModAct_info : InTag, public DataElemInfo {
    using data_elem_type = HvBattUsgType;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgModReq_info : OutTag, public DataElemInfo {
    using data_elem_type = HvBattUsgType;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgOfChrgBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgOfChrgBlkd2_info : InTag, public DataElemInfo {
    using data_elem_type = HvBattUsgBlkdTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgOfHldBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgOfHldBlkd2_info : InTag, public DataElemInfo {
    using data_elem_type = HvBattUsgBlkdTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgOfHldSmtBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgOfHldSmtBlkd2_info : InTag, public DataElemInfo {
    using data_elem_type = HvBattUsgBlkdTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgOfHldSpd_info : OutTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->150

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnHvBattUsgStsDispd_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnModElecDrvBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnModOfSaveBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnModOfTracBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnModOffroadBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PrpsnModSptBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PtCluTq_info : InTag, public DataElemInfo {
    using data_elem_type = PtCluTq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PtDrvrSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = PtDrvrSetg2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PtGearTar_info : InTag, public DataElemInfo {
    using data_elem_type = PtGearAct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PtTqAtWhlFrntAct_info : InTag, public DataElemInfo {
    using data_elem_type = PtTqAtWhlFrntActRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PwrChrgDetdForPrkgHeatrElec_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct PwrSplyErrSts_info : InTag, public DataElemInfo {
    using data_elem_type = PwrSplyErrSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RainSenMemdReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ReAxleWarn_info : InTag, public DataElemInfo {
    using data_elem_type = AxleWarn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RmnLockgPrsnlReq_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RngbdIllmnCmd_info : OutTag, public DataElemInfo {
    using data_elem_type = RngBdIllmnCmdPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RoadFricIndcnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RoadFricIndcnSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RoadFricWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = RoadFricWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RoadSgnInfoActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RoadSgnInfoSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RoadSpdLimActvSts_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RotyDirReq2_info : InTag, public DataElemInfo {
    using data_elem_type = RotyDirUI;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RotyDirReq1_info : InTag, public DataElemInfo {
    using data_elem_type = RotyDirUI1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RotyPosReq2_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->31

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RotyPosReq1_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->31

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RsdsSysStsLe_info : InTag, public DataElemInfo {
    using data_elem_type = RsdsSysSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct RsdsSysStsRi_info : InTag, public DataElemInfo {
    using data_elem_type = RsdsSysSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SaveSetgToMemPrmnt_info : OutTag, public DataElemInfo {
    using data_elem_type = OffOnAut1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ScrBarVolIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // 0->100

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ScrMaxFillgVol_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->31

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct ScrReagentTankVol_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->31

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatBackUnlckdThrd_info : InTag, public DataElemInfo {
    using data_elem_type = SeatBackUnlckd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatDispBtnPsd_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatDispBtnPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatHeatDurgClimaEnad_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatHeatDurgClimaEnad;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatHeatgAutCdn_info : InTag, public DataElemInfo {
    using data_elem_type = Flg1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatSwtLeSigThrd_info : OutTag, public DataElemInfo {
    using data_elem_type = SwtVertSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatSwtRiSigThrd_info : OutTag, public DataElemInfo {
    using data_elem_type = SwtVertSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatVentnAutCdn_info : InTag, public DataElemInfo {
    using data_elem_type = Flg1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SetOfLang_info : OutTag, public DataElemInfo {
    using data_elem_type = SetOfLang;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SetOfLangAck_info : InTag, public DataElemInfo {
    using data_elem_type = FailrNoFailr1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SetTiAndDate_info : OutTag, public DataElemInfo {
    using data_elem_type = DateTi1ForSet;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SftySigGroupFromAudSafe_info : OutTag, public DataElemInfo {
    using data_elem_type = SftySigGroupFromAudSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SftyWarnGroupFromAsySafe_info : InTag, public DataElemInfo {
    using data_elem_type = SftyWarnGroupFromAsySafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SnsrParkAssiFrnt_info : InTag, public DataElemInfo {
    using data_elem_type = SnsrParkAssi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SnsrParkAssiRe_info : InTag, public DataElemInfo {
    using data_elem_type = SnsrParkAssi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SoundExtActvSts_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WhlMotSysSpdAct_info : InTag, public DataElemInfo {
    using data_elem_type = double;  // -1638.4->1638.3

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SpdAlrmActvForRoadSgnInfo_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SpdCameraInfoSeldForRoadSgnInfo_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SpdLimAdpvSts_info : InTag, public DataElemInfo {
    using data_elem_type = OffOnNotAvlSrvRqrd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SpdLimWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = SpdLimWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SpdWarn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SrvRst_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SrvTrig_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t;  // 0->15

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerAssiActvForAutDrv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerAssiLvlForAutDrv_info : OutTag, public DataElemInfo {
    using data_elem_type = SteerAssiLvlForAutDrvPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = SteerSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerWarnReqForAutDrv_info : InTag, public DataElemInfo {
    using data_elem_type = SteerWarnReqForAutDrv1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerWarnReqForLaneKeepAid_info : InTag, public DataElemInfo {
    using data_elem_type = WarnLeRi;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerWhlHeatgDurgClimaEnad_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerWhlHeatgOnReq_info : OutTag, public DataElemInfo {
    using data_elem_type = SteerWhlHeatgOnCmdTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerWhlHeatgStrtAutCdnOk_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerWhlSnsrSafe_info : InTag, public DataElemInfo {
    using data_elem_type = SteerWhlSnsrSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SteerWhlSwtPwr_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct StopStrtInhb_info : InTag, public DataElemInfo {
    using data_elem_type = uint32_t;  // 0->16777215

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct StrtInProgs_info : InTag, public DataElemInfo {
    using data_elem_type = StrtInProgs1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SunRoofPosnSts_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SuspSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = SuspSetgRec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SwtAcptReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SwtAtCenSts_info : OutTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SwtForPassAirbCutOffSt_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TankFlapSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TelmSts_info : InTag, public DataElemInfo {
    using data_elem_type = TelmSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TiAndDateIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = DateTi30;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TiCorrnByMapData_info : OutTag, public DataElemInfo {
    using data_elem_type = TiCorrn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TiGapAdpvSeld_info : OutTag, public DataElemInfo {
    using data_elem_type = TiGapAdpvSeldPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TiGapLimd_info : OutTag, public DataElemInfo {
    using data_elem_type = TiGapPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TireCircumCalByNav_info : OutTag, public DataElemInfo {
    using data_elem_type = TireCircumCalByNav;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TireMonCalReq_info : OutTag, public DataElemInfo {
    using data_elem_type = StrtStopReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TireMonrDispReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TirePAbsltValFrntRi_info : InTag, public DataElemInfo {
    using data_elem_type = TirePAbsltValFrntRi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TirePAbsltValFrntLe_info : InTag, public DataElemInfo {
    using data_elem_type = TirePAbsltValFrntLe3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TirePAbsltValReLe_info : InTag, public DataElemInfo {
    using data_elem_type = TirePAbsltValReLe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TirePAbsltValReRi_info : InTag, public DataElemInfo {
    using data_elem_type = TirePAbsltValReRi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TirePCalSts_info : InTag, public DataElemInfo {
    using data_elem_type = TirePCalSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TirePMonData_info : InTag, public DataElemInfo {
    using data_elem_type = TirePMonData1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TnlEntryOrEnd_info : InTag, public DataElemInfo {
    using data_elem_type = NoYes1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TrSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TrfcRglnSeldForRoadSgnInfo_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TripModSeln_info : OutTag, public DataElemInfo {
    using data_elem_type = TripModSeln;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TrlrLampChkSts1WdSts1_info : InTag, public DataElemInfo {
    using data_elem_type = TrlrLampChkSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TrlrLampChkAutReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TrlrLampChkRemReq_info : OutTag, public DataElemInfo {
    using data_elem_type = StrtStopReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TrlrLampChkSts1WdSts2_info : InTag, public DataElemInfo {
    using data_elem_type = TrlrLampChkSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TrlrPrsntSts1WdSts_info : InTag, public DataElemInfo {
    using data_elem_type = TrlrPrsntSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TrsmGearAct_info : InTag, public DataElemInfo {
    using data_elem_type = PtGearAct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct LiExtReq1WdReq3_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TwliBriRaw_info : InTag, public DataElemInfo {
    using data_elem_type = TwliBriRaw;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TwliBriSts_info : InTag, public DataElemInfo {
    using data_elem_type = TwliBriSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct TwliBriStsForSeatHeatrRe_info : OutTag, public DataElemInfo {
    using data_elem_type = TwliBriSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UkwnCptReqToInfoMgr_info : InTag, public DataElemInfo {
    using data_elem_type = UInt64Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UkwnCptReqToSoundMgr_info : InTag, public DataElemInfo {
    using data_elem_type = UInt64Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UkwnCptRespFromInfoMgr_info : OutTag, public DataElemInfo {
    using data_elem_type = UInt64Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UkwnCptRespFromSoundMgr_info : OutTag, public DataElemInfo {
    using data_elem_type = UInt64Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UnlckFbVisReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UnlckKeylsReq_info : OutTag, public DataElemInfo {
    using data_elem_type = UnlckKeylsCfgPen2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UnlckRemReq_info : OutTag, public DataElemInfo {
    using data_elem_type = UnlckRemCfgPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSetSpdForKeySpdLimn_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSetSpdForKeySpdLimn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSetSpdForKeySpdWarn_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSetSpdForKeySpdWarn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSetVolMaxForKeyVolLimn_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSetVolMaxForKeyVolLimn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSwtDispClimaReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffIdle1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSwtDispClimaReqForRowSec_info : InTag, public DataElemInfo {
    using data_elem_type = UsrSwtDispClimaReqForRowSec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatHeatReLeSts_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatClimaLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatHeatReRiSts_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatClimaLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSwtDispClimaSts_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSwtDispClimaSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSwtDispForSecRowSeatVentn_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSwtDispForSecRowSeatVentn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSwtDispReqForSecRowSeatVentn_info : InTag, public DataElemInfo {
    using data_elem_type = UsrSwtDispReqForSecRowSeatVentn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct UsrSwtDispReqVrnt_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSwtDispReqVrnt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatHeatLvlReqLe_info : InTag, public DataElemInfo {
    using data_elem_type = SeatClimaLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct SeatHeatLvlReqRi_info : InTag, public DataElemInfo {
    using data_elem_type = SeatClimaLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VFCVectorIHU_info : OutTag, public DataElemInfo {
    using data_elem_type = VFCGrp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehActvMsgToDrvr_info : InTag, public DataElemInfo {
    using data_elem_type = VehActv1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehBattU_info : InTag, public DataElemInfo {
    using data_elem_type = SysU;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehCfgPrm_info : InTag, public DataElemInfo {
    using data_elem_type = VehCfgPrm;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehDecelCtrlSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = NormSptPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehMNom_info : InTag, public DataElemInfo {
    using data_elem_type = VehMNomRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehModMngtGlbSafe1_info : InTag, public DataElemInfo {
    using data_elem_type = VehModMngtGlbSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehMtnStSafe_info : InTag, public DataElemInfo {
    using data_elem_type = VehMtnStSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct AudWarn_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehSpdAvgIndcd_info : InTag, public DataElemInfo {
    using data_elem_type = VehSpdIndcd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehSpdCtrlActvSts_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehSpdIndcd_info : InTag, public DataElemInfo {
    using data_elem_type = VehSpdIndcd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VehSpdLgtSafe_info : InTag, public DataElemInfo {
    using data_elem_type = VehSpdLgtSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct Vin_info : InTag, public DataElemInfo {
    using data_elem_type = Vin1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VisnAgWideCfg_info : OutTag, public DataElemInfo {
    using data_elem_type = VisnAgWideCfg1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VisnAgWideSts_info : InTag, public DataElemInfo {
    using data_elem_type = AvlSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VisnImgAgWideInUse_info : InTag, public DataElemInfo {
    using data_elem_type = VisnImgAgWideInUse;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VisnImgAgWideReq_info : OutTag, public DataElemInfo {
    using data_elem_type = VisnImgAgWideInUse;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct VisnImgCamReq_info : OutTag, public DataElemInfo {
    using data_elem_type = CamIndReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WarnTypForLaneChgWarn_info : OutTag, public DataElemInfo {
    using data_elem_type = WarnTypForLaneChgWarnPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WarnTypForLaneKeepAid_info : OutTag, public DataElemInfo {
    using data_elem_type = WarnTypForLaneChgWarnPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WhlCircum_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t;  // 0->4095

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WhlMotSysTqEst_info : InTag, public DataElemInfo {
    using data_elem_type = TqSafe2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WhlRotToothCntr_info : InTag, public DataElemInfo {
    using data_elem_type = WhlRotToothCntrRec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WinPosnStsAtDrv_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WinPosnStsAtDrvrRe_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WinPosnStsAtPass_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WinPosnStsAtPassRe_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WipgInfo_info : InTag, public DataElemInfo {
    using data_elem_type = WipgInfo;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WiprFrntSrvModReq_info : OutTag, public DataElemInfo {
    using data_elem_type = WiprFrntSrvModReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WiprInPosnForSrv_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WiprReAutReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct WshrFldSts1WdElmHMI_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct iTPMSCalPsbl_info : InTag, public DataElemInfo {
    using data_elem_type = CalPsbl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct iTPMSCalReq_info : OutTag, public DataElemInfo {
    using data_elem_type = StrtStopReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct iTPMSCalSts_info : InTag, public DataElemInfo {
    using data_elem_type = iTPMSTirePCalSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

struct iTPMSTirePMSts_info : InTag, public DataElemInfo {
    using data_elem_type = iTPMSTirePMSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};

}  // end of namespace
#endif