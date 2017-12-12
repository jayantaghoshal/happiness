/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <map>
#include <memory>
#include "ModuleBase.h"
#include "android/hardware/automotive/vehicle/2.0/types.h"
#include "fanlevelimpl.h"
#include "i_vehicle_hal_impl.h"
#include "notifiable_property.h"
#include "tempimpl.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;

struct propertyKey {
    propertyKey(vhal20::VehicleProperty vehicleproperty, vhal20::VehicleAreaZone areaId)
        : vehicleproperty{vehicleproperty}, areaId{areaId} {}
    propertyKey(const vhal20::VehiclePropValue& prop)
        : vehicleproperty{static_cast<vhal20::VehicleProperty>(prop.prop)},
          areaId{static_cast<vhal20::VehicleAreaZone>(prop.areaId)} {}
    const vhal20::VehicleProperty vehicleproperty;
    const vhal20::VehicleAreaZone areaId;

    bool operator==(const propertyKey& o) const { return vehicleproperty == o.vehicleproperty && areaId == o.areaId; }

    bool operator<(const propertyKey& o) const {
        return vehicleproperty < o.vehicleproperty || (vehicleproperty == o.vehicleproperty && areaId < o.areaId);
    }
};

class HvacModule : public vhal20::impl::ModuleBase {
  public:
    HvacModule(vhal20::impl::IVehicleHalImpl* vehicleHal);

    int setProp(const vhal20::VehiclePropValue& propValue) override;

    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue) override;

    std::vector<vhal20::VehiclePropValue> getAllPropValues() override;

    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    void initHandledProperties();

    std::unique_ptr<SubscriptionBase> m_fanLevelHandle;
    NotifiableProperty<int32_t> m_fanLevel;
    FanLevelImpl m_fanLevelImpl;
    std::map<propertyKey, vhal20::VehiclePropValue> m_propValues;

    std::unique_ptr<SubscriptionBase> m_temperatureLeftHandle;
    std::unique_ptr<SubscriptionBase> m_temperatureRightHandle;
    TempImpl m_tempImpl;
    NotifiableProperty<float> m_temperature;
};
