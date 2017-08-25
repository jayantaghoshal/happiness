# Signal scaling database
# --- AUTO GENERATED ---
# Inputs: generate_signal_scaling.py 
#    --swcinputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml 
#    --cominputfile=../../../hardware/signals/dataelements/AutosarCodeGen/databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Com.arxml 
#    --fdxdescriptionfile=FDXDescriptionFile.xml 
#    --out=generated/pyDataElements.py


# Controls the ABS warning indication to the driver.
class AbsWarnIndcnReq:
    de_name     = "BrkAndAbsWarnIndcnReq.AbsWarnIndcnReq"
    fdx_name    = "AbsWarnIndcnReq"
    fdx_groupid = 1373
    class map:
        AbsWarnIndcnOnReq = 0
        AbsWarnIndcnFlsgReq = 1
        Resd2 = 2
        AbsWarnIndcnOffReq = 3
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class AccAdprSpdLimActvPen:
    de_name     = "AccAdprSpdLimActv.Pen"
    fdx_name    = "AccAdprSpdLimActvPen"
    fdx_groupid = 1275
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
    

class AccAdprSpdLimActvSts:
    de_name     = "AccAdprSpdLimActv.Sts"
    fdx_name    = "AccAdprSpdLimActvSts"
    fdx_groupid = 1275
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class AccAdprTurnSpdActvPen:
    de_name     = "AccAdprTurnSpdActv.Pen"
    fdx_name    = "AccAdprTurnSpdActvPen"
    fdx_groupid = 1282
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
    

class AccAdprTurnSpdActvSts:
    de_name     = "AccAdprTurnSpdActv.Sts"
    fdx_name    = "AccAdprTurnSpdActvSts"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class AccAutResuWarnReq:
    de_name     = "AccAutResuWarnReq"
    fdx_name    = "AccAutResuWarnReq"
    fdx_groupid = 1145
    class map:
        Off = 0
        On = 1
    

class AccSts:
    de_name     = "AccSts"
    fdx_name    = "AccSts"
    fdx_groupid = 1368
    class map:
        Off = 1
        Stb = 2
        Stop = 3
        StopTmp = 4
        Actv = 5
    

class ActvOfHorn:
    de_name     = "ActvOfHorn"
    fdx_name    = "ActvOfHorn"
    fdx_groupid = 1220
    class map:
        Off = 0
        On = 1
    

# Unit: Unitless,  Range:0->255
class ADataRawSafeChks:
    de_name     = "ADataRawSafe.Chks"
    fdx_name    = "ADataRawSafeChks"
    fdx_groupid = 1353
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class ADataRawSafeCntr:
    de_name     = "ADataRawSafe.Cntr"
    fdx_name    = "ADataRawSafeCntr"
    fdx_groupid = 1353
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class AdjSpdLimnSts:
    de_name     = "AdjSpdLimnSts"
    fdx_name    = "AdjSpdLimnSts"
    fdx_groupid = 1368
    class map:
        Off = 1
        Stb = 2
        Actv = 3
        Ovrd = 4
    

class AdjSpdLimnWarn:
    de_name     = "AdjSpdLimnWarn"
    fdx_name    = "AdjSpdLimnWarn"
    fdx_groupid = 1368
    class map:
        NoWarn = 0
        SoundWarn = 1
        VisWarn = 2
        SoundAndVisWarn = 3
    

# Unit: Unitless,  Range:0->255
class AgDataRawSafeChks:
    de_name     = "AgDataRawSafe.Chks"
    fdx_name    = "AgDataRawSafeChks"
    fdx_groupid = 1353
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class AgDataRawSafeCntr:
    de_name     = "AgDataRawSafe.Cntr"
    fdx_name    = "AgDataRawSafeCntr"
    fdx_groupid = 1353
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class AirClngReq:
    de_name     = "AirClngReq"
    fdx_name    = "AirClngReq"
    fdx_groupid = 1279
    class map:
        NoReq = 0
        On = 1
        Off = 2
    

# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class ALat1:
    de_name     = "ADataRawSafe.ALat"
    fdx_name    = "ALat1"
    fdx_groupid = 1353
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class ALat1Qf1:
    de_name     = "ADataRawSafe.ALat1Qf"
    fdx_name    = "ALat1Qf1"
    fdx_groupid = 1353
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3
    

# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class ALgt1:
    de_name     = "ADataRawSafe.ALgt"
    fdx_name    = "ALgt1"
    fdx_groupid = 1353
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class ALgt1Qf1:
    de_name     = "ADataRawSafe.ALgt1Qf"
    fdx_name    = "ALgt1Qf1"
    fdx_groupid = 1353
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3
    

# Unit: Unitless,  Range:-100->309.5, Resolution: (0.1*x+-100.0, raw is unsigned, 12 bits )
class AmbTIndcd:
    de_name     = "AmbTIndcdWithUnit.AmbTIndcd"
    fdx_name    = "AmbTIndcd"
    fdx_groupid = 1231
    min    = -100
    max    = 309.5
    scale  = 0.1
    offset = -100.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class AmbTIndcdQf:
    de_name     = "AmbTIndcdWithUnit.QF"
    fdx_name    = "AmbTIndcdQf"
    fdx_groupid = 1231
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

class AmbTIndcdUnit:
    de_name     = "AmbTIndcdWithUnit.AmbTIndcdUnit"
    fdx_name    = "AmbTIndcdUnit"
    fdx_groupid = 1231
    class map:
        Celsius = 0
        Fahrenheit = 1
        UkwnUnit = 2
    

class AmbTRawQly:
    de_name     = "AmbTRaw.Qly"
    fdx_name    = "AmbTRawQly"
    fdx_groupid = 1219
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

# Unit: degC,  Range:-70->134.7, Resolution: (0.1*x+-70.0, raw is unsigned, 11 bits )
class AmbTRawVal:
    de_name     = "AmbTRaw.AmbTVal"
    fdx_name    = "AmbTRawVal"
    fdx_groupid = 1219
    min    = -70
    max    = 134.7
    scale  = 0.1
    offset = -70.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->7
class AntithftRednReq:
    de_name     = "AntithftRednReq"
    fdx_name    = "AntithftRednReq"
    fdx_groupid = 1278
    min = 0
    max = 7
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class AsyLaneChgPsbl:
    de_name     = "AsyLaneChg.Psbl"
    fdx_name    = "AsyLaneChgPsbl"
    fdx_groupid = 1144
    class map:
        NotPsbl = 0
        Le = 1
        Ri = 2
        Both = 3
    

class AsyLaneChgSts:
    de_name     = "AsyLaneChg.Sts"
    fdx_name    = "AsyLaneChgSts"
    fdx_groupid = 1144
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
    

class AsyLaneChgTyp:
    de_name     = "AsyLaneChg.Typ"
    fdx_name    = "AsyLaneChgTyp"
    fdx_groupid = 1144
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
    

class AsySteerApplyRqrd:
    de_name     = "AsySteerApplyRqrd"
    fdx_name    = "AsySteerApplyRqrd"
    fdx_groupid = 1139
    class map:
        NoWarn = 0
        Lvl1 = 1
        Lvl2 = 2
    

# Unit: Unitless,  Range:0->255
class AudMsgReq:
    de_name     = "AudMsgReq"
    fdx_name    = "AudMsgReq"
    fdx_groupid = 1230
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class AudWarn:
    de_name     = "AudWarn"
    fdx_name    = "AudWarn"
    fdx_groupid = 1216
    def AudWarn_r2p(raw):
        return raw
    def AudWarn_p2r(physical):
        return physical

# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class AudWarnActv:
    de_name     = "AudWarnActv"
    fdx_name    = "AudWarnActv"
    fdx_groupid = 1280
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3
    

# Unit: m/s^2,  Range:-139->139, Resolution: (0.0085*x+0.0, raw is signed, 15 bits )
class AVert2:
    de_name     = "ADataRawSafe.AVert"
    fdx_name    = "AVert2"
    fdx_groupid = 1353
    min    = -139
    max    = 139
    scale  = 0.0085
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class AVert2Qf1:
    de_name     = "ADataRawSafe.AVertQf"
    fdx_name    = "AVert2Qf1"
    fdx_groupid = 1353
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3
    

# Unit: Unitless,  Range:0->255
class BackCntrForMissCom:
    de_name     = "BackCntrForMissCom"
    fdx_name    = "BackCntrForMissCom"
    fdx_groupid = 1366
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: %,  Range:0->102.3, Resolution: (0.1*x+0.0, raw is unsigned, 10 bits )
class BarForFuEco:
    de_name     = "CchForFuEco.BarForFuEco"
    fdx_name    = "BarForFuEco"
    fdx_groupid = 1371
    min    = 0
    max    = 102.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: km,  Range:0->2000000, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class BkpOfDstTrvld:
    de_name     = "BkpOfDstTrvld"
    fdx_name    = "BkpOfDstTrvld"
    fdx_groupid = 1231
    min    = 0
    max    = 2000000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class BltLockStAtDrvrForBltLockSt1:
    de_name     = "BltLockStAtDrvr.BltLockSt1"
    fdx_name    = "BltLockStAtDrvrForBltLockSt1"
    fdx_groupid = 1351
    class map:
        Unlock = 0
        Lock = 1
    

class BltLockStAtDrvrForDevErrSts2:
    de_name     = "BltLockStAtDrvr.BltLockSts"
    fdx_name    = "BltLockStAtDrvrForDevErrSts2"
    fdx_groupid = 1351
    class map:
        NoFlt = 0
        Flt = 1
    

class BltLockStAtPassForBltLockSt1:
    de_name     = "BltLockStAtPass.BltLockSt1"
    fdx_name    = "BltLockStAtPassForBltLockSt1"
    fdx_groupid = 1352
    class map:
        Unlock = 0
        Lock = 1
    

class BltLockStAtPassForBltLockSts:
    de_name     = "BltLockStAtPass.BltLockSts"
    fdx_name    = "BltLockStAtPassForBltLockSts"
    fdx_groupid = 1352
    class map:
        NoFlt = 0
        Flt = 1
    

# Description if a component is equipped or not.
class BltLockStAtRowSecLeForBltLockEquid:
    de_name     = "BltLockStAtRowSecLe.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecLeForBltLockEquid"
    fdx_groupid = 1352
    class map:
        Equid = 0
        NotEquid = 1
    

class BltLockStAtRowSecLeForBltLockSt1:
    de_name     = "BltLockStAtRowSecLe.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecLeForBltLockSt1"
    fdx_groupid = 1352
    class map:
        Unlock = 0
        Lock = 1
    

class BltLockStAtRowSecLeForBltLockSts:
    de_name     = "BltLockStAtRowSecLe.BltLockSts"
    fdx_name    = "BltLockStAtRowSecLeForBltLockSts"
    fdx_groupid = 1352
    class map:
        NoFlt = 0
        Flt = 1
    

# Description if a component is equipped or not.
class BltLockStAtRowSecMidForBltLockEquid:
    de_name     = "BltLockStAtRowSecMid.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecMidForBltLockEquid"
    fdx_groupid = 1352
    class map:
        Equid = 0
        NotEquid = 1
    

class BltLockStAtRowSecMidForBltLockSt1:
    de_name     = "BltLockStAtRowSecMid.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecMidForBltLockSt1"
    fdx_groupid = 1352
    class map:
        Unlock = 0
        Lock = 1
    

class BltLockStAtRowSecMidForBltLockSts:
    de_name     = "BltLockStAtRowSecMid.BltLockSts"
    fdx_name    = "BltLockStAtRowSecMidForBltLockSts"
    fdx_groupid = 1352
    class map:
        NoFlt = 0
        Flt = 1
    

# Description if a component is equipped or not.
class BltLockStAtRowSecRiForBltLockEquid:
    de_name     = "BltLockStAtRowSecRi.BltLockEquid"
    fdx_name    = "BltLockStAtRowSecRiForBltLockEquid"
    fdx_groupid = 1352
    class map:
        Equid = 0
        NotEquid = 1
    

class BltLockStAtRowSecRiForBltLockSt1:
    de_name     = "BltLockStAtRowSecRi.BltLockSt1"
    fdx_name    = "BltLockStAtRowSecRiForBltLockSt1"
    fdx_groupid = 1352
    class map:
        Unlock = 0
        Lock = 1
    

class BltLockStAtRowSecRiForBltLockSts:
    de_name     = "BltLockStAtRowSecRi.BltLockSts"
    fdx_name    = "BltLockStAtRowSecRiForBltLockSts"
    fdx_groupid = 1352
    class map:
        NoFlt = 0
        Flt = 1
    

# Description if a component is equipped or not.
class BltLockStAtRowThrdLeForBltLockEquid:
    de_name     = "BltLockStAtRowThrdLe.BltLockEquid"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockEquid"
    fdx_groupid = 1352
    class map:
        Equid = 0
        NotEquid = 1
    

class BltLockStAtRowThrdLeForBltLockSt1:
    de_name     = "BltLockStAtRowThrdLe.BltLockSt1"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockSt1"
    fdx_groupid = 1352
    class map:
        Unlock = 0
        Lock = 1
    

class BltLockStAtRowThrdLeForBltLockSts:
    de_name     = "BltLockStAtRowThrdLe.BltLockSts"
    fdx_name    = "BltLockStAtRowThrdLeForBltLockSts"
    fdx_groupid = 1352
    class map:
        NoFlt = 0
        Flt = 1
    

# Description if a component is equipped or not.
class BltLockStAtRowThrdRiForBltLockEquid:
    de_name     = "BltLockStAtRowThrdRi.BltLockEquid"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockEquid"
    fdx_groupid = 1352
    class map:
        Equid = 0
        NotEquid = 1
    

class BltLockStAtRowThrdRiForBltLockSt1:
    de_name     = "BltLockStAtRowThrdRi.BltLockSt1"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockSt1"
    fdx_groupid = 1352
    class map:
        Unlock = 0
        Lock = 1
    

class BltLockStAtRowThrdRiForBltLockSts:
    de_name     = "BltLockStAtRowThrdRi.BltLockSts"
    fdx_name    = "BltLockStAtRowThrdRiForBltLockSts"
    fdx_groupid = 1352
    class map:
        NoFlt = 0
        Flt = 1
    

class BltRmnSound1:
    de_name     = "BltRmnSound1"
    fdx_name    = "BltRmnSound1"
    fdx_groupid = 1231
    class map:
        NoTrig = 0
        Trig = 1
    

class BltRmnSound2:
    de_name     = "BltRmnSound2"
    fdx_name    = "BltRmnSound2"
    fdx_groupid = 1231
    class map:
        NoTrig = 0
        Trig = 1
    

class BltRmnSound3:
    de_name     = "BltRmnSound3"
    fdx_name    = "BltRmnSound3"
    fdx_groupid = 1231
    class map:
        NoTrig = 0
        Trig = 1
    

class BltRmnSound4:
    de_name     = "BltRmnSound4"
    fdx_name    = "BltRmnSound4"
    fdx_groupid = 1231
    class map:
        NoTrig = 0
        Trig = 1
    

# Unit: Unitless,  Range:0->255
class BrkAndAbsWarnIndcnReqChks:
    de_name     = "BrkAndAbsWarnIndcnReq.BrkAndAbsWarnIndcnReqChks"
    fdx_name    = "BrkAndAbsWarnIndcnReqChks"
    fdx_groupid = 1373
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class BrkAndAbsWarnIndcnReqCntr:
    de_name     = "BrkAndAbsWarnIndcnReq.BrkAndAbsWarnIndcnReqCntr"
    fdx_name    = "BrkAndAbsWarnIndcnReqCntr"
    fdx_groupid = 1373
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# 0= High fluid level
# 1= Low fluid level
class BrkFldLvl:
    de_name     = "BrkFldLvl"
    fdx_name    = "BrkFldLvl"
    fdx_groupid = 1374
    class map:
        FldLvlHi = 0
        FldLvlLo = 1
    

# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlFrntLeAct:
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlFrntLeAct"
    fdx_name    = "BrkFricTqAtWhlFrntLeAct"
    fdx_groupid = 1377
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlFrntRiAct:
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlFrntRiAct"
    fdx_name    = "BrkFricTqAtWhlFrntRiAct"
    fdx_groupid = 1377
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlReLeAct:
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlReLeAct"
    fdx_name    = "BrkFricTqAtWhlReLeAct"
    fdx_groupid = 1377
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Nm,  Range:0->8191, Resolution: (1.0*x+0.0, raw is unsigned, 13 bits )
class BrkFricTqAtWhlReRiAct:
    de_name     = "BrkFricTqAtWhlAct.BrkFricTqAtWhlReRiAct"
    fdx_name    = "BrkFricTqAtWhlReRiAct"
    fdx_groupid = 1377
    min    = 0
    max    = 8191
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class BrkRelsWarnReq:
    de_name     = "BrkRelsWarnReq"
    fdx_name    = "BrkRelsWarnReq"
    fdx_groupid = 1370
    class map:
        No = 0
        Yes = 1
    

# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class BrkWarnIndcnReq:
    de_name     = "BrkAndAbsWarnIndcnReq.BrkWarnIndcnReq"
    fdx_name    = "BrkWarnIndcnReq"
    fdx_groupid = 1373
    class map:
        On = 0
        Off = 1
    

class BtnAudFbSts:
    de_name     = "BtnAudFbSts"
    fdx_name    = "BtnAudFbSts"
    fdx_groupid = 1231
    class map:
        NotPsd = 0
        Psd = 1
    

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
    class map:
        NoBtnPsd = 0
        BtnVolUpPsd = 1
        BtnVolDwnPsd = 2
    

# Represents status of a (multilevel) button "Left/Right. 
# 
# Semantic defined by receiver side. (Previou/Next, FFBW/FFWD, etc)
class BtnMmedLeRiSts:
    de_name     = "BtnMmedLeRiSts"
    fdx_name    = "BtnMmedLeRiSts"
    fdx_groupid = 1230
    class map:
        NoBtnPsd = 0
        BtnLePsd = 1
        BtnRiPsd = 2
    

class BtnMmedModSts:
    de_name     = "BtnMmedModSts"
    fdx_name    = "BtnMmedModSts"
    fdx_groupid = 1232
    class map:
        NotPsd = 0
        Psd = 1
    

# Represents status of a button "Set". 
# 
# Semantic defined by receiver side. (Activate/Deactivate, Play/Pause, Mute/UnMute etc)
class BtnMmedSetSts:
    de_name     = "BtnMmedSetSts"
    fdx_name    = "BtnMmedSetSts"
    fdx_groupid = 1230
    class map:
        BtnSetNotPsd = 0
        BtnSetPsd = 1
    

# Carries call presence information. Defined for CallSts range [0-1]
# 
# 0 = Inactive. There is no active call present with any telephony solution in the system.
# 1 = Active. Atleast one active call is present in the system.
class CallStsIndcn:
    de_name     = "CallSts"
    fdx_name    = "CallStsIndcn"
    fdx_groupid = 1277
    class map:
        Inactive = 0
        Active = 1
    

class CarModSts1:
    de_name     = "VehModMngtGlbSafe1.CarModSts1"
    fdx_name    = "CarModSts1"
    fdx_groupid = 1212
    class map:
        CarModNorm = 0
        CarModTrnsp = 1
        CarModFcy = 2
        CarModCrash = 3
        CarModDyno = 5
    

# Unit: Unitless,  Range:0->7
class CarModSubtypWdCarModSubtyp:
    de_name     = "VehModMngtGlbSafe1.CarModSubtypWdCarModSubtyp"
    fdx_name    = "CarModSubtypWdCarModSubtyp"
    fdx_groupid = 1212
    min = 0
    max = 7
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: s,  Range:0->4.294967294E8, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class CarTiGlb:
    de_name     = "CarTiGlb"
    fdx_name    = "CarTiGlb"
    fdx_groupid = 1216
    min    = 0
    max    = 4.294967294E8
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class ChdLockgProtnStsToHmi:
    de_name     = "ChdLockgProtnStsToHmi"
    fdx_name    = "ChdLockgProtnStsToHmi"
    fdx_groupid = 1214
    class map:
        OnOffSafeInvld1 = 0
        OnOffSafeOn = 1
        OnOffSafeOff = 2
        OnOffSafeInvld2 = 3
    

class ChdWinProtnStsAtDrvrRe:
    de_name     = "ChdWinProtnStsAtDrvrRe"
    fdx_name    = "ChdWinProtnStsAtDrvrRe"
    fdx_groupid = 1219
    class map:
        Off = 0
        On = 1
    

class ChdWinProtnStsAtPassRe:
    de_name     = "ChdWinProtnStsAtPassRe"
    fdx_name    = "ChdWinProtnStsAtPassRe"
    fdx_groupid = 1219
    class map:
        Off = 0
        On = 1
    

class ClimaActv:
    de_name     = "ClimaActv"
    fdx_name    = "ClimaActv"
    fdx_groupid = 1214
    class map:
        Off = 0
        On = 1
    

class ClimaEcoModRqrd:
    de_name     = "PostDrvgClimaReq"
    fdx_name    = "ClimaEcoModRqrd"
    fdx_groupid = 1276
    class map:
        Off = 0
        On = 1
    

# Unit: W,  Range:0->2047, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class ClimaPwrCns:
    de_name     = "ClimaPwrCns"
    fdx_name    = "ClimaPwrCns"
    fdx_groupid = 1217
    min    = 0
    max    = 2047
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class ClimaRqrd:
    de_name     = "ClimaRqrdFromHmi"
    fdx_name    = "ClimaRqrd"
    fdx_groupid = 1282
    class map:
        NoReq = 0
        On = 1
        Off = 2
    

# Unit: s,  Range:0->4.294967294E8, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class ClimaTmr:
    de_name     = "ClimaTmr"
    fdx_name    = "ClimaTmr"
    fdx_groupid = 1279
    min    = 0
    max    = 4.294967294E8
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class ClimaTmrSts:
    de_name     = "ClimaTmrSts"
    fdx_name    = "ClimaTmrSts"
    fdx_groupid = 1218
    class map:
        Off = 0
        On = 1
    

class ClimaTmrStsRqrd:
    de_name     = "ClimaTmrStsRqrd"
    fdx_name    = "ClimaTmrStsRqrd"
    fdx_groupid = 1279
    class map:
        Off = 0
        On = 1
    

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
    

# 24h or 12h indication of clock
class ClkFmt:
    de_name     = "IndcnUnit.ClkFmt"
    fdx_name    = "ClkFmt"
    fdx_groupid = 1282
    class map:
        Hr24 = 0
        Hr12 = 1
        UkwnClkFmt = 2
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class CllsnFwdWarnActvPen:
    de_name     = "CllsnFwdWarnActv.Pen"
    fdx_name    = "CllsnFwdWarnActvPen"
    fdx_groupid = 1282
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
    

class CllsnFwdWarnActvSts:
    de_name     = "CllsnFwdWarnActv.Sts"
    fdx_name    = "CllsnFwdWarnActvSts"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class CllsnFwdWarnReq:
    de_name     = "CllsnFwdWarnReq"
    fdx_name    = "CllsnFwdWarnReq"
    fdx_groupid = 1138
    class map:
        Off = 0
        On = 1
    

class CllsnWarnReq:
    de_name     = "CllsnWarnReq"
    fdx_name    = "CllsnWarnReq"
    fdx_groupid = 1138
    class map:
        Off = 0
        On = 1
    

class CllsnWarnSideLe:
    de_name     = "CllsnWarnSide.Le"
    fdx_name    = "CllsnWarnSideLe"
    fdx_groupid = 1138
    class map:
        NoWarn = 0
        WarnLvl1 = 1
        WarnLvl2 = 2
        Resd3 = 3
    

class CllsnWarnSideRi:
    de_name     = "CllsnWarnSide.Ri"
    fdx_name    = "CllsnWarnSideRi"
    fdx_groupid = 1138
    class map:
        NoWarn = 0
        WarnLvl1 = 1
        WarnLvl2 = 2
        Resd3 = 3
    

class ClngActv:
    de_name     = "ClngActv"
    fdx_name    = "ClngActv"
    fdx_groupid = 1219
    class map:
        NoReq = 0
        On = 1
        Off = 2
    

class ClngRqrdFromHmi:
    de_name     = "ClngRqrdFromHmi"
    fdx_name    = "ClngRqrdFromHmi"
    fdx_groupid = 1280
    class map:
        NoReq = 0
        On = 1
        Off = 2
    

class ClsAutEna:
    de_name     = "ClsAutEna"
    fdx_name    = "ClsAutEna"
    fdx_groupid = 1280
    class map:
        Disabled = 0
        Enabled = 1
    

class CmftFctActv:
    de_name     = "CmftFctActv"
    fdx_name    = "CmftFctActv"
    fdx_groupid = 1280
    class map:
        Off = 0
        On = 1
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

# Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )
class CmptmtAirTEstimdExtdComptmtT:
    de_name     = "CmptmtAirTEstimdExtd.ComptmtT"
    fdx_name    = "CmptmtAirTEstimdExtdComptmtT"
    fdx_groupid = 1219
    min    = -60
    max    = 125
    scale  = 0.1
    offset = -60.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class CmptmtAirTEstimdExtdQlyFlg:
    de_name     = "CmptmtAirTEstimdExtd.QlyFlg"
    fdx_name    = "CmptmtAirTEstimdExtdQlyFlg"
    fdx_groupid = 1219
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

