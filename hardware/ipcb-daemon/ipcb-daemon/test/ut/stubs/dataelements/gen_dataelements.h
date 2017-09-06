/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
// C++ code generator for AUTOSAR v1.0
// Generated at: 2016-04-28T11:06:19.388754
// Source: SPA1610_IHUVOLVO8_151214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml
#ifndef _DATAELEMENTS_H
#define _DATAELEMENTS_H

#include "gen_datatypes.h"

namespace autosar {

enum class Dir {
    IN,
    OUT
};

struct DataElemInfo {
    virtual const char* name() const=0;
    virtual const char* typeName() const=0;
    virtual Dir direction() const=0;
};

// Provide compile time info if a data-element is in or out
class InTag {
};

class OutTag {
};


/** 
                IMU acceleration data */ 
struct ADataRawSafe_info : InTag, public DataElemInfo {
    using data_elem_type = ADataRawSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting Adaptive Cruise Control Speed Limit Adaptation On/Off status. */ 
struct AccAdprSpdLimActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting Adaptive Cruise Control Curve Speed Adaptation On/Off status
 */ 
struct AccAdprTurnSpdActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The signal indicates the status (or state) of the ACC; Off, Standby, Actv etc. */ 
struct AccSts_info : InTag, public DataElemInfo {
    using data_elem_type = AccSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Active Noise Control in Drive Mode Individual */ 
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


/** 
                Distribution of the activation of Eco-Cruise function */ 
struct ActvnOfCrsEco_info : OutTag, public DataElemInfo {
    using data_elem_type = ActvnOfCrsEco;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of activation/deactvation of the function PAS, note not the graphics trigger
 */ 
struct ActvnOfPrkgAssi_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Activation of semi-automatic parking function */ 
struct ActvnOfPrkgAut_info : OutTag, public DataElemInfo {
    using data_elem_type = ActvnOfPrkgAut;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Activates/deactivates the symbol illumination of the CCSM. Dependent on VMM signals mostly. */ 
struct ActvnOfSwtIllmnCen_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Activates/deactivates the symbol illumination of the RSCM and RSHC. Dependent on VMM signals mostly. */ 
struct ActvnOfSwtIllmnClima_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Activates/deactivates the symbol illumination of the drive mode switch. Dependent on VMM signals mostly. */ 
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


/** 
                The signal indicates the status (or state) of the Speed Limiter (ASL/AVSL); Off, Standby, Actv etc. */ 
struct AdjSpdLimnSts_info : InTag, public DataElemInfo {
    using data_elem_type = AdjSpdLimnSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The signal triggers visual and/or audial warnings to the driver if the Speed Limiter (ASL/AVSL) can't reach the desired set speed limit. */ 
struct AdjSpdLimnWarn_info : InTag, public DataElemInfo {
    using data_elem_type = AdjSpdLimnWarnCoding;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting Adaptive Cruise Control Curve Speed Adaptation mode status
 */ 
struct AdprTurnSpdMod_info : OutTag, public DataElemInfo {
    using data_elem_type = AdprTurnSpdModPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                IMU angular rate data. */ 
struct AgDataRawSafe_info : InTag, public DataElemInfo {
    using data_elem_type = AgDataRawSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for AHBC lights
 */ 
struct LiExtReq1WdReq5_info : OutTag, public DataElemInfo {
    using data_elem_type = SlowNormFastPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Ionizer request from HMI to CCM.
  */ 
struct AirClngReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffNoReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicated (damped) ambient temp data from the instrument cluster. */ 
struct AmbTIndcdWithUnit_info : InTag, public DataElemInfo {
    using data_elem_type = AmbTIndcdWithUnit;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Unfiltered ambient temp raw data in deg C. */ 
struct AmbTRaw_info : InTag, public DataElemInfo {
    using data_elem_type = AmbTWithQly;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Used to distribute the setting Reduced guard. */ 
struct AntithftRednReq_info : OutTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->7

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to the driver information system to inform that driver has to put hands on steering wheel. */ 
struct AsySteerApplyRqrd_info : InTag, public DataElemInfo {
    using data_elem_type = LvlWarn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Used by the DIM to request audio chimes */ 
struct AudMsgReq_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->255

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal to CEM that sound has been played for GearPositionWarning. */ 
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
    using data_elem_type = uint8_t; // 0->255

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the Blind Spot Information System (BLIS) Lane Change Merge Aid (LCMA) function: */ 
struct BlisSwOnOff_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                State of belt lock at driver seating position */ 
struct BltLockStAtDrvr_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStFrnt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                State of belt lock at passenger seating position */ 
struct BltLockStAtPass_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStFrnt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                State of belt lock at seating position at left of second row */ 
struct BltLockStAtRowSecLe_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                State of belt lock at seating position at middle of second row */ 
struct BltLockStAtRowSecMid_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                State of belt lock at seating position at right of second row */ 
struct BltLockStAtRowSecRi_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                State of belt lock at seating position at left of third row */ 
struct BltLockStAtRowThrdLe_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                State of belt lock at seating position at right of third row */ 
struct BltLockStAtRowThrdRi_info : InTag, public DataElemInfo {
    using data_elem_type = BltLockStRe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Safety belt reminder sound trigger for sound type 1 */ 
struct BltRmnSound1_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Safety belt reminder sound trigger for sound type 2 */ 
struct BltRmnSound2_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Safety belt reminder sound trigger for sound type 3 */ 
struct BltRmnSound3_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Safety belt reminder sound trigger for sound type 4 */ 
struct BltRmnSound4_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request for warning indication to driver in case of brake failure (red warning) or ABS failure (yellow warning) */ 
struct BrkAndAbsWarnIndcnReq_info : InTag, public DataElemInfo {
    using data_elem_type = BrkAndAbsWarnIndcnReqRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Warning request for low brake fluid level. */ 
struct BrkFldLvl_info : InTag, public DataElemInfo {
    using data_elem_type = FldLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Actual service brake torque for each wheel. */ 
struct BrkFricTqAtWhlAct_info : InTag, public DataElemInfo {
    using data_elem_type = BrkFricTqAtWhlActRec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Driver warning during vehicle standstill for releasing the brakes without possibility to park the vehicle. */ 
struct BrkRelsWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = NoYes1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Btn1 is the ON/OFF/PAUSE button pressed - set to "1" when user press this button */ 
struct Btn1ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Btn2 is the SKIP BACKWARD button - set to "1" when user press this button */ 
struct Btn2ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Btn3 is the SKIP FORWARD button - set to "1" when pressed else set to "0" */ 
struct Btn3ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Btn 4 is the FRONT defroster button - set to "1"  when pressed else "0" */ 
struct Btn4ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Btn5 is the REAR defroster button pressed - set to "1" when user pressed this button */ 
struct Btn5ForUsrSwtPanFrntReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                That is the "Push to Talk" Button signal sent from DIM to IHU ( SWS Button R6 )
Shall start the Speech Session

?
  */ 
struct BtnAudFbSts_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of volume button presses on steering wheel */ 
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


/** 
                Distribution of left and right steering wheel button presses */ 
struct BtnMmedLeRiSts_info : InTag, public DataElemInfo {
    using data_elem_type = BtnMmedLeRiSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                That is the "Mode" Button signal sent from DIM to IHU ( SWS Button R7 )
Shall cancel the Speech Session */ 
struct BtnMmedModSts_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of "set" steering wheel button press */ 
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


/** 
                Presents a tiime value that shall be used by all ECUs to time-stamp diagnsotic trouble codes (DTC) according to the requirements for Global DTC snapshot data. */ 
struct CarTiGlb_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.1*x+0.0 (0->4.294967294E8) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Eco Coach data, both calculated instantaneous and grade eco coach data. */ 
struct CchForFuEco_info : InTag, public DataElemInfo {
    using data_elem_type = CchForFuEco1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                "Signal sent from the Driver door module to the Driver information module to inform if the electric child lock  is ative or not
"
 */ 
struct ChdLockgProtnStsToHmi_info : InTag, public DataElemInfo {
    using data_elem_type = OnOffSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                "Signal sent from the rear driver door module to the Driver door module that informs of the local status of the power window disable functionality.
"
 */ 
struct ChdWinProtnStsAtDrvrRe_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                "Signal sent from the rear passenger door module to the Driver door module that informs of the local status of the power window disable functionality.
"
 */ 
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


/** 
                Informs if a parking climatisation is active (either pre- or post conditioning). */ 
struct ClimaActv_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Actual current consumption of electric climate loads */ 
struct ClimaPwrCns_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t; // 1.0*x+0.0 (0->2047) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Pre-Climatisation request from HMI. */ 
struct ClimaRqrdFromHmi_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffNoReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Time that the car shall be finished with pre-condition. */ 
struct ClimaTmr_info : OutTag, public DataElemInfo {
    using data_elem_type = double; // 0.1*x+0.0 (0->4.294967294E8) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Informs if any timers are set for pre conditioning. */ 
struct ClimaTmrSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                request to wake up at time sent in "ClimaTmr"-signal. */ 
struct ClimaTmrStsRqrd_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Informs if anything has gone wrong with the parking climatisation. */ 
struct ClimaWarn_info : InTag, public DataElemInfo {
    using data_elem_type = ClimaWarn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                "Distribution of the setting Collision Avoidance Sensitivity Level  
" */ 
struct CllsnAidSnvtySeld_info : OutTag, public DataElemInfo {
    using data_elem_type = SnvtyPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of collision warning
 */ 
struct CllsnFwdWarnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to activate collision warning against forward object */ 
struct CllsnFwdWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to activate general collision warning. */ 
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


/** 
                Determine the setting for automatic Roof Hatch Maneuvering upon ignition off */ 
struct ClsAutEna_info : OutTag, public DataElemInfo {
    using data_elem_type = EnableDisableCoding;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Estimated compartment temperature with quality flag */ 
struct CmptmtAirTEstimdExtd_info : InTag, public DataElemInfo {
    using data_elem_type = CmptmtAirTEstimdExtd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Measured temperature in compartment. Sensor has a small aspiration fan that sucks air pass temperature sensor. Component is located in the middle center console.  */ 
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


/** 
                Request to activate Autonomous Drive longitudinal cancel warning */ 
struct CnclWarnLgtForAutDrv_info : InTag, public DataElemInfo {
    using data_elem_type = NoYesCrit1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Control signal for VDDM to see if ESM is up.

Also signals DIM for trigger of message. */ 
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


/** 
                Distribution of forced cooling request of center display components to Climate   */ 
struct CoolgReqForDispCen_info : OutTag, public DataElemInfo {
    using data_elem_type = CoolgReqForDisp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status of 'forced cooling' of infotainment components */ 
struct CoolgStsForDisp_info : InTag, public DataElemInfo {
    using data_elem_type = CoolgStsForDisp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The signal indicates the status (or state) of the Cruise Controller; Off, Standby, Actv etc. */ 
struct CrsCtrlrSts_info : InTag, public DataElemInfo {
    using data_elem_type = CrsCtrlrSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Curvature speed adaption status. */ 
struct CrvtSpdAdpvSts_info : InTag, public DataElemInfo {
    using data_elem_type = OffOnNotAvlSrvRqrd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of curve speed warning sensitivity */ 
struct CrvtSpdEstimrSnvtySeld_info : OutTag, public DataElemInfo {
    using data_elem_type = SnvtyPen3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of active status of curve speed warning
 */ 
struct CrvtSpdWarnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to activate warning for too high speed for upcoming curvature */ 
struct CrvtSpdWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = TurnSpdWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Curve Speed Warning function status. */ 
struct CrvtSpdWarnSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicates if a left CTRA warning shall be shown to the driver or not. */ 
struct CtraIndcnLe_info : InTag, public DataElemInfo {
    using data_elem_type = CtraIndcn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicates if a right CTRA warning shall be shown to the driver or not. */ 
struct CtraIndcnRi_info : InTag, public DataElemInfo {
    using data_elem_type = CtraIndcn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Shows if the CTRA function is activated or deactivated.  */ 
struct CtraOn1_info : InTag, public DataElemInfo {
    using data_elem_type = LcmaCtraOn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting for cross traffic alert
 */ 
struct CtraSwOnOff_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Movement command to the back window sun curtain */ 
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


/** 
                Days left before service shall be done */ 
struct DayToSrv_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t; // 0->750

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for deactivating Leveling Control and Dampening Control for Suspension
 */ 
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


/** 
                signal to send diagnostic information from CCSM to LIN19 master IHU.
0-No fault(OK), 1-Fan Error, 2-Outside voltage range, 3-Watch dog error,4-Mem error,5-spare,6-spare,7-SnrFltT
 */ 
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


/** 
                Status if the touch screen or home button is used. */ 
struct DispAndHomeBtnSts_info : OutTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Electric battery energy absorbed by the HV Battery. 

NB: Only power flowing into HV battery is integrated. 

Energy=integral( max( battery power flowing in , 0) ) */ 
struct DispBattEgyIn_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.5*x+0.0 (0->127.5) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Electric battery energy delivered by the HV Battery. 

NB: Only power flowing out of the HV battery is integrated. 

Energy=integral( max( battery power flowing out , 0) ) */ 
struct DispBattEgyOut_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.5*x+0.0 (0->127.5) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Description:
Actual powertrain propulsion mode of the hybrid propulsion system.
Usage: For hybrid power flow status display in HMI.
 */ 
struct DispOfPrpsnMod_info : InTag, public DataElemInfo {
    using data_elem_type = DispOfPrpsnMod3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of functional status of the center(IHU) display 
 */ 
struct DispStsCen_info : OutTag, public DataElemInfo {
    using data_elem_type = FltCfmd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                DoorDrvrLockReSts - Driver Side Rear Door Lock Status

Lock status:

LockStsUkwn - Lock Status Unknown
Unlckd - Unlocked
Lockd - Locked
SafeLockd - Safe Locked (Double Locked) */ 
struct DoorDrvrLockReSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                DoorDrvrLockSts - Driver Door Lock Status

Lock status:

LockStsUkwn - Lock Status Unknown
Unlckd - Unlocked
Lockd - Locked
SafeLockd - Safe Locked (Double Locked) */ 
struct DoorDrvrLockSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                DoorDrvrReSts - Driver Side Rear Door Status

Ukwn - Unknown status
Opend - Opened
Clsd - Closed */ 
struct DoorDrvrReSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                DoorDrvrSts - Driver Door Status

Ukwn - Unknown status
Opend - Opened
Clsd - Closed */ 
struct DoorDrvrSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                DoorPassLockReSts - Passenger Side Rear Door Lock Status

Lock status:

LockStsUkwn - Lock Status Unknown
Unlckd - Unlocked
Lockd - Locked
SafeLockd - Safe Locked (Double Locked) */ 
struct DoorPassLockReSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                DoorPassLockSts - Passenger Door Lock Status

Lock status:

LockStsUkwn - Lock Status Unknown
Unlckd - Unlocked
Lockd - Locked
SafeLockd - Safe Locked (Double Locked) */ 
struct DoorPassLockSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                DoorPassReSts - Passenger Side Rear Door Status

Ukwn - Unknown status
Opend - Opened
Clsd - Closed */ 
struct DoorPassReSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                DoorPassSts - Passenger Door Status

Ukwn - Unknown status
Opend - Opened
Clsd - Closed */ 
struct DoorPassSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distributes information to subsystem Propulsion that the Drive Mode popup menu is activated in CSD
 */ 
struct DrvModDispd_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distributes the desired Drive Mode chosen by the user to all nodes which determine the charactaristics of the vehicle
 */ 
struct DrvModReq_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvModReqType1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Wrong way alert information from the navigation system. The vehicle is driving towards the direction of the traffic: No/Yes. */ 
struct DrvgInWrgDirOfTrfc_info : OutTag, public DataElemInfo {
    using data_elem_type = NoYes1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of activation request for gentleman function
 */ 
struct DrvrCtrlOfPassSeatFrntReqd_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The Remote control of passenger (Gentleman Function) seat function shall let the Driver control the Passengers power seat. The function shall be activated by entering a menu in the vehicles center stack display (Touch screen) and activate it. When the Gentleman Function is activated the adjustment switches attached to the driver side's seat ECU shall control the passenger side's seat instead of its own seat. It shall be done by sending a signal from the driver ECU to the passenger.
The Driver ECU shall receive information from subsystem HMI Management & Display whether the user has selected the Gentleman Function to be active or not. This shall be done via the signal DrvrCtrlOfPassSeatFrntReqd (Off/On). The Driver ECU shall report status of the Gentleman Function to HMI and the Passenger ECU via the signal DrvrCtrlOfPassSeatFrntSts (Not Available/Available/Deactivated/Activated).
 */ 
struct DrvrCtrlOfPassSeatFrntSts_info : InTag, public DataElemInfo {
    using data_elem_type = ActvnAvl1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Desired Driver Direction
 
Desired Driver Direction is an interpretation of driver input. 
It specifies in which longitudinal direction the driver requests that any accelerating propulsive torque shall be delivered. There are three possible directions:
 
1. Forward - The driver intends propulsion to move the vehicle forward
2. Reverse - The driver intends propulsion to move the vehicle backwards
3. Neutral - The driver intends no powered movement of the vehicle 
 
Note that the latter in the case of a conventional automatic gearbox thus applies to gear positions "N" and "P".
 
Note that Desired Driver Direction describes the driver's request, which not necessarily coincides with the actual state of the powertrain at all times. In e.g. shift-by-wire solutions the driver's choice of "driving direction" is based on gear selector input. Changes to Desired Driving Direction will thus be known a short while before the powertrain has reconfigured accordingly.
 
Note that Desired Driver Direction will not reflect implausible driver requests, e.g. changing from Forward to Reverse while moving forward in high speed.
 
Note that Desired Driver Direction will initialize to the current powertrain configuration during startup (ECU boot, reset etc). */ 
struct DrvrDesDir_info : InTag, public DataElemInfo {
    using data_elem_type = DrvrDesDir1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for which Drive Mode layout in DIM that shall be displayed in Drive Mode Individual
 */ 
struct DrvrDispSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrDispSetgReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Easy entry
 */ 
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


/** 
                Determine the setting for which Background to be used in DIM
 */ 
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


/** 
                Used for Apix 2 protocol from IHU to DIM
 */ 
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


/** 
                Determine the setting for which Skin/Theme to be used in DIM
 */ 
struct DrvrHmiUsrIfSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrHmiUserIfSetgReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                When the massage system has been activated it will stay on for approximately 10 minutes. A signal Driver/Passenger Massage Running {On, Off} shall be send On to HMI if system running (Signal names: DrvrMassgRunng/PassMassgRunng).  */ 
struct DrvrMassgRunng_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Current driving performance level. */ 
struct DrvrPfmncLvl_info : InTag, public DataElemInfo {
    using data_elem_type = DrvrPfmncLvl2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting (on or off) for Driver Impairment Monitor (DIMON) 
 */ 
struct DrvrPfmncMonActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Driver Impairment Monitor function status */ 
struct DrvrPfmncSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to activate warning for reduced driver performance. */ 
struct DrvrPfmncWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = DrvrPfmncWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Disitribution of Driver Impairment Monitor (DIMON)  to navigation system.
 */ 
struct DrvrPfmncWarnReqToNav_info : OutTag, public DataElemInfo {
    using data_elem_type = DrvrPfmncWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The PSMD/PSMP multifunction switch consists of a 4-way adjustment switch and a 2-way integrated switch. By pressing the integrated switch up or down, the next supported seat function shall be activated. Information of which function that is currently active shall be transmitted from the seat Control subsystem ECU's via the signals DrvrSeatActvSpplFct and PassSeatActvSpplFct to the subsystem Hmi managagement & control.
 */ 
struct DrvrSeatActvSpplFct_info : InTag, public DataElemInfo {
    using data_elem_type = SeatActvSpplFct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distributes massage settings for the driver.
 */ 
struct DrvrSeatDispMassgFct_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatMassgFct;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Handles the two way communication for driver multi seat control.
 */ 
struct DrvrSeatDispSpplFct_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatActvSpplFct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                To maneuver the driver and passenger front seat massage system the customer shall be able to access the system functions via both the so called multifunction switch on the Seat and via the IP touch screen. The DrvrSeatMassgFct/PassSeatMassgFct signal will contain data for navigating in the massage menu in the CSD. */ 
struct DrvrSeatMassgFct_info : InTag, public DataElemInfo {
    using data_elem_type = SeatMassgFct;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The Remote control of passenger (Gentleman Function) seat function shall let the Driver control the Passengers power seat. The function shall be activated by entering a menu in the vehicles center stack display (Touch screen) and activate it. When the Gentleman Function is activated the adjustment switches attached to the driver side's seat ECU shall control the passenger side's seat instead of its own seat. It shall be done by sending a signal from the driver ECU to the passenger.

Signalgroup DrvrSeatSwtSts shall be transmitted from The Driver ECU to the Passenger ECU for control of the passenger seat.
 */ 
struct DrvrSeatSwtSts_info : InTag, public DataElemInfo {
    using data_elem_type = DrvrSeatSwtSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Current calculated driver's workload level */ 
struct DrvrWLoadLvl_info : InTag, public DataElemInfo {
    using data_elem_type = Lvl30;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting  Distance Alert (DA)
 */ 
struct DstNotifActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distance Alert function status */ 
struct DstNotifSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicated distance to Empty (D.T.E.) with indicated unit */ 
struct DstToEmptyIndcd_info : InTag, public DataElemInfo {
    using data_elem_type = DstToEmptyWithUnit;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distance To Manoeuvre Location provided by the Navigation system.
- When there is no GNSS receiver fitted or the car is outside of map data coverage; SpprtForFct = No
- PosnQly indicates the quality of the positioning.
- When under route guidance, the distance in metres to the next manoueuvre is indicated by DstToManv. If the distance is >=4000 metres or if route guidance is not active then DstToManv = 4000.
- Before preparation guidance timing for the next manoueuvre, CntDwnToManvStrt = No while after preparation guidance timing CntDwnToManvStrt = Yes.

