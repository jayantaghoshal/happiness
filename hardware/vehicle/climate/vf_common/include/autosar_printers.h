/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "logging.h"

#include <gen_datatypes.h>

namespace autosar
{

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const UsgModSts1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const CarModSts1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const VehModMngtGlbSafe1&            value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const FctSts2& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const VehSpdLgtSafe1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const GenQf1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const VehMtnStSafe1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const VehMtnSt2& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const DrvrDesDir1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const OnOff1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const DoorSts2& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const MirrFoldStsTyp& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const VisnImgAgWideInUse&            value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const CamIndReq& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PreClngNotif& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PrkgAssiSts3& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const SnsrParkAssi1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const DstOfSnsr1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const WarnDir1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const ParkAssiLeRi1& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PrkgAssiFailr2& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const TrlrPrsntSts& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PrkgTypVld1&  value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const IndOfPrkgAutSts& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const PrkgAutSts3& value);


}