# Unit: degC,  Range:-60->125, Resolution: (0.1*x+-60.0, raw is unsigned, 11 bits )
class CmptmtTFrnt:
    de_name     = "CmptmtTFrnt.CmptmtTFrnt"
    fdx_name    = "CmptmtTFrnt"
    fdx_groupid = 1279
    min    = -60
    max    = 125
    scale  = 0.1
    offset = -60.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Interpretation:
# SnsrDataUndefd - Accuracy of data undefinable
# FanNotRunning - Aspiration fan not running
# SnsrDataNotOk - Data accuracy not within specification
# SnsrDataOk - Data is calculated within specified accuracy
class CmptmtTFrntQf:
    de_name     = "CmptmtTFrnt.CmptmtTFrntQf"
    fdx_name    = "CmptmtTFrntQf"
    fdx_groupid = 1279
    class map:
        SnsrDataUndefd = 0
        FanNotRunning = 1
        SnsrDataNotOk = 2
        SnsrDataOk = 3
    

# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnForCrsCtrl:
    de_name     = "CnclWarnForCrsCtrl"
    fdx_name    = "CnclWarnForCrsCtrl"
    fdx_groupid = 1378
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3
    

# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnLatForAutDrv:
    de_name     = "SftyWarnGroupFromAsySafe.CnclWarnLatForAutDrv"
    fdx_name    = "CnclWarnLatForAutDrv"
    fdx_groupid = 1138
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3
    

# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class CnclWarnLgtForAutDrv:
    de_name     = "CnclWarnLgtForAutDrv"
    fdx_name    = "CnclWarnLgtForAutDrv"
    fdx_groupid = 1142
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3
    

class CntDwnToManvStrtInDstToManvLocn:
    de_name     = "DstToManvLocn.CntDwnToManvStrt"
    fdx_name    = "CntDwnToManvStrtInDstToManvLocn"
    fdx_groupid = 1280
    class map:
        No = 0
        Yes = 1
    

class ComLostExtrSound:
    de_name     = "ComLostExtrSound"
    fdx_name    = "ComLostExtrSound"
    fdx_groupid = 1375
    def ComLostExtrSound_r2p(raw):
        return raw
    def ComLostExtrSound_p2r(physical):
        return physical

# Unit: Unitless,  Range:0->15
class ConSftyWarnConSftyWarnId:
    de_name     = "ConSftyWarn.ConSftyWarnId"
    fdx_name    = "ConSftyWarnConSftyWarnId"
    fdx_groupid = 1144
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class ConSftyWarnConSftyWarnLvl:
    de_name     = "ConSftyWarn.ConSftyWarnLvl"
    fdx_name    = "ConSftyWarnConSftyWarnLvl"
    fdx_groupid = 1144
    class map:
        NoWarn = 0
        LoLvl = 1
        MedLvl = 2
        HiLvl = 3
    

class ConSftyWarnConSftyWarnSnd:
    de_name     = "ConSftyWarn.ConSftyWarnSnd"
    fdx_name    = "ConSftyWarnConSftyWarnSnd"
    fdx_groupid = 1144
    class map:
        Off = 0
        On = 1
    

# Unit: Unitless,  Range:0->15
class ConSftyWarnConSftyWarnTyp:
    de_name     = "ConSftyWarn.ConSftyWarnTyp"
    fdx_name    = "ConSftyWarnConSftyWarnTyp"
    fdx_groupid = 1144
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: m,  Range:0->65535
class ConSftyWarnDistanceToWarning:
    de_name     = "ConSftyWarn.DistanceToWarning"
    fdx_name    = "ConSftyWarnDistanceToWarning"
    fdx_groupid = 1144
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Interpretation:
# NoReq - No active cooling requested
# LoReq - Low cooling need
# HiReq - High cooling need
class CoolgReqForDispCen:
    de_name     = "CoolgReqForDispCen"
    fdx_name    = "CoolgReqForDispCen"
    fdx_groupid = 1279
    class map:
        NoReq = 0
        LoReq = 1
        HiReq = 2
    

# Interpretation:
# NoCoolgActv - No cooling request received, no actions taken
# CoolgLvlStrtd1 - Activated cooling in level 1 started
# CoolgLvlStrtd2 - Activated cooling in level 2 started
class CoolgStsForDisp:
    de_name     = "CoolgStsForDisp"
    fdx_name    = "CoolgStsForDisp"
    fdx_groupid = 1218
    class map:
        NoCoolgActv = 0
        CoolgLvlStrtd1 = 1
        CoolgLvlStrtd2 = 2
        Resd = 3
    

class CrsCtrlrSts:
    de_name     = "CrsCtrlrSts"
    fdx_name    = "CrsCtrlrSts"
    fdx_groupid = 1368
    class map:
        Off = 1
        Stb = 2
        Actv = 3
    

class CrvtSpdAdpvSts:
    de_name     = "CrvtSpdAdpvSts"
    fdx_name    = "CrvtSpdAdpvSts"
    fdx_groupid = 1142
    class map:
        Off = 0
        On = 1
        NotAvl = 2
        SrvRqrd = 3
    

# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No warning
# 1			1			WarnForCmngCrvtLe	Warning For Coming Curvature Left	Warning for upcoming left curve
# 2			2			WarnForCmngCrvtRi	Warning For Coming Curvature Right	Warning for upcoming right curve
class CrvtSpdWarnReq:
    de_name     = "CrvtSpdWarnReq"
    fdx_name    = "CrvtSpdWarnReq"
    fdx_groupid = 1142
    class map:
        NoWarn = 0
        WarnForCmngCrvtLe = 1
        WarnForCmngCrvtRi = 2
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

class CtraIndcnLe:
    de_name     = "CtraIndcnLe"
    fdx_name    = "CtraIndcnLe"
    fdx_groupid = 1139
    class map:
        NoCtraWarn = 0
        CtraWarn = 1
    

class CtraIndcnRi:
    de_name     = "CtraIndcnRi"
    fdx_name    = "CtraIndcnRi"
    fdx_groupid = 1139
    class map:
        NoCtraWarn = 0
        CtraWarn = 1
    

class CtraOn1:
    de_name     = "CtraOn1"
    fdx_name    = "CtraOn1"
    fdx_groupid = 1138
    class map:
        StrtUpOn = 0
        On = 1
        Off = 2
        TrlrOff = 3
    

class CtraSwOnOff:
    de_name     = "CtraSwOnOff"
    fdx_name    = "CtraSwOnOff"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

class CurtActvnReReq:
    de_name     = "CurtActvnReReq"
    fdx_name    = "CurtActvnReReq"
    fdx_groupid = 1276
    class map:
        BtnReld = 0
        BtnPsd = 1
        Spare1 = 2
        Spare2 = 3
    

class DateOrTi:
    de_name     = "SetTiAndDate.DateOrTi"
    fdx_name    = "DateOrTi"
    fdx_groupid = 1277
    class map:
        None_ = 0
        Date = 1
        Ti = 2
        DateTi = 3
    

# Unit: Days,  Range:1->31
class Day:
    de_name     = "TiAndDateIndcn.Day"
    fdx_name    = "Day"
    fdx_groupid = 1217
    min = 1
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Days,  Range:1->31
class Day1:
    de_name     = "SetTiAndDate.Day"
    fdx_name    = "Day1"
    fdx_groupid = 1277
    min = 1
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Days,  Range:0->750
class DayToSrv:
    de_name     = "DayToSrv"
    fdx_name    = "DayToSrv"
    fdx_groupid = 1232
    min = 0
    max = 750
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class DispAndHomeBtnSts:
    de_name     = "DispAndHomeBtnSts"
    fdx_name    = "DispAndHomeBtnSts"
    fdx_groupid = 1277
    class map:
        NoTrig = 0
        Trig = 1
    

# Unit: Wh,  Range:0->127.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class DispBattEgyIn:
    de_name     = "DispBattEgyIn"
    fdx_name    = "DispBattEgyIn"
    fdx_groupid = 1375
    min    = 0
    max    = 127.5
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Wh,  Range:0->127.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class DispBattEgyOut:
    de_name     = "DispBattEgyOut"
    fdx_name    = "DispBattEgyOut"
    fdx_groupid = 1375
    min    = 0
    max    = 127.5
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class DispOfPrpsnMod:
    de_name     = "DispOfPrpsnMod"
    fdx_name    = "DispOfPrpsnMod"
    fdx_groupid = 1372
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
    

class DoorDrvrLockReSts:
    de_name     = "DoorDrvrLockReSts"
    fdx_name    = "DoorDrvrLockReSts"
    fdx_groupid = 1219
    class map:
        LockStsUkwn = 0
        Unlckd = 1
        Lockd = 2
        SafeLockd = 3
    

class DoorDrvrLockSts:
    de_name     = "DoorDrvrLockSts"
    fdx_name    = "DoorDrvrLockSts"
    fdx_groupid = 1219
    class map:
        LockStsUkwn = 0
        Unlckd = 1
        Lockd = 2
        SafeLockd = 3
    

# Status of the door.
class DoorDrvrReSts:
    de_name     = "DoorDrvrReSts"
    fdx_name    = "DoorDrvrReSts"
    fdx_groupid = 1214
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2
    

# Status of the door.
class DoorDrvrSts:
    de_name     = "DoorDrvrSts"
    fdx_name    = "DoorDrvrSts"
    fdx_groupid = 1212
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2
    

class DoorPassLockReSts:
    de_name     = "DoorPassLockReSts"
    fdx_name    = "DoorPassLockReSts"
    fdx_groupid = 1219
    class map:
        LockStsUkwn = 0
        Unlckd = 1
        Lockd = 2
        SafeLockd = 3
    

class DoorPassLockSts:
    de_name     = "DoorPassLockSts"
    fdx_name    = "DoorPassLockSts"
    fdx_groupid = 1219
    class map:
        LockStsUkwn = 0
        Unlckd = 1
        Lockd = 2
        SafeLockd = 3
    

# Status of the door.
class DoorPassReSts:
    de_name     = "DoorPassReSts"
    fdx_name    = "DoorPassReSts"
    fdx_groupid = 1212
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2
    

# Status of the door.
class DoorPassSts:
    de_name     = "DoorPassSts"
    fdx_name    = "DoorPassSts"
    fdx_groupid = 1212
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2
    

class DriveAwayInfoWarnReqAudWarnReq:
    de_name     = "DriveAwayInfoWarnReq.AudWarnReq"
    fdx_name    = "DriveAwayInfoWarnReqAudWarnReq"
    fdx_groupid = 1145
    class map:
        Off = 0
        PrepareToLeave = 1
        LeaveNow = 2
    

class DriveAwayInfoWarnReqCtrlDirOfTrfcLi:
    de_name     = "DriveAwayInfoWarnReq.CtrlDirOfTrfcLi"
    fdx_name    = "DriveAwayInfoWarnReqCtrlDirOfTrfcLi"
    fdx_groupid = 1145
    class map:
        NoDirection = 0
        Straight = 1
        Left = 2
        LeftStraight = 3
        Right = 4
        StraightRight = 5
        LeftRight = 6
        All = 7
    

class DriveAwayInfoWarnReqInterruptionSrc:
    de_name     = "DriveAwayInfoWarnReq.InterruptionSrc"
    fdx_name    = "DriveAwayInfoWarnReqInterruptionSrc"
    fdx_groupid = 1145
    class map:
        Off = 0
        TiOut = 1
        ResuPsd = 2
        BrkPedl = 3
        AccrPedlPsd = 4
        Resd5 = 5
        Resd6 = 6
        Resd7 = 7
    

class DriveAwayInfoWarnReqReqSrc:
    de_name     = "DriveAwayInfoWarnReq.ReqSrc"
    fdx_name    = "DriveAwayInfoWarnReqReqSrc"
    fdx_groupid = 1145
    class map:
        NoReq = 0
        TtgReq = 1
        Resd2 = 2
        Resd3 = 3
        Resd4 = 4
        Resd5 = 5
        Resd6 = 6
        Resd7 = 7
    

# Unit: Unitless,  Range:0->255
class DriveAwayInfoWarnReqTiToDrvCntDwnTi:
    de_name     = "DriveAwayInfoWarnReq.TiToDrvCntDwnTi"
    fdx_name    = "DriveAwayInfoWarnReqTiToDrvCntDwnTi"
    fdx_groupid = 1145
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class DriveAwayInfoWarnReqVisWarnReq:
    de_name     = "DriveAwayInfoWarnReq.VisWarnReq"
    fdx_name    = "DriveAwayInfoWarnReqVisWarnReq"
    fdx_groupid = 1145
    class map:
        Off = 0
        Countdown = 1
        LeaveNow = 2
    

class DrvgInWrgDirOfTrfc:
    de_name     = "DrvgInWrgDirOfTrfc"
    fdx_name    = "DrvgInWrgDirOfTrfc"
    fdx_groupid = 1280
    class map:
        No = 0
        Yes = 1
    

class DrvModReq:
    de_name     = "DrvModReq"
    fdx_name    = "DrvModReq"
    fdx_groupid = 1274
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
    

class DrvrCtrlOfPassSeatFrntReqd:
    de_name     = "DrvrCtrlOfPassSeatFrntReqd"
    fdx_name    = "DrvrCtrlOfPassSeatFrntReqd"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class DrvrCtrlOfPassSeatFrntSts:
    de_name     = "DrvrCtrlOfPassSeatFrntSts"
    fdx_name    = "DrvrCtrlOfPassSeatFrntSts"
    fdx_groupid = 1212
    class map:
        NotAvl = 0
        Avl = 1
        Deactvd = 2
        Actvd = 3
    

# Describes the desired vehicle movement direction from the driver.
# Contains only the drivers longitudinal direction intent. In which direction shall the car move.
class DrvrDesDir:
    de_name     = "DrvrDesDir"
    fdx_name    = "DrvrDesDir"
    fdx_groupid = 1369
    class map:
        Undefd = 0
        Fwd = 1
        Rvs = 2
        Neut = 3
        Resd0 = 4
        Resd1 = 5
        Resd2 = 6
        Resd3 = 7
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrDispSetgPen:
    de_name     = "DrvrDispSetg.Pen"
    fdx_name    = "DrvrDispSetgPen"
    fdx_groupid = 1280
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
    

class DrvrDispSetgSts:
    de_name     = "DrvrDispSetg.Sts"
    fdx_name    = "DrvrDispSetgSts"
    fdx_groupid = 1280
    class map:
        Cmft = 0
        Eco = 1
        Dyn = 2
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiBackGndInfoSetgPen:
    de_name     = "DrvrHmiBackGndInfoSetg.Pen"
    fdx_name    = "DrvrHmiBackGndInfoSetgPen"
    fdx_groupid = 1282
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
    

class DrvrHmiBackGndInfoSetgSetg:
    de_name     = "DrvrHmiBackGndInfoSetg.Setg"
    fdx_name    = "DrvrHmiBackGndInfoSetgSetg"
    fdx_groupid = 1282
    class map:
        DrvrHmiBackGndInfoSetg0 = 0
        DrvrHmiBackGndInfoSetg1 = 1
        DrvrHmiBackGndInfoSetg2 = 2
        DrvrHmiBackGndInfoSetg3 = 3
        DrvrHmiBackGndInfoSetg4 = 4
        DrvrHmiBackGndInfoSetg5 = 5
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiDispdModPen:
    de_name     = "DrvrHmiDispdMod.Pen"
    fdx_name    = "DrvrHmiDispdModPen"
    fdx_groupid = 1279
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
    

class DrvrHmiDispdModSts:
    de_name     = "DrvrHmiDispdMod.Sts"
    fdx_name    = "DrvrHmiDispdModSts"
    fdx_groupid = 1279
    class map:
        DispDftMod = 0
        DispAudMod = 1
        DispNavMod = 2
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrHmiUsrIfSetgPen:
    de_name     = "DrvrHmiUsrIfSetg.Pen"
    fdx_name    = "DrvrHmiUsrIfSetgPen"
    fdx_groupid = 1282
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
    

class DrvrHmiUsrIfSetgSetg:
    de_name     = "DrvrHmiUsrIfSetg.Setg"
    fdx_name    = "DrvrHmiUsrIfSetgSetg"
    fdx_groupid = 1282
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
    

class DrvrMassgRunng:
    de_name     = "DrvrMassgRunng"
    fdx_name    = "DrvrMassgRunng"
    fdx_groupid = 1218
    class map:
        Off = 0
        On = 1
    

class DrvrPfmncLvl:
    de_name     = "DrvrPfmncLvl"
    fdx_name    = "DrvrPfmncLvl"
    fdx_groupid = 1143
    class map:
        Ukwn = 0
        DrvrLvl1 = 1
        DrvrLvl2 = 2
        DrvrLvl3 = 3
        DrvrLvl4 = 4
        DrvrLvl5 = 5
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class DrvrPfmncMonActvPen:
    de_name     = "DrvrPfmncMonActv.Pen"
    fdx_name    = "DrvrPfmncMonActvPen"
    fdx_groupid = 1275
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
    

class DrvrPfmncMonActvSts:
    de_name     = "DrvrPfmncMonActv.Sts"
    fdx_name    = "DrvrPfmncMonActvSts"
    fdx_groupid = 1275
    class map:
        Off = 0
        On = 1
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No Warning
# 1			1			WarnForDrvrNotActv	Warning For Driver Not Active	Warning for Inattentive Driver
# 2			2			WarnForDrvrTire		Warning For Driver Tire		Warning for Sleepy Driver
# 3			3			NotInUse			Not In Use			Not used
class DrvrPfmncWarnReq:
    de_name     = "DrvrPfmncWarnReq"
    fdx_name    = "DrvrPfmncWarnReq"
    fdx_groupid = 1143
    class map:
        NoWarn = 0
        WarnForDrvrNotActv = 1
        WarnForDrvrTire = 2
        NotInUse = 3
    

class DrvrSeatActvSpplFct:
    de_name     = "DrvrSeatActvSpplFct"
    fdx_name    = "DrvrSeatActvSpplFct"
    fdx_groupid = 1218
    class map:
        NotAvl = 0
        LumExtnAndLumHei = 1
        BackBlster = 2
        CushExtn = 3
        HdrestHeiAndHdrestTilt = 4
        MassgFct = 5
    

class DrvrSeatDispMassgFctMassgInten:
    de_name     = "DrvrSeatDispMassgFct.MassgInten"
    fdx_name    = "DrvrSeatDispMassgFctMassgInten"
    fdx_groupid = 1274
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2
    

class DrvrSeatDispMassgFctMassgProg:
    de_name     = "DrvrSeatDispMassgFct.MassgProg"
    fdx_name    = "DrvrSeatDispMassgFctMassgProg"
    fdx_groupid = 1274
    class map:
        Prog1 = 0
        Prog2 = 1
        Prog3 = 2
        Prog4 = 3
        Prog5 = 4
    

class DrvrSeatDispMassgFctMassgSpdLvl:
    de_name     = "DrvrSeatDispMassgFct.MassgSpdLvl"
    fdx_name    = "DrvrSeatDispMassgFctMassgSpdLvl"
    fdx_groupid = 1274
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2
    

class DrvrSeatDispMassgFctOnOff:
    de_name     = "DrvrSeatDispMassgFct.OnOff"
    fdx_name    = "DrvrSeatDispMassgFctOnOff"
    fdx_groupid = 1274
    class map:
        Off = 0
        On = 1
    

class DrvrSeatDispSpplFct:
    de_name     = "DrvrSeatDispSpplFct"
    fdx_name    = "DrvrSeatDispSpplFct"
    fdx_groupid = 1279
    class map:
        NotAvl = 0
        LumExtnAndLumHei = 1
        BackBlster = 2
        CushExtn = 3
        HdrestHeiAndHdrestTilt = 4
        MassgFct = 5
    

class DrvrSeatMassgFctMassgInten:
    de_name     = "DrvrSeatMassgFct.MassgInten"
    fdx_name    = "DrvrSeatMassgFctMassgInten"
    fdx_groupid = 1217
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2
    

class DrvrSeatMassgFctMassgProg:
    de_name     = "DrvrSeatMassgFct.MassgProg"
    fdx_name    = "DrvrSeatMassgFctMassgProg"
    fdx_groupid = 1217
    class map:
        Prog1 = 0
        Prog2 = 1
        Prog3 = 2
        Prog4 = 3
        Prog5 = 4
    

class DrvrSeatMassgFctMassgSpdLvl:
    de_name     = "DrvrSeatMassgFct.MassgSpdLvl"
    fdx_name    = "DrvrSeatMassgFctMassgSpdLvl"
    fdx_groupid = 1217
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2
    

class DrvrSeatMassgFctOnOff:
    de_name     = "DrvrSeatMassgFct.OnOff"
    fdx_name    = "DrvrSeatMassgFctOnOff"
    fdx_groupid = 1217
    class map:
        Off = 0
        On = 1
    

class DrvrSeatSwtAdjmtOfSpplFctHozlSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_name    = "DrvrSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2
    

class DrvrSeatSwtAdjmtOfSpplFctVertSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtAdjmtOfSpplFctVertSts"
    fdx_name    = "DrvrSeatSwtAdjmtOfSpplFctVertSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class DrvrSeatSwtHeiFrntSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtHeiFrntSts"
    fdx_name    = "DrvrSeatSwtHeiFrntSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class DrvrSeatSwtHeiSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtHeiSts"
    fdx_name    = "DrvrSeatSwtHeiSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class DrvrSeatSwtInclSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtInclSts"
    fdx_name    = "DrvrSeatSwtInclSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2
    

class DrvrSeatSwtSelnOfSpplFctSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtSelnOfSpplFctSts"
    fdx_name    = "DrvrSeatSwtSelnOfSpplFctSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class DrvrSeatSwtSldSts:
    de_name     = "DrvrSeatSwtSts.DrvrSeatSwtSldSts"
    fdx_name    = "DrvrSeatSwtSldSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2
    

class DrvrWLoadLvl:
    de_name     = "DrvrWLoadLvl"
    fdx_name    = "DrvrWLoadLvl"
    fdx_groupid = 1232
    class map:
        NoLvl = 0
        LoLvl = 1
        MidLvl = 2
        HiLvl = 3
    

class DstLong:
    de_name     = "IndcnUnit.DstLong"
    fdx_name    = "DstLong"
    fdx_groupid = 1282
    class map:
        km = 0
        miles = 1
        UkwnUnit = 2
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

class DstSho:
    de_name     = "IndcnUnit.DstSho"
    fdx_name    = "DstSho"
    fdx_groupid = 1282
    class map:
        Mtr = 0
        Ft = 1
        Yards = 2
        UkwnUnit = 3
    

# Unit: Unitless,  Range:0->2000
class DstToEmpty:
    de_name     = "DstToEmptyIndcd.DstToEmpty"
    fdx_name    = "DstToEmpty"
    fdx_groupid = 1232
    min = 0
    max = 2000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: m,  Range:0->4000
class DstToManvInDstToManvLocn:
    de_name     = "DstToManvLocn.DstToManv"
    fdx_name    = "DstToManvInDstToManvLocn"
    fdx_groupid = 1280
    min = 0
    max = 4000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: km,  Range:0->32000
class DstToSrv:
    de_name     = "DstToSrv"
    fdx_name    = "DstToSrv"
    fdx_groupid = 1232
    min = 0
    max = 32000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: km,  Range:0->9999.9, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class DstTrvld1:
    de_name     = "DstTrvld1"
    fdx_name    = "DstTrvld1"
    fdx_groupid = 1232
    min    = 0
    max    = 9999.9
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: km,  Range:0->9999.9, Resolution: (0.1*x+0.0, raw is unsigned, 0 bits )
class DstTrvld2:
    de_name     = "DstTrvld2"
    fdx_name    = "DstTrvld2"
    fdx_groupid = 1230
    min    = 0
    max    = 9999.9
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m,  Range:0->255
class DstTrvldHiResl:
    de_name     = "DstTrvldHiResl"
    fdx_name    = "DstTrvldHiResl"
    fdx_groupid = 1230
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Milg,  Range:0->1000000
class DstTrvldMst2:
    de_name     = "DstTrvldMst2"
    fdx_name    = "DstTrvldMst2"
    fdx_groupid = 1232
    min = 0
    max = 1000000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class DstUnit:
    de_name     = "DstToEmptyIndcd.DstUnit"
    fdx_name    = "DstUnit"
    fdx_groupid = 1232
    class map:
        km = 0
        miles = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class EasyInOutDrvrSeatAdjmtPen:
    de_name     = "EasyInOutDrvrSeatAdjmt.Pen"
    fdx_name    = "EasyInOutDrvrSeatAdjmtPen"
    fdx_groupid = 1276
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
    

class EasyInOutDrvrSeatAdjmtSts:
    de_name     = "EasyInOutDrvrSeatAdjmt.Sts"
    fdx_name    = "EasyInOutDrvrSeatAdjmtSts"
    fdx_groupid = 1276
    class map:
        Off = 0
        On = 1
    