 */ 
struct DstToManvLocn_info : OutTag, public DataElemInfo {
    using data_elem_type = DstToManvLocnByNav;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distance (km) left before service shall be done */ 
struct DstToSrv_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t; // 0->32000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Value of the indicated Trip meter 1. */ 
struct DstTrvld1_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.1*x+0.0 (0->9999.9) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Value of the indicated Trip meter 2. */ 
struct DstTrvld2_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.1*x+0.0 (0->9999.9) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                High resolution trip meter value, resolution of 1m with a modulus 256 counter (counts 0 -> 255 -> 0 etc). */ 
struct DstTrvldHiResl_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->255

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicated odometer (total driven distance) value in km. */ 
struct BkpOfDstTrvld_info : InTag, public DataElemInfo {
    using data_elem_type = uint32_t; // 1.0*x+0.0 (0->2000000) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicated odometer value in unit miles. */ 
struct DstTrvldMst2_info : InTag, public DataElemInfo {
    using data_elem_type = uint32_t; // 0->1000000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of setting for Easy Ingress and Egress
 */ 
struct EasyInOutDrvrSeatAdjmt_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Energy save selected by user during Drive Mode individual */ 
struct EgySave_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicates the current status of the Emergency Assistance function (VEH-FCT : 184B - Emergency Assist / MAIN; 0). */ 
struct EmgyAsscSts_info : InTag, public DataElemInfo {
    using data_elem_type = AsscSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to display warning / info message to customer in DIM regarding engine cooling system. Possible warning messages:
- no message
- high engine temp reduce speed
- high engine temp stop safely
- high engine temp stop engine
- high engine temp see manual
- coolant low stop engine
- coolant low stop safely
- coolant low see manual
- coolant low check level
- coolant system service required */ 
struct EngCooltIndcnReq_info : InTag, public DataElemInfo {
    using data_elem_type = EngCooltIndcn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Engine coolant level. 
0 = High coolant level (normal)
1 = Low coolant level */ 
struct EngCooltLvl_info : InTag, public DataElemInfo {
    using data_elem_type = FldLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The transmitted econometer data for used for calculations in DIM shall be unfiltered and without any offset. Thus, this econometer data shall be as close to the true current fuel consumption by engine as possible, at all times.
 */ 
struct EngFuCns_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 200.0*x+0.0 (0->51000) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The econometer data signal for used for driver display have the following requirement for Econometer equalization: 

Deviations from normal data (at e.g. regeneration) shall be taken care of by the ECM (Engine Control Module) fuel consumption algorithm.

Extra fuel consumption during these events shall be stored locally in the ECM, to be transmitted as a distributed small added extra consumption data. This extra value shall be so designed such that there is no to the customer visible significant jumps in the transmitted ECON signal (that is related to the extra fuel consumed during the specific event). The total transmitted fuel consumption as indicated by the ECON signal, shall over time equal true consumption.
 */ 
struct EngFuCnsFild_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 200.0*x+0.0 (0->51000) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Engine hours left before a service shall be done */ 
struct EngHrToSrv_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t; // 0->2000

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Coasting (Engine propulsion) in Drive Mode Individual
 */ 
struct EngIdleEco1_info : OutTag, public DataElemInfo {
    using data_elem_type = EngIdleEco1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Engine Speed [rpm]
Engine Speed derivative [rpm/s] */ 
struct EngNSafe_info : InTag, public DataElemInfo {
    using data_elem_type = EngNSafe;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Engine oil level, Range 0-120 % */ 
struct EngOilLvl_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.5*x+0.0 (0->120) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Engine oil level status information.
If no status currently can be derived "Not available" will be shown. */ 
struct EngOilLvlSts_info : InTag, public DataElemInfo {
    using data_elem_type = EngOilLvlSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to set low engine oil pressure warning to driver */ 
struct EngOilPWarn_info : InTag, public DataElemInfo {
    using data_elem_type = EngOilPWarn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Engine speed value for engine speed meter in the driver information display. Note that this speed value may differ from the real actual engine speed. */ 
struct EngSpdDispd_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.5*x+0.0 (0->16383.5) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Start/Stop activation in Drive Mode Individual
 */ 
struct EngStrtStopDrvModIndSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting for Start/stop
 */ 
struct EngStrtStopSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Automatic activation of Electronic Parking Brake
 */ 
struct EpbApplyAutSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Automatic activation of Electronic Parking Brake during engine stall
 */ 
struct EpbEngStallApplySetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                E2E secured Epb request to display Red EPB tell tale.
Record has checksum and counter */ 
struct EpbLampReq_info : InTag, public DataElemInfo {
    using data_elem_type = EpbLampReqRec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distributes the Drivers request to enable ESC Sport Mode
 */ 
struct EscSptModReqdByDrvr_info : OutTag, public DataElemInfo {
    using data_elem_type = EscSptModReqdByDrvrRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Information on activation/ deactivation of Electronic Stability Control (ESC) */ 
struct EscSt_info : InTag, public DataElemInfo {
    using data_elem_type = EscStSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Esc warning indication request or indication to driver that the Esc function is in active intervention. In case of disabled Esc the warning telltale shall be permanently lit. In case of active intervention indication the telltale will flash. */ 
struct EscWarnIndcnReq_info : InTag, public DataElemInfo {
    using data_elem_type = EscWarnIndcnReqRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Mirror Folding
 */ 
struct ExtrMirrFoldSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Mirror Tilting
 */ 
struct ExtrMirrTiltSetg2_info : OutTag, public DataElemInfo {
    using data_elem_type = ExtrMirrTiltSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Mirror Tinting
 */ 
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
    using data_elem_type = uint16_t; // 0->65535

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Light source failure (open load or short circuit) indication to driver, left front direction indicator. */ 
struct FltIndcrTurnLeFrnt_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Light source failure (open load or short circuit) indication to driver, left rear direction indicator. */ 
struct FltIndcrTurnLeRe_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Light source failure (open load or short circuit) indication to driver, right front direction indicator. */ 
struct FltIndcrTurnRiFrnt_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Light source failure (open load or short circuit) indication to driver, right rear direction indicator. */ 
struct FltIndcrTurnRiRe_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Light source failure (open load or short circuit) indication to driver, right DRL. */ 
struct FltOfLiDaytiRunngRi_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Warning for front axle, 
3 warning status:
No warning (Normal), soft or hard warning. */ 
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


/** 
                Informs if the fuel operated heater is active, this signal is used for ISO symbol in DIM. */ 
struct FuHeatrActv_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Informs of how much fuel the heater has consumed since entering Usage mod driving. Signal is modulus. */ 
struct FuHeatrFuCns1_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 200.0*x+0.0 (0->51000) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Informs how much that have been used by the fuel operated heater when not in usage mode driving. Will reset to 0 when leaving usage mode driving. */ 
struct FuHeatrFuCnsDurgCyc1_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.01*x+0.0 (0->1.27) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicated fuel level in Liter, the value is damped and can differ from the real raw fuel level values. */ 
struct FuLvlIndcd_info : InTag, public DataElemInfo {
    using data_elem_type = FuLvlValWithQly;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal that indicated if the yellow low fuel tell tale is lit */ 
struct FuLvlLoIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of low fuel warnings to navigation system
 */ 
struct FuLvlLoIndcnToNav_info : OutTag, public DataElemInfo {
    using data_elem_type = FuLvlLoIndcnToNav1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal that indicated if the RED low fuel warning LED is lit */ 
struct FuLvlLoWarn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Record contains two signals.
1. Actual gear displayed for the driver
2. Recommended shift up or down to obtain maximal fuel economy and driveability. */ 
struct GearIndcnRec_info : InTag, public DataElemInfo {
    using data_elem_type = GearIndcnRec2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                This signal will initiate global or local reset of functionality within the infotainment area */ 
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


/** 
                Distribution of activation of head rest fold
 */ 
struct HdrestFoldReq2_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal from HMI saying if additional heater is allowed or not. */ 
struct HeatrDurgDrvgReqd_info : OutTag, public DataElemInfo {
    using data_elem_type = OffOnAut1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                A signal indicating if any fault connected to generating sound warnings, cancellation sounds etc is detected. This signal does not cover safety related sound generation. */ 
struct HmiAudSts_info : OutTag, public DataElemInfo {
    using data_elem_type = FltCfmd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Used for Apix 2 protocol from DIM to IHU
 */ 
struct HmiCenForDrvrHmi_info : InTag, public DataElemInfo {
    using data_elem_type = HmiCenForDrvrHmi;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                By user requested air distribution for first and second row */ 
struct HmiCmptmtAirDistbnFrnt_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiCmptmtAirDistbnFrnt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                User request to use AC auto or off */ 
struct HmiCmptmtCoolgReq_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiCmptmtCoolgReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                User requested set temperatures for all available climate zones */ 
struct HmiCmptmtTSp_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiCmptmtTSp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Description added for E4.2
 */ 
struct HmiDefrstElecReq_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiDefrstElecReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Front windscreen, rear window and exterior rear view mirror electrical defroster status as indicated in the HMI (does not reflect when the defrosters have been activated by climate control hidden from driver). Includes information regarding how they have been activated and their availability. */ 
struct HmiDefrstElecSts_info : InTag, public DataElemInfo {
    using data_elem_type = HmiDefrstElecSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Max defroster request by user. */ 
struct HmiDefrstMaxReq_info : OutTag, public DataElemInfo {
    using data_elem_type = ActrReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                User requested fan level for first row. */ 
struct HmiHvacFanLvlFrnt_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiHvacFanLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                User requested fan level for second row */ 
struct HmiHvacFanLvlRe_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiHvacFanLvl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Third row climate activated.  */ 
struct HmiHvacReCtrl_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiHvacReCtrl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Manual recirculation requested by user.  */ 
struct HmiHvacRecircCmd_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiHvacRecircCmd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                HMI/user requests for heating and ventilation of seats for first and second row. */ 
struct HmiSeatClima_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiSeatClima;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                HMI/user requests for ventilation of second row. */ 
struct HmiSeatClimaExtd_info : OutTag, public DataElemInfo {
    using data_elem_type = HmiSeatClimaExtd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                HoodSts - Hood Status

Ukwn - Unknown status
Opend - Opened
Clsd - Closed */ 
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


/** 
                Setting from IHU, indicating whether the driver wishes to activate or deactivate the HUD
 */ 
struct HudActvReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicates the current status of the HUD: whether the HUD is on (displaying image on the windscreen) or not. */ 
struct HudActvSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                
Determine that HUD settings are allowed to be adjusted.
 */ 
struct HudAdjmtReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Diagnostics for HUD.
 */ 
struct HudDiagc_info : InTag, public DataElemInfo {
    using data_elem_type = HudDiagc;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribute that HUD illumination and pictures adjustments are allowed to perform. */ 
struct HudErgoSetgReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicates the current status of the HUD in terms of whether HUD adjustments shall be allowed by the HMI in DIM and/or IHU. The signal shall indicate whether HUD is available, in calibration mode, is faulty (preventing adjustment) or temporarily unavailable. */ 
struct HudSts_info : InTag, public DataElemInfo {
    using data_elem_type = HudStsForHmi;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The settings regarding which functions and features that shall be visible in HUD is contained in this data element.
 */ 
struct HudVisFctSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = HudVisFctSetgReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Estimated total mass air flow in front HVAC.  */ 
struct HvacAirMFlowEstimd_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t; // 1.0*x+0.0 (0->1000) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Electronic Horizon data corresponding to the ADASIS v2 META-DATA message. */ 
struct HznData_info : OutTag, public DataElemInfo {
    using data_elem_type = HznDataGroup2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Electronic Horizon data corresponding to the ADASIS v2 STUB message. */ 
struct HznEdge_info : OutTag, public DataElemInfo {
    using data_elem_type = HznEdgeGroup2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Electronic Horizon data corresponding to the ADASIS v2 POSITION message. */ 
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


/** 
                Electronic Horizon data corresponding to the ADASIS v2 PROFILE LONG message. */ 
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


/** 
                Electronic Horizon data corresponding to the ADASIS v2 PROFILE SHORT message. */ 
struct HznProfSho_info : OutTag, public DataElemInfo {
    using data_elem_type = HznProfSho2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Electronic Horizon data corresponding to the ADASIS v2 SEGMENT message. */ 
struct HznSeg_info : OutTag, public DataElemInfo {
    using data_elem_type = HznSegGroup2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status of the Electronic Horizon Provider, part of the Navigation system.
- The EH function is supported and active and a horizon is provided; HznSplyElectcSts = Runng.
- The EH function is supported but the provider is not active (e.g during startup); HznSplyElectcSts = NotRunng.
- Providing an EH is not supported by the current system configuration or the function is not enabled; HznSplyElectcSts = NoSpprt.
- EH provider status unknown; HznSplyElectcSts = Ukwn. */ 
struct HznSplyElectcSts_info : OutTag, public DataElemInfo {
    using data_elem_type = HznSplyElectcSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicating the status of the PAS function, if the park distance information is on or off. */ 
struct IndcnOfParkAssiSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicating which automatic parking function the driver has activated by the activation interface (button). */ 
struct IndcnOfPrkgAutSts_info : InTag, public DataElemInfo {
    using data_elem_type = IndOfPrkgAutSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Settings for units and formats that shall be used for indication in the vehicle */ 
struct IndcnUnit_info : OutTag, public DataElemInfo {
    using data_elem_type = IndcnUnit;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Controls the Direction Indicator tell-tales and sound to the driver.

Signal will be event driven and holds every flank on and off of the telltales/sound.

This signal will not be used for the Hazard Warning Light Button in instrument panel. */ 
struct IndcrDisp1WdSts_info : InTag, public DataElemInfo {
    using data_elem_type = IndcrSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Provides the state and not the event of HWL/Direction Indicator. */ 
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
    using data_elem_type = uint16_t; // 0->65535

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal that represents the perceived audio noise (in dB RMS over 3 seconds) in the coupé. */ 
struct InsdCarNoiseMeasd_info : OutTag, public DataElemInfo {
    using data_elem_type = double; // 0.5*x+0.0 (0->127.5) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal to trigger sound for ACC Auto Cancel.

Yes = Trigger Sound
No = No sound */ 
struct IntAudCnclWarn_info : OutTag, public DataElemInfo {
    using data_elem_type = YesNo2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Rheostat setting.
Set by user through rheostat wheel situated on MyCar Module. */ 
struct IntrBriSts_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->15

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


struct IntrBriStsForSeatHeatrRe_info : OutTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->15

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Interior Ambient Light
 */ 
struct IntrLiAmbLiSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = IntrLiAmbLiSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Interior "Mood" Light
 */ 
struct IntrLiSurrndgsLiSetgLi_info : OutTag, public DataElemInfo {
    using data_elem_type = IntrLiSurrndgsLiSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting Interior and Exterior Mirror Auto dimming
 */ 
struct IntrMirrTintgSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = MirrDimPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal indicating which mode the driver has selected for the Lane Keeping Aid (LKA) function. */ 
struct IntvAndWarnModForLaneKeepAid_info : OutTag, public DataElemInfo {
    using data_elem_type = WarnAndIntvPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Key lost warning indication for DIM to show a message and for IHU to play a sound. */ 
struct KeyLostWarnIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = KeyLostWarnIndcn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                When the user chooses do disconnect the key from profile in the CSD, this signal group shall be sent with active profile and status "true" */ 
struct KeyProfMpgUpd_info : OutTag, public DataElemInfo {
    using data_elem_type = KeyProfMpgUpd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Handles request for different key searches.
 */ 
struct KeyReadReqFromSetgMgr_info : OutTag, public DataElemInfo {
    using data_elem_type = KeyLocn1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                When a key search is finished and one key is connected to profile (and mapping stored in CEM), this signal shall be updated with "key ID" and status "true" to HMI to be stored in IHU */ 
struct KeyReadStsToProfCtrl_info : InTag, public DataElemInfo {
    using data_elem_type = KeyReadStsToProfCtrl;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Key reminder indication for IHU to play a sound. */ 
struct KeyRmnIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Is used to trig (activate) a warning sound for defined restricted key speed values. When signal = 1 is send to the audio system shall there be ONE warning sound. */ 
struct KeySpdWarn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal to enable the connected service booking to supress the service indicator from the VCM ECU when a booking is accepted and active. */ 
struct LampSuppSrv_info : OutTag, public DataElemInfo {
    using data_elem_type = SrvSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal indicating that the driver has enabled the Lane Departure Warning (LDW) function. */ 
struct LaneChgWarnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Lane Departure Warning function status */ 
struct LaneChgWarnSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Traffic Jam Assist lane detection status. */ 
struct LaneDetnStsForAutDrv_info : InTag, public DataElemInfo {
    using data_elem_type = LaneDetnSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Lane Keep Aid lane detection status. */ 
struct LaneDetnStsForLaneKeepAid_info : InTag, public DataElemInfo {
    using data_elem_type = LaneDetnSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting Lane Keeping Aid.

