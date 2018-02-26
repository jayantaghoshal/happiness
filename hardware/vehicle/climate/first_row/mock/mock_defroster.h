/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "idefroster.h"

#include <gmock/gmock.h>

class MockDefroster : public IDefroster
{
public:
    MOCK_METHOD1(request, void(OnOff));
};