# Unit: Unitless,  Range:0->15
class EgyLvlElecMai:
    de_name     = "VehModMngtGlbSafe1.EgyLvlElecMai"
    fdx_name    = "EgyLvlElecMai"
    fdx_groupid = 1212
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class EgyLvlElecSubtyp:
    de_name     = "VehModMngtGlbSafe1.EgyLvlElecSubtyp"
    fdx_name    = "EgyLvlElecSubtyp"
    fdx_groupid = 1212
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class EgySave:
    de_name     = "EgySave"
    fdx_name    = "EgySave"
    fdx_groupid = 1279
    class map:
        Off = 0
        On = 1
    

class EmgyAsscSts:
    de_name     = "EmgyAsscSts"
    fdx_name    = "EmgyAsscSts"
    fdx_groupid = 1215
    class map:
        Idle = 0
        AsscDataTrfInin = 1
        AsscDataTrf = 2
        AsscReqActv = 3
        AsscReqNotAprvd = 4
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

class EngCooltIndcnReq:
    de_name     = "EngCooltIndcnReq"
    fdx_name    = "EngCooltIndcnReq"
    fdx_groupid = 1373
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
    

# 0= High fluid level
# 1= Low fluid level
class EngCooltLvl:
    de_name     = "EngCooltLvl"
    fdx_name    = "EngCooltLvl"
    fdx_groupid = 1373
    class map:
        FldLvlHi = 0
        FldLvlLo = 1
    

# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class EngFuCns:
    de_name     = "EngFuCns"
    fdx_name    = "EngFuCns"
    fdx_groupid = 1373
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class EngFuCnsFild:
    de_name     = "EngFuCnsFild"
    fdx_name    = "EngFuCnsFild"
    fdx_groupid = 1376
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: hours,  Range:0->2000
class EngHrToSrv:
    de_name     = "EngHrToSrv"
    fdx_name    = "EngHrToSrv"
    fdx_groupid = 1232
    min = 0
    max = 2000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: rpm,  Range:0->16383, Resolution: (0.5*x+0.0, raw is unsigned, 15 bits )
class EngN:
    de_name     = "EngNSafe.EngN"
    fdx_name    = "EngN"
    fdx_groupid = 1370
    min    = 0
    max    = 16383
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255
class EngNChks:
    de_name     = "EngNSafe.EngNChks"
    fdx_name    = "EngNChks"
    fdx_groupid = 1370
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class EngNCntr:
    de_name     = "EngNSafe.EngNCntr"
    fdx_name    = "EngNCntr"
    fdx_groupid = 1370
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: RpmPerSec,  Range:-30000->30000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class EngNSafeEngNGrdt:
    de_name     = "EngNSafe.EngNGrdt"
    fdx_name    = "EngNSafeEngNGrdt"
    fdx_groupid = 1370
    min    = -30000
    max    = 30000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: %,  Range:0->120, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class EngOilLvl:
    de_name     = "EngOilLvl"
    fdx_name    = "EngOilLvl"
    fdx_groupid = 1375
    min    = 0
    max    = 120
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Engine oil level warning information
class EngOilLvlSts:
    de_name     = "EngOilLvlSts"
    fdx_name    = "EngOilLvlSts"
    fdx_groupid = 1375
    class map:
        OilLvlOk = 0
        OilLvlLo1 = 1
        OilLvlLo2 = 2
        OilLvlHi = 3
        OilLvlSrvRqrd = 4
        Resd = 5
    

# Engine oil pressure warning information
class EngOilPWarn:
    de_name     = "EngOilPWarn"
    fdx_name    = "EngOilPWarn"
    fdx_groupid = 1374
    class map:
        EngOilPOk = 0
        EngOilPNotOk = 1
    

# Unit: rpm,  Range:0->16383.5, Resolution: (0.5*x+0.0, raw is unsigned, 0 bits )
class EngSpdDispd:
    de_name     = "EngSpdDispd"
    fdx_name    = "EngSpdDispd"
    fdx_groupid = 1371
    min    = 0
    max    = 16383.5
    scale  = 0.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class EngStrtStopSetg:
    de_name     = "EngStrtStopSetg"
    fdx_name    = "EngStrtStopSetg"
    fdx_groupid = 1282
    class map:
        On = 0
        Off = 1
    

class EpbLampReq:
    de_name     = "EpbLampReq.EpbLampReq"
    fdx_name    = "EpbLampReq"
    fdx_groupid = 1374
    class map:
        On = 0
        Off = 1
        Flash2 = 2
        Flash3 = 3
    

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class EpbLampReqChks:
    de_name     = "EpbLampReq.EpbLampReqChks"
    fdx_name    = "EpbLampReqChks"
    fdx_groupid = 1374
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->15, Resolution: (1.0*x+0.0, raw is unsigned, 4 bits )
class EpbLampReqCntr:
    de_name     = "EpbLampReq.EpbLampReqCntr"
    fdx_name    = "EpbLampReqCntr"
    fdx_groupid = 1374
    min    = 0
    max    = 15
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Information on activation/ deactivation of Electronic Stability Control (ESC)
class EscSt:
    de_name     = "EscSt.EscSt"
    fdx_name    = "EscSt"
    fdx_groupid = 1372
    class map:
        Inin = 0
        Ok = 1
        TmpErr = 2
        PrmntErr = 3
        UsrOff = 4
    

# Unit: Unitless,  Range:0->255
class EscStChks:
    de_name     = "EscSt.EscStChks"
    fdx_name    = "EscStChks"
    fdx_groupid = 1372
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class EscStCntr:
    de_name     = "EscSt.EscStCntr"
    fdx_name    = "EscStCntr"
    fdx_groupid = 1372
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class EscWarnIndcnReq:
    de_name     = "EscWarnIndcnReq.EscWarnIndcnReq"
    fdx_name    = "EscWarnIndcnReq"
    fdx_groupid = 1374
    class map:
        EscWarnIndcnOnReq = 0
        EscWarnIndcnFlsgReq = 1
        Resd2 = 2
        EscWarnIndcnOffReq = 3
    

# Unit: Unitless,  Range:0->255
class EscWarnIndcnReqChks:
    de_name     = "EscWarnIndcnReq.EscWarnIndcnReqChks"
    fdx_name    = "EscWarnIndcnReqChks"
    fdx_groupid = 1374
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class EscWarnIndcnReqCntr:
    de_name     = "EscWarnIndcnReq.EscWarnIndcnReqCntr"
    fdx_name    = "EscWarnIndcnReqCntr"
    fdx_groupid = 1374
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ExtrMirrFoldSetgPen:
    de_name     = "ExtrMirrFoldSetg.Pen"
    fdx_name    = "ExtrMirrFoldSetgPen"
    fdx_groupid = 1282
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
    

class ExtrMirrFoldSetgSts:
    de_name     = "ExtrMirrFoldSetg.Sts"
    fdx_name    = "ExtrMirrFoldSetgSts"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ExtrMirrTiltSetg2IdPen:
    de_name     = "ExtrMirrTiltSetg2.IdPen"
    fdx_name    = "ExtrMirrTiltSetg2IdPen"
    fdx_groupid = 1282
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
    

# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class ExtrMirrTiltSetg2MirrDrvr:
    de_name     = "ExtrMirrTiltSetg2.MirrDrvr"
    fdx_name    = "ExtrMirrTiltSetg2MirrDrvr"
    fdx_groupid = 1282
    class map:
        On = 0
        Off = 1
    

# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class ExtrMirrTiltSetg2MirrPass:
    de_name     = "ExtrMirrTiltSetg2.MirrPass"
    fdx_name    = "ExtrMirrTiltSetg2MirrPass"
    fdx_groupid = 1282
    class map:
        On = 0
        Off = 1
    

# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class FanForCmptmtTRunng:
    de_name     = "CmptmtTFrnt.FanForCmptmtTRunng"
    fdx_name    = "FanForCmptmtTRunng"
    fdx_groupid = 1279
    class map:
        Rst = 0
        Set = 1
    

class FltEgyCnsWdSts:
    de_name     = "VehModMngtGlbSafe1.FltEgyCnsWdSts"
    fdx_name    = "FltEgyCnsWdSts"
    fdx_groupid = 1212
    class map:
        NoFlt = 0
        Flt = 1
    

class FltIndcrTurnLeFrnt:
    de_name     = "FltIndcrTurnLeFrnt"
    fdx_name    = "FltIndcrTurnLeFrnt"
    fdx_groupid = 1220
    class map:
        NoFlt = 0
        Flt = 1
    

class FltIndcrTurnLeRe:
    de_name     = "FltIndcrTurnLeRe"
    fdx_name    = "FltIndcrTurnLeRe"
    fdx_groupid = 1220
    class map:
        NoFlt = 0
        Flt = 1
    

class FltIndcrTurnRiFrnt:
    de_name     = "FltIndcrTurnRiFrnt"
    fdx_name    = "FltIndcrTurnRiFrnt"
    fdx_groupid = 1220
    class map:
        NoFlt = 0
        Flt = 1
    

class FltIndcrTurnRiRe:
    de_name     = "FltIndcrTurnRiRe"
    fdx_name    = "FltIndcrTurnRiRe"
    fdx_groupid = 1220
    class map:
        NoFlt = 0
        Flt = 1
    

class FltOfLiDaytiRunngRi:
    de_name     = "FltOfLiDaytiRunngRi"
    fdx_name    = "FltOfLiDaytiRunngRi"
    fdx_groupid = 1220
    class map:
        NoFlt = 0
        Flt = 1
    

# Unit: Unitless,  Range:0->65535
class FRNetworkStatus:
    de_name     = "FRNetworkStatus"
    fdx_name    = "FRNetworkStatus"
    fdx_groupid = 1366
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class FrntAxleWarn:
    de_name     = "FrntAxleWarn"
    fdx_name    = "FrntAxleWarn"
    fdx_groupid = 1217
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        Resd = 3
    

class FrntWiprLvrReq2FrntWiprLvrCmd1:
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrCmd1"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrCmd1"
    fdx_groupid = 1217
    class map:
        FrntWiprOff = 0
        FrntWiprSngStk = 1
        FrntWiprIntm = 2
        FrntWiprContnsLoSpd = 3
        FrntWiprContnsHiSpd = 4
    

# Unit: Unitless,  Range:0->3
class FrntWiprLvrReq2FrntWiprLvrCntr:
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrCntr"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrCntr"
    fdx_groupid = 1217
    min = 0
    max = 3
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class FrntWiprLvrReq2FrntWiprLvrCrc:
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrCrc"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrCrc"
    fdx_groupid = 1217
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class FrntWiprLvrReq2FrntWiprLvrQf:
    de_name     = "FrntWiprLvrReq2.FrntWiprLvrQf"
    fdx_name    = "FrntWiprLvrReq2FrntWiprLvrQf"
    fdx_groupid = 1217
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3
    

# LperHectoKiloM = L/100Km
# KiloMPerL = Km/L
# MilePerGallonEngland = MPG (UK)
# MilePerGallonUsa = MPG (US)
class FuCnsUnit:
    de_name     = "IndcnUnit.FuCnsUnit"
    fdx_name    = "FuCnsUnit"
    fdx_groupid = 1282
    class map:
        LperHectoKiloM = 0
        KiloMPerL = 1
        MilePerGallonEngland = 2
        MilePerGallonUsa = 3
        UkwnUnit = 4
    

class FuHeatrActv:
    de_name     = "FuHeatrActv"
    fdx_name    = "FuHeatrActv"
    fdx_groupid = 1218
    class map:
        Off = 0
        On = 1
    

# Unit: MicroL,  Range:0->51000, Resolution: (200.0*x+0.0, raw is unsigned, 0 bits )
class FuHeatrFuCns1:
    de_name     = "FuHeatrFuCns1"
    fdx_name    = "FuHeatrFuCns1"
    fdx_groupid = 1217
    min    = 0
    max    = 51000
    scale  = 200.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Litre,  Range:0->1.27, Resolution: (0.01*x+0.0, raw is unsigned, 0 bits )
class FuHeatrFuCnsDurgCyc1:
    de_name     = "FuHeatrFuCnsDurgCyc1"
    fdx_name    = "FuHeatrFuCnsDurgCyc1"
    fdx_groupid = 1218
    min    = 0
    max    = 1.27
    scale  = 0.01
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class FuLvlIndcdQly:
    de_name     = "FuLvlIndcd.GenQF"
    fdx_name    = "FuLvlIndcdQly"
    fdx_groupid = 1230
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

# Unit: Litre,  Range:0->204.6, Resolution: (0.2*x+0.0, raw is unsigned, 10 bits )
class FuLvlIndcdVal:
    de_name     = "FuLvlIndcd.FuLvlValFromFuTbl"
    fdx_name    = "FuLvlIndcdVal"
    fdx_groupid = 1230
    min    = 0
    max    = 204.6
    scale  = 0.2
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class FuLvlLoIndcn:
    de_name     = "FuLvlLoIndcn"
    fdx_name    = "FuLvlLoIndcn"
    fdx_groupid = 1232
    class map:
        NoTrig = 0
        Trig = 1
    

class FuLvlLoWarn:
    de_name     = "FuLvlLoWarn"
    fdx_name    = "FuLvlLoWarn"
    fdx_groupid = 1232
    class map:
        NoTrig = 0
        Trig = 1
    

class GearIndcn:
    de_name     = "GearIndcnRec.GearIndcn"
    fdx_name    = "GearIndcn"
    fdx_groupid = 1369
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
    

class GearShiftIndcn:
    de_name     = "GearIndcnRec.GearShiftIndcn"
    fdx_name    = "GearShiftIndcn"
    fdx_groupid = 1369
    class map:
        NoShiftIndcn = 0
        ShiftUpIndcn = 1
        CoolShiftIndcn = 2
        ShiftDwnIndcn = 3
    

class GearTarIndcn:
    de_name     = "GearIndcnRec.GearTarIndcn"
    fdx_name    = "GearTarIndcn"
    fdx_groupid = 1369
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
    

# Unit: Unitless,  Range:0->102.3, Resolution: (0.1*x+0.0, raw is unsigned, 10 bits )
class GrdForFuEco:
    de_name     = "CchForFuEco.GrdForFuEco"
    fdx_name    = "GrdForFuEco"
    fdx_groupid = 1371
    min    = 0
    max    = 102.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class HdrestFoldReq2:
    de_name     = "HdrestFoldReq2"
    fdx_name    = "HdrestFoldReq2"
    fdx_groupid = 1280
    def HdrestFoldReq2_r2p(raw):
        return raw
    def HdrestFoldReq2_p2r(physical):
        return physical

class HeatrDurgDrvgReqd:
    de_name     = "HeatrDurgDrvgReqd"
    fdx_name    = "HeatrDurgDrvgReqd"
    fdx_groupid = 1276
    class map:
        Off = 0
        On = 1
        Aut = 2
    

class HiQlyInTireCircumCalByNav:
    de_name     = "TireCircumCalByNav.HiQly"
    fdx_name    = "HiQlyInTireCircumCalByNav"
    fdx_groupid = 1280
    class map:
        No = 0
        Yes = 1
    

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
    class map:
        Flr = 0
        Vent = 1
        Defrst = 2
        FlrDefrst = 3
        FlrVent = 4
        VentDefrst = 5
        FlrVentDefrst = 6
        Aut = 7
    

class HmiCmptmtCoolgReq:
    de_name     = "HmiCmptmtCoolgReq"
    fdx_name    = "HmiCmptmtCoolgReq"
    fdx_groupid = 1274
    class map:
        Off = 0
        Auto = 1
    

# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowFirstLe:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe"
    fdx_name    = "HmiCmptmtTSpForRowFirstLe"
    fdx_groupid = 1274
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowFirstRi:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi"
    fdx_name    = "HmiCmptmtTSpForRowFirstRi"
    fdx_groupid = 1274
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowSecLe:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowSecLe"
    fdx_name    = "HmiCmptmtTSpForRowSecLe"
    fdx_groupid = 1274
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Deg,  Range:15->30.5, Resolution: (0.5*x+15.0, raw is unsigned, 5 bits )
class HmiCmptmtTSpForRowSecRi:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpForRowSecRi"
    fdx_name    = "HmiCmptmtTSpForRowSecRi"
    fdx_groupid = 1274
    min    = 15
    max    = 30.5
    scale  = 0.5
    offset = 15.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class HmiCmptmtTSpSpclForRowFirstLe:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe"
    fdx_name    = "HmiCmptmtTSpSpclForRowFirstLe"
    fdx_groupid = 1274
    class map:
        Norm = 0
        Lo = 1
        Hi = 2
    

class HmiCmptmtTSpSpclForRowFirstRi:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi"
    fdx_name    = "HmiCmptmtTSpSpclForRowFirstRi"
    fdx_groupid = 1274
    class map:
        Norm = 0
        Lo = 1
        Hi = 2
    

class HmiCmptmtTSpSpclForRowSecLe:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowSecLe"
    fdx_name    = "HmiCmptmtTSpSpclForRowSecLe"
    fdx_groupid = 1274
    class map:
        Norm = 0
        Lo = 1
        Hi = 2
    

class HmiCmptmtTSpSpclForRowSecRi:
    de_name     = "HmiCmptmtTSp.HmiCmptmtTSpSpclForRowSecRi"
    fdx_name    = "HmiCmptmtTSpSpclForRowSecRi"
    fdx_groupid = 1274
    class map:
        Norm = 0
        Lo = 1
        Hi = 2
    

class HmiDefrstElecForMirrReq:
    de_name     = "HmiDefrstElecReq.MirrElecReq"
    fdx_name    = "HmiDefrstElecForMirrReq"
    fdx_groupid = 1279
    class map:
        Off = 0
        On = 1
        AutOn = 2
    

class HmiDefrstElecFrntReq:
    de_name     = "HmiDefrstElecReq.FrntElecReq"
    fdx_name    = "HmiDefrstElecFrntReq"
    fdx_groupid = 1279
    class map:
        Off = 0
        On = 1
        AutOn = 2
    

class HmiDefrstElecReReq:
    de_name     = "HmiDefrstElecReq.ReElecReq"
    fdx_name    = "HmiDefrstElecReReq"
    fdx_groupid = 1279
    class map:
        Off = 0
        On = 1
        AutOn = 2
    

class HmiDefrstFrntSts:
    de_name     = "HmiDefrstElecSts.Frnt"
    fdx_name    = "HmiDefrstFrntSts"
    fdx_groupid = 1220
    class map:
        Off = 0
        On = 1
        Limited = 2
        NotAvailable = 3
        TmrOff = 4
        AutoCdn = 5
    

class HmiDefrstMaxReq:
    de_name     = "HmiDefrstMaxReq"
    fdx_name    = "HmiDefrstMaxReq"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
        AutOn = 2
    

class HmiDfrstReSts:
    de_name     = "HmiDefrstElecSts.Re"
    fdx_name    = "HmiDfrstReSts"
    fdx_groupid = 1220
    class map:
        Off = 0
        On = 1
        Limited = 2
        NotAvailable = 3
        TmrOff = 4
        AutoCdn = 5
    

class HmiHvacFanLvlFrnt:
    de_name     = "HmiHvacFanLvlFrnt"
    fdx_name    = "HmiHvacFanLvlFrnt"
    fdx_groupid = 1276
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
    

class HmiHvacFanLvlRe:
    de_name     = "HmiHvacFanLvlRe"
    fdx_name    = "HmiHvacFanLvlRe"
    fdx_groupid = 1276
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
    class map:
        Aut = 0
        AutWithAirQly = 1
        RecircFull = 2
    

class HmiHvacReCtrl:
    de_name     = "HmiHvacReCtrl"
    fdx_name    = "HmiHvacReCtrl"
    fdx_groupid = 1276
    class map:
        Off = 0
        OffWithNoOccpt = 1
        On = 2
    

class HmiMirrDefrstSts:
    de_name     = "HmiDefrstElecSts.Mirrr"
    fdx_name    = "HmiMirrDefrstSts"
    fdx_groupid = 1220
    class map:
        Off = 0
        On = 1
        Limited = 2
        NotAvailable = 3
        TmrOff = 4
        AutoCdn = 5
    

class HmiSeatClimaExtdHmiSeatVentnForRowSecLe:
    de_name     = "HmiSeatClimaExtd.HmiSeatVentnForRowSecLe"
    fdx_name    = "HmiSeatClimaExtdHmiSeatVentnForRowSecLe"
    fdx_groupid = 1279
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

class HmiSeatClimaExtdHmiSeatVentnForRowSecRi:
    de_name     = "HmiSeatClimaExtd.HmiSeatVentnForRowSecRi"
    fdx_name    = "HmiSeatClimaExtdHmiSeatVentnForRowSecRi"
    fdx_groupid = 1279
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

class HmiSeatHeatgForRowFirstLe:
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowFirstLe"
    fdx_name    = "HmiSeatHeatgForRowFirstLe"
    fdx_groupid = 1279
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

class HmiSeatHeatgForRowFirstRi:
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowFirstRi"
    fdx_name    = "HmiSeatHeatgForRowFirstRi"
    fdx_groupid = 1279
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

class HmiSeatHeatgForRowSecLe:
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowSecLe"
    fdx_name    = "HmiSeatHeatgForRowSecLe"
    fdx_groupid = 1279
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

class HmiSeatHeatgForRowSecRi:
    de_name     = "HmiSeatClima.HmiSeatHeatgForRowSecRi"
    fdx_name    = "HmiSeatHeatgForRowSecRi"
    fdx_groupid = 1279
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

class HmiSeatVentnForRowFirstLe:
    de_name     = "HmiSeatClima.HmiSeatVentnForRowFirstLe"
    fdx_name    = "HmiSeatVentnForRowFirstLe"
    fdx_groupid = 1279
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

class HmiSeatVentnForRowFirstRi:
    de_name     = "HmiSeatClima.HmiSeatVentnForRowFirstRi"
    fdx_name    = "HmiSeatVentnForRowFirstRi"
    fdx_groupid = 1279
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

# Status of the door.
class HoodSts:
    de_name     = "HoodSts"
    fdx_name    = "HoodSts"
    fdx_groupid = 1212
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2
    

# Unit: hours,  Range:0->23
class Hr:
    de_name     = "TiAndDateIndcn.Hr1"
    fdx_name    = "Hr"
    fdx_groupid = 1217
    min = 0
    max = 23
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: hours,  Range:0->23
class Hr1:
    de_name     = "SetTiAndDate.Hour"
    fdx_name    = "Hr1"
    fdx_groupid = 1277
    min = 0
    max = 23
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class HudActvReqPen:
    de_name     = "HudActvReq.Pen"
    fdx_name    = "HudActvReqPen"
    fdx_groupid = 1275
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
    

class HudActvReqSts:
    de_name     = "HudActvReq.Sts"
    fdx_name    = "HudActvReqSts"
    fdx_groupid = 1275
    class map:
        Off = 0
        On = 1
    

class HudActvSts:
    de_name     = "HudActvSts"
    fdx_name    = "HudActvSts"
    fdx_groupid = 1230
    class map:
        Off = 0
        On = 1
    

class HudAdjmtReq:
    de_name     = "HudAdjmtReq"
    fdx_name    = "HudAdjmtReq"
    fdx_groupid = 1280
    class map:
        Off = 0
        On = 1
    

class HudDiagcHudCircShoSts:
    de_name     = "HudDiagc.HudCircShoSts"
    fdx_name    = "HudDiagcHudCircShoSts"
    fdx_groupid = 1230
    class map:
        NoErr = 0
        Err = 1
    

class HudDiagcHudCricOpenSts:
    de_name     = "HudDiagc.HudCricOpenSts"
    fdx_name    = "HudDiagcHudCricOpenSts"
    fdx_groupid = 1230
    class map:
        NoErr = 0
        Err = 1
    

class HudDiagcHudTSts:
    de_name     = "HudDiagc.HudTSts"
    fdx_name    = "HudDiagcHudTSts"
    fdx_groupid = 1230
    class map:
        NoErr = 0
        Err = 1
    

class HudDiagcImgHudErrSts:
    de_name     = "HudDiagc.ImgHudErrSts"
    fdx_name    = "HudDiagcImgHudErrSts"
    fdx_groupid = 1230
    class map:
        NoErr = 0
        Err = 1
    

class HudDiagcImgHudTmpNotAvlSts:
    de_name     = "HudDiagc.ImgHudTmpNotAvlSts"
    fdx_name    = "HudDiagcImgHudTmpNotAvlSts"
    fdx_groupid = 1230
    class map:
        Avl = 0
        NotAvl = 1
    

class HudErgoSetgReq:
    de_name     = "HudErgoSetgReq"
    fdx_name    = "HudErgoSetgReq"
    fdx_groupid = 1275
    class map:
        Off = 0
        On = 1
    

class HudSts:
    de_name     = "HudSts"
    fdx_name    = "HudSts"
    fdx_groupid = 1230
    class map:
        HudStsAvl = 0
        HudStsCalMod = 1
        HudStsTmpNotAvl = 2
        HudStsErr = 3
        Resd1 = 4
        Resd2 = 5
        Resd3 = 6
    

