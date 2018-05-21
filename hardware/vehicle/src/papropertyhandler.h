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

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

namespace {

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

inline vhal20::VehiclePropertyStatus toVhalStatus(vccvhal10::PAStatus status) {
    switch (status) {
        case vccvhal10::PAStatus::Disabled:
            return vhal20::VehiclePropertyStatus::UNAVAILABLE;
        case vccvhal10::PAStatus::Active:
            return vhal20::VehiclePropertyStatus::AVAILABLE;
        case vccvhal10::PAStatus::SystemError:
            return vhal20::VehiclePropertyStatus::ERROR;
        default:
            return vhal20::VehiclePropertyStatus::ERROR;
    }
}
}  // namespace

namespace PaPropHandlerHelper {
const vccvhal10::VehicleProperty notUsedStatusID{vccvhal10::VehicleProperty::INVALID};

vhal20::VehiclePropConfig BoolConfig(vccvhal10::VehicleProperty property);
}  // namespace PaPropHandlerHelper

template <typename T>
class PAPropHandler {
  public:
    PAPropHandler(vhal20::VehiclePropConfig valueConfig,
                  vccvhal10::VehicleProperty statusID,
                  std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                  vhal20::impl::IVehicleHalImpl* vehicleHal)
        : valueProp_{valueConfig, dispatcher, vehicleHal},
          statusProp_{createStatusConfig(valueConfig, statusID), dispatcher, vehicleHal},
          use_vendor_status_{statusID != PaPropHandlerHelper::notUsedStatusID} {}

    void PushProp(T value, vccvhal10::PAStatus status, int32_t zone = 0) {
        valueProp_.PushProp(value, toVhalStatus(status), zone);

        if (use_vendor_status_) {
            statusProp_.PushProp(static_cast<int32_t>(status), vhal20::VehiclePropertyStatus::AVAILABLE, zone);
        }
    }

    void PushValue(T value, int32_t zone = 0) { valueProp_.PushValue(value, zone); }

    void PushStatus(vccvhal10::PAStatus status, int32_t zone = 0) {
        valueProp_.PushStatus(toVhalStatus(status), zone);
        if (use_vendor_status_) {
            statusProp_.PushProp(static_cast<int32_t>(status), vhal20::VehiclePropertyStatus::AVAILABLE, zone);
        }
    }

    void subscribe_set_prop(std::function<void(T value, int32_t zone)> func) { valueProp_.subscribe_set_prop(func); }

    void registerToVehicleHal() {
        valueProp_.registerToVehicleHal();
        if (use_vendor_status_) {
            statusProp_.registerToVehicleHal();
        }
    }

  private:
    VhalPropertyHandler<T> valueProp_;
    VhalPropertyHandler<int32_t> statusProp_;

    // use_vendor_status_ is used to indicate if separate status of a property shall be used. Oterwise only the
    // VehiclePropertyStatus in the VehiclePropertyValue is used
    const bool use_vendor_status_;
};