 */ 
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


/** 
                Lane Keeping Aid function status */ 
struct LaneKeepAidSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                A warning request related to a dangerous lateral movement of the vehicle due to a threat on the left or right side. */ 
struct LatMovmtWarn_info : InTag, public DataElemInfo {
    using data_elem_type = WarnLeRi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Shows if the LCMA function is activated or deactivated */ 
struct LcmaOn1_info : InTag, public DataElemInfo {
    using data_elem_type = LcmaCtraOn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Bending light control to be On or Off
 */ 
struct LiExtReq1WdReq1_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Autolight Transition
 */ 
struct LiExtReq2WdReq1_info : OutTag, public DataElemInfo {
    using data_elem_type = LiAutTranPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for High beam lights
 */ 
struct LiExtReq1WdReq4_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Corner lights
 */ 
struct LiExtReq1WdReq6_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Daytime Running Lamps
 */ 
struct LiExtReq1WdReq2_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Light source failure (open load or short circuit) indication to driver, side direction indicator driver side. */ 
struct LiDrvrFltIndcrTurn_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                External interface for front defroster LED
 */ 
struct LiForBtn4ForUsrSwtPanFrntCmd_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                External interface for rear defroster LED
 */ 
struct LiForBtn5ForUsrSwtPanFrntCmd_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Approach lights
 */ 
struct LiExtSafe1WdReq2_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Home Safe Light
 */ 
struct LiExtSafe1WdReq1_info : OutTag, public DataElemInfo {
    using data_elem_type = LiTiPen2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal used to represent push button switch at end of stalk(TRIP) switch.
0-Not pressed and 1-Pressed  */ 
struct LiLvrSwt1Req_info : InTag, public DataElemInfo {
    using data_elem_type = SwtPush;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Light source failure (open load or short circuit) indication to driver, side direction indicator passenger side. */ 
struct LiPassFltIndcrTurn_info : InTag, public DataElemInfo {
    using data_elem_type = DevErrSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of  DIMON Extra Stimuli setting
 */ 
struct LiSeldForDrvrPfmncMon_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Tourist Lights
 */ 
struct LiTrfcSide1WdReq1_info : OutTag, public DataElemInfo {
    using data_elem_type = LiTrfcSide1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of Speed locking setting
 */ 
struct LockSpdReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal LockgCenSts provides central locking status to other subsystems.

Signal is a record with three items:

- LockSt (Lock State)
- TrigSrc (Trig Source)
- UpdEve (Will be set to TRUE during one second at each new central locking or unlocking event)

The signal shall be interpreted as the latest executed central locking request performed by user or system. Note that individual side doors and tailgate/bootlid may have a lock status which differs from the central locking status. Hence, this signal shall not be used for customer feedback regarding central locking state of the vehicle. */ 
struct LockgCenSts_info : InTag, public DataElemInfo {
    using data_elem_type = LockgCenSts3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                LockgCenStsForUsrFb - Central Locking Status for User Feedback

The signal is used to communicate central locking status to the user of the vehicle

Undefd - ?Undefined status (to be used when sufficient information about door and lock status is not available)
Opend - ?At least one door or taligate/bootlid or hood is open
Clsd (Closed) - ?All doors, tailgate/bootlid and hood are closed (at least one door or tailgate/bootlid is unlocked)
Lockd (Locked) - ?All doors, tailgate/bootlid and hood are closed and all doors and tailgate/bootlid are locked (at least one door is unsafed)
Safe - ?All doors, tailgate/bootlid and hood are closed, tailgate/bootlid is locked and all doors are safed (double locked) */ 
struct LockgCenStsForUsrFb_info : InTag, public DataElemInfo {
    using data_elem_type = LockSt2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of Audible locking feedback setting for individual user profiles.
 */ 
struct LockgFbSoundReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of Visible locking feedback setting for individual user profiles.
 */ 
struct LockgFbVisReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Handles private locking, both unlock and locking mechanism.
 */ 
struct LockgPrsnlReqFromHmi_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffNoReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                LockgPrsnlSts - Private Locking Status

Current private locking status. To be used for user feedback.

On - Private locking is ON
Off - Private locking is OFF */ 
struct LockgPrsnlSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Current Clean Zone level sent to HMI. */ 
struct LvlOfClimaCmft_info : InTag, public DataElemInfo {
    using data_elem_type = LvlOfClimaCmft;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Driver selected Adaptive Cruise Control (ACC) Speed Limit Adaptation (SLA) automatic / semi automatic status. */ 
struct LvlSeldForSpdLimAdpv_info : OutTag, public DataElemInfo {
    using data_elem_type = LvlSeldForSpdLimAdpvPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distributing that the massage menu view is activated.
 */ 
struct MassgFctActv_info : OutTag, public DataElemInfo {
    using data_elem_type = MassgFctActv;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                When an ergonomic setting is saved this shall be acknowledged via a sound. This signal is the trigger. */ 
struct MemBtnSound_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Reports the status of the mirror dipping mode. Driver side. */ 
struct MirrDwnStsAtDrvr_info : InTag, public DataElemInfo {
    using data_elem_type = MirrDwnStsTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Reports the status of the mirror dipping mode. Passanger side. */ 
struct MirrDwnStsAtPass_info : InTag, public DataElemInfo {
    using data_elem_type = MirrDwnStsTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Reports the fold status of the outer rear view mirrors. Driver side. */ 
struct MirrFoldStsAtDrvr_info : InTag, public DataElemInfo {
    using data_elem_type = MirrFoldStsTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Reports the fold status of the outer rear view mirrors. Passanger side. */ 
struct MirrFoldStsAtPass_info : InTag, public DataElemInfo {
    using data_elem_type = MirrFoldStsTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Multimedia Head Power Mode
Power State of the IHU.
The states are described in the data type. */ 
struct MmedHdPwrMod_info : OutTag, public DataElemInfo {
    using data_elem_type = MmedMaiPwrMod;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Multimedia Hmi Mode Standard
This signal displays the currently used Infotainment mode of the Infotainment system. The modes are described in the Infotainment Mode Manager
 */ 
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
    using data_elem_type = uint16_t; // 0->65535

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Compartment Material temperature sensor */ 
struct MtrlSnsrT_info : InTag, public DataElemInfo {
    using data_elem_type = MtrlSnsrT;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Network ControllerActivate:
Wakeup signal from the VCM, with the purpose of waking up other infotainment nodes when a function on that node is requested.
Bit 1: Prio high/normal
Bit 2-5: Requested function number (Function no 0-15) */ 
struct NetCtrlrActvt_info : InTag, public DataElemInfo {
    using data_elem_type = NetActvtRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Network Head Activate:
Wakeup signal from the Inftotainment HEAD Unit (IHU), with the purpose of waking up other infotainment nodes when a function on that node is requested.
Bit 1: Prio high/normal
Bit 2-5: Requested function number (Function no 0-15) */ 
struct NetHdActvt_info : OutTag, public DataElemInfo {
    using data_elem_type = NetActvtRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Network ControllerActivate:
Wakeup signal from the TEM, with the purpose of waking up other infotainment nodes when a function on that node is requested.
Bit 1: Prio high/normal
Bit 2-5: Requested function number (Function no 0-15) */ 
struct NetTelmActvt_info : InTag, public DataElemInfo {
    using data_elem_type = NetActvtRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                True (Trig) if a new trip has been started, i.e. the car has been abandoned long enough for triggering a reset. */ 
struct NewTripCdn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                No entrance warning. */ 
struct NoEntryWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = Warn2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal to set DIM message that no sound system available.

FALSE = Sound OK
TRUE = No Sound = Trigger DIM message */ 
struct NoSoundSys_info : OutTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Contains notification number used in Check view
 */ 
struct NotifChkDistbn_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t; // 0->65535

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Number of belts that is currently used in the vehicle. Originates from LC: ExtSigIf1. */ 
struct NrOfBltAppld_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->7

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                When a key search is performed (key-proofile mapping), if more than one key is found this signal is set to "true" to HMI M&D */ 
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


/** 
                Speed Alert Offset Menu Setting. Offset to speed limit when the driver shall get a speed alert warning. Unit km/h or mph depending on the setting in the car. */ 
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


/** 
                Distribution of the setting temporary deactivation of passive arming

 */ 
struct PasAlrmDeactvnReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal is used to request HMI to re-activate Pasive Arming in user settings when the 24h disabling timer times out. */ 
struct PasAlrmSts_info : InTag, public DataElemInfo {
    using data_elem_type = NoReqOffReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                When the massage system has been activated it will stay on for approximately 10 minutes. A signal Driver/Passenger Massage Running {On, Off} shall be send On to HMI if system running (Signal names: DrvrMassgRunng/PassMassgRunng). */ 
struct PassMassgRunng_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The PSMD/PSMP multifunction switch consists of a 4-way adjustment switch and a 2-way integrated switch. By pressing the integrated switch up or down, the next supported seat function shall be activated. Information of which function that is currently active shall be transmitted from the seat Control subsystem ECU's via the signals DrvrSeatActvSpplFct and PassSeatActvSpplFct to the subsystem Hmi managagement & control.
 */ 
struct PassSeatActvSpplFct_info : InTag, public DataElemInfo {
    using data_elem_type = SeatActvSpplFct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distributes massage settings for the passenger
 */ 
struct PassSeatDispMassgFct_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatMassgFct;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Handles the two way communication for passenger multi seat control.
 */ 
struct PassSeatDispSpplFct_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatActvSpplFct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                To maneuver the driver and passenger front seat massage system the customer shall be able to access the system functions via both the so called multifunction switch on the Seat and via the IP touch screen. The DrvrSeatMassgFct/PassSeatMassgFct signal will contain data for navigating in the massage menu in the CSD. */ 
struct PassSeatMassgFct_info : InTag, public DataElemInfo {
    using data_elem_type = SeatMassgFct;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Occupation status of the front passenger seat. */ 
struct PassSeatSts_info : InTag, public DataElemInfo {
    using data_elem_type = PassSeatSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Passenger Seat Switch Status */ 
struct PassSeatSwtSts2_info : InTag, public DataElemInfo {
    using data_elem_type = PassSeatSwtSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Calculated pinion steer angle at the front steering device */ 
struct PinionSteerAg1_info : InTag, public DataElemInfo {
    using data_elem_type = PinionSteerAg1Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Position from Navigation
Position of vehicle calculated by the navigation system by use of GNSS and/or dead reckoning.
 */ 
struct PosnFromNav_info : OutTag, public DataElemInfo {
    using data_elem_type = PosnFromNav;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                GNSS positioning data provided by the VCM GNSS receiver.
Data includes vehicle position (lat, long, altitude), vehicle movement (drection, speed, vertical and horizontal velocity), UTC time, positioing accuracy (DOP values) and positining solution information (number of satellites, etc). */ 
struct PosnFromSatlt_info : InTag, public DataElemInfo {
    using data_elem_type = PosnFromSatlt;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Informs if post climatisation is available. */ 
struct PostDrvgClimaAvl_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal for requesting post conditioning. */ 
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


/** 
                Indication of failure in the Park assist system.  */ 
struct PrkgAssiFailr_info : InTag, public DataElemInfo {
    using data_elem_type = PrkgAssiFailr2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicating that the autonomous parking system currently is in an active parking maneuver.
 */ 
struct PrkgAssiManvActvSts_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Information such that a progress bar can be created and shown to the driver. The progress bar represent the distance to next action needed by the driver.
 */ 
struct PrkgAssiManvProgs_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->63

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicating which sensors are active. */ 
struct PrkgAssiSts_info : InTag, public DataElemInfo {
    using data_elem_type = PrkgAssiSts3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Show the status of the Autonomous parking maneuver. */ 
struct PrkgAutSts_info : InTag, public DataElemInfo {
    using data_elem_type = PrkgAutSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Parking camera system availability status. Set by WAM (PAC) to indicate functional status. */ 
struct PrkgCamSysAvlSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal sent to CEM that the user has confirmed that the park-out maneuver is about to begin. */ 
struct PrkgOutCfm_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicate which parking type that has been detected by the autonomous parking system. */ 
struct PrkgTypVld_info : InTag, public DataElemInfo {
    using data_elem_type = PrkgTypVld1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal communicates which profiles that shall be active in the HMI */ 
struct ProfAct_info : InTag, public DataElemInfo {
    using data_elem_type = ProfAct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                distribution of Change of user profile
 */ 
struct ProfChg_info : OutTag, public DataElemInfo {
    using data_elem_type = IdPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal communicates which profiles that are restricted (feature Restricted Key). */ 
struct ProfLimd_info : InTag, public DataElemInfo {
    using data_elem_type = ProfLimd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal communicates which profile that is active in the car (Personal settings). */ 
struct ProfPenSts1_info : InTag, public DataElemInfo {
    using data_elem_type = IdPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Actual Mode for Propulsion HV Battery usage mode, including Hold, Smart Hold and Charge */ 
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


/** 
                The propulsion HV Battery usage mode CHARGE is blocked/unavailable */ 
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


/** 
                The propulsion HV Battery usage mode HOLD is blocked/unavailable */ 
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


/** 
                The propulsion HV Battery usage mode SMART HOLD is blocked/unavailable */ 
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
    using data_elem_type = uint8_t; // 0->150

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


/** 
                Propulsion mode PURE ( i.e. Electric Drive) is not available.
 */ 
struct PrpsnModElecDrvBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Propulsion mode SAVE is not available. */ 
struct PrpsnModOfSaveBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Propulsion mode AWD (i.e. Traction) is not available. */ 
struct PrpsnModOfTracBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Propulsion mode XC ( i.e. Offroad) is not available.  */ 
struct PrpsnModOffroadBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Propulsion mode POWER ( i.e. Sport) is not available. */ 
struct PrpsnModSptBlkd_info : InTag, public DataElemInfo {
    using data_elem_type = Typ1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Acual crank shaft torque. */ 
struct PtCluTq_info : InTag, public DataElemInfo {
    using data_elem_type = PtCluTq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Powertrain charactaristics in Drive Mode Individual
 */ 
struct PtDrvrSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = PtDrvrSetg2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Target gear.
TArget gear is for most cases identical to actual gear. The value differs to actual gear during gear shifts and for a hybrid when in electric mode. */ 
struct PtGearTar_info : InTag, public DataElemInfo {
    using data_elem_type = PtGearAct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Actual torque delivered by the powertrain to the front axle (torque before propshaft to rear subtracts torque from front).  */ 
struct PtTqAtWhlFrntAct_info : InTag, public DataElemInfo {
    using data_elem_type = PtTqAtWhlFrntActRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Sending power connection status to DIM and IHU.
True==Connected
False==Not Connected */ 
struct PwrChrgDetdForPrkgHeatrElec_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status of Power supply system
0: No fault
1; Alternator mechanical fault
2: Alternator electrical fault
3: Alternator temperature fault
4: Alternator communication fault
5: Over voltage during driving
6: Battery switch communication fault
7: Battery switch HW fault
8: Low voltage during driving
9: Battery sensor communication fault
10: Battery sensor HW fault
11: Support battery charging fault
12: DcDc communication fault
13: DcDc electrical fault
14: DcDc temperature fault