class HudVisFctSetgHudFct00:
    de_name     = "HudVisFctSetg.HudFct00"
    fdx_name    = "HudVisFctSetgHudFct00"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct01:
    de_name     = "HudVisFctSetg.HudFct01"
    fdx_name    = "HudVisFctSetgHudFct01"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct02:
    de_name     = "HudVisFctSetg.HudFct02"
    fdx_name    = "HudVisFctSetgHudFct02"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct03:
    de_name     = "HudVisFctSetg.HudFct03"
    fdx_name    = "HudVisFctSetgHudFct03"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct04:
    de_name     = "HudVisFctSetg.HudFct04"
    fdx_name    = "HudVisFctSetgHudFct04"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct05:
    de_name     = "HudVisFctSetg.HudFct05"
    fdx_name    = "HudVisFctSetgHudFct05"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct06:
    de_name     = "HudVisFctSetg.HudFct06"
    fdx_name    = "HudVisFctSetgHudFct06"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct07:
    de_name     = "HudVisFctSetg.HudFct07"
    fdx_name    = "HudVisFctSetgHudFct07"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct08:
    de_name     = "HudVisFctSetg.HudFct08"
    fdx_name    = "HudVisFctSetgHudFct08"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct09:
    de_name     = "HudVisFctSetg.HudFct09"
    fdx_name    = "HudVisFctSetgHudFct09"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct10:
    de_name     = "HudVisFctSetg.HudFct10"
    fdx_name    = "HudVisFctSetgHudFct10"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct11:
    de_name     = "HudVisFctSetg.HudFct11"
    fdx_name    = "HudVisFctSetgHudFct11"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct12:
    de_name     = "HudVisFctSetg.HudFct12"
    fdx_name    = "HudVisFctSetgHudFct12"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct13:
    de_name     = "HudVisFctSetg.HudFct13"
    fdx_name    = "HudVisFctSetgHudFct13"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct14:
    de_name     = "HudVisFctSetg.HudFct14"
    fdx_name    = "HudVisFctSetgHudFct14"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct15:
    de_name     = "HudVisFctSetg.HudFct15"
    fdx_name    = "HudVisFctSetgHudFct15"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct16:
    de_name     = "HudVisFctSetg.HudFct16"
    fdx_name    = "HudVisFctSetgHudFct16"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct17:
    de_name     = "HudVisFctSetg.HudFct17"
    fdx_name    = "HudVisFctSetgHudFct17"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct18:
    de_name     = "HudVisFctSetg.HudFct18"
    fdx_name    = "HudVisFctSetgHudFct18"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class HudVisFctSetgHudFct19:
    de_name     = "HudVisFctSetg.HudFct19"
    fdx_name    = "HudVisFctSetgHudFct19"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class HudVisFctSetgPen:
    de_name     = "HudVisFctSetg.Pen"
    fdx_name    = "HudVisFctSetgPen"
    fdx_groupid = 1282
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
    

# Unit: Kg / h,  Range:0->1000, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class HvacAirMFlowEstimd:
    de_name     = "HvacAirMFlowEstimd"
    fdx_name    = "HvacAirMFlowEstimd"
    fdx_groupid = 1214
    min    = 0
    max    = 1000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class HznRstExtd:
    de_name     = "HznRstExtd"
    fdx_name    = "HznRstExtd"
    fdx_groupid = 1377
    def HznRstExtd_r2p(raw):
        return raw
    def HznRstExtd_p2r(physical):
        return physical

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IdPenForUnits:
    de_name     = "IndcnUnit.IdPenForUnit"
    fdx_name    = "IdPenForUnits"
    fdx_groupid = 1282
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
    

class IndcnOfParkAssiSts:
    de_name     = "IndcnOfParkAssiSts"
    fdx_name    = "IndcnOfParkAssiSts"
    fdx_groupid = 1327
    class map:
        Off = 0
        On = 1
    

class IndcnOfPrkgAutSts:
    de_name     = "IndcnOfPrkgAutSts"
    fdx_name    = "IndcnOfPrkgAutSts"
    fdx_groupid = 1327
    class map:
        Off = 0
        PrkgTypAutSeln = 1
        PrkgTypPara = 2
        PrkgTypPerp = 3
        PrkgOutManv = 4
    

# DateFmt1 = YMD
# DateFmt2 = DMY
# DateFmt3 = MDY
class IndcnUnitDateFmt:
    de_name     = "IndcnUnit.DateFmt"
    fdx_name    = "IndcnUnitDateFmt"
    fdx_groupid = 1282
    class map:
        DateFmt1 = 0
        DateFmt2 = 1
        DateFmt3 = 2
        UkwnFmt = 3
    

class IndcrDisp1WdSts:
    de_name     = "IndcrDisp1WdSts"
    fdx_name    = "IndcrDisp1WdSts"
    fdx_groupid = 1213
    class map:
        Off = 0
        LeOn = 1
        RiOn = 2
        LeAndRiOn = 3
    

class IndcrTurnSts1WdSts:
    de_name     = "IndcrTurnSts1WdSts"
    fdx_name    = "IndcrTurnSts1WdSts"
    fdx_groupid = 1212
    class map:
        Off = 0
        LeOn = 1
        RiOn = 2
        LeAndRiOn = 3
    

# Unit: Unitless,  Range:0->65535
class IniValSigCfgIDBackboneFR:
    de_name     = "IniValSigCfgIDBackboneFR"
    fdx_name    = "IniValSigCfgIDBackboneFR"
    fdx_groupid = 1244
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class IntrBriSts:
    de_name     = "IntrBriSts"
    fdx_name    = "IntrBriSts"
    fdx_groupid = 1212
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class IntrLiAmbLiSetgForLiInten:
    de_name     = "IntrLiAmbLiSetg.LiInten"
    fdx_name    = "IntrLiAmbLiSetgForLiInten"
    fdx_groupid = 1282
    class map:
        Off = 0
        IntenLo = 1
        IntenHi = 2
    

# Amibiance LIgt color setting:
# 0 = Temperature
# 1 = Off
# 2 - 8 = Colour 1 - 7
class IntrLiAmbLiSetgForLiTintg:
    de_name     = "IntrLiAmbLiSetg.LiTintg"
    fdx_name    = "IntrLiAmbLiSetgForLiTintg"
    fdx_groupid = 1282
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
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrLiAmbLiSetgPen:
    de_name     = "IntrLiAmbLiSetg.Pen"
    fdx_name    = "IntrLiAmbLiSetgPen"
    fdx_groupid = 1282
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
    

class IntrLiSurrndgsLiSetgForLiInten:
    de_name     = "IntrLiSurrndgsLiSetgLi.LiInten"
    fdx_name    = "IntrLiSurrndgsLiSetgForLiInten"
    fdx_groupid = 1282
    class map:
        Off = 0
        IntenLo = 1
        IntenHi = 2
    

# 0 = Full
# 1 = Reduced
class IntrLiSurrndgsLiSetgForLiLvl:
    de_name     = "IntrLiSurrndgsLiSetgLi.LiLvl"
    fdx_name    = "IntrLiSurrndgsLiSetgForLiLvl"
    fdx_groupid = 1282
    class map:
        LvlFull = 0
        LvlReduced = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrLiSurrndgsLiSetgPen:
    de_name     = "IntrLiSurrndgsLiSetgLi.Pen"
    fdx_name    = "IntrLiSurrndgsLiSetgPen"
    fdx_groupid = 1282
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
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class IntrMirrTintgSetgPen:
    de_name     = "IntrMirrTintgSetg.Pen"
    fdx_name    = "IntrMirrTintgSetgPen"
    fdx_groupid = 1279
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
    

class IntrMirrTintgSetgSts:
    de_name     = "IntrMirrTintgSetg.MirrDimLvl"
    fdx_name    = "IntrMirrTintgSetgSts"
    fdx_groupid = 1279
    class map:
        Normal = 0
        Dark = 1
        Light = 2
        Inhibit = 3
    

# Warning status	                                
# == 0	Calibration status OK (Normal IHU behavior) 
# == 1	Calibration not possible (All 4 rolling wheels shall be grey, menu item for recalibration of iTPMS system shall be unavailable and gray).
class iTPMSCalPsbl:
    de_name     = "iTPMSCalPsbl"
    fdx_name    = "iTPMSCalPsbl"
    fdx_groupid = 1377
    class map:
        CalPsbl = 0
        CalNoPsbl = 1
    

# Calibration status
class iTPMSCalSts:
    de_name     = "iTPMSCalSts"
    fdx_name    = "iTPMSCalSts"
    fdx_groupid = 1377
    class map:
        NoCalReq = 0
        CalOn = 1
        CalCmpl = 2
        CalFaild = 3
        Resd1 = 4
        Resd2 = 5
        Resd3 = 6
    

# iTPMS system status and warnings
class iTPMSTirePMSts:
    de_name     = "iTPMSTirePMSts"
    fdx_name    = "iTPMSTirePMSts"
    fdx_groupid = 1377
    class map:
        NoWarn = 0
        CmnWarn = 1
        WarnFL = 2
        WarnFR = 3
        WarnRL = 4
        WarnRR = 5
        SysUnAvi = 6
        SysFailr = 7
    

class KeyLostWarnIndcn:
    de_name     = "KeyLostWarnIndcn"
    fdx_name    = "KeyLostWarnIndcn"
    fdx_groupid = 1213
    class map:
        KeyLostNoMsg = 0
        KeyMsgAndSoundLost = 1
        KeyMsgLost = 2
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class KeyProfMpgUpdForIdPen:
    de_name     = "KeyProfMpgUpd.KeyProfMpgUpdForIdPen"
    fdx_name    = "KeyProfMpgUpdForIdPen"
    fdx_groupid = 1278
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
    

class KeyProfMpgUpdKeyProfMpgUpdOff:
    de_name     = "KeyProfMpgUpd.KeyProfMpgUpdOff"
    fdx_name    = "KeyProfMpgUpdKeyProfMpgUpdOff"
    fdx_groupid = 1278
    def KeyProfMpgUpdKeyProfMpgUpdOff_r2p(raw):
        return raw
    def KeyProfMpgUpdKeyProfMpgUpdOff_p2r(physical):
        return physical

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
    

class KeyReadStsToProfCtrlBoolean:
    de_name     = "KeyReadStsToProfCtrl.Boolean"
    fdx_name    = "KeyReadStsToProfCtrlBoolean"
    fdx_groupid = 1214
    def KeyReadStsToProfCtrlBoolean_r2p(raw):
        return raw
    def KeyReadStsToProfCtrlBoolean_p2r(physical):
        return physical

class KeyReadStsToProfCtrlKeyId:
    de_name     = "KeyReadStsToProfCtrl.KeyId"
    fdx_name    = "KeyReadStsToProfCtrlKeyId"
    fdx_groupid = 1214
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
    

class KeyRmnIndcn:
    de_name     = "KeyRmnIndcn"
    fdx_name    = "KeyRmnIndcn"
    fdx_groupid = 1213
    class map:
        Off = 0
        On = 1
    

class KeySpdWarn:
    de_name     = "KeySpdWarn"
    fdx_name    = "KeySpdWarn"
    fdx_groupid = 1230
    class map:
        NoTrig = 0
        Trig = 1
    

# Used be Connected service booking to inform Driver information about the current status of a service booking.
class LampSuppSrv:
    de_name     = "LampSuppSrv"
    fdx_name    = "LampSuppSrv"
    fdx_groupid = 1280
    class map:
        NoSrv = 0
        Srv = 1
        CfmdSrv = 2
        SrvPassd = 3
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LaneChgWarnActvPen:
    de_name     = "LaneChgWarnActv.Pen"
    fdx_name    = "LaneChgWarnActvPen"
    fdx_groupid = 1282
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
    

class LaneChgWarnActvSts:
    de_name     = "LaneChgWarnActv.Sts"
    fdx_name    = "LaneChgWarnActvSts"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoLaneDetd	No Lane Detedcted		No lane tracked
# 1			1			LaneLeDetd	Lane Left Detected		Left lane marking tracked
# 2			2			LaneRiDetd	Lane Right Detected	Right lane marking tracked
# 3			3			LaneLeAndRiDetd	Lane Left And Right Detected	Left and right lane marking tracked
class LaneDetnStsForAutDrv:
    de_name     = "LaneDetnStsForAutDrv"
    fdx_name    = "LaneDetnStsForAutDrv"
    fdx_groupid = 1142
    class map:
        NoLaneDetd = 0
        LaneLeDetd = 1
        LaneRiDetd = 2
        LaneLeAndRiDetd = 3
    

# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoLaneDetd	No Lane Detedcted		No lane tracked
# 1			1			LaneLeDetd	Lane Left Detected		Left lane marking tracked
# 2			2			LaneRiDetd	Lane Right Detected	Right lane marking tracked
# 3			3			LaneLeAndRiDetd	Lane Left And Right Detected	Left and right lane marking tracked
class LaneDetnStsForLaneKeepAid:
    de_name     = "LaneDetnStsForLaneKeepAid"
    fdx_name    = "LaneDetnStsForLaneKeepAid"
    fdx_groupid = 1142
    class map:
        NoLaneDetd = 0
        LaneLeDetd = 1
        LaneRiDetd = 2
        LaneLeAndRiDetd = 3
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LaneKeepAidActvPen:
    de_name     = "LaneKeepAidActv.Pen"
    fdx_name    = "LaneKeepAidActvPen"
    fdx_groupid = 1282
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
    

class LaneKeepAidActvSts:
    de_name     = "LaneKeepAidActv.Sts"
    fdx_name    = "LaneKeepAidActvSts"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

class LatMovmtWarn:
    de_name     = "LatMovmtWarn"
    fdx_name    = "LatMovmtWarn"
    fdx_groupid = 1139
    class map:
        NoWarn = 0
        WarnLe = 1
        WarnRi = 2
    

class LcmaOn1:
    de_name     = "LcmaOn1"
    fdx_name    = "LcmaOn1"
    fdx_groupid = 1143
    class map:
        StrtUpOn = 0
        On = 1
        Off = 2
        TrlrOff = 3
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiAdpvReqPen:
    de_name     = "LiExtReq1WdReq1.Pen"
    fdx_name    = "LiAdpvReqPen"
    fdx_groupid = 1278
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
    

class LiAdpvReqSts:
    de_name     = "LiExtReq1WdReq1.Sts"
    fdx_name    = "LiAdpvReqSts"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiAutTranReqPen:
    de_name     = "LiExtReq2WdReq1.Pen"
    fdx_name    = "LiAutTranReqPen"
    fdx_groupid = 1280
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
    

# SntvyLo = Sensitivity Low, light transition happens later
# SntvyLo = Sensitivity Norma 
# SntvyLo = Sensitivity High, light transition happens earlier
class LiAutTranReqSts:
    de_name     = "LiExtReq2WdReq1.Sts"
    fdx_name    = "LiAutTranReqSts"
    fdx_groupid = 1280
    class map:
        SnvtyLo = 0
        SnvtyNorm = 1
        SnvtyHi = 2
        Resd = 3
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiBeamHiAuxReqPen:
    de_name     = "LiExtReq1WdReq4.Pen"
    fdx_name    = "LiBeamHiAuxReqPen"
    fdx_groupid = 1278
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
    

class LiBeamHiAuxReqSts:
    de_name     = "LiExtReq1WdReq4.Sts"
    fdx_name    = "LiBeamHiAuxReqSts"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiCornrgReqPen:
    de_name     = "LiExtReq1WdReq6.Pen"
    fdx_name    = "LiCornrgReqPen"
    fdx_groupid = 1278
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
    

class LiCornrgReqSts:
    de_name     = "LiExtReq1WdReq6.Sts"
    fdx_name    = "LiCornrgReqSts"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiDaytiRunngReqPen:
    de_name     = "LiExtReq1WdReq2.Pen"
    fdx_name    = "LiDaytiRunngReqPen"
    fdx_groupid = 1278
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
    

class LiDaytiRunngReqSts:
    de_name     = "LiExtReq1WdReq2.Sts"
    fdx_name    = "LiDaytiRunngReqSts"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

class LiDrvrFltIndcrTurn:
    de_name     = "LiDrvrFltIndcrTurn"
    fdx_name    = "LiDrvrFltIndcrTurn"
    fdx_groupid = 1218
    class map:
        NoFlt = 0
        Flt = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiExtReq1WdReq5IdPen:
    de_name     = "LiExtReq1WdReq5.IdPen"
    fdx_name    = "LiExtReq1WdReq5IdPen"
    fdx_groupid = 1279
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
    

class LiExtReq1WdReq5SlowNormFast:
    de_name     = "LiExtReq1WdReq5.SlowNormFast"
    fdx_name    = "LiExtReq1WdReq5SlowNormFast"
    fdx_groupid = 1279
    class map:
        Ukwn = 0
        Slow = 1
        Norm = 2
        Fast = 3
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiHomeLvngReqPen:
    de_name     = "LiExtSafe1WdReq2.Pen"
    fdx_name    = "LiHomeLvngReqPen"
    fdx_groupid = 1278
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
    

class LiHomeLvngReqSts:
    de_name     = "LiExtSafe1WdReq2.Sts"
    fdx_name    = "LiHomeLvngReqSts"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiHomeSafeReqPen:
    de_name     = "LiExtSafe1WdReq1.Pen"
    fdx_name    = "LiHomeSafeReqPen"
    fdx_groupid = 1278
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
    

# Enumeration for intervals in tenths of seconds.
# E.g 
# 3 = 30 seconds
# 4 = 40 seconds
class LiHomeSafeReqSts:
    de_name     = "LiExtSafe1WdReq1.Sts"
    fdx_name    = "LiHomeSafeReqSts"
    fdx_groupid = 1278
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
    

class LiLvrSwt1:
    de_name     = "LiLvrSwt1Req"
    fdx_name    = "LiLvrSwt1"
    fdx_groupid = 1212
    class map:
        NotPsd = 0
        Psd = 1
    

class LiPassFltIndcrTurn:
    de_name     = "LiPassFltIndcrTurn"
    fdx_name    = "LiPassFltIndcrTurn"
    fdx_groupid = 1218
    class map:
        NoFlt = 0
        Flt = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LiSeldForDrvrPfmncMonPen:
    de_name     = "LiSeldForDrvrPfmncMon.Pen"
    fdx_name    = "LiSeldForDrvrPfmncMonPen"
    fdx_groupid = 1276
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
    

class LiSeldForDrvrPfmncMonSts:
    de_name     = "LiSeldForDrvrPfmncMon.Sts"
    fdx_name    = "LiSeldForDrvrPfmncMonSts"
    fdx_groupid = 1276
    class map:
        Off = 0
        On = 1
    

# TrfcSideTmpLe = Traffic Side Temporary Left. Vehicle drives on left side of road
# TrfcSideTmpRi =  Traffic Side Temporary Right. Vehicle drives on right side of road
class LiTrfcSideReq:
    de_name     = "LiTrfcSide1WdReq1"
    fdx_name    = "LiTrfcSideReq"
    fdx_groupid = 1278
    class map:
        Off = 0
        TrfcSideTmpLe = 1
        TrfcSideTmpRi = 2
        Resd = 3
    

class LockgCenStsForUsrFb:
    de_name     = "LockgCenStsForUsrFb"
    fdx_name    = "LockgCenStsForUsrFb"
    fdx_groupid = 1220
    class map:
        Undefd = 0
        Opend = 1
        Clsd = 2
        Lockd = 3
        Safe = 4
    

class LockgCenStsLockSt:
    de_name     = "LockgCenSts.LockSt"
    fdx_name    = "LockgCenStsLockSt"
    fdx_groupid = 1213
    class map:
        LockUndefd = 0
        LockUnlckd = 1
        LockTrUnlckd = 2
        LockLockd = 3
    

class LockgCenStsUpdEve:
    de_name     = "LockgCenSts.UpdEve"
    fdx_name    = "LockgCenStsUpdEve"
    fdx_groupid = 1213
    def LockgCenStsUpdEve_r2p(raw):
        return raw
    def LockgCenStsUpdEve_p2r(physical):
        return physical

class LockgCenTrigSrc:
    de_name     = "LockgCenSts.TrigSrc"
    fdx_name    = "LockgCenTrigSrc"
    fdx_groupid = 1213
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
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockgFbSoundReqPen:
    de_name     = "LockgFbSoundReq.Pen"
    fdx_name    = "LockgFbSoundReqPen"
    fdx_groupid = 1277
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
    

class LockgFbSoundReqSts:
    de_name     = "LockgFbSoundReq.Sts"
    fdx_name    = "LockgFbSoundReqSts"
    fdx_groupid = 1277
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockgFbVisReqPen:
    de_name     = "LockgFbVisReq.Pen"
    fdx_name    = "LockgFbVisReqPen"
    fdx_groupid = 1276
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
    

class LockgFbVisReqSts:
    de_name     = "LockgFbVisReq.Sts"
    fdx_name    = "LockgFbVisReqSts"
    fdx_groupid = 1276
    class map:
        Off = 0
        On = 1
    

class LockgPrsnlReqFromHmi:
    de_name     = "LockgPrsnlReqFromHmi"
    fdx_name    = "LockgPrsnlReqFromHmi"
    fdx_groupid = 1278
    class map:
        NoReq = 0
        On = 1
        Off = 2
    

class LockgPrsnlSts:
    de_name     = "LockgPrsnlSts"
    fdx_name    = "LockgPrsnlSts"
    fdx_groupid = 1216
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class LockSpdReqPen:
    de_name     = "LockSpdReq.Pen"
    fdx_name    = "LockSpdReqPen"
    fdx_groupid = 1277
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
    

class LockSpdReqSts:
    de_name     = "LockSpdReq.Sts"
    fdx_name    = "LockSpdReqSts"
    fdx_groupid = 1277
    class map:
        Off = 0
        On = 1
    

class LvlOfClimaCmft:
    de_name     = "LvlOfClimaCmft"
    fdx_name    = "LvlOfClimaCmft"
    fdx_groupid = 1219
    class map:
        Off = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
        Lvl4 = 4
        Lvl5 = 5
        Lvl6 = 6
        Lvl7 = 7
    

class MassgFctActvDrvrMassgFctActv:
    de_name     = "MassgFctActv.DrvrMassgFctActv"
    fdx_name    = "MassgFctActvDrvrMassgFctActv"
    fdx_groupid = 1279
    class map:
        Off = 0
        On = 1
    

class MassgFctActvPassMassgFctActv:
    de_name     = "MassgFctActv.PassMassgFctActv"
    fdx_name    = "MassgFctActvPassMassgFctActv"
    fdx_groupid = 1279
    class map:
        Off = 0
        On = 1
    

class MemBtnSound:
    de_name     = "MemBtnSound"
    fdx_name    = "MemBtnSound"
    fdx_groupid = 1216
    def MemBtnSound_r2p(raw):
        return raw
    def MemBtnSound_p2r(physical):
        return physical

# Unit: Mins,  Range:0->59
class Mins:
    de_name     = "TiAndDateIndcn.Mins1"
    fdx_name    = "Mins"
    fdx_groupid = 1217
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Mins,  Range:0->59
class Mins1:
    de_name     = "SetTiAndDate.Minute"
    fdx_name    = "Mins1"
    fdx_groupid = 1277
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class MirrDwnStsAtDrvr:
    de_name     = "MirrDwnStsAtDrvr"
    fdx_name    = "MirrDwnStsAtDrvr"
    fdx_groupid = 1218
    class map:
        MirrTiltUndefd = 0
        MirrUpPosn = 1
        MirrTiltPosn = 2
        MirrMovgToUpPosn = 3
        MirrMovgToTiltPosn = 4
    

class MirrDwnStsAtPass:
    de_name     = "MirrDwnStsAtPass"
    fdx_name    = "MirrDwnStsAtPass"
    fdx_groupid = 1220
    class map:
        MirrTiltUndefd = 0
        MirrUpPosn = 1
        MirrTiltPosn = 2
        MirrMovgToUpPosn = 3
        MirrMovgToTiltPosn = 4
    

class MirrFoldStsAtDrvr:
    de_name     = "MirrFoldStsAtDrvr"
    fdx_name    = "MirrFoldStsAtDrvr"
    fdx_groupid = 1212
    class map:
        MirrFoldPosnUndefd = 0
        MirrNotFoldPosn = 1
        MirrFoldPosn = 2
        MirrMovgToNotFold = 3
        MirrMovgToFold = 4
    

class MirrFoldStsAtPass:
    de_name     = "MirrFoldStsAtPass"
    fdx_name    = "MirrFoldStsAtPass"
    fdx_groupid = 1213
    class map:
        MirrFoldPosnUndefd = 0
        MirrNotFoldPosn = 1
        MirrFoldPosn = 2
        MirrMovgToNotFold = 3
        MirrMovgToFold = 4
    

class MmedHmiModStd:
    de_name     = "MmedHmiModStd"
    fdx_name    = "MmedHmiModStd"
    fdx_groupid = 1279
    class map:
        InfModeOff = 0
        InfModeOn = 1
        InfModePartial = 2
        InfModeWelcome = 3
    

