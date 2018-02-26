/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#ifndef CLIMATERESET_H
#define CLIMATERESET_H
#include "notifiable_property.h"
#include <Application_dataelement.h>

class ClimateResetLogic final
{
public:
    enum class ClimateResetEvent
    {
        NO_REQ,
        ACTIVATED
    };

    explicit ClimateResetLogic(NotifiableProperty<ClimateResetEvent>&);
    ~ClimateResetLogic(){};

    void request();

private:
    using Mutex = std::recursive_mutex;
    Mutex mutex_;

    NotifiableProperty<ClimateResetEvent>& shareClimateResetEvent;

    // FlexRay signals
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info>          parkingClimate;
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModes;

    autosar::UsgModSts1 usageMode_;
    autosar::CarModSts1 carMode_;

    bool active_;
    bool carConfigOk_;

    bool parkingClimateActive_;

    void handleParkingClimate();
    void handleVehicleMode();
    bool isCarConfigValid() const;
    void activationCheck();
};

#endif
