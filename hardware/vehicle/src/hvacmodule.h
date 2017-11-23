#pragma once

#include <map>
#include <memory>
#include "ModuleBase.h"
#include "android/hardware/automotive/vehicle/2.0/types.h"
#include "fanlevelimpl.h"
#include "i_vehicle_hal_impl.h"
#include "notifiable_property.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;

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
    std::map<vhal20::VehicleProperty, vhal20::VehiclePropValue> m_propValues;
};