class MmedMaiPwrMod:
    de_name     = "MmedHdPwrMod"
    fdx_name    = "MmedMaiPwrMod"
    fdx_groupid = 1276
    class map:
        IHUStateSleep = 0
        IHUStateStandby = 1
        IHUStatePartial = 2
        IHUStateOn = 3
    

# Unit: Unitless,  Range:0->65535
class MstCfgIDBackboneFR:
    de_name     = "MstCfgIDBackboneFR"
    fdx_name    = "MstCfgIDBackboneFR"
    fdx_groupid = 1366
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Month,  Range:1->12
class Mth:
    de_name     = "TiAndDateIndcn.Mth1"
    fdx_name    = "Mth"
    fdx_groupid = 1217
    min = 1
    max = 12
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Month,  Range:1->12
class Mth1:
    de_name     = "SetTiAndDate.Month"
    fdx_name    = "Mth1"
    fdx_groupid = 1277
    min = 1
    max = 12
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class NetCtrlrActvtPrio:
    de_name     = "NetCtrlrActvt.Prio"
    fdx_name    = "NetCtrlrActvtPrio"
    fdx_groupid = 1366
    class map:
        PrioNormal = 0
        PrioHigh = 1
    

# Unit: Unitless,  Range:0->255
class NetCtrlrActvtResourceGroup:
    de_name     = "NetCtrlrActvt.ResourceGroup"
    fdx_name    = "NetCtrlrActvtResourceGroup"
    fdx_groupid = 1366
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class NetHdActvtPrio:
    de_name     = "NetHdActvt.Prio"
    fdx_name    = "NetHdActvtPrio"
    fdx_groupid = 1279
    class map:
        PrioNormal = 0
        PrioHigh = 1
    

# Unit: Unitless,  Range:0->255
class NetHdActvtResourceGroup:
    de_name     = "NetHdActvt.ResourceGroup"
    fdx_name    = "NetHdActvtResourceGroup"
    fdx_groupid = 1279
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class NetTelmActvtPrio:
    de_name     = "NetTelmActvt.Prio"
    fdx_name    = "NetTelmActvtPrio"
    fdx_groupid = 1214
    class map:
        PrioNormal = 0
        PrioHigh = 1
    

# Unit: Unitless,  Range:0->255
class NetTelmActvtResourceGroup:
    de_name     = "NetTelmActvt.ResourceGroup"
    fdx_name    = "NetTelmActvtResourceGroup"
    fdx_groupid = 1214
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class NewTripCdn:
    de_name     = "NewTripCdn"
    fdx_name    = "NewTripCdn"
    fdx_groupid = 1232
    class map:
        NoTrig = 0
        Trig = 1
    

class NoEntryWarnReq:
    de_name     = "NoEntryWarnReq"
    fdx_name    = "NoEntryWarnReq"
    fdx_groupid = 1142
    class map:
        NoWarn = 0
        Warn = 1
    

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo1InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo1"
    fdx_name    = "NoOfSatltForSysNo1InPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo2InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo2"
    fdx_name    = "NoOfSatltForSysNo2InPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo3InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo3"
    fdx_name    = "NoOfSatltForSysNo3InPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo4InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo4"
    fdx_name    = "NoOfSatltForSysNo4InPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo5InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo5"
    fdx_name    = "NoOfSatltForSysNo5InPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->31
class NoOfSatltForSysNo6InPosnFromSatlt:
    de_name     = "PosnFromSatlt.NoOfSatltForSysNo6"
    fdx_name    = "NoOfSatltForSysNo6InPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class NoSoundSys:
    de_name     = "NoSoundSys"
    fdx_name    = "NoSoundSys"
    fdx_groupid = 1280
    def NoSoundSys_r2p(raw):
        return raw
    def NoSoundSys_p2r(physical):
        return physical

# Unit: Unitless,  Range:0->65535
class NotifChkDistbn:
    de_name     = "NotifChkDistbn"
    fdx_name    = "NotifChkDistbn"
    fdx_groupid = 1232
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->7
class NrOfBltAppld:
    de_name     = "NrOfBltAppld"
    fdx_name    = "NrOfBltAppld"
    fdx_groupid = 1350
    min = 0
    max = 7
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class NrOfKeyAvl:
    de_name     = "NrOfKeyAvl"
    fdx_name    = "NrOfKeyAvl"
    fdx_groupid = 1216
    def NrOfKeyAvl_r2p(raw):
        return raw
    def NrOfKeyAvl_p2r(physical):
        return physical

class OffsForDrvrSpprtFctActvSts:
    de_name     = "OffsForDrvrSpprtFctActvSts"
    fdx_name    = "OffsForDrvrSpprtFctActvSts"
    fdx_groupid = 1280
    class map:
        Off = 0
        On = 1
    

class ParkAssiDstFrntOfAudSideWarn:
    de_name     = "SnsrParkAssiFrnt.AudSideWarn"
    fdx_name    = "ParkAssiDstFrntOfAudSideWarn"
    fdx_groupid = 1327
    class map:
        None_ = 0
        Left = 1
        Right = 2
        LeftAndRight = 3
    

class ParkAssiDstFrntOfAudWarnDir:
    de_name     = "SnsrParkAssiFrnt.AudWarnDir"
    fdx_name    = "ParkAssiDstFrntOfAudWarnDir"
    fdx_groupid = 1327
    class map:
        NoWarn = 0
        LeftSide = 1
        OuterLeft = 2
        InnerLeft = 3
        InnerRight = 4
        OuterRight = 5
        RightSide = 6
    

class ParkAssiDstFrntOfDstOfSnsrInsdRi:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrInsdRi"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrInsdRi"
    fdx_groupid = 1327
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
    

class ParkAssiDstFrntOfDstOfSnsrLeSide:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrLeSide"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrLeSide"
    fdx_groupid = 1327
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
    

class ParkAssiDstFrntOfDstOfSnsrOutdRi:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrOutdRi"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrOutdRi"
    fdx_groupid = 1327
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
    

class ParkAssiDstFrntOfDstOfSnsrRiSide:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrRiSide"
    fdx_name    = "ParkAssiDstFrntOfDstOfSnsrRiSide"
    fdx_groupid = 1327
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
    

class ParkAssiDstFrntOfSnsrInsdLe:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrInsdLe"
    fdx_name    = "ParkAssiDstFrntOfSnsrInsdLe"
    fdx_groupid = 1327
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
    

class ParkAssiDstFrntOfSnsrOutdLe:
    de_name     = "SnsrParkAssiFrnt.DstOfSnsrOutdLe"
    fdx_name    = "ParkAssiDstFrntOfSnsrOutdLe"
    fdx_groupid = 1327
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
    

class ParkAssiDstReOfAudSideWarn:
    de_name     = "SnsrParkAssiRe.AudSideWarn"
    fdx_name    = "ParkAssiDstReOfAudSideWarn"
    fdx_groupid = 1327
    class map:
        None_ = 0
        Left = 1
        Right = 2
        LeftAndRight = 3
    

class ParkAssiDstReOfAudWarnDir:
    de_name     = "SnsrParkAssiRe.AudWarnDir"
    fdx_name    = "ParkAssiDstReOfAudWarnDir"
    fdx_groupid = 1327
    class map:
        NoWarn = 0
        LeftSide = 1
        OuterLeft = 2
        InnerLeft = 3
        InnerRight = 4
        OuterRight = 5
        RightSide = 6
    

class ParkAssiDstReOfDstOfSnsrInsdLe:
    de_name     = "SnsrParkAssiRe.DstOfSnsrInsdLe"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrInsdLe"
    fdx_groupid = 1327
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
    

class ParkAssiDstReOfDstOfSnsrInsdRi:
    de_name     = "SnsrParkAssiRe.DstOfSnsrInsdRi"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrInsdRi"
    fdx_groupid = 1327
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
    

class ParkAssiDstReOfDstOfSnsrLeSide:
    de_name     = "SnsrParkAssiRe.DstOfSnsrLeSide"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrLeSide"
    fdx_groupid = 1327
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
    

class ParkAssiDstReOfDstOfSnsrOutdLe:
    de_name     = "SnsrParkAssiRe.DstOfSnsrOutdLe"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrOutdLe"
    fdx_groupid = 1327
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
    

class ParkAssiDstReOfDstOfSnsrOutdRi:
    de_name     = "SnsrParkAssiRe.DstOfSnsrOutdRi"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrOutdRi"
    fdx_groupid = 1327
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
    

class ParkAssiDstReOfDstOfSnsrRiSide:
    de_name     = "SnsrParkAssiRe.DstOfSnsrRiSide"
    fdx_name    = "ParkAssiDstReOfDstOfSnsrRiSide"
    fdx_groupid = 1327
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
    

class PasAlrmDeactvnReq:
    de_name     = "PasAlrmDeactvnReq"
    fdx_name    = "PasAlrmDeactvnReq"
    fdx_groupid = 1280
    class map:
        Off = 0
        On = 1
    

class PasAlrmSts:
    de_name     = "PasAlrmSts"
    fdx_name    = "PasAlrmSts"
    fdx_groupid = 1219
    class map:
        NoReq = 0
        OffReq = 1
    

class PassMassgRunng:
    de_name     = "PassMassgRunng"
    fdx_name    = "PassMassgRunng"
    fdx_groupid = 1218
    class map:
        Off = 0
        On = 1
    

class PassSeatActvSpplFct:
    de_name     = "PassSeatActvSpplFct"
    fdx_name    = "PassSeatActvSpplFct"
    fdx_groupid = 1218
    class map:
        NotAvl = 0
        LumExtnAndLumHei = 1
        BackBlster = 2
        CushExtn = 3
        HdrestHeiAndHdrestTilt = 4
        MassgFct = 5
    

class PassSeatDispMassgFctMassgInten:
    de_name     = "PassSeatDispMassgFct.MassgInten"
    fdx_name    = "PassSeatDispMassgFctMassgInten"
    fdx_groupid = 1282
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2
    

class PassSeatDispMassgFctMassgProg:
    de_name     = "PassSeatDispMassgFct.MassgProg"
    fdx_name    = "PassSeatDispMassgFctMassgProg"
    fdx_groupid = 1282
    class map:
        Prog1 = 0
        Prog2 = 1
        Prog3 = 2
        Prog4 = 3
        Prog5 = 4
    

class PassSeatDispMassgFctMassgSpdLvl:
    de_name     = "PassSeatDispMassgFct.MassgSpdLvl"
    fdx_name    = "PassSeatDispMassgFctMassgSpdLvl"
    fdx_groupid = 1282
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2
    

class PassSeatDispMassgFctOnOff:
    de_name     = "PassSeatDispMassgFct.OnOff"
    fdx_name    = "PassSeatDispMassgFctOnOff"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
    

class PassSeatDispSpplFct:
    de_name     = "PassSeatDispSpplFct"
    fdx_name    = "PassSeatDispSpplFct"
    fdx_groupid = 1279
    class map:
        NotAvl = 0
        LumExtnAndLumHei = 1
        BackBlster = 2
        CushExtn = 3
        HdrestHeiAndHdrestTilt = 4
        MassgFct = 5
    

class PassSeatMassgFctMassgInten:
    de_name     = "PassSeatMassgFct.MassgInten"
    fdx_name    = "PassSeatMassgFctMassgInten"
    fdx_groupid = 1217
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2
    

class PassSeatMassgFctMassgProg:
    de_name     = "PassSeatMassgFct.MassgProg"
    fdx_name    = "PassSeatMassgFctMassgProg"
    fdx_groupid = 1217
    class map:
        Prog1 = 0
        Prog2 = 1
        Prog3 = 2
        Prog4 = 3
        Prog5 = 4
    

class PassSeatMassgFctMassgSpdLvl:
    de_name     = "PassSeatMassgFct.MassgSpdLvl"
    fdx_name    = "PassSeatMassgFctMassgSpdLvl"
    fdx_groupid = 1217
    class map:
        IntenLo = 0
        IntenNorm = 1
        IntenHi = 2
    

class PassSeatMassgFctOnOff:
    de_name     = "PassSeatMassgFct.OnOff"
    fdx_name    = "PassSeatMassgFctOnOff"
    fdx_groupid = 1217
    class map:
        Off = 0
        On = 1
    

class PassSeatSts:
    de_name     = "PassSeatSts"
    fdx_name    = "PassSeatSts"
    fdx_groupid = 1350
    class map:
        Empty = 0
        Fmale = 1
        OccptLrg = 2
        Ukwn = 3
    

class PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2
    

class PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtAdjmtOfSpplFctVerSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class PassSeatSwtSts2PassSeatSwtHeiFrntSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtHeiFrntSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtHeiFrntSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class PassSeatSwtSts2PassSeatSwtHeiSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtHeiSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtHeiSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class PassSeatSwtSts2PassSeatSwtInclSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtInclSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtInclSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2
    

class PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtSelnOfSpplFctStsSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class PassSeatSwtSts2PassSeatSwtSldSts:
    de_name     = "PassSeatSwtSts2.PassSeatSwtSldSts"
    fdx_name    = "PassSeatSwtSts2PassSeatSwtSldSts"
    fdx_groupid = 1213
    class map:
        Idle = 0
        Fwd = 1
        Backw = 2
    

# Unit: rad,  Range:-14.5->14.5, Resolution: (0.0009765625*x+0.0, raw is signed, 15 bits )
class PinionSteerAg1:
    de_name     = "PinionSteerAg1.PinionSteerAg1"
    fdx_name    = "PinionSteerAg1"
    fdx_groupid = 1369
    min    = -14.5
    max    = 14.5
    scale  = 0.0009765625
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class PinionSteerAg1Qf:
    de_name     = "PinionSteerAg1.PinionSteerAg1Qf"
    fdx_name    = "PinionSteerAg1Qf"
    fdx_groupid = 1369
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

# Unit: m,  Range:-100->6000, Resolution: (0.1*x+-100.0, raw is unsigned, 16 bits )
class PosnAltiInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnAlti"
    fdx_name    = "PosnAltiInPosnFromSatlt"
    fdx_groupid = 1367
    min    = -100
    max    = 6000
    scale  = 0.1
    offset = -100.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Deg,  Range:0->359.99, Resolution: (0.01*x+0.0, raw is unsigned, 16 bits )
class PosnDirInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnDir"
    fdx_name    = "PosnDirInPosnFromSatlt"
    fdx_groupid = 1367
    min    = 0
    max    = 359.99
    scale  = 0.01
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Deg,  Range:-90->90, Resolution: (2.7777777777777776e-07*x+0.0, raw is signed, 30 bits )
class PosnLatInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnLat"
    fdx_name    = "PosnLatInPosnFromSatlt"
    fdx_groupid = 1367
    min    = -90
    max    = 90
    scale  = 2.7777777777777776e-07
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Deg,  Range:-180->180, Resolution: (2.7777777777777776e-07*x+0.0, raw is signed, 31 bits )
class PosnLgtInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnLgt"
    fdx_name    = "PosnLgtInPosnFromSatlt"
    fdx_groupid = 1367
    min    = -180
    max    = 180
    scale  = 2.7777777777777776e-07
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class PosnQlyInDstToManvLocn:
    de_name     = "DstToManvLocn.PosnQly"
    fdx_name    = "PosnQlyInDstToManvLocn"
    fdx_groupid = 1280
    class map:
        QlyOfLvl0 = 0
        QlyOfLvl1 = 1
        QlyOfLvl2 = 2
        QlyOfLvl3 = 3
        QlyOfLvl4 = 4
        QlyOfLvl5 = 5
        QlyOfLvl6 = 6
        QlyOfLvl7 = 7
    

# Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 17 bits )
class PosnSpdInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnSpd"
    fdx_name    = "PosnSpdInPosnFromSatlt"
    fdx_groupid = 1367
    min    = 0
    max    = 100
    scale  = 0.001
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m/s,  Range:0->100, Resolution: (0.001*x+0.0, raw is unsigned, 17 bits )
class PosnVHozlInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnVHozl"
    fdx_name    = "PosnVHozlInPosnFromSatlt"
    fdx_groupid = 1367
    min    = 0
    max    = 100
    scale  = 0.001
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m/s,  Range:-100->100, Resolution: (0.001*x+0.0, raw is signed, 18 bits )
class PosnVVertInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PosnVVert"
    fdx_name    = "PosnVVertInPosnFromSatlt"
    fdx_groupid = 1367
    min    = -100
    max    = 100
    scale  = 0.001
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class PostDrvgClimaAvl:
    de_name     = "PostDrvgClimaAvl"
    fdx_name    = "PostDrvgClimaAvl"
    fdx_groupid = 1218
    class map:
        Off = 0
        On = 1
    

class PreClngNotif:
    de_name     = "PreClngNotif"
    fdx_name    = "PreClngNotif"
    fdx_groupid = 1219
    class map:
        NoReq = 0
        NoWarn = 1
        Done = 2
        Err = 3
        Intrpt = 4
        Spare1 = 5
        Spare2 = 6
        Spare3 = 7
    

# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreHozlDilInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PreHozlDil"
    fdx_name    = "PreHozlDilInPosnFromSatlt"
    fdx_groupid = 1367
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PrePosnDilInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PrePosnDil"
    fdx_name    = "PrePosnDilInPosnFromSatlt"
    fdx_groupid = 1367
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreTiDilInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PreTiDil"
    fdx_name    = "PreTiDilInPosnFromSatlt"
    fdx_groupid = 1367
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->25.5, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class PreVertDilInPosnFromSatlt:
    de_name     = "PosnFromSatlt.PreVertDil"
    fdx_name    = "PreVertDilInPosnFromSatlt"
    fdx_groupid = 1367
    min    = 0
    max    = 25.5
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class PrkgAssiFailr:
    de_name     = "PrkgAssiFailr"
    fdx_name    = "PrkgAssiFailr"
    fdx_groupid = 1327
    class map:
        NoFailr = 0
        SgnlFailr = 1
        SnsrFailr = 2
        DgrSnsr = 3
    

class PrkgAssiManvActvSts:
    de_name     = "PrkgAssiManvActvSts"
    fdx_name    = "PrkgAssiManvActvSts"
    fdx_groupid = 1327
    class map:
        Off = 0
        On = 1
    

# Unit: Unitless,  Range:0->63
class PrkgAssiManvProgs:
    de_name     = "PrkgAssiManvProgs"
    fdx_name    = "PrkgAssiManvProgs"
    fdx_groupid = 1327
    min = 0
    max = 63
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class PrkgAssiSts:
    de_name     = "PrkgAssiSts"
    fdx_name    = "PrkgAssiSts"
    fdx_groupid = 1327
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
    

class PrkgAutSts:
    de_name     = "PrkgAutSts"
    fdx_name    = "PrkgAutSts"
    fdx_groupid = 1327
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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

class PrkgTypVld:
    de_name     = "PrkgTypVld"
    fdx_name    = "PrkgTypVld"
    fdx_groupid = 1327
    class map:
        NoPrkgDetd = 0
        ParaPrkgDetd = 1
        PerpPrkgDetd = 2
        ParaAndPerpPrkgDetd = 3
    

class ProfActProf1:
    de_name     = "ProfAct.Prof1"
    fdx_name    = "ProfActProf1"
    fdx_groupid = 1216
    def ProfActProf1_r2p(raw):
        return raw
    def ProfActProf1_p2r(physical):
        return physical

class ProfActProf10:
    de_name     = "ProfAct.Prof10"
    fdx_name    = "ProfActProf10"
    fdx_groupid = 1216
    def ProfActProf10_r2p(raw):
        return raw
    def ProfActProf10_p2r(physical):
        return physical

class ProfActProf11:
    de_name     = "ProfAct.Prof11"
    fdx_name    = "ProfActProf11"
    fdx_groupid = 1216
    def ProfActProf11_r2p(raw):
        return raw
    def ProfActProf11_p2r(physical):
        return physical

class ProfActProf12:
    de_name     = "ProfAct.Prof12"
    fdx_name    = "ProfActProf12"
    fdx_groupid = 1216
    def ProfActProf12_r2p(raw):
        return raw
    def ProfActProf12_p2r(physical):
        return physical

class ProfActProf2:
    de_name     = "ProfAct.Prof2"
    fdx_name    = "ProfActProf2"
    fdx_groupid = 1216
    def ProfActProf2_r2p(raw):
        return raw
    def ProfActProf2_p2r(physical):
        return physical

class ProfActProf3:
    de_name     = "ProfAct.Prof3"
    fdx_name    = "ProfActProf3"
    fdx_groupid = 1216
    def ProfActProf3_r2p(raw):
        return raw
    def ProfActProf3_p2r(physical):
        return physical

class ProfActProf4:
    de_name     = "ProfAct.Prof4"
    fdx_name    = "ProfActProf4"
    fdx_groupid = 1216
    def ProfActProf4_r2p(raw):
        return raw
    def ProfActProf4_p2r(physical):
        return physical

class ProfActProf5:
    de_name     = "ProfAct.Prof5"
    fdx_name    = "ProfActProf5"
    fdx_groupid = 1216
    def ProfActProf5_r2p(raw):
        return raw
    def ProfActProf5_p2r(physical):
        return physical

class ProfActProf6:
    de_name     = "ProfAct.Prof6"
    fdx_name    = "ProfActProf6"
    fdx_groupid = 1216
    def ProfActProf6_r2p(raw):
        return raw
    def ProfActProf6_p2r(physical):
        return physical

class ProfActProf7:
    de_name     = "ProfAct.Prof7"
    fdx_name    = "ProfActProf7"
    fdx_groupid = 1216
    def ProfActProf7_r2p(raw):
        return raw
    def ProfActProf7_p2r(physical):
        return physical

class ProfActProf8:
    de_name     = "ProfAct.Prof8"
    fdx_name    = "ProfActProf8"
    fdx_groupid = 1216
    def ProfActProf8_r2p(raw):
        return raw
    def ProfActProf8_p2r(physical):
        return physical

class ProfActProf9:
    de_name     = "ProfAct.Prof9"
    fdx_name    = "ProfActProf9"
    fdx_groupid = 1216
    def ProfActProf9_r2p(raw):
        return raw
    def ProfActProf9_p2r(physical):
        return physical

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ProfChg:
    de_name     = "ProfChg"
    fdx_name    = "ProfChg"
    fdx_groupid = 1278
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
    

class ProfLimdProf1:
    de_name     = "ProfLimd.Prof1"
    fdx_name    = "ProfLimdProf1"
    fdx_groupid = 1214
    def ProfLimdProf1_r2p(raw):
        return raw
    def ProfLimdProf1_p2r(physical):
        return physical

class ProfLimdProf10:
    de_name     = "ProfLimd.Prof10"
    fdx_name    = "ProfLimdProf10"
    fdx_groupid = 1214
    def ProfLimdProf10_r2p(raw):
        return raw
    def ProfLimdProf10_p2r(physical):
        return physical

class ProfLimdProf11:
    de_name     = "ProfLimd.Prof11"
    fdx_name    = "ProfLimdProf11"
    fdx_groupid = 1214
    def ProfLimdProf11_r2p(raw):
        return raw
    def ProfLimdProf11_p2r(physical):
        return physical

class ProfLimdProf12:
    de_name     = "ProfLimd.Prof12"
    fdx_name    = "ProfLimdProf12"
    fdx_groupid = 1214
    def ProfLimdProf12_r2p(raw):
        return raw
    def ProfLimdProf12_p2r(physical):
        return physical

class ProfLimdProf2:
    de_name     = "ProfLimd.Prof2"
    fdx_name    = "ProfLimdProf2"
    fdx_groupid = 1214
    def ProfLimdProf2_r2p(raw):
        return raw
    def ProfLimdProf2_p2r(physical):
        return physical

class ProfLimdProf3:
    de_name     = "ProfLimd.Prof3"
    fdx_name    = "ProfLimdProf3"
    fdx_groupid = 1214
    def ProfLimdProf3_r2p(raw):
        return raw
    def ProfLimdProf3_p2r(physical):
        return physical

class ProfLimdProf4:
    de_name     = "ProfLimd.Prof4"
    fdx_name    = "ProfLimdProf4"
    fdx_groupid = 1214
    def ProfLimdProf4_r2p(raw):
        return raw
    def ProfLimdProf4_p2r(physical):
        return physical

class ProfLimdProf5:
    de_name     = "ProfLimd.Prof5"
    fdx_name    = "ProfLimdProf5"
    fdx_groupid = 1214
    def ProfLimdProf5_r2p(raw):
        return raw
    def ProfLimdProf5_p2r(physical):
        return physical

class ProfLimdProf6:
    de_name     = "ProfLimd.Prof6"
    fdx_name    = "ProfLimdProf6"
    fdx_groupid = 1214
    def ProfLimdProf6_r2p(raw):
        return raw
    def ProfLimdProf6_p2r(physical):
        return physical

class ProfLimdProf7:
    de_name     = "ProfLimd.Prof7"
    fdx_name    = "ProfLimdProf7"
    fdx_groupid = 1214
    def ProfLimdProf7_r2p(raw):
        return raw
    def ProfLimdProf7_p2r(physical):
        return physical