 */ 
struct PwrSplyErrSts_info : InTag, public DataElemInfo {
    using data_elem_type = PwrSplyErrSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determines if the rainsensor is memorized in the next driving cycle or not.

 */ 
struct RainSenMemdReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Warning for rear axle, 
3 warning status:
No warning (Normal), soft or hard warning. */ 
struct ReAxleWarn_info : InTag, public DataElemInfo {
    using data_elem_type = AxleWarn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                RmnLockgPrsnlReq - Request for Private Locking Reminder

When private locking is ON, and the user tries to open the glove box or trunk via instrument panel button, a private locking reminder shall be shown to the user via HMI. The reminder shall be shown when this signal changes value from Off to On. */ 
struct RmnLockgPrsnlReq_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Running Board Illumination
 */ 
struct RngbdIllmnCmd_info : OutTag, public DataElemInfo {
    using data_elem_type = RngBdIllmnCmdPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the Road Friction Information (RFI) setting
 */ 
struct RoadFricIndcnActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Road Friction Indication function status. */ 
struct RoadFricIndcnSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Road friction status to inform the driver about present road friction. */ 
struct RoadFricWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = RoadFricWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for the function Traffic Sign Information to be ON or OFF
 */ 
struct RoadSgnInfoActv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Road Sign Information function status. */ 
struct RoadSgnInfoSts_info : InTag, public DataElemInfo {
    using data_elem_type = FctSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting Road Speed Limit 
 */ 
struct RoadSpdLimActvSts_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Periodic signal on LIN 19, shows direction of SBSM knob(forward/backward, idle and Spare).
The signal RotyDirReq2 shall reflect the last directionof the SBSM rotary knob.
The signal shall be provided when the SBSM is ON.
The signal value Idle shall be default before any movement have been done after POR.
?0-?Idle,1-?Forward,?2-?Backward,?3-?Spare
The signal value Failr shall be set when no direction info is availible 
User input generic requirement for turn knobs "Req. 199813 Signaling Rotary switch absolute postion"
 */ 
struct RotyDirReq2_info : InTag, public DataElemInfo {
    using data_elem_type = RotyDirUI;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status signal of rotary knob DIRECTION sent periodically. 
0- ?Idle, ?1- ?Clockwise(CW), ?2- ?Counter clockwise(CCW), ?3- ?Spare */ 
struct RotyDirReq1_info : InTag, public DataElemInfo {
    using data_elem_type = RotyDirUI1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The signal 'RotyPosReq2' shall reflect the absolute postion of the SBSM rotary knob and be sent periodically on LIN 19.
Bit0 to bit4 shall hold the absolute position of the switch since start-up.
Bit0 to bit4 has initial- and minimum value 0 and maximum value 31. 
At start up of LIN communication the initial/default value of 'RotyPosReq' is 0x00000
When the knob is activated following decoding shall be done by XX:
If the knob is turned forward --> Then bit0 to bit4 shall shall decrease by 1 for every step turned.
If the knob is turned backward --> Then bit0 to bit4 shall shall increase with 1 for every step turned.
 */ 
struct RotyPosReq2_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->31

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The signal 'RotyPosReq1' shall be sent by the XX on LIN Y to the master Z.
Bit0 to bit4 shall hold the absolute position of the switch since start-up.
Bit0 to bit4 has initial- and minimum value 0 and maximum value 31. 
At start up of LIN communication the initial/default value of 'RotyPosReq' is 0x00000

When the knob is activated following decoding shall be done by XX:
If the knob is turned clock wise (CW) --> Then bit0 to bit4 shall shall decrease by 1 for every step turned.
If the knob is turned counter clock wise (CCW) --> Then bit0 to bit4 shall shall increase with 1 for every step turned.
(example see req-266313) */ 
struct RotyPosReq1_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->31

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status signal which indicates the status of the different state machines. */ 
struct RsdsSysStsLe_info : InTag, public DataElemInfo {
    using data_elem_type = RsdsSysSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status signal which indicates the status of the different state machines. */ 
struct RsdsSysStsRi_info : InTag, public DataElemInfo {
    using data_elem_type = RsdsSysSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal for personal settings saving concept

0: Profile locked, no saving to be performed
1: On the event (toggle 0 to 1) settings shall be saved (manually saving)
2: Automatic saving */ 
struct SaveSetgToMemPrmnt_info : OutTag, public DataElemInfo {
    using data_elem_type = OffOnAut1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                This signal is sent from Engine to HMI management and display containing information regarding tank level indication to driver.

The signal is meant to be used showing tank level in IHU in 25% steps (100%, 75%, 50%, 25% and 0%).

The signal is sent from Engine in 5% steps but are visualized in IHU according to above information. */ 
struct ScrBarVolIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 5.0*x+0.0 (0->100) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


struct ScrMaxFillgVol_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->31

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


struct ScrReagentTankVol_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 0->31

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status flag from the LIN slave for third row fold function to indicate that Seat backrest is Unlocked in Unfolded position.

