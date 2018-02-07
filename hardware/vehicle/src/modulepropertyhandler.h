/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <functional>
#include <map>
#include "ModuleBase.h"
#include "i_vehicle_hal_impl.h"
#include "notifiable_property.h"
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {
namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

/*
 * @brief Helper class for VHAL modules. Inherit this class in any new VHAL module, e.g. hvacmodule.h.
 * Then instantiate one IPropertyHandler for each required property and zone e.g. one PropertyHandlerFloat for
 * HVAC_TEMPERATURE_SET
 * and ROW_1_RIGHT and one for HVAC_TEMPERATURE_SET and ROW_1_LEFT.
 */
class ModulePropertyHandler : public vhal20::impl::ModuleBase {
  public:
    ModulePropertyHandler(vhal20::impl::IVehicleHalImpl* vehicleHal);

    int setProp(const vhal20::VehiclePropValue& propValue) override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& status) override;
    std::vector<vhal20::VehiclePropValue> getAllPropValues() override;
    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  protected:
    class IPropertyHandler {
      public:
        virtual ~IPropertyHandler() = default;
        virtual void set(const vhal20::VehiclePropValue& value) = 0;
        virtual vhal20::VehiclePropValue get() = 0;

      protected:
        IPropertyHandler() = default;
    };

    /*
     * @brief Helper class for VHAL properties of type Float. Instantiate one of these classes for each property and
     * zone.
     *
     * @param set_prop: Function that holds the logic to set the requested property value.
     * @param notifiable_prop_getter: Function to retrive a ReadOnlyNotifiableProperty to the actual property value.
     * @param pushProp: Function to notify upper layers that the property has changed value.
     * @param prop_value: VehiclePropValue to be handled by this Helper
     */
    class PropertyHandlerFloat : public IPropertyHandler {
      public:
        PropertyHandlerFloat(const std::function<int(float)> set_prop,
                             const std::function<ReadOnlyNotifiableProperty<float>*()> notifiable_prop_getter,
                             const std::function<void(vhal20::VehiclePropValue)> pushProp,
                             vhal20::VehiclePropValue prop_value);

        void set(const vhal20::VehiclePropValue& value) override;
        vhal20::VehiclePropValue get() override;

      private:
        const std::function<int(float)> set_prop_;
        const std::function<ReadOnlyNotifiableProperty<float>*()> notifiable_prop_getter_;
        vhal20::VehiclePropValue prop_value_;
        SubscriptionHandle subscription_handle_;
    };

    /*
     * @brief Helper class for VHAL properties of type Int32. Instantiate one of these classes for each property and
     * zone.
     *
     * @param set_prop: Function that holds the logic to set the requested property value.
     * @param notifiable_prop_getter: Function to retrive a ReadOnlyNotifiableProperty to the actual property value.
     * @param pushProp: Function to notify upper layers that the property has changed value.
     * @param prop_value: VehiclePropValue to be handled by this Helper
     */
    class PropertyHandlerInt32 : public IPropertyHandler {
      public:
        PropertyHandlerInt32(const std::function<int(int)> set_prop,
                             const std::function<ReadOnlyNotifiableProperty<int>*()> notifiable_prop_getter,
                             const std::function<void(vhal20::VehiclePropValue)> pushProp,
                             vhal20::VehiclePropValue prop_value);

        void set(const vhal20::VehiclePropValue& value) override;  // Uses the int in VehiclePropValue
        vhal20::VehiclePropValue get() override;                   // Uses the int in VehiclePropValue

      private:
        const std::function<int(int)> set_prop_;
        const std::function<ReadOnlyNotifiableProperty<int>*()> notifiable_prop_getter_;
        vhal20::VehiclePropValue prop_value_;
        SubscriptionHandle subscription_handle_;
    };

    /*
     * @brief One VehicleProperties and its corresponding zones including config.
     */
    struct PropertyAndConfig {
        vhal20::VehiclePropConfig prop_config;
        std::map<vhal20::VehicleAreaZone, std::unique_ptr<IPropertyHandler>> property_per_zone_handlers;
    };

    // All VehicleProperties handled by a VHAL module
    std::map<vccvhal10::VehicleProperty, PropertyAndConfig> propertyhandlers_;
};

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