class ProfLimdProf8:
    de_name     = "ProfLimd.Prof8"
    fdx_name    = "ProfLimdProf8"
    fdx_groupid = 1214
    def ProfLimdProf8_r2p(raw):
        return raw
    def ProfLimdProf8_p2r(physical):
        return physical

class ProfLimdProf9:
    de_name     = "ProfLimd.Prof9"
    fdx_name    = "ProfLimdProf9"
    fdx_groupid = 1214
    def ProfLimdProf9_r2p(raw):
        return raw
    def ProfLimdProf9_p2r(physical):
        return physical

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class ProfPenSts1:
    de_name     = "ProfPenSts1"
    fdx_name    = "ProfPenSts1"
    fdx_groupid = 1214
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
    

# Propulsion HV Battery usage mode type
class PrpsnHvBattUsgModAct:
    de_name     = "PrpsnHvBattUsgModAct"
    fdx_name    = "PrpsnHvBattUsgModAct"
    fdx_groupid = 1372
    class map:
        NoReq = 0
        Norm = 1
        Hld = 2
        HldSmt = 3
        Chrg = 4
        NotOfUse1 = 5
        NotOfUse2 = 6
        NotOfUse3 = 7
    

# Propulsion HV Battery usage mode type
class PrpsnHvBattUsgModReq:
    de_name     = "PrpsnHvBattUsgModReq"
    fdx_name    = "PrpsnHvBattUsgModReq"
    fdx_groupid = 1276
    class map:
        NoReq = 0
        Norm = 1
        Hld = 2
        HldSmt = 3
        Chrg = 4
        NotOfUse1 = 5
        NotOfUse2 = 6
        NotOfUse3 = 7
    

class PrpsnHvBattUsgOfChrgBlkd:
    de_name     = "PrpsnHvBattUsgOfChrgBlkd"
    fdx_name    = "PrpsnHvBattUsgOfChrgBlkd"
    fdx_groupid = 1377
    def PrpsnHvBattUsgOfChrgBlkd_r2p(raw):
        return raw
    def PrpsnHvBattUsgOfChrgBlkd_p2r(physical):
        return physical

# HV battery usage function blocked types
class PrpsnHvBattUsgOfChrgBlkd2:
    de_name     = "PrpsnHvBattUsgOfChrgBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfChrgBlkd2"
    fdx_groupid = 1377
    class map:
        NotBlkd = 0
        BlkdByBattLvl = 1
        BlkdBySysFlt = 2
        NotOfUse = 3
    

class PrpsnHvBattUsgOfHldBlkd:
    de_name     = "PrpsnHvBattUsgOfHldBlkd"
    fdx_name    = "PrpsnHvBattUsgOfHldBlkd"
    fdx_groupid = 1377
    def PrpsnHvBattUsgOfHldBlkd_r2p(raw):
        return raw
    def PrpsnHvBattUsgOfHldBlkd_p2r(physical):
        return physical

# HV battery usage function blocked types
class PrpsnHvBattUsgOfHldBlkd2:
    de_name     = "PrpsnHvBattUsgOfHldBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfHldBlkd2"
    fdx_groupid = 1377
    class map:
        NotBlkd = 0
        BlkdByBattLvl = 1
        BlkdBySysFlt = 2
        NotOfUse = 3
    

class PrpsnHvBattUsgOfHldSmtBlkd:
    de_name     = "PrpsnHvBattUsgOfHldSmtBlkd"
    fdx_name    = "PrpsnHvBattUsgOfHldSmtBlkd"
    fdx_groupid = 1377
    def PrpsnHvBattUsgOfHldSmtBlkd_r2p(raw):
        return raw
    def PrpsnHvBattUsgOfHldSmtBlkd_p2r(physical):
        return physical

# HV battery usage function blocked types
class PrpsnHvBattUsgOfHldSmtBlkd2:
    de_name     = "PrpsnHvBattUsgOfHldSmtBlkd2"
    fdx_name    = "PrpsnHvBattUsgOfHldSmtBlkd2"
    fdx_groupid = 1377
    class map:
        NotBlkd = 0
        BlkdByBattLvl = 1
        BlkdBySysFlt = 2
        NotOfUse = 3
    

# Unit: Km/h,  Range:0->150
class PrpsnHvBattUsgOfHldSpd:
    de_name     = "PrpsnHvBattUsgOfHldSpd"
    fdx_name    = "PrpsnHvBattUsgOfHldSpd"
    fdx_groupid = 1279
    min = 0
    max = 150
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Basic type list
# 8 types
class PrpsnModElecDrvBlkd:
    de_name     = "PrpsnModElecDrvBlkd"
    fdx_name    = "PrpsnModElecDrvBlkd"
    fdx_groupid = 1374
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7
    

# Basic type list
# 8 types
class PrpsnModOffroadBlkd:
    de_name     = "PrpsnModOffroadBlkd"
    fdx_name    = "PrpsnModOffroadBlkd"
    fdx_groupid = 1375
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7
    

# Basic type list
# 8 types
class PrpsnModOfSaveBlkd:
    de_name     = "PrpsnModOfSaveBlkd"
    fdx_name    = "PrpsnModOfSaveBlkd"
    fdx_groupid = 1374
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7
    

# Basic type list
# 8 types
class PrpsnModOfTracBlkd:
    de_name     = "PrpsnModOfTracBlkd"
    fdx_name    = "PrpsnModOfTracBlkd"
    fdx_groupid = 1374
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7
    

# Basic type list
# 8 types
class PrpsnModSptBlkd:
    de_name     = "PrpsnModSptBlkd"
    fdx_name    = "PrpsnModSptBlkd"
    fdx_groupid = 1374
    class map:
        Typ0 = 0
        Typ1 = 1
        Typ2 = 2
        Typ3 = 3
        Typ4 = 4
        Typ5 = 5
        Typ6 = 6
        Typ7 = 7
    

# Unit: NewtonMeter,  Range:-1024->1023, Resolution: (1.0*x+0.0, raw is unsigned, 11 bits )
class PtCluTqPtCluTq:
    de_name     = "PtCluTq.PtCluTq"
    fdx_name    = "PtCluTqPtCluTq"
    fdx_groupid = 1371
    min    = -1024
    max    = 1023
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: NewtonMeter,  Range:-1024->1023, Resolution: (1.0*x+0.0, raw is unsigned, 11 bits )
class PtCluTqPtCluTqDyn:
    de_name     = "PtCluTq.PtCluTqDyn"
    fdx_name    = "PtCluTqPtCluTqDyn"
    fdx_groupid = 1371
    min    = -1024
    max    = 1023
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# General DATA-TYPE to indicate quality. Each quality deviation value, De0-De7, shall be specified on the DATA-ELEMENT using this DATA-TYPE.
class PtCluTqPtCluTqQly:
    de_name     = "PtCluTq.PtCluTqQly"
    fdx_name    = "PtCluTqPtCluTqQly"
    fdx_groupid = 1371
    class map:
        De0 = 0
        De1 = 1
        De2 = 2
        De3 = 3
        De4 = 4
        De5 = 5
        De6 = 6
        De7 = 7
    

class PtGearTar:
    de_name     = "PtGearTar"
    fdx_name    = "PtGearTar"
    fdx_groupid = 1374
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
    

# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtAxleFrntAct:
    de_name     = "PtTqAtWhlFrntAct.PtTqAtAxleFrntAct"
    fdx_name    = "PtTqAtAxleFrntAct"
    fdx_groupid = 1370
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtWhlFrntLeAct:
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlFrntLeAct"
    fdx_name    = "PtTqAtWhlFrntLeAct"
    fdx_groupid = 1370
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: NewtonMeter,  Range:-20000->20000, Resolution: (1.0*x+0.0, raw is signed, 16 bits )
class PtTqAtWhlFrntRiAct:
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlFrntRiAct"
    fdx_name    = "PtTqAtWhlFrntRiAct"
    fdx_groupid = 1370
    min    = -20000
    max    = 20000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# General DATA-TYPE to indicate quality. Each quality deviation value, De0-De7, shall be specified on the DATA-ELEMENT using this DATA-TYPE.
class PtTqAtWhlsFrntQly:
    de_name     = "PtTqAtWhlFrntAct.PtTqAtWhlsFrntQly"
    fdx_name    = "PtTqAtWhlsFrntQly"
    fdx_groupid = 1370
    class map:
        De0 = 0
        De1 = 1
        De2 = 2
        De3 = 3
        De4 = 4
        De5 = 5
        De6 = 6
        De7 = 7
    

class PUnit:
    de_name     = "IndcnUnit.PUnit"
    fdx_name    = "PUnit"
    fdx_groupid = 1282
    class map:
        Pa = 0
        Psi = 1
        Bar = 2
        Atm = 3
        UkwnUnit = 4
    

class PwrChrgDetdForPrkgHeatrElec:
    de_name     = "PwrChrgDetdForPrkgHeatrElec"
    fdx_name    = "PwrChrgDetdForPrkgHeatrElec"
    fdx_groupid = 1219
    def PwrChrgDetdForPrkgHeatrElec_r2p(raw):
        return raw
    def PwrChrgDetdForPrkgHeatrElec_p2r(physical):
        return physical

# Unit: Unitless,  Range:0->15
class PwrLvlElecMai:
    de_name     = "VehModMngtGlbSafe1.PwrLvlElecMai"
    fdx_name    = "PwrLvlElecMai"
    fdx_groupid = 1212
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class PwrLvlElecSubtyp:
    de_name     = "VehModMngtGlbSafe1.PwrLvlElecSubtyp"
    fdx_name    = "PwrLvlElecSubtyp"
    fdx_groupid = 1212
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Status of power supply components
class PwrSplyErrSts:
    de_name     = "PwrSplyErrSts"
    fdx_name    = "PwrSplyErrSts"
    fdx_groupid = 1215
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
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RainSenMemdReqPen:
    de_name     = "RainSenMemdReq.Pen"
    fdx_name    = "RainSenMemdReqPen"
    fdx_groupid = 1278
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
    

class RainSenMemdReqSts:
    de_name     = "RainSenMemdReq.Sts"
    fdx_name    = "RainSenMemdReqSts"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

class ReAxleWarn:
    de_name     = "ReAxleWarn"
    fdx_name    = "ReAxleWarn"
    fdx_groupid = 1217
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        Resd = 3
    

class RmnLockgPrsnlReq:
    de_name     = "RmnLockgPrsnlReq"
    fdx_name    = "RmnLockgPrsnlReq"
    fdx_groupid = 1216
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RngbdIllmnCmdPen:
    de_name     = "RngbdIllmnCmd.Pen"
    fdx_name    = "RngbdIllmnCmdPen"
    fdx_groupid = 1280
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
    

# For running board illumination setting value
class RngbdIllmnCmdSts:
    de_name     = "RngbdIllmnCmd.Cmd"
    fdx_name    = "RngbdIllmnCmdSts"
    fdx_groupid = 1280
    class map:
        Di = 0
        Aut = 1
        Resd2 = 2
        Resd3 = 3
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RoadFricIndcnActvPen:
    de_name     = "RoadFricIndcnActv.Pen"
    fdx_name    = "RoadFricIndcnActvPen"
    fdx_groupid = 1276
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
    

class RoadFricIndcnActvSts:
    de_name     = "RoadFricIndcnActv.Sts"
    fdx_name    = "RoadFricIndcnActvSts"
    fdx_groupid = 1276
    class map:
        Off = 0
        On = 1
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

# Implementation Value Min	Implementation Value Max	Physical Value	"Long name"		Description
# 0			0			NoWarn		No Warning		No warning
# 1			1			FricLoPsbl	Friction 	Low Possible		Potentially low friction
# 2			2			FricLoCfmd	Friction Low Confirmed	Confirmed low friction
class RoadFricWarnReq:
    de_name     = "RoadFricWarnReq"
    fdx_name    = "RoadFricWarnReq"
    fdx_groupid = 1143
    class map:
        NoWarn = 0
        FricLoPsbl = 1
        FricLoCfmd = 2
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class RoadSgnInfoActvPen:
    de_name     = "RoadSgnInfoActv.Pen"
    fdx_name    = "RoadSgnInfoActvPen"
    fdx_groupid = 1275
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
    

class RoadSgnInfoActvSts:
    de_name     = "RoadSgnInfoActv.Sts"
    fdx_name    = "RoadSgnInfoActvSts"
    fdx_groupid = 1275
    class map:
        Off = 0
        On = 1
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

class RoadSpdLimActvSts:
    de_name     = "RoadSpdLimActvSts"
    fdx_name    = "RoadSpdLimActvSts"
    fdx_groupid = 1280
    class map:
        Off = 0
        On = 1
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

# Unit: rad/s,  Range:-6->6, Resolution: (0.000244140625*x+0.0, raw is signed, 16 bits )
class RollRate1:
    de_name     = "AgDataRawSafe.RollRate"
    fdx_name    = "RollRate1"
    fdx_groupid = 1353
    min    = -6
    max    = 6
    scale  = 0.000244140625
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class RollRate1Qf1:
    de_name     = "AgDataRawSafe.RollRateQf"
    fdx_name    = "RollRate1Qf1"
    fdx_groupid = 1353
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3
    

class RsdsSysStsLeCtraSts:
    de_name     = "RsdsSysStsLe.CtraSts"
    fdx_name    = "RsdsSysStsLeCtraSts"
    fdx_groupid = 1143
    class map:
        Off = 0
        On = 1
    

class RsdsSysStsLeCtraSts1:
    de_name     = "RsdsSysStsLe.CtraSts1"
    fdx_name    = "RsdsSysStsLeCtraSts1"
    fdx_groupid = 1143
    class map:
        Active = 0
        Inactive = 1
    

class RsdsSysStsLeLcmaSts:
    de_name     = "RsdsSysStsLe.LcmaSts"
    fdx_name    = "RsdsSysStsLeLcmaSts"
    fdx_groupid = 1143
    class map:
        Off = 0
        On = 1
    

class RsdsSysStsLeLcmaSts1:
    de_name     = "RsdsSysStsLe.LcmaSts1"
    fdx_name    = "RsdsSysStsLeLcmaSts1"
    fdx_groupid = 1143
    class map:
        Active = 0
        Inactive = 1
    

class RsdsSysStsLeRcmSts:
    de_name     = "RsdsSysStsLe.RcmSts"
    fdx_name    = "RsdsSysStsLeRcmSts"
    fdx_groupid = 1143
    class map:
        Off = 0
        On = 1
    

class RsdsSysStsLeRcwSts:
    de_name     = "RsdsSysStsLe.RcwSts"
    fdx_name    = "RsdsSysStsLeRcwSts"
    fdx_groupid = 1143
    class map:
        Off = 0
        On = 1
    

class RsdsSysStsLeRsdsSts:
    de_name     = "RsdsSysStsLe.RsdsSts"
    fdx_name    = "RsdsSysStsLeRsdsSts"
    fdx_groupid = 1143
    class map:
        Cfg = 0
        StrtUp = 1
        Runng = 2
        Blkd = 3
        Faulty = 4
        Shutdown = 5
        Hot = 6
        Cal = 7
    

class RsdsSysStsRiCtraSts:
    de_name     = "RsdsSysStsRi.CtraSts"
    fdx_name    = "RsdsSysStsRiCtraSts"
    fdx_groupid = 1143
    class map:
        Off = 0
        On = 1
    

class RsdsSysStsRiCtraSts1:
    de_name     = "RsdsSysStsRi.CtraSts1"
    fdx_name    = "RsdsSysStsRiCtraSts1"
    fdx_groupid = 1143
    class map:
        Active = 0
        Inactive = 1
    

class RsdsSysStsRiLcmaSts:
    de_name     = "RsdsSysStsRi.LcmaSts"
    fdx_name    = "RsdsSysStsRiLcmaSts"
    fdx_groupid = 1143
    class map:
        Off = 0
        On = 1
    

class RsdsSysStsRiLcmaSts1:
    de_name     = "RsdsSysStsRi.LcmaSts1"
    fdx_name    = "RsdsSysStsRiLcmaSts1"
    fdx_groupid = 1143
    class map:
        Active = 0
        Inactive = 1
    

class RsdsSysStsRiRcmSts:
    de_name     = "RsdsSysStsRi.RcmSts"
    fdx_name    = "RsdsSysStsRiRcmSts"
    fdx_groupid = 1143
    class map:
        Off = 0
        On = 1
    

class RsdsSysStsRiRcwSts:
    de_name     = "RsdsSysStsRi.RcwSts"
    fdx_name    = "RsdsSysStsRiRcwSts"
    fdx_groupid = 1143
    class map:
        Off = 0
        On = 1
    

class RsdsSysStsRiRsdsSts:
    de_name     = "RsdsSysStsRi.RsdsSts"
    fdx_name    = "RsdsSysStsRiRsdsSts"
    fdx_groupid = 1143
    class map:
        Cfg = 0
        StrtUp = 1
        Runng = 2
        Blkd = 3
        Faulty = 4
        Shutdown = 5
        Hot = 6
        Cal = 7
    

# 0: No navigation solution
# 1: 3 satelites solution (2D)
# 2: more than 3 satellites solution (3D)
# 3: CSF (Cold Start Fix)
class SatltPosnStsPrm1InPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltPosnStsPrm1"
    fdx_name    = "SatltPosnStsPrm1InPosnFromSatlt"
    fdx_groupid = 1367
    class map:
        PosnStsOfLvl1 = 0
        PosnStsOfLvl2 = 1
        PosnStsOfLvl3 = 2
        PosnStsOfLvl4 = 3
    

class SatltPosnStsPrm2InPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltPosnStsPrm2"
    fdx_name    = "SatltPosnStsPrm2InPosnFromSatlt"
    fdx_groupid = 1367
    def SatltPosnStsPrm2InPosnFromSatlt_r2p(raw):
        return raw
    def SatltPosnStsPrm2InPosnFromSatlt_p2r(physical):
        return physical

class SatltPosnStsPrm3InPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltPosnStsPrm3"
    fdx_name    = "SatltPosnStsPrm3InPosnFromSatlt"
    fdx_groupid = 1367
    def SatltPosnStsPrm3InPosnFromSatlt_r2p(raw):
        return raw
    def SatltPosnStsPrm3InPosnFromSatlt_p2r(physical):
        return physical

class SatltSysNo1InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo1InUse"
    fdx_name    = "SatltSysNo1InUseInPosnFromSatlt"
    fdx_groupid = 1367
    class map:
        No = 0
        Yes = 1
    

class SatltSysNo2InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo2InUse"
    fdx_name    = "SatltSysNo2InUseInPosnFromSatlt"
    fdx_groupid = 1367
    class map:
        No = 0
        Yes = 1
    

class SatltSysNo3InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo3InUse"
    fdx_name    = "SatltSysNo3InUseInPosnFromSatlt"
    fdx_groupid = 1367
    class map:
        No = 0
        Yes = 1
    

class SatltSysNo4InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo4InUse"
    fdx_name    = "SatltSysNo4InUseInPosnFromSatlt"
    fdx_groupid = 1367
    class map:
        No = 0
        Yes = 1
    

class SatltSysNo5InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo5InUse"
    fdx_name    = "SatltSysNo5InUseInPosnFromSatlt"
    fdx_groupid = 1367
    class map:
        No = 0
        Yes = 1
    

class SatltSysNo6InUseInPosnFromSatlt:
    de_name     = "PosnFromSatlt.SatltSysNo6InUse"
    fdx_name    = "SatltSysNo6InUseInPosnFromSatlt"
    fdx_groupid = 1367
    class map:
        No = 0
        Yes = 1
    

class SaveSetgToMemPrmnt:
    de_name     = "SaveSetgToMemPrmnt"
    fdx_name    = "SaveSetgToMemPrmnt"
    fdx_groupid = 1282
    class map:
        Off = 0
        On = 1
        Aut = 2
    

# Unit: %,  Range:0->100, Resolution: (5.0*x+0.0, raw is unsigned, 0 bits )
class ScrBarVolIndcn:
    de_name     = "ScrBarVolIndcn"
    fdx_name    = "ScrBarVolIndcn"
    fdx_groupid = 1377
    min    = 0
    max    = 100
    scale  = 5.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Litre,  Range:0->31
class ScrMaxFillgVol:
    de_name     = "ScrMaxFillgVol"
    fdx_name    = "ScrMaxFillgVol"
    fdx_groupid = 1377
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Litre,  Range:0->31
class ScrReagentTankVol:
    de_name     = "ScrReagentTankVol"
    fdx_name    = "ScrReagentTankVol"
    fdx_groupid = 1377
    min = 0
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class SeatBackUnlckdThrdLe:
    de_name     = "SeatBackUnlckdThrd.SeatBackUnlckdLe"
    fdx_name    = "SeatBackUnlckdThrdLe"
    fdx_groupid = 1218
    def SeatBackUnlckdThrdLe_r2p(raw):
        return raw
    def SeatBackUnlckdThrdLe_p2r(physical):
        return physical

class SeatBackUnlckdThrdRi:
    de_name     = "SeatBackUnlckdThrd.SeatBackUnlckdRi"
    fdx_name    = "SeatBackUnlckdThrdRi"
    fdx_groupid = 1218
    def SeatBackUnlckdThrdRi_r2p(raw):
        return raw
    def SeatBackUnlckdThrdRi_p2r(physical):
        return physical

class SeatDispBtnPsdDrvrSeatDispBtnPsd:
    de_name     = "SeatDispBtnPsd.DrvrSeatDispBtnPsd"
    fdx_name    = "SeatDispBtnPsdDrvrSeatDispBtnPsd"
    fdx_groupid = 1278
    def SeatDispBtnPsdDrvrSeatDispBtnPsd_r2p(raw):
        return raw
    def SeatDispBtnPsdDrvrSeatDispBtnPsd_p2r(physical):
        return physical

class SeatDispBtnPsdPassSeatDispBtnPsd:
    de_name     = "SeatDispBtnPsd.PassSeatDispBtnPsd"
    fdx_name    = "SeatDispBtnPsdPassSeatDispBtnPsd"
    fdx_groupid = 1278
    def SeatDispBtnPsdPassSeatDispBtnPsd_r2p(raw):
        return raw
    def SeatDispBtnPsdPassSeatDispBtnPsd_p2r(physical):
        return physical

class SeatHeatDurgClimaEnad:
    de_name     = "SeatHeatDurgClimaEnad"
    fdx_name    = "SeatHeatDurgClimaEnad"
    fdx_groupid = 1279
    class map:
        SeatHeatOff = 0
        SeatDrvOn = 1
        SeatPassOn = 2
        SeatDrvrAndPass = 3
    

# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class SeatHeatgAutCdn:
    de_name     = "SeatHeatgAutCdn"
    fdx_name    = "SeatHeatgAutCdn"
    fdx_groupid = 1216
    class map:
        Rst = 0
        Set = 1
    

class SeatSwtLeSigThrd:
    de_name     = "SeatSwtLeSigThrd"
    fdx_name    = "SeatSwtLeSigThrd"
    fdx_groupid = 1282
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

class SeatSwtRiSigThrd:
    de_name     = "SeatSwtRiSigThrd"
    fdx_name    = "SeatSwtRiSigThrd"
    fdx_groupid = 1282
    class map:
        Idle = 0
        Up = 1
        Dwn = 2
    

# Flag	Gives the status of a flag to represent a boolean status (true or false)		0	Rst	Flag reset: Flag is not set (FALSE)
# 		                                                                                                                	1	Set	Flag set: Flag is set (TRUE)
class SeatVentnAutCdn:
    de_name     = "SeatVentnAutCdn"
    fdx_name    = "SeatVentnAutCdn"
    fdx_groupid = 1220
    class map:
        Rst = 0
        Set = 1
    

# Unit: s,  Range:0->59
class Sec:
    de_name     = "TiAndDateIndcn.Sec1"
    fdx_name    = "Sec"
    fdx_groupid = 1217
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: s,  Range:0->59
class Sec1:
    de_name     = "SetTiAndDate.Second"
    fdx_name    = "Sec1"
    fdx_groupid = 1277
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SetOfLangIdPen:
    de_name     = "SetOfLang.IdPen"
    fdx_name    = "SetOfLangIdPen"
    fdx_groupid = 1277
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
    

class SftyCchAccActvnStsActv:
    de_name     = "SftyCchAccActvnSts.Actv"
    fdx_name    = "SftyCchAccActvnStsActv"
    fdx_groupid = 1145
    class map:
        No = 0
        Yes = 1
    

# Unit: Unitless,  Range:0->15
class SftyCchAccActvnStsDay:
    de_name     = "SftyCchAccActvnSts.Day"
    fdx_name    = "SftyCchAccActvnStsDay"
    fdx_groupid = 1145
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class SftyCchAccActvnStsMth:
    de_name     = "SftyCchAccActvnSts.Mth"
    fdx_name    = "SftyCchAccActvnStsMth"
    fdx_groupid = 1145
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class SftyCchAccActvnStsYr:
    de_name     = "SftyCchAccActvnSts.Yr"
    fdx_name    = "SftyCchAccActvnStsYr"
    fdx_groupid = 1145
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class SftyCchLaneKeepAidActvnStsActv:
    de_name     = "SftyCchLaneKeepAidActvnSts.Actv"
    fdx_name    = "SftyCchLaneKeepAidActvnStsActv"
    fdx_groupid = 1145
    class map:
        No = 0
        Yes = 1
    