 */ 
struct SeatBackUnlckdThrd_info : InTag, public DataElemInfo {
    using data_elem_type = SeatBackUnlckd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of when the multi function menue is accesed on the IHU.
 */ 
struct SeatDispBtnPsd_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatDispBtnPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal for deciding wether seatclimatisation shall be enabled during pre-conditioning (in hybrids). */ 
struct SeatHeatDurgClimaEnad_info : OutTag, public DataElemInfo {
    using data_elem_type = SeatHeatDurgClimaEnad;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal sent to HMI when conditions for Auto seat heating is true. When signal is true seat heating will be set to auto level from HMI (user selection). Signal will be true until conditions is no longer met, and seat heating will then be deactivated by HMI. Signal is used for auto of both driver and paseenger front seat heating and also for steering wheel heating.  */ 
struct SeatHeatgAutCdn_info : InTag, public DataElemInfo {
    using data_elem_type = Flg1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Seat Switch Left Signal Third
 */ 
struct SeatSwtLeSigThrd_info : OutTag, public DataElemInfo {
    using data_elem_type = SwtVertSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Seat Switch Right Signal Third
 */ 
struct SeatSwtRiSigThrd_info : OutTag, public DataElemInfo {
    using data_elem_type = SwtVertSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal sent to HMI when conditions for Auto seat ventilation is true. When signal is true saved seat ventilation level in the setting will be indicates . Signal will be true until conditions is no longer met, and seat ventilation will then be deactivated by HMI. Signal is used only for driver side.  */ 
struct SeatVentnAutCdn_info : InTag, public DataElemInfo {
    using data_elem_type = Flg1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting for default language of the car
 */ 
struct SetOfLang_info : OutTag, public DataElemInfo {
    using data_elem_type = SetOfLang;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Confirmation of setting the language, used in order to have synchronisation between IHU and DIM.
 */ 
struct SetOfLangAck_info : InTag, public DataElemInfo {
    using data_elem_type = FailrNoFailr1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal between the infotainment system and the CEM that will be used for update the clock and date calculation data in CEM. See requirement for better understanding of the clock settings signals. */ 
struct SetTiAndDate_info : OutTag, public DataElemInfo {
    using data_elem_type = DateTi1ForSet;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Record with safety signals from Audio. */ 
struct SftySigGroupFromAudSafe_info : OutTag, public DataElemInfo {
    using data_elem_type = SftySigGroupFromAudSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to activate Autonomous Drive Drive Steering Override warning */ 
struct SftyWarnGroupFromAsySafe_info : InTag, public DataElemInfo {
    using data_elem_type = SftyWarnGroupFromAsySafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Sensor distance data from the six front park assist sensors and audio request direction.
 */ 
struct SnsrParkAssiFrnt_info : InTag, public DataElemInfo {
    using data_elem_type = SnsrParkAssi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Sensor distance data from the six rear park assist sensors and audio request direction.
 */ 
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


/** 
                The actual speed of the ERAD electric motor scaled to the wheel domain. */ 
struct WhlMotSysSpdAct_info : InTag, public DataElemInfo {
    using data_elem_type = double; // 0.1*x+-1638.4 (-1638.4->1638.3) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Speed Limit warning (i.e. Speed Alert) to be ON or OFF
 */ 
struct SpdAlrmActvForRoadSgnInfo_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal that indicates if Traffic Sign Information (TSI) Speed Camera Information is turned on by driver. */ 
struct SpdCameraInfoSeldForRoadSgnInfo_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Speed limit adaption status. */ 
struct SpdLimAdpvSts_info : InTag, public DataElemInfo {
    using data_elem_type = OffOnNotAvlSrvRqrd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to activate warning for too high speed compared to current speed limit */ 
struct SpdLimWarnReq_info : InTag, public DataElemInfo {
    using data_elem_type = SpdLimWarnReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                This signal will trig (activate) a warning sound when the indicated vehicle speed is equal to 120km/h or speed waning values defined by restricted key. When signal = 1 is send to the audio system shall there be ONE warning sound. */ 
struct SpdWarn_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                This signal indicates (during about 10sec) when a reste of the service reminder is done in instrument cluster */ 
struct SrvRst_info : InTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Information that a service reminder message is shown in the instrument cluster. Gives information of which parameter that trigged the message and which service reminder message that is shown. */ 
struct SrvTrig_info : InTag, public DataElemInfo {
    using data_elem_type = uint8_t; // 1.0*x+0.0 (0->15) raw is unsigned

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Autonomous drive steering assistance availablility setting selected by driver. */ 
struct SteerAssiActvForAutDrv_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Autonomous drive steering level setting selected by driver. */ 
struct SteerAssiLvlForAutDrv_info : OutTag, public DataElemInfo {
    using data_elem_type = SteerAssiLvlForAutDrvPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Disitribution of Steering characteristics settings
 */ 
struct SteerSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = SteerSetg;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to inform driver that manual steering left/right is necessary to keep vehicle on roadway. */ 
struct SteerWarnReqForAutDrv_info : InTag, public DataElemInfo {
    using data_elem_type = SteerWarnReqForAutDrv1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                A warning request to inform the driver that the vehicle is about to depart from the lane. */ 
struct SteerWarnReqForLaneKeepAid_info : InTag, public DataElemInfo {
    using data_elem_type = WarnLeRi;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Enable signal for steering wheel heating during pre-conditioning. */ 
struct SteerWhlHeatgDurgClimaEnad_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Request to start the steering wheel heating at a certain heat level */ 
struct SteerWhlHeatgOnReq_info : OutTag, public DataElemInfo {
    using data_elem_type = SteerWhlHeatgOnCmdTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Information about if it is OK to start the steering wheel heating automatically */ 
struct SteerWhlHeatgStrtAutCdnOk_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Actual steering wheel angle from the actual centre position of the steering wheel. The signal value is a signed value from zero (at centre steering wheel position) where a left turn (counterclockwise from steering wheel centre position) is represented by positive values and right turn (clockwise from steering wheel centre position) will have negative value.  */ 
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


/** 
                Stop Start inhibit reason, displayed to driver in IHU.

Bit 0: Electrical system
Bit 1: Climate system
Bit 2: Engine system
Bit 3: Chassi system
Bit 4: Environmental
Bit 5: Ambient temperature
Bit 6: High altitude
Bit 7: Inclination
Bit 8: Driver active
Bit 9: Driver seatbelt
Bit 10: Driver door open
Bit 11: Hood ajar
Bit 12: Steering
Bit 13: Queue assist
Bit 14: Trailer connected
Bit 15: Driver deselected
Bit 16: Service
Bit 17: TBD
Bit 18: TBD
Bit 19: TBD
Bit 20: TBD
Bit 21: TBD
Bit 22: TBD
Bit 23: TBD */ 
struct StopStrtInhb_info : InTag, public DataElemInfo {
    using data_elem_type = uint32_t; // 0->16777215

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicating to HMI that a driver start is initiated  */ 
struct StrtInProgs_info : InTag, public DataElemInfo {
    using data_elem_type = StrtInProgs1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                The position of the sun roof panel */ 
struct SunRoofPosnSts_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Leveling Control and Dampening Control charactaristics for Suspension
 */ 
struct SuspSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = SuspSetgRec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status signal 'SwtAcptReq' of DMSM "accept" push button shall be sent periodically on LIN 19.
This status signal shall reflect the status of the DMSM accept push button.
0-not pressed, 1-Pressed

