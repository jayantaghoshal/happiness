/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

enum OnOff
{
    Off = 0,
    On  = 1
};

class IDefroster
{
public:
    virtual ~IDefroster() = default;

    virtual void request(OnOff requestedState) = 0;
};
