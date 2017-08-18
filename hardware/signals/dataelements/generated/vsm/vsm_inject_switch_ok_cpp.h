/*!
 * \file
 * C++ code generator for AUTOSAR v1.0
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved. Delphi Confidential
 * Generated at: 2017-04-03T08:04:02.284168
 * Source: SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml
 */

    case SignalGroup|ComConf_ComSignalGroup_igADataRawSafe_mrx:
    {
        if (sizeof(ADataRawSafe) == length) {
            log_verbose() << "Received ADataRawSafe (" << ComConf_ComSignalGroup_igADataRawSafe_mrx << ")";
            const ADataRawSafe& rteValue = *static_cast<const ADataRawSafe*>(buffer);
            autosar::ADataRawSafe_info::data_elem_type deValue;
            deValue.AVert = static_cast<decltype(deValue.AVert)>( toSignedFromRaw<15>(rteValue.AVert2) ) * 0.0085 + 0.0;
            deValue.ALgt = static_cast<decltype(deValue.ALgt)>( toSignedFromRaw<15>(rteValue.ALgt1) ) * 0.0085 + 0.0;
            deValue.Chks = static_cast<decltype(deValue.Chks)>(rteValue.ADataRawSafeChks);
            deValue.AVertQf = static_cast<decltype(deValue.AVertQf)>(rteValue.AVert2Qf1);
            deValue.ALat = static_cast<decltype(deValue.ALat)>( toSignedFromRaw<15>(rteValue.ALat1) ) * 0.0085 + 0.0;
            deValue.Cntr = static_cast<decltype(deValue.Cntr)>(rteValue.ADataRawSafeCntr);
            deValue.ALat1Qf = static_cast<decltype(deValue.ALat1Qf)>(rteValue.ALat1Qf1);
            deValue.ALgt1Qf = static_cast<decltype(deValue.ALgt1Qf)>(rteValue.ALgt1Qf1);

            ADataRawSafe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ADataRawSafe (" << ComConf_ComSignalGroup_igADataRawSafe_mrx << "). Got " << length << ", expected " << sizeof(ADataRawSafe);
        }
    }
    break;

    case ComConf_ComSignal_isAccAutResuWarnReq_mrx:
    {
        if (sizeof(AccAutResuWarnReq) == length) {
            log_verbose() << "Received AccAutResuWarnReq (" << ComConf_ComSignal_isAccAutResuWarnReq_mrx << ")";
            const AccAutResuWarnReq& rteValue = *static_cast<const AccAutResuWarnReq*>(buffer);
            autosar::AccAutResuWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AccAutResuWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AccAutResuWarnReq (" << ComConf_ComSignal_isAccAutResuWarnReq_mrx << "). Got " << length << ", expected " << sizeof(AccAutResuWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isAccSts_mrx:
    {
        if (sizeof(AccSts) == length) {
            log_verbose() << "Received AccSts (" << ComConf_ComSignal_isAccSts_mrx << ")";
            const AccSts& rteValue = *static_cast<const AccSts*>(buffer);
            autosar::AccSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AccSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AccSts (" << ComConf_ComSignal_isAccSts_mrx << "). Got " << length << ", expected " << sizeof(AccSts);
        }
    }
    break;

    case ComConf_ComSignal_isActvOfHorn_mrx:
    {
        if (sizeof(ActvOfHorn) == length) {
            log_verbose() << "Received ActvOfHorn (" << ComConf_ComSignal_isActvOfHorn_mrx << ")";
            const ActvOfHorn& rteValue = *static_cast<const ActvOfHorn*>(buffer);
            autosar::ActvOfHorn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ActvOfHorn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ActvOfHorn (" << ComConf_ComSignal_isActvOfHorn_mrx << "). Got " << length << ", expected " << sizeof(ActvOfHorn);
        }
    }
    break;

    case ComConf_ComSignal_isAdjSpdLimnSts_mrx:
    {
        if (sizeof(AdjSpdLimnSts) == length) {
            log_verbose() << "Received AdjSpdLimnSts (" << ComConf_ComSignal_isAdjSpdLimnSts_mrx << ")";
            const AdjSpdLimnSts& rteValue = *static_cast<const AdjSpdLimnSts*>(buffer);
            autosar::AdjSpdLimnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AdjSpdLimnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AdjSpdLimnSts (" << ComConf_ComSignal_isAdjSpdLimnSts_mrx << "). Got " << length << ", expected " << sizeof(AdjSpdLimnSts);
        }
    }
    break;

    case ComConf_ComSignal_isAdjSpdLimnWarn_mrx:
    {
        if (sizeof(AdjSpdLimnWarn) == length) {
            log_verbose() << "Received AdjSpdLimnWarn (" << ComConf_ComSignal_isAdjSpdLimnWarn_mrx << ")";
            const AdjSpdLimnWarn& rteValue = *static_cast<const AdjSpdLimnWarn*>(buffer);
            autosar::AdjSpdLimnWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AdjSpdLimnWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AdjSpdLimnWarn (" << ComConf_ComSignal_isAdjSpdLimnWarn_mrx << "). Got " << length << ", expected " << sizeof(AdjSpdLimnWarn);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igAgDataRawSafe_mrx:
    {
        if (sizeof(AgDataRawSafe) == length) {
            log_verbose() << "Received AgDataRawSafe (" << ComConf_ComSignalGroup_igAgDataRawSafe_mrx << ")";
            const AgDataRawSafe& rteValue = *static_cast<const AgDataRawSafe*>(buffer);
            autosar::AgDataRawSafe_info::data_elem_type deValue;
            deValue.YawRate = static_cast<decltype(deValue.YawRate)>( toSignedFromRaw<16>(rteValue.YawRate1) ) * 2.44140625E-4 + 0.0;
            deValue.RollRateQf = static_cast<decltype(deValue.RollRateQf)>(rteValue.RollRate1Qf1);
            deValue.Chks = static_cast<decltype(deValue.Chks)>(rteValue.AgDataRawSafeChks);
            deValue.RollRate = static_cast<decltype(deValue.RollRate)>( toSignedFromRaw<16>(rteValue.RollRate1) ) * 2.44140625E-4 + 0.0;
            deValue.Cntr = static_cast<decltype(deValue.Cntr)>(rteValue.AgDataRawSafeCntr);
            deValue.YawRateQf = static_cast<decltype(deValue.YawRateQf)>(rteValue.YawRate1Qf1);

            AgDataRawSafe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AgDataRawSafe (" << ComConf_ComSignalGroup_igAgDataRawSafe_mrx << "). Got " << length << ", expected " << sizeof(AgDataRawSafe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igAmbTIndcdWithUnit_mrx:
    {
        if (sizeof(AmbTIndcdWithUnit) == length) {
            log_verbose() << "Received AmbTIndcdWithUnit (" << ComConf_ComSignalGroup_igAmbTIndcdWithUnit_mrx << ")";
            const AmbTIndcdWithUnit& rteValue = *static_cast<const AmbTIndcdWithUnit*>(buffer);
            autosar::AmbTIndcdWithUnit_info::data_elem_type deValue;
            deValue.AmbTIndcdUnit_ = static_cast<decltype(deValue.AmbTIndcdUnit_)>(rteValue.AmbTIndcdUnit);
            deValue.QF = static_cast<decltype(deValue.QF)>(rteValue.AmbTIndcdQf);
            deValue.AmbTIndcd = static_cast<decltype(deValue.AmbTIndcd)>( toUnsignedFromRaw<12>(rteValue.AmbTIndcd) ) * 0.1 + -100.0;

            AmbTIndcdWithUnit_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AmbTIndcdWithUnit (" << ComConf_ComSignalGroup_igAmbTIndcdWithUnit_mrx << "). Got " << length << ", expected " << sizeof(AmbTIndcdWithUnit);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igAmbTRaw_mrx:
    {
        if (sizeof(AmbTRaw) == length) {
            log_verbose() << "Received AmbTRaw (" << ComConf_ComSignalGroup_igAmbTRaw_mrx << ")";
            const AmbTRaw& rteValue = *static_cast<const AmbTRaw*>(buffer);
            autosar::AmbTRaw_info::data_elem_type deValue;
            deValue.AmbTVal = static_cast<decltype(deValue.AmbTVal)>( toUnsignedFromRaw<11>(rteValue.AmbTRawVal) ) * 0.1 + -70.0;
            deValue.Qly = static_cast<decltype(deValue.Qly)>(rteValue.AmbTRawQly);

            AmbTRaw_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AmbTRaw (" << ComConf_ComSignalGroup_igAmbTRaw_mrx << "). Got " << length << ", expected " << sizeof(AmbTRaw);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igAsyLaneChg_mrx:
    {
        if (sizeof(AsyLaneChg) == length) {
            log_verbose() << "Received AsyLaneChg (" << ComConf_ComSignalGroup_igAsyLaneChg_mrx << ")";
            const AsyLaneChg& rteValue = *static_cast<const AsyLaneChg*>(buffer);
            autosar::AsyLaneChg_info::data_elem_type deValue;
            deValue.Psbl = static_cast<decltype(deValue.Psbl)>(rteValue.AsyLaneChgPsbl);
            deValue.Typ = static_cast<decltype(deValue.Typ)>(rteValue.AsyLaneChgTyp);
            deValue.Sts = static_cast<decltype(deValue.Sts)>(rteValue.AsyLaneChgSts);

            AsyLaneChg_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AsyLaneChg (" << ComConf_ComSignalGroup_igAsyLaneChg_mrx << "). Got " << length << ", expected " << sizeof(AsyLaneChg);
        }
    }
    break;

    case ComConf_ComSignal_isAsySteerApplyRqrd_mrx:
    {
        if (sizeof(AsySteerApplyRqrd) == length) {
            log_verbose() << "Received AsySteerApplyRqrd (" << ComConf_ComSignal_isAsySteerApplyRqrd_mrx << ")";
            const AsySteerApplyRqrd& rteValue = *static_cast<const AsySteerApplyRqrd*>(buffer);
            autosar::AsySteerApplyRqrd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AsySteerApplyRqrd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AsySteerApplyRqrd (" << ComConf_ComSignal_isAsySteerApplyRqrd_mrx << "). Got " << length << ", expected " << sizeof(AsySteerApplyRqrd);
        }
    }
    break;

    case ComConf_ComSignal_isAudMsgReq_mrx:
    {
        if (sizeof(AudMsgReq) == length) {
            log_verbose() << "Received AudMsgReq (" << ComConf_ComSignal_isAudMsgReq_mrx << ")";
            const AudMsgReq& rteValue = *static_cast<const AudMsgReq*>(buffer);
            autosar::AudMsgReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AudMsgReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AudMsgReq (" << ComConf_ComSignal_isAudMsgReq_mrx << "). Got " << length << ", expected " << sizeof(AudMsgReq);
        }
    }
    break;

    case ComConf_ComSignal_isBackCntrForMissCom_mrx:
    {
        if (sizeof(BackCntrForMissCom) == length) {
            log_verbose() << "Received BackCntrForMissCom (" << ComConf_ComSignal_isBackCntrForMissCom_mrx << ")";
            const BackCntrForMissCom& rteValue = *static_cast<const BackCntrForMissCom*>(buffer);
            autosar::BackCntrForMissCom_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BackCntrForMissCom_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BackCntrForMissCom (" << ComConf_ComSignal_isBackCntrForMissCom_mrx << "). Got " << length << ", expected " << sizeof(BackCntrForMissCom);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtDrvr_mrx:
    {
        if (sizeof(BltLockStAtDrvr) == length) {
            log_verbose() << "Received BltLockStAtDrvr (" << ComConf_ComSignalGroup_igBltLockStAtDrvr_mrx << ")";
            const BltLockStAtDrvr& rteValue = *static_cast<const BltLockStAtDrvr*>(buffer);
            autosar::BltLockStAtDrvr_info::data_elem_type deValue;
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtDrvrForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtDrvrForDevErrSts2);

            BltLockStAtDrvr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltLockStAtDrvr (" << ComConf_ComSignalGroup_igBltLockStAtDrvr_mrx << "). Got " << length << ", expected " << sizeof(BltLockStAtDrvr);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtPass_mrx:
    {
        if (sizeof(BltLockStAtPass) == length) {
            log_verbose() << "Received BltLockStAtPass (" << ComConf_ComSignalGroup_igBltLockStAtPass_mrx << ")";
            const BltLockStAtPass& rteValue = *static_cast<const BltLockStAtPass*>(buffer);
            autosar::BltLockStAtPass_info::data_elem_type deValue;
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtPassForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtPassForBltLockSts);

            BltLockStAtPass_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltLockStAtPass (" << ComConf_ComSignalGroup_igBltLockStAtPass_mrx << "). Got " << length << ", expected " << sizeof(BltLockStAtPass);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowSecLe_mrx:
    {
        if (sizeof(BltLockStAtRowSecLe) == length) {
            log_verbose() << "Received BltLockStAtRowSecLe (" << ComConf_ComSignalGroup_igBltLockStAtRowSecLe_mrx << ")";
            const BltLockStAtRowSecLe& rteValue = *static_cast<const BltLockStAtRowSecLe*>(buffer);
            autosar::BltLockStAtRowSecLe_info::data_elem_type deValue;
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowSecLeForBltLockEquid);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowSecLeForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowSecLeForBltLockSts);

            BltLockStAtRowSecLe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltLockStAtRowSecLe (" << ComConf_ComSignalGroup_igBltLockStAtRowSecLe_mrx << "). Got " << length << ", expected " << sizeof(BltLockStAtRowSecLe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowSecMid_mrx:
    {
        if (sizeof(BltLockStAtRowSecMid) == length) {
            log_verbose() << "Received BltLockStAtRowSecMid (" << ComConf_ComSignalGroup_igBltLockStAtRowSecMid_mrx << ")";
            const BltLockStAtRowSecMid& rteValue = *static_cast<const BltLockStAtRowSecMid*>(buffer);
            autosar::BltLockStAtRowSecMid_info::data_elem_type deValue;
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowSecMidForBltLockEquid);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowSecMidForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowSecMidForBltLockSts);

            BltLockStAtRowSecMid_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltLockStAtRowSecMid (" << ComConf_ComSignalGroup_igBltLockStAtRowSecMid_mrx << "). Got " << length << ", expected " << sizeof(BltLockStAtRowSecMid);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowSecRi_mrx:
    {
        if (sizeof(BltLockStAtRowSecRi) == length) {
            log_verbose() << "Received BltLockStAtRowSecRi (" << ComConf_ComSignalGroup_igBltLockStAtRowSecRi_mrx << ")";
            const BltLockStAtRowSecRi& rteValue = *static_cast<const BltLockStAtRowSecRi*>(buffer);
            autosar::BltLockStAtRowSecRi_info::data_elem_type deValue;
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowSecRiForBltLockSts);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowSecRiForBltLockSt1);
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowSecRiForBltLockEquid);

            BltLockStAtRowSecRi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltLockStAtRowSecRi (" << ComConf_ComSignalGroup_igBltLockStAtRowSecRi_mrx << "). Got " << length << ", expected " << sizeof(BltLockStAtRowSecRi);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowThrdLe_mrx:
    {
        if (sizeof(BltLockStAtRowThrdLe) == length) {
            log_verbose() << "Received BltLockStAtRowThrdLe (" << ComConf_ComSignalGroup_igBltLockStAtRowThrdLe_mrx << ")";
            const BltLockStAtRowThrdLe& rteValue = *static_cast<const BltLockStAtRowThrdLe*>(buffer);
            autosar::BltLockStAtRowThrdLe_info::data_elem_type deValue;
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowThrdLeForBltLockSt1);
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowThrdLeForBltLockEquid);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowThrdLeForBltLockSts);

            BltLockStAtRowThrdLe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltLockStAtRowThrdLe (" << ComConf_ComSignalGroup_igBltLockStAtRowThrdLe_mrx << "). Got " << length << ", expected " << sizeof(BltLockStAtRowThrdLe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowThrdRi_mrx:
    {
        if (sizeof(BltLockStAtRowThrdRi) == length) {
            log_verbose() << "Received BltLockStAtRowThrdRi (" << ComConf_ComSignalGroup_igBltLockStAtRowThrdRi_mrx << ")";
            const BltLockStAtRowThrdRi& rteValue = *static_cast<const BltLockStAtRowThrdRi*>(buffer);
            autosar::BltLockStAtRowThrdRi_info::data_elem_type deValue;
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowThrdRiForBltLockSts);
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowThrdRiForBltLockEquid);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowThrdRiForBltLockSt1);

            BltLockStAtRowThrdRi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltLockStAtRowThrdRi (" << ComConf_ComSignalGroup_igBltLockStAtRowThrdRi_mrx << "). Got " << length << ", expected " << sizeof(BltLockStAtRowThrdRi);
        }
    }
    break;

    case ComConf_ComSignal_isBltRmnSound1_mrx:
    {
        if (sizeof(BltRmnSound1) == length) {
            log_verbose() << "Received BltRmnSound1 (" << ComConf_ComSignal_isBltRmnSound1_mrx << ")";
            const BltRmnSound1& rteValue = *static_cast<const BltRmnSound1*>(buffer);
            autosar::BltRmnSound1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BltRmnSound1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltRmnSound1 (" << ComConf_ComSignal_isBltRmnSound1_mrx << "). Got " << length << ", expected " << sizeof(BltRmnSound1);
        }
    }
    break;

    case ComConf_ComSignal_isBltRmnSound2_mrx:
    {
        if (sizeof(BltRmnSound2) == length) {
            log_verbose() << "Received BltRmnSound2 (" << ComConf_ComSignal_isBltRmnSound2_mrx << ")";
            const BltRmnSound2& rteValue = *static_cast<const BltRmnSound2*>(buffer);
            autosar::BltRmnSound2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BltRmnSound2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltRmnSound2 (" << ComConf_ComSignal_isBltRmnSound2_mrx << "). Got " << length << ", expected " << sizeof(BltRmnSound2);
        }
    }
    break;

    case ComConf_ComSignal_isBltRmnSound3_mrx:
    {
        if (sizeof(BltRmnSound3) == length) {
            log_verbose() << "Received BltRmnSound3 (" << ComConf_ComSignal_isBltRmnSound3_mrx << ")";
            const BltRmnSound3& rteValue = *static_cast<const BltRmnSound3*>(buffer);
            autosar::BltRmnSound3_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BltRmnSound3_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltRmnSound3 (" << ComConf_ComSignal_isBltRmnSound3_mrx << "). Got " << length << ", expected " << sizeof(BltRmnSound3);
        }
    }
    break;

    case ComConf_ComSignal_isBltRmnSound4_mrx:
    {
        if (sizeof(BltRmnSound4) == length) {
            log_verbose() << "Received BltRmnSound4 (" << ComConf_ComSignal_isBltRmnSound4_mrx << ")";
            const BltRmnSound4& rteValue = *static_cast<const BltRmnSound4*>(buffer);
            autosar::BltRmnSound4_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BltRmnSound4_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BltRmnSound4 (" << ComConf_ComSignal_isBltRmnSound4_mrx << "). Got " << length << ", expected " << sizeof(BltRmnSound4);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBrkAndAbsWarnIndcnReqGroup_mrx:
    {
        if (sizeof(BrkAndAbsWarnIndcnReqGroup) == length) {
            log_verbose() << "Received BrkAndAbsWarnIndcnReqGroup (" << ComConf_ComSignalGroup_igBrkAndAbsWarnIndcnReqGroup_mrx << ")";
            const BrkAndAbsWarnIndcnReqGroup& rteValue = *static_cast<const BrkAndAbsWarnIndcnReqGroup*>(buffer);
            autosar::BrkAndAbsWarnIndcnReq_info::data_elem_type deValue;
            deValue.BrkWarnIndcnReq = static_cast<decltype(deValue.BrkWarnIndcnReq)>(rteValue.BrkWarnIndcnReq);
            deValue.BrkAndAbsWarnIndcnReqCntr = static_cast<decltype(deValue.BrkAndAbsWarnIndcnReqCntr)>(rteValue.BrkAndAbsWarnIndcnReqCntr);
            deValue.AbsWarnIndcnReq_ = static_cast<decltype(deValue.AbsWarnIndcnReq_)>(rteValue.AbsWarnIndcnReq);
            deValue.BrkAndAbsWarnIndcnReqChks = static_cast<decltype(deValue.BrkAndAbsWarnIndcnReqChks)>(rteValue.BrkAndAbsWarnIndcnReqChks);

            BrkAndAbsWarnIndcnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BrkAndAbsWarnIndcnReqGroup (" << ComConf_ComSignalGroup_igBrkAndAbsWarnIndcnReqGroup_mrx << "). Got " << length << ", expected " << sizeof(BrkAndAbsWarnIndcnReqGroup);
        }
    }
    break;

    case ComConf_ComSignal_isBrkFldLvl_mrx:
    {
        if (sizeof(BrkFldLvl) == length) {
            log_verbose() << "Received BrkFldLvl (" << ComConf_ComSignal_isBrkFldLvl_mrx << ")";
            const BrkFldLvl& rteValue = *static_cast<const BrkFldLvl*>(buffer);
            autosar::BrkFldLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BrkFldLvl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BrkFldLvl (" << ComConf_ComSignal_isBrkFldLvl_mrx << "). Got " << length << ", expected " << sizeof(BrkFldLvl);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBrkFricTqAtWhlAct_mrx:
    {
        if (sizeof(BrkFricTqAtWhlAct) == length) {
            log_verbose() << "Received BrkFricTqAtWhlAct (" << ComConf_ComSignalGroup_igBrkFricTqAtWhlAct_mrx << ")";
            const BrkFricTqAtWhlAct& rteValue = *static_cast<const BrkFricTqAtWhlAct*>(buffer);
            autosar::BrkFricTqAtWhlAct_info::data_elem_type deValue;
            deValue.BrkFricTqAtWhlFrntRiAct = static_cast<decltype(deValue.BrkFricTqAtWhlFrntRiAct)>( toUnsignedFromRaw<13>(rteValue.BrkFricTqAtWhlFrntRiAct) );
            deValue.BrkFricTqAtWhlFrntLeAct = static_cast<decltype(deValue.BrkFricTqAtWhlFrntLeAct)>( toUnsignedFromRaw<13>(rteValue.BrkFricTqAtWhlFrntLeAct) );
            deValue.BrkFricTqAtWhlReRiAct = static_cast<decltype(deValue.BrkFricTqAtWhlReRiAct)>( toUnsignedFromRaw<13>(rteValue.BrkFricTqAtWhlReRiAct) );
            deValue.BrkFricTqAtWhlReLeAct = static_cast<decltype(deValue.BrkFricTqAtWhlReLeAct)>( toUnsignedFromRaw<13>(rteValue.BrkFricTqAtWhlReLeAct) );

            BrkFricTqAtWhlAct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BrkFricTqAtWhlAct (" << ComConf_ComSignalGroup_igBrkFricTqAtWhlAct_mrx << "). Got " << length << ", expected " << sizeof(BrkFricTqAtWhlAct);
        }
    }
    break;

    case ComConf_ComSignal_isBrkRelsWarnReq_mrx:
    {
        if (sizeof(BrkRelsWarnReq) == length) {
            log_verbose() << "Received BrkRelsWarnReq (" << ComConf_ComSignal_isBrkRelsWarnReq_mrx << ")";
            const BrkRelsWarnReq& rteValue = *static_cast<const BrkRelsWarnReq*>(buffer);
            autosar::BrkRelsWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BrkRelsWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BrkRelsWarnReq (" << ComConf_ComSignal_isBrkRelsWarnReq_mrx << "). Got " << length << ", expected " << sizeof(BrkRelsWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isBtn1ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn1ForUsrSwtPanFrntReq) == length) {
            log_verbose() << "Received Btn1ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn1ForUsrSwtPanFrntReq_mrx << ")";
            const Btn1ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn1ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn1ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn1ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for Btn1ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn1ForUsrSwtPanFrntReq_mrx << "). Got " << length << ", expected " << sizeof(Btn1ForUsrSwtPanFrntReq);
        }
    }
    break;

    case ComConf_ComSignal_isBtn2ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn2ForUsrSwtPanFrntReq) == length) {
            log_verbose() << "Received Btn2ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn2ForUsrSwtPanFrntReq_mrx << ")";
            const Btn2ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn2ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn2ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn2ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for Btn2ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn2ForUsrSwtPanFrntReq_mrx << "). Got " << length << ", expected " << sizeof(Btn2ForUsrSwtPanFrntReq);
        }
    }
    break;

    case ComConf_ComSignal_isBtn3ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn3ForUsrSwtPanFrntReq) == length) {
            log_verbose() << "Received Btn3ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn3ForUsrSwtPanFrntReq_mrx << ")";
            const Btn3ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn3ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn3ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn3ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for Btn3ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn3ForUsrSwtPanFrntReq_mrx << "). Got " << length << ", expected " << sizeof(Btn3ForUsrSwtPanFrntReq);
        }
    }
    break;

    case ComConf_ComSignal_isBtn4ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn4ForUsrSwtPanFrntReq) == length) {
            log_verbose() << "Received Btn4ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn4ForUsrSwtPanFrntReq_mrx << ")";
            const Btn4ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn4ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn4ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn4ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for Btn4ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn4ForUsrSwtPanFrntReq_mrx << "). Got " << length << ", expected " << sizeof(Btn4ForUsrSwtPanFrntReq);
        }
    }
    break;

    case ComConf_ComSignal_isBtn5ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn5ForUsrSwtPanFrntReq) == length) {
            log_verbose() << "Received Btn5ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn5ForUsrSwtPanFrntReq_mrx << ")";
            const Btn5ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn5ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn5ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn5ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for Btn5ForUsrSwtPanFrntReq (" << ComConf_ComSignal_isBtn5ForUsrSwtPanFrntReq_mrx << "). Got " << length << ", expected " << sizeof(Btn5ForUsrSwtPanFrntReq);
        }
    }
    break;

    case ComConf_ComSignal_isBtnAudFbSts_mrx:
    {
        if (sizeof(BtnAudFbSts) == length) {
            log_verbose() << "Received BtnAudFbSts (" << ComConf_ComSignal_isBtnAudFbSts_mrx << ")";
            const BtnAudFbSts& rteValue = *static_cast<const BtnAudFbSts*>(buffer);
            autosar::BtnAudFbSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnAudFbSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BtnAudFbSts (" << ComConf_ComSignal_isBtnAudFbSts_mrx << "). Got " << length << ", expected " << sizeof(BtnAudFbSts);
        }
    }
    break;

    case ComConf_ComSignal_isBtnAudVolSts_mrx:
    {
        if (sizeof(BtnAudVolSts) == length) {
            log_verbose() << "Received BtnAudVolSts (" << ComConf_ComSignal_isBtnAudVolSts_mrx << ")";
            const BtnAudVolSts& rteValue = *static_cast<const BtnAudVolSts*>(buffer);
            autosar::BtnAudVolSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnAudVolSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BtnAudVolSts (" << ComConf_ComSignal_isBtnAudVolSts_mrx << "). Got " << length << ", expected " << sizeof(BtnAudVolSts);
        }
    }
    break;

    case ComConf_ComSignal_isBtnMmedLeRiSts_mrx:
    {
        if (sizeof(BtnMmedLeRiSts) == length) {
            log_verbose() << "Received BtnMmedLeRiSts (" << ComConf_ComSignal_isBtnMmedLeRiSts_mrx << ")";
            const BtnMmedLeRiSts& rteValue = *static_cast<const BtnMmedLeRiSts*>(buffer);
            autosar::BtnMmedLeRiSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnMmedLeRiSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BtnMmedLeRiSts (" << ComConf_ComSignal_isBtnMmedLeRiSts_mrx << "). Got " << length << ", expected " << sizeof(BtnMmedLeRiSts);
        }
    }
    break;

    case ComConf_ComSignal_isBtnMmedModSts_mrx:
    {
        if (sizeof(BtnMmedModSts) == length) {
            log_verbose() << "Received BtnMmedModSts (" << ComConf_ComSignal_isBtnMmedModSts_mrx << ")";
            const BtnMmedModSts& rteValue = *static_cast<const BtnMmedModSts*>(buffer);
            autosar::BtnMmedModSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnMmedModSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BtnMmedModSts (" << ComConf_ComSignal_isBtnMmedModSts_mrx << "). Got " << length << ", expected " << sizeof(BtnMmedModSts);
        }
    }
    break;

    case ComConf_ComSignal_isBtnMmedSetSts_mrx:
    {
        if (sizeof(BtnMmedSetSts) == length) {
            log_verbose() << "Received BtnMmedSetSts (" << ComConf_ComSignal_isBtnMmedSetSts_mrx << ")";
            const BtnMmedSetSts& rteValue = *static_cast<const BtnMmedSetSts*>(buffer);
            autosar::BtnMmedSetSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnMmedSetSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BtnMmedSetSts (" << ComConf_ComSignal_isBtnMmedSetSts_mrx << "). Got " << length << ", expected " << sizeof(BtnMmedSetSts);
        }
    }
    break;

    case ComConf_ComSignal_isCCSMBtn6_mrx:
    {
        if (sizeof(CCSMBtn6) == length) {
            log_verbose() << "Received CCSMBtn6 (" << ComConf_ComSignal_isCCSMBtn6_mrx << ")";
            const CCSMBtn6& rteValue = *static_cast<const CCSMBtn6*>(buffer);
            autosar::CCSMBtn6_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CCSMBtn6_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CCSMBtn6 (" << ComConf_ComSignal_isCCSMBtn6_mrx << "). Got " << length << ", expected " << sizeof(CCSMBtn6);
        }
    }
    break;

    case ComConf_ComSignal_isCarTiGlb_mrx:
    {
        if (sizeof(CarTiGlb) == length) {
            log_verbose() << "Received CarTiGlb (" << ComConf_ComSignal_isCarTiGlb_mrx << ")";
            const CarTiGlb& rteValue = *static_cast<const CarTiGlb*>(buffer);
            autosar::CarTiGlb_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.1 + 0.0;
            CarTiGlb_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CarTiGlb (" << ComConf_ComSignal_isCarTiGlb_mrx << "). Got " << length << ", expected " << sizeof(CarTiGlb);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igCchForFuEco_mrx:
    {
        if (sizeof(CchForFuEco) == length) {
            log_verbose() << "Received CchForFuEco (" << ComConf_ComSignalGroup_igCchForFuEco_mrx << ")";
            const CchForFuEco& rteValue = *static_cast<const CchForFuEco*>(buffer);
            autosar::CchForFuEco_info::data_elem_type deValue;
            deValue.GrdForFuEco = static_cast<decltype(deValue.GrdForFuEco)>( toUnsignedFromRaw<10>(rteValue.GrdForFuEco) ) * 0.1 + 0.0;
            deValue.BarForFuEco = static_cast<decltype(deValue.BarForFuEco)>( toUnsignedFromRaw<10>(rteValue.BarForFuEco) ) * 0.1 + 0.0;

            CchForFuEco_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CchForFuEco (" << ComConf_ComSignalGroup_igCchForFuEco_mrx << "). Got " << length << ", expected " << sizeof(CchForFuEco);
        }
    }
    break;

    case ComConf_ComSignal_isChdLockgProtnStsToHmi_mrx:
    {
        if (sizeof(ChdLockgProtnStsToHmi) == length) {
            log_verbose() << "Received ChdLockgProtnStsToHmi (" << ComConf_ComSignal_isChdLockgProtnStsToHmi_mrx << ")";
            const ChdLockgProtnStsToHmi& rteValue = *static_cast<const ChdLockgProtnStsToHmi*>(buffer);
            autosar::ChdLockgProtnStsToHmi_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ChdLockgProtnStsToHmi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ChdLockgProtnStsToHmi (" << ComConf_ComSignal_isChdLockgProtnStsToHmi_mrx << "). Got " << length << ", expected " << sizeof(ChdLockgProtnStsToHmi);
        }
    }
    break;

    case ComConf_ComSignal_isChdWinProtnStsAtDrvrRe_mrx:
    {
        if (sizeof(ChdWinProtnStsAtDrvrRe) == length) {
            log_verbose() << "Received ChdWinProtnStsAtDrvrRe (" << ComConf_ComSignal_isChdWinProtnStsAtDrvrRe_mrx << ")";
            const ChdWinProtnStsAtDrvrRe& rteValue = *static_cast<const ChdWinProtnStsAtDrvrRe*>(buffer);
            autosar::ChdWinProtnStsAtDrvrRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ChdWinProtnStsAtDrvrRe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ChdWinProtnStsAtDrvrRe (" << ComConf_ComSignal_isChdWinProtnStsAtDrvrRe_mrx << "). Got " << length << ", expected " << sizeof(ChdWinProtnStsAtDrvrRe);
        }
    }
    break;

    case ComConf_ComSignal_isChdWinProtnStsAtPassRe_mrx:
    {
        if (sizeof(ChdWinProtnStsAtPassRe) == length) {
            log_verbose() << "Received ChdWinProtnStsAtPassRe (" << ComConf_ComSignal_isChdWinProtnStsAtPassRe_mrx << ")";
            const ChdWinProtnStsAtPassRe& rteValue = *static_cast<const ChdWinProtnStsAtPassRe*>(buffer);
            autosar::ChdWinProtnStsAtPassRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ChdWinProtnStsAtPassRe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ChdWinProtnStsAtPassRe (" << ComConf_ComSignal_isChdWinProtnStsAtPassRe_mrx << "). Got " << length << ", expected " << sizeof(ChdWinProtnStsAtPassRe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igChrgrHwAprvdWirelsAdr_mrx:
    {
        if (sizeof(ChrgrHwAprvdWirelsAdr) == length) {
            log_verbose() << "Received ChrgrHwAprvdWirelsAdr (" << ComConf_ComSignalGroup_igChrgrHwAprvdWirelsAdr_mrx << ")";
            const ChrgrHwAprvdWirelsAdr& rteValue = *static_cast<const ChrgrHwAprvdWirelsAdr*>(buffer);
            autosar::ChrgrHwAprvdWirelsAdr_info::data_elem_type deValue;
            deValue.HwOffsAdr6 = static_cast<decltype(deValue.HwOffsAdr6)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr6);
            deValue.HwOffsAdr4 = static_cast<decltype(deValue.HwOffsAdr4)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr4);
            deValue.HwOffsAdr5 = static_cast<decltype(deValue.HwOffsAdr5)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr5);
            deValue.HwOffsAdr2 = static_cast<decltype(deValue.HwOffsAdr2)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr2);
            deValue.HwOffsAdr3 = static_cast<decltype(deValue.HwOffsAdr3)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr3);
            deValue.HwOffsAdr1 = static_cast<decltype(deValue.HwOffsAdr1)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr1);
            deValue.AprvdSts_ = static_cast<decltype(deValue.AprvdSts_)>(rteValue.ChrgrHwAprvdWirelsAdrAprvdSts);

            ChrgrHwAprvdWirelsAdr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ChrgrHwAprvdWirelsAdr (" << ComConf_ComSignalGroup_igChrgrHwAprvdWirelsAdr_mrx << "). Got " << length << ", expected " << sizeof(ChrgrHwAprvdWirelsAdr);
        }
    }
    break;

    case ComConf_ComSignal_isChrgrWirelsSts_mrx:
    {
        if (sizeof(ChrgrWirelsSts) == length) {
            log_verbose() << "Received ChrgrWirelsSts (" << ComConf_ComSignal_isChrgrWirelsSts_mrx << ")";
            const ChrgrWirelsSts& rteValue = *static_cast<const ChrgrWirelsSts*>(buffer);
            autosar::ChrgrWirelsSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ChrgrWirelsSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ChrgrWirelsSts (" << ComConf_ComSignal_isChrgrWirelsSts_mrx << "). Got " << length << ", expected " << sizeof(ChrgrWirelsSts);
        }
    }
    break;

    case ComConf_ComSignal_isClimaActv_mrx:
    {
        if (sizeof(ClimaActv) == length) {
            log_verbose() << "Received ClimaActv (" << ComConf_ComSignal_isClimaActv_mrx << ")";
            const ClimaActv& rteValue = *static_cast<const ClimaActv*>(buffer);
            autosar::ClimaActv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ClimaActv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ClimaActv (" << ComConf_ComSignal_isClimaActv_mrx << "). Got " << length << ", expected " << sizeof(ClimaActv);
        }
    }
    break;

    case ComConf_ComSignal_isClimaPwrCns_mrx:
    {
        if (sizeof(ClimaPwrCns) == length) {
            log_verbose() << "Received ClimaPwrCns (" << ComConf_ComSignal_isClimaPwrCns_mrx << ")";
            const ClimaPwrCns& rteValue = *static_cast<const ClimaPwrCns*>(buffer);
            autosar::ClimaPwrCns_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) );
            ClimaPwrCns_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ClimaPwrCns (" << ComConf_ComSignal_isClimaPwrCns_mrx << "). Got " << length << ", expected " << sizeof(ClimaPwrCns);
        }
    }
    break;

    case ComConf_ComSignal_isClimaTmrSts_mrx:
    {
        if (sizeof(ClimaTmrSts) == length) {
            log_verbose() << "Received ClimaTmrSts (" << ComConf_ComSignal_isClimaTmrSts_mrx << ")";
            const ClimaTmrSts& rteValue = *static_cast<const ClimaTmrSts*>(buffer);
            autosar::ClimaTmrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ClimaTmrSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ClimaTmrSts (" << ComConf_ComSignal_isClimaTmrSts_mrx << "). Got " << length << ", expected " << sizeof(ClimaTmrSts);
        }
    }
    break;

    case ComConf_ComSignal_isClimaWarn_mrx:
    {
        if (sizeof(ClimaWarn) == length) {
            log_verbose() << "Received ClimaWarn (" << ComConf_ComSignal_isClimaWarn_mrx << ")";
            const ClimaWarn& rteValue = *static_cast<const ClimaWarn*>(buffer);
            autosar::ClimaWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ClimaWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ClimaWarn (" << ComConf_ComSignal_isClimaWarn_mrx << "). Got " << length << ", expected " << sizeof(ClimaWarn);
        }
    }
    break;

    case ComConf_ComSignal_isCllsnFwdWarnReq_mrx:
    {
        if (sizeof(CllsnFwdWarnReq) == length) {
            log_verbose() << "Received CllsnFwdWarnReq (" << ComConf_ComSignal_isCllsnFwdWarnReq_mrx << ")";
            const CllsnFwdWarnReq& rteValue = *static_cast<const CllsnFwdWarnReq*>(buffer);
            autosar::CllsnFwdWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CllsnFwdWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CllsnFwdWarnReq (" << ComConf_ComSignal_isCllsnFwdWarnReq_mrx << "). Got " << length << ", expected " << sizeof(CllsnFwdWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isCllsnWarnReq_mrx:
    {
        if (sizeof(CllsnWarnReq) == length) {
            log_verbose() << "Received CllsnWarnReq (" << ComConf_ComSignal_isCllsnWarnReq_mrx << ")";
            const CllsnWarnReq& rteValue = *static_cast<const CllsnWarnReq*>(buffer);
            autosar::CllsnWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CllsnWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CllsnWarnReq (" << ComConf_ComSignal_isCllsnWarnReq_mrx << "). Got " << length << ", expected " << sizeof(CllsnWarnReq);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igCllsnWarnSide_mrx:
    {
        if (sizeof(CllsnWarnSide) == length) {
            log_verbose() << "Received CllsnWarnSide (" << ComConf_ComSignalGroup_igCllsnWarnSide_mrx << ")";
            const CllsnWarnSide& rteValue = *static_cast<const CllsnWarnSide*>(buffer);
            autosar::CllsnWarnSide_info::data_elem_type deValue;
            deValue.Le = static_cast<decltype(deValue.Le)>(rteValue.CllsnWarnSideLe);
            deValue.Ri = static_cast<decltype(deValue.Ri)>(rteValue.CllsnWarnSideRi);

            CllsnWarnSide_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CllsnWarnSide (" << ComConf_ComSignalGroup_igCllsnWarnSide_mrx << "). Got " << length << ", expected " << sizeof(CllsnWarnSide);
        }
    }
    break;

    case ComConf_ComSignal_isClngActv_mrx:
    {
        if (sizeof(ClngActv) == length) {
            log_verbose() << "Received ClngActv (" << ComConf_ComSignal_isClngActv_mrx << ")";
            const ClngActv& rteValue = *static_cast<const ClngActv*>(buffer);
            autosar::ClngActv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ClngActv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ClngActv (" << ComConf_ComSignal_isClngActv_mrx << "). Got " << length << ", expected " << sizeof(ClngActv);
        }
    }
    break;

    case ComConf_ComSignal_isCmftFctSts_mrx:
    {
        if (sizeof(CmftFctSts) == length) {
            log_verbose() << "Received CmftFctSts (" << ComConf_ComSignal_isCmftFctSts_mrx << ")";
            const CmftFctSts& rteValue = *static_cast<const CmftFctSts*>(buffer);
            autosar::CmftFctSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CmftFctSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CmftFctSts (" << ComConf_ComSignal_isCmftFctSts_mrx << "). Got " << length << ", expected " << sizeof(CmftFctSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igCmptmtAirTEstimdExtd_mrx:
    {
        if (sizeof(CmptmtAirTEstimdExtd) == length) {
            log_verbose() << "Received CmptmtAirTEstimdExtd (" << ComConf_ComSignalGroup_igCmptmtAirTEstimdExtd_mrx << ")";
            const CmptmtAirTEstimdExtd& rteValue = *static_cast<const CmptmtAirTEstimdExtd*>(buffer);
            autosar::CmptmtAirTEstimdExtd_info::data_elem_type deValue;
            deValue.ComptmtT = static_cast<decltype(deValue.ComptmtT)>( toUnsignedFromRaw<11>(rteValue.CmptmtAirTEstimdExtdComptmtT) ) * 0.1 + -60.0;
            deValue.QlyFlg = static_cast<decltype(deValue.QlyFlg)>(rteValue.CmptmtAirTEstimdExtdQlyFlg);

            CmptmtAirTEstimdExtd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CmptmtAirTEstimdExtd (" << ComConf_ComSignalGroup_igCmptmtAirTEstimdExtd_mrx << "). Got " << length << ", expected " << sizeof(CmptmtAirTEstimdExtd);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igCmptmtTFrntGroup_mrx:
    {
        if (sizeof(CmptmtTFrntGroup) == length) {
            log_verbose() << "Received CmptmtTFrntGroup (" << ComConf_ComSignalGroup_igCmptmtTFrntGroup_mrx << ")";
            const CmptmtTFrntGroup& rteValue = *static_cast<const CmptmtTFrntGroup*>(buffer);
            autosar::CmptmtTFrnt_info::data_elem_type deValue;
            deValue.CmptmtTFrnt = static_cast<decltype(deValue.CmptmtTFrnt)>( toUnsignedFromRaw<11>(rteValue.CmptmtTFrnt) ) * 0.1 + -60.0;
            deValue.CmptmtTFrntQf_ = static_cast<decltype(deValue.CmptmtTFrntQf_)>(rteValue.CmptmtTFrntQf);
            deValue.FanForCmptmtTRunng = static_cast<decltype(deValue.FanForCmptmtTRunng)>(rteValue.FanForCmptmtTRunng);

            CmptmtTFrnt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CmptmtTFrntGroup (" << ComConf_ComSignalGroup_igCmptmtTFrntGroup_mrx << "). Got " << length << ", expected " << sizeof(CmptmtTFrntGroup);
        }
    }
    break;

    case ComConf_ComSignal_isCnclWarnForCrsCtrl_mrx:
    {
        if (sizeof(CnclWarnForCrsCtrl) == length) {
            log_verbose() << "Received CnclWarnForCrsCtrl (" << ComConf_ComSignal_isCnclWarnForCrsCtrl_mrx << ")";
            const CnclWarnForCrsCtrl& rteValue = *static_cast<const CnclWarnForCrsCtrl*>(buffer);
            autosar::CnclWarnForCrsCtrl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CnclWarnForCrsCtrl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CnclWarnForCrsCtrl (" << ComConf_ComSignal_isCnclWarnForCrsCtrl_mrx << "). Got " << length << ", expected " << sizeof(CnclWarnForCrsCtrl);
        }
    }
    break;

    case ComConf_ComSignal_isCnclWarnLgtForAutDrv_mrx:
    {
        if (sizeof(CnclWarnLgtForAutDrv) == length) {
            log_verbose() << "Received CnclWarnLgtForAutDrv (" << ComConf_ComSignal_isCnclWarnLgtForAutDrv_mrx << ")";
            const CnclWarnLgtForAutDrv& rteValue = *static_cast<const CnclWarnLgtForAutDrv*>(buffer);
            autosar::CnclWarnLgtForAutDrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CnclWarnLgtForAutDrv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CnclWarnLgtForAutDrv (" << ComConf_ComSignal_isCnclWarnLgtForAutDrv_mrx << "). Got " << length << ", expected " << sizeof(CnclWarnLgtForAutDrv);
        }
    }
    break;

    case ComConf_ComSignal_isComLostExtrSound_mrx:
    {
        if (sizeof(ComLostExtrSound) == length) {
            log_verbose() << "Received ComLostExtrSound (" << ComConf_ComSignal_isComLostExtrSound_mrx << ")";
            const ComLostExtrSound& rteValue = *static_cast<const ComLostExtrSound*>(buffer);
            autosar::ComLostExtrSound_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ComLostExtrSound_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ComLostExtrSound (" << ComConf_ComSignal_isComLostExtrSound_mrx << "). Got " << length << ", expected " << sizeof(ComLostExtrSound);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igConSftyWarn_mrx:
    {
        if (sizeof(ConSftyWarn) == length) {
            log_verbose() << "Received ConSftyWarn (" << ComConf_ComSignalGroup_igConSftyWarn_mrx << ")";
            const ConSftyWarn& rteValue = *static_cast<const ConSftyWarn*>(buffer);
            autosar::ConSftyWarn_info::data_elem_type deValue;
            deValue.ConSftyWarnId = static_cast<decltype(deValue.ConSftyWarnId)>(rteValue.ConSftyWarnConSftyWarnId);
            deValue.DistanceToWarning = static_cast<decltype(deValue.DistanceToWarning)>(rteValue.ConSftyWarnDistanceToWarning);
            deValue.ConSftyWarnSnd = static_cast<decltype(deValue.ConSftyWarnSnd)>(rteValue.ConSftyWarnConSftyWarnSnd);
            deValue.ConSftyWarnLvl = static_cast<decltype(deValue.ConSftyWarnLvl)>(rteValue.ConSftyWarnConSftyWarnLvl);
            deValue.ConSftyWarnTyp = static_cast<decltype(deValue.ConSftyWarnTyp)>(rteValue.ConSftyWarnConSftyWarnTyp);

            ConSftyWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ConSftyWarn (" << ComConf_ComSignalGroup_igConSftyWarn_mrx << "). Got " << length << ", expected " << sizeof(ConSftyWarn);
        }
    }
    break;

    case ComConf_ComSignal_isCoolgStsForDisp_mrx:
    {
        if (sizeof(CoolgStsForDisp) == length) {
            log_verbose() << "Received CoolgStsForDisp (" << ComConf_ComSignal_isCoolgStsForDisp_mrx << ")";
            const CoolgStsForDisp& rteValue = *static_cast<const CoolgStsForDisp*>(buffer);
            autosar::CoolgStsForDisp_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CoolgStsForDisp_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CoolgStsForDisp (" << ComConf_ComSignal_isCoolgStsForDisp_mrx << "). Got " << length << ", expected " << sizeof(CoolgStsForDisp);
        }
    }
    break;

    case ComConf_ComSignal_isCrsCtrlrSts_mrx:
    {
        if (sizeof(CrsCtrlrSts) == length) {
            log_verbose() << "Received CrsCtrlrSts (" << ComConf_ComSignal_isCrsCtrlrSts_mrx << ")";
            const CrsCtrlrSts& rteValue = *static_cast<const CrsCtrlrSts*>(buffer);
            autosar::CrsCtrlrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CrsCtrlrSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CrsCtrlrSts (" << ComConf_ComSignal_isCrsCtrlrSts_mrx << "). Got " << length << ", expected " << sizeof(CrsCtrlrSts);
        }
    }
    break;

    case ComConf_ComSignal_isCrvtSpdAdpvSts_mrx:
    {
        if (sizeof(CrvtSpdAdpvSts) == length) {
            log_verbose() << "Received CrvtSpdAdpvSts (" << ComConf_ComSignal_isCrvtSpdAdpvSts_mrx << ")";
            const CrvtSpdAdpvSts& rteValue = *static_cast<const CrvtSpdAdpvSts*>(buffer);
            autosar::CrvtSpdAdpvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CrvtSpdAdpvSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CrvtSpdAdpvSts (" << ComConf_ComSignal_isCrvtSpdAdpvSts_mrx << "). Got " << length << ", expected " << sizeof(CrvtSpdAdpvSts);
        }
    }
    break;

    case ComConf_ComSignal_isCrvtSpdWarnReq_mrx:
    {
        if (sizeof(CrvtSpdWarnReq) == length) {
            log_verbose() << "Received CrvtSpdWarnReq (" << ComConf_ComSignal_isCrvtSpdWarnReq_mrx << ")";
            const CrvtSpdWarnReq& rteValue = *static_cast<const CrvtSpdWarnReq*>(buffer);
            autosar::CrvtSpdWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CrvtSpdWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CrvtSpdWarnReq (" << ComConf_ComSignal_isCrvtSpdWarnReq_mrx << "). Got " << length << ", expected " << sizeof(CrvtSpdWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isCrvtSpdWarnSts_mrx:
    {
        if (sizeof(CrvtSpdWarnSts) == length) {
            log_verbose() << "Received CrvtSpdWarnSts (" << ComConf_ComSignal_isCrvtSpdWarnSts_mrx << ")";
            const CrvtSpdWarnSts& rteValue = *static_cast<const CrvtSpdWarnSts*>(buffer);
            autosar::CrvtSpdWarnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CrvtSpdWarnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CrvtSpdWarnSts (" << ComConf_ComSignal_isCrvtSpdWarnSts_mrx << "). Got " << length << ", expected " << sizeof(CrvtSpdWarnSts);
        }
    }
    break;

    case ComConf_ComSignal_isCtraIndcnLe_mrx:
    {
        if (sizeof(CtraIndcnLe) == length) {
            log_verbose() << "Received CtraIndcnLe (" << ComConf_ComSignal_isCtraIndcnLe_mrx << ")";
            const CtraIndcnLe& rteValue = *static_cast<const CtraIndcnLe*>(buffer);
            autosar::CtraIndcnLe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CtraIndcnLe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CtraIndcnLe (" << ComConf_ComSignal_isCtraIndcnLe_mrx << "). Got " << length << ", expected " << sizeof(CtraIndcnLe);
        }
    }
    break;

    case ComConf_ComSignal_isCtraIndcnRi_mrx:
    {
        if (sizeof(CtraIndcnRi) == length) {
            log_verbose() << "Received CtraIndcnRi (" << ComConf_ComSignal_isCtraIndcnRi_mrx << ")";
            const CtraIndcnRi& rteValue = *static_cast<const CtraIndcnRi*>(buffer);
            autosar::CtraIndcnRi_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CtraIndcnRi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CtraIndcnRi (" << ComConf_ComSignal_isCtraIndcnRi_mrx << "). Got " << length << ", expected " << sizeof(CtraIndcnRi);
        }
    }
    break;

    case ComConf_ComSignal_isCtraOn1_mrx:
    {
        if (sizeof(CtraOn1) == length) {
            log_verbose() << "Received CtraOn1 (" << ComConf_ComSignal_isCtraOn1_mrx << ")";
            const CtraOn1& rteValue = *static_cast<const CtraOn1*>(buffer);
            autosar::CtraOn1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CtraOn1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for CtraOn1 (" << ComConf_ComSignal_isCtraOn1_mrx << "). Got " << length << ", expected " << sizeof(CtraOn1);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDataSpclDMSM_mrx:
    {
        if (sizeof(DataSpclDMSM) == length) {
            log_verbose() << "Received DataSpclDMSM (" << ComConf_ComSignalGroup_igDataSpclDMSM_mrx << ")";
            const DataSpclDMSM& rteValue = *static_cast<const DataSpclDMSM*>(buffer);
            autosar::DataSpclDMSM_info::data_elem_type deValue;
            deValue.DataNrSpcl3 = static_cast<decltype(deValue.DataNrSpcl3)>(rteValue.DataSpclDMSMDataNrSpcl3);
            deValue.DataIdn = static_cast<decltype(deValue.DataIdn)>(rteValue.DataSpclDMSMDataIdn);
            deValue.DataNrSpcl2 = static_cast<decltype(deValue.DataNrSpcl2)>(rteValue.DataSpclDMSMDataNrSpcl2);
            deValue.DataNrSpcl1 = static_cast<decltype(deValue.DataNrSpcl1)>(rteValue.DataSpclDMSMDataNrSpcl1);

            DataSpclDMSM_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DataSpclDMSM (" << ComConf_ComSignalGroup_igDataSpclDMSM_mrx << "). Got " << length << ", expected " << sizeof(DataSpclDMSM);
        }
    }
    break;

    case ComConf_ComSignal_isDayToSrv_mrx:
    {
        if (sizeof(DayToSrv) == length) {
            log_verbose() << "Received DayToSrv (" << ComConf_ComSignal_isDayToSrv_mrx << ")";
            const DayToSrv& rteValue = *static_cast<const DayToSrv*>(buffer);
            autosar::DayToSrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DayToSrv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DayToSrv (" << ComConf_ComSignal_isDayToSrv_mrx << "). Got " << length << ", expected " << sizeof(DayToSrv);
        }
    }
    break;

    case ComConf_ComSignal_isDiagcCCSM_mrx:
    {
        if (sizeof(DiagcCCSM) == length) {
            log_verbose() << "Received DiagcCCSM (" << ComConf_ComSignal_isDiagcCCSM_mrx << ")";
            const DiagcCCSM& rteValue = *static_cast<const DiagcCCSM*>(buffer);
            autosar::DiagcCCSM_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DiagcCCSM_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DiagcCCSM (" << ComConf_ComSignal_isDiagcCCSM_mrx << "). Got " << length << ", expected " << sizeof(DiagcCCSM);
        }
    }
    break;

    case ComConf_ComSignal_isDiagcRCSM_mrx:
    {
        if (sizeof(DiagcRCSM) == length) {
            log_verbose() << "Received DiagcRCSM (" << ComConf_ComSignal_isDiagcRCSM_mrx << ")";
            const DiagcRCSM& rteValue = *static_cast<const DiagcRCSM*>(buffer);
            autosar::DiagcRCSM_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DiagcRCSM_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DiagcRCSM (" << ComConf_ComSignal_isDiagcRCSM_mrx << "). Got " << length << ", expected " << sizeof(DiagcRCSM);
        }
    }
    break;

    case ComConf_ComSignal_isDiagcRSHC_mrx:
    {
        if (sizeof(DiagcRSHC) == length) {
            log_verbose() << "Received DiagcRSHC (" << ComConf_ComSignal_isDiagcRSHC_mrx << ")";
            const DiagcRSHC& rteValue = *static_cast<const DiagcRSHC*>(buffer);
            autosar::DiagcRSHC_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DiagcRSHC_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DiagcRSHC (" << ComConf_ComSignal_isDiagcRSHC_mrx << "). Got " << length << ", expected " << sizeof(DiagcRSHC);
        }
    }
    break;

    case ComConf_ComSignal_isDiagcStsDMSM_mrx:
    {
        if (sizeof(DiagcStsDMSM) == length) {
            log_verbose() << "Received DiagcStsDMSM (" << ComConf_ComSignal_isDiagcStsDMSM_mrx << ")";
            const DiagcStsDMSM& rteValue = *static_cast<const DiagcStsDMSM*>(buffer);
            autosar::DiagcStsDMSM_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DiagcStsDMSM_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DiagcStsDMSM (" << ComConf_ComSignal_isDiagcStsDMSM_mrx << "). Got " << length << ", expected " << sizeof(DiagcStsDMSM);
        }
    }
    break;

    case ComConf_ComSignal_isDispBattEgyIn_mrx:
    {
        if (sizeof(DispBattEgyIn) == length) {
            log_verbose() << "Received DispBattEgyIn (" << ComConf_ComSignal_isDispBattEgyIn_mrx << ")";
            const DispBattEgyIn& rteValue = *static_cast<const DispBattEgyIn*>(buffer);
            autosar::DispBattEgyIn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.5 + 0.0;
            DispBattEgyIn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DispBattEgyIn (" << ComConf_ComSignal_isDispBattEgyIn_mrx << "). Got " << length << ", expected " << sizeof(DispBattEgyIn);
        }
    }
    break;

    case ComConf_ComSignal_isDispBattEgyOut_mrx:
    {
        if (sizeof(DispBattEgyOut) == length) {
            log_verbose() << "Received DispBattEgyOut (" << ComConf_ComSignal_isDispBattEgyOut_mrx << ")";
            const DispBattEgyOut& rteValue = *static_cast<const DispBattEgyOut*>(buffer);
            autosar::DispBattEgyOut_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.5 + 0.0;
            DispBattEgyOut_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DispBattEgyOut (" << ComConf_ComSignal_isDispBattEgyOut_mrx << "). Got " << length << ", expected " << sizeof(DispBattEgyOut);
        }
    }
    break;

    case ComConf_ComSignal_isDispOfPrpsnMod_mrx:
    {
        if (sizeof(DispOfPrpsnMod) == length) {
            log_verbose() << "Received DispOfPrpsnMod (" << ComConf_ComSignal_isDispOfPrpsnMod_mrx << ")";
            const DispOfPrpsnMod& rteValue = *static_cast<const DispOfPrpsnMod*>(buffer);
            autosar::DispOfPrpsnMod_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DispOfPrpsnMod_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DispOfPrpsnMod (" << ComConf_ComSignal_isDispOfPrpsnMod_mrx << "). Got " << length << ", expected " << sizeof(DispOfPrpsnMod);
        }
    }
    break;

    case ComConf_ComSignal_isDoorDrvrLockReSts_mrx:
    {
        if (sizeof(DoorDrvrLockReSts) == length) {
            log_verbose() << "Received DoorDrvrLockReSts (" << ComConf_ComSignal_isDoorDrvrLockReSts_mrx << ")";
            const DoorDrvrLockReSts& rteValue = *static_cast<const DoorDrvrLockReSts*>(buffer);
            autosar::DoorDrvrLockReSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorDrvrLockReSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DoorDrvrLockReSts (" << ComConf_ComSignal_isDoorDrvrLockReSts_mrx << "). Got " << length << ", expected " << sizeof(DoorDrvrLockReSts);
        }
    }
    break;

    case ComConf_ComSignal_isDoorDrvrLockSts_mrx:
    {
        if (sizeof(DoorDrvrLockSts) == length) {
            log_verbose() << "Received DoorDrvrLockSts (" << ComConf_ComSignal_isDoorDrvrLockSts_mrx << ")";
            const DoorDrvrLockSts& rteValue = *static_cast<const DoorDrvrLockSts*>(buffer);
            autosar::DoorDrvrLockSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorDrvrLockSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DoorDrvrLockSts (" << ComConf_ComSignal_isDoorDrvrLockSts_mrx << "). Got " << length << ", expected " << sizeof(DoorDrvrLockSts);
        }
    }
    break;

    case ComConf_ComSignal_isDoorDrvrReSts_mrx:
    {
        if (sizeof(DoorDrvrReSts) == length) {
            log_verbose() << "Received DoorDrvrReSts (" << ComConf_ComSignal_isDoorDrvrReSts_mrx << ")";
            const DoorDrvrReSts& rteValue = *static_cast<const DoorDrvrReSts*>(buffer);
            autosar::DoorDrvrReSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorDrvrReSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DoorDrvrReSts (" << ComConf_ComSignal_isDoorDrvrReSts_mrx << "). Got " << length << ", expected " << sizeof(DoorDrvrReSts);
        }
    }
    break;

    case ComConf_ComSignal_isDoorDrvrSts_mrx:
    {
        if (sizeof(DoorDrvrSts) == length) {
            log_verbose() << "Received DoorDrvrSts (" << ComConf_ComSignal_isDoorDrvrSts_mrx << ")";
            const DoorDrvrSts& rteValue = *static_cast<const DoorDrvrSts*>(buffer);
            autosar::DoorDrvrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorDrvrSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DoorDrvrSts (" << ComConf_ComSignal_isDoorDrvrSts_mrx << "). Got " << length << ", expected " << sizeof(DoorDrvrSts);
        }
    }
    break;

    case ComConf_ComSignal_isDoorPassLockReSts_mrx:
    {
        if (sizeof(DoorPassLockReSts) == length) {
            log_verbose() << "Received DoorPassLockReSts (" << ComConf_ComSignal_isDoorPassLockReSts_mrx << ")";
            const DoorPassLockReSts& rteValue = *static_cast<const DoorPassLockReSts*>(buffer);
            autosar::DoorPassLockReSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorPassLockReSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DoorPassLockReSts (" << ComConf_ComSignal_isDoorPassLockReSts_mrx << "). Got " << length << ", expected " << sizeof(DoorPassLockReSts);
        }
    }
    break;

    case ComConf_ComSignal_isDoorPassLockSts_mrx:
    {
        if (sizeof(DoorPassLockSts) == length) {
            log_verbose() << "Received DoorPassLockSts (" << ComConf_ComSignal_isDoorPassLockSts_mrx << ")";
            const DoorPassLockSts& rteValue = *static_cast<const DoorPassLockSts*>(buffer);
            autosar::DoorPassLockSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorPassLockSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DoorPassLockSts (" << ComConf_ComSignal_isDoorPassLockSts_mrx << "). Got " << length << ", expected " << sizeof(DoorPassLockSts);
        }
    }
    break;

    case ComConf_ComSignal_isDoorPassReSts_mrx:
    {
        if (sizeof(DoorPassReSts) == length) {
            log_verbose() << "Received DoorPassReSts (" << ComConf_ComSignal_isDoorPassReSts_mrx << ")";
            const DoorPassReSts& rteValue = *static_cast<const DoorPassReSts*>(buffer);
            autosar::DoorPassReSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorPassReSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DoorPassReSts (" << ComConf_ComSignal_isDoorPassReSts_mrx << "). Got " << length << ", expected " << sizeof(DoorPassReSts);
        }
    }
    break;

    case ComConf_ComSignal_isDoorPassSts_mrx:
    {
        if (sizeof(DoorPassSts) == length) {
            log_verbose() << "Received DoorPassSts (" << ComConf_ComSignal_isDoorPassSts_mrx << ")";
            const DoorPassSts& rteValue = *static_cast<const DoorPassSts*>(buffer);
            autosar::DoorPassSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorPassSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DoorPassSts (" << ComConf_ComSignal_isDoorPassSts_mrx << "). Got " << length << ", expected " << sizeof(DoorPassSts);
        }
    }
    break;

    case ComConf_ComSignal_isDriveAwayInfoActvSts_mrx:
    {
        if (sizeof(DriveAwayInfoActvSts) == length) {
            log_verbose() << "Received DriveAwayInfoActvSts (" << ComConf_ComSignal_isDriveAwayInfoActvSts_mrx << ")";
            const DriveAwayInfoActvSts& rteValue = *static_cast<const DriveAwayInfoActvSts*>(buffer);
            autosar::DriveAwayInfoActvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DriveAwayInfoActvSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DriveAwayInfoActvSts (" << ComConf_ComSignal_isDriveAwayInfoActvSts_mrx << "). Got " << length << ", expected " << sizeof(DriveAwayInfoActvSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDriveAwayInfoWarnReq_mrx:
    {
        if (sizeof(DriveAwayInfoWarnReq) == length) {
            log_verbose() << "Received DriveAwayInfoWarnReq (" << ComConf_ComSignalGroup_igDriveAwayInfoWarnReq_mrx << ")";
            const DriveAwayInfoWarnReq& rteValue = *static_cast<const DriveAwayInfoWarnReq*>(buffer);
            autosar::DriveAwayInfoWarnReq_info::data_elem_type deValue;
            deValue.VisWarnReq = static_cast<decltype(deValue.VisWarnReq)>(rteValue.DriveAwayInfoWarnReqVisWarnReq);
            deValue.TiToDrvCntDwnTi = static_cast<decltype(deValue.TiToDrvCntDwnTi)>(rteValue.DriveAwayInfoWarnReqTiToDrvCntDwnTi);
            deValue.InterruptionSrc = static_cast<decltype(deValue.InterruptionSrc)>(rteValue.DriveAwayInfoWarnReqInterruptionSrc);
            deValue.AudWarnReq = static_cast<decltype(deValue.AudWarnReq)>(rteValue.DriveAwayInfoWarnReqAudWarnReq);
            deValue.CtrlDirOfTrfcLi = static_cast<decltype(deValue.CtrlDirOfTrfcLi)>(rteValue.DriveAwayInfoWarnReqCtrlDirOfTrfcLi);
            deValue.ReqSrc = static_cast<decltype(deValue.ReqSrc)>(rteValue.DriveAwayInfoWarnReqReqSrc);

            DriveAwayInfoWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DriveAwayInfoWarnReq (" << ComConf_ComSignalGroup_igDriveAwayInfoWarnReq_mrx << "). Got " << length << ", expected " << sizeof(DriveAwayInfoWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isDrvrCtrlOfPassSeatFrntSts_mrx:
    {
        if (sizeof(DrvrCtrlOfPassSeatFrntSts) == length) {
            log_verbose() << "Received DrvrCtrlOfPassSeatFrntSts (" << ComConf_ComSignal_isDrvrCtrlOfPassSeatFrntSts_mrx << ")";
            const DrvrCtrlOfPassSeatFrntSts& rteValue = *static_cast<const DrvrCtrlOfPassSeatFrntSts*>(buffer);
            autosar::DrvrCtrlOfPassSeatFrntSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrCtrlOfPassSeatFrntSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrCtrlOfPassSeatFrntSts (" << ComConf_ComSignal_isDrvrCtrlOfPassSeatFrntSts_mrx << "). Got " << length << ", expected " << sizeof(DrvrCtrlOfPassSeatFrntSts);
        }
    }
    break;

    case ComConf_ComSignal_isDrvrDesDir_mrx:
    {
        if (sizeof(DrvrDesDir) == length) {
            log_verbose() << "Received DrvrDesDir (" << ComConf_ComSignal_isDrvrDesDir_mrx << ")";
            const DrvrDesDir& rteValue = *static_cast<const DrvrDesDir*>(buffer);
            autosar::DrvrDesDir_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrDesDir_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrDesDir (" << ComConf_ComSignal_isDrvrDesDir_mrx << "). Got " << length << ", expected " << sizeof(DrvrDesDir);
        }
    }
    break;

    case ComConf_ComSignal_isDrvrMassgRunng_mrx:
    {
        if (sizeof(DrvrMassgRunng) == length) {
            log_verbose() << "Received DrvrMassgRunng (" << ComConf_ComSignal_isDrvrMassgRunng_mrx << ")";
            const DrvrMassgRunng& rteValue = *static_cast<const DrvrMassgRunng*>(buffer);
            autosar::DrvrMassgRunng_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrMassgRunng_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrMassgRunng (" << ComConf_ComSignal_isDrvrMassgRunng_mrx << "). Got " << length << ", expected " << sizeof(DrvrMassgRunng);
        }
    }
    break;

    case ComConf_ComSignal_isDrvrPfmncLvl_mrx:
    {
        if (sizeof(DrvrPfmncLvl) == length) {
            log_verbose() << "Received DrvrPfmncLvl (" << ComConf_ComSignal_isDrvrPfmncLvl_mrx << ")";
            const DrvrPfmncLvl& rteValue = *static_cast<const DrvrPfmncLvl*>(buffer);
            autosar::DrvrPfmncLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrPfmncLvl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrPfmncLvl (" << ComConf_ComSignal_isDrvrPfmncLvl_mrx << "). Got " << length << ", expected " << sizeof(DrvrPfmncLvl);
        }
    }
    break;

    case ComConf_ComSignal_isDrvrPfmncSts_mrx:
    {
        if (sizeof(DrvrPfmncSts) == length) {
            log_verbose() << "Received DrvrPfmncSts (" << ComConf_ComSignal_isDrvrPfmncSts_mrx << ")";
            const DrvrPfmncSts& rteValue = *static_cast<const DrvrPfmncSts*>(buffer);
            autosar::DrvrPfmncSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrPfmncSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrPfmncSts (" << ComConf_ComSignal_isDrvrPfmncSts_mrx << "). Got " << length << ", expected " << sizeof(DrvrPfmncSts);
        }
    }
    break;

    case ComConf_ComSignal_isDrvrPfmncWarnReq_mrx:
    {
        if (sizeof(DrvrPfmncWarnReq) == length) {
            log_verbose() << "Received DrvrPfmncWarnReq (" << ComConf_ComSignal_isDrvrPfmncWarnReq_mrx << ")";
            const DrvrPfmncWarnReq& rteValue = *static_cast<const DrvrPfmncWarnReq*>(buffer);
            autosar::DrvrPfmncWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrPfmncWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrPfmncWarnReq (" << ComConf_ComSignal_isDrvrPfmncWarnReq_mrx << "). Got " << length << ", expected " << sizeof(DrvrPfmncWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isDrvrSeatActvSpplFct_mrx:
    {
        if (sizeof(DrvrSeatActvSpplFct) == length) {
            log_verbose() << "Received DrvrSeatActvSpplFct (" << ComConf_ComSignal_isDrvrSeatActvSpplFct_mrx << ")";
            const DrvrSeatActvSpplFct& rteValue = *static_cast<const DrvrSeatActvSpplFct*>(buffer);
            autosar::DrvrSeatActvSpplFct_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrSeatActvSpplFct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrSeatActvSpplFct (" << ComConf_ComSignal_isDrvrSeatActvSpplFct_mrx << "). Got " << length << ", expected " << sizeof(DrvrSeatActvSpplFct);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDrvrSeatMassgFct_mrx:
    {
        if (sizeof(DrvrSeatMassgFct) == length) {
            log_verbose() << "Received DrvrSeatMassgFct (" << ComConf_ComSignalGroup_igDrvrSeatMassgFct_mrx << ")";
            const DrvrSeatMassgFct& rteValue = *static_cast<const DrvrSeatMassgFct*>(buffer);
            autosar::DrvrSeatMassgFct_info::data_elem_type deValue;
            deValue.MassgSpdLvl = static_cast<decltype(deValue.MassgSpdLvl)>(rteValue.DrvrSeatMassgFctMassgSpdLvl);
            deValue.MassgInten = static_cast<decltype(deValue.MassgInten)>(rteValue.DrvrSeatMassgFctMassgInten);
            deValue.MassgProg = static_cast<decltype(deValue.MassgProg)>(rteValue.DrvrSeatMassgFctMassgProg);
            deValue.OnOff = static_cast<decltype(deValue.OnOff)>(rteValue.DrvrSeatMassgFctOnOff);

            DrvrSeatMassgFct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrSeatMassgFct (" << ComConf_ComSignalGroup_igDrvrSeatMassgFct_mrx << "). Got " << length << ", expected " << sizeof(DrvrSeatMassgFct);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDrvrSeatSwtSts_mrx:
    {
        if (sizeof(DrvrSeatSwtSts) == length) {
            log_verbose() << "Received DrvrSeatSwtSts (" << ComConf_ComSignalGroup_igDrvrSeatSwtSts_mrx << ")";
            const DrvrSeatSwtSts& rteValue = *static_cast<const DrvrSeatSwtSts*>(buffer);
            autosar::DrvrSeatSwtSts_info::data_elem_type deValue;
            deValue.DrvrSeatSwtInclSts = static_cast<decltype(deValue.DrvrSeatSwtInclSts)>(rteValue.DrvrSeatSwtInclSts);
            deValue.DrvrSeatSwtAdjmtOfSpplFctHozlSts = static_cast<decltype(deValue.DrvrSeatSwtAdjmtOfSpplFctHozlSts)>(rteValue.DrvrSeatSwtAdjmtOfSpplFctHozlSts);
            deValue.DrvrSeatSwtSelnOfSpplFctSts = static_cast<decltype(deValue.DrvrSeatSwtSelnOfSpplFctSts)>(rteValue.DrvrSeatSwtSelnOfSpplFctSts);
            deValue.DrvrSeatSwtSldSts = static_cast<decltype(deValue.DrvrSeatSwtSldSts)>(rteValue.DrvrSeatSwtSldSts);
            deValue.DrvrSeatSwtHeiFrntSts = static_cast<decltype(deValue.DrvrSeatSwtHeiFrntSts)>(rteValue.DrvrSeatSwtHeiFrntSts);
            deValue.DrvrSeatSwtHeiSts = static_cast<decltype(deValue.DrvrSeatSwtHeiSts)>(rteValue.DrvrSeatSwtHeiSts);
            deValue.DrvrSeatSwtAdjmtOfSpplFctVertSts = static_cast<decltype(deValue.DrvrSeatSwtAdjmtOfSpplFctVertSts)>(rteValue.DrvrSeatSwtAdjmtOfSpplFctVertSts);

            DrvrSeatSwtSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrSeatSwtSts (" << ComConf_ComSignalGroup_igDrvrSeatSwtSts_mrx << "). Got " << length << ", expected " << sizeof(DrvrSeatSwtSts);
        }
    }
    break;

    case ComConf_ComSignal_isDrvrWLoadLvl_mrx:
    {
        if (sizeof(DrvrWLoadLvl) == length) {
            log_verbose() << "Received DrvrWLoadLvl (" << ComConf_ComSignal_isDrvrWLoadLvl_mrx << ")";
            const DrvrWLoadLvl& rteValue = *static_cast<const DrvrWLoadLvl*>(buffer);
            autosar::DrvrWLoadLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrWLoadLvl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DrvrWLoadLvl (" << ComConf_ComSignal_isDrvrWLoadLvl_mrx << "). Got " << length << ", expected " << sizeof(DrvrWLoadLvl);
        }
    }
    break;

    case ComConf_ComSignal_isDstNotifSts_mrx:
    {
        if (sizeof(DstNotifSts) == length) {
            log_verbose() << "Received DstNotifSts (" << ComConf_ComSignal_isDstNotifSts_mrx << ")";
            const DstNotifSts& rteValue = *static_cast<const DstNotifSts*>(buffer);
            autosar::DstNotifSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DstNotifSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DstNotifSts (" << ComConf_ComSignal_isDstNotifSts_mrx << "). Got " << length << ", expected " << sizeof(DstNotifSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDstToEmptyIndcd_mrx:
    {
        if (sizeof(DstToEmptyIndcd) == length) {
            log_verbose() << "Received DstToEmptyIndcd (" << ComConf_ComSignalGroup_igDstToEmptyIndcd_mrx << ")";
            const DstToEmptyIndcd& rteValue = *static_cast<const DstToEmptyIndcd*>(buffer);
            autosar::DstToEmptyIndcd_info::data_elem_type deValue;
            deValue.DstToEmpty = static_cast<decltype(deValue.DstToEmpty)>(rteValue.DstToEmpty);
            deValue.DstUnit_ = static_cast<decltype(deValue.DstUnit_)>(rteValue.DstUnit);

            DstToEmptyIndcd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DstToEmptyIndcd (" << ComConf_ComSignalGroup_igDstToEmptyIndcd_mrx << "). Got " << length << ", expected " << sizeof(DstToEmptyIndcd);
        }
    }
    break;

    case ComConf_ComSignal_isDstToSrv_mrx:
    {
        if (sizeof(DstToSrv) == length) {
            log_verbose() << "Received DstToSrv (" << ComConf_ComSignal_isDstToSrv_mrx << ")";
            const DstToSrv& rteValue = *static_cast<const DstToSrv*>(buffer);
            autosar::DstToSrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DstToSrv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DstToSrv (" << ComConf_ComSignal_isDstToSrv_mrx << "). Got " << length << ", expected " << sizeof(DstToSrv);
        }
    }
    break;

    case ComConf_ComSignal_isDstTrvld1_mrx:
    {
        if (sizeof(DstTrvld1) == length) {
            log_verbose() << "Received DstTrvld1 (" << ComConf_ComSignal_isDstTrvld1_mrx << ")";
            const DstTrvld1& rteValue = *static_cast<const DstTrvld1*>(buffer);
            autosar::DstTrvld1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.1 + 0.0;
            DstTrvld1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DstTrvld1 (" << ComConf_ComSignal_isDstTrvld1_mrx << "). Got " << length << ", expected " << sizeof(DstTrvld1);
        }
    }
    break;

    case ComConf_ComSignal_isDstTrvld2_mrx:
    {
        if (sizeof(DstTrvld2) == length) {
            log_verbose() << "Received DstTrvld2 (" << ComConf_ComSignal_isDstTrvld2_mrx << ")";
            const DstTrvld2& rteValue = *static_cast<const DstTrvld2*>(buffer);
            autosar::DstTrvld2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.1 + 0.0;
            DstTrvld2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DstTrvld2 (" << ComConf_ComSignal_isDstTrvld2_mrx << "). Got " << length << ", expected " << sizeof(DstTrvld2);
        }
    }
    break;

    case ComConf_ComSignal_isDstTrvldHiResl_mrx:
    {
        if (sizeof(DstTrvldHiResl) == length) {
            log_verbose() << "Received DstTrvldHiResl (" << ComConf_ComSignal_isDstTrvldHiResl_mrx << ")";
            const DstTrvldHiResl& rteValue = *static_cast<const DstTrvldHiResl*>(buffer);
            autosar::DstTrvldHiResl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DstTrvldHiResl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DstTrvldHiResl (" << ComConf_ComSignal_isDstTrvldHiResl_mrx << "). Got " << length << ", expected " << sizeof(DstTrvldHiResl);
        }
    }
    break;

    case ComConf_ComSignal_isBkpOfDstTrvld_mrx:
    {
        if (sizeof(BkpOfDstTrvld) == length) {
            log_verbose() << "Received BkpOfDstTrvld (" << ComConf_ComSignal_isBkpOfDstTrvld_mrx << ")";
            const BkpOfDstTrvld& rteValue = *static_cast<const BkpOfDstTrvld*>(buffer);
            autosar::BkpOfDstTrvld_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) );
            BkpOfDstTrvld_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for BkpOfDstTrvld (" << ComConf_ComSignal_isBkpOfDstTrvld_mrx << "). Got " << length << ", expected " << sizeof(BkpOfDstTrvld);
        }
    }
    break;

    case ComConf_ComSignal_isDstTrvldMst2_mrx:
    {
        if (sizeof(DstTrvldMst2) == length) {
            log_verbose() << "Received DstTrvldMst2 (" << ComConf_ComSignal_isDstTrvldMst2_mrx << ")";
            const DstTrvldMst2& rteValue = *static_cast<const DstTrvldMst2*>(buffer);
            autosar::DstTrvldMst2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DstTrvldMst2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for DstTrvldMst2 (" << ComConf_ComSignal_isDstTrvldMst2_mrx << "). Got " << length << ", expected " << sizeof(DstTrvldMst2);
        }
    }
    break;

    case ComConf_ComSignal_isEmgyAsscSts_mrx:
    {
        if (sizeof(EmgyAsscSts) == length) {
            log_verbose() << "Received EmgyAsscSts (" << ComConf_ComSignal_isEmgyAsscSts_mrx << ")";
            const EmgyAsscSts& rteValue = *static_cast<const EmgyAsscSts*>(buffer);
            autosar::EmgyAsscSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EmgyAsscSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EmgyAsscSts (" << ComConf_ComSignal_isEmgyAsscSts_mrx << "). Got " << length << ", expected " << sizeof(EmgyAsscSts);
        }
    }
    break;

    case ComConf_ComSignal_isEmgyVehWarnSts_mrx:
    {
        if (sizeof(EmgyVehWarnSts) == length) {
            log_verbose() << "Received EmgyVehWarnSts (" << ComConf_ComSignal_isEmgyVehWarnSts_mrx << ")";
            const EmgyVehWarnSts& rteValue = *static_cast<const EmgyVehWarnSts*>(buffer);
            autosar::EmgyVehWarnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EmgyVehWarnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EmgyVehWarnSts (" << ComConf_ComSignal_isEmgyVehWarnSts_mrx << "). Got " << length << ", expected " << sizeof(EmgyVehWarnSts);
        }
    }
    break;

    case ComConf_ComSignal_isEngCooltIndcnReq_mrx:
    {
        if (sizeof(EngCooltIndcnReq) == length) {
            log_verbose() << "Received EngCooltIndcnReq (" << ComConf_ComSignal_isEngCooltIndcnReq_mrx << ")";
            const EngCooltIndcnReq& rteValue = *static_cast<const EngCooltIndcnReq*>(buffer);
            autosar::EngCooltIndcnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngCooltIndcnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngCooltIndcnReq (" << ComConf_ComSignal_isEngCooltIndcnReq_mrx << "). Got " << length << ", expected " << sizeof(EngCooltIndcnReq);
        }
    }
    break;

    case ComConf_ComSignal_isEngCooltLvl_mrx:
    {
        if (sizeof(EngCooltLvl) == length) {
            log_verbose() << "Received EngCooltLvl (" << ComConf_ComSignal_isEngCooltLvl_mrx << ")";
            const EngCooltLvl& rteValue = *static_cast<const EngCooltLvl*>(buffer);
            autosar::EngCooltLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngCooltLvl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngCooltLvl (" << ComConf_ComSignal_isEngCooltLvl_mrx << "). Got " << length << ", expected " << sizeof(EngCooltLvl);
        }
    }
    break;

    case ComConf_ComSignal_isEngFuCns_mrx:
    {
        if (sizeof(EngFuCns) == length) {
            log_verbose() << "Received EngFuCns (" << ComConf_ComSignal_isEngFuCns_mrx << ")";
            const EngFuCns& rteValue = *static_cast<const EngFuCns*>(buffer);
            autosar::EngFuCns_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 200.0 + 0.0;
            EngFuCns_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngFuCns (" << ComConf_ComSignal_isEngFuCns_mrx << "). Got " << length << ", expected " << sizeof(EngFuCns);
        }
    }
    break;

    case ComConf_ComSignal_isEngFuCnsFild_mrx:
    {
        if (sizeof(EngFuCnsFild) == length) {
            log_verbose() << "Received EngFuCnsFild (" << ComConf_ComSignal_isEngFuCnsFild_mrx << ")";
            const EngFuCnsFild& rteValue = *static_cast<const EngFuCnsFild*>(buffer);
            autosar::EngFuCnsFild_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 200.0 + 0.0;
            EngFuCnsFild_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngFuCnsFild (" << ComConf_ComSignal_isEngFuCnsFild_mrx << "). Got " << length << ", expected " << sizeof(EngFuCnsFild);
        }
    }
    break;

    case ComConf_ComSignal_isEngHrToSrv_mrx:
    {
        if (sizeof(EngHrToSrv) == length) {
            log_verbose() << "Received EngHrToSrv (" << ComConf_ComSignal_isEngHrToSrv_mrx << ")";
            const EngHrToSrv& rteValue = *static_cast<const EngHrToSrv*>(buffer);
            autosar::EngHrToSrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngHrToSrv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngHrToSrv (" << ComConf_ComSignal_isEngHrToSrv_mrx << "). Got " << length << ", expected " << sizeof(EngHrToSrv);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igEngNSafe_mrx:
    {
        if (sizeof(EngNSafe) == length) {
            log_verbose() << "Received EngNSafe (" << ComConf_ComSignalGroup_igEngNSafe_mrx << ")";
            const EngNSafe& rteValue = *static_cast<const EngNSafe*>(buffer);
            autosar::EngNSafe_info::data_elem_type deValue;
            deValue.EngNGrdt = static_cast<decltype(deValue.EngNGrdt)>( toSignedFromRaw<16>(rteValue.EngNSafeEngNGrdt) );
            deValue.EngNChks = static_cast<decltype(deValue.EngNChks)>(rteValue.EngNChks);
            deValue.EngN = static_cast<decltype(deValue.EngN)>( toUnsignedFromRaw<15>(rteValue.EngN) ) * 0.5 + 0.0;
            deValue.EngNCntr = static_cast<decltype(deValue.EngNCntr)>(rteValue.EngNCntr);

            EngNSafe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngNSafe (" << ComConf_ComSignalGroup_igEngNSafe_mrx << "). Got " << length << ", expected " << sizeof(EngNSafe);
        }
    }
    break;

    case ComConf_ComSignal_isEngOilLvl_mrx:
    {
        if (sizeof(EngOilLvl) == length) {
            log_verbose() << "Received EngOilLvl (" << ComConf_ComSignal_isEngOilLvl_mrx << ")";
            const EngOilLvl& rteValue = *static_cast<const EngOilLvl*>(buffer);
            autosar::EngOilLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.5 + 0.0;
            EngOilLvl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngOilLvl (" << ComConf_ComSignal_isEngOilLvl_mrx << "). Got " << length << ", expected " << sizeof(EngOilLvl);
        }
    }
    break;

    case ComConf_ComSignal_isEngOilLvlSts_mrx:
    {
        if (sizeof(EngOilLvlSts) == length) {
            log_verbose() << "Received EngOilLvlSts (" << ComConf_ComSignal_isEngOilLvlSts_mrx << ")";
            const EngOilLvlSts& rteValue = *static_cast<const EngOilLvlSts*>(buffer);
            autosar::EngOilLvlSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngOilLvlSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngOilLvlSts (" << ComConf_ComSignal_isEngOilLvlSts_mrx << "). Got " << length << ", expected " << sizeof(EngOilLvlSts);
        }
    }
    break;

    case ComConf_ComSignal_isEngOilPWarn_mrx:
    {
        if (sizeof(EngOilPWarn) == length) {
            log_verbose() << "Received EngOilPWarn (" << ComConf_ComSignal_isEngOilPWarn_mrx << ")";
            const EngOilPWarn& rteValue = *static_cast<const EngOilPWarn*>(buffer);
            autosar::EngOilPWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngOilPWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngOilPWarn (" << ComConf_ComSignal_isEngOilPWarn_mrx << "). Got " << length << ", expected " << sizeof(EngOilPWarn);
        }
    }
    break;

    case ComConf_ComSignal_isEngSpdDispd_mrx:
    {
        if (sizeof(EngSpdDispd) == length) {
            log_verbose() << "Received EngSpdDispd (" << ComConf_ComSignal_isEngSpdDispd_mrx << ")";
            const EngSpdDispd& rteValue = *static_cast<const EngSpdDispd*>(buffer);
            autosar::EngSpdDispd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.5 + 0.0;
            EngSpdDispd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EngSpdDispd (" << ComConf_ComSignal_isEngSpdDispd_mrx << "). Got " << length << ", expected " << sizeof(EngSpdDispd);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igEpbLampReqGroup_mrx:
    {
        if (sizeof(EpbLampReqGroup) == length) {
            log_verbose() << "Received EpbLampReqGroup (" << ComConf_ComSignalGroup_igEpbLampReqGroup_mrx << ")";
            const EpbLampReqGroup& rteValue = *static_cast<const EpbLampReqGroup*>(buffer);
            autosar::EpbLampReq_info::data_elem_type deValue;
            deValue.EpbLampReqCntr = static_cast<decltype(deValue.EpbLampReqCntr)>( toUnsignedFromRaw<4>(rteValue.EpbLampReqCntr) );
            deValue.EpbLampReq = static_cast<decltype(deValue.EpbLampReq)>(rteValue.EpbLampReq);
            deValue.EpbLampReqChks = static_cast<decltype(deValue.EpbLampReqChks)>( toUnsignedFromRaw<8>(rteValue.EpbLampReqChks) );

            EpbLampReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EpbLampReqGroup (" << ComConf_ComSignalGroup_igEpbLampReqGroup_mrx << "). Got " << length << ", expected " << sizeof(EpbLampReqGroup);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igEscStGroup_mrx:
    {
        if (sizeof(EscStGroup) == length) {
            log_verbose() << "Received EscStGroup (" << ComConf_ComSignalGroup_igEscStGroup_mrx << ")";
            const EscStGroup& rteValue = *static_cast<const EscStGroup*>(buffer);
            autosar::EscSt_info::data_elem_type deValue;
            deValue.EscStCntr = static_cast<decltype(deValue.EscStCntr)>(rteValue.EscStCntr);
            deValue.EscSt = static_cast<decltype(deValue.EscSt)>(rteValue.EscSt);
            deValue.EscStChks = static_cast<decltype(deValue.EscStChks)>(rteValue.EscStChks);

            EscSt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EscStGroup (" << ComConf_ComSignalGroup_igEscStGroup_mrx << "). Got " << length << ", expected " << sizeof(EscStGroup);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igEscWarnIndcnReqGroup_mrx:
    {
        if (sizeof(EscWarnIndcnReqGroup) == length) {
            log_verbose() << "Received EscWarnIndcnReqGroup (" << ComConf_ComSignalGroup_igEscWarnIndcnReqGroup_mrx << ")";
            const EscWarnIndcnReqGroup& rteValue = *static_cast<const EscWarnIndcnReqGroup*>(buffer);
            autosar::EscWarnIndcnReq_info::data_elem_type deValue;
            deValue.EscWarnIndcnReq_ = static_cast<decltype(deValue.EscWarnIndcnReq_)>(rteValue.EscWarnIndcnReq);
            deValue.EscWarnIndcnReqCntr = static_cast<decltype(deValue.EscWarnIndcnReqCntr)>(rteValue.EscWarnIndcnReqCntr);
            deValue.EscWarnIndcnReqChks = static_cast<decltype(deValue.EscWarnIndcnReqChks)>(rteValue.EscWarnIndcnReqChks);

            EscWarnIndcnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for EscWarnIndcnReqGroup (" << ComConf_ComSignalGroup_igEscWarnIndcnReqGroup_mrx << "). Got " << length << ", expected " << sizeof(EscWarnIndcnReqGroup);
        }
    }
    break;

    case ComConf_ComSignal_isFRNetworkStatus_mrx:
    {
        if (sizeof(FRNetworkStatus) == length) {
            log_verbose() << "Received FRNetworkStatus (" << ComConf_ComSignal_isFRNetworkStatus_mrx << ")";
            const FRNetworkStatus& rteValue = *static_cast<const FRNetworkStatus*>(buffer);
            autosar::FRNetworkStatus_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FRNetworkStatus_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FRNetworkStatus (" << ComConf_ComSignal_isFRNetworkStatus_mrx << "). Got " << length << ", expected " << sizeof(FRNetworkStatus);
        }
    }
    break;

    case ComConf_ComSignal_isFltIndcrTurnLeFrnt_mrx:
    {
        if (sizeof(FltIndcrTurnLeFrnt) == length) {
            log_verbose() << "Received FltIndcrTurnLeFrnt (" << ComConf_ComSignal_isFltIndcrTurnLeFrnt_mrx << ")";
            const FltIndcrTurnLeFrnt& rteValue = *static_cast<const FltIndcrTurnLeFrnt*>(buffer);
            autosar::FltIndcrTurnLeFrnt_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltIndcrTurnLeFrnt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FltIndcrTurnLeFrnt (" << ComConf_ComSignal_isFltIndcrTurnLeFrnt_mrx << "). Got " << length << ", expected " << sizeof(FltIndcrTurnLeFrnt);
        }
    }
    break;

    case ComConf_ComSignal_isFltIndcrTurnLeRe_mrx:
    {
        if (sizeof(FltIndcrTurnLeRe) == length) {
            log_verbose() << "Received FltIndcrTurnLeRe (" << ComConf_ComSignal_isFltIndcrTurnLeRe_mrx << ")";
            const FltIndcrTurnLeRe& rteValue = *static_cast<const FltIndcrTurnLeRe*>(buffer);
            autosar::FltIndcrTurnLeRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltIndcrTurnLeRe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FltIndcrTurnLeRe (" << ComConf_ComSignal_isFltIndcrTurnLeRe_mrx << "). Got " << length << ", expected " << sizeof(FltIndcrTurnLeRe);
        }
    }
    break;

    case ComConf_ComSignal_isFltIndcrTurnRiFrnt_mrx:
    {
        if (sizeof(FltIndcrTurnRiFrnt) == length) {
            log_verbose() << "Received FltIndcrTurnRiFrnt (" << ComConf_ComSignal_isFltIndcrTurnRiFrnt_mrx << ")";
            const FltIndcrTurnRiFrnt& rteValue = *static_cast<const FltIndcrTurnRiFrnt*>(buffer);
            autosar::FltIndcrTurnRiFrnt_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltIndcrTurnRiFrnt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FltIndcrTurnRiFrnt (" << ComConf_ComSignal_isFltIndcrTurnRiFrnt_mrx << "). Got " << length << ", expected " << sizeof(FltIndcrTurnRiFrnt);
        }
    }
    break;

    case ComConf_ComSignal_isFltIndcrTurnRiRe_mrx:
    {
        if (sizeof(FltIndcrTurnRiRe) == length) {
            log_verbose() << "Received FltIndcrTurnRiRe (" << ComConf_ComSignal_isFltIndcrTurnRiRe_mrx << ")";
            const FltIndcrTurnRiRe& rteValue = *static_cast<const FltIndcrTurnRiRe*>(buffer);
            autosar::FltIndcrTurnRiRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltIndcrTurnRiRe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FltIndcrTurnRiRe (" << ComConf_ComSignal_isFltIndcrTurnRiRe_mrx << "). Got " << length << ", expected " << sizeof(FltIndcrTurnRiRe);
        }
    }
    break;

    case ComConf_ComSignal_isFltOfLiDaytiRunngRi_mrx:
    {
        if (sizeof(FltOfLiDaytiRunngRi) == length) {
            log_verbose() << "Received FltOfLiDaytiRunngRi (" << ComConf_ComSignal_isFltOfLiDaytiRunngRi_mrx << ")";
            const FltOfLiDaytiRunngRi& rteValue = *static_cast<const FltOfLiDaytiRunngRi*>(buffer);
            autosar::FltOfLiDaytiRunngRi_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltOfLiDaytiRunngRi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FltOfLiDaytiRunngRi (" << ComConf_ComSignal_isFltOfLiDaytiRunngRi_mrx << "). Got " << length << ", expected " << sizeof(FltOfLiDaytiRunngRi);
        }
    }
    break;

    case ComConf_ComSignal_isFrntAxleWarn_mrx:
    {
        if (sizeof(FrntAxleWarn) == length) {
            log_verbose() << "Received FrntAxleWarn (" << ComConf_ComSignal_isFrntAxleWarn_mrx << ")";
            const FrntAxleWarn& rteValue = *static_cast<const FrntAxleWarn*>(buffer);
            autosar::FrntAxleWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FrntAxleWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FrntAxleWarn (" << ComConf_ComSignal_isFrntAxleWarn_mrx << "). Got " << length << ", expected " << sizeof(FrntAxleWarn);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igFrntWiprLvrReq2_mrx:
    {
        if (sizeof(FrntWiprLvrReq2) == length) {
            log_verbose() << "Received FrntWiprLvrReq2 (" << ComConf_ComSignalGroup_igFrntWiprLvrReq2_mrx << ")";
            const FrntWiprLvrReq2& rteValue = *static_cast<const FrntWiprLvrReq2*>(buffer);
            autosar::FrntWiprLvrReq2_info::data_elem_type deValue;
            deValue.FrntWiprLvrQf = static_cast<decltype(deValue.FrntWiprLvrQf)>(rteValue.FrntWiprLvrReq2FrntWiprLvrQf);
            deValue.FrntWiprLvrCrc = static_cast<decltype(deValue.FrntWiprLvrCrc)>(rteValue.FrntWiprLvrReq2FrntWiprLvrCrc);
            deValue.FrntWiprLvrCntr = static_cast<decltype(deValue.FrntWiprLvrCntr)>(rteValue.FrntWiprLvrReq2FrntWiprLvrCntr);
            deValue.FrntWiprLvrCmd1_ = static_cast<decltype(deValue.FrntWiprLvrCmd1_)>(rteValue.FrntWiprLvrReq2FrntWiprLvrCmd1);

            FrntWiprLvrReq2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FrntWiprLvrReq2 (" << ComConf_ComSignalGroup_igFrntWiprLvrReq2_mrx << "). Got " << length << ", expected " << sizeof(FrntWiprLvrReq2);
        }
    }
    break;

    case ComConf_ComSignal_isFuHeatrActv_mrx:
    {
        if (sizeof(FuHeatrActv) == length) {
            log_verbose() << "Received FuHeatrActv (" << ComConf_ComSignal_isFuHeatrActv_mrx << ")";
            const FuHeatrActv& rteValue = *static_cast<const FuHeatrActv*>(buffer);
            autosar::FuHeatrActv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FuHeatrActv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FuHeatrActv (" << ComConf_ComSignal_isFuHeatrActv_mrx << "). Got " << length << ", expected " << sizeof(FuHeatrActv);
        }
    }
    break;

    case ComConf_ComSignal_isFuHeatrFuCns1_mrx:
    {
        if (sizeof(FuHeatrFuCns1) == length) {
            log_verbose() << "Received FuHeatrFuCns1 (" << ComConf_ComSignal_isFuHeatrFuCns1_mrx << ")";
            const FuHeatrFuCns1& rteValue = *static_cast<const FuHeatrFuCns1*>(buffer);
            autosar::FuHeatrFuCns1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 200.0 + 0.0;
            FuHeatrFuCns1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FuHeatrFuCns1 (" << ComConf_ComSignal_isFuHeatrFuCns1_mrx << "). Got " << length << ", expected " << sizeof(FuHeatrFuCns1);
        }
    }
    break;

    case ComConf_ComSignal_isFuHeatrFuCnsDurgCyc1_mrx:
    {
        if (sizeof(FuHeatrFuCnsDurgCyc1) == length) {
            log_verbose() << "Received FuHeatrFuCnsDurgCyc1 (" << ComConf_ComSignal_isFuHeatrFuCnsDurgCyc1_mrx << ")";
            const FuHeatrFuCnsDurgCyc1& rteValue = *static_cast<const FuHeatrFuCnsDurgCyc1*>(buffer);
            autosar::FuHeatrFuCnsDurgCyc1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.01 + 0.0;
            FuHeatrFuCnsDurgCyc1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FuHeatrFuCnsDurgCyc1 (" << ComConf_ComSignal_isFuHeatrFuCnsDurgCyc1_mrx << "). Got " << length << ", expected " << sizeof(FuHeatrFuCnsDurgCyc1);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igFuLvlIndcd_mrx:
    {
        if (sizeof(FuLvlIndcd) == length) {
            log_verbose() << "Received FuLvlIndcd (" << ComConf_ComSignalGroup_igFuLvlIndcd_mrx << ")";
            const FuLvlIndcd& rteValue = *static_cast<const FuLvlIndcd*>(buffer);
            autosar::FuLvlIndcd_info::data_elem_type deValue;
            deValue.GenQF = static_cast<decltype(deValue.GenQF)>(rteValue.FuLvlIndcdQly);
            deValue.FuLvlValFromFuTbl = static_cast<decltype(deValue.FuLvlValFromFuTbl)>( toUnsignedFromRaw<10>(rteValue.FuLvlIndcdVal) ) * 0.2 + 0.0;

            FuLvlIndcd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FuLvlIndcd (" << ComConf_ComSignalGroup_igFuLvlIndcd_mrx << "). Got " << length << ", expected " << sizeof(FuLvlIndcd);
        }
    }
    break;

    case ComConf_ComSignal_isFuLvlLoIndcn_mrx:
    {
        if (sizeof(FuLvlLoIndcn) == length) {
            log_verbose() << "Received FuLvlLoIndcn (" << ComConf_ComSignal_isFuLvlLoIndcn_mrx << ")";
            const FuLvlLoIndcn& rteValue = *static_cast<const FuLvlLoIndcn*>(buffer);
            autosar::FuLvlLoIndcn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FuLvlLoIndcn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FuLvlLoIndcn (" << ComConf_ComSignal_isFuLvlLoIndcn_mrx << "). Got " << length << ", expected " << sizeof(FuLvlLoIndcn);
        }
    }
    break;

    case ComConf_ComSignal_isFuLvlLoWarn_mrx:
    {
        if (sizeof(FuLvlLoWarn) == length) {
            log_verbose() << "Received FuLvlLoWarn (" << ComConf_ComSignal_isFuLvlLoWarn_mrx << ")";
            const FuLvlLoWarn& rteValue = *static_cast<const FuLvlLoWarn*>(buffer);
            autosar::FuLvlLoWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FuLvlLoWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for FuLvlLoWarn (" << ComConf_ComSignal_isFuLvlLoWarn_mrx << "). Got " << length << ", expected " << sizeof(FuLvlLoWarn);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igGearIndcnRec_mrx:
    {
        if (sizeof(GearIndcnRec) == length) {
            log_verbose() << "Received GearIndcnRec (" << ComConf_ComSignalGroup_igGearIndcnRec_mrx << ")";
            const GearIndcnRec& rteValue = *static_cast<const GearIndcnRec*>(buffer);
            autosar::GearIndcnRec_info::data_elem_type deValue;
            deValue.GearIndcn = static_cast<decltype(deValue.GearIndcn)>(rteValue.GearIndcn);
            deValue.GearShiftIndcn_ = static_cast<decltype(deValue.GearShiftIndcn_)>(rteValue.GearShiftIndcn);
            deValue.GearTarIndcn = static_cast<decltype(deValue.GearTarIndcn)>(rteValue.GearTarIndcn);

            GearIndcnRec_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for GearIndcnRec (" << ComConf_ComSignalGroup_igGearIndcnRec_mrx << "). Got " << length << ", expected " << sizeof(GearIndcnRec);
        }
    }
    break;

    case ComConf_ComSignal_isHmiCenForDrvrHmi_mrx:
    {
        if (sizeof(HmiCenForDrvrHmi) == length) {
            log_verbose() << "Received HmiCenForDrvrHmi (" << ComConf_ComSignal_isHmiCenForDrvrHmi_mrx << ")";
            const HmiCenForDrvrHmi& rteValue = *static_cast<const HmiCenForDrvrHmi*>(buffer);
            autosar::HmiCenForDrvrHmi_info::data_elem_type deValue;
            for (unsigned int i=0; i<deValue.size(); ++i) deValue[i] = static_cast<autosar::HmiCenForDrvrHmi_info::data_elem_type::value_type>( rteValue[i] );
            HmiCenForDrvrHmi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HmiCenForDrvrHmi (" << ComConf_ComSignal_isHmiCenForDrvrHmi_mrx << "). Got " << length << ", expected " << sizeof(HmiCenForDrvrHmi);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igHMIDefrstSts_mrx:
    {
        if (sizeof(HMIDefrstSts) == length) {
            log_verbose() << "Received HMIDefrstSts (" << ComConf_ComSignalGroup_igHMIDefrstSts_mrx << ")";
            const HMIDefrstSts& rteValue = *static_cast<const HMIDefrstSts*>(buffer);
            autosar::HmiDefrstElecSts_info::data_elem_type deValue;
            deValue.Re = static_cast<decltype(deValue.Re)>(rteValue.HmiDfrstReSts);
            deValue.Frnt = static_cast<decltype(deValue.Frnt)>(rteValue.HmiDefrstFrntSts);
            deValue.Mirrr = static_cast<decltype(deValue.Mirrr)>(rteValue.HmiMirrDefrstSts);

            HmiDefrstElecSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HMIDefrstSts (" << ComConf_ComSignalGroup_igHMIDefrstSts_mrx << "). Got " << length << ", expected " << sizeof(HMIDefrstSts);
        }
    }
    break;

    case ComConf_ComSignal_isHoodSts_mrx:
    {
        if (sizeof(HoodSts) == length) {
            log_verbose() << "Received HoodSts (" << ComConf_ComSignal_isHoodSts_mrx << ")";
            const HoodSts& rteValue = *static_cast<const HoodSts*>(buffer);
            autosar::HoodSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HoodSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HoodSts (" << ComConf_ComSignal_isHoodSts_mrx << "). Got " << length << ", expected " << sizeof(HoodSts);
        }
    }
    break;

    case ComConf_ComSignal_isHudActvSts_mrx:
    {
        if (sizeof(HudActvSts) == length) {
            log_verbose() << "Received HudActvSts (" << ComConf_ComSignal_isHudActvSts_mrx << ")";
            const HudActvSts& rteValue = *static_cast<const HudActvSts*>(buffer);
            autosar::HudActvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HudActvSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HudActvSts (" << ComConf_ComSignal_isHudActvSts_mrx << "). Got " << length << ", expected " << sizeof(HudActvSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igHudDiagc_mrx:
    {
        if (sizeof(HudDiagc) == length) {
            log_verbose() << "Received HudDiagc (" << ComConf_ComSignalGroup_igHudDiagc_mrx << ")";
            const HudDiagc& rteValue = *static_cast<const HudDiagc*>(buffer);
            autosar::HudDiagc_info::data_elem_type deValue;
            deValue.HudTSts = static_cast<decltype(deValue.HudTSts)>(rteValue.HudDiagcHudTSts);
            deValue.HudCircShoSts = static_cast<decltype(deValue.HudCircShoSts)>(rteValue.HudDiagcHudCircShoSts);
            deValue.HudCricOpenSts = static_cast<decltype(deValue.HudCricOpenSts)>(rteValue.HudDiagcHudCricOpenSts);
            deValue.ImgHudTmpNotAvlSts = static_cast<decltype(deValue.ImgHudTmpNotAvlSts)>(rteValue.HudDiagcImgHudTmpNotAvlSts);
            deValue.ImgHudErrSts = static_cast<decltype(deValue.ImgHudErrSts)>(rteValue.HudDiagcImgHudErrSts);

            HudDiagc_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HudDiagc (" << ComConf_ComSignalGroup_igHudDiagc_mrx << "). Got " << length << ", expected " << sizeof(HudDiagc);
        }
    }
    break;

    case ComConf_ComSignal_isHudSts_mrx:
    {
        if (sizeof(HudSts) == length) {
            log_verbose() << "Received HudSts (" << ComConf_ComSignal_isHudSts_mrx << ")";
            const HudSts& rteValue = *static_cast<const HudSts*>(buffer);
            autosar::HudSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HudSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HudSts (" << ComConf_ComSignal_isHudSts_mrx << "). Got " << length << ", expected " << sizeof(HudSts);
        }
    }
    break;

    case ComConf_ComSignal_isHvacAirMFlowEstimd_mrx:
    {
        if (sizeof(HvacAirMFlowEstimd) == length) {
            log_verbose() << "Received HvacAirMFlowEstimd (" << ComConf_ComSignal_isHvacAirMFlowEstimd_mrx << ")";
            const HvacAirMFlowEstimd& rteValue = *static_cast<const HvacAirMFlowEstimd*>(buffer);
            autosar::HvacAirMFlowEstimd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) );
            HvacAirMFlowEstimd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HvacAirMFlowEstimd (" << ComConf_ComSignal_isHvacAirMFlowEstimd_mrx << "). Got " << length << ", expected " << sizeof(HvacAirMFlowEstimd);
        }
    }
    break;

    case ComConf_ComSignal_isHznRstExtd_mrx:
    {
        if (sizeof(HznRstExtd) == length) {
            log_verbose() << "Received HznRstExtd (" << ComConf_ComSignal_isHznRstExtd_mrx << ")";
            const HznRstExtd& rteValue = *static_cast<const HznRstExtd*>(buffer);
            autosar::HznRstExtd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HznRstExtd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HznRstExtd (" << ComConf_ComSignal_isHznRstExtd_mrx << "). Got " << length << ", expected " << sizeof(HznRstExtd);
        }
    }
    break;

    case ComConf_ComSignal_isHzrdLiWarnSts_mrx:
    {
        if (sizeof(HzrdLiWarnSts) == length) {
            log_verbose() << "Received HzrdLiWarnSts (" << ComConf_ComSignal_isHzrdLiWarnSts_mrx << ")";
            const HzrdLiWarnSts& rteValue = *static_cast<const HzrdLiWarnSts*>(buffer);
            autosar::HzrdLiWarnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HzrdLiWarnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for HzrdLiWarnSts (" << ComConf_ComSignal_isHzrdLiWarnSts_mrx << "). Got " << length << ", expected " << sizeof(HzrdLiWarnSts);
        }
    }
    break;

    case ComConf_ComSignal_isIndcnOfParkAssiSts_mrx:
    {
        if (sizeof(IndcnOfParkAssiSts) == length) {
            log_verbose() << "Received IndcnOfParkAssiSts (" << ComConf_ComSignal_isIndcnOfParkAssiSts_mrx << ")";
            const IndcnOfParkAssiSts& rteValue = *static_cast<const IndcnOfParkAssiSts*>(buffer);
            autosar::IndcnOfParkAssiSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IndcnOfParkAssiSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for IndcnOfParkAssiSts (" << ComConf_ComSignal_isIndcnOfParkAssiSts_mrx << "). Got " << length << ", expected " << sizeof(IndcnOfParkAssiSts);
        }
    }
    break;

    case ComConf_ComSignal_isIndcnOfPrkgAutSts_mrx:
    {
        if (sizeof(IndcnOfPrkgAutSts) == length) {
            log_verbose() << "Received IndcnOfPrkgAutSts (" << ComConf_ComSignal_isIndcnOfPrkgAutSts_mrx << ")";
            const IndcnOfPrkgAutSts& rteValue = *static_cast<const IndcnOfPrkgAutSts*>(buffer);
            autosar::IndcnOfPrkgAutSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IndcnOfPrkgAutSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for IndcnOfPrkgAutSts (" << ComConf_ComSignal_isIndcnOfPrkgAutSts_mrx << "). Got " << length << ", expected " << sizeof(IndcnOfPrkgAutSts);
        }
    }
    break;

    case ComConf_ComSignal_isIndcrDisp1WdSts_mrx:
    {
        if (sizeof(IndcrDisp1WdSts) == length) {
            log_verbose() << "Received IndcrDisp1WdSts (" << ComConf_ComSignal_isIndcrDisp1WdSts_mrx << ")";
            const IndcrDisp1WdSts& rteValue = *static_cast<const IndcrDisp1WdSts*>(buffer);
            autosar::IndcrDisp1WdSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IndcrDisp1WdSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for IndcrDisp1WdSts (" << ComConf_ComSignal_isIndcrDisp1WdSts_mrx << "). Got " << length << ", expected " << sizeof(IndcrDisp1WdSts);
        }
    }
    break;

    case ComConf_ComSignal_isIndcrTurnSts1WdSts_mrx:
    {
        if (sizeof(IndcrTurnSts1WdSts) == length) {
            log_verbose() << "Received IndcrTurnSts1WdSts (" << ComConf_ComSignal_isIndcrTurnSts1WdSts_mrx << ")";
            const IndcrTurnSts1WdSts& rteValue = *static_cast<const IndcrTurnSts1WdSts*>(buffer);
            autosar::IndcrTurnSts1WdSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IndcrTurnSts1WdSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for IndcrTurnSts1WdSts (" << ComConf_ComSignal_isIndcrTurnSts1WdSts_mrx << "). Got " << length << ", expected " << sizeof(IndcrTurnSts1WdSts);
        }
    }
    break;

    case ComConf_ComSignal_isIniValSigCfgIDBackboneFR_mrx:
    {
        if (sizeof(IniValSigCfgIDBackboneFR) == length) {
            log_verbose() << "Received IniValSigCfgIDBackboneFR (" << ComConf_ComSignal_isIniValSigCfgIDBackboneFR_mrx << ")";
            const IniValSigCfgIDBackboneFR& rteValue = *static_cast<const IniValSigCfgIDBackboneFR*>(buffer);
            autosar::IniValSigCfgIDBackboneFR_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IniValSigCfgIDBackboneFR_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for IniValSigCfgIDBackboneFR (" << ComConf_ComSignal_isIniValSigCfgIDBackboneFR_mrx << "). Got " << length << ", expected " << sizeof(IniValSigCfgIDBackboneFR);
        }
    }
    break;

    case ComConf_ComSignal_isIntrBriSts_mrx:
    {
        if (sizeof(IntrBriSts) == length) {
            log_verbose() << "Received IntrBriSts (" << ComConf_ComSignal_isIntrBriSts_mrx << ")";
            const IntrBriSts& rteValue = *static_cast<const IntrBriSts*>(buffer);
            autosar::IntrBriSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IntrBriSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for IntrBriSts (" << ComConf_ComSignal_isIntrBriSts_mrx << "). Got " << length << ", expected " << sizeof(IntrBriSts);
        }
    }
    break;

    case ComConf_ComSignal_isKeyLostWarnIndcn_mrx:
    {
        if (sizeof(KeyLostWarnIndcn) == length) {
            log_verbose() << "Received KeyLostWarnIndcn (" << ComConf_ComSignal_isKeyLostWarnIndcn_mrx << ")";
            const KeyLostWarnIndcn& rteValue = *static_cast<const KeyLostWarnIndcn*>(buffer);
            autosar::KeyLostWarnIndcn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            KeyLostWarnIndcn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for KeyLostWarnIndcn (" << ComConf_ComSignal_isKeyLostWarnIndcn_mrx << "). Got " << length << ", expected " << sizeof(KeyLostWarnIndcn);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igKeyReadStsToProfCtrl_mrx:
    {
        if (sizeof(KeyReadStsToProfCtrl) == length) {
            log_verbose() << "Received KeyReadStsToProfCtrl (" << ComConf_ComSignalGroup_igKeyReadStsToProfCtrl_mrx << ")";
            const KeyReadStsToProfCtrl& rteValue = *static_cast<const KeyReadStsToProfCtrl*>(buffer);
            autosar::KeyReadStsToProfCtrl_info::data_elem_type deValue;
            deValue.KeyId = static_cast<decltype(deValue.KeyId)>(rteValue.KeyReadStsToProfCtrlKeyId);
            deValue.Boolean = static_cast<decltype(deValue.Boolean)>(rteValue.KeyReadStsToProfCtrlBoolean);

            KeyReadStsToProfCtrl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for KeyReadStsToProfCtrl (" << ComConf_ComSignalGroup_igKeyReadStsToProfCtrl_mrx << "). Got " << length << ", expected " << sizeof(KeyReadStsToProfCtrl);
        }
    }
    break;

    case ComConf_ComSignal_isKeyRmnIndcn_mrx:
    {
        if (sizeof(KeyRmnIndcn) == length) {
            log_verbose() << "Received KeyRmnIndcn (" << ComConf_ComSignal_isKeyRmnIndcn_mrx << ")";
            const KeyRmnIndcn& rteValue = *static_cast<const KeyRmnIndcn*>(buffer);
            autosar::KeyRmnIndcn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            KeyRmnIndcn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for KeyRmnIndcn (" << ComConf_ComSignal_isKeyRmnIndcn_mrx << "). Got " << length << ", expected " << sizeof(KeyRmnIndcn);
        }
    }
    break;

    case ComConf_ComSignal_isKeySpdWarn_mrx:
    {
        if (sizeof(KeySpdWarn) == length) {
            log_verbose() << "Received KeySpdWarn (" << ComConf_ComSignal_isKeySpdWarn_mrx << ")";
            const KeySpdWarn& rteValue = *static_cast<const KeySpdWarn*>(buffer);
            autosar::KeySpdWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            KeySpdWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for KeySpdWarn (" << ComConf_ComSignal_isKeySpdWarn_mrx << "). Got " << length << ", expected " << sizeof(KeySpdWarn);
        }
    }
    break;

    case ComConf_ComSignal_isLaneChgWarnSts_mrx:
    {
        if (sizeof(LaneChgWarnSts) == length) {
            log_verbose() << "Received LaneChgWarnSts (" << ComConf_ComSignal_isLaneChgWarnSts_mrx << ")";
            const LaneChgWarnSts& rteValue = *static_cast<const LaneChgWarnSts*>(buffer);
            autosar::LaneChgWarnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LaneChgWarnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LaneChgWarnSts (" << ComConf_ComSignal_isLaneChgWarnSts_mrx << "). Got " << length << ", expected " << sizeof(LaneChgWarnSts);
        }
    }
    break;

    case ComConf_ComSignal_isLaneDetnStsForAutDrv_mrx:
    {
        if (sizeof(LaneDetnStsForAutDrv) == length) {
            log_verbose() << "Received LaneDetnStsForAutDrv (" << ComConf_ComSignal_isLaneDetnStsForAutDrv_mrx << ")";
            const LaneDetnStsForAutDrv& rteValue = *static_cast<const LaneDetnStsForAutDrv*>(buffer);
            autosar::LaneDetnStsForAutDrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LaneDetnStsForAutDrv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LaneDetnStsForAutDrv (" << ComConf_ComSignal_isLaneDetnStsForAutDrv_mrx << "). Got " << length << ", expected " << sizeof(LaneDetnStsForAutDrv);
        }
    }
    break;

    case ComConf_ComSignal_isLaneDetnStsForLaneKeepAid_mrx:
    {
        if (sizeof(LaneDetnStsForLaneKeepAid) == length) {
            log_verbose() << "Received LaneDetnStsForLaneKeepAid (" << ComConf_ComSignal_isLaneDetnStsForLaneKeepAid_mrx << ")";
            const LaneDetnStsForLaneKeepAid& rteValue = *static_cast<const LaneDetnStsForLaneKeepAid*>(buffer);
            autosar::LaneDetnStsForLaneKeepAid_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LaneDetnStsForLaneKeepAid_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LaneDetnStsForLaneKeepAid (" << ComConf_ComSignal_isLaneDetnStsForLaneKeepAid_mrx << "). Got " << length << ", expected " << sizeof(LaneDetnStsForLaneKeepAid);
        }
    }
    break;

    case ComConf_ComSignal_isLaneKeepAidSts_mrx:
    {
        if (sizeof(LaneKeepAidSts) == length) {
            log_verbose() << "Received LaneKeepAidSts (" << ComConf_ComSignal_isLaneKeepAidSts_mrx << ")";
            const LaneKeepAidSts& rteValue = *static_cast<const LaneKeepAidSts*>(buffer);
            autosar::LaneKeepAidSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LaneKeepAidSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LaneKeepAidSts (" << ComConf_ComSignal_isLaneKeepAidSts_mrx << "). Got " << length << ", expected " << sizeof(LaneKeepAidSts);
        }
    }
    break;

    case ComConf_ComSignal_isLatMovmtWarn_mrx:
    {
        if (sizeof(LatMovmtWarn) == length) {
            log_verbose() << "Received LatMovmtWarn (" << ComConf_ComSignal_isLatMovmtWarn_mrx << ")";
            const LatMovmtWarn& rteValue = *static_cast<const LatMovmtWarn*>(buffer);
            autosar::LatMovmtWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LatMovmtWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LatMovmtWarn (" << ComConf_ComSignal_isLatMovmtWarn_mrx << "). Got " << length << ", expected " << sizeof(LatMovmtWarn);
        }
    }
    break;

    case ComConf_ComSignal_isLcmaOn1_mrx:
    {
        if (sizeof(LcmaOn1) == length) {
            log_verbose() << "Received LcmaOn1 (" << ComConf_ComSignal_isLcmaOn1_mrx << ")";
            const LcmaOn1& rteValue = *static_cast<const LcmaOn1*>(buffer);
            autosar::LcmaOn1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LcmaOn1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LcmaOn1 (" << ComConf_ComSignal_isLcmaOn1_mrx << "). Got " << length << ", expected " << sizeof(LcmaOn1);
        }
    }
    break;

    case ComConf_ComSignal_isLiDrvrFltIndcrTurn_mrx:
    {
        if (sizeof(LiDrvrFltIndcrTurn) == length) {
            log_verbose() << "Received LiDrvrFltIndcrTurn (" << ComConf_ComSignal_isLiDrvrFltIndcrTurn_mrx << ")";
            const LiDrvrFltIndcrTurn& rteValue = *static_cast<const LiDrvrFltIndcrTurn*>(buffer);
            autosar::LiDrvrFltIndcrTurn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LiDrvrFltIndcrTurn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LiDrvrFltIndcrTurn (" << ComConf_ComSignal_isLiDrvrFltIndcrTurn_mrx << "). Got " << length << ", expected " << sizeof(LiDrvrFltIndcrTurn);
        }
    }
    break;

    case ComConf_ComSignal_isLiLvrSwt1_mrx:
    {
        if (sizeof(LiLvrSwt1) == length) {
            log_verbose() << "Received LiLvrSwt1 (" << ComConf_ComSignal_isLiLvrSwt1_mrx << ")";
            const LiLvrSwt1& rteValue = *static_cast<const LiLvrSwt1*>(buffer);
            autosar::LiLvrSwt1Req_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LiLvrSwt1Req_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LiLvrSwt1 (" << ComConf_ComSignal_isLiLvrSwt1_mrx << "). Got " << length << ", expected " << sizeof(LiLvrSwt1);
        }
    }
    break;

    case ComConf_ComSignal_isLiPassFltIndcrTurn_mrx:
    {
        if (sizeof(LiPassFltIndcrTurn) == length) {
            log_verbose() << "Received LiPassFltIndcrTurn (" << ComConf_ComSignal_isLiPassFltIndcrTurn_mrx << ")";
            const LiPassFltIndcrTurn& rteValue = *static_cast<const LiPassFltIndcrTurn*>(buffer);
            autosar::LiPassFltIndcrTurn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LiPassFltIndcrTurn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LiPassFltIndcrTurn (" << ComConf_ComSignal_isLiPassFltIndcrTurn_mrx << "). Got " << length << ", expected " << sizeof(LiPassFltIndcrTurn);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igLockgCenSts_mrx:
    {
        if (sizeof(LockgCenSts) == length) {
            log_verbose() << "Received LockgCenSts (" << ComConf_ComSignalGroup_igLockgCenSts_mrx << ")";
            const LockgCenSts& rteValue = *static_cast<const LockgCenSts*>(buffer);
            autosar::LockgCenSts_info::data_elem_type deValue;
            deValue.UpdEve = static_cast<decltype(deValue.UpdEve)>(rteValue.LockgCenStsUpdEve);
            deValue.LockSt = static_cast<decltype(deValue.LockSt)>(rteValue.LockgCenStsLockSt);
            deValue.TrigSrc = static_cast<decltype(deValue.TrigSrc)>(rteValue.LockgCenTrigSrc);

            LockgCenSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LockgCenSts (" << ComConf_ComSignalGroup_igLockgCenSts_mrx << "). Got " << length << ", expected " << sizeof(LockgCenSts);
        }
    }
    break;

    case ComConf_ComSignal_isLockgCenStsForUsrFb_mrx:
    {
        if (sizeof(LockgCenStsForUsrFb) == length) {
            log_verbose() << "Received LockgCenStsForUsrFb (" << ComConf_ComSignal_isLockgCenStsForUsrFb_mrx << ")";
            const LockgCenStsForUsrFb& rteValue = *static_cast<const LockgCenStsForUsrFb*>(buffer);
            autosar::LockgCenStsForUsrFb_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LockgCenStsForUsrFb_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LockgCenStsForUsrFb (" << ComConf_ComSignal_isLockgCenStsForUsrFb_mrx << "). Got " << length << ", expected " << sizeof(LockgCenStsForUsrFb);
        }
    }
    break;

    case ComConf_ComSignal_isLockgPrsnlSts_mrx:
    {
        if (sizeof(LockgPrsnlSts) == length) {
            log_verbose() << "Received LockgPrsnlSts (" << ComConf_ComSignal_isLockgPrsnlSts_mrx << ")";
            const LockgPrsnlSts& rteValue = *static_cast<const LockgPrsnlSts*>(buffer);
            autosar::LockgPrsnlSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LockgPrsnlSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LockgPrsnlSts (" << ComConf_ComSignal_isLockgPrsnlSts_mrx << "). Got " << length << ", expected " << sizeof(LockgPrsnlSts);
        }
    }
    break;

    case ComConf_ComSignal_isLvlOfClimaCmft_mrx:
    {
        if (sizeof(LvlOfClimaCmft) == length) {
            log_verbose() << "Received LvlOfClimaCmft (" << ComConf_ComSignal_isLvlOfClimaCmft_mrx << ")";
            const LvlOfClimaCmft& rteValue = *static_cast<const LvlOfClimaCmft*>(buffer);
            autosar::LvlOfClimaCmft_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LvlOfClimaCmft_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for LvlOfClimaCmft (" << ComConf_ComSignal_isLvlOfClimaCmft_mrx << "). Got " << length << ", expected " << sizeof(LvlOfClimaCmft);
        }
    }
    break;

    case ComConf_ComSignal_isMemBtnSound_mrx:
    {
        if (sizeof(MemBtnSound) == length) {
            log_verbose() << "Received MemBtnSound (" << ComConf_ComSignal_isMemBtnSound_mrx << ")";
            const MemBtnSound& rteValue = *static_cast<const MemBtnSound*>(buffer);
            autosar::MemBtnSound_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MemBtnSound_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for MemBtnSound (" << ComConf_ComSignal_isMemBtnSound_mrx << "). Got " << length << ", expected " << sizeof(MemBtnSound);
        }
    }
    break;

    case ComConf_ComSignal_isMirrDwnStsAtDrvr_mrx:
    {
        if (sizeof(MirrDwnStsAtDrvr) == length) {
            log_verbose() << "Received MirrDwnStsAtDrvr (" << ComConf_ComSignal_isMirrDwnStsAtDrvr_mrx << ")";
            const MirrDwnStsAtDrvr& rteValue = *static_cast<const MirrDwnStsAtDrvr*>(buffer);
            autosar::MirrDwnStsAtDrvr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MirrDwnStsAtDrvr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for MirrDwnStsAtDrvr (" << ComConf_ComSignal_isMirrDwnStsAtDrvr_mrx << "). Got " << length << ", expected " << sizeof(MirrDwnStsAtDrvr);
        }
    }
    break;

    case ComConf_ComSignal_isMirrDwnStsAtPass_mrx:
    {
        if (sizeof(MirrDwnStsAtPass) == length) {
            log_verbose() << "Received MirrDwnStsAtPass (" << ComConf_ComSignal_isMirrDwnStsAtPass_mrx << ")";
            const MirrDwnStsAtPass& rteValue = *static_cast<const MirrDwnStsAtPass*>(buffer);
            autosar::MirrDwnStsAtPass_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MirrDwnStsAtPass_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for MirrDwnStsAtPass (" << ComConf_ComSignal_isMirrDwnStsAtPass_mrx << "). Got " << length << ", expected " << sizeof(MirrDwnStsAtPass);
        }
    }
    break;

    case ComConf_ComSignal_isMirrFoldStsAtDrvr_mrx:
    {
        if (sizeof(MirrFoldStsAtDrvr) == length) {
            log_verbose() << "Received MirrFoldStsAtDrvr (" << ComConf_ComSignal_isMirrFoldStsAtDrvr_mrx << ")";
            const MirrFoldStsAtDrvr& rteValue = *static_cast<const MirrFoldStsAtDrvr*>(buffer);
            autosar::MirrFoldStsAtDrvr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MirrFoldStsAtDrvr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for MirrFoldStsAtDrvr (" << ComConf_ComSignal_isMirrFoldStsAtDrvr_mrx << "). Got " << length << ", expected " << sizeof(MirrFoldStsAtDrvr);
        }
    }
    break;

    case ComConf_ComSignal_isMirrFoldStsAtPass_mrx:
    {
        if (sizeof(MirrFoldStsAtPass) == length) {
            log_verbose() << "Received MirrFoldStsAtPass (" << ComConf_ComSignal_isMirrFoldStsAtPass_mrx << ")";
            const MirrFoldStsAtPass& rteValue = *static_cast<const MirrFoldStsAtPass*>(buffer);
            autosar::MirrFoldStsAtPass_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MirrFoldStsAtPass_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for MirrFoldStsAtPass (" << ComConf_ComSignal_isMirrFoldStsAtPass_mrx << "). Got " << length << ", expected " << sizeof(MirrFoldStsAtPass);
        }
    }
    break;

    case ComConf_ComSignal_isMstCfgIDBackboneFR_mrx:
    {
        if (sizeof(MstCfgIDBackboneFR) == length) {
            log_verbose() << "Received MstCfgIDBackboneFR (" << ComConf_ComSignal_isMstCfgIDBackboneFR_mrx << ")";
            const MstCfgIDBackboneFR& rteValue = *static_cast<const MstCfgIDBackboneFR*>(buffer);
            autosar::MstCfgIDBackboneFR_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MstCfgIDBackboneFR_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for MstCfgIDBackboneFR (" << ComConf_ComSignal_isMstCfgIDBackboneFR_mrx << "). Got " << length << ", expected " << sizeof(MstCfgIDBackboneFR);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igNetCtrlrActvt_mrx:
    {
        if (sizeof(NetCtrlrActvt) == length) {
            log_verbose() << "Received NetCtrlrActvt (" << ComConf_ComSignalGroup_igNetCtrlrActvt_mrx << ")";
            const NetCtrlrActvt& rteValue = *static_cast<const NetCtrlrActvt*>(buffer);
            autosar::NetCtrlrActvt_info::data_elem_type deValue;
            deValue.ResourceGroup = static_cast<decltype(deValue.ResourceGroup)>(rteValue.NetCtrlrActvtResourceGroup);
            deValue.Prio = static_cast<decltype(deValue.Prio)>(rteValue.NetCtrlrActvtPrio);

            NetCtrlrActvt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for NetCtrlrActvt (" << ComConf_ComSignalGroup_igNetCtrlrActvt_mrx << "). Got " << length << ", expected " << sizeof(NetCtrlrActvt);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igNetTelmActvt_mrx:
    {
        if (sizeof(NetTelmActvt) == length) {
            log_verbose() << "Received NetTelmActvt (" << ComConf_ComSignalGroup_igNetTelmActvt_mrx << ")";
            const NetTelmActvt& rteValue = *static_cast<const NetTelmActvt*>(buffer);
            autosar::NetTelmActvt_info::data_elem_type deValue;
            deValue.ResourceGroup = static_cast<decltype(deValue.ResourceGroup)>(rteValue.NetTelmActvtResourceGroup);
            deValue.Prio = static_cast<decltype(deValue.Prio)>(rteValue.NetTelmActvtPrio);

            NetTelmActvt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for NetTelmActvt (" << ComConf_ComSignalGroup_igNetTelmActvt_mrx << "). Got " << length << ", expected " << sizeof(NetTelmActvt);
        }
    }
    break;

    case ComConf_ComSignal_isNewTripCdn_mrx:
    {
        if (sizeof(NewTripCdn) == length) {
            log_verbose() << "Received NewTripCdn (" << ComConf_ComSignal_isNewTripCdn_mrx << ")";
            const NewTripCdn& rteValue = *static_cast<const NewTripCdn*>(buffer);
            autosar::NewTripCdn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NewTripCdn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for NewTripCdn (" << ComConf_ComSignal_isNewTripCdn_mrx << "). Got " << length << ", expected " << sizeof(NewTripCdn);
        }
    }
    break;

    case ComConf_ComSignal_isNoEntryWarnReq_mrx:
    {
        if (sizeof(NoEntryWarnReq) == length) {
            log_verbose() << "Received NoEntryWarnReq (" << ComConf_ComSignal_isNoEntryWarnReq_mrx << ")";
            const NoEntryWarnReq& rteValue = *static_cast<const NoEntryWarnReq*>(buffer);
            autosar::NoEntryWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NoEntryWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for NoEntryWarnReq (" << ComConf_ComSignal_isNoEntryWarnReq_mrx << "). Got " << length << ", expected " << sizeof(NoEntryWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isNotifChkDistbn_mrx:
    {
        if (sizeof(NotifChkDistbn) == length) {
            log_verbose() << "Received NotifChkDistbn (" << ComConf_ComSignal_isNotifChkDistbn_mrx << ")";
            const NotifChkDistbn& rteValue = *static_cast<const NotifChkDistbn*>(buffer);
            autosar::NotifChkDistbn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NotifChkDistbn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for NotifChkDistbn (" << ComConf_ComSignal_isNotifChkDistbn_mrx << "). Got " << length << ", expected " << sizeof(NotifChkDistbn);
        }
    }
    break;

    case ComConf_ComSignal_isNrOfBltAppld_mrx:
    {
        if (sizeof(NrOfBltAppld) == length) {
            log_verbose() << "Received NrOfBltAppld (" << ComConf_ComSignal_isNrOfBltAppld_mrx << ")";
            const NrOfBltAppld& rteValue = *static_cast<const NrOfBltAppld*>(buffer);
            autosar::NrOfBltAppld_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NrOfBltAppld_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for NrOfBltAppld (" << ComConf_ComSignal_isNrOfBltAppld_mrx << "). Got " << length << ", expected " << sizeof(NrOfBltAppld);
        }
    }
    break;

    case ComConf_ComSignal_isNrOfKeyAvl_mrx:
    {
        if (sizeof(NrOfKeyAvl) == length) {
            log_verbose() << "Received NrOfKeyAvl (" << ComConf_ComSignal_isNrOfKeyAvl_mrx << ")";
            const NrOfKeyAvl& rteValue = *static_cast<const NrOfKeyAvl*>(buffer);
            autosar::NrOfKeyAvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NrOfKeyAvl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for NrOfKeyAvl (" << ComConf_ComSignal_isNrOfKeyAvl_mrx << "). Got " << length << ", expected " << sizeof(NrOfKeyAvl);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igNrSerlDMSM_mrx:
    {
        if (sizeof(NrSerlDMSM) == length) {
            log_verbose() << "Received NrSerlDMSM (" << ComConf_ComSignalGroup_igNrSerlDMSM_mrx << ")";
            const NrSerlDMSM& rteValue = *static_cast<const NrSerlDMSM*>(buffer);
            autosar::NrSerlDMSM_info::data_elem_type deValue;
            deValue.Nr3 = static_cast<decltype(deValue.Nr3)>(rteValue.NrSerlDMSMNr3);
            deValue.Nr2 = static_cast<decltype(deValue.Nr2)>(rteValue.NrSerlDMSMNr2);
            deValue.Nr1 = static_cast<decltype(deValue.Nr1)>(rteValue.NrSerlDMSMNr1);
            deValue.Nr4 = static_cast<decltype(deValue.Nr4)>(rteValue.NrSerlDMSMNr4);

            NrSerlDMSM_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for NrSerlDMSM (" << ComConf_ComSignalGroup_igNrSerlDMSM_mrx << "). Got " << length << ", expected " << sizeof(NrSerlDMSM);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPartNrDMSM_mrx:
    {
        if (sizeof(PartNrDMSM) == length) {
            log_verbose() << "Received PartNrDMSM (" << ComConf_ComSignalGroup_igPartNrDMSM_mrx << ")";
            const PartNrDMSM& rteValue = *static_cast<const PartNrDMSM*>(buffer);
            autosar::PartNrDMSM_info::data_elem_type deValue;
            deValue.EndSgn1 = static_cast<decltype(deValue.EndSgn1)>(rteValue.PartNrDMSMEndSgn1);
            deValue.EndSgn2 = static_cast<decltype(deValue.EndSgn2)>(rteValue.PartNrDMSMEndSgn2);
            deValue.EndSgn3 = static_cast<decltype(deValue.EndSgn3)>(rteValue.PartNrDMSMEndSgn3);
            deValue.Nr3 = static_cast<decltype(deValue.Nr3)>(rteValue.PartNrDMSMNr3);
            deValue.Nr2 = static_cast<decltype(deValue.Nr2)>(rteValue.PartNrDMSMNr2);
            deValue.Nr1 = static_cast<decltype(deValue.Nr1)>(rteValue.PartNrDMSMNr1);
            deValue.Nr4 = static_cast<decltype(deValue.Nr4)>(rteValue.PartNrDMSMNr4);

            PartNrDMSM_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PartNrDMSM (" << ComConf_ComSignalGroup_igPartNrDMSM_mrx << "). Got " << length << ", expected " << sizeof(PartNrDMSM);
        }
    }
    break;

    case ComConf_ComSignal_isPasAlrmSts_mrx:
    {
        if (sizeof(PasAlrmSts) == length) {
            log_verbose() << "Received PasAlrmSts (" << ComConf_ComSignal_isPasAlrmSts_mrx << ")";
            const PasAlrmSts& rteValue = *static_cast<const PasAlrmSts*>(buffer);
            autosar::PasAlrmSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PasAlrmSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PasAlrmSts (" << ComConf_ComSignal_isPasAlrmSts_mrx << "). Got " << length << ", expected " << sizeof(PasAlrmSts);
        }
    }
    break;

    case ComConf_ComSignal_isPassMassgRunng_mrx:
    {
        if (sizeof(PassMassgRunng) == length) {
            log_verbose() << "Received PassMassgRunng (" << ComConf_ComSignal_isPassMassgRunng_mrx << ")";
            const PassMassgRunng& rteValue = *static_cast<const PassMassgRunng*>(buffer);
            autosar::PassMassgRunng_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PassMassgRunng_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PassMassgRunng (" << ComConf_ComSignal_isPassMassgRunng_mrx << "). Got " << length << ", expected " << sizeof(PassMassgRunng);
        }
    }
    break;

    case ComConf_ComSignal_isPassSeatActvSpplFct_mrx:
    {
        if (sizeof(PassSeatActvSpplFct) == length) {
            log_verbose() << "Received PassSeatActvSpplFct (" << ComConf_ComSignal_isPassSeatActvSpplFct_mrx << ")";
            const PassSeatActvSpplFct& rteValue = *static_cast<const PassSeatActvSpplFct*>(buffer);
            autosar::PassSeatActvSpplFct_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PassSeatActvSpplFct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PassSeatActvSpplFct (" << ComConf_ComSignal_isPassSeatActvSpplFct_mrx << "). Got " << length << ", expected " << sizeof(PassSeatActvSpplFct);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPassSeatMassgFct_mrx:
    {
        if (sizeof(PassSeatMassgFct) == length) {
            log_verbose() << "Received PassSeatMassgFct (" << ComConf_ComSignalGroup_igPassSeatMassgFct_mrx << ")";
            const PassSeatMassgFct& rteValue = *static_cast<const PassSeatMassgFct*>(buffer);
            autosar::PassSeatMassgFct_info::data_elem_type deValue;
            deValue.MassgSpdLvl = static_cast<decltype(deValue.MassgSpdLvl)>(rteValue.PassSeatMassgFctMassgSpdLvl);
            deValue.MassgInten = static_cast<decltype(deValue.MassgInten)>(rteValue.PassSeatMassgFctMassgInten);
            deValue.MassgProg = static_cast<decltype(deValue.MassgProg)>(rteValue.PassSeatMassgFctMassgProg);
            deValue.OnOff = static_cast<decltype(deValue.OnOff)>(rteValue.PassSeatMassgFctOnOff);

            PassSeatMassgFct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PassSeatMassgFct (" << ComConf_ComSignalGroup_igPassSeatMassgFct_mrx << "). Got " << length << ", expected " << sizeof(PassSeatMassgFct);
        }
    }
    break;

    case ComConf_ComSignal_isPassSeatSts_mrx:
    {
        if (sizeof(PassSeatSts) == length) {
            log_verbose() << "Received PassSeatSts (" << ComConf_ComSignal_isPassSeatSts_mrx << ")";
            const PassSeatSts& rteValue = *static_cast<const PassSeatSts*>(buffer);
            autosar::PassSeatSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PassSeatSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PassSeatSts (" << ComConf_ComSignal_isPassSeatSts_mrx << "). Got " << length << ", expected " << sizeof(PassSeatSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPassSeatSwtSts2_mrx:
    {
        if (sizeof(PassSeatSwtSts2) == length) {
            log_verbose() << "Received PassSeatSwtSts2 (" << ComConf_ComSignalGroup_igPassSeatSwtSts2_mrx << ")";
            const PassSeatSwtSts2& rteValue = *static_cast<const PassSeatSwtSts2*>(buffer);
            autosar::PassSeatSwtSts2_info::data_elem_type deValue;
            deValue.PassSeatSwtHeiSts = static_cast<decltype(deValue.PassSeatSwtHeiSts)>(rteValue.PassSeatSwtSts2PassSeatSwtHeiSts);
            deValue.PassSeatSwtSldSts = static_cast<decltype(deValue.PassSeatSwtSldSts)>(rteValue.PassSeatSwtSts2PassSeatSwtSldSts);
            deValue.PassSeatSwtAdjmtOfSpplFctVerSts = static_cast<decltype(deValue.PassSeatSwtAdjmtOfSpplFctVerSts)>(rteValue.PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts);
            deValue.PassSeatSwtAdjmtOfSpplFctHozlSts = static_cast<decltype(deValue.PassSeatSwtAdjmtOfSpplFctHozlSts)>(rteValue.PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts);
            deValue.PassSeatSwtSelnOfSpplFctStsSts = static_cast<decltype(deValue.PassSeatSwtSelnOfSpplFctStsSts)>(rteValue.PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts);
            deValue.PassSeatSwtHeiFrntSts = static_cast<decltype(deValue.PassSeatSwtHeiFrntSts)>(rteValue.PassSeatSwtSts2PassSeatSwtHeiFrntSts);
            deValue.PassSeatSwtInclSts = static_cast<decltype(deValue.PassSeatSwtInclSts)>(rteValue.PassSeatSwtSts2PassSeatSwtInclSts);

            PassSeatSwtSts2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PassSeatSwtSts2 (" << ComConf_ComSignalGroup_igPassSeatSwtSts2_mrx << "). Got " << length << ", expected " << sizeof(PassSeatSwtSts2);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPinionSteerAg1Group_mrx:
    {
        if (sizeof(PinionSteerAg1Group) == length) {
            log_verbose() << "Received PinionSteerAg1Group (" << ComConf_ComSignalGroup_igPinionSteerAg1Group_mrx << ")";
            const PinionSteerAg1Group& rteValue = *static_cast<const PinionSteerAg1Group*>(buffer);
            autosar::PinionSteerAg1_info::data_elem_type deValue;
            deValue.PinionSteerAg1 = static_cast<decltype(deValue.PinionSteerAg1)>( toSignedFromRaw<15>(rteValue.PinionSteerAg1) ) * 9.765625E-4 + 0.0;
            deValue.PinionSteerAg1Qf = static_cast<decltype(deValue.PinionSteerAg1Qf)>(rteValue.PinionSteerAg1Qf);

            PinionSteerAg1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PinionSteerAg1Group (" << ComConf_ComSignalGroup_igPinionSteerAg1Group_mrx << "). Got " << length << ", expected " << sizeof(PinionSteerAg1Group);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPosnFromSatlt_mrx:
    {
        if (sizeof(PosnFromSatlt) == length) {
            log_verbose() << "Received PosnFromSatlt (" << ComConf_ComSignalGroup_igPosnFromSatlt_mrx << ")";
            const PosnFromSatlt& rteValue = *static_cast<const PosnFromSatlt*>(buffer);
            autosar::PosnFromSatlt_info::data_elem_type deValue;
            deValue.NoOfSatltForSysNo6 = static_cast<decltype(deValue.NoOfSatltForSysNo6)>(rteValue.NoOfSatltForSysNo6InPosnFromSatlt);
            deValue.PosnVVert = static_cast<decltype(deValue.PosnVVert)>( toSignedFromRaw<18>(rteValue.PosnVVertInPosnFromSatlt) ) * 0.001 + 0.0;
            deValue.PreVertDil = static_cast<decltype(deValue.PreVertDil)>( toUnsignedFromRaw<8>(rteValue.PreVertDilInPosnFromSatlt) ) * 0.1 + 0.0;
            deValue.NoOfSatltForSysNo3 = static_cast<decltype(deValue.NoOfSatltForSysNo3)>(rteValue.NoOfSatltForSysNo3InPosnFromSatlt);
            deValue.NoOfSatltForSysNo2 = static_cast<decltype(deValue.NoOfSatltForSysNo2)>(rteValue.NoOfSatltForSysNo2InPosnFromSatlt);
            deValue.NoOfSatltForSysNo1 = static_cast<decltype(deValue.NoOfSatltForSysNo1)>(rteValue.NoOfSatltForSysNo1InPosnFromSatlt);
            deValue.NoOfSatltForSysNo5 = static_cast<decltype(deValue.NoOfSatltForSysNo5)>(rteValue.NoOfSatltForSysNo5InPosnFromSatlt);
            deValue.TiForMth = static_cast<decltype(deValue.TiForMth)>(rteValue.TiForMthInPosnFromSatlt);
            deValue.PosnSpd = static_cast<decltype(deValue.PosnSpd)>( toUnsignedFromRaw<17>(rteValue.PosnSpdInPosnFromSatlt) ) * 0.001 + 0.0;
            deValue.TiForDay = static_cast<decltype(deValue.TiForDay)>(rteValue.TiForDayInPosnFromSatlt);
            deValue.PosnVHozl = static_cast<decltype(deValue.PosnVHozl)>( toUnsignedFromRaw<17>(rteValue.PosnVHozlInPosnFromSatlt) ) * 0.001 + 0.0;
            deValue.TiForMins = static_cast<decltype(deValue.TiForMins)>(rteValue.TiForMinsInPosnFromSatlt);
            deValue.PosnDir = static_cast<decltype(deValue.PosnDir)>( toUnsignedFromRaw<16>(rteValue.PosnDirInPosnFromSatlt) ) * 0.01 + 0.0;
            deValue.PosnLgt = static_cast<decltype(deValue.PosnLgt)>( toSignedFromRaw<31>(rteValue.PosnLgtInPosnFromSatlt) ) * 2.7777777777777776E-7 + 0.0;
            deValue.SatltSysNo6InUse = static_cast<decltype(deValue.SatltSysNo6InUse)>(rteValue.SatltSysNo6InUseInPosnFromSatlt);
            deValue.PosnLat = static_cast<decltype(deValue.PosnLat)>( toSignedFromRaw<30>(rteValue.PosnLatInPosnFromSatlt) ) * 2.7777777777777776E-7 + 0.0;
            deValue.PreHozlDil = static_cast<decltype(deValue.PreHozlDil)>( toUnsignedFromRaw<8>(rteValue.PreHozlDilInPosnFromSatlt) ) * 0.1 + 0.0;
            deValue.SatltSysNo5InUse = static_cast<decltype(deValue.SatltSysNo5InUse)>(rteValue.SatltSysNo5InUseInPosnFromSatlt);
            deValue.SatltSysNo4InUse = static_cast<decltype(deValue.SatltSysNo4InUse)>(rteValue.SatltSysNo4InUseInPosnFromSatlt);
            deValue.PreTiDil = static_cast<decltype(deValue.PreTiDil)>( toUnsignedFromRaw<8>(rteValue.PreTiDilInPosnFromSatlt) ) * 0.1 + 0.0;
            deValue.PosnAlti = static_cast<decltype(deValue.PosnAlti)>( toUnsignedFromRaw<16>(rteValue.PosnAltiInPosnFromSatlt) ) * 0.1 + -100.0;
            deValue.NoOfSatltForSysNo4 = static_cast<decltype(deValue.NoOfSatltForSysNo4)>(rteValue.NoOfSatltForSysNo4InPosnFromSatlt);
            deValue.SatltPosnStsPrm1 = static_cast<decltype(deValue.SatltPosnStsPrm1)>(rteValue.SatltPosnStsPrm1InPosnFromSatlt);
            deValue.SatltPosnStsPrm3 = static_cast<decltype(deValue.SatltPosnStsPrm3)>(rteValue.SatltPosnStsPrm3InPosnFromSatlt);
            deValue.SatltPosnStsPrm2 = static_cast<decltype(deValue.SatltPosnStsPrm2)>(rteValue.SatltPosnStsPrm2InPosnFromSatlt);
            deValue.TiForYr = static_cast<decltype(deValue.TiForYr)>(rteValue.TiForYrInPosnFromSatlt);
            deValue.TiForHr = static_cast<decltype(deValue.TiForHr)>(rteValue.TiForHrInPosnFromSatlt);
            deValue.SatltSysNo1InUse = static_cast<decltype(deValue.SatltSysNo1InUse)>(rteValue.SatltSysNo1InUseInPosnFromSatlt);
            deValue.SatltSysNo3InUse = static_cast<decltype(deValue.SatltSysNo3InUse)>(rteValue.SatltSysNo3InUseInPosnFromSatlt);
            deValue.TiForSec = static_cast<decltype(deValue.TiForSec)>(rteValue.TiForSecInPosnFromSatlt);
            deValue.SatltSysNo2InUse = static_cast<decltype(deValue.SatltSysNo2InUse)>(rteValue.SatltSysNo2InUseInPosnFromSatlt);
            deValue.PrePosnDil = static_cast<decltype(deValue.PrePosnDil)>( toUnsignedFromRaw<8>(rteValue.PrePosnDilInPosnFromSatlt) ) * 0.1 + 0.0;

            PosnFromSatlt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PosnFromSatlt (" << ComConf_ComSignalGroup_igPosnFromSatlt_mrx << "). Got " << length << ", expected " << sizeof(PosnFromSatlt);
        }
    }
    break;

    case ComConf_ComSignal_isPostDrvgClimaAvl_mrx:
    {
        if (sizeof(PostDrvgClimaAvl) == length) {
            log_verbose() << "Received PostDrvgClimaAvl (" << ComConf_ComSignal_isPostDrvgClimaAvl_mrx << ")";
            const PostDrvgClimaAvl& rteValue = *static_cast<const PostDrvgClimaAvl*>(buffer);
            autosar::PostDrvgClimaAvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PostDrvgClimaAvl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PostDrvgClimaAvl (" << ComConf_ComSignal_isPostDrvgClimaAvl_mrx << "). Got " << length << ", expected " << sizeof(PostDrvgClimaAvl);
        }
    }
    break;

    case ComConf_ComSignal_isPreClngNotif_mrx:
    {
        if (sizeof(PreClngNotif) == length) {
            log_verbose() << "Received PreClngNotif (" << ComConf_ComSignal_isPreClngNotif_mrx << ")";
            const PreClngNotif& rteValue = *static_cast<const PreClngNotif*>(buffer);
            autosar::PreClngNotif_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PreClngNotif_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PreClngNotif (" << ComConf_ComSignal_isPreClngNotif_mrx << "). Got " << length << ", expected " << sizeof(PreClngNotif);
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAssiFailr_mrx:
    {
        if (sizeof(PrkgAssiFailr) == length) {
            log_verbose() << "Received PrkgAssiFailr (" << ComConf_ComSignal_isPrkgAssiFailr_mrx << ")";
            const PrkgAssiFailr& rteValue = *static_cast<const PrkgAssiFailr*>(buffer);
            autosar::PrkgAssiFailr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAssiFailr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrkgAssiFailr (" << ComConf_ComSignal_isPrkgAssiFailr_mrx << "). Got " << length << ", expected " << sizeof(PrkgAssiFailr);
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAssiManvActvSts_mrx:
    {
        if (sizeof(PrkgAssiManvActvSts) == length) {
            log_verbose() << "Received PrkgAssiManvActvSts (" << ComConf_ComSignal_isPrkgAssiManvActvSts_mrx << ")";
            const PrkgAssiManvActvSts& rteValue = *static_cast<const PrkgAssiManvActvSts*>(buffer);
            autosar::PrkgAssiManvActvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAssiManvActvSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrkgAssiManvActvSts (" << ComConf_ComSignal_isPrkgAssiManvActvSts_mrx << "). Got " << length << ", expected " << sizeof(PrkgAssiManvActvSts);
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAssiManvProgs_mrx:
    {
        if (sizeof(PrkgAssiManvProgs) == length) {
            log_verbose() << "Received PrkgAssiManvProgs (" << ComConf_ComSignal_isPrkgAssiManvProgs_mrx << ")";
            const PrkgAssiManvProgs& rteValue = *static_cast<const PrkgAssiManvProgs*>(buffer);
            autosar::PrkgAssiManvProgs_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAssiManvProgs_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrkgAssiManvProgs (" << ComConf_ComSignal_isPrkgAssiManvProgs_mrx << "). Got " << length << ", expected " << sizeof(PrkgAssiManvProgs);
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAssiSts_mrx:
    {
        if (sizeof(PrkgAssiSts) == length) {
            log_verbose() << "Received PrkgAssiSts (" << ComConf_ComSignal_isPrkgAssiSts_mrx << ")";
            const PrkgAssiSts& rteValue = *static_cast<const PrkgAssiSts*>(buffer);
            autosar::PrkgAssiSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAssiSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrkgAssiSts (" << ComConf_ComSignal_isPrkgAssiSts_mrx << "). Got " << length << ", expected " << sizeof(PrkgAssiSts);
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAutSts_mrx:
    {
        if (sizeof(PrkgAutSts) == length) {
            log_verbose() << "Received PrkgAutSts (" << ComConf_ComSignal_isPrkgAutSts_mrx << ")";
            const PrkgAutSts& rteValue = *static_cast<const PrkgAutSts*>(buffer);
            autosar::PrkgAutSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAutSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrkgAutSts (" << ComConf_ComSignal_isPrkgAutSts_mrx << "). Got " << length << ", expected " << sizeof(PrkgAutSts);
        }
    }
    break;

    case ComConf_ComSignal_isPrkgCamSysAvlSts_mrx:
    {
        if (sizeof(PrkgCamSysAvlSts) == length) {
            log_verbose() << "Received PrkgCamSysAvlSts (" << ComConf_ComSignal_isPrkgCamSysAvlSts_mrx << ")";
            const PrkgCamSysAvlSts& rteValue = *static_cast<const PrkgCamSysAvlSts*>(buffer);
            autosar::PrkgCamSysAvlSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgCamSysAvlSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrkgCamSysAvlSts (" << ComConf_ComSignal_isPrkgCamSysAvlSts_mrx << "). Got " << length << ", expected " << sizeof(PrkgCamSysAvlSts);
        }
    }
    break;

    case ComConf_ComSignal_isPrkgTypVld_mrx:
    {
        if (sizeof(PrkgTypVld) == length) {
            log_verbose() << "Received PrkgTypVld (" << ComConf_ComSignal_isPrkgTypVld_mrx << ")";
            const PrkgTypVld& rteValue = *static_cast<const PrkgTypVld*>(buffer);
            autosar::PrkgTypVld_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgTypVld_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrkgTypVld (" << ComConf_ComSignal_isPrkgTypVld_mrx << "). Got " << length << ", expected " << sizeof(PrkgTypVld);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igProfAct_mrx:
    {
        if (sizeof(ProfAct) == length) {
            log_verbose() << "Received ProfAct (" << ComConf_ComSignalGroup_igProfAct_mrx << ")";
            const ProfAct& rteValue = *static_cast<const ProfAct*>(buffer);
            autosar::ProfAct_info::data_elem_type deValue;
            deValue.Prof2 = static_cast<decltype(deValue.Prof2)>(rteValue.ProfActProf2);
            deValue.Prof3 = static_cast<decltype(deValue.Prof3)>(rteValue.ProfActProf3);
            deValue.Prof1 = static_cast<decltype(deValue.Prof1)>(rteValue.ProfActProf1);
            deValue.Prof10 = static_cast<decltype(deValue.Prof10)>(rteValue.ProfActProf10);
            deValue.Prof4 = static_cast<decltype(deValue.Prof4)>(rteValue.ProfActProf4);
            deValue.Prof12 = static_cast<decltype(deValue.Prof12)>(rteValue.ProfActProf12);
            deValue.Prof5 = static_cast<decltype(deValue.Prof5)>(rteValue.ProfActProf5);
            deValue.Prof8 = static_cast<decltype(deValue.Prof8)>(rteValue.ProfActProf8);
            deValue.Prof9 = static_cast<decltype(deValue.Prof9)>(rteValue.ProfActProf9);
            deValue.Prof6 = static_cast<decltype(deValue.Prof6)>(rteValue.ProfActProf6);
            deValue.Prof11 = static_cast<decltype(deValue.Prof11)>(rteValue.ProfActProf11);
            deValue.Prof7 = static_cast<decltype(deValue.Prof7)>(rteValue.ProfActProf7);

            ProfAct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ProfAct (" << ComConf_ComSignalGroup_igProfAct_mrx << "). Got " << length << ", expected " << sizeof(ProfAct);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igProfLimd_mrx:
    {
        if (sizeof(ProfLimd) == length) {
            log_verbose() << "Received ProfLimd (" << ComConf_ComSignalGroup_igProfLimd_mrx << ")";
            const ProfLimd& rteValue = *static_cast<const ProfLimd*>(buffer);
            autosar::ProfLimd_info::data_elem_type deValue;
            deValue.Prof2 = static_cast<decltype(deValue.Prof2)>(rteValue.ProfLimdProf2);
            deValue.Prof3 = static_cast<decltype(deValue.Prof3)>(rteValue.ProfLimdProf3);
            deValue.Prof1 = static_cast<decltype(deValue.Prof1)>(rteValue.ProfLimdProf1);
            deValue.Prof10 = static_cast<decltype(deValue.Prof10)>(rteValue.ProfLimdProf10);
            deValue.Prof7 = static_cast<decltype(deValue.Prof7)>(rteValue.ProfLimdProf7);
            deValue.Prof12 = static_cast<decltype(deValue.Prof12)>(rteValue.ProfLimdProf12);
            deValue.Prof5 = static_cast<decltype(deValue.Prof5)>(rteValue.ProfLimdProf5);
            deValue.Prof8 = static_cast<decltype(deValue.Prof8)>(rteValue.ProfLimdProf8);
            deValue.Prof9 = static_cast<decltype(deValue.Prof9)>(rteValue.ProfLimdProf9);
            deValue.Prof4 = static_cast<decltype(deValue.Prof4)>(rteValue.ProfLimdProf4);
            deValue.Prof6 = static_cast<decltype(deValue.Prof6)>(rteValue.ProfLimdProf6);
            deValue.Prof11 = static_cast<decltype(deValue.Prof11)>(rteValue.ProfLimdProf11);

            ProfLimd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ProfLimd (" << ComConf_ComSignalGroup_igProfLimd_mrx << "). Got " << length << ", expected " << sizeof(ProfLimd);
        }
    }
    break;

    case ComConf_ComSignal_isProfPenSts1_mrx:
    {
        if (sizeof(ProfPenSts1) == length) {
            log_verbose() << "Received ProfPenSts1 (" << ComConf_ComSignal_isProfPenSts1_mrx << ")";
            const ProfPenSts1& rteValue = *static_cast<const ProfPenSts1*>(buffer);
            autosar::ProfPenSts1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ProfPenSts1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ProfPenSts1 (" << ComConf_ComSignal_isProfPenSts1_mrx << "). Got " << length << ", expected " << sizeof(ProfPenSts1);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgModAct_mrx:
    {
        if (sizeof(PrpsnHvBattUsgModAct) == length) {
            log_verbose() << "Received PrpsnHvBattUsgModAct (" << ComConf_ComSignal_isPrpsnHvBattUsgModAct_mrx << ")";
            const PrpsnHvBattUsgModAct& rteValue = *static_cast<const PrpsnHvBattUsgModAct*>(buffer);
            autosar::PrpsnHvBattUsgModAct_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgModAct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnHvBattUsgModAct (" << ComConf_ComSignal_isPrpsnHvBattUsgModAct_mrx << "). Got " << length << ", expected " << sizeof(PrpsnHvBattUsgModAct);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfChrgBlkd) == length) {
            log_verbose() << "Received PrpsnHvBattUsgOfChrgBlkd (" << ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd_mrx << ")";
            const PrpsnHvBattUsgOfChrgBlkd& rteValue = *static_cast<const PrpsnHvBattUsgOfChrgBlkd*>(buffer);
            autosar::PrpsnHvBattUsgOfChrgBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfChrgBlkd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnHvBattUsgOfChrgBlkd (" << ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd_mrx << "). Got " << length << ", expected " << sizeof(PrpsnHvBattUsgOfChrgBlkd);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd2_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfChrgBlkd2) == length) {
            log_verbose() << "Received PrpsnHvBattUsgOfChrgBlkd2 (" << ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd2_mrx << ")";
            const PrpsnHvBattUsgOfChrgBlkd2& rteValue = *static_cast<const PrpsnHvBattUsgOfChrgBlkd2*>(buffer);
            autosar::PrpsnHvBattUsgOfChrgBlkd2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfChrgBlkd2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnHvBattUsgOfChrgBlkd2 (" << ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd2_mrx << "). Got " << length << ", expected " << sizeof(PrpsnHvBattUsgOfChrgBlkd2);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfHldBlkd) == length) {
            log_verbose() << "Received PrpsnHvBattUsgOfHldBlkd (" << ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd_mrx << ")";
            const PrpsnHvBattUsgOfHldBlkd& rteValue = *static_cast<const PrpsnHvBattUsgOfHldBlkd*>(buffer);
            autosar::PrpsnHvBattUsgOfHldBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfHldBlkd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnHvBattUsgOfHldBlkd (" << ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd_mrx << "). Got " << length << ", expected " << sizeof(PrpsnHvBattUsgOfHldBlkd);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd2_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfHldBlkd2) == length) {
            log_verbose() << "Received PrpsnHvBattUsgOfHldBlkd2 (" << ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd2_mrx << ")";
            const PrpsnHvBattUsgOfHldBlkd2& rteValue = *static_cast<const PrpsnHvBattUsgOfHldBlkd2*>(buffer);
            autosar::PrpsnHvBattUsgOfHldBlkd2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfHldBlkd2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnHvBattUsgOfHldBlkd2 (" << ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd2_mrx << "). Got " << length << ", expected " << sizeof(PrpsnHvBattUsgOfHldBlkd2);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfHldSmtBlkd) == length) {
            log_verbose() << "Received PrpsnHvBattUsgOfHldSmtBlkd (" << ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd_mrx << ")";
            const PrpsnHvBattUsgOfHldSmtBlkd& rteValue = *static_cast<const PrpsnHvBattUsgOfHldSmtBlkd*>(buffer);
            autosar::PrpsnHvBattUsgOfHldSmtBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfHldSmtBlkd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnHvBattUsgOfHldSmtBlkd (" << ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd_mrx << "). Got " << length << ", expected " << sizeof(PrpsnHvBattUsgOfHldSmtBlkd);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd2_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfHldSmtBlkd2) == length) {
            log_verbose() << "Received PrpsnHvBattUsgOfHldSmtBlkd2 (" << ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd2_mrx << ")";
            const PrpsnHvBattUsgOfHldSmtBlkd2& rteValue = *static_cast<const PrpsnHvBattUsgOfHldSmtBlkd2*>(buffer);
            autosar::PrpsnHvBattUsgOfHldSmtBlkd2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfHldSmtBlkd2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnHvBattUsgOfHldSmtBlkd2 (" << ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd2_mrx << "). Got " << length << ", expected " << sizeof(PrpsnHvBattUsgOfHldSmtBlkd2);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModElecDrvBlkd_mrx:
    {
        if (sizeof(PrpsnModElecDrvBlkd) == length) {
            log_verbose() << "Received PrpsnModElecDrvBlkd (" << ComConf_ComSignal_isPrpsnModElecDrvBlkd_mrx << ")";
            const PrpsnModElecDrvBlkd& rteValue = *static_cast<const PrpsnModElecDrvBlkd*>(buffer);
            autosar::PrpsnModElecDrvBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModElecDrvBlkd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnModElecDrvBlkd (" << ComConf_ComSignal_isPrpsnModElecDrvBlkd_mrx << "). Got " << length << ", expected " << sizeof(PrpsnModElecDrvBlkd);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModOfSaveBlkd_mrx:
    {
        if (sizeof(PrpsnModOfSaveBlkd) == length) {
            log_verbose() << "Received PrpsnModOfSaveBlkd (" << ComConf_ComSignal_isPrpsnModOfSaveBlkd_mrx << ")";
            const PrpsnModOfSaveBlkd& rteValue = *static_cast<const PrpsnModOfSaveBlkd*>(buffer);
            autosar::PrpsnModOfSaveBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModOfSaveBlkd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnModOfSaveBlkd (" << ComConf_ComSignal_isPrpsnModOfSaveBlkd_mrx << "). Got " << length << ", expected " << sizeof(PrpsnModOfSaveBlkd);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModOfTracBlkd_mrx:
    {
        if (sizeof(PrpsnModOfTracBlkd) == length) {
            log_verbose() << "Received PrpsnModOfTracBlkd (" << ComConf_ComSignal_isPrpsnModOfTracBlkd_mrx << ")";
            const PrpsnModOfTracBlkd& rteValue = *static_cast<const PrpsnModOfTracBlkd*>(buffer);
            autosar::PrpsnModOfTracBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModOfTracBlkd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnModOfTracBlkd (" << ComConf_ComSignal_isPrpsnModOfTracBlkd_mrx << "). Got " << length << ", expected " << sizeof(PrpsnModOfTracBlkd);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModOffroadBlkd_mrx:
    {
        if (sizeof(PrpsnModOffroadBlkd) == length) {
            log_verbose() << "Received PrpsnModOffroadBlkd (" << ComConf_ComSignal_isPrpsnModOffroadBlkd_mrx << ")";
            const PrpsnModOffroadBlkd& rteValue = *static_cast<const PrpsnModOffroadBlkd*>(buffer);
            autosar::PrpsnModOffroadBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModOffroadBlkd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnModOffroadBlkd (" << ComConf_ComSignal_isPrpsnModOffroadBlkd_mrx << "). Got " << length << ", expected " << sizeof(PrpsnModOffroadBlkd);
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModSptBlkd_mrx:
    {
        if (sizeof(PrpsnModSptBlkd) == length) {
            log_verbose() << "Received PrpsnModSptBlkd (" << ComConf_ComSignal_isPrpsnModSptBlkd_mrx << ")";
            const PrpsnModSptBlkd& rteValue = *static_cast<const PrpsnModSptBlkd*>(buffer);
            autosar::PrpsnModSptBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModSptBlkd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PrpsnModSptBlkd (" << ComConf_ComSignal_isPrpsnModSptBlkd_mrx << "). Got " << length << ", expected " << sizeof(PrpsnModSptBlkd);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPtCluTq_mrx:
    {
        if (sizeof(PtCluTq) == length) {
            log_verbose() << "Received PtCluTq (" << ComConf_ComSignalGroup_igPtCluTq_mrx << ")";
            const PtCluTq& rteValue = *static_cast<const PtCluTq*>(buffer);
            autosar::PtCluTq_info::data_elem_type deValue;
            deValue.PtCluTq = static_cast<decltype(deValue.PtCluTq)>( toUnsignedFromRaw<11>(rteValue.PtCluTqPtCluTq) );
            deValue.PtCluTqDyn = static_cast<decltype(deValue.PtCluTqDyn)>( toUnsignedFromRaw<11>(rteValue.PtCluTqPtCluTqDyn) );
            deValue.PtCluTqQly = static_cast<decltype(deValue.PtCluTqQly)>(rteValue.PtCluTqPtCluTqQly);

            PtCluTq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PtCluTq (" << ComConf_ComSignalGroup_igPtCluTq_mrx << "). Got " << length << ", expected " << sizeof(PtCluTq);
        }
    }
    break;

    case ComConf_ComSignal_isPtGearTar_mrx:
    {
        if (sizeof(PtGearTar) == length) {
            log_verbose() << "Received PtGearTar (" << ComConf_ComSignal_isPtGearTar_mrx << ")";
            const PtGearTar& rteValue = *static_cast<const PtGearTar*>(buffer);
            autosar::PtGearTar_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PtGearTar_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PtGearTar (" << ComConf_ComSignal_isPtGearTar_mrx << "). Got " << length << ", expected " << sizeof(PtGearTar);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPtTqAtWhlFrntActGroup_mrx:
    {
        if (sizeof(PtTqAtWhlFrntActGroup) == length) {
            log_verbose() << "Received PtTqAtWhlFrntActGroup (" << ComConf_ComSignalGroup_igPtTqAtWhlFrntActGroup_mrx << ")";
            const PtTqAtWhlFrntActGroup& rteValue = *static_cast<const PtTqAtWhlFrntActGroup*>(buffer);
            autosar::PtTqAtWhlFrntAct_info::data_elem_type deValue;
            deValue.PtTqAtWhlsFrntQly = static_cast<decltype(deValue.PtTqAtWhlsFrntQly)>(rteValue.PtTqAtWhlsFrntQly);
            deValue.PtTqAtWhlFrntLeAct = static_cast<decltype(deValue.PtTqAtWhlFrntLeAct)>( toSignedFromRaw<16>(rteValue.PtTqAtWhlFrntLeAct) );
            deValue.PtTqAtWhlFrntRiAct = static_cast<decltype(deValue.PtTqAtWhlFrntRiAct)>( toSignedFromRaw<16>(rteValue.PtTqAtWhlFrntRiAct) );
            deValue.PtTqAtAxleFrntAct = static_cast<decltype(deValue.PtTqAtAxleFrntAct)>( toSignedFromRaw<16>(rteValue.PtTqAtAxleFrntAct) );

            PtTqAtWhlFrntAct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PtTqAtWhlFrntActGroup (" << ComConf_ComSignalGroup_igPtTqAtWhlFrntActGroup_mrx << "). Got " << length << ", expected " << sizeof(PtTqAtWhlFrntActGroup);
        }
    }
    break;

    case ComConf_ComSignal_isPwrChrgDetdForPrkgHeatrElec_mrx:
    {
        if (sizeof(PwrChrgDetdForPrkgHeatrElec) == length) {
            log_verbose() << "Received PwrChrgDetdForPrkgHeatrElec (" << ComConf_ComSignal_isPwrChrgDetdForPrkgHeatrElec_mrx << ")";
            const PwrChrgDetdForPrkgHeatrElec& rteValue = *static_cast<const PwrChrgDetdForPrkgHeatrElec*>(buffer);
            autosar::PwrChrgDetdForPrkgHeatrElec_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PwrChrgDetdForPrkgHeatrElec_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PwrChrgDetdForPrkgHeatrElec (" << ComConf_ComSignal_isPwrChrgDetdForPrkgHeatrElec_mrx << "). Got " << length << ", expected " << sizeof(PwrChrgDetdForPrkgHeatrElec);
        }
    }
    break;

    case ComConf_ComSignal_isPwrSplyErrSts_mrx:
    {
        if (sizeof(PwrSplyErrSts) == length) {
            log_verbose() << "Received PwrSplyErrSts (" << ComConf_ComSignal_isPwrSplyErrSts_mrx << ")";
            const PwrSplyErrSts& rteValue = *static_cast<const PwrSplyErrSts*>(buffer);
            autosar::PwrSplyErrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PwrSplyErrSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for PwrSplyErrSts (" << ComConf_ComSignal_isPwrSplyErrSts_mrx << "). Got " << length << ", expected " << sizeof(PwrSplyErrSts);
        }
    }
    break;

    case ComConf_ComSignal_isReAxleWarn_mrx:
    {
        if (sizeof(ReAxleWarn) == length) {
            log_verbose() << "Received ReAxleWarn (" << ComConf_ComSignal_isReAxleWarn_mrx << ")";
            const ReAxleWarn& rteValue = *static_cast<const ReAxleWarn*>(buffer);
            autosar::ReAxleWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ReAxleWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ReAxleWarn (" << ComConf_ComSignal_isReAxleWarn_mrx << "). Got " << length << ", expected " << sizeof(ReAxleWarn);
        }
    }
    break;

    case ComConf_ComSignal_isRmnLockgPrsnlReq_mrx:
    {
        if (sizeof(RmnLockgPrsnlReq) == length) {
            log_verbose() << "Received RmnLockgPrsnlReq (" << ComConf_ComSignal_isRmnLockgPrsnlReq_mrx << ")";
            const RmnLockgPrsnlReq& rteValue = *static_cast<const RmnLockgPrsnlReq*>(buffer);
            autosar::RmnLockgPrsnlReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RmnLockgPrsnlReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RmnLockgPrsnlReq (" << ComConf_ComSignal_isRmnLockgPrsnlReq_mrx << "). Got " << length << ", expected " << sizeof(RmnLockgPrsnlReq);
        }
    }
    break;

    case ComConf_ComSignal_isRoadFricIndcnSts_mrx:
    {
        if (sizeof(RoadFricIndcnSts) == length) {
            log_verbose() << "Received RoadFricIndcnSts (" << ComConf_ComSignal_isRoadFricIndcnSts_mrx << ")";
            const RoadFricIndcnSts& rteValue = *static_cast<const RoadFricIndcnSts*>(buffer);
            autosar::RoadFricIndcnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RoadFricIndcnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RoadFricIndcnSts (" << ComConf_ComSignal_isRoadFricIndcnSts_mrx << "). Got " << length << ", expected " << sizeof(RoadFricIndcnSts);
        }
    }
    break;

    case ComConf_ComSignal_isRoadFricWarnReq_mrx:
    {
        if (sizeof(RoadFricWarnReq) == length) {
            log_verbose() << "Received RoadFricWarnReq (" << ComConf_ComSignal_isRoadFricWarnReq_mrx << ")";
            const RoadFricWarnReq& rteValue = *static_cast<const RoadFricWarnReq*>(buffer);
            autosar::RoadFricWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RoadFricWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RoadFricWarnReq (" << ComConf_ComSignal_isRoadFricWarnReq_mrx << "). Got " << length << ", expected " << sizeof(RoadFricWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isRoadSgnInfoSts_mrx:
    {
        if (sizeof(RoadSgnInfoSts) == length) {
            log_verbose() << "Received RoadSgnInfoSts (" << ComConf_ComSignal_isRoadSgnInfoSts_mrx << ")";
            const RoadSgnInfoSts& rteValue = *static_cast<const RoadSgnInfoSts*>(buffer);
            autosar::RoadSgnInfoSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RoadSgnInfoSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RoadSgnInfoSts (" << ComConf_ComSignal_isRoadSgnInfoSts_mrx << "). Got " << length << ", expected " << sizeof(RoadSgnInfoSts);
        }
    }
    break;

    case ComConf_ComSignal_isRoadUsrProtnSts_mrx:
    {
        if (sizeof(RoadUsrProtnSts) == length) {
            log_verbose() << "Received RoadUsrProtnSts (" << ComConf_ComSignal_isRoadUsrProtnSts_mrx << ")";
            const RoadUsrProtnSts& rteValue = *static_cast<const RoadUsrProtnSts*>(buffer);
            autosar::RoadUsrProtnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RoadUsrProtnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RoadUsrProtnSts (" << ComConf_ComSignal_isRoadUsrProtnSts_mrx << "). Got " << length << ", expected " << sizeof(RoadUsrProtnSts);
        }
    }
    break;

    case ComConf_ComSignal_isRotyDirReq2_mrx:
    {
        if (sizeof(RotyDirReq2) == length) {
            log_verbose() << "Received RotyDirReq2 (" << ComConf_ComSignal_isRotyDirReq2_mrx << ")";
            const RotyDirReq2& rteValue = *static_cast<const RotyDirReq2*>(buffer);
            autosar::RotyDirReq2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RotyDirReq2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RotyDirReq2 (" << ComConf_ComSignal_isRotyDirReq2_mrx << "). Got " << length << ", expected " << sizeof(RotyDirReq2);
        }
    }
    break;

    case ComConf_ComSignal_isRotyDirReq1_mrx:
    {
        if (sizeof(RotyDirReq1) == length) {
            log_verbose() << "Received RotyDirReq1 (" << ComConf_ComSignal_isRotyDirReq1_mrx << ")";
            const RotyDirReq1& rteValue = *static_cast<const RotyDirReq1*>(buffer);
            autosar::RotyDirReq1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RotyDirReq1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RotyDirReq1 (" << ComConf_ComSignal_isRotyDirReq1_mrx << "). Got " << length << ", expected " << sizeof(RotyDirReq1);
        }
    }
    break;

    case ComConf_ComSignal_isRotyPosReq2_mrx:
    {
        if (sizeof(RotyPosReq2) == length) {
            log_verbose() << "Received RotyPosReq2 (" << ComConf_ComSignal_isRotyPosReq2_mrx << ")";
            const RotyPosReq2& rteValue = *static_cast<const RotyPosReq2*>(buffer);
            autosar::RotyPosReq2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RotyPosReq2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RotyPosReq2 (" << ComConf_ComSignal_isRotyPosReq2_mrx << "). Got " << length << ", expected " << sizeof(RotyPosReq2);
        }
    }
    break;

    case ComConf_ComSignal_isRotyPosReq1_mrx:
    {
        if (sizeof(RotyPosReq1) == length) {
            log_verbose() << "Received RotyPosReq1 (" << ComConf_ComSignal_isRotyPosReq1_mrx << ")";
            const RotyPosReq1& rteValue = *static_cast<const RotyPosReq1*>(buffer);
            autosar::RotyPosReq1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RotyPosReq1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RotyPosReq1 (" << ComConf_ComSignal_isRotyPosReq1_mrx << "). Got " << length << ", expected " << sizeof(RotyPosReq1);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igRsdsSysStsLe_mrx:
    {
        if (sizeof(RsdsSysStsLe) == length) {
            log_verbose() << "Received RsdsSysStsLe (" << ComConf_ComSignalGroup_igRsdsSysStsLe_mrx << ")";
            const RsdsSysStsLe& rteValue = *static_cast<const RsdsSysStsLe*>(buffer);
            autosar::RsdsSysStsLe_info::data_elem_type deValue;
            deValue.RcmSts = static_cast<decltype(deValue.RcmSts)>(rteValue.RsdsSysStsLeRcmSts);
            deValue.RsdsSts = static_cast<decltype(deValue.RsdsSts)>(rteValue.RsdsSysStsLeRsdsSts);
            deValue.CtraSts = static_cast<decltype(deValue.CtraSts)>(rteValue.RsdsSysStsLeCtraSts);
            deValue.RcwSts = static_cast<decltype(deValue.RcwSts)>(rteValue.RsdsSysStsLeRcwSts);
            deValue.LcmaSts = static_cast<decltype(deValue.LcmaSts)>(rteValue.RsdsSysStsLeLcmaSts);
            deValue.CtraSts1 = static_cast<decltype(deValue.CtraSts1)>(rteValue.RsdsSysStsLeCtraSts1);
            deValue.LcmaSts1 = static_cast<decltype(deValue.LcmaSts1)>(rteValue.RsdsSysStsLeLcmaSts1);

            RsdsSysStsLe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RsdsSysStsLe (" << ComConf_ComSignalGroup_igRsdsSysStsLe_mrx << "). Got " << length << ", expected " << sizeof(RsdsSysStsLe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igRsdsSysStsRi_mrx:
    {
        if (sizeof(RsdsSysStsRi) == length) {
            log_verbose() << "Received RsdsSysStsRi (" << ComConf_ComSignalGroup_igRsdsSysStsRi_mrx << ")";
            const RsdsSysStsRi& rteValue = *static_cast<const RsdsSysStsRi*>(buffer);
            autosar::RsdsSysStsRi_info::data_elem_type deValue;
            deValue.RcmSts = static_cast<decltype(deValue.RcmSts)>(rteValue.RsdsSysStsRiRcmSts);
            deValue.RsdsSts = static_cast<decltype(deValue.RsdsSts)>(rteValue.RsdsSysStsRiRsdsSts);
            deValue.CtraSts = static_cast<decltype(deValue.CtraSts)>(rteValue.RsdsSysStsRiCtraSts);
            deValue.RcwSts = static_cast<decltype(deValue.RcwSts)>(rteValue.RsdsSysStsRiRcwSts);
            deValue.LcmaSts = static_cast<decltype(deValue.LcmaSts)>(rteValue.RsdsSysStsRiLcmaSts);
            deValue.CtraSts1 = static_cast<decltype(deValue.CtraSts1)>(rteValue.RsdsSysStsRiCtraSts1);
            deValue.LcmaSts1 = static_cast<decltype(deValue.LcmaSts1)>(rteValue.RsdsSysStsRiLcmaSts1);

            RsdsSysStsRi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for RsdsSysStsRi (" << ComConf_ComSignalGroup_igRsdsSysStsRi_mrx << "). Got " << length << ", expected " << sizeof(RsdsSysStsRi);
        }
    }
    break;

    case ComConf_ComSignal_isScrBarVolIndcn_mrx:
    {
        if (sizeof(ScrBarVolIndcn) == length) {
            log_verbose() << "Received ScrBarVolIndcn (" << ComConf_ComSignal_isScrBarVolIndcn_mrx << ")";
            const ScrBarVolIndcn& rteValue = *static_cast<const ScrBarVolIndcn*>(buffer);
            autosar::ScrBarVolIndcn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 5.0 + 0.0;
            ScrBarVolIndcn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ScrBarVolIndcn (" << ComConf_ComSignal_isScrBarVolIndcn_mrx << "). Got " << length << ", expected " << sizeof(ScrBarVolIndcn);
        }
    }
    break;

    case ComConf_ComSignal_isScrMaxFillgVol_mrx:
    {
        if (sizeof(ScrMaxFillgVol) == length) {
            log_verbose() << "Received ScrMaxFillgVol (" << ComConf_ComSignal_isScrMaxFillgVol_mrx << ")";
            const ScrMaxFillgVol& rteValue = *static_cast<const ScrMaxFillgVol*>(buffer);
            autosar::ScrMaxFillgVol_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ScrMaxFillgVol_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ScrMaxFillgVol (" << ComConf_ComSignal_isScrMaxFillgVol_mrx << "). Got " << length << ", expected " << sizeof(ScrMaxFillgVol);
        }
    }
    break;

    case ComConf_ComSignal_isScrReagentTankVol_mrx:
    {
        if (sizeof(ScrReagentTankVol) == length) {
            log_verbose() << "Received ScrReagentTankVol (" << ComConf_ComSignal_isScrReagentTankVol_mrx << ")";
            const ScrReagentTankVol& rteValue = *static_cast<const ScrReagentTankVol*>(buffer);
            autosar::ScrReagentTankVol_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ScrReagentTankVol_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for ScrReagentTankVol (" << ComConf_ComSignal_isScrReagentTankVol_mrx << "). Got " << length << ", expected " << sizeof(ScrReagentTankVol);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSeatBackUnlckdThrd_mrx:
    {
        if (sizeof(SeatBackUnlckdThrd) == length) {
            log_verbose() << "Received SeatBackUnlckdThrd (" << ComConf_ComSignalGroup_igSeatBackUnlckdThrd_mrx << ")";
            const SeatBackUnlckdThrd& rteValue = *static_cast<const SeatBackUnlckdThrd*>(buffer);
            autosar::SeatBackUnlckdThrd_info::data_elem_type deValue;
            deValue.SeatBackUnlckdLe = static_cast<decltype(deValue.SeatBackUnlckdLe)>(rteValue.SeatBackUnlckdThrdLe);
            deValue.SeatBackUnlckdRi = static_cast<decltype(deValue.SeatBackUnlckdRi)>(rteValue.SeatBackUnlckdThrdRi);

            SeatBackUnlckdThrd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SeatBackUnlckdThrd (" << ComConf_ComSignalGroup_igSeatBackUnlckdThrd_mrx << "). Got " << length << ", expected " << sizeof(SeatBackUnlckdThrd);
        }
    }
    break;

    case ComConf_ComSignal_isSeatHeatgAutCdn_mrx:
    {
        if (sizeof(SeatHeatgAutCdn) == length) {
            log_verbose() << "Received SeatHeatgAutCdn (" << ComConf_ComSignal_isSeatHeatgAutCdn_mrx << ")";
            const SeatHeatgAutCdn& rteValue = *static_cast<const SeatHeatgAutCdn*>(buffer);
            autosar::SeatHeatgAutCdn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SeatHeatgAutCdn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SeatHeatgAutCdn (" << ComConf_ComSignal_isSeatHeatgAutCdn_mrx << "). Got " << length << ", expected " << sizeof(SeatHeatgAutCdn);
        }
    }
    break;

    case ComConf_ComSignal_isSeatVentnAutCdn_mrx:
    {
        if (sizeof(SeatVentnAutCdn) == length) {
            log_verbose() << "Received SeatVentnAutCdn (" << ComConf_ComSignal_isSeatVentnAutCdn_mrx << ")";
            const SeatVentnAutCdn& rteValue = *static_cast<const SeatVentnAutCdn*>(buffer);
            autosar::SeatVentnAutCdn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SeatVentnAutCdn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SeatVentnAutCdn (" << ComConf_ComSignal_isSeatVentnAutCdn_mrx << "). Got " << length << ", expected " << sizeof(SeatVentnAutCdn);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchAccActvnSts_mrx:
    {
        if (sizeof(SftyCchAccActvnSts) == length) {
            log_verbose() << "Received SftyCchAccActvnSts (" << ComConf_ComSignalGroup_igSftyCchAccActvnSts_mrx << ")";
            const SftyCchAccActvnSts& rteValue = *static_cast<const SftyCchAccActvnSts*>(buffer);
            autosar::SftyCchAccActvnSts_info::data_elem_type deValue;
            deValue.Mth = static_cast<decltype(deValue.Mth)>(rteValue.SftyCchAccActvnStsMth);
            deValue.Yr = static_cast<decltype(deValue.Yr)>(rteValue.SftyCchAccActvnStsYr);
            deValue.Day = static_cast<decltype(deValue.Day)>(rteValue.SftyCchAccActvnStsDay);
            deValue.Actv = static_cast<decltype(deValue.Actv)>(rteValue.SftyCchAccActvnStsActv);

            SftyCchAccActvnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchAccActvnSts (" << ComConf_ComSignalGroup_igSftyCchAccActvnSts_mrx << "). Got " << length << ", expected " << sizeof(SftyCchAccActvnSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchDrvgCycIdx_mrx:
    {
        if (sizeof(SftyCchDrvgCycIdx) == length) {
            log_verbose() << "Received SftyCchDrvgCycIdx (" << ComConf_ComSignalGroup_igSftyCchDrvgCycIdx_mrx << ")";
            const SftyCchDrvgCycIdx& rteValue = *static_cast<const SftyCchDrvgCycIdx*>(buffer);
            autosar::SftyCchDrvgCycIdx_info::data_elem_type deValue;
            deValue.LaneKeepCntxt = static_cast<decltype(deValue.LaneKeepCntxt)>(rteValue.SftyCchDrvgCycIdxLaneKeepCntxt);
            deValue.KeepSpdLimCntxt = static_cast<decltype(deValue.KeepSpdLimCntxt)>(rteValue.SftyCchDrvgCycIdxKeepSpdLimCntxt);
            deValue.TotAvl = static_cast<decltype(deValue.TotAvl)>(rteValue.SftyCchDrvgCycIdxTotAvl);
            deValue.KeepSpdLim = static_cast<decltype(deValue.KeepSpdLim)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxKeepSpdLim) ) * 0.1 + 0.0;
            deValue.AttentionAvl = static_cast<decltype(deValue.AttentionAvl)>(rteValue.SftyCchDrvgCycIdxAttentionAvl);
            deValue.Attention = static_cast<decltype(deValue.Attention)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxAttention) ) * 0.1 + 0.0;
            deValue.AttentionCntxt = static_cast<decltype(deValue.AttentionCntxt)>(rteValue.SftyCchDrvgCycIdxAttentionCntxt);
            deValue.Tot = static_cast<decltype(deValue.Tot)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxTot) ) * 0.1 + 0.0;
            deValue.DstToVehCntxt = static_cast<decltype(deValue.DstToVehCntxt)>(rteValue.SftyCchDrvgCycIdxDstToVehCntxt);
            deValue.KeepSpdLimAvl = static_cast<decltype(deValue.KeepSpdLimAvl)>(rteValue.SftyCchDrvgCycIdxKeepSpdLimAvl);
            deValue.LaneKeep = static_cast<decltype(deValue.LaneKeep)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxLaneKeep) ) * 0.1 + 0.0;
            deValue.TotCntxt = static_cast<decltype(deValue.TotCntxt)>(rteValue.SftyCchDrvgCycIdxTotCntxt);
            deValue.FldTotAvl = static_cast<decltype(deValue.FldTotAvl)>(rteValue.SftyCchDrvgCycIdxFldTotAvl);
            deValue.DstToVehAvl = static_cast<decltype(deValue.DstToVehAvl)>(rteValue.SftyCchDrvgCycIdxDstToVehAvl);
            deValue.LaneKeepAvl = static_cast<decltype(deValue.LaneKeepAvl)>(rteValue.SftyCchDrvgCycIdxLaneKeepAvl);
            deValue.FldTot = static_cast<decltype(deValue.FldTot)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxFldTot) ) * 0.1 + 0.0;
            deValue.DstToVeh = static_cast<decltype(deValue.DstToVeh)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxDstToVeh) ) * 0.1 + 0.0;

            SftyCchDrvgCycIdx_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchDrvgCycIdx (" << ComConf_ComSignalGroup_igSftyCchDrvgCycIdx_mrx << "). Got " << length << ", expected " << sizeof(SftyCchDrvgCycIdx);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchLaneKeepAidActvnSts_mrx:
    {
        if (sizeof(SftyCchLaneKeepAidActvnSts) == length) {
            log_verbose() << "Received SftyCchLaneKeepAidActvnSts (" << ComConf_ComSignalGroup_igSftyCchLaneKeepAidActvnSts_mrx << ")";
            const SftyCchLaneKeepAidActvnSts& rteValue = *static_cast<const SftyCchLaneKeepAidActvnSts*>(buffer);
            autosar::SftyCchLaneKeepAidActvnSts_info::data_elem_type deValue;
            deValue.Mth = static_cast<decltype(deValue.Mth)>(rteValue.SftyCchLaneKeepAidActvnStsMth);
            deValue.Yr = static_cast<decltype(deValue.Yr)>(rteValue.SftyCchLaneKeepAidActvnStsYr);
            deValue.Day = static_cast<decltype(deValue.Day)>(rteValue.SftyCchLaneKeepAidActvnStsDay);
            deValue.Actv = static_cast<decltype(deValue.Actv)>(rteValue.SftyCchLaneKeepAidActvnStsActv);

            SftyCchLaneKeepAidActvnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchLaneKeepAidActvnSts (" << ComConf_ComSignalGroup_igSftyCchLaneKeepAidActvnSts_mrx << "). Got " << length << ", expected " << sizeof(SftyCchLaneKeepAidActvnSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchLongTermIdx_mrx:
    {
        if (sizeof(SftyCchLongTermIdx) == length) {
            log_verbose() << "Received SftyCchLongTermIdx (" << ComConf_ComSignalGroup_igSftyCchLongTermIdx_mrx << ")";
            const SftyCchLongTermIdx& rteValue = *static_cast<const SftyCchLongTermIdx*>(buffer);
            autosar::SftyCchLongTermIdx_info::data_elem_type deValue;
            deValue.TotAvl = static_cast<decltype(deValue.TotAvl)>(rteValue.SftyCchLongTermIdxTotAvl);
            deValue.AttentionAvl = static_cast<decltype(deValue.AttentionAvl)>(rteValue.SftyCchLongTermIdxAttentionAvl);
            deValue.Attention = static_cast<decltype(deValue.Attention)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxAttention) ) * 0.1 + 0.0;
            deValue.Tot = static_cast<decltype(deValue.Tot)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxTot) ) * 0.1 + 0.0;
            deValue.KeepSpdLimAvl = static_cast<decltype(deValue.KeepSpdLimAvl)>(rteValue.SftyCchLongTermIdxKeepSpdLimAvl);
            deValue.LaneKeep = static_cast<decltype(deValue.LaneKeep)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxLaneKeep) ) * 0.1 + 0.0;
            deValue.DstToVehAvl = static_cast<decltype(deValue.DstToVehAvl)>(rteValue.SftyCchLongTermIdxDstToVehAvl);
            deValue.LaneKeepAvl = static_cast<decltype(deValue.LaneKeepAvl)>(rteValue.SftyCchLongTermIdxLaneKeepAvl);
            deValue.KeepSpdLim = static_cast<decltype(deValue.KeepSpdLim)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxKeepSpdLim) ) * 0.1 + 0.0;
            deValue.DstToVeh = static_cast<decltype(deValue.DstToVeh)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxDstToVeh) ) * 0.1 + 0.0;

            SftyCchLongTermIdx_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchLongTermIdx (" << ComConf_ComSignalGroup_igSftyCchLongTermIdx_mrx << "). Got " << length << ", expected " << sizeof(SftyCchLongTermIdx);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchMidTermIdx_mrx:
    {
        if (sizeof(SftyCchMidTermIdx) == length) {
            log_verbose() << "Received SftyCchMidTermIdx (" << ComConf_ComSignalGroup_igSftyCchMidTermIdx_mrx << ")";
            const SftyCchMidTermIdx& rteValue = *static_cast<const SftyCchMidTermIdx*>(buffer);
            autosar::SftyCchMidTermIdx_info::data_elem_type deValue;
            deValue.TotAvl = static_cast<decltype(deValue.TotAvl)>(rteValue.SftyCchMidTermIdxTotAvl);
            deValue.LaneKeep = static_cast<decltype(deValue.LaneKeep)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxLaneKeep) ) * 0.1 + 0.0;
            deValue.DstToVehAvl = static_cast<decltype(deValue.DstToVehAvl)>(rteValue.SftyCchMidTermIdxDstToVehAvl);
            deValue.Attention = static_cast<decltype(deValue.Attention)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxAttention) ) * 0.1 + 0.0;
            deValue.KeepSpdLim = static_cast<decltype(deValue.KeepSpdLim)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxKeepSpdLim) ) * 0.1 + 0.0;
            deValue.KeepSpdLimAvl = static_cast<decltype(deValue.KeepSpdLimAvl)>(rteValue.SftyCchMidTermIdxKeepSpdLimAvl);
            deValue.Tot = static_cast<decltype(deValue.Tot)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxTot) ) * 0.1 + 0.0;
            deValue.AttentionAvl = static_cast<decltype(deValue.AttentionAvl)>(rteValue.SftyCchMidTermIdxAttentionAvl);
            deValue.LaneKeepAvl = static_cast<decltype(deValue.LaneKeepAvl)>(rteValue.SftyCchMidTermIdxLaneKeepAvl);
            deValue.DstToVeh = static_cast<decltype(deValue.DstToVeh)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxDstToVeh) ) * 0.1 + 0.0;

            SftyCchMidTermIdx_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchMidTermIdx (" << ComConf_ComSignalGroup_igSftyCchMidTermIdx_mrx << "). Got " << length << ", expected " << sizeof(SftyCchMidTermIdx);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchPilotAssiActvnSts_mrx:
    {
        if (sizeof(SftyCchPilotAssiActvnSts) == length) {
            log_verbose() << "Received SftyCchPilotAssiActvnSts (" << ComConf_ComSignalGroup_igSftyCchPilotAssiActvnSts_mrx << ")";
            const SftyCchPilotAssiActvnSts& rteValue = *static_cast<const SftyCchPilotAssiActvnSts*>(buffer);
            autosar::SftyCchPilotAssiActvnSts_info::data_elem_type deValue;
            deValue.Mth = static_cast<decltype(deValue.Mth)>(rteValue.SftyCchPilotAssiActvnStsMth);
            deValue.Yr = static_cast<decltype(deValue.Yr)>(rteValue.SftyCchPilotAssiActvnStsYr);
            deValue.Day = static_cast<decltype(deValue.Day)>(rteValue.SftyCchPilotAssiActvnStsDay);
            deValue.Actv = static_cast<decltype(deValue.Actv)>(rteValue.SftyCchPilotAssiActvnStsActv);

            SftyCchPilotAssiActvnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchPilotAssiActvnSts (" << ComConf_ComSignalGroup_igSftyCchPilotAssiActvnSts_mrx << "). Got " << length << ", expected " << sizeof(SftyCchPilotAssiActvnSts);
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchPrimFbAftDrvg_mrx:
    {
        if (sizeof(SftyCchPrimFbAftDrvg) == length) {
            log_verbose() << "Received SftyCchPrimFbAftDrvg (" << ComConf_ComSignal_isSftyCchPrimFbAftDrvg_mrx << ")";
            const SftyCchPrimFbAftDrvg& rteValue = *static_cast<const SftyCchPrimFbAftDrvg*>(buffer);
            autosar::SftyCchPrimFbAftDrvg_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchPrimFbAftDrvg_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchPrimFbAftDrvg (" << ComConf_ComSignal_isSftyCchPrimFbAftDrvg_mrx << "). Got " << length << ", expected " << sizeof(SftyCchPrimFbAftDrvg);
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchPrimFbDurgDrvg_mrx:
    {
        if (sizeof(SftyCchPrimFbDurgDrvg) == length) {
            log_verbose() << "Received SftyCchPrimFbDurgDrvg (" << ComConf_ComSignal_isSftyCchPrimFbDurgDrvg_mrx << ")";
            const SftyCchPrimFbDurgDrvg& rteValue = *static_cast<const SftyCchPrimFbDurgDrvg*>(buffer);
            autosar::SftyCchPrimFbDurgDrvg_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchPrimFbDurgDrvg_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchPrimFbDurgDrvg (" << ComConf_ComSignal_isSftyCchPrimFbDurgDrvg_mrx << "). Got " << length << ", expected " << sizeof(SftyCchPrimFbDurgDrvg);
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchSecFbAftDrvg_mrx:
    {
        if (sizeof(SftyCchSecFbAftDrvg) == length) {
            log_verbose() << "Received SftyCchSecFbAftDrvg (" << ComConf_ComSignal_isSftyCchSecFbAftDrvg_mrx << ")";
            const SftyCchSecFbAftDrvg& rteValue = *static_cast<const SftyCchSecFbAftDrvg*>(buffer);
            autosar::SftyCchSecFbAftDrvg_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchSecFbAftDrvg_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchSecFbAftDrvg (" << ComConf_ComSignal_isSftyCchSecFbAftDrvg_mrx << "). Got " << length << ", expected " << sizeof(SftyCchSecFbAftDrvg);
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchSecFbDurgDrvg_mrx:
    {
        if (sizeof(SftyCchSecFbDurgDrvg) == length) {
            log_verbose() << "Received SftyCchSecFbDurgDrvg (" << ComConf_ComSignal_isSftyCchSecFbDurgDrvg_mrx << ")";
            const SftyCchSecFbDurgDrvg& rteValue = *static_cast<const SftyCchSecFbDurgDrvg*>(buffer);
            autosar::SftyCchSecFbDurgDrvg_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchSecFbDurgDrvg_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchSecFbDurgDrvg (" << ComConf_ComSignal_isSftyCchSecFbDurgDrvg_mrx << "). Got " << length << ", expected " << sizeof(SftyCchSecFbDurgDrvg);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchShortTermIdx_mrx:
    {
        if (sizeof(SftyCchShortTermIdx) == length) {
            log_verbose() << "Received SftyCchShortTermIdx (" << ComConf_ComSignalGroup_igSftyCchShortTermIdx_mrx << ")";
            const SftyCchShortTermIdx& rteValue = *static_cast<const SftyCchShortTermIdx*>(buffer);
            autosar::SftyCchShortTermIdx_info::data_elem_type deValue;
            deValue.LaneKeep = static_cast<decltype(deValue.LaneKeep)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxLaneKeep) ) * 0.1 + 0.0;
            deValue.TotAvl = static_cast<decltype(deValue.TotAvl)>(rteValue.SftyCchShortTermIdxTotAvl);
            deValue.AttentionAvl = static_cast<decltype(deValue.AttentionAvl)>(rteValue.SftyCchShortTermIdxAttentionAvl);
            deValue.Attention = static_cast<decltype(deValue.Attention)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxAttention) ) * 0.1 + 0.0;
            deValue.Tot = static_cast<decltype(deValue.Tot)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxTot) ) * 0.1 + 0.0;
            deValue.KeepSpdLimAvl = static_cast<decltype(deValue.KeepSpdLimAvl)>(rteValue.SftyCchShortTermIdxKeepSpdLimAvl);
            deValue.KeepSpdLim = static_cast<decltype(deValue.KeepSpdLim)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxKeepSpdLim) ) * 0.1 + 0.0;
            deValue.DstToVehAvl = static_cast<decltype(deValue.DstToVehAvl)>(rteValue.SftyCchShortTermIdxDstToVehAvl);
            deValue.LaneKeepAvl = static_cast<decltype(deValue.LaneKeepAvl)>(rteValue.SftyCchShortTermIdxLaneKeepAvl);
            deValue.DstToVeh = static_cast<decltype(deValue.DstToVeh)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxDstToVeh) ) * 0.1 + 0.0;

            SftyCchShortTermIdx_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchShortTermIdx (" << ComConf_ComSignalGroup_igSftyCchShortTermIdx_mrx << "). Got " << length << ", expected " << sizeof(SftyCchShortTermIdx);
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchSts_mrx:
    {
        if (sizeof(SftyCchSts) == length) {
            log_verbose() << "Received SftyCchSts (" << ComConf_ComSignal_isSftyCchSts_mrx << ")";
            const SftyCchSts& rteValue = *static_cast<const SftyCchSts*>(buffer);
            autosar::SftyCchSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyCchSts (" << ComConf_ComSignal_isSftyCchSts_mrx << "). Got " << length << ", expected " << sizeof(SftyCchSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyWarnGroupFromAsySafe_mrx:
    {
        if (sizeof(SftyWarnGroupFromAsySafe) == length) {
            log_verbose() << "Received SftyWarnGroupFromAsySafe (" << ComConf_ComSignalGroup_igSftyWarnGroupFromAsySafe_mrx << ")";
            const SftyWarnGroupFromAsySafe& rteValue = *static_cast<const SftyWarnGroupFromAsySafe*>(buffer);
            autosar::SftyWarnGroupFromAsySafe_info::data_elem_type deValue;
            deValue.CnclWarnLatForAutDrv = static_cast<decltype(deValue.CnclWarnLatForAutDrv)>(rteValue.CnclWarnLatForAutDrv);
            deValue.SteerOvrdWarnReqForAutDrv = static_cast<decltype(deValue.SteerOvrdWarnReqForAutDrv)>(rteValue.SteerOvrdWarnReqForAutDrv);
            deValue.SftyWarnGroupFromAsySafeChks = static_cast<decltype(deValue.SftyWarnGroupFromAsySafeChks)>(rteValue.SftyWarnGroupFromAsySafeChks);
            deValue.SteerStsForAutDrv = static_cast<decltype(deValue.SteerStsForAutDrv)>(rteValue.SteerStsForAutDrv);
            deValue.SftyWarnGroupFromAsySafeCntr = static_cast<decltype(deValue.SftyWarnGroupFromAsySafeCntr)>(rteValue.SftyWarnGroupFromAsySafeCntr);

            SftyWarnGroupFromAsySafe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SftyWarnGroupFromAsySafe (" << ComConf_ComSignalGroup_igSftyWarnGroupFromAsySafe_mrx << "). Got " << length << ", expected " << sizeof(SftyWarnGroupFromAsySafe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSnsrParkAssiFrnt_mrx:
    {
        if (sizeof(SnsrParkAssiFrnt) == length) {
            log_verbose() << "Received SnsrParkAssiFrnt (" << ComConf_ComSignalGroup_igSnsrParkAssiFrnt_mrx << ")";
            const SnsrParkAssiFrnt& rteValue = *static_cast<const SnsrParkAssiFrnt*>(buffer);
            autosar::SnsrParkAssiFrnt_info::data_elem_type deValue;
            deValue.DstOfSnsrOutdRi = static_cast<decltype(deValue.DstOfSnsrOutdRi)>(rteValue.ParkAssiDstFrntOfDstOfSnsrOutdRi);
            deValue.DstOfSnsrRiSide = static_cast<decltype(deValue.DstOfSnsrRiSide)>(rteValue.ParkAssiDstFrntOfDstOfSnsrRiSide);
            deValue.DstOfSnsrOutdLe = static_cast<decltype(deValue.DstOfSnsrOutdLe)>(rteValue.ParkAssiDstFrntOfSnsrOutdLe);
            deValue.DstOfSnsrInsdLe = static_cast<decltype(deValue.DstOfSnsrInsdLe)>(rteValue.ParkAssiDstFrntOfSnsrInsdLe);
            deValue.AudSideWarn = static_cast<decltype(deValue.AudSideWarn)>(rteValue.ParkAssiDstFrntOfAudSideWarn);
            deValue.AudWarnDir = static_cast<decltype(deValue.AudWarnDir)>(rteValue.ParkAssiDstFrntOfAudWarnDir);
            deValue.DstOfSnsrLeSide = static_cast<decltype(deValue.DstOfSnsrLeSide)>(rteValue.ParkAssiDstFrntOfDstOfSnsrLeSide);
            deValue.DstOfSnsrInsdRi = static_cast<decltype(deValue.DstOfSnsrInsdRi)>(rteValue.ParkAssiDstFrntOfDstOfSnsrInsdRi);

            SnsrParkAssiFrnt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SnsrParkAssiFrnt (" << ComConf_ComSignalGroup_igSnsrParkAssiFrnt_mrx << "). Got " << length << ", expected " << sizeof(SnsrParkAssiFrnt);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSnsrParkAssiRe_mrx:
    {
        if (sizeof(SnsrParkAssiRe) == length) {
            log_verbose() << "Received SnsrParkAssiRe (" << ComConf_ComSignalGroup_igSnsrParkAssiRe_mrx << ")";
            const SnsrParkAssiRe& rteValue = *static_cast<const SnsrParkAssiRe*>(buffer);
            autosar::SnsrParkAssiRe_info::data_elem_type deValue;
            deValue.DstOfSnsrOutdRi = static_cast<decltype(deValue.DstOfSnsrOutdRi)>(rteValue.ParkAssiDstReOfDstOfSnsrOutdRi);
            deValue.DstOfSnsrInsdLe = static_cast<decltype(deValue.DstOfSnsrInsdLe)>(rteValue.ParkAssiDstReOfDstOfSnsrInsdLe);
            deValue.DstOfSnsrRiSide = static_cast<decltype(deValue.DstOfSnsrRiSide)>(rteValue.ParkAssiDstReOfDstOfSnsrRiSide);
            deValue.DstOfSnsrOutdLe = static_cast<decltype(deValue.DstOfSnsrOutdLe)>(rteValue.ParkAssiDstReOfDstOfSnsrOutdLe);
            deValue.AudSideWarn = static_cast<decltype(deValue.AudSideWarn)>(rteValue.ParkAssiDstReOfAudSideWarn);
            deValue.AudWarnDir = static_cast<decltype(deValue.AudWarnDir)>(rteValue.ParkAssiDstReOfAudWarnDir);
            deValue.DstOfSnsrLeSide = static_cast<decltype(deValue.DstOfSnsrLeSide)>(rteValue.ParkAssiDstReOfDstOfSnsrLeSide);
            deValue.DstOfSnsrInsdRi = static_cast<decltype(deValue.DstOfSnsrInsdRi)>(rteValue.ParkAssiDstReOfDstOfSnsrInsdRi);

            SnsrParkAssiRe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SnsrParkAssiRe (" << ComConf_ComSignalGroup_igSnsrParkAssiRe_mrx << "). Got " << length << ", expected " << sizeof(SnsrParkAssiRe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSnsrPrkgAssiFrnt_mrx:
    {
        if (sizeof(SnsrPrkgAssiFrnt) == length) {
            log_verbose() << "Received SnsrPrkgAssiFrnt (" << ComConf_ComSignalGroup_igSnsrPrkgAssiFrnt_mrx << ")";
            const SnsrPrkgAssiFrnt& rteValue = *static_cast<const SnsrPrkgAssiFrnt*>(buffer);
            autosar::SnsrPrkgAssiFrnt_info::data_elem_type deValue;
            deValue.SnsrDstInsdLe = static_cast<decltype(deValue.SnsrDstInsdLe)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiFrntSnsrDstInsdLe) ) * -1.0 + 1023.0;
            deValue.SnsrDstSideRi = static_cast<decltype(deValue.SnsrDstSideRi)>( toUnsignedFromRaw<9>(rteValue.SnsrPrkgAssiFrntSnsrDstSideRi) ) * -1.0 + 511.0;
            deValue.SnsrDstOutdRi = static_cast<decltype(deValue.SnsrDstOutdRi)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiFrntSnsrDstOutdRi) ) * -1.0 + 1023.0;
            deValue.SnsrDstInsdRi = static_cast<decltype(deValue.SnsrDstInsdRi)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiFrntSnsrDstInsdRi) ) * -1.0 + 1023.0;
            deValue.AudSideWarn = static_cast<decltype(deValue.AudSideWarn)>(rteValue.SnsrPrkgAssiFrntAudSideWarn);
            deValue.AudWarnDir = static_cast<decltype(deValue.AudWarnDir)>(rteValue.SnsrPrkgAssiFrntAudWarnDir);
            deValue.SnsrDstSideLe = static_cast<decltype(deValue.SnsrDstSideLe)>( toUnsignedFromRaw<9>(rteValue.SnsrPrkgAssiFrntSnsrDstSideLe) ) * -1.0 + 511.0;
            deValue.SnsrDstOutdLe = static_cast<decltype(deValue.SnsrDstOutdLe)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiFrntSnsrDstOutdLe) ) * -1.0 + 1023.0;

            SnsrPrkgAssiFrnt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SnsrPrkgAssiFrnt (" << ComConf_ComSignalGroup_igSnsrPrkgAssiFrnt_mrx << "). Got " << length << ", expected " << sizeof(SnsrPrkgAssiFrnt);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSnsrPrkgAssiRe_mrx:
    {
        if (sizeof(SnsrPrkgAssiRe) == length) {
            log_verbose() << "Received SnsrPrkgAssiRe (" << ComConf_ComSignalGroup_igSnsrPrkgAssiRe_mrx << ")";
            const SnsrPrkgAssiRe& rteValue = *static_cast<const SnsrPrkgAssiRe*>(buffer);
            autosar::SnsrPrkgAssiRe_info::data_elem_type deValue;
            deValue.SnsrDstInsdLe = static_cast<decltype(deValue.SnsrDstInsdLe)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiReSnsrDstInsdLe) ) * -1.0 + 1023.0;
            deValue.SnsrDstSideRi = static_cast<decltype(deValue.SnsrDstSideRi)>( toUnsignedFromRaw<9>(rteValue.SnsrPrkgAssiReSnsrDstSideRi) ) * -1.0 + 511.0;
            deValue.SnsrDstOutdRi = static_cast<decltype(deValue.SnsrDstOutdRi)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiReSnsrDstOutdRi) ) * -1.0 + 1023.0;
            deValue.SnsrDstInsdRi = static_cast<decltype(deValue.SnsrDstInsdRi)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiReSnsrDstInsdRi) ) * -1.0 + 1023.0;
            deValue.AudSideWarn = static_cast<decltype(deValue.AudSideWarn)>(rteValue.SnsrPrkgAssiReAudSideWarn);
            deValue.AudWarnDir = static_cast<decltype(deValue.AudWarnDir)>(rteValue.SnsrPrkgAssiReAudWarnDir);
            deValue.SnsrDstSideLe = static_cast<decltype(deValue.SnsrDstSideLe)>( toUnsignedFromRaw<9>(rteValue.SnsrPrkgAssiReSnsrDstSideLe) ) * -1.0 + 511.0;
            deValue.SnsrDstOutdLe = static_cast<decltype(deValue.SnsrDstOutdLe)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiReSnsrDstOutdLe) ) * -1.0 + 1023.0;

            SnsrPrkgAssiRe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SnsrPrkgAssiRe (" << ComConf_ComSignalGroup_igSnsrPrkgAssiRe_mrx << "). Got " << length << ", expected " << sizeof(SnsrPrkgAssiRe);
        }
    }
    break;

    case ComConf_ComSignal_isWhlMotSysSpdAct_mrx:
    {
        if (sizeof(WhlMotSysSpdAct) == length) {
            log_verbose() << "Received WhlMotSysSpdAct (" << ComConf_ComSignal_isWhlMotSysSpdAct_mrx << ")";
            const WhlMotSysSpdAct& rteValue = *static_cast<const WhlMotSysSpdAct*>(buffer);
            autosar::WhlMotSysSpdAct_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.1 + -1638.4;
            WhlMotSysSpdAct_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WhlMotSysSpdAct (" << ComConf_ComSignal_isWhlMotSysSpdAct_mrx << "). Got " << length << ", expected " << sizeof(WhlMotSysSpdAct);
        }
    }
    break;

    case ComConf_ComSignal_isSpdLimAdpvSts_mrx:
    {
        if (sizeof(SpdLimAdpvSts) == length) {
            log_verbose() << "Received SpdLimAdpvSts (" << ComConf_ComSignal_isSpdLimAdpvSts_mrx << ")";
            const SpdLimAdpvSts& rteValue = *static_cast<const SpdLimAdpvSts*>(buffer);
            autosar::SpdLimAdpvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SpdLimAdpvSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SpdLimAdpvSts (" << ComConf_ComSignal_isSpdLimAdpvSts_mrx << "). Got " << length << ", expected " << sizeof(SpdLimAdpvSts);
        }
    }
    break;

    case ComConf_ComSignal_isSpdLimWarnReq_mrx:
    {
        if (sizeof(SpdLimWarnReq) == length) {
            log_verbose() << "Received SpdLimWarnReq (" << ComConf_ComSignal_isSpdLimWarnReq_mrx << ")";
            const SpdLimWarnReq& rteValue = *static_cast<const SpdLimWarnReq*>(buffer);
            autosar::SpdLimWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SpdLimWarnReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SpdLimWarnReq (" << ComConf_ComSignal_isSpdLimWarnReq_mrx << "). Got " << length << ", expected " << sizeof(SpdLimWarnReq);
        }
    }
    break;

    case ComConf_ComSignal_isSpdWarn_mrx:
    {
        if (sizeof(SpdWarn) == length) {
            log_verbose() << "Received SpdWarn (" << ComConf_ComSignal_isSpdWarn_mrx << ")";
            const SpdWarn& rteValue = *static_cast<const SpdWarn*>(buffer);
            autosar::SpdWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SpdWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SpdWarn (" << ComConf_ComSignal_isSpdWarn_mrx << "). Got " << length << ", expected " << sizeof(SpdWarn);
        }
    }
    break;

    case ComConf_ComSignal_isSrvRst_mrx:
    {
        if (sizeof(SrvRst) == length) {
            log_verbose() << "Received SrvRst (" << ComConf_ComSignal_isSrvRst_mrx << ")";
            const SrvRst& rteValue = *static_cast<const SrvRst*>(buffer);
            autosar::SrvRst_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SrvRst_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SrvRst (" << ComConf_ComSignal_isSrvRst_mrx << "). Got " << length << ", expected " << sizeof(SrvRst);
        }
    }
    break;

    case ComConf_ComSignal_isSrvTrig_mrx:
    {
        if (sizeof(SrvTrig) == length) {
            log_verbose() << "Received SrvTrig (" << ComConf_ComSignal_isSrvTrig_mrx << ")";
            const SrvTrig& rteValue = *static_cast<const SrvTrig*>(buffer);
            autosar::SrvTrig_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) );
            SrvTrig_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SrvTrig (" << ComConf_ComSignal_isSrvTrig_mrx << "). Got " << length << ", expected " << sizeof(SrvTrig);
        }
    }
    break;

    case ComConf_ComSignal_isSteerWarnReqForAutDrv_mrx:
    {
        if (sizeof(SteerWarnReqForAutDrv) == length) {
            log_verbose() << "Received SteerWarnReqForAutDrv (" << ComConf_ComSignal_isSteerWarnReqForAutDrv_mrx << ")";
            const SteerWarnReqForAutDrv& rteValue = *static_cast<const SteerWarnReqForAutDrv*>(buffer);
            autosar::SteerWarnReqForAutDrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SteerWarnReqForAutDrv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SteerWarnReqForAutDrv (" << ComConf_ComSignal_isSteerWarnReqForAutDrv_mrx << "). Got " << length << ", expected " << sizeof(SteerWarnReqForAutDrv);
        }
    }
    break;

    case ComConf_ComSignal_isSteerWarnReqForLaneKeepAid_mrx:
    {
        if (sizeof(SteerWarnReqForLaneKeepAid) == length) {
            log_verbose() << "Received SteerWarnReqForLaneKeepAid (" << ComConf_ComSignal_isSteerWarnReqForLaneKeepAid_mrx << ")";
            const SteerWarnReqForLaneKeepAid& rteValue = *static_cast<const SteerWarnReqForLaneKeepAid*>(buffer);
            autosar::SteerWarnReqForLaneKeepAid_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SteerWarnReqForLaneKeepAid_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SteerWarnReqForLaneKeepAid (" << ComConf_ComSignal_isSteerWarnReqForLaneKeepAid_mrx << "). Got " << length << ", expected " << sizeof(SteerWarnReqForLaneKeepAid);
        }
    }
    break;

    case ComConf_ComSignal_isSteerWhlHeatgStrtAutCdnOk_mrx:
    {
        if (sizeof(SteerWhlHeatgStrtAutCdnOk) == length) {
            log_verbose() << "Received SteerWhlHeatgStrtAutCdnOk (" << ComConf_ComSignal_isSteerWhlHeatgStrtAutCdnOk_mrx << ")";
            const SteerWhlHeatgStrtAutCdnOk& rteValue = *static_cast<const SteerWhlHeatgStrtAutCdnOk*>(buffer);
            autosar::SteerWhlHeatgStrtAutCdnOk_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SteerWhlHeatgStrtAutCdnOk_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SteerWhlHeatgStrtAutCdnOk (" << ComConf_ComSignal_isSteerWhlHeatgStrtAutCdnOk_mrx << "). Got " << length << ", expected " << sizeof(SteerWhlHeatgStrtAutCdnOk);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSteerWhlSnsr_mrx:
    {
        if (sizeof(SteerWhlSnsr) == length) {
            log_verbose() << "Received SteerWhlSnsr (" << ComConf_ComSignalGroup_igSteerWhlSnsr_mrx << ")";
            const SteerWhlSnsr& rteValue = *static_cast<const SteerWhlSnsr*>(buffer);
            autosar::SteerWhlSnsrSafe_info::data_elem_type deValue;
            deValue.SteerWhlAg = static_cast<decltype(deValue.SteerWhlAg)>( toSignedFromRaw<15>(rteValue.SteerWhlAgSafe) ) * 9.765625E-4 + 0.0;
            deValue.SteerWhlSnsrChks = static_cast<decltype(deValue.SteerWhlSnsrChks)>(rteValue.SteerWhlSnsrSafeChks);
            deValue.SteerWhlAgSpd = static_cast<decltype(deValue.SteerWhlAgSpd)>( toSignedFromRaw<14>(rteValue.SteerWhlAgSpdSafe) ) * 0.0078125 + 0.0;
            deValue.SteerWhlSnsrQf = static_cast<decltype(deValue.SteerWhlSnsrQf)>(rteValue.SteerWhlSnsrQf);
            deValue.SteerWhlSnsrCntr = static_cast<decltype(deValue.SteerWhlSnsrCntr)>(rteValue.SteerWhlSnsrSafeCntr);

            SteerWhlSnsrSafe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SteerWhlSnsr (" << ComConf_ComSignalGroup_igSteerWhlSnsr_mrx << "). Got " << length << ", expected " << sizeof(SteerWhlSnsr);
        }
    }
    break;

    case ComConf_ComSignal_isSteerWhlSwtPwr_mrx:
    {
        if (sizeof(SteerWhlSwtPwr) == length) {
            log_verbose() << "Received SteerWhlSwtPwr (" << ComConf_ComSignal_isSteerWhlSwtPwr_mrx << ")";
            const SteerWhlSwtPwr& rteValue = *static_cast<const SteerWhlSwtPwr*>(buffer);
            autosar::SteerWhlSwtPwr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SteerWhlSwtPwr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SteerWhlSwtPwr (" << ComConf_ComSignal_isSteerWhlSwtPwr_mrx << "). Got " << length << ", expected " << sizeof(SteerWhlSwtPwr);
        }
    }
    break;

    case ComConf_ComSignal_isStopStrtInhb_mrx:
    {
        if (sizeof(StopStrtInhb) == length) {
            log_verbose() << "Received StopStrtInhb (" << ComConf_ComSignal_isStopStrtInhb_mrx << ")";
            const StopStrtInhb& rteValue = *static_cast<const StopStrtInhb*>(buffer);
            autosar::StopStrtInhb_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            StopStrtInhb_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for StopStrtInhb (" << ComConf_ComSignal_isStopStrtInhb_mrx << "). Got " << length << ", expected " << sizeof(StopStrtInhb);
        }
    }
    break;

    case ComConf_ComSignal_isStrtInProgs_mrx:
    {
        if (sizeof(StrtInProgs) == length) {
            log_verbose() << "Received StrtInProgs (" << ComConf_ComSignal_isStrtInProgs_mrx << ")";
            const StrtInProgs& rteValue = *static_cast<const StrtInProgs*>(buffer);
            autosar::StrtInProgs_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            StrtInProgs_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for StrtInProgs (" << ComConf_ComSignal_isStrtInProgs_mrx << "). Got " << length << ", expected " << sizeof(StrtInProgs);
        }
    }
    break;

    case ComConf_ComSignal_isSunRoofPosnSts_mrx:
    {
        if (sizeof(SunRoofPosnSts) == length) {
            log_verbose() << "Received SunRoofPosnSts (" << ComConf_ComSignal_isSunRoofPosnSts_mrx << ")";
            const SunRoofPosnSts& rteValue = *static_cast<const SunRoofPosnSts*>(buffer);
            autosar::SunRoofPosnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SunRoofPosnSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SunRoofPosnSts (" << ComConf_ComSignal_isSunRoofPosnSts_mrx << "). Got " << length << ", expected " << sizeof(SunRoofPosnSts);
        }
    }
    break;

    case ComConf_ComSignal_isSwtAcptReq_mrx:
    {
        if (sizeof(SwtAcptReq) == length) {
            log_verbose() << "Received SwtAcptReq (" << ComConf_ComSignal_isSwtAcptReq_mrx << ")";
            const SwtAcptReq& rteValue = *static_cast<const SwtAcptReq*>(buffer);
            autosar::SwtAcptReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SwtAcptReq_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SwtAcptReq (" << ComConf_ComSignal_isSwtAcptReq_mrx << "). Got " << length << ", expected " << sizeof(SwtAcptReq);
        }
    }
    break;

    case ComConf_ComSignal_isSwtForPassAirbCutOffSt_mrx:
    {
        if (sizeof(SwtForPassAirbCutOffSt) == length) {
            log_verbose() << "Received SwtForPassAirbCutOffSt (" << ComConf_ComSignal_isSwtForPassAirbCutOffSt_mrx << ")";
            const SwtForPassAirbCutOffSt& rteValue = *static_cast<const SwtForPassAirbCutOffSt*>(buffer);
            autosar::SwtForPassAirbCutOffSt_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SwtForPassAirbCutOffSt_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SwtForPassAirbCutOffSt (" << ComConf_ComSignal_isSwtForPassAirbCutOffSt_mrx << "). Got " << length << ", expected " << sizeof(SwtForPassAirbCutOffSt);
        }
    }
    break;

    case ComConf_ComSignal_isTankFlapSts_mrx:
    {
        if (sizeof(TankFlapSts) == length) {
            log_verbose() << "Received TankFlapSts (" << ComConf_ComSignal_isTankFlapSts_mrx << ")";
            const TankFlapSts& rteValue = *static_cast<const TankFlapSts*>(buffer);
            autosar::TankFlapSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TankFlapSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TankFlapSts (" << ComConf_ComSignal_isTankFlapSts_mrx << "). Got " << length << ", expected " << sizeof(TankFlapSts);
        }
    }
    break;

    case ComConf_ComSignal_isTelmSts_mrx:
    {
        if (sizeof(TelmSts) == length) {
            log_verbose() << "Received TelmSts (" << ComConf_ComSignal_isTelmSts_mrx << ")";
            const TelmSts& rteValue = *static_cast<const TelmSts*>(buffer);
            autosar::TelmSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TelmSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TelmSts (" << ComConf_ComSignal_isTelmSts_mrx << "). Got " << length << ", expected " << sizeof(TelmSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTiAndDateIndcn_mrx:
    {
        if (sizeof(TiAndDateIndcn) == length) {
            log_verbose() << "Received TiAndDateIndcn (" << ComConf_ComSignalGroup_igTiAndDateIndcn_mrx << ")";
            const TiAndDateIndcn& rteValue = *static_cast<const TiAndDateIndcn*>(buffer);
            autosar::TiAndDateIndcn_info::data_elem_type deValue;
            deValue.Mth1 = static_cast<decltype(deValue.Mth1)>(rteValue.Mth);
            deValue.Mins1 = static_cast<decltype(deValue.Mins1)>(rteValue.Mins);
            deValue.Hr1 = static_cast<decltype(deValue.Hr1)>(rteValue.Hr);
            deValue.Sec1 = static_cast<decltype(deValue.Sec1)>(rteValue.Sec);
            deValue.DataValid = static_cast<decltype(deValue.DataValid)>(rteValue.TiAndDateVld);
            deValue.Yr1 = static_cast<decltype(deValue.Yr1)>(rteValue.Yr);
            deValue.Day = static_cast<decltype(deValue.Day)>(rteValue.Day);

            TiAndDateIndcn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TiAndDateIndcn (" << ComConf_ComSignalGroup_igTiAndDateIndcn_mrx << "). Got " << length << ", expected " << sizeof(TiAndDateIndcn);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePAbsltValFrntRi_mrx:
    {
        if (sizeof(TirePAbsltValFrntRi) == length) {
            log_verbose() << "Received TirePAbsltValFrntRi (" << ComConf_ComSignalGroup_igTirePAbsltValFrntRi_mrx << ")";
            const TirePAbsltValFrntRi& rteValue = *static_cast<const TirePAbsltValFrntRi*>(buffer);
            autosar::TirePAbsltValFrntRi_info::data_elem_type deValue;
            deValue.TirePAbsltVal1 = static_cast<decltype(deValue.TirePAbsltVal1)>( toUnsignedFromRaw<8>(rteValue.TirePAbsltValFrntRiTirePAbsltVal1) ) * 2.5 + 0.0;
            deValue.TirePAbsltValQF = static_cast<decltype(deValue.TirePAbsltValQF)>(rteValue.TirePAbsltValQFForFrnRi);
            deValue.TirePPosn_ = static_cast<decltype(deValue.TirePPosn_)>(rteValue.TirePAbsltValFrntRiTirePPosn);

            TirePAbsltValFrntRi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TirePAbsltValFrntRi (" << ComConf_ComSignalGroup_igTirePAbsltValFrntRi_mrx << "). Got " << length << ", expected " << sizeof(TirePAbsltValFrntRi);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePAbsltValFrntLe_mrx:
    {
        if (sizeof(TirePAbsltValFrntLe) == length) {
            log_verbose() << "Received TirePAbsltValFrntLe (" << ComConf_ComSignalGroup_igTirePAbsltValFrntLe_mrx << ")";
            const TirePAbsltValFrntLe& rteValue = *static_cast<const TirePAbsltValFrntLe*>(buffer);
            autosar::TirePAbsltValFrntLe_info::data_elem_type deValue;
            deValue.TirepabsltVal1 = static_cast<decltype(deValue.TirepabsltVal1)>( toUnsignedFromRaw<8>(rteValue.TirePAbsltValForFrntLe) ) * 2.5 + 0.0;
            deValue.TirePabsltValQF = static_cast<decltype(deValue.TirePabsltValQF)>(rteValue.TirePAbsltVaQflForFrntLe);
            deValue.TirePPosn_ = static_cast<decltype(deValue.TirePPosn_)>(rteValue.TirePPosnForFrntLe);

            TirePAbsltValFrntLe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TirePAbsltValFrntLe (" << ComConf_ComSignalGroup_igTirePAbsltValFrntLe_mrx << "). Got " << length << ", expected " << sizeof(TirePAbsltValFrntLe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePAbsltValReLe_mrx:
    {
        if (sizeof(TirePAbsltValReLe) == length) {
            log_verbose() << "Received TirePAbsltValReLe (" << ComConf_ComSignalGroup_igTirePAbsltValReLe_mrx << ")";
            const TirePAbsltValReLe& rteValue = *static_cast<const TirePAbsltValReLe*>(buffer);
            autosar::TirePAbsltValReLe_info::data_elem_type deValue;
            deValue.TirePAbsltVal1 = static_cast<decltype(deValue.TirePAbsltVal1)>( toUnsignedFromRaw<8>(rteValue.TirePAbsltValForReLe) ) * 2.5 + 0.0;
            deValue.TirePAbsltValQF = static_cast<decltype(deValue.TirePAbsltValQF)>(rteValue.TirePAbsltValQfForReLe);
            deValue.TirePPosn_ = static_cast<decltype(deValue.TirePPosn_)>(rteValue.TirePPosnForReLe);

            TirePAbsltValReLe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TirePAbsltValReLe (" << ComConf_ComSignalGroup_igTirePAbsltValReLe_mrx << "). Got " << length << ", expected " << sizeof(TirePAbsltValReLe);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePAbsltValReRi_mrx:
    {
        if (sizeof(TirePAbsltValReRi) == length) {
            log_verbose() << "Received TirePAbsltValReRi (" << ComConf_ComSignalGroup_igTirePAbsltValReRi_mrx << ")";
            const TirePAbsltValReRi& rteValue = *static_cast<const TirePAbsltValReRi*>(buffer);
            autosar::TirePAbsltValReRi_info::data_elem_type deValue;
            deValue.TirePAbsltVal1 = static_cast<decltype(deValue.TirePAbsltVal1)>( toUnsignedFromRaw<8>(rteValue.TirePAbsltValForReRi) ) * 2.5 + 0.0;
            deValue.TirePAbsltValQF = static_cast<decltype(deValue.TirePAbsltValQF)>(rteValue.TirePAbsltValQfForReRi);
            deValue.TirePPosn_ = static_cast<decltype(deValue.TirePPosn_)>(rteValue.TirePPosnForReRi);

            TirePAbsltValReRi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TirePAbsltValReRi (" << ComConf_ComSignalGroup_igTirePAbsltValReRi_mrx << "). Got " << length << ", expected " << sizeof(TirePAbsltValReRi);
        }
    }
    break;

    case ComConf_ComSignal_isTirePCalSts_mrx:
    {
        if (sizeof(TirePCalSts) == length) {
            log_verbose() << "Received TirePCalSts (" << ComConf_ComSignal_isTirePCalSts_mrx << ")";
            const TirePCalSts& rteValue = *static_cast<const TirePCalSts*>(buffer);
            autosar::TirePCalSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TirePCalSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TirePCalSts (" << ComConf_ComSignal_isTirePCalSts_mrx << "). Got " << length << ", expected " << sizeof(TirePCalSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePMonData_mrx:
    {
        if (sizeof(TirePMonData) == length) {
            log_verbose() << "Received TirePMonData (" << ComConf_ComSignalGroup_igTirePMonData_mrx << ")";
            const TirePMonData& rteValue = *static_cast<const TirePMonData*>(buffer);
            autosar::TirePMonData_info::data_elem_type deValue;
            deValue.TirePFrntRe = static_cast<decltype(deValue.TirePFrntRe)>(rteValue.TirePFrntRi);
            deValue.TirePMonSts1_ = static_cast<decltype(deValue.TirePMonSts1_)>(rteValue.TirePMonSts);
            deValue.TirePFrntLe = static_cast<decltype(deValue.TirePFrntLe)>(rteValue.TirePFrntLe);
            deValue.TirePReLe = static_cast<decltype(deValue.TirePReLe)>(rteValue.TirePReLe);
            deValue.TirePReRi = static_cast<decltype(deValue.TirePReRi)>(rteValue.TirePReRi);

            TirePMonData_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TirePMonData (" << ComConf_ComSignalGroup_igTirePMonData_mrx << "). Got " << length << ", expected " << sizeof(TirePMonData);
        }
    }
    break;

    case ComConf_ComSignal_isTnlEntryOrEnd_mrx:
    {
        if (sizeof(TnlEntryOrEnd) == length) {
            log_verbose() << "Received TnlEntryOrEnd (" << ComConf_ComSignal_isTnlEntryOrEnd_mrx << ")";
            const TnlEntryOrEnd& rteValue = *static_cast<const TnlEntryOrEnd*>(buffer);
            autosar::TnlEntryOrEnd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TnlEntryOrEnd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TnlEntryOrEnd (" << ComConf_ComSignal_isTnlEntryOrEnd_mrx << "). Got " << length << ", expected " << sizeof(TnlEntryOrEnd);
        }
    }
    break;

    case ComConf_ComSignal_isTrSts_mrx:
    {
        if (sizeof(TrSts) == length) {
            log_verbose() << "Received TrSts (" << ComConf_ComSignal_isTrSts_mrx << ")";
            const TrSts& rteValue = *static_cast<const TrSts*>(buffer);
            autosar::TrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TrSts (" << ComConf_ComSignal_isTrSts_mrx << "). Got " << length << ", expected " << sizeof(TrSts);
        }
    }
    break;

    case ComConf_ComSignal_isTrfcLiSpprtSts_mrx:
    {
        if (sizeof(TrfcLiSpprtSts) == length) {
            log_verbose() << "Received TrfcLiSpprtSts (" << ComConf_ComSignal_isTrfcLiSpprtSts_mrx << ")";
            const TrfcLiSpprtSts& rteValue = *static_cast<const TrfcLiSpprtSts*>(buffer);
            autosar::TrfcLiSpprtSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrfcLiSpprtSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TrfcLiSpprtSts (" << ComConf_ComSignal_isTrfcLiSpprtSts_mrx << "). Got " << length << ", expected " << sizeof(TrfcLiSpprtSts);
        }
    }
    break;

    case ComConf_ComSignal_isTrlrLampActvtChk_mrx:
    {
        if (sizeof(TrlrLampActvtChk) == length) {
            log_verbose() << "Received TrlrLampActvtChk (" << ComConf_ComSignal_isTrlrLampActvtChk_mrx << ")";
            const TrlrLampActvtChk& rteValue = *static_cast<const TrlrLampActvtChk*>(buffer);
            autosar::TrlrLampChkSts1WdSts1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrlrLampChkSts1WdSts1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TrlrLampActvtChk (" << ComConf_ComSignal_isTrlrLampActvtChk_mrx << "). Got " << length << ", expected " << sizeof(TrlrLampActvtChk);
        }
    }
    break;

    case ComConf_ComSignal_isTrlrLampChkSts_mrx:
    {
        if (sizeof(TrlrLampChkSts) == length) {
            log_verbose() << "Received TrlrLampChkSts (" << ComConf_ComSignal_isTrlrLampChkSts_mrx << ")";
            const TrlrLampChkSts& rteValue = *static_cast<const TrlrLampChkSts*>(buffer);
            autosar::TrlrLampChkSts1WdSts2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrlrLampChkSts1WdSts2_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TrlrLampChkSts (" << ComConf_ComSignal_isTrlrLampChkSts_mrx << "). Got " << length << ", expected " << sizeof(TrlrLampChkSts);
        }
    }
    break;

    case ComConf_ComSignal_isTrlrPrsnt_mrx:
    {
        if (sizeof(TrlrPrsnt) == length) {
            log_verbose() << "Received TrlrPrsnt (" << ComConf_ComSignal_isTrlrPrsnt_mrx << ")";
            const TrlrPrsnt& rteValue = *static_cast<const TrlrPrsnt*>(buffer);
            autosar::TrlrPrsntSts1WdSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrlrPrsntSts1WdSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TrlrPrsnt (" << ComConf_ComSignal_isTrlrPrsnt_mrx << "). Got " << length << ", expected " << sizeof(TrlrPrsnt);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTwliBriRawGroup_mrx:
    {
        if (sizeof(TwliBriRawGroup) == length) {
            log_verbose() << "Received TwliBriRawGroup (" << ComConf_ComSignalGroup_igTwliBriRawGroup_mrx << ")";
            const TwliBriRawGroup& rteValue = *static_cast<const TwliBriRawGroup*>(buffer);
            autosar::TwliBriRaw_info::data_elem_type deValue;
            deValue.TwliBriRawQf = static_cast<decltype(deValue.TwliBriRawQf)>(rteValue.TwliBriRawQf);
            deValue.TwliBriRaw1 = static_cast<decltype(deValue.TwliBriRaw1)>(rteValue.TwliBriRaw);

            TwliBriRaw_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TwliBriRawGroup (" << ComConf_ComSignalGroup_igTwliBriRawGroup_mrx << "). Got " << length << ", expected " << sizeof(TwliBriRawGroup);
        }
    }
    break;

    case ComConf_ComSignal_isTwliBriSts_mrx:
    {
        if (sizeof(TwliBriSts) == length) {
            log_verbose() << "Received TwliBriSts (" << ComConf_ComSignal_isTwliBriSts_mrx << ")";
            const TwliBriSts& rteValue = *static_cast<const TwliBriSts*>(buffer);
            autosar::TwliBriSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TwliBriSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for TwliBriSts (" << ComConf_ComSignal_isTwliBriSts_mrx << "). Got " << length << ", expected " << sizeof(TwliBriSts);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igUkwnCptReqToInfoMgr_mrx:
    {
        if (sizeof(UkwnCptReqToInfoMgr) == length) {
            log_verbose() << "Received UkwnCptReqToInfoMgr (" << ComConf_ComSignalGroup_igUkwnCptReqToInfoMgr_mrx << ")";
            const UkwnCptReqToInfoMgr& rteValue = *static_cast<const UkwnCptReqToInfoMgr*>(buffer);
            autosar::UkwnCptReqToInfoMgr_info::data_elem_type deValue;
            deValue.Byte7 = static_cast<decltype(deValue.Byte7)>(rteValue.UkwnCptReqToInfoMgrByte7);
            deValue.Byte6 = static_cast<decltype(deValue.Byte6)>(rteValue.UkwnCptReqToInfoMgrByte6);
            deValue.Byte5 = static_cast<decltype(deValue.Byte5)>(rteValue.UkwnCptReqToInfoMgrByte5);
            deValue.Byte4 = static_cast<decltype(deValue.Byte4)>(rteValue.UkwnCptReqToInfoMgrByte4);
            deValue.Byte3 = static_cast<decltype(deValue.Byte3)>(rteValue.UkwnCptReqToInfoMgrByte3);
            deValue.Byte2 = static_cast<decltype(deValue.Byte2)>(rteValue.UkwnCptReqToInfoMgrByte2);
            deValue.Byte1 = static_cast<decltype(deValue.Byte1)>(rteValue.UkwnCptReqToInfoMgrByte1);
            deValue.Byte0 = static_cast<decltype(deValue.Byte0)>(rteValue.UkwnCptReqToInfoMgrByte0);

            UkwnCptReqToInfoMgr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for UkwnCptReqToInfoMgr (" << ComConf_ComSignalGroup_igUkwnCptReqToInfoMgr_mrx << "). Got " << length << ", expected " << sizeof(UkwnCptReqToInfoMgr);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igUkwnCptReqToSoundMgr_mrx:
    {
        if (sizeof(UkwnCptReqToSoundMgr) == length) {
            log_verbose() << "Received UkwnCptReqToSoundMgr (" << ComConf_ComSignalGroup_igUkwnCptReqToSoundMgr_mrx << ")";
            const UkwnCptReqToSoundMgr& rteValue = *static_cast<const UkwnCptReqToSoundMgr*>(buffer);
            autosar::UkwnCptReqToSoundMgr_info::data_elem_type deValue;
            deValue.Byte7 = static_cast<decltype(deValue.Byte7)>(rteValue.UkwnCptReqToSoundMgrByte7);
            deValue.Byte6 = static_cast<decltype(deValue.Byte6)>(rteValue.UkwnCptReqToSoundMgrByte6);
            deValue.Byte5 = static_cast<decltype(deValue.Byte5)>(rteValue.UkwnCptReqToSoundMgrByte5);
            deValue.Byte4 = static_cast<decltype(deValue.Byte4)>(rteValue.UkwnCptReqToSoundMgrByte4);
            deValue.Byte3 = static_cast<decltype(deValue.Byte3)>(rteValue.UkwnCptReqToSoundMgrByte3);
            deValue.Byte2 = static_cast<decltype(deValue.Byte2)>(rteValue.UkwnCptReqToSoundMgrByte2);
            deValue.Byte1 = static_cast<decltype(deValue.Byte1)>(rteValue.UkwnCptReqToSoundMgrByte1);
            deValue.Byte0 = static_cast<decltype(deValue.Byte0)>(rteValue.UkwnCptReqToSoundMgrByte0);

            UkwnCptReqToSoundMgr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for UkwnCptReqToSoundMgr (" << ComConf_ComSignalGroup_igUkwnCptReqToSoundMgr_mrx << "). Got " << length << ", expected " << sizeof(UkwnCptReqToSoundMgr);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igUsrSwtDispClimaReqForRowSec_mrx:
    {
        if (sizeof(UsrSwtDispClimaReqForRowSec) == length) {
            log_verbose() << "Received UsrSwtDispClimaReqForRowSec (" << ComConf_ComSignalGroup_igUsrSwtDispClimaReqForRowSec_mrx << ")";
            const UsrSwtDispClimaReqForRowSec& rteValue = *static_cast<const UsrSwtDispClimaReqForRowSec*>(buffer);
            autosar::UsrSwtDispClimaReqForRowSec_info::data_elem_type deValue;
            deValue.UsrSwtDispUpdClimaReqForTSpForRowSecLe = static_cast<decltype(deValue.UsrSwtDispUpdClimaReqForTSpForRowSecLe)>(rteValue.UsrSwtDispUpdClimaReqForTSpForRowSecLe);
            deValue.UsrSwtDispUpdClimaReqForTSpForRowSecRi = static_cast<decltype(deValue.UsrSwtDispUpdClimaReqForTSpForRowSecRi)>(rteValue.UsrSwtDispUpdClimaReqForTSpForRowSecRi);
            deValue.UsrSwtDispClimaReqForTSpForRowSecRi = static_cast<decltype(deValue.UsrSwtDispClimaReqForTSpForRowSecRi)>( toUnsignedFromRaw<5>(rteValue.UsrSwtDispClimaReqForTSpForRowSecRi) ) * 0.5 + 15.0;
            deValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe = static_cast<decltype(deValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe)>(rteValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe);
            deValue.UsrSwtDispReqForFanLvlForRowSec = static_cast<decltype(deValue.UsrSwtDispReqForFanLvlForRowSec)>(rteValue.UsrSwtDispReqForFanLvlForRowSec);
            deValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe = static_cast<decltype(deValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe)>(rteValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe);
            deValue.UsrSwtDispClimaReqForTSpForRowSecLe = static_cast<decltype(deValue.UsrSwtDispClimaReqForTSpForRowSecLe)>( toUnsignedFromRaw<5>(rteValue.UsrSwtDispClimaReqForTSpForRowSecLe) ) * 0.5 + 15.0;
            deValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi = static_cast<decltype(deValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi)>(rteValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi);
            deValue.UsrSwtDispClimaReqForTSpSpclForRowSecLe = static_cast<decltype(deValue.UsrSwtDispClimaReqForTSpSpclForRowSecLe)>(rteValue.UsrSwtDispClimaReqForTSpSpclForRowSecLe);
            deValue.UsrSwtDispClimaReqForTSpSpclForRowSecRi = static_cast<decltype(deValue.UsrSwtDispClimaReqForTSpSpclForRowSecRi)>(rteValue.UsrSwtDispClimaReqForTSpSpclForRowSecRi);
            deValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi = static_cast<decltype(deValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi)>(rteValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi);
            deValue.UsrSwtDispUpdReqForFanLvlForRowSec = static_cast<decltype(deValue.UsrSwtDispUpdReqForFanLvlForRowSec)>(rteValue.UsrSwtDispUpdReqForFanLvlForRowSec);

            UsrSwtDispClimaReqForRowSec_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for UsrSwtDispClimaReqForRowSec (" << ComConf_ComSignalGroup_igUsrSwtDispClimaReqForRowSec_mrx << "). Got " << length << ", expected " << sizeof(UsrSwtDispClimaReqForRowSec);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igUsrSwtDispReqForSecRowSeatVentn_mrx:
    {
        if (sizeof(UsrSwtDispReqForSecRowSeatVentn) == length) {
            log_verbose() << "Received UsrSwtDispReqForSecRowSeatVentn (" << ComConf_ComSignalGroup_igUsrSwtDispReqForSecRowSeatVentn_mrx << ")";
            const UsrSwtDispReqForSecRowSeatVentn& rteValue = *static_cast<const UsrSwtDispReqForSecRowSeatVentn*>(buffer);
            autosar::UsrSwtDispReqForSecRowSeatVentn_info::data_elem_type deValue;
            deValue.usrSwtDispUpdReqForSecRowSeatVentnRi = static_cast<decltype(deValue.usrSwtDispUpdReqForSecRowSeatVentnRi)>(rteValue.UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnRi);
            deValue.usrSwtDispUpdReqForSecRowSeatVentnLe = static_cast<decltype(deValue.usrSwtDispUpdReqForSecRowSeatVentnLe)>(rteValue.UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnLe);
            deValue.UsrSwtDispReqForSecRowSeatVentnLe = static_cast<decltype(deValue.UsrSwtDispReqForSecRowSeatVentnLe)>(rteValue.UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe);
            deValue.UsrSwtDispReqForSecRowSeatVentnRi = static_cast<decltype(deValue.UsrSwtDispReqForSecRowSeatVentnRi)>(rteValue.UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnRi);

            UsrSwtDispReqForSecRowSeatVentn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for UsrSwtDispReqForSecRowSeatVentn (" << ComConf_ComSignalGroup_igUsrSwtDispReqForSecRowSeatVentn_mrx << "). Got " << length << ", expected " << sizeof(UsrSwtDispReqForSecRowSeatVentn);
        }
    }
    break;

    case ComConf_ComSignal_isSeatHeatLvlReqLe_mrx:
    {
        if (sizeof(SeatHeatLvlReqLe) == length) {
            log_verbose() << "Received SeatHeatLvlReqLe (" << ComConf_ComSignal_isSeatHeatLvlReqLe_mrx << ")";
            const SeatHeatLvlReqLe& rteValue = *static_cast<const SeatHeatLvlReqLe*>(buffer);
            autosar::SeatHeatLvlReqLe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SeatHeatLvlReqLe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SeatHeatLvlReqLe (" << ComConf_ComSignal_isSeatHeatLvlReqLe_mrx << "). Got " << length << ", expected " << sizeof(SeatHeatLvlReqLe);
        }
    }
    break;

    case ComConf_ComSignal_isSeatHeatLvlReqRi_mrx:
    {
        if (sizeof(SeatHeatLvlReqRi) == length) {
            log_verbose() << "Received SeatHeatLvlReqRi (" << ComConf_ComSignal_isSeatHeatLvlReqRi_mrx << ")";
            const SeatHeatLvlReqRi& rteValue = *static_cast<const SeatHeatLvlReqRi*>(buffer);
            autosar::SeatHeatLvlReqRi_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SeatHeatLvlReqRi_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for SeatHeatLvlReqRi (" << ComConf_ComSignal_isSeatHeatLvlReqRi_mrx << "). Got " << length << ", expected " << sizeof(SeatHeatLvlReqRi);
        }
    }
    break;

    case ComConf_ComSignal_isVehActvMsgToDrvr_mrx:
    {
        if (sizeof(VehActvMsgToDrvr) == length) {
            log_verbose() << "Received VehActvMsgToDrvr (" << ComConf_ComSignal_isVehActvMsgToDrvr_mrx << ")";
            const VehActvMsgToDrvr& rteValue = *static_cast<const VehActvMsgToDrvr*>(buffer);
            autosar::VehActvMsgToDrvr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            VehActvMsgToDrvr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehActvMsgToDrvr (" << ComConf_ComSignal_isVehActvMsgToDrvr_mrx << "). Got " << length << ", expected " << sizeof(VehActvMsgToDrvr);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehBattUGroup_mrx:
    {
        if (sizeof(VehBattUGroup) == length) {
            log_verbose() << "Received VehBattUGroup (" << ComConf_ComSignalGroup_igVehBattUGroup_mrx << ")";
            const VehBattUGroup& rteValue = *static_cast<const VehBattUGroup*>(buffer);
            autosar::VehBattU_info::data_elem_type deValue;
            deValue.SysU = static_cast<decltype(deValue.SysU)>( toUnsignedFromRaw<8>(rteValue.VehBattUSysU) ) * 0.1 + 0.0;
            deValue.SysUQf = static_cast<decltype(deValue.SysUQf)>(rteValue.VehBattUSysUQf);

            VehBattU_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehBattUGroup (" << ComConf_ComSignalGroup_igVehBattUGroup_mrx << "). Got " << length << ", expected " << sizeof(VehBattUGroup);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehCfgPrm_mrx:
    {
        if (sizeof(VehCfgPrm) == length) {
            log_verbose() << "Received VehCfgPrm (" << ComConf_ComSignalGroup_igVehCfgPrm_mrx << ")";
            const VehCfgPrm& rteValue = *static_cast<const VehCfgPrm*>(buffer);
            autosar::VehCfgPrm_info::data_elem_type deValue;
            deValue.CCPBytePosn2 = static_cast<decltype(deValue.CCPBytePosn2)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal1) );
            deValue.CCPBytePosn3 = static_cast<decltype(deValue.CCPBytePosn3)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal2) );
            deValue.CCPBytePosn4 = static_cast<decltype(deValue.CCPBytePosn4)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal3) );
            deValue.CCPBytePosn5 = static_cast<decltype(deValue.CCPBytePosn5)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal4) );
            deValue.CCPBytePosn6 = static_cast<decltype(deValue.CCPBytePosn6)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal5) );
            deValue.CCPBytePosn7 = static_cast<decltype(deValue.CCPBytePosn7)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal6) );
            deValue.CCPBytePosn8 = static_cast<decltype(deValue.CCPBytePosn8)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal7) );
            deValue.BlkIDBytePosn1 = static_cast<decltype(deValue.BlkIDBytePosn1)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmBlk) );

            VehCfgPrm_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehCfgPrm (" << ComConf_ComSignalGroup_igVehCfgPrm_mrx << "). Got " << length << ", expected " << sizeof(VehCfgPrm);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehMGroup_mrx:
    {
        if (sizeof(VehMGroup) == length) {
            log_verbose() << "Received VehMGroup (" << ComConf_ComSignalGroup_igVehMGroup_mrx << ")";
            const VehMGroup& rteValue = *static_cast<const VehMGroup*>(buffer);
            autosar::VehMNom_info::data_elem_type deValue;
            deValue.TrlrM_ = static_cast<decltype(deValue.TrlrM_)>(rteValue.VehMNomTrlrM);
            deValue.VehMQly = static_cast<decltype(deValue.VehMQly)>(rteValue.VehMQly);
            deValue.VehM = static_cast<decltype(deValue.VehM)>( toUnsignedFromRaw<14>(rteValue.VehM) );

            VehMNom_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehMGroup (" << ComConf_ComSignalGroup_igVehMGroup_mrx << "). Got " << length << ", expected " << sizeof(VehMGroup);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehModMngtGlbSafe1_mrx:
    {
        if (sizeof(VehModMngtGlbSafe1) == length) {
            log_verbose() << "Received VehModMngtGlbSafe1 (" << ComConf_ComSignalGroup_igVehModMngtGlbSafe1_mrx << ")";
            const VehModMngtGlbSafe1& rteValue = *static_cast<const VehModMngtGlbSafe1*>(buffer);
            autosar::VehModMngtGlbSafe1_info::data_elem_type deValue;
            deValue.UsgModSts = static_cast<decltype(deValue.UsgModSts)>(rteValue.UsgModSts);
            deValue.EgyLvlElecMai = static_cast<decltype(deValue.EgyLvlElecMai)>(rteValue.EgyLvlElecMai);
            deValue.CarModSubtypWdCarModSubtyp = static_cast<decltype(deValue.CarModSubtypWdCarModSubtyp)>(rteValue.CarModSubtypWdCarModSubtyp);
            deValue.EgyLvlElecSubtyp = static_cast<decltype(deValue.EgyLvlElecSubtyp)>(rteValue.EgyLvlElecSubtyp);
            deValue.FltEgyCnsWdSts = static_cast<decltype(deValue.FltEgyCnsWdSts)>(rteValue.FltEgyCnsWdSts);
            deValue.PwrLvlElecSubtyp = static_cast<decltype(deValue.PwrLvlElecSubtyp)>(rteValue.PwrLvlElecSubtyp);
            deValue.Chks = static_cast<decltype(deValue.Chks)>(rteValue.VehModMngtGlbSafe1Chks);
            deValue.Cntr = static_cast<decltype(deValue.Cntr)>(rteValue.VehModMngtGlbSafe1Cntr);
            deValue.CarModSts1_ = static_cast<decltype(deValue.CarModSts1_)>(rteValue.CarModSts1);
            deValue.PwrLvlElecMai = static_cast<decltype(deValue.PwrLvlElecMai)>(rteValue.PwrLvlElecMai);

            VehModMngtGlbSafe1_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehModMngtGlbSafe1 (" << ComConf_ComSignalGroup_igVehModMngtGlbSafe1_mrx << "). Got " << length << ", expected " << sizeof(VehModMngtGlbSafe1);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehMtnStSafeGroup_mrx:
    {
        if (sizeof(VehMtnStSafeGroup) == length) {
            log_verbose() << "Received VehMtnStSafeGroup (" << ComConf_ComSignalGroup_igVehMtnStSafeGroup_mrx << ")";
            const VehMtnStSafeGroup& rteValue = *static_cast<const VehMtnStSafeGroup*>(buffer);
            autosar::VehMtnStSafe_info::data_elem_type deValue;
            deValue.VehMtnStCntr = static_cast<decltype(deValue.VehMtnStCntr)>(rteValue.VehMtnStCntr);
            deValue.VehMtnStChks = static_cast<decltype(deValue.VehMtnStChks)>(rteValue.VehMtnStChks);
            deValue.VehMtnSt = static_cast<decltype(deValue.VehMtnSt)>(rteValue.VehMtnStSafe);

            VehMtnStSafe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehMtnStSafeGroup (" << ComConf_ComSignalGroup_igVehMtnStSafeGroup_mrx << "). Got " << length << ", expected " << sizeof(VehMtnStSafeGroup);
        }
    }
    break;

    case ComConf_ComSignal_isAudWarn_mrx:
    {
        if (sizeof(AudWarn) == length) {
            log_verbose() << "Received AudWarn (" << ComConf_ComSignal_isAudWarn_mrx << ")";
            const AudWarn& rteValue = *static_cast<const AudWarn*>(buffer);
            autosar::AudWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AudWarn_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for AudWarn (" << ComConf_ComSignal_isAudWarn_mrx << "). Got " << length << ", expected " << sizeof(AudWarn);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehSpdAvgIndcd_mrx:
    {
        if (sizeof(VehSpdAvgIndcd) == length) {
            log_verbose() << "Received VehSpdAvgIndcd (" << ComConf_ComSignalGroup_igVehSpdAvgIndcd_mrx << ")";
            const VehSpdAvgIndcd& rteValue = *static_cast<const VehSpdAvgIndcd*>(buffer);
            autosar::VehSpdAvgIndcd_info::data_elem_type deValue;
            deValue.VehSpdIndcd = static_cast<decltype(deValue.VehSpdIndcd)>(rteValue.VehSpdAvg);
            deValue.VeSpdIndcdUnit = static_cast<decltype(deValue.VeSpdIndcdUnit)>(rteValue.VehSpdAvgUnit);

            VehSpdAvgIndcd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehSpdAvgIndcd (" << ComConf_ComSignalGroup_igVehSpdAvgIndcd_mrx << "). Got " << length << ", expected " << sizeof(VehSpdAvgIndcd);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehSpdIndcd_mrx:
    {
        if (sizeof(VehSpdIndcd) == length) {
            log_verbose() << "Received VehSpdIndcd (" << ComConf_ComSignalGroup_igVehSpdIndcd_mrx << ")";
            const VehSpdIndcd& rteValue = *static_cast<const VehSpdIndcd*>(buffer);
            autosar::VehSpdIndcd_info::data_elem_type deValue;
            deValue.VehSpdIndcd = static_cast<decltype(deValue.VehSpdIndcd)>(rteValue.VehSpdIndcdVal);
            deValue.VeSpdIndcdUnit = static_cast<decltype(deValue.VeSpdIndcdUnit)>(rteValue.VehSpdIndcdUnit);

            VehSpdIndcd_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehSpdIndcd (" << ComConf_ComSignalGroup_igVehSpdIndcd_mrx << "). Got " << length << ", expected " << sizeof(VehSpdIndcd);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehSpdLgtSafeGroup_mrx:
    {
        if (sizeof(VehSpdLgtSafeGroup) == length) {
            log_verbose() << "Received VehSpdLgtSafeGroup (" << ComConf_ComSignalGroup_igVehSpdLgtSafeGroup_mrx << ")";
            const VehSpdLgtSafeGroup& rteValue = *static_cast<const VehSpdLgtSafeGroup*>(buffer);
            autosar::VehSpdLgtSafe_info::data_elem_type deValue;
            deValue.VehSpdLgtCntr = static_cast<decltype(deValue.VehSpdLgtCntr)>(rteValue.VehSpdLgtSafeCntr);
            deValue.VehSpdLgtChks = static_cast<decltype(deValue.VehSpdLgtChks)>(rteValue.VehSpdLgtSafeChks);
            deValue.VehSpdLgt = static_cast<decltype(deValue.VehSpdLgt)>( toUnsignedFromRaw<15>(rteValue.VehSpdLgtSafe) ) * 0.00391 + 0.0;
            deValue.VehSpdLgtQf = static_cast<decltype(deValue.VehSpdLgtQf)>(rteValue.VehSpdLgtSafeQf);

            VehSpdLgtSafe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VehSpdLgtSafeGroup (" << ComConf_ComSignalGroup_igVehSpdLgtSafeGroup_mrx << "). Got " << length << ", expected " << sizeof(VehSpdLgtSafeGroup);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVin_mrx:
    {
        if (sizeof(Vin) == length) {
            log_verbose() << "Received Vin (" << ComConf_ComSignalGroup_igVin_mrx << ")";
            const Vin& rteValue = *static_cast<const Vin*>(buffer);
            autosar::Vin_info::data_elem_type deValue;
            deValue.BlockNr = static_cast<decltype(deValue.BlockNr)>( toUnsignedFromRaw<8>(rteValue.VinBlk) );
            deValue.VINSignalPos1 = static_cast<decltype(deValue.VINSignalPos1)>( toUnsignedFromRaw<8>(rteValue.VinPosn1) );
            deValue.VINSignalPos2 = static_cast<decltype(deValue.VINSignalPos2)>( toUnsignedFromRaw<8>(rteValue.VinPosn2) );
            deValue.VINSignalPos3 = static_cast<decltype(deValue.VINSignalPos3)>( toUnsignedFromRaw<8>(rteValue.VinPosn3) );
            deValue.VINSignalPos4 = static_cast<decltype(deValue.VINSignalPos4)>( toUnsignedFromRaw<8>(rteValue.VinPosn4) );
            deValue.VINSignalPos5 = static_cast<decltype(deValue.VINSignalPos5)>( toUnsignedFromRaw<8>(rteValue.VinPosn5) );
            deValue.VINSignalPos6 = static_cast<decltype(deValue.VINSignalPos6)>( toUnsignedFromRaw<8>(rteValue.VinPosn6) );
            deValue.VINSignalPos7 = static_cast<decltype(deValue.VINSignalPos7)>( toUnsignedFromRaw<8>(rteValue.VinPosn7) );

            Vin_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for Vin (" << ComConf_ComSignalGroup_igVin_mrx << "). Got " << length << ", expected " << sizeof(Vin);
        }
    }
    break;

    case ComConf_ComSignal_isVisnAgWideSts_mrx:
    {
        if (sizeof(VisnAgWideSts) == length) {
            log_verbose() << "Received VisnAgWideSts (" << ComConf_ComSignal_isVisnAgWideSts_mrx << ")";
            const VisnAgWideSts& rteValue = *static_cast<const VisnAgWideSts*>(buffer);
            autosar::VisnAgWideSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            VisnAgWideSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VisnAgWideSts (" << ComConf_ComSignal_isVisnAgWideSts_mrx << "). Got " << length << ", expected " << sizeof(VisnAgWideSts);
        }
    }
    break;

    case ComConf_ComSignal_isVisnImgAgWideInUse_mrx:
    {
        if (sizeof(VisnImgAgWideInUse) == length) {
            log_verbose() << "Received VisnImgAgWideInUse (" << ComConf_ComSignal_isVisnImgAgWideInUse_mrx << ")";
            const VisnImgAgWideInUse& rteValue = *static_cast<const VisnImgAgWideInUse*>(buffer);
            autosar::VisnImgAgWideInUse_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            VisnImgAgWideInUse_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for VisnImgAgWideInUse (" << ComConf_ComSignal_isVisnImgAgWideInUse_mrx << "). Got " << length << ", expected " << sizeof(VisnImgAgWideInUse);
        }
    }
    break;

    case ComConf_ComSignal_isWhlCircum_mrx:
    {
        if (sizeof(WhlCircum) == length) {
            log_verbose() << "Received WhlCircum (" << ComConf_ComSignal_isWhlCircum_mrx << ")";
            const WhlCircum& rteValue = *static_cast<const WhlCircum*>(buffer);
            autosar::WhlCircum_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WhlCircum_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WhlCircum (" << ComConf_ComSignal_isWhlCircum_mrx << "). Got " << length << ", expected " << sizeof(WhlCircum);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igWhlMotSysTqEstGroup_mrx:
    {
        if (sizeof(WhlMotSysTqEstGroup) == length) {
            log_verbose() << "Received WhlMotSysTqEstGroup (" << ComConf_ComSignalGroup_igWhlMotSysTqEstGroup_mrx << ")";
            const WhlMotSysTqEstGroup& rteValue = *static_cast<const WhlMotSysTqEstGroup*>(buffer);
            autosar::WhlMotSysTqEst_info::data_elem_type deValue;
            deValue.SpdLimn = static_cast<decltype(deValue.SpdLimn)>( toUnsignedFromRaw<8>(rteValue.WhlMotSysTqEstSpdLimn) ) * 10.0 + 0.0;
            deValue.TqActQlyFac = static_cast<decltype(deValue.TqActQlyFac)>(rteValue.WhlMotSysTqEstQlyFac);
            deValue.TqActCntr = static_cast<decltype(deValue.TqActCntr)>(rteValue.WhlMotSysTqEstCntr);
            deValue.TqAct = static_cast<decltype(deValue.TqAct)>( toUnsignedFromRaw<12>(rteValue.WhlMotSysTqEst) ) * 4.0 + -8188.0;
            deValue.TqActChks = static_cast<decltype(deValue.TqActChks)>(rteValue.WhlMotSysTqEstChks);

            WhlMotSysTqEst_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WhlMotSysTqEstGroup (" << ComConf_ComSignalGroup_igWhlMotSysTqEstGroup_mrx << "). Got " << length << ", expected " << sizeof(WhlMotSysTqEstGroup);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igWhlRotToothCntr_mrx:
    {
        if (sizeof(WhlRotToothCntr) == length) {
            log_verbose() << "Received WhlRotToothCntr (" << ComConf_ComSignalGroup_igWhlRotToothCntr_mrx << ")";
            const WhlRotToothCntr& rteValue = *static_cast<const WhlRotToothCntr*>(buffer);
            autosar::WhlRotToothCntr_info::data_elem_type deValue;
            deValue.WhlRotToothCntrReLe = static_cast<decltype(deValue.WhlRotToothCntrReLe)>( toUnsignedFromRaw<8>(rteValue.WhlRotToothCntrReLe) );
            deValue.WhlRotToothCntrFrntRi = static_cast<decltype(deValue.WhlRotToothCntrFrntRi)>( toUnsignedFromRaw<8>(rteValue.WhlRotToothCntrFrntRi) );
            deValue.WhlRotToothCntrReRi = static_cast<decltype(deValue.WhlRotToothCntrReRi)>( toUnsignedFromRaw<8>(rteValue.WhlRotToothCntrReRi) );
            deValue.WhlRotToothCntrFrntLe = static_cast<decltype(deValue.WhlRotToothCntrFrntLe)>( toUnsignedFromRaw<8>(rteValue.WhlRotToothCntrFrntLe) );

            WhlRotToothCntr_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WhlRotToothCntr (" << ComConf_ComSignalGroup_igWhlRotToothCntr_mrx << "). Got " << length << ", expected " << sizeof(WhlRotToothCntr);
        }
    }
    break;

    case ComConf_ComSignal_isWinPosnStsDrv_mrx:
    {
        if (sizeof(WinPosnStsDrv) == length) {
            log_verbose() << "Received WinPosnStsDrv (" << ComConf_ComSignal_isWinPosnStsDrv_mrx << ")";
            const WinPosnStsDrv& rteValue = *static_cast<const WinPosnStsDrv*>(buffer);
            autosar::WinPosnStsAtDrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WinPosnStsAtDrv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WinPosnStsDrv (" << ComConf_ComSignal_isWinPosnStsDrv_mrx << "). Got " << length << ", expected " << sizeof(WinPosnStsDrv);
        }
    }
    break;

    case ComConf_ComSignal_isWinPosnStsAtDrvrRe_mrx:
    {
        if (sizeof(WinPosnStsAtDrvrRe) == length) {
            log_verbose() << "Received WinPosnStsAtDrvrRe (" << ComConf_ComSignal_isWinPosnStsAtDrvrRe_mrx << ")";
            const WinPosnStsAtDrvrRe& rteValue = *static_cast<const WinPosnStsAtDrvrRe*>(buffer);
            autosar::WinPosnStsAtDrvrRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WinPosnStsAtDrvrRe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WinPosnStsAtDrvrRe (" << ComConf_ComSignal_isWinPosnStsAtDrvrRe_mrx << "). Got " << length << ", expected " << sizeof(WinPosnStsAtDrvrRe);
        }
    }
    break;

    case ComConf_ComSignal_isWinPosnStsPass_mrx:
    {
        if (sizeof(WinPosnStsPass) == length) {
            log_verbose() << "Received WinPosnStsPass (" << ComConf_ComSignal_isWinPosnStsPass_mrx << ")";
            const WinPosnStsPass& rteValue = *static_cast<const WinPosnStsPass*>(buffer);
            autosar::WinPosnStsAtPass_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WinPosnStsAtPass_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WinPosnStsPass (" << ComConf_ComSignal_isWinPosnStsPass_mrx << "). Got " << length << ", expected " << sizeof(WinPosnStsPass);
        }
    }
    break;

    case ComConf_ComSignal_isWinPosnStsRePass_mrx:
    {
        if (sizeof(WinPosnStsRePass) == length) {
            log_verbose() << "Received WinPosnStsRePass (" << ComConf_ComSignal_isWinPosnStsRePass_mrx << ")";
            const WinPosnStsRePass& rteValue = *static_cast<const WinPosnStsRePass*>(buffer);
            autosar::WinPosnStsAtPassRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WinPosnStsAtPassRe_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WinPosnStsRePass (" << ComConf_ComSignal_isWinPosnStsRePass_mrx << "). Got " << length << ", expected " << sizeof(WinPosnStsRePass);
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igWipgInfo_mrx:
    {
        if (sizeof(WipgInfo) == length) {
            log_verbose() << "Received WipgInfo (" << ComConf_ComSignalGroup_igWipgInfo_mrx << ")";
            const WipgInfo& rteValue = *static_cast<const WipgInfo*>(buffer);
            autosar::WipgInfo_info::data_elem_type deValue;
            deValue.WiprActv = static_cast<decltype(deValue.WiprActv)>(rteValue.WipgInfoWiprActv);
            deValue.WiprInWipgAr = static_cast<decltype(deValue.WiprInWipgAr)>(rteValue.WipgInfoWiprInWipgAr);
            deValue.WipgSpdInfo_ = static_cast<decltype(deValue.WipgSpdInfo_)>(rteValue.WipgInfoWipgSpdInfo);

            WipgInfo_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WipgInfo (" << ComConf_ComSignalGroup_igWipgInfo_mrx << "). Got " << length << ", expected " << sizeof(WipgInfo);
        }
    }
    break;

    case ComConf_ComSignal_isWiprInPosnForSrv_mrx:
    {
        if (sizeof(WiprInPosnForSrv) == length) {
            log_verbose() << "Received WiprInPosnForSrv (" << ComConf_ComSignal_isWiprInPosnForSrv_mrx << ")";
            const WiprInPosnForSrv& rteValue = *static_cast<const WiprInPosnForSrv*>(buffer);
            autosar::WiprInPosnForSrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WiprInPosnForSrv_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WiprInPosnForSrv (" << ComConf_ComSignal_isWiprInPosnForSrv_mrx << "). Got " << length << ", expected " << sizeof(WiprInPosnForSrv);
        }
    }
    break;

    case ComConf_ComSignal_isWshrFldSts_mrx:
    {
        if (sizeof(WshrFldSts) == length) {
            log_verbose() << "Received WshrFldSts (" << ComConf_ComSignal_isWshrFldSts_mrx << ")";
            const WshrFldSts& rteValue = *static_cast<const WshrFldSts*>(buffer);
            autosar::WshrFldSts1WdElmHMI_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WshrFldSts1WdElmHMI_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for WshrFldSts (" << ComConf_ComSignal_isWshrFldSts_mrx << "). Got " << length << ", expected " << sizeof(WshrFldSts);
        }
    }
    break;

    case ComConf_ComSignal_isiTPMSCalPsbl_mrx:
    {
        if (sizeof(iTPMSCalPsbl) == length) {
            log_verbose() << "Received iTPMSCalPsbl (" << ComConf_ComSignal_isiTPMSCalPsbl_mrx << ")";
            const iTPMSCalPsbl& rteValue = *static_cast<const iTPMSCalPsbl*>(buffer);
            autosar::iTPMSCalPsbl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            iTPMSCalPsbl_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for iTPMSCalPsbl (" << ComConf_ComSignal_isiTPMSCalPsbl_mrx << "). Got " << length << ", expected " << sizeof(iTPMSCalPsbl);
        }
    }
    break;

    case ComConf_ComSignal_isiTPMSCalSts_mrx:
    {
        if (sizeof(iTPMSCalSts) == length) {
            log_verbose() << "Received iTPMSCalSts (" << ComConf_ComSignal_isiTPMSCalSts_mrx << ")";
            const iTPMSCalSts& rteValue = *static_cast<const iTPMSCalSts*>(buffer);
            autosar::iTPMSCalSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            iTPMSCalSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for iTPMSCalSts (" << ComConf_ComSignal_isiTPMSCalSts_mrx << "). Got " << length << ", expected " << sizeof(iTPMSCalSts);
        }
    }
    break;

    case ComConf_ComSignal_isiTPMSTirePMSts_mrx:
    {
        if (sizeof(iTPMSTirePMSts) == length) {
            log_verbose() << "Received iTPMSTirePMSts (" << ComConf_ComSignal_isiTPMSTirePMSts_mrx << ")";
            const iTPMSTirePMSts& rteValue = *static_cast<const iTPMSTirePMSts*>(buffer);
            autosar::iTPMSTirePMSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            iTPMSTirePMSts_de->inject(deValue);
        } else {
            log_error() << "Wrong buffer size received for iTPMSTirePMSts (" << ComConf_ComSignal_isiTPMSTirePMSts_mrx << "). Got " << length << ", expected " << sizeof(iTPMSTirePMSts);
        }
    }
    break;