 */ 
struct SwtAcptReq_info : InTag, public DataElemInfo {
    using data_elem_type = PsdNotPsd;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status if any of the center console buttons is used. */ 
struct SwtAtCenSts_info : OutTag, public DataElemInfo {
    using data_elem_type = Trig1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Passenger airbag status, based on PACOS input. */ 
struct SwtForPassAirbCutOffSt_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                TankFlapSts -Tank Flap Status

Ukwn - Unknown status
Opend - Opened
Clsd - Closed */ 
struct TankFlapSts_info : InTag, public DataElemInfo {
    using data_elem_type = DoorSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Inidactes the current telematic/subscription status and telematic system error indication. */ 
struct TelmSts_info : InTag, public DataElemInfo {
    using data_elem_type = TelmSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Current time (Hr, Min and Sec) and date (Year, Months and days) data for indication. See requirement for more information about the signals. */ 
struct TiAndDateIndcn_info : InTag, public DataElemInfo {
    using data_elem_type = DateTi30;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                UTC Time Correction Data from Navigation map data.
- Availability of this functionality is based on map data coverage and current vehicle position and is indicated by the SpprtForFct parameter.
- TiCorrnByMapData is provided interally within the IHU.

Example 1
The system has no map data, the vehicle is located in area without map data for time correction or the vehicle position is unknown:
SpprtForFct = No
Other parameter values N/A.

Example 2
Vehicle is identified by map data as located within the UTC - 3:30 time zone (e.g easten part of Canada). Daylight saving time of 1 h currently apply:
SpprtForFct = Yes
HrCorrn = -3
MinsCorrn = -30
DayLiSaveTi = 1

Example 3
Vehicle is identified by map data as located within the UTC + 2:00 time zone (e.g Finland). No daylight saving time currently apply:
SpprtForFct = Yes
HrCorrn = 2
MinsCorrn = 0
DayLiSaveTi = 0 */ 
struct TiCorrnByMapData_info : OutTag, public DataElemInfo {
    using data_elem_type = TiCorrn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Driver selected Adaptive Cruise Control (ACC) time gap as adaptive or manual */ 
struct TiGapAdpvSeld_info : OutTag, public DataElemInfo {
    using data_elem_type = TiGapAdpvSeldPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Time gap setting for ACC(Adaptive Cruise Control) in restricted mode
 */ 
struct TiGapLimd_info : OutTag, public DataElemInfo {
    using data_elem_type = TiGapPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Tire Circumference Calculated by the Navigation system.
- The calculated tire circumference in mm; TireCircum
- When there is no GNSS receiver fitted; SpprtForFct = No
- When SpprtForFct = Yes and the calibration has reached a stable circumference value; TireCircumCalByNav.HiQly = Yes

