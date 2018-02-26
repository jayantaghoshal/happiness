/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "autosar_printers.h"

namespace autosar
{
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const UsgModSts1& value)
{
    switch (value)
    {
    case UsgModSts1::UsgModAbdnd:
        out << "UsgModAbdnd";
        break;
    case UsgModSts1::UsgModActv:
        out << "UsgModActv";
        break;
    case UsgModSts1::UsgModCnvinc:
        out << "UsgModCnvinc";
        break;
    case UsgModSts1::UsgModDrvg:
        out << "UsgModDrvg";
        break;
    case UsgModSts1::UsgModInActv:
        out << "UsgModInActv";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const CarModSts1& value)
{
    switch (value)
    {
    case CarModSts1::CarModNorm:
        out << "CarModNorm";
        break;
    case CarModSts1::CarModTrnsp:
        out << "CarModTrnsp";
        break;
    case CarModSts1::CarModFcy:
        out << "CarModFcy";
        break;
    case CarModSts1::CarModCrash:
        out << "CarModCrash";
        break;
    case CarModSts1::CarModDyno:
        out << "CarModDyno";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const VehModMngtGlbSafe1&            value)
{
    out << "UsgModSts: " << value.UsgModSts << ", CarModSts1: " << value.CarModSts1_;
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const FctSts2& value)
{
    switch (value)
    {
    case FctSts2::Ukwn:
        out << "Ukwn";
        break;
    case FctSts2::Off:
        out << "Off";
        break;
    case FctSts2::SpdLoStb:
        out << "SpdLoStb";
        break;
    case FctSts2::SpdHiStb:
        out << "SpdHiStb";
        break;
    case FctSts2::NotAvl:
        out << "NotAvl";
        break;
    case FctSts2::SrvRqrd:
        out << "SrvRqrd";
        break;
    case FctSts2::On:
        out << "On";
        break;
    case FctSts2::NotInUse:
        out << "NotInUse";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const GenQf1& value)
{
    switch (value)
    {
    case GenQf1::AccurData:
        out << "AccurData";
        break;
    case GenQf1::DataAccurNotWithinSpcn:
        out << "DataAccurNotWithinSpcn";
        break;
    case GenQf1::TmpUndefdData:
        out << "TmpUndefdData";
        break;
    case GenQf1::UndefindDataAccur:
        out << "UndefindDataAccur";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const VehSpdLgtSafe1& value)
{
    out << "VehSpdLgt: " << value.VehSpdLgt << ", " << value.VehSpdLgtQf;
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const VehMtnSt2& value)
{
    switch (value)
    {
    case VehMtnSt2::Ukwn:
        out << "Ukwn";
        break;
    case VehMtnSt2::StandStillVal1:
        out << "StandStillVal1";
        break;
    case VehMtnSt2::StandStillVal2:
        out << "StandStillVal2";
        break;
    case VehMtnSt2::StandStillVal3:
        out << "StandStillVal3";
        break;
    case VehMtnSt2::RollgFwdVal1:
        out << "RollgFwdVal1";
        break;
    case VehMtnSt2::RollgFwdVal2:
        out << "RollgFwdVal2";
        break;
    case VehMtnSt2::RollgBackwVal1:
        out << "RollgBackwVal1";
        break;
    case VehMtnSt2::RollgBackwVal2:
        out << "RollgBackwVal2";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const VehMtnStSafe1& value)
{
    out << "VehMtnSt: " << value.VehMtnSt;
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const DrvrDesDir1& value)
{
    switch (value)
    {
    case DrvrDesDir1::Undefd:
        out << "Undefd";
        break;
    case DrvrDesDir1::Fwd:
        out << "Fwd";
        break;
    case DrvrDesDir1::Rvs:
        out << "Rvs";
        break;
    case DrvrDesDir1::Neut:
        out << "Neut";
        break;
    case DrvrDesDir1::Resd0:
        out << "Resd0";
        break;
    case DrvrDesDir1::Resd1:
        out << "Resd1";
        break;
    case DrvrDesDir1::Resd2:
        out << "Resd2";
        break;
    case DrvrDesDir1::Resd3:
        out << "Resd3";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const OnOff1& value)
{
    switch (value)
    {
    case OnOff1::Off:
        out << "Off";
        break;
    case OnOff1::On:
        out << "On";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const DoorSts2& value)
{
    switch (value)
    {
    case DoorSts2::Ukwn:
        out << "Ukwn";
        break;
    case DoorSts2::Opend:
        out << "Opend";
        break;
    case DoorSts2::Clsd:
        out << "Clsd";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const MirrFoldStsTyp& value)
{
    switch (value)
    {
    case MirrFoldStsTyp::MirrFoldPosnUndefd:
        out << "MirrFoldPosnUndefd";
        break;
    case MirrFoldStsTyp::MirrNotFoldPosn:
        out << "MirrNotFoldPosn";
        break;
    case MirrFoldStsTyp::MirrFoldPosn:
        out << "MirrFoldPosn";
        break;
    case MirrFoldStsTyp::MirrMovgToNotFold:
        out << "MirrMovgToNotFold";
        break;
    case MirrFoldStsTyp::MirrMovgToFold:
        out << "MirrMovgToFold";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const VisnImgAgWideInUse&            value)
{
    switch (value)
    {
    case VisnImgAgWideInUse::NoImg:
        out << "NoImg";
        break;
    case VisnImgAgWideInUse::Img1:
        out << "Img1";
        break;
    case VisnImgAgWideInUse::Img2:
        out << "Img2";
        break;
    case VisnImgAgWideInUse::Img3:
        out << "Img3";
        break;
    case VisnImgAgWideInUse::Img4:
        out << "Img4";
        break;
    case VisnImgAgWideInUse::Img5:
        out << "Img5";
        break;
    case VisnImgAgWideInUse::Img6:
        out << "Img6";
        break;
    case VisnImgAgWideInUse::Img7:
        out << "Img7";
        break;
    case VisnImgAgWideInUse::Img8:
        out << "Img8";
        break;
    case VisnImgAgWideInUse::Img9:
        out << "Img9";
        break;
    case VisnImgAgWideInUse::Img10:
        out << "Img10";
        break;
    case VisnImgAgWideInUse::Img11:
        out << "Img11";
        break;
    case VisnImgAgWideInUse::Img12:
        out << "Img12";
        break;
    case VisnImgAgWideInUse::Img13:
        out << "Img13";
        break;
    case VisnImgAgWideInUse::Img14:
        out << "Img14";
        break;
    case VisnImgAgWideInUse::Img15:
        out << "Img15";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const CamIndReq& value)
{
    out << "CamFrnt: " << value.CamFrnt << "CamRe: " << value.CamRe << "CamLe: " << value.CamLe
        << "CamRi: " << value.CamRi;
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PreClngNotif& value)
{
    switch (value)
    {
    case PreClngNotif::NoReq:
        out << "NoReq";
        break;
    case PreClngNotif::NoWarn:
        out << "NoWarn";
        break;
    case PreClngNotif::Done:
        out << "Done";
        break;
    case PreClngNotif::Err:
        out << "Err";
        break;
    case PreClngNotif::Intrpt:
        out << "Intrpt";
        break;
    case PreClngNotif::Spare1:
    case PreClngNotif::Spare2:
    case PreClngNotif::Spare3:
        out << "Spare";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PrkgAssiSts3& value)
{
    switch (value)
    {
    case PrkgAssiSts3::SnsrAllOff:
        out << "SnsrAllOff";
        break;
    case PrkgAssiSts3::FourReSnsrActv:
        out << "FourReSnsrActv";
        break;
    case PrkgAssiSts3::FourFrntFourReSnsrAct:
        out << "FourFrntFourReSnsrAct";
        break;
    case PrkgAssiSts3::SixFrntSnsrTwoReActv:
        out << "SixFrntSnsrTwoReActv";
        break;
    case PrkgAssiSts3::Resd7:
        out << "Resd7";
        break;
    case PrkgAssiSts3::SnsrAllStb:
        out << "SnsrAllStb";
        break;
    case PrkgAssiSts3::Resd3:
        out << "Resd3";
        break;
    case PrkgAssiSts3::Resd4:
        out << "Resd4";
        break;
    case PrkgAssiSts3::Resd5:
        out << "Resd5";
        break;
    case PrkgAssiSts3::SnsrFailrAllOff:
        out << "SnsrFailrAllOff";
        break;
    case PrkgAssiSts3::SnsrAllDeactvd:
        out << "SnsrAllDeactvd";
        break;
    case PrkgAssiSts3::SnsrTrlrOff4:
        out << "SnsrTrlrOff4";
        break;
    case PrkgAssiSts3::SnsrTrlrOff12:
        out << "SnsrTrlrOff12";
        break;
    case PrkgAssiSts3::SixFrntSnsrAndSixReActv:
        out << "SixFrntSnsrAndSixReActv";
        break;
    case PrkgAssiSts3::Resd1:
        out << "Resd1";
        break;
    case PrkgAssiSts3::Resd2:
        out << "Resd2";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const SnsrParkAssi1& value)
{
    out << "DstOfSnsrInsdLe: " << value.DstOfSnsrInsdLe << "DstOfSnsrOutdLe: " << value.DstOfSnsrOutdLe
        << "DstOfSnsrLeSide: " << value.DstOfSnsrLeSide << "DstOfSnsrInsdRi: " << value.DstOfSnsrInsdRi
        << "DstOfSnsrOutdRi: " << value.DstOfSnsrOutdRi << "DstOfSnsrRiSide: " << value.DstOfSnsrRiSide
        << "AudWarnDir: " << value.AudWarnDir << "AudSideWarn: " << value.AudSideWarn;

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const DstOfSnsr1& value)
{
    switch (value)
    {
    case DstOfSnsr1::DistanceInfinityOrNotUsed:
        out << "DistanceInfinityOrNotUsed";
        break;
    case DstOfSnsr1::Distance150cm:
        out << "Distance150cm";
        break;
    case DstOfSnsr1::Distance146cm:
        out << "Distance146cm";
        break;
    case DstOfSnsr1::Distance142cm:
        out << "Distance142cm";
        break;
    case DstOfSnsr1::Distance138cm:
        out << "Distance138cm";
        break;
    case DstOfSnsr1::Distance134cm:
        out << "Distance134cm";
        break;
    case DstOfSnsr1::Distance130cm:
        out << "Distance130cm";
        break;
    case DstOfSnsr1::Distance126cm:
        out << "Distance126cm";
        break;
    case DstOfSnsr1::Distance122cm:
        out << "Distance122cm";
        break;
    case DstOfSnsr1::Distance118cm:
        out << "Distance118cm";
        break;
    case DstOfSnsr1::Distance114cm:
        out << "Distance114cm";
        break;
    case DstOfSnsr1::Distance110cm:
        out << "Distance110cm";
        break;
    case DstOfSnsr1::Distance106cm:
        out << "Distance106cm";
        break;
    case DstOfSnsr1::Distance102m:
        out << "Distance102m";
        break;
    case DstOfSnsr1::Distance98cm:
        out << "Distance98cm";
        break;
    case DstOfSnsr1::Distance94cm:
        out << "Distance94cm";
        break;
    case DstOfSnsr1::Distance90cm:
        out << "Distance90cm";
        break;
    case DstOfSnsr1::Distance86cm:
        out << "Distance86cm";
        break;
    case DstOfSnsr1::Distance82cm:
        out << "Distance82cm";
        break;
    case DstOfSnsr1::Distance78cm:
        out << "Distance78cm";
        break;
    case DstOfSnsr1::Distance74cm:
        out << "Distance74cm";
        break;
    case DstOfSnsr1::Distance70cm:
        out << "Distance70cm";
        break;
    case DstOfSnsr1::Distance66cm:
        out << "Distance66cm";
        break;
    case DstOfSnsr1::Distance62cm:
        out << "Distance62cm";
        break;
    case DstOfSnsr1::Distance58cm:
        out << "Distance58cm";
        break;
    case DstOfSnsr1::Distance54cm:
        out << "Distance54cm";
        break;
    case DstOfSnsr1::Distance50cm:
        out << "Distance50cm";
        break;
    case DstOfSnsr1::Distance46cm:
        out << "Distance46cm";
        break;
    case DstOfSnsr1::Distance42cm:
        out << "Distance42cm";
        break;
    case DstOfSnsr1::Distance38cm:
        out << "Distance38cm";
        break;
    case DstOfSnsr1::Distance34cm:
        out << "Distance34cm";
        break;
    case DstOfSnsr1::Distance30cm:
        out << "Distance30cm";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const WarnDir1& value)
{
    switch (value)
    {
    case WarnDir1::NoWarn:
        out << "NoWarn";
        break;
    case WarnDir1::LeftSide:
        out << "LeftSide";
        break;
    case WarnDir1::OuterLeft:
        out << "OuterLeft";
        break;
    case WarnDir1::InnerLeft:
        out << "InnerLeft";
        break;
    case WarnDir1::InnerRight:
        out << "InnerRight";
        break;
    case WarnDir1::OuterRight:
        out << "OuterRight";
        break;
    case WarnDir1::RightSide:
        out << "RightSide";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const ParkAssiLeRi1& value)
{
    switch (value)
    {
    case ParkAssiLeRi1::None:
        out << "None";
        break;
    case ParkAssiLeRi1::Left:
        out << "Left";
        break;
    case ParkAssiLeRi1::Right:
        out << "Right";
        break;
    case ParkAssiLeRi1::LeftAndRight:
        out << "LeftAndRight";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PrkgAssiFailr2& value)
{
    switch (value)
    {
    case PrkgAssiFailr2::NoFailr:
        out << "NoFailr";
        break;
    case PrkgAssiFailr2::SgnlFailr:
        out << "SgnlFailr";
        break;
    case PrkgAssiFailr2::SnsrFailr:
        out << "SnsrFailr";
        break;
    case PrkgAssiFailr2::DgrSnsr:
        out << "DgrSnsr";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const TrlrPrsntSts& value)
{
    switch (value)
    {
    case TrlrPrsntSts::TrlrNotPrsnt:
        out << "TrlrNotPrsnt";
        break;
    case TrlrPrsntSts::TrlrPrsnt:
        out << "TrlrPrsnt";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PrkgTypVld1& value)
{
    switch (value)
    {
    case PrkgTypVld1::NoPrkgDetd:
        out << "NoPrkgDetd";
        break;
    case PrkgTypVld1::ParaPrkgDetd:
        out << "ParaPrkgDetd";
        break;
    case PrkgTypVld1::PerpPrkgDetd:
        out << "PerpPrkgDetd";
        break;
    case PrkgTypVld1::ParaAndPerpPrkgDetd:
        out << "ParaAndPerpPrkgDetd";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const IndOfPrkgAutSts& value)
{
    switch (value)
    {
    case IndOfPrkgAutSts::Off:
        out << "Off";
        break;
    case IndOfPrkgAutSts::PrkgTypAutSeln:
        out << "PrkgTypAutSeln";
        break;
    case IndOfPrkgAutSts::PrkgTypPara:
        out << "PrkgTypPara";
        break;
    case IndOfPrkgAutSts::PrkgTypPerp:
        out << "PrkgTypPerp";
        break;
    case IndOfPrkgAutSts::PrkgOutManv:
        out << "PrkgOutManv";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PrkgAutSts3& value)
{
    switch (value)
    {
    case PrkgAutSts3::SAPNotActive:
        out << "SAPNotActive";
        break;
    case PrkgAutSts3::SAPPassiveScan:
        out << "SAPPassiveScan";
        break;
    case PrkgAutSts3::OverspeedSlowDown:
        out << "OverspeedSlowDown";
        break;
    case PrkgAutSts3::ScanLeft:
        out << "ScanLeft";
        break;
    case PrkgAutSts3::ScanRight:
        out << "ScanRight";
        break;
    case PrkgAutSts3::SAPPSlotFoundLeftGoForward:
        out << "SAPPSlotFoundLeftGoForward";
        break;
    case PrkgAutSts3::SAPPSlotFoundRightGoForward:
        out << "SAPPSlotFoundRightGoForward";
        break;
    case PrkgAutSts3::PPASlotFoundLeftGoForward:
        out << "PPASlotFoundLeftGoForward";
        break;
    case PrkgAutSts3::PPASlotFoundRightGoForward:
        out << "PPASlotFoundRightGoForward";
        break;
    case PrkgAutSts3::SAPStopToParkLeft:
        out << "SAPStopToParkLeft";
        break;
    case PrkgAutSts3::SAPStopToParkRight:
        out << "SAPStopToParkRight";
        break;
    case PrkgAutSts3::PPAStopToParkLeft:
        out << "PPAStopToParkLeft";
        break;
    case PrkgAutSts3::PPAStopToParkRight:
        out << "PPAStopToParkRight";
        break;
    case PrkgAutSts3::SAPPStopAndReverseGearLeft:
        out << "SAPPStopAndReverseGearLeft";
        break;
    case PrkgAutSts3::SAPPStopAndReverseGearRight:
        out << "SAPPStopAndReverseGearRight";
        break;
    case PrkgAutSts3::PPAStopAndReverseGearLeft:
        out << "PPAStopAndReverseGearLeft";
        break;
    case PrkgAutSts3::PPAStopAndReverseGearRight:
        out << "PPAStopAndReverseGearRight";
        break;
    case PrkgAutSts3::SAPPLookAroundLeft:
        out << "SAPPLookAroundLeft";
        break;
    case PrkgAutSts3::SAPPLookAroundRight:
        out << "SAPPLookAroundRight";
        break;
    case PrkgAutSts3::PPALookAroundLeft:
        out << "PPALookAroundLeft";
        break;
    case PrkgAutSts3::PPALookAroundRight:
        out << "PPALookAroundRight";
        break;
    case PrkgAutSts3::SAPPFirstMoveLeft:
        out << "SAPPFirstMoveLeft";
        break;
    case PrkgAutSts3::SAPPFirstMoveRight:
        out << "SAPPFirstMoveRight";
        break;
    case PrkgAutSts3::PPAFirstMoveLeft:
        out << "PPAFirstMoveLeft";
        break;
    case PrkgAutSts3::PPAFirstMoveRight:
        out << "PPAFirstMoveRight";
        break;
    case PrkgAutSts3::SAPPChangeToForwardLeft:
        out << "SAPPChangeToForwardLeft";
        break;
    case PrkgAutSts3::SAPPChangeToForwardRight:
        out << "SAPPChangeToForwardRight";
        break;
    case PrkgAutSts3::PPAChangeToForwardLeft:
        out << "PPAChangeToForwardLeft";
        break;
    case PrkgAutSts3::PPAChangeToForwardRight:
        out << "PPAChangeToForwardRight";
        break;
    case PrkgAutSts3::SAPPMoveForwardLeft:
        out << "SAPPMoveForwardLeft";
        break;
    case PrkgAutSts3::SAPPMoveForwardRight:
        out << "SAPPMoveForwardRight";
        break;
    case PrkgAutSts3::PPAMoveForwardLeft:
        out << "PPAMoveForwardLeft";
        break;
    case PrkgAutSts3::PPAMoveForwardRight:
        out << "PPAMoveForwardRight";
        break;
    case PrkgAutSts3::SAPPChangeToReverseLeft:
        out << "SAPPChangeToReverseLeft";
        break;
    case PrkgAutSts3::SAPPChangeToReverseRight:
        out << "SAPPChangeToReverseRight";
        break;
    case PrkgAutSts3::PPAChangeToReverseLeft:
        out << "PPAChangeToReverseLeft";
        break;
    case PrkgAutSts3::PPAChangeToReverseRight:
        out << "PPAChangeToReverseRight";
        break;
    case PrkgAutSts3::PPAMoveRearwardLeft:
        out << "PPAMoveRearwardLeft";
        break;
    case PrkgAutSts3::PPAMoveRearwardRight:
        out << "PPAMoveRearwardRight";
        break;
    case PrkgAutSts3::Finished:
        out << "Finished";
        break;
    case PrkgAutSts3::ParkingFail:
        out << "ParkingFail";
        break;
    case PrkgAutSts3::TemporaryNotAvailable:
        out << "TemporaryNotAvailable";
        break;
    case PrkgAutSts3::CancelHighSpeed:
        out << "CancelHighSpeed";
        break;
    case PrkgAutSts3::CancelSlipperyRoad:
        out << "CancelSlipperyRoad";
        break;
    case PrkgAutSts3::CancelTrailerConnect:
        out << "CancelTrailerConnect";
        break;
    case PrkgAutSts3::CancelEngineStall:
        out << "CancelEngineStall";
        break;
    case PrkgAutSts3::CancelSteringIntervention:
        out << "CancelSteringIntervention";
        break;
    case PrkgAutSts3::CancelDriverDeactivation:
        out << "CancelDriverDeactivation";
        break;
    case PrkgAutSts3::CancelManyAttempts:
        out << "CancelManyAttempts";
        break;
    case PrkgAutSts3::CancelWrongGear:
        out << "CancelWrongGear";
        break;
    case PrkgAutSts3::Resume:
        out << "Resume";
        break;
    case PrkgAutSts3::SelectParkOutSide:
        out << "SelectParkOutSide";
        break;
    case PrkgAutSts3::POAScannig:
        out << "POAScannig";
        break;
    case PrkgAutSts3::POAPathNotFound:
        out << "POAPathNotFound";
        break;
    case PrkgAutSts3::POAChanageToForwardGearLeft:
        out << "POAChanageToForwardGearLeft";
        break;
    case PrkgAutSts3::POAChanageToForwardGearRight:
        out << "POAChanageToForwardGearRight";
        break;
    case PrkgAutSts3::POAChanageToReverseGearLeft:
        out << "POAChanageToReverseGearLeft";
        break;
    case PrkgAutSts3::POAChanageToReverseGearRight:
        out << "POAChanageToReverseGearRight";
        break;
    case PrkgAutSts3::POAStopAndChangeToForwardGearLeft:
        out << "POAStopAndChangeToForwardGearLeft";
        break;
    case PrkgAutSts3::POAStopAndChangeToForwardGearRight:
        out << "POAStopAndChangeToForwardGearRight";
        break;
    case PrkgAutSts3::POAStopAndChangeToReverseGearLeft:
        out << "POAStopAndChangeToReverseGearLeft";
        break;
    case PrkgAutSts3::POAStopAndChangeToReverseGearRight:
        out << "POAStopAndChangeToReverseGearRight";
        break;
    case PrkgAutSts3::POARearwardMoveLeft:
        out << "POARearwardMoveLeft";
        break;
    case PrkgAutSts3::POARearwardMoveRight:
        out << "POARearwardMoveRight";
        break;
    case PrkgAutSts3::POAForwardMoveLeft:
        out << "POAForwardMoveLeft";
        break;
    case PrkgAutSts3::POAForwardMoveRight:
        out << "POAForwardMoveRight";
        break;
    case PrkgAutSts3::POAFinishedGoLeft:
        out << "POAFinishedGoLeft";
        break;
    case PrkgAutSts3::POAFinishedGoRight:
        out << "POAFinishedGoRight";
        break;
    case PrkgAutSts3::POALookAroundLeft:
        out << "POALookAroundLeft";
        break;
    case PrkgAutSts3::POALookAroundRight:
        out << "POALookAroundRight";
        break;
    case PrkgAutSts3::POAFailr:
        out << "POAFailr";
        break;
    case PrkgAutSts3::EmgyBrkDsbld:
        out << "EmgyBrkDsbld";
        break;
    case PrkgAutSts3::EmgyBrkActv:
        out << "EmgyBrkActv";
        break;
    case PrkgAutSts3::EmgyBrkUsrActn:
        out << "EmgyBrkUsrActn";
        break;
    default:
        out << "NOT MAPPED: " << value;
        break;
    }
    return out;
}

}