# Unit: Unitless,  Range:0->15
class SftyCchLaneKeepAidActvnStsDay:
    de_name     = "SftyCchLaneKeepAidActvnSts.Day"
    fdx_name    = "SftyCchLaneKeepAidActvnStsDay"
    fdx_groupid = 1145
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class SftyCchLaneKeepAidActvnStsMth:
    de_name     = "SftyCchLaneKeepAidActvnSts.Mth"
    fdx_name    = "SftyCchLaneKeepAidActvnStsMth"
    fdx_groupid = 1145
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class SftyCchLaneKeepAidActvnStsYr:
    de_name     = "SftyCchLaneKeepAidActvnSts.Yr"
    fdx_name    = "SftyCchLaneKeepAidActvnStsYr"
    fdx_groupid = 1145
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxAttention:
    de_name     = "SftyCchLongTermIdx.Attention"
    fdx_name    = "SftyCchLongTermIdxAttention"
    fdx_groupid = 1145
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class SftyCchLongTermIdxAttentionAvl:
    de_name     = "SftyCchLongTermIdx.AttentionAvl"
    fdx_name    = "SftyCchLongTermIdxAttentionAvl"
    fdx_groupid = 1145
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2
    

# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxDstToVeh:
    de_name     = "SftyCchLongTermIdx.DstToVeh"
    fdx_name    = "SftyCchLongTermIdxDstToVeh"
    fdx_groupid = 1145
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class SftyCchLongTermIdxDstToVehAvl:
    de_name     = "SftyCchLongTermIdx.DstToVehAvl"
    fdx_name    = "SftyCchLongTermIdxDstToVehAvl"
    fdx_groupid = 1145
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2
    

# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxKeepSpdLim:
    de_name     = "SftyCchLongTermIdx.KeepSpdLim"
    fdx_name    = "SftyCchLongTermIdxKeepSpdLim"
    fdx_groupid = 1145
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class SftyCchLongTermIdxKeepSpdLimAvl:
    de_name     = "SftyCchLongTermIdx.KeepSpdLimAvl"
    fdx_name    = "SftyCchLongTermIdxKeepSpdLimAvl"
    fdx_groupid = 1145
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2
    

# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxLaneKeep:
    de_name     = "SftyCchLongTermIdx.LaneKeep"
    fdx_name    = "SftyCchLongTermIdxLaneKeep"
    fdx_groupid = 1145
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class SftyCchLongTermIdxLaneKeepAvl:
    de_name     = "SftyCchLongTermIdx.LaneKeepAvl"
    fdx_name    = "SftyCchLongTermIdxLaneKeepAvl"
    fdx_groupid = 1145
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2
    

# Unit: Unitless,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class SftyCchLongTermIdxTot:
    de_name     = "SftyCchLongTermIdx.Tot"
    fdx_name    = "SftyCchLongTermIdxTot"
    fdx_groupid = 1145
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class SftyCchLongTermIdxTotAvl:
    de_name     = "SftyCchLongTermIdx.TotAvl"
    fdx_name    = "SftyCchLongTermIdxTotAvl"
    fdx_groupid = 1145
    class map:
        NoData = 0
        NotAvl = 1
        Avl = 2
    

# Unit: Unitless,  Range:0->255
class SftyWarnGroupFromAsySafeChks:
    de_name     = "SftyWarnGroupFromAsySafe.SftyWarnGroupFromAsySafeChks"
    fdx_name    = "SftyWarnGroupFromAsySafeChks"
    fdx_groupid = 1138
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class SftyWarnGroupFromAsySafeCntr:
    de_name     = "SftyWarnGroupFromAsySafe.SftyWarnGroupFromAsySafeCntr"
    fdx_name    = "SftyWarnGroupFromAsySafeCntr"
    fdx_groupid = 1138
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SpdAlrmActvForRoadSgnInfoPen:
    de_name     = "SpdAlrmActvForRoadSgnInfo.Pen"
    fdx_name    = "SpdAlrmActvForRoadSgnInfoPen"
    fdx_groupid = 1275
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
    

class SpdAlrmActvForRoadSgnInfoSts:
    de_name     = "SpdAlrmActvForRoadSgnInfo.Sts"
    fdx_name    = "SpdAlrmActvForRoadSgnInfoSts"
    fdx_groupid = 1275
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class SpdCameraInfoSeldForRoadSgnInfoPen:
    de_name     = "SpdCameraInfoSeldForRoadSgnInfo.Pen"
    fdx_name    = "SpdCameraInfoSeldForRoadSgnInfoPen"
    fdx_groupid = 1275
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
    

class SpdCameraInfoSeldForRoadSgnInfoSts:
    de_name     = "SpdCameraInfoSeldForRoadSgnInfo.Sts"
    fdx_name    = "SpdCameraInfoSeldForRoadSgnInfoSts"
    fdx_groupid = 1275
    class map:
        Off = 0
        On = 1
    

class SpdLimAdpvSts:
    de_name     = "SpdLimAdpvSts"
    fdx_name    = "SpdLimAdpvSts"
    fdx_groupid = 1142
    class map:
        Off = 0
        On = 1
        NotAvl = 2
        SrvRqrd = 3
    

# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"			Description
# 0			0			NoWarn			No Warning			No warning
# 1			1			SpdHi			Speed High			Too high speed compared to speed limit
# 2			2			SpdHiAndSpdCtrlPassd	Speed High And Speed Control Passed	Too high speed and passing speed camera
class SpdLimWarnReq:
    de_name     = "SpdLimWarnReq"
    fdx_name    = "SpdLimWarnReq"
    fdx_groupid = 1142
    class map:
        NoWarn = 0
        SpdHi = 1
        SpdHiAndSpdCtrlPassd = 2
    

class SpdUnit:
    de_name     = "IndcnUnit.SpdUnit"
    fdx_name    = "SpdUnit"
    fdx_groupid = 1282
    class map:
        Kmph = 0
        Mph = 1
        UkwnUnit = 2
    

class SpdWarn:
    de_name     = "SpdWarn"
    fdx_name    = "SpdWarn"
    fdx_groupid = 1230
    class map:
        NoTrig = 0
        Trig = 1
    

class SpprtForFctInDstToManvLocn:
    de_name     = "DstToManvLocn.SpprtForFct"
    fdx_name    = "SpprtForFctInDstToManvLocn"
    fdx_groupid = 1280
    class map:
        No = 0
        Yes = 1
    

class SpprtForFctInTireCircumCalByNav:
    de_name     = "TireCircumCalByNav.SpprtForFct"
    fdx_name    = "SpprtForFctInTireCircumCalByNav"
    fdx_groupid = 1280
    class map:
        No = 0
        Yes = 1
    

class SrvRst:
    de_name     = "SrvRst"
    fdx_name    = "SrvRst"
    fdx_groupid = 1232
    class map:
        NoTrig = 0
        Trig = 1
    

# Unit: Unitless,  Range:0->15, Resolution: (1.0*x+0.0, raw is unsigned, 0 bits )
class SrvTrig:
    de_name     = "SrvTrig"
    fdx_name    = "SrvTrig"
    fdx_groupid = 1232
    min    = 0
    max    = 15
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Implementation Value Min	Implementation Value Max	Physical Value	Long name	Description
# 0			0			NotVld1		NotValid1		Not valid
# 1			1			No		No		No
# 2			2			Yes		Yes		Yes
# 3			3			NotVld2		NotValid2		Not valid
class SteerOvrdWarnReqForAutDrv:
    de_name     = "SftyWarnGroupFromAsySafe.SteerOvrdWarnReqForAutDrv"
    fdx_name    = "SteerOvrdWarnReqForAutDrv"
    fdx_groupid = 1138
    class map:
        NotVld1 = 0
        No = 1
        Yes = 2
        NotVld2 = 3
    

class SteerStsForAutDrv:
    de_name     = "SftyWarnGroupFromAsySafe.SteerStsForAutDrv"
    fdx_name    = "SteerStsForAutDrv"
    fdx_groupid = 1138
    class map:
        Off = 0
        Stb = 1
        Actv = 2
        OvrdByDrvr = 3
        PndStb = 4
    

# Implementation Value Min	Implementation Value Max	Physical Value		"Long name"				Description
# 0			0			NoWarn			No Warning				No warning
# 1			1			DispWarnForDrvrSteerLe	Display Warning For Driver Steering Left	Show warning that driver shall steer left
# 2			2			DispWarnForDrvrSteerRi	Display Warning For Driver Steering Right	Show warning that driver shall steer right
# 3			3			DispGenWarn		Display Generic Warning			Show general warning
class SteerWarnReqForAutDrv:
    de_name     = "SteerWarnReqForAutDrv"
    fdx_name    = "SteerWarnReqForAutDrv"
    fdx_groupid = 1139
    class map:
        NoWarn = 0
        DispWarnForDrvrSteerLe = 1
        DispWarnForDrvrSteerRi = 2
        DispGenWarn = 3
    

class SteerWarnReqForLaneKeepAid:
    de_name     = "SteerWarnReqForLaneKeepAid"
    fdx_name    = "SteerWarnReqForLaneKeepAid"
    fdx_groupid = 1142
    class map:
        NoWarn = 0
        WarnLe = 1
        WarnRi = 2
    

# Unit: rad,  Range:-14.5->14.5, Resolution: (0.0009765625*x+0.0, raw is signed, 15 bits )
class SteerWhlAgSafe:
    de_name     = "SteerWhlSnsrSafe.SteerWhlAg"
    fdx_name    = "SteerWhlAgSafe"
    fdx_groupid = 1368
    min    = -14.5
    max    = 14.5
    scale  = 0.0009765625
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: rad/s,  Range:-50->50, Resolution: (0.0078125*x+0.0, raw is signed, 14 bits )
class SteerWhlAgSpdSafe:
    de_name     = "SteerWhlSnsrSafe.SteerWhlAgSpd"
    fdx_name    = "SteerWhlAgSpdSafe"
    fdx_groupid = 1368
    min    = -50
    max    = 50
    scale  = 0.0078125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class SteerWhlHeatgDurgClimaEnad:
    de_name     = "SteerWhlHeatgDurgClimaEnad"
    fdx_name    = "SteerWhlHeatgDurgClimaEnad"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

class SteerWhlHeatgOnReq:
    de_name     = "SteerWhlHeatgOnReq"
    fdx_name    = "SteerWhlHeatgOnReq"
    fdx_groupid = 1278
    class map:
        Off = 0
        Lo = 1
        Med = 2
        Hi = 3
    

class SteerWhlHeatgStrtAutCdnOk:
    de_name     = "SteerWhlHeatgStrtAutCdnOk"
    fdx_name    = "SteerWhlHeatgStrtAutCdnOk"
    fdx_groupid = 1219
    class map:
        Off = 0
        On = 1
    

class SteerWhlSnsrQf:
    de_name     = "SteerWhlSnsrSafe.SteerWhlSnsrQf"
    fdx_name    = "SteerWhlSnsrQf"
    fdx_groupid = 1368
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

# Unit: Unitless,  Range:0->255
class SteerWhlSnsrSafeChks:
    de_name     = "SteerWhlSnsrSafe.SteerWhlSnsrChks"
    fdx_name    = "SteerWhlSnsrSafeChks"
    fdx_groupid = 1368
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class SteerWhlSnsrSafeCntr:
    de_name     = "SteerWhlSnsrSafe.SteerWhlSnsrCntr"
    fdx_name    = "SteerWhlSnsrSafeCntr"
    fdx_groupid = 1368
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class SteerWhlSwtPwr:
    de_name     = "SteerWhlSwtPwr"
    fdx_name    = "SteerWhlSwtPwr"
    fdx_groupid = 1230
    class map:
        Off = 0
        On = 1
    

# Unit: Unitless,  Range:0->16777215
class StopStrtInhb:
    de_name     = "StopStrtInhb"
    fdx_name    = "StopStrtInhb"
    fdx_groupid = 1368
    min = 0
    max = 16777215
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class StrtInProgs:
    de_name     = "StrtInProgs"
    fdx_name    = "StrtInProgs"
    fdx_groupid = 1220
    class map:
        StrtStsOff = 0
        StrtStsImminent = 1
        StrtStsStrtng = 2
        StrtStsRunng = 3
    

class SunRoofPosnSts:
    de_name     = "SunRoofPosnSts"
    fdx_name    = "SunRoofPosnSts"
    fdx_groupid = 1214
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
    

class SwtAtCenSts:
    de_name     = "SwtAtCenSts"
    fdx_name    = "SwtAtCenSts"
    fdx_groupid = 1277
    class map:
        NoTrig = 0
        Trig = 1
    

# PLILJESS:
# Autosar AI Tables Standard DATA-TYPE.
# Please consider using OnOff1 first hand to avoid confusion.
class SwtForPassAirbCutOffSt:
    de_name     = "SwtForPassAirbCutOffSt"
    fdx_name    = "SwtForPassAirbCutOffSt"
    fdx_groupid = 1350
    class map:
        On = 0
        Off = 1
    

# Status of the door.
class TankFlapSts:
    de_name     = "TankFlapSts"
    fdx_name    = "TankFlapSts"
    fdx_groupid = 1216
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2
    

class TelmSts:
    de_name     = "TelmSts"
    fdx_name    = "TelmSts"
    fdx_groupid = 1218
    class map:
        NoErr = 0
        TelmErr = 1
        TelmDi = 2
        TelmEnd = 3
    

class TiAndDateVld:
    de_name     = "TiAndDateIndcn.DataValid"
    fdx_name    = "TiAndDateVld"
    fdx_groupid = 1217
    class map:
        No = 0
        Yes = 1
    

# Unit: Days,  Range:1->31
class TiForDayInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForDay"
    fdx_name    = "TiForDayInPosnFromSatlt"
    fdx_groupid = 1367
    min = 1
    max = 31
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: hours,  Range:0->23
class TiForHrInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForHr"
    fdx_name    = "TiForHrInPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 23
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Mins,  Range:0->59
class TiForMinsInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForMins"
    fdx_name    = "TiForMinsInPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Month,  Range:1->12
class TiForMthInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForMth"
    fdx_name    = "TiForMthInPosnFromSatlt"
    fdx_groupid = 1367
    min = 1
    max = 12
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: s,  Range:0->59
class TiForSecInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForSec"
    fdx_name    = "TiForSecInPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 59
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Year,  Range:0->99
class TiForYrInPosnFromSatlt:
    de_name     = "PosnFromSatlt.TiForYr"
    fdx_name    = "TiForYrInPosnFromSatlt"
    fdx_groupid = 1367
    min = 0
    max = 99
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TiGapLimdPen:
    de_name     = "TiGapLimd.Pen"
    fdx_name    = "TiGapLimdPen"
    fdx_groupid = 1277
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
    

# Unit: s,  Range:0->6.3, Resolution: (0.1*x+0.0, raw is unsigned, 6 bits )
class TiGapLimdSts:
    de_name     = "TiGapLimd.Sts"
    fdx_name    = "TiGapLimdSts"
    fdx_groupid = 1277
    min    = 0
    max    = 6.3
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->4095
class TireCircumInTireCircumCalByNav:
    de_name     = "TireCircumCalByNav.TireCircum"
    fdx_name    = "TireCircumInTireCircumCalByNav"
    fdx_groupid = 1280
    min = 0
    max = 4095
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Used for sending a Start or a Stop/Cancel request from HMI to a core function
class TireMonCalReq:
    de_name     = "TireMonCalReq"
    fdx_name    = "TireMonCalReq"
    fdx_groupid = 1277
    class map:
        Idle = 0
        StrtReq = 1
        StopReq = 2
        Resd = 3
    

class TireMonrDispReq:
    de_name     = "TireMonrDispReq"
    fdx_name    = "TireMonrDispReq"
    fdx_groupid = 1277
    class map:
        Off = 0
        On = 1
    

# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForFrntLe:
    de_name     = "TirePAbsltValFrntLe.TirepabsltVal1"
    fdx_name    = "TirePAbsltValForFrntLe"
    fdx_groupid = 1217
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForReLe:
    de_name     = "TirePAbsltValReLe.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValForReLe"
    fdx_groupid = 1217
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValForReRi:
    de_name     = "TirePAbsltValReRi.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValForReRi"
    fdx_groupid = 1217
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: hPa,  Range:0->637.5, Resolution: (2.5*x+0.0, raw is unsigned, 8 bits )
class TirePAbsltValFrntRiTirePAbsltVal1:
    de_name     = "TirePAbsltValFrntRi.TirePAbsltVal1"
    fdx_name    = "TirePAbsltValFrntRiTirePAbsltVal1"
    fdx_groupid = 1217
    min    = 0
    max    = 637.5
    scale  = 2.5
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class TirePAbsltValFrntRiTirePPosn:
    de_name     = "TirePAbsltValFrntRi.TirePPosn"
    fdx_name    = "TirePAbsltValFrntRiTirePPosn"
    fdx_groupid = 1217
    class map:
        NoPReadingOrPReadingOkWithOutPosn = 0
        PReadingWithPosn = 1
    

class TirePAbsltValQFForFrnRi:
    de_name     = "TirePAbsltValFrntRi.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQFForFrnRi"
    fdx_groupid = 1217
    class map:
        Norm = 0
        OldVal = 1
        UpdVal = 2
        SnsrFlt = 3
    

class TirePAbsltValQfForReLe:
    de_name     = "TirePAbsltValReLe.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQfForReLe"
    fdx_groupid = 1217
    class map:
        Norm = 0
        OldVal = 1
        UpdVal = 2
        SnsrFlt = 3
    

class TirePAbsltValQfForReRi:
    de_name     = "TirePAbsltValReRi.TirePAbsltValQF"
    fdx_name    = "TirePAbsltValQfForReRi"
    fdx_groupid = 1217
    class map:
        Norm = 0
        OldVal = 1
        UpdVal = 2
        SnsrFlt = 3
    

class TirePAbsltVaQflForFrntLe:
    de_name     = "TirePAbsltValFrntLe.TirePabsltValQF"
    fdx_name    = "TirePAbsltVaQflForFrntLe"
    fdx_groupid = 1217
    class map:
        Norm = 0
        OldVal = 1
        UpdVal = 2
        SnsrFlt = 3
    

class TirePCalSts:
    de_name     = "TirePCalSts"
    fdx_name    = "TirePCalSts"
    fdx_groupid = 1217
    class map:
        Norm = 0
        Cal = 1
        CalNotCmpl = 2
        CalCmpl = 3
    

class TirePFrntLe:
    de_name     = "TirePMonData.TirePFrntLe"
    fdx_name    = "TirePFrntLe"
    fdx_groupid = 1217
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        ResdWarn = 3
    

class TirePFrntRi:
    de_name     = "TirePMonData.TirePFrntRe"
    fdx_name    = "TirePFrntRi"
    fdx_groupid = 1217
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        ResdWarn = 3
    

class TirePMonSts:
    de_name     = "TirePMonData.TirePMonSts1"
    fdx_name    = "TirePMonSts"
    fdx_groupid = 1217
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
    

class TirePPosnForFrntLe:
    de_name     = "TirePAbsltValFrntLe.TirePPosn"
    fdx_name    = "TirePPosnForFrntLe"
    fdx_groupid = 1217
    class map:
        NoPReadingOrPReadingOkWithOutPosn = 0
        PReadingWithPosn = 1
    

class TirePPosnForReLe:
    de_name     = "TirePAbsltValReLe.TirePPosn"
    fdx_name    = "TirePPosnForReLe"
    fdx_groupid = 1217
    class map:
        NoPReadingOrPReadingOkWithOutPosn = 0
        PReadingWithPosn = 1
    

class TirePPosnForReRi:
    de_name     = "TirePAbsltValReRi.TirePPosn"
    fdx_name    = "TirePPosnForReRi"
    fdx_groupid = 1217
    class map:
        NoPReadingOrPReadingOkWithOutPosn = 0
        PReadingWithPosn = 1
    

class TirePReLe:
    de_name     = "TirePMonData.TirePReLe"
    fdx_name    = "TirePReLe"
    fdx_groupid = 1217
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        ResdWarn = 3
    

class TirePReRi:
    de_name     = "TirePMonData.TirePReRi"
    fdx_name    = "TirePReRi"
    fdx_groupid = 1217
    class map:
        NoWarn = 0
        SoftWarn = 1
        HardWarn = 2
        ResdWarn = 3
    

class TnlEntryOrEnd:
    de_name     = "TnlEntryOrEnd"
    fdx_name    = "TnlEntryOrEnd"
    fdx_groupid = 1139
    class map:
        No = 0
        Yes = 1
    

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
    class map:
        Ukwn = 0
        Off = 1
        SpdLoStb = 2
        SpdHiStb = 3
        NotAvl = 4
        SrvRqrd = 5
        On = 6
        NotInUse = 7
    

# Selects trip trip meter type
# Trip1 - Reset by driver
# Trip2 - Reset by timer (new trip condition)
# Resd - Reserved for future Trip indication modes
class TripModSeln:
    de_name     = "TripModSeln"
    fdx_name    = "TripModSeln"
    fdx_groupid = 1276
    class map:
        Ukwn = 0
        Trip1 = 1
        Trip2 = 2
        Resd = 3
    

# 0 = Trailer Lamp Check not possible to execute
# 1 = Trailer Lamp Check possible to execute
class TrlrLampActvtChk:
    de_name     = "TrlrLampChkSts1WdSts1"
    fdx_name    = "TrlrLampActvtChk"
    fdx_groupid = 1214
    class map:
        TrlrLampChkNotAvl = 0
        TrlrLampChkAvl = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TrlrLampChkAutReqPen:
    de_name     = "TrlrLampChkAutReq.Pen"
    fdx_name    = "TrlrLampChkAutReqPen"
    fdx_groupid = 1275
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
    

class TrlrLampChkAutReqSts:
    de_name     = "TrlrLampChkAutReq.Sts"
    fdx_name    = "TrlrLampChkAutReqSts"
    fdx_groupid = 1275
    class map:
        Off = 0
        On = 1
    

# Used for sending a Start or a Stop/Cancel request from HMI to a core function
class TrlrLampChkRemReq:
    de_name     = "TrlrLampChkRemReq"
    fdx_name    = "TrlrLampChkRemReq"
    fdx_groupid = 1275
    class map:
        Idle = 0
        StrtReq = 1
        StopReq = 2
        Resd = 3
    

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
    class map:
        Idle = 0
        TrlrLampChkStrtd = 1
        TrlrLampChkCmpl = 2
        CnclOper = 3
        TrlrLampEngOffReqd = 4
        Resd1 = 5
        Resd2 = 6
        Resd3 = 7
    

# 0 = Trailer unconnected
# 1 = Trailer connected to the vehicle
class TrlrPrsnt:
    de_name     = "TrlrPrsntSts1WdSts"
    fdx_name    = "TrlrPrsnt"
    fdx_groupid = 1216
    class map:
        TrlrNotPrsnt = 0
        TrlrPrsnt = 1
    

# Status of the door.
class TrSts:
    de_name     = "TrSts"
    fdx_name    = "TrSts"
    fdx_groupid = 1212
    class map:
        Ukwn = 0
        Opend = 1
        Clsd = 2
    

class TUnit:
    de_name     = "IndcnUnit.TUnit"
    fdx_name    = "TUnit"
    fdx_groupid = 1282
    class map:
        Celsius = 0
        Fahrenheit = 1
        UkwnUnit = 2
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class TurnAutFlsgReqPen:
    de_name     = "LiExtReq1WdReq3.Pen"
    fdx_name    = "TurnAutFlsgReqPen"
    fdx_groupid = 1278
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
    

class TurnAutFlsgReqSts:
    de_name     = "LiExtReq1WdReq3.Sts"
    fdx_name    = "TurnAutFlsgReqSts"
    fdx_groupid = 1278
    class map:
        Off = 0
        On = 1
    

# Unit: Unitless,  Range:0->10000
class TwliBriRaw:
    de_name     = "TwliBriRaw.TwliBriRaw1"
    fdx_name    = "TwliBriRaw"
    fdx_groupid = 1213
    min = 0
    max = 10000
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class TwliBriRawQf:
    de_name     = "TwliBriRaw.TwliBriRawQf"
    fdx_name    = "TwliBriRawQf"
    fdx_groupid = 1213
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

