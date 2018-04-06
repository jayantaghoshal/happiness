/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */
#pragma once

#include "ModuleBase.h"
#include "adasisFlexrayConverterImpl.h"
#include "i_vehicle_hal_impl.h"
#include "timerMain.h"

/*
 * VmsHal to say to VHAL that we will implement propert and odnt through a stupid error
 */

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;

class vmsHal : public vhal20::impl::ModuleBase {
  public:
    vmsHal(vhal20::impl::IVehicleHalImpl* vehicleHal);

    int setProp(const vhal20::VehiclePropValue& propValue) override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& status) override;

    std::vector<vhal20::VehiclePropConfig> listProperties() override;
    void sendPositionMessage();
    void sendShortHorizonMessage();
    void sendLongHorizonMessage();
    void sendHorizonMetaData();
    void sendStubMessage();
    void sendSegmentMessage();

  private:
    bool _flagLayerNotSet;
    bool timerStarted;
    const std::vector<vhal20::VehiclePropConfig> vehicleMessageService;
    std::unique_ptr<timerGoogleImpl> googleMessageTimer;
    adasisFlexrayConverterImpl* googleAdasisConverter;
};
