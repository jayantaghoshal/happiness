/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

// clang-format off
/*!
 * \file
 * C++ code generator for AUTOSAR v1.0
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved. Delphi Confidential
 * Source: databases/SPA2210_IHUVOLVO27_161214_AR403_UnFlattened_Splitted_WithSparePNC_Swc.arxml
 */

    case SignalGroup|ComConf_ComSignalGroup_igADataRawSafe_mrx:
    {
        if (sizeof(ADataRawSafe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ADataRawSafe (%u)", ComConf_ComSignalGroup_igADataRawSafe_mrx);
            const ADataRawSafe& rteValue = *static_cast<const ADataRawSafe*>(buffer);
            autosar::ADataRawSafe_info::data_elem_type deValue;
            deValue.ALat = static_cast<decltype(deValue.ALat)>( toSignedFromRaw<15>(rteValue.ALat1) ) * 0.0085 + 0.0;
            deValue.ALat1Qf = static_cast<decltype(deValue.ALat1Qf)>(rteValue.ALat1Qf1);
            deValue.ALgt = static_cast<decltype(deValue.ALgt)>( toSignedFromRaw<15>(rteValue.ALgt1) ) * 0.0085 + 0.0;
            deValue.ALgt1Qf = static_cast<decltype(deValue.ALgt1Qf)>(rteValue.ALgt1Qf1);
            deValue.AVert = static_cast<decltype(deValue.AVert)>( toSignedFromRaw<15>(rteValue.AVert2) ) * 0.0085 + 0.0;
            deValue.AVertQf = static_cast<decltype(deValue.AVertQf)>(rteValue.AVert2Qf1);
            deValue.Chks = static_cast<decltype(deValue.Chks)>(rteValue.ADataRawSafeChks);
            deValue.Cntr = static_cast<decltype(deValue.Cntr)>(rteValue.ADataRawSafeCntr);

            ADataRawSafe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ADataRawSafe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igADataRawSafe_mrx, length, static_cast<unsigned long>(sizeof(ADataRawSafe)));
        }
    }
    break;

    case ComConf_ComSignal_isAccAutResuWarnReq_mrx:
    {
        if (sizeof(AccAutResuWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AccAutResuWarnReq (%u)", ComConf_ComSignal_isAccAutResuWarnReq_mrx);
            const AccAutResuWarnReq& rteValue = *static_cast<const AccAutResuWarnReq*>(buffer);
            autosar::AccAutResuWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AccAutResuWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AccAutResuWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isAccAutResuWarnReq_mrx, length, static_cast<unsigned long>(sizeof(AccAutResuWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isAccSts_mrx:
    {
        if (sizeof(AccSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AccSts (%u)", ComConf_ComSignal_isAccSts_mrx);
            const AccSts& rteValue = *static_cast<const AccSts*>(buffer);
            autosar::AccSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AccSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AccSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isAccSts_mrx, length, static_cast<unsigned long>(sizeof(AccSts)));
        }
    }
    break;

    case ComConf_ComSignal_isActvOfHorn_mrx:
    {
        if (sizeof(ActvOfHorn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ActvOfHorn (%u)", ComConf_ComSignal_isActvOfHorn_mrx);
            const ActvOfHorn& rteValue = *static_cast<const ActvOfHorn*>(buffer);
            autosar::ActvOfHorn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ActvOfHorn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ActvOfHorn (%u). Got %zu , expected %lu", ComConf_ComSignal_isActvOfHorn_mrx, length, static_cast<unsigned long>(sizeof(ActvOfHorn)));
        }
    }
    break;

    case ComConf_ComSignal_isAdjSpdLimnSts_mrx:
    {
        if (sizeof(AdjSpdLimnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AdjSpdLimnSts (%u)", ComConf_ComSignal_isAdjSpdLimnSts_mrx);
            const AdjSpdLimnSts& rteValue = *static_cast<const AdjSpdLimnSts*>(buffer);
            autosar::AdjSpdLimnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AdjSpdLimnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AdjSpdLimnSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isAdjSpdLimnSts_mrx, length, static_cast<unsigned long>(sizeof(AdjSpdLimnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isAdjSpdLimnWarn_mrx:
    {
        if (sizeof(AdjSpdLimnWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AdjSpdLimnWarn (%u)", ComConf_ComSignal_isAdjSpdLimnWarn_mrx);
            const AdjSpdLimnWarn& rteValue = *static_cast<const AdjSpdLimnWarn*>(buffer);
            autosar::AdjSpdLimnWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AdjSpdLimnWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AdjSpdLimnWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isAdjSpdLimnWarn_mrx, length, static_cast<unsigned long>(sizeof(AdjSpdLimnWarn)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igAgDataRawSafe_mrx:
    {
        if (sizeof(AgDataRawSafe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AgDataRawSafe (%u)", ComConf_ComSignalGroup_igAgDataRawSafe_mrx);
            const AgDataRawSafe& rteValue = *static_cast<const AgDataRawSafe*>(buffer);
            autosar::AgDataRawSafe_info::data_elem_type deValue;
            deValue.Chks = static_cast<decltype(deValue.Chks)>(rteValue.AgDataRawSafeChks);
            deValue.Cntr = static_cast<decltype(deValue.Cntr)>(rteValue.AgDataRawSafeCntr);
            deValue.RollRate = static_cast<decltype(deValue.RollRate)>( toSignedFromRaw<16>(rteValue.RollRate1) ) * 0.000244140625 + 0.0;
            deValue.RollRateQf = static_cast<decltype(deValue.RollRateQf)>(rteValue.RollRate1Qf1);
            deValue.YawRate = static_cast<decltype(deValue.YawRate)>( toSignedFromRaw<16>(rteValue.YawRate1) ) * 0.000244140625 + 0.0;
            deValue.YawRateQf = static_cast<decltype(deValue.YawRateQf)>(rteValue.YawRate1Qf1);

            AgDataRawSafe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AgDataRawSafe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igAgDataRawSafe_mrx, length, static_cast<unsigned long>(sizeof(AgDataRawSafe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igAmbTIndcdWithUnit_mrx:
    {
        if (sizeof(AmbTIndcdWithUnit) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AmbTIndcdWithUnit (%u)", ComConf_ComSignalGroup_igAmbTIndcdWithUnit_mrx);
            const AmbTIndcdWithUnit& rteValue = *static_cast<const AmbTIndcdWithUnit*>(buffer);
            autosar::AmbTIndcdWithUnit_info::data_elem_type deValue;
            deValue.AmbTIndcd = static_cast<decltype(deValue.AmbTIndcd)>( toUnsignedFromRaw<12>(rteValue.AmbTIndcd) ) * 0.1 + -100.0;
            deValue.AmbTIndcdUnit_ = static_cast<decltype(deValue.AmbTIndcdUnit_)>(rteValue.AmbTIndcdUnit);
            deValue.QF = static_cast<decltype(deValue.QF)>(rteValue.AmbTIndcdQf);

            AmbTIndcdWithUnit_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AmbTIndcdWithUnit (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igAmbTIndcdWithUnit_mrx, length, static_cast<unsigned long>(sizeof(AmbTIndcdWithUnit)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igAmbTRaw_mrx:
    {
        if (sizeof(AmbTRaw) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AmbTRaw (%u)", ComConf_ComSignalGroup_igAmbTRaw_mrx);
            const AmbTRaw& rteValue = *static_cast<const AmbTRaw*>(buffer);
            autosar::AmbTRaw_info::data_elem_type deValue;
            deValue.AmbTVal = static_cast<decltype(deValue.AmbTVal)>( toUnsignedFromRaw<11>(rteValue.AmbTRawVal) ) * 0.1 + -70.0;
            deValue.Qly = static_cast<decltype(deValue.Qly)>(rteValue.AmbTRawQly);

            AmbTRaw_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AmbTRaw (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igAmbTRaw_mrx, length, static_cast<unsigned long>(sizeof(AmbTRaw)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igAsyLaneChg_mrx:
    {
        if (sizeof(AsyLaneChg) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AsyLaneChg (%u)", ComConf_ComSignalGroup_igAsyLaneChg_mrx);
            const AsyLaneChg& rteValue = *static_cast<const AsyLaneChg*>(buffer);
            autosar::AsyLaneChg_info::data_elem_type deValue;
            deValue.Psbl = static_cast<decltype(deValue.Psbl)>(rteValue.AsyLaneChgPsbl);
            deValue.Sts = static_cast<decltype(deValue.Sts)>(rteValue.AsyLaneChgSts);
            deValue.Typ = static_cast<decltype(deValue.Typ)>(rteValue.AsyLaneChgTyp);

            AsyLaneChg_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AsyLaneChg (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igAsyLaneChg_mrx, length, static_cast<unsigned long>(sizeof(AsyLaneChg)));
        }
    }
    break;

    case ComConf_ComSignal_isAsySteerApplyRqrd_mrx:
    {
        if (sizeof(AsySteerApplyRqrd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AsySteerApplyRqrd (%u)", ComConf_ComSignal_isAsySteerApplyRqrd_mrx);
            const AsySteerApplyRqrd& rteValue = *static_cast<const AsySteerApplyRqrd*>(buffer);
            autosar::AsySteerApplyRqrd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AsySteerApplyRqrd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AsySteerApplyRqrd (%u). Got %zu , expected %lu", ComConf_ComSignal_isAsySteerApplyRqrd_mrx, length, static_cast<unsigned long>(sizeof(AsySteerApplyRqrd)));
        }
    }
    break;

    case ComConf_ComSignal_isAudMsgReq_mrx:
    {
        if (sizeof(AudMsgReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AudMsgReq (%u)", ComConf_ComSignal_isAudMsgReq_mrx);
            const AudMsgReq& rteValue = *static_cast<const AudMsgReq*>(buffer);
            autosar::AudMsgReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AudMsgReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AudMsgReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isAudMsgReq_mrx, length, static_cast<unsigned long>(sizeof(AudMsgReq)));
        }
    }
    break;

    case ComConf_ComSignal_isBackCntrForMissCom_mrx:
    {
        if (sizeof(BackCntrForMissCom) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BackCntrForMissCom (%u)", ComConf_ComSignal_isBackCntrForMissCom_mrx);
            const BackCntrForMissCom& rteValue = *static_cast<const BackCntrForMissCom*>(buffer);
            autosar::BackCntrForMissCom_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BackCntrForMissCom_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BackCntrForMissCom (%u). Got %zu , expected %lu", ComConf_ComSignal_isBackCntrForMissCom_mrx, length, static_cast<unsigned long>(sizeof(BackCntrForMissCom)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtDrvr_mrx:
    {
        if (sizeof(BltLockStAtDrvr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltLockStAtDrvr (%u)", ComConf_ComSignalGroup_igBltLockStAtDrvr_mrx);
            const BltLockStAtDrvr& rteValue = *static_cast<const BltLockStAtDrvr*>(buffer);
            autosar::BltLockStAtDrvr_info::data_elem_type deValue;
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtDrvrForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtDrvrForDevErrSts2);

            BltLockStAtDrvr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltLockStAtDrvr (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBltLockStAtDrvr_mrx, length, static_cast<unsigned long>(sizeof(BltLockStAtDrvr)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtPass_mrx:
    {
        if (sizeof(BltLockStAtPass) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltLockStAtPass (%u)", ComConf_ComSignalGroup_igBltLockStAtPass_mrx);
            const BltLockStAtPass& rteValue = *static_cast<const BltLockStAtPass*>(buffer);
            autosar::BltLockStAtPass_info::data_elem_type deValue;
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtPassForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtPassForBltLockSts);

            BltLockStAtPass_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltLockStAtPass (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBltLockStAtPass_mrx, length, static_cast<unsigned long>(sizeof(BltLockStAtPass)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowSecLe_mrx:
    {
        if (sizeof(BltLockStAtRowSecLe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltLockStAtRowSecLe (%u)", ComConf_ComSignalGroup_igBltLockStAtRowSecLe_mrx);
            const BltLockStAtRowSecLe& rteValue = *static_cast<const BltLockStAtRowSecLe*>(buffer);
            autosar::BltLockStAtRowSecLe_info::data_elem_type deValue;
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowSecLeForBltLockEquid);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowSecLeForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowSecLeForBltLockSts);

            BltLockStAtRowSecLe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltLockStAtRowSecLe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBltLockStAtRowSecLe_mrx, length, static_cast<unsigned long>(sizeof(BltLockStAtRowSecLe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowSecMid_mrx:
    {
        if (sizeof(BltLockStAtRowSecMid) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltLockStAtRowSecMid (%u)", ComConf_ComSignalGroup_igBltLockStAtRowSecMid_mrx);
            const BltLockStAtRowSecMid& rteValue = *static_cast<const BltLockStAtRowSecMid*>(buffer);
            autosar::BltLockStAtRowSecMid_info::data_elem_type deValue;
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowSecMidForBltLockEquid);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowSecMidForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowSecMidForBltLockSts);

            BltLockStAtRowSecMid_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltLockStAtRowSecMid (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBltLockStAtRowSecMid_mrx, length, static_cast<unsigned long>(sizeof(BltLockStAtRowSecMid)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowSecRi_mrx:
    {
        if (sizeof(BltLockStAtRowSecRi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltLockStAtRowSecRi (%u)", ComConf_ComSignalGroup_igBltLockStAtRowSecRi_mrx);
            const BltLockStAtRowSecRi& rteValue = *static_cast<const BltLockStAtRowSecRi*>(buffer);
            autosar::BltLockStAtRowSecRi_info::data_elem_type deValue;
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowSecRiForBltLockEquid);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowSecRiForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowSecRiForBltLockSts);

            BltLockStAtRowSecRi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltLockStAtRowSecRi (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBltLockStAtRowSecRi_mrx, length, static_cast<unsigned long>(sizeof(BltLockStAtRowSecRi)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowThrdLe_mrx:
    {
        if (sizeof(BltLockStAtRowThrdLe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltLockStAtRowThrdLe (%u)", ComConf_ComSignalGroup_igBltLockStAtRowThrdLe_mrx);
            const BltLockStAtRowThrdLe& rteValue = *static_cast<const BltLockStAtRowThrdLe*>(buffer);
            autosar::BltLockStAtRowThrdLe_info::data_elem_type deValue;
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowThrdLeForBltLockEquid);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowThrdLeForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowThrdLeForBltLockSts);

            BltLockStAtRowThrdLe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltLockStAtRowThrdLe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBltLockStAtRowThrdLe_mrx, length, static_cast<unsigned long>(sizeof(BltLockStAtRowThrdLe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBltLockStAtRowThrdRi_mrx:
    {
        if (sizeof(BltLockStAtRowThrdRi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltLockStAtRowThrdRi (%u)", ComConf_ComSignalGroup_igBltLockStAtRowThrdRi_mrx);
            const BltLockStAtRowThrdRi& rteValue = *static_cast<const BltLockStAtRowThrdRi*>(buffer);
            autosar::BltLockStAtRowThrdRi_info::data_elem_type deValue;
            deValue.BltLockEquid = static_cast<decltype(deValue.BltLockEquid)>(rteValue.BltLockStAtRowThrdRiForBltLockEquid);
            deValue.BltLockSt1_ = static_cast<decltype(deValue.BltLockSt1_)>(rteValue.BltLockStAtRowThrdRiForBltLockSt1);
            deValue.BltLockSts = static_cast<decltype(deValue.BltLockSts)>(rteValue.BltLockStAtRowThrdRiForBltLockSts);

            BltLockStAtRowThrdRi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltLockStAtRowThrdRi (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBltLockStAtRowThrdRi_mrx, length, static_cast<unsigned long>(sizeof(BltLockStAtRowThrdRi)));
        }
    }
    break;

    case ComConf_ComSignal_isBltRmnSound1_mrx:
    {
        if (sizeof(BltRmnSound1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltRmnSound1 (%u)", ComConf_ComSignal_isBltRmnSound1_mrx);
            const BltRmnSound1& rteValue = *static_cast<const BltRmnSound1*>(buffer);
            autosar::BltRmnSound1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BltRmnSound1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltRmnSound1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isBltRmnSound1_mrx, length, static_cast<unsigned long>(sizeof(BltRmnSound1)));
        }
    }
    break;

    case ComConf_ComSignal_isBltRmnSound2_mrx:
    {
        if (sizeof(BltRmnSound2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltRmnSound2 (%u)", ComConf_ComSignal_isBltRmnSound2_mrx);
            const BltRmnSound2& rteValue = *static_cast<const BltRmnSound2*>(buffer);
            autosar::BltRmnSound2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BltRmnSound2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltRmnSound2 (%u). Got %zu , expected %lu", ComConf_ComSignal_isBltRmnSound2_mrx, length, static_cast<unsigned long>(sizeof(BltRmnSound2)));
        }
    }
    break;

    case ComConf_ComSignal_isBltRmnSound3_mrx:
    {
        if (sizeof(BltRmnSound3) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltRmnSound3 (%u)", ComConf_ComSignal_isBltRmnSound3_mrx);
            const BltRmnSound3& rteValue = *static_cast<const BltRmnSound3*>(buffer);
            autosar::BltRmnSound3_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BltRmnSound3_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltRmnSound3 (%u). Got %zu , expected %lu", ComConf_ComSignal_isBltRmnSound3_mrx, length, static_cast<unsigned long>(sizeof(BltRmnSound3)));
        }
    }
    break;

    case ComConf_ComSignal_isBltRmnSound4_mrx:
    {
        if (sizeof(BltRmnSound4) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BltRmnSound4 (%u)", ComConf_ComSignal_isBltRmnSound4_mrx);
            const BltRmnSound4& rteValue = *static_cast<const BltRmnSound4*>(buffer);
            autosar::BltRmnSound4_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BltRmnSound4_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BltRmnSound4 (%u). Got %zu , expected %lu", ComConf_ComSignal_isBltRmnSound4_mrx, length, static_cast<unsigned long>(sizeof(BltRmnSound4)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBrkAndAbsWarnIndcnReqGroup_mrx:
    {
        if (sizeof(BrkAndAbsWarnIndcnReqGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BrkAndAbsWarnIndcnReqGroup (%u)", ComConf_ComSignalGroup_igBrkAndAbsWarnIndcnReqGroup_mrx);
            const BrkAndAbsWarnIndcnReqGroup& rteValue = *static_cast<const BrkAndAbsWarnIndcnReqGroup*>(buffer);
            autosar::BrkAndAbsWarnIndcnReq_info::data_elem_type deValue;
            deValue.AbsWarnIndcnReq_ = static_cast<decltype(deValue.AbsWarnIndcnReq_)>(rteValue.AbsWarnIndcnReq);
            deValue.BrkAndAbsWarnIndcnReqChks = static_cast<decltype(deValue.BrkAndAbsWarnIndcnReqChks)>(rteValue.BrkAndAbsWarnIndcnReqChks);
            deValue.BrkAndAbsWarnIndcnReqCntr = static_cast<decltype(deValue.BrkAndAbsWarnIndcnReqCntr)>(rteValue.BrkAndAbsWarnIndcnReqCntr);
            deValue.BrkWarnIndcnReq = static_cast<decltype(deValue.BrkWarnIndcnReq)>(rteValue.BrkWarnIndcnReq);

            BrkAndAbsWarnIndcnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BrkAndAbsWarnIndcnReqGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBrkAndAbsWarnIndcnReqGroup_mrx, length, static_cast<unsigned long>(sizeof(BrkAndAbsWarnIndcnReqGroup)));
        }
    }
    break;

    case ComConf_ComSignal_isBrkFldLvl_mrx:
    {
        if (sizeof(BrkFldLvl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BrkFldLvl (%u)", ComConf_ComSignal_isBrkFldLvl_mrx);
            const BrkFldLvl& rteValue = *static_cast<const BrkFldLvl*>(buffer);
            autosar::BrkFldLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BrkFldLvl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BrkFldLvl (%u). Got %zu , expected %lu", ComConf_ComSignal_isBrkFldLvl_mrx, length, static_cast<unsigned long>(sizeof(BrkFldLvl)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igBrkFricTqAtWhlAct_mrx:
    {
        if (sizeof(BrkFricTqAtWhlAct) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BrkFricTqAtWhlAct (%u)", ComConf_ComSignalGroup_igBrkFricTqAtWhlAct_mrx);
            const BrkFricTqAtWhlAct& rteValue = *static_cast<const BrkFricTqAtWhlAct*>(buffer);
            autosar::BrkFricTqAtWhlAct_info::data_elem_type deValue;
            deValue.BrkFricTqAtWhlFrntLeAct = static_cast<decltype(deValue.BrkFricTqAtWhlFrntLeAct)>( toUnsignedFromRaw<13>(rteValue.BrkFricTqAtWhlFrntLeAct) ) * 1.0 + 0.0;
            deValue.BrkFricTqAtWhlFrntRiAct = static_cast<decltype(deValue.BrkFricTqAtWhlFrntRiAct)>( toUnsignedFromRaw<13>(rteValue.BrkFricTqAtWhlFrntRiAct) ) * 1.0 + 0.0;
            deValue.BrkFricTqAtWhlReLeAct = static_cast<decltype(deValue.BrkFricTqAtWhlReLeAct)>( toUnsignedFromRaw<13>(rteValue.BrkFricTqAtWhlReLeAct) ) * 1.0 + 0.0;
            deValue.BrkFricTqAtWhlReRiAct = static_cast<decltype(deValue.BrkFricTqAtWhlReRiAct)>( toUnsignedFromRaw<13>(rteValue.BrkFricTqAtWhlReRiAct) ) * 1.0 + 0.0;

            BrkFricTqAtWhlAct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BrkFricTqAtWhlAct (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igBrkFricTqAtWhlAct_mrx, length, static_cast<unsigned long>(sizeof(BrkFricTqAtWhlAct)));
        }
    }
    break;

    case ComConf_ComSignal_isBrkRelsWarnReq_mrx:
    {
        if (sizeof(BrkRelsWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BrkRelsWarnReq (%u)", ComConf_ComSignal_isBrkRelsWarnReq_mrx);
            const BrkRelsWarnReq& rteValue = *static_cast<const BrkRelsWarnReq*>(buffer);
            autosar::BrkRelsWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BrkRelsWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BrkRelsWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isBrkRelsWarnReq_mrx, length, static_cast<unsigned long>(sizeof(BrkRelsWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isBtn1ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn1ForUsrSwtPanFrntReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received Btn1ForUsrSwtPanFrntReq (%u)", ComConf_ComSignal_isBtn1ForUsrSwtPanFrntReq_mrx);
            const Btn1ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn1ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn1ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn1ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for Btn1ForUsrSwtPanFrntReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtn1ForUsrSwtPanFrntReq_mrx, length, static_cast<unsigned long>(sizeof(Btn1ForUsrSwtPanFrntReq)));
        }
    }
    break;

    case ComConf_ComSignal_isBtn2ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn2ForUsrSwtPanFrntReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received Btn2ForUsrSwtPanFrntReq (%u)", ComConf_ComSignal_isBtn2ForUsrSwtPanFrntReq_mrx);
            const Btn2ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn2ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn2ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn2ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for Btn2ForUsrSwtPanFrntReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtn2ForUsrSwtPanFrntReq_mrx, length, static_cast<unsigned long>(sizeof(Btn2ForUsrSwtPanFrntReq)));
        }
    }
    break;

    case ComConf_ComSignal_isBtn3ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn3ForUsrSwtPanFrntReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received Btn3ForUsrSwtPanFrntReq (%u)", ComConf_ComSignal_isBtn3ForUsrSwtPanFrntReq_mrx);
            const Btn3ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn3ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn3ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn3ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for Btn3ForUsrSwtPanFrntReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtn3ForUsrSwtPanFrntReq_mrx, length, static_cast<unsigned long>(sizeof(Btn3ForUsrSwtPanFrntReq)));
        }
    }
    break;

    case ComConf_ComSignal_isBtn4ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn4ForUsrSwtPanFrntReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received Btn4ForUsrSwtPanFrntReq (%u)", ComConf_ComSignal_isBtn4ForUsrSwtPanFrntReq_mrx);
            const Btn4ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn4ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn4ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn4ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for Btn4ForUsrSwtPanFrntReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtn4ForUsrSwtPanFrntReq_mrx, length, static_cast<unsigned long>(sizeof(Btn4ForUsrSwtPanFrntReq)));
        }
    }
    break;

    case ComConf_ComSignal_isBtn5ForUsrSwtPanFrntReq_mrx:
    {
        if (sizeof(Btn5ForUsrSwtPanFrntReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received Btn5ForUsrSwtPanFrntReq (%u)", ComConf_ComSignal_isBtn5ForUsrSwtPanFrntReq_mrx);
            const Btn5ForUsrSwtPanFrntReq& rteValue = *static_cast<const Btn5ForUsrSwtPanFrntReq*>(buffer);
            autosar::Btn5ForUsrSwtPanFrntReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            Btn5ForUsrSwtPanFrntReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for Btn5ForUsrSwtPanFrntReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtn5ForUsrSwtPanFrntReq_mrx, length, static_cast<unsigned long>(sizeof(Btn5ForUsrSwtPanFrntReq)));
        }
    }
    break;

    case ComConf_ComSignal_isBtnAudFbSts_mrx:
    {
        if (sizeof(BtnAudFbSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BtnAudFbSts (%u)", ComConf_ComSignal_isBtnAudFbSts_mrx);
            const BtnAudFbSts& rteValue = *static_cast<const BtnAudFbSts*>(buffer);
            autosar::BtnAudFbSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnAudFbSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BtnAudFbSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtnAudFbSts_mrx, length, static_cast<unsigned long>(sizeof(BtnAudFbSts)));
        }
    }
    break;

    case ComConf_ComSignal_isBtnAudVolSts_mrx:
    {
        if (sizeof(BtnAudVolSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BtnAudVolSts (%u)", ComConf_ComSignal_isBtnAudVolSts_mrx);
            const BtnAudVolSts& rteValue = *static_cast<const BtnAudVolSts*>(buffer);
            autosar::BtnAudVolSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnAudVolSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BtnAudVolSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtnAudVolSts_mrx, length, static_cast<unsigned long>(sizeof(BtnAudVolSts)));
        }
    }
    break;

    case ComConf_ComSignal_isBtnMmedLeRiSts_mrx:
    {
        if (sizeof(BtnMmedLeRiSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BtnMmedLeRiSts (%u)", ComConf_ComSignal_isBtnMmedLeRiSts_mrx);
            const BtnMmedLeRiSts& rteValue = *static_cast<const BtnMmedLeRiSts*>(buffer);
            autosar::BtnMmedLeRiSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnMmedLeRiSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BtnMmedLeRiSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtnMmedLeRiSts_mrx, length, static_cast<unsigned long>(sizeof(BtnMmedLeRiSts)));
        }
    }
    break;

    case ComConf_ComSignal_isBtnMmedModSts_mrx:
    {
        if (sizeof(BtnMmedModSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BtnMmedModSts (%u)", ComConf_ComSignal_isBtnMmedModSts_mrx);
            const BtnMmedModSts& rteValue = *static_cast<const BtnMmedModSts*>(buffer);
            autosar::BtnMmedModSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnMmedModSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BtnMmedModSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtnMmedModSts_mrx, length, static_cast<unsigned long>(sizeof(BtnMmedModSts)));
        }
    }
    break;

    case ComConf_ComSignal_isBtnMmedSetSts_mrx:
    {
        if (sizeof(BtnMmedSetSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BtnMmedSetSts (%u)", ComConf_ComSignal_isBtnMmedSetSts_mrx);
            const BtnMmedSetSts& rteValue = *static_cast<const BtnMmedSetSts*>(buffer);
            autosar::BtnMmedSetSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            BtnMmedSetSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BtnMmedSetSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isBtnMmedSetSts_mrx, length, static_cast<unsigned long>(sizeof(BtnMmedSetSts)));
        }
    }
    break;

    case ComConf_ComSignal_isCCSMBtn6_mrx:
    {
        if (sizeof(CCSMBtn6) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CCSMBtn6 (%u)", ComConf_ComSignal_isCCSMBtn6_mrx);
            const CCSMBtn6& rteValue = *static_cast<const CCSMBtn6*>(buffer);
            autosar::CCSMBtn6_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CCSMBtn6_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CCSMBtn6 (%u). Got %zu , expected %lu", ComConf_ComSignal_isCCSMBtn6_mrx, length, static_cast<unsigned long>(sizeof(CCSMBtn6)));
        }
    }
    break;

    case ComConf_ComSignal_isCarTiGlb_mrx:
    {
        if (sizeof(CarTiGlb) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CarTiGlb (%u)", ComConf_ComSignal_isCarTiGlb_mrx);
            const CarTiGlb& rteValue = *static_cast<const CarTiGlb*>(buffer);
            autosar::CarTiGlb_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.1 + 0.0;
            CarTiGlb_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CarTiGlb (%u). Got %zu , expected %lu", ComConf_ComSignal_isCarTiGlb_mrx, length, static_cast<unsigned long>(sizeof(CarTiGlb)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igCchForFuEco_mrx:
    {
        if (sizeof(CchForFuEco) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CchForFuEco (%u)", ComConf_ComSignalGroup_igCchForFuEco_mrx);
            const CchForFuEco& rteValue = *static_cast<const CchForFuEco*>(buffer);
            autosar::CchForFuEco_info::data_elem_type deValue;
            deValue.BarForFuEco = static_cast<decltype(deValue.BarForFuEco)>( toUnsignedFromRaw<10>(rteValue.BarForFuEco) ) * 0.1 + 0.0;
            deValue.GrdForFuEco = static_cast<decltype(deValue.GrdForFuEco)>( toUnsignedFromRaw<10>(rteValue.GrdForFuEco) ) * 0.1 + 0.0;

            CchForFuEco_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CchForFuEco (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igCchForFuEco_mrx, length, static_cast<unsigned long>(sizeof(CchForFuEco)));
        }
    }
    break;

    case ComConf_ComSignal_isChdLockgProtnStsToHmi_mrx:
    {
        if (sizeof(ChdLockgProtnStsToHmi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ChdLockgProtnStsToHmi (%u)", ComConf_ComSignal_isChdLockgProtnStsToHmi_mrx);
            const ChdLockgProtnStsToHmi& rteValue = *static_cast<const ChdLockgProtnStsToHmi*>(buffer);
            autosar::ChdLockgProtnStsToHmi_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ChdLockgProtnStsToHmi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ChdLockgProtnStsToHmi (%u). Got %zu , expected %lu", ComConf_ComSignal_isChdLockgProtnStsToHmi_mrx, length, static_cast<unsigned long>(sizeof(ChdLockgProtnStsToHmi)));
        }
    }
    break;

    case ComConf_ComSignal_isChdWinProtnStsAtDrvrRe_mrx:
    {
        if (sizeof(ChdWinProtnStsAtDrvrRe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ChdWinProtnStsAtDrvrRe (%u)", ComConf_ComSignal_isChdWinProtnStsAtDrvrRe_mrx);
            const ChdWinProtnStsAtDrvrRe& rteValue = *static_cast<const ChdWinProtnStsAtDrvrRe*>(buffer);
            autosar::ChdWinProtnStsAtDrvrRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ChdWinProtnStsAtDrvrRe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ChdWinProtnStsAtDrvrRe (%u). Got %zu , expected %lu", ComConf_ComSignal_isChdWinProtnStsAtDrvrRe_mrx, length, static_cast<unsigned long>(sizeof(ChdWinProtnStsAtDrvrRe)));
        }
    }
    break;

    case ComConf_ComSignal_isChdWinProtnStsAtPassRe_mrx:
    {
        if (sizeof(ChdWinProtnStsAtPassRe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ChdWinProtnStsAtPassRe (%u)", ComConf_ComSignal_isChdWinProtnStsAtPassRe_mrx);
            const ChdWinProtnStsAtPassRe& rteValue = *static_cast<const ChdWinProtnStsAtPassRe*>(buffer);
            autosar::ChdWinProtnStsAtPassRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ChdWinProtnStsAtPassRe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ChdWinProtnStsAtPassRe (%u). Got %zu , expected %lu", ComConf_ComSignal_isChdWinProtnStsAtPassRe_mrx, length, static_cast<unsigned long>(sizeof(ChdWinProtnStsAtPassRe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igChrgrHwAprvdWirelsAdr_mrx:
    {
        if (sizeof(ChrgrHwAprvdWirelsAdr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ChrgrHwAprvdWirelsAdr (%u)", ComConf_ComSignalGroup_igChrgrHwAprvdWirelsAdr_mrx);
            const ChrgrHwAprvdWirelsAdr& rteValue = *static_cast<const ChrgrHwAprvdWirelsAdr*>(buffer);
            autosar::ChrgrHwAprvdWirelsAdr_info::data_elem_type deValue;
            deValue.AprvdSts_ = static_cast<decltype(deValue.AprvdSts_)>(rteValue.ChrgrHwAprvdWirelsAdrAprvdSts);
            deValue.HwOffsAdr1 = static_cast<decltype(deValue.HwOffsAdr1)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr1);
            deValue.HwOffsAdr2 = static_cast<decltype(deValue.HwOffsAdr2)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr2);
            deValue.HwOffsAdr3 = static_cast<decltype(deValue.HwOffsAdr3)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr3);
            deValue.HwOffsAdr4 = static_cast<decltype(deValue.HwOffsAdr4)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr4);
            deValue.HwOffsAdr5 = static_cast<decltype(deValue.HwOffsAdr5)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr5);
            deValue.HwOffsAdr6 = static_cast<decltype(deValue.HwOffsAdr6)>(rteValue.ChrgrHwAprvdWirelsAdrHwOffsAdr6);

            ChrgrHwAprvdWirelsAdr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ChrgrHwAprvdWirelsAdr (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igChrgrHwAprvdWirelsAdr_mrx, length, static_cast<unsigned long>(sizeof(ChrgrHwAprvdWirelsAdr)));
        }
    }
    break;

    case ComConf_ComSignal_isChrgrWirelsSts_mrx:
    {
        if (sizeof(ChrgrWirelsSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ChrgrWirelsSts (%u)", ComConf_ComSignal_isChrgrWirelsSts_mrx);
            const ChrgrWirelsSts& rteValue = *static_cast<const ChrgrWirelsSts*>(buffer);
            autosar::ChrgrWirelsSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ChrgrWirelsSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ChrgrWirelsSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isChrgrWirelsSts_mrx, length, static_cast<unsigned long>(sizeof(ChrgrWirelsSts)));
        }
    }
    break;

    case ComConf_ComSignal_isClimaActv_mrx:
    {
        if (sizeof(ClimaActv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ClimaActv (%u)", ComConf_ComSignal_isClimaActv_mrx);
            const ClimaActv& rteValue = *static_cast<const ClimaActv*>(buffer);
            autosar::ClimaActv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ClimaActv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ClimaActv (%u). Got %zu , expected %lu", ComConf_ComSignal_isClimaActv_mrx, length, static_cast<unsigned long>(sizeof(ClimaActv)));
        }
    }
    break;

    case ComConf_ComSignal_isClimaPwrCns_mrx:
    {
        if (sizeof(ClimaPwrCns) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ClimaPwrCns (%u)", ComConf_ComSignal_isClimaPwrCns_mrx);
            const ClimaPwrCns& rteValue = *static_cast<const ClimaPwrCns*>(buffer);
            autosar::ClimaPwrCns_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 1.0 + 0.0;
            ClimaPwrCns_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ClimaPwrCns (%u). Got %zu , expected %lu", ComConf_ComSignal_isClimaPwrCns_mrx, length, static_cast<unsigned long>(sizeof(ClimaPwrCns)));
        }
    }
    break;

    case ComConf_ComSignal_isClimaTmrSts_mrx:
    {
        if (sizeof(ClimaTmrSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ClimaTmrSts (%u)", ComConf_ComSignal_isClimaTmrSts_mrx);
            const ClimaTmrSts& rteValue = *static_cast<const ClimaTmrSts*>(buffer);
            autosar::ClimaTmrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ClimaTmrSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ClimaTmrSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isClimaTmrSts_mrx, length, static_cast<unsigned long>(sizeof(ClimaTmrSts)));
        }
    }
    break;

    case ComConf_ComSignal_isClimaWarn_mrx:
    {
        if (sizeof(ClimaWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ClimaWarn (%u)", ComConf_ComSignal_isClimaWarn_mrx);
            const ClimaWarn& rteValue = *static_cast<const ClimaWarn*>(buffer);
            autosar::ClimaWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ClimaWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ClimaWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isClimaWarn_mrx, length, static_cast<unsigned long>(sizeof(ClimaWarn)));
        }
    }
    break;

    case ComConf_ComSignal_isCllsnFwdWarnReq_mrx:
    {
        if (sizeof(CllsnFwdWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CllsnFwdWarnReq (%u)", ComConf_ComSignal_isCllsnFwdWarnReq_mrx);
            const CllsnFwdWarnReq& rteValue = *static_cast<const CllsnFwdWarnReq*>(buffer);
            autosar::CllsnFwdWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CllsnFwdWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CllsnFwdWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isCllsnFwdWarnReq_mrx, length, static_cast<unsigned long>(sizeof(CllsnFwdWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isCllsnWarnReq_mrx:
    {
        if (sizeof(CllsnWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CllsnWarnReq (%u)", ComConf_ComSignal_isCllsnWarnReq_mrx);
            const CllsnWarnReq& rteValue = *static_cast<const CllsnWarnReq*>(buffer);
            autosar::CllsnWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CllsnWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CllsnWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isCllsnWarnReq_mrx, length, static_cast<unsigned long>(sizeof(CllsnWarnReq)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igCllsnWarnSide_mrx:
    {
        if (sizeof(CllsnWarnSide) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CllsnWarnSide (%u)", ComConf_ComSignalGroup_igCllsnWarnSide_mrx);
            const CllsnWarnSide& rteValue = *static_cast<const CllsnWarnSide*>(buffer);
            autosar::CllsnWarnSide_info::data_elem_type deValue;
            deValue.Le = static_cast<decltype(deValue.Le)>(rteValue.CllsnWarnSideLe);
            deValue.Ri = static_cast<decltype(deValue.Ri)>(rteValue.CllsnWarnSideRi);

            CllsnWarnSide_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CllsnWarnSide (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igCllsnWarnSide_mrx, length, static_cast<unsigned long>(sizeof(CllsnWarnSide)));
        }
    }
    break;

    case ComConf_ComSignal_isClngActv_mrx:
    {
        if (sizeof(ClngActv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ClngActv (%u)", ComConf_ComSignal_isClngActv_mrx);
            const ClngActv& rteValue = *static_cast<const ClngActv*>(buffer);
            autosar::ClngActv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ClngActv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ClngActv (%u). Got %zu , expected %lu", ComConf_ComSignal_isClngActv_mrx, length, static_cast<unsigned long>(sizeof(ClngActv)));
        }
    }
    break;

    case ComConf_ComSignal_isCmftFctSts_mrx:
    {
        if (sizeof(CmftFctSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CmftFctSts (%u)", ComConf_ComSignal_isCmftFctSts_mrx);
            const CmftFctSts& rteValue = *static_cast<const CmftFctSts*>(buffer);
            autosar::CmftFctSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CmftFctSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CmftFctSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isCmftFctSts_mrx, length, static_cast<unsigned long>(sizeof(CmftFctSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igCmptmtAirTEstimdExtd_mrx:
    {
        if (sizeof(CmptmtAirTEstimdExtd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CmptmtAirTEstimdExtd (%u)", ComConf_ComSignalGroup_igCmptmtAirTEstimdExtd_mrx);
            const CmptmtAirTEstimdExtd& rteValue = *static_cast<const CmptmtAirTEstimdExtd*>(buffer);
            autosar::CmptmtAirTEstimdExtd_info::data_elem_type deValue;
            deValue.ComptmtT = static_cast<decltype(deValue.ComptmtT)>( toUnsignedFromRaw<11>(rteValue.CmptmtAirTEstimdExtdComptmtT) ) * 0.1 + -60.0;
            deValue.QlyFlg = static_cast<decltype(deValue.QlyFlg)>(rteValue.CmptmtAirTEstimdExtdQlyFlg);

            CmptmtAirTEstimdExtd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CmptmtAirTEstimdExtd (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igCmptmtAirTEstimdExtd_mrx, length, static_cast<unsigned long>(sizeof(CmptmtAirTEstimdExtd)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igCmptmtTFrntGroup_mrx:
    {
        if (sizeof(CmptmtTFrntGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CmptmtTFrntGroup (%u)", ComConf_ComSignalGroup_igCmptmtTFrntGroup_mrx);
            const CmptmtTFrntGroup& rteValue = *static_cast<const CmptmtTFrntGroup*>(buffer);
            autosar::CmptmtTFrnt_info::data_elem_type deValue;
            deValue.CmptmtTFrnt = static_cast<decltype(deValue.CmptmtTFrnt)>( toUnsignedFromRaw<11>(rteValue.CmptmtTFrnt) ) * 0.1 + -60.0;
            deValue.CmptmtTFrntQf_ = static_cast<decltype(deValue.CmptmtTFrntQf_)>(rteValue.CmptmtTFrntQf);
            deValue.FanForCmptmtTRunng = static_cast<decltype(deValue.FanForCmptmtTRunng)>(rteValue.FanForCmptmtTRunng);

            CmptmtTFrnt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CmptmtTFrntGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igCmptmtTFrntGroup_mrx, length, static_cast<unsigned long>(sizeof(CmptmtTFrntGroup)));
        }
    }
    break;

    case ComConf_ComSignal_isCnclWarnForCrsCtrl_mrx:
    {
        if (sizeof(CnclWarnForCrsCtrl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CnclWarnForCrsCtrl (%u)", ComConf_ComSignal_isCnclWarnForCrsCtrl_mrx);
            const CnclWarnForCrsCtrl& rteValue = *static_cast<const CnclWarnForCrsCtrl*>(buffer);
            autosar::CnclWarnForCrsCtrl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CnclWarnForCrsCtrl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CnclWarnForCrsCtrl (%u). Got %zu , expected %lu", ComConf_ComSignal_isCnclWarnForCrsCtrl_mrx, length, static_cast<unsigned long>(sizeof(CnclWarnForCrsCtrl)));
        }
    }
    break;

    case ComConf_ComSignal_isCnclWarnLgtForAutDrv_mrx:
    {
        if (sizeof(CnclWarnLgtForAutDrv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CnclWarnLgtForAutDrv (%u)", ComConf_ComSignal_isCnclWarnLgtForAutDrv_mrx);
            const CnclWarnLgtForAutDrv& rteValue = *static_cast<const CnclWarnLgtForAutDrv*>(buffer);
            autosar::CnclWarnLgtForAutDrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CnclWarnLgtForAutDrv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CnclWarnLgtForAutDrv (%u). Got %zu , expected %lu", ComConf_ComSignal_isCnclWarnLgtForAutDrv_mrx, length, static_cast<unsigned long>(sizeof(CnclWarnLgtForAutDrv)));
        }
    }
    break;

    case ComConf_ComSignal_isComLostExtrSound_mrx:
    {
        if (sizeof(ComLostExtrSound) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ComLostExtrSound (%u)", ComConf_ComSignal_isComLostExtrSound_mrx);
            const ComLostExtrSound& rteValue = *static_cast<const ComLostExtrSound*>(buffer);
            autosar::ComLostExtrSound_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ComLostExtrSound_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ComLostExtrSound (%u). Got %zu , expected %lu", ComConf_ComSignal_isComLostExtrSound_mrx, length, static_cast<unsigned long>(sizeof(ComLostExtrSound)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igConSftyWarn_mrx:
    {
        if (sizeof(ConSftyWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ConSftyWarn (%u)", ComConf_ComSignalGroup_igConSftyWarn_mrx);
            const ConSftyWarn& rteValue = *static_cast<const ConSftyWarn*>(buffer);
            autosar::ConSftyWarn_info::data_elem_type deValue;
            deValue.ConSftyWarnId = static_cast<decltype(deValue.ConSftyWarnId)>(rteValue.ConSftyWarnConSftyWarnId);
            deValue.ConSftyWarnLvl = static_cast<decltype(deValue.ConSftyWarnLvl)>(rteValue.ConSftyWarnConSftyWarnLvl);
            deValue.ConSftyWarnSnd = static_cast<decltype(deValue.ConSftyWarnSnd)>(rteValue.ConSftyWarnConSftyWarnSnd);
            deValue.ConSftyWarnTyp = static_cast<decltype(deValue.ConSftyWarnTyp)>(rteValue.ConSftyWarnConSftyWarnTyp);
            deValue.DistanceToWarning = static_cast<decltype(deValue.DistanceToWarning)>(rteValue.ConSftyWarnDistanceToWarning);

            ConSftyWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ConSftyWarn (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igConSftyWarn_mrx, length, static_cast<unsigned long>(sizeof(ConSftyWarn)));
        }
    }
    break;

    case ComConf_ComSignal_isCoolgStsForDisp_mrx:
    {
        if (sizeof(CoolgStsForDisp) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CoolgStsForDisp (%u)", ComConf_ComSignal_isCoolgStsForDisp_mrx);
            const CoolgStsForDisp& rteValue = *static_cast<const CoolgStsForDisp*>(buffer);
            autosar::CoolgStsForDisp_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CoolgStsForDisp_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CoolgStsForDisp (%u). Got %zu , expected %lu", ComConf_ComSignal_isCoolgStsForDisp_mrx, length, static_cast<unsigned long>(sizeof(CoolgStsForDisp)));
        }
    }
    break;

    case ComConf_ComSignal_isCrsCtrlrSts_mrx:
    {
        if (sizeof(CrsCtrlrSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CrsCtrlrSts (%u)", ComConf_ComSignal_isCrsCtrlrSts_mrx);
            const CrsCtrlrSts& rteValue = *static_cast<const CrsCtrlrSts*>(buffer);
            autosar::CrsCtrlrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CrsCtrlrSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CrsCtrlrSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isCrsCtrlrSts_mrx, length, static_cast<unsigned long>(sizeof(CrsCtrlrSts)));
        }
    }
    break;

    case ComConf_ComSignal_isCrvtSpdAdpvSts_mrx:
    {
        if (sizeof(CrvtSpdAdpvSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CrvtSpdAdpvSts (%u)", ComConf_ComSignal_isCrvtSpdAdpvSts_mrx);
            const CrvtSpdAdpvSts& rteValue = *static_cast<const CrvtSpdAdpvSts*>(buffer);
            autosar::CrvtSpdAdpvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CrvtSpdAdpvSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CrvtSpdAdpvSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isCrvtSpdAdpvSts_mrx, length, static_cast<unsigned long>(sizeof(CrvtSpdAdpvSts)));
        }
    }
    break;

    case ComConf_ComSignal_isCrvtSpdWarnReq_mrx:
    {
        if (sizeof(CrvtSpdWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CrvtSpdWarnReq (%u)", ComConf_ComSignal_isCrvtSpdWarnReq_mrx);
            const CrvtSpdWarnReq& rteValue = *static_cast<const CrvtSpdWarnReq*>(buffer);
            autosar::CrvtSpdWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CrvtSpdWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CrvtSpdWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isCrvtSpdWarnReq_mrx, length, static_cast<unsigned long>(sizeof(CrvtSpdWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isCrvtSpdWarnSts_mrx:
    {
        if (sizeof(CrvtSpdWarnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CrvtSpdWarnSts (%u)", ComConf_ComSignal_isCrvtSpdWarnSts_mrx);
            const CrvtSpdWarnSts& rteValue = *static_cast<const CrvtSpdWarnSts*>(buffer);
            autosar::CrvtSpdWarnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CrvtSpdWarnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CrvtSpdWarnSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isCrvtSpdWarnSts_mrx, length, static_cast<unsigned long>(sizeof(CrvtSpdWarnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isCtraIndcnLe_mrx:
    {
        if (sizeof(CtraIndcnLe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CtraIndcnLe (%u)", ComConf_ComSignal_isCtraIndcnLe_mrx);
            const CtraIndcnLe& rteValue = *static_cast<const CtraIndcnLe*>(buffer);
            autosar::CtraIndcnLe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CtraIndcnLe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CtraIndcnLe (%u). Got %zu , expected %lu", ComConf_ComSignal_isCtraIndcnLe_mrx, length, static_cast<unsigned long>(sizeof(CtraIndcnLe)));
        }
    }
    break;

    case ComConf_ComSignal_isCtraIndcnRi_mrx:
    {
        if (sizeof(CtraIndcnRi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CtraIndcnRi (%u)", ComConf_ComSignal_isCtraIndcnRi_mrx);
            const CtraIndcnRi& rteValue = *static_cast<const CtraIndcnRi*>(buffer);
            autosar::CtraIndcnRi_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CtraIndcnRi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CtraIndcnRi (%u). Got %zu , expected %lu", ComConf_ComSignal_isCtraIndcnRi_mrx, length, static_cast<unsigned long>(sizeof(CtraIndcnRi)));
        }
    }
    break;

    case ComConf_ComSignal_isCtraOn1_mrx:
    {
        if (sizeof(CtraOn1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received CtraOn1 (%u)", ComConf_ComSignal_isCtraOn1_mrx);
            const CtraOn1& rteValue = *static_cast<const CtraOn1*>(buffer);
            autosar::CtraOn1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            CtraOn1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for CtraOn1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isCtraOn1_mrx, length, static_cast<unsigned long>(sizeof(CtraOn1)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDataSpclDMSM_mrx:
    {
        if (sizeof(DataSpclDMSM) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DataSpclDMSM (%u)", ComConf_ComSignalGroup_igDataSpclDMSM_mrx);
            const DataSpclDMSM& rteValue = *static_cast<const DataSpclDMSM*>(buffer);
            autosar::DataSpclDMSM_info::data_elem_type deValue;
            deValue.DataIdn = static_cast<decltype(deValue.DataIdn)>(rteValue.DataSpclDMSMDataIdn);
            deValue.DataNrSpcl1 = static_cast<decltype(deValue.DataNrSpcl1)>(rteValue.DataSpclDMSMDataNrSpcl1);
            deValue.DataNrSpcl2 = static_cast<decltype(deValue.DataNrSpcl2)>(rteValue.DataSpclDMSMDataNrSpcl2);
            deValue.DataNrSpcl3 = static_cast<decltype(deValue.DataNrSpcl3)>(rteValue.DataSpclDMSMDataNrSpcl3);

            DataSpclDMSM_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DataSpclDMSM (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igDataSpclDMSM_mrx, length, static_cast<unsigned long>(sizeof(DataSpclDMSM)));
        }
    }
    break;

    case ComConf_ComSignal_isDayToSrv_mrx:
    {
        if (sizeof(DayToSrv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DayToSrv (%u)", ComConf_ComSignal_isDayToSrv_mrx);
            const DayToSrv& rteValue = *static_cast<const DayToSrv*>(buffer);
            autosar::DayToSrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DayToSrv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DayToSrv (%u). Got %zu , expected %lu", ComConf_ComSignal_isDayToSrv_mrx, length, static_cast<unsigned long>(sizeof(DayToSrv)));
        }
    }
    break;

    case ComConf_ComSignal_isDiagcCCSM_mrx:
    {
        if (sizeof(DiagcCCSM) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DiagcCCSM (%u)", ComConf_ComSignal_isDiagcCCSM_mrx);
            const DiagcCCSM& rteValue = *static_cast<const DiagcCCSM*>(buffer);
            autosar::DiagcCCSM_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DiagcCCSM_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DiagcCCSM (%u). Got %zu , expected %lu", ComConf_ComSignal_isDiagcCCSM_mrx, length, static_cast<unsigned long>(sizeof(DiagcCCSM)));
        }
    }
    break;

    case ComConf_ComSignal_isDiagcRCSM_mrx:
    {
        if (sizeof(DiagcRCSM) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DiagcRCSM (%u)", ComConf_ComSignal_isDiagcRCSM_mrx);
            const DiagcRCSM& rteValue = *static_cast<const DiagcRCSM*>(buffer);
            autosar::DiagcRCSM_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DiagcRCSM_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DiagcRCSM (%u). Got %zu , expected %lu", ComConf_ComSignal_isDiagcRCSM_mrx, length, static_cast<unsigned long>(sizeof(DiagcRCSM)));
        }
    }
    break;

    case ComConf_ComSignal_isDiagcRSHC_mrx:
    {
        if (sizeof(DiagcRSHC) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DiagcRSHC (%u)", ComConf_ComSignal_isDiagcRSHC_mrx);
            const DiagcRSHC& rteValue = *static_cast<const DiagcRSHC*>(buffer);
            autosar::DiagcRSHC_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DiagcRSHC_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DiagcRSHC (%u). Got %zu , expected %lu", ComConf_ComSignal_isDiagcRSHC_mrx, length, static_cast<unsigned long>(sizeof(DiagcRSHC)));
        }
    }
    break;

    case ComConf_ComSignal_isDiagcStsDMSM_mrx:
    {
        if (sizeof(DiagcStsDMSM) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DiagcStsDMSM (%u)", ComConf_ComSignal_isDiagcStsDMSM_mrx);
            const DiagcStsDMSM& rteValue = *static_cast<const DiagcStsDMSM*>(buffer);
            autosar::DiagcStsDMSM_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DiagcStsDMSM_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DiagcStsDMSM (%u). Got %zu , expected %lu", ComConf_ComSignal_isDiagcStsDMSM_mrx, length, static_cast<unsigned long>(sizeof(DiagcStsDMSM)));
        }
    }
    break;

    case ComConf_ComSignal_isDispBattEgyIn_mrx:
    {
        if (sizeof(DispBattEgyIn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DispBattEgyIn (%u)", ComConf_ComSignal_isDispBattEgyIn_mrx);
            const DispBattEgyIn& rteValue = *static_cast<const DispBattEgyIn*>(buffer);
            autosar::DispBattEgyIn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.5 + 0.0;
            DispBattEgyIn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DispBattEgyIn (%u). Got %zu , expected %lu", ComConf_ComSignal_isDispBattEgyIn_mrx, length, static_cast<unsigned long>(sizeof(DispBattEgyIn)));
        }
    }
    break;

    case ComConf_ComSignal_isDispBattEgyOut_mrx:
    {
        if (sizeof(DispBattEgyOut) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DispBattEgyOut (%u)", ComConf_ComSignal_isDispBattEgyOut_mrx);
            const DispBattEgyOut& rteValue = *static_cast<const DispBattEgyOut*>(buffer);
            autosar::DispBattEgyOut_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.5 + 0.0;
            DispBattEgyOut_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DispBattEgyOut (%u). Got %zu , expected %lu", ComConf_ComSignal_isDispBattEgyOut_mrx, length, static_cast<unsigned long>(sizeof(DispBattEgyOut)));
        }
    }
    break;

    case ComConf_ComSignal_isDispOfPrpsnMod_mrx:
    {
        if (sizeof(DispOfPrpsnMod) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DispOfPrpsnMod (%u)", ComConf_ComSignal_isDispOfPrpsnMod_mrx);
            const DispOfPrpsnMod& rteValue = *static_cast<const DispOfPrpsnMod*>(buffer);
            autosar::DispOfPrpsnMod_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DispOfPrpsnMod_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DispOfPrpsnMod (%u). Got %zu , expected %lu", ComConf_ComSignal_isDispOfPrpsnMod_mrx, length, static_cast<unsigned long>(sizeof(DispOfPrpsnMod)));
        }
    }
    break;

    case ComConf_ComSignal_isDoorDrvrLockReSts_mrx:
    {
        if (sizeof(DoorDrvrLockReSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DoorDrvrLockReSts (%u)", ComConf_ComSignal_isDoorDrvrLockReSts_mrx);
            const DoorDrvrLockReSts& rteValue = *static_cast<const DoorDrvrLockReSts*>(buffer);
            autosar::DoorDrvrLockReSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorDrvrLockReSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DoorDrvrLockReSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDoorDrvrLockReSts_mrx, length, static_cast<unsigned long>(sizeof(DoorDrvrLockReSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDoorDrvrLockSts_mrx:
    {
        if (sizeof(DoorDrvrLockSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DoorDrvrLockSts (%u)", ComConf_ComSignal_isDoorDrvrLockSts_mrx);
            const DoorDrvrLockSts& rteValue = *static_cast<const DoorDrvrLockSts*>(buffer);
            autosar::DoorDrvrLockSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorDrvrLockSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DoorDrvrLockSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDoorDrvrLockSts_mrx, length, static_cast<unsigned long>(sizeof(DoorDrvrLockSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDoorDrvrReSts_mrx:
    {
        if (sizeof(DoorDrvrReSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DoorDrvrReSts (%u)", ComConf_ComSignal_isDoorDrvrReSts_mrx);
            const DoorDrvrReSts& rteValue = *static_cast<const DoorDrvrReSts*>(buffer);
            autosar::DoorDrvrReSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorDrvrReSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DoorDrvrReSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDoorDrvrReSts_mrx, length, static_cast<unsigned long>(sizeof(DoorDrvrReSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDoorDrvrSts_mrx:
    {
        if (sizeof(DoorDrvrSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DoorDrvrSts (%u)", ComConf_ComSignal_isDoorDrvrSts_mrx);
            const DoorDrvrSts& rteValue = *static_cast<const DoorDrvrSts*>(buffer);
            autosar::DoorDrvrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorDrvrSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DoorDrvrSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDoorDrvrSts_mrx, length, static_cast<unsigned long>(sizeof(DoorDrvrSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDoorPassLockReSts_mrx:
    {
        if (sizeof(DoorPassLockReSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DoorPassLockReSts (%u)", ComConf_ComSignal_isDoorPassLockReSts_mrx);
            const DoorPassLockReSts& rteValue = *static_cast<const DoorPassLockReSts*>(buffer);
            autosar::DoorPassLockReSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorPassLockReSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DoorPassLockReSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDoorPassLockReSts_mrx, length, static_cast<unsigned long>(sizeof(DoorPassLockReSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDoorPassLockSts_mrx:
    {
        if (sizeof(DoorPassLockSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DoorPassLockSts (%u)", ComConf_ComSignal_isDoorPassLockSts_mrx);
            const DoorPassLockSts& rteValue = *static_cast<const DoorPassLockSts*>(buffer);
            autosar::DoorPassLockSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorPassLockSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DoorPassLockSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDoorPassLockSts_mrx, length, static_cast<unsigned long>(sizeof(DoorPassLockSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDoorPassReSts_mrx:
    {
        if (sizeof(DoorPassReSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DoorPassReSts (%u)", ComConf_ComSignal_isDoorPassReSts_mrx);
            const DoorPassReSts& rteValue = *static_cast<const DoorPassReSts*>(buffer);
            autosar::DoorPassReSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorPassReSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DoorPassReSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDoorPassReSts_mrx, length, static_cast<unsigned long>(sizeof(DoorPassReSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDoorPassSts_mrx:
    {
        if (sizeof(DoorPassSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DoorPassSts (%u)", ComConf_ComSignal_isDoorPassSts_mrx);
            const DoorPassSts& rteValue = *static_cast<const DoorPassSts*>(buffer);
            autosar::DoorPassSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DoorPassSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DoorPassSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDoorPassSts_mrx, length, static_cast<unsigned long>(sizeof(DoorPassSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDriveAwayInfoActvSts_mrx:
    {
        if (sizeof(DriveAwayInfoActvSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DriveAwayInfoActvSts (%u)", ComConf_ComSignal_isDriveAwayInfoActvSts_mrx);
            const DriveAwayInfoActvSts& rteValue = *static_cast<const DriveAwayInfoActvSts*>(buffer);
            autosar::DriveAwayInfoActvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DriveAwayInfoActvSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DriveAwayInfoActvSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDriveAwayInfoActvSts_mrx, length, static_cast<unsigned long>(sizeof(DriveAwayInfoActvSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDriveAwayInfoWarnReq_mrx:
    {
        if (sizeof(DriveAwayInfoWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DriveAwayInfoWarnReq (%u)", ComConf_ComSignalGroup_igDriveAwayInfoWarnReq_mrx);
            const DriveAwayInfoWarnReq& rteValue = *static_cast<const DriveAwayInfoWarnReq*>(buffer);
            autosar::DriveAwayInfoWarnReq_info::data_elem_type deValue;
            deValue.AudWarnReq = static_cast<decltype(deValue.AudWarnReq)>(rteValue.DriveAwayInfoWarnReqAudWarnReq);
            deValue.CtrlDirOfTrfcLi = static_cast<decltype(deValue.CtrlDirOfTrfcLi)>(rteValue.DriveAwayInfoWarnReqCtrlDirOfTrfcLi);
            deValue.InterruptionSrc = static_cast<decltype(deValue.InterruptionSrc)>(rteValue.DriveAwayInfoWarnReqInterruptionSrc);
            deValue.ReqSrc = static_cast<decltype(deValue.ReqSrc)>(rteValue.DriveAwayInfoWarnReqReqSrc);
            deValue.TiToDrvCntDwnTi = static_cast<decltype(deValue.TiToDrvCntDwnTi)>(rteValue.DriveAwayInfoWarnReqTiToDrvCntDwnTi);
            deValue.VisWarnReq = static_cast<decltype(deValue.VisWarnReq)>(rteValue.DriveAwayInfoWarnReqVisWarnReq);

            DriveAwayInfoWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DriveAwayInfoWarnReq (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igDriveAwayInfoWarnReq_mrx, length, static_cast<unsigned long>(sizeof(DriveAwayInfoWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isDrvrCtrlOfPassSeatFrntSts_mrx:
    {
        if (sizeof(DrvrCtrlOfPassSeatFrntSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrCtrlOfPassSeatFrntSts (%u)", ComConf_ComSignal_isDrvrCtrlOfPassSeatFrntSts_mrx);
            const DrvrCtrlOfPassSeatFrntSts& rteValue = *static_cast<const DrvrCtrlOfPassSeatFrntSts*>(buffer);
            autosar::DrvrCtrlOfPassSeatFrntSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrCtrlOfPassSeatFrntSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrCtrlOfPassSeatFrntSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDrvrCtrlOfPassSeatFrntSts_mrx, length, static_cast<unsigned long>(sizeof(DrvrCtrlOfPassSeatFrntSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDrvrDesDir_mrx:
    {
        if (sizeof(DrvrDesDir) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrDesDir (%u)", ComConf_ComSignal_isDrvrDesDir_mrx);
            const DrvrDesDir& rteValue = *static_cast<const DrvrDesDir*>(buffer);
            autosar::DrvrDesDir_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrDesDir_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrDesDir (%u). Got %zu , expected %lu", ComConf_ComSignal_isDrvrDesDir_mrx, length, static_cast<unsigned long>(sizeof(DrvrDesDir)));
        }
    }
    break;

    case ComConf_ComSignal_isDrvrMassgRunng_mrx:
    {
        if (sizeof(DrvrMassgRunng) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrMassgRunng (%u)", ComConf_ComSignal_isDrvrMassgRunng_mrx);
            const DrvrMassgRunng& rteValue = *static_cast<const DrvrMassgRunng*>(buffer);
            autosar::DrvrMassgRunng_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrMassgRunng_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrMassgRunng (%u). Got %zu , expected %lu", ComConf_ComSignal_isDrvrMassgRunng_mrx, length, static_cast<unsigned long>(sizeof(DrvrMassgRunng)));
        }
    }
    break;

    case ComConf_ComSignal_isDrvrPfmncLvl_mrx:
    {
        if (sizeof(DrvrPfmncLvl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrPfmncLvl (%u)", ComConf_ComSignal_isDrvrPfmncLvl_mrx);
            const DrvrPfmncLvl& rteValue = *static_cast<const DrvrPfmncLvl*>(buffer);
            autosar::DrvrPfmncLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrPfmncLvl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrPfmncLvl (%u). Got %zu , expected %lu", ComConf_ComSignal_isDrvrPfmncLvl_mrx, length, static_cast<unsigned long>(sizeof(DrvrPfmncLvl)));
        }
    }
    break;

    case ComConf_ComSignal_isDrvrPfmncSts_mrx:
    {
        if (sizeof(DrvrPfmncSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrPfmncSts (%u)", ComConf_ComSignal_isDrvrPfmncSts_mrx);
            const DrvrPfmncSts& rteValue = *static_cast<const DrvrPfmncSts*>(buffer);
            autosar::DrvrPfmncSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrPfmncSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrPfmncSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDrvrPfmncSts_mrx, length, static_cast<unsigned long>(sizeof(DrvrPfmncSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDrvrPfmncWarnReq_mrx:
    {
        if (sizeof(DrvrPfmncWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrPfmncWarnReq (%u)", ComConf_ComSignal_isDrvrPfmncWarnReq_mrx);
            const DrvrPfmncWarnReq& rteValue = *static_cast<const DrvrPfmncWarnReq*>(buffer);
            autosar::DrvrPfmncWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrPfmncWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrPfmncWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isDrvrPfmncWarnReq_mrx, length, static_cast<unsigned long>(sizeof(DrvrPfmncWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isDrvrSeatActvSpplFct_mrx:
    {
        if (sizeof(DrvrSeatActvSpplFct) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrSeatActvSpplFct (%u)", ComConf_ComSignal_isDrvrSeatActvSpplFct_mrx);
            const DrvrSeatActvSpplFct& rteValue = *static_cast<const DrvrSeatActvSpplFct*>(buffer);
            autosar::DrvrSeatActvSpplFct_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrSeatActvSpplFct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrSeatActvSpplFct (%u). Got %zu , expected %lu", ComConf_ComSignal_isDrvrSeatActvSpplFct_mrx, length, static_cast<unsigned long>(sizeof(DrvrSeatActvSpplFct)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDrvrSeatMassgFct_mrx:
    {
        if (sizeof(DrvrSeatMassgFct) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrSeatMassgFct (%u)", ComConf_ComSignalGroup_igDrvrSeatMassgFct_mrx);
            const DrvrSeatMassgFct& rteValue = *static_cast<const DrvrSeatMassgFct*>(buffer);
            autosar::DrvrSeatMassgFct_info::data_elem_type deValue;
            deValue.MassgInten = static_cast<decltype(deValue.MassgInten)>(rteValue.DrvrSeatMassgFctMassgInten);
            deValue.MassgProg = static_cast<decltype(deValue.MassgProg)>(rteValue.DrvrSeatMassgFctMassgProg);
            deValue.MassgSpdLvl = static_cast<decltype(deValue.MassgSpdLvl)>(rteValue.DrvrSeatMassgFctMassgSpdLvl);
            deValue.OnOff = static_cast<decltype(deValue.OnOff)>(rteValue.DrvrSeatMassgFctOnOff);

            DrvrSeatMassgFct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrSeatMassgFct (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igDrvrSeatMassgFct_mrx, length, static_cast<unsigned long>(sizeof(DrvrSeatMassgFct)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDrvrSeatSwtSts_mrx:
    {
        if (sizeof(DrvrSeatSwtSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrSeatSwtSts (%u)", ComConf_ComSignalGroup_igDrvrSeatSwtSts_mrx);
            const DrvrSeatSwtSts& rteValue = *static_cast<const DrvrSeatSwtSts*>(buffer);
            autosar::DrvrSeatSwtSts_info::data_elem_type deValue;
            deValue.DrvrSeatSwtAdjmtOfSpplFctHozlSts = static_cast<decltype(deValue.DrvrSeatSwtAdjmtOfSpplFctHozlSts)>(rteValue.DrvrSeatSwtAdjmtOfSpplFctHozlSts);
            deValue.DrvrSeatSwtAdjmtOfSpplFctVertSts = static_cast<decltype(deValue.DrvrSeatSwtAdjmtOfSpplFctVertSts)>(rteValue.DrvrSeatSwtAdjmtOfSpplFctVertSts);
            deValue.DrvrSeatSwtHeiFrntSts = static_cast<decltype(deValue.DrvrSeatSwtHeiFrntSts)>(rteValue.DrvrSeatSwtHeiFrntSts);
            deValue.DrvrSeatSwtHeiSts = static_cast<decltype(deValue.DrvrSeatSwtHeiSts)>(rteValue.DrvrSeatSwtHeiSts);
            deValue.DrvrSeatSwtInclSts = static_cast<decltype(deValue.DrvrSeatSwtInclSts)>(rteValue.DrvrSeatSwtInclSts);
            deValue.DrvrSeatSwtSelnOfSpplFctSts = static_cast<decltype(deValue.DrvrSeatSwtSelnOfSpplFctSts)>(rteValue.DrvrSeatSwtSelnOfSpplFctSts);
            deValue.DrvrSeatSwtSldSts = static_cast<decltype(deValue.DrvrSeatSwtSldSts)>(rteValue.DrvrSeatSwtSldSts);

            DrvrSeatSwtSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrSeatSwtSts (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igDrvrSeatSwtSts_mrx, length, static_cast<unsigned long>(sizeof(DrvrSeatSwtSts)));
        }
    }
    break;

    case ComConf_ComSignal_isDrvrWLoadLvl_mrx:
    {
        if (sizeof(DrvrWLoadLvl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DrvrWLoadLvl (%u)", ComConf_ComSignal_isDrvrWLoadLvl_mrx);
            const DrvrWLoadLvl& rteValue = *static_cast<const DrvrWLoadLvl*>(buffer);
            autosar::DrvrWLoadLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DrvrWLoadLvl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DrvrWLoadLvl (%u). Got %zu , expected %lu", ComConf_ComSignal_isDrvrWLoadLvl_mrx, length, static_cast<unsigned long>(sizeof(DrvrWLoadLvl)));
        }
    }
    break;

    case ComConf_ComSignal_isDstNotifSts_mrx:
    {
        if (sizeof(DstNotifSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DstNotifSts (%u)", ComConf_ComSignal_isDstNotifSts_mrx);
            const DstNotifSts& rteValue = *static_cast<const DstNotifSts*>(buffer);
            autosar::DstNotifSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DstNotifSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DstNotifSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isDstNotifSts_mrx, length, static_cast<unsigned long>(sizeof(DstNotifSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igDstToEmptyIndcd_mrx:
    {
        if (sizeof(DstToEmptyIndcd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DstToEmptyIndcd (%u)", ComConf_ComSignalGroup_igDstToEmptyIndcd_mrx);
            const DstToEmptyIndcd& rteValue = *static_cast<const DstToEmptyIndcd*>(buffer);
            autosar::DstToEmptyIndcd_info::data_elem_type deValue;
            deValue.DstToEmpty = static_cast<decltype(deValue.DstToEmpty)>(rteValue.DstToEmpty);
            deValue.DstUnit_ = static_cast<decltype(deValue.DstUnit_)>(rteValue.DstUnit);

            DstToEmptyIndcd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DstToEmptyIndcd (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igDstToEmptyIndcd_mrx, length, static_cast<unsigned long>(sizeof(DstToEmptyIndcd)));
        }
    }
    break;

    case ComConf_ComSignal_isDstToSrv_mrx:
    {
        if (sizeof(DstToSrv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DstToSrv (%u)", ComConf_ComSignal_isDstToSrv_mrx);
            const DstToSrv& rteValue = *static_cast<const DstToSrv*>(buffer);
            autosar::DstToSrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DstToSrv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DstToSrv (%u). Got %zu , expected %lu", ComConf_ComSignal_isDstToSrv_mrx, length, static_cast<unsigned long>(sizeof(DstToSrv)));
        }
    }
    break;

    case ComConf_ComSignal_isDstTrvld1_mrx:
    {
        if (sizeof(DstTrvld1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DstTrvld1 (%u)", ComConf_ComSignal_isDstTrvld1_mrx);
            const DstTrvld1& rteValue = *static_cast<const DstTrvld1*>(buffer);
            autosar::DstTrvld1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.1 + 0.0;
            DstTrvld1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DstTrvld1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isDstTrvld1_mrx, length, static_cast<unsigned long>(sizeof(DstTrvld1)));
        }
    }
    break;

    case ComConf_ComSignal_isDstTrvld2_mrx:
    {
        if (sizeof(DstTrvld2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DstTrvld2 (%u)", ComConf_ComSignal_isDstTrvld2_mrx);
            const DstTrvld2& rteValue = *static_cast<const DstTrvld2*>(buffer);
            autosar::DstTrvld2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.1 + 0.0;
            DstTrvld2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DstTrvld2 (%u). Got %zu , expected %lu", ComConf_ComSignal_isDstTrvld2_mrx, length, static_cast<unsigned long>(sizeof(DstTrvld2)));
        }
    }
    break;

    case ComConf_ComSignal_isDstTrvldHiResl_mrx:
    {
        if (sizeof(DstTrvldHiResl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DstTrvldHiResl (%u)", ComConf_ComSignal_isDstTrvldHiResl_mrx);
            const DstTrvldHiResl& rteValue = *static_cast<const DstTrvldHiResl*>(buffer);
            autosar::DstTrvldHiResl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DstTrvldHiResl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DstTrvldHiResl (%u). Got %zu , expected %lu", ComConf_ComSignal_isDstTrvldHiResl_mrx, length, static_cast<unsigned long>(sizeof(DstTrvldHiResl)));
        }
    }
    break;

    case ComConf_ComSignal_isBkpOfDstTrvld_mrx:
    {
        if (sizeof(BkpOfDstTrvld) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received BkpOfDstTrvld (%u)", ComConf_ComSignal_isBkpOfDstTrvld_mrx);
            const BkpOfDstTrvld& rteValue = *static_cast<const BkpOfDstTrvld*>(buffer);
            autosar::BkpOfDstTrvld_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 1.0 + 0.0;
            BkpOfDstTrvld_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for BkpOfDstTrvld (%u). Got %zu , expected %lu", ComConf_ComSignal_isBkpOfDstTrvld_mrx, length, static_cast<unsigned long>(sizeof(BkpOfDstTrvld)));
        }
    }
    break;

    case ComConf_ComSignal_isDstTrvldMst2_mrx:
    {
        if (sizeof(DstTrvldMst2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received DstTrvldMst2 (%u)", ComConf_ComSignal_isDstTrvldMst2_mrx);
            const DstTrvldMst2& rteValue = *static_cast<const DstTrvldMst2*>(buffer);
            autosar::DstTrvldMst2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            DstTrvldMst2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for DstTrvldMst2 (%u). Got %zu , expected %lu", ComConf_ComSignal_isDstTrvldMst2_mrx, length, static_cast<unsigned long>(sizeof(DstTrvldMst2)));
        }
    }
    break;

    case ComConf_ComSignal_isEmgyAsscSts_mrx:
    {
        if (sizeof(EmgyAsscSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EmgyAsscSts (%u)", ComConf_ComSignal_isEmgyAsscSts_mrx);
            const EmgyAsscSts& rteValue = *static_cast<const EmgyAsscSts*>(buffer);
            autosar::EmgyAsscSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EmgyAsscSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EmgyAsscSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isEmgyAsscSts_mrx, length, static_cast<unsigned long>(sizeof(EmgyAsscSts)));
        }
    }
    break;

    case ComConf_ComSignal_isEmgyVehWarnSts_mrx:
    {
        if (sizeof(EmgyVehWarnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EmgyVehWarnSts (%u)", ComConf_ComSignal_isEmgyVehWarnSts_mrx);
            const EmgyVehWarnSts& rteValue = *static_cast<const EmgyVehWarnSts*>(buffer);
            autosar::EmgyVehWarnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EmgyVehWarnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EmgyVehWarnSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isEmgyVehWarnSts_mrx, length, static_cast<unsigned long>(sizeof(EmgyVehWarnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isEngCooltIndcnReq_mrx:
    {
        if (sizeof(EngCooltIndcnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngCooltIndcnReq (%u)", ComConf_ComSignal_isEngCooltIndcnReq_mrx);
            const EngCooltIndcnReq& rteValue = *static_cast<const EngCooltIndcnReq*>(buffer);
            autosar::EngCooltIndcnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngCooltIndcnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngCooltIndcnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngCooltIndcnReq_mrx, length, static_cast<unsigned long>(sizeof(EngCooltIndcnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isEngCooltLvl_mrx:
    {
        if (sizeof(EngCooltLvl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngCooltLvl (%u)", ComConf_ComSignal_isEngCooltLvl_mrx);
            const EngCooltLvl& rteValue = *static_cast<const EngCooltLvl*>(buffer);
            autosar::EngCooltLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngCooltLvl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngCooltLvl (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngCooltLvl_mrx, length, static_cast<unsigned long>(sizeof(EngCooltLvl)));
        }
    }
    break;

    case ComConf_ComSignal_isEngFuCns_mrx:
    {
        if (sizeof(EngFuCns) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngFuCns (%u)", ComConf_ComSignal_isEngFuCns_mrx);
            const EngFuCns& rteValue = *static_cast<const EngFuCns*>(buffer);
            autosar::EngFuCns_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 200.0 + 0.0;
            EngFuCns_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngFuCns (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngFuCns_mrx, length, static_cast<unsigned long>(sizeof(EngFuCns)));
        }
    }
    break;

    case ComConf_ComSignal_isEngFuCnsFild_mrx:
    {
        if (sizeof(EngFuCnsFild) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngFuCnsFild (%u)", ComConf_ComSignal_isEngFuCnsFild_mrx);
            const EngFuCnsFild& rteValue = *static_cast<const EngFuCnsFild*>(buffer);
            autosar::EngFuCnsFild_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 200.0 + 0.0;
            EngFuCnsFild_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngFuCnsFild (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngFuCnsFild_mrx, length, static_cast<unsigned long>(sizeof(EngFuCnsFild)));
        }
    }
    break;

    case ComConf_ComSignal_isEngHrToSrv_mrx:
    {
        if (sizeof(EngHrToSrv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngHrToSrv (%u)", ComConf_ComSignal_isEngHrToSrv_mrx);
            const EngHrToSrv& rteValue = *static_cast<const EngHrToSrv*>(buffer);
            autosar::EngHrToSrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngHrToSrv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngHrToSrv (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngHrToSrv_mrx, length, static_cast<unsigned long>(sizeof(EngHrToSrv)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igEngNSafe_mrx:
    {
        if (sizeof(EngNSafe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngNSafe (%u)", ComConf_ComSignalGroup_igEngNSafe_mrx);
            const EngNSafe& rteValue = *static_cast<const EngNSafe*>(buffer);
            autosar::EngNSafe_info::data_elem_type deValue;
            deValue.EngN = static_cast<decltype(deValue.EngN)>( toUnsignedFromRaw<15>(rteValue.EngN) ) * 0.5 + 0.0;
            deValue.EngNChks = static_cast<decltype(deValue.EngNChks)>(rteValue.EngNChks);
            deValue.EngNCntr = static_cast<decltype(deValue.EngNCntr)>(rteValue.EngNCntr);
            deValue.EngNGrdt = static_cast<decltype(deValue.EngNGrdt)>( toSignedFromRaw<16>(rteValue.EngNSafeEngNGrdt) ) * 1.0 + 0.0;

            EngNSafe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngNSafe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igEngNSafe_mrx, length, static_cast<unsigned long>(sizeof(EngNSafe)));
        }
    }
    break;

    case ComConf_ComSignal_isEngOilLvl_mrx:
    {
        if (sizeof(EngOilLvl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngOilLvl (%u)", ComConf_ComSignal_isEngOilLvl_mrx);
            const EngOilLvl& rteValue = *static_cast<const EngOilLvl*>(buffer);
            autosar::EngOilLvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.5 + 0.0;
            EngOilLvl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngOilLvl (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngOilLvl_mrx, length, static_cast<unsigned long>(sizeof(EngOilLvl)));
        }
    }
    break;

    case ComConf_ComSignal_isEngOilLvlSts_mrx:
    {
        if (sizeof(EngOilLvlSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngOilLvlSts (%u)", ComConf_ComSignal_isEngOilLvlSts_mrx);
            const EngOilLvlSts& rteValue = *static_cast<const EngOilLvlSts*>(buffer);
            autosar::EngOilLvlSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngOilLvlSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngOilLvlSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngOilLvlSts_mrx, length, static_cast<unsigned long>(sizeof(EngOilLvlSts)));
        }
    }
    break;

    case ComConf_ComSignal_isEngOilPWarn_mrx:
    {
        if (sizeof(EngOilPWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngOilPWarn (%u)", ComConf_ComSignal_isEngOilPWarn_mrx);
            const EngOilPWarn& rteValue = *static_cast<const EngOilPWarn*>(buffer);
            autosar::EngOilPWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            EngOilPWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngOilPWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngOilPWarn_mrx, length, static_cast<unsigned long>(sizeof(EngOilPWarn)));
        }
    }
    break;

    case ComConf_ComSignal_isEngSpdDispd_mrx:
    {
        if (sizeof(EngSpdDispd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EngSpdDispd (%u)", ComConf_ComSignal_isEngSpdDispd_mrx);
            const EngSpdDispd& rteValue = *static_cast<const EngSpdDispd*>(buffer);
            autosar::EngSpdDispd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.5 + 0.0;
            EngSpdDispd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EngSpdDispd (%u). Got %zu , expected %lu", ComConf_ComSignal_isEngSpdDispd_mrx, length, static_cast<unsigned long>(sizeof(EngSpdDispd)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igEpbLampReqGroup_mrx:
    {
        if (sizeof(EpbLampReqGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EpbLampReqGroup (%u)", ComConf_ComSignalGroup_igEpbLampReqGroup_mrx);
            const EpbLampReqGroup& rteValue = *static_cast<const EpbLampReqGroup*>(buffer);
            autosar::EpbLampReq_info::data_elem_type deValue;
            deValue.EpbLampReq = static_cast<decltype(deValue.EpbLampReq)>(rteValue.EpbLampReq);
            deValue.EpbLampReqChks = static_cast<decltype(deValue.EpbLampReqChks)>( toUnsignedFromRaw<8>(rteValue.EpbLampReqChks) ) * 1.0 + 0.0;
            deValue.EpbLampReqCntr = static_cast<decltype(deValue.EpbLampReqCntr)>( toUnsignedFromRaw<4>(rteValue.EpbLampReqCntr) ) * 1.0 + 0.0;

            EpbLampReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EpbLampReqGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igEpbLampReqGroup_mrx, length, static_cast<unsigned long>(sizeof(EpbLampReqGroup)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igEscStGroup_mrx:
    {
        if (sizeof(EscStGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EscStGroup (%u)", ComConf_ComSignalGroup_igEscStGroup_mrx);
            const EscStGroup& rteValue = *static_cast<const EscStGroup*>(buffer);
            autosar::EscSt_info::data_elem_type deValue;
            deValue.EscSt = static_cast<decltype(deValue.EscSt)>(rteValue.EscSt);
            deValue.EscStChks = static_cast<decltype(deValue.EscStChks)>(rteValue.EscStChks);
            deValue.EscStCntr = static_cast<decltype(deValue.EscStCntr)>(rteValue.EscStCntr);

            EscSt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EscStGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igEscStGroup_mrx, length, static_cast<unsigned long>(sizeof(EscStGroup)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igEscWarnIndcnReqGroup_mrx:
    {
        if (sizeof(EscWarnIndcnReqGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received EscWarnIndcnReqGroup (%u)", ComConf_ComSignalGroup_igEscWarnIndcnReqGroup_mrx);
            const EscWarnIndcnReqGroup& rteValue = *static_cast<const EscWarnIndcnReqGroup*>(buffer);
            autosar::EscWarnIndcnReq_info::data_elem_type deValue;
            deValue.EscWarnIndcnReq_ = static_cast<decltype(deValue.EscWarnIndcnReq_)>(rteValue.EscWarnIndcnReq);
            deValue.EscWarnIndcnReqChks = static_cast<decltype(deValue.EscWarnIndcnReqChks)>(rteValue.EscWarnIndcnReqChks);
            deValue.EscWarnIndcnReqCntr = static_cast<decltype(deValue.EscWarnIndcnReqCntr)>(rteValue.EscWarnIndcnReqCntr);

            EscWarnIndcnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for EscWarnIndcnReqGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igEscWarnIndcnReqGroup_mrx, length, static_cast<unsigned long>(sizeof(EscWarnIndcnReqGroup)));
        }
    }
    break;

    case ComConf_ComSignal_isFRNetworkStatus_mrx:
    {
        if (sizeof(FRNetworkStatus) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FRNetworkStatus (%u)", ComConf_ComSignal_isFRNetworkStatus_mrx);
            const FRNetworkStatus& rteValue = *static_cast<const FRNetworkStatus*>(buffer);
            autosar::FRNetworkStatus_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FRNetworkStatus_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FRNetworkStatus (%u). Got %zu , expected %lu", ComConf_ComSignal_isFRNetworkStatus_mrx, length, static_cast<unsigned long>(sizeof(FRNetworkStatus)));
        }
    }
    break;

    case ComConf_ComSignal_isFltIndcrTurnLeFrnt_mrx:
    {
        if (sizeof(FltIndcrTurnLeFrnt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FltIndcrTurnLeFrnt (%u)", ComConf_ComSignal_isFltIndcrTurnLeFrnt_mrx);
            const FltIndcrTurnLeFrnt& rteValue = *static_cast<const FltIndcrTurnLeFrnt*>(buffer);
            autosar::FltIndcrTurnLeFrnt_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltIndcrTurnLeFrnt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FltIndcrTurnLeFrnt (%u). Got %zu , expected %lu", ComConf_ComSignal_isFltIndcrTurnLeFrnt_mrx, length, static_cast<unsigned long>(sizeof(FltIndcrTurnLeFrnt)));
        }
    }
    break;

    case ComConf_ComSignal_isFltIndcrTurnLeRe_mrx:
    {
        if (sizeof(FltIndcrTurnLeRe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FltIndcrTurnLeRe (%u)", ComConf_ComSignal_isFltIndcrTurnLeRe_mrx);
            const FltIndcrTurnLeRe& rteValue = *static_cast<const FltIndcrTurnLeRe*>(buffer);
            autosar::FltIndcrTurnLeRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltIndcrTurnLeRe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FltIndcrTurnLeRe (%u). Got %zu , expected %lu", ComConf_ComSignal_isFltIndcrTurnLeRe_mrx, length, static_cast<unsigned long>(sizeof(FltIndcrTurnLeRe)));
        }
    }
    break;

    case ComConf_ComSignal_isFltIndcrTurnRiFrnt_mrx:
    {
        if (sizeof(FltIndcrTurnRiFrnt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FltIndcrTurnRiFrnt (%u)", ComConf_ComSignal_isFltIndcrTurnRiFrnt_mrx);
            const FltIndcrTurnRiFrnt& rteValue = *static_cast<const FltIndcrTurnRiFrnt*>(buffer);
            autosar::FltIndcrTurnRiFrnt_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltIndcrTurnRiFrnt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FltIndcrTurnRiFrnt (%u). Got %zu , expected %lu", ComConf_ComSignal_isFltIndcrTurnRiFrnt_mrx, length, static_cast<unsigned long>(sizeof(FltIndcrTurnRiFrnt)));
        }
    }
    break;

    case ComConf_ComSignal_isFltIndcrTurnRiRe_mrx:
    {
        if (sizeof(FltIndcrTurnRiRe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FltIndcrTurnRiRe (%u)", ComConf_ComSignal_isFltIndcrTurnRiRe_mrx);
            const FltIndcrTurnRiRe& rteValue = *static_cast<const FltIndcrTurnRiRe*>(buffer);
            autosar::FltIndcrTurnRiRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltIndcrTurnRiRe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FltIndcrTurnRiRe (%u). Got %zu , expected %lu", ComConf_ComSignal_isFltIndcrTurnRiRe_mrx, length, static_cast<unsigned long>(sizeof(FltIndcrTurnRiRe)));
        }
    }
    break;

    case ComConf_ComSignal_isFltOfLiDaytiRunngRi_mrx:
    {
        if (sizeof(FltOfLiDaytiRunngRi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FltOfLiDaytiRunngRi (%u)", ComConf_ComSignal_isFltOfLiDaytiRunngRi_mrx);
            const FltOfLiDaytiRunngRi& rteValue = *static_cast<const FltOfLiDaytiRunngRi*>(buffer);
            autosar::FltOfLiDaytiRunngRi_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FltOfLiDaytiRunngRi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FltOfLiDaytiRunngRi (%u). Got %zu , expected %lu", ComConf_ComSignal_isFltOfLiDaytiRunngRi_mrx, length, static_cast<unsigned long>(sizeof(FltOfLiDaytiRunngRi)));
        }
    }
    break;

    case ComConf_ComSignal_isFrntAxleWarn_mrx:
    {
        if (sizeof(FrntAxleWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FrntAxleWarn (%u)", ComConf_ComSignal_isFrntAxleWarn_mrx);
            const FrntAxleWarn& rteValue = *static_cast<const FrntAxleWarn*>(buffer);
            autosar::FrntAxleWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FrntAxleWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FrntAxleWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isFrntAxleWarn_mrx, length, static_cast<unsigned long>(sizeof(FrntAxleWarn)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igFrntWiprLvrReq2_mrx:
    {
        if (sizeof(FrntWiprLvrReq2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FrntWiprLvrReq2 (%u)", ComConf_ComSignalGroup_igFrntWiprLvrReq2_mrx);
            const FrntWiprLvrReq2& rteValue = *static_cast<const FrntWiprLvrReq2*>(buffer);
            autosar::FrntWiprLvrReq2_info::data_elem_type deValue;
            deValue.FrntWiprLvrCmd1_ = static_cast<decltype(deValue.FrntWiprLvrCmd1_)>(rteValue.FrntWiprLvrReq2FrntWiprLvrCmd1);
            deValue.FrntWiprLvrCntr = static_cast<decltype(deValue.FrntWiprLvrCntr)>(rteValue.FrntWiprLvrReq2FrntWiprLvrCntr);
            deValue.FrntWiprLvrCrc = static_cast<decltype(deValue.FrntWiprLvrCrc)>(rteValue.FrntWiprLvrReq2FrntWiprLvrCrc);
            deValue.FrntWiprLvrQf = static_cast<decltype(deValue.FrntWiprLvrQf)>(rteValue.FrntWiprLvrReq2FrntWiprLvrQf);

            FrntWiprLvrReq2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FrntWiprLvrReq2 (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igFrntWiprLvrReq2_mrx, length, static_cast<unsigned long>(sizeof(FrntWiprLvrReq2)));
        }
    }
    break;

    case ComConf_ComSignal_isFuHeatrActv_mrx:
    {
        if (sizeof(FuHeatrActv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FuHeatrActv (%u)", ComConf_ComSignal_isFuHeatrActv_mrx);
            const FuHeatrActv& rteValue = *static_cast<const FuHeatrActv*>(buffer);
            autosar::FuHeatrActv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FuHeatrActv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FuHeatrActv (%u). Got %zu , expected %lu", ComConf_ComSignal_isFuHeatrActv_mrx, length, static_cast<unsigned long>(sizeof(FuHeatrActv)));
        }
    }
    break;

    case ComConf_ComSignal_isFuHeatrFuCns1_mrx:
    {
        if (sizeof(FuHeatrFuCns1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FuHeatrFuCns1 (%u)", ComConf_ComSignal_isFuHeatrFuCns1_mrx);
            const FuHeatrFuCns1& rteValue = *static_cast<const FuHeatrFuCns1*>(buffer);
            autosar::FuHeatrFuCns1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 200.0 + 0.0;
            FuHeatrFuCns1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FuHeatrFuCns1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isFuHeatrFuCns1_mrx, length, static_cast<unsigned long>(sizeof(FuHeatrFuCns1)));
        }
    }
    break;

    case ComConf_ComSignal_isFuHeatrFuCnsDurgCyc1_mrx:
    {
        if (sizeof(FuHeatrFuCnsDurgCyc1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FuHeatrFuCnsDurgCyc1 (%u)", ComConf_ComSignal_isFuHeatrFuCnsDurgCyc1_mrx);
            const FuHeatrFuCnsDurgCyc1& rteValue = *static_cast<const FuHeatrFuCnsDurgCyc1*>(buffer);
            autosar::FuHeatrFuCnsDurgCyc1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.01 + 0.0;
            FuHeatrFuCnsDurgCyc1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FuHeatrFuCnsDurgCyc1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isFuHeatrFuCnsDurgCyc1_mrx, length, static_cast<unsigned long>(sizeof(FuHeatrFuCnsDurgCyc1)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igFuLvlIndcd_mrx:
    {
        if (sizeof(FuLvlIndcd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FuLvlIndcd (%u)", ComConf_ComSignalGroup_igFuLvlIndcd_mrx);
            const FuLvlIndcd& rteValue = *static_cast<const FuLvlIndcd*>(buffer);
            autosar::FuLvlIndcd_info::data_elem_type deValue;
            deValue.FuLvlValFromFuTbl = static_cast<decltype(deValue.FuLvlValFromFuTbl)>( toUnsignedFromRaw<10>(rteValue.FuLvlIndcdVal) ) * 0.2 + 0.0;
            deValue.GenQF = static_cast<decltype(deValue.GenQF)>(rteValue.FuLvlIndcdQly);

            FuLvlIndcd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FuLvlIndcd (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igFuLvlIndcd_mrx, length, static_cast<unsigned long>(sizeof(FuLvlIndcd)));
        }
    }
    break;

    case ComConf_ComSignal_isFuLvlLoIndcn_mrx:
    {
        if (sizeof(FuLvlLoIndcn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FuLvlLoIndcn (%u)", ComConf_ComSignal_isFuLvlLoIndcn_mrx);
            const FuLvlLoIndcn& rteValue = *static_cast<const FuLvlLoIndcn*>(buffer);
            autosar::FuLvlLoIndcn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FuLvlLoIndcn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FuLvlLoIndcn (%u). Got %zu , expected %lu", ComConf_ComSignal_isFuLvlLoIndcn_mrx, length, static_cast<unsigned long>(sizeof(FuLvlLoIndcn)));
        }
    }
    break;

    case ComConf_ComSignal_isFuLvlLoWarn_mrx:
    {
        if (sizeof(FuLvlLoWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received FuLvlLoWarn (%u)", ComConf_ComSignal_isFuLvlLoWarn_mrx);
            const FuLvlLoWarn& rteValue = *static_cast<const FuLvlLoWarn*>(buffer);
            autosar::FuLvlLoWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            FuLvlLoWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for FuLvlLoWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isFuLvlLoWarn_mrx, length, static_cast<unsigned long>(sizeof(FuLvlLoWarn)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igGearIndcnRec_mrx:
    {
        if (sizeof(GearIndcnRec) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received GearIndcnRec (%u)", ComConf_ComSignalGroup_igGearIndcnRec_mrx);
            const GearIndcnRec& rteValue = *static_cast<const GearIndcnRec*>(buffer);
            autosar::GearIndcnRec_info::data_elem_type deValue;
            deValue.GearIndcn = static_cast<decltype(deValue.GearIndcn)>(rteValue.GearIndcn);
            deValue.GearShiftIndcn_ = static_cast<decltype(deValue.GearShiftIndcn_)>(rteValue.GearShiftIndcn);
            deValue.GearTarIndcn = static_cast<decltype(deValue.GearTarIndcn)>(rteValue.GearTarIndcn);

            GearIndcnRec_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for GearIndcnRec (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igGearIndcnRec_mrx, length, static_cast<unsigned long>(sizeof(GearIndcnRec)));
        }
    }
    break;

    case ComConf_ComSignal_isHmiCenForDrvrHmi_mrx:
    {
        if (sizeof(HmiCenForDrvrHmi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HmiCenForDrvrHmi (%u)", ComConf_ComSignal_isHmiCenForDrvrHmi_mrx);
            const HmiCenForDrvrHmi& rteValue = *static_cast<const HmiCenForDrvrHmi*>(buffer);
            autosar::HmiCenForDrvrHmi_info::data_elem_type deValue;
            for (unsigned int i=0; i<deValue.size(); ++i) deValue[i] = static_cast<autosar::HmiCenForDrvrHmi_info::data_elem_type::value_type>( rteValue[i] );
            HmiCenForDrvrHmi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HmiCenForDrvrHmi (%u). Got %zu , expected %lu", ComConf_ComSignal_isHmiCenForDrvrHmi_mrx, length, static_cast<unsigned long>(sizeof(HmiCenForDrvrHmi)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igHMIDefrstSts_mrx:
    {
        if (sizeof(HMIDefrstSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HMIDefrstSts (%u)", ComConf_ComSignalGroup_igHMIDefrstSts_mrx);
            const HMIDefrstSts& rteValue = *static_cast<const HMIDefrstSts*>(buffer);
            autosar::HmiDefrstElecSts_info::data_elem_type deValue;
            deValue.Frnt = static_cast<decltype(deValue.Frnt)>(rteValue.HmiDefrstFrntSts);
            deValue.Mirrr = static_cast<decltype(deValue.Mirrr)>(rteValue.HmiMirrDefrstSts);
            deValue.Re = static_cast<decltype(deValue.Re)>(rteValue.HmiDfrstReSts);

            HmiDefrstElecSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HMIDefrstSts (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igHMIDefrstSts_mrx, length, static_cast<unsigned long>(sizeof(HMIDefrstSts)));
        }
    }
    break;

    case ComConf_ComSignal_isHoodSts_mrx:
    {
        if (sizeof(HoodSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HoodSts (%u)", ComConf_ComSignal_isHoodSts_mrx);
            const HoodSts& rteValue = *static_cast<const HoodSts*>(buffer);
            autosar::HoodSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HoodSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HoodSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isHoodSts_mrx, length, static_cast<unsigned long>(sizeof(HoodSts)));
        }
    }
    break;

    case ComConf_ComSignal_isHudActvSts_mrx:
    {
        if (sizeof(HudActvSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HudActvSts (%u)", ComConf_ComSignal_isHudActvSts_mrx);
            const HudActvSts& rteValue = *static_cast<const HudActvSts*>(buffer);
            autosar::HudActvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HudActvSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HudActvSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isHudActvSts_mrx, length, static_cast<unsigned long>(sizeof(HudActvSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igHudDiagc_mrx:
    {
        if (sizeof(HudDiagc) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HudDiagc (%u)", ComConf_ComSignalGroup_igHudDiagc_mrx);
            const HudDiagc& rteValue = *static_cast<const HudDiagc*>(buffer);
            autosar::HudDiagc_info::data_elem_type deValue;
            deValue.HudCircShoSts = static_cast<decltype(deValue.HudCircShoSts)>(rteValue.HudDiagcHudCircShoSts);
            deValue.HudCricOpenSts = static_cast<decltype(deValue.HudCricOpenSts)>(rteValue.HudDiagcHudCricOpenSts);
            deValue.HudTSts = static_cast<decltype(deValue.HudTSts)>(rteValue.HudDiagcHudTSts);
            deValue.ImgHudErrSts = static_cast<decltype(deValue.ImgHudErrSts)>(rteValue.HudDiagcImgHudErrSts);
            deValue.ImgHudTmpNotAvlSts = static_cast<decltype(deValue.ImgHudTmpNotAvlSts)>(rteValue.HudDiagcImgHudTmpNotAvlSts);

            HudDiagc_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HudDiagc (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igHudDiagc_mrx, length, static_cast<unsigned long>(sizeof(HudDiagc)));
        }
    }
    break;

    case ComConf_ComSignal_isHudSts_mrx:
    {
        if (sizeof(HudSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HudSts (%u)", ComConf_ComSignal_isHudSts_mrx);
            const HudSts& rteValue = *static_cast<const HudSts*>(buffer);
            autosar::HudSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HudSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HudSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isHudSts_mrx, length, static_cast<unsigned long>(sizeof(HudSts)));
        }
    }
    break;

    case ComConf_ComSignal_isHvacAirMFlowEstimd_mrx:
    {
        if (sizeof(HvacAirMFlowEstimd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HvacAirMFlowEstimd (%u)", ComConf_ComSignal_isHvacAirMFlowEstimd_mrx);
            const HvacAirMFlowEstimd& rteValue = *static_cast<const HvacAirMFlowEstimd*>(buffer);
            autosar::HvacAirMFlowEstimd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 1.0 + 0.0;
            HvacAirMFlowEstimd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HvacAirMFlowEstimd (%u). Got %zu , expected %lu", ComConf_ComSignal_isHvacAirMFlowEstimd_mrx, length, static_cast<unsigned long>(sizeof(HvacAirMFlowEstimd)));
        }
    }
    break;

    case ComConf_ComSignal_isHznRstExtd_mrx:
    {
        if (sizeof(HznRstExtd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HznRstExtd (%u)", ComConf_ComSignal_isHznRstExtd_mrx);
            const HznRstExtd& rteValue = *static_cast<const HznRstExtd*>(buffer);
            autosar::HznRstExtd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HznRstExtd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HznRstExtd (%u). Got %zu , expected %lu", ComConf_ComSignal_isHznRstExtd_mrx, length, static_cast<unsigned long>(sizeof(HznRstExtd)));
        }
    }
    break;

    case ComConf_ComSignal_isHzrdLiWarnSts_mrx:
    {
        if (sizeof(HzrdLiWarnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received HzrdLiWarnSts (%u)", ComConf_ComSignal_isHzrdLiWarnSts_mrx);
            const HzrdLiWarnSts& rteValue = *static_cast<const HzrdLiWarnSts*>(buffer);
            autosar::HzrdLiWarnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            HzrdLiWarnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for HzrdLiWarnSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isHzrdLiWarnSts_mrx, length, static_cast<unsigned long>(sizeof(HzrdLiWarnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isIndcnOfParkAssiSts_mrx:
    {
        if (sizeof(IndcnOfParkAssiSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received IndcnOfParkAssiSts (%u)", ComConf_ComSignal_isIndcnOfParkAssiSts_mrx);
            const IndcnOfParkAssiSts& rteValue = *static_cast<const IndcnOfParkAssiSts*>(buffer);
            autosar::IndcnOfParkAssiSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IndcnOfParkAssiSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for IndcnOfParkAssiSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isIndcnOfParkAssiSts_mrx, length, static_cast<unsigned long>(sizeof(IndcnOfParkAssiSts)));
        }
    }
    break;

    case ComConf_ComSignal_isIndcnOfPrkgAutSts_mrx:
    {
        if (sizeof(IndcnOfPrkgAutSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received IndcnOfPrkgAutSts (%u)", ComConf_ComSignal_isIndcnOfPrkgAutSts_mrx);
            const IndcnOfPrkgAutSts& rteValue = *static_cast<const IndcnOfPrkgAutSts*>(buffer);
            autosar::IndcnOfPrkgAutSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IndcnOfPrkgAutSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for IndcnOfPrkgAutSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isIndcnOfPrkgAutSts_mrx, length, static_cast<unsigned long>(sizeof(IndcnOfPrkgAutSts)));
        }
    }
    break;

    case ComConf_ComSignal_isIndcrDisp1WdSts_mrx:
    {
        if (sizeof(IndcrDisp1WdSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received IndcrDisp1WdSts (%u)", ComConf_ComSignal_isIndcrDisp1WdSts_mrx);
            const IndcrDisp1WdSts& rteValue = *static_cast<const IndcrDisp1WdSts*>(buffer);
            autosar::IndcrDisp1WdSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IndcrDisp1WdSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for IndcrDisp1WdSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isIndcrDisp1WdSts_mrx, length, static_cast<unsigned long>(sizeof(IndcrDisp1WdSts)));
        }
    }
    break;

    case ComConf_ComSignal_isIndcrTurnSts1WdSts_mrx:
    {
        if (sizeof(IndcrTurnSts1WdSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received IndcrTurnSts1WdSts (%u)", ComConf_ComSignal_isIndcrTurnSts1WdSts_mrx);
            const IndcrTurnSts1WdSts& rteValue = *static_cast<const IndcrTurnSts1WdSts*>(buffer);
            autosar::IndcrTurnSts1WdSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IndcrTurnSts1WdSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for IndcrTurnSts1WdSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isIndcrTurnSts1WdSts_mrx, length, static_cast<unsigned long>(sizeof(IndcrTurnSts1WdSts)));
        }
    }
    break;

    case ComConf_ComSignal_isIniValSigCfgIDBackboneFR_mrx:
    {
        if (sizeof(IniValSigCfgIDBackboneFR) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received IniValSigCfgIDBackboneFR (%u)", ComConf_ComSignal_isIniValSigCfgIDBackboneFR_mrx);
            const IniValSigCfgIDBackboneFR& rteValue = *static_cast<const IniValSigCfgIDBackboneFR*>(buffer);
            autosar::IniValSigCfgIDBackboneFR_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IniValSigCfgIDBackboneFR_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for IniValSigCfgIDBackboneFR (%u). Got %zu , expected %lu", ComConf_ComSignal_isIniValSigCfgIDBackboneFR_mrx, length, static_cast<unsigned long>(sizeof(IniValSigCfgIDBackboneFR)));
        }
    }
    break;

    case ComConf_ComSignal_isIntrBriSts_mrx:
    {
        if (sizeof(IntrBriSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received IntrBriSts (%u)", ComConf_ComSignal_isIntrBriSts_mrx);
            const IntrBriSts& rteValue = *static_cast<const IntrBriSts*>(buffer);
            autosar::IntrBriSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            IntrBriSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for IntrBriSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isIntrBriSts_mrx, length, static_cast<unsigned long>(sizeof(IntrBriSts)));
        }
    }
    break;

    case ComConf_ComSignal_isKeyLostWarnIndcn_mrx:
    {
        if (sizeof(KeyLostWarnIndcn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received KeyLostWarnIndcn (%u)", ComConf_ComSignal_isKeyLostWarnIndcn_mrx);
            const KeyLostWarnIndcn& rteValue = *static_cast<const KeyLostWarnIndcn*>(buffer);
            autosar::KeyLostWarnIndcn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            KeyLostWarnIndcn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for KeyLostWarnIndcn (%u). Got %zu , expected %lu", ComConf_ComSignal_isKeyLostWarnIndcn_mrx, length, static_cast<unsigned long>(sizeof(KeyLostWarnIndcn)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igKeyReadStsToProfCtrl_mrx:
    {
        if (sizeof(KeyReadStsToProfCtrl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received KeyReadStsToProfCtrl (%u)", ComConf_ComSignalGroup_igKeyReadStsToProfCtrl_mrx);
            const KeyReadStsToProfCtrl& rteValue = *static_cast<const KeyReadStsToProfCtrl*>(buffer);
            autosar::KeyReadStsToProfCtrl_info::data_elem_type deValue;
            deValue.Boolean = static_cast<decltype(deValue.Boolean)>(rteValue.KeyReadStsToProfCtrlBoolean);
            deValue.KeyId = static_cast<decltype(deValue.KeyId)>(rteValue.KeyReadStsToProfCtrlKeyId);

            KeyReadStsToProfCtrl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for KeyReadStsToProfCtrl (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igKeyReadStsToProfCtrl_mrx, length, static_cast<unsigned long>(sizeof(KeyReadStsToProfCtrl)));
        }
    }
    break;

    case ComConf_ComSignal_isKeyRmnIndcn_mrx:
    {
        if (sizeof(KeyRmnIndcn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received KeyRmnIndcn (%u)", ComConf_ComSignal_isKeyRmnIndcn_mrx);
            const KeyRmnIndcn& rteValue = *static_cast<const KeyRmnIndcn*>(buffer);
            autosar::KeyRmnIndcn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            KeyRmnIndcn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for KeyRmnIndcn (%u). Got %zu , expected %lu", ComConf_ComSignal_isKeyRmnIndcn_mrx, length, static_cast<unsigned long>(sizeof(KeyRmnIndcn)));
        }
    }
    break;

    case ComConf_ComSignal_isKeySpdWarn_mrx:
    {
        if (sizeof(KeySpdWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received KeySpdWarn (%u)", ComConf_ComSignal_isKeySpdWarn_mrx);
            const KeySpdWarn& rteValue = *static_cast<const KeySpdWarn*>(buffer);
            autosar::KeySpdWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            KeySpdWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for KeySpdWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isKeySpdWarn_mrx, length, static_cast<unsigned long>(sizeof(KeySpdWarn)));
        }
    }
    break;

    case ComConf_ComSignal_isLaneChgWarnSts_mrx:
    {
        if (sizeof(LaneChgWarnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LaneChgWarnSts (%u)", ComConf_ComSignal_isLaneChgWarnSts_mrx);
            const LaneChgWarnSts& rteValue = *static_cast<const LaneChgWarnSts*>(buffer);
            autosar::LaneChgWarnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LaneChgWarnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LaneChgWarnSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isLaneChgWarnSts_mrx, length, static_cast<unsigned long>(sizeof(LaneChgWarnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isLaneDetnStsForAutDrv_mrx:
    {
        if (sizeof(LaneDetnStsForAutDrv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LaneDetnStsForAutDrv (%u)", ComConf_ComSignal_isLaneDetnStsForAutDrv_mrx);
            const LaneDetnStsForAutDrv& rteValue = *static_cast<const LaneDetnStsForAutDrv*>(buffer);
            autosar::LaneDetnStsForAutDrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LaneDetnStsForAutDrv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LaneDetnStsForAutDrv (%u). Got %zu , expected %lu", ComConf_ComSignal_isLaneDetnStsForAutDrv_mrx, length, static_cast<unsigned long>(sizeof(LaneDetnStsForAutDrv)));
        }
    }
    break;

    case ComConf_ComSignal_isLaneDetnStsForLaneKeepAid_mrx:
    {
        if (sizeof(LaneDetnStsForLaneKeepAid) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LaneDetnStsForLaneKeepAid (%u)", ComConf_ComSignal_isLaneDetnStsForLaneKeepAid_mrx);
            const LaneDetnStsForLaneKeepAid& rteValue = *static_cast<const LaneDetnStsForLaneKeepAid*>(buffer);
            autosar::LaneDetnStsForLaneKeepAid_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LaneDetnStsForLaneKeepAid_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LaneDetnStsForLaneKeepAid (%u). Got %zu , expected %lu", ComConf_ComSignal_isLaneDetnStsForLaneKeepAid_mrx, length, static_cast<unsigned long>(sizeof(LaneDetnStsForLaneKeepAid)));
        }
    }
    break;

    case ComConf_ComSignal_isLaneKeepAidSts_mrx:
    {
        if (sizeof(LaneKeepAidSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LaneKeepAidSts (%u)", ComConf_ComSignal_isLaneKeepAidSts_mrx);
            const LaneKeepAidSts& rteValue = *static_cast<const LaneKeepAidSts*>(buffer);
            autosar::LaneKeepAidSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LaneKeepAidSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LaneKeepAidSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isLaneKeepAidSts_mrx, length, static_cast<unsigned long>(sizeof(LaneKeepAidSts)));
        }
    }
    break;

    case ComConf_ComSignal_isLatMovmtWarn_mrx:
    {
        if (sizeof(LatMovmtWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LatMovmtWarn (%u)", ComConf_ComSignal_isLatMovmtWarn_mrx);
            const LatMovmtWarn& rteValue = *static_cast<const LatMovmtWarn*>(buffer);
            autosar::LatMovmtWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LatMovmtWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LatMovmtWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isLatMovmtWarn_mrx, length, static_cast<unsigned long>(sizeof(LatMovmtWarn)));
        }
    }
    break;

    case ComConf_ComSignal_isLcmaOn1_mrx:
    {
        if (sizeof(LcmaOn1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LcmaOn1 (%u)", ComConf_ComSignal_isLcmaOn1_mrx);
            const LcmaOn1& rteValue = *static_cast<const LcmaOn1*>(buffer);
            autosar::LcmaOn1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LcmaOn1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LcmaOn1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isLcmaOn1_mrx, length, static_cast<unsigned long>(sizeof(LcmaOn1)));
        }
    }
    break;

    case ComConf_ComSignal_isLiDrvrFltIndcrTurn_mrx:
    {
        if (sizeof(LiDrvrFltIndcrTurn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LiDrvrFltIndcrTurn (%u)", ComConf_ComSignal_isLiDrvrFltIndcrTurn_mrx);
            const LiDrvrFltIndcrTurn& rteValue = *static_cast<const LiDrvrFltIndcrTurn*>(buffer);
            autosar::LiDrvrFltIndcrTurn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LiDrvrFltIndcrTurn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LiDrvrFltIndcrTurn (%u). Got %zu , expected %lu", ComConf_ComSignal_isLiDrvrFltIndcrTurn_mrx, length, static_cast<unsigned long>(sizeof(LiDrvrFltIndcrTurn)));
        }
    }
    break;

    case ComConf_ComSignal_isLiLvrSwt1_mrx:
    {
        if (sizeof(LiLvrSwt1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LiLvrSwt1 (%u)", ComConf_ComSignal_isLiLvrSwt1_mrx);
            const LiLvrSwt1& rteValue = *static_cast<const LiLvrSwt1*>(buffer);
            autosar::LiLvrSwt1Req_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LiLvrSwt1Req_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LiLvrSwt1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isLiLvrSwt1_mrx, length, static_cast<unsigned long>(sizeof(LiLvrSwt1)));
        }
    }
    break;

    case ComConf_ComSignal_isLiPassFltIndcrTurn_mrx:
    {
        if (sizeof(LiPassFltIndcrTurn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LiPassFltIndcrTurn (%u)", ComConf_ComSignal_isLiPassFltIndcrTurn_mrx);
            const LiPassFltIndcrTurn& rteValue = *static_cast<const LiPassFltIndcrTurn*>(buffer);
            autosar::LiPassFltIndcrTurn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LiPassFltIndcrTurn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LiPassFltIndcrTurn (%u). Got %zu , expected %lu", ComConf_ComSignal_isLiPassFltIndcrTurn_mrx, length, static_cast<unsigned long>(sizeof(LiPassFltIndcrTurn)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igLockgCenSts_mrx:
    {
        if (sizeof(LockgCenSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LockgCenSts (%u)", ComConf_ComSignalGroup_igLockgCenSts_mrx);
            const LockgCenSts& rteValue = *static_cast<const LockgCenSts*>(buffer);
            autosar::LockgCenSts_info::data_elem_type deValue;
            deValue.LockSt = static_cast<decltype(deValue.LockSt)>(rteValue.LockgCenStsLockSt);
            deValue.TrigSrc = static_cast<decltype(deValue.TrigSrc)>(rteValue.LockgCenTrigSrc);
            deValue.UpdEve = static_cast<decltype(deValue.UpdEve)>(rteValue.LockgCenStsUpdEve);

            LockgCenSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LockgCenSts (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igLockgCenSts_mrx, length, static_cast<unsigned long>(sizeof(LockgCenSts)));
        }
    }
    break;

    case ComConf_ComSignal_isLockgCenStsForUsrFb_mrx:
    {
        if (sizeof(LockgCenStsForUsrFb) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LockgCenStsForUsrFb (%u)", ComConf_ComSignal_isLockgCenStsForUsrFb_mrx);
            const LockgCenStsForUsrFb& rteValue = *static_cast<const LockgCenStsForUsrFb*>(buffer);
            autosar::LockgCenStsForUsrFb_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LockgCenStsForUsrFb_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LockgCenStsForUsrFb (%u). Got %zu , expected %lu", ComConf_ComSignal_isLockgCenStsForUsrFb_mrx, length, static_cast<unsigned long>(sizeof(LockgCenStsForUsrFb)));
        }
    }
    break;

    case ComConf_ComSignal_isLockgPrsnlSts_mrx:
    {
        if (sizeof(LockgPrsnlSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LockgPrsnlSts (%u)", ComConf_ComSignal_isLockgPrsnlSts_mrx);
            const LockgPrsnlSts& rteValue = *static_cast<const LockgPrsnlSts*>(buffer);
            autosar::LockgPrsnlSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LockgPrsnlSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LockgPrsnlSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isLockgPrsnlSts_mrx, length, static_cast<unsigned long>(sizeof(LockgPrsnlSts)));
        }
    }
    break;

    case ComConf_ComSignal_isLvlOfClimaCmft_mrx:
    {
        if (sizeof(LvlOfClimaCmft) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received LvlOfClimaCmft (%u)", ComConf_ComSignal_isLvlOfClimaCmft_mrx);
            const LvlOfClimaCmft& rteValue = *static_cast<const LvlOfClimaCmft*>(buffer);
            autosar::LvlOfClimaCmft_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            LvlOfClimaCmft_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for LvlOfClimaCmft (%u). Got %zu , expected %lu", ComConf_ComSignal_isLvlOfClimaCmft_mrx, length, static_cast<unsigned long>(sizeof(LvlOfClimaCmft)));
        }
    }
    break;

    case ComConf_ComSignal_isMemBtnSound_mrx:
    {
        if (sizeof(MemBtnSound) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received MemBtnSound (%u)", ComConf_ComSignal_isMemBtnSound_mrx);
            const MemBtnSound& rteValue = *static_cast<const MemBtnSound*>(buffer);
            autosar::MemBtnSound_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MemBtnSound_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for MemBtnSound (%u). Got %zu , expected %lu", ComConf_ComSignal_isMemBtnSound_mrx, length, static_cast<unsigned long>(sizeof(MemBtnSound)));
        }
    }
    break;

    case ComConf_ComSignal_isMirrDwnStsAtDrvr_mrx:
    {
        if (sizeof(MirrDwnStsAtDrvr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received MirrDwnStsAtDrvr (%u)", ComConf_ComSignal_isMirrDwnStsAtDrvr_mrx);
            const MirrDwnStsAtDrvr& rteValue = *static_cast<const MirrDwnStsAtDrvr*>(buffer);
            autosar::MirrDwnStsAtDrvr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MirrDwnStsAtDrvr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for MirrDwnStsAtDrvr (%u). Got %zu , expected %lu", ComConf_ComSignal_isMirrDwnStsAtDrvr_mrx, length, static_cast<unsigned long>(sizeof(MirrDwnStsAtDrvr)));
        }
    }
    break;

    case ComConf_ComSignal_isMirrDwnStsAtPass_mrx:
    {
        if (sizeof(MirrDwnStsAtPass) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received MirrDwnStsAtPass (%u)", ComConf_ComSignal_isMirrDwnStsAtPass_mrx);
            const MirrDwnStsAtPass& rteValue = *static_cast<const MirrDwnStsAtPass*>(buffer);
            autosar::MirrDwnStsAtPass_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MirrDwnStsAtPass_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for MirrDwnStsAtPass (%u). Got %zu , expected %lu", ComConf_ComSignal_isMirrDwnStsAtPass_mrx, length, static_cast<unsigned long>(sizeof(MirrDwnStsAtPass)));
        }
    }
    break;

    case ComConf_ComSignal_isMirrFoldStsAtDrvr_mrx:
    {
        if (sizeof(MirrFoldStsAtDrvr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received MirrFoldStsAtDrvr (%u)", ComConf_ComSignal_isMirrFoldStsAtDrvr_mrx);
            const MirrFoldStsAtDrvr& rteValue = *static_cast<const MirrFoldStsAtDrvr*>(buffer);
            autosar::MirrFoldStsAtDrvr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MirrFoldStsAtDrvr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for MirrFoldStsAtDrvr (%u). Got %zu , expected %lu", ComConf_ComSignal_isMirrFoldStsAtDrvr_mrx, length, static_cast<unsigned long>(sizeof(MirrFoldStsAtDrvr)));
        }
    }
    break;

    case ComConf_ComSignal_isMirrFoldStsAtPass_mrx:
    {
        if (sizeof(MirrFoldStsAtPass) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received MirrFoldStsAtPass (%u)", ComConf_ComSignal_isMirrFoldStsAtPass_mrx);
            const MirrFoldStsAtPass& rteValue = *static_cast<const MirrFoldStsAtPass*>(buffer);
            autosar::MirrFoldStsAtPass_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MirrFoldStsAtPass_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for MirrFoldStsAtPass (%u). Got %zu , expected %lu", ComConf_ComSignal_isMirrFoldStsAtPass_mrx, length, static_cast<unsigned long>(sizeof(MirrFoldStsAtPass)));
        }
    }
    break;

    case ComConf_ComSignal_isMstCfgIDBackboneFR_mrx:
    {
        if (sizeof(MstCfgIDBackboneFR) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received MstCfgIDBackboneFR (%u)", ComConf_ComSignal_isMstCfgIDBackboneFR_mrx);
            const MstCfgIDBackboneFR& rteValue = *static_cast<const MstCfgIDBackboneFR*>(buffer);
            autosar::MstCfgIDBackboneFR_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            MstCfgIDBackboneFR_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for MstCfgIDBackboneFR (%u). Got %zu , expected %lu", ComConf_ComSignal_isMstCfgIDBackboneFR_mrx, length, static_cast<unsigned long>(sizeof(MstCfgIDBackboneFR)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igNetCtrlrActvt_mrx:
    {
        if (sizeof(NetCtrlrActvt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received NetCtrlrActvt (%u)", ComConf_ComSignalGroup_igNetCtrlrActvt_mrx);
            const NetCtrlrActvt& rteValue = *static_cast<const NetCtrlrActvt*>(buffer);
            autosar::NetCtrlrActvt_info::data_elem_type deValue;
            deValue.Prio = static_cast<decltype(deValue.Prio)>(rteValue.NetCtrlrActvtPrio);
            deValue.ResourceGroup = static_cast<decltype(deValue.ResourceGroup)>(rteValue.NetCtrlrActvtResourceGroup);

            NetCtrlrActvt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for NetCtrlrActvt (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igNetCtrlrActvt_mrx, length, static_cast<unsigned long>(sizeof(NetCtrlrActvt)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igNetTelmActvt_mrx:
    {
        if (sizeof(NetTelmActvt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received NetTelmActvt (%u)", ComConf_ComSignalGroup_igNetTelmActvt_mrx);
            const NetTelmActvt& rteValue = *static_cast<const NetTelmActvt*>(buffer);
            autosar::NetTelmActvt_info::data_elem_type deValue;
            deValue.Prio = static_cast<decltype(deValue.Prio)>(rteValue.NetTelmActvtPrio);
            deValue.ResourceGroup = static_cast<decltype(deValue.ResourceGroup)>(rteValue.NetTelmActvtResourceGroup);

            NetTelmActvt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for NetTelmActvt (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igNetTelmActvt_mrx, length, static_cast<unsigned long>(sizeof(NetTelmActvt)));
        }
    }
    break;

    case ComConf_ComSignal_isNewTripCdn_mrx:
    {
        if (sizeof(NewTripCdn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received NewTripCdn (%u)", ComConf_ComSignal_isNewTripCdn_mrx);
            const NewTripCdn& rteValue = *static_cast<const NewTripCdn*>(buffer);
            autosar::NewTripCdn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NewTripCdn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for NewTripCdn (%u). Got %zu , expected %lu", ComConf_ComSignal_isNewTripCdn_mrx, length, static_cast<unsigned long>(sizeof(NewTripCdn)));
        }
    }
    break;

    case ComConf_ComSignal_isNoEntryWarnReq_mrx:
    {
        if (sizeof(NoEntryWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received NoEntryWarnReq (%u)", ComConf_ComSignal_isNoEntryWarnReq_mrx);
            const NoEntryWarnReq& rteValue = *static_cast<const NoEntryWarnReq*>(buffer);
            autosar::NoEntryWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NoEntryWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for NoEntryWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isNoEntryWarnReq_mrx, length, static_cast<unsigned long>(sizeof(NoEntryWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isNotifChkDistbn_mrx:
    {
        if (sizeof(NotifChkDistbn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received NotifChkDistbn (%u)", ComConf_ComSignal_isNotifChkDistbn_mrx);
            const NotifChkDistbn& rteValue = *static_cast<const NotifChkDistbn*>(buffer);
            autosar::NotifChkDistbn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NotifChkDistbn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for NotifChkDistbn (%u). Got %zu , expected %lu", ComConf_ComSignal_isNotifChkDistbn_mrx, length, static_cast<unsigned long>(sizeof(NotifChkDistbn)));
        }
    }
    break;

    case ComConf_ComSignal_isNrOfBltAppld_mrx:
    {
        if (sizeof(NrOfBltAppld) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received NrOfBltAppld (%u)", ComConf_ComSignal_isNrOfBltAppld_mrx);
            const NrOfBltAppld& rteValue = *static_cast<const NrOfBltAppld*>(buffer);
            autosar::NrOfBltAppld_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NrOfBltAppld_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for NrOfBltAppld (%u). Got %zu , expected %lu", ComConf_ComSignal_isNrOfBltAppld_mrx, length, static_cast<unsigned long>(sizeof(NrOfBltAppld)));
        }
    }
    break;

    case ComConf_ComSignal_isNrOfKeyAvl_mrx:
    {
        if (sizeof(NrOfKeyAvl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received NrOfKeyAvl (%u)", ComConf_ComSignal_isNrOfKeyAvl_mrx);
            const NrOfKeyAvl& rteValue = *static_cast<const NrOfKeyAvl*>(buffer);
            autosar::NrOfKeyAvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            NrOfKeyAvl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for NrOfKeyAvl (%u). Got %zu , expected %lu", ComConf_ComSignal_isNrOfKeyAvl_mrx, length, static_cast<unsigned long>(sizeof(NrOfKeyAvl)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igNrSerlDMSM_mrx:
    {
        if (sizeof(NrSerlDMSM) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received NrSerlDMSM (%u)", ComConf_ComSignalGroup_igNrSerlDMSM_mrx);
            const NrSerlDMSM& rteValue = *static_cast<const NrSerlDMSM*>(buffer);
            autosar::NrSerlDMSM_info::data_elem_type deValue;
            deValue.Nr1 = static_cast<decltype(deValue.Nr1)>(rteValue.NrSerlDMSMNr1);
            deValue.Nr2 = static_cast<decltype(deValue.Nr2)>(rteValue.NrSerlDMSMNr2);
            deValue.Nr3 = static_cast<decltype(deValue.Nr3)>(rteValue.NrSerlDMSMNr3);
            deValue.Nr4 = static_cast<decltype(deValue.Nr4)>(rteValue.NrSerlDMSMNr4);

            NrSerlDMSM_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for NrSerlDMSM (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igNrSerlDMSM_mrx, length, static_cast<unsigned long>(sizeof(NrSerlDMSM)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPartNrDMSM_mrx:
    {
        if (sizeof(PartNrDMSM) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PartNrDMSM (%u)", ComConf_ComSignalGroup_igPartNrDMSM_mrx);
            const PartNrDMSM& rteValue = *static_cast<const PartNrDMSM*>(buffer);
            autosar::PartNrDMSM_info::data_elem_type deValue;
            deValue.EndSgn1 = static_cast<decltype(deValue.EndSgn1)>(rteValue.PartNrDMSMEndSgn1);
            deValue.EndSgn2 = static_cast<decltype(deValue.EndSgn2)>(rteValue.PartNrDMSMEndSgn2);
            deValue.EndSgn3 = static_cast<decltype(deValue.EndSgn3)>(rteValue.PartNrDMSMEndSgn3);
            deValue.Nr1 = static_cast<decltype(deValue.Nr1)>(rteValue.PartNrDMSMNr1);
            deValue.Nr2 = static_cast<decltype(deValue.Nr2)>(rteValue.PartNrDMSMNr2);
            deValue.Nr3 = static_cast<decltype(deValue.Nr3)>(rteValue.PartNrDMSMNr3);
            deValue.Nr4 = static_cast<decltype(deValue.Nr4)>(rteValue.PartNrDMSMNr4);

            PartNrDMSM_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PartNrDMSM (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igPartNrDMSM_mrx, length, static_cast<unsigned long>(sizeof(PartNrDMSM)));
        }
    }
    break;

    case ComConf_ComSignal_isPasAlrmSts_mrx:
    {
        if (sizeof(PasAlrmSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PasAlrmSts (%u)", ComConf_ComSignal_isPasAlrmSts_mrx);
            const PasAlrmSts& rteValue = *static_cast<const PasAlrmSts*>(buffer);
            autosar::PasAlrmSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PasAlrmSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PasAlrmSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isPasAlrmSts_mrx, length, static_cast<unsigned long>(sizeof(PasAlrmSts)));
        }
    }
    break;

    case ComConf_ComSignal_isPassMassgRunng_mrx:
    {
        if (sizeof(PassMassgRunng) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PassMassgRunng (%u)", ComConf_ComSignal_isPassMassgRunng_mrx);
            const PassMassgRunng& rteValue = *static_cast<const PassMassgRunng*>(buffer);
            autosar::PassMassgRunng_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PassMassgRunng_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PassMassgRunng (%u). Got %zu , expected %lu", ComConf_ComSignal_isPassMassgRunng_mrx, length, static_cast<unsigned long>(sizeof(PassMassgRunng)));
        }
    }
    break;

    case ComConf_ComSignal_isPassSeatActvSpplFct_mrx:
    {
        if (sizeof(PassSeatActvSpplFct) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PassSeatActvSpplFct (%u)", ComConf_ComSignal_isPassSeatActvSpplFct_mrx);
            const PassSeatActvSpplFct& rteValue = *static_cast<const PassSeatActvSpplFct*>(buffer);
            autosar::PassSeatActvSpplFct_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PassSeatActvSpplFct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PassSeatActvSpplFct (%u). Got %zu , expected %lu", ComConf_ComSignal_isPassSeatActvSpplFct_mrx, length, static_cast<unsigned long>(sizeof(PassSeatActvSpplFct)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPassSeatMassgFct_mrx:
    {
        if (sizeof(PassSeatMassgFct) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PassSeatMassgFct (%u)", ComConf_ComSignalGroup_igPassSeatMassgFct_mrx);
            const PassSeatMassgFct& rteValue = *static_cast<const PassSeatMassgFct*>(buffer);
            autosar::PassSeatMassgFct_info::data_elem_type deValue;
            deValue.MassgInten = static_cast<decltype(deValue.MassgInten)>(rteValue.PassSeatMassgFctMassgInten);
            deValue.MassgProg = static_cast<decltype(deValue.MassgProg)>(rteValue.PassSeatMassgFctMassgProg);
            deValue.MassgSpdLvl = static_cast<decltype(deValue.MassgSpdLvl)>(rteValue.PassSeatMassgFctMassgSpdLvl);
            deValue.OnOff = static_cast<decltype(deValue.OnOff)>(rteValue.PassSeatMassgFctOnOff);

            PassSeatMassgFct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PassSeatMassgFct (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igPassSeatMassgFct_mrx, length, static_cast<unsigned long>(sizeof(PassSeatMassgFct)));
        }
    }
    break;

    case ComConf_ComSignal_isPassSeatSts_mrx:
    {
        if (sizeof(PassSeatSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PassSeatSts (%u)", ComConf_ComSignal_isPassSeatSts_mrx);
            const PassSeatSts& rteValue = *static_cast<const PassSeatSts*>(buffer);
            autosar::PassSeatSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PassSeatSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PassSeatSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isPassSeatSts_mrx, length, static_cast<unsigned long>(sizeof(PassSeatSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPassSeatSwtSts2_mrx:
    {
        if (sizeof(PassSeatSwtSts2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PassSeatSwtSts2 (%u)", ComConf_ComSignalGroup_igPassSeatSwtSts2_mrx);
            const PassSeatSwtSts2& rteValue = *static_cast<const PassSeatSwtSts2*>(buffer);
            autosar::PassSeatSwtSts2_info::data_elem_type deValue;
            deValue.PassSeatSwtAdjmtOfSpplFctHozlSts = static_cast<decltype(deValue.PassSeatSwtAdjmtOfSpplFctHozlSts)>(rteValue.PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctHozlSts);
            deValue.PassSeatSwtAdjmtOfSpplFctVerSts = static_cast<decltype(deValue.PassSeatSwtAdjmtOfSpplFctVerSts)>(rteValue.PassSeatSwtSts2PassSeatSwtAdjmtOfSpplFctVerSts);
            deValue.PassSeatSwtHeiFrntSts = static_cast<decltype(deValue.PassSeatSwtHeiFrntSts)>(rteValue.PassSeatSwtSts2PassSeatSwtHeiFrntSts);
            deValue.PassSeatSwtHeiSts = static_cast<decltype(deValue.PassSeatSwtHeiSts)>(rteValue.PassSeatSwtSts2PassSeatSwtHeiSts);
            deValue.PassSeatSwtInclSts = static_cast<decltype(deValue.PassSeatSwtInclSts)>(rteValue.PassSeatSwtSts2PassSeatSwtInclSts);
            deValue.PassSeatSwtSelnOfSpplFctStsSts = static_cast<decltype(deValue.PassSeatSwtSelnOfSpplFctStsSts)>(rteValue.PassSeatSwtSts2PassSeatSwtSelnOfSpplFctStsSts);
            deValue.PassSeatSwtSldSts = static_cast<decltype(deValue.PassSeatSwtSldSts)>(rteValue.PassSeatSwtSts2PassSeatSwtSldSts);

            PassSeatSwtSts2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PassSeatSwtSts2 (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igPassSeatSwtSts2_mrx, length, static_cast<unsigned long>(sizeof(PassSeatSwtSts2)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPinionSteerAg1Group_mrx:
    {
        if (sizeof(PinionSteerAg1Group) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PinionSteerAg1Group (%u)", ComConf_ComSignalGroup_igPinionSteerAg1Group_mrx);
            const PinionSteerAg1Group& rteValue = *static_cast<const PinionSteerAg1Group*>(buffer);
            autosar::PinionSteerAg1_info::data_elem_type deValue;
            deValue.PinionSteerAg1 = static_cast<decltype(deValue.PinionSteerAg1)>( toSignedFromRaw<15>(rteValue.PinionSteerAg1) ) * 0.0009765625 + 0.0;
            deValue.PinionSteerAg1Qf = static_cast<decltype(deValue.PinionSteerAg1Qf)>(rteValue.PinionSteerAg1Qf);

            PinionSteerAg1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PinionSteerAg1Group (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igPinionSteerAg1Group_mrx, length, static_cast<unsigned long>(sizeof(PinionSteerAg1Group)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPosnFromSatlt_mrx:
    {
        if (sizeof(PosnFromSatlt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PosnFromSatlt (%u)", ComConf_ComSignalGroup_igPosnFromSatlt_mrx);
            const PosnFromSatlt& rteValue = *static_cast<const PosnFromSatlt*>(buffer);
            autosar::PosnFromSatlt_info::data_elem_type deValue;
            deValue.NoOfSatltForSysNo1 = static_cast<decltype(deValue.NoOfSatltForSysNo1)>(rteValue.NoOfSatltForSysNo1InPosnFromSatlt);
            deValue.NoOfSatltForSysNo2 = static_cast<decltype(deValue.NoOfSatltForSysNo2)>(rteValue.NoOfSatltForSysNo2InPosnFromSatlt);
            deValue.NoOfSatltForSysNo3 = static_cast<decltype(deValue.NoOfSatltForSysNo3)>(rteValue.NoOfSatltForSysNo3InPosnFromSatlt);
            deValue.NoOfSatltForSysNo4 = static_cast<decltype(deValue.NoOfSatltForSysNo4)>(rteValue.NoOfSatltForSysNo4InPosnFromSatlt);
            deValue.NoOfSatltForSysNo5 = static_cast<decltype(deValue.NoOfSatltForSysNo5)>(rteValue.NoOfSatltForSysNo5InPosnFromSatlt);
            deValue.NoOfSatltForSysNo6 = static_cast<decltype(deValue.NoOfSatltForSysNo6)>(rteValue.NoOfSatltForSysNo6InPosnFromSatlt);
            deValue.PosnAlti = static_cast<decltype(deValue.PosnAlti)>( toUnsignedFromRaw<16>(rteValue.PosnAltiInPosnFromSatlt) ) * 0.1 + -100.0;
            deValue.PosnDir = static_cast<decltype(deValue.PosnDir)>( toUnsignedFromRaw<16>(rteValue.PosnDirInPosnFromSatlt) ) * 0.01 + 0.0;
            deValue.PosnLat = static_cast<decltype(deValue.PosnLat)>( toSignedFromRaw<30>(rteValue.PosnLatInPosnFromSatlt) ) * 2.7777777777777776e-07 + 0.0;
            deValue.PosnLgt = static_cast<decltype(deValue.PosnLgt)>( toSignedFromRaw<31>(rteValue.PosnLgtInPosnFromSatlt) ) * 2.7777777777777776e-07 + 0.0;
            deValue.PosnSpd = static_cast<decltype(deValue.PosnSpd)>( toUnsignedFromRaw<17>(rteValue.PosnSpdInPosnFromSatlt) ) * 0.001 + 0.0;
            deValue.PosnVHozl = static_cast<decltype(deValue.PosnVHozl)>( toUnsignedFromRaw<17>(rteValue.PosnVHozlInPosnFromSatlt) ) * 0.001 + 0.0;
            deValue.PosnVVert = static_cast<decltype(deValue.PosnVVert)>( toSignedFromRaw<18>(rteValue.PosnVVertInPosnFromSatlt) ) * 0.001 + 0.0;
            deValue.PreHozlDil = static_cast<decltype(deValue.PreHozlDil)>( toUnsignedFromRaw<8>(rteValue.PreHozlDilInPosnFromSatlt) ) * 0.1 + 0.0;
            deValue.PrePosnDil = static_cast<decltype(deValue.PrePosnDil)>( toUnsignedFromRaw<8>(rteValue.PrePosnDilInPosnFromSatlt) ) * 0.1 + 0.0;
            deValue.PreTiDil = static_cast<decltype(deValue.PreTiDil)>( toUnsignedFromRaw<8>(rteValue.PreTiDilInPosnFromSatlt) ) * 0.1 + 0.0;
            deValue.PreVertDil = static_cast<decltype(deValue.PreVertDil)>( toUnsignedFromRaw<8>(rteValue.PreVertDilInPosnFromSatlt) ) * 0.1 + 0.0;
            deValue.SatltPosnStsPrm1 = static_cast<decltype(deValue.SatltPosnStsPrm1)>(rteValue.SatltPosnStsPrm1InPosnFromSatlt);
            deValue.SatltPosnStsPrm2 = static_cast<decltype(deValue.SatltPosnStsPrm2)>(rteValue.SatltPosnStsPrm2InPosnFromSatlt);
            deValue.SatltPosnStsPrm3 = static_cast<decltype(deValue.SatltPosnStsPrm3)>(rteValue.SatltPosnStsPrm3InPosnFromSatlt);
            deValue.SatltSysNo1InUse = static_cast<decltype(deValue.SatltSysNo1InUse)>(rteValue.SatltSysNo1InUseInPosnFromSatlt);
            deValue.SatltSysNo2InUse = static_cast<decltype(deValue.SatltSysNo2InUse)>(rteValue.SatltSysNo2InUseInPosnFromSatlt);
            deValue.SatltSysNo3InUse = static_cast<decltype(deValue.SatltSysNo3InUse)>(rteValue.SatltSysNo3InUseInPosnFromSatlt);
            deValue.SatltSysNo4InUse = static_cast<decltype(deValue.SatltSysNo4InUse)>(rteValue.SatltSysNo4InUseInPosnFromSatlt);
            deValue.SatltSysNo5InUse = static_cast<decltype(deValue.SatltSysNo5InUse)>(rteValue.SatltSysNo5InUseInPosnFromSatlt);
            deValue.SatltSysNo6InUse = static_cast<decltype(deValue.SatltSysNo6InUse)>(rteValue.SatltSysNo6InUseInPosnFromSatlt);
            deValue.TiForDay = static_cast<decltype(deValue.TiForDay)>(rteValue.TiForDayInPosnFromSatlt);
            deValue.TiForHr = static_cast<decltype(deValue.TiForHr)>(rteValue.TiForHrInPosnFromSatlt);
            deValue.TiForMins = static_cast<decltype(deValue.TiForMins)>(rteValue.TiForMinsInPosnFromSatlt);
            deValue.TiForMth = static_cast<decltype(deValue.TiForMth)>(rteValue.TiForMthInPosnFromSatlt);
            deValue.TiForSec = static_cast<decltype(deValue.TiForSec)>(rteValue.TiForSecInPosnFromSatlt);
            deValue.TiForYr = static_cast<decltype(deValue.TiForYr)>(rteValue.TiForYrInPosnFromSatlt);

            PosnFromSatlt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PosnFromSatlt (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igPosnFromSatlt_mrx, length, static_cast<unsigned long>(sizeof(PosnFromSatlt)));
        }
    }
    break;

    case ComConf_ComSignal_isPostDrvgClimaAvl_mrx:
    {
        if (sizeof(PostDrvgClimaAvl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PostDrvgClimaAvl (%u)", ComConf_ComSignal_isPostDrvgClimaAvl_mrx);
            const PostDrvgClimaAvl& rteValue = *static_cast<const PostDrvgClimaAvl*>(buffer);
            autosar::PostDrvgClimaAvl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PostDrvgClimaAvl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PostDrvgClimaAvl (%u). Got %zu , expected %lu", ComConf_ComSignal_isPostDrvgClimaAvl_mrx, length, static_cast<unsigned long>(sizeof(PostDrvgClimaAvl)));
        }
    }
    break;

    case ComConf_ComSignal_isPreClngNotif_mrx:
    {
        if (sizeof(PreClngNotif) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PreClngNotif (%u)", ComConf_ComSignal_isPreClngNotif_mrx);
            const PreClngNotif& rteValue = *static_cast<const PreClngNotif*>(buffer);
            autosar::PreClngNotif_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PreClngNotif_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PreClngNotif (%u). Got %zu , expected %lu", ComConf_ComSignal_isPreClngNotif_mrx, length, static_cast<unsigned long>(sizeof(PreClngNotif)));
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAssiFailr_mrx:
    {
        if (sizeof(PrkgAssiFailr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrkgAssiFailr (%u)", ComConf_ComSignal_isPrkgAssiFailr_mrx);
            const PrkgAssiFailr& rteValue = *static_cast<const PrkgAssiFailr*>(buffer);
            autosar::PrkgAssiFailr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAssiFailr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrkgAssiFailr (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrkgAssiFailr_mrx, length, static_cast<unsigned long>(sizeof(PrkgAssiFailr)));
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAssiManvActvSts_mrx:
    {
        if (sizeof(PrkgAssiManvActvSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrkgAssiManvActvSts (%u)", ComConf_ComSignal_isPrkgAssiManvActvSts_mrx);
            const PrkgAssiManvActvSts& rteValue = *static_cast<const PrkgAssiManvActvSts*>(buffer);
            autosar::PrkgAssiManvActvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAssiManvActvSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrkgAssiManvActvSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrkgAssiManvActvSts_mrx, length, static_cast<unsigned long>(sizeof(PrkgAssiManvActvSts)));
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAssiManvProgs_mrx:
    {
        if (sizeof(PrkgAssiManvProgs) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrkgAssiManvProgs (%u)", ComConf_ComSignal_isPrkgAssiManvProgs_mrx);
            const PrkgAssiManvProgs& rteValue = *static_cast<const PrkgAssiManvProgs*>(buffer);
            autosar::PrkgAssiManvProgs_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAssiManvProgs_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrkgAssiManvProgs (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrkgAssiManvProgs_mrx, length, static_cast<unsigned long>(sizeof(PrkgAssiManvProgs)));
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAssiSts_mrx:
    {
        if (sizeof(PrkgAssiSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrkgAssiSts (%u)", ComConf_ComSignal_isPrkgAssiSts_mrx);
            const PrkgAssiSts& rteValue = *static_cast<const PrkgAssiSts*>(buffer);
            autosar::PrkgAssiSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAssiSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrkgAssiSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrkgAssiSts_mrx, length, static_cast<unsigned long>(sizeof(PrkgAssiSts)));
        }
    }
    break;

    case ComConf_ComSignal_isPrkgAutSts_mrx:
    {
        if (sizeof(PrkgAutSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrkgAutSts (%u)", ComConf_ComSignal_isPrkgAutSts_mrx);
            const PrkgAutSts& rteValue = *static_cast<const PrkgAutSts*>(buffer);
            autosar::PrkgAutSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgAutSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrkgAutSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrkgAutSts_mrx, length, static_cast<unsigned long>(sizeof(PrkgAutSts)));
        }
    }
    break;

    case ComConf_ComSignal_isPrkgCamSysAvlSts_mrx:
    {
        if (sizeof(PrkgCamSysAvlSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrkgCamSysAvlSts (%u)", ComConf_ComSignal_isPrkgCamSysAvlSts_mrx);
            const PrkgCamSysAvlSts& rteValue = *static_cast<const PrkgCamSysAvlSts*>(buffer);
            autosar::PrkgCamSysAvlSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgCamSysAvlSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrkgCamSysAvlSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrkgCamSysAvlSts_mrx, length, static_cast<unsigned long>(sizeof(PrkgCamSysAvlSts)));
        }
    }
    break;

    case ComConf_ComSignal_isPrkgTypVld_mrx:
    {
        if (sizeof(PrkgTypVld) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrkgTypVld (%u)", ComConf_ComSignal_isPrkgTypVld_mrx);
            const PrkgTypVld& rteValue = *static_cast<const PrkgTypVld*>(buffer);
            autosar::PrkgTypVld_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrkgTypVld_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrkgTypVld (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrkgTypVld_mrx, length, static_cast<unsigned long>(sizeof(PrkgTypVld)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igProfAct_mrx:
    {
        if (sizeof(ProfAct) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ProfAct (%u)", ComConf_ComSignalGroup_igProfAct_mrx);
            const ProfAct& rteValue = *static_cast<const ProfAct*>(buffer);
            autosar::ProfAct_info::data_elem_type deValue;
            deValue.Prof1 = static_cast<decltype(deValue.Prof1)>(rteValue.ProfActProf1);
            deValue.Prof10 = static_cast<decltype(deValue.Prof10)>(rteValue.ProfActProf10);
            deValue.Prof11 = static_cast<decltype(deValue.Prof11)>(rteValue.ProfActProf11);
            deValue.Prof12 = static_cast<decltype(deValue.Prof12)>(rteValue.ProfActProf12);
            deValue.Prof2 = static_cast<decltype(deValue.Prof2)>(rteValue.ProfActProf2);
            deValue.Prof3 = static_cast<decltype(deValue.Prof3)>(rteValue.ProfActProf3);
            deValue.Prof4 = static_cast<decltype(deValue.Prof4)>(rteValue.ProfActProf4);
            deValue.Prof5 = static_cast<decltype(deValue.Prof5)>(rteValue.ProfActProf5);
            deValue.Prof6 = static_cast<decltype(deValue.Prof6)>(rteValue.ProfActProf6);
            deValue.Prof7 = static_cast<decltype(deValue.Prof7)>(rteValue.ProfActProf7);
            deValue.Prof8 = static_cast<decltype(deValue.Prof8)>(rteValue.ProfActProf8);
            deValue.Prof9 = static_cast<decltype(deValue.Prof9)>(rteValue.ProfActProf9);

            ProfAct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ProfAct (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igProfAct_mrx, length, static_cast<unsigned long>(sizeof(ProfAct)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igProfLimd_mrx:
    {
        if (sizeof(ProfLimd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ProfLimd (%u)", ComConf_ComSignalGroup_igProfLimd_mrx);
            const ProfLimd& rteValue = *static_cast<const ProfLimd*>(buffer);
            autosar::ProfLimd_info::data_elem_type deValue;
            deValue.Prof1 = static_cast<decltype(deValue.Prof1)>(rteValue.ProfLimdProf1);
            deValue.Prof10 = static_cast<decltype(deValue.Prof10)>(rteValue.ProfLimdProf10);
            deValue.Prof11 = static_cast<decltype(deValue.Prof11)>(rteValue.ProfLimdProf11);
            deValue.Prof12 = static_cast<decltype(deValue.Prof12)>(rteValue.ProfLimdProf12);
            deValue.Prof2 = static_cast<decltype(deValue.Prof2)>(rteValue.ProfLimdProf2);
            deValue.Prof3 = static_cast<decltype(deValue.Prof3)>(rteValue.ProfLimdProf3);
            deValue.Prof4 = static_cast<decltype(deValue.Prof4)>(rteValue.ProfLimdProf4);
            deValue.Prof5 = static_cast<decltype(deValue.Prof5)>(rteValue.ProfLimdProf5);
            deValue.Prof6 = static_cast<decltype(deValue.Prof6)>(rteValue.ProfLimdProf6);
            deValue.Prof7 = static_cast<decltype(deValue.Prof7)>(rteValue.ProfLimdProf7);
            deValue.Prof8 = static_cast<decltype(deValue.Prof8)>(rteValue.ProfLimdProf8);
            deValue.Prof9 = static_cast<decltype(deValue.Prof9)>(rteValue.ProfLimdProf9);

            ProfLimd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ProfLimd (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igProfLimd_mrx, length, static_cast<unsigned long>(sizeof(ProfLimd)));
        }
    }
    break;

    case ComConf_ComSignal_isProfPenSts1_mrx:
    {
        if (sizeof(ProfPenSts1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ProfPenSts1 (%u)", ComConf_ComSignal_isProfPenSts1_mrx);
            const ProfPenSts1& rteValue = *static_cast<const ProfPenSts1*>(buffer);
            autosar::ProfPenSts1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ProfPenSts1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ProfPenSts1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isProfPenSts1_mrx, length, static_cast<unsigned long>(sizeof(ProfPenSts1)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgModAct_mrx:
    {
        if (sizeof(PrpsnHvBattUsgModAct) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnHvBattUsgModAct (%u)", ComConf_ComSignal_isPrpsnHvBattUsgModAct_mrx);
            const PrpsnHvBattUsgModAct& rteValue = *static_cast<const PrpsnHvBattUsgModAct*>(buffer);
            autosar::PrpsnHvBattUsgModAct_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgModAct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnHvBattUsgModAct (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnHvBattUsgModAct_mrx, length, static_cast<unsigned long>(sizeof(PrpsnHvBattUsgModAct)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfChrgBlkd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnHvBattUsgOfChrgBlkd (%u)", ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd_mrx);
            const PrpsnHvBattUsgOfChrgBlkd& rteValue = *static_cast<const PrpsnHvBattUsgOfChrgBlkd*>(buffer);
            autosar::PrpsnHvBattUsgOfChrgBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfChrgBlkd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnHvBattUsgOfChrgBlkd (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd_mrx, length, static_cast<unsigned long>(sizeof(PrpsnHvBattUsgOfChrgBlkd)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd2_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfChrgBlkd2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnHvBattUsgOfChrgBlkd2 (%u)", ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd2_mrx);
            const PrpsnHvBattUsgOfChrgBlkd2& rteValue = *static_cast<const PrpsnHvBattUsgOfChrgBlkd2*>(buffer);
            autosar::PrpsnHvBattUsgOfChrgBlkd2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfChrgBlkd2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnHvBattUsgOfChrgBlkd2 (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnHvBattUsgOfChrgBlkd2_mrx, length, static_cast<unsigned long>(sizeof(PrpsnHvBattUsgOfChrgBlkd2)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfHldBlkd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnHvBattUsgOfHldBlkd (%u)", ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd_mrx);
            const PrpsnHvBattUsgOfHldBlkd& rteValue = *static_cast<const PrpsnHvBattUsgOfHldBlkd*>(buffer);
            autosar::PrpsnHvBattUsgOfHldBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfHldBlkd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnHvBattUsgOfHldBlkd (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd_mrx, length, static_cast<unsigned long>(sizeof(PrpsnHvBattUsgOfHldBlkd)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd2_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfHldBlkd2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnHvBattUsgOfHldBlkd2 (%u)", ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd2_mrx);
            const PrpsnHvBattUsgOfHldBlkd2& rteValue = *static_cast<const PrpsnHvBattUsgOfHldBlkd2*>(buffer);
            autosar::PrpsnHvBattUsgOfHldBlkd2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfHldBlkd2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnHvBattUsgOfHldBlkd2 (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnHvBattUsgOfHldBlkd2_mrx, length, static_cast<unsigned long>(sizeof(PrpsnHvBattUsgOfHldBlkd2)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfHldSmtBlkd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnHvBattUsgOfHldSmtBlkd (%u)", ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd_mrx);
            const PrpsnHvBattUsgOfHldSmtBlkd& rteValue = *static_cast<const PrpsnHvBattUsgOfHldSmtBlkd*>(buffer);
            autosar::PrpsnHvBattUsgOfHldSmtBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfHldSmtBlkd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnHvBattUsgOfHldSmtBlkd (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd_mrx, length, static_cast<unsigned long>(sizeof(PrpsnHvBattUsgOfHldSmtBlkd)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd2_mrx:
    {
        if (sizeof(PrpsnHvBattUsgOfHldSmtBlkd2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnHvBattUsgOfHldSmtBlkd2 (%u)", ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd2_mrx);
            const PrpsnHvBattUsgOfHldSmtBlkd2& rteValue = *static_cast<const PrpsnHvBattUsgOfHldSmtBlkd2*>(buffer);
            autosar::PrpsnHvBattUsgOfHldSmtBlkd2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnHvBattUsgOfHldSmtBlkd2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnHvBattUsgOfHldSmtBlkd2 (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnHvBattUsgOfHldSmtBlkd2_mrx, length, static_cast<unsigned long>(sizeof(PrpsnHvBattUsgOfHldSmtBlkd2)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModElecDrvBlkd_mrx:
    {
        if (sizeof(PrpsnModElecDrvBlkd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnModElecDrvBlkd (%u)", ComConf_ComSignal_isPrpsnModElecDrvBlkd_mrx);
            const PrpsnModElecDrvBlkd& rteValue = *static_cast<const PrpsnModElecDrvBlkd*>(buffer);
            autosar::PrpsnModElecDrvBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModElecDrvBlkd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnModElecDrvBlkd (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnModElecDrvBlkd_mrx, length, static_cast<unsigned long>(sizeof(PrpsnModElecDrvBlkd)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModOfSaveBlkd_mrx:
    {
        if (sizeof(PrpsnModOfSaveBlkd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnModOfSaveBlkd (%u)", ComConf_ComSignal_isPrpsnModOfSaveBlkd_mrx);
            const PrpsnModOfSaveBlkd& rteValue = *static_cast<const PrpsnModOfSaveBlkd*>(buffer);
            autosar::PrpsnModOfSaveBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModOfSaveBlkd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnModOfSaveBlkd (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnModOfSaveBlkd_mrx, length, static_cast<unsigned long>(sizeof(PrpsnModOfSaveBlkd)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModOfTracBlkd_mrx:
    {
        if (sizeof(PrpsnModOfTracBlkd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnModOfTracBlkd (%u)", ComConf_ComSignal_isPrpsnModOfTracBlkd_mrx);
            const PrpsnModOfTracBlkd& rteValue = *static_cast<const PrpsnModOfTracBlkd*>(buffer);
            autosar::PrpsnModOfTracBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModOfTracBlkd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnModOfTracBlkd (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnModOfTracBlkd_mrx, length, static_cast<unsigned long>(sizeof(PrpsnModOfTracBlkd)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModOffroadBlkd_mrx:
    {
        if (sizeof(PrpsnModOffroadBlkd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnModOffroadBlkd (%u)", ComConf_ComSignal_isPrpsnModOffroadBlkd_mrx);
            const PrpsnModOffroadBlkd& rteValue = *static_cast<const PrpsnModOffroadBlkd*>(buffer);
            autosar::PrpsnModOffroadBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModOffroadBlkd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnModOffroadBlkd (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnModOffroadBlkd_mrx, length, static_cast<unsigned long>(sizeof(PrpsnModOffroadBlkd)));
        }
    }
    break;

    case ComConf_ComSignal_isPrpsnModSptBlkd_mrx:
    {
        if (sizeof(PrpsnModSptBlkd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PrpsnModSptBlkd (%u)", ComConf_ComSignal_isPrpsnModSptBlkd_mrx);
            const PrpsnModSptBlkd& rteValue = *static_cast<const PrpsnModSptBlkd*>(buffer);
            autosar::PrpsnModSptBlkd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PrpsnModSptBlkd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PrpsnModSptBlkd (%u). Got %zu , expected %lu", ComConf_ComSignal_isPrpsnModSptBlkd_mrx, length, static_cast<unsigned long>(sizeof(PrpsnModSptBlkd)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPtCluTq_mrx:
    {
        if (sizeof(PtCluTq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PtCluTq (%u)", ComConf_ComSignalGroup_igPtCluTq_mrx);
            const PtCluTq& rteValue = *static_cast<const PtCluTq*>(buffer);
            autosar::PtCluTq_info::data_elem_type deValue;
            deValue.PtCluTq = static_cast<decltype(deValue.PtCluTq)>( toUnsignedFromRaw<11>(rteValue.PtCluTqPtCluTq) ) * 1.0 + 0.0;
            deValue.PtCluTqDyn = static_cast<decltype(deValue.PtCluTqDyn)>( toUnsignedFromRaw<11>(rteValue.PtCluTqPtCluTqDyn) ) * 1.0 + 0.0;
            deValue.PtCluTqQly = static_cast<decltype(deValue.PtCluTqQly)>(rteValue.PtCluTqPtCluTqQly);

            PtCluTq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PtCluTq (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igPtCluTq_mrx, length, static_cast<unsigned long>(sizeof(PtCluTq)));
        }
    }
    break;

    case ComConf_ComSignal_isPtGearTar_mrx:
    {
        if (sizeof(PtGearTar) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PtGearTar (%u)", ComConf_ComSignal_isPtGearTar_mrx);
            const PtGearTar& rteValue = *static_cast<const PtGearTar*>(buffer);
            autosar::PtGearTar_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PtGearTar_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PtGearTar (%u). Got %zu , expected %lu", ComConf_ComSignal_isPtGearTar_mrx, length, static_cast<unsigned long>(sizeof(PtGearTar)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igPtTqAtWhlFrntActGroup_mrx:
    {
        if (sizeof(PtTqAtWhlFrntActGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PtTqAtWhlFrntActGroup (%u)", ComConf_ComSignalGroup_igPtTqAtWhlFrntActGroup_mrx);
            const PtTqAtWhlFrntActGroup& rteValue = *static_cast<const PtTqAtWhlFrntActGroup*>(buffer);
            autosar::PtTqAtWhlFrntAct_info::data_elem_type deValue;
            deValue.PtTqAtAxleFrntAct = static_cast<decltype(deValue.PtTqAtAxleFrntAct)>( toSignedFromRaw<16>(rteValue.PtTqAtAxleFrntAct) ) * 1.0 + 0.0;
            deValue.PtTqAtWhlFrntLeAct = static_cast<decltype(deValue.PtTqAtWhlFrntLeAct)>( toSignedFromRaw<16>(rteValue.PtTqAtWhlFrntLeAct) ) * 1.0 + 0.0;
            deValue.PtTqAtWhlFrntRiAct = static_cast<decltype(deValue.PtTqAtWhlFrntRiAct)>( toSignedFromRaw<16>(rteValue.PtTqAtWhlFrntRiAct) ) * 1.0 + 0.0;
            deValue.PtTqAtWhlsFrntQly = static_cast<decltype(deValue.PtTqAtWhlsFrntQly)>(rteValue.PtTqAtWhlsFrntQly);

            PtTqAtWhlFrntAct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PtTqAtWhlFrntActGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igPtTqAtWhlFrntActGroup_mrx, length, static_cast<unsigned long>(sizeof(PtTqAtWhlFrntActGroup)));
        }
    }
    break;

    case ComConf_ComSignal_isPwrChrgDetdForPrkgHeatrElec_mrx:
    {
        if (sizeof(PwrChrgDetdForPrkgHeatrElec) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PwrChrgDetdForPrkgHeatrElec (%u)", ComConf_ComSignal_isPwrChrgDetdForPrkgHeatrElec_mrx);
            const PwrChrgDetdForPrkgHeatrElec& rteValue = *static_cast<const PwrChrgDetdForPrkgHeatrElec*>(buffer);
            autosar::PwrChrgDetdForPrkgHeatrElec_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PwrChrgDetdForPrkgHeatrElec_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PwrChrgDetdForPrkgHeatrElec (%u). Got %zu , expected %lu", ComConf_ComSignal_isPwrChrgDetdForPrkgHeatrElec_mrx, length, static_cast<unsigned long>(sizeof(PwrChrgDetdForPrkgHeatrElec)));
        }
    }
    break;

    case ComConf_ComSignal_isPwrSplyErrSts_mrx:
    {
        if (sizeof(PwrSplyErrSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received PwrSplyErrSts (%u)", ComConf_ComSignal_isPwrSplyErrSts_mrx);
            const PwrSplyErrSts& rteValue = *static_cast<const PwrSplyErrSts*>(buffer);
            autosar::PwrSplyErrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            PwrSplyErrSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for PwrSplyErrSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isPwrSplyErrSts_mrx, length, static_cast<unsigned long>(sizeof(PwrSplyErrSts)));
        }
    }
    break;

    case ComConf_ComSignal_isReAxleWarn_mrx:
    {
        if (sizeof(ReAxleWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ReAxleWarn (%u)", ComConf_ComSignal_isReAxleWarn_mrx);
            const ReAxleWarn& rteValue = *static_cast<const ReAxleWarn*>(buffer);
            autosar::ReAxleWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ReAxleWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ReAxleWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isReAxleWarn_mrx, length, static_cast<unsigned long>(sizeof(ReAxleWarn)));
        }
    }
    break;

    case ComConf_ComSignal_isRmnLockgPrsnlReq_mrx:
    {
        if (sizeof(RmnLockgPrsnlReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RmnLockgPrsnlReq (%u)", ComConf_ComSignal_isRmnLockgPrsnlReq_mrx);
            const RmnLockgPrsnlReq& rteValue = *static_cast<const RmnLockgPrsnlReq*>(buffer);
            autosar::RmnLockgPrsnlReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RmnLockgPrsnlReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RmnLockgPrsnlReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isRmnLockgPrsnlReq_mrx, length, static_cast<unsigned long>(sizeof(RmnLockgPrsnlReq)));
        }
    }
    break;

    case ComConf_ComSignal_isRoadFricIndcnSts_mrx:
    {
        if (sizeof(RoadFricIndcnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RoadFricIndcnSts (%u)", ComConf_ComSignal_isRoadFricIndcnSts_mrx);
            const RoadFricIndcnSts& rteValue = *static_cast<const RoadFricIndcnSts*>(buffer);
            autosar::RoadFricIndcnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RoadFricIndcnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RoadFricIndcnSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isRoadFricIndcnSts_mrx, length, static_cast<unsigned long>(sizeof(RoadFricIndcnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isRoadFricWarnReq_mrx:
    {
        if (sizeof(RoadFricWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RoadFricWarnReq (%u)", ComConf_ComSignal_isRoadFricWarnReq_mrx);
            const RoadFricWarnReq& rteValue = *static_cast<const RoadFricWarnReq*>(buffer);
            autosar::RoadFricWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RoadFricWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RoadFricWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isRoadFricWarnReq_mrx, length, static_cast<unsigned long>(sizeof(RoadFricWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isRoadSgnInfoSts_mrx:
    {
        if (sizeof(RoadSgnInfoSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RoadSgnInfoSts (%u)", ComConf_ComSignal_isRoadSgnInfoSts_mrx);
            const RoadSgnInfoSts& rteValue = *static_cast<const RoadSgnInfoSts*>(buffer);
            autosar::RoadSgnInfoSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RoadSgnInfoSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RoadSgnInfoSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isRoadSgnInfoSts_mrx, length, static_cast<unsigned long>(sizeof(RoadSgnInfoSts)));
        }
    }
    break;

    case ComConf_ComSignal_isRoadUsrProtnSts_mrx:
    {
        if (sizeof(RoadUsrProtnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RoadUsrProtnSts (%u)", ComConf_ComSignal_isRoadUsrProtnSts_mrx);
            const RoadUsrProtnSts& rteValue = *static_cast<const RoadUsrProtnSts*>(buffer);
            autosar::RoadUsrProtnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RoadUsrProtnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RoadUsrProtnSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isRoadUsrProtnSts_mrx, length, static_cast<unsigned long>(sizeof(RoadUsrProtnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isRotyDirReq2_mrx:
    {
        if (sizeof(RotyDirReq2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RotyDirReq2 (%u)", ComConf_ComSignal_isRotyDirReq2_mrx);
            const RotyDirReq2& rteValue = *static_cast<const RotyDirReq2*>(buffer);
            autosar::RotyDirReq2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RotyDirReq2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RotyDirReq2 (%u). Got %zu , expected %lu", ComConf_ComSignal_isRotyDirReq2_mrx, length, static_cast<unsigned long>(sizeof(RotyDirReq2)));
        }
    }
    break;

    case ComConf_ComSignal_isRotyDirReq1_mrx:
    {
        if (sizeof(RotyDirReq1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RotyDirReq1 (%u)", ComConf_ComSignal_isRotyDirReq1_mrx);
            const RotyDirReq1& rteValue = *static_cast<const RotyDirReq1*>(buffer);
            autosar::RotyDirReq1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RotyDirReq1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RotyDirReq1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isRotyDirReq1_mrx, length, static_cast<unsigned long>(sizeof(RotyDirReq1)));
        }
    }
    break;

    case ComConf_ComSignal_isRotyPosReq2_mrx:
    {
        if (sizeof(RotyPosReq2) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RotyPosReq2 (%u)", ComConf_ComSignal_isRotyPosReq2_mrx);
            const RotyPosReq2& rteValue = *static_cast<const RotyPosReq2*>(buffer);
            autosar::RotyPosReq2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RotyPosReq2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RotyPosReq2 (%u). Got %zu , expected %lu", ComConf_ComSignal_isRotyPosReq2_mrx, length, static_cast<unsigned long>(sizeof(RotyPosReq2)));
        }
    }
    break;

    case ComConf_ComSignal_isRotyPosReq1_mrx:
    {
        if (sizeof(RotyPosReq1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RotyPosReq1 (%u)", ComConf_ComSignal_isRotyPosReq1_mrx);
            const RotyPosReq1& rteValue = *static_cast<const RotyPosReq1*>(buffer);
            autosar::RotyPosReq1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            RotyPosReq1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RotyPosReq1 (%u). Got %zu , expected %lu", ComConf_ComSignal_isRotyPosReq1_mrx, length, static_cast<unsigned long>(sizeof(RotyPosReq1)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igRsdsSysStsLe_mrx:
    {
        if (sizeof(RsdsSysStsLe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RsdsSysStsLe (%u)", ComConf_ComSignalGroup_igRsdsSysStsLe_mrx);
            const RsdsSysStsLe& rteValue = *static_cast<const RsdsSysStsLe*>(buffer);
            autosar::RsdsSysStsLe_info::data_elem_type deValue;
            deValue.CtraSts = static_cast<decltype(deValue.CtraSts)>(rteValue.RsdsSysStsLeCtraSts);
            deValue.CtraSts1 = static_cast<decltype(deValue.CtraSts1)>(rteValue.RsdsSysStsLeCtraSts1);
            deValue.LcmaSts = static_cast<decltype(deValue.LcmaSts)>(rteValue.RsdsSysStsLeLcmaSts);
            deValue.LcmaSts1 = static_cast<decltype(deValue.LcmaSts1)>(rteValue.RsdsSysStsLeLcmaSts1);
            deValue.RcmSts = static_cast<decltype(deValue.RcmSts)>(rteValue.RsdsSysStsLeRcmSts);
            deValue.RcwSts = static_cast<decltype(deValue.RcwSts)>(rteValue.RsdsSysStsLeRcwSts);
            deValue.RsdsSts = static_cast<decltype(deValue.RsdsSts)>(rteValue.RsdsSysStsLeRsdsSts);

            RsdsSysStsLe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RsdsSysStsLe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igRsdsSysStsLe_mrx, length, static_cast<unsigned long>(sizeof(RsdsSysStsLe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igRsdsSysStsRi_mrx:
    {
        if (sizeof(RsdsSysStsRi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received RsdsSysStsRi (%u)", ComConf_ComSignalGroup_igRsdsSysStsRi_mrx);
            const RsdsSysStsRi& rteValue = *static_cast<const RsdsSysStsRi*>(buffer);
            autosar::RsdsSysStsRi_info::data_elem_type deValue;
            deValue.CtraSts = static_cast<decltype(deValue.CtraSts)>(rteValue.RsdsSysStsRiCtraSts);
            deValue.CtraSts1 = static_cast<decltype(deValue.CtraSts1)>(rteValue.RsdsSysStsRiCtraSts1);
            deValue.LcmaSts = static_cast<decltype(deValue.LcmaSts)>(rteValue.RsdsSysStsRiLcmaSts);
            deValue.LcmaSts1 = static_cast<decltype(deValue.LcmaSts1)>(rteValue.RsdsSysStsRiLcmaSts1);
            deValue.RcmSts = static_cast<decltype(deValue.RcmSts)>(rteValue.RsdsSysStsRiRcmSts);
            deValue.RcwSts = static_cast<decltype(deValue.RcwSts)>(rteValue.RsdsSysStsRiRcwSts);
            deValue.RsdsSts = static_cast<decltype(deValue.RsdsSts)>(rteValue.RsdsSysStsRiRsdsSts);

            RsdsSysStsRi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for RsdsSysStsRi (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igRsdsSysStsRi_mrx, length, static_cast<unsigned long>(sizeof(RsdsSysStsRi)));
        }
    }
    break;

    case ComConf_ComSignal_isScrBarVolIndcn_mrx:
    {
        if (sizeof(ScrBarVolIndcn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ScrBarVolIndcn (%u)", ComConf_ComSignal_isScrBarVolIndcn_mrx);
            const ScrBarVolIndcn& rteValue = *static_cast<const ScrBarVolIndcn*>(buffer);
            autosar::ScrBarVolIndcn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 5.0 + 0.0;
            ScrBarVolIndcn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ScrBarVolIndcn (%u). Got %zu , expected %lu", ComConf_ComSignal_isScrBarVolIndcn_mrx, length, static_cast<unsigned long>(sizeof(ScrBarVolIndcn)));
        }
    }
    break;

    case ComConf_ComSignal_isScrMaxFillgVol_mrx:
    {
        if (sizeof(ScrMaxFillgVol) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ScrMaxFillgVol (%u)", ComConf_ComSignal_isScrMaxFillgVol_mrx);
            const ScrMaxFillgVol& rteValue = *static_cast<const ScrMaxFillgVol*>(buffer);
            autosar::ScrMaxFillgVol_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ScrMaxFillgVol_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ScrMaxFillgVol (%u). Got %zu , expected %lu", ComConf_ComSignal_isScrMaxFillgVol_mrx, length, static_cast<unsigned long>(sizeof(ScrMaxFillgVol)));
        }
    }
    break;

    case ComConf_ComSignal_isScrReagentTankVol_mrx:
    {
        if (sizeof(ScrReagentTankVol) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received ScrReagentTankVol (%u)", ComConf_ComSignal_isScrReagentTankVol_mrx);
            const ScrReagentTankVol& rteValue = *static_cast<const ScrReagentTankVol*>(buffer);
            autosar::ScrReagentTankVol_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            ScrReagentTankVol_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for ScrReagentTankVol (%u). Got %zu , expected %lu", ComConf_ComSignal_isScrReagentTankVol_mrx, length, static_cast<unsigned long>(sizeof(ScrReagentTankVol)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSeatBackUnlckdThrd_mrx:
    {
        if (sizeof(SeatBackUnlckdThrd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SeatBackUnlckdThrd (%u)", ComConf_ComSignalGroup_igSeatBackUnlckdThrd_mrx);
            const SeatBackUnlckdThrd& rteValue = *static_cast<const SeatBackUnlckdThrd*>(buffer);
            autosar::SeatBackUnlckdThrd_info::data_elem_type deValue;
            deValue.SeatBackUnlckdLe = static_cast<decltype(deValue.SeatBackUnlckdLe)>(rteValue.SeatBackUnlckdThrdLe);
            deValue.SeatBackUnlckdRi = static_cast<decltype(deValue.SeatBackUnlckdRi)>(rteValue.SeatBackUnlckdThrdRi);

            SeatBackUnlckdThrd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SeatBackUnlckdThrd (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSeatBackUnlckdThrd_mrx, length, static_cast<unsigned long>(sizeof(SeatBackUnlckdThrd)));
        }
    }
    break;

    case ComConf_ComSignal_isSeatHeatgAutCdn_mrx:
    {
        if (sizeof(SeatHeatgAutCdn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SeatHeatgAutCdn (%u)", ComConf_ComSignal_isSeatHeatgAutCdn_mrx);
            const SeatHeatgAutCdn& rteValue = *static_cast<const SeatHeatgAutCdn*>(buffer);
            autosar::SeatHeatgAutCdn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SeatHeatgAutCdn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SeatHeatgAutCdn (%u). Got %zu , expected %lu", ComConf_ComSignal_isSeatHeatgAutCdn_mrx, length, static_cast<unsigned long>(sizeof(SeatHeatgAutCdn)));
        }
    }
    break;

    case ComConf_ComSignal_isSeatVentnAutCdn_mrx:
    {
        if (sizeof(SeatVentnAutCdn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SeatVentnAutCdn (%u)", ComConf_ComSignal_isSeatVentnAutCdn_mrx);
            const SeatVentnAutCdn& rteValue = *static_cast<const SeatVentnAutCdn*>(buffer);
            autosar::SeatVentnAutCdn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SeatVentnAutCdn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SeatVentnAutCdn (%u). Got %zu , expected %lu", ComConf_ComSignal_isSeatVentnAutCdn_mrx, length, static_cast<unsigned long>(sizeof(SeatVentnAutCdn)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchAccActvnSts_mrx:
    {
        if (sizeof(SftyCchAccActvnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchAccActvnSts (%u)", ComConf_ComSignalGroup_igSftyCchAccActvnSts_mrx);
            const SftyCchAccActvnSts& rteValue = *static_cast<const SftyCchAccActvnSts*>(buffer);
            autosar::SftyCchAccActvnSts_info::data_elem_type deValue;
            deValue.Actv = static_cast<decltype(deValue.Actv)>(rteValue.SftyCchAccActvnStsActv);
            deValue.Day = static_cast<decltype(deValue.Day)>(rteValue.SftyCchAccActvnStsDay);
            deValue.Mth = static_cast<decltype(deValue.Mth)>(rteValue.SftyCchAccActvnStsMth);
            deValue.Yr = static_cast<decltype(deValue.Yr)>(rteValue.SftyCchAccActvnStsYr);

            SftyCchAccActvnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchAccActvnSts (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSftyCchAccActvnSts_mrx, length, static_cast<unsigned long>(sizeof(SftyCchAccActvnSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchDrvgCycIdx_mrx:
    {
        if (sizeof(SftyCchDrvgCycIdx) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchDrvgCycIdx (%u)", ComConf_ComSignalGroup_igSftyCchDrvgCycIdx_mrx);
            const SftyCchDrvgCycIdx& rteValue = *static_cast<const SftyCchDrvgCycIdx*>(buffer);
            autosar::SftyCchDrvgCycIdx_info::data_elem_type deValue;
            deValue.Attention = static_cast<decltype(deValue.Attention)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxAttention) ) * 0.1 + 0.0;
            deValue.AttentionAvl = static_cast<decltype(deValue.AttentionAvl)>(rteValue.SftyCchDrvgCycIdxAttentionAvl);
            deValue.AttentionCntxt = static_cast<decltype(deValue.AttentionCntxt)>(rteValue.SftyCchDrvgCycIdxAttentionCntxt);
            deValue.DstToVeh = static_cast<decltype(deValue.DstToVeh)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxDstToVeh) ) * 0.1 + 0.0;
            deValue.DstToVehAvl = static_cast<decltype(deValue.DstToVehAvl)>(rteValue.SftyCchDrvgCycIdxDstToVehAvl);
            deValue.DstToVehCntxt = static_cast<decltype(deValue.DstToVehCntxt)>(rteValue.SftyCchDrvgCycIdxDstToVehCntxt);
            deValue.FldTot = static_cast<decltype(deValue.FldTot)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxFldTot) ) * 0.1 + 0.0;
            deValue.FldTotAvl = static_cast<decltype(deValue.FldTotAvl)>(rteValue.SftyCchDrvgCycIdxFldTotAvl);
            deValue.KeepSpdLim = static_cast<decltype(deValue.KeepSpdLim)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxKeepSpdLim) ) * 0.1 + 0.0;
            deValue.KeepSpdLimAvl = static_cast<decltype(deValue.KeepSpdLimAvl)>(rteValue.SftyCchDrvgCycIdxKeepSpdLimAvl);
            deValue.KeepSpdLimCntxt = static_cast<decltype(deValue.KeepSpdLimCntxt)>(rteValue.SftyCchDrvgCycIdxKeepSpdLimCntxt);
            deValue.LaneKeep = static_cast<decltype(deValue.LaneKeep)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxLaneKeep) ) * 0.1 + 0.0;
            deValue.LaneKeepAvl = static_cast<decltype(deValue.LaneKeepAvl)>(rteValue.SftyCchDrvgCycIdxLaneKeepAvl);
            deValue.LaneKeepCntxt = static_cast<decltype(deValue.LaneKeepCntxt)>(rteValue.SftyCchDrvgCycIdxLaneKeepCntxt);
            deValue.Tot = static_cast<decltype(deValue.Tot)>( toUnsignedFromRaw<6>(rteValue.SftyCchDrvgCycIdxTot) ) * 0.1 + 0.0;
            deValue.TotAvl = static_cast<decltype(deValue.TotAvl)>(rteValue.SftyCchDrvgCycIdxTotAvl);
            deValue.TotCntxt = static_cast<decltype(deValue.TotCntxt)>(rteValue.SftyCchDrvgCycIdxTotCntxt);

            SftyCchDrvgCycIdx_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchDrvgCycIdx (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSftyCchDrvgCycIdx_mrx, length, static_cast<unsigned long>(sizeof(SftyCchDrvgCycIdx)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchLaneKeepAidActvnSts_mrx:
    {
        if (sizeof(SftyCchLaneKeepAidActvnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchLaneKeepAidActvnSts (%u)", ComConf_ComSignalGroup_igSftyCchLaneKeepAidActvnSts_mrx);
            const SftyCchLaneKeepAidActvnSts& rteValue = *static_cast<const SftyCchLaneKeepAidActvnSts*>(buffer);
            autosar::SftyCchLaneKeepAidActvnSts_info::data_elem_type deValue;
            deValue.Actv = static_cast<decltype(deValue.Actv)>(rteValue.SftyCchLaneKeepAidActvnStsActv);
            deValue.Day = static_cast<decltype(deValue.Day)>(rteValue.SftyCchLaneKeepAidActvnStsDay);
            deValue.Mth = static_cast<decltype(deValue.Mth)>(rteValue.SftyCchLaneKeepAidActvnStsMth);
            deValue.Yr = static_cast<decltype(deValue.Yr)>(rteValue.SftyCchLaneKeepAidActvnStsYr);

            SftyCchLaneKeepAidActvnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchLaneKeepAidActvnSts (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSftyCchLaneKeepAidActvnSts_mrx, length, static_cast<unsigned long>(sizeof(SftyCchLaneKeepAidActvnSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchLongTermIdx_mrx:
    {
        if (sizeof(SftyCchLongTermIdx) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchLongTermIdx (%u)", ComConf_ComSignalGroup_igSftyCchLongTermIdx_mrx);
            const SftyCchLongTermIdx& rteValue = *static_cast<const SftyCchLongTermIdx*>(buffer);
            autosar::SftyCchLongTermIdx_info::data_elem_type deValue;
            deValue.Attention = static_cast<decltype(deValue.Attention)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxAttention) ) * 0.1 + 0.0;
            deValue.AttentionAvl = static_cast<decltype(deValue.AttentionAvl)>(rteValue.SftyCchLongTermIdxAttentionAvl);
            deValue.DstToVeh = static_cast<decltype(deValue.DstToVeh)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxDstToVeh) ) * 0.1 + 0.0;
            deValue.DstToVehAvl = static_cast<decltype(deValue.DstToVehAvl)>(rteValue.SftyCchLongTermIdxDstToVehAvl);
            deValue.KeepSpdLim = static_cast<decltype(deValue.KeepSpdLim)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxKeepSpdLim) ) * 0.1 + 0.0;
            deValue.KeepSpdLimAvl = static_cast<decltype(deValue.KeepSpdLimAvl)>(rteValue.SftyCchLongTermIdxKeepSpdLimAvl);
            deValue.LaneKeep = static_cast<decltype(deValue.LaneKeep)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxLaneKeep) ) * 0.1 + 0.0;
            deValue.LaneKeepAvl = static_cast<decltype(deValue.LaneKeepAvl)>(rteValue.SftyCchLongTermIdxLaneKeepAvl);
            deValue.Tot = static_cast<decltype(deValue.Tot)>( toUnsignedFromRaw<6>(rteValue.SftyCchLongTermIdxTot) ) * 0.1 + 0.0;
            deValue.TotAvl = static_cast<decltype(deValue.TotAvl)>(rteValue.SftyCchLongTermIdxTotAvl);

            SftyCchLongTermIdx_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchLongTermIdx (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSftyCchLongTermIdx_mrx, length, static_cast<unsigned long>(sizeof(SftyCchLongTermIdx)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchMidTermIdx_mrx:
    {
        if (sizeof(SftyCchMidTermIdx) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchMidTermIdx (%u)", ComConf_ComSignalGroup_igSftyCchMidTermIdx_mrx);
            const SftyCchMidTermIdx& rteValue = *static_cast<const SftyCchMidTermIdx*>(buffer);
            autosar::SftyCchMidTermIdx_info::data_elem_type deValue;
            deValue.Attention = static_cast<decltype(deValue.Attention)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxAttention) ) * 0.1 + 0.0;
            deValue.AttentionAvl = static_cast<decltype(deValue.AttentionAvl)>(rteValue.SftyCchMidTermIdxAttentionAvl);
            deValue.DstToVeh = static_cast<decltype(deValue.DstToVeh)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxDstToVeh) ) * 0.1 + 0.0;
            deValue.DstToVehAvl = static_cast<decltype(deValue.DstToVehAvl)>(rteValue.SftyCchMidTermIdxDstToVehAvl);
            deValue.KeepSpdLim = static_cast<decltype(deValue.KeepSpdLim)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxKeepSpdLim) ) * 0.1 + 0.0;
            deValue.KeepSpdLimAvl = static_cast<decltype(deValue.KeepSpdLimAvl)>(rteValue.SftyCchMidTermIdxKeepSpdLimAvl);
            deValue.LaneKeep = static_cast<decltype(deValue.LaneKeep)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxLaneKeep) ) * 0.1 + 0.0;
            deValue.LaneKeepAvl = static_cast<decltype(deValue.LaneKeepAvl)>(rteValue.SftyCchMidTermIdxLaneKeepAvl);
            deValue.Tot = static_cast<decltype(deValue.Tot)>( toUnsignedFromRaw<6>(rteValue.SftyCchMidTermIdxTot) ) * 0.1 + 0.0;
            deValue.TotAvl = static_cast<decltype(deValue.TotAvl)>(rteValue.SftyCchMidTermIdxTotAvl);

            SftyCchMidTermIdx_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchMidTermIdx (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSftyCchMidTermIdx_mrx, length, static_cast<unsigned long>(sizeof(SftyCchMidTermIdx)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchPilotAssiActvnSts_mrx:
    {
        if (sizeof(SftyCchPilotAssiActvnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchPilotAssiActvnSts (%u)", ComConf_ComSignalGroup_igSftyCchPilotAssiActvnSts_mrx);
            const SftyCchPilotAssiActvnSts& rteValue = *static_cast<const SftyCchPilotAssiActvnSts*>(buffer);
            autosar::SftyCchPilotAssiActvnSts_info::data_elem_type deValue;
            deValue.Actv = static_cast<decltype(deValue.Actv)>(rteValue.SftyCchPilotAssiActvnStsActv);
            deValue.Day = static_cast<decltype(deValue.Day)>(rteValue.SftyCchPilotAssiActvnStsDay);
            deValue.Mth = static_cast<decltype(deValue.Mth)>(rteValue.SftyCchPilotAssiActvnStsMth);
            deValue.Yr = static_cast<decltype(deValue.Yr)>(rteValue.SftyCchPilotAssiActvnStsYr);

            SftyCchPilotAssiActvnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchPilotAssiActvnSts (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSftyCchPilotAssiActvnSts_mrx, length, static_cast<unsigned long>(sizeof(SftyCchPilotAssiActvnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchPrimFbAftDrvg_mrx:
    {
        if (sizeof(SftyCchPrimFbAftDrvg) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchPrimFbAftDrvg (%u)", ComConf_ComSignal_isSftyCchPrimFbAftDrvg_mrx);
            const SftyCchPrimFbAftDrvg& rteValue = *static_cast<const SftyCchPrimFbAftDrvg*>(buffer);
            autosar::SftyCchPrimFbAftDrvg_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchPrimFbAftDrvg_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchPrimFbAftDrvg (%u). Got %zu , expected %lu", ComConf_ComSignal_isSftyCchPrimFbAftDrvg_mrx, length, static_cast<unsigned long>(sizeof(SftyCchPrimFbAftDrvg)));
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchPrimFbDurgDrvg_mrx:
    {
        if (sizeof(SftyCchPrimFbDurgDrvg) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchPrimFbDurgDrvg (%u)", ComConf_ComSignal_isSftyCchPrimFbDurgDrvg_mrx);
            const SftyCchPrimFbDurgDrvg& rteValue = *static_cast<const SftyCchPrimFbDurgDrvg*>(buffer);
            autosar::SftyCchPrimFbDurgDrvg_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchPrimFbDurgDrvg_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchPrimFbDurgDrvg (%u). Got %zu , expected %lu", ComConf_ComSignal_isSftyCchPrimFbDurgDrvg_mrx, length, static_cast<unsigned long>(sizeof(SftyCchPrimFbDurgDrvg)));
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchSecFbAftDrvg_mrx:
    {
        if (sizeof(SftyCchSecFbAftDrvg) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchSecFbAftDrvg (%u)", ComConf_ComSignal_isSftyCchSecFbAftDrvg_mrx);
            const SftyCchSecFbAftDrvg& rteValue = *static_cast<const SftyCchSecFbAftDrvg*>(buffer);
            autosar::SftyCchSecFbAftDrvg_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchSecFbAftDrvg_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchSecFbAftDrvg (%u). Got %zu , expected %lu", ComConf_ComSignal_isSftyCchSecFbAftDrvg_mrx, length, static_cast<unsigned long>(sizeof(SftyCchSecFbAftDrvg)));
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchSecFbDurgDrvg_mrx:
    {
        if (sizeof(SftyCchSecFbDurgDrvg) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchSecFbDurgDrvg (%u)", ComConf_ComSignal_isSftyCchSecFbDurgDrvg_mrx);
            const SftyCchSecFbDurgDrvg& rteValue = *static_cast<const SftyCchSecFbDurgDrvg*>(buffer);
            autosar::SftyCchSecFbDurgDrvg_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchSecFbDurgDrvg_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchSecFbDurgDrvg (%u). Got %zu , expected %lu", ComConf_ComSignal_isSftyCchSecFbDurgDrvg_mrx, length, static_cast<unsigned long>(sizeof(SftyCchSecFbDurgDrvg)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyCchShortTermIdx_mrx:
    {
        if (sizeof(SftyCchShortTermIdx) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchShortTermIdx (%u)", ComConf_ComSignalGroup_igSftyCchShortTermIdx_mrx);
            const SftyCchShortTermIdx& rteValue = *static_cast<const SftyCchShortTermIdx*>(buffer);
            autosar::SftyCchShortTermIdx_info::data_elem_type deValue;
            deValue.Attention = static_cast<decltype(deValue.Attention)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxAttention) ) * 0.1 + 0.0;
            deValue.AttentionAvl = static_cast<decltype(deValue.AttentionAvl)>(rteValue.SftyCchShortTermIdxAttentionAvl);
            deValue.DstToVeh = static_cast<decltype(deValue.DstToVeh)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxDstToVeh) ) * 0.1 + 0.0;
            deValue.DstToVehAvl = static_cast<decltype(deValue.DstToVehAvl)>(rteValue.SftyCchShortTermIdxDstToVehAvl);
            deValue.KeepSpdLim = static_cast<decltype(deValue.KeepSpdLim)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxKeepSpdLim) ) * 0.1 + 0.0;
            deValue.KeepSpdLimAvl = static_cast<decltype(deValue.KeepSpdLimAvl)>(rteValue.SftyCchShortTermIdxKeepSpdLimAvl);
            deValue.LaneKeep = static_cast<decltype(deValue.LaneKeep)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxLaneKeep) ) * 0.1 + 0.0;
            deValue.LaneKeepAvl = static_cast<decltype(deValue.LaneKeepAvl)>(rteValue.SftyCchShortTermIdxLaneKeepAvl);
            deValue.Tot = static_cast<decltype(deValue.Tot)>( toUnsignedFromRaw<6>(rteValue.SftyCchShortTermIdxTot) ) * 0.1 + 0.0;
            deValue.TotAvl = static_cast<decltype(deValue.TotAvl)>(rteValue.SftyCchShortTermIdxTotAvl);

            SftyCchShortTermIdx_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchShortTermIdx (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSftyCchShortTermIdx_mrx, length, static_cast<unsigned long>(sizeof(SftyCchShortTermIdx)));
        }
    }
    break;

    case ComConf_ComSignal_isSftyCchSts_mrx:
    {
        if (sizeof(SftyCchSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyCchSts (%u)", ComConf_ComSignal_isSftyCchSts_mrx);
            const SftyCchSts& rteValue = *static_cast<const SftyCchSts*>(buffer);
            autosar::SftyCchSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SftyCchSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyCchSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isSftyCchSts_mrx, length, static_cast<unsigned long>(sizeof(SftyCchSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSftyWarnGroupFromAsySafe_mrx:
    {
        if (sizeof(SftyWarnGroupFromAsySafe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SftyWarnGroupFromAsySafe (%u)", ComConf_ComSignalGroup_igSftyWarnGroupFromAsySafe_mrx);
            const SftyWarnGroupFromAsySafe& rteValue = *static_cast<const SftyWarnGroupFromAsySafe*>(buffer);
            autosar::SftyWarnGroupFromAsySafe_info::data_elem_type deValue;
            deValue.CnclWarnLatForAutDrv = static_cast<decltype(deValue.CnclWarnLatForAutDrv)>(rteValue.CnclWarnLatForAutDrv);
            deValue.SftyWarnGroupFromAsySafeChks = static_cast<decltype(deValue.SftyWarnGroupFromAsySafeChks)>(rteValue.SftyWarnGroupFromAsySafeChks);
            deValue.SftyWarnGroupFromAsySafeCntr = static_cast<decltype(deValue.SftyWarnGroupFromAsySafeCntr)>(rteValue.SftyWarnGroupFromAsySafeCntr);
            deValue.SteerOvrdWarnReqForAutDrv = static_cast<decltype(deValue.SteerOvrdWarnReqForAutDrv)>(rteValue.SteerOvrdWarnReqForAutDrv);
            deValue.SteerStsForAutDrv = static_cast<decltype(deValue.SteerStsForAutDrv)>(rteValue.SteerStsForAutDrv);

            SftyWarnGroupFromAsySafe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SftyWarnGroupFromAsySafe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSftyWarnGroupFromAsySafe_mrx, length, static_cast<unsigned long>(sizeof(SftyWarnGroupFromAsySafe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSnsrParkAssiFrnt_mrx:
    {
        if (sizeof(SnsrParkAssiFrnt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SnsrParkAssiFrnt (%u)", ComConf_ComSignalGroup_igSnsrParkAssiFrnt_mrx);
            const SnsrParkAssiFrnt& rteValue = *static_cast<const SnsrParkAssiFrnt*>(buffer);
            autosar::SnsrParkAssiFrnt_info::data_elem_type deValue;
            deValue.AudSideWarn = static_cast<decltype(deValue.AudSideWarn)>(rteValue.ParkAssiDstFrntOfAudSideWarn);
            deValue.AudWarnDir = static_cast<decltype(deValue.AudWarnDir)>(rteValue.ParkAssiDstFrntOfAudWarnDir);
            deValue.DstOfSnsrInsdLe = static_cast<decltype(deValue.DstOfSnsrInsdLe)>(rteValue.ParkAssiDstFrntOfSnsrInsdLe);
            deValue.DstOfSnsrInsdRi = static_cast<decltype(deValue.DstOfSnsrInsdRi)>(rteValue.ParkAssiDstFrntOfDstOfSnsrInsdRi);
            deValue.DstOfSnsrLeSide = static_cast<decltype(deValue.DstOfSnsrLeSide)>(rteValue.ParkAssiDstFrntOfDstOfSnsrLeSide);
            deValue.DstOfSnsrOutdLe = static_cast<decltype(deValue.DstOfSnsrOutdLe)>(rteValue.ParkAssiDstFrntOfSnsrOutdLe);
            deValue.DstOfSnsrOutdRi = static_cast<decltype(deValue.DstOfSnsrOutdRi)>(rteValue.ParkAssiDstFrntOfDstOfSnsrOutdRi);
            deValue.DstOfSnsrRiSide = static_cast<decltype(deValue.DstOfSnsrRiSide)>(rteValue.ParkAssiDstFrntOfDstOfSnsrRiSide);

            SnsrParkAssiFrnt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SnsrParkAssiFrnt (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSnsrParkAssiFrnt_mrx, length, static_cast<unsigned long>(sizeof(SnsrParkAssiFrnt)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSnsrParkAssiRe_mrx:
    {
        if (sizeof(SnsrParkAssiRe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SnsrParkAssiRe (%u)", ComConf_ComSignalGroup_igSnsrParkAssiRe_mrx);
            const SnsrParkAssiRe& rteValue = *static_cast<const SnsrParkAssiRe*>(buffer);
            autosar::SnsrParkAssiRe_info::data_elem_type deValue;
            deValue.AudSideWarn = static_cast<decltype(deValue.AudSideWarn)>(rteValue.ParkAssiDstReOfAudSideWarn);
            deValue.AudWarnDir = static_cast<decltype(deValue.AudWarnDir)>(rteValue.ParkAssiDstReOfAudWarnDir);
            deValue.DstOfSnsrInsdLe = static_cast<decltype(deValue.DstOfSnsrInsdLe)>(rteValue.ParkAssiDstReOfDstOfSnsrInsdLe);
            deValue.DstOfSnsrInsdRi = static_cast<decltype(deValue.DstOfSnsrInsdRi)>(rteValue.ParkAssiDstReOfDstOfSnsrInsdRi);
            deValue.DstOfSnsrLeSide = static_cast<decltype(deValue.DstOfSnsrLeSide)>(rteValue.ParkAssiDstReOfDstOfSnsrLeSide);
            deValue.DstOfSnsrOutdLe = static_cast<decltype(deValue.DstOfSnsrOutdLe)>(rteValue.ParkAssiDstReOfDstOfSnsrOutdLe);
            deValue.DstOfSnsrOutdRi = static_cast<decltype(deValue.DstOfSnsrOutdRi)>(rteValue.ParkAssiDstReOfDstOfSnsrOutdRi);
            deValue.DstOfSnsrRiSide = static_cast<decltype(deValue.DstOfSnsrRiSide)>(rteValue.ParkAssiDstReOfDstOfSnsrRiSide);

            SnsrParkAssiRe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SnsrParkAssiRe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSnsrParkAssiRe_mrx, length, static_cast<unsigned long>(sizeof(SnsrParkAssiRe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSnsrPrkgAssiFrnt_mrx:
    {
        if (sizeof(SnsrPrkgAssiFrnt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SnsrPrkgAssiFrnt (%u)", ComConf_ComSignalGroup_igSnsrPrkgAssiFrnt_mrx);
            const SnsrPrkgAssiFrnt& rteValue = *static_cast<const SnsrPrkgAssiFrnt*>(buffer);
            autosar::SnsrPrkgAssiFrnt_info::data_elem_type deValue;
            deValue.AudSideWarn = static_cast<decltype(deValue.AudSideWarn)>(rteValue.SnsrPrkgAssiFrntAudSideWarn);
            deValue.AudWarnDir = static_cast<decltype(deValue.AudWarnDir)>(rteValue.SnsrPrkgAssiFrntAudWarnDir);
            deValue.SnsrDstInsdLe = static_cast<decltype(deValue.SnsrDstInsdLe)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiFrntSnsrDstInsdLe) ) * -1.0 + 1023.0;
            deValue.SnsrDstInsdRi = static_cast<decltype(deValue.SnsrDstInsdRi)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiFrntSnsrDstInsdRi) ) * -1.0 + 1023.0;
            deValue.SnsrDstOutdLe = static_cast<decltype(deValue.SnsrDstOutdLe)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiFrntSnsrDstOutdLe) ) * -1.0 + 1023.0;
            deValue.SnsrDstOutdRi = static_cast<decltype(deValue.SnsrDstOutdRi)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiFrntSnsrDstOutdRi) ) * -1.0 + 1023.0;
            deValue.SnsrDstSideLe = static_cast<decltype(deValue.SnsrDstSideLe)>( toUnsignedFromRaw<9>(rteValue.SnsrPrkgAssiFrntSnsrDstSideLe) ) * -1.0 + 511.0;
            deValue.SnsrDstSideRi = static_cast<decltype(deValue.SnsrDstSideRi)>( toUnsignedFromRaw<9>(rteValue.SnsrPrkgAssiFrntSnsrDstSideRi) ) * -1.0 + 511.0;

            SnsrPrkgAssiFrnt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SnsrPrkgAssiFrnt (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSnsrPrkgAssiFrnt_mrx, length, static_cast<unsigned long>(sizeof(SnsrPrkgAssiFrnt)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSnsrPrkgAssiRe_mrx:
    {
        if (sizeof(SnsrPrkgAssiRe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SnsrPrkgAssiRe (%u)", ComConf_ComSignalGroup_igSnsrPrkgAssiRe_mrx);
            const SnsrPrkgAssiRe& rteValue = *static_cast<const SnsrPrkgAssiRe*>(buffer);
            autosar::SnsrPrkgAssiRe_info::data_elem_type deValue;
            deValue.AudSideWarn = static_cast<decltype(deValue.AudSideWarn)>(rteValue.SnsrPrkgAssiReAudSideWarn);
            deValue.AudWarnDir = static_cast<decltype(deValue.AudWarnDir)>(rteValue.SnsrPrkgAssiReAudWarnDir);
            deValue.SnsrDstInsdLe = static_cast<decltype(deValue.SnsrDstInsdLe)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiReSnsrDstInsdLe) ) * -1.0 + 1023.0;
            deValue.SnsrDstInsdRi = static_cast<decltype(deValue.SnsrDstInsdRi)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiReSnsrDstInsdRi) ) * -1.0 + 1023.0;
            deValue.SnsrDstOutdLe = static_cast<decltype(deValue.SnsrDstOutdLe)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiReSnsrDstOutdLe) ) * -1.0 + 1023.0;
            deValue.SnsrDstOutdRi = static_cast<decltype(deValue.SnsrDstOutdRi)>( toUnsignedFromRaw<10>(rteValue.SnsrPrkgAssiReSnsrDstOutdRi) ) * -1.0 + 1023.0;
            deValue.SnsrDstSideLe = static_cast<decltype(deValue.SnsrDstSideLe)>( toUnsignedFromRaw<9>(rteValue.SnsrPrkgAssiReSnsrDstSideLe) ) * -1.0 + 511.0;
            deValue.SnsrDstSideRi = static_cast<decltype(deValue.SnsrDstSideRi)>( toUnsignedFromRaw<9>(rteValue.SnsrPrkgAssiReSnsrDstSideRi) ) * -1.0 + 511.0;

            SnsrPrkgAssiRe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SnsrPrkgAssiRe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSnsrPrkgAssiRe_mrx, length, static_cast<unsigned long>(sizeof(SnsrPrkgAssiRe)));
        }
    }
    break;

    case ComConf_ComSignal_isWhlMotSysSpdAct_mrx:
    {
        if (sizeof(WhlMotSysSpdAct) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WhlMotSysSpdAct (%u)", ComConf_ComSignal_isWhlMotSysSpdAct_mrx);
            const WhlMotSysSpdAct& rteValue = *static_cast<const WhlMotSysSpdAct*>(buffer);
            autosar::WhlMotSysSpdAct_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 0.1 + -1638.4;
            WhlMotSysSpdAct_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WhlMotSysSpdAct (%u). Got %zu , expected %lu", ComConf_ComSignal_isWhlMotSysSpdAct_mrx, length, static_cast<unsigned long>(sizeof(WhlMotSysSpdAct)));
        }
    }
    break;

    case ComConf_ComSignal_isSpdLimAdpvSts_mrx:
    {
        if (sizeof(SpdLimAdpvSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SpdLimAdpvSts (%u)", ComConf_ComSignal_isSpdLimAdpvSts_mrx);
            const SpdLimAdpvSts& rteValue = *static_cast<const SpdLimAdpvSts*>(buffer);
            autosar::SpdLimAdpvSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SpdLimAdpvSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SpdLimAdpvSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isSpdLimAdpvSts_mrx, length, static_cast<unsigned long>(sizeof(SpdLimAdpvSts)));
        }
    }
    break;

    case ComConf_ComSignal_isSpdLimWarnReq_mrx:
    {
        if (sizeof(SpdLimWarnReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SpdLimWarnReq (%u)", ComConf_ComSignal_isSpdLimWarnReq_mrx);
            const SpdLimWarnReq& rteValue = *static_cast<const SpdLimWarnReq*>(buffer);
            autosar::SpdLimWarnReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SpdLimWarnReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SpdLimWarnReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isSpdLimWarnReq_mrx, length, static_cast<unsigned long>(sizeof(SpdLimWarnReq)));
        }
    }
    break;

    case ComConf_ComSignal_isSpdWarn_mrx:
    {
        if (sizeof(SpdWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SpdWarn (%u)", ComConf_ComSignal_isSpdWarn_mrx);
            const SpdWarn& rteValue = *static_cast<const SpdWarn*>(buffer);
            autosar::SpdWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SpdWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SpdWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isSpdWarn_mrx, length, static_cast<unsigned long>(sizeof(SpdWarn)));
        }
    }
    break;

    case ComConf_ComSignal_isSrvRst_mrx:
    {
        if (sizeof(SrvRst) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SrvRst (%u)", ComConf_ComSignal_isSrvRst_mrx);
            const SrvRst& rteValue = *static_cast<const SrvRst*>(buffer);
            autosar::SrvRst_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SrvRst_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SrvRst (%u). Got %zu , expected %lu", ComConf_ComSignal_isSrvRst_mrx, length, static_cast<unsigned long>(sizeof(SrvRst)));
        }
    }
    break;

    case ComConf_ComSignal_isSrvTrig_mrx:
    {
        if (sizeof(SrvTrig) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SrvTrig (%u)", ComConf_ComSignal_isSrvTrig_mrx);
            const SrvTrig& rteValue = *static_cast<const SrvTrig*>(buffer);
            autosar::SrvTrig_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>( toUnsignedFromRaw<0>(rteValue) ) * 1.0 + 0.0;
            SrvTrig_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SrvTrig (%u). Got %zu , expected %lu", ComConf_ComSignal_isSrvTrig_mrx, length, static_cast<unsigned long>(sizeof(SrvTrig)));
        }
    }
    break;

    case ComConf_ComSignal_isSteerWarnReqForAutDrv_mrx:
    {
        if (sizeof(SteerWarnReqForAutDrv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SteerWarnReqForAutDrv (%u)", ComConf_ComSignal_isSteerWarnReqForAutDrv_mrx);
            const SteerWarnReqForAutDrv& rteValue = *static_cast<const SteerWarnReqForAutDrv*>(buffer);
            autosar::SteerWarnReqForAutDrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SteerWarnReqForAutDrv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SteerWarnReqForAutDrv (%u). Got %zu , expected %lu", ComConf_ComSignal_isSteerWarnReqForAutDrv_mrx, length, static_cast<unsigned long>(sizeof(SteerWarnReqForAutDrv)));
        }
    }
    break;

    case ComConf_ComSignal_isSteerWarnReqForLaneKeepAid_mrx:
    {
        if (sizeof(SteerWarnReqForLaneKeepAid) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SteerWarnReqForLaneKeepAid (%u)", ComConf_ComSignal_isSteerWarnReqForLaneKeepAid_mrx);
            const SteerWarnReqForLaneKeepAid& rteValue = *static_cast<const SteerWarnReqForLaneKeepAid*>(buffer);
            autosar::SteerWarnReqForLaneKeepAid_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SteerWarnReqForLaneKeepAid_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SteerWarnReqForLaneKeepAid (%u). Got %zu , expected %lu", ComConf_ComSignal_isSteerWarnReqForLaneKeepAid_mrx, length, static_cast<unsigned long>(sizeof(SteerWarnReqForLaneKeepAid)));
        }
    }
    break;

    case ComConf_ComSignal_isSteerWhlHeatgStrtAutCdnOk_mrx:
    {
        if (sizeof(SteerWhlHeatgStrtAutCdnOk) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SteerWhlHeatgStrtAutCdnOk (%u)", ComConf_ComSignal_isSteerWhlHeatgStrtAutCdnOk_mrx);
            const SteerWhlHeatgStrtAutCdnOk& rteValue = *static_cast<const SteerWhlHeatgStrtAutCdnOk*>(buffer);
            autosar::SteerWhlHeatgStrtAutCdnOk_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SteerWhlHeatgStrtAutCdnOk_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SteerWhlHeatgStrtAutCdnOk (%u). Got %zu , expected %lu", ComConf_ComSignal_isSteerWhlHeatgStrtAutCdnOk_mrx, length, static_cast<unsigned long>(sizeof(SteerWhlHeatgStrtAutCdnOk)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igSteerWhlSnsr_mrx:
    {
        if (sizeof(SteerWhlSnsr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SteerWhlSnsr (%u)", ComConf_ComSignalGroup_igSteerWhlSnsr_mrx);
            const SteerWhlSnsr& rteValue = *static_cast<const SteerWhlSnsr*>(buffer);
            autosar::SteerWhlSnsrSafe_info::data_elem_type deValue;
            deValue.SteerWhlAg = static_cast<decltype(deValue.SteerWhlAg)>( toSignedFromRaw<15>(rteValue.SteerWhlAgSafe) ) * 0.0009765625 + 0.0;
            deValue.SteerWhlAgSpd = static_cast<decltype(deValue.SteerWhlAgSpd)>( toSignedFromRaw<14>(rteValue.SteerWhlAgSpdSafe) ) * 0.0078125 + 0.0;
            deValue.SteerWhlSnsrChks = static_cast<decltype(deValue.SteerWhlSnsrChks)>(rteValue.SteerWhlSnsrSafeChks);
            deValue.SteerWhlSnsrCntr = static_cast<decltype(deValue.SteerWhlSnsrCntr)>(rteValue.SteerWhlSnsrSafeCntr);
            deValue.SteerWhlSnsrQf = static_cast<decltype(deValue.SteerWhlSnsrQf)>(rteValue.SteerWhlSnsrQf);

            SteerWhlSnsrSafe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SteerWhlSnsr (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igSteerWhlSnsr_mrx, length, static_cast<unsigned long>(sizeof(SteerWhlSnsr)));
        }
    }
    break;

    case ComConf_ComSignal_isSteerWhlSwtPwr_mrx:
    {
        if (sizeof(SteerWhlSwtPwr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SteerWhlSwtPwr (%u)", ComConf_ComSignal_isSteerWhlSwtPwr_mrx);
            const SteerWhlSwtPwr& rteValue = *static_cast<const SteerWhlSwtPwr*>(buffer);
            autosar::SteerWhlSwtPwr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SteerWhlSwtPwr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SteerWhlSwtPwr (%u). Got %zu , expected %lu", ComConf_ComSignal_isSteerWhlSwtPwr_mrx, length, static_cast<unsigned long>(sizeof(SteerWhlSwtPwr)));
        }
    }
    break;

    case ComConf_ComSignal_isStopStrtInhb_mrx:
    {
        if (sizeof(StopStrtInhb) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received StopStrtInhb (%u)", ComConf_ComSignal_isStopStrtInhb_mrx);
            const StopStrtInhb& rteValue = *static_cast<const StopStrtInhb*>(buffer);
            autosar::StopStrtInhb_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            StopStrtInhb_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for StopStrtInhb (%u). Got %zu , expected %lu", ComConf_ComSignal_isStopStrtInhb_mrx, length, static_cast<unsigned long>(sizeof(StopStrtInhb)));
        }
    }
    break;

    case ComConf_ComSignal_isStrtInProgs_mrx:
    {
        if (sizeof(StrtInProgs) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received StrtInProgs (%u)", ComConf_ComSignal_isStrtInProgs_mrx);
            const StrtInProgs& rteValue = *static_cast<const StrtInProgs*>(buffer);
            autosar::StrtInProgs_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            StrtInProgs_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for StrtInProgs (%u). Got %zu , expected %lu", ComConf_ComSignal_isStrtInProgs_mrx, length, static_cast<unsigned long>(sizeof(StrtInProgs)));
        }
    }
    break;

    case ComConf_ComSignal_isSunRoofPosnSts_mrx:
    {
        if (sizeof(SunRoofPosnSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SunRoofPosnSts (%u)", ComConf_ComSignal_isSunRoofPosnSts_mrx);
            const SunRoofPosnSts& rteValue = *static_cast<const SunRoofPosnSts*>(buffer);
            autosar::SunRoofPosnSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SunRoofPosnSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SunRoofPosnSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isSunRoofPosnSts_mrx, length, static_cast<unsigned long>(sizeof(SunRoofPosnSts)));
        }
    }
    break;

    case ComConf_ComSignal_isSwtAcptReq_mrx:
    {
        if (sizeof(SwtAcptReq) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SwtAcptReq (%u)", ComConf_ComSignal_isSwtAcptReq_mrx);
            const SwtAcptReq& rteValue = *static_cast<const SwtAcptReq*>(buffer);
            autosar::SwtAcptReq_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SwtAcptReq_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SwtAcptReq (%u). Got %zu , expected %lu", ComConf_ComSignal_isSwtAcptReq_mrx, length, static_cast<unsigned long>(sizeof(SwtAcptReq)));
        }
    }
    break;

    case ComConf_ComSignal_isSwtForPassAirbCutOffSt_mrx:
    {
        if (sizeof(SwtForPassAirbCutOffSt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SwtForPassAirbCutOffSt (%u)", ComConf_ComSignal_isSwtForPassAirbCutOffSt_mrx);
            const SwtForPassAirbCutOffSt& rteValue = *static_cast<const SwtForPassAirbCutOffSt*>(buffer);
            autosar::SwtForPassAirbCutOffSt_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SwtForPassAirbCutOffSt_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SwtForPassAirbCutOffSt (%u). Got %zu , expected %lu", ComConf_ComSignal_isSwtForPassAirbCutOffSt_mrx, length, static_cast<unsigned long>(sizeof(SwtForPassAirbCutOffSt)));
        }
    }
    break;

    case ComConf_ComSignal_isTankFlapSts_mrx:
    {
        if (sizeof(TankFlapSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TankFlapSts (%u)", ComConf_ComSignal_isTankFlapSts_mrx);
            const TankFlapSts& rteValue = *static_cast<const TankFlapSts*>(buffer);
            autosar::TankFlapSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TankFlapSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TankFlapSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isTankFlapSts_mrx, length, static_cast<unsigned long>(sizeof(TankFlapSts)));
        }
    }
    break;

    case ComConf_ComSignal_isTelmSts_mrx:
    {
        if (sizeof(TelmSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TelmSts (%u)", ComConf_ComSignal_isTelmSts_mrx);
            const TelmSts& rteValue = *static_cast<const TelmSts*>(buffer);
            autosar::TelmSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TelmSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TelmSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isTelmSts_mrx, length, static_cast<unsigned long>(sizeof(TelmSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTiAndDateIndcn_mrx:
    {
        if (sizeof(TiAndDateIndcn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TiAndDateIndcn (%u)", ComConf_ComSignalGroup_igTiAndDateIndcn_mrx);
            const TiAndDateIndcn& rteValue = *static_cast<const TiAndDateIndcn*>(buffer);
            autosar::TiAndDateIndcn_info::data_elem_type deValue;
            deValue.DataValid = static_cast<decltype(deValue.DataValid)>(rteValue.TiAndDateVld);
            deValue.Day = static_cast<decltype(deValue.Day)>(rteValue.Day);
            deValue.Hr1 = static_cast<decltype(deValue.Hr1)>(rteValue.Hr);
            deValue.Mins1 = static_cast<decltype(deValue.Mins1)>(rteValue.Mins);
            deValue.Mth1 = static_cast<decltype(deValue.Mth1)>(rteValue.Mth);
            deValue.Sec1 = static_cast<decltype(deValue.Sec1)>(rteValue.Sec);
            deValue.Yr1 = static_cast<decltype(deValue.Yr1)>(rteValue.Yr);

            TiAndDateIndcn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TiAndDateIndcn (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igTiAndDateIndcn_mrx, length, static_cast<unsigned long>(sizeof(TiAndDateIndcn)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePAbsltValFrntRi_mrx:
    {
        if (sizeof(TirePAbsltValFrntRi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TirePAbsltValFrntRi (%u)", ComConf_ComSignalGroup_igTirePAbsltValFrntRi_mrx);
            const TirePAbsltValFrntRi& rteValue = *static_cast<const TirePAbsltValFrntRi*>(buffer);
            autosar::TirePAbsltValFrntRi_info::data_elem_type deValue;
            deValue.TirePAbsltVal1 = static_cast<decltype(deValue.TirePAbsltVal1)>( toUnsignedFromRaw<8>(rteValue.TirePAbsltValFrntRiTirePAbsltVal1) ) * 2.5 + 0.0;
            deValue.TirePAbsltValQF = static_cast<decltype(deValue.TirePAbsltValQF)>(rteValue.TirePAbsltValQFForFrnRi);
            deValue.TirePPosn_ = static_cast<decltype(deValue.TirePPosn_)>(rteValue.TirePAbsltValFrntRiTirePPosn);

            TirePAbsltValFrntRi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TirePAbsltValFrntRi (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igTirePAbsltValFrntRi_mrx, length, static_cast<unsigned long>(sizeof(TirePAbsltValFrntRi)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePAbsltValFrntLe_mrx:
    {
        if (sizeof(TirePAbsltValFrntLe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TirePAbsltValFrntLe (%u)", ComConf_ComSignalGroup_igTirePAbsltValFrntLe_mrx);
            const TirePAbsltValFrntLe& rteValue = *static_cast<const TirePAbsltValFrntLe*>(buffer);
            autosar::TirePAbsltValFrntLe_info::data_elem_type deValue;
            deValue.TirePPosn_ = static_cast<decltype(deValue.TirePPosn_)>(rteValue.TirePPosnForFrntLe);
            deValue.TirePabsltValQF = static_cast<decltype(deValue.TirePabsltValQF)>(rteValue.TirePAbsltVaQflForFrntLe);
            deValue.TirepabsltVal1 = static_cast<decltype(deValue.TirepabsltVal1)>( toUnsignedFromRaw<8>(rteValue.TirePAbsltValForFrntLe) ) * 2.5 + 0.0;

            TirePAbsltValFrntLe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TirePAbsltValFrntLe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igTirePAbsltValFrntLe_mrx, length, static_cast<unsigned long>(sizeof(TirePAbsltValFrntLe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePAbsltValReLe_mrx:
    {
        if (sizeof(TirePAbsltValReLe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TirePAbsltValReLe (%u)", ComConf_ComSignalGroup_igTirePAbsltValReLe_mrx);
            const TirePAbsltValReLe& rteValue = *static_cast<const TirePAbsltValReLe*>(buffer);
            autosar::TirePAbsltValReLe_info::data_elem_type deValue;
            deValue.TirePAbsltVal1 = static_cast<decltype(deValue.TirePAbsltVal1)>( toUnsignedFromRaw<8>(rteValue.TirePAbsltValForReLe) ) * 2.5 + 0.0;
            deValue.TirePAbsltValQF = static_cast<decltype(deValue.TirePAbsltValQF)>(rteValue.TirePAbsltValQfForReLe);
            deValue.TirePPosn_ = static_cast<decltype(deValue.TirePPosn_)>(rteValue.TirePPosnForReLe);

            TirePAbsltValReLe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TirePAbsltValReLe (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igTirePAbsltValReLe_mrx, length, static_cast<unsigned long>(sizeof(TirePAbsltValReLe)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePAbsltValReRi_mrx:
    {
        if (sizeof(TirePAbsltValReRi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TirePAbsltValReRi (%u)", ComConf_ComSignalGroup_igTirePAbsltValReRi_mrx);
            const TirePAbsltValReRi& rteValue = *static_cast<const TirePAbsltValReRi*>(buffer);
            autosar::TirePAbsltValReRi_info::data_elem_type deValue;
            deValue.TirePAbsltVal1 = static_cast<decltype(deValue.TirePAbsltVal1)>( toUnsignedFromRaw<8>(rteValue.TirePAbsltValForReRi) ) * 2.5 + 0.0;
            deValue.TirePAbsltValQF = static_cast<decltype(deValue.TirePAbsltValQF)>(rteValue.TirePAbsltValQfForReRi);
            deValue.TirePPosn_ = static_cast<decltype(deValue.TirePPosn_)>(rteValue.TirePPosnForReRi);

            TirePAbsltValReRi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TirePAbsltValReRi (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igTirePAbsltValReRi_mrx, length, static_cast<unsigned long>(sizeof(TirePAbsltValReRi)));
        }
    }
    break;

    case ComConf_ComSignal_isTirePCalSts_mrx:
    {
        if (sizeof(TirePCalSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TirePCalSts (%u)", ComConf_ComSignal_isTirePCalSts_mrx);
            const TirePCalSts& rteValue = *static_cast<const TirePCalSts*>(buffer);
            autosar::TirePCalSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TirePCalSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TirePCalSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isTirePCalSts_mrx, length, static_cast<unsigned long>(sizeof(TirePCalSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTirePMonData_mrx:
    {
        if (sizeof(TirePMonData) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TirePMonData (%u)", ComConf_ComSignalGroup_igTirePMonData_mrx);
            const TirePMonData& rteValue = *static_cast<const TirePMonData*>(buffer);
            autosar::TirePMonData_info::data_elem_type deValue;
            deValue.TirePFrntLe = static_cast<decltype(deValue.TirePFrntLe)>(rteValue.TirePFrntLe);
            deValue.TirePFrntRe = static_cast<decltype(deValue.TirePFrntRe)>(rteValue.TirePFrntRi);
            deValue.TirePMonSts1_ = static_cast<decltype(deValue.TirePMonSts1_)>(rteValue.TirePMonSts);
            deValue.TirePReLe = static_cast<decltype(deValue.TirePReLe)>(rteValue.TirePReLe);
            deValue.TirePReRi = static_cast<decltype(deValue.TirePReRi)>(rteValue.TirePReRi);

            TirePMonData_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TirePMonData (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igTirePMonData_mrx, length, static_cast<unsigned long>(sizeof(TirePMonData)));
        }
    }
    break;

    case ComConf_ComSignal_isTnlEntryOrEnd_mrx:
    {
        if (sizeof(TnlEntryOrEnd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TnlEntryOrEnd (%u)", ComConf_ComSignal_isTnlEntryOrEnd_mrx);
            const TnlEntryOrEnd& rteValue = *static_cast<const TnlEntryOrEnd*>(buffer);
            autosar::TnlEntryOrEnd_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TnlEntryOrEnd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TnlEntryOrEnd (%u). Got %zu , expected %lu", ComConf_ComSignal_isTnlEntryOrEnd_mrx, length, static_cast<unsigned long>(sizeof(TnlEntryOrEnd)));
        }
    }
    break;

    case ComConf_ComSignal_isTrSts_mrx:
    {
        if (sizeof(TrSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TrSts (%u)", ComConf_ComSignal_isTrSts_mrx);
            const TrSts& rteValue = *static_cast<const TrSts*>(buffer);
            autosar::TrSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TrSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isTrSts_mrx, length, static_cast<unsigned long>(sizeof(TrSts)));
        }
    }
    break;

    case ComConf_ComSignal_isTrfcLiSpprtSts_mrx:
    {
        if (sizeof(TrfcLiSpprtSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TrfcLiSpprtSts (%u)", ComConf_ComSignal_isTrfcLiSpprtSts_mrx);
            const TrfcLiSpprtSts& rteValue = *static_cast<const TrfcLiSpprtSts*>(buffer);
            autosar::TrfcLiSpprtSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrfcLiSpprtSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TrfcLiSpprtSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isTrfcLiSpprtSts_mrx, length, static_cast<unsigned long>(sizeof(TrfcLiSpprtSts)));
        }
    }
    break;

    case ComConf_ComSignal_isTrlrLampActvtChk_mrx:
    {
        if (sizeof(TrlrLampActvtChk) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TrlrLampActvtChk (%u)", ComConf_ComSignal_isTrlrLampActvtChk_mrx);
            const TrlrLampActvtChk& rteValue = *static_cast<const TrlrLampActvtChk*>(buffer);
            autosar::TrlrLampChkSts1WdSts1_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrlrLampChkSts1WdSts1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TrlrLampActvtChk (%u). Got %zu , expected %lu", ComConf_ComSignal_isTrlrLampActvtChk_mrx, length, static_cast<unsigned long>(sizeof(TrlrLampActvtChk)));
        }
    }
    break;

    case ComConf_ComSignal_isTrlrLampChkSts_mrx:
    {
        if (sizeof(TrlrLampChkSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TrlrLampChkSts (%u)", ComConf_ComSignal_isTrlrLampChkSts_mrx);
            const TrlrLampChkSts& rteValue = *static_cast<const TrlrLampChkSts*>(buffer);
            autosar::TrlrLampChkSts1WdSts2_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrlrLampChkSts1WdSts2_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TrlrLampChkSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isTrlrLampChkSts_mrx, length, static_cast<unsigned long>(sizeof(TrlrLampChkSts)));
        }
    }
    break;

    case ComConf_ComSignal_isTrlrPrsnt_mrx:
    {
        if (sizeof(TrlrPrsnt) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TrlrPrsnt (%u)", ComConf_ComSignal_isTrlrPrsnt_mrx);
            const TrlrPrsnt& rteValue = *static_cast<const TrlrPrsnt*>(buffer);
            autosar::TrlrPrsntSts1WdSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TrlrPrsntSts1WdSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TrlrPrsnt (%u). Got %zu , expected %lu", ComConf_ComSignal_isTrlrPrsnt_mrx, length, static_cast<unsigned long>(sizeof(TrlrPrsnt)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igTwliBriRawGroup_mrx:
    {
        if (sizeof(TwliBriRawGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TwliBriRawGroup (%u)", ComConf_ComSignalGroup_igTwliBriRawGroup_mrx);
            const TwliBriRawGroup& rteValue = *static_cast<const TwliBriRawGroup*>(buffer);
            autosar::TwliBriRaw_info::data_elem_type deValue;
            deValue.TwliBriRaw1 = static_cast<decltype(deValue.TwliBriRaw1)>(rteValue.TwliBriRaw);
            deValue.TwliBriRawQf = static_cast<decltype(deValue.TwliBriRawQf)>(rteValue.TwliBriRawQf);

            TwliBriRaw_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TwliBriRawGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igTwliBriRawGroup_mrx, length, static_cast<unsigned long>(sizeof(TwliBriRawGroup)));
        }
    }
    break;

    case ComConf_ComSignal_isTwliBriSts_mrx:
    {
        if (sizeof(TwliBriSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received TwliBriSts (%u)", ComConf_ComSignal_isTwliBriSts_mrx);
            const TwliBriSts& rteValue = *static_cast<const TwliBriSts*>(buffer);
            autosar::TwliBriSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            TwliBriSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for TwliBriSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isTwliBriSts_mrx, length, static_cast<unsigned long>(sizeof(TwliBriSts)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igUkwnCptReqToInfoMgr_mrx:
    {
        if (sizeof(UkwnCptReqToInfoMgr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received UkwnCptReqToInfoMgr (%u)", ComConf_ComSignalGroup_igUkwnCptReqToInfoMgr_mrx);
            const UkwnCptReqToInfoMgr& rteValue = *static_cast<const UkwnCptReqToInfoMgr*>(buffer);
            autosar::UkwnCptReqToInfoMgr_info::data_elem_type deValue;
            deValue.Byte0 = static_cast<decltype(deValue.Byte0)>(rteValue.UkwnCptReqToInfoMgrByte0);
            deValue.Byte1 = static_cast<decltype(deValue.Byte1)>(rteValue.UkwnCptReqToInfoMgrByte1);
            deValue.Byte2 = static_cast<decltype(deValue.Byte2)>(rteValue.UkwnCptReqToInfoMgrByte2);
            deValue.Byte3 = static_cast<decltype(deValue.Byte3)>(rteValue.UkwnCptReqToInfoMgrByte3);
            deValue.Byte4 = static_cast<decltype(deValue.Byte4)>(rteValue.UkwnCptReqToInfoMgrByte4);
            deValue.Byte5 = static_cast<decltype(deValue.Byte5)>(rteValue.UkwnCptReqToInfoMgrByte5);
            deValue.Byte6 = static_cast<decltype(deValue.Byte6)>(rteValue.UkwnCptReqToInfoMgrByte6);
            deValue.Byte7 = static_cast<decltype(deValue.Byte7)>(rteValue.UkwnCptReqToInfoMgrByte7);

            UkwnCptReqToInfoMgr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for UkwnCptReqToInfoMgr (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igUkwnCptReqToInfoMgr_mrx, length, static_cast<unsigned long>(sizeof(UkwnCptReqToInfoMgr)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igUkwnCptReqToSoundMgr_mrx:
    {
        if (sizeof(UkwnCptReqToSoundMgr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received UkwnCptReqToSoundMgr (%u)", ComConf_ComSignalGroup_igUkwnCptReqToSoundMgr_mrx);
            const UkwnCptReqToSoundMgr& rteValue = *static_cast<const UkwnCptReqToSoundMgr*>(buffer);
            autosar::UkwnCptReqToSoundMgr_info::data_elem_type deValue;
            deValue.Byte0 = static_cast<decltype(deValue.Byte0)>(rteValue.UkwnCptReqToSoundMgrByte0);
            deValue.Byte1 = static_cast<decltype(deValue.Byte1)>(rteValue.UkwnCptReqToSoundMgrByte1);
            deValue.Byte2 = static_cast<decltype(deValue.Byte2)>(rteValue.UkwnCptReqToSoundMgrByte2);
            deValue.Byte3 = static_cast<decltype(deValue.Byte3)>(rteValue.UkwnCptReqToSoundMgrByte3);
            deValue.Byte4 = static_cast<decltype(deValue.Byte4)>(rteValue.UkwnCptReqToSoundMgrByte4);
            deValue.Byte5 = static_cast<decltype(deValue.Byte5)>(rteValue.UkwnCptReqToSoundMgrByte5);
            deValue.Byte6 = static_cast<decltype(deValue.Byte6)>(rteValue.UkwnCptReqToSoundMgrByte6);
            deValue.Byte7 = static_cast<decltype(deValue.Byte7)>(rteValue.UkwnCptReqToSoundMgrByte7);

            UkwnCptReqToSoundMgr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for UkwnCptReqToSoundMgr (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igUkwnCptReqToSoundMgr_mrx, length, static_cast<unsigned long>(sizeof(UkwnCptReqToSoundMgr)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igUsrSwtDispClimaReqForRowSec_mrx:
    {
        if (sizeof(UsrSwtDispClimaReqForRowSec) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received UsrSwtDispClimaReqForRowSec (%u)", ComConf_ComSignalGroup_igUsrSwtDispClimaReqForRowSec_mrx);
            const UsrSwtDispClimaReqForRowSec& rteValue = *static_cast<const UsrSwtDispClimaReqForRowSec*>(buffer);
            autosar::UsrSwtDispClimaReqForRowSec_info::data_elem_type deValue;
            deValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe = static_cast<decltype(deValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe)>(rteValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecLe);
            deValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi = static_cast<decltype(deValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi)>(rteValue.UsrSwtDispClimaReqForSeatHeatLvlForRowSecRi);
            deValue.UsrSwtDispClimaReqForTSpForRowSecLe = static_cast<decltype(deValue.UsrSwtDispClimaReqForTSpForRowSecLe)>( toUnsignedFromRaw<5>(rteValue.UsrSwtDispClimaReqForTSpForRowSecLe) ) * 0.5 + 15.0;
            deValue.UsrSwtDispClimaReqForTSpForRowSecRi = static_cast<decltype(deValue.UsrSwtDispClimaReqForTSpForRowSecRi)>( toUnsignedFromRaw<5>(rteValue.UsrSwtDispClimaReqForTSpForRowSecRi) ) * 0.5 + 15.0;
            deValue.UsrSwtDispClimaReqForTSpSpclForRowSecLe = static_cast<decltype(deValue.UsrSwtDispClimaReqForTSpSpclForRowSecLe)>(rteValue.UsrSwtDispClimaReqForTSpSpclForRowSecLe);
            deValue.UsrSwtDispClimaReqForTSpSpclForRowSecRi = static_cast<decltype(deValue.UsrSwtDispClimaReqForTSpSpclForRowSecRi)>(rteValue.UsrSwtDispClimaReqForTSpSpclForRowSecRi);
            deValue.UsrSwtDispReqForFanLvlForRowSec = static_cast<decltype(deValue.UsrSwtDispReqForFanLvlForRowSec)>(rteValue.UsrSwtDispReqForFanLvlForRowSec);
            deValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe = static_cast<decltype(deValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe)>(rteValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecLe);
            deValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi = static_cast<decltype(deValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi)>(rteValue.UsrSwtDispUpdClimaReqForSeatHeatLvlForRowSecRi);
            deValue.UsrSwtDispUpdClimaReqForTSpForRowSecLe = static_cast<decltype(deValue.UsrSwtDispUpdClimaReqForTSpForRowSecLe)>(rteValue.UsrSwtDispUpdClimaReqForTSpForRowSecLe);
            deValue.UsrSwtDispUpdClimaReqForTSpForRowSecRi = static_cast<decltype(deValue.UsrSwtDispUpdClimaReqForTSpForRowSecRi)>(rteValue.UsrSwtDispUpdClimaReqForTSpForRowSecRi);
            deValue.UsrSwtDispUpdReqForFanLvlForRowSec = static_cast<decltype(deValue.UsrSwtDispUpdReqForFanLvlForRowSec)>(rteValue.UsrSwtDispUpdReqForFanLvlForRowSec);

            UsrSwtDispClimaReqForRowSec_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for UsrSwtDispClimaReqForRowSec (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igUsrSwtDispClimaReqForRowSec_mrx, length, static_cast<unsigned long>(sizeof(UsrSwtDispClimaReqForRowSec)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igUsrSwtDispReqForSecRowSeatVentn_mrx:
    {
        if (sizeof(UsrSwtDispReqForSecRowSeatVentn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received UsrSwtDispReqForSecRowSeatVentn (%u)", ComConf_ComSignalGroup_igUsrSwtDispReqForSecRowSeatVentn_mrx);
            const UsrSwtDispReqForSecRowSeatVentn& rteValue = *static_cast<const UsrSwtDispReqForSecRowSeatVentn*>(buffer);
            autosar::UsrSwtDispReqForSecRowSeatVentn_info::data_elem_type deValue;
            deValue.UsrSwtDispReqForSecRowSeatVentnLe = static_cast<decltype(deValue.UsrSwtDispReqForSecRowSeatVentnLe)>(rteValue.UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnLe);
            deValue.UsrSwtDispReqForSecRowSeatVentnRi = static_cast<decltype(deValue.UsrSwtDispReqForSecRowSeatVentnRi)>(rteValue.UsrSwtDispReqForSecRowSeatVentnUsrSwtDispReqForSecRowSeatVentnRi);
            deValue.usrSwtDispUpdReqForSecRowSeatVentnLe = static_cast<decltype(deValue.usrSwtDispUpdReqForSecRowSeatVentnLe)>(rteValue.UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnLe);
            deValue.usrSwtDispUpdReqForSecRowSeatVentnRi = static_cast<decltype(deValue.usrSwtDispUpdReqForSecRowSeatVentnRi)>(rteValue.UsrSwtDispReqForSecRowSeatVentnusrSwtDispUpdReqForSecRowSeatVentnRi);

            UsrSwtDispReqForSecRowSeatVentn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for UsrSwtDispReqForSecRowSeatVentn (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igUsrSwtDispReqForSecRowSeatVentn_mrx, length, static_cast<unsigned long>(sizeof(UsrSwtDispReqForSecRowSeatVentn)));
        }
    }
    break;

    case ComConf_ComSignal_isSeatHeatLvlReqLe_mrx:
    {
        if (sizeof(SeatHeatLvlReqLe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SeatHeatLvlReqLe (%u)", ComConf_ComSignal_isSeatHeatLvlReqLe_mrx);
            const SeatHeatLvlReqLe& rteValue = *static_cast<const SeatHeatLvlReqLe*>(buffer);
            autosar::SeatHeatLvlReqLe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SeatHeatLvlReqLe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SeatHeatLvlReqLe (%u). Got %zu , expected %lu", ComConf_ComSignal_isSeatHeatLvlReqLe_mrx, length, static_cast<unsigned long>(sizeof(SeatHeatLvlReqLe)));
        }
    }
    break;

    case ComConf_ComSignal_isSeatHeatLvlReqRi_mrx:
    {
        if (sizeof(SeatHeatLvlReqRi) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received SeatHeatLvlReqRi (%u)", ComConf_ComSignal_isSeatHeatLvlReqRi_mrx);
            const SeatHeatLvlReqRi& rteValue = *static_cast<const SeatHeatLvlReqRi*>(buffer);
            autosar::SeatHeatLvlReqRi_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            SeatHeatLvlReqRi_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for SeatHeatLvlReqRi (%u). Got %zu , expected %lu", ComConf_ComSignal_isSeatHeatLvlReqRi_mrx, length, static_cast<unsigned long>(sizeof(SeatHeatLvlReqRi)));
        }
    }
    break;

    case ComConf_ComSignal_isVehActvMsgToDrvr_mrx:
    {
        if (sizeof(VehActvMsgToDrvr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehActvMsgToDrvr (%u)", ComConf_ComSignal_isVehActvMsgToDrvr_mrx);
            const VehActvMsgToDrvr& rteValue = *static_cast<const VehActvMsgToDrvr*>(buffer);
            autosar::VehActvMsgToDrvr_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            VehActvMsgToDrvr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehActvMsgToDrvr (%u). Got %zu , expected %lu", ComConf_ComSignal_isVehActvMsgToDrvr_mrx, length, static_cast<unsigned long>(sizeof(VehActvMsgToDrvr)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehBattUGroup_mrx:
    {
        if (sizeof(VehBattUGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehBattUGroup (%u)", ComConf_ComSignalGroup_igVehBattUGroup_mrx);
            const VehBattUGroup& rteValue = *static_cast<const VehBattUGroup*>(buffer);
            autosar::VehBattU_info::data_elem_type deValue;
            deValue.SysU = static_cast<decltype(deValue.SysU)>( toUnsignedFromRaw<8>(rteValue.VehBattUSysU) ) * 0.1 + 0.0;
            deValue.SysUQf = static_cast<decltype(deValue.SysUQf)>(rteValue.VehBattUSysUQf);

            VehBattU_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehBattUGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVehBattUGroup_mrx, length, static_cast<unsigned long>(sizeof(VehBattUGroup)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehCfgPrm_mrx:
    {
        if (sizeof(VehCfgPrm) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehCfgPrm (%u)", ComConf_ComSignalGroup_igVehCfgPrm_mrx);
            const VehCfgPrm& rteValue = *static_cast<const VehCfgPrm*>(buffer);
            autosar::VehCfgPrm_info::data_elem_type deValue;
            deValue.BlkIDBytePosn1 = static_cast<decltype(deValue.BlkIDBytePosn1)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmBlk) ) * 1.0 + 0.0;
            deValue.CCPBytePosn2 = static_cast<decltype(deValue.CCPBytePosn2)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal1) ) * 1.0 + 0.0;
            deValue.CCPBytePosn3 = static_cast<decltype(deValue.CCPBytePosn3)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal2) ) * 1.0 + 0.0;
            deValue.CCPBytePosn4 = static_cast<decltype(deValue.CCPBytePosn4)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal3) ) * 1.0 + 0.0;
            deValue.CCPBytePosn5 = static_cast<decltype(deValue.CCPBytePosn5)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal4) ) * 1.0 + 0.0;
            deValue.CCPBytePosn6 = static_cast<decltype(deValue.CCPBytePosn6)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal5) ) * 1.0 + 0.0;
            deValue.CCPBytePosn7 = static_cast<decltype(deValue.CCPBytePosn7)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal6) ) * 1.0 + 0.0;
            deValue.CCPBytePosn8 = static_cast<decltype(deValue.CCPBytePosn8)>( toUnsignedFromRaw<8>(rteValue.VehCfgPrmVal7) ) * 1.0 + 0.0;

            VehCfgPrm_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehCfgPrm (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVehCfgPrm_mrx, length, static_cast<unsigned long>(sizeof(VehCfgPrm)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehMGroup_mrx:
    {
        if (sizeof(VehMGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehMGroup (%u)", ComConf_ComSignalGroup_igVehMGroup_mrx);
            const VehMGroup& rteValue = *static_cast<const VehMGroup*>(buffer);
            autosar::VehMNom_info::data_elem_type deValue;
            deValue.TrlrM_ = static_cast<decltype(deValue.TrlrM_)>(rteValue.VehMNomTrlrM);
            deValue.VehM = static_cast<decltype(deValue.VehM)>( toUnsignedFromRaw<14>(rteValue.VehM) ) * 1.0 + 0.0;
            deValue.VehMQly = static_cast<decltype(deValue.VehMQly)>(rteValue.VehMQly);

            VehMNom_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehMGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVehMGroup_mrx, length, static_cast<unsigned long>(sizeof(VehMGroup)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehModMngtGlbSafe1_mrx:
    {
        if (sizeof(VehModMngtGlbSafe1) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehModMngtGlbSafe1 (%u)", ComConf_ComSignalGroup_igVehModMngtGlbSafe1_mrx);
            const VehModMngtGlbSafe1& rteValue = *static_cast<const VehModMngtGlbSafe1*>(buffer);
            autosar::VehModMngtGlbSafe1_info::data_elem_type deValue;
            deValue.CarModSts1_ = static_cast<decltype(deValue.CarModSts1_)>(rteValue.CarModSts1);
            deValue.CarModSubtypWdCarModSubtyp = static_cast<decltype(deValue.CarModSubtypWdCarModSubtyp)>(rteValue.CarModSubtypWdCarModSubtyp);
            deValue.Chks = static_cast<decltype(deValue.Chks)>(rteValue.VehModMngtGlbSafe1Chks);
            deValue.Cntr = static_cast<decltype(deValue.Cntr)>(rteValue.VehModMngtGlbSafe1Cntr);
            deValue.EgyLvlElecMai = static_cast<decltype(deValue.EgyLvlElecMai)>(rteValue.EgyLvlElecMai);
            deValue.EgyLvlElecSubtyp = static_cast<decltype(deValue.EgyLvlElecSubtyp)>(rteValue.EgyLvlElecSubtyp);
            deValue.FltEgyCnsWdSts = static_cast<decltype(deValue.FltEgyCnsWdSts)>(rteValue.FltEgyCnsWdSts);
            deValue.PwrLvlElecMai = static_cast<decltype(deValue.PwrLvlElecMai)>(rteValue.PwrLvlElecMai);
            deValue.PwrLvlElecSubtyp = static_cast<decltype(deValue.PwrLvlElecSubtyp)>(rteValue.PwrLvlElecSubtyp);
            deValue.UsgModSts = static_cast<decltype(deValue.UsgModSts)>(rteValue.UsgModSts);

            VehModMngtGlbSafe1_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehModMngtGlbSafe1 (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVehModMngtGlbSafe1_mrx, length, static_cast<unsigned long>(sizeof(VehModMngtGlbSafe1)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehMtnStSafeGroup_mrx:
    {
        if (sizeof(VehMtnStSafeGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehMtnStSafeGroup (%u)", ComConf_ComSignalGroup_igVehMtnStSafeGroup_mrx);
            const VehMtnStSafeGroup& rteValue = *static_cast<const VehMtnStSafeGroup*>(buffer);
            autosar::VehMtnStSafe_info::data_elem_type deValue;
            deValue.VehMtnSt = static_cast<decltype(deValue.VehMtnSt)>(rteValue.VehMtnStSafe);
            deValue.VehMtnStChks = static_cast<decltype(deValue.VehMtnStChks)>(rteValue.VehMtnStChks);
            deValue.VehMtnStCntr = static_cast<decltype(deValue.VehMtnStCntr)>(rteValue.VehMtnStCntr);

            VehMtnStSafe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehMtnStSafeGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVehMtnStSafeGroup_mrx, length, static_cast<unsigned long>(sizeof(VehMtnStSafeGroup)));
        }
    }
    break;

    case ComConf_ComSignal_isAudWarn_mrx:
    {
        if (sizeof(AudWarn) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received AudWarn (%u)", ComConf_ComSignal_isAudWarn_mrx);
            const AudWarn& rteValue = *static_cast<const AudWarn*>(buffer);
            autosar::AudWarn_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            AudWarn_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for AudWarn (%u). Got %zu , expected %lu", ComConf_ComSignal_isAudWarn_mrx, length, static_cast<unsigned long>(sizeof(AudWarn)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehSpdAvgIndcd_mrx:
    {
        if (sizeof(VehSpdAvgIndcd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehSpdAvgIndcd (%u)", ComConf_ComSignalGroup_igVehSpdAvgIndcd_mrx);
            const VehSpdAvgIndcd& rteValue = *static_cast<const VehSpdAvgIndcd*>(buffer);
            autosar::VehSpdAvgIndcd_info::data_elem_type deValue;
            deValue.VeSpdIndcdUnit = static_cast<decltype(deValue.VeSpdIndcdUnit)>(rteValue.VehSpdAvgUnit);
            deValue.VehSpdIndcd = static_cast<decltype(deValue.VehSpdIndcd)>(rteValue.VehSpdAvg);

            VehSpdAvgIndcd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehSpdAvgIndcd (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVehSpdAvgIndcd_mrx, length, static_cast<unsigned long>(sizeof(VehSpdAvgIndcd)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehSpdIndcd_mrx:
    {
        if (sizeof(VehSpdIndcd) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehSpdIndcd (%u)", ComConf_ComSignalGroup_igVehSpdIndcd_mrx);
            const VehSpdIndcd& rteValue = *static_cast<const VehSpdIndcd*>(buffer);
            autosar::VehSpdIndcd_info::data_elem_type deValue;
            deValue.VeSpdIndcdUnit = static_cast<decltype(deValue.VeSpdIndcdUnit)>(rteValue.VehSpdIndcdUnit);
            deValue.VehSpdIndcd = static_cast<decltype(deValue.VehSpdIndcd)>(rteValue.VehSpdIndcdVal);

            VehSpdIndcd_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehSpdIndcd (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVehSpdIndcd_mrx, length, static_cast<unsigned long>(sizeof(VehSpdIndcd)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVehSpdLgtSafeGroup_mrx:
    {
        if (sizeof(VehSpdLgtSafeGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VehSpdLgtSafeGroup (%u)", ComConf_ComSignalGroup_igVehSpdLgtSafeGroup_mrx);
            const VehSpdLgtSafeGroup& rteValue = *static_cast<const VehSpdLgtSafeGroup*>(buffer);
            autosar::VehSpdLgtSafe_info::data_elem_type deValue;
            deValue.VehSpdLgt = static_cast<decltype(deValue.VehSpdLgt)>( toUnsignedFromRaw<15>(rteValue.VehSpdLgtSafe) ) * 0.00391 + 0.0;
            deValue.VehSpdLgtChks = static_cast<decltype(deValue.VehSpdLgtChks)>(rteValue.VehSpdLgtSafeChks);
            deValue.VehSpdLgtCntr = static_cast<decltype(deValue.VehSpdLgtCntr)>(rteValue.VehSpdLgtSafeCntr);
            deValue.VehSpdLgtQf = static_cast<decltype(deValue.VehSpdLgtQf)>(rteValue.VehSpdLgtSafeQf);

            VehSpdLgtSafe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VehSpdLgtSafeGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVehSpdLgtSafeGroup_mrx, length, static_cast<unsigned long>(sizeof(VehSpdLgtSafeGroup)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igVin_mrx:
    {
        if (sizeof(Vin) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received Vin (%u)", ComConf_ComSignalGroup_igVin_mrx);
            const Vin& rteValue = *static_cast<const Vin*>(buffer);
            autosar::Vin_info::data_elem_type deValue;
            deValue.BlockNr = static_cast<decltype(deValue.BlockNr)>( toUnsignedFromRaw<8>(rteValue.VinBlk) ) * 1.0 + 0.0;
            deValue.VINSignalPos1 = static_cast<decltype(deValue.VINSignalPos1)>( toUnsignedFromRaw<8>(rteValue.VinPosn1) ) * 1.0 + 0.0;
            deValue.VINSignalPos2 = static_cast<decltype(deValue.VINSignalPos2)>( toUnsignedFromRaw<8>(rteValue.VinPosn2) ) * 1.0 + 0.0;
            deValue.VINSignalPos3 = static_cast<decltype(deValue.VINSignalPos3)>( toUnsignedFromRaw<8>(rteValue.VinPosn3) ) * 1.0 + 0.0;
            deValue.VINSignalPos4 = static_cast<decltype(deValue.VINSignalPos4)>( toUnsignedFromRaw<8>(rteValue.VinPosn4) ) * 1.0 + 0.0;
            deValue.VINSignalPos5 = static_cast<decltype(deValue.VINSignalPos5)>( toUnsignedFromRaw<8>(rteValue.VinPosn5) ) * 1.0 + 0.0;
            deValue.VINSignalPos6 = static_cast<decltype(deValue.VINSignalPos6)>( toUnsignedFromRaw<8>(rteValue.VinPosn6) ) * 1.0 + 0.0;
            deValue.VINSignalPos7 = static_cast<decltype(deValue.VINSignalPos7)>( toUnsignedFromRaw<8>(rteValue.VinPosn7) ) * 1.0 + 0.0;

            Vin_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for Vin (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igVin_mrx, length, static_cast<unsigned long>(sizeof(Vin)));
        }
    }
    break;

    case ComConf_ComSignal_isVisnAgWideSts_mrx:
    {
        if (sizeof(VisnAgWideSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VisnAgWideSts (%u)", ComConf_ComSignal_isVisnAgWideSts_mrx);
            const VisnAgWideSts& rteValue = *static_cast<const VisnAgWideSts*>(buffer);
            autosar::VisnAgWideSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            VisnAgWideSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VisnAgWideSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isVisnAgWideSts_mrx, length, static_cast<unsigned long>(sizeof(VisnAgWideSts)));
        }
    }
    break;

    case ComConf_ComSignal_isVisnImgAgWideInUse_mrx:
    {
        if (sizeof(VisnImgAgWideInUse) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received VisnImgAgWideInUse (%u)", ComConf_ComSignal_isVisnImgAgWideInUse_mrx);
            const VisnImgAgWideInUse& rteValue = *static_cast<const VisnImgAgWideInUse*>(buffer);
            autosar::VisnImgAgWideInUse_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            VisnImgAgWideInUse_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for VisnImgAgWideInUse (%u). Got %zu , expected %lu", ComConf_ComSignal_isVisnImgAgWideInUse_mrx, length, static_cast<unsigned long>(sizeof(VisnImgAgWideInUse)));
        }
    }
    break;

    case ComConf_ComSignal_isWhlCircum_mrx:
    {
        if (sizeof(WhlCircum) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WhlCircum (%u)", ComConf_ComSignal_isWhlCircum_mrx);
            const WhlCircum& rteValue = *static_cast<const WhlCircum*>(buffer);
            autosar::WhlCircum_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WhlCircum_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WhlCircum (%u). Got %zu , expected %lu", ComConf_ComSignal_isWhlCircum_mrx, length, static_cast<unsigned long>(sizeof(WhlCircum)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igWhlMotSysTqEstGroup_mrx:
    {
        if (sizeof(WhlMotSysTqEstGroup) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WhlMotSysTqEstGroup (%u)", ComConf_ComSignalGroup_igWhlMotSysTqEstGroup_mrx);
            const WhlMotSysTqEstGroup& rteValue = *static_cast<const WhlMotSysTqEstGroup*>(buffer);
            autosar::WhlMotSysTqEst_info::data_elem_type deValue;
            deValue.SpdLimn = static_cast<decltype(deValue.SpdLimn)>( toUnsignedFromRaw<8>(rteValue.WhlMotSysTqEstSpdLimn) ) * 10.0 + 0.0;
            deValue.TqAct = static_cast<decltype(deValue.TqAct)>( toUnsignedFromRaw<12>(rteValue.WhlMotSysTqEst) ) * 4.0 + -8188.0;
            deValue.TqActChks = static_cast<decltype(deValue.TqActChks)>(rteValue.WhlMotSysTqEstChks);
            deValue.TqActCntr = static_cast<decltype(deValue.TqActCntr)>(rteValue.WhlMotSysTqEstCntr);
            deValue.TqActQlyFac = static_cast<decltype(deValue.TqActQlyFac)>(rteValue.WhlMotSysTqEstQlyFac);

            WhlMotSysTqEst_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WhlMotSysTqEstGroup (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igWhlMotSysTqEstGroup_mrx, length, static_cast<unsigned long>(sizeof(WhlMotSysTqEstGroup)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igWhlRotToothCntr_mrx:
    {
        if (sizeof(WhlRotToothCntr) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WhlRotToothCntr (%u)", ComConf_ComSignalGroup_igWhlRotToothCntr_mrx);
            const WhlRotToothCntr& rteValue = *static_cast<const WhlRotToothCntr*>(buffer);
            autosar::WhlRotToothCntr_info::data_elem_type deValue;
            deValue.WhlRotToothCntrFrntLe = static_cast<decltype(deValue.WhlRotToothCntrFrntLe)>( toUnsignedFromRaw<8>(rteValue.WhlRotToothCntrFrntLe) ) * 1.0 + 0.0;
            deValue.WhlRotToothCntrFrntRi = static_cast<decltype(deValue.WhlRotToothCntrFrntRi)>( toUnsignedFromRaw<8>(rteValue.WhlRotToothCntrFrntRi) ) * 1.0 + 0.0;
            deValue.WhlRotToothCntrReLe = static_cast<decltype(deValue.WhlRotToothCntrReLe)>( toUnsignedFromRaw<8>(rteValue.WhlRotToothCntrReLe) ) * 1.0 + 0.0;
            deValue.WhlRotToothCntrReRi = static_cast<decltype(deValue.WhlRotToothCntrReRi)>( toUnsignedFromRaw<8>(rteValue.WhlRotToothCntrReRi) ) * 1.0 + 0.0;

            WhlRotToothCntr_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WhlRotToothCntr (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igWhlRotToothCntr_mrx, length, static_cast<unsigned long>(sizeof(WhlRotToothCntr)));
        }
    }
    break;

    case ComConf_ComSignal_isWinPosnStsDrv_mrx:
    {
        if (sizeof(WinPosnStsDrv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WinPosnStsDrv (%u)", ComConf_ComSignal_isWinPosnStsDrv_mrx);
            const WinPosnStsDrv& rteValue = *static_cast<const WinPosnStsDrv*>(buffer);
            autosar::WinPosnStsAtDrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WinPosnStsAtDrv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WinPosnStsDrv (%u). Got %zu , expected %lu", ComConf_ComSignal_isWinPosnStsDrv_mrx, length, static_cast<unsigned long>(sizeof(WinPosnStsDrv)));
        }
    }
    break;

    case ComConf_ComSignal_isWinPosnStsAtDrvrRe_mrx:
    {
        if (sizeof(WinPosnStsAtDrvrRe) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WinPosnStsAtDrvrRe (%u)", ComConf_ComSignal_isWinPosnStsAtDrvrRe_mrx);
            const WinPosnStsAtDrvrRe& rteValue = *static_cast<const WinPosnStsAtDrvrRe*>(buffer);
            autosar::WinPosnStsAtDrvrRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WinPosnStsAtDrvrRe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WinPosnStsAtDrvrRe (%u). Got %zu , expected %lu", ComConf_ComSignal_isWinPosnStsAtDrvrRe_mrx, length, static_cast<unsigned long>(sizeof(WinPosnStsAtDrvrRe)));
        }
    }
    break;

    case ComConf_ComSignal_isWinPosnStsPass_mrx:
    {
        if (sizeof(WinPosnStsPass) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WinPosnStsPass (%u)", ComConf_ComSignal_isWinPosnStsPass_mrx);
            const WinPosnStsPass& rteValue = *static_cast<const WinPosnStsPass*>(buffer);
            autosar::WinPosnStsAtPass_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WinPosnStsAtPass_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WinPosnStsPass (%u). Got %zu , expected %lu", ComConf_ComSignal_isWinPosnStsPass_mrx, length, static_cast<unsigned long>(sizeof(WinPosnStsPass)));
        }
    }
    break;

    case ComConf_ComSignal_isWinPosnStsRePass_mrx:
    {
        if (sizeof(WinPosnStsRePass) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WinPosnStsRePass (%u)", ComConf_ComSignal_isWinPosnStsRePass_mrx);
            const WinPosnStsRePass& rteValue = *static_cast<const WinPosnStsRePass*>(buffer);
            autosar::WinPosnStsAtPassRe_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WinPosnStsAtPassRe_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WinPosnStsRePass (%u). Got %zu , expected %lu", ComConf_ComSignal_isWinPosnStsRePass_mrx, length, static_cast<unsigned long>(sizeof(WinPosnStsRePass)));
        }
    }
    break;

    case SignalGroup|ComConf_ComSignalGroup_igWipgInfo_mrx:
    {
        if (sizeof(WipgInfo) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WipgInfo (%u)", ComConf_ComSignalGroup_igWipgInfo_mrx);
            const WipgInfo& rteValue = *static_cast<const WipgInfo*>(buffer);
            autosar::WipgInfo_info::data_elem_type deValue;
            deValue.WipgSpdInfo_ = static_cast<decltype(deValue.WipgSpdInfo_)>(rteValue.WipgInfoWipgSpdInfo);
            deValue.WiprActv = static_cast<decltype(deValue.WiprActv)>(rteValue.WipgInfoWiprActv);
            deValue.WiprInWipgAr = static_cast<decltype(deValue.WiprInWipgAr)>(rteValue.WipgInfoWiprInWipgAr);

            WipgInfo_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WipgInfo (%u). Got %zu, expected %lu", ComConf_ComSignalGroup_igWipgInfo_mrx, length, static_cast<unsigned long>(sizeof(WipgInfo)));
        }
    }
    break;

    case ComConf_ComSignal_isWiprInPosnForSrv_mrx:
    {
        if (sizeof(WiprInPosnForSrv) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WiprInPosnForSrv (%u)", ComConf_ComSignal_isWiprInPosnForSrv_mrx);
            const WiprInPosnForSrv& rteValue = *static_cast<const WiprInPosnForSrv*>(buffer);
            autosar::WiprInPosnForSrv_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WiprInPosnForSrv_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WiprInPosnForSrv (%u). Got %zu , expected %lu", ComConf_ComSignal_isWiprInPosnForSrv_mrx, length, static_cast<unsigned long>(sizeof(WiprInPosnForSrv)));
        }
    }
    break;

    case ComConf_ComSignal_isWshrFldSts_mrx:
    {
        if (sizeof(WshrFldSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received WshrFldSts (%u)", ComConf_ComSignal_isWshrFldSts_mrx);
            const WshrFldSts& rteValue = *static_cast<const WshrFldSts*>(buffer);
            autosar::WshrFldSts1WdElmHMI_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            WshrFldSts1WdElmHMI_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for WshrFldSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isWshrFldSts_mrx, length, static_cast<unsigned long>(sizeof(WshrFldSts)));
        }
    }
    break;

    case ComConf_ComSignal_isiTPMSCalPsbl_mrx:
    {
        if (sizeof(iTPMSCalPsbl) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received iTPMSCalPsbl (%u)", ComConf_ComSignal_isiTPMSCalPsbl_mrx);
            const iTPMSCalPsbl& rteValue = *static_cast<const iTPMSCalPsbl*>(buffer);
            autosar::iTPMSCalPsbl_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            iTPMSCalPsbl_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for iTPMSCalPsbl (%u). Got %zu , expected %lu", ComConf_ComSignal_isiTPMSCalPsbl_mrx, length, static_cast<unsigned long>(sizeof(iTPMSCalPsbl)));
        }
    }
    break;

    case ComConf_ComSignal_isiTPMSCalSts_mrx:
    {
        if (sizeof(iTPMSCalSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received iTPMSCalSts (%u)", ComConf_ComSignal_isiTPMSCalSts_mrx);
            const iTPMSCalSts& rteValue = *static_cast<const iTPMSCalSts*>(buffer);
            autosar::iTPMSCalSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            iTPMSCalSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for iTPMSCalSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isiTPMSCalSts_mrx, length, static_cast<unsigned long>(sizeof(iTPMSCalSts)));
        }
    }
    break;

    case ComConf_ComSignal_isiTPMSTirePMSts_mrx:
    {
        if (sizeof(iTPMSTirePMSts) == length) {
            ALOG(LOG_VERBOSE, "VSMInject", "Received iTPMSTirePMSts (%u)", ComConf_ComSignal_isiTPMSTirePMSts_mrx);
            const iTPMSTirePMSts& rteValue = *static_cast<const iTPMSTirePMSts*>(buffer);
            autosar::iTPMSTirePMSts_info::data_elem_type deValue;
            deValue = static_cast<decltype(deValue)>(rteValue);
            iTPMSTirePMSts_de->inject(deValue);
        } else {
            ALOG(LOG_ERROR, "VSMInject", "Wrong buffer size received for iTPMSTirePMSts (%u). Got %zu , expected %lu", ComConf_ComSignal_isiTPMSTirePMSts_mrx, length, static_cast<unsigned long>(sizeof(iTPMSTirePMSts)));
        }
    }
    break;

    // clang-format on