 */ 
struct TireCircumCalByNav_info : OutTag, public DataElemInfo {
    using data_elem_type = TireCircumCalByNav;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Used to request intiation of calibration routine for TPMS
 */ 
struct TireMonCalReq_info : OutTag, public DataElemInfo {
    using data_elem_type = StrtStopReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for TPMS to be displayed to user in DIM
 */ 
struct TireMonrDispReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Contains the absolut pressure value i Hecto Pascal, Status of the value, if the value is current, old or system is updating the value and inform sensor fault.POsiitioning information (If positioning OK/ NotOK or system without positioning) */ 
struct TirePAbsltValFrntRi_info : InTag, public DataElemInfo {
    using data_elem_type = TirePAbsltValFrntRi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Contains the absolut pressure value i KPascal, Status of the value, if the value is current, old or system is updating the value and inform sensor fault.POsiitioning information (If positioning OK/ NotOK or system without positioning) */ 
struct TirePAbsltValFrntLe_info : InTag, public DataElemInfo {
    using data_elem_type = TirePAbsltValFrntLe3;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Contains the absolut pressure value i Kilo Pascal (KPa) , Status of the value, if the value is current, old or system is updating the value and inform sensor fault.POsiitioning information (If positioning OK/ NotOK or system without positioning) */ 
struct TirePAbsltValReLe_info : InTag, public DataElemInfo {
    using data_elem_type = TirePAbsltValReLe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Contains the absolut pressure value i Hecto Pascal, Status of the value, if the value is current, old or system is updating the value and inform sensor fault.POsiitioning information (If positioning OK/ NotOK or system without positioning) */ 
struct TirePAbsltValReRi_info : InTag, public DataElemInfo {
    using data_elem_type = TirePAbsltValReRi1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                to send calibration status to HMI.
Inform if no calibration (normal), Calibration ongoing or calibration is complated.
 */ 
struct TirePCalSts_info : InTag, public DataElemInfo {
    using data_elem_type = TirePCalSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Data about Tire pressure  */ 
struct TirePMonData_info : InTag, public DataElemInfo {
    using data_elem_type = TirePMonData1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicates if the vehicle is entering or exiting a tunnel. */ 
struct TnlEntryOrEnd_info : InTag, public DataElemInfo {
    using data_elem_type = NoYes1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                TrSts - Tailgate/Bootlid Status

Ukwn - Unknown status
Opend - Opened
Clsd - Closed */ 
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


/** 
                Selects type of trip meter for indication:
Trip1 - Reset by driver
Trip2 - Reset by timer (new trip condition) */ 
struct TripModSeln_info : OutTag, public DataElemInfo {
    using data_elem_type = TripModSeln;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Information to HMI,Trailer Lamp check possible/not possible to execute */ 
struct TrlrLampChkSts1WdSts1_info : InTag, public DataElemInfo {
    using data_elem_type = TrlrLampChkSts1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting automatic trailer lamp check
 */ 
struct TrlrLampChkAutReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of manual trailer lamp check activation
 */ 
struct TrlrLampChkRemReq_info : OutTag, public DataElemInfo {
    using data_elem_type = StrtStopReq1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Status information about Trailer Lamp Check, (Started, Completed) */ 
struct TrlrLampChkSts1WdSts2_info : InTag, public DataElemInfo {
    using data_elem_type = TrlrLampChkSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                status containing information about if a trailer is connected to the vehicle or not */ 
struct TrlrPrsntSts1WdSts_info : InTag, public DataElemInfo {
    using data_elem_type = TrlrPrsntSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Transmission Gear Actual. Actual gear engaged sent by transmission supplier.  */ 
struct TrsmGearAct_info : InTag, public DataElemInfo {
    using data_elem_type = PtGearAct1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Triple Turn Signal (also known as Comfort turn signal) */ 
struct LiExtReq1WdReq3_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Outside ambient light measured by SUS and sent to CEM as lux value. */ 
struct TwliBriRaw_info : InTag, public DataElemInfo {
    using data_elem_type = TwliBriRaw;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Day/night status signal. Lux value (not this signal) sent from SUS to CEM and CEM sets day or night signal. */ 
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


/** 
                Encryption request to FCD slave ECU. Scrambled and indiscernable outside ECU. */ 
struct UkwnCptReqToInfoMgr_info : InTag, public DataElemInfo {
    using data_elem_type = UInt64Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Encryption request to FCD slave ECU. Scrambled and indiscernable outside ECU. */ 
struct UkwnCptReqToSoundMgr_info : InTag, public DataElemInfo {
    using data_elem_type = UInt64Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Encryption response from FCD slave ECU. Scrambled and indiscernable outside ECU. */ 
struct UkwnCptRespFromInfoMgr_info : OutTag, public DataElemInfo {
    using data_elem_type = UInt64Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Encryption response from FCD slave ECU. Scrambled and indiscernable outside ECU. */ 
struct UkwnCptRespFromSoundMgr_info : OutTag, public DataElemInfo {
    using data_elem_type = UInt64Rec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of setting for Visible unlocking feedback

