/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include <functional>
#include <memory>

#include "activesafetymodule.h"
#include "daisettingimpl.h"
#include "i_vehicle_hal_impl.h"

#undef LOG_TAG
#define LOG_TAG "ActiveSafetyModule"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace std::placeholders;

using namespace android;

ActiveSafetyModule::ActiveSafetyModule(
        ::android::hardware::automotive::vehicle::V2_0::impl::IVehicleHalImpl* vehicleHal,
        android::sp<SettingsFramework::SettingsManagerHidl> settings_manager)
    : ModulePropertyHandler(vehicleHal), m_DAISettingImpl(settings_manager) {
    {  // DAI_Setting
        if (isDriveAwayInfoEnabledInCarConfig()) {
            PropertyAndConfig DAI_Setting_property;

            // Config for all zones
            DAI_Setting_property.prop_config.prop = toInt(vccvhal10::VehicleProperty::DAI_SETTING);
            DAI_Setting_property.prop_config.access = VehiclePropertyAccess::READ_WRITE;
            DAI_Setting_property.prop_config.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
            DAI_Setting_property.prop_config.areaConfigs.resize(1);
            DAI_Setting_property.prop_config.areaConfigs[0].areaId = 0;
            DAI_Setting_property.prop_config.areaConfigs[0].minInt32Value = 0;
            DAI_Setting_property.prop_config.areaConfigs[0].maxInt32Value = 2;

            // DAI_Setting
            vhal20::VehiclePropValue prop_value;
            prop_value.prop = DAI_Setting_property.prop_config.prop;
            prop_value.areaId = 0;

            // Put Helper for DAI_Setting in map
            DAI_Setting_property.property_per_zone_handlers[static_cast<VehicleAreaZone>(prop_value.areaId)] =
                    std::make_unique<PropertyHandlerInt32>(
                            [this](int settings_value) {
                                m_DAISettingImpl.setDAISetting(settings_value);
                                return 0;
                            },
                            [this]() { return m_DAISettingImpl.DAISetting(); },
                            [this](vhal20::VehiclePropValue prop_value) { pushProp(prop_value); },
                            prop_value);

            // Property handler configured, put it in map
            propertyhandlers_[static_cast<vccvhal10::VehicleProperty>(prop_value.prop)] =
                    std::move(DAI_Setting_property);
        }
    }
}
