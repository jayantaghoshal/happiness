/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <vhal_v2_0/VehicleUtils.h>
#include <functional>
#include "IDispatcher.h"
#include "i_vehicle_hal_impl.h"
#include "propertyhandler.h"
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

inline vhal20::VehiclePropConfig createStatusConfig(vhal20::VehiclePropConfig valueConfig,
                                                    vccvhal10::VehicleProperty statusID) {
    vhal20::VehiclePropConfig statusConfig;
    statusConfig.prop = vhal20::toInt(statusID);
    statusConfig.access = vhal20::VehiclePropertyAccess::READ;
    statusConfig.changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE;
    statusConfig.areaConfigs.resize(valueConfig.areaConfigs.size());
    for (size_t i = 0; i < valueConfig.areaConfigs.size(); i++) {
        const auto& valueAreaCfg = valueConfig.areaConfigs[i];
        statusConfig.areaConfigs[i].areaId = valueAreaCfg.areaId;
        statusConfig.areaConfigs[i].minInt32Value = 0;  // PASTATUS_NA
        statusConfig.areaConfigs[i].maxInt32Value = 3;  // PASTATUS_ERROR
    }
    return statusConfig;
}

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

template <typename T>
class PAPropHandler {
  public:
    PAPropHandler(vhal20::VehiclePropConfig valueConfig,
                  vccvhal10::VehicleProperty statusID,
                  std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                  vhal20::impl::IVehicleHalImpl* vehicleHal)
        : valueProp_{valueConfig, dispatcher, vehicleHal},
          statusProp_{createStatusConfig(valueConfig, statusID), dispatcher, vehicleHal} {}

    void PushProp(T value, vccvhal10::PAStatus status, int32_t zone = 0) {
        PAPropHandler::setValue(value, zone);
        PAPropHandler::setStatus(status, zone);
    }
    void subscribe_set_prop(std::function<void(T value, int32_t zone)> func) { valueProp_.subscribe_set_prop(func); }
    void registerToVehicleHal() {
        valueProp_.registerToVehicleHal();
        statusProp_.registerToVehicleHal();
    }

  private:
    void setValue(T value, int32_t zone) { valueProp_.PushProp(value, zone); }
    void setStatus(vccvhal10::PAStatus status, int32_t zone) {
        statusProp_.PushProp(static_cast<int32_t>(status), zone);
    }

    VhalPropertyHandler<T> valueProp_;
    VhalPropertyHandler<int32_t> statusProp_;
};