class TwliBriSts:
    de_name     = "TwliBriSts"
    fdx_name    = "TwliBriSts"
    fdx_groupid = 1212
    class map:
        Night = 0
        Day = 1
    

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte0:
    de_name     = "UkwnCptReqToInfoMgr.Byte0"
    fdx_name    = "UkwnCptReqToInfoMgrByte0"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte1:
    de_name     = "UkwnCptReqToInfoMgr.Byte1"
    fdx_name    = "UkwnCptReqToInfoMgrByte1"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte2:
    de_name     = "UkwnCptReqToInfoMgr.Byte2"
    fdx_name    = "UkwnCptReqToInfoMgrByte2"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte3:
    de_name     = "UkwnCptReqToInfoMgr.Byte3"
    fdx_name    = "UkwnCptReqToInfoMgrByte3"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte4:
    de_name     = "UkwnCptReqToInfoMgr.Byte4"
    fdx_name    = "UkwnCptReqToInfoMgrByte4"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte5:
    de_name     = "UkwnCptReqToInfoMgr.Byte5"
    fdx_name    = "UkwnCptReqToInfoMgrByte5"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte6:
    de_name     = "UkwnCptReqToInfoMgr.Byte6"
    fdx_name    = "UkwnCptReqToInfoMgrByte6"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToInfoMgrByte7:
    de_name     = "UkwnCptReqToInfoMgr.Byte7"
    fdx_name    = "UkwnCptReqToInfoMgrByte7"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte0:
    de_name     = "UkwnCptReqToSoundMgr.Byte0"
    fdx_name    = "UkwnCptReqToSoundMgrByte0"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte1:
    de_name     = "UkwnCptReqToSoundMgr.Byte1"
    fdx_name    = "UkwnCptReqToSoundMgrByte1"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte2:
    de_name     = "UkwnCptReqToSoundMgr.Byte2"
    fdx_name    = "UkwnCptReqToSoundMgrByte2"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte3:
    de_name     = "UkwnCptReqToSoundMgr.Byte3"
    fdx_name    = "UkwnCptReqToSoundMgrByte3"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte4:
    de_name     = "UkwnCptReqToSoundMgr.Byte4"
    fdx_name    = "UkwnCptReqToSoundMgrByte4"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte5:
    de_name     = "UkwnCptReqToSoundMgr.Byte5"
    fdx_name    = "UkwnCptReqToSoundMgrByte5"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte6:
    de_name     = "UkwnCptReqToSoundMgr.Byte6"
    fdx_name    = "UkwnCptReqToSoundMgrByte6"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptReqToSoundMgrByte7:
    de_name     = "UkwnCptReqToSoundMgr.Byte7"
    fdx_name    = "UkwnCptReqToSoundMgrByte7"
    fdx_groupid = 1220
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte0:
    de_name     = "UkwnCptRespFromInfoMgr.Byte0"
    fdx_name    = "UkwnCptRespFromInfoMgrByte0"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte1:
    de_name     = "UkwnCptRespFromInfoMgr.Byte1"
    fdx_name    = "UkwnCptRespFromInfoMgrByte1"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte2:
    de_name     = "UkwnCptRespFromInfoMgr.Byte2"
    fdx_name    = "UkwnCptRespFromInfoMgrByte2"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte3:
    de_name     = "UkwnCptRespFromInfoMgr.Byte3"
    fdx_name    = "UkwnCptRespFromInfoMgrByte3"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte4:
    de_name     = "UkwnCptRespFromInfoMgr.Byte4"
    fdx_name    = "UkwnCptRespFromInfoMgrByte4"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte5:
    de_name     = "UkwnCptRespFromInfoMgr.Byte5"
    fdx_name    = "UkwnCptRespFromInfoMgrByte5"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte6:
    de_name     = "UkwnCptRespFromInfoMgr.Byte6"
    fdx_name    = "UkwnCptRespFromInfoMgrByte6"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromInfoMgrByte7:
    de_name     = "UkwnCptRespFromInfoMgr.Byte7"
    fdx_name    = "UkwnCptRespFromInfoMgrByte7"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte0:
    de_name     = "UkwnCptRespFromSoundMgr.Byte0"
    fdx_name    = "UkwnCptRespFromSoundMgrByte0"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte1:
    de_name     = "UkwnCptRespFromSoundMgr.Byte1"
    fdx_name    = "UkwnCptRespFromSoundMgrByte1"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte2:
    de_name     = "UkwnCptRespFromSoundMgr.Byte2"
    fdx_name    = "UkwnCptRespFromSoundMgrByte2"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte3:
    de_name     = "UkwnCptRespFromSoundMgr.Byte3"
    fdx_name    = "UkwnCptRespFromSoundMgrByte3"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte4:
    de_name     = "UkwnCptRespFromSoundMgr.Byte4"
    fdx_name    = "UkwnCptRespFromSoundMgrByte4"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte5:
    de_name     = "UkwnCptRespFromSoundMgr.Byte5"
    fdx_name    = "UkwnCptRespFromSoundMgrByte5"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte6:
    de_name     = "UkwnCptRespFromSoundMgr.Byte6"
    fdx_name    = "UkwnCptRespFromSoundMgrByte6"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255
class UkwnCptRespFromSoundMgrByte7:
    de_name     = "UkwnCptRespFromSoundMgr.Byte7"
    fdx_name    = "UkwnCptRespFromSoundMgrByte7"
    fdx_groupid = 1280
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckFbVisReqPen:
    de_name     = "UnlckFbVisReq.Pen"
    fdx_name    = "UnlckFbVisReqPen"
    fdx_groupid = 1277
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
    

class UnlckFbVisReqSts:
    de_name     = "UnlckFbVisReq.Sts"
    fdx_name    = "UnlckFbVisReqSts"
    fdx_groupid = 1277
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckKeylsReqPen:
    de_name     = "UnlckKeylsReq.IdPen"
    fdx_name    = "UnlckKeylsReqPen"
    fdx_groupid = 1277
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
    

class UnlckKeylsReqSts:
    de_name     = "UnlckKeylsReq.KeylsCfg"
    fdx_name    = "UnlckKeylsReqSts"
    fdx_groupid = 1277
    class map:
        UnlckCen = 0
        UnlckInd = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UnlckRemReqPen:
    de_name     = "UnlckRemReq.Pen"
    fdx_name    = "UnlckRemReqPen"
    fdx_groupid = 1276
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
    

class UnlckRemReqSts:
    de_name     = "UnlckRemReq.Sts"
    fdx_name    = "UnlckRemReqSts"
    fdx_groupid = 1276
    class map:
        UnlckCen = 0
        UnlckStep2 = 1
    

# UsageMode Data
class UsgModSts:
    de_name     = "VehModMngtGlbSafe1.UsgModSts"
    fdx_name    = "UsgModSts"
    fdx_groupid = 1212
    class map:
        UsgModAbdnd = 0
        UsgModInActv = 1
        UsgModCnvinc = 2
        UsgModActv = 11
        UsgModDrvg = 13
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UsrSetSpdForKeySpdLimnPen:
    de_name     = "UsrSetSpdForKeySpdLimn.Pen"
    fdx_name    = "UsrSetSpdForKeySpdLimnPen"
    fdx_groupid = 1280
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
    

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdLimnSts:
    de_name     = "UsrSetSpdForKeySpdLimn.Sts"
    fdx_name    = "UsrSetSpdForKeySpdLimnSts"
    fdx_groupid = 1280
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn1:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn1"
    fdx_name    = "UsrSetSpdForKeySpdWarn1"
    fdx_groupid = 1277
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn2:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn2"
    fdx_name    = "UsrSetSpdForKeySpdWarn2"
    fdx_groupid = 1277
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn3:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn3"
    fdx_name    = "UsrSetSpdForKeySpdWarn3"
    fdx_groupid = 1277
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn4:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn4"
    fdx_name    = "UsrSetSpdForKeySpdWarn4"
    fdx_groupid = 1277
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn5:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn5"
    fdx_name    = "UsrSetSpdForKeySpdWarn5"
    fdx_groupid = 1277
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: m/s,  Range:0->125, Resolution: (0.03125*x+0.0, raw is unsigned, 12 bits )
class UsrSetSpdForKeySpdWarn6:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarn6"
    fdx_name    = "UsrSetSpdForKeySpdWarn6"
    fdx_groupid = 1277
    min    = 0
    max    = 125
    scale  = 0.03125
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class UsrSetSpdForKeySpdWarnPen:
    de_name     = "UsrSetSpdForKeySpdWarn.UsrSetSpdForKeySpdWarnPen"
    fdx_name    = "UsrSetSpdForKeySpdWarnPen"
    fdx_groupid = 1277
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
    

# 0  No text
# 1  "Engine is running"
# 2  "Ignition on"
# 3  TBD
class VehActvMsgToDrvr:
    de_name     = "VehActvMsgToDrvr"
    fdx_name    = "VehActvMsgToDrvr"
    fdx_groupid = 1217
    class map:
        NoMsg = 0
        Msg1 = 1
        Msg2 = 2
        Msg3 = 3
    

# Unit: Volt,  Range:0->25, Resolution: (0.1*x+0.0, raw is unsigned, 8 bits )
class VehBattUSysU:
    de_name     = "VehBattU.SysU"
    fdx_name    = "VehBattUSysU"
    fdx_groupid = 1218
    min    = 0
    max    = 25
    scale  = 0.1
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class VehBattUSysUQf:
    de_name     = "VehBattU.SysUQf"
    fdx_name    = "VehBattUSysUQf"
    fdx_groupid = 1218
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmBlk:
    de_name     = "VehCfgPrm.BlkIDBytePosn1"
    fdx_name    = "VehCfgPrmBlk"
    fdx_groupid = 1215
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal1:
    de_name     = "VehCfgPrm.CCPBytePosn2"
    fdx_name    = "VehCfgPrmVal1"
    fdx_groupid = 1215
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal2:
    de_name     = "VehCfgPrm.CCPBytePosn3"
    fdx_name    = "VehCfgPrmVal2"
    fdx_groupid = 1215
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal3:
    de_name     = "VehCfgPrm.CCPBytePosn4"
    fdx_name    = "VehCfgPrmVal3"
    fdx_groupid = 1215
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal4:
    de_name     = "VehCfgPrm.CCPBytePosn5"
    fdx_name    = "VehCfgPrmVal4"
    fdx_groupid = 1215
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal5:
    de_name     = "VehCfgPrm.CCPBytePosn6"
    fdx_name    = "VehCfgPrmVal5"
    fdx_groupid = 1215
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal6:
    de_name     = "VehCfgPrm.CCPBytePosn7"
    fdx_name    = "VehCfgPrmVal6"
    fdx_groupid = 1215
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VehCfgPrmVal7:
    de_name     = "VehCfgPrm.CCPBytePosn8"
    fdx_name    = "VehCfgPrmVal7"
    fdx_groupid = 1215
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: kg,  Range:0->10000, Resolution: (1.0*x+0.0, raw is unsigned, 14 bits )
class VehM:
    de_name     = "VehMNom.VehM"
    fdx_name    = "VehM"
    fdx_groupid = 1373
    min    = 0
    max    = 10000
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class VehMNomTrlrM:
    de_name     = "VehMNom.TrlrM"
    fdx_name    = "VehMNomTrlrM"
    fdx_groupid = 1373
    class map:
        Lvl0 = 0
        Lvl1 = 1
        Lvl2 = 2
        Lvl3 = 3
    

# Unit: Unitless,  Range:0->255
class VehModMngtGlbSafe1Chks:
    de_name     = "VehModMngtGlbSafe1.Chks"
    fdx_name    = "VehModMngtGlbSafe1Chks"
    fdx_groupid = 1212
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class VehModMngtGlbSafe1Cntr:
    de_name     = "VehModMngtGlbSafe1.Cntr"
    fdx_name    = "VehModMngtGlbSafe1Cntr"
    fdx_groupid = 1212
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Main usage shall be to indicate quality for vehicle state estimations, not to be used for failure code setting and function degradation unless absolutely necassary.
class VehMQly:
    de_name     = "VehMNom.VehMQly"
    fdx_name    = "VehMQly"
    fdx_groupid = 1373
    class map:
        Flt = 0
        NoInfo = 1
        Vld = 2
    

# Unit: Unitless,  Range:0->255
class VehMtnStChks:
    de_name     = "VehMtnStSafe.VehMtnStChks"
    fdx_name    = "VehMtnStChks"
    fdx_groupid = 1368
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class VehMtnStCntr:
    de_name     = "VehMtnStSafe.VehMtnStCntr"
    fdx_name    = "VehMtnStCntr"
    fdx_groupid = 1368
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class VehMtnStSafe:
    de_name     = "VehMtnStSafe.VehMtnSt"
    fdx_name    = "VehMtnStSafe"
    fdx_groupid = 1368
    class map:
        Ukwn = 0
        StandStillVal1 = 1
        StandStillVal2 = 2
        StandStillVal3 = 3
        RollgFwdVal1 = 4
        RollgFwdVal2 = 5
        RollgBackwVal1 = 6
        RollgBackwVal2 = 7
    

# Unit: Unitless,  Range:0->511
class VehSpdAvg:
    de_name     = "VehSpdAvgIndcd.VehSpdIndcd"
    fdx_name    = "VehSpdAvg"
    fdx_groupid = 1232
    min = 0
    max = 511
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class VehSpdAvgUnit:
    de_name     = "VehSpdAvgIndcd.VeSpdIndcdUnit"
    fdx_name    = "VehSpdAvgUnit"
    fdx_groupid = 1232
    class map:
        Kmph = 0
        Mph = 1
        UkwnUnit = 2
    

class VehSpdCtrlActvSts:
    de_name     = "VehSpdCtrlActvSts"
    fdx_name    = "VehSpdCtrlActvSts"
    fdx_groupid = 1280
    class map:
        Off = 0
        On = 1
    

class VehSpdIndcdUnit:
    de_name     = "VehSpdIndcd.VeSpdIndcdUnit"
    fdx_name    = "VehSpdIndcdUnit"
    fdx_groupid = 1229
    class map:
        Kmph = 0
        Mph = 1
        UkwnUnit = 2
    

# Unit: Unitless,  Range:0->511
class VehSpdIndcdVal:
    de_name     = "VehSpdIndcd.VehSpdIndcd"
    fdx_name    = "VehSpdIndcdVal"
    fdx_groupid = 1229
    min = 0
    max = 511
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: m/s,  Range:0->125, Resolution: (0.00391*x+0.0, raw is unsigned, 15 bits )
class VehSpdLgtSafe:
    de_name     = "VehSpdLgtSafe.VehSpdLgt"
    fdx_name    = "VehSpdLgtSafe"
    fdx_groupid = 1368
    min    = 0
    max    = 125
    scale  = 0.00391
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255
class VehSpdLgtSafeChks:
    de_name     = "VehSpdLgtSafe.VehSpdLgtChks"
    fdx_name    = "VehSpdLgtSafeChks"
    fdx_groupid = 1368
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class VehSpdLgtSafeCntr:
    de_name     = "VehSpdLgtSafe.VehSpdLgtCntr"
    fdx_name    = "VehSpdLgtSafeCntr"
    fdx_groupid = 1368
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class VehSpdLgtSafeQf:
    de_name     = "VehSpdLgtSafe.VehSpdLgtQf"
    fdx_name    = "VehSpdLgtSafeQf"
    fdx_groupid = 1368
    class map:
        UndefindDataAccur = 0
        TmpUndefdData = 1
        DataAccurNotWithinSpcn = 2
        AccurData = 3
    

# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp1:
    de_name     = "VFCVectorIHU.Grp1"
    fdx_name    = "VFCVectorIHUGrp1"
    fdx_groupid = 1282
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp2:
    de_name     = "VFCVectorIHU.Grp2"
    fdx_name    = "VFCVectorIHUGrp2"
    fdx_groupid = 1282
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->65535
class VFCVectorIHUGrp3:
    de_name     = "VFCVectorIHU.Grp3"
    fdx_name    = "VFCVectorIHUGrp3"
    fdx_groupid = 1282
    min = 0
    max = 65535
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinBlk:
    de_name     = "Vin.BlockNr"
    fdx_name    = "VinBlk"
    fdx_groupid = 1218
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn1:
    de_name     = "Vin.VINSignalPos1"
    fdx_name    = "VinPosn1"
    fdx_groupid = 1218
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn2:
    de_name     = "Vin.VINSignalPos2"
    fdx_name    = "VinPosn2"
    fdx_groupid = 1218
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn3:
    de_name     = "Vin.VINSignalPos3"
    fdx_name    = "VinPosn3"
    fdx_groupid = 1218
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn4:
    de_name     = "Vin.VINSignalPos4"
    fdx_name    = "VinPosn4"
    fdx_groupid = 1218
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn5:
    de_name     = "Vin.VINSignalPos5"
    fdx_name    = "VinPosn5"
    fdx_groupid = 1218
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn6:
    de_name     = "Vin.VINSignalPos6"
    fdx_name    = "VinPosn6"
    fdx_groupid = 1218
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class VinPosn7:
    de_name     = "Vin.VINSignalPos7"
    fdx_name    = "VinPosn7"
    fdx_groupid = 1218
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Availability status
# 0= not available
# 1= available
class VisnAgWideSts:
    de_name     = "VisnAgWideSts"
    fdx_name    = "VisnAgWideSts"
    fdx_groupid = 1142
    class map:
        NotAvl = 0
        Avl = 1
    

class VisnImgAgWideInUse:
    de_name     = "VisnImgAgWideInUse"
    fdx_name    = "VisnImgAgWideInUse"
    fdx_groupid = 1143
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
    

# Gallon 1 = UK gallons
# Gallon 2 = US gallons
class VolUnit:
    de_name     = "IndcnUnit.VolUnit"
    fdx_name    = "VolUnit"
    fdx_groupid = 1282
    class map:
        litre = 0
        gallon1 = 1
        gallon2 = 2
        UkwnUnit = 3
    

# Unit: Unitless,  Range:0->4095
class WhlCircum:
    de_name     = "WhlCircum"
    fdx_name    = "WhlCircum"
    fdx_groupid = 1214
    min = 0
    max = 4095
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: rpm,  Range:-1638.4->1638.3, Resolution: (0.1*x+-1638.4, raw is unsigned, 0 bits )
class WhlMotSysSpdAct:
    de_name     = "WhlMotSysSpdAct"
    fdx_name    = "WhlMotSysSpdAct"
    fdx_groupid = 1369
    min    = -1638.4
    max    = 1638.3
    scale  = 0.1
    offset = -1638.4
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: NewtonMeter,  Range:-8188->8192, Resolution: (4.0*x+-8188.0, raw is unsigned, 12 bits )
class WhlMotSysTqEst:
    de_name     = "WhlMotSysTqEst.TqAct"
    fdx_name    = "WhlMotSysTqEst"
    fdx_groupid = 1370
    min    = -8188
    max    = 8192
    scale  = 4.0
    offset = -8188.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255
class WhlMotSysTqEstChks:
    de_name     = "WhlMotSysTqEst.TqActChks"
    fdx_name    = "WhlMotSysTqEstChks"
    fdx_groupid = 1370
    min = 0
    max = 255
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Unitless,  Range:0->15
class WhlMotSysTqEstCntr:
    de_name     = "WhlMotSysTqEst.TqActCntr"
    fdx_name    = "WhlMotSysTqEstCntr"
    fdx_groupid = 1370
    min = 0
    max = 15
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

class WhlMotSysTqEstQlyFac:
    de_name     = "WhlMotSysTqEst.TqActQlyFac"
    fdx_name    = "WhlMotSysTqEstQlyFac"
    fdx_groupid = 1370
    class map:
        QfUndefd = 0
        QfInProgs = 1
        QfNotSpc = 2
        QfSnsrDataOk = 3
    

# Unit: rpm,  Range:0->2550, Resolution: (10.0*x+0.0, raw is unsigned, 8 bits )
class WhlMotSysTqEstSpdLimn:
    de_name     = "WhlMotSysTqEst.SpdLimn"
    fdx_name    = "WhlMotSysTqEstSpdLimn"
    fdx_groupid = 1370
    min    = 0
    max    = 2550
    scale  = 10.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrFrntLe:
    de_name     = "WhlRotToothCntr.WhlRotToothCntrFrntLe"
    fdx_name    = "WhlRotToothCntrFrntLe"
    fdx_groupid = 1369
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrFrntRi:
    de_name     = "WhlRotToothCntr.WhlRotToothCntrFrntRi"
    fdx_name    = "WhlRotToothCntrFrntRi"
    fdx_groupid = 1369
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrReLe:
    de_name     = "WhlRotToothCntr.WhlRotToothCntrReLe"
    fdx_name    = "WhlRotToothCntrReLe"
    fdx_groupid = 1369
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

# Unit: Unitless,  Range:0->255, Resolution: (1.0*x+0.0, raw is unsigned, 8 bits )
class WhlRotToothCntrReRi:
    de_name     = "WhlRotToothCntr.WhlRotToothCntrReRi"
    fdx_name    = "WhlRotToothCntrReRi"
    fdx_groupid = 1369
    min    = 0
    max    = 255
    scale  = 1.0
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class WinPosnStsAtDrvrRe:
    de_name     = "WinPosnStsAtDrvrRe"
    fdx_name    = "WinPosnStsAtDrvrRe"
    fdx_groupid = 1216
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
    

class WinPosnStsDrv:
    de_name     = "WinPosnStsAtDrv"
    fdx_name    = "WinPosnStsDrv"
    fdx_groupid = 1219
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
    

class WinPosnStsPass:
    de_name     = "WinPosnStsAtPass"
    fdx_name    = "WinPosnStsPass"
    fdx_groupid = 1219
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
    

class WinPosnStsRePass:
    de_name     = "WinPosnStsAtPassRe"
    fdx_name    = "WinPosnStsRePass"
    fdx_groupid = 1217
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
    

# The signal reflects the actual wiping speed or mode regardless of activation source. Both rain sensor and wiper stalk info are used to create the signal. When in interval mode the wipes are carried out in low speed.
class WipgInfoWipgSpdInfo:
    de_name     = "WipgInfo.WipgSpdInfo"
    fdx_name    = "WipgInfoWipgSpdInfo"
    fdx_groupid = 1216
    class map:
        Off = 0
        IntlLo = 1
        IntlHi = 2
        WipgSpd4045 = 3
        WipgSpd4650 = 4
        WipgSpd5155 = 5
        WipgSpd5660 = 6
        WiprErr = 7
    

class WipgInfoWiprActv:
    de_name     = "WipgInfo.WiprActv"
    fdx_name    = "WipgInfoWiprActv"
    fdx_groupid = 1216
    class map:
        Off = 0
        On = 1
    

class WipgInfoWiprInWipgAr:
    de_name     = "WipgInfo.WiprInWipgAr"
    fdx_name    = "WipgInfoWiprInWipgAr"
    fdx_groupid = 1216
    class map:
        Off = 0
        On = 1
    

class WiprFrntSrvModReq:
    de_name     = "WiprFrntSrvModReq"
    fdx_name    = "WiprFrntSrvModReq"
    fdx_groupid = 1277
    class map:
        NoActn = 0
        ActvtSrvPosn = 1
        DeActvtSrvPosn = 2
    

class WiprInPosnForSrv:
    de_name     = "WiprInPosnForSrv"
    fdx_name    = "WiprInPosnForSrv"
    fdx_groupid = 1219
    class map:
        Off = 0
        On = 1
    

# Carries a user profile ID for personalization of functionality and settings. Defined for profile  ID range [1 - 13]
# 
# Value 0 means profile ID has (yet) not been calculated on sender side. Receiver side defines behaviour e.g use of stored profiel ID/last received profile ID
# Value 15 refers to all profile IDs and is used in setting tupples <setting value, profile ID> to signal that the setting value is associated with all user profiles.
class WiprReAutReqPen:
    de_name     = "WiprReAutReq.Pen"
    fdx_name    = "WiprReAutReqPen"
    fdx_groupid = 1280
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
    

class WiprReAutReqSts:
    de_name     = "WiprReAutReq.Sts"
    fdx_name    = "WiprReAutReqSts"
    fdx_groupid = 1280
    class map:
        Off = 0
        On = 1
    

class WshrFldSts:
    de_name     = "WshrFldSts1WdElmHMI"
    fdx_name    = "WshrFldSts"
    fdx_groupid = 1219
    class map:
        Off = 0
        On = 1
    

# Unit: rad/s,  Range:-6->6, Resolution: (0.000244140625*x+0.0, raw is signed, 16 bits )
class YawRate1:
    de_name     = "AgDataRawSafe.YawRate"
    fdx_name    = "YawRate1"
    fdx_groupid = 1353
    min    = -6
    max    = 6
    scale  = 0.000244140625
    offset = 0.0
    def r2p(cls, raw):
        return (raw * cls.scale) + cls.offset
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return (physical - cls.offset) / cls.scale

class YawRate1Qf1:
    de_name     = "AgDataRawSafe.YawRateQf"
    fdx_name    = "YawRate1Qf1"
    fdx_groupid = 1353
    class map:
        DevOfDataUndefd = 0
        DataTmpUndefdAndEvlnInProgs = 1
        DevOfDataNotWithinRngAllwd = 2
        DataCalcdWithDevDefd = 3
    

# Unit: Year,  Range:0->99
class Yr:
    de_name     = "TiAndDateIndcn.Yr1"
    fdx_name    = "Yr"
    fdx_groupid = 1217
    min = 0
    max = 99
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

# Unit: Year,  Range:0->99
class Yr1:
    de_name     = "SetTiAndDate.Year"
    fdx_name    = "Yr1"
    fdx_groupid = 1277
    min = 0
    max = 99
    def r2p(cls, raw):
        return raw
    def p2r(cls, physical):
        assert(cls.min < physical < cls.max)
        return physical