 */ 
struct UnlckFbVisReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of setting for keyless unlocking via HMI when vehicle is equipped with keyless entry system,
 */ 
struct UnlckKeylsReq_info : OutTag, public DataElemInfo {
    using data_elem_type = UnlckKeylsCfgPen2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of setting Remote unlocking
 */ 
struct UnlckRemReq_info : OutTag, public DataElemInfo {
    using data_elem_type = UnlckRemCfgPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of max speed limit to powertrain when restricted key is used.
 */ 
struct UsrSetSpdForKeySpdLimn_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSetSpdForKeySpdLimn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting speed alert, when restricted key is used is the setting not possible to change.
 */ 
struct UsrSetSpdForKeySpdWarn_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSetSpdForKeySpdWarn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of maximum volume for when restricted key is used. 
 */ 
struct UsrSetVolMaxForKeyVolLimn_info : OutTag, public DataElemInfo {
    using data_elem_type = UsrSetVolMaxForKeyVolLimn;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution whether user interface for second row climate shall be enabled
 */ 
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


/** 
                Sending function status to rear seat switch module, among other: rear and left temperature.
 */ 
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


/** 
                Sending configuration status to rear seat switch module, involves both unit and on/off status
 */ 
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


/** 
                Present vehicle voltage measured by CEM with QF.

QF values:
3: Value Reliable
2: Value from back up sensor
1: Init
0: Value not reliable */ 
struct VehBattU_info : InTag, public DataElemInfo {
    using data_elem_type = SysU;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                VehicleConfigurationParameters: Car Config container signal. 
Values of Car Config parameters #1 - #504 are mapped into 72 blocks. 
Each block is published in this signal using an 8-byte record: Block counter (MSB) + 7 parameter value byte positions.
Block counter runs through all 72 blocks in a sequential loop (normal operation). 
When transmitted, block 1 will contain current values on Car Config parameters #1 - #7, block 2 -> #8 - #14 ... block 72 -> #498 - #504.
Any faulty parameter value is indicated using 255. */ 
struct VehCfgPrm_info : InTag, public DataElemInfo {
    using data_elem_type = VehCfgPrm;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Determine the setting for Brake charactaristics in Drive Mode Individual
 */ 
struct VehDecelCtrlSetg_info : OutTag, public DataElemInfo {
    using data_elem_type = NormSptPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Estimated vehicle mass including trailer. */ 
struct VehMNom_info : InTag, public DataElemInfo {
    using data_elem_type = VehMNomRec1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                 VehModMngtGlbSafe1  is a system signal group sent globally in an end to end protected record that consists of  CarModSts1, CarModSubtypWdCarModSubtyp, UsgModSts,  EgyLvlElecMai,  EgyLvlElecSubtyp,  PwrLvlElecMai, PwrLvlElecSubtyp and FltEngyCnsWdSts

CarModSts1 : Factory, Transport, Dyno, Normal. Crash 

UsgModSts : Abandoned, Inactive, Convenience, Active, Driving

CarModSubtypWdCarModSubtyp: examples for CarModSubtyp: Factory Driving, Transport Driving */ 
struct VehModMngtGlbSafe1_info : InTag, public DataElemInfo {
    using data_elem_type = VehModMngtGlbSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Vehicle motion state information based on wheel speed sensors. Provides information about vehicle stand still and vehicle rolling direction. */ 
struct VehMtnStSafe_info : InTag, public DataElemInfo {
    using data_elem_type = VehMtnStSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Boolean that specifies if there should audio warning when Driver attempts to leave the car when gear is not in P */ 
struct AudWarn_info : InTag, public DataElemInfo {
    using data_elem_type = bool;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicated Average Speed (trip computer function AVS) with current unit.
There are two versions of the trip computer function AFS, where version 1 is a mean value that can be resetted by the driver and version 2 is a mean value which is automatically resetted by the system if the vehicle hasn't been used for less then 4 h.
So the data container signal (VehSpdAvgIndcd.VehSpdAvg) will flip it's MSB to be able to send both version 1 and version 2 of the data. So when bit 8 = 0 then AFS value for version 1 is transmitted and when bit 8 = 1 then AFS value for version 2 is transmitted. */ 
struct VehSpdAvgIndcd_info : InTag, public DataElemInfo {
    using data_elem_type = VehSpdIndcd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting Vehicle Speed Control On/Off Status
 */ 
struct VehSpdCtrlActvSts_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Indicated (filtered) vehicle speed value with the current vehicle speed unit. */ 
struct VehSpdIndcd_info : InTag, public DataElemInfo {
    using data_elem_type = VehSpdIndcd1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Vehicle speed longitudinal based on wheel speed sensors and longitudinal acceleration.
end2endprotection=profile1A dataid="12345" */ 
struct VehSpdLgtSafe_info : InTag, public DataElemInfo {
    using data_elem_type = VehSpdLgtSafe1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                VehicleIdentificationNumber: The Vin signal data is divided into 3 separate blocks that are transmitted in a sequential loop, 
using an 8-byte record: Block counter (MSB) + 7 character/ASCII byte positions.
When transmitted, block 1 will contain current VIN characters #1 - #7, block 2 -> #8 - #14, block 3 -> #15 - #17.
Any fault is indicated using 255. */ 
struct Vin_info : InTag, public DataElemInfo {
    using data_elem_type = Vin1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Used to request graphical overlays and trajectory guidelines for PAC
 */ 
struct VisnAgWideCfg_info : OutTag, public DataElemInfo {
    using data_elem_type = VisnAgWideCfg1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Wide Angle vision Module (WAM) status. Signal used to indicate the status of WAM (PAC).
Note: Signal replaced by PrkgCamSysAvlSts from E3 in SPA. */ 
struct VisnAgWideSts_info : InTag, public DataElemInfo {
    using data_elem_type = AvlSts2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Wide Angle vision Module (WAM) view in use. Acknowledgement signal provided by WAM (PAC) to indicate the view currently provided over the video interface.
Signal coding according to WAM (PAC) SWRS. */ 
struct VisnImgAgWideInUse_info : InTag, public DataElemInfo {
    using data_elem_type = VisnImgAgWideInUse;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Used to request different camera views

Logical Value	Autosar Naming (Short name)		System			Description
0		NoImg				All			No Image
1		Img1				PAC360			Rear View + Rear Junction View
2		Img2				PAC360			Zoomed Rear View + Rear Junction View
3		Img3				PAC360 or Front Cam	Front View + Front Junction View
4		Img4				PAC360 or Front Cam	Zoomed Front View + Front Junction View
5		Img5				PAC360			Left Side View
6		Img6				PAC360			Right Side View
7		Img7				PAC360			360 View
8		Img8				Rear Cam			Rear View
9		Img9				Not Used			Not Used
10		Img10				Not Used			Not Used
11		Img11				Not Used			Not Used
12		Img12				Not Used			Not Used
13		Img13				Not Used			Not Used
14		Img14				Not Used			Not Used
15		Img15				Not Used			Not Used */ 
struct VisnImgAgWideReq_info : OutTag, public DataElemInfo {
    using data_elem_type = VisnImgAgWideInUse;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Sets the cameras to use for stitching in the PAC360 top view. */ 
struct VisnImgCamReq_info : OutTag, public DataElemInfo {
    using data_elem_type = CamIndReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal indicating which warning mode the driver has selected for the Lane Departure Warning (LDW) function. */ 
struct WarnTypForLaneChgWarn_info : OutTag, public DataElemInfo {
    using data_elem_type = WarnTypForLaneChgWarnPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Signal indicating which warning mode the driver has selected for the Lane Keeping Aid (LKA) function. */ 
struct WarnTypForLaneKeepAid_info : OutTag, public DataElemInfo {
    using data_elem_type = WarnTypForLaneChgWarnPen1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                WheelCircumference: High average tyre rolling circumference valid for a specific group of tyres. 
Value in mm. Fault indicated using 4095. */ 
struct WhlCircum_info : InTag, public DataElemInfo {
    using data_elem_type = uint16_t; // 0->4095

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Estimated electric motor torque referred to the  wheel axle. The torque is positive in the forward rotation direction. 
 */ 
struct WhlMotSysTqEst_info : InTag, public DataElemInfo {
    using data_elem_type = TqSafe2;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Wheel rotation ticks for each individual wheel. */ 
struct WhlRotToothCntr_info : InTag, public DataElemInfo {
    using data_elem_type = WhlRotToothCntrRec;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                "Signal sent to inform the rest of the car of the window position status for the front driver side door. The value of the signal is either an approximated percentage of openness, of the window is moving or if the position is unkown.
"
 */ 
struct WinPosnStsAtDrv_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                "Signal sent to inform the rest of the car of the window position status for the rear driver side door. The value of the signal is either an approximated percentage of openness, of the window is moving or if the position is unkown.
"
 */ 
struct WinPosnStsAtDrvrRe_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                "Signal sent to inform the rest of the car of the window position status for the front passenger side door. The value of the signal is either an approximated percentage of openness, of the window is moving or if the position is unkown.
"
 */ 
struct WinPosnStsAtPass_info : InTag, public DataElemInfo {
    using data_elem_type = WinAndRoofAndCurtPosnTyp;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                "Signal sent to inform the rest of the car of the window position status for the rear passenger side door. The value of the signal is either an approximated percentage of openness, of the window is moving or if the position is unkown.
"
 */ 
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


/** 
                Control of the windscreen wiper service position.
 */ 
struct WiprFrntSrvModReq_info : OutTag, public DataElemInfo {
    using data_elem_type = WiprFrntSrvModReq;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Information regarding when the windscreen wipers are in service position where for example the wipers can be raised for wiper blade exchange. */ 
struct WiprInPosnForSrv_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Distribution of the setting Auto Rear Wiping  */ 
struct WiprReAutReq_info : OutTag, public DataElemInfo {
    using data_elem_type = OnOffPen;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Information regarding if the washer fluid level is low. 
On = Washer Fluid Level Low
Off = Washer Fluid Level Not Low.  */ 
struct WshrFldSts1WdElmHMI_info : InTag, public DataElemInfo {
    using data_elem_type = OnOff1;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Output from iTPMS to indicate if a new calibration is possible or not. */ 
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


/** 
                Output from iTPMS to indicate calibration request status. */ 
struct iTPMSCalSts_info : InTag, public DataElemInfo {
    using data_elem_type = iTPMSTirePCalSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


/** 
                Output from iTPMS to indicate tire pressure warnings (puncture and diffusion) or iTPMS malfunction. */ 
struct iTPMSTirePMSts_info : InTag, public DataElemInfo {
    using data_elem_type = iTPMSTirePMSts;

    static const char* Name();
    static const char* TypeName();
    static Dir Direction();

    const char* name() const override;
    const char* typeName() const override;
    Dir direction() const override;
};


} // end of namespace
#